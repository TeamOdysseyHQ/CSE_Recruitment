# Rover Recruitment Task Submission: Master Showcase

This document provides a comprehensive guide to all implemented tasks, including the source code locations, execution commands, and expected outputs for each category.

---

## 🚦 Recommended Execution Order
To demonstrate the full integrated system, follow this order:

1.  **Step 1: Simulation Base (Category 2)** — Launch Gazebo and RViz first to establish the simulation clock and the robot's physical presence.
2.  **Step 2: Embedded Logic (Category 1)** — Upload the code to your Teensy 4.1. This will begin publishing sensor data to the ROS topics established in Step 1.
3.  **Step 3: Service Deployment (Category 3)** — Run the `install_automation.sh` script to confirm that the background services can successfully bind to the active ROS environment.
4.  **Step 4: The Sabotage Sequence (Category 3)** — Run the `sabotage_infrastructure.sh` script last to crash the services and generate your final `Crash_Logs.txt` report for evaluation.

---

## 🛰️ Category 1: Embedded Systems & MicroROS (Task 3 & Advanced)
**Objective:** Parse GPS, IMU, and Encoder data on Teensy 4.1 and publish them to ROS 2 topics using micro-ROS.

### 📁 Code Locations & Files
- **Primary Logic:** `embedded_workspace/src/main.cpp`
- **Project Config:** `embedded_workspace/platformio.ini`

### 🚀 How to Run
1. Open the `embedded_workspace` folder in VS Code with the **PlatformIO IDE** extension installed.
2. Connect your **Teensy 4.1** via USB.
3. Click the **"PlatformIO: Upload"** button (arrow icon) in the bottom status bar.
4. Open the **Serial Monitor** at **115200 baud** to see formatted sensor prints.

### 📊 Expected Output
- **Serial Monitor:** Continuous prints for GPS (Lat/Lon), IMU (Euler Angles), and Encoder (RPM/Distance).
- **ROS 2 Topics:** Once connected to a micro-ROS agent, you will see:
  - `/gps/fix` (sensor_msgs/NavSatFix)
  - `/imu/data` (sensor_msgs/Imu)
  - `/motor/rpm` (std_msgs/Float32)

---

## 🤖 Category 2: ROS 2 & Simulation (Task 3 & Core)
**Objective:** Spawn the rover in Gazebo Harmonic and visualize the TF tree in RViz2.

### 📁 Code Locations & Files
- **Package Path:** `src/mini_model_description/`
- **Launch Script:** `src/mini_model_description/launch/simulation.launch.py`
- **Controller Config:** `src/mini_model_description/config/rover_controllers.yaml`
- **RViz Config:** `src/mini_model_description/rviz/rover.rviz`
- **Robot Model:** `src/mini_model_description/urdf/mini_model.xacro`

### 🚀 How to Run
Run the following commands in your main terminal:
```bash
# Build the package
colcon build

# Source and launch the simulation
source install/setup.bash
ros2 launch mini_model_description simulation.launch.py
```

### 📊 Expected Output
- **Gazebo Sim:** The rover model should appear correctly in an empty world.
- **RViz2:** A window will open automatically showing the rover model and an unbroken, green-status **TF Tree** (`odom` → `base_link` → `all joints`).
- **Topics:** Use `ros2 topic echo /tf` to see the live transform publishing from simulation time.

---

## 🛠️ Category 3: Automation & Dashboard (Entirely)
**Objective:** Orchestrate background services using `systemd` and document intentional "sabotage" crash outcomes.

### 📁 Code Locations & Files
- **Service Files:** `system_services/ros2_bridge.service` and `system_services/telemetry_logger.service`
- **Install Script:** `install_automation.sh`
- **Sabotage Script:** `sabotage_infrastructure.sh`
- **Prototypes:** `src/telemetry_logger.py` and `src/ros2_bridge_stub.sh`

### 🚀 How to Run
1. **To Install Services:**
```bash
chmod +x install_automation.sh
./install_automation.sh
```
2. **To Execute Sabotage (Task 3.2):**
```bash
chmod +x sabotage_infrastructure.sh
./sabotage_infrastructure.sh
```

### 📊 Expected Output
- **Normal Operation:** `systemctl status telemetry_logger` should show "Active (running)".
- **Sabotage Output:** Running the sabotage script will populate a raw error report in **`Crash_Logs.txt`**.
- **Submission Document:** A detailed analysis of all 4 crash outcomes is available in **`Category_3_Submission.md`**.

---
**Total Tasks Completed:** 100%
**Status:** Ready for Submission
