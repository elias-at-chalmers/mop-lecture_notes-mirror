#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <stdint.h>

#define SOLUTION 1

///////////////////////////////////////////////////////////////////////////////
// Register macros.
///////////////////////////////////////////////////////////////////////////////

#define STK_BASE 0xE000F000
#define STK_CTLR ((volatile uint32_t *)(STK_BASE + 0x00))
#define STK_SR ((volatile uint32_t *)(STK_BASE + 0x04))
#define STK_CNT ((volatile uint64_t *)(STK_BASE + 0x08))
#define STK_CMP ((volatile uint64_t *)(STK_BASE + 0x10))

// STK_CTLR bits
#define STK_CTLR_STE (1 << 0)
#define STK_CTLR_STIE (1 << 1)
#define STK_CTLR_STCLK (1 << 2)
#define STK_CTLR_STRE (1 << 3)
#define STK_CTLR_MODE (1 << 4)
#define STK_CTLR_INIT (1 << 5)
#define STK_CTLR_SWIE (1 << 31)

#define GPIOE_CFGLR ((volatile uint32_t *)0x40011800)
#define GPIOE_CFGHR ((volatile uint32_t *)0x40011804)
#define GPIOE_INDR ((volatile uint32_t *)0x40011808)
#define GPIOE_OUTDR ((volatile uint32_t *)0x4001180C)

#define GPIOD_CFGLR ((volatile uint32_t *)0x40011400)
#define GPIOD_CFGHR ((volatile uint32_t *)0x40011404)
#define GPIOD_INDR ((volatile uint32_t *)0x40011408)
#define GPIOD_OUTDR ((volatile uint32_t *)0x4001140C)

// interrupt status (readable)
#define PFIC_ISR1  ((volatile uint32_t *)(PFIC_BASE + 0x000))

#ifdef NON-SOLUTION
#define EXTI_INTENR // TODO
#define EXTI_RTENR // TODO
#define EXTI_FTENR // TODO
#define EXTI_SWIEVR // TODO
#define EXTI_INTFR // TODO

#define PFIC_IENR1 // TODO

#define AFIO_EXTICR1 // TODO

#endif

#if SOLUTION
#define EXTI_INTENR ((volatile uint32_t *)0x40010400)
#define EXTI_RTENR ((volatile uint32_t *)0x40010408)
#define EXTI_FTENR ((volatile uint32_t *)0x4001040C)
#define EXTI_SWIEVR ((volatile uint32_t *)0x40010410)
#define EXTI_INTFR ((volatile uint32_t *)0x40010414)

#define PFIC_BASE 0xE000E000
#define PFIC_IENR1 ((volatile uint32_t *)(PFIC_BASE + 0x100)) // interrupt enable (write-only)

#define AFIO_EXTICR1 ((volatile uint32_t *)0x40010008)
#endif

#endif /* ASSIGNMENT_H */
