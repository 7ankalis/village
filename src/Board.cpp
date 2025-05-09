#include "Board.h"
#include "Raider.h"
#include "Bomberman.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <unistd.h>
#include <termios.h>

using namespace std;

/* Constructor for Board class
 * Initializes:
 * - Player at starting position (margin+2, height/2)
 * - Townhall at (80, height/2)
 * - Left text area for displaying game stats
 * - Spawn counter and rate for enemies
 * - Game over flag set to false
 * - Enemy type counters
 */
Board::Board() : player(margin + 2, height / 2), 
                 townhall(80, height / 2),
                 leftTexts(height - 2, string(margin - 1, ' ')),
                 spawnCounter(0),
                 spawnRate(30),
                 gameOver(false),
                 raiderCount(0),
                 bombermanCount(0) {}

/* Draws a building on the console
 * Handles both bordered buildings (walls, mines, collectors) and simple icons
 * Uses ANSI escape codes for positioning
 */
void Board::drawBuilding(const Building& building) const {
    int startX = building.getPosition().x;
    int startY = building.getPosition().y;
    string icon = building.getIcon();

    if (building.Border()) {
        int sizeX = building.getSizeX();
        int sizeY = building.getSizeY();

        // Draw top border
        cout << "\033[" << startY << ";" << startX << "H+";
        for (int i = 0; i < sizeX - 2; ++i) cout << "-";
        cout << "+";

        // Draw middle rows with icon centered
        for (int j = 1; j < sizeY - 1; ++j) {
            cout << "\033[" << startY + j << ";" << startX << "H|";
            for (int i = 1; i < sizeX - 1; ++i) {
                if (i == sizeX/2 && j == sizeY/2) {
                    cout << icon;
                    if (i < sizeX - 2) ++i;  // Skip next position if icon is 2 chars wide
                } else {
                    cout << " ";
                }
            }
            cout << "|";
        }

        // Draw bottom border
        cout << "\033[" << startY + sizeY - 1 << ";" << startX << "H+";
        for (int i = 0; i < sizeX - 2; ++i) cout << "-";
        cout << "+";
    } else {
        // Simple icon without border
        cout << "\033[" << startY << ";" << startX << "H" << icon;
    }
}

/* Checks if two buildings are colliding by comparing their bounding boxes
 * Returns true if buildings overlap, false otherwise
 */
bool Board::areBuildingsColliding(const Building& b1, const Building& b2) const {
    // Calculate bounding boxes for both buildings
    int x1_min = b1.getPosition().x;
    int y1_min = b1.getPosition().y;
    int x1_max = x1_min + b1.getSizeX();
    int y1_max = y1_min + b1.getSizeY();

    int x2_min = b2.getPosition().x;
    int y2_min = b2.getPosition().y;
    int x2_max = x2_min + b2.getSizeX();
    int y2_max = y2_min + b2.getSizeY();

    // Check for non-overlapping conditions
    bool noOverlap = x1_max <= x2_min || x2_max <= x1_min ||
                     y1_max <= y2_min || y2_max <= y1_min;

    return !noOverlap;  // Return true if buildings overlap
}

/* Checks if a specific position is occupied by any building
 * Can ignore a specific building (useful when checking movement for existing buildings)
 */
bool Board::isPositionOccupied(const Position& pos, const Building* ignore) const {
    for (const auto& wall : walls) {
        if (ignore != &wall &&
            areBuildingsColliding(Building(pos.x, pos.y, 1, 1, 0, 0, 0, 0, "", false), wall)) {
            return true;
        }
    }
    return false;
}

/* Checks if a building can be placed at its current location
 * Verifies collisions with all other buildings except the one being ignored
 */
