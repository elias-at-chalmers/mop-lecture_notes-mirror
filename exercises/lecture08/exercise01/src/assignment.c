///////////////////////////////////////////////////////////////////////////////
// Lecture08 - Exercise01
// ============================================================================
// 
// Connect a console to the simulator as usual. 
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdio.h>

#define SOLUTION 1

///////////////////////////////////////////////////////////////////////////////
// Assignment 1 
// ============================================================================
// Create a struct defenition of systick so that the code in this assignment
// compiles, and the check passes. 
///////////////////////////////////////////////////////////////////////////////

#if SOLUTION
typedef struct {
    union {
        struct {
            uint32_t ste : 1; // SysTick Timer Enable
            uint32_t stie : 1; // SysTick Timer Interrupt Enable
            uint32_t stclk : 1; // SysTick Timer Clock Source
            uint32_t stre : 1; // SysTick Timer Reload Enable
            uint32_t mode : 1; // SysTick Timer Mode
            uint32_t init : 1;
            uint32_t reserved : 25;
            uint32_t swie : 1;
        } ctlr;
        uint32_t CTLR;
    };
    uint32_t SR;
    union {
        struct {
            uint32_t CNTL;
            uint32_t CNTH;
        };
        uint64_t CNT;
    };
    union {
        struct {
            uint32_t CMPL;
            uint32_t CMPH;
        };
        uint64_t CMP;
    };
} SysTick_t;

volatile SysTick_t * systick = (SysTick_t*) 0xE000F000;
#endif

void assignment1()
{
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 1: At minimum, this code should work
    ///////////////////////////////////////////////////////////////////////////
    systick->CTLR = 0b10100;   // Write nonsense values for checking
    systick->SR = 0;    
    systick->CNTL = 1;  
    systick->CNTH = 2; 
    systick->CMPL = 3; 
    systick->CMPH = 4; 
    // Bitfields

    // CMP 64
}


int moles[4] = {0, 1, 0, 1};

#define GPIOD_CFGLR ((volatile uint32_t *)0x40011400)
#define GPIOD_INDR  ((volatile uint32_t *)0x40011408)
#define GPIOD_OUTDR ((volatile uint32_t *)0x4001140C)
#define GPIOE_CFGLR ((volatile uint32_t *)0x40011800)
#define GPIOE_OUTDR ((volatile uint32_t *)0x4001180C)


int whackamole()
{
    ///////////////////////////////////////////////////////////////////////////
    // To be able to read the top row of buttons from the keypad, we configure
    // the lower four bits as input, pull-up, and the fifth bit as output, 
    // open-drain. Top row (pin 5) always active (0).
    ///////////////////////////////////////////////////////////////////////////
    *GPIOD_CFGLR = 0x68888;         // Pin 0-3, Pull Up; Pin 4, Open Drain
    *GPIOD_OUTDR = 0xF;             // Output Pin 5, Active (0), 
                                    // Input Pins 0-3 Pull-Up (1)
    ///////////////////////////////////////////////////////////////////////////
    // To show the moles on the bargraph, configure PE[0:7] as output, 
    // push-pull.
    ///////////////////////////////////////////////////////////////////////////
    *GPIOE_CFGLR = 0x22222222;        // Pin 0-7, Push-Pull Output

    int game_over = 0;
    int score = 0; 
    while(!game_over) {
        ///////////////////////////////////////////////////////////////////////
        // Read all four buttons
        ///////////////////////////////////////////////////////////////////////
        uint8_t buttons = *GPIOD_INDR & 0xF; // Read the state of the buttons
        for(int i = 0; i < 4; i++) {
            ///////////////////////////////////////////////////////////////////
            // If any button is pressed, and there is a mole at that position,
            // kill the mole and increase score. 
            ///////////////////////////////////////////////////////////////////
            if((buttons & (1 << i)) == 0) { // If button is pressed
                if(moles[i]) { // If there is a mole
                    moles[i] = 0; // Kill the mole
                    score++;
                }
            }
        }
        ///////////////////////////////////////////////////////////////////////
        // Show the moles on the bargraph (two bars per mole).
        ///////////////////////////////////////////////////////////////////////
        *GPIOE_OUTDR =  (moles[0] << 6) | (moles[0] << 7) |
                        (moles[1] << 4) | (moles[1] << 5) |
                        (moles[2] << 2) | (moles[2] << 3) |
                        (moles[3] << 0) | (moles[3] << 1);
        ///////////////////////////////////////////////////////////////////////
        // If all moles are out, game over.
        ///////////////////////////////////////////////////////////////////////
        if(moles[0] & moles[1] & moles[2] & moles[3]) game_over = 1;
    }
    printf("Game Over! Your score is %d\n", score);
}