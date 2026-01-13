#include <stdio.h>
#include <stdint.h>
#include "assignment.h"


int main(void)
{
    ///////////////////////////////////////////////////////
    // Test assignment 1 for various hero 
    // stats but with no weapons or enemy armour
    ///////////////////////////////////////////////////////
    printf("Testing assignment 1: Unarmed combat:\n");
    int teet_damage[] = {10, 2, 5, 0, 1};
    int test_dexterity[] = {5, 2, 8, 12, 20}; 
    int test_expected[] = {25, 6, 18, 12, 22};
    for(int i = 0; i < 5; i++)
    {
        hero_strength = teet_damage[i];
        hero_dexterity = test_dexterity[i];
        hero_weapon = 0; // unarmed
        int damage = hero_damage();
        printf("STR: %d DEX: %d => Damage: %d...", hero_strength, hero_dexterity, damage);
        if (damage == test_expected[i]) {
            printf(" PASS\n");
        }
        else {
            printf(" FAIL (expected %d)\n", test_expected[i]);
        }
    }       

    ///////////////////////////////////////////////////////
    // Test assignment 2 for various combinations of hero
    // stats and weapons but with no enemy armour
    ///////////////////////////////////////////////////////
    printf("\nTesting assignment 2: Armed combat:\n");
    int test_weapons[] = {0, 1, 2, 3, 2};
    int test_expected2[] = {25, 11, 25, 18, 29};
    for(int i = 0; i < 5; i++)
    {
        hero_strength = teet_damage[i];
        hero_dexterity = test_dexterity[i];
        hero_weapon = test_weapons[i];
        int damage = hero_damage();
        char * weapon_name;
        if(test_weapons[i] == 0) weapon_name = "unarmed";
        else if(test_weapons[i] == 1) weapon_name = "sword";
        else if(test_weapons[i] == 2) weapon_name = "axe";
        else if(test_weapons[i] == 3) weapon_name = "spear";
        printf("STR: %d DEX: %d WEAPON: %s => Damage: %d", 
            hero_strength, hero_dexterity, weapon_name, damage);
        if (damage == test_expected2[i]) {
            printf(" PASS\n");
        }
        else {
            printf(" FAIL (expected %d)\n", test_expected2[i]);
        }        
    }

    ///////////////////////////////////////////////////////
    // Test assignment 3 for various combinations of hero
    // stats, weapons, and enemy armour
    ///////////////////////////////////////////////////////
    printf("\nTesting assignment 3: Armed combat vs Armour:\n");
    int test_expected3[] = {5, -7, 9, 4, 17};
    enemy_armour = 10;
    for(int i = 0; i < 5; i++)
    {
        hero_strength = teet_damage[i];
        hero_dexterity = test_dexterity[i];
        hero_weapon = test_weapons[i];
        char * weapon_name;
        if(test_weapons[i] == 0) weapon_name = "unarmed";
        else if(test_weapons[i] == 1) weapon_name = "sword";
        else if(test_weapons[i] == 2) weapon_name = "axe";
        else if(test_weapons[i] == 3) weapon_name = "spear";        
        int damage = hero_damage();
        printf("STR: %d DEX: %d WEAPON: %s\nARMOUR: %d", 
            hero_strength, hero_dexterity, weapon_name, enemy_armour);
        printf(" => Damage: %d...", damage);
        if (damage == test_expected3[i]) {
            printf(" PASS\n");
        }
        else {
            printf(" FAIL (expected %d)\n", test_expected3[i]);
        }        
    }
    return 0;
}