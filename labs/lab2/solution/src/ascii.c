#include "ascii.h"
#include "gpio.h"
#include "systick.h"

unsigned char ascii_read_controller() {
  unsigned char controller;
  ascii_ctrl_bit_set(ASCII_B_E);
#ifdef BUGS
#else
  delay_nano(360);
#endif
  controller = *GPIO_INDR(GPIO_E) >> 8;
  ascii_ctrl_bit_clear(ASCII_B_E);
  return controller;
}

unsigned char ascii_read_data() {
  unsigned char data;
  *GPIO_CFGHR(GPIO_E) &= ~0xFFFFFFFF;
  *GPIO_CFGHR(GPIO_E) |=  0x88888888;
  *GPIO_OUTDR(GPIO_E) &= ~0xFF00;
  ascii_ctrl_bit_set(ASCII_B_RS);
  ascii_ctrl_bit_set(ASCII_B_RW);
  data = ascii_read_controller();
  *GPIO_CFGHR(GPIO_E) &= ~0xFFFFFFFF;
  *GPIO_CFGHR(GPIO_E) |=  0x22222222;
  return data;
}

unsigned char ascii_read_status() {
  unsigned char status;
  *GPIO_CFGHR(GPIO_E) &= ~0xFFFFFFFF;
  *GPIO_CFGHR(GPIO_E) |=  0x88888888;
  *GPIO_OUTDR(GPIO_E) &= ~0xFF00;
  ascii_ctrl_bit_clear(ASCII_B_RS);
  ascii_ctrl_bit_set(ASCII_B_RW);
  status = ascii_read_controller();
  *GPIO_CFGHR(GPIO_E) &= ~0xFFFFFFFF;
  *GPIO_CFGHR(GPIO_E) |=  0x22222222;
  return status;
}

void ascii_write_char(unsigned char c) {
  while(ascii_read_status() & 0x80);
  delay_micro(8);
  ascii_write_data(c);
  delay_micro(43);
}

void ascii_write_cmd(unsigned char command) {
  while(ascii_read_status() & 0x80);
  delay_micro(8);
  ascii_ctrl_bit_clear(ASCII_B_RS);
  ascii_ctrl_bit_clear(ASCII_B_RW);
  ascii_write_controller(command);
}

void ascii_write_controller(unsigned char byte) {
  delay_nano(40);
  ascii_ctrl_bit_set(ASCII_B_E);
  *GPIO_OUTDR(GPIO_E) &= ~0xFF00;
  *GPIO_OUTDR(GPIO_E) |= ((unsigned short) byte) << 8;
  delay_nano(230);
  ascii_ctrl_bit_clear(ASCII_B_E);
  delay_nano(10);
}

void ascii_write_data(unsigned char data) {
  ascii_ctrl_bit_set(ASCII_B_RS);
  ascii_ctrl_bit_clear(ASCII_B_RW);
  ascii_write_controller(data);
}

void ascii_gotoxy(int x, int y) {
  unsigned char address = x - 1;
  if(y == 2) {
    address += 0x40;
  }
  ascii_write_cmd(0x80 | address);
  delay_micro(39);
}

// Port E 15-0: ASCII display
// - data (15-8) out (push/pull, 2 MHz)
// - ctrl (7-0)  out (push/pull, 2 MHz)
void init_ascii() {
  *GPIO_CFGHR(GPIO_E) &= ~0xFFFFFFFF;
  *GPIO_CFGHR(GPIO_E) |=  0x22222222;
  *GPIO_CFGLR(GPIO_E) &= ~0xFFFFFFFF;
  *GPIO_CFGLR(GPIO_E) |=  0x22222222;
  // Function set: 2 rows, 5x8 size
  ascii_write_cmd(0x38);
  delay_micro(39);
  // Display control: display on, cursor on, cursor non-blinking
  ascii_write_cmd(0x0E);
  delay_micro(39);
  // Clear display
  ascii_write_cmd(0x01);
  delay_micro(1530);
  // Entry mode set: increment, shift on
  ascii_write_cmd(0x03);
  delay_micro(39);
}
