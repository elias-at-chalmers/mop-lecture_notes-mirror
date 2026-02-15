#include "gpio.h"
#include "keypad.h"
#include "systick.h"

// Read keypad and return pressed key value (0xFF if none pressed)
unsigned char keyb() {
  const unsigned char key_values[4][4] = {
      { '1', '2', '3', 'A' },
      { '4', '5', '6', 'B' },
      { '7', '8', '9', 'C' },
      { 'E', '0', 'F', 'D' },
  };
  unsigned int row;
  unsigned int col;
  // For each row:
  for (row = 1; row <= 4; row++) {
    // Activate row
    kbd_activate(row);
    // Speed set to 2 MHz
    // => period of 1s / (2 * 10^6) = 1 / 2 us = 500 ns
    // => need to wait at least 500 ns before checking columns
    delay_nano(500);
    // Read columns
    col = kbd_getcol();
    // If any column is active:
    // Deactivate all rows and return corresponding key value
    if (col) {
      kbd_activate(0);
      return key_values[row - 1][col - 1];
    }
  }
  // No key press detected:
  // Deactivate all rows and return 0xFF
  kbd_activate(0);
  return 0xFF;
}

// Read columns, returning first active one left-to-right (0 if none active)
unsigned int kbd_getcol() {
  // Read port D [0:15]
  unsigned short indr = *GPIO_INDR(GPIO_D);
  // Check pins [8:11] (active if 0 due to pull-up)
  if ((indr & 0x0100) == 0) return 1;
  if ((indr & 0x0200) == 0) return 2;
  if ((indr & 0x0400) == 0) return 3;
  if ((indr & 0x0800) == 0) return 4;
  return 0;
}

// Activate given row ∈ [1..4]
void kbd_activate(unsigned int row) {
  // Deactivate all rows by default
  *GPIO_OUTDR(GPIO_D) |= 0xF000;
  // Activate given row [1..4] corresponding to pins [12:15]
  // (activate by clearing due to pull-up)
#ifdef BUGS
  switch (row) {
    case 1: *GPIO_OUTDR(GPIO_D) &= ~0x1000;
    case 2: *GPIO_OUTDR(GPIO_D) &= ~0x2000;
    case 3: *GPIO_OUTDR(GPIO_D) &= ~0x4000;
    case 4: *GPIO_OUTDR(GPIO_D) &= ~0x8000;
  }
#else
  switch (row) {
    case 1: *GPIO_OUTDR(GPIO_D) &= ~0x1000; break;
    case 2: *GPIO_OUTDR(GPIO_D) &= ~0x2000; break;
    case 3: *GPIO_OUTDR(GPIO_D) &= ~0x4000; break;
    case 4: *GPIO_OUTDR(GPIO_D) &= ~0x8000; break;
  }
#endif
}

// Initialize keypad
void init_keypad() {
  // Configure port D
  // * pins [ 8:11] as digital input, pull-up
  // * pins [12:15] as digital output, open drain
#ifdef BUGS
  *GPIO_CFGHR(GPIO_D) = 0x88886666;
  *GPIO_OUTDR(GPIO_D) &= ~0xF000;
  *GPIO_OUTDR(GPIO_D) |=  0xF000;
#else
  *GPIO_CFGHR(GPIO_D) = 0x66668888;
  *GPIO_OUTDR(GPIO_D) &= ~0x0F00;
  *GPIO_OUTDR(GPIO_D) |=  0x0F00;
#endif
  // Deactivate all rows
  kbd_activate(0);
}
