#!/bin/bash

# Configuration and Breaking for Category 3 Task 3.2

LOG_FILE="/media/aryan-k-k/2TB_KK/CSE_Recruitment/Crash_Logs.txt"
echo "Executing Sabotage Mode..." > $LOG_FILE

# SABOTAGE 1: Circular Dependency
echo "--> 1. Injecting Circular Lock..."
sudo sed -i '/Wants=network-online.target/a Requires=telemetry_logger.service' /etc/systemd/system/ros2_bridge.service
sudo systemctl daemon-reload
sudo systemctl restart telemetry_logger.service >> $LOG_FILE 2>&1 || true

# SABOTAGE 2: Bad Binary
echo "--> 2. Hijacking Executable Path..."
sudo sed -i '/Requires=telemetry_logger.service/d' /etc/systemd/system/ros2_bridge.service
sudo sed -i 's/\/bin\/bash/\/bin\/bash_invalid/g' /etc/systemd/system/ros2_bridge.service
sudo systemctl daemon-reload
sudo systemctl restart ros2_bridge.service >> $LOG_FILE 2>&1 || true

# SABOTAGE 3: Permissions
echo "--> 3. Overriding Permissions..."
sudo sed -i 's/User=root/User=nobody/g' /etc/systemd/system/telemetry_logger.service
sudo systemctl daemon-reload
sudo systemctl restart telemetry_logger.service >> $LOG_FILE 2>&1 || true

echo "Sabotage Complete. Logs saved to Crash_Logs.txt"
