# in linux sudo nano /usr/local/bin/ros2_bridge.py

#!/usr/bin/env python3
import time, logging

logging.basicConfig(
    filename='/var/log/ros2_bridge.log',
    level=logging.INFO,
    format='%(asctime)s %(message)s'
)

logging.info("ROS2 Bridge started")
print("ROS2 Bridge running...", flush=True)

while True:
    logging.info("ROS2 Bridge: heartbeat")
    print("ROS2 Bridge: heartbeat", flush=True)
    time.sleep(5)