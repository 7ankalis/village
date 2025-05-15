/**
 * @file Raider.cpp
 * @brief Implementation of the Raider enemy class
 */

#include "Raider.h"
#include <cmath>

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
 * Raiders attack any building except walls: prioritizing resource buildings and townhall
 * 
 * @param walls Vector of walls (ignored by Raiders)
 * @param goldMines Vector of gold mines (high priority)
 * @param elixirCollectors Vector of elixir collectors (high priority)
 * @param townhall Town hall reference (attacked if closest)
 * @return Pointer to the closest building to attack, or nullptr if no building is in range
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
    
    // Check town hall (will attack if it's closest)
    double dist_th = calculateDistance(myPos, townhall.getPosition());
    if (dist_th < minDist) {
        minDist = dist_th;
        closestTarget = const_cast<TownHall*>(&townhall);
    }
    
    // Raiders completely ignore walls - no wall checking code here
    
    return (minDist < 2) ? closestTarget : nullptr;
}
