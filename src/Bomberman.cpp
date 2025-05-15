/**
 * @file Bomberman.cpp
 * @brief Implementation of the Bomberman enemy class
 */

#include "Bomberman.h"
#include <cmath>

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
