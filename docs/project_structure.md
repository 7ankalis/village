# VillageGame Project Structure Documentation

This document provides a comprehensive overview of the VillageGame codebase structure, including class hierarchies, inheritance relationships, attributes, methods, and their functionality.

## Table of Contents
1. [Game Overview](#game-overview)
2. [Basic Classes](#basic-classes)
   - [Position](#position)
   - [Resources](#resources)
3. [Entity Hierarchy](#entity-hierarchy)
   - [Entity Base Class](#entity-base-class)
   - [Player](#player)
   - [NPC](#npc)
   - [Enemy Hierarchy](#enemy-hierarchy)
4. [Building Hierarchy](#building-hierarchy)
   - [Building Base Class](#building-base-class)
   - [Town Hall](#town-hall)
   - [Wall](#wall)
   - [Resource Generators](#resource-generators)
     - [Gold Mine](#gold-mine)
5. [Building Instructions](#building-instructions)

## Building Instructions

### Prerequisites
- CMake (version 3.10 or higher)
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Git (for cloning the repository)

### Build Steps

#### Linux/macOS
1. Clone the repository (if you haven't already):
   ```bash
   git clone https://github.com/yourusername/VillageGame.git
   cd VillageGame
   ```

2. Create a build directory and navigate to it:
   ```bash
   mkdir -p build
   cd build
   ```

3. Generate the build files using CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   make
   ```

5. Run the game:
   ```bash
   ./game
   ```

#### Windows with Visual Studio
1. Clone the repository (if you haven't already)
2. Open the folder in Visual Studio with "Open Folder" option
3. Visual Studio should automatically detect the CMakeLists.txt and configure the project
4. Build the project using the Build menu or by pressing F7
5. Run the game by pressing F5 or through the Debug menu

#### Windows with CMake GUI
1. Clone the repository
2. Open CMake GUI
3. Set the source directory to the repository root
4. Set the build directory to a new folder (e.g., build)
5. Click "Configure" and select your compiler
6. Click "Generate" to create the project files
7. Open the generated project in your IDE and build

### Troubleshooting
- If you encounter missing header files, ensure that your IDE or build system correctly includes the `include/` directory
- For compilation errors related to C++17 features, verify that your compiler supports C++17 and is correctly configured
- If certain source files appear to be missing during compilation, check that all source files in the `src/` directory are included in your build

### Project Structure
- `include/`: Header files
- `src/`: Source files
- `docs/`: Documentation
- `CMakeLists.txt`: CMake configuration file
     - [Elixir Collector](#elixir-collector)
5. [Game Board](#game-board)
6. [Input Handling](#input-handling)
7. [Game Loop](#game-loop)

---

## Game Overview

VillageGame is a console-based strategy game where players build and defend a village against incoming enemy attackers. The player controls a builder character who can move around the map, construct different types of buildings, collect resources, and strategically defend their town hall from enemies that spawn at the edges of the map.

### Core Game Mechanics

- **Resource Management**: Collect and spend gold and elixir to build structures
- **Building Placement**: Strategically place defensive walls and resource-generating buildings
- **Enemy Defense**: Defend against different enemy types with varying behaviors
- **Turn-Based Gameplay**: Enemies move and attack when the player takes movement actions

---

## Basic Classes

### Position

The `Position` class represents 2D coordinates in the game world.

**Attributes**:
- `x` (int): X-coordinate position
- `y` (int): Y-coordinate position

**Methods**:
- `Position(int x = 0, int y = 0)`: Constructor with default values
- `bool operator==(const Position& other) const`: Equality comparison operator

### Resources

The `Resources` class tracks a player's available resources.

**Attributes**:
- `gold` (int): Amount of gold resources
- `elixir` (int): Amount of elixir resources

**Methods**:
- `void spendGold(int amount)`: Reduces gold by specified amount
- `void spendElixir(int amount)`: Reduces elixir by specified amount

---

## Entity Hierarchy

### Entity Base Class

The `Entity` class serves as the base for all movable objects in the game.

**Attributes**:
- `pos` (Position): Current position on the game board
- `icon` (string): Visual representation (emoji) displayed on the board

**Methods**:
- `Entity(int x, int y, const string& icon)`: Constructor
- `const Position& getPosition() const`: Returns the entity's position
- `const string& getIcon() const`: Returns the entity's visual representation
- `void setPosition(int x, int y)`: Updates the entity's position

### Player

The `Player` class represents the user-controlled character in the game.

**Inheritance**: Inherits from `Entity`

**Attributes**:
- All attributes from `Entity`
- `resources` (Resources): Player's current resource stockpile

**Methods**:
- `Player(int x, int y)`: Constructor initializing player with default icon
- `Resources& getResources()`: Returns mutable reference to resources
- `const Resources& getResources() const`: Returns immutable reference to resources

### NPC

The `Npc` class is the base class for all non-player characters.

**Inheritance**: Inherits from `Entity`

**Methods**:
- `Npc(int x, int y, const string& icon)`: Constructor with position and visual representation

### Enemy Hierarchy

The base `Enemy` class and its derived types implement AI-controlled opponents.

#### Enemy Base Class

**Inheritance**: Inherits from `Npc`

**Attributes**:
- All attributes from `Npc`
- `damage` (int): Amount of damage inflicted on buildings
- `speedCounter` (int): Counter for controlling movement speed
- `speed` (const int): How frequently the enemy moves (lower is faster)
- `isAttacking` (bool): Flag indicating whether enemy is attacking
- `targetBuilding` (Building*): Pointer to building being attacked
- `type` (EnemyType): Type of enemy (RAIDER or BOMBERMAN)

**Methods**:
- `Enemy(int x, int y, EnemyType type, const string& icon, int dmg, int spd)`: Constructor
- `bool update(...)`: Updates enemy position/state and returns true if townhall is destroyed
- `int getDamage() const`: Returns damage value
- `EnemyType getType() const`: Returns enemy type
- `Building* findTarget(...)`: Finds closest building according to preferences
- `int calculateDistance(const Position& pos1, const Position& pos2) const`: Utility function

#### Raider

Raiders are enemies that prioritize attacking resource buildings and the town hall.

**Inheritance**: Inherits from `Enemy`

**Attributes**:
- All attributes from `Enemy`

**Methods**:
- `Raider(int x, int y)`: Constructor that initializes raider with sword icon "🗡️"
- `Building* findTarget(...)` override: Prioritizes resource buildings and town hall, ignores walls

#### Bomberman

Bombermen are specialized enemies that focus on destroying walls first.

**Inheritance**: Inherits from `Enemy`

**Attributes**:
- All attributes from `Enemy`

**Methods**:
- `Bomberman(int x, int y)`: Constructor that initializes bomberman with bomb icon "💣"
- `Building* findTarget(...)` override: Prioritizes walls over other buildings

---

## Building Hierarchy

### Building Base Class

The `Building` class serves as the base for all static structures.

**Attributes**:
- `pos` (Position): Location on the game board
- `sizeX`, `sizeY` (int): Dimensions in console characters
- `costGold`, `costElixir` (int): Resource costs to construct
- `health` (int): Current health points
- `maxInstances` (int): Maximum number allowed per player
- `icon` (string): Visual representation (emoji)
- `hasBorder` (bool): Whether building has visible border

**Methods**:
- `Building(int x, int y, int sizeX, int sizeY, int costGold, int costElixir, int health, int maxInstances, const string& icon, bool hasBorder = true)`: Constructor
- `const Position& getPosition() const`: Returns position
- `const string& getIcon() const`: Returns visual representation
- `int getCostGold() const`: Returns gold cost
- `int getCostElixir() const`: Returns elixir cost
- `int getHealth() const`: Returns current health
- `int getMaxInstances() const`: Returns maximum allowed instances
- `int getSizeX() const`: Returns width
- `int getSizeY() const`: Returns height
- `bool Border() const`: Returns whether building has a border
- `void setPosition(int x, int y)`: Updates position
- `void takeDamage(int damage)`: Reduces health by specified amount

### Town Hall

The `TownHall` class represents the central building that must be protected.

**Inheritance**: Inherits from `Building`

**Attributes**:
- All attributes from `Building`

**Methods**:
- `TownHall(int x, int y)`: Constructor that sets up a 9x5 building with castle emoji "🏰" and 500 health

### Wall

The `Wall` class represents defensive barriers that block enemy movement.

**Inheritance**: Inherits from `Building`

**Attributes**:
- All attributes from `Building`

**Methods**:
- `Wall(int x, int y)`: Constructor that sets up a 1x1 building with stone emoji "🧱", 50 health, and costs 10 gold

### Resource Generators

Resource generators are buildings that produce resources over time.

#### ResourceGenerator Base Class

**Inheritance**: Inherits from `Building`

**Attributes**:
- All attributes from `Building`
- `currentAmount` (int): Current amount of resources generated
- `capacity` (const int): Maximum amount before collection required

**Methods**:
- `ResourceGenerator(int x, int y, int sizeX, int sizeY, int costGold, int costElixir, int health, int maxInstances, const string& icon, int capacity)`: Constructor
- `virtual void update() = 0`: Pure virtual method to update resource amount
- `virtual int collect() = 0`: Pure virtual method to collect resources

#### Gold Mine

The `GoldMine` class generates gold resources over time.

**Inheritance**: Inherits from `ResourceGenerator`

**Attributes**:
- All attributes from `ResourceGenerator`

**Methods**:
- `GoldMine(int x, int y)`: Constructor initializing a 7x3 building with gold icon "💰", costs 0 gold and 50 elixir
- `void update() override`: Increases current gold amount by 5 each update
- `int collect() override`: Returns and resets accumulated gold if capacity reached

#### Elixir Collector

The `ElixirCollector` class generates elixir resources over time.

**Inheritance**: Inherits from `ResourceGenerator`

**Attributes**:
- All attributes from `ResourceGenerator`

**Methods**:
- `ElixirCollector(int x, int y)`: Constructor initializing a 7x3 building with water drop icon "💧", costs 100 gold
- `ElixirCollector& operator=(const ElixirCollector& other)`: Assignment operator
- `void update() override`: Increases current elixir amount by 5 each update
- `int collect() override`: Returns and resets accumulated elixir if capacity reached

---

## Game Board

The `Board` class manages the game state, handles rendering, and coordinates all game elements.

**Attributes**:
- `width`, `height` (const int): Dimensions of game board
- `margin` (const int): Left margin for UI elements
- `player` (Player): Player-controlled character
- `townhall` (TownHall): Central building to protect
- `walls` (vector<Wall>): Collection of wall structures
- `goldMines` (vector<GoldMine>): Collection of gold mines
- `elixirCollectors` (vector<ElixirCollector>): Collection of elixir collectors
- `enemies` (vector<unique_ptr<Enemy>>): Collection of enemy units
- `leftTexts` (vector<string>): Text for UI sidebar
- `spawnCounter` (int): Counter for enemy spawn timing
- `spawnRate` (const int): How frequently enemies spawn
- `gameOver` (bool): Flag indicating game over state
- `raiderCount`, `bombermanCount` (int): Counters for enemy statistics

**Private Methods**:
- `void drawBuilding(const Building& building) const`: Renders a building
- `bool areBuildingsColliding(const Building& b1, const Building& b2) const`: Collision detection
- `bool isPositionOccupied(const Position& pos, const Building* ignore = nullptr) const`: Position check
- `bool CanBuild(const Building* building, const Building* ignore = nullptr) const`: Building placement validation
- `void spawnEnemy()`: Creates new enemies at map edges
- `void updateEnemies()`: Updates all enemy positions and attacks
- `void renderTopBorder() const`: Draws top game border
- `void renderBottomBorder() const`: Draws bottom game border
- `void renderMiddle() const`: Draws middle section with stats

**Public Methods**:
- `Board()`: Constructor initializing game state
- `bool tryMovePlayer(char direction)`: Attempts to move player
- `bool placeWall()`: Attempts to place wall at player's position
- `bool placeGoldMine()`: Attempts to place gold mine at player's position
- `bool placeElixirCollector()`: Attempts to place elixir collector at player's position
- `void collectResources()`: Collects resources from buildings player stands on
- `void updateResources()`: Updates resource generation in all buildings
- `void update()`: Main game state update function
- `void render()`: Renders entire game state to console

---

## Input Handling

The `InputManager` class handles keyboard input for the game.

**Methods**:
- `InputManager()`: Constructor setting up terminal for input
- `~InputManager()`: Destructor restoring terminal settings
- `char getInput()`: Returns character representing player input (U/D/L/R for movement, etc.)

---

## Game Loop

The main game loop in `main.cpp` orchestrates the game flow:

1. Initialize the game board and input manager
2. Enter main loop:
   - Render the current game state
   - Get player input
   - Handle input (move player, place buildings, collect resources)
   - Update game state (spawn enemies, update resources)
   - Repeat until player quits or game over

The game uses a turn-based system where enemies only move or attack when the player takes a movement action.

---

## Class Relationships Diagram
