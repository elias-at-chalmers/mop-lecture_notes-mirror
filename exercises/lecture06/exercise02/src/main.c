#include <stdio.h>
#include <stdint.h>

extern uint64_t ns_to_ticks(uint64_t ns);
extern void delay(uint64_t ns);
extern void delay_us(uint32_t ns);


///////////////////////////////////////////////////////////////////////////////
// Basic Timers
//
#define TIMER6_IRQ_NUM    70
#define TIMER7_IRQ_NUM    71

typedef struct {
    union {
        uint32_t CTRL1;            // 0x00
        struct {
            uint32_t enable : 1;
            uint32_t udis   : 1;
            uint32_t urs    : 1;
            uint32_t opm    : 1;
            uint32_t        : 3;
            uint32_t arpe   : 1;
            uint32_t        : 24;
        } ctrl1;
    };
    uint32_t CTRL2;                // 0x04
    uint32_t RESERVED0;            // 0x08
    uint32_t DMAINTENR;             // 0x0C
    uint32_t INTFR;                 // 0x10
    uint32_t SWEVGR;                // 0x14
    uint32_t RESERVED1[3];          // 0x18, 0x1C, 0x20
    uint32_t CNT;                   // 0x24 (lower 16 bits valid)
    uint32_t PSC;                   // 0x28 (lower 16 bits valid)
    uint32_t ATRLR;                 // 0x2C (lower 16 bits valid)
} TIMER_t; 

volatile TIMER_t* timer6 = (TIMER_t*)0x40001000;
volatile TIMER_t* timer7 = (TIMER_t*)0x40001400;
///////////////////////////////////////////////////////////////////////////////

static inline __attribute__((always_inline)) void start_timer6(void)
{
        *timer6 = (TIMER_t){0}; // Clear everything to 0
        timer6->ctrl1.enable = 0;
        timer6->CNT = 0;   // Reset counter
        timer6->PSC = 143;  // Tick every 1us (144MHz / (143 + 1) = 1MHz)
        timer6->ATRLR = 0xFFFF; 
        timer6->ctrl1.enable = 1; // Enable timer    
}

int abs(int x) {
    if(x < 0) return -x;
    return x;
}   

int main(void)
{
    start_timer6(); // Run this once, cause the first run will not have PSC updated. 

    printf("Testing assignment 2...\n");
    uint64_t ticks = ns_to_ticks(10000); // 10us
    if(ticks == 1440 || ticks == 1439 || ticks == 1441) {
        printf("ns_to_ticks(10000) = %llu. PASSED!\n", ticks);
    } else {
        printf("ns_to_ticks(10000) = %llu. FAILED!\n", ticks);
        printf("144Mhz means 144M ticks per second, or 144 ticks per microsecond, or 0.144 ticks per nanosecond.\n");
    }



    printf("Testing assignment 3...\n");
    start_timer6(); 
    delay(10000);
    int32_t time = timer6->CNT;
    printf("delay(10000) took %uus\n", (unsigned int) time);
    start_timer6(); 
    delay(100000);
    time = timer6->CNT;
    printf("delay(100000) took %uus\n", (unsigned int) time);
    start_timer6(); 
    delay(1000000);
    time = timer6->CNT;
    printf("delay(1000000) took %uus\n", (unsigned int) time);
    if(abs(time - 1000) < 20) {
        printf("Close enough. PASSED!\n");
    } else {
        printf("Too far off. FAILED!\n");
    }

    printf("Testing assignment 3...\n");
    start_timer6(); 
    delay_us(10);
    time = timer6->CNT;
    printf("delay_us(10) took %uus\n", (unsigned int) time);
    if(abs(time - 10) < 20) {
        printf("Close enough. PASSED!\n");
    } else {
        printf("Too far off. FAILED!\n");
    }
}

