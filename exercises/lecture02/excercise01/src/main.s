// This line makes the main function visible to the linker
.globl main

// We put all of out code in the text section
.text

// You program starts here
main:
    ###########################################################################
    # Assignment 1: Calculate t0 = a + b
    # =================================================
    # Check that you have the correct value in t0 using "register list" 
    # in the debugger
    ###########################################################################

    ###########################################################################
    # Assignment 3: Store the value of t0 into variable c
    # ==================================================================
    # What is the address of c?
    # Open the memory view in the debugger to check that c has the correct value
    # If not, or if your simserver yells at you, check the hint in assignment 2
    ###########################################################################

// Forever loop
end: j end

// We put all of our data in the data section
.data

.align 1
a: .hword 10
b: .byte 20
###########################################################################
# Assigment 2: Declare a variable c that is a signed 4-byte integer and
#               initialize it to 1000
# =================================================
# (hint: don't forget about alignment!)
###########################################################################

