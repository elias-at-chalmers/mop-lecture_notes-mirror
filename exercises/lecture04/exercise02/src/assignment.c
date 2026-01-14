///////////////////////////////////////////////////////////////////////////////
// In this C exercise, you will test some very basic C programming by 
// implementing a small part of a simple RPG combat system. 
// 
// Start simserver with a console connected as usual, 
// and try to pass all tests. 
//
// Do the assignments in order, as each assignment
// builds on the previous one.
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#define SOLUTION

int hero_strength = 0; 
int hero_dexterity = 0;
int hero_weapon = 0; // 0 = unarmed, 1 = sword, 2 = axe, 3 = spear
int enemy_armour = 0; 

int enemy_defense()
{
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 3: 
    // ====================================================
    // Enemy defense is enemy_armour * 2
    // Every time the hero attacks, the enemy's armour
    // is reduced by 1 (to a minimum of 0)
    ///////////////////////////////////////////////////////////////////////////
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
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2: 
    // ====================================================
    // Return weapon bonus based on hero_weapon value.
    //
    // unarmed = 0
    // sword = 5
    // axe = 7
    // spear = 6
    ///////////////////////////////////////////////////////////////////////////
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
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 1: 
    // ====================================================
    // Calculate and return damage done by hero as:
    // 
    // DAMAGE = STRENGTH * 2 + DEXTERITY + 
    //              (weapon_bonus() - enemy_armour())
    ///////////////////////////////////////////////////////////////////////////
#ifdef SOLUTION    
    int damage = hero_strength * 2 + hero_dexterity + (weapon_bonus() - enemy_defense());
    return damage;
#endif
    return 0;    
}

