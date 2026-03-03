/*
 * 	libdbg307.c
 *	dbg307 builtin functions
 *  stub library
 *  NOTE: Running the builtin functions works fine, however stepping through doesn't.
 *  Version 2025-06-06
 */
 
#include	"dbg307.h"

__attribute__((naked)) void dbg_restart(void){
	__asm__ volatile(" la a7,0x08000200\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) void dbg_clockinit(void){
	__asm__ volatile(" la a7,0x08000204\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) void dbg_portinit(void){
	__asm__ volatile(" la a7,0x08000208\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) void dbg_outchar(unsigned char c){
	__asm__ volatile(" la a7,0x08000210\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) unsigned char dbg_tstchar(void){
	__asm__ volatile(" la a7,0x08000214\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) int dbg_tft_init(int option){
	__asm__ volatile(" la a7,0x08000230\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) void dbg_tft_lcd_ellipse(int xc, int yc, int rx, int ry, int colour, int fill){
	__asm__ volatile(" la a7,0x08000234\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) void dbg_tft_lcd_rect(int xc, int yc, int xr, int yr, int colour, int fill){
	__asm__ volatile(" la a7,0x08000238\n");				
	__asm__ volatile(" jr a7\n");				
}	
		
__attribute__((naked)) void dbg_tft_lcd_line(int  x1, int y1, int x2, int y2, int colour){
	__asm__ volatile(" la a7,0x0800023C\n");				
	__asm__ volatile(" jr a7\n");				
}	

__attribute__((naked)) void dbg_tft_lcd_crosshair(int x,int y,int colour){
	__asm__ volatile(" la a7,0x08000240\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) void dbg_tft_lcd_pixel(int x,int y, int colour){
	__asm__ volatile(" la a7,0x08000244\n");				
	__asm__ volatile(" jr a7\n");				
}	
	
__attribute__((naked)) void dbg_delay_ms(unsigned int ms){
	__asm__ volatile(" la a7,0x08000248\n");				
	__asm__ volatile(" jr a7\n");				
}

__attribute__((naked)) int dbg_tft_tp_scan(int tp){
	__asm__ volatile(" la a7,0x0800024C\n");				
	__asm__ volatile(" jr a7\n");				
}	

__attribute__((naked)) int dbg_tft_tp_getpos( int *x, int *y ){
	__asm__ volatile(" la a7,0x08000250\n");				
	__asm__ volatile(" jr a7\n");				
}	
		



