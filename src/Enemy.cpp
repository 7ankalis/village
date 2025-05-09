/**
 * @file Enemy.cpp
 * @brief Implementation of the Enemy class hierarchy, representing different types of opponents
 */

#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <random>

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
Enemy::Enemy(int x, int y, EnemyType type, const string& icon, int dmg, int spd)
    : Npc(x, y, icon),
      damage(dmg),
      speedCounter(0),
      speed(spd),
      isAttacking(false),
      targetBuilding(nullptr),
      type(type) {}

/**
 * @brief Calculate distance between two positions
 * 
 * @param pos1 First position
 * @param pos2 Second position
 * @return Euclidean distance between positions
 */
int Enemy::calculateDistance(const Position& pos1, const Position& pos2) const {
    double dx = pos2.x - pos1.x;
    double dy = pos2.y - pos1.y;
    return sqrt(dx*dx + dy*dy);
}

/**
 * @brief Find the closest building to attack
 * 
 * Base implementation prioritizes any closest building
 * 
 * @param walls Vector of walls
 * @param goldMines Vector of gold mines
 * @param elixirCollectors Vector of elixir collectors
 * @param townhall Town hall reference
 * @return Pointer to the closest building, or nullptr if no building is in range
 */
Building* Enemy::findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                         vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) {
    // Base implementation prioritizes any closest building
    Building* closestBuilding = nullptr;
    double minDist = 1000000;  // Large initial value
    Position myPos = getPosition();
    
    // Check walls
    for (auto& wall : walls) {
        double dist = calculateDistance(myPos, wall.getPosition());
        if (dist < minDist) {
            minDist = dist;
            closestBuilding = &wall;
        }
    }
    
    // Check gold mines
    for (auto& mine : goldMines) {
        double dist = calculateDistance(myPos, mine.getPosition());
        if (dist < minDist) {
            minDist = dist;
            closestBuilding = &mine;
        }
    }
    
    // Check elixir collectors
    for (auto& collector : elixirCollectors) {
        double dist = calculateDistance(myPos, collector.getPosition());
        if (dist < minDist) {
            minDist = dist;
            closestBuilding = &collector;
        }
    }
    
    // Check town hall
    double dist_th = calculateDistance(myPos, townhall.getPosition());
    if (dist_th < minDist) {
        minDist = dist_th;
        closestBuilding = const_cast<TownHall*>(&townhall);
    }
    
    return (minDist < 2) ? closestBuilding : nullptr;
}

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
bool Enemy::update(const Position& targetPos, vector<Wall>& walls, vector<GoldMine>& goldMines,
                  vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) {
    // Static random number generator
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> random_move(-1, 1);
    static uniform_int_distribution<> random_chance(1, 10);
    
    // If already attacking a building, continue attack
    if (isAttacking && targetBuilding) {
        targetBuilding->takeDamage(damage);
        
        // Check if we've destroyed townhall
        if (targetBuilding == &townhall && townhall.getHealth() <= 0) {
            return true;  // Game over condition
        }
        
        // Stop attacking if building is destroyed
        if (targetBuilding->getHealth() <= 0) {
            isAttacking = false;
            targetBuilding = nullptr;
        }
        
        return false;
    }
    
    // Speed control - only move/find targets when counter reaches speed
    speedCounter++;
    if (speedCounter >= speed) {
        speedCounter = 0;
        
        // Try to find any nearby target to attack
        Building* target = findTarget(walls, goldMines, elixirCollectors, townhall);
        
        // If adjacent to a building, attack it
        if (target) {
            // Check if this is a Raider trying to attack a wall, which shouldn't happen
            if (getType() == EnemyType::RAIDER) {
                for (auto& wall : walls) {
                    if (target == &wall) {
                        // Raiders don't attack walls, just stop in front of them
                        return false;
                    }
                }
            }
            
            isAttacking = true;
            targetBuilding = target;
            target->takeDamage(damage);
            
            // Check if we've destroyed townhall
            if (target == &townhall && townhall.getHealth() <= 0) {
                return true;  // Game over condition
            }
            
            return false;
        }
        
        Position myPos = getPosition();
        int dx = 0, dy = 0;
        
        // Determine base movement direction towards Town Hall
        if (myPos.x < targetPos.x) dx = 1;
        else if (myPos.x > targetPos.x) dx = -1;
        
        if (myPos.y < targetPos.y) dy = 1;
        else if (myPos.y > targetPos.y) dy = -1;
        
        // Add randomness to movement (different behaviors based on enemy type)
        if (getType() == EnemyType::RAIDER) {
            // Raiders are more direct but occasionally zigzag (20% chance)
            if (random_chance(gen) <= 2) {
                // Random horizontal or vertical deviation
                if (random_chance(gen) <= 5) {
                    dx += random_move(gen);
                } else {
                    dy += random_move(gen);
                }
            }
        } else if (getType() == EnemyType::BOMBERMAN) {
            // Bombermen are more erratic with higher chance of random movement (30% chance)
            if (random_chance(gen) <= 3) {
                dx += random_move(gen);
                dy += random_move(gen);
            }
        }
        
        // Ensure we have some movement and stay in bounds
        if (dx == 0 && dy == 0) {
            dx = random_move(gen);
            if (dx == 0) dx = 1;
        }
        
        // Create new position with randomized movement
        Position newPos(myPos.x + dx, myPos.y + dy);
        
        // Ensure enemy stays within game borders
        if (newPos.x < 31) newPos.x = 31;  // Left margin
        if (newPos.x > 145) newPos.x = 145;  // Right margin
        if (newPos.y < 1) newPos.y = 1;  // Top border
        if (newPos.y > 32) newPos.y = 32;  // Bottom border
        
        // Check if we would collide with a wall (important for Raiders)
        bool wallCollision = false;
        for (auto& wall : walls) {
            if (calculateDistance(newPos, wall.getPosition()) < 2) {
                wallCollision = true;
                
                // If this is a Raider, try to move around the wall
                if (getType() == EnemyType::RAIDER) {
                    // Try to find an alternative path around the wall
                    Position altPos1(myPos.x + dx, myPos.y);
                    Position altPos2(myPos.x, myPos.y + dy);
                    bool canMove1 = true, canMove2 = true;
                    
                    // Check if alternative positions would hit walls
                    for (auto& wall : walls) {
                        if (calculateDistance(altPos1, wall.getPosition()) < 2) canMove1 = false;
                        if (calculateDistance(altPos2, wall.getPosition()) < 2) canMove2 = false;
                    }
                    
                    // Choose an available alternative path
                    if (canMove1) {
                        newPos = altPos1;
                        wallCollision = false;
                    } else if (canMove2) {
                        newPos = altPos2;
                        wallCollision = false;
                    } else {
                        return false; // Still blocked, wait for next turn
                    }
                }
                // Bomberman will try to attack the wall through findTarget next turn
                break;
            }
        }
        
        // Move the enemy if no collision with wall (or is Bomberman who can destroy walls)
        if (!wallCollision || getType() == EnemyType::BOMBERMAN) {
            setPosition(newPos.x, newPos.y);
        }
    }
    return false;
}

