# Micromouse Flood Fill Maze Solver (C++)

This project implements a Micromouse maze-solving algorithm in C++ using the flood fill method.  
It is designed to work with the [mackorone/mms](https://github.com/mackorone/mms) Micromouse simulator.

## Features

- Flood fill algorithm for optimal maze solving
- Dynamic wall detection and mapping using the simulator API
- Traversal from start to goal and back, with a speed run on the discovered shortest path

## Installing a Compiler and CMake

You need a C++20-compatible compiler and CMake (version 3.20 or newer).

### Windows

- **Compiler:**  
  Install [MinGW-w64](https://www.mingw-w64.org/) or [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) (with Desktop development with C++ workload).
- **CMake:**  
  Download and install from [cmake.org/download](https://cmake.org/download/).

Add both the compiler and CMake to your system PATH.

### macOS

- **Compiler:**  
  Install Xcode Command Line Tools by running:  
  ```
  xcode-select --install
  ```
- **CMake:**  
  Install via [Homebrew](https://brew.sh/):  
  ```
  brew install cmake
  ```

### Linux

- **Compiler and CMake:**  
  Install via your package manager. For example, on Ubuntu:  
  ```
  sudo apt update
  sudo apt install gcc
  sudo apt install build-essential cmake
  ```

## Building (with CMake)

1. Open a terminal in the `Code` directory.
2. Run:
    ```
    cmake -S . -B cmake-build-debug
    cmake --build cmake-build-debug
    ```
   The executable will be placed in `Code/bin/` (e.g., `bin/micromouse_sim` or `bin/micromouse_sim.exe`).

## Running in the Simulator

1. [Download the Micromouse simulator](https://github.com/mackorone/mms#download).
2. Run the simulator and click the "+" button to configure a new algorithm.
3. Enter the configuration for your algorithm:
    - **Name:** (Any name you like)
    - **Directory:** Path to this repository's `Code` folder
    - **Build command:**  
      ```
      cmake --build cmake-build-debug
      ```
    - **Run command:**  
      ```
      the path for directory + ./bin/micromouse_sim
      ```
      (on Windows, use `the path for directory + bin\micromouse_sim.exe`)
4. Click the "Run" button in the simulator.

## How it Works

- The robot starts at (0, 0) facing North.
- It explores the maze using the flood fill algorithm, updating its map with wall information from the simulator API.
- After reaching the goal (center), it returns to the start, further refining its map.
- Finally, it performs a speed run using the shortest known path.

## Notes

- Requires a C++20-compatible compiler (e.g., g++, clang++, or MSVC).
- Communication with the simulator is via stdin/stdout; use stderr for debug output.
- The API interface is defined in `API.hpp` and implemented in `API.cpp`.
- Maze and flood fill logic are in `maze.*` and `flood_fill.*` respectively.
- For more information on the simulator API, see [mackorone/mms#mouse-api](https://github.com/mackorone/mms#mouse-api).
