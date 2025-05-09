/**
 * @file Player.cpp
 * @brief Implementation of the Player class, representing the user-controlled character
 */

#include "Player.h"

/**
 * @brief Constructor for Player
 * 
 * Creates a player with the following properties:
 * - Icon: "👷" (construction worker)
 * - Starting resources: 400 gold and 400 elixir
 * - Position: Specified by constructor parameters
 * 
 * @param x Initial X-coordinate
 * @param y Initial Y-coordinate
 */
Player::Player(int x, int y) : Entity(x, y, "👷"), resources(400, 400) {}

/**
 * @brief Get mutable reference to player's resources
 * 
 * Used when player resources need to be modified (collecting, spending)
 * 
 * @return Reference to Resources object
 */
Resources& Player::getResources() { return resources; }

/**
 * @brief Get immutable reference to player's resources
 * 
 * Used for read-only access to resource values
 * 
 * @return Const reference to Resources object
 */
const Resources& Player::getResources() const { return resources; }
