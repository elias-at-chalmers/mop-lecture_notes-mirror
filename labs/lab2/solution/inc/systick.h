#ifndef SYSTICK_H
#define SYSTICK_H

// SysTick registers
#define STK_CTRL  ((volatile unsigned int  *) (0xE000F000 + 0x00))
#define STK_SR    ((volatile unsigned int  *) (0xE000F000 + 0x04))
#define STK_CNTL  ((volatile unsigned int  *) (0xE000F000 + 0x08))
#define STK_CNTH  ((volatile unsigned int  *) (0xE000F000 + 0x0C))
#define STK_CMPLR ((volatile unsigned int  *) (0xE000F000 + 0x10))
#define STK_CMPHR ((volatile unsigned int  *) (0xE000F000 + 0x14))

// SysTick bits
#define STK_B_SWIE (1 << 31)
#define STK_B_INIT (1 <<  5)
#define STK_B_MODE (1 <<  4)
#define STK_B_RE   (1 <<  3)
#define STK_B_CLK  (1 <<  2)
#define STK_B_IE   (1 <<  1)
#define STK_B_EN   (1 <<  0)
#define STK_B_CNT  (1 <<  0)

// Delay given amount of microseconds
void delay_micro(unsigned int us);

// Delay given amount of milliseconds
void delay_milli(unsigned int ms);

// Delay given amount of nanoseconds
void delay_nano(unsigned int ns);

#endif
