#include <stdio.h>
#include <stdint.h>

extern short global_a, global_b;


void swap(int32_t *a, int32_t *b);
void swap2(int32_t address_to_a, int32_t address_to_b);
void swap_arrays(int32_t *a, int32_t *b, int32_t length);
void filter_array(const uint32_t *a, int32_t length, 
                  uint8_t *b, int32_t *num_copied);
void set_first_element(short *fifth_element, short *ptr_to_slightly_before_a);
void swap_globals(short ** ptr_to_ptr_to_a, short ** ptr_to_ptr_to_b);




///////////////////////////////////////////////////////////////////////////////
// Assignment 6: This one is a bit tricky. You have two global variables
//               `global_a` and `global_b` of type `short`. Your function
//               shall swap these values and then put a pointer to `global_a`
//               into the location pointed to by `ptr_to_ptr_to_a` and a 
//               pointer to `global_b` into the location pointed to by 
//               `ptr_to_ptr_to_b`.
///////////////////////////////////////////////////////////////////////////////


int main(void)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 1
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 1...\n");
    int32_t a = 10;
    int32_t b = 20;
    int32_t old_a = a, old_b = b;
    printf("Before swap: x = %d, y = %d\n", a, b);
    swap(&a, &b);
    printf("After swap: x = %d, y = %d\n", a, b);   
    if(a == old_b && b == old_a) printf("PASSED!\n\n");
    else printf("FAILED!\n\n");
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 2
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 2...\n");
    a = 30;
    b = 40;
    old_a = a, old_b = b;
    printf("Before swap2: a = %d, b = %d\n", a, b);
    swap2((int32_t)&a, (int32_t)&b);
    printf("After swap2: a = %d, b = %d\n", a, b);
    if(a == old_b && b == old_a) printf("PASSED!\n\n");
    else printf("FAILED!\n\n");
    ///////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // Test assignment 3
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 3...\n");
    #define LENGTH 5
    int32_t arr1[LENGTH] = {1, 2, 3, 4, 5};
    int32_t arr2[LENGTH] = {6, 7, 8, 9, 10};
    int32_t old_arr1[LENGTH] = {1, 2, 3, 4, 5};
    int32_t old_arr2[LENGTH] = {6, 7, 8, 9, 10};
    printf("Before swap_arrays:\n");
    printf("arr1: ");
    for(int i = 0; i < LENGTH; i++) printf("%d ", arr1[i]);
    printf("\narr2: ");
    for(int i = 0; i < LENGTH; i++) printf("%d ", arr2[i]);
    printf("\n");
    swap_arrays(arr1, arr2, LENGTH);
    printf("After swap_arrays:\n");
    printf("arr1: ");
    for(int i = 0; i < LENGTH; i++) printf("%d ", arr1[i]);
    printf("\narr2: ");
    for(int i = 0; i < LENGTH; i++) printf("%d ", arr2[i]);
    printf("\n");
    int pass = 1; 
    for(int i=0; i < LENGTH; i++) 
        if(arr1[i] != old_arr2[i] || arr2[i] != old_arr1[i]) 
            pass = 0; 
    if(pass) printf("PASSED!\n\n");
    else printf("FAILED!\n\n");
    ///////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // Test assignment 4
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 4...\n");
    int32_t input[] = {100, 256, 50, 300,
                            200, 128, 512, 0xFF};
    int32_t length = sizeof(input)/sizeof(input[0]);
    uint8_t output[length];
    int32_t num_copied = 0;
    int32_t expected_num_copied = 4; // 100, 50, 200, 128, 0xFF is not < 0xFF
    uint8_t expected_output[] = {100, 50, 200, 128};
    printf("Input array: ");
    for(int32_t i = 0; i < length; i++) printf("%d ", input[i]);
    printf("\n");
    filter_array((const uint32_t*)input, length, output, &num_copied);
    printf("Output array: ");
    for(int32_t i = 0; i < expected_num_copied; i++) printf("%d ", output[i]);
    printf("\nNumber of copied elements: %d\n", num_copied);
    pass = 1;
    if(num_copied != expected_num_copied) pass = 0;
    for(int32_t i = 0; i < expected_num_copied; i++)
        if(output[i] != expected_output[i]) pass = 0;
    if(pass) printf("PASSED!\n\n");
    else printf("FAILED!\n\n");
    ///////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // Test assignment 5
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 5...\n");
    short arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int a_var = 0;
    short *fifth_element = &arr[4];
    short *ptr_to_slightly_before_a = (short *)((uint8_t *)&        a_var - 6);
    printf("Before set_first_element:\n");
    printf("arr[0] = %d\n", arr[0]);
    printf("a_var = 0x%X\n", a_var);
    set_first_element(fifth_element, ptr_to_slightly_before_a);
    printf("After set_first_element:\n");
    printf("arr[0] = %d\n", arr[0]);
    printf("a_var = 0x%X\n", a_var);
    if(arr[0] == 42 && a_var == 0xDEADBEEF) printf("PASSED!\n\n");
    else printf("FAILED!\n\n");
    /////////////////////////////////////////////////////////////////////////////// 

    
    //////////////////////////////////////////////////////////////////////////////
    // Test assignment 6
    ///////////////////////////////////////////////////////////////////////////////
    int old_global_a = global_a;
    int old_global_b = global_b;
    printf("Testing assignment 6...\n");
    printf("global_a = %d\n", global_a);
    printf("global_b = %d\n", global_b);
    printf("Running swap_globals()...\n");
    short *ptr_a = NULL, *ptr_b = NULL;
    swap_globals(&ptr_a, &ptr_b);
    printf("After swap_globals():\n");
    printf("global_a = %d\n", global_a);
    printf("global_b = %d\n", global_b);
    if(global_a == old_global_b && global_b == old_global_a) 
        printf("PASSED!\n\n");
    else 
        printf("FAILED!\n\n");
    /////////////////////////////////////////////////////////////////////////////
}

