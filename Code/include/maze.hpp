#pragma once

#include <cstdint>

// Directions for walls
enum class Direction {
    North = 0,
    East = 1,
    South = 2,
    West = 3
};

namespace Maze {
    // Maze is square so only need to define one dimension
    // For different sized mazes only this value needs to change
    constexpr uint8_t mazeWidth = 8;

    // Direction vectors for North, East, South, West regardless of robot orientation
    // Used for neighbor access
    // Example: To get the cell to the North of (x, y), use (x + dx[0], y + dy[0])
    constexpr int8_t dx[4] = {0, 1, 0, -1};
    constexpr int8_t dy[4] = {-1, 0, 1, 0};

    //  4 x 2 bit segments for 4 walls
    //  00 = unexplored
    //  01 = no wall
    //  10 = wall
    //  11 = outer wall
    // XXxxxxxx = North
    // xxXxlsx = East
    // xxxxXXxx = South
    // xxxxxxXX = West
    //Example: 0b01100110 = North: no wall, East: wall, South: no wall, West: wall
    inline uint8_t maze[mazeWidth * mazeWidth] = {0};

    constexpr uint8_t unexploredValue = 0b00; // Value representing unexplored
    constexpr uint8_t hasWallValue = 0b10; // Value representing a wall
    constexpr uint8_t noWallValue = 0b01;  // Value representing no wall
    constexpr uint8_t outerWallValue = 0b11; // Value representing an outer wall

    // Initializes the maze with outer walls
    void init();

    // Returns the maze cell at (x, y)
    uint8_t accessCell(uint8_t x, uint8_t y);

    // Sets the wall in the specified direction for the cell at (x, y) to value
    void setWall(uint8_t x, uint8_t y, Direction dir, uint8_t value);
    bool hasWall(uint8_t x, uint8_t y, Direction dir);
}
