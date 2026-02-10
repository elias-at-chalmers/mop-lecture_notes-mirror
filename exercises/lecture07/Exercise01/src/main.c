#include <stdio.h>
#include <stdint.h>

struct Player_Stats {
    uint16_t level;         // player level
    uint8_t  lives;         // remaining lives
    uint32_t score;         // total score
    uint8_t  powerups;      // bit flags
};

struct Player_Stats2 {
    uint16_t level;         // player level
    uint8_t  lives;         // remaining lives
    uint32_t score;         // total score

    /* power-up flags */
    uint8_t shield  : 1;    // bit 0
    uint8_t speed   : 1;    // bit 1
    uint8_t double_points : 1; // bit 2
    uint8_t piercing_bullets : 1;    // bit 3
    uint8_t reserved_1 : 4;

    uint8_t test_byte;        // to check if we write a word by mistake
};

struct Player_Stats3 {
    uint16_t level;     
    uint8_t  lives;     
    uint32_t score;

    union {
        uint8_t powerups;   // raw byte access
        struct {
            uint8_t shield            : 1;  // bit 0
            uint8_t speed             : 1;  // bit 1
            uint8_t double_points     : 1;  // bit 2
            uint8_t piercing_bullets  : 1;  // bit 3
            uint8_t reserved_1        : 4;  // bits 4–7
        };
    };

    uint8_t test_byte; 
};

extern void increase_score(struct Player_Stats *status, uint32_t score);
extern void give_double_points(struct Player_Stats2 *status);
extern uint8_t chaos(struct Player_Stats3 *status);

int main(void)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 1
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 1...\n");
    struct Player_Stats status = {.level=0xAAAA, .lives=0xBB, .score=0xCCCCCCCC, .powerups=0xDD};
    increase_score(&status, 871);
    int a1_flag = 1;

    if (status.level != 0xAAAA || status.lives != 0xBB || status.powerups != 0xDD){
        printf("Wrong fields of Player_Stats are modified!\n");
        a1_flag = 0;
    }
    if (status.score != 0xCCCCD033){
        printf("Score is updated incorrectly!\n");
        a1_flag = 0;
    }

    if (a1_flag){
        printf("PASSED!\n\n");
    }
    else{
        printf("FAILED!\n\n");
    }
    ///////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 2
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 2...\n");
    struct Player_Stats2 status2 = {.level=0xAAAA, .lives=0xBB, .score=0xCCCCCCCC, 
                                   .shield=0, .speed=0, .double_points=0, .piercing_bullets=0, .reserved_1=0x0,
                                   .test_byte=0x0};
    
    give_double_points(&status2);
    
    int a2_flag = 1;
    if (status2.double_points != 1){
        printf("Double points bit is not set!\n");
        a2_flag = 0;
    }
    if (status2.test_byte != 0x0 || status2.reserved_1 != 0x0){
        printf("Wrong bits are modified!\n");
        a2_flag = 0;
    }
    if (status2.level != 0xAAAA || status2.lives != 0xBB || status2.score != 0xCCCCCCCC){
        printf("Wrong fields in struct are modified\n");
        a2_flag = 0;
    }


    if (a2_flag){
        printf("PASSED!\n\n");
    }
    else{
        printf("FAILED!\n\n");
    }
    ///////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 3
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 3...\n");
    struct Player_Stats3 status3 = {.level=0xAAAA, .lives=0xBB, .score=0xCCCCCCCC, 
                                   .shield=1, .speed=0, .double_points=1, .piercing_bullets=1, .reserved_1=0x5,
                                   .test_byte=0x55};

    uint8_t res = chaos(&status3);

    int a3_flag = 1;
    if (status3.double_points != 1){
        printf("Double points bit is not set!\n");
        a3_flag = 0;
    }
    if (status3.shield != 0 || status3.speed != 1 || status3.piercing_bullets != 0){
        printf("Powerup bits flipped incorrectly!\n");
        a3_flag = 0;
    }
    if (res != 0x6){
        printf("Wrong value is returned\n");
        a3_flag = 0;
    }
    
    if (status3.test_byte != 0x55 || status3.reserved_1 != 0x5){
        printf("Bits outside of powerups are modified!\n");
        a3_flag = 0;
    }
    if (status3.level != 0xAAAA || status3.lives != 0xBB || status3.score != 0xCCCCCCCC){
        printf("Wrong fields in struct are modified\n");
        a3_flag = 0;
    }

    if (a3_flag){
        printf("PASSED!\n\n");
    }
    else{
        printf("FAILED!\n\n");
    }
    ///////////////////////////////////////////////////////////////////////////////

}

