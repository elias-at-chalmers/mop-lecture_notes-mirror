//############################################################
//# You should not be here!!!
//# Go to assignments.s
//#############################################################

#include <stdio.h>
#include <stdint.h>

extern void sum_of_array(void);
extern volatile int sum_result;


int main(void)
{
    volatile uint32_t *arr_words = (volatile uint32_t *)0x20018000;
    arr_words[0] = 1;
    arr_words[1] = 2;
    arr_words[2] = 3;
    arr_words[3] = 4;
    arr_words[4] = 5;
    arr_words[5] = -0xF;

    printf("Testing sum_of_array() function... \n");
    sum_of_array();
    if (sum_result != 0xF) {
        printf("Failed. Expected sum_result is 0xF, got %x \n", sum_result);
    } 
    else{
        printf("Passed.\n Brilliant!\n");
    }

}
