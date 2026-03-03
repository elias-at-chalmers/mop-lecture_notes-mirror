/*
 * 	libmd307.c
 *  Minimal library for C-library support
 *  The library can be built in two separate flavours:
 *  1. CRT and tft uses debugger (dbg307) built in routines, requires dbg307 Version 0.87  or later
 *  2. With preprocessor macro CUSTOM, this build assumes no dbg307 built in routines, everything has to be provided.
 *  The different confugurations behaves somewhat different:
 *  COLDSTART assumes MD307 is started from a cold reset by the debugger.
 *  Other configurations assumes that MD307 has been initialized by software debugger dbg307
 *  and uses the dbg307 builtin functions (see libdbg307). 
 *  Version 2025-06-06
 */

#include	<errno.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include 	<sys/time.h>
#include 	<time.h>
#include 	<ctype.h>

#include 	"md307.h"

#ifndef CUSTOM
#include	"dbg307.h"
#endif

/* Select timer6 as system timer */
#define CRT_TIM	TIM6
#define CRT_TIM_IRQ	TIMER6_IRQ

/* Constants defined by linker */
extern char __heap_low; 
extern char __heap_top; 
extern char __bss_start__; 
extern char __bss_end__; 

struct tms {
clock_t tms_utime;  /* user time */
clock_t tms_stime;  /* system time */
clock_t tms_cutime; /* user time of children */
clock_t tms_cstime; /* system time of children */
};

static uint64_t current_time;	/* Number of ticks, 10 ms period */

#if defined COLDSTART || defined CUSTOM
/* Setup PLL-clock */
int _pll_init_144(void)
{
  /* Sets System clock frequency to 144MHz and configure HCLK, PCLK2 and PCLK1 prescalers. */
  volatile unsigned int StartUpCounter = 0;
  volatile unsigned int HSEStatus = 0;
  
  /* Reset pending iniz... */
  *RCC_CTLR |= (uint32_t)0x00000001;
  *RCC_CFGR0 &= (uint32_t)0xF8FF0000; 
  *RCC_CTLR &= (uint32_t)0xFEF6FFFF; /* Turn off PLL:s ? */
  *RCC_CTLR &= (uint32_t)0xFFFBFFFF;
  *RCC_CFGR0 &= (uint32_t)0xFF80FFFF;
  *RCC_CTLR &= (uint32_t)0xEBFFFFFF;
  *RCC_INTR = 0x00FF0000; /* Clear all interrupt flags */
  *RCC_CFGR2 = 0x00000000;
	
  *RCC_CTLR |= ((uint32_t)RCC_HSEON);
  /* Wait till HSE is ready or time out. */
  do {
    HSEStatus = *RCC_CTLR & RCC_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ( (*RCC_CTLR & RCC_HSERDY) == 0 )
  {
     /* If HSE fails to start-up, 
	  * the application will have a bad clock configuration. 
	  * Notify caller */
		return 1;
  }
    
  *RCC_CFGR0 |= (uint32_t)RCC_HPRE_DIV1;  /* HCLK = SYSCLK */
  *RCC_CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;  /* PCLK2 = HCLK */
  *RCC_CFGR2 |= (uint32_t)RCC_PPRE1_DIV2;  /* PCLK1 = HCLK/2 */
  /*  PLL configuration: PLLCLK = HSE * 18 = 144 MHz */
  *RCC_CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));
  *RCC_CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL18_EXTEN); // D8C
  *RCC_CTLR |= RCC_PLLON;  /* Enable PLL */
  while((*RCC_CTLR & RCC_PLLRDY) == 0) {}  /* Wait till PLL is ready */
  *RCC_CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW)); /* Select PLL as system clock source */
  *RCC_CFGR0 |= 2; // PLL used as system clock (uint32_t)RCC_SW_PLL;
  while ((*RCC_CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08){} /* Wait till PLL is used as system clock source */
  return 0;
}
#endif

__attribute__((interrupt )) void crt_timer_interrupt( void )
{  /* 1 ms period */
	CRT_TIM->intfr &= ~TIM_UIF;
	current_time++;
}

volatile void crt_timer_init( )
{
	/* System core clock: 144MHz */
	RCC->apb1cenr	|= 0x10; 	/* Enable TIMER 6 */	
	RCC->apb1prstr	|= 0x10; 	/* Reset TIMER 6 */	
	RCC->apb1prstr	&= ~0x10; 	
	CRT_TIM->ctlr1 &= ~TIM_CEN;	 	 
	*IRQVEC(CRT_TIM_IRQ)= crt_timer_interrupt; 				/* set interrupt routine */
	*PFIC_IENR(CRT_TIM_IRQ) |= PFIC_IRQ_BPOS( CRT_TIM_IRQ );	/* enable interrupt */
	 
	 /* 10 ms */
#if defined SIMULATOR 
	CRT_TIM->psc= 1; 
	CRT_TIM->atrlr = 999;
#else
	CRT_TIM->psc= 143; 
	CRT_TIM->atrlr = 9999;
#endif
	CRT_TIM->dmaintenr |= TIM_UIE;
	CRT_TIM->ctlr1 |= TIM_CEN;
}


