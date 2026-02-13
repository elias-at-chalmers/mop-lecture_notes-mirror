#include <stdio.h>
#include <stdint.h>

void assignment_1(void);
void gpio_d_set_pin_input( int pin, int mode);
void gpio_d_set_pin_output(int pin, int mode);
void gpio_d_set_pin_high(int pin);
void gpio_d_set_pin_low(int pin);

int get_keyboard_button(void);

#define GPIO_D_BASE 0x40011400
#define GPIO_D_CFGLR ((volatile uint32_t *)(GPIO_D_BASE + 0x00))
#define GPIO_D_CFGHR ((volatile uint32_t *)(GPIO_D_BASE + 0x04))
#define GPIO_D_INDR  ((volatile uint32_t *)(GPIO_D_BASE + 0x08))
#define GPIO_D_OUTDR ((volatile uint16_t *)(GPIO_D_BASE + 0x0C))
#define GPIO_D_BSHR  ((volatile uint32_t *)(GPIO_D_BASE + 0x10))
#define GPIO_D_BCR   ((volatile uint16_t *)(GPIO_D_BASE + 0x14))

uint32_t seed = 0x4756ABCD;
uint32_t simplerand() {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return seed;    
}

int main(void)
{   
    ///////////////////////////////////////////////////////////////////////////
    // Test assignment 1
    ///////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 1...");
    fflush(stdout);

    // Set OUTDR so everything is pull up before we test
    *GPIO_D_OUTDR = 0xFFFF;
    // Set CFG to weird values so we can check that other pins
    // have not been tampered with
    *GPIO_D_CFGLR = 0x12345678;
    *GPIO_D_CFGHR = 0x9ABCDEF0;

    assignment_1(); 

    // Test that nothing is changed EXCEPT for pin 11
    if((*GPIO_D_CFGLR) != 0x12345678) {
        printf("FAILED (Why are you changing in CFGLR?)\n");
        return 1;
    }   
    if((*GPIO_D_CFGHR & ~(0xF << (3 * 4))) != (0x9ABCDEF0 & ~(0xF << (3 * 4)))) {
        printf("FAILED (You have modified pins that are not for pin 11!)\n");
        return 1;
    }
    if((*GPIO_D_CFGHR & (0xF << (3 * 4))) != (0x8 << (3 * 4))) {
        printf("FAILED (pin 11 not configured correctly)\n");
        return 1;
    }
    // Make sure OUTDR bit 11 is 0 (pull-down)
    if((*GPIO_D_OUTDR & (1 << 11)) != 0) {
        printf("FAILED (pin 11 configured for pull-up instead of pull-down)\n");
        return 1;
    }
    printf("PASSED\n");

    ///////////////////////////////////////////////////////////////////////////
    // Test assignment 2
    ///////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 2...");
    fflush(stdout);

    for(int i=0; i<10; i++) {
        int pin = simplerand() % 16;
        int mode = simplerand();
        mode = mode % 3; // 0 = pull-down, 1 = pull-up, 2 = floating

        // Reset registers to known values
        uint32_t orig_outdr = simplerand() & 0xFFFF;
        uint32_t orig_cfglr = simplerand();
        uint32_t orig_cfghr = simplerand();
        *GPIO_D_OUTDR = orig_outdr;
        *GPIO_D_CFGLR = orig_cfglr;
        *GPIO_D_CFGHR = orig_cfghr;

        gpio_d_set_pin_input(pin, mode);

        // Check that only the relevant bits were changed
        uint32_t expected_cnfmode;
        if(mode == 2) expected_cnfmode = 0x4; // floating
        else expected_cnfmode = 0x8; // pull-up/down

        uint32_t reg = (pin < 8) ? *GPIO_D_CFGLR : *GPIO_D_CFGHR;
        int idx = pin % 8;
        if((reg & (0xF << (idx * 4))) != (expected_cnfmode << (idx * 4))) {
            printf("FAILED (pin %d not configured correctly)\n", pin);
            return 1;
        }

        uint32_t masked_reg = reg & ~(0xF << (idx * 4));
        uint32_t masked_orig_cfglr = orig_cfglr & ~(0xF << (idx * 4));
        uint32_t masked_orig_cfghr = orig_cfghr & ~(0xF << (idx * 4));
        if(masked_reg != (pin < 8 ? masked_orig_cfglr : masked_orig_cfghr)) {
            printf("FAILED (configuring pin %d, but other pins are also modified!)\n", pin);
            return 1;
        }

        // Check OUTDR for pull-up/down
        if(mode == 1) { // pull-up
            if((*GPIO_D_OUTDR & (1 << pin)) == 0) {
                printf("FAILED (pin %d not set for pull-up)\n", pin);
                return 1;
            }
        } else if(mode == 0) { // pull-down
            if((*GPIO_D_OUTDR & (1 << pin)) != 0) {
                printf("FAILED (pin %d not set for pull-down)\n", pin);
                return 1;
            }
        }
        printf(".");
        fflush(stdout);
    }
    printf("PASSED\n");


    ///////////////////////////////////////////////////////////////////////////
    // Test assignment 3
    ///////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 3...");
    fflush(stdout);
    for(int i=0; i<10; i++) {
        int pin = simplerand() % 16;
        int mode = simplerand() % 2; // 0 = pushpull, 1 = open-drain

        // Reset registers to known values
        uint32_t orig_outdr = simplerand() & 0xFFFF;
        uint32_t orig_cfglr = simplerand();
        uint32_t orig_cfghr = simplerand();
        *GPIO_D_OUTDR = orig_outdr;
        *GPIO_D_CFGLR = orig_cfglr;
        *GPIO_D_CFGHR = orig_cfghr;
        gpio_d_set_pin_output(pin, mode);
        // Check that only the relevant bits were changed
        uint32_t expected_cnfmode;
        if(mode == 1) expected_cnfmode = 0x6; // open-drain
        else expected_cnfmode = 0x2; // push-pull
        uint32_t reg = (pin < 8) ? *GPIO_D_CFGLR : *GPIO_D_CFGHR;

        int idx = pin % 8;
        if((reg & (0xF << (idx * 4))) != (expected_cnfmode << (idx * 4))) {
            printf("FAILED (pin %d not configured correctly)\n", pin);
            return 1;
        }

        uint32_t masked_reg = reg & ~(0xF << (idx * 4));
        uint32_t masked_orig_cfglr = orig_cfglr & ~(0xF << (idx * 4));
        uint32_t masked_orig_cfghr = orig_cfghr & ~(0xF << (idx * 4));
        if(masked_reg != (pin < 8 ? masked_orig_cfglr : masked_orig_cfghr)) {
            printf("FAILED (configuring pin %d, but other pins are also modified!)\n", pin);
            return 1;
        }

        printf(".");
        fflush(stdout);
    }
    printf("PASSED\n");


    ///////////////////////////////////////////////////////////////////////////
    // Test assignment 4
    ///////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 4...");
    fflush(stdout);
    for(int i=0; i<10; i++) {
        int pin = simplerand() % 16;
        // Reset OUTDR to known value
        uint32_t orig_outdr = simplerand() & 0xFFFF;
        *GPIO_D_OUTDR = orig_outdr;
        gpio_d_set_pin_high(pin);
        if((*GPIO_D_OUTDR & (1 << pin)) == 0) {
            printf("FAILED (pin %d not set high)\n", pin);
            return 1;
        }
        // Check that other bits were not changed
        if(((*GPIO_D_OUTDR & ~(1 << pin)) != (orig_outdr & ~(1 << pin)))) {
                printf("FAILED (pin %d modified other pins!)\n", pin);
                return 1;
            }
        printf(".");
        fflush(stdout);
    }
    printf("PASSED\n");

    ///////////////////////////////////////////////////////////////////////////
    // Test Assignment 5
    ///////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 5...");
    fflush(stdout);
    for(int i=0; i<10; i++) {
        int pin = simplerand() % 16;
        // Reset OUTDR to known value
        uint32_t orig_outdr = simplerand() & 0xFFFF;
        *GPIO_D_OUTDR = orig_outdr;
        gpio_d_set_pin_low(pin);
        if((*GPIO_D_OUTDR & (1 << pin)) != 0) {
            printf("FAILED (pin %d not set low)\n", pin);
            return 1;
        }
        // Check that other bits were not changed
        if(((*GPIO_D_OUTDR & ~(1 << pin)) != (orig_outdr & ~(1 << pin)))) {
                    printf("FAILED (pin %d modified other pins!)\n", pin);
                    return 1;
                }       
        printf(".");
        fflush(stdout);
    }
    printf("PASSED\n");

    ///////////////////////////////////////////////////////////////////////////
    // Test Assignment 6
    ///////////////////////////////////////////////////////////////////////////

    printf("You made it to the final task!\n");

    printf("I will print the button number when you press a button.\n");

    printf("Verify that they correspond to the correct keys on your keyboard.\n");

    printf("If you see phantom buttons being pressed, try to add a dely after you activate a row in get_keyboard_button().\n");

    while(1) {
        int button = get_keyboard_button();
        if(button >= 0) {
            printf("Button pressed: %d\n", button);
        }
    }
    return 0;
}
