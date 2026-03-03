#include <stdint.h>
extern int  tft_init();
extern void (*tft_pixel)(int x, int y, uint16_t color);
extern void (*tft_line)(int x1, int y1, int x2, int y2, uint16_t color);
extern void (*tft_rect)(int x, int y, int w, int h, uint16_t color, int filled);
extern void (*tft_ellipse)(int x, int y, int rx, int ry, uint16_t color, int filled);
extern void (*tft_sprite)(int x, int y, uint16_t *data, int w, int h);