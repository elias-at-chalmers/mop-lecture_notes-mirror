#include <stdint.h>
#define SOLUTION

int hero_strength = 0; 
int hero_dexterity = 0;
int hero_weapon = 0; // 0 = unarmed, 1 = sword, 2 = axe, 3 = spear
int enemy_armour = 0; // Array of armour values

int enemy_defense()
{
    ///////////////////////////////////////////////////////
    // Assignment 3: 
    // ====================================================
    // Enemy defense is enemy_armour * 2
    // Every time the hero attacks, the enemy's armour
    // is reduced by 1 (to a minimum of 0)
    ///////////////////////////////////////////////////////
#ifdef SOLUTION    
    int defense = enemy_armour * 2;
    if (enemy_armour > 0) {
        enemy_armour--;
    }
    return defense;
#else
    return 0; 
#endif
}

int weapon_bonus()
{
    ///////////////////////////////////////////////////////
    // Assignment 2: 
    // ====================================================
    // Return weapon bonus based on hero_weapon value.
    //
    // unarmed = 0
    // sword = 5
    // axe = 7
    // spear = 6
    ///////////////////////////////////////////////////////
#ifdef SOLUTION    
    switch (hero_weapon)
    {
        case 1: return 5;
        case 2: return 7;
        case 3: return 6;
        default:            
    }
#endif    
    return 0; 
}

int hero_damage()
{
    ///////////////////////////////////////////////////////
    // Assignment 1: 
    // ====================================================
    // Calculate damage dealt in unarmed combat against
    // a monster without armour:
    // DAMAGE = STRENGTH * 2 + DEXTERITY + 
    //              (weapon_bonus() - enemy_armour())
    ///////////////////////////////////////////////////////
#ifdef SOLUTION    
    int damage = hero_strength * 2 + hero_dexterity + (weapon_bonus() - enemy_defense());
    return damage;
#endif
    return 0;    
}

