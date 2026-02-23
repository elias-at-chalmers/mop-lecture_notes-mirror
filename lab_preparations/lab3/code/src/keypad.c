#include "keypad.h"
#include "gpio.h"
#include "systick.h"

// Read keypad and return pressed key value (0xFF if none pressed)
unsigned char keyb() {
    const unsigned char key_values[4][4] = {
        {0x1, 0x2, 0x3, 0xA},
        {0x4, 0x5, 0x6, 0xB},
        {0x7, 0x8, 0x9, 0xC},
        {0xE, 0x0, 0xF, 0xD},
    };
    unsigned int row;
    unsigned int col;
    // For each row:
    for (row = 1; row <= 4; row++) {
        // Activate row
        kbd_activate(row);
        // Slight delay to allow signals to propagate
        for (volatile int i = 0; i < 1000; i++)
            ;
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
    // Check pins [0:3] (active if 0 due to pull-up)
    if ((indr & 0x0001) == 0)
        return 1;
    if ((indr & 0x0002) == 0)
        return 2;
    if ((indr & 0x0004) == 0)
        return 3;
    if ((indr & 0x0008) == 0)
        return 4;
    return 0;
}

// Activate given row ∈ [1..4]
void kbd_activate(unsigned int row) {
    // Deactivate all rows by default
    *GPIO_OUTDR(GPIO_D) |= 0x00F0;
    // Activate given row [1..4] corresponding to pins [4:7]
    // (activate by clearing due to open-drain)
    switch (row) {
    case 1:
        *GPIO_OUTDR(GPIO_D) &= ~0x0010;
        break;
    case 2:
        *GPIO_OUTDR(GPIO_D) &= ~0x0020;
        break;
    case 3:
        *GPIO_OUTDR(GPIO_D) &= ~0x0040;
        break;
    case 4:
        *GPIO_OUTDR(GPIO_D) &= ~0x0080;
        break;
    }
}
