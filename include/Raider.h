#ifndef RAIDER_H
#define RAIDER_H

#include "Enemy.h"
#include <vector>

/**
 * @brief Raider enemy class
 * 
 * Raiders attack any building except walls, prioritizing resource buildings and townhall.
 * They are faster than Bombermen but deal less damage.
 */
class Raider : public Enemy {
public:
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
    Raider(int x, int y);
    
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
    Building* findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) override;
};

#endif // RAIDER_H
