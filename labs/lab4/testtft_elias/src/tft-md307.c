/*
 * tft-md307.c
 * tft library for md307
 * Build target selection — set TARGET to one of:
 *   TARGET_SIM: Simulator  - TFT calls go via ecall trap
 *   TARGET_HW:  Hardware   - TFT calls go direct (OpenOCD)
 *   TARGET_DBG: Debugger   - TFT calls forwarded to DBG307
 *
 * TARGET is defined by the build system via -DTARGET=<value>
 */
#define TARGET_SIM  0
#define TARGET_HW   1
#define TARGET_DBG  2

#if !defined(TARGET)
#  error "TARGET must be defined to TARGET_SIM, TARGET_HW, or TARGET_DBG"
#endif

#if TARGET == TARGET_DBG
/* dbg307 builtin functions */
#include <dbg307.h>
int tft_init(int option){
	return dbg_tft_init(option);
}
void tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill){
	dbg_tft_lcd_ellipse(xc,  yc,  rx,  ry, colour, fill);
}
void tft_lcd_rect(int xc, int yc, int xr, int yr, int colour, int fill){
	dbg_tft_lcd_rect( xc,  yc,  xr,  yr,  colour, fill);				
}			
void tft_lcd_line(int  x1, int y1, int x2, int y2, int colour){
	dbg_tft_lcd_line( x1, y1, x2, y2, colour);				
}	
void tft_lcd_crosshair(int x,int y,int colour){
	dbg_tft_lcd_crosshair( x, y, colour);				
}
void tft_lcd_pixel(int x,int y, int colour){
	dbg_tft_lcd_pixel( x, y, colour);				
}	
	
void delay_ms(unsigned int ms){
	dbg_delay_ms(ms);				
}
int tft_tp_scan(int tp){
	return (dbg_tft_tp_scan(tp));				
}	
int tft_tp_getpos( int *x, int *y ){
	return (dbg_tft_tp_getpos( x, y ));				
}	

#else /* TARGET_HW or TARGET_SIM */
#include "md307.h"
#include "tftmd307.h"
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define CMD_RDX 0XD0
#define CMD_RDY 0X90

static _lcd_dev lcddev;
static _m_tp_dev tp_dev;
static	int _option;					

static uint8_t 	_tp_read_xy2(uint16_t *x,uint16_t *y);
static uint16_t _tp_read_xoy(uint8_t xy);
static uint8_t 	_tp_read_xy(uint16_t *x,uint16_t *y);
static void 	_tp_write_byte(uint8_t num);
static uint16_t _tp_read_ad(uint8_t CMD);

static void _lcd_write_16bit_data(uint16_t Data);
static void _tft_lcd_setwindow(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
static void _tft_lcd_direction(int direction);
static void _tft_lcd_fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
static void _tft_lcd_sprite(int x, int y, uint16_t *data, int w, int h);



#if TARGET == TARGET_SIM
/* Dispatch system call */
 __attribute__((interrupt("machine"))) void ecall()
{
	/* Since we have no idea about register usage in the called routines, we simply has to save/restore them all.
	 * Also, this function has to end with an 'mret'.
	 * The attribute will do all of this for us. */
	volatile unsigned int * frame;
	volatile PTFT_CALL tft_frame;
	__asm__ volatile (" mv %0,a7 \t"	: "=r" (tft_frame) );  /* a7 into 'caller_frame' */	
	frame = tft_frame->caller_frame;	
	switch( tft_frame->function )
	{
		case 0: frame[0] = _tft_init(frame[0]);
				break;
		case 1: _tft_lcd_ellipse(frame[0], frame[1], frame[2], frame[3], frame[4], frame[5]); 
				break;				
		case 2: _tft_lcd_rect(frame[0], frame[1], frame[2], frame[3], frame[4], frame[5]); 
				break;
		case 3: _tft_lcd_line(frame[0], frame[1], frame[2], frame[3], frame[4]); 
				break;				
		case 4: _tft_lcd_crosshair(frame[0], frame[1], frame[2]); 
				break;
		case 5: _tft_lcd_pixel(frame[0], frame[1], frame[2]); 
				break;
				
		case 50: _delay_ms(frame[0]); 
				break;
				
		case 70: frame[0] = _tft_tp_scan(frame[0]); 
				break;
		case 71: frame[0] = _tft_tp_getpos((int *) frame[0], (int *) frame[1]); 
				break;

	}
	/* Update PC otherwise the trap will be taken forever... */
	__asm__ volatile(" csrr t0,mepc\n"); /* Copy mepc... */		
	__asm__ volatile(" addi t0,t0,4\n");		
	__asm__ volatile(" csrw mepc,t0\n");
}

int tft_init(int option)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[1];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 0;
	caller_frame[0]=option;
	frame.caller_frame = caller_frame;
	*((void (**)(void) ) (0x2001C014) ) = ecall;	/* Ecall exception vector (m-mode) */
	__asm__ volatile(" ecall\n");
	return caller_frame[0];
}

