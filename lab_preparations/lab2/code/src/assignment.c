///////////////////////////////////////////////////////////////////////////////
// Lab Preparation 2
// ============================================================================
// In Lab 2 you will need to know how the ASCII display works, so in this lab 
// you will write a program that interacts with it, on the simulator. 
//
// YOU ARE ALSO EXPECTED TO HAVE DONE THE EXERCISE FOR LECTURE 05, so you know 
// how the keyboard input works. 
//
// IMPORTANT: Before you do this lab, go through Lecture06, Excercise 02, where
// you learn how to create short delays using systick.
//
// In this assignment there are test_functions already in place where we try
// to check that you have solved each assignment correctly. Once you have 
// passed an assignment, you can comment these out.
// 
// Please see the quickguide for a summary of the ASCII Display.
// 
// In the simulator, connect the LC display to GPIO Port E
///////////////////////////////////////////////////////////////////////////////

#define SOLUTION 1

#include <stdint.h>

void check_assignment_1(); 
void check_assignment_2_1(uint8_t command_or_data);
void check_assignment_2_2();
void check_assignment_5_1();
void check_assignment_5_2();
void check_assignment_5_3();
void check_assignment_5_4();
void check_assignment_5_5();

///////////////////////////////////////////////////////////////////////////////
// Register macros. 
///////////////////////////////////////////////////////////////////////////////

/* TODO: Put all your register macros here */

#if SOLUTION
#define STK_BASE     0xE000F000UL
#define STK_CTLR     ((volatile uint32_t *)(STK_BASE + 0x00))
#define STK_SR       ((volatile uint32_t *)(STK_BASE + 0x04))
#define STK_CNT      ((volatile uint64_t *)(STK_BASE + 0x08))
#define STK_CMP      ((volatile uint64_t *)(STK_BASE + 0x10))
// STK_CTLR bits
#define STK_CTLR_STE     (1 << 0)
#define STK_CTLR_STIE    (1 << 1)
#define STK_CTLR_STCLK   (1 << 2)
#define STK_CTLR_STRE    (1 << 3)
#define STK_CTLR_MODE    (1 << 4)
#define STK_CTLR_INIT    (1 << 5)
#define STK_CTLR_SWIE    (1 << 31)


#define GPIOE_CFGLR ((volatile uint32_t *)0x40011800)
#define GPIOE_CFGHR ((volatile uint32_t *)0x40011804)
#define GPIOE_INDR ((volatile uint32_t *)0x40011808)
#define GPIOE_OUTDR ((volatile uint32_t *)0x4001180C)

#define EN 4 // Bit 2
#define RW 2 // Bit 1
#define RS 1 // Bit 0

#endif


///////////////////////////////////////////////////////////////////////////////
// Assignment 0: If you have done Lecture06 Exercise02, you already have
//               the code for this. Implement the delay functions below.
//               They are not tested in this assignment, so make sure they
//               work using the Lecture06 Exercise02.
///////////////////////////////////////////////////////////////////////////////
void delay(uint64_t ns)
{
    // TODO: delay for ns nanoseconds, using the systick timer. 
#if SOLUTION
    // Convert ns to ticks (144MHz = 144 ticks per microsecond = 0.144 ticks per ns)
    uint64_t ticks = (ns * 144000000ULL) / 1000000000ULL;    
    *STK_CTLR = 0; // Disable SysTick
    *STK_CMP = ticks; // Set compare value
    *STK_CNT = 0; // Reset counter
    *STK_SR = 0; // Clear status register
    *STK_CTLR = STK_CTLR_INIT | STK_CTLR_STE | STK_CTLR_STCLK; // Enable, use HCLK
    // Wait for count flag (bit 0 in STK_SR)
    while ((*STK_SR & 1) == 0) {
            // busy wait
    }
    *STK_CTLR = 0; // Disable SysTick
#endif    
}

void delay_us(uint32_t us)
{
    // TODO: delay for us microseconds, using delay(ns)
#if SOLUTION
    delay(us * 1000ULL);
#endif    
}

