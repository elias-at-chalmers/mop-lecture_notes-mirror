#include "systick.h"

// Delay given amount of microseconds
void delay_micro(unsigned int us) {
  // Clock frequency 144 MHz (144 cycles per µs)
  unsigned long long count = 144 * ((unsigned long long) us);
  // Clear control and status registers
  *STK_CTRL = 0;
  *STK_SR = 0;
  // Set counter compare registers
  *STK_CMPHR = count >> 32;
  *STK_CMPLR = count;
  // Clock source: processor clock (CLK),
  // Initialise counter registers (INIT) when timer is enabled,
  // Enable timer (EN)
  *STK_CTRL = STK_B_CLK | STK_B_INIT | STK_B_EN;
  // Wait until status bit is set
  while((*STK_SR & 1) == 0);
  // Clear control register
  *STK_CTRL = 0;
}

// Delay given amount of milliseconds
void delay_milli(unsigned int ms) {
  // Clock frequency 144 MHz (144 * 1000 cycles per ms)
  unsigned long long count = 144 * 1000 * ((unsigned long long) ms);
  // Clear control and status registers
  *STK_CTRL = 0;
  *STK_SR = 0;
  // Set counter compare registers
  *STK_CMPHR = count >> 32;
  *STK_CMPLR = count;
  // Clock source: processor clock (CLK),
  // Initialize counter registers (INIT) when timer is enabled,
  // Enable timer (EN)
  *STK_CTRL = STK_B_CLK | STK_B_INIT | STK_B_EN;
  // Wait until status bit is set
  while((*STK_SR & 1) == 0);
  // Clear control register
  *STK_CTRL = 0;
}

// Delay given amount of nanoseconds
void delay_nano(unsigned int ns) {
  // Clock frequency 144 MHz (144 / 1000 cycles per ns)
#ifdef BUGS
  unsigned long long count = (144 * ((unsigned long long) ns)) / 100;
#else
  unsigned long long count = (144 * ((unsigned long long) ns)) / 1000;
#endif
  // Clear control and status registers
  *STK_CTRL = 0;
  *STK_SR = 0;
  // Set counter compare registers
  *STK_CMPHR = count >> 32;
  *STK_CMPLR = count;
  // Clock source: processor clock (CLK),
  // Initialise counter registers (INIT) when timer is enabled,
  // Enable timer (EN)
  *STK_CTRL = STK_B_CLK | STK_B_INIT | STK_B_EN;
  // Wait until status bit is set
  while((*STK_SR & 1) == 0);
  // Clear control register
  *STK_CTRL = 0;
}
