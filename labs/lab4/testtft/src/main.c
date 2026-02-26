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
extern void tft_lcd_pixel(int x, int y, uint16_t color);                              // a0-a2
extern void tft_lcd_line(int x1, int y1, int x2, int y2, uint16_t color);            // a0-a4
extern void tft_lcd_rect(int x, int y, int w, int h, uint16_t color, int filled);    // a0-a5
extern void tft_lcd_ellipse(int x, int y, int rx, int ry, uint16_t color, int filled); // a0-a5
extern void tft_crosshair(int x, int y, uint16_t color);                              // a0-a2

void _delay_ms(unsigned int ms);	

int main(void)
{
    tft_init(1);

    // Fill screen with random colors
    //while(1) {
    //    tft_lcd_rect(0, 0, 479, 319, RGB565(rand() % 32, rand() % 64, rand() % 32), 1);
    //}
    tft_lcd_rect(0, 0, 479, 319, COLOR_BLACK, 1);
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
            //tft_lcd_rect(0, 0, 479, 319, COLOR_BLACK, 1);
            tft_lcd_rect(x, 160, 16, 16, COLOR_WHITE, 1);
            for(volatile unsigned int _d = 0; _d < 600000; _d++); /* ~16ms @ 144MHz */
            tft_lcd_rect(x, 160, 16, 16, COLOR_BLACK, 1);
        }
    }
}