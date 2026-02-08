#include <stdio.h>
#include <stdint.h>

extern uint64_t ns_to_ticks(uint64_t ns);
extern void delay(uint64_t ns);
extern void delay_us(uint32_t ns);
extern void delay_ms(uint32_t ms);

int abs(int x) {
    if(x < 0) return -x;
    return x;
}   

// WCH CH32V SysTick register definitions
#define STK_BASE     0xE000F000UL
#define STK_CTLR     ((volatile uint32_t *)(STK_BASE + 0x00))
#define STK_SR       ((volatile uint32_t *)(STK_BASE + 0x04))
#define STK_CNTL      ((volatile uint32_t *)(STK_BASE + 0x08))
#define STK_CNTH      ((volatile uint32_t *)(STK_BASE + 0x0C))
#define STK_CMPL      ((volatile uint32_t *)(STK_BASE + 0x10))
#define STK_CMPH      ((volatile uint32_t *)(STK_BASE + 0x14))
// STK_CTLR bits
#define STK_CTLR_STE     (1 << 0) 
#define STK_CTLR_STIE    (1 << 1) 
#define STK_CTLR_STCLK   (1 << 2) 
#define STK_CTLR_STRE    (1 << 3) 
#define STK_CTLR_MODE    (1 << 4) 
#define STK_CTLR_INIT    (1 << 5) 
#define STK_CTLR_SWIE    (1 << 31) 


void check_assignment_3() {
    uint32_t ctlr = *STK_CTLR;
    if((ctlr & STK_CTLR_STCLK) == 0) {
        printf("Hint: Your SysTick timer is configured to only tick every 8 clock cycles. Make sure to set the STLK flag in the control register.\n");
    }
    if((ctlr & STK_CTLR_INIT) == 0) {
        printf("Hint: You did not set the INIT flag, so the timer will not copy the value from CMP to CNT before starting.");
    }
    if(ctlr & STK_CTLR_STE) {
        printf("Hint: You have already enabled the timer, so these hints may not be accurate.\n");
    }
    if(ctlr & STK_CTLR_MODE) {
        printf("Hint: You have set the MODE flag, which means the timer will count UP instead of down.");
    }
    if(*STK_CMPH != 0) {
        printf("Hint: CMPH is not 0, so you are trying to delay for a VERY long time");
    }
    if(*STK_SR != 0) {
        printf("Hint: The status register is not cleared so you won't be able to see when you are done.\n");
    }
}

int main(void)
{
    printf("Testing assignment 2...\n");
    uint64_t ticks = ns_to_ticks(10000); // 10us
    if(ticks == 1440 || ticks == 1439 || ticks == 1441) {
        printf("ns_to_ticks(10000) = %u. PASSED!\n", (unsigned int)ticks);
    } else {
        printf("ns_to_ticks(10000) = %u. FAILED!\n", (unsigned int)ticks);
        printf("144Mhz means 144M ticks per second, or 144 ticks per microsecond, or 0.144 ticks per nanosecond.\n");
    }

    printf("Testing assignment 3...");
    delay(10000); // This will call check_assignment_3()
    if(*STK_CTLR & 0x1) {
        printf("\ndelay() did not disable the timer after finishing. FAILED!\n");
    } else {
        if(*STK_SR & 0x1) {
            printf("\ndelay() did not clear the count flag after finishing. FAILED!\n");
        } else {
            printf("PASSED!\n");
        }
    }

    printf("Assuming you have implemented the delay_ms() function correctly, you should se a new dot APPROXIMATELY every second.\n");

    int ctr = 0;
    while(1) {
        delay_ms(10);
        putchar('.');
        ctr++; 
        if(ctr == 10) {
            for(int i=0; i<10; i++) {
                putchar('\b');
            }
            ctr = 0;    
        }
    }


}

