.section .text
.globl main

.equ GPIO_D_CFGLR, 0x40011400
.equ GPIO_D_CFGHR, 0x40011404
.equ GPIO_D_INDR,  0x40011408
.equ GPIO_D_OUTDR, 0x4001140C

main:
    # Configure GPIO D, pin 2 as output, 2Mhz, push-pull 
    la t0, GPIO_D_CFGLR
    lw t1, 0(t0)
    andi t1, t1, 0xFFFFF0FF  # Clear bits for pin 2
    ori t1, t1, 0x00000200   # Set pin 2 
    sw t1, 0(t0)



    la t0, GPIO_D_CFGHR
    li t1, 0x88888888
    sw t1, 0(t0)  # Set upper 8 pins as input with pull-up/down
loop:
    la t0, GPIO_D_INDR
    lw t1, 0(t0)          # Read input from DIP switch
    srl t1, t1, 8        # Shift right to get upper
    andi t1, t1, 0xFF     # Mask to get only upper 8 bits
    la t0, GPIO_D_OUTDR
    sw t1, 0(t0)          # Write to bargraph
    j loop                # Repeat indefinitely
    ret



