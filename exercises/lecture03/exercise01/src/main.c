#include <stdio.h>
#include <stdint.h>

extern int question1(int a, int b);
extern unsigned int question2(unsigned int a, unsigned int b);
extern unsigned question3(unsigned a, unsigned b,
                           unsigned c, unsigned d);
extern int question4(int a, int b,
                           int c, int d);
extern int question5(void);
extern int question6(signed char* array, int length);



int main(void)
{
    int num_passed = 0;

    printf("Assignment 1: ... ");

    if (
        question1(2, 0) == 1   &&
        question1(2, 1) == 2   &&
        question1(2, 4) == 16  &&
        question1(3, 3) == 27 &&
        question1(5, 2) == 25
       )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }


    /*
    * Assignment 2: unsigned minimum
    */
    printf("Assignment 2: ... ");

    if (
        question2(3, 7) == 3 &&

        /* large unsigned corner cases */
        question2(0xFFFFFFFFu, 1) == 1 &&     // blt fails here
        question2(0x80000000u, 0) == 0 &&     // blt fails here

        question2(100, 200) == 100
    )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }


    /*
    * Assignment 3: unsigned min of four
    */
    printf("Assignment 3: ... ");

    if (
        question3(4, 7, 9, 2) == 2 &&
        question3(10, 20, 1, 30) == 1 &&
        question3(3, 2, 1, 0) == 0 &&
        question3(0xFFFFFFFFu, 5, 7, 9) == 5
    )
    {
        printf(" Passed\n");
        num_passed++;
    } else {
        printf(" Failed\n");
    }


    /*
    * Assignment 4 — signed max with register-clobbering callee
    */
    printf("Assignment 4: ... ");

    if (
        question4(1, 7, 4, 2) == 7 &&
        question4(-3, -1, -2, -4) == -1 &&
        question4(10, -10, 5, 6) == 10 &&
        question4(-100, 20, -50, 5) == 20 &&
        question4(0, 0, 0, -1) == 0
    )
    {
        printf(" Passed\n");
        num_passed++;
    }
    else {
        printf(" Failed\n");
    }

    
    /*
    * Assignment 5 — array max
    */
    {
        printf("Assignment 5: ... ");
        int result = question5();

        if (result == 42) {
            printf(" Passed\n");
            num_passed++;
        }
        else {
            printf(" Failed (got %d)\n", result);
        }
    }


    /*
    * Assignment 6 — array as argument
    */
    {
        signed char data_array[] = {
            10, 22, 110, 5, 3, 42, 17, 8, 33, 14, -110, 29
        };
        int sum = 0; 
        for(unsigned int i=0; i<sizeof(data_array); i++) {
            sum += data_array[i]; 
        }

        printf("Assignment 6: ... ");
        int result = question6(data_array, sizeof(data_array));

        if (result == sum) {
            printf(" Passed\n");
            num_passed++;
        }
        else {
            printf(" Failed (got %d)\n", result);
        }    
    }

    printf("Passed %d/6 tests\n", num_passed);
    if (num_passed == 6) {
        printf("All assignments passed! Well done!\n");
    }    

}


// Evil max function that clobbers all t-registers
int max(int a, int b)
{
    __asm volatile("li t0, 0xbeef;\n");
    __asm volatile("li t1, 0xbeef;\n");
    __asm volatile("li t2, 0xbeef;\n");
    __asm volatile("li t3, 0xbeef;\n");
    __asm volatile("li t4, 0xbeef;\n");
    __asm volatile("li t5, 0xbeef;\n");
    __asm volatile("li t6, 0xbeef;\n");
    return (a > b) ? a : b;
}   