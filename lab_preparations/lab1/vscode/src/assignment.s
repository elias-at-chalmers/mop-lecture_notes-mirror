###############################################################################
# Lab 1 - Preparation Task
#
#         Your task is to write an assembler function which will be tested 
#by external C code. 
#         The test harness will output results on the USART, so, if you are
#         running on Simserver, you need to attach a Console to USART1:
#         Serial communication interface 1 -> 06 Console
#
#         Start with Assignment 1 and work your way through the rest of the 
#         assignments in order.
###############################################################################

# Make the assembly functions visible to main.c (C test harness code)
.global copyvec, hidden, copyelements 

hidden:
    ###########################################################################
    # Assignment 1: 
    # This short assignment will teach you to use the debugger and memory view.
    # The function is called from the C startup file, so place a breakpoint 
    # on the first line in this function (`ret`). 
    #
    # The only parameter to this function is the address of an array we have 
    # hidden somewhere in memory. Before calling this function, the main
    # function will have placed that address in register a0 (first parameter
    # register).
    #
    # 1.1:  Use the "register" list to the left to find out what that address is
    #       and write it down. (you will use this to pass the assignment)
    #
    # 1.2:  Open the "memory" view below and press the `+` button to create 
    #       a new view. Type in the address you just found, to see what is 
    #       in memory at that address. 
    #     
    #       On the right side of the memory, each byte of memory is decoded as 
    #       an ascii character. What is the message we have hidden? 
    #
    # Once you have written down the address and the message, and feel like
    # you understand what the memory viewer does, move on to the next
    # assignment. 
    ###########################################################################
    ret


###############################################################################
# void copyvec(int src[], short dst[], int size);
# ================================================
# The funtion should copy `size` elements from the array src to the array `dst`
# You may assume that both arrays contain at least `size` elements.
# 
# Input: 
#   a0: The address to the first element of an array called "src" 
#       containing elements of type signed int
#   a1: The address to the first element of an array called "dst"
#       containing elements of type signed short (2 bytes)
#   a2: The number of elements to copy
###############################################################################

copyvec: 
    ###########################################################################
    # Assignment 2: 
    # Write a loop that copies `size` elements from src to dst
    # 
    # If you are having trouble, use the debugger and memory view (as in the
    # first assignment) to see what is getting copied. 
    ###########################################################################
    li t0, 0                        # t0: loop counter
    mv t1, a0                       # Address to src element
    mv t2, a1                       # Address to dst element
loop: 
    beq t0, a2, done                # If loop counter eached size, we are done
    lw t3, 0(t1)                    # Load element from src
    addi t1, t1, 4                  # Move t1 to next element
    sh t3, 0(t2)                    # Store element in dst
    addi t2, t2, 2                  # Move t2 to next element
    addi t0, t0, 1                  # Increase loop counter
    j loop
done: 
    ret                             # Return from function

###############################################################################
# int copyelements(int src[], short dst[], int start, int end);
# ===============================================================
# This function shall copy PART of the src array to the dst array.
# You MUST use the copyvec function for this.
# 
# Input: 
#   a0: The address to the first element of an array called "src" 
#       containing elements of type signed int
#   a1: The address to the first element of an array called "dst"
#       containing elements of type signed short (2 bytes)
#   a2: The first element to copy
#   a3: The last element to copy
#
# Output: 
#   a0: The function shall return the number of elements copied
###############################################################################

copyelements: 
    
    ###########################################################################
    # Assignment 4 (not 3, do 3 first): 
    #
    # Remember to push any registers (that are callee saved) to the stack
    ###########################################################################
    addi sp, sp, -8
    sw s0, 4(sp)
    sw ra, 0(sp)

    ###########################################################################
    # Assignment 3: 
    # Implement this pseudo code 
    # 
    # src_start = <address to src[start]>
    # dst_start = <address to dst[start]>
    # size = <number of elements to copy>
    # copyvec(src_start, dst_start, size)
    #
    # To copy the correct elements
    #
    # Then place a breakpoint at the `ret` instruction and check with the
    # view if the copying worked.
    ###########################################################################
    li t0, 4
    mul t0, a2, t0                  # Put offset from src in t0
    add t0, a0, t0                  # t0 is src_start
    li t1, 2
    mul t1, a2, t1                  # Put offset from src in t0
    add t1, a1, t1                  # t1 is dst_start
    sub t2, a3, a2                  
    addi s0, t2, 1                  # s0 is number of elements
    mv a0, t0
    mv a1, t1
    mv a2, s0
    call copyvec
    mv a0, s0                       # s0 is safe to use here

    ###########################################################################
    # Assignment 5:
    #
    # Remember to restore any values you pushed to the stack.
    ###########################################################################
    lw s0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8

    ret

###############################################################################
# Assignment 6:
# If all tests passed, the secret password will be printed in the console.
# Submit that along with the hidden message and address in Canvas.
###############################################################################