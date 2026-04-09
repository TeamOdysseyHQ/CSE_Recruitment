ros2_bridge.service

[Unit]
Description=ROS2 Bridge Service
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
ExecStart=/usr/bin/python3 /usr/local/bin/ros2_bridge.py
Restart=on-failure
RestartSec=5
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=rover_ground_control.target



telemetry_logger.service

[Unit]
Description=Telemetry Logging Service
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
ExecStart=/usr/bin/python3 /usr/local/bin/telemetry_logger.py
Restart=on-failure
RestartSec=5
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=rover_ground_control.target

rover_ground_control.target

[Unit]
Description=Rover Ground Control System
After=network-online.target
Wants=network-online.target ros2_bridge.service telemetry_logger.service

[Install]
WantedBy=multi-user.target




To enable and start the services:-
# Reload systemd to pick up new files
sudo systemctl daemon-reload

# Enable all three (auto-start on boot)
sudo systemctl enable ros2_bridge.service
sudo systemctl enable telemetry_logger.service
sudo systemctl enable rover_ground_control.target

# Start them now
sudo systemctl start rover_ground_control.target

to verify if it is running

# Check status of both
sudo systemctl status ros2_bridge.service
sudo systemctl status telemetry_logger.service

# Watch live logs
journalctl -fu ros2_bridge.service
journalctl -fu telemetry_logger.service




breaking the services:-

# Corrupt the script
sudo sh -c 'echo "exit 1" > /usr/local/bin/ros2_bridge.sh'
sudo systemctl restart ros2-bridge.service
# Service will immediately fail

# Remove execute permission
sudo chmod -x /usr/local/bin/telemetry_logger.sh
sudo systemctl restart telemetry-logger.service
# Fails with "Permission denied"

# Break the unit file syntax
sudo sh -c 'echo "INVALID_LINE" >> /etc/systemd/system/ros2-bridge.service'
sudo systemctl daemon-reload
sudo systemctl restart ros2-bridge.service
# systemd rejects the malformed unit



# Kill the process and disable restart capability
sudo systemctl stop ros2-bridge.service
sudo systemctl stop telemetry-logger.service

# Disable both so they won't come back on reboot
sudo systemctl disable ros2-bridge.service
sudo systemctl disable telemetry-logger.service

# Nuke the unit files entirely
sudo rm /etc/systemd/system/ros2-bridge.service
sudo rm /etc/systemd/system/telemetry-logger.service
sudo systemctl daemon-reload

# Now neither service exists or can start
systemctl status ros2-bridge.service  # "could not be found"
