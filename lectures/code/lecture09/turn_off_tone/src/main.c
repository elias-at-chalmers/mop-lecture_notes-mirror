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

#define PFIC_IENR1 ((volatile uint32_t*) 0xE000E100)

volatile int play_sound = 1;

__attribute__((interrupt("machine")))
void EXTI1_Handler(void)
{
    play_sound = !play_sound; 
    *EXTI_INTFR |= 0b10;
}

int main(void)
{
    ////////////////////////////////////////////////////////////////////////////
    // Assignment: Stop the annoying sound. 
    ////////////////////////////////////////////////////////////////////////////

    GPIO_E->CFGLR = 0x88888888; // Set PE0-7 as input, pull up/down
    GPIO_E->OUTDR = 0xFFFF; // Pull up

    // Välj att routa Port E vidare på pinne 1
    *AFIO_EXTICR1 = 0x40;

    *EXTI_RTENR = 0b10;
    *EXTI_FTENR = 0b00;
    *EXTI_INTENR = 0b10;

    *PFIC_IENR1 |= (1<<23);


    




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
