///////////////////////////////////////////////////////////////////////////////
// Lecture 06 - Exercise 00
// ============================================================================
// In this exercise you will practice pointers to ports and pointers to constant 
// position in memory.
// Like previous exercises, you should connect a console to usart1 and 
// implement the functions below to pass all test. 
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#define SOLUTION 1

///////////////////////////////////////////////////////////////////////////////
// Assignment 1: pointer ´a´ points to a string of chracters ended by '\0'.
//               Your task is to make all lowercase letters in the english
//               alphabet uppercase. 
///////////////////////////////////////////////////////////////////////////////
void uppercase(char *a)
{
#if SOLUTION
    while (*a != '\0') {
        if (*a >= 'a' && *a <= 'z')
            *a += 'A' - 'a';
        a++;
    }
#else
    // Your code here
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 2: `a` and `b` point to two strings of chracters both ended by '\0'.
//               Your task is to return 1 if they are equal and 0 otherwise.
//               The comparison must be case insensitive for the english
//               alphabet.
///////////////////////////////////////////////////////////////////////////////
int comparecaseless(char *a, char *b)
{
#if SOLUTION
    char ac, bc;
    while ((ac = *a++) != '\0' && (bc = *b++) != '\0') {
        ac += ac >= 'a' && ac <= 'z' ? 'A' - 'a' : 0;
        bc += bc >= 'a' && bc <= 'z' ? 'A' - 'a' : 0;
        if (ac != bc)
            return 0;
    }
    return 1;
#else
    // Your code here
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3: write a #define statement for PORT, so that the function
//               ´read_second_byte()´ reads the second byte from the port
//               located in the memory by the address ´data_port_address´.
///////////////////////////////////////////////////////////////////////////////

extern const int data_port_address;
#if SOLUTION
#define PORT (volatile char*) (data_port_address + 1)
#else
    // Your code here
#endif

char read_second_byte()
{
    return *PORT;
}