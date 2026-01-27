#ifndef ASCII_H
#define ASCII_H

#define ASCII_B_E  (1 << 2)
#define ASCII_B_RW (1 << 1)
#define ASCII_B_RS (1 << 0)

#ifdef BUGS
#define ascii_ctrl_bit_set(mask)   *GPIO_OUTDR(GPIO_E) &=  ((unsigned char) mask)
#define ascii_ctrl_bit_clear(mask) *GPIO_OUTDR(GPIO_E) &= ~((unsigned char) mask)
#else
#define ascii_ctrl_bit_set(mask)   *GPIO_OUTDR(GPIO_E) |=  ((unsigned char) mask)
#define ascii_ctrl_bit_clear(mask) *GPIO_OUTDR(GPIO_E) &= ~((unsigned char) mask)
#endif

unsigned char ascii_read_controller();
unsigned char ascii_read_data();
unsigned char ascii_read_status();

void ascii_write_char(unsigned char c);
void ascii_write_cmd(unsigned char command);
void ascii_write_controller(unsigned char byte);
void ascii_write_data(unsigned char data);

void ascii_gotoxy(int x, int y);

void init_ascii();

#endif
