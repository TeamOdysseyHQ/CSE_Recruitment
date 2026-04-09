# Category 2: ROS2 & Simulation

This covers the core components for the Category 2 requirements.

## 1. Description
I established a full ROS 2 package `mini_model_description` containing the URDF and launching tools to fulfill the Category 2 tasks.

I successfully:
1. Created `package.xml` and `CMakeLists.txt` for `rclcpp`, `gazebo_ros_pkgs`, `ros_gz_sim`, `xacro`, `ros2controlcli` and `robot_state_publisher`.
2. Configured a complete `sim.launch.py` to seamlessly orchestrate the nodes (Xacro conversion, Gazebo GUI, TF broadcoasters, ros2_control diff-drive spawners, and RViz2 visualization).
3. Set up the difference drive controller `config/controllers.yaml` linking the `diff_drive_controller` directly to the `Revolute 29-38` continuous wheel joints defined in the original URDF.
4. Bound `odom_frame_id` and `base_frame_id` natively into the ROS2 controller configuration out of the box, fulfilling the precise Odometry & TF tree requirement without utilizing extra static TF nodes!

## 2. Launch Instructions

Since I implemented the **Task 2 Bonus Requirement (`Create a single launch file that starts the full simulation stack in one command`)** by default, you can spin up Gazebo, ros2_control, teleop controllers, and RViz2 seamlessly:

```bash
# From your ROS 2 workspace root:
colcon build --packages-select mini_model_description
source install/setup.bash
ros2 launch mini_model_description sim.launch.py
```

## Bonus Task Completions Included:
*   **Bonus Task 2:** Orchestrated `sim.launch.py` file to start Gazebo, the ros2_control spawner entities (Joint State and Diff Drive), and robot_state_publisher.
*   **Bonus Task 3:** Integrated an RViz2 spawn node at the boot of the launcher to verify the generated Tf Tree natively alongside Gazebo!