void delay_ms(uint32_t ms)
{
    // TODO: delay for ms milliseconds, using delay(ns)
#if SOLUTION
    delay(ms * 1000000ULL);
#endif    
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 1: Configure GPIO Port E so that all pins [0:15] are set as 
//               output, 50MHz, push-pull.
///////////////////////////////////////////////////////////////////////////////
void init_gpio_port_e()
{
#if SOLUTION
    // Set all pins to output, 2MHz, push-pull
    *GPIOE_CFGLR = 0x22222222;
    *GPIOE_CFGHR = 0x22222222;
#endif    
}



///////////////////////////////////////////////////////////////////////////////
// Assignment 2: Implement the write cycle. This will be used to write both 
//               commands and data to the display. 
//    
//               Look at the "Writing a command or data to the display" 
//               timing diagram in the quickguide, to see how this works.
// 
// Input: command_or_data - the byte to write to the display. This can be 
//                          either a command or data, depending on the state 
//                          of the control lines.
///////////////////////////////////////////////////////////////////////////////
void ascii_write_controller(uint8_t command_or_data)
{
    // The control lines (RS and RW) are set before calling this function.
    // According to the quickguide, you need to wait 40ns after setting the
    // control lines before initiating a write to the display. 
    //
    // 40ns is the time it takes to execute 8 instructions, so that time 
    // will already have passed by the time the processor has entered this
    // function. 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2.1: Set E = 1 to start the write cycle
    //                 Then write the command or data to GPIOE_OUTDR[8..15].
    //                 Hint: Make sure your data write does not overwrite 
    //                 the control bits in GPIOE_OUTDR[0..7]!
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION
    *GPIOE_OUTDR &= 0x00FF; 
    *GPIOE_OUTDR |= command_or_data << 8; // Set data on output lines
    *GPIOE_OUTDR |= EN; // Set E = 1 to start write cycle
#endif
    check_assignment_2_1(command_or_data);

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2.2: Wait for at least tsu2 = 80ns, then set E = 0 to end 
    //                 the write cycle.
    // TODO: Fix this, needs to be 230 b.c. tw.
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION
    delay(230); // Wait for tsu2 = 80ns
    *GPIOE_OUTDR &= ~EN; // Set E = 0 to end write cycle

    delay(500); // Wait for tsu2 = 80ns

#endif
    check_assignment_2_2();

    // Finally, wait for th = 10ns before returning from the function. This 
    //is the time it takes for the display to process the command or data after 
    // the write cycle has ended.
    //
    // 10ns is the time it takes to execute 2 instructions, so that time will
    // already have passed by the this function returns. 
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3: Implement code for writing a command to the display.
///////////////////////////////////////////////////////////////////////////////
void ascii_write_command(uint8_t command)
{
    // TODO: Set RS = 0 and RW = 0 to indicate a command write, then call
    //       ascii_write_controller to write the command to the display.
#if SOLUTION
    *GPIOE_OUTDR &= ~RW; // Clear RW
    *GPIOE_OUTDR &= ~RS; // Clear RS
    ascii_write_controller(command);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 4: Just read this
// 
// Now that you have implemented a way for us to write commands to the display
// we need to run some magic code to initialize it. This is done by sending a 
// specific sequence of commands to the display, according to the datasheet.
// 
// For the simulator, this is not required at all (and, in fact, takes too long
// time), but if you want to run this on real hardware later, you will need 
// this initialization sequence.
///////////////////////////////////////////////////////////////////////////////

void ascii_initialize_display()
{
    //return; // Not needed for the simulator.
    // According to the datasheet, we need to wait for more than 15ms after
    // VCC rises to 4.5V before sending the first command.
    delay_ms(20);
    // Send Function Set command (0x38) three times, with specific delays
    ascii_write_command(0x38);
    delay_ms(5); // Wait for more than 4.1ms
    ascii_write_command(0x38);
    delay_us(200); // Wait for more than 100µs
    ascii_write_command(0x38);
    // Now the display is initialized.
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 5: Implement a function that reads the status byte from the 
//               display, to see if it is busy or ready for the next command.
//
//               Look at the "Reading status or data from the display" timing 
//               diagram in the quickguide, to see how this works.
//
// Output: 1 if busy, 0 if ready for new command.
///////////////////////////////////////////////////////////////////////////////
uint8_t ascii_read_status()
{
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5.1: Temporarily configure GPIO Port E pins [8:15] as 
    //                 input, floating.
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION
    *GPIOE_CFGHR = 0x44444444; // Clear higher config register
#endif
    check_assignment_5_1(); 
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5.2: Set RS = 0 (command) and RW = 1 (read) to indicate 
    //                 a status read.
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION
    *GPIOE_OUTDR &= ~RS; // Clear RS
    *GPIOE_OUTDR |= RW; // Set RW
#endif    
    check_assignment_5_2(); 
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5.3: Set E = 1 to start the read cycle,
    //                 wait for tD = 360ns
    //                 then read the status byte from GPIO Port E pins [8:15]
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION

    // ERIK
    __asm__("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"); // 144MHz means each cycle approximately 7ns. 60ns delay is ~9 cycles


    *GPIOE_OUTDR |= EN; // Set E = 1 to start read cycle
    delay(360); // Wait for tD = 360ns
    uint8_t status = (*GPIOE_INDR >> 8) & 0xFF; // Read status byte

    // ERIK
    delay(1000);
#endif    
    check_assignment_5_3(); 
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5.4: Set E = 0 to end the read cycle.
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION
    *GPIOE_OUTDR &= ~EN; // Clear E = 0 to end read cycle
#endif    
    check_assignment_5_4(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5.5: Configure GPIO Port E pins [8:15] back to output,
    //                 50MHz, push-pull.
    ///////////////////////////////////////////////////////////////////////////
    // Your code here
#if SOLUTION
    *GPIOE_CFGHR = 0x22222222; // Set higher config register back to output
#endif
    check_assignment_5_5(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5.6: Return the busy flag (bit 7 of the status byte)
    ///////////////////////////////////////////////////////////////////////////
    // Replace this
#if SOLUTION
    return (status >> 7) & 1; // Return busy flag
#endif
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 6: Implement code for writing a chareacter (data) to the display.
///////////////////////////////////////////////////////////////////////////////
void ascii_write_data(uint8_t data)
{
    // TODO: Set RS = 1 (data) and RW = 0 (write) to indicate a command write, 
    //       then call ascii_write_controller to write the character to the 
    //       display.
#if SOLUTION
    *GPIOE_OUTDR &= ~RW; // Clear RW
    *GPIOE_OUTDR |= RS; // Set RS
    ascii_write_controller(data);

#endif
}