/**
 * @brief Get enemy's damage value
 * 
 * @return Damage amount per attack
 */
int Enemy::getDamage() const {
    return damage;
}

/**
 * @brief Get the enemy's type
 * 
 * @return The EnemyType (RAIDER or BOMBERMAN)
 */
EnemyType Enemy::getType() const {
    return type;
}

/**
 * @brief Constructor for Raider enemy
 * 
 * Creates a Raider with:
 * - Icon: "🗡️" (sword)
 * - Damage: 15 points per attack
 * - Speed: 12 (faster than Bomberman)
 * 
 * @param x Initial X-coordinate
 * @param y Initial Y-coordinate
 */
Raider::Raider(int x, int y)
    : Enemy(x, y, EnemyType::RAIDER, "🗡️", 15, 12) {}

/**
 * @brief Find target for Raider
 * 
 * Raiders only attack resource buildings and townhall, completely ignoring walls
 * 
 * @param walls Vector of walls (ignored by Raiders)
 * @param goldMines Vector of gold mines
 * @param elixirCollectors Vector of elixir collectors
 * @param townhall Town hall reference
 * @return Pointer to the closest building, or nullptr if no building is in range
 */
Building* Raider::findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                          vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) {
    // Raiders only target resources and townhall, never walls
    Building* closestTarget = nullptr;
    double minDist = 1000000;  // Large initial value
    Position myPos = getPosition();
    
    // Check gold mines first (high priority)
    for (auto& mine : goldMines) {
        double dist = calculateDistance(myPos, mine.getPosition());
        if (dist < minDist) {
            minDist = dist;
            closestTarget = &mine;
        }
    }
    
    // Check elixir collectors (high priority)
    for (auto& collector : elixirCollectors) {
        double dist = calculateDistance(myPos, collector.getPosition());
        if (dist < minDist) {
            minDist = dist;
            closestTarget = &collector;
        }
    }
    
    // Check town hall (last priority but will attack if it's closest)
    double dist_th = calculateDistance(myPos, townhall.getPosition());
    if (dist_th < minDist) {
        minDist = dist_th;
        closestTarget = const_cast<TownHall*>(&townhall);
    }
    
    // Raiders completely ignore walls - no wall checking code here
    
    return (minDist < 2) ? closestTarget : nullptr;
}

