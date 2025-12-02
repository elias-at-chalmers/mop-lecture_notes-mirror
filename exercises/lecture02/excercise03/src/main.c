#include <stdio.h>
#include <stdint.h>

extern void question1(void);
extern int  question2(void);
extern int  question3(void);
extern void question4(int x);
extern int  question5(void);
extern int  question6(void);
extern int  question7(int a);

extern int shared_value;
extern char byte_array[4];

int main(void)
{
    printf("Testing memory exercises:\n");
    int num_passed = 0;

    /*
     * Assignment 1: Write constant to global variable
     */
    printf("Assignment 1: ... ");
    shared_value = 0;
    question1();
    if(shared_value == 42) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed (got %d)\n", shared_value);
    }

    /*
     * Assignment 2: Read global variable
     */
    printf("Assignment 2: ... ");
    shared_value = 123;
    if(question2() == 123) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    /*
    * Assignment 3: raw SRAM copy
    */
    printf("Assignment 3: ... ");

    volatile uint32_t *src = (volatile uint32_t *)0x20010000;
    volatile uint32_t *dst = (volatile uint32_t *)0x20010004;

    *src = 42;
    *dst = 0;

    question3();

    if (*dst == 42) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }
    /*
     * Assignment 4: Write function parameter to memory
     */
    printf("Assignment 4: ... ");
    shared_value = 0;
    question4(77);
    if(shared_value == 77) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }

    /*
     * Assignment 5: Read then modify memory
     */
    printf("Assignment 5: ... ");
    shared_value = 20;
    if(question5() == 42) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed (got %d)\n", question5());
    }

    /*
    * Assignment 6: signed byte load
    */
    printf("Assignment 6: ... ");
    if (question6() == -5) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed (got %d)\n", question6());
    }

    /*
    * Assignment 7: unsigned short load
    */
    printf("Assignment 7: ... ");
    if (question7(-30000) == 0x10000) {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed (got %d)\n", question7(-30000));
    }

    printf("\nYou passed %d out of 5 assignments.\n", num_passed);
}