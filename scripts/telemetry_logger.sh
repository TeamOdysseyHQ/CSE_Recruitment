#!/bin/bash

source /opt/ros/humble/setup.bash

until ros2 topic list | grep -q "/chatter"
do
  sleep 2
done

ros2 topic echo /chatter >> ~/log.txt
