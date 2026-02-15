#include "ascii.h"
#include "gpio.h"
#include "keypad.h"
#include "systick.h"

// keypad     <-> D[15..8]
// ascii_data <-> E[15..8]
// ascii_ctrl <-> E[ 7..0]
void init_app() {
  init_keypad();
  init_ascii();
}

int main() {
  unsigned char input;
  // Initialize keypad and ASCII display
  init_app();
#ifdef TESTS
  // Task 1.2
#define MS 0 // Change the number to something reasonable!
  delay_milli(MS);
  delay_micro(1000 * MS);
  delay_nano(1000 * 1000 * MS);
  // Task 1.4
  *GPIO_OUTDR(GPIO_E) &= ~0x00FF;
  ascii_ctrl_bit_set(1 << 7);
  unsigned char ctrl = *GPIO_OUTDR(GPIO_E) & 0xFF;
  // Check here that bit 7 of ctrl is set
  ascii_ctrl_bit_clear(1 << 7);
  ctrl = *GPIO_OUTDR(GPIO_E);
  // Check here that bit 7 of ctrl is cleared
#endif
  // Forever:
  while(1) {
    // Move cursor back to start of row after filling it
    ascii_gotoxy(1, 1);
    for(int x = 0; x < 16; x++) {
      // Await input
      do {
        input = keyb();
      } while(input == 0xFF);
      // Print input
      ascii_write_char(input);
      // Avoid reading input too often
      delay_milli(250);
    }
  }
  return 0;
}
