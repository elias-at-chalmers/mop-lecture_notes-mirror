#include "ascii.h"
#include "gpio.h"
#include "systick.h"

// Read status or data
// (control bits RS and RW are assumed to be set accordingly before calling)
unsigned char ascii_read_controller() {
  unsigned char controller;
  // E = 1
  ascii_ctrl_bit_set(ASCII_B_E);
  // Delay 360 ns
#ifdef BUGS
#else
  delay_nano(360);
#endif
  // Read data register
  controller = *GPIO_INDR(GPIO_E) >> 8;
  // E = 0
  ascii_ctrl_bit_clear(ASCII_B_E);
  // Return data register
  return controller;
}

// Read data
unsigned char ascii_read_data() {
  unsigned char data;
  // Configure port E [8:15] as floating input
  *GPIO_CFGHR(GPIO_E) = 0x44444444;
  // RS = 1
  ascii_ctrl_bit_set(ASCII_B_RS);
  // RW = 1
  ascii_ctrl_bit_set(ASCII_B_RW);
  // Read data
  data = ascii_read_controller();
  // Configure port E [8:15] as digital output, push/pull, 2 MHz
  *GPIO_CFGHR(GPIO_E) = 0x22222222;
  // Return data
  return data;
}

// Read status
unsigned char ascii_read_status() {
  unsigned char status;
  // Configure port E [8:15] as floating input
  *GPIO_CFGHR(GPIO_E) = 0x44444444;
  // RS = 0
  ascii_ctrl_bit_clear(ASCII_B_RS);
  // RW = 1
  ascii_ctrl_bit_set(ASCII_B_RW);
  // Read status
  status = ascii_read_controller();
  // Configure port E [8:15] as digital output, push/pull, 2 MHz
  *GPIO_CFGHR(GPIO_E) = 0x22222222;
  // Return status
  return status;
}

// Output given char to display
void ascii_write_char(unsigned char c) {
  // Wait until status flag is clear, then another 8 µs
  while(ascii_read_status() & 0x80);
  delay_micro(8);
  // Write given char, then wait corresponding execution time (43 µs)
  ascii_write_data(c);
  delay_micro(43);
}

// Write command
void ascii_write_cmd(unsigned char command) {
  // Wait until status flag is clear, then another 8 µs
  while(ascii_read_status() & 0x80);
  delay_micro(8);
  // RS = 0
  ascii_ctrl_bit_clear(ASCII_B_RS);
  // RW = 0
  ascii_ctrl_bit_clear(ASCII_B_RW);
  // Write given command
  ascii_write_controller(command);
}

// Write command or data
// (control bits RS and RW are assumed to be set accordingly before calling)
void ascii_write_controller(unsigned char byte) {
  // Delay 40 ns
  delay_nano(40);
  // E = 1
  ascii_ctrl_bit_set(ASCII_B_E);
  // Write given byte to data register
  *GPIO_OUTDR(GPIO_E) &= ~0xFF00;
  *GPIO_OUTDR(GPIO_E) |= ((unsigned short) byte) << 8;
  // Delay 230 ns
  delay_nano(230);
  // E = 0
  ascii_ctrl_bit_clear(ASCII_B_E);
  // Delay 10 ns
  delay_nano(10);
}

// Write data
void ascii_write_data(unsigned char data) {
  // RS = 1
  ascii_ctrl_bit_set(ASCII_B_RS);
  // RW = 0
  ascii_ctrl_bit_clear(ASCII_B_RW);
  // Write given data
  ascii_write_controller(data);
}

// Select ASCII display cursor position
// x ∈ [1..20] (column)
// y ∈ [1..2]  (row)
void ascii_gotoxy(int x, int y) {
  // x and y need to be translated to an address:
  // * [0..19]      (row 1)
  // * [0..19] + 64 (row 2)
  unsigned char address = x - 1;
  if(y == 2) {
    address += 0x40;
  }
  // Set address (bits [0:6] = address, bit 7 = 1),
  // then wait corresponding execution time (39 µs)
  ascii_write_cmd(0x80 | address);
  delay_micro(39);
}

// Initialize ASCII display
void init_ascii() {
  // Configure port E [8:15] as digital output, push/pull, 2 MHz
  *GPIO_CFGHR(GPIO_E) = 0x22222222;
  // Configure port E [0:7] as digital output, push/pull, 2 MHz
  *GPIO_CFGLR(GPIO_E) = 0x22222222;
  // Function set: 2 rows, 5x8 size (execution time: 39 µs)
  ascii_write_cmd(0x38);
  delay_micro(39);
  // Display control: display on, cursor on, cursor non-blinking
  // (execution time: 39 µs)
  ascii_write_cmd(0x0E);
  delay_micro(39);
  // Clear display (execution time: 1.53 ms)
  ascii_write_cmd(0x01);
  delay_micro(1530);
  // Entry mode set: increment, shift on (execution time: 39 µs)
  ascii_write_cmd(0x03);
  delay_micro(39);
}
