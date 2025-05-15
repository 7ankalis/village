#ifndef VILLAGEGAME_TROOP_H
#define VILLAGEGAME_TROOP_H

#include "Entity.h"
#include "Position.h"
#include <memory>
#include <string>

// Forward declaration
class Enemy;

/**
 * Abstract base class for all troops that can be controlled by the player.
 * Troops can move around the map and attack enemies.
 */
class Troop : public Entity {
protected:
    int health;
    int damage;
    int range;
    int speed;

public:
    /**
     * Constructor for Troop
     * @param x Initial x-coordinate
     * @param y Initial y-coordinate
     * @param repr Visual representation (emoji)
     * @param health Initial health points
     * @param damage Attack damage
     * @param range Attack range
     * @param speed Movement speed (tiles per turn)
     */
    Troop(int x, int y, const std::string& repr, int health, int damage, int range, int speed);
    
    /**
     * Virtual destructor
     */
    virtual ~Troop() = default;
    
    /**
     * Abstract method to attack a target
     * @param target The enemy to attack
     * @return True if attack was successful, false otherwise
     */
    virtual bool attack(std::shared_ptr<Enemy> target) = 0;
    
    /**
     * Move towards a target position
     * @param targetPosition The position to move towards
     * @return True if moved, false otherwise
     */
    bool moveTowards(const Position& targetPosition);
    
    /**
     * Take damage when attacked
     * @param amount The amount of damage to take
     */
    void takeDamage(int amount);
    
    /**
     * Check if the troop is still alive
     * @return True if health > 0, false otherwise
     */
    bool isAlive() const;
    
    /**
     * Get the current health
     * @return Current health points
     */
    int getHealth() const;
    
    /**
     * Get the damage value
     * @return Damage points
     */
    int getDamage() const;
    
    /**
     * Get the attack range
     * @return Attack range
     */
    int getRange() const;
    
    /**
     * Get the movement speed
     * @return Movement speed
     */
    int getSpeed() const;
};

#endif // VILLAGEGAME_TROOP_H
