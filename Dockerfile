# Use the official ROS 2 Jazzy image
FROM ros:jazzy

SHELL ["/bin/bash", "-c"]
ENV DEBIAN_FRONTEND=noninteractive

# --- Add arguments for user/group IDs ---
# These values will be passed in from docker-compose
ARG USER_ID=1000
ARG GROUP_ID=1000

# Install dependencies (ROS, Gazebo, build tools)
RUN apt-get update && apt-get install -y \
    sudo \
    build-essential \
    cmake \
    git \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-jazzy-xacro \
    ros-jazzy-teleop-twist-keyboard \
    ros-jazzy-ros-gz-sim \
    ros-jazzy-ros-gz-bridge \
    vim \
    && rm -rf /var/lib/apt/lists/*

# --- Create 'ros' user with specific UID/GID ---
# This ensures file permissions inside the container match your host machine
RUN \
    export CONFLICT_G_NAME=$(getent group $GROUP_ID | cut -d: -f1) && \
    export CONFLICT_U_NAME=$(getent passwd $USER_ID | cut -d: -f1) && \
    \
    # 1. Fix the Group
    if [ ! -z "$CONFLICT_G_NAME" ]; then \
    # If group exists but has wrong name, rename it
    if [ "$CONFLICT_G_NAME" != "ros" ]; then \
    groupmod -n ros $CONFLICT_G_NAME; \
    fi; \
    else \
    # If no group exists, create it
    groupadd -g $GROUP_ID ros; \
    fi && \
    \
    # 2. Fix the User
    if [ ! -z "$CONFLICT_U_NAME" ]; then \
    # If user exists but has wrong name, rename it and set its primary group
    if [ "$CONFLICT_U_NAME" != "ros" ]; then \
    usermod -l ros -g $GROUP_ID $CONFLICT_U_NAME; \
    fi; \
    else \
    # If no user exists, create it
    useradd -m -s /bin/bash -u $USER_ID -g $GROUP_ID ros; \
    fi && \
    \
    # 3. Ensure user is in sudo group and has correct home dir
    usermod -aG sudo ros && \
    usermod -d /home/ros -m ros && \
    echo '%sudo ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers.d/sudo-nopasswd

# Initialize rosdep (root)
RUN if [ ! -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then \
    rosdep init; \
    fi && \
    rosdep update

# Switch to 'ros' user
USER ros
# Set the working directory to match the compose file
WORKDIR /home/ros/ros2_ws

# Create the 'src' directory
# Your local ./src folder will be mounted here
RUN mkdir src

# 1. Copy the DDS config file into the image
COPY cyclonedds.xml /home/ros/cyclonedds.xml

# 2. Set the DDS environment variable
ENV CYCLONEDDS_URI=file:///home/ros/cyclonedds.xml

# 3. Set the Gazebo plugin paths to point to our merged install directory
ENV GZ_SIM_SYSTEM_PLUGIN_PATH=$GZ_SIM_SYSTEM_PLUGIN_PATH:/home/ros/ros2_ws/install/lib
ENV GZ_GUI_PLUGIN_PATH=$GZ_GUI_PLUGIN_PATH:/home/ros/ros2_ws/install/lib

# --- Sourcing Setup ---
# Automatically source ROS 2 and our workspace (if it exists)
RUN echo "source /opt/ros/jazzy/setup.bash" >> /home/ros/.bashrc && \
    echo "if [ -f /home/ros/ros2_ws/install/setup.bash ]; then source /home/ros/ros2_ws/install/setup.bash; fi" >> /home/ros/.bashrc

# Default command to start a shell
CMD ["/bin/bash"]
