############################################################
# Lecture 02 - Exercise 4
# ==========================================================
#
# This is the only file you should edit for the excercise. 
# The file main.c is a test harness to help you test your code.
#
# Before you start: 
# Start simserver and choose Server->IO Setup
# For "Serial Communication Interface 1", choose "06 Console"
# Then run the program with F5 (if it breaks on main, 
# continue with F5 again)
# You should see output on the simserver console telling you 
# which assignments are passed. 
#
############################################################
    .text
    .global question1
    .global question2
    .global question3
    .global question4
    .global question5
    .global question6
    .global question7


############################################################
# Global variables
# Return here for Assignment 5
############################################################

.data
.align 2
# TODO: add global variables here
#   signed char x = -45;
x: .byte -45
#   unsigned char y = 210;
y: .byte 210
#   unsigned int z = 4000;
z: .word 4000



.text
############################################################
# Assignment 1
#
# Input:
#   a0 = char ascii_code
#
# Task:
#   Convert input ´ascii_code´ into a decimal value 
#   the code represents. 
#
# Hint: Link to ASCII table: 
#      https://www.asciitable.com/
#
############################################################
question1:
    addi     a0, a0, -'0'   # Convert ASCII to decimal
    ret


############################################################
# Assignment 2
#
# Input:
#   a0 = char digit1
#   a1 = char digit2
#   a2 = char digit3
#                           
# Task:
#   Registers a0-a2 contain digits (0-9) in ASCII code.
#   Convert ASCII into corresponding decimals and form
#   a single integer out of them.
#
#   Example: a0='1', a1='2', a2='3' => a0 = 123
#
#   Hint1: 123 = 1*100 + 2*10 + 3
#
############################################################
question2:
    addi     a0, a0, -'0'   # Convert ASCII to decimal
    addi     a1, a1, -'0'   # Convert ASCII to decimal
    addi     a2, a2, -'0'   # Convert ASCII to decimal
    li t0, 100
    mul     a0, a0, t0    # digit1 * 100
    li t0, 10
    mul     a1, a1, t0    # digit2 * 10
    add     a0, a0, a1    # add digit2
    add     a0, a0, a2    # add digit3
    ret

############################################################
# Assignment 3
#             
# Input:              
#   a0 = unsigned char pattern
#
# Task:
#   Given a one-byte pattern in a0, fill all the bytes of a0
#   with that pattern.
#
#   Save the result in a0.
#  
# Example: a0=0x55 => a0=0x55555555
#
############################################################
question3:
    andi    a0, a0, 0xFF        # Ensure a0 is 1 byte
    slli    t0, a0, 8           # Shift left by 8
    or      a0, a0, t0         # Combine
    slli    t0, a0, 16          # Shift left by 16
    or      a0, a0, t0         # Combine            
    ret

############################################################
# Assignment 4
#             
# Input:              
#   a0 = unsigned char value
#
# Task:
#   Calculate the number of ones in a0.
#   You do not need loops for this.
#
#   Save the result in a0.
#  
# Example: a0=0b00001101 => a0=3
#
# Hint: how can you isolate the first bit?
#
# Hint2: after you implement this, ask your favourite AI tool
#        to solve this, and get your mind blown.
#
############################################################
question4:
    andi    t0, a0, 0x01        # Isolate bit 0
    srli    t1, a0, 1           # Shift right by 1
    andi    t1, t1, 0x01        # Isolate bit 1
    add     t0, t0, t1         # Sum bits 0 and 1
    srli    t1, a0, 2           # Shift right by 2
    andi    t1, t1, 0x01        # Isolate bit 2
    add     t0, t0, t1         # Sum bits 0-2
    srli    t1, a0, 3           # Shift right by 3
    andi    t1, t1, 0x01        # Isolate bit 3
    add     t0, t0, t1         # Sum bits 0-3
    srli    t1, a0, 4           # Shift right by 4
    andi    t1, t1, 0x01        # Isolate bit 4
    add     t0, t0, t1         # Sum bits 0-4
    srli    t1, a0, 5           # Shift right by 5
    andi    t1, t1, 0x01        # Isolate bit 5     
    add     t0, t0, t1         # Sum bits 0-5   
    srli    t1, a0, 6           # Shift right by 6
    andi    t1, t1, 0x01        # Isolate bit 6     
    add     t0, t0, t1         # Sum bits 0-6   
    srli    t1, a0, 7           # Shift right by 7
    andi    t1, t1, 0x01        # Isolate bit 7     
    add     a0, t0, t1         # Sum bits 0-7       
    ret


############################################################
# Assignment 5 (with a curve ball)
#
# Task:
#   Go to the line 34 of this file and allocate memory
#   for the following variables with initial values:    
#   signed char x = -45;
#   unsigned char y = 210;
#   unsigned int z = 4000;
#
#   Then implement operation:
#       a0 = (x & 0x0FF0) - z + y 
#
############################################################

question5:
    # The thing that might confuse students (and did confuse yours truly) is that
    # lb will sign extend the signed char x when loading it into the register.
    lb      t0, x              # Load signed char x
    li      t1, 0x0FF0
    and     t0, t0, t1
    lw      t1, z              # Load unsigned int z
    sub     t0, t0, t1         # x - z
    lbu     t1, y              # Load unsigned char y
    add     a0, t0, t1         # (x & 0x0FF0) - z + y
    ret
    