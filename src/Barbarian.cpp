#include "Barbarian.h"
#include "Enemy.h"
#include <cmath>

// Barbarian constants
constexpr int BARBARIAN_HEALTH = 60;
constexpr int BARBARIAN_DAMAGE = 25;
constexpr int BARBARIAN_RANGE = 1;
constexpr int BARBARIAN_SPEED = 1;

Barbarian::Barbarian(int x, int y)
    : Troop(x, y, "🧔🏾‍♂️", BARBARIAN_HEALTH, BARBARIAN_DAMAGE, BARBARIAN_RANGE, BARBARIAN_SPEED) {
}

bool Barbarian::attack(std::shared_ptr<Enemy> target) {
    if (!target || target->getHealth() <= 0 || !isAlive()) {
        return false;
    }
    
    // Get positions
    Position myPos = getPosition();
    Position targetPos = target->getPosition();
    
    // Calculate distance to target
    int distance = std::abs(myPos.x - targetPos.x) +
                  std::abs(myPos.y - targetPos.y);
    
    // Barbarians can only attack adjacent enemies (range = 1)
    if (distance <= range) {
        int damage = getDamage();
        target->setHealth(target->getHealth() - damage);
        return true;
    }
    
    return false;
}
