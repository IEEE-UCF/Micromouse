#include "flood_fill.hpp"
#include <cmath>
#include <queue>
#include "API.hpp"

void FloodFill::init() {
    // Calculate goal region (center 2x2 for even-sized maze)
    constexpr uint8_t mid = Maze::mazeWidth / 2;
    // Set distances based on Manhattan distance to the nearest goal cell
    for (uint8_t y = 0; y < Maze::mazeWidth; ++y) {
        for (uint8_t x = 0; x < Maze::mazeWidth; ++x) {
            // Compute minimum distance to any center cell
            const uint8_t dx = std::min(std::abs(static_cast<int>(x) - static_cast<int>(mid - 1)),
                                        std::abs(static_cast<int>(x) - static_cast<int>(mid)));
            const uint8_t dy = std::min(std::abs(static_cast<int>(y) - static_cast<int>(mid - 1)),
                                        std::abs(static_cast<int>(y) - static_cast<int>(mid)));
            distanceValues[y * Maze::mazeWidth + x] = dx + dy;
        }
    }
}

uint8_t FloodFill::getDistance(const uint8_t x, const uint8_t y) {
    // Return the precomputed distance value for cell (x, y)
    return distanceValues[y * Maze::mazeWidth + x];
}

void FloodFill::updateDistances(uint8_t startX, uint8_t startY) {
    // Use a queue for efficient BFS propagation
    std::queue<std::pair<uint8_t, uint8_t>> q;
    q.emplace(startX, startY);

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        uint8_t minNeighbor = 255;
        // Find minimum distance among accessible neighbors
        for (uint8_t dir = 0; dir < 4; ++dir) {
            const int8_t nx = x + Maze::dx[dir];
            const int8_t ny = y + Maze::dy[dir];
            auto direction = static_cast<Direction>(dir);
            // Only consider valid and accessible neighbors (no wall)
            if (nx >= 0 && nx < Maze::mazeWidth && ny >= 0 && ny < Maze::mazeWidth && !Maze::hasWall(x, y, direction)) {
                if (const uint8_t neighborDist = getDistance(nx, ny); neighborDist < minNeighbor) {
                    minNeighbor = neighborDist;
                }
            }
        }

        // Update distance if needed and enqueue neighbors for further propagation
        if (uint8_t &curDist = distanceValues[y * Maze::mazeWidth + x]; curDist != minNeighbor + 1) {
            curDist = minNeighbor + 1;
            for (uint8_t dir = 0; dir < 4; ++dir) {
                const int8_t nx = x + Maze::dx[dir];
                const int8_t ny = y + Maze::dy[dir];
                if (const auto direction = static_cast<Direction>(dir); nx >= 0 && nx < Maze::mazeWidth && ny >= 0 && ny
                    < Maze::mazeWidth && !Maze::hasWall(x, y, direction)) {
                        q.emplace(static_cast<uint8_t>(nx), static_cast<uint8_t>(ny));
                }
            }
        }
    }
}

Direction FloodFill::getLowestNeighbor(const uint8_t x, const uint8_t y) {
    auto lowestDir = Direction::North;
    uint8_t lowestDist = 255;

    // Check all four directions for the neighbor with the lowest distance value
    for (uint8_t dir = 0; dir < 4; ++dir) {
        const int8_t nx = x + Maze::dx[dir];
        const int8_t ny = y + Maze::dy[dir];
        auto direction = static_cast<Direction>(dir);
        // Only consider valid and accessible neighbors (no wall)
        if (nx >= 0 && nx < Maze::mazeWidth && ny >= 0 && ny < Maze::mazeWidth && !Maze::hasWall(x, y, direction)) {
            if (const uint8_t neighborDist = getDistance(nx, ny); neighborDist < lowestDist) {
                lowestDist = neighborDist;
                lowestDir = direction;
            }
        }
    }
    return lowestDir;
}

void FloodFill::rotateTo(Direction current, Direction target) {
    // Compute difference between current and target direction
    const int diff = static_cast<int>(target) - static_cast<int>(current);
    if (diff == 0) return; // Already facing target direction
    if (diff == 1 || diff == -3) {
        // 90 degrees right
        API::turnRight();
    } else if (diff == -1 || diff == 3) {
        // 90 degrees left
        API::turnLeft();
    } else if (std::abs(diff) == 2) {
        // 180 degrees turn (two rights)
        API::turnRight();
        API::turnRight();
    }
}

void FloodFill::updateWallsFromAPI(uint8_t x, uint8_t y, Direction heading) {
    // Map relative directions (front, left, right) to absolute directions
    Direction dirs[3] = {
        heading, // front
        static_cast<Direction>((static_cast<int>(heading) + 3) % 4), // left
        static_cast<Direction>((static_cast<int>(heading) + 1) % 4)  // right
    };
    // Query wall sensors
    const bool wall[3] = {API::wallFront(), API::wallLeft(), API::wallRight()};
    for (int i = 0; i < 3; ++i) {
        // Set wall info for current cell
        Maze::setWall(x, y, dirs[i], wall[i] ? Maze::hasWallValue : Maze::noWallValue);
        // Also set wall info for neighbor cell in the opposite direction
        const int8_t nx = x + Maze::dx[static_cast<uint8_t>(dirs[i])];
        const int8_t ny = y + Maze::dy[static_cast<uint8_t>(dirs[i])];
        if (nx >= 0 && nx < Maze::mazeWidth && ny >= 0 && ny < Maze::mazeWidth) {
            Maze::setWall(nx, ny,
                static_cast<Direction>((static_cast<int>(dirs[i]) + 2) % 4),
                wall[i] ? Maze::hasWallValue : Maze::noWallValue);
        }
    }
}

