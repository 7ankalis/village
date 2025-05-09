cmake_minimum_required(VERSION 3.10)
project(VillageGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include header files
include_directories(include)

# Get all source files from src/
file(GLOB SOURCES src/*.cpp)

# Explicitly list the new source files to ensure they're included
set(SOURCES ${SOURCES}
    src/Raider.cpp
    src/Bomberman.cpp
)

# Create the executable
add_executable(game ${SOURCES})