volatile void crt_timer_deinit( )
{
	 CRT_TIM->ctlr1 &= ~TIM_CEN;
}


 
clock_t _times(struct tms *buf) {
    buf->tms_utime = buf->tms_stime = current_time;
    return current_time;
}


int _gettimeofday( struct timeval *tv, void *tzvp )
{
    uint64_t t = current_time;  /* get uptime in milliseconds */
    tv->tv_sec = t / 100;  		/* convert to seconds */
    tv->tv_usec = ( t % 100 );  /* get remaining microseconds */
    return 0;  					/* return non-zero for error */
} 

int _settimeofday( struct timeval *tv, void *tzvp )
{
    current_time = 100 * tv->tv_sec ;  
	current_time += tv->tv_usec;
    return 0;  					/* return non-zero for error */
} 



#ifdef CUSTOM
unsigned char  _tstchar( void )
{
	if(USART1->statr & 0x20)
		return (char) USART1->datar;
	return 0;
}
void _outchar( unsigned char ch )
{
	USART1->datar = (unsigned long) ch;
 	while (( USART1->statr & 0x80)==0);
}

# else 
/* Stubs for all dbg307 builtin functions */
unsigned char _tstchar( void )
{
	return dbg_tstchar();
}

void _outchar( unsigned char ch )
{
	dbg_outchar(ch);
}

void delay_ms( unsigned int ms ){ 
	dbg_delay_ms(ms);
}

#endif


/* *********************** */

static char *heap_end;

char * _sbrk(int incr) {
	char *prev_heap_end;
 	if (heap_end == 0) {
		heap_end = &__heap_low;
	}
	prev_heap_end = heap_end;
	if (heap_end + incr > &__heap_top) {		
		errno = ENOMEM;
		return (char *)-1;
	}
	heap_end += incr;
	return (char *) prev_heap_end;	
}

__attribute__ ( (used) )		/* Might be optimised away otherwise... */
volatile void _crt_init( void) {
	char *s, *e;
	
#if defined COLDSTART	
	/* Setup the system clock */
	extern int _pll_init_144();
	while( _pll_init_144() ){}
	/* Initialise console USART*/
	RCC->apb2cenr |= (1<<14); 		/* Enable USART 1/SPI1  clock */
	RCC->apb2cenr |= (1<<2); 		/* Enable GPIO A clock */
	RCC->apb2cenr |= (1<<0); 		/* Enable AF IO clock  */
	RCC->apb2prstr	|= (1<<14); 	/* Reset USART1 */	
	RCC->apb2prstr	&= ~(1<<14); 	
	*GPIO_CFGHR(GPIO_A) |= (0xB0);			/* PA9 AF USART1 TX */
	*GPIO_CFGHR(GPIO_A) |= (0x400);		/* PA10 AF USART1 RX floating*/
	USART1->brr = 0x4E2;	/* Set UART baudrate (115 200, 144 MHz) */
	USART1->ctlr1 = 0x200C;	/* Enable USART, TE/RX */
# endif	
	/* Setup memory heap */
	heap_end = 0;
	s = &__bss_start__;
	e = &__bss_end__;
	while( s < e )
		*s++ = 0;
		
 	/* Turn off stdio buffering */
	setvbuf( stdin, NULL, _IONBF, 0 );
	setvbuf( stdout, NULL, _IONBF, 0 );
	setvbuf( stderr, NULL, _IONBF, 0 );
	
	crt_timer_init();
}

__attribute__ ( (used) )		
volatile void _crt_deinit( void ) {
	crt_timer_deinit( );
}

void _exit( int a )
{
	if( a )
		printf("\nProgram exited with code %d\n",a);
	_crt_deinit( );
	__asm__ volatile(" j __crt_reentry$\n");				
}

int _close(int file) {
      return -1;
}

int _open(const char *name, int flags, int mode)
{ 
      return -1;
}

int _fstat(int file, struct stat *st)
{ 
	st->st_mode = S_IFCHR; 
	return 0; 
}

int _lseek(int file, int ptr, int dir)
{ 
   return 0;
}

int _isatty(int file)
{
   return 0;
}


int _write(int file, char *ptr, int len)
{
    int rval = len;
 	while (len--)
      _outchar(*ptr++);
	return rval;
}

int _read(int file, char *ptr, int len)
{
    char c;
	int items = 0;
    while( 1 )
    {
		c = _tstchar();
		if( c != 0)
        {
            *ptr++ = c;
             items++;
        }
		if( len == items)
			return items;
    }
}

pid_t _getpid(void)
{
	return 0;
}

int  _kill(pid_t pid, int sig)
{
	return 0;
	
}

/* *********************** */