/**
 * @brief Constructor for Bomberman enemy
 * 
 * Creates a Bomberman with:
 * - Icon: "💣" (bomb)
 * - Damage: 25 points per attack (higher than Raider)
 * - Speed: 20 (slower than Raider)
 * 
 * @param x Initial X-coordinate
 * @param y Initial Y-coordinate
 */
Bomberman::Bomberman(int x, int y)
    : Enemy(x, y, EnemyType::BOMBERMAN, "💣", 25, 20) {}

/**
 * @brief Find target for Bomberman
 * 
 * Bombermen prioritize walls over other buildings
 * 
 * @param walls Vector of walls
 * @param goldMines Vector of gold mines
 * @param elixirCollectors Vector of elixir collectors
 * @param townhall Town hall reference
 * @return Pointer to the target building, or nullptr if no building is in range
 */
Building* Bomberman::findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                             vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) {
    // Bombermen prioritize walls over other buildings
    Building* closestWall = nullptr;
    double minWallDist = 1000000;  // Large initial value
    Position myPos = getPosition();
    
    // Check walls
    for (auto& wall : walls) {
        double dist = calculateDistance(myPos, wall.getPosition());
        if (dist < minWallDist) {
            minWallDist = dist;
            closestWall = &wall;
        }
    }
    
    // If there's a wall nearby, target it
    if (closestWall && minWallDist < 2) {
        return closestWall;
    }
    
    // If no walls or walls are too far, check other buildings
    Building* closestOther = nullptr;
    double minOtherDist = 1000000;
    
    // Check gold mines
    for (auto& mine : goldMines) {
        double dist = calculateDistance(myPos, mine.getPosition());
        if (dist < minOtherDist) {
            minOtherDist = dist;
            closestOther = &mine;
        }
    }
    
    // Check elixir collectors
    for (auto& collector : elixirCollectors) {
        double dist = calculateDistance(myPos, collector.getPosition());
        if (dist < minOtherDist) {
            minOtherDist = dist;
            closestOther = &collector;
        }
    }
    
    // Check town hall
    double dist_th = calculateDistance(myPos, townhall.getPosition());
    if (dist_th < minOtherDist) {
        minOtherDist = dist_th;
        closestOther = const_cast<TownHall*>(&townhall);
    }
    
    return (minOtherDist < 2) ? closestOther : nullptr;
}


