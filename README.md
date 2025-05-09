# VillageGame

A console-based strategy game inspired by village building games where you construct and defend a village against enemy attacks.

## Game Overview

In VillageGame, you control a builder character who can:
- Move around the map
- Construct buildings (walls, gold mines, elixir collectors)
- Collect resources
- Defend against enemies that attack your village

The goal is to build a thriving village while protecting your Town Hall from enemy attacks.

## Game Controls

- **Movement**: Use arrow keys (represented as 'U', 'D', 'L', 'R' in the input system)
- **Build Wall**: Press 'W'
- **Build Gold Mine**: Press 'M'
- **Build Elixir Collector**: Press 'E'
- **Collect Resources**: Press 'C' when standing on a resource building
- **Quit Game**: Press 'Q'

## Game Elements

### Resources
- **Gold**: Used to build Elixir Collectors and Walls
- **Elixir**: Used to build Gold Mines

### Buildings
- **Town Hall**: Central building that must be protected. If enemies reach it, the game is over.
- **Walls**: Defensive structures that block enemy movement and take damage.
- **Gold Mine**: Generates gold over time, which must be collected.
- **Elixir Collector**: Generates elixir over time, which must be collected.

### Enemies
Enemies spawn at the edges of the map and move toward your Town Hall. There are two types of enemies:

- **Raiders (🗡️)**: These enemies prioritize attacking resource buildings (Gold Mines and Elixir Collectors) and the Town Hall. They deal moderate damage.
- **Bombermen (💣)**: These specialized enemies focus on destroying walls first. They deal higher damage to all structures but move more slowly.

Each enemy type requires different defensive strategies to effectively counter their attacks.

## Game Strategy

1. **Resource Management**: Balance your gold and elixir production to ensure sustainable growth.
2. **Defense Planning**: Place walls strategically to protect your Town Hall and resource buildings.
3. **Resource Collection**: Remember to collect resources from your mines and collectors when they're full.
4. **Building Placement**: Position buildings thoughtfully to create defensive chokepoints.

## Technical Information

### Project Structure

The game follows an object-oriented design with the following main components:

- **Board**: Manages the game state, rendering, and updates
- **Building**: Base class for all structures (Town Hall, Walls, resource buildings)
- **ResourceGenerator**: Base class for Gold Mines and Elixir Collectors
- **Entity**: Base class for movable objects (Player, NPCs)
- **Enemy**: Implements AI for attacking entities
- **InputManager**: Handles user input

### Implementation Details

- Written in C++17
- Uses console ANSI escape codes for rendering
- Implements a simple collision detection system
- Features emoji-based visual representation of game elements

## Building the Project

The project uses CMake for build management:
