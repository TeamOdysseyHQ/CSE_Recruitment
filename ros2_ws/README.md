# ROS2 Rover Simulation – Software Task

## Overview
This project implements a basic rover simulation workflow using **ROS2 and Gazebo**.  
The goal of the project is to simulate a rover and implement software components that allow it to process sensor data and publish useful information for robot navigation.

The system includes multiple ROS2 nodes responsible for publishing and subscribing to sensor data such as IMU, GPS, and encoder readings.

---

## Objectives

The objectives of this project were:

- Spawn a rover model in **Gazebo simulation**
- Configure a **differential drive controller**
- Publish **odometry information**
- Implement sensor data publishers
- Create a **micro-ROS subscriber node**
- Establish communication between ROS2 nodes through topics

---

## System Architecture

The project follows a modular ROS2 architecture where different nodes handle different responsibilities.

IMU Node ────► Data Processing / ROS Topics / GPS Node ─────► Encoder Node ──► Velocity / Motion Estimation micro-ROS Subscriber ◄── Sensor Data Topics


Each sensor publishes data to ROS2 topics, which can be consumed by other nodes for navigation, analysis, or control.

---

## Project Structure

robot_micro_ros_project
│
├── ros2_ws
│ ├── src
│ │ ├── micro_ros_subscriber
│ │ ├── imu_node
│ │ ├── gps_node
│ │ └── encoder_node
│ │
│ ├── build
│ ├── install
│ └── log


The **src** folder contains all the ROS2 packages developed for this project.

---

## Implemented Components

### IMU Node
Publishes simulated IMU data representing orientation and acceleration.

### GPS Node
Publishes simulated GPS data representing rover position.

### Encoder Node
Simulates wheel encoder data used to estimate velocity and movement.

### micro-ROS Subscriber
Subscribes to ROS topics and processes incoming sensor data.

---

## How to Run the Project

Build the workspace:

```bash
cd ros2_ws
colcon build

Source the workspace:

source install/setup.bash

Run the nodes:

ros2 run micro_ros_subscriber subscriber_node

## Testing

You can verify that nodes are publishing correctly using:

ros2 topic list

To inspect messages:

ros2 topic echo <topic_name>


## Tools and Technologies

ROS2
Gazebo
Python
micro-ROS
Linux (WSL)