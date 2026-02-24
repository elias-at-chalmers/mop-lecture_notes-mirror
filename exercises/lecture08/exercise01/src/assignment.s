.section .text

.global InitInterrupts
.extern SysTick_Handler

.set SOLUTION, 1

InitInterrupts: 
###############################################################################
# Assignment 2.1: This function is called from the whackamole game loop. 
#                 Here, you need to: 
#                 1. Initialize the CSR register "mtvec" to point at the 
#                    SysTick_Handler, in the C code. 
#                 2. Enable interrupts from SysTick, by setting the 
#                    corresponding bit in the PFIC_IENR1 register (see Vector 
#                    Table and PFIC in the quickguide).
# Hint: Read lecture 08 to remember that CSR registers are accessed using csrw and
#       csrr instructions (which are in the QuickGuide).
###############################################################################
.if SOLUTION
    .equ PFIC_IENR1, 0xE000E100  # PFIC Interrupt Enable Register 1
    la t0, SysTick_Handler
    csrw mtvec, t0
    li t0, 1 << 12  
    la t1, PFIC_IENR1
    sw t0, 0(t1)  # Enable SysTick interrupt (bit 12)
    ret
.endif
