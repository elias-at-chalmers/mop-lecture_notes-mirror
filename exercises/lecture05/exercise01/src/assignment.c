
#include <stdint.h>

#define SOLUTION 1
///////////////////////////////////////////////////////////////////////////////
// Assignment 1
// ===========================================================================
// Write a function that configures pin 11 of GPIO port D as input with 
// pull-down resistor enabled.
// 
// ERIK: The test harness should set ODATA to 0xFFFF so it is pull up unless
//       they configure it. And give the hint, of course. 
//       It should also check that no other bits were changed. 
///////////////////////////////////////////////////////////////////////////////



// TODO: Macro definitions here
#if SOLUTION
#define GPIO_D_BASE 0x40011400
#define GPIO_D_CFGLR ((volatile uint32_t *)(GPIO_D_BASE + 0x00))
#define GPIO_D_CFGHR ((volatile uint32_t *)(GPIO_D_BASE + 0x04))
#define GPIO_D_INDR  ((volatile uint16_t *)(GPIO_D_BASE + 0x08))
#define GPIO_D_OUTDR ((volatile uint16_t *)(GPIO_D_BASE + 0x0C))
#define GPIO_D_BSHR  ((volatile uint32_t *)(GPIO_D_BASE + 0x10))
#define GPIO_D_BCR   ((volatile uint16_t *)(GPIO_D_BASE + 0x14))
#else
#define GPIO_D_BASE  /* TODO */
#define GPIO_D_CFGLR /* TODO */
#define GPIO_D_CFGHR /* TODO */
#define GPIO_D_INDR  /* TODO */
#define GPIO_D_OUTDR /* TODO */
#define GPIO_D_BSHR  /* TODO */
#define GPIO_D_BCR   /* TODO */
#endif

