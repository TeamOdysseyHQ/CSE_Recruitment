#!/usr/bin/env python3
import time, logging, sys

logging.basicConfig(
    filename='/var/log/telemetry.log',
    level=logging.INFO,
    format='%(asctime)s %(message)s'
)

logging.info("Telemetry Logger started")
print("Telemetry Logger running...", flush=True)

while True:
    logging.info("Telemetry: battery=98% signal=strong")
    print("Telemetry: battery=98% signal=strong", flush=True)
    time.sleep(5)

''' in linux sudo nano /usr/local/bin/telemetry_logger.py
     making it executable with :-
     sudo chmod +x /usr/local/bin/ros2_bridge.py
     sudo chmod +x /usr/local/bin/telemetry_logger.py
'''    