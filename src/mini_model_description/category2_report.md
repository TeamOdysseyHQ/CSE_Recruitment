# Category 2 – ROS2 & Simulation

## My Understanding of Task 2

1. The goal is to simulate a rover in Gazebo using ROS2.
2. It involves connecting a robot model (URDF/Xacro) with simulation and control systems.
3. ROS2 is used to handle robot description, communication, and control.
4. Gazebo is used to visualize and simulate the rover.

Basically this task shows how a rover is simulated before implementing it on real hardware.


# Core Tasks

## Task 1 – Spawn Rover Model in Gazebo

What I Did:
 - The given repository was cloned inside the ROS2 workspace src folder.
 - Verified that the mini_model_description package exists and contains the required urdf and mesh files.
 - Built the workspace and sourced the setup file so ROS2 can detect the package.
 - Checked whether the package is recognized by ROS2.
 - Created a launch folder and added a launch file.
 
What I observed:
- Gazebo opened successfully.
- The rover model appeared in the simulation.
- There were warnings related to materials and sensors, but the robot was visible and loaded correctly.

Task 1 was successful


## Task 2 – Differential Drive Controller (Attempted)

1. This task is used to make the rover move using ros2_control.
2. It converts the rover from a static model into a movable robot.

What I tried:
 Added ros2_control configuration into the Xacro file.
- Created a configuration file for the differential drive controller.
- Defined wheel joints, wheel separation, and radius.
- Modified the launch file to include controller-related nodes.

Errors:
1. Plugin not found error 
2. Controller manager not starting 


Link to LLM used : https://chatgpt.com/share/69d7a96f-d3a4-8320-8249-5ac742db1834
