#include <stdio.h>
#include <stdint.h>
#include <limits.h>

extern int question1(int a);
extern int question2(int a, int b);
extern int question3(int a);
extern int question4(int a);
extern int question5(int a);

int main(void)
{
    printf("Testing your solutions:\n");
    int num_passed = 0;

    /*
     * Assignment 1: multiply by 2
     */
    printf("Assignment 1: ... ");
    if (
        question1(1)   == 2  &&
        question1(7)   == 14 &&
        question1(-5)  == -10
       )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    /*
     * Assignment 2: multiply two numbers
     */
    printf("Assignment 2: ... ");
    if (
        question2(6, 7)     == 42 &&
        question2(-4, 5)    == -20 &&
        question2(-3, -8)  == 24
       )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    /*
     * Assignment 3: compute a^2 + 10
     */
    printf("Assignment 3: ... ");
    if (
        question3(2)   == 14 &&
        question3(-4)  == 26 &&
        question3(7)   == 59
       )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    /*
    * Assignment 4: detect negative short
    */
    printf("Assignment 4: ... ");
    if (
        question4((short)5) == 0 &&
        question4((short)-3) == 1 &&
        question4((short)0) == 0 &&
        question4((short)32767) == 0 &&
        question4((short)-32768) == 1
    )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    /*
     * Assignment 5: no MUL allowed
     * Multiply input by 7 using shifts/adds only
     */
    printf("Assignment 5: ... ");
    if (
        question5(6)   == 42 &&
        question5(-3)  == -21 &&
        question5(10)  == 70
       )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    printf("\nYou passed %d out of 5 assignments.\n", num_passed);
    if(num_passed == 5) {
        printf("Congratulations!\n");
    } else {
        printf("Keep trying!\n");
    }   
}