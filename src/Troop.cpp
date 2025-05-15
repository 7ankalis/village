#include "Troop.h"
#include "Enemy.h"
#include <cmath>
#include <algorithm>

Troop::Troop(int x, int y, const std::string& repr, int health, int damage, int range, int speed)
    : Entity(x, y, repr), health(health), damage(damage), range(range), speed(speed) {
}

bool Troop::moveTowards(const Position& targetPosition) {
    // Get our current position
    Position currentPos = getPosition();
    
    // Calculate direction towards target
    int dx = targetPosition.getX() - currentPos.x;
    int dy = targetPosition.getY() - currentPos.y;
    
    // Normalize to get direction
    if (dx != 0) dx = dx > 0 ? 1 : -1;
    if (dy != 0) dy = dy > 0 ? 1 : -1;
    
    // Calculate new position
    Position newPos = currentPos;
    
    // Prioritize moving along the axis with the greater distance
    if (std::abs(targetPosition.getX() - currentPos.x) > std::abs(targetPosition.getY() - currentPos.y)) {
        if (dx != 0) {
            newPos.x += dx;
            setPosition(newPos.x, newPos.y);
            return true;
        } else if (dy != 0) {
            newPos.y += dy;
            setPosition(newPos.x, newPos.y);
            return true;
        }
    } else {
        if (dy != 0) {
            newPos.y += dy;
            setPosition(newPos.x, newPos.y);
            return true;
        } else if (dx != 0) {
            newPos.x += dx;
            setPosition(newPos.x, newPos.y);
            return true;
        }
    }
    
    return false; // No movement occurred
}

void Troop::takeDamage(int amount) {
    health = std::max(0, health - amount);
}

bool Troop::isAlive() const {
    return health > 0;
}

int Troop::getHealth() const {
    return health;
}

int Troop::getDamage() const {
    return damage;
}

int Troop::getRange() const {
    return range;
}

int Troop::getSpeed() const {
    return speed;
}
