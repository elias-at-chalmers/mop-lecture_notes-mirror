#ifndef KEYPAD_H
#define KEYPAD_H

// Read keypad and return pressed key value (0xFF if none pressed)
unsigned char keyb();

// Read columns, returning first active one left-to-right (0 if none active)
unsigned int kbd_getcol();

// Activate given row ∈ [1..4]
void kbd_activate(unsigned int row);

// Initialize keypad
void init_keypad();

#endif
