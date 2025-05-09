/**
 * @file ElixirCollector.cpp
 * @brief Implementation of the ElixirCollector class, which generates elixir resources
 */

#include "ElixirCollector.h"

/**
 * @brief Constructor for ElixirCollector
 * 
 * Initializes an elixir collector with the following properties:
 * - Size: 7x3 characters
 * - Cost: 100 gold, 0 elixir
 * - Health: 100 points
 * - Max instances: 3 per player
 * - Initial icon: "💧" (water drop)
 * - Resource capacity: 100 elixir
 * 
 * @param x X-coordinate for collector placement
 * @param y Y-coordinate for collector placement
 */
ElixirCollector::ElixirCollector(int x, int y) : ResourceGenerator(x, y, 7, 3, 100, 0, 100, 3, "💧", 100) {}

/**
 * @brief Assignment operator for ElixirCollector
 * 
 * Copies all properties from another ElixirCollector instance to this one
 * 
 * @param other The ElixirCollector to copy from
 * @return Reference to this ElixirCollector after assignment
 */
ElixirCollector& ElixirCollector::operator=(const ElixirCollector& other) {
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
 * @brief Updates the elixir collector's resource accumulation
 * 
 * Generates 5 elixir per update until reaching capacity
 * Changes icon to "🧪" (test tube) when full
 */
void ElixirCollector::update() {
    if (currentAmount < capacity) {
        currentAmount += 5;
        if (currentAmount >= capacity) {
            icon = "🧪";  // Change to test tube icon when full
        }
    }
}

/**
 * @brief Collects accumulated elixir from the collector
 * 
 * @return The amount of elixir collected if collector is full, 0 otherwise
 */
int ElixirCollector::collect() {
    if (currentAmount >= capacity) {
        int collected = currentAmount;
        currentAmount = 0;
        icon = "‼️ ";  // Reset to empty icon after collection
        return collected;
    }
    return 0;  // Nothing to collect yet
}
