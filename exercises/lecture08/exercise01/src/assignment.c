///////////////////////////////////////////////////////////////////////////////
// Lecture08 - Exercise01
// ============================================================================
// In this exercies, you will learn about interrupts and systick by 
// implementing the ancient traditional japaneese game of "Whack-a-mole".
// (https://en.wikipedia.org/wiki/Whac-A-Mole)
//
// The exercise is not the easiest one, but when you finish you will be an 
// expert on systick interrupts! Also, you will have developed your first video
// game, if you hadn't done so already.
//
// Connect a console to the simulator as usual. 
// Connect a keypad to GPIOD[0:7]
// Connect a bargraph to GPIOE[0:7]
//
// The code will not compile and run until you have completed assignment 1.
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
void check_assignment2_1(); 
void check_assignment2_2(); 
void InitInterrupts();

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
    ///////////////////////////////////////////////////////////////////////////
    // If you want to practice your bitfields, make this code work as well
    ///////////////////////////////////////////////////////////////////////////
    // systick->ctlr.ste = 0;
    // systick->ctlr.stie = 0;
    // systick->ctlr.stclk = 1;
    // systick->ctlr.stre = 0;
    // systick->ctlr.mode = 1;
    // systick->ctlr.init = 0;
    // systick->ctlr.swie = 0;
    ///////////////////////////////////////////////////////////////////////////
    // If you want to be really snazzy, make this work as well.
    ///////////////////////////////////////////////////////////////////////////
    // systick->CNT = 0x0000000200000001; // Set CNTL to 1, CNTH to 2.
    // systick->CMP = 0x0000000400000003; // Set CMPL to 3, CMPH to 4.
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 2: Whack-a-mole
// ============================================================================
// In the code below, some lazy person has implemented half a game. The point
// of the game is to whack the "moles" (each represented by 2 leds on the
// bargraph) by pressing the corresponding buttons on the top row of the keypad. 
//
// Currently, if you run the program, one mole will pop up, and you can whack it
// with the buttons, but no new moles appear. Your task is to complete the game
// by implementing the remaining assignments. 
//
// First, have a look at the code and make sure you understand how it works. 
// 
// You then find the first assignment in the assembly file, "assignment.s"
///////////////////////////////////////////////////////////////////////////////


// Note, this has to be volatile, since it is changed in the interrupt handler.
volatile int moles[4] = {1, 0, 0, 0};

#define GPIOD_CFGLR ((volatile uint32_t *)0x40011400)
#define GPIOD_INDR  ((volatile uint32_t *)0x40011408)
#define GPIOD_OUTDR ((volatile uint32_t *)0x4001140C)
#define GPIOE_CFGLR ((volatile uint32_t *)0x40011800)
#define GPIOE_OUTDR ((volatile uint32_t *)0x4001180C)

///////////////////////////////////////////////////////////////////////////////
// Assignment 2.2: Initialize the SysTick timer to generate an interrupt every
//                 10 ms. (On the hardware this would be way too fast,
//                 but in the simulator, it will be okay.)
//
//                 The counter should count to the compare value, and then 
//                 reset to 0 and continue counting. You should enable 
//                 interrupts.
// 
//                 Do NOT start the timer yet. 
///////////////////////////////////////////////////////////////////////////////
void InitSystick()
{
    // Remember clock-frequency = clocks / time, so clocks = clock-frequency * time.
    // Clock frequency is 144Mhz = 144000000Hz, time is 10ms = (1/100) seconds
    // (we cannot check the value you set, so do it right! :) )
    //
    // Important! Do not forget to disable the timer by setting all of CTLR to 0
    //            before writing to the other registers. Otherwise, the 
    //            simulator gets confused. 
    #if SOLUTION
    systick->CTLR = 0; // Disable systick and clear configuration. 
    systick->CMP = 1440000; // Set the compare value for a 10ms delay (assuming a 144 MHz clock)
    systick->CNT = 0; // Clear the counter
    systick->SR = 0; // Clear the status register
    systick->ctlr.init = 1; // Initialize the timer when enabled
    systick->ctlr.stclk = 1; // Use the processor clock as the source (144MHz)
    systick->ctlr.stie = 1; // Enable interrupts
    systick->ctlr.mode = 0; // Count UP to CMP value
    systick->ctlr.stre = 1; // Enable reload of the timer when it reaches the CMP value
    systick->ctlr.ste = 0; // Enable the timer
    #endif
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 2.4: If your code ends up in here, you are almost done!
//                 Some time has passed and you should activate another mole.
//                 (by writing 1 to one of the elements in the "moles" array).
//                 You can use the rand() function (from stdlib.h) to pick one
//                 randomly.)                
// 
//                 Don't forget to reset the systick status register. Otherwise
//                 you will not get another interrupt. 
///////////////////////////////////////////////////////////////////////////////
__attribute__((interrupt("machine"))) // This attribute is required so the 
                                      // compiler knows it should save any 
                                      // registers it uses, and use "mret" to
                                      // return. 
void SysTick_Handler(void) 
{
    #if SOLUTION
    int random_mole = rand() % 4;
    moles[random_mole] = 1; // Activate a random mole
    if(systick->SR == 1) systick->SR = 0; // Clear the status
    #endif
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4: You're done, really, but why not make the game more 
    //               challenging by decreasing the time between moles for every
    //               mole you whack? 
    ///////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3: If your game works, implement InitInterrupts() in C and you
//               can get rid of the assembly file.
///////////////////////////////////////////////////////////////////////////////
#if SOLUTION
// void InitInterrupts()
// {
//     // Write address of SysTick_Handler to mtvec
//     __asm volatile("csrw mtvec, %0" : : "r"((uint32_t)&SysTick_Handler));
    
//     // Enable SysTick interrupts in PFIC
//     *((volatile uint32_t *)0xE000E100) |= (1 << 12); // Set the bit corresponding to SysTick interrupt 
// }
#endif

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

    ///////////////////////////////////////////////////////////////////////////
    // This function should initialize interrupts. 
    // It needs to be implemented in the assembly file, "assignment.s".
    ///////////////////////////////////////////////////////////////////////////
    InitInterrupts();
    check_assignment2_1(); 
    InitSystick(); 
    check_assignment2_2();

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2.3: Now start the timer. Put a breakpoint in SysTick_Handler
    //                 and make sure it is called. 
    ///////////////////////////////////////////////////////////////////////////
#if SOLUTION
    systick->ctlr.ste = 1; // Enable the timer
#endif  


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
                // Wait until the button is released, to avoid multiple counts 
                // for one press.
                while(((*GPIOD_INDR) & (1 << i)) == 0);
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
        if(moles[0] & moles[1] & moles[2] & moles[3]) 
            game_over = 1;
    }
    printf("Game Over! Your score is %d\n", score);
    ///////////////////////////////////////////////////////////////////////////
    // Blink lights to indicate game over.
    ///////////////////////////////////////////////////////////////////////////
    *GPIOE_OUTDR = 0x0; 
    while(1) {
        *GPIOE_OUTDR ^= 0xFF; 
        for(int i = 0; i < 1000; i++); // Delay
    }
    return 0;
}