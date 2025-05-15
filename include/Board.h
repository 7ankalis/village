#ifndef BOARD_H
#define BOARD_H
using namespace std;
#include "Player.h"
#include "TownHall.h"
#include "Wall.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "Enemy.h"
#include "Troop.h"
#include "Archer.h"
#include "Barbarian.h"
#include <vector>
#include <string>
#include <memory>

class Board {
private:
    const int width = 147;
    const int height = 33;
    const int margin = 30;

    Player player;
    TownHall townhall;
    vector<Wall> walls;
    vector<GoldMine> goldMines;
    vector<ElixirCollector> elixirCollectors;
    vector<unique_ptr<Enemy>> enemies;
    vector<unique_ptr<Troop>> troops;  // Collection of troops
    vector<string> leftTexts;
    int spawnCounter;
    const int spawnRate;
    bool gameOver;
    
    // Track enemy types for UI
    int raiderCount;
    int bombermanCount;
    
    // Track troop counts for UI
    int archerCount = 0;
    int barbarianCount = 0;

    void drawBuilding(const Building& building) const;
    bool areBuildingsColliding(const Building& b1, const Building& b2) const;
    bool isPositionOccupied(const Position& pos, const Building* ignore = nullptr) const;
    bool CanBuild(const Building* building, const Building* ignore = nullptr) const;
    void spawnEnemy();
    void updateEnemies();
    void updateTroops();  // New method to update troops
    void renderTopBorder() const;
    void renderBottomBorder() const;
    void renderMiddle() const;

public:
    Board();
    bool tryMovePlayer(char direction);
    bool placeWall();
    bool placeGoldMine();
    bool placeElixirCollector();
    bool trainArcher();    // Train an archer
    bool trainBarbarian(); // Train a barbarian
    void collectResources();
    void updateResources();
    void update();
    void render();
    
    // Add a troop to the board
    template<typename T>
    bool addTroop(unique_ptr<T> troop) {
        troops.push_back(std::move(troop));
        // Increment appropriate counter based on troop type
        if (dynamic_cast<Archer*>(troops.back().get())) {
            archerCount++;
        } else if (dynamic_cast<Barbarian*>(troops.back().get())) {
            barbarianCount++;
        }
        return true;
    }
};

#endif
