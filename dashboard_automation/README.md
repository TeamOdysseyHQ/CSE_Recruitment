# Category 3: Automation & Dashboard

This directory contains the files and documentation for completing Category 3 task requirements.

## Task 3.1: Systemd Service Orchestration

To orchestrate the two services native to Linux, we use `systemd`. We fulfill the requirements in the following way:

1. **Network Constraint**: We use `Requires=network-online.target` and `After=network-online.target` to ensure the services only start when the network comes up. 
2. **Dependency Ordering**: `telemetry-logger.service` has `Requires=ros2-bridge.service` and `After=ros2-bridge.service` to ensure the bridge starts *before* the logger, enforcing the correct dependency order.
3. **Autostart Together**: We introduce a systemd target `rover-ground-control.target` that requires both services. By configuring both services with `PartOf=rover-ground-control.target` and enabling the target, we can start, stop, and enable the entire stack simultaneously in one command (`systemctl start rover-ground-control.target`).

**Files Provided (`/systemd` folder)**
- `rover-ground-control.target`
- `ros2-bridge.service`
- `telemetry-logger.service`

**Setup Commands:**
```bash
sudo cp systemd/* /etc/systemd/system/
sudo systemctl daemon-reload
# Enable all components to start together
sudo systemctl enable rover-ground-control.target
sudo systemctl enable ros2-bridge.service telemetry-logger.service
# Start up the automation orchestrator
sudo systemctl start rover-ground-control.target
```

---

## Task 3.2: Breaking the Automation

**Scenario:** We are to intentionally break the automation tool we created (as Tung tung sahur!).

**Method Used (The Sabotage): Systemd Circular Dependency Loop & Permissions Breach**

I took the following steps to cause total failure on the ground control setup:

1. **Introduce a Circular Dependency:**
   I modified the `ros2-bridge.service` to silently add `Requires=telemetry-logger.service`.
   Because `telemetry-logger.service` inherently demands `Requires=ros2-bridge.service`, this introduced a classic Circular Dependency constraint. `systemd` detects the loop `(ros2-bridge.service -> telemetry-logger.service -> ros2-bridge.service)` and will aggressively fail to start either service to prevent an infinite loop, ultimately crashing the automation target entirely upon boot.

2. **Revoke Execute Permissions:**
   As a secondary fail-safe to guarantee a complete crash and hide tracks, I ran:
   ```bash
   sudo chmod -x /opt/rover/bin/ros2_bridge.py
   sudo chmod -x /opt/rover/bin/telemetry_logger.py
   ```
   Now, even if the circular dependency is resolved, the system user running the service gets an `Exec format error / Permission denied` crash logs when attempting to trigger the source scripts.

3. **Break Config Formatting:** 
   In `rover-ground-control.target`, an invalid attribute was added maliciously: 
   `ConditionPathExists=/dev/null/supersecretflag`
   Which will always be false and block the entire target from waking up the dependencies.

**Resulting Failures:**
* **Error in ROS 2 Service:** Fails to start due to permission denied (Code=exited, status=203/EXEC).
* **Error in Telemetry Logging Service:** Fails because its dependency (ROS 2 Bridge) is broken/dead.
* **Error in service configuration / Crash automation:** Systemd refuses to load the Target map, citing infinite dependency loops and failed launch conditions. The automation fails immediately on boot!