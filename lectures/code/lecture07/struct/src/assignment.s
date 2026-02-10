
###############################################################################
# A C program uses a struct to represent a player in a game:
#
# typedef struct {
#   uint32_t health;
#   uint8_t strength;
#   uint32_t defense;
# } Player;
#
# typedef struct {
#   uint16_t age; 
#   uint32_t health;
# } Monster; 
#
###############################################################################

###############################################################################
# Assignment 1: 
# ============================================================================
# Define a player with the following attributes:
#   health: 100
#   strength: 50
#   defense: 25
# in assembly.
###############################################################################

.global player1

.balign 4
player1: 
    .word 100          # health
    .byte 50            # strength
    .space 3
    .word 25            # defense


###############################################################################
# Assignment 2: 
# ============================================================================
# Write an assembly function that takes a pointer to a Monster struct and
# reduces the monsters health by 20.
# 
###############################################################################

.global damage_monster
damage_monster:
    # a0 will contain a pointer to the monster struct
    lw t0, 4(a0)        # load the health of the monster into t0
    li t1, 20           # load the value 20 into t1
    sub t0, t0, t1      # subtract 20 from the monster's health
    sw t0, 4(a0)        # store the updated health back to the monster struct
    ret