import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
import xacro

def generate_launch_description():
    # 1. Setup paths
    pkg_path = get_package_share_directory('mini_model_description')
    xacro_file = os.path.join(pkg_path, 'urdf', 'mini_model.xacro')
    
    # 2. Process Xacro
    robot_description_config = xacro.process_file(xacro_file)
    params = {'robot_description': robot_description_config.toxml(), 'use_sim_time': True}

    # 3. Nodes
    # Publishes the TF tree using robot_description and /joint_states
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[params]
    )

    # Launch Gazebo Sim (Ignition)
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('ros_gz_sim'), 'launch', 'gz_sim.launch.py')]),
        launch_arguments={'gz_args': '-r empty.sdf'}.items(),
    )

    # Spawn the robot into the Gazebo world
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-topic', 'robot_description', '-name', 'mini_rover'],
        output='screen'
    )

    # 4. Controller Spawners
    # joint_state_broadcaster publishes /joint_states to complete the TF tree
    joint_state_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
    )

    # Drives the 6 wheels (Revolute 29, 30, 31, 36, 37, 38)
    velocity_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["velocity_controller"],
    )

    # Controls the 4 steering joints (Revolute 27, 28, 34, 35)
    position_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["position_controller"],
    )

    return LaunchDescription([
        node_robot_state_publisher,
        gazebo,
        spawn_entity,
        # TimerAction ensures Gazebo is fully loaded before controllers try to hook in
        TimerAction(period=.0, actions=[
            joint_state_broadcaster, 
            velocity_controller, 
            position_controller
        ]),
    ])