bool Board::CanBuild(const Building* building, const Building* ignore) const {
    // Check against walls
    for (const auto& wall : walls) {
        if (ignore != &wall && areBuildingsColliding(*building, wall)) return false;
    }
    
    // Check against gold mines
    for (const auto& mine : goldMines) {
        if (ignore != &mine && areBuildingsColliding(*building, mine)) return false;
    }
    
    // Check against elixir collectors
    for (const auto& collector : elixirCollectors) {
        if (ignore != &collector && areBuildingsColliding(*building, collector)) return false;
    }
    
    // Check against townhall
    if (areBuildingsColliding(*building, townhall)) return false;

    return true;  // No collisions found
}

/* Spawns enemies at random positions around the map edges
 * Uses a counter and spawn rate to control frequency
 * Enemies spawn from all four edges with varied patterns
 */
void Board::spawnEnemy() {
    spawnCounter++;
    if (spawnCounter >= spawnRate) {
        spawnCounter = 0;
        
        // Random number generation setup
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> edge_dis(0, 3);  // 0=top, 1=right, 2=bottom, 3=left
        uniform_int_distribution<> type_dis(0, 9);  // For enemy type (0-9)
        
        // Distributions for different edges - defined outside the switch
        uniform_int_distribution<> x_dis(margin + 1, width - 2);
        uniform_int_distribution<> y_dis(1, height - 2);
        
        // Determine spawn position (random edge)
        int x, y;
        int edge = edge_dis(gen);
        
        switch (edge) {
            case 0: // Top edge
                x = x_dis(gen);
                y = 1;
                break;
                
            case 1: // Right edge
                x = width - 2;
                y = y_dis(gen);
                break;
                
            case 2: // Bottom edge
                x = x_dis(gen);
                y = height - 2;
                break;
                
            case 3: // Left edge
                x = margin + 1;
                y = y_dis(gen);
                break;
        }
        
        // Determine enemy type: 0-3 for Raiders (40%), 4-9 for Bombermen (60%)
        int enemyTypeRoll = type_dis(gen);
        if (enemyTypeRoll < 4) {
            enemies.push_back(make_unique<Raider>(x, y));
            raiderCount++;
        } else {
            enemies.push_back(make_unique<Bomberman>(x, y));
            bombermanCount++;
        }
        
        // Occasionally spawn groups of enemies (10% chance)
        if (type_dis(gen) < 1) {
            // Spawn a small cluster of 1-2 additional enemies around the same point
            int extraEnemies = 1 + (rand() % 2); // Simple random 1-2 to avoid distribution scope issues
            
            for (int i = 0; i < extraEnemies; i++) {
                // Generate small random offsets (-3 to +3)
                int offsetX = rand() % 7 - 3;
                int offsetY = rand() % 7 - 3;
                
                int groupX = x + offsetX;
                int groupY = y + offsetY;
                
                // Keep within bounds
                if (groupX < margin + 1) groupX = margin + 1;
                if (groupX > width - 2) groupX = width - 2;
                if (groupY < 1) groupY = 1;
                if (groupY > height - 2) groupY = height - 2;
                
                // 50/50 chance of same or different enemy type
                int groupTypeRoll = rand() % 10; // Simple random 0-9
                
                if (groupTypeRoll < 5) {
                    // Same type as original
                    if (enemyTypeRoll < 4) {
                        enemies.push_back(make_unique<Raider>(groupX, groupY));
                        raiderCount++;
                    } else {
                        enemies.push_back(make_unique<Bomberman>(groupX, groupY));
                        bombermanCount++;
                    }
                } else {
                    // Different type as original
                    if (enemyTypeRoll < 4) {
                        enemies.push_back(make_unique<Bomberman>(groupX, groupY));
                        bombermanCount++;
                    } else {
                        enemies.push_back(make_unique<Raider>(groupX, groupY));
                        raiderCount++;
                    }
                }
            }
        }
    }
}

/* Updates all enemies' positions and checks for game over condition
 * Also removes destroyed buildings
 */
