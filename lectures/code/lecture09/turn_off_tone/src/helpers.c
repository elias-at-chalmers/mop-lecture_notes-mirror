#include <stdlib.h>
#include "helpers.h"

uint8_t GetTemperature() 
{
    static int temp = 5; 
    int xi = (rand() & 0xFFFF) << 8 | (rand() & 0xF);
    if(xi == 1) {temp += 1;}
    else if(xi == 2) {temp -= 1;}
    temp = MAX(MIN(temp, 9), 0);
    return temp; 
} 

uint8_t Get7SegCode(uint8_t num) 
{
    // This function returns the 7-segment code for the given number (0-9)
    // The mapping is based on a common cathode 7-segment display
    const uint8_t seg_codes[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };
    if (num < 10) {
        return seg_codes[num];
    } else {
        return 0; // Return blank for numbers outside the range of 0-9
    }
}
