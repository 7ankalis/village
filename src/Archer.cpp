#include "Archer.h"
#include "Enemy.h"
#include <cmath>

// Archer constants
constexpr int ARCHER_HEALTH = 30;
constexpr int ARCHER_DAMAGE = 15;
constexpr int ARCHER_RANGE = 4;
constexpr int ARCHER_SPEED = 1;

Archer::Archer(int x, int y)
    : Troop(x, y, "🏹", ARCHER_HEALTH, ARCHER_DAMAGE, ARCHER_RANGE, ARCHER_SPEED) {
}

bool Archer::attack(std::shared_ptr<Enemy> target) {
    if (!target || target->getHealth() <= 0 || !isAlive()) {
        return false;
    }
    
    // Get positions
    Position myPos = getPosition();
    Position targetPos = target->getPosition();
    
    // Calculate distance to target
    int distance = std::abs(myPos.x - targetPos.x) +
                  std::abs(myPos.y - targetPos.y);
    
    // Check if target is within range
    if (distance <= range) {
        int damage = getDamage();
        target->setHealth(target->getHealth() - damage);
        return true;
    }
    
    return false;
}
