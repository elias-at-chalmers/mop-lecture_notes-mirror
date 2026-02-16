#include <stdio.h>
#include <stdint.h>

#define STK_CTLR    ((volatile uint32_t*) 0xE000F000)
#define STK_SR      ((volatile uint32_t*) 0xE000F004)
#define STK_CNTL    ((volatile uint32_t*) 0xE000F008)
#define STK_CNTH    ((volatile uint32_t*) 0xE000F00C)
#define STK_CMPL    ((volatile uint32_t*) 0xE000F010)
#define STK_CMPH    ((volatile uint32_t*) 0xE000F014)

#define GPIOD_CFGLR ((volatile uint32_t*) 0x40011400)
#define GPIOD_CFGHR ((volatile uint32_t*) 0x40011404)
#define GPIOD_INDR   ((volatile uint32_t*) 0x40011408)
#define GPIOD_OUTDR  ((volatile uint32_t*) 0x4001140C)

#define PFIC_BASE       0xE000E000
#define PFIC_IENR1     ((volatile uint32_t *)(PFIC_BASE + 0x100))

__attribute__((interrupt("machine")))
void SysTick_Handler(void)
{
    *STK_SR = 0;        // Reset the status, so we will get a new interrupt in 2ms
    *GPIOD_OUTDR ^= 0x1;    // Flip GPIO D0 pin
}

int main(void)
{
    // Enable Systick Interrupt in PFIC

    *PFIC_IENR1 |= (1 << 12);

    __asm volatile ("csrw mtvec, %0" :: "r"(SysTick_Handler));
    *GPIOD_CFGLR = 0x00000002; // PD0 as output, push-pull, 2Mhz

    *STK_CMPH = 0; 
    *STK_CMPL = 147000*2;
    *STK_SR = 0;
    *STK_CTLR = 0b101111;


    
    while(1)
    {
        // if (*STK_SR & 0b1) {
        //     *GPIOD_OUTDR ^= 0b1; // Toggle PD0
        //     *STK_SR = 0; // Clear the interrupt flag
        // }
    }

}

