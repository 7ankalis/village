#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include "Enemy.h"
#include <vector>

/**
 * @brief Bomberman enemy class
 * 
 * Bombermen prioritize destroying walls before attacking other buildings.
 * They are slower than Raiders but deal more damage.
 */
class Bomberman : public Enemy {
public:
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
    Bomberman(int x, int y);
    
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
    Building* findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) override;
};

#endif // BOMBERMAN_H
