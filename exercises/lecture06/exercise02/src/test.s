.global test

.equ STK_CTLR, 0xE000F000
.equ STK_SR,   0xE000F004
.equ STK_CNTL,  0xE000F008
.equ STK_CNTH,  0xE000F00C
.equ STK_CMPL,   0xE000F010
.equ STK_CMPH,   0xE000F014

test: 
    la t0, STK_CTLR
    li t1, 0x0
    sw t1, 0(t0)         # Disable SysTick

    la t0, STK_CMPL
    li t1, 1000000
    sw t1, 0(t0)         # Load compare value

    la t0, STK_CMPH
    li t1, 0x0
    sw t1, 0(t0)         # Load compare value high

    la t0, STK_CNTL
    li t1, 0x0
    sw t1, 0(t0)         # Reset current value low

    la t0, STK_CNTH
    li t1, 0x0
    sw t1, 0(t0)         # Reset current value high 

    la t0, STK_CTLR
    li t1, 0xb101
    sw t1, 0(t0)         # Enable SysTick with interrupt and processor clock

loop: 
    la t0, STK_CNTL
    lw t1, 0(t0)
    nop
    la t0, STK_CNTL
    lw t1, 0(t0)
    nop    
    j loop

