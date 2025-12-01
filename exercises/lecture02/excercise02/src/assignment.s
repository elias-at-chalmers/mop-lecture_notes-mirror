    .text
    .global question1
    .global question2
    .global question3
    .global question4
    .global question5

############################################################
# Assignment 1
#
# Input:
#   a0 = integer a
#
# Task:
#   Return:
#       a * 2
#
############################################################
question1:
    # TODO

    slli a0, a0, 1

    ret


############################################################
# Assignment 2
#
# Input:
#   a0 = integer a
#   a1 = integer b
#
# Task:
#   Return:
#       a * b
#
############################################################
question2:
    # TODO
    mul a0, a0, a1

    ret


############################################################
# Assignment 3
#
# Input:
#   a0 = integer a
#
# Task:
#   Compute:
#
#       a*a + 10
#
############################################################
question3:
    # TODO

    mul t0, a0, a0
    addi a0, t0, 10

    ret


############################################################
# Assignment 4
#
# Input:
#   a0 = short x   (passed as a signed 32-bit integer)
#
# Task:
#   Return:
#       1   if x is negative
#       0   if x is positive or zero
#
# Notes:
#   * Do NOT use branches.
#   * Detect negativity by examining the sign bit.
#
############################################################
question4:
    # TODO
    srli a0, a0, 31

    ret


############################################################
# Assignment 5  (Challenge)
#
# Input:
#   a0 = integer a
#
# Task:
#   Return:
#
#       a * 7
#
# Rules:
#   * Do NOT use 'mul'
#   * Only shifts and adds/subs allowed
#
############################################################
question5:
    # TODO
    slli t0, a0, 3
    sub  a0, t0, a0
    ret