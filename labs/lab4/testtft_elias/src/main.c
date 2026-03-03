#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
// RGB565 color helpers
#define RGB565(r, g, b) ((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))
#define COLOR_BLACK  0x0000
#define COLOR_WHITE  0xFFFF
#define COLOR_RED    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_BLUE   0x001F

// Signatures reverse-engineered from libtftmd307-sim.a disassembly:
// Each public wrapper saves its argument registers (a0..aN) then issues an ecall.
extern int  tft_init(int type);
extern void tft_pixel(int x, int y, uint16_t color);                              // a0-a2
extern void tft_line(int x1, int y1, int x2, int y2, uint16_t color);            // a0-a4
extern void tft_rect(int x, int y, int w, int h, uint16_t color, int filled);    // a0-a5
extern void tft_ellipse(int x, int y, int rx, int ry, uint16_t color, int filled); // a0-a5
extern void tft_sprite(int x, int y, uint16_t *data, int w, int h);                     // a0-a2



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

int main(void)
{
    tft_init(1);

    // Fill screen with random colors
    //while(1) {
    //    tft_lcd_rect(0, 0, 479, 319, RGB565(rand() % 32, rand() % 64, rand() % 32), 1);
    //}
    tft_rect(0, 0, 479, 319, COLOR_BLACK, 1);
    //tft_lcd_ellipse(120, 160, 50, 80, COLOR_WHITE, 1);
    //tft_lcd_pixel(120, 160, COLOR_RED); // Center of the ellipse
    // Draw a red diagonal line from top-left to bottom-right
    //tft_lcd_line(0, 0, 239, 319, COLOR_RED);
    // Draw a green line across the middle horizontally
    //tft_lcd_line(0, 160, 239, 160, COLOR_GREEN);
    // Draw a blue line down the middle vertically
    //tft_lcd_line(120, 0, 120, 319, COLOR_BLUE);
    while(1){
        for(int x = 0; x < 480; x++){
            #if 0
            tft_lcd_rect(x, 160, 2, 2, COLOR_WHITE, 1);
            //for(volatile unsigned int _d = 0; _d < 600; _d++); /* ~16ms @ 144MHz */
            tft_lcd_rect(x, 160, 2, 2, COLOR_BLACK, 1);
            #elif 0
            tft_lcd_line(x, 160, x + 0, 160 + 1, COLOR_WHITE);
            for(volatile unsigned int _d = 0; _d < 6000; _d++);
            tft_lcd_line(x, 160, x + 0, 160 + 1, COLOR_BLACK);
            #elif 0
            tft_lcd_pixel(x, 160, COLOR_WHITE);
            for(volatile unsigned int _d = 0; _d < 600; _d++);
            tft_lcd_pixel(x, 160, COLOR_BLACK);
            #elif 0
            asm_pixel(x, 160, COLOR_WHITE);
            //for(volatile unsigned int _d = 0; _d < 600; _d++);
            asm_pixel(x, 160, COLOR_BLACK);
            #else 
            //tft_lcd_ellipse(x, 160, 7, 7, 0xFFFF, 1); // a0-a5
            tft_sprite(x, 160, ball_sprite, 16, 16);
            //tft_sprite(x, 160, ball_sprite, 16, 16);
            for(volatile unsigned int _d = 0; _d < 600; _d++);
            tft_rect(x, 160, 16, 16, COLOR_BLACK, 1);
            #endif
        }
    }
}