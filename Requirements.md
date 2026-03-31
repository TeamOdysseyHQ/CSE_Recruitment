# CSE Rover Recruitment — Software  Requirements Document

---

## Overview
This document outlines all the software requirements for the **3 recruitment task categories** (ROS2 & Simulation, Embedded Systems, Automation & Dashboard).

---

## Category 1: ROS2 & Simulation

### Operating System
- **Linux (Ubuntu 22.04 LTS)** — Recommended for native ROS2 support
- *Alternative:* Windows with WSL2 + Ubuntu image (for those on Windows)

### Required Software

#### ROS2 Installation
- **ROS2 Humble** (recommended for 2024+) or **ROS2 Foxy**
  - Installation guide: https://docs.ros.org/en/humble/Installation.html
  - Includes: `ros2`, `colcon`, `rosdep`

#### Simulation Engines (Choose One and make sure to look into both of them Determine according to You which would be best for our usecase of a Rover)
- **Gazebo** (Gazebo Fortress)
  - https://gazebosim.org/docs/fortress/install/
  - Integrates natively with ROS2
  
- **CoppeliaSim** (Edu or Pro version)
  - https://www.coppeliarobotics.com/downloads
  - ROS2 plugin available: https://github.com/CoppeliaRobotics/simExtROS2

#### Visualization
- **RViz2** (included with ROS2)
  - Used for viewing robot models, transforms, sensor data

#### Build & Dependency Tools
- **colcon** (ROS2 build tool) — included with ROS2
- **rosdep** (dependency resolver) — included with ROS2
- **pip** (Python package manager)

### Key ROS2 Packages
- `ros2_control` — Control framework
- `robot_state_publisher` — Publish robot transforms
- `joint_state_publisher_gui` — GUI for joint state publishing
- `rqt` — ROS2 GUI tools suite
- `teleop_twist_keyboard` — Keyboard teleop (optional; u can create custom one )

## Category 2: Embedded Systems Programming & MicroROS


### Software & Tools

#### IDE or Extension tht can used for this :

**PlatformIO** (Recommended )
  - VS Code extension
  - Better dependency management

- **Arduino IDE 2.0+** 
  - Download: https://www.arduino.cc/en/software
  - Need to setup for teensy Support 


#### Teensy Support
- **Teensyduino** — Arduino IDE plugin for Teensy
  - https://www.pjrc.com/teensy/td_download.html

#### Required Libraries

**For GPS:**
- `TinyGPS++` Library tht can be used u can also use any other suitable library for gps
  - GitHub: https://github.com/mikalhart/TinyGPSPlus

**For IMU:**
- `Adafruit_BNO055` (For the BNO055 IMU)
- Resources : https://github.com/arduino-libraries/BNO055

**For Encoder :**
- The Encoder tht are being used are OE775 Encoders
- Resources :- https://robu.in/wp-content/uploads/2019/03/302048.pdf

#### MicroROS Stack
- **micro_ros_arduino** — MicroROS for Arduino/Teensy
  - Installation: https://github.com/micro-ROS/micro_ros_arduino
  - Resources : https://micro.ros.org/docs/tutorials/core/overview/
                https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code

- **ROS2 Agent** (on host machine)
  - Runs on Linux/Windows to bridge Teensy to ROS2 network



