#!/bin/bash
set -e # Exit immediately if a command fails

# Navigate to the workspace
cd /home/ros/ros2_ws

echo "--- Installing ROS 2 dependencies ---"
rosdep update
rosdep install --from-paths src -i -y --rosdistro jazzy

echo "--- Building workspace (merged install) ---"
# We use --merge-install to put all files in a single install/lib directory
colcon build --merge-install

echo ""
echo "--- Build complete! ---"

# Check if the --run flag was passed
if [ "$1" == "--run" ]; then
    echo "--- Sourcing and launching simulation ---"
    ros2 run xacro xacro src/micromouse_description/xacro/urdf/micromouse_robot.urdf.xacro > ~/micromouse_robot.urdf
    source install/setup.bash
    ros2 launch micromouse_description micromouse_launch.py
else
    echo ""
    echo "To run your simulation, use:"
    echo "source install/setup.bash"
    echo "ros2 launch micromouse_description micromouse_launch.py"
    echo ""
    echo "Or, re-run this script with the --run flag:"
    echo "bash build_workspace.sh --run"
fi