############################################################
# Lecture 04 - Exercise 01
#
# For this exercice, you need to connect a bargraph to 
# GPIO port D, bits 8-15.
############################################################

.section .text
.globl main
main: 
    ########################################################
    # Assignment 1: Configure GPIOD pins 8-15 as:
    #                - output 2Mhz
    #                - push-pull
    # (refer to the lecture notes and/or QuickGuide)
    # (also make sure you have done the quiz for this lecture)
    ########################################################
    la t0, 0x40011404               # Load address of CFGHR to t0
    li t1, 0x22222222               # Set all pins to output 2MHz push-pull
    sw t1, 0(t0)                    # Write to CFGHR

    j assignment6

assignment4_loop: 
    ########################################################
    # Assignment 4: Blink one LED by calling the set_led
    # and clear_led functions and then loop.
    #
    # Step through it with the debugger to see it work
    # Press F5 to let it run at full speed.
    ########################################################
    li a0, 0                         # Load led number 0
    jal set_led                      # Call set_led(0)
    li a0, 0                         # Load led number 0
    jal clear_led                    # Call clear_led(0)
    j assignment4_loop               # Repeat forever

assignment5_loop: 
    ########################################################
    # Assignment 5: Create a "delay" function that simply
    #               loops a certain number of times to make
    #               transitions visible to the human eye.
    #
    ########################################################
    li a0, 0                         # Load led number 0
    jal set_led                      # Call set_led(0)
    call delay
    li a0, 0                         # Load led number 0
    jal clear_led                    # Call clear_led(0)
    call delay
    j assignment5_loop               # Repeat forever

delay: 
    li t0, 0x1FFFFF                  # Load loop count
delay_loop:
    addi t0, t0, -1                 # Decrement loop counter
    bnez t0, delay_loop             # Loop until zero
    ret

assignment6: 
    li t4, 0
assignment6_loop: 
    ########################################################
    # Assignment 6: Challenge!
    #               In each iteration of the loop, mvoe to 
    #               the next led in the bargraph, going back 
    #               to the first led after the last one.    
    ########################################################
    mv a0, t4                         # Load led number 0
    jal clear_led 
    addi t4, t4, 1                  # Increment led number
    andi t4, t4, 7                  # Wrap around after led 7
    mv a0, t4                         # Load led number 0
    jal set_led 
    call delay
    j assignment6_loop               # Repeat forever


set_led:
    ########################################################
    # Assignment 2: 
    # Write a function that turns on a specific led
    # It should not affect the other leds
    # You will do this by setting the correct bit in the 
    # GPIOD ODATA register
    #
    # Input: 
    # a0 - The led (0-7) to turn on
    ########################################################
    la t0, 0x4001140C               # Load address of ODATA to t0
    li t1, 0x100                    # Load 0b100000000 into t1 (bit 8)
    sll t1, t1, a0                  # Shift left by a0
    lh t2, 0(t0)                    # Load current ODATA value into t
    or t2, t2, t1                   # Set the correct bit
    sh t2, 0(t0)                    # Write back to ODATA
    ret

clear_led:
    ########################################################
    # Assignment 3:
    # Write a function that turns off a specific led
    # It should not affect the other leds
    # You will do this by clearing the correct bit in the
    # GPIOD ODATA register
    #
    # Input:
    # a0 - The led (0-7) to turn off
    ########################################################
    la t0, 0x4001140C               # Load address of ODATA
    li t1, 0x100                    # Load 0b100000000 into t1 (bit 8)
    sll t1, t1, a0                  # Shift left by a0
    not t1, t1                      # Invert bits to create mask
    lh t2, 0(t0)                    # Load current ODATA value into t
    and t2, t2, t1                  # Clear the correct bit
    sh t2, 0(t0)                    # Write back to ODATA
    ret