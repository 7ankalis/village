/**
 * @file Building.cpp
 * @brief Implementation of the Building class, which is the base class for all structures in the game
 */

#include "Building.h"
using namespace std;

/**
 * @brief Constructor for the Building class
 * 
 * @param x X-coordinate for building placement
 * @param y Y-coordinate for building placement
 * @param sizeX Width of the building in characters
 * @param sizeY Height of the building in characters
 * @param costGold Gold cost to construct this building
 * @param costElixir Elixir cost to construct this building
 * @param health Initial and maximum health points of the building
 * @param maxInstances Maximum number of this building type allowed
 * @param icon Visual representation of the building (emoji or character)
 * @param hasBorder Whether the building should be drawn with a border
 */
Building::Building(int x, int y, int sizeX, int sizeY, int costGold, int costElixir, 
         int health, int maxInstances, const string& icon, bool hasBorder)
    : pos(x, y), sizeX(sizeX), sizeY(sizeY), costGold(costGold), 
      costElixir(costElixir), health(health), maxInstances(maxInstances), 
      icon(icon), hasBorder(hasBorder) {}

// Accessor methods

/**
 * @brief Get the building's current position
 * @return Reference to the Position object
 */
const Position& Building::getPosition() const { return pos; }

/**
 * @brief Get the building's visual representation
 * @return Reference to the icon string
 */
const string& Building::getIcon() const { return icon; }

/**
 * @brief Get the gold cost of the building
 * @return Gold cost value
 */
int Building::getCostGold() const { return costGold; }

/**
 * @brief Get the elixir cost of the building
 * @return Elixir cost value
 */
int Building::getCostElixir() const { return costElixir; }

/**
 * @brief Get the current health of the building
 * @return Current health value
 */
int Building::getHealth() const { return health; }

/**
 * @brief Get the maximum allowed instances of this building
 * @return Maximum instances value
 */
int Building::getMaxInstances() const { return maxInstances; }

/**
 * @brief Get the width of the building
 * @return Width in character units
 */
int Building::getSizeX() const { return sizeX; }

/**
 * @brief Get the height of the building
 * @return Height in character units
 */
int Building::getSizeY() const { return sizeY; }

/**
 * @brief Check if the building has a border
 * @return true if the building has a border, false otherwise
 */
bool Building::Border() const { return hasBorder; }

/**
 * @brief Set a new position for the building
 * 
 * @param x New X-coordinate
 * @param y New Y-coordinate
 */
void Building::setPosition(int x, int y) { pos = Position(x, y); }

/**
 * @brief Apply damage to the building
 * 
 * @param damage Amount of health to subtract
 */
void Building::takeDamage(int damage) { health -= damage; }
