#include <stdint.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Lecture 06 - Exercise 02
// ============================================================================
// In this exercise, you will learn to use the SysTick timer to implement a 
// delay function. 
//
// Like previous exercises, your code will be tested from an external main.c
// file. Since your code will run on a simulator, meassuring the ACTUAL time
// is not possible (real-time for the simulator depends on the host computer).
//
// Instead, the test code will use another timer (TIMER6) to measure how many
// clock-cycles your delay function takes, and calculate the real time it would
// have taken on hardware. This is not super exact, so don't worry if your 
// results are a bit off.
//
///////////////////////////////////////////////////////////////////////////////

#define SOLUTION 1

// Ignore this, it is just a declaration of a function you can use to test your
// code later. 
void check_assignment_3();

///////////////////////////////////////////////////////////////////////////////
// Assignment 1: 
// Open the quickguide and create definitions for the SysTick registers.
// Remember that STK_CNT and STK_CMP are 64-bit registers so you will either
// need to define them as uint64_t pointers, or as two uint32_t pointers.
///////////////////////////////////////////////////////////////////////////////

#if SOLUTION
// WCH CH32V SysTick register definitions
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
#else
#define STK_BASE     /* TODO */
#define STK_CTLR     /* TODO */
#define STK_SR       /* TODO */
#define STK_CNT      /* TODO */
#define STK_CMP      /* TODO */
#endif

///////////////////////////////////////////////////////////////////////////////
// Assignment 2: 
// The system runs at 144Mhz. Implement a function that calulates the number 
// of ticks needed for the requested delay in nanoseconds.
///////////////////////////////////////////////////////////////////////////////
uint64_t ns_to_ticks(uint64_t ns)
{
#if SOLUTION
	return (ns * 144ULL) / 1000ULL; 
#else
	// Your code here
	return 0; 
#endif
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 3: 
// Implement a function that uses the SysTick timer to create a delay
// of the requested length in nanoseconds.
///////////////////////////////////////////////////////////////////////////////
void delay(uint64_t ns)
{
#if SOLUTION	
	// Convert ns to ticks (144MHz = 144 ticks per microsecond = 0.144 ticks per ns)
	uint64_t ticks = ns_to_ticks(ns);
	*STK_CTLR = 0; // Disable SysTick
	*STK_CMP = ticks; // Set compare value
	*STK_CNT = 0; // Reset counter
	*STK_SR = 0; // Clear status register
	*STK_CTLR = STK_CTLR_STCLK | STK_CTLR_INIT; // Enable, use HCLK

	check_assignment_3();

	*STK_CTLR |= STK_CTLR_STE; // Start timer

	// Wait for count flag (bit 0 in STK_SR)
	while ((*STK_SR & 1) == 0) {
		// busy wait
	}
	*STK_CTLR = 0; // Disable SysTick
	*STK_SR = 0; // Clear status register
#else
	// Initialize systick so that it counts DOWN from STK_CMP.
	// STK_CMP should be set to the number of ticks corresponding to the requested delay.

	// You can uncomment this call to get some hints about your configuration.
	// But try to solve it yourself first!
	//check_assignment_3();

	// Now start the timer and then read the status register until the count flag is set. 

	// Finally, disable the timer again, and clear the status register.
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 4:
// Write a delay_us(), and a delay_ms() function that uses your delay(ns) function.
///////////////////////////////////////////////////////////////////////////////
void delay_us(uint32_t us)
{
#if SOLUTION
	delay(us * 1000); // 1 microsecond = 1000 nanoseconds
#endif
}

void delay_ms(uint32_t ms)
{
#if SOLUTION
	delay(ms * 1000000); // 1 millisecond = 1000000 nanoseconds
#endif
}