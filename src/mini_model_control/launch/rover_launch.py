import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import ExecuteProcess, TimerAction, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command


def generate_launch_description():

    pkg_description = get_package_share_directory('mini_model_description')
    pkg_control     = get_package_share_directory('mini_model_control')
    pkg_gazebo_ros  = get_package_share_directory('gazebo_ros')

    xacro_file  = os.path.join(pkg_description, 'urdf', 'rover.xacro')
    controllers = os.path.join(pkg_control,     'config', 'rover_controllers.yaml')

    robot_description = ParameterValue(
        Command(['xacro ', xacro_file]),
        value_type=str
    )

    # ── 1. Gazebo Classic ──────────────────────────────────────────────────────
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gazebo.launch.py')
        )
    )

    # ── 2. Robot State Publisher ───────────────────────────────────────────────
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'use_sim_time': True,
        }]
    )

    # ── 3. Spawn into Gazebo ───────────────────────────────────────────────────
    # -z 0.0 because the base_joint origin already lifts the model 0.15 m above
    # base_footprint, and the rpy fix rotates it upright.
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        name='spawn_entity',
        output='screen',
        arguments=[
            '-topic', 'robot_description',
            '-entity', 'rover',
            '-z', '0.0',
        ]
    )

    # ── 4. Spawn controllers after Gazebo + model are ready ───────────────────
    # 'spawner' (ros2_controllers >= 0.4) replaces the deprecated load_controller CLI.
    load_joint_state_broadcaster = TimerAction(
        period=8.0,
        actions=[
            Node(
                package='controller_manager',
                executable='spawner',
                arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
                output='screen',
            )
        ]
    )

    load_drive_controller = TimerAction(
        period=10.0,
        actions=[
            Node(
                package='controller_manager',
                executable='spawner',
                arguments=['rover_drive_controller', '--controller-manager', '/controller_manager'],
                output='screen',
            )
        ]
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        spawn_robot,
        load_joint_state_broadcaster,
        load_drive_controller,
    ])