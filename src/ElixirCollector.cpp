#include "ElixirCollector.h"

ElixirCollector::ElixirCollector(int x, int y) : ResourceGenerator(x, y, 7, 3, 100, 0, 100, 3, "💧", 100) {}

ElixirCollector& ElixirCollector::operator=(const ElixirCollector& other) {
    if (this != &other) {
        pos = other.pos;
        sizeX = other.sizeX;
        sizeY = other.sizeY;
        costGold = other.costGold;
        costElixir = other.costElixir;
        health = other.health;
        maxInstances = other.maxInstances;
        icon = other.icon;
        hasBorder = other.hasBorder;
        currentAmount = other.currentAmount;
    }
    return *this;
}

void ElixirCollector::update() {
    if (currentAmount < capacity) {
        currentAmount += 5;
        if (currentAmount >= capacity) {
            icon = "🧪";
        }
    }
}

int ElixirCollector::collect() {
    if (currentAmount >= capacity) {
        int collected = currentAmount;
        currentAmount = 0;
        icon = "‼️ ";
        return collected;
    }
    return 0;
}
