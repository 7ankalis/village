/**
 * @file GoldMine.cpp
 * @brief Implementation of the GoldMine class, which generates gold resources
 */

#include "GoldMine.h"

/**
 * @brief Constructor for GoldMine
 * 
 * Initializes a gold mine with the following properties:
 * - Size: 7x3 characters
 * - Cost: 0 gold, 100 elixir
 * - Health: 100 points
 * - Max instances: 3 per player
 * - Initial icon: "🪨" (rock)
 * - Resource capacity: 100 gold
 * 
 * @param x X-coordinate for mine placement
 * @param y Y-coordinate for mine placement
 */
GoldMine::GoldMine(int x, int y) : ResourceGenerator(x, y, 7, 3, 0, 100, 100, 3, "🪨", 100) {}

/**
 * @brief Assignment operator for GoldMine
 * 
 * Copies all properties from another GoldMine instance to this one
 * 
 * @param other The GoldMine to copy from
 * @return Reference to this GoldMine after assignment
 */
GoldMine& GoldMine::operator=(const GoldMine& other) {
    if (this != &other) {
        // Copy Building properties
        pos = other.pos;
        sizeX = other.sizeX;
        sizeY = other.sizeY;
        costGold = other.costGold;
        costElixir = other.costElixir;
        health = other.health;
        maxInstances = other.maxInstances;
        icon = other.icon;
        hasBorder = other.hasBorder;
        
        // Copy ResourceGenerator properties
        currentAmount = other.currentAmount;
    }
    return *this;
}

/**
 * @brief Updates the gold mine's resource accumulation
 * 
 * Generates 5 gold per update until reaching capacity
 * Changes icon to "🪙" (coin) when full
 */
void GoldMine::update() {
    if (currentAmount < capacity) {
        currentAmount += 5;
        if (currentAmount >= capacity) {
            icon = "🪙";  // Change to coin icon when full
        }
    }
}

/**
 * @brief Collects accumulated gold from the mine
 * 
 * @return The amount of gold collected if mine is full, 0 otherwise
 */
int GoldMine::collect() {
    if (currentAmount >= capacity) {
        int collected = currentAmount;
        currentAmount = 0;
        icon = "🪨";  // Reset to rock icon after collection
        return collected;
    }
    return 0;  // Nothing to collect yet
}
