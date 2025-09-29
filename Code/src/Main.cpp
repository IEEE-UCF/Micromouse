#include "API.hpp"
#include "maze.hpp"
#include "flood_fill.hpp"

int main(int argc, char* argv[]) {
    API::log("Running...");
    Maze::init();
    FloodFill::init();

    // Explore to goal, updating map
    FloodFill::exploreToGoal();

    // Explore back to start, updating map
    FloodFill::returnToStart();

    // Optionally, do a speed run using the fastest known path
    FloodFill::traverseMaze();

    return 0;
}