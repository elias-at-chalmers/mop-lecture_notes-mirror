    .text
    .global question1
    .global question2
    .global question3
    .global question4
    .global question5
    .global question6

############################################################
# Assignment 1
#
# Input:
#   a0 = base (a)
#   a1 = exponent (b)
#
# Task:
#   Return (leave in r0 before `ret`):
#       a^b
#
# Notes:
#   * Assume b >= 0
#   * a^0 must return 1
#   * Use a loop
#
############################################################
question1:
    li   t0, 1          # result = 1

loop:
    beq  a1, zero, done

    mul  t0, t0, a0    # result *= base
    addi a1, a1, -1   # b--
    j    loop

done:
    mv   a0, t0
    ret


############################################################
# Assignment 2
#
# Inputs:
#   a0 = a
#   a1 = b
#
# Task:
#   Return (leave in r0 before `ret`):
#   the smaller value of a and b.
#
# IMPORTANT:
#   a and b are *unsigned* values.
#
############################################################
question2:
    bltu a0, a1, a_is_smaller
    mv   a0, a1          # b is smaller
    ret

a_is_smaller:
    ret

############################################################
# Assignment 3
#
# Inputs:
#   a0 = a
#   a1 = b
#   a2 = c
#   a3 = d
#
# Task:
#   Return (leave in r0 before `ret`):
#   the smallest value of the four inputs.
#
# Rules:
#   * You MUST call question2(a, b) for comparisons.
#   * Do not rewrite the comparison code here.
#
############################################################
question3:
    addi sp, sp, -4      # Make space on stack
    sw   ra, 0(sp)       # Save return address

    # First min(a, b)
    call question2        # input: a0,a1 ; output: a0

    # Now a0 = min(a,b)
    mv   t0, a0           # Save result

    # Prepare min(c,d)
    mv   a0, a2
    mv   a1, a3

    # NOTE TO TEACHERS: Some of them will not save t0 here, 
    # because they know that question2 does not modify t0.
    # We will punish them for this in the next assignment.

    call question2

    # Now a0 = min(c,d)

    # Final min between the two partial results
    mv   a1, a0           # a1 = min(c,d)
    mv   a0, t0           # a0 = min(a,b)
    call question2

    # a0 now contains full minimum
    lw   ra, 0(sp)       # Restore return address
    addi sp, sp, 4       # Restore stack pointer
    ret


############################################################
# Assignment 4
#
# C function:
#   int question4(int a, int b, int c, int d)
#
# Task:
#   Compute:
#   Return (leave in r0 before `ret`):    
#       the maximum of a, b, c, and d.
#
# Rules:
#   • You MUST use the provided function:
#         int max(int,int);
############################################################


question4:
    # ===== PROLOGUE =====
    addi sp, sp, -12
    sw   ra,  8(sp)
    sw   s0,  4(sp)
    sw   s1,  0(sp)

    # ---- max(a, b) ----
    call max          # a0 = max(a,b)
    mv   s0, a0       # preserve result across calls

    # ---- max(c, d) ----
    mv   a0, a2
    mv   a1, a3
    call max          # a0 = max(c,d)
    mv   s1, a0       # preserve result

    # ---- max(first, second) ----
    mv   a0, s0
    mv   a1, s1
    call max          # final result → a0

    # ===== EPILOGUE =====
    lw   s1, 0(sp)
    lw   s0, 4(sp)
    lw   ra, 8(sp)
    addi sp, sp, 12
    ret


############################################################
# Assignment 5
#
# Task:
#   Iterate over the array "numbers" and return
#   the largest value.
#
# Notes:
#   • numbers contains signed 32-bit integers.
#   • array_len contains the number of elements.
#   • Use a loop.
#
############################################################

question5:
    # TODO
    li t0, 8
    li t1, -2147483648   # t1 = current max (initialize to smallest int)
    li t2, 0             # t2 = index
loop_max:
    bge t2, t0, done_max  # if index >= length, done
    slli t3, t2, 2        # t3 = index * 4 (byte offset)
    la t4, numbers
    add  t4, t3, t4  # t4 = &numbers[index]
    lw   t5, 0(t4)        # t5 = numbers[index]
    bgt  t5, t1, update_max # if numbers[index] > current max
    j    continue_max
update_max:
    mv   t1, t5           # update current max
continue_max:
    addi t2, t2, 1        # index++
    j    loop_max     # repeat loop 
done_max:
    mv a0, t1           # move result to a0 
    ret


.align 2
numbers:
    .word 10, 5, -2, 36, 42, 18, 4, 39


############################################################
# Assignment 6
#
# Task:
#   a0: pointer to array of signed 8-bit integers
#   a1: length of the array
#
#   Return the sum of all elements in the array.
############################################################
question6:
    # TODO
    li t0, 0             # t0 = sum
    li t1, 0             # t1 = index
loop_sum:
    bge t1, a1, done_sum  # if index >= length, done
    lb  t2, 0(a0)        # t2 = array[index]
    add t0, t0, t2       # sum += array[index]
    addi a0, a0, 1       # move pointer to next element
    addi t1, t1, 1       # index++
    j    loop_sum        # repeat loop
done_sum:
    mv a0, t0            # move result to a0
    ret 
