# Use the official ROS 2 Jazzy image
FROM ros:jazzy

SHELL ["/bin/bash", "-c"]

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies, including the crucial ROS-Gazebo bridge packages
RUN apt-get update && apt-get install -y \
    sudo \
    build-essential \
    cmake \
    git \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-jazzy-teleop-twist-keyboard \
    ros-jazzy-ros-gz-sim \
    ros-jazzy-ros-gz-bridge \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Create 'ros' user, add to sudo group, and allow passwordless sudo
RUN useradd -m -s /bin/bash ros && \
    usermod -aG sudo ros && \
    echo '%sudo ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers.d/sudo-nopasswd

# Initialize rosdep (root)
RUN if [ ! -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then \
        rosdep init; \
    fi && \
    rosdep update

# Switch to 'ros' user
USER ros
WORKDIR /home/ros/ros2_ws

# --- AUTOMATED BUILD PROCESS ---
# This single RUN command performs all the necessary build steps.
RUN \
    # Source the ROS environment to make Gazebo libraries visible
    source /opt/ros/jazzy/setup.bash && \
    # 1. Get the source code
    git clone https://github.com/pedrofza/Ratada.git src/Ratada && \
    \
    # 2. Build and install the Gazebo workspace
    cd src/Ratada/gz_ws && \
    chmod +x buildws.sh installws.sh && \
    ./buildws.sh . && ./installws.sh . && \
    \
    # 3. Build the ROS 2 workspace
    cd /home/ros/ros2_ws/src/Ratada/ros2_ws && \
    colcon build

# Source all environments automatically for interactive terminals
RUN echo "source /opt/ros/jazzy/setup.bash" >> /home/ros/.bashrc && \
    echo "export GZ_GUI_PLUGIN_PATH=/home/ros/ros2_ws/src/Ratada/gz_ws/install/gui" >> /home/ros/.bashrc && \
    echo "export GZ_SIM_SYSTEM_PLUGIN_PATH=/home/ros/ros2_ws/src/Ratada/gz_ws/install/system" >> /home/ros/.bashrc && \
    echo "source /home/ros/ros2_ws/src/Ratada/ros2_ws/install/setup.bash" >> /home/ros/.bashrc

CMD ["/bin/bash"]

# docker build -t ros-ratada-env .
#
#docker run -it --rm `
#    --name ratada_sim_container `
#    --privileged `
#    --net=host `
#    -e DISPLAY=host.docker.internal:0.0 `
#    -v /tmp/.X11-unix:/tmp/.X11-unix:rw `
#    ros-ratada-env `
#    /bin/bash