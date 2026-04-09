from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterValue
import os

def generate_launch_description():

    pkg_path = os.path.join(
        os.getenv('HOME'),
        'ros2_ws/src/mini_model_description'
    )

    urdf_file = os.path.join(pkg_path, 'urdf', 'mini_model.xacro')

    robot_description = ParameterValue(
        Command(['xacro', urdf_file]),   # FIXED
        value_type=str
    )

    return LaunchDescription([

        # Start Gazebo FIRST
        ExecuteProcess(
            cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_factory.so'],
            output='screen'
        ),

        # Robot State Publisher
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description}],
            output='screen'
        ),

        # Spawn robot
        Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=[
                '-entity', 'rover',
                '-topic', 'robot_description'
            ],
            output='screen'
        ),

        # Spawn controllers (AFTER gazebo loads plugin)
        Node(
            package="controller_manager",
            executable="spawner",
            arguments=["joint_state_broadcaster"],
            output="screen",
        ),

        Node(
            package="controller_manager",
            executable="spawner",
            arguments=["diff_drive_controller"],
            output="screen",
        ),
    ])
