#include "maze.hpp"

void Maze::init() {
    // Top row (North walls)
    for (uint8_t x = 0; x < mazeWidth; ++x)
        setWall(x, 0, Direction::North, outerWallValue);

    // Bottom row (South walls)
    for (uint8_t x = 0; x < mazeWidth; ++x)
        setWall(x, mazeWidth - 1, Direction::South, outerWallValue);

    // Left column (West walls)
    for (uint8_t y = 0; y < mazeWidth; ++y)
        setWall(0, y, Direction::West, outerWallValue);

    // Right column (East walls)
    for (uint8_t y = 0; y < mazeWidth; ++y)
        setWall(mazeWidth - 1, y, Direction::East, outerWallValue);
}

uint8_t Maze::accessCell(const uint8_t x, const uint8_t y) {
    return maze[y * mazeWidth + x];
}

void Maze::setWall(const uint8_t x, const uint8_t y, const Direction dir, uint8_t value) {
    // Ensure value is only 2 bits
    value &= 0b11;
    // Calculate the bit position for the direction
    const uint8_t shift = (3 - static_cast<uint8_t>(dir)) * 2;
    // Clear the 2 bits for the direction
    maze[y * mazeWidth + x] &= ~(0b11 << shift);
    // Set the new value
    maze[y * mazeWidth + x] |= (value << shift);
}

bool Maze::hasWall(const uint8_t x, const uint8_t y, const Direction dir) {
    const uint8_t cell = accessCell(x, y);
    // Calculate the bit position for the direction
    const uint8_t shift = (3 - static_cast<uint8_t>(dir)) * 2;
    // Extract the 2 bits for the direction
    const uint8_t wallValue = (cell >> shift) & 0b11;
    // Return true if it's a wall or an outer wall
    return wallValue == hasWallValue || wallValue == outerWallValue;
}