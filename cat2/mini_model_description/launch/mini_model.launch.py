from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.actions import ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
import os

from ament_index_python.packages import get_package_share_directory
import xacro


def generate_launch_description():

    # Get package path
    pkg_name = 'mini_model_description'
    pkg_path = get_package_share_directory(pkg_name)

    # Path to your xacro file
    xacro_file = os.path.join(pkg_path, 'urdf', 'mini_model.xacro')

    # Process xacro → robot_description
    robot_description_config = xacro.process_file(xacro_file)
    robot_description = {'robot_description': robot_description_config.toxml()}

    gazebo = ExecuteProcess(
    cmd=[
        'gazebo',
        '--verbose',
        '-s', 'libgazebo_ros_factory.so'
    ],
    output='screen'
)

    return LaunchDescription([

        gazebo,

        # Publish robot state (TF, joints)
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[robot_description],
            output='screen'
        ),

        # Spawn robot into Gazebo
        Node(
             package='gazebo_ros',
             executable='spawn_entity.py',
             arguments=[
                '-topic', 'robot_description',
                '-entity', 'mini_model',
                '-x', '0',
                '-y', '0',
                '-z', '0.2'
             ],
             output='screen'
        ),

        

        Node(
            package='controller_manager',
            executable='spawner',
            arguments=['joint_state_broadcaster'],
        ),

        Node(
            package='controller_manager',
            executable='spawner',
            arguments=['diff_cont'],
        ),
    ])