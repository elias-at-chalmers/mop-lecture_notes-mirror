#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint16_t CEN  :1;  // bit 0
    uint16_t UDIS :1;  // bit 1
    uint16_t URS  :1;  // bit 2
    uint16_t OPM  :1;  // bit 3
    uint16_t      :3;  // bits 4–6 reserved
    uint16_t ARPE :1;  // bit 7
    uint16_t      :8;  // bits 8–15 reserved
} CTLR1_t;

typedef struct {
    uint16_t      :4;  // bits 0–3 reserved
    uint16_t MMS  :3;  // bits 4–6
    uint16_t      :9;  // bits 7–15 reserved
} CTLR2_t;

typedef struct {
    uint16_t UIE  :1;  // bit 0
    uint16_t      :7;  // bits 1–7 reserved
    uint16_t UDE  :1;  // bit 8
    uint16_t      :7;  // bits 9–15 reserved
} DMAINTENR_t;

typedef struct {
    union { uint16_t CTLR1;     
            CTLR1_t  ctlr1; }; 
    uint16_t _pad0;  // 0x00
    union { uint16_t CTLR2;     
            CTLR2_t  ctlr2; }; 
    uint16_t _pad1;  // 0x04
    uint16_t _reserved0; uint16_t _pad2;  // 0x08
    union { uint16_t DMAINTENR; 
            DMAINTENR_t dmaintenr; }; 
    uint16_t _pad3; // 0x0C
    uint16_t INTFR; uint16_t _pad4;  // 0x10
    uint16_t SWEVGR; uint16_t _pad5;  // 0x14
    uint16_t _reserved1; uint16_t _pad6;  // 0x18
    uint16_t _reserved2; uint16_t _pad7;  // 0x1C
    uint16_t _reserved3; uint16_t _pad8;  // 0x20
    uint16_t CNT;       uint16_t _pad9;  // 0x24
    uint16_t PSC;       uint16_t _pad10; // 0x28
    uint16_t ATRLR;     uint16_t _pad11; // 0x2C
} TIMER_t;

//#define TIM6 ((TIM6_t *)0x40001000)
TIMER_t * timer6 = (TIMER_t *)0x40001000;

void delay_1s(void)
{
    
    timer6->ctlr1.CEN = 0;      /* 1. Stop timer */
    timer6->PSC = 14399;        /* 2. Prescaler: 144 MHz / (14399 + 1) = 10 kHz */
    timer6->ATRLR = 9999;       /* 3. Auto-reload: 10000 ticks = 1 second */
    timer6->SWEVGR = 1;         /* 4. Load PSC and ATRLR */
    timer6->INTFR = 0;          /* 5. Clear update flag */
    timer6->ctlr1.CEN = 1;      /* 6. Enable timer */
    while (timer6->INTFR == 0); /* 7. Wait for overflow */
    timer6->ctlr1.CEN = 0;      /* 8. Stop timer (optional) */
    timer6->INTFR = 0;          /* 9. Clear flag */
}

int main(void)
{
    while(1) {
        printf("Hello every second!\n");
        delay_1s();
    }
}

