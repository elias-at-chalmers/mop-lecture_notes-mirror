#include <stdio.h>
#include <stdint.h>
#include "helpers.h"

#define INTERRUPTS_ENABLED 1

///////////////////////////////////////////////////////////////////////////////
// 1. Declare a structure for the GPIO ports and define GPIO_D and GPIO_E
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint32_t CFGLR; 
    uint32_t CFGHR;
    uint16_t INDR;
    uint16_t reserved0; 
    uint16_t OUTDR;
    uint16_t reserved1;
    uint32_t BSHR;
    uint16_t BCR; 
    uint16_t reserved2;
    uint32_t LCKR;
} GPIO_t;
volatile GPIO_t* GPIO_D = (GPIO_t*) 0x40011400;
volatile GPIO_t* GPIO_E = (GPIO_t*) 0x40011800;

///////////////////////////////////////////////////////////////////////////////
// 2. Write a function that configures a GPIO pin
///////////////////////////////////////////////////////////////////////////////
void configure_gpio_pin(volatile GPIO_t* gpio, uint8_t pin, uint8_t CNF, uint8_t MODE)
{
    volatile uint32_t * config_register = (pin < 8) ? &gpio->CFGLR : &gpio->CFGHR;
    uint8_t shift = (pin % 8) * 4; // Each pin configuration takes 4 bits
    uint32_t config_value = (CNF << 2) | MODE; // Combine CNF and MODE into a single value
    *config_register &= ~(0xF << shift); // Clear the existing configuration for the pin
    *config_register |= (config_value << shift); // Set the new configuration for the pin
}

///////////////////////////////////////////////////////////////////////////////
// 4. Declare a structure for the SysTick Timer
///////////////////////////////////////////////////////////////////////////////
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
        };
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

SysTick_t * systick = (SysTick_t*) 0xE000F000;

#if INTERRUPTS_ENABLED
__attribute__((interrupt("machine")))
void SysTick_Handler(void) 
{
    GPIO_D->OUTDR ^= 0b1; // Toggle PD0
    systick->SR = 0; // Clear the status
}

#define PFIC_BASE       0xE000E000
#define PFIC_IENR1     ((volatile uint32_t *)(PFIC_BASE + 0x100))

#endif

void beep(void)
{
#if INTERRUPTS_ENABLED
    __asm volatile("csrw mtvec, %0" : : "r" (SysTick_Handler));

    // Enable Systick Interrupt in PFIC
    *PFIC_IENR1 |= (1 << 12);
#endif   

    ///////////////////////////////////////////////////////////////////////////
    // 3. Configure PD0 as an output pin, push-pull, 2 MHz
    ///////////////////////////////////////////////////////////////////////////
    configure_gpio_pin(GPIO_D, 0, 0b00, 0b10);

    ///////////////////////////////////////////////////////////////////////////
    // 5. Initialize the SysTick timer to continously count to 2ms
    ///////////////////////////////////////////////////////////////////////////
    systick->CMP = 147000*2; // Set the compare value for a 100ms delay (assuming a 14.7 MHz clock)
    systick->CTLR = 0; // Disable systick and clear configuration. 
    systick->SR = 0; // Clear the status register
    systick->init = 1; // Initialize the timer when enabled
    systick->mode = 0; // Count UP to CMP value
    systick->stre = 1; // Enable reload of the timer when it reaches the CMP value
    systick->stclk = 1; // Use the processor clock as the source (144MHz)
    systick->stie = 0; // No interrupts (yet)

#if INTERRUPTS_ENABLED
    systick->stie = 1; // Enable interrupts
    systick->ste = 1; // Enable the timer
    return; 
#else    

    systick->ste = 1; // Enable the timer
    
    ///////////////////////////////////////////////////////////////////////////
    // 6. Flip PD0 every time the timer reaches the CMP value (every 2ms)
    ///////////////////////////////////////////////////////////////////////////
    while(1)
    {
        if (systick->SR != 0) {
            GPIO_D->OUTDR ^= 0b1; // Toggle PD0
            systick->SR = 0; // Clear the status
        }
    }
#endif
}


void show_temperature()
{
    ///////////////////////////////////////////////////////////////////////////
    // 7. Configure PE8-15 as output, push-pull, 2 MHz
    ///////////////////////////////////////////////////////////////////////////
    for(uint32_t pin = 8; pin < 16; pin++) {
        configure_gpio_pin(GPIO_E, pin, 0b00, 0b10);
    }   

    ///////////////////////////////////////////////////////////////////////////
    // 8. Continously read the temperature from the sensor and display on the 
    //    7-segment display connected to PE8-15.
    ///////////////////////////////////////////////////////////////////////////
    while(1)
    {
        uint8_t t = GetTemperature(); // Assume this function reads the temperature from the sensor
        uint8_t seg_code = Get7SegCode(t);
        GPIO_E->OUTDR = (seg_code << 8); // Output the segment code to PE8-15
    }
}

int main()
{
    beep();
    show_temperature();
    return 0;
}   