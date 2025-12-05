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


.global main
.data 
.align 2
.equ FIRST, 9
.equ LAST, 16
.equ NUM_VALUES, 17


### BUGGED LINE ###############################################################
#src: .word 1, 2, 4, 8, 16, 32, 64, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072
### FIXED LINE ################################################################
src: .word 1, 2, 4, 8, 16, 32, 64, 256, 512, 1024, 2048, 4096, 8192, 16384, 0xAA, 0xBB, 0xCC
###############################################################################
dst: .space NUM_VALUES*2

.text 


###############################################################################
# The main function runs the copyelements function and then runs a little 
# test to see if it works. 
# THIS CODE IS BUG FREE. Understand what it does, but the bugs are not here. 
###############################################################################
main: 
    ###########################################################################
    # Copy last half of src to dst
    ###########################################################################

    # (in s registers because we need them after the call as well)
    la s1, src      # Address to src 
    la s2, dst      # Address to dst
    li s3, FIRST    # First element to copy
    li s4, LAST     # Final element to copy
    # Fill arguments and call copyelements
    mv a0, s1
    mv a1, s2
    mv a2, s3
    mv a3, s4
    call copyelements
    ###########################################################################
    # Check that it worked
    ###########################################################################
    li t0, FIRST        # Loop counter, i
    li t1, LAST
testloop: 
    # Calculate address to src[i], and read    
    slli t2, t0, 2   # t2 = t0 * 4
    add t2, t2, s1
    lw  t2, 0(t2)
    # Calculate address to dst[i]
    slli t3, t0, 1   # t3 = t0 * 2
    add t3, t3, s2
    lh  t3, 0(t3)
    # If they are not equal, we failed
    bne t2, t3, error
    # Otherwise proceed
    addi t0, t0, 1
    ble t0, t1, testloop
worked: 
    # If we get here the program worked!
    j worked    
error:
    # If we get here, it failed
    j error





copyvec: 
    li t0, 0                        # t0: loop counter
    mv t1, a0                       # Address to src element
    mv t2, a1                       # Address to dst element
loop: 
    beq t0, a2, done                # If loop counter eached size, we are done
    lw t3, 0(t1)                    # Load element from src
    ### BUGGED LINE ###########################################################
    #addi t1, t1, 2                  # Move t1 to next element
    ### FIXED LINE ############################################################
    addi t1, t1, 4                  # Move t1 to next element
    ###########################################################################
    sh t3, 0(t2)                    # Store element in dst
    addi t2, t2, 2                  # Move t2 to next element
    addi t0, t0, 1                  # Increase loop counter
    j loop
done: 
    ret                             # Return from function

copyelements: 
    
    ###########################################################################
    # Assignment 4 (not 3, do 3 first): 
    #
    # Remember to push any registers (that are callee saved) to the stack
    ###########################################################################
    addi sp, sp, -8
    ### BUGGED LINE ###########################################################
    ### FIXED LINE ############################################################    
    sw s1, 4(sp)
    ###########################################################################
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
    addi s1, t2, 1                  # s1 is number of elements
    mv a0, t0
    mv a1, t1
    mv a2, s1
    call copyvec
    mv a0, s1                     # s1 is safe to use here

    ###########################################################################
    # Assignment 5:
    #
    # Remember to restore any values you pushed to the stack.
    ###########################################################################

    ### BUGGED LINE ###########################################################
    ### FIXED LINE ############################################################    
    lw s1, 4(sp)
    ###########################################################################
    lw ra, 0(sp)
    addi sp, sp, 8

    ret

###############################################################################
# Assignment 6:
# If all tests passed, the secret password will be printed in the console.
# Submit that along with the hidden message and address in Canvas.
###############################################################################