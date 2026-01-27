#ifndef KEYPAD_H
#define KEYPAD_H

unsigned char keyb();
unsigned int kbd_getcol();

void kbd_activate(unsigned int row);

void init_keypad();

#endif
