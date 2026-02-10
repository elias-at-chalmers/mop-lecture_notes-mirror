#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t health;
    uint8_t strength;
    uint32_t defense;
} Player;

typedef struct {
  uint16_t age; 
  uint32_t health;
} Monster; 


extern Player player1;
void damage_monster(Monster* monster);

int main(void)
{
    printf("Testing Assignment 1...");
    int passed = 1; 
    if(player1.health != 100) {
        printf("Player1 health does not match expected value .\n");
        passed = 0; 
    } 
    if(player1.strength != 50) {
        printf("Player1 strength does not match expected value.\n");
        passed = 0; 
    }
    if(player1.defense != 25) {
        printf("Player1 defense does not match expected value.\n");
        passed = 0; 
    }
    if(passed) {
        printf("PASSED!.\n");
    } else {
        printf("FAILED.\n");
    }

    printf("Testing Assignment 2...");
    Monster monster1 = {30, 200};
    damage_monster(&monster1);
    passed = 1;
    if(monster1.age != 30) {
        printf("Monster age changed during Assignment 2!\n");
        passed = 0;
    }
    if(monster1.health != 180) {
        printf("Monster health was not properly reduced.\n");
        passed = 0;
    }
    if(passed) {
        printf("PASSED!.\n");
    } else {
        printf("FAILED.\n");
    }

}