void Board::updateEnemies() {
    for (auto& enemy : enemies) {
        if (enemy->update(townhall.getPosition(), walls, goldMines, elixirCollectors, townhall)) {
            gameOver = true;  // Townhall was destroyed
            return;           // No need to continue; game is over
        }
    }

    // Remove destroyed buildings
    walls.erase(remove_if(walls.begin(), walls.end(), 
        [](const Wall& w) { return w.getHealth() <= 0; }), walls.end());
    goldMines.erase(remove_if(goldMines.begin(), goldMines.end(), 
        [](const GoldMine& m) { return m.getHealth() <= 0; }), goldMines.end());
    elixirCollectors.erase(remove_if(elixirCollectors.begin(), elixirCollectors.end(), 
        [](const ElixirCollector& e) { return e.getHealth() <= 0; }), elixirCollectors.end());
}

/* Attempts to move player in specified direction
 * Returns true if move was successful, false if blocked
 */
bool Board::tryMovePlayer(char direction) {
    Position newPos = player.getPosition();
    switch(direction) {
        case 'U': if (newPos.y > 1) newPos.y--; break;
        case 'D': if (newPos.y < height - 2) newPos.y++; break;
        case 'L': if (newPos.x > margin + 2) newPos.x -= 2; break;
        case 'R': if (newPos.x < width - 4) newPos.x += 2; break;
        default: return false;
    }
    
    if (!isPositionOccupied(newPos)) {
        player.setPosition(newPos.x, newPos.y);
        return true;
    }
    return false;
}

/* Attempts to place a wall at player's current position
 * Checks building rules, collisions, and resource availability
 */
bool Board::placeWall() {
    Position pos = player.getPosition();
    Wall newWall(pos.x, pos.y);

    if (!CanBuild(&newWall)) return false;
    if (walls.size() >= static_cast<size_t>(newWall.getMaxInstances())) return false;

    if (player.getResources().gold >= newWall.getCostGold() && 
        player.getResources().elixir >= newWall.getCostElixir()) {
        player.getResources().spendGold(newWall.getCostGold());
        player.getResources().spendElixir(newWall.getCostElixir());
        walls.push_back(newWall);
        return true;
    }

    return false;
}

/* Attempts to place a gold mine centered on player's position
 * Checks building rules, collisions, and resource availability
 */
bool Board::placeGoldMine() {
    Position pos = player.getPosition();
    GoldMine newMine(0, 0);
    int centerX = pos.x - newMine.getSizeX() / 2;
    int centerY = pos.y - newMine.getSizeY() / 2;
    GoldMine mineToPlace(centerX, centerY);

    if (!CanBuild(&mineToPlace)) return false;
    if (goldMines.size() >= static_cast<size_t>(newMine.getMaxInstances())) return false;

    if (player.getResources().elixir >= newMine.getCostElixir()) {
        player.getResources().spendElixir(newMine.getCostElixir());
        goldMines.push_back(mineToPlace);
        return true;
    }

    return false;
}

/* Attempts to place an elixir collector centered on player's position
 * Checks building rules, collisions, and resource availability
 */
bool Board::placeElixirCollector() {
    Position pos = player.getPosition();
    ElixirCollector newCollector(0, 0);
    int centerX = pos.x - newCollector.getSizeX() / 2;
    int centerY = pos.y - newCollector.getSizeY() / 2;
    ElixirCollector collectorToPlace(centerX, centerY);

    if (!CanBuild(&collectorToPlace)) return false;
    if (elixirCollectors.size() >= static_cast<size_t>(newCollector.getMaxInstances())) return false;

    if (player.getResources().gold >= newCollector.getCostGold()) {
        player.getResources().spendGold(newCollector.getCostGold());
        elixirCollectors.push_back(collectorToPlace);
        return true;
    }

    return false;
}

/* Collects resources from buildings player is currently standing on
 * Only collects from one building of each type per call
 */
