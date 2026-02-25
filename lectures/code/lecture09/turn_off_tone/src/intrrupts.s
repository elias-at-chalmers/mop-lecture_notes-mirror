.section .text

.global init_interrupts
.extern EXTI1_Handler
init_interupts: 
la t0, vector_table
ori t0, t0, 1
csrw mtvec, t0
ret




.align 2
vector_table:
.space 23 * 4
j EXTI1_Handler