############################################################
# Lecture 03 - Exercise 02
#
# This is a debugging exercise: you getting working code
# with an error in it. Your task is to find and fix the error.
#
# `sum_of_array` is the entry point to this code (line 37).
# Set a breakpoint there, step through the program,
# verify that the correct data is fetched from memory.
#
# The function should calculate the sum of first 5 elements
# of the arrays stored at address 0x20018000.
# Variables `array_addres` and `n_words` define those constants.
# The result should be stored into `sum_result`.
#
# Before you start: 
# Start simserver and choose Server->IO Setup
# For "Serial Communication Interface 1", choose "06 Console".
# If you let the program run fully (press F5), the test
# result will be printed to console.
#
# Note: see an Introduction to Debugging on Canvas if you
#       do not know how to start.
#
############################################################


    .globl sum_of_array
    .globl sum_result

    .data
    .align 2
array_address:   .word 0x20018000    # address of the array with data
n_words:        .word 5             # number of elements to sum
sum_result:     .word 0             # the result should be saved in this variable

    .text
#void sum_of_array(void);
sum_of_array:                       # entry point into the program
    lw   a0, array_address
    lw   a1, n_words

    addi sp, sp, -4                 # save the return address to the stack
    sw   ra, 0(sp)

    jal  ra, sum_array              # call ´sum_array´ subroutine

    lw   ra, 0(sp)                  # restore the return address from the stack
    addi sp, sp, 4


    la   t0, sum_result             # save the result of ´sum_array´ into ´sum_result´
    sw   a0, 0(t0)

    ret

# int sum_array(int* a, int n);
# calculates sum of ´n´ elements in array ´a´
# returns the sum in register a0
sum_array:
    mv   t0, zero                # t0 is the running sum; sum = 0
    mv   t1, zero                # t1 is the index of an element in the array; i = 0
    mv   t2, a0                  # t2 is the address of the array;

# while(i<n) {sum+=a[i]};
loop:
    bge  t1, a1, loop_exit
    lw   t3, 0(t2)
    add  t0, t0, t3
    addi t2, t2, 4               
    addi t1, t1, 1
    j    loop

loop_exit:
    mv   a0, t0                  # return value in a0

    ret
   
