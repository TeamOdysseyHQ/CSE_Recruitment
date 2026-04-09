#!/bin/bash

source /opt/ros/humble/setup.bash
source ~/ros2_ws/install/setup.bash

echo "ROS Service Started"

# Keep service alive
while true; do
    ros2 topic list
    sleep 5
done