void tft_lcd_ellipse(int xc, int yc, int xw, int yw, int colour, int fill)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	// was "=r" before, but that causes the compiler to use a0 for the frame pointer
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 1;
	caller_frame[0]=xc;
	caller_frame[1]=yc;
	caller_frame[2]=xw;
	caller_frame[3]=yw;
	caller_frame[4]=colour;
	caller_frame[5]=fill;
	frame.caller_frame = caller_frame;	
	__asm__ volatile(" ecall\n");
}

void tft_lcd_rect(int x1, int y1, int x2, int y2, int colour, int fill)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 2;
	caller_frame[0]=x1;
	caller_frame[1]=y1;
	caller_frame[2]=x2;
	caller_frame[3]=y2;
	caller_frame[4]=colour;
	caller_frame[5]=fill;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}

void tft_lcd_line(int x1, int y1, int x2, int y2, int colour)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 3;
	caller_frame[0]=x1;
	caller_frame[1]=y1;
	caller_frame[2]=x2;
	caller_frame[3]=y2;
	caller_frame[4]=colour;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}


void tft_crosshair(int x,int y,int colour)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 4;
	caller_frame[0]=x;
	caller_frame[1]=y;
	caller_frame[2]=colour;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}


void tft_lcd_pixel(int x,int y,int colour)
{
#if 0 
	// setpixel causes an illegal instruction for some reason.
      // Using setline instead. Desperate times...
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 5;
	caller_frame[0]=x;
	caller_frame[1]=y;
	caller_frame[2]=colour;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
#else
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 3;
	caller_frame[0]=x;
	caller_frame[1]=y;
	caller_frame[2]=x;
	caller_frame[3]=y+1;
	caller_frame[4]=colour;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");	
#endif
}




int tft_tp_scan(int param)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[1];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 70;
	caller_frame[0]=param;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
	return caller_frame[0];
}

int tft_tp_getpos(int *x, int *y)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[2];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 71;
	caller_frame[0]= (int) x;
	caller_frame[1]= (int) y;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
	return caller_frame[0];
}

void delay_ms(unsigned int ms)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[1];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t" : : "r" (frameptr) );  /* 'frame' into a7 */
	frame.function = 50;
	caller_frame[0]=ms;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}

#elif TARGET == TARGET_HW
/* Direct calls */
int 	_tft_tp_scan(int tp);
int 	_tft_tp_getpos( int *x, int *y );
int		_tft_init(int);
void 	_tft_lcd_pixel(int x,int y, int colour);
void 	_tft_lcd_crosshair(int x,int y,int colour);
void 	_tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill);
void 	_tft_lcd_rect(int x1, int y1, int x2, int y2, uint16_t colour, int fill);
void 	_tft_lcd_line(int  x1, int y1, int x2, int y2, int colour);
/* direct calls */
void	_delay_ms(unsigned int ms);
void 	delay_us(unsigned int  us);


int tft_init(int option){
	return _tft_init(option);
}
void tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill){
	_tft_lcd_ellipse(xc,  yc,  rx,  ry, colour, fill);
}
void tft_lcd_rect(int xc, int yc, int xr, int yr, int colour, int fill){
	_tft_lcd_rect( xc,  yc,  xr,  yr,  colour, fill);				
}			


void tft_lcd_sprite(int x, int y, uint16_t *data, int w, int h){
	_tft_lcd_sprite(x, y, data, w, h);
}


void tft_lcd_line(int  x1, int y1, int x2, int y2, int colour){
	_tft_lcd_line( x1, y1, x2, y2, colour);				
}	
void tft_lcd_crosshair(int x,int y,int colour){
	_tft_lcd_crosshair( x, y, colour);				
}
void tft_lcd_pixel(int x,int y, int colour){
	_tft_lcd_pixel( x, y, colour);				
}	
	
int tft_tp_scan(int tp){
	return (_tft_tp_scan(tp));				
}	
int tft_tp_getpos( int *x, int *y ){
	return (_tft_tp_getpos( x, y ));				
}

void delay_ms(unsigned int ms){
	_delay_ms(ms);	
}

#else
#  error "Unknown TARGET value"
#endif

/* Setup PLL-clock */
static int _pll_init_144(void)
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
/* Delay functions using TIMER7 */
void timer7_delay1ms(void )
{
	/* System core clock: 144MHz */
	 /* freq: PSC is # of microsecs-1 */
	 unsigned int i;
	*TIM7_CTLR1 = 0;
	*TIM7_CTLR2 = 0;
	*TIM7_DMAINTENR = 0;
	*TIM7_INTFR = 0;
	*TIM7_PSC= 143; /* 1 us */
	*TIM7_ATRLR = 999; /* times 1000  -> 1 ms */
	*TIM7_CTLR1 = TIM_CEN;
	while( (*TIM7_INTFR & 1 )==0) {}
	*TIM7_CTLR1 = 0;
}
void _delay_ms( unsigned int ms )
{
	while( ms-- )
		timer7_delay1ms();
}

static void timer7_delay1us(void )
{
	/* System core clock: 144MHz */
	 /* freq: PSC is # of microsecs-1 */
	*TIM7_CTLR1 = 0;
	*TIM7_INTFR = 0;
	*TIM7_PSC= 0; /* 1 us */
	*TIM7_ATRLR = 143; /* 1 us */
	*TIM7_CTLR1 = TIM_CEN ;
	while( (*TIM7_INTFR & 1 )==0) {}
	*TIM7_CTLR1 = 0;
}
void delay_us( unsigned int us )
/* Internal use and global when compile/link 
 * not included as library calls */
{
	while( us-- )
		timer7_delay1us();
}

