#include <stdio.h>
#include <stdint.h>
void blink(void);

      




#define AFIO_EXTICR1 ((volatile uint32_t*) 0x40010008)
#define AFIO_EXTICR2 ((volatile uint32_t*) 0x4001000C)
#define AFIO_EXTICR3 ((volatile uint32_t*) 0x40010010)
#define AFIO_EXTICR4 ((volatile uint32_t*) 0x40010014)

int main(void)
{
    *AFIO_EXTICR1 = 0; 
    *AFIO_EXTICR2 = 0; 
    *AFIO_EXTICR3 = 0; 
    *AFIO_EXTICR4 = 0; 
    
    blink(); 
}

