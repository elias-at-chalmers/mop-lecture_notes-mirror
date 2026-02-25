///////////////////////////////////////////////////////////////////////////////
// Lecture 09 - Exercise 01
// A complicated light-switch. 
// ============================================================================
// When you start this program, you shall see the lights on the bargraph
// blinking. Your job is to turn them off. You will do this using interrupts,
// without modifying the original program. 
//
// When the highest switch (but 7) on the DIPSWITCH goes from 0 to 1, the
// lights should stop blinking, and when it goes from 1 to 0, it should start 
// again. This will be implemented using interrupts. 
// 
// Follow the lecture notes or slides as you do these assignments so you
// remember what you are doing. 
//
// Connect: 
// Console to USART 1 as usual
// Bargraph to GPIOD[0:7]
// Dipswitch to GPIOE[0:7]
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdio.h>
#include "peripherals.h"

#define SOLUTION 1

volatile GPIO_t* GPIO_D = (GPIO_t*) 0x40011400;
volatile GPIO_t* GPIO_E = (GPIO_t*) 0x40011800;
volatile SysTick_t * systick = (SysTick_t*) 0xE000F000;
volatile int light_on = 1;

void init_interrupts(void);

////////////////////////////////////////////////////////////////////////////////
// Insert your macrodefinitions or structs for AFIO, EXTI, and PFIC up here
// when you need them. 
////////////////////////////////////////////////////////////////////////////////
#if SOLUTION
#define AFIO_EXTICR1 ((volatile uint32_t*) 0x40010008)
#define AFIO_EXTICR2 ((volatile uint32_t*) 0x4001000C)
#define AFIO_EXTICR3 ((volatile uint32_t*) 0x40010010)
#define AFIO_EXTICR4 ((volatile uint32_t*) 0x40010014)

#define EXTI_INTENR ((volatile uint32_t*) 0x40010400)
#define EXTI_EVENR ((volatile uint32_t*) 0x40010404)
#define EXTI_RTENR ((volatile uint32_t*) 0x40010408)
#define EXTI_FTENR ((volatile uint32_t*) 0x4001040C)
#define EXTI_SWIEVR ((volatile uint32_t*) 0x40010410)
#define EXTI_INTFR ((volatile uint32_t*) 0x40010414)

#define PFIC_IENR1 ((volatile uint32_t*) 0xE000E100)
#define PFIC_IENR2 ((volatile uint32_t*) 0xE000E104)
#endif



#define FAILED(n)                       \
    do {                                \
        printf("\nFAILED: %s", n);      \
        assignment_passed = 0;          \
    } while(0);  

