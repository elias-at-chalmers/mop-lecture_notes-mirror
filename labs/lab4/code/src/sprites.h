///////////////////////////////////////////////////////////////////////////////
//  sprites.h
//  ===========================================================================
//  These are some "sprites" (little bitmaps) that can be used with the 
//  tft_sprite() function. Usually, sprites are stored in png files or similar, 
//  but here we are hardcoding them as C arrays for simplicity. 
//
//  The colors are in RGB565 format, which is a common 16-bit color format used 
//  in embedded graphics. It means that the most significant 5 bits are red, 
//  the next 6 bits are green, and the least significant 5 bits are blue.
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

uint16_t ball_sprite[16*16] = {
    #define BK 0x0000   /* transparent/black         */
    #define DG 0x4208   /* dark  gray  RGB(64, 64,64) */
    #define LG 0xC618   /* light gray  RGB(192,192,192) */
    #define WH 0xFFFF   /* white highlight            */
    /* row  0 */ BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,
    /* row  1 */ BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,
    /* row  2 */ BK,BK,BK,BK,BK,DG,DG,DG,DG,DG,DG,BK,BK,BK,BK,BK,
    /* row  3 */ BK,BK,BK,DG,DG,LG,LG,LG,LG,LG,LG,DG,DG,BK,BK,BK,
    /* row  4 */ BK,BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,BK,
    /* row  5 */ BK,BK,DG,LG,LG,WH,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,
    /* row  6 */ BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,
    /* row  7 */ BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,
    /* row  8 */ BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,
    /* row  9 */ BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,
    /* row 10 */ BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,
    /* row 11 */ BK,BK,BK,DG,LG,LG,LG,LG,LG,LG,LG,LG,DG,BK,BK,BK,
    /* row 12 */ BK,BK,BK,DG,DG,LG,LG,LG,LG,LG,LG,DG,DG,BK,BK,BK,
    /* row 13 */ BK,BK,BK,BK,BK,DG,DG,DG,DG,DG,DG,BK,BK,BK,BK,BK,
    /* row 14 */ BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,
    /* row 15 */ BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,BK,
    #undef BK
    #undef DG
    #undef LG
    #undef WH
};