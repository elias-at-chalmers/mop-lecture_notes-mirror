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
    la t0, 0x40011400               # Load address of CFGLR to t0
    li t1, 0x22222222               # Set all pins to output 2MHz push-pull
    sw t1, 0(t0)                    # Write to CFGHR



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
    li t1, 0x1
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
    li t1, 0x1
    sll t1, t1, a0                  # Shift left by a0
    not t1, t1                      # Invert bits to create mask
    lh t2, 0(t0)                    # Load current ODATA value into t
    and t2, t2, t1                  # Clear the correct bit
    sh t2, 0(t0)                    # Write back to ODATA
    ret