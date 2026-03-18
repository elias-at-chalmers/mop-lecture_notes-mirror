############################################################
# Lab 1 - Assignment 2
############################################################

.section .text
.globl main
main: 
    ########################################################
    # Task 1: Configure chosen GPIOD pin as:
    #                - output 2Mhz
    #                - push-pull
    # (refer to the lecture notes and/or QuickGuide)
    ########################################################
    la t0, 0x40011400               # Load address of CFGLR to t0
    li t1, 0x22222222               # Set all pins to output 2MHz push-pull
    sw t1, 0(t0)                    # Write to CFGHR


#########################################################
# Task 2: Make it blink
#########################################################
    li s0, 0x40011410               # BSR address (GPIOD)
    li s1, 0x40011414               # BCR address (GPIOD)
    li s2, (1 << 6)                 # Bit mask for PD6

blink_loop:
    sw s2, 0(s0)                    # Turn LED on (set PD6 via BSR)
    jal delay                       # Wait a while
    sw s2, 0(s1)                    # Turn LED off (clear PD6 via BCR)
    jal delay                       # Wait a while
    j blink_loop

delay:
    li t3, 500000                   # Delay counter (adjust for desired speed)
delay_loop:
    addi t3, t3, -1
    bnez t3, delay_loop
    ret

    

#########################################################
# Task 3: Making it pulsate
#########################################################

    li t0, 0                   # Starting duty cycle red
    li t1, 0                   # Starting duty cycle green
    li t2, 0                   # Starting duty cycle blue

loop: 
    #########################################################
    # Outer, never ending loop
    #########################################################
        # Increase and wrap around all duty cycles
        li t3, 0x7FF
        addi t0, t0, 1
        and t0, t0, t3
        addi t1, t1, 1
        and t1, t1, t3
        addi t2, t2, 1
        and t2, t2, t3

        #####################################################
        # One period
        #####################################################
        li t3, 0                    # Counter
        # Light all leds
        li t4, 0x40011410               # BSR
        li t5, 0xFF
        sw t5, 0(t4)
        li t4, 0x40011414               # BCR (which we need soon)

        period: 
           blt t3, t0, red_on
           li t5, 0x1
           sw t5, 0(t4)
red_on:    blt t3, t1, green_on
           li t5, 0x10
           sw t5, 0(t4)
green_on:  blt t3, t2, blue_on
           li t5, 0x80
           sw t5, 0(t4)          
blue_on: 
            addi t3, t3, 1          # Increment counter
            li t5, 4096
            blt t3, t5, period

    j loop




    ###########################################################################
    # Below is the startup code, no need to look for bugs here!
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
