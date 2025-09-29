#pragma once

#include "maze.hpp"
#include <vector>

namespace FloodFill {
    // Stores calculated distances from each cell to the goal
    inline uint8_t distanceValues[Maze::mazeWidth * Maze::mazeWidth] = {0};

    // Initializes the distance values
    void init();

    // Returns the distance value at (x, y)
    uint8_t getDistance(uint8_t x, uint8_t y);

    // Update incorrect distances from a starting cell until correct
    void updateDistances(uint8_t startX, uint8_t startY);

    // Returns the direction of the neighbor with the lowest distance value
    Direction getLowestNeighbor(uint8_t x, uint8_t y);

    // Explore an untraveled (if possible) path to the goal
    void exploreToGoal();

    // Move the robot from goal to start
    void returnToStart();

    // Full maze traversal from start to goal and back to start using fastest found path
    void traverseMaze();

    // Display the distance values and walls in the simulator
    void printDataToSim();

    // Helper function to rotate robot to face a desired direction
    void rotateTo(Direction current, Direction target);

    // Helper to update Maze wall info from API sensors at (x, y) and heading
    void updateWallsFromAPI(uint8_t x, uint8_t y, Direction heading);

}