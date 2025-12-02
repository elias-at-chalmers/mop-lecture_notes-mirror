    .text
    .global question1
    .global question2
    .global question3
    .global question4
    .global question5
    .global question6
    .global question7
    .global shared_value

############################################################
# Global variables (students are NOT meant to modify these)
############################################################

    .data
    .align 2
shared_value:
    .word 0
signed_byte:    
    .byte -5
    .byte 0
    .byte 0
    .byte 0
unsigned_half: 
    .half -30000
    .half 0
.text


############################################################
# Assignment 1
#
# Task:
#   Store the value 42 into the global variable 'shared_value'
#
############################################################
question1:
    # TODO
    la t0, shared_value   # Load address of shared_value
    li t1, 42             # Load immediate value 42
    sw t1, 0(t0)          # Store 42 into shared_value

    ret


############################################################
# Assignment 2
#
# Task:
#   Load the value from 'shared_value' and
#   return it in a0.
#
############################################################
question2:
    # TODO
    la t0, shared_value   # Load address of shared_value
    lw a0, 0(t0)          # Load value from shared_value into a0

    ret

############################################################
# Assignment 3
#
# Task:
#   Read the word stored at address 0x20010000
#   and write the same word to address 0x20010004.
#
# Notes:
#   Both addresses are guaranteed to be valid SRAM
#   locations. Do not use the stack.
#
############################################################
question3:
    # TODO
    li t0, 0x20010000     # Load source address
    lw t1, 0(t0)          # Load word from source address
    li t2, 0x20010004     # Load destination address
    sw t1, 0(t2)          # Store word to destination address
    
    ret

############################################################
# Assignment 4
#
# Input:
#   a0 = integer x
#
# Task:
#   Store x into shared_value.
#
############################################################
question4:
    # TODO
    la t0, shared_value   # Load address of shared_value
    sw a0, 0(t0)          # Store x (in a0) into shared_value    

    ret


############################################################
# Assignment 5
#
# Task:
#   Load 'shared_value', add 22 to it, and
#   return the result in a0.
#
############################################################
question5:
    # TODO
    la t0, shared_value   # Load address of shared_value
    lw t1, 0(t0)          # Load value from shared_value
    li t2, 22             # Load immediate value 22
    add a0, t1, t2        # Add shared_value + 22 and store in a0
    
    ret

############################################################
# Assignment 6
#
# Task:
#   Read the value at label 'signed_value2' 
#   and return it.
#
# Notes:
#   * 'signed_value2' is a signed 8-bit value.
#
############################################################
question6:
    la   t0, signed_byte
    lb   a0, 0(t0)
    ret    

############################################################
# Assignment 7
#
# Task:
#   Read the value at label 'unsigned_half1' and subtract
#   the value in a0
#   and return it.
#
# Notes:
#   * 'unsigned_half' is an unsigned 16-bit value.
#
############################################################
question7:
    # TODO
    la   t0, unsigned_half
    lhu  a1, 0(t0)
    sub a0, a1, a0
    ret    