static void timer7_delay250ns(void )
{
	/* System core clock: 144MHz */
	 /* freq: PSC is # of microsecs-1 */
	 int i;
	*TIM7_CTLR1 = 0;
	*TIM7_INTFR = 0;
	*TIM7_PSC= 0; /* 1 us */
	*TIM7_ATRLR = 30; /* approx 200 ns */
	*TIM7_CTLR1 = TIM_CEN ;
	while( (*TIM7_INTFR & 1 )==0) {}
	*TIM7_CTLR1 = 0;
}
void delay_250ns( void )
/* Internal use and global when compile/link 
 * not included as library calls */
{
	timer7_delay250ns();
}


#if 0
void	_delay_ms(unsigned int ms)
{
	unsigned long long count = 144000*(unsigned long long) ms;
	*STK_CTLR = 0;
	*STK_SR = 0;	
	*STK_CMP = count;	
	*STK_CTLR = STK_B_CLK | STK_B_INIT | STK_B_EN ;  
	while( (*STK_SR & 1 )==0);
	*STK_CTLR = 0; 
}

void delay_us(unsigned int us)
{
	unsigned long long count = 144*(unsigned long long) us;
	*STK_CTLR = 0;
	*STK_SR = 0;	
	*STK_CMPHR = 0;
	*STK_CMPLR = count;	
	*STK_CTLR = STK_B_INIT|STK_B_CLK|STK_B_CM| STK_B_EN ;  
	while( (*STK_SR & 1 )==0);
	*STK_CTLR = 0; 
}
#endif

/* Initialize the tft display interface */ 
static void tft_display_init(void)	
{	
	/* Setup GPIO pins for the wiring interface (LCD and SPI) */
 	/* Enable clocks */
	*RCC_APB2PCENR |= ((SPI1EN)|(IOPBEN)|(IOPAEN)|(AFIOEN));	
	*RCC_APB2PRSTR |= ((SPI1EN)|(IOPBEN)|(IOPAEN)|(AFIOEN));	/* Assert RESET */
	*RCC_APB2PRSTR &= ~((SPI1EN)|(IOPBEN)|(IOPAEN)|(AFIOEN));	/* Release RESET */

	/* Set up GPIO pins in port B for LED control signals */	
	*GPIO_CFGHR(GPIO_B) &= 0x0000FFFF;	/* Setup PB12-15  as output, push/pull */
	*GPIO_CFGHR(GPIO_B) |= 0x33330000;	

 /* Set up GPIO pins in port A for SPI
		PA5 = SCK
		PA6 = MISO
		PA7 = MOSI 
 */
	*GPIO_CFGLR(GPIO_A) &= 0x000FFFFF;	
	*GPIO_CFGLR(GPIO_A) |= 0xB4B00000;	/* PA5,7 is AF PUSH PULL */
	*GPIO_OUTDR(GPIO_A) |= (1<<6);			/* PA6 is Pullup */

/* Calm hardware */
	LCD_CS_SET;
	LCD_RST_SET;
	
/* Init and enable SPI1 */
#define MSTR	(1<<2)
#define SSM		(1<<9)
#define SSI		(1<<8)
#define SPIENABLE		(1<<6)
#define FPCLK_DIV_1	(0<<3)
#define FPCLK_DIV_4	(1<<3)
#define FPCLK_DIV_8	(2<<3)
#define FPCLK_DIV_16	(3<<3)
#define FPCLK_DIV_32	(4<<3)
#define FPCLK_DIV_64	(5<<3)
#define FPCLK_DIV_128	(6<<3)
#define FPCLK_DIV_256	(7<<3)

	SPI1->cr1 = 0;
	SPI1->cr2 = 0;

	SPI1->cr1 =  MSTR|SSI|SSM|FPCLK_DIV_4;

	SPI1->i2scfgr = 0; 
	SPI1->crcpr = 7;
	
	SPI1->cr1 |=  SPIENABLE;
		
	lcddev.width = 0;			
	lcddev.height= 0;				
	lcddev.id= 0;					
	lcddev.dir= 0;					
	lcddev.wramcmd= 0;		
	lcddev.setxcmd= 0;			
	lcddev.setycmd= 0;			
}


static uint8_t SPI_WriteByte(PSPI_WCH_DEVICE SPIx, uint8_t byte)
{
	uint16_t s=0;;
	while(1)
	{
		s = SPIx->sr;  	/* Wait for TXE */
		if( s & 2 )break;
	}
	SPIx->dr= (uint8_t) byte;	 	/* Send byte */
	while(1)
	{
		s = SPIx->sr;  	/* Wait for RXE */
		if( s & 1)break;
	}
	return (uint8_t) SPIx->dr;
} 

/* Write-only variant: waits for TXE then loads next byte immediately.
 * Drains RXNE non-blocking to prevent overflow flag.
 * Call SPI_WaitDone() after the last byte before raising CS. */
