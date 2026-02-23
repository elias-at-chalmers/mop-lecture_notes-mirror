#ifndef GPIO_H
#define GPIO_H

// GPIO ports
#define GPIO_A 0x40010800
#define GPIO_B 0x40010C00
#define GPIO_C 0x40011000
#define GPIO_D 0x40011400
#define GPIO_E 0x40011800

// GPIO port registers
#define GPIO_CFGLR(port) ((volatile unsigned int   *) (port + 0x00))
#define GPIO_CFGHR(port) ((volatile unsigned int   *) (port + 0x04))
#define GPIO_INDR(port)  ((volatile unsigned short *) (port + 0x08))
#define GPIO_OUTDR(port) ((volatile unsigned short *) (port + 0x0C))
#define GPIO_BSHR(port)  ((volatile unsigned int   *) (port + 0x10))
#define GPIO_BCR(port)   ((volatile unsigned int   *) (port + 0x14))
#define GPIO_LCKR(port)  ((volatile unsigned int   *) (port + 0x18))

#endif
