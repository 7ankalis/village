#ifndef VILLAGEGAME_ARCHER_H
#define VILLAGEGAME_ARCHER_H

#include "Troop.h"

/**
 * Archer class - a ranged attack troop
 * Represented by bow emoji 🏹
 */
class Archer : public Troop {
public:
    /**
     * Constructor for Archer
     * @param x Initial x-coordinate
     * @param y Initial y-coordinate
     */
    Archer(int x, int y);
    
    /**
     * Attack an enemy if within range
     * @param target The enemy to attack
     * @return True if attack was successful, false otherwise
     */
    bool attack(std::shared_ptr<Enemy> target) override;
};

#endif // VILLAGEGAME_ARCHER_H
