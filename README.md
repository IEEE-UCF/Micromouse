# Micromouse ROS 2 Development Environment

This repository contains the configuration and source files for simulating a micromouse robot using ROS 2 (Jazzy) and Gazebo/Ignition.

---

## Getting Started

This project is managed via a Docker container and uses a PowerShell helper script (`dev.ps1`) for seamless environment control.

### Prerequisites

* **Docker & Docker Compose:** Required for orchestrating and running the containerized development environment.
* **PowerShell:** The helper script (`dev.ps1`) is written in PowerShell.

### Setup and Build

1.  **Build the Docker Image:** This creates the 'micromouse\_sim' image containing ROS 2 Jazzy, Gazebo, and all dependencies.
    ```bash
    .\dev.ps1 -Build
    ```
2.  **Start the Container:** Start the 'ros-dev' container in the background.
    ```bash
    .\dev.ps1 -Up
    ```

---

## Usage

The primary way to interact with the environment is through the `dev.ps1` helper script.

| Command | Description | Example |
| :--- | :--- | :--- |
| **-Build** | Builds the Docker image. | `.\dev.ps1 -Build` |
| **-Up** | Starts the container in detached mode. | `.\dev.ps1 -Up` |
| **-Shell** | Opens an interactive bash session inside the running container. | `.\dev.ps1 -Shell` |
| **-Logs** | Follows the container's logs (use `Ctrl+C` to stop). | `.\dev.ps1 -Logs` |
| **-Down** | Stops and removes the container. | `.\dev.ps1 -Down` |

---

## Workspace Build and Run

The `build_workspace.sh` script handles package installation and compilation within the container.

1.  **Enter the Container Shell:**
    ```bash
    .\dev.ps1 -Shell
    ```

2.  **Build the Workspace:** This installs ROS dependencies (`rosdep install`) and compiles the workspace using `colcon build --merge-install`.
    ```bash
    bash build_workspace.sh
    ```

### Launching the Simulation

To launch the robot in Gazebo/Ignition, use one of the following methods from within the container shell:

| Method | Command | Description |
| :--- | :--- | :--- |
| **Direct Launch** | `bash build_workspace.sh --run` | Executes the build, automatically sources the workspace, generates the URDF using `xacro`, and launches the simulation. |
| **Manual Launch** | `source install/setup.bash` | Used after a successful build to load the ROS environment variables. |
| | `ros2 launch micromouse_description micromouse_launch.py` | Launches the Gazebo world and the robot model. |

---

## ⚙️ Robot Configuration Notes

The robot's structure and behavior are defined across several XACRO files:

* **`micromouse_robot.urdf.xacro` (Main File):** Defines the geometry, materials, links, and joints with the final stable geometry for $1 \text{mm}$ ground clearance.
* **`micromouse_robot.gazebo.xacro`:** Defines friction coefficients, system plugins (Diff Drive, IMU System), and uses **stable Ultrasonic Ray Sensors** instead of crash-prone custom/LiDAR definitions.
* **`body.xacro`, `wheel.xacro`, `caster.xacro`:** Define the link structure and custom inertia macros.
