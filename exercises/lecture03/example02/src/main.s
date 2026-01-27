###############################################################################
# Lecture 03 - Example 02
# ============================================================================
# The C standard library (which this program links against) provides the 
# functions: 
# 
# int putchar(int c) - Prints the ASCII character corresponding to c.
#                      Returns -1 on error, otherwise returns the character 
#                      printed.
# 
# Use this function to write another function: 
# void print_string(address, length)
# 
# You need to connect a console to USART1 in the simulator.
###############################################################################
.section .text
.global main

# These lines tell the assembler that these functions are defined elsewhere
.extern putchar

print_string: 
    ## PROLOGUE ###############################################################
    addi sp, sp, -12
    sw ra, 8(sp)
    sw s1, 4(sp)
    sw s0, 0(sp)
    ###########################################################################
    li t0, 0            # Initialize index i = 0
    mv s0, a0           # s0 = address to current character
    mv s1, a1           # s1 = length
loop:
    bge t0, s1, done    # if i >= length, exit loop
    lb a0, 0(s0)        # Load byte at address s0 into a0 (current character)
    call putchar        # Call putchar to print the character
    addi s0, s0, 1      # Move to the next character
    addi t0, t0, 1      # i++
    j loop
done: 
    ## EPILOGUE ###############################################################
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw ra, 8(sp)
    addi sp, sp, 12
    ###########################################################################
    ret

main:

loop2: 

    la a0, string1      # Load address of string1
    li a1, 4            # Length of string1
    call print_string

    la a0, string2      # Load address of string2
    li a1, 3            # Length of string2
    call print_string

    la a0, string3      # Load address of string3
    li a1, 5            # Length of string3
    call print_string

    j loop2

string1: .ascii "MOP\n"
string2: .ascii "is\n"
string3: .ascii "fun!\n"

