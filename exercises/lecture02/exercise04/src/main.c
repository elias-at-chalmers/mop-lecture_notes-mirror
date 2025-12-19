#include <stdio.h>
#include <stdint.h>

extern int  question1(char code);
extern int  question2(char digit1, char digit2, char digit3);
extern unsigned int question3(unsigned char pattern);
extern int question4(unsigned char x);
extern int question5(void);

extern int shared_value;
extern char byte_array[4];

int main(void)
{
    printf("Arithmetic and logic manipulations:\n");
    int num_passed = 0;

    /*
     * Assignment 1: Convert ASCII character into a number 
     */
    printf("Question 1: ... ");
    int test1a = question1(0x30);
    int test1b = question1(0x39);
    if( test1a == 0 &&
        test1b == 9 ) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf("Failed. (0x30 converted into %d, 0x39 into %d)\n", test1a, test1b);
    }

    /*
     * Assignment 2: Convert an array of ASCII character into a number 
     */
    printf("Question 2: ... ");
    int test2a = question2('1', '2', '3');
    if( test2a == 123 ) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf("Failed. ('123' converted into %d)\n", test2a);
    }

    /*
     * Assignment 3: Propagate pattern
     */

    printf("Question 3: ... ");
    if( question3(0x55) == (unsigned int)0x55555555 &&
        question3(0xAA) == (unsigned int)0xAAAAAAAA ) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf("Failed.\n");
    }

    /*
     * Assignment 4: Calculate number of ones in a byte
     */

    printf("Question 4: ... ");
    if( question4(0b1010101) == 4 &&
        question4(0b0) == 0 ) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf("Failed.\n");
    }

    /*
     * Assignment 5: Signed/unsigned loads
     */

    printf("Question 5: ... ");
    if( question5() == 258) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf("Failed. Expected 258\n");
    }


    printf("\nYou passed %d out of 5 assignments.\n", num_passed);
}