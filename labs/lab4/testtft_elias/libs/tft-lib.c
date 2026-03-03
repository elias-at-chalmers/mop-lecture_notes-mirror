/*
 * 	tft-lib.c
 *	Stubs for calling library routines
 *  through "ecall"
 */

#ifdef CUSTOM


#include <stdint.h>
#include <md307.h>
#include <tftmd307.h>


int tft_init(int option)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[1];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into a7 */	
	frame.function = 0;
	caller_frame[0]=option;
	frame.caller_frame = caller_frame;
#ifndef NOLIB
	*((void (**)(void) ) (0x2001C014) ) = ecall;	/* Ecall exception vector (m-mode) */
#endif
	__asm__ volatile(" ecall\n");
	return caller_frame[0];
}

void tft_lcd_ellipse(int xc, int yc, int xw, int yw, int colour, int fill)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into a7 */
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
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
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
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
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
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
	frame.function = 4;
	caller_frame[0]=x;
	caller_frame[1]=y;
	caller_frame[2]=colour;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}


void tft_lcd_pixel(int x,int y,int colour)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[6];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
	frame.function = 5;
	caller_frame[0]=x;
	caller_frame[1]=y;
	caller_frame[2]=colour;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}


void delay_ms(unsigned int ms)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[1];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
	frame.function = 50;
	caller_frame[0]=ms;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
}


int tp_scan(int param)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[1];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
	frame.function = 70;
	caller_frame[0]=param;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
	return caller_frame[0];
}

int tp_getpos(int *x, int *y)
{
	volatile TFT_CALL frame;
	unsigned int caller_frame[2];
	volatile PTFT_CALL frameptr = &frame;
	__asm__ volatile (" mv a7,%0 \t"	: "=r" (frameptr) );  /* 'frame' into t1 */
	frame.function = 71;
	caller_frame[0]= (int) x;
	caller_frame[1]= (int) y;
	frame.caller_frame = caller_frame;
	__asm__ volatile(" ecall\n");
	return caller_frame[0];
}

#endif /* CUSTOM */

