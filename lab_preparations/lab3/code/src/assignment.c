///////////////////////////////////////////////////////////////////////////////
// Lab Preparation 3
// ============================================================================
// In Lab 3 you will need to understand how internal and external interrupts
// work.
//
// In this lab preparation, you will use (internal) interrupts from systick to
// output a square wave on GPIO Port E pin 0, by writing values 0, 1, 0, 1, ...
//
// Moreover, you will use (external) interrupts from the keypad to handle
// key presses from the user. Remember, in lab 2, you wrote a program that
// continously polled the keypad for key presses. Using interrupts, we
// can instead have the CPU only handle the keypad when a key is actually
// pressed, and let it do other things the rest of the time.
//
// In the simulator IO Setup, connect
// Parallel port (GPIO E pin 0-7) -> 01 8 segment bargraph   AND
// Parallel port (GPIO D pin 0-7) -> 12 keypad AND
// Serial Communications Interface 1 -> 06 Console
///////////////////////////////////////////////////////////////////////////////

#include "assignment.h"
#include "keypad.h"
#include <stdint.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Assignment 1.0: Open the file "assignment.h" Complete the register macros
// at the end of the file (marked "TODO") so that you can use them in the next
// assignments.
//
// NOTE: in "assignment.h" you will also find useful macros for the GPIO and 
// Systick registers that you may use if you wish
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Assignment 1.1: Configure the GPIO pins as follows:
//               * GPIO Port D pins 0-3 (keypad columns): digital input, pull-up
//                 Enable the pull-ups by writing 1s to the corresponding bits
//                 in GPIOD_OUTDR.
//               * GPIO Port D pins 4-7 (keypad rows): digital output, open
//                 drain, 2 MHz.
//               * GPIO Port E pin 0 (bargraph): digital output, push-pull, 
//                 2 MHz.
//               * GPIO Port D pins 8-15 (bargraph): digital output,
//                 push-pull, 2 MHz. Turn all LEDs off.
//
//               Finally, the output pins values should be set as follows:
//               * Keypad row pins = 0 (to activate them.)
//               * Bargraph pins = 0 (all LEDs off)
///////////////////////////////////////////////////////////////////////////////

