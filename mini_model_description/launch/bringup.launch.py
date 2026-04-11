from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_path = get_package_share_directory('mini_model_description')

    return LaunchDescription([
        # Launch Gazebo + spawn robot
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(pkg_path, 'launch', 'gazebo.launch.py')
            )
        ),

        TimerAction(period=5.0, actions=[   # was 3.0
	    Node(
		package='controller_manager',
		executable='spawner',
		arguments=['joint_state_broadcaster'],
		output='screen'
	    ),
	]),
	TimerAction(period=8.0, actions=[   # was 5.0
	    Node(
		package='controller_manager',
		executable='spawner',
		arguments=['diff_drive_controller'],
		output='screen'
	    ),
	]),
    ])
