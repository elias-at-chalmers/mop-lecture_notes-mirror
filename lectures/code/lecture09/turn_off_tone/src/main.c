#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helpers.h"

volatile GPIO_t* GPIO_D = (GPIO_t*) 0x40011400;
volatile GPIO_t* GPIO_E = (GPIO_t*) 0x40011800;
volatile SysTick_t * systick = (SysTick_t*) 0xE000F000;

#define EXTI_INTENR ((volatile uint32_t*) 0x40010400)
#define EXTI_EVENR ((volatile uint32_t*) 0x40010404)
#define EXTI_RTENR ((volatile uint32_t*) 0x40010408)
#define EXTI_FTENR ((volatile uint32_t*) 0x4001040C)
#define EXTI_SWIEVR ((volatile uint32_t*) 0x40010410)
#define EXTI_INTFR ((volatile uint32_t*) 0x40010414)

#define AFIO_EXTICR1 ((volatile uint32_t*) 0x40010008)
#define AFIO_EXTICR2 ((volatile uint32_t*) 0x4001000C)
#define AFIO_EXTICR3 ((volatile uint32_t*) 0x40010010)
#define AFIO_EXTICR4 ((volatile uint32_t*) 0x40010014)

volatile int play_sound = 1;

extern void init_interrupts(); 

__attribute__((interrupt("machine")))
void EXTI1_Handler(void) 
{
    play_sound = !play_sound; // Toggle the sound on/off state

    for(int i = 0; i < 100000; i++); // Simple debounce delay

    *EXTI_INTFR |= 0b10;  // Acknowledge the interrupt (zeroes the corresponding bit in EXTI_INTFR)
}

int main(void)
{
    ////////////////////////////////////////////////////////////////////////////
    // Assignment: Stop the annoying sound. 
    ////////////////////////////////////////////////////////////////////////////
    init_interrupts();

    // Configure PE1 as an input pin with pull-up resistor
    GPIO_E->CFGLR = 0x22222222; // Set PE0-7 as output, push-pull, 2 MHz
    GPIO_E->OUTDR = 0xFF; // Enable pull-up resistors on PE0-7


    // We need pin 1 of the GPIOE port to be connected to EXTI
    *AFIO_EXTICR1 &= ~(0xF << 4); // Clear the EXTI1 bits
    *AFIO_EXTICR1 |= (0x4 << 4); // Set EXTI1 to be connected to GPIOE pin 1

    // Configure EXTI1 to trigger on the falling edge (when the button is released)
    *EXTI_FTENR |= 0b10; // Enable falling edge trigger for EXTI1
    *EXTI_RTENR &= ~0b10; // Disable rising edge trigger for EXTI1

    // Enable the EXTI1 interrupt
    *EXTI_INTENR |= 0b10; // Enable EXTI1 interrupt

    // Enable PFIC interrupt for EXTI1 (IRQ number 23)
    *((volatile uint32_t*) 0xE000E100) |= (1 << 23);    
    

    // Program below creates an annoying sound

    ///////////////////////////////////////////////////////////////////////////
    // Configure PD0 as an output pin, push-pull, 2 MHz
    ///////////////////////////////////////////////////////////////////////////
    GPIO_D->CFGLR = 0x22222222; // Set PD0-7 as output, push-pull, 2 MHz
    
    ///////////////////////////////////////////////////////////////////////////
    // Flip PD0 every time the timer reaches the CMP value (every 2ms)
    ///////////////////////////////////////////////////////////////////////////
    volatile uint32_t period = 144000*2; 
    while(1)
    {
        ///////////////////////////////////////////////////////////////////////////
        // 5. Initialize the SysTick timer to count to 2ms
        ///////////////////////////////////////////////////////////////////////////
        systick->CTLR = 0; // Disable systick and clear configuration. 
        systick->CNT = 0; // Clear the current count
        systick->CMP = period; // Set the compare value for a 2ms delay (assuming a 144 MHz clock)
        systick->SR = 0; // Clear the status register
        systick->CTLR = 0b100101; 
        while (systick->SR == 0);
        GPIO_D->OUTDR ^= 0b1; // Toggle PD0
        if(myrand() % 100 == 0) { period += 1000; }

        while(!play_sound); 
    }
}
