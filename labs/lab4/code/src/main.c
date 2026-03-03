///////////////////////////////////////////////////////////////////////////////
// Lab 4: Write a little game!
// ===========================================================================
// The introduction to this lab will be a few smaller assignments as usual, 
// but after that, it is up to you to use what you have learned to write a 
// simple game.
// 
// The game will play on the 480x320 TFT display, so you need to connect that 
// in the simulator. You will not have to program that directly, instead you 
// will use the tftlib library we have provided. 
// It has the following simple API: 
// - tft_init() to initialize the display (call this once at the start of your 
//   program)
// - tft_ellipse(xc, yc, rx, ry, color, filled) to draw an ellipse (filled or not)
//     - xc, yc are the center coordinates of the ellipse
//     - rx, ry are the radius along the x and y axes, respectively
// - tft_rect(x1, y1, w, h, color, filled) to draw a rectangle (filled or not)
// - tft_line(x1, y1, x2, y2, color) to draw a line
// - tft_pixel(x, y, color) to draw a single pixel
// - tft_sprite(x, y, data, w, h) to draw a sprite (a small bitmap) at the 
//   given location.
// 
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../tftlib/tft.h"
#include "sprites/sprite_17_2.h"
// RGB565 color helpers
#define RGB565(r, g, b) ((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))
#define COLOR_BLACK  0x0000
#define COLOR_WHITE  0xFFFF
#define COLOR_RED    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_BLUE   0x001F

#define SOLUTION 1

int main(void)
{
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 1: Initialize the display and fill it with a solid color
    // =======================================================================
    // Once tft_init() has completed, the screen should be all yellow. Fill it 
    // with a black background color (use COLOR_BLACK as the color argument) 
    // using tft_rect().
    // Note: On the simulator, w has to be given as 479, and h as 319, to fill
    //       the whole screen. 
    ///////////////////////////////////////////////////////////////////////////
#if SOLUTION     
    tft_init();
    tft_rect(0, 0, 479, 319, COLOR_BLACK, 1);
#endif
    
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2: Draw a ball in the middle of the screen
    // =======================================================================
    // Use the tft_ellipse() function to draw a ball in the middle of 
    // the screen. Choose any color for the ball. It should have  
    // The colors are in RGB565 format, which is a common 16-bit color format 
    // used in embedded graphics. It means that the most significant 5 bits are red,    
    // the next 6 bits are green, and the least significant 5 bits are blue.
    // 
    // You can use the RGB565(r, g, b) macro above to convert 8-bit RGB values 
    // to RGB565 format. For example, RGB565(255, 0, 0) will give you the 
    // RGB565 value for pure red.
    ///////////////////////////////////////////////////////////////////////////
#if SOLUTION
    tft_ellipse(240, 160, 8, 8, RGB565(25, 25, 50), 1);
#endif  

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 3: Make it move!
    // =======================================================================
    // Now, make the ball move across the screen from left to right. We have 
    // given you a simple loop that will calculate the center and bounce on 
    // the edges, so just fill in the drawing code. 
    // 
    // A few hints: 
    // - Drawing the ball does not erase the previous one. On desktop games
    //   we usually clear the whole screen every frame. You can try that, but
    //   you will find that it is very slow. Instead, just erase the previous
    //   ball by drawing it again, in the background color. 
    // - When you do that, there will be flickering. To reduce the flickering
    //   you can pause for a short time after drawing the ball, before erasing 
    //   it. Use SysTick, or just a simple for-loop delay. 
    // - When you run this on hardware, timings might be very different. 
    ///////////////////////////////////////////////////////////////////////////
#if 0    
    int center_x = 240;
    int center_y = 160;
    int speed_x = 1;
    while(1) {
        if (center_x <= 8 || center_x >= 472) { // Bounce on the edges
            speed_x = -speed_x;
        }   
#if SOLUTION
        tft_ellipse(center_x, center_y, 8, 8, RGB565(25, 25, 50), 1);
        for(volatile unsigned int _d = 0; _d < 600; _d++); // Simple delay
        tft_ellipse(center_x, center_y, 8, 8, COLOR_BLACK, 1);
#else
        // Draw the ball here
#endif
        center_x += speed_x;
    }
#elif 0
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 3: Make it bounce in 2D!
    // =======================================================================
    // Now modify the code to make the ball bounce in both x and y directions. 
    ///////////////////////////////////////////////////////////////////////////
    int center_x = 240;
    int center_y = 160;
    int speed_x = 1;
    int speed_y = 1; 
    while(1) {
        if (center_x <= 8 || center_x >= 472) { // Bounce on the edges
            speed_x = -speed_x;
        }   
        if (center_y <= 8 || center_y >= 312) { // Bounce on the edges
            speed_y = -speed_y;
        }
        tft_ellipse(center_x, center_y, 8, 8, RGB565(25, 25, 50), 1);
        for(volatile unsigned int _d = 0; _d < 600; _d++); // Simple delay
        tft_ellipse(center_x, center_y, 8, 8, COLOR_BLACK, 1);
        center_x += speed_x;
        center_y += speed_y;
    }
#elif 1
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4: Use a sprite!
    // =======================================================================
    // Just for fun, let's use a sprite instead of an ellipse. A sprite is a 
    // small image, and we have included a bunch of them in the sprites/ 
    // folder that you can include if you want. 
    // 
    // You can find that "sprite_17_2.h" has been included above. Look at that
    // file and you will see that it is a 16x16 pixel image in the right format.
    //
    // Note that this will be very slow on the simulator, but quite fast on 
    // hardware, so if you want to use sprites in your game, you might want to
    // use rectangles as placeholders while developing. 
    ///////////////////////////////////////////////////////////////////////////
    int center_x = 240;
    int center_y = 160;
    int speed_x = 1;
    int speed_y = 1; 
    while(1) {
        if (center_x <= 8 || center_x >= 472) { // Bounce on the edges
            speed_x = -speed_x;
        }   
        if (center_y <= 8 || center_y >= 312) { // Bounce on the edges
            speed_y = -speed_y;
        }
        tft_sprite(center_x - 8, center_y - 8, sprite_17_2, 16, 16);
        for(volatile unsigned int _d = 0; _d < 6; _d++); // Simple delay
        tft_rect(center_x - 8, center_y - 8, 16, 16, COLOR_BLACK, 1);
        center_x += speed_x;
        center_y += speed_y;
    }

#endif

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4: And now...
    // =======================================================================
    // If you add a little paddle (a rectangle) that can be moved with the 
    // keypad, you can easily complete a little pong game. Or you could do 
    // something completely different. 
    // 
    // See the canvas page for instructions and points. 
    ///////////////////////////////////////////////////////////////////////////
}