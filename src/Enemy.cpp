/**
 * @file Enemy.cpp
 * @brief Implementation of the Enemy class, representing opponents that attack player buildings
 */

#include "Enemy.h"
#include "Wall.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "TownHall.h"

/**
 * @brief Constructor for Enemy
 * 
 * Creates an enemy with the following properties:
 * - Icon: "👹" (ogre)
 * - Damage: 10 points per attack
 * - Speed: Moves once every 3 update cycles
 * 
 * @param x Initial X-coordinate
 * @param y Initial Y-coordinate
 */
Enemy::Enemy(int x, int y) : Npc(x, y, "👹"), damage(10), speedCounter(0), speed(3),
                             isAttacking(false), targetBuilding(nullptr) {}

/**
 * @brief Updates enemy position and handles attacks on buildings
 * 
 * This method:
 * 1. Checks if the enemy has reached the town hall (game over condition)
 * 2. Attacks any buildings it encounters (walls, mines, collectors)
 * 3. Moves toward the target position (usually the town hall) if not attacking
 * 
 * @param targetPos Target position (usually town hall position)
 * @param walls Vector of walls that can be attacked
 * @param goldMines Vector of gold mines that can be attacked
 * @param elixirCollectors Vector of elixir collectors that can be attacked
 * @param townhall Town hall reference, used to check game over condition
 * @return true if town hall is reached (game over), false otherwise
 */
bool Enemy::update(const Position& targetPos, vector<Wall>& walls, vector<GoldMine>& goldMines,
                   vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) {
    // Speed control - only move/attack when counter reaches speed
    speedCounter++;
    if (speedCounter >= speed) {
        speedCounter = 0;

        Position pos = getPosition();

        // Check if enemy has reached the town hall (game over condition)
        Position thPos = townhall.getPosition();
        if (pos.x >= thPos.x && pos.x < thPos.x + townhall.getSizeX() &&
            pos.y >= thPos.y && pos.y < thPos.y + townhall.getSizeY()) {
            return true;  // Game over - town hall reached
        }

        // Check for walls to attack
        for (auto& wall : walls) {
            if (pos.x == wall.getPosition().x && pos.y == wall.getPosition().y && wall.getHealth() > 0) {
                isAttacking = true;
                targetBuilding = &wall;
                targetBuilding->takeDamage(damage);
                if (targetBuilding->getHealth() <= 0) {
                    isAttacking = false;
                    targetBuilding = nullptr;
                }
                return false;
            }
        }

        // Check for gold mines to attack
        for (auto& mine : goldMines) {
            Position minePos = mine.getPosition();
            if (pos.x >= minePos.x && pos.x < minePos.x + mine.getSizeX() &&
                pos.y >= minePos.y && pos.y < minePos.y + mine.getSizeY() && mine.getHealth() > 0) {
                isAttacking = true;
                targetBuilding = &mine;
                targetBuilding->takeDamage(damage);
                if (targetBuilding->getHealth() <= 0) {
                    isAttacking = false;
                    targetBuilding = nullptr;
                }
                return false;
            }
        }

        // Check for elixir collectors to attack
        for (auto& collector : elixirCollectors) {
            Position colPos = collector.getPosition();
            if (pos.x >= colPos.x && pos.x < colPos.x + collector.getSizeX() &&
                pos.y >= colPos.y && pos.y < colPos.y + collector.getSizeY() && collector.getHealth() > 0) {
                isAttacking = true;
                targetBuilding = &collector;
                targetBuilding->takeDamage(damage);
                if (targetBuilding->getHealth() <= 0) {
                    isAttacking = false;
                    targetBuilding = nullptr;
                }
                return false;
            }
        }

        // Move toward target position if not attacking
        // Simple pathfinding - move in both X and Y directions toward target
        if (pos.x < targetPos.x) pos.x++;
        else if (pos.x > targetPos.x) pos.x--;

        if (pos.y < targetPos.y) pos.y++;
        else if (pos.y > targetPos.y) pos.y--;

        setPosition(pos.x, pos.y);
    }
    return false;  // Game continues
}

/**
 * @brief Get enemy's damage value
 * 
 * @return Damage amount per attack
 */
int Enemy::getDamage() const {
    return damage;
}


