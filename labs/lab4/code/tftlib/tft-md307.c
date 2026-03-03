
#include "md307.h"
#include "tftmd307.h"
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int  hw_tft_init(int option);
void hw_tft_ellipse(int xc, int yc, int rx, int ry, uint16_t colour, int fill);
void hw_tft_rect(int x1, int y1, int x2, int y2, uint16_t colour, int fill);
void hw_tft_line(int x1, int y1, int x2, int y2, uint16_t colour);
void hw_tft_pixel(int x, int y, uint16_t colour);
void hw_tft_sprite(int x, int y, uint16_t *data, int w, int h);

void (*tft_ellipse)(int xc, int yc, int rx, int ry, uint16_t colour, int fill);
void (*tft_rect)(int x1, int y1, int x2, int y2, uint16_t colour, int fill);
void (*tft_line)(int  x1, int y1, int x2, int y2, uint16_t colour);
void (*tft_pixel)(int, int, uint16_t colour);
void (*tft_sprite)(int x, int y, uint16_t *data, int w, int h);

void tft_init()
{
	tft_ellipse = hw_tft_ellipse;
	tft_rect = hw_tft_rect;
	tft_line = hw_tft_line;
	tft_pixel = hw_tft_pixel;
	tft_sprite = hw_tft_sprite;
	hw_tft_init(1);
}


#define CMD_RDX 0XD0
#define CMD_RDY 0X90

static _lcd_dev lcddev;
static	int _option;					


static void _lcd_write_16bit_data(uint16_t Data);
static void _tft_lcd_setwindow(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
static void _tft_lcd_direction(int direction);
static void _tft_lcd_fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);

/* Delay functions using TIMER7 */
void timer7_delay1ms(void )
{
	/* System core clock: 144MHz */
	 /* freq: PSC is # of microsecs-1 */
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
/* Public direct calls */
void hw_tft_pixel(int x,int y, uint16_t colour)
{
	_tft_lcd_setwindow(x,y,x,y);
	_lcd_write_16bit_data(colour); 
}


static void _draw_quadrants(int x, int y, int xc, int yc, int colour, int fill) 
{
    if (fill) {
      for (int xx = xc - x; xx <= xc + x; xx++) {
        hw_tft_pixel(xx, yc + y, colour);
        hw_tft_pixel(xx, yc - y, colour);
     }
    } else {
    	hw_tft_pixel(x + xc, y + yc, colour);
      	hw_tft_pixel(-x + xc, y + yc, colour);      
      	hw_tft_pixel(x + xc, -y + yc, colour);
      	hw_tft_pixel(-x + xc, -y + yc, colour);
    }    
}


void hw_tft_sprite(int x, int y, uint16_t *data, int w, int h)
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

void hw_tft_line(int  x1, int y1, int x2, int y2, uint16_t colour)
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
		hw_tft_pixel(uRow,uCol,colour); 
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

void hw_tft_rect(int x1, int y1, int x2, int y2, uint16_t colour, int fill)
{
	if( fill )
	{
		_tft_lcd_fill((unsigned short)x1,(unsigned short)y1,(unsigned short)(x1+x2),(unsigned short)(y1+y2),(unsigned short)colour);
	}else{
		hw_tft_line(x1,y1,x1+x2,y1,colour);
		hw_tft_line(x1,y1,x1,y1+y2,colour);
		hw_tft_line(x1+x2,y1,x1+x2,y1+y2,colour);
		hw_tft_line(x1,y1+y2,x1+x2,y1+y2,colour);		
	}
}  

void hw_tft_ellipse (int xc, int yc, int rx, int ry, uint16_t colour, int fill) 
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

int hw_tft_init(int option)
{  
	switch( option & 7 )
	{
		case 1: case 2: case 4:
		break;	/* Ok driver select */
		default: return 0;
	}
	_option = option;
	


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
