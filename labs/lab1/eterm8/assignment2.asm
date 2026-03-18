###############################################################################
# Lab 1, Assignment 2
###############################################################################

.global main
.text 

main: 
    ###########################################################################
    # Implement your program here!
    ###########################################################################
loop: j loop








    ###########################################################################
    # Below is the startup code, no need to modify it!
    ###########################################################################

.section .start_section

_start: 
    # Set stackpointer to the top of RAM
    la sp,0x2001C000

    # This is a fancy way of enabling interrupts and jumping to main.
    # (Since we set MPIE=1 and then `mret`, we go "back" to having interrupts enabled)
    la t0, main
    la ra, exit
    csrw mepc, t0
    li   t1, (3 << 11)          # MPP = 3 (Machine)
    ori  t1, t1, (1 << 7)       # MPIE = 1
    csrw mstatus, t1
    mret

# Call C runtime de-initialization (never reached)
exit: 
    j .
