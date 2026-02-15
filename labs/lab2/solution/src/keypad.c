#include "gpio.h"
#include "keypad.h"
#include "systick.h"

unsigned char keyb() {
  const unsigned char key_values[4][4] = {
      { '1', '2', '3', 'A' },
      { '4', '5', '6', 'B' },
      { '7', '8', '9', 'C' },
      { 'E', '0', 'F', 'D' },
  };
  unsigned int row;
  unsigned int col;
  for (row = 1; row <= 4; row++) {
    kbd_activate(row);
    // Speed set to 2 MHz
    // => period of 1s / (2 * 10^6) = 1 / 2 us = 500 ns
    // => need to wait at least 500 ns before checking columns
    delay_nano(500);
    col = kbd_getcol();
    if (col) {
      kbd_activate(0);
      return key_values[row - 1][col - 1];
    }
  }
  kbd_activate(0);
  return 0xFF;
}

unsigned int kbd_getcol() {
  unsigned short indr = *GPIO_INDR(GPIO_D);
  if ((indr & 0x0100) == 0) return 1;
  if ((indr & 0x0200) == 0) return 2;
  if ((indr & 0x0400) == 0) return 3;
  if ((indr & 0x0800) == 0) return 4;
  return 0;
}

void kbd_activate(unsigned int row) {
  *GPIO_OUTDR(GPIO_D) |= 0xF000;
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

// Port D 15-8: keypad
// - rows (15-12) out (open drain, 2 MHz)
// - cols (11-8)  in  (pull up)
void init_keypad() {
#ifdef BUGS
  *GPIO_CFGHR(GPIO_D) = 0x88886666;
  *GPIO_OUTDR(GPIO_D) &= ~0xF000;
  *GPIO_OUTDR(GPIO_D) |=  0xF000;
#else
  *GPIO_CFGHR(GPIO_D) = 0x66668888;
  *GPIO_OUTDR(GPIO_D) &= ~0x0F00;
  *GPIO_OUTDR(GPIO_D) |=  0x0F00;
#endif
  kbd_activate(0);
}
