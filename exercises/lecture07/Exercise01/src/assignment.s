###############################################################################
## Lecture 07 - Exercise 01 - Structs, Unions and their alignment
## ============================================================================
## In this exercise you will practice understanding allignment in C structures.
## As always, connect a console to usart1 and implement the functions below 
## to pass all test. 
###############################################################################

.global increase_score
.global give_double_points
.global chaos

###############################################################################
## Assignment 1:  Player_Stats structure is defined in C:
##
##              struct Player_Stats {
##                 uint16_t level;         // player level
##                 uint8_t  lives;         // remaining lives
##                 uint32_t score;         // total score
##                 uint8_t  powerups;      // bit flags
##             };
##
## Implement a function that adds `uint32_t score` to the `Player_Stats.score`
## void add_score (struct PlayerStats *status, uint32_t score);               
##
## Hint: use MemoryView tool to peak how data is packed in memory.
##       Take value in `a0` as a base address. The structure is initialised as:
##        {.level=0xAAAA, .lives=0xBB, .score=0xCCCCCCCC, .powerups=0xDD};
###############################################################################

increase_score:
    # a0 = pointer to struct Player_Stats
    # a1 = score to add

    lw  t0, 4(a0)      # load current score
    add t0, t0, a1     # add score
    sw  t0, 4(a0)      # store updated score
    
    ret

###############################################################################
## Assignment 2:  Player_Stats structure is now expanded with a bit field:
##
##              struct Player_Stats2 {
##                 uint16_t level;         // player level
##                 uint8_t  lives;         // remaining lives
##                 uint32_t score;         // total score
##                 
##                 /* power-up flags */
##                 uint8_t shield  : 1;             // bit 0
##                 uint8_t speed   : 1;             // bit 1
##                 uint8_t double_points : 1;       // bit 2
##                 uint8_t piercing_bullets : 1;    // bit 3
##                 uint8_t reserved_1 : 4;          // reserved
##             };
##
## Implement a function that sets double points bit to a given Player_Stats2
## void give_double_points(struct Player_Stats2 *status);               
##
## Note: do not modify any other bit in the structure!
##
###############################################################################

give_double_points:
    # a0 = pointer to struct Player_Stats

    lbu t0, 8(a0)
    or t0, t0, 0b100
    sb t0, 8(a0)

    ret

###############################################################################
## Assignment 3:  Player_Stats structure is now expanded with a union:
##
##                  struct Player_Stats3 {
##                      uint16_t level;     
##                      uint8_t  lives;     
##                      uint32_t score;
##
##                       union {
##                           uint8_t powerups;   // raw byte access
##                           struct {
##                              uint8_t shield            : 1;  // bit 0
##                              uint8_t speed             : 1;  // bit 1
##                              uint8_t double_points     : 1;  // bit 2
##                              uint8_t piercing_bullets  : 1;  // bit 3
##                              uint8_t reserved_1        : 4;  // bits 4–7
##                          };
##                       }; 
##                  };
##
## Implement a function `uint8_t chaos(struct Player_Stats3 *status)` which flips
## all the powerups of the player (0 to 1, and 1 to 0), but sets double_points.
##
## Reserved bits must remain unchanged.
## Function should returns `powerups` field without the reserved bits.
##           
## Note: XOR 1 allows you to flip bits.
##
###############################################################################

chaos:
    lbu   t0, 8(a0)          # t0 = status->powerups  (uint8_t)
    xori  t0, t0, 0x0F       # t0 = ~t0
    ori   t0, t0, 0b100      # set bit double_points
    sb    t0, 8(a0)

    andi  t0, t0, 0xf
    mv    a0, t0

    ret
