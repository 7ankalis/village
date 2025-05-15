#ifndef ENEMY_H
#define ENEMY_H
using namespace std;
#include "Npc.h"
#include "Wall.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "TownHall.h"
#include <vector>

// Define enemy types
enum class EnemyType {
    RAIDER,   // Attacks all buildings except walls
    BOMBERMAN // Specializes in destroying walls
};

/**
 * @brief Base class for enemy entities
 * 
 * Provides common functionality for all enemy types, including
 * movement, attack behaviors, and targeting logic.
 */
class Enemy : public Npc {
protected:
    int health;
    int damage;
    int speedCounter;
    const int speed;
    bool isAttacking;
    Building* targetBuilding;
    EnemyType type;

public:
    /**
     * @brief Base constructor for Enemy
     * 
     * Creates an enemy with specified properties
     * 
     * @param x Initial X-coordinate
     * @param y Initial Y-coordinate
     * @param type The type of enemy (RAIDER or BOMBERMAN)
     * @param icon Visual representation of the enemy
     * @param dmg Damage points per attack
     * @param spd Speed value (lower is faster)
     */
    Enemy(int x, int y, EnemyType type, const string& icon = "👹", int dmg = 10, int spd = 15);
    virtual ~Enemy() = default;

    /**
     * @brief Updates enemy position and handles attacks on buildings
     * 
     * This method:
     * 1. Continues attacking if already attacking a building
     * 2. Finds a new target if not attacking
     * 3. Moves toward the target position with randomized variations in path
     * 
     * @param targetPos Target position (usually town hall position)
     * @param walls Vector of walls that can be attacked
     * @param goldMines Vector of gold mines that can be attacked
     * @param elixirCollectors Vector of elixir collectors that can be attacked
     * @param townhall Town hall reference, used to check game over condition
     * @return true if town hall is destroyed (game over), false otherwise
     */
    virtual bool update(const Position& targetPos, vector<Wall>& walls, vector<GoldMine>& goldMines,
                vector<ElixirCollector>& elixirCollectors, const TownHall& townhall);
    
    /**
     * @brief Get enemy's damage value
     * 
     * @return Damage amount per attack
     */
    int getDamage() const;
    
    /**
     * @brief Get the enemy's type
     * 
     * @return The EnemyType (RAIDER or BOMBERMAN)
     */
    EnemyType getType() const;
    
    /**
     * @brief Get the enemy's health
     * 
     * @return Current health points
     */
    int getHealth() const { return health; }
    
    /**
     * @brief Set the enemy's health
     * 
     * @param newHealth New health value
     */
    void setHealth(int newHealth) { health = newHealth; }
    
    /**
     * @brief Check if the enemy is alive
     * 
     * @return true if health > 0, false otherwise
     */
    bool isAlive() const { return health > 0; }
    
    /**
     * @brief Find the closest building to attack
     * 
     * Base implementation prioritizes any closest building
     * To be overridden by derived classes for specific targeting behavior
     * 
     * @param walls Vector of walls
     * @param goldMines Vector of gold mines
     * @param elixirCollectors Vector of elixir collectors
     * @param townhall Town hall reference
     * @return Pointer to the closest building, or nullptr if no building is in range
     */
    virtual Building* findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                      vector<ElixirCollector>& elixirCollectors, const TownHall& townhall);
                      
    /**
     * @brief Calculate distance between two positions
     * 
     * @param pos1 First position
     * @param pos2 Second position
     * @return Euclidean distance between positions
     */
    int calculateDistance(const Position& pos1, const Position& pos2) const;
};

#endif