static void SPI_WriteByteOnly(PSPI_WCH_DEVICE SPIx, uint8_t byte)
{
	while(!(SPIx->sr & 2));         /* Wait for TXE */
	SPIx->dr = byte;
	if(SPIx->sr & 1) (void)SPIx->dr; /* Drain RX if ready (non-blocking) */
}

/* Wait for the shift register to finish and drain the final RX byte.
 * Must be called after the last SPI_WriteByteOnly before raising CS. */
static void SPI_WaitDone(PSPI_WCH_DEVICE SPIx)
{
	while(SPIx->sr & (1<<7));       /* Wait for BSY = 0 */
	if(SPIx->sr & 1) (void)SPIx->dr; /* Drain final RX byte */
}


/* Software implementation of SPI-protocol */ 	 			    					   
static void _tp_write_byte(uint8_t num)    
{  	
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80) TDIN_SET;  
		else TDIN_CLR;   
		num<<=1;    
		TCLK_CLR;
		delay_us(1);
		TCLK_SET;	        
	}		 			    
}

/* Read adc values from touch screen IC (SPI bus)
	Read command,0xD0 for x,	0x90 for y */
static uint16_t _tp_read_ad(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	uint16_t Num=0; 
	uint16_t test=0; 
	TCLK_CLR;
	TDIN_CLR;
	TCS_CLR;
	_tp_write_byte(CMD);
	delay_us(6);
	TCLK_CLR;
	delay_us(1);    	   
	TCLK_SET;
	delay_us(1);    
	TCLK_CLR;
	for(count=0;count<16;count++) 
	{ 				  
		Num<<=1; 	 
		TCLK_CLR;
		delay_us(1);    
		TCLK_SET;
		test = *GPIO_INDR(GPIO_C);
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;  
	TCS_SET;
	return(Num);  
}

#define READ_TIMES 5 
#define LOST_VAL 1

/* Read the touch screen coordinates (x or y),
	Read the READ_TIMES secondary data in succession 
	and sort the data in ascending order,
	Then remove the lowest and highest number of LOST_VAL 
	and take the average
   parameter:xy:Read command(CMD_RDX/CMD_RDY) */  
static uint16_t _tp_read_xoy(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=_tp_read_ad(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//ÅÅÐò
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//ÉýÐòÅÅÁÐ
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

/*
 Read touch screen x and y coordinates,
The minimum value can not be less than 100
parameters 	x:Read x coordinate of the touch screen
			y:Read y coordinate of the touch screen
retvalue   :0-fail,1-success
*/ 
static  uint8_t _tp_read_xy(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=_tp_read_xoy(CMD_RDX);
	ytemp=_tp_read_xoy(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//¶ÁÊýÊ§°Ü
	*x=xtemp;
	*y=ytemp;
	return 1;
}

#define ERR_RANGE 50 

/* Read the touch screen coordinates twice in a row, 
	and the deviation of these two times can not exceed ERR_RANGE, 
	satisfy the condition, then think the reading is correct, 
	otherwise the reading is wrong.
	This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
*/ 
static uint8_t _tp_read_xy2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=_tp_read_xy(&x1,&y1);   
    if(flag==0)return(0);
    flag=_tp_read_xy(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//Ç°ºóÁ½´Î²ÉÑùÔÚ+-50ÄÚ
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 


int _tft_tp_getpos( int *x, int *y )
{
	if(tp_dev.sta&TP_PRES_DOWN){
		*x = tp_dev.x;
		*y = tp_dev.y;
		return 1;
	}
	return 0;
}
				  
/* Scan touch event				
   param tp: 	0-screen coordinate 
				1-Physical coordinates(For special occasions such as calibration)
	return value:Current touch screen status,
				0-no touch
				1-touch
*/  	
int _tft_tp_scan(int tp)
{		
	int32_t xx,yy;	
	if(PEN==0) /* Touch is active */
	{
		if(tp)_tp_read_xy2(&tp_dev.x,&tp_dev.y);
		else if(_tp_read_xy2(&tp_dev.x,&tp_dev.y))
		{
			/* Is there a mixup between x and y when reading the ADC ??? */
			xx = 320-(( (uint32_t) tp_dev.x*320)/4092) ;
			yy = 480-(( (uint32_t) tp_dev.y*480)/4092) ;
	 		tp_dev.x=(uint16_t) yy;
			tp_dev.y=(uint16_t) xx;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;
			tp_dev.x0=tp_dev.x;
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{	/* Touch has been released */
		if(tp_dev.sta&TP_PRES_DOWN)
		{
			tp_dev.sta&=~(1<<7);	
		}else
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;
}
	  
	  


/*****************************************************************************
/* lcd graphics 
 */




/* Write an 8-bit command to the LCD screen */
static void lcd_write_register(uint8_t data)
{ 
	LCD_CS_CLR;     
	LCD_RS_CLR;	  
	SPI_WriteByte(SPI1,data);
	LCD_CS_SET;	
}

/* Write an 8-bit data to the LCD screen */
static void lcd_write_data(uint8_t data)
{
	LCD_CS_CLR;
	LCD_RS_SET;
	SPI_WriteByte(SPI1,data);
	LCD_CS_SET;
}

/* Write 16-bit data to the LCD screen */	 
static void _lcd_write_16bit_data(uint16_t Data)
{	
	LCD_CS_CLR;
	LCD_RS_SET;
	if( _option & TFT_INIT_ST7796S)
	{
		SPI_WriteByte(SPI1,Data>>8);
		SPI_WriteByte(SPI1,Data);
	}
	else if( _option & TFT_INIT_ILI9488)
	{
		SPI_WriteByte(SPI1,(Data>>8)&0xF8);
		SPI_WriteByte(SPI1,(Data>>3)&0xFC);
		SPI_WriteByte(SPI1,Data<<3);
	}
 	LCD_CS_SET;
}

/*
 * Set LCD display window
 * xStar:the beginning x coordinate of the LCD display window
	yStar:the beginning y coordinate of the LCD display window
	xEnd:the endning x coordinate of the LCD display window
	yEnd:the endning y coordinate of the LCD display window
 */ 
static void _tft_lcd_setwindow(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	lcd_write_register(lcddev.setxcmd);	
	lcd_write_data(xStar>>8);
	lcd_write_data(0x00FF&xStar);		
	lcd_write_data(xEnd>>8);
	lcd_write_data(0x00FF&xEnd);
	lcd_write_register(0);	

	lcd_write_register(lcddev.setycmd);	

	lcd_write_data(yStar>>8);
	lcd_write_data(0x00FF&yStar);		
	lcd_write_data(yEnd>>8);
	lcd_write_data(0x00FF&yEnd);
	lcd_write_register(0);	

	lcd_write_register(lcddev.wramcmd);
}   



static void tft_fill_colour(uint16_t colour)
{
	unsigned int i,j;  
	_tft_lcd_setwindow(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	if( _option & TFT_INIT_ST7796S)
	{
		uint8_t hi = colour >> 8, lo = colour;
		for(i=0;i<lcddev.height;i++)
			for(j=0;j<lcddev.width;j++){
				SPI_WriteByteOnly(SPI1, hi);
				SPI_WriteByteOnly(SPI1, lo);
			}
	}
	else if( _option & TFT_INIT_ILI9488)
	{
		uint8_t r = (colour>>8)&0xF8, g = (colour>>3)&0xFC, b = colour<<3;
		for(i=0;i<lcddev.height;i++)
			for(j=0;j<lcddev.width;j++){
				SPI_WriteByteOnly(SPI1, r);
				SPI_WriteByteOnly(SPI1, g);
				SPI_WriteByteOnly(SPI1, b);
			}
	}
	SPI_WaitDone(SPI1);
	LCD_CS_SET;
} 



/*****************************************************************************
 * @name       :void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
static void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	_tft_lcd_setwindow(Xpos,Ypos,Xpos,Ypos);	
} 

/* Set the display direction of LCD screen
 * direction:0-0 degree
	1-90 degree
	2-180 degree
	3-270 degree
*/ 
static void _tft_lcd_direction(int direction)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	lcddev.dir = direction;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;
			lcd_write_register(0x36);  
			lcd_write_data((1<<3)|(1<<6));
			break;
		
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcd_write_register(0x36);  
			lcd_write_data((1<<3)|(1<<5));	    		 
			break;
		
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			lcd_write_register(0x36);  
			lcd_write_data(((1<<3)|(1<<7)));	    		 
			break;
		
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcd_write_register(0x36);  
			lcd_write_data((1<<3)|(1<<7)|(1<<6)|(1<<5));	    		 
			break;	
		default:break;
	}		
}	 


/* 
	fast fill the specified area
	sx:the beginning x coordinate of the specified area
    sy:the beginning y coordinate of the specified area
	ex:the ending x coordinate of the specified area
	ey:the ending y coordinate of the specified area
	color:the filled color value
*/
static void _tft_lcd_fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{  	
	uint16_t i,j;			
	uint16_t width=ex-sx+1; 
	uint16_t height=ey-sy+1;
	_tft_lcd_setwindow(sx,sy,ex,ey);
	LCD_CS_CLR;
	LCD_RS_SET;
	if( _option & TFT_INIT_ST7796S)
	{
		uint8_t hi = color >> 8, lo = color;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++){
				SPI_WriteByteOnly(SPI1, hi);
				SPI_WriteByteOnly(SPI1, lo);
			}
	}
	else if( _option & TFT_INIT_ILI9488)
	{
		uint8_t r = (color>>8)&0xF8, g = (color>>3)&0xFC, b = color<<3;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++){
				SPI_WriteByteOnly(SPI1, r);
				SPI_WriteByteOnly(SPI1, g);
				SPI_WriteByteOnly(SPI1, b);
			}
	}
	SPI_WaitDone(SPI1);
	LCD_CS_SET;
}





/**
 * @param {number} rx Radius along the x-axis.
 * @param {number} ry Radius along the y-axis.
 * @param {number} xc Center point along the x-axis.
 * @param {number} yc Center point along the y-axis.
 * @param {Color} c The color to use.
 * @param {boolean} filled Should the interior of the circle be filled in?
 */

static void _draw_quadrants(int x, int y, int xc, int yc, int colour, int fill) 
{
    if (fill) {
      for (int xx = xc - x; xx <= xc + x; xx++) {
        _tft_lcd_pixel(xx, yc + y, colour);
        _tft_lcd_pixel(xx, yc - y, colour);
     }
    } else {
      _tft_lcd_pixel(x + xc, y + yc, colour);
      _tft_lcd_pixel(-x + xc, y + yc, colour);
      
      _tft_lcd_pixel(x + xc, -y + yc, colour);
      _tft_lcd_pixel(-x + xc, -y + yc, colour);
    }    
}

/* Public direct calls */
void _tft_lcd_pixel(int x,int y, int colour)
{
	_tft_lcd_setwindow(x,y,x,y);
	_lcd_write_16bit_data(colour); 
}

void _tft_lcd_sprite(int x, int y, uint16_t *data, int w, int h)
{
	/* Set window once and stream all pixels — same approach as _tft_lcd_fill */
	_tft_lcd_setwindow(x, y, x + w - 1, y + h - 1);
	LCD_CS_CLR;
	LCD_RS_SET;
	int n = w * h;
	if( _option & TFT_INIT_ST7796S )
	{
		for(int i = 0; i < n; i++){
			uint16_t c = data[i];
			SPI_WriteByteOnly(SPI1, c >> 8);
			SPI_WriteByteOnly(SPI1, c);
		}
	}
	else if( _option & TFT_INIT_ILI9488 )
	{
		for(int i = 0; i < n; i++){
			uint16_t c = data[i];
			SPI_WriteByteOnly(SPI1, (c >> 8) & 0xF8);   /* R */
			SPI_WriteByteOnly(SPI1, (c >> 3) & 0xFC);   /* G */
			SPI_WriteByteOnly(SPI1, (c << 3) & 0xF8);   /* B */
		}
	}
	SPI_WaitDone(SPI1);
	LCD_CS_SET;
}

void _tft_lcd_line(int  x1, int y1, int x2, int y2, int colour)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0; 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ ) 
	{  
		_tft_lcd_pixel(uRow,uCol,colour); 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

void _tft_lcd_rect(int x1, int y1, int x2, int y2, uint16_t colour, int fill)
{
	if( fill )
	{
		_tft_lcd_fill((unsigned short)x1,(unsigned short)y1,(unsigned short)(x1+x2),(unsigned short)(y1+y2),(unsigned short)colour);
	}else{
		_tft_lcd_line(x1,y1,x1+x2,y1,colour);
		_tft_lcd_line(x1,y1,x1,y1+y2,colour);
		_tft_lcd_line(x1+x2,y1,x1+x2,y1+y2,colour);
		_tft_lcd_line(x1,y1+y2,x1+x2,y1+y2,colour);		
	}
}  

void _tft_lcd_ellipse (int xc, int yc, int rx, int ry, int colour, int fill) 
{
  // Draw points based on 4-way symmetry.
  float d1,dx,dy;
  
  int x = 0;
  int y = ry;

  // Initial decision parameter of region 1.
  d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
  dx = 2 * ry * ry * x;
  dy = 2 * rx * rx * y;

  // For region 1
  while (dx < dy) {
    _draw_quadrants(x, y,xc,yc,colour,fill);
    
    // Checking and updating value of decision parameter based on algorithm
    if (d1 < 0) {
      x++;
      dx = dx + (2 * ry * ry);
      d1 = d1 + dx + (ry * ry);
    } else {
      x++;
      y--;
      dx = dx + (2 * ry * ry);
      dy = dy - (2 * rx * rx);
      d1 = d1 + dx - dy + (ry * ry);
    }
  }

  // Decision parameter of region 2
  float d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);
  
  // Plotting points of region 2
  while (y >= 0) {
    //drawQuadrants(x, y);
    _draw_quadrants(x, y,xc,yc,colour,fill);
   
    // Checking and updating parameter value based on algorithm
    if (d2 > 0) {
      y--;
      dy = dy - (2 * rx * rx);
      d2 = d2 + (rx * rx) - dy;
    } else {
      y--;
      x++;
      dx = dx + (2 * ry * ry);
      dy = dy - (2 * rx * rx);
      d2 = d2 + dx - dy + (rx * rx);
    }
  }
}



void _tft_lcd_crosshair(int x,int y,int colour)
{
	_tft_lcd_line(x-12,y,x+13,y,colour);
	_tft_lcd_line(x,y-12,x,y+13,colour);
	_tft_lcd_pixel(x+1,y+1,colour);
	_tft_lcd_pixel(x-1,y+1,colour);
	_tft_lcd_pixel(x+1,y-1,colour);
	_tft_lcd_pixel(x-1,y-1,colour);
	_tft_lcd_ellipse(x,y,6,6,colour,0);
}	


int _tft_init(int option)
{  
	switch( option & 7 )
	{
		case 1: case 2: case 4:
		break;	/* Ok driver select */
		default: return 0;
	}
	_option = option;
	
	if( option & TFT_INIT_TOUCH)
	{
		*RCC_APB2PCENR |= IOPCEN;	
		*RCC_APB2PRSTR |= IOPCEN;	/* Assert RESET */
		*RCC_APB2PRSTR &= ~IOPCEN;	/* Release RESET */
			
		/* TOUCH PAD: Setup PC0, PC1 as input pull-up, PC2,PC3,PC4  as output, push/pull */
		*GPIO_CFGLR(GPIO_C) &= 0xFFF00000;	
		*GPIO_CFGLR(GPIO_C) |= 0x00033344;	
		*GPIO_OUTDR(GPIO_C)   = 0x0003;		/* PC0,1 pull-up */	
		tp_dev.x0 = 0;
		tp_dev.y0 = 0;
		tp_dev.x = 0;
		tp_dev.y = 0;						   	    
		tp_dev.sta = 0;
		
		_tp_read_xy(&tp_dev.x,&tp_dev.y); 		
	}

	while(_pll_init_144() );	/* Setup PLL 144MHz */

  * RCC_APB1PCENR |= TIM7EN;	/* Enable and calm timer 7 (used for delays... */
  * RCC_APB1PRSTR |= TIM7EN;
  * RCC_APB1PRSTR &= ~TIM7EN;
	*TIM7_CTLR1 = 0;
	tft_display_init(); 	
	/* Reset the TFT */
	LCD_LED_CLR;	 
	LCD_RST_CLR;
	_delay_ms(200);	
	LCD_RST_SET;
	_delay_ms(120);
	
	/* Start init */
 	lcd_write_register(0x01);	/* Software reset */
	
	if( option & TFT_INIT_ST7796S )
	{
		lcd_write_register(0xF0);
		lcd_write_data(0xC3);
		
		lcd_write_register(0xF0);
		lcd_write_data(0x96);
		
		lcd_write_register(0x36);
		lcd_write_data(0x68);	
		
		lcd_write_register(0x3A);
		lcd_write_data(0x05);
		
		lcd_write_register(0xB0);
		lcd_write_data(0x80);	
		
		lcd_write_register(0xB6);
		lcd_write_data(0x00);
		lcd_write_data(0x02);	

		lcd_write_register(0xB5);
		lcd_write_data(0x02);	/* VFP */
		lcd_write_data(0x03);	/* VBP */
		lcd_write_data(0x00);
		lcd_write_data(0x04);	/*HBP */
		
		lcd_write_register(0xB1);
		lcd_write_data(0x80);	
		lcd_write_data(0x10);	

		lcd_write_register(0xB4);
		lcd_write_data(0x00);
		
		lcd_write_register(0xB7);
		lcd_write_data(0xC6);
		
		lcd_write_register(0xC5);
		lcd_write_data(0x24);
		
		lcd_write_register(0xE4);
		lcd_write_data(0x31);
		
		lcd_write_register(0xE8);
		lcd_write_data(0x40);
		lcd_write_data(0x8A);
		lcd_write_data(0x00);
		lcd_write_data(0x00);
		lcd_write_data(0x29);
		lcd_write_data(0x19);
		lcd_write_data(0xA5);
		lcd_write_data(0x33);
		
		lcd_write_register(0xC2);
		
		lcd_write_register(0xA7);
		
		lcd_write_register(0xE0);
		lcd_write_data(0xF0);
		lcd_write_data(0x09);
		lcd_write_data(0x13);
		lcd_write_data(0x12);
		lcd_write_data(0x12);
		lcd_write_data(0x2B);
		lcd_write_data(0x3C);
		lcd_write_data(0x44);
		lcd_write_data(0x4B);
		lcd_write_data(0x1B);
		lcd_write_data(0x18);
		lcd_write_data(0x17);
		lcd_write_data(0x1D);
		lcd_write_data(0x21);

		lcd_write_register(0XE1);
		lcd_write_data(0xF0);
		lcd_write_data(0x09);
		lcd_write_data(0x13);
		lcd_write_data(0x0C);
		lcd_write_data(0x0D);
		lcd_write_data(0x27);
		lcd_write_data(0x3B);
		lcd_write_data(0x44);
		lcd_write_data(0x4D);
		lcd_write_data(0x0B);
		lcd_write_data(0x17);
		lcd_write_data(0x17);
		lcd_write_data(0x1D);
		lcd_write_data(0x21);

		lcd_write_register(0X36);
		lcd_write_data(0xEC);
		lcd_write_register(0xF0);
		lcd_write_data(0xC3);
		lcd_write_register(0xF0);
		lcd_write_data(0x69);		
		lcd_write_register(0X13);
		
		lcd_write_register(0x13);
		lcd_write_register(0x11);
		lcd_write_register(0x29);
		_tft_lcd_direction(1);	
	}
	else if( option & TFT_INIT_ILI9488 )
	{
		lcd_write_register(0xF7);
		lcd_write_data(0xA9);
		lcd_write_data(0x51);
		lcd_write_data(0x2C);
		lcd_write_data(0x82);
		
		lcd_write_register(0xC0);
		lcd_write_data(0x11);
		lcd_write_data(0x09);
		
		lcd_write_register(0xC1);
		lcd_write_data(0x44);
		
		lcd_write_register(0XC5);
		lcd_write_data(0x00);
		lcd_write_data(0x0A);
		lcd_write_data(0x80);
		
		lcd_write_register(0xB1);
		lcd_write_data(0xB0);
		lcd_write_data(0x11);
		
		lcd_write_register(0xB4);
		lcd_write_data(0x02);

		lcd_write_register(0xB6);
		
		lcd_write_data(0x02);
		lcd_write_data(0x02);
		lcd_write_data(0x3B);
		
		
		lcd_write_register(0xB7);
		lcd_write_data(0xc6);
		
		lcd_write_register(0xBE);
		lcd_write_data(0x00);
		lcd_write_data(0x04);
		
		lcd_write_register(0xE9);
		lcd_write_data(0x00);
		
		lcd_write_register(0x36);
		lcd_write_data(0x48);
		lcd_write_register(0x3A);
		lcd_write_data(0x06);
		
		lcd_write_register(0xE0);
		lcd_write_data(0x00);
		lcd_write_data(0x07);
		lcd_write_data(0x10);
		lcd_write_data(0x09);
		lcd_write_data(0x17);
		lcd_write_data(0x0B);
		lcd_write_data(0x41);
		lcd_write_data(0x89);
		lcd_write_data(0x4B);
		lcd_write_data(0x0A);
		lcd_write_data(0x0C);
		lcd_write_data(0x0E);
		lcd_write_data(0x18);
		lcd_write_data(0x1B);
		lcd_write_data(0x0F);
		
		lcd_write_register(0XE1);
		lcd_write_data(0x00);
		lcd_write_data(0x17);
		lcd_write_data(0x1A);
		lcd_write_data(0x04);
		lcd_write_data(0x0E);
		lcd_write_data(0x06);
		lcd_write_data(0x2F);
		lcd_write_data(0x45);
		lcd_write_data(0x43);
		lcd_write_data(0x02);
		lcd_write_data(0x0A);
		lcd_write_data(0x09);
		lcd_write_data(0x32);
		lcd_write_data(0x36);
		lcd_write_data(0x0F);

		lcd_write_register(0x11);
		_delay_ms(200);
		
		lcd_write_register(0x29);
		_tft_lcd_direction(1);
	}
	else if( option & TFT_INIT_ILI9486 )
	{
		lcd_write_register(0XF1);
		lcd_write_data(0x36);
		lcd_write_data(0x04);
		lcd_write_data(0x00);
		lcd_write_data(0x3C);
		lcd_write_data(0X0F);
		lcd_write_data(0x8F);
		
		lcd_write_register(0XF2);
		lcd_write_data(0x18);
		lcd_write_data(0xA3);
		lcd_write_data(0x12);
		lcd_write_data(0x02);
		lcd_write_data(0XB2);
		lcd_write_data(0x12);
		lcd_write_data(0xFF);
		lcd_write_data(0x10);
		lcd_write_data(0x00);
		
		lcd_write_register(0XF8);
		lcd_write_data(0x21);
		lcd_write_data(0x04);
		
		lcd_write_register(0XF9);
		lcd_write_data(0x00);
		lcd_write_data(0x08);
		
		lcd_write_register(0x36);
		lcd_write_data(0x08);
		
		lcd_write_register(0xB4);
		lcd_write_data(0x00);
		
		lcd_write_register(0xC1);
		lcd_write_data(0x47); //0x41
		
		lcd_write_register(0xC5);
		lcd_write_data(0x00);
		lcd_write_data(0xAF); //0x91
		lcd_write_data(0x80);
		lcd_write_data(0x00);
		
		lcd_write_register(0xE0);
		lcd_write_data(0x0F);
		lcd_write_data(0x1F);
		lcd_write_data(0x1C);
		lcd_write_data(0x0C);
		lcd_write_data(0x0F);
		lcd_write_data(0x08);
		lcd_write_data(0x48);
		lcd_write_data(0x98);
		lcd_write_data(0x37);
		lcd_write_data(0x0A);
		lcd_write_data(0x13);
		lcd_write_data(0x04);
		lcd_write_data(0x11);
		lcd_write_data(0x0D);
		lcd_write_data(0x00);
		
		lcd_write_register(0xE1);
		lcd_write_data(0x0F);
		lcd_write_data(0x32);
		lcd_write_data(0x2E);
		lcd_write_data(0x0B);
		lcd_write_data(0x0D);
		lcd_write_data(0x05);
		lcd_write_data(0x47);
		lcd_write_data(0x75);
		lcd_write_data(0x37);
		lcd_write_data(0x06);
		lcd_write_data(0x10);
		lcd_write_data(0x03);
		lcd_write_data(0x24);
		lcd_write_data(0x20);
		lcd_write_data(0x00);
		
		lcd_write_register(0x3A);
		lcd_write_data(0x66);
		
		lcd_write_register(0x11);
		
		lcd_write_register(0x36);
		lcd_write_data(0x28);
		
		_delay_ms(120);
		lcd_write_register(0x29);
		_tft_lcd_direction(3);
	}
	_tft_lcd_setwindow(0,0,lcddev.width-1,lcddev.height-1);
	tft_fill_colour(TFT_YELLOW);
	LCD_LED_SET;	
	return 1; 
}


#endif /* TARGET_HW or TARGET_SIM */

