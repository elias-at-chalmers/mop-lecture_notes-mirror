#ifndef ASCII_H
#define ASCII_H

// Control bits
#define ASCII_B_E  (1 << 2)
#define ASCII_B_RW (1 << 1)
#define ASCII_B_RS (1 << 0)

// Control bit set/clear helpers
#ifdef BUGS
#define ascii_ctrl_bit_set(mask)   *GPIO_OUTDR(GPIO_E) &=  ((unsigned char) mask)
#define ascii_ctrl_bit_clear(mask) *GPIO_OUTDR(GPIO_E) &= ~((unsigned char) mask)
#else
#define ascii_ctrl_bit_set(mask)   *GPIO_OUTDR(GPIO_E) |=  ((unsigned char) mask)
#define ascii_ctrl_bit_clear(mask) *GPIO_OUTDR(GPIO_E) &= ~((unsigned char) mask)
#endif

// Read status or data
// (control bits RS and RW are assumed to be set accordingly before calling)
unsigned char ascii_read_controller();

// Read data
unsigned char ascii_read_data();

// Read status
unsigned char ascii_read_status();

// Output given char to display
void ascii_write_char(unsigned char c);

// Write command
void ascii_write_cmd(unsigned char command);

// Write command or data
// (control bits RS and RW are assumed to be set accordingly before calling)
void ascii_write_controller(unsigned char byte);

// Write data
void ascii_write_data(unsigned char data);

// Select ASCII display cursor position
// x ∈ [1..20] (column)
// y ∈ [1..2]  (row)
void ascii_gotoxy(int x, int y);

// Initialize ASCII display
void init_ascii();

#endif
