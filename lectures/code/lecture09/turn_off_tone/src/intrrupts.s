.section .text

.global init_interrupts
.extern EXTI1_Handler

default_handler:
    # Default interrupt handler: just return
    j .

init_interrupts: 
    # Set mtvec to point to the vector table (with mode 0b01 for vectored mode)
    la t0, vector_table
    ori t0, t0, 0b1 
    csrw mtvec, t0
    ret

.align 4
vector_table: 

.rept 23
j default_handler
.endr
j EXTI1_Handler  # IRQ 23:  EXTI1 handler
.rept 200
j default_handler
.endr