void Board::collectResources() {
    Position pos = player.getPosition();

    // Check gold mines
    for (auto& mine : goldMines) {
        Position bPos = mine.getPosition();
        if (pos.x >= bPos.x && pos.x < bPos.x + mine.getSizeX() &&
            pos.y >= bPos.y && pos.y < bPos.y + mine.getSizeY()) {
            int collected = mine.collect();
            if (collected > 0) {
                player.getResources().gold += collected;
                break;  // Only collect from one mine at a time
            }
        }
    }

    // Check elixir collectors
    for (auto& collector : elixirCollectors) {
        Position bPos = collector.getPosition();
        if (pos.x >= bPos.x && pos.x < bPos.x + collector.getSizeX() &&
            pos.y >= bPos.y && pos.y < bPos.y + collector.getSizeY()) {
            int collected = collector.collect();
            if (collected > 0) {
                player.getResources().elixir += collected;
                break;  // Only collect from one collector at a time
            }
        }
    }
}

/* Updates resource production for all resource-generating buildings */
void Board::updateResources() {
    for (auto& mine : goldMines) mine.update();
    for (auto& collector : elixirCollectors) collector.update();
}

/* Main game update function - handles enemy spawning, movement, and resource updates */
void Board::update() {
    if (gameOver) return;
    spawnEnemy();
    updateEnemies();
    updateResources();
}

/* Renders the entire game state including:
 * - Borders and UI
 * - All buildings
 * - Enemies
 * - Player
 * - Game over message if applicable
 */
void Board::render() {
    system("clear");
    renderTopBorder();
    renderMiddle();
    renderBottomBorder();

    // Draw all game objects
    drawBuilding(townhall);
    for (const auto& wall : walls) drawBuilding(wall);
    for (const auto& mine : goldMines) drawBuilding(mine);
    for (const auto& collector : elixirCollectors) drawBuilding(collector);

    // Draw enemies
    for (const auto& enemy : enemies) {
        cout << "\033[" << enemy->getPosition().y << ";" << enemy->getPosition().x << "H";
        cout << enemy->getIcon();
    }

    // Draw player
    cout << "\033[" << player.getPosition().y << ";" << player.getPosition().x << "H";
    cout << player.getIcon();

    // Game over handling
    if (gameOver) {
        string message = "GAME OVER - Town Hall Destroyed!";
        cout << "\033[" << height/2 << ";" << (width - message.length())/2 << "H";
        cout << message;
        cout << "\033[" << height << ";0H";
        exit(0);
    }
}

/* Renders the top border of the game UI with margin separator */
void Board::renderTopBorder() const {
    cout << "+";
    for (int x = 1; x < width - 1; x++) {
        cout << (x == margin ? "+" : "-");
    }
    cout << "+" << endl;
}

/* Renders the bottom border of the game UI with margin separator */
void Board::renderBottomBorder() const {
    cout << "+";
    for (int x = 1; x < width - 1; x++) {
        cout << (x == margin ? "+" : "-");
    }
    cout << "+" << endl;
}

/* Renders the middle section of the game UI including:
 * - Resource counts
 * - Building counts
 * - Townhall health
 * - Enemy count
 */
void Board::renderMiddle() const {
    for (int y = 1; y < height - 1; y++) {
        cout << "|";

        // Display various game stats in the left margin
        if (y == 1) {
            string line = "Gold = " + to_string(player.getResources().gold);
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 2) {
            string line = "Elixir = " + to_string(player.getResources().elixir);
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 3) {
            string line = "Walls = " + to_string(walls.size()) + "/200";
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 4) {
            string line = "Gold Mines = " + to_string(goldMines.size()) + "/3";
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 5) {
            string line = "Elixir Generators = " + to_string(elixirCollectors.size()) + "/3";
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 6) {
            string line = "Town Hall HP = " + to_string(townhall.getHealth());
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 7) {
            string line = "Enemies = " + to_string(enemies.size());
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 8) {
            string line = "Raiders = " + to_string(raiderCount);
            cout << line << string(margin - 1 - line.length(), ' ');
        } else if (y == 9) {
            string line = "Bombermen = " + to_string(bombermanCount);
            cout << line << string(margin - 1 - line.length(), ' ');
        } else {
            cout << string(margin - 1, ' ');
        }

        cout << "|";
        cout << string(width - margin - 2, ' ') << "|" << endl;
    }
}