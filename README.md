# Micromouse ROS 2 Development Environment

This repository provides a complete Docker-based development and simulation environment for the IEEE UCF Micromouse robot using **ROS 2 Jazzy** and **Gazebo/Ignition**. 

---

## Table of Contents
* [1. Prerequisites](#1-prerequisites)
* [2. Clone the Repository](#2-clone-the-repository)
* [3. Configure and Start VcXsrv](#3-configure-and-start-vcxsrv)
* [4. Using `dev.ps1`](#4-using-devps1)
* [5. Building and Running the Simulation](#5-building-and-running-the-simulation)
* [6. Robot Configuration](#6-robot-configuration)
* [7. Manual Docker Commands](#7-manual-docker-commands)
* [8. Repository Structure](#8-repository-structure)
* [9. Troubleshooting](#9-troubleshooting)
* [10. Recommended System Resources](#10-recommended-system-resources)

---

## 1. Prerequisites

Before starting, install and configure:

### 1.1 WSL 2
Required for Docker Desktop. Install using [Microsoft’s guide](https://learn.microsoft.com/en-us/windows/wsl/install).

### 1.2 Docker Desktop
Download from [Docker](https://www.docker.com/products/docker-desktop/) and enable **WSL 2 backend**.

### 1.3 VcXsrv (X Server)
Required to display Gazebo/Ignition GUI from Linux container to Windows. Download from [SourceForge](https://sourceforge.net/projects/vcxsrv/).

### 1.4 Git
Required to clone the repository. Download from [git-scm.com](https://git-scm.com/install/windows).

---

## 2. Clone the Repository

Open **PowerShell** and run:

```powershell
git clone https://github.com/IEEE-UCF/Micromouse.git
cd Micromouse
git checkout ROS2
````

---

## 3. Configure and Start VcXsrv

Before starting the container:

1. Launch **XLaunch** (VcXsrv wizard).
2. Display settings: *Multiple windows*
3. Client startup: *Start no client*
4. Extra settings: **Disable access control** (critical)
5. Finish setup; VcXsrv icon appears in system tray.

---

## 4. Using `dev.ps1`

`dev.ps1` helps manage the Docker container. Commands:

| Command  | Description                                     |
| -------- | ----------------------------------------------- |
| `-Build` | Build the Docker image                          |
| `-Up`    | Start container in detached mode                |
| `-Shell` | Open bash shell in container (with DISPLAY set) |
| `-Logs`  | Follow container logs                           |
| `-Down`  | Stop and remove container                       |

Example:

```powershell
# Build Docker image
.\dev.ps1 -Build

# Start container
.\dev.ps1 -Up

# Open container shell
.\dev.ps1 -Shell

# Follow logs
.\dev.ps1 -Logs

# Stop container
.\dev.ps1 -Down
```

---

## 5. Building and Running the Simulation


### 5.1 Enter Container Shell

```powershell
.\dev.ps1 -Shell
```

Prompt example:

```
root@ros-dev:/app#
```

All commands below are executed **inside the container shell**.

### 5.2 Build the Workspace and Launch Simulation

#### Option A: Full Build + Run

```bash
bash build_workspace.sh --run
```

#### Option B: Manual Launch

```bash
bash build_workspace.sh
source install/setup.bash
ros2 launch micromouse_description micromouse_launch.py
```

---

## 6. Robot Configuration

XACRO files define the robot:

* `micromouse_robot.urdf.xacro` 
* `micromouse_robot.gazebo.xacro`
* `body.xacro`, `wheel.xacro`, `caster.xacro`

---

## 7. Manual Docker Commands

| Action          | Command                                 | Equivalent |
| --------------- | --------------------------------------- | ---------- |
| Build image     | `docker-compose build`                  | -Build     |
| Start container | `docker-compose up -d`                  | -Up        |
| Open shell      | `docker-compose exec ros-dev /bin/bash` | -Shell     |
| Follow logs     | `docker-compose logs -f ros-dev`        | -Logs      |
| Stop & remove   | `docker-compose down`                   | -Down      |

---

## 8. Repository Structure

```
.
├── .gitignore               # Files and folders for Git to ignore
├── Dockerfile               # Defines the main Docker image with ROS 2, Gazebo, & deps
├── LICENSE                  # Project license
├── Micromouse competition rules.pdf # Rules PDF for the competition
├── README.md                # This documentation file
├── build_workspace.sh       # (Inside container) Script to build the ROS 2 workspace
├── cyclonedds.xml           # Configuration for the ROS 2 middleware
├── dev.ps1                  # (On host) PowerShell helper script for managing Docker
├── docker-compose.yaml      # Defines the `ros-dev` container service
├── mazes/                   # Files for different maze layouts
└── src/                     # All ROS 2 source code and packages
    ├── encoder_gz_sensor/   # Gazebo plugin for a custom encoder sensor
    ├── encoder_gz_system/   # Gazebo plugin for the encoder system
    ├── micromouse_description/ # Contains the robot's URDF/XACRO model files
    ├── micromouse_gz_gui/   # Custom Gazebo GUI plugins
    └── micromouse_gz_system/  # Core Gazebo system plugins (diff drive, IMU, etc.)
```

---

## 9. Troubleshooting

* Gazebo not opening: ensure VcXsrv running, DISPLAY auto-set, access control disabled
* Black/frozen GUI: restart VcXsrv
* Docker fails: ensure WSL2 backend active, run `wsl --shutdown`
* Build failures: `.\dev.ps1 -Down` then `.\dev.ps1 -Build`

---

## 10. Recommended System Resources

* Quad-core CPU or better
* 8 GB RAM minimum (16 GB preferred)
* Hardware acceleration enabled

---
