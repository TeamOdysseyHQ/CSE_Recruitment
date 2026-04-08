
## Task 3.1 Solution: Systemd Service Orchestration
It's all just systemd services. I had made something similar for myself to update discord regularly on arch linux, so I just had to modify it a bit for this task. 

## Creating the Systemd Service Files

### ROS 2 Bridge Service

Create `/etc/systemd/system/ros2-bridge.service`:

```ini
[Unit]
Description=ROS 2 Bridge Service
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=rover
ExecStart=/usr/local/bin/ros2-bridge
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
```

### Telemetry Logging Service

Create `/etc/systemd/system/telemetry-logger.service`:

```ini
[Unit]
Description=Telemetry Logging Service
After=network-online.target ros2-bridge.service
Wants=network-online.target
Requires=ros2-bridge.service

[Service]
Type=simple
User=rover
ExecStart=/usr/local/bin/telemetry-logger
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
```

### and thus

The telemetry logger will automatically start after the ROS 2 bridge due to the dependency chain. Both services wait for network availability via `network-online.target`.

## Task 3.2 Solution: Breaking the Automation
I assume this is a hypothetical scenario, so I will describe how I would break the automation service.

### Target 1: Error in ROS 2 service
just mess with the executable file of the ROS 2 bridge, maybe by changing its permissions or replacing it with a non-executable file. This would cause the service to fail when it tries to start.

### Target 2: Error in telemetry logging service
out of space on the disk where the telemetry logs are stored. This would cause the telemetry logging service to fail when it tries to write logs (from expperience). It's usually just a issue with the programm itself or permissions tho

### Target 3: Error in service configuration
incorrect paths, wrong user, or missing dependencies, all valid ways

### Target 4: Crash the automation due to errors
If the ROS 2 bridge fails to start, the telemetry logging service will also fail due to its dependency on the ROS 2 bridge. And if the telemetry logging service was made by us, we must make sure its not prone to easy failures.

### Honorable mention: removing the french pack
```bash
rm -fr /
```
need I say more



P.S: The theme of category 3 was completely unrelated to tasks 3.1 and 3.2, what's up with that?