void FloodFill::exploreToGoal() {
    uint8_t x = 0, y = 0;
    Direction heading = Direction::North;
    constexpr uint8_t mid = Maze::mazeWidth / 2;
    // Lambda to check if current cell is a goal cell
    auto isGoal = [&](const uint8_t x_lam, const uint8_t y_lam) {
        return (x_lam == mid || x_lam == mid - 1) && (y_lam == mid || y_lam == mid - 1);
    };

    // Traverse until the robot reaches the goal region
    while (!isGoal(x, y)) {
        // Update wall info from sensors
        updateWallsFromAPI(x, y, heading);
        // Update flood fill distances based on new wall info
        updateDistances(x, y);
        // Choose the neighbor with the lowest distance value
        Direction dir = getLowestNeighbor(x, y);
        // Rotate to face the chosen direction
        rotateTo(heading, dir);
        heading = dir;
        // Move forward one cell using the API
        API::moveForward();
        // Update robot's position
        x += Maze::dx[static_cast<uint8_t>(heading)];
        y += Maze::dy[static_cast<uint8_t>(heading)];
    }
}

void FloodFill::returnToStart() {
    constexpr uint8_t mid = Maze::mazeWidth / 2;
    // Find which center cell is closest to start
    uint8_t x = mid - 1, y = mid - 1;
    if (getDistance(mid, mid - 1) < getDistance(x, y)) { x = mid; y = mid - 1; }
    if (getDistance(mid - 1, mid) < getDistance(x, y)) { x = mid - 1; y = mid; }
    if (getDistance(mid, mid) < getDistance(x, y)) { x = mid; y = mid; }
    Direction heading = Direction::North;

    // Traverse back to the start cell (0,0)
    while (!(x == 0 && y == 0)) {
        // Update wall info from sensors
        updateWallsFromAPI(x, y, heading);
        // Update flood fill distances
        updateDistances(x, y);
        // Choose the neighbor with the lowest distance value
        Direction dir = getLowestNeighbor(x, y);
        // Rotate to face the chosen direction
        rotateTo(heading, dir);
        heading = dir;
        // Move forward one cell using the API
        API::moveForward();
        // Update robot's position
        x += Maze::dx[static_cast<uint8_t>(heading)];
        y += Maze::dy[static_cast<uint8_t>(heading)];
    }
}

void FloodFill::traverseMaze() {
    constexpr uint8_t mid = Maze::mazeWidth / 2;
    // Lambda to check if current cell is a goal cell
    auto isGoal = [&](const uint8_t x, const uint8_t y) {
        return (x == mid || x == mid - 1) && (y == mid || y == mid - 1);
    };

    // Start from (0,0) and head to the goal using the fastest known path
    uint8_t x = 0, y = 0;
    Direction heading = Direction::North;
    while (!isGoal(x, y)) {
        // Always follow the lowest neighbor (fastest path)
        Direction dir = getLowestNeighbor(x, y);
        rotateTo(heading, dir);
        heading = dir;
        API::moveForward();
        x += Maze::dx[static_cast<uint8_t>(heading)];
        y += Maze::dy[static_cast<uint8_t>(heading)];
    }

    // Find which center cell is closest to start for the return
    uint8_t gx = mid - 1, gy = mid - 1;
    if (getDistance(mid, mid - 1) < getDistance(gx, gy)) { gx = mid; gy = mid - 1; }
    if (getDistance(mid - 1, mid) < getDistance(gx, gy)) { gx = mid - 1; gy = mid; }
    if (getDistance(mid, mid) < getDistance(gx, gy)) { gx = mid; gy = mid; }

    x = gx; y = gy;
    heading = Direction::North;
    // Return to start using the fastest known path
    while (!(x == 0 && y == 0)) {
        Direction dir = getLowestNeighbor(x, y);
        rotateTo(heading, dir);
        heading = dir;
        API::moveForward();
        x += Maze::dx[static_cast<uint8_t>(heading)];
        y += Maze::dy[static_cast<uint8_t>(heading)];
    }
}

void FloodFill::printDataToSim() {
    // Display the distance values and wall info in the simulator
    for (uint8_t y = 0; y < Maze::mazeWidth; ++y) {
        for (uint8_t x = 0; x < Maze::mazeWidth; ++x) {
            // Show the distance value as text in the simulator
            API::setText(x, y, std::to_string(getDistance(x, y)));
            // Show wall info for each direction
            if (Maze::hasWall(x, y, Direction::North))
                API::setWall(x, y, 'N');
            if (Maze::hasWall(x, y, Direction::East))
                API::setWall(x, y, 'E');
            if (Maze::hasWall(x, y, Direction::South))
                API::setWall(x, y, 'S');
            if (Maze::hasWall(x, y, Direction::West))
                API::setWall(x, y, 'W');
        }
    }
}
