###############################################################################
# Lab 1 - Preparation Task
#
#         Like many of the previous exercises, your task here is to write a 
#         function and the function will be tested by external C code. 
#         The test harness will output results on the USART, so, if you are
#         running on a simulator, you need to attach a Console to USART1
#
#         Start with Assignment 1 and work your way through the rest of the 
#         assignments in order.
###############################################################################


###############################################################################
# For the lab assignment, introduce the following bugs: 
#
# 1. Use a larger than int number as input. This requires them to use the 
#    debugger or memory viewer to inspect why the program fails. 
# 2. Change "addi t1, t1, 4" to 2. Will cause alignment errors, and force them 
#    to follow the program until it crashes. 
# 3. Use bge for this one: "beq t0, a2, done". Will cause an "off by one" error.
# 4. Do not use s0, and make sure that crashes. Will force them to think about 
#    register saving. 
# 5. Forget to save t registers before calling copy_elements
###############################################################################
.global main
.data 
.align 2
.equ FIRST, 9
.equ LAST, 16
.equ NUM_VALUES, 17
src: .word 1, 2, 4, 8, 16, 32, 64, 256, 512, 1024, 2048, 4096, 8192, 16384, 0xaa, 0xbb, 0xcc #32768, 65536, 131072
dst: .space NUM_VALUES*2

.text 

main: 
    ###########################################################################
    # Copy last half of src to dst
    ###########################################################################

    # (in s registers because we need them after the call as well)
    la s0, src      # Address to src 
    la s1, dst      # Address to dst
    li s2, FIRST    # First element to copy
    li s3, LAST     # Final element to copy

    # Fill arguments and call copyelements
    mv a0, s0
    mv a1, s1
    mv a2, s2
    mv a3, s3
    call copyelements

    ###########################################################################
    # Check that it worked
    ###########################################################################
    li t0, FIRST        # Loop counter, i
    li t1, LAST

testloop: 
    # Calculate address to src[i], and read    
    slli t2, t0, 2   # t2 = t0 * 4
    add t2, t2, s0
    lw  t2, 0(t2)
    # Calculate address to dst[i]
    slli t3, t0, 1   # t3 = t0 * 2
    add t3, t3, s1
    lh  t3, 0(t3)
    # If they are not equal, we failed
    bne t2, t3, error
    # Otherwise proceed
    addi t0, t0, 1
    ble t0, t1, testloop
    # If we get here the program worked!
worked: 
    j worked    
error:
    # If we get here, it failed
    j error





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