void init_gpio() {
    // GPIOD [0:7] keypad
    // - cols [0:3] (input, pull-up)
    // - rows [4:7] (output, open drain, 2 MHz)
    *GPIOD_CFGLR = 0x66668888;

    // Enable pull-ups on columns (pins 0-3)
    *GPIOD_OUTDR |= (0xF << 0);

    // Activate all rows (pins 4-7): set to LOW
    *GPIOD_OUTDR &= ~(0xF << 4);

    // GPIOE [0] bargraph (output, push/pull, 2 MHz)
    *GPIOE_CFGLR &= ~(0xF << 0);
    *GPIOE_CFGLR |= (0x2 << 0);

    // GPIOD [8:15] bargraph (output, push/pull, 2 MHz)
    *GPIOD_CFGHR = 0x22222222;

    // turn off port D pins 8-15 (LEDs)
    *GPIOD_OUTDR &= ~(0xFF << 8);

    // turn off port E pin 0 (LED)
    *GPIOE_OUTDR &= ~0x1;
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 1.2: Setup the systick timer to generate an interrupt at a rate
//                 that is determined by the input argument "period_in_us".
///////////////////////////////////////////////////////////////////////////////

void systick_periodic_micro(unsigned int period_in_us) {
#if SOLUTION
    // Set compare value. 
    // HINT: Processor clock is 144 MHz => 144 000 000 ticks per second
    *STK_CMP = 144 * period_in_us - 1;

    // Reset counter to 0
    *STK_CNT = 0;

    // Enable timer, enable interrupt, enable reload, use processor clock
    *STK_CTLR = STK_CTLR_STE | STK_CTLR_STIE | STK_CTLR_STCLK | STK_CTLR_STRE;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 1.3: This is the interrupt handler for the systick timer. It
//                 should only do two things: toggle the output value on GPIO
//                 Port E pin 0, and set the Systick status register to 0 to
//                 acknowledge the interrupt.
///////////////////////////////////////////////////////////////////////////////

__attribute__((interrupt("machine"))) void systick_handler(void) {
#if SOLUTION
    // Flip GPIO E0 pin
    *GPIOE_OUTDR ^= 0x1;
    
    // Reset the status, so we will get a new interrupt
    *STK_SR = 0;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 1.4: Open the file "interrupts.s", go to line 19, and write the
//                 the missing assembly code to jump to the function
//                 systick_handler
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Assignment 1.5: This function should stop the square wave by disabling
//                 the systick timer.
///////////////////////////////////////////////////////////////////////////////
void systick_stop() {
#if SOLUTION
    // Disable the systick timer and its interrupt
    *STK_CTLR = 0;
#endif
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 2.0: In this function you will configure the interrupts by:
//               * Enabling the systick interrupt in the PFIC_IENR1 register
//               * Enabling interrupts for the input pins (Port D pins 0-3),
//                 so that an interrupt is triggered when a key is pressed
//                 (falling edge) or released (rising edge.)
//                 HINT: use the PFIC_IENR1, AFIO_EXTICR1, EXTI_INTENR,
//                 EXTI_RTENR, and EXTI_FTENR registers for this.
///////////////////////////////////////////////////////////////////////////////

void init_interrupts() {
#if SOLUTION
    // Enable interrupts for Systick
    *PFIC_IENR1 |= (1 << 12);

    // Enable external interrupts for pins 0-3
    *PFIC_IENR1 |= (1 << 22) | (1 << 23) | (1 << 24) | (1 << 25);

    // Clear EXTI0-3 configuration
    *AFIO_EXTICR1 &= ~(uint32_t)0xFFFF;
    // Set EXTI0-3 to Port D
    *AFIO_EXTICR1 |= (uint32_t)0x3333;

    // Enable interrupts for EXTI0-3
    *EXTI_INTENR |= 0xF;
    // Trigger on rising edge
    *EXTI_RTENR |= 0xF;
    // Trigger on falling edge
    *EXTI_FTENR |= 0xF;
#endif
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 2.1: exti_handler() is the interrupt handler for interrupts
//                 triggered by the keypad. Implement the function so that it:
//                 * reads the value of the currently pressed key, and stores
//                   it in a variable "key".
//                   HINT: there is already a function for reading the keypad
//                   declared in keypad.h.
//                 * clears the interrupt pending flags for all the keypad pins
///////////////////////////////////////////////////////////////////////////////

__attribute__((interrupt("machine"))) void exti_handler() {
#if SOLUTION
    // Read the currently pressed key and store it in a variable
    volatile unsigned char key = keyb();

    // Set rows (pins 4-7) to LOW to activate them
    *GPIOD_OUTDR &= ~0x00F0; 

    // Acknowledge pending EXTI flags on keypad lines
    *EXTI_INTFR |= 0xF;
#endif
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 2.2: Open the file "vector_table.s", go to lines 22-25, and write
//                 the missing assembly code to jump to the function
//                 exti_handler
//                 Note: since we have 4 pins for the keypad, we're dealing
//                 with 4 different external interrupts (EXTI0-3), but we use
//                 the same handler for all of them, since they are all
//                 triggered by the same event (a key press).
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Final tests: Did all the automated tests pass? Great, time for you to test
// the interrupts yourself: 
//     * Connect the bargraph and the keypad as described in the beginning
//       of this file. 
//     * Run the program.
//     * After all tests are done, bit 0 of the bargraph should be blinking!
//     * Put a breakpoint at the end of exti_handler() 
//     * Press keys on the keypad, and verify that the correct key value is
//       stored in the variable you created.
///////////////////////////////////////////////////////////////////////////////