void check_assignment_1()
{
    printf("Checking assignment 1...");
    int assignment_passed = 1; 
    if(((GPIO_E->CFGLR >> 7*4) & 0xF) != 0x8) FAILED("PE7 not configured as input with pull-up/down");
    if((GPIO_E->OUTDR & 0b10000000) != 0b10000000) FAILED("PE7 not pulled up");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_2()
{
    printf("Checking assignment 2...");
    int assignment_passed = 1; 
    if((*AFIO_EXTICR1 != 0) || (*AFIO_EXTICR3 != 0) || (*AFIO_EXTICR4 != 0) || (*AFIO_EXTICR2 & 0x0FFF) != 0) 
        FAILED("You have changed the AFIO_EXTICR configuration for pins other than PE7");
    if((*AFIO_EXTICR2 & 0xF000) != 0x4000) FAILED("PE7 not routed to EXTI7");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_3()
{
    printf("Checking assignment 3...");
    int assignment_passed = 1; 
    if((*EXTI_RTENR & 0b10000000) != 0b10000000) FAILED("Rising edge trigger not enabled for EXTI7");
    if((*EXTI_FTENR & 0b10000000) != 0) FAILED("Falling edge trigger should be disabled for EXTI7");
    if((*EXTI_INTENR & 0b10000000) != 0b10000000) FAILED("Interrupt not enabled for EXTI7");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_4()
{
    printf("Checking assignment 4...");
    int assignment_passed = 1; 
    *PFIC_IENR1 = 5; 
    int b = *PFIC_IENR1;

    *PFIC_IENR2 = 5; 
    int a = *PFIC_IENR2;
    if((*PFIC_IENR2 & (1<<7)) != (1<<7)) FAILED("Interrupt for EXTI7 not enabled in PFIC");
    if((*PFIC_IENR2 & ~(1<<7)) != 0) FAILED("You have enabled interrupts other than EXTI7 in PFIC");
    if(assignment_passed) printf("PASSED!\n");
}

////////////////////////////////////////////////////////////////////////////////
// Assignment 7: Write the interrupt handler. 
//               All this handler has to do is to toggle the state of the 
//               light_on variable, and clear the interrupt flag for EXTI7. 
////////////////////////////////////////////////////////////////////////////////

__attribute__((interrupt("machine"))) void exti5_9_handler()
{
#if SOLUTION
    light_on = !light_on; 
    *EXTI_INTFR |= 0b10000000; // Clear the interrupt flag for EXTI7
#else
    // <your code here>
#endif
}

void blink()
{    
    ///////////////////////////////////////////////////////////////////////////
    // Configure Bargraph (do not modify this part)
    ///////////////////////////////////////////////////////////////////////////
    GPIO_D->CFGLR = 0x22222222; // Set PD0-7 as output, push-pull, 2 MHz
    GPIO_D->OUTDR = 0; // Turn off all lights

    ////////////////////////////////////////////////////////////////////////////
    // Assignment 1: Configure the GPIO ports
    //               The DIL swithces are simple switches where one end is 
    //               connected to ground and the other is connected to the 
    //               GPIO pin. Thus, they have to be configured as input pins
    //               with PULL-UP. 
    ////////////////////////////////////////////////////////////////////////////
#if SOLUTION
    GPIO_E->CFGLR = 0x88888888; // Set PE0-7 as input, pull up/down
    GPIO_E->OUTDR = 0xFFFF; // Pull up
#else
    // <your code here>
    //
    // To test this step, you can temporarily uncomment the next line. This is 
    // an endless loop that will read the state of the switches and output it to the
    // bargraph, so you can verify that you have configured the GPIO ports correctly.
    // while(1) GPIO_D->OUTDR = GPIO_E->INDR;
#endif
    check_assignment_1(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2: Routing the right pin to EXTI.
    //               Since we want to trigger an interrupt when the state of PE7 
    //               changes, we need to configure the AFIO so that Port E is 
    //               routed for EXTI7. (see lecture notes or slides)
    // 
    //               (you will need to create a macro or struct for AFIO for 
    //                this one)
    ///////////////////////////////////////////////////////////////////////////
#if SOLUTION
    *AFIO_EXTICR2 = 0x4000; // Route PE7 to EXTI7
#else
    // <your code here>
#endif
    check_assignment_2();

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 3: Configuring EXTI
    //               Now that Pin 7 of Port E is routed to EXTI7, we need to 
    //               configure EXTI so that it triggers an interrupt when the 
    //               state of the pin goes from 0 to 1 (rising edge).
    //
    //               Check the lecture notes os slides. 
    //               
    //               (you will need to create a macro or struct for the EXTI 
    //                registers for this one)
    ///////////////////////////////////////////////////////////////////////////
#if SOLUTION
    *EXTI_RTENR |= 0b10000000; // Enable rising edge trigger for EXTI7
    *EXTI_FTENR &= ~0b10000000; // Disable falling edge trigger for EXTI7
    *EXTI_INTENR |= 0b10000000; // Enable interrupt for EXTI7
#else
    // <your code here> 
    // Configure EXTI_RTENR, EXTI_FTENR, and EXTI_INTENR for pin 7
#endif
    check_assignment_3();

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4: Configuring PFIC
    //               If everything is right so far, an interrupt request will 
    //               be triggered from the EXTI module to the PFIC when the 
    //               state of PE7 goes from 0 to 1. 
    //               We must now configure the PFIC so that it enables that 
    //               interrupt and sends it on to the CPU. 
    // 
    //               You will need to configure PFIC_IENR1 for this, and you
    //               will have to check the vector table in the quickguide
    //               to see which interrupt number corresponds to EXTI7.
    ///////////////////////////////////////////////////////////////////////////
#if SOLUTION
    *PFIC_IENR2 |= (1<<7);
#endif
    check_assignment_4(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5: Initializing the vector table
    //               If everything is correct, and someone flips the switch 
    //               from 0 to 1, an interrupt request will be sent to the CPU.
    //               The CPU will then look at the CSR register `mtvec` to find
    //               our which address to jump to, and in which way (direct or
    //               vectored mode). 
    // 
    //               At the moment, mtvec is empty, so the CPU will crash and 
    //               burn. Implement the function init_interrupts in 
    //               assignment.s to fix this. 
    ///////////////////////////////////////////////////////////////////////////
    init_interrupts(); 


    ///////////////////////////////////////////////////////////////////////////
    // The program below blinks a light and should not be modified
    ///////////////////////////////////////////////////////////////////////////
    while(1)
    {
        systick->CTLR = 0; // Disable systick and clear configuration. 
        systick->CNT = 0; // Clear the current count
        systick->CMP = 144000; 
        systick->SR = 0; // Clear the status register
        systick->CTLR = 0b100101; 
        while (systick->SR == 0);
        GPIO_D->OUTDR ^= 0xFF; // Toggle PD0
        while(!light_on); // Just wait if light is turned off
    }
}