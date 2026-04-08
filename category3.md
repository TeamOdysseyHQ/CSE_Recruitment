## Category 3 

## 3.1
The objective was to make two programs (ROS2 Bridge service and Telemetry_logger service) that automatically starts when system boots.

condition 1(start only after network)
code -
After=network-online.target
Wants=network-online.target

condition 2 (Dependency ordering)
code-
After=ros2_bridge.service
Requires=ros2_bridge.service

condition 3 (Autostart on reboot)
code -
sudo systemctl enable ros2_bridge
sudo systemctl enable telemetry_logger

In addition to this "restart=always" is used to automatically restart system if it crashes


A master service (rover_system.service)ensures both ROS2 and telemetry_logger start together .

## 3.2
Breaking
1.Break ROS2 bridge service - the line that defines ROS environment is commented in order to get the error
result - command was not found ultimately programs fails at startup
2.break telemetry logger
changed topic dependency from /chatter to /fake_topic
result - service shows active , script execution appeared stuck
3.Service configuration error 
the ExecStart path was intentionally changed 
result- system fails immediately after startup
4.System crash
added exit 1 in both .sh files 
result - both systems crash and enter failed state 

reference - https://chatgpt.com/share/69d6883b-7f1c-8320-be8f-d89f69964c9f
