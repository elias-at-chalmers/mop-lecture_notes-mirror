###############################################################################
# Lecture 03 - Example 01
# ============================================================================
# The C standard library (which this program links against) provides the 
# functions: 
# 
# int putchar(int c) - Prints the ASCII character corresponding to c.
#                      Returns -1 on error, otherwise returns the character 
#                      printed.
# 
# int rand(void) - Returns a pseudo-random integer in the range 0 to 32767
#
# ============================================================================
# Write a program that continually renders random, lower case, letters to 
# the console. 
# 
# You need to connect a console to USART1 in the simulator.
###############################################################################
.section .text
.global main

# These lines tell the assembler that these functions are defined elsewhere
.extern putchar
.extern rand

write_random_char:
    ## PROLOGUE ###############################################################
    sw ra, 0(sp)
    addi sp, sp, -4
    ###########################################################################

    # Get a random integer into a0
    call rand
    # Convert it to a lower case letter
    li t0, 26
    remu    a0, a0, t0        # Map to range 0-25
    addi     a0, a0, 'a'       # Map to ASCII 'a

    # Print the character
    call    putchar

    ## EPILOGUE ###############################################################
    addi sp, sp, 4
    lw ra, 0(sp)
    ###########################################################################
    ret

main:
    mv a0, a1
    # Infinite loop
.loop:
    call    write_random_char
    j    .loop   
    ret


