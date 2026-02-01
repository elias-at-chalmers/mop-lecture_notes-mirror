///////////////////////////////////////////////////////////////////////////////
// Lecture 06 - Exercise 01
// ============================================================================
// In this exercise you will practice pointers in C. 
// Like previous exercises, you should connect a console to usart1 and 
// implement the functions below to pass all test. 
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#define SOLUTION 1  

///////////////////////////////////////////////////////////////////////////////
// Assignment 1: When the function returns, the value originally pointed to 
//               by `a` should be stored in the location pointed to by `b` and
//               vice versa. Implement the body of the function to achieve this.
///////////////////////////////////////////////////////////////////////////////
void swap(int32_t *a, int32_t *b)
{
    // Your code here
#if SOLUTION
    int32_t temp = *a;
    *a = *b;
    *b = temp;
#endif    
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 2: In this assignment, you get the addresses as integer values.
//               When the function returns, the value originally stored at the
//               address `address_to_a` should be stored at the address
//               `address_to_b` and vice versa. Implement the body of the
//               function to achieve this.
///////////////////////////////////////////////////////////////////////////////
void swap2(int32_t address_to_a, int32_t address_to_b)
{
    // Your code here
#if SOLUTION
    int32_t temp = *(int32_t *)address_to_a;
    *(int32_t *)address_to_a = *(int32_t *)address_to_b;
    *(int32_t *)address_to_b = temp;
#endif    
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3: `a` and `b` point to the beginning of two arrays of equal 
//               length `length`. When the function returns, the contents of
//               the two arrays should be swapped. Implement the body of the
//               function to achieve this.
///////////////////////////////////////////////////////////////////////////////
void swap_arrays(int32_t *a, int32_t *b, int32_t length)
{
    // Your code here   
#if SOLUTION
    for (int32_t i = 0; i < length; i++)
    {
        int32_t temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }   
#endif 
}   

///////////////////////////////////////////////////////////////////////////////
// Assignment 4: `a` points to an array of unsigned  integers of length 
//               `length`. All elements that are smaller than 0xFF shall be 
//               copied to the unsigned char array pointed to by `b`. The number
//               of copied elements shall be written into the location pointed to
//               by `num_copied`. Implement the body of the function to achieve
//               this.
///////////////////////////////////////////////////////////////////////////////
void filter_array(const uint32_t *a, int32_t length, 
                  uint8_t *b, int32_t *num_copied)
{
    // Your code here
#if SOLUTION
    int32_t count = 0;
    for (int32_t i = 0; i < length; i++)
    {
        if (a[i] < 0xFF)
        {
            b[count] = (uint8_t)a[i];
            count++;
        }
    }
    *num_copied = count;
#endif    
}   

///////////////////////////////////////////////////////////////////////////////
// Assignment 5: `fifth_element` points to the FIFTH element of an array of
//                shorts. Change the value of the FIRST element in the array 
//                to 42.
//                `ptr_to_slightly_before_a` points to an address 6 bytes
//                before a variable a of type unsigned int. Change the value
//                of `a` to 0xDEADBEEF.
///////////////////////////////////////////////////////////////////////////////
void set_first_element(short *fifth_element, short *ptr_to_slightly_before_a)
{
    // Your code here
#if SOLUTION
    // This actually works, but I doubt they will do this. 
    // Either way they will hopefully learn something about
    // pointer arithmetic.
    fifth_element[-4] = 42;
    unsigned int *a = (unsigned int *)(ptr_to_slightly_before_a + 3);
    *a = 0xDEADBEEF;
#endif
}



///////////////////////////////////////////////////////////////////////////////
// Assignment 6: This one is a bit tricky. You have two global variables
//               `global_a` and `global_b` of type `short`. Your function
//               shall swap these values and then put a pointer to `global_a`
//               into the location pointed to by `ptr_to_ptr_to_a` and a 
//               pointer to `global_b` into the location pointed to by 
//               `ptr_to_ptr_to_b`.
///////////////////////////////////////////////////////////////////////////////

short global_a = 42;
short global_b = 43; 

void swap_globals(short ** ptr_to_ptr_to_a, short ** ptr_to_ptr_to_b)
{
    // Your code here
#if SOLUTION
    short temp = global_a;  
    global_a = global_b;
    global_b = temp;
    *ptr_to_ptr_to_a = &global_a;
    *ptr_to_ptr_to_b = &global_b;
#endif
}