void assignment_1(void)
{
#if SOLUTION
    // Clear the 4 bits for pin 11
    *GPIO_D_CFGHR &= ~(0xF << (5 * 4));
    // Set the bits for pin 11 to input with pull-up/down
    *GPIO_D_CFGHR |= (0x8 << (5 * 4));
    // Set GPIO_D_OUTDR bit 11 to 0 for pull-down
    *GPIO_D_OUTDR &= ~(1 << 11);
#else
    // Your code here
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 2
// ===========================================================================
// Write a more general function that configures a pin of GPIO port D as
// input with pull up or down or floating.
// 
// ERIK: Test harness will make sure they only modified the relevant bits
//       and set them correctly. Need to check OUTDR as well for pull-up/down.
//       
//       Oh... for the keyboard, they really only need 8 pins... 
///////////////////////////////////////////////////////////////////////////////

void gpio_d_set_pin_input( 
    int pin,    // The pin to configure (0-15)
    int mode    // 0 = PULLDOWN, 1 = PULLUP, 2 = FLOATING 
    )
{
#if SOLUTION
    // ERIK: We will accept much less pretty solutions here.
    // Set 4 bit CNF and MODE bitmask
    uint32_t cnfmode = 0x0; // mode = 00 = input
    if(mode == 2) cnfmode |= 0x4; // floating
    else cnfmode |= 0x8; // pull-up/pull-down
    // Set the correct bits in CFGLR or CFGHR
    int idx = pin % 8;
    if(pin < 8) {
        *GPIO_D_CFGLR &= ~(0xF << (idx * 4));
        *GPIO_D_CFGLR |= (cnfmode << (idx * 4));
    }
    else{
        *GPIO_D_CFGHR &= ~(0xF << (idx * 4));
        *GPIO_D_CFGHR |= (cnfmode << (idx * 4));
    }
    // Set pull-up/pull-down in OUTDR
    if(mode == 0) *GPIO_D_OUTDR &= ~(1 << pin); // pull-down
    else if(mode == 1) *GPIO_D_OUTDR |= (1 << pin);  // pull-up
#else 
    // Your code here
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3
// ===========================================================================
// Write a function that configures a pin of GPIO port D as
// output with either push-pull or open-drain mode.
// The pin should be configured as output at 2 MHz.
///////////////////////////////////////////////////////////////////////////////
void gpio_d_set_pin_output( 
    int pin,    // The pin to configure
    int mode    // 0 = PUSHPULL, 1 = OPENDRAIN
    )
{
#if SOLUTION
    uint32_t cnfmode = 0x2; // mode = 10 = output at 2 MHz
    if(mode == 1) cnfmode |= 0x4; // open-drain
    else cnfmode |= 0x0; // push-pull
    // Set the correct bits in CFGLR or CFGHR
    int idx = pin % 8;
    if(pin < 8) {
        *GPIO_D_CFGLR &= ~(0xF << (idx * 4));
        *GPIO_D_CFGLR |= (cnfmode << (idx * 4));
    }
    else{
        *GPIO_D_CFGHR &= ~(0xF << (idx * 4));
        *GPIO_D_CFGHR |= (cnfmode << (idx * 4));
    }
#else
    // Your code here
#endif    
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 4
// ===========================================================================
// Write a function that sets one pin of GPIO port D high.
// The function should use the read-modify-write registers (BSHR or BCR)
///////////////////////////////////////////////////////////////////////////////
void gpio_d_set_pin_high(int pin)
{
#if SOLUTION
    *GPIO_D_BSHR = 1 << pin;     
#else
    // Your code here   
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 5
// ===========================================================================
// Write a function that sets one pin of GPIO port D low.
// The function should use the read-modify-write registers (BSHR or BCR)
void gpio_d_set_pin_low(int pin)
{   
#if SOLUTION
    *GPIO_D_BCR = 1 << pin; 
#else
    // Your code here   
#endif
}    

///////////////////////////////////////////////////////////////////////////////
// Assignment 6
// ===========================================================================
// Assume that the keyboard is connected to GPIO port D pins 0-7
// Write a function that returns the number of the button on the keyboard: 
//
// /-------------------\
// |  0 |  1 |  2 |  3 |
// |----|----|----|----|    
// |  4 |  5 |  6 |  7 |
// |----|----|----|----|
// |  8 |  9 | 10 | 11 |
// |----|----|----|----|
// | 12 | 13 | 14 | 15 |
// \-------------------/
// 
// Follow the instructions in comments
// You can the functions you have written above
///////////////////////////////////////////////////////////////////////////////
int get_keyboard_button(void)
{
#if SOLUTION
    // Configure the row-selection pins (pins 4-7) as output, open drain, at 2 MHz
    for(int pin = 4; pin <= 7; pin++) gpio_d_set_pin_output(pin, 1); // open-drain

    // Configure the row-read pins (pins 0-3) as input with pull-up resistors
    for(int pin = 0; pin <= 3; pin++) gpio_d_set_pin_input(pin, 1); // pull-up

    // Deactivate all rows by setting pins the row selection pins  4-7 high
    // (remember that they are active low, so 0 selects a row)
    for(int pin = 4; pin <= 7; pin++) gpio_d_set_pin_high(pin);

    for(int row = 0; row < 4; row++) {
        // Activate the current row by setting the corresponding pin low
        gpio_d_set_pin_low(4 + row);
        // Read the row (buttons 0-3, 4-7, 8-11, or 12-15) as the lower 4 bits of INDR

        // Check if any button in this row is pressed (low)
        uint8_t row_data = *GPIO_D_INDR & 0x0F;
        if(row_data != 0xF) {
            // Find the first bit that is low (button pressed)
            // and return the number of the button
            for(int col = 0; col < 4; col++) {
                if((row_data & (1 << col)) == 0) {
                    return row * 4 + col;
                }
            }
        }
        // Deactivate the current row by setting the corresponding pin high
        gpio_d_set_pin_high(4 + row);            
    }
    return -1; // No button pressed
#else
    // Configure the row-selection pins (pins 4-7) as output, open drain, at 2 MHz

    // Configure the row-read pins (pins 0-3) as input with pull-up resistors

    // Deactivate all rows by setting pins the row selection pins  4-7 high
    // (remember that they are active low, so 0 selects a row)

    for(int row = 0; row < 4; row++) {
        // Activate the current row by setting the corresponding pin low

        // Read the row (buttons 0-3, 4-7, 8-11, or 12-15) as the lower 4 bits of INDR

        // Check if any button in this row is pressed (low)

        //   and, if so, return the number of the button

        // Deactivate the current row by setting the corresponding pin high
    }
    return -1; // No button pressed    return -1;  
#endif
}



