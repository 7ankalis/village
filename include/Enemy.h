#ifndef ENEMY_H
#define ENEMY_H
using namespace std;
#include "Npc.h"
#include "Wall.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "TownHall.h"
#include <vector>

// Define enemy types
enum class EnemyType {
    RAIDER,   // Attacks all buildings
    BOMBERMAN // Specializes in destroying walls
};

class Enemy : public Npc {
protected:
    int damage;
    int speedCounter;
    const int speed;
    bool isAttacking;
    Building* targetBuilding;
    EnemyType type;

public:
    Enemy(int x, int y, EnemyType type, const string& icon = "👹", int dmg = 10, int spd = 15);
    virtual ~Enemy() = default;

    virtual bool update(const Position& targetPos, vector<Wall>& walls, vector<GoldMine>& goldMines,
                vector<ElixirCollector>& elixirCollectors, const TownHall& townhall);
    
    int getDamage() const;
    EnemyType getType() const;
    
    // Find closest building according to enemy's preference
    virtual Building* findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                      vector<ElixirCollector>& elixirCollectors, const TownHall& townhall);
                      
    // Helper function to calculate distance between positions
    int calculateDistance(const Position& pos1, const Position& pos2) const;
};

class Raider : public Enemy {
public:
    Raider(int x, int y);
    
    // Override to prioritize resource buildings and townhall
    Building* findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) override;
};

class Bomberman : public Enemy {
public:
    Bomberman(int x, int y);
    
    // Override to prioritize walls
    Building* findTarget(vector<Wall>& walls, vector<GoldMine>& goldMines,
                vector<ElixirCollector>& elixirCollectors, const TownHall& townhall) override;
};

#endif


