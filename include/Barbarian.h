#ifndef VILLAGEGAME_BARBARIAN_H
#define VILLAGEGAME_BARBARIAN_H

#include "Troop.h"

/**
 * Barbarian class - a melee attack troop
 * Represented by barbarian emoji 🧔🏾‍♂️
 */
class Barbarian : public Troop {
public:
    /**
     * Constructor for Barbarian
     * @param x Initial x-coordinate
     * @param y Initial y-coordinate
     */
    Barbarian(int x, int y);
    
    /**
     * Attack an enemy if adjacent
     * @param target The enemy to attack
     * @return True if attack was successful, false otherwise
     */
    bool attack(std::shared_ptr<Enemy> target) override;
};

#endif // VILLAGEGAME_BARBARIAN_H
