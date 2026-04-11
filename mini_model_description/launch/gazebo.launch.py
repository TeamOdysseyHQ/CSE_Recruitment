import os
from launch import LaunchDescription
from launch.actions import ExecuteProcess, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    pkg_path = get_package_share_directory('mini_model_description')
    xacro_file = os.path.join(pkg_path, 'urdf', 'mini_model.xacro')
    gazebo_ros_path = get_package_share_directory('gazebo_ros')

    robot_description = ParameterValue(
        Command(['xacro ', xacro_file]),
        value_type=str
    )

    return LaunchDescription([
        # Launch Gazebo Classic
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(gazebo_ros_path, 'launch', 'gazebo.launch.py')
            )
        ),

        # Publish robot description
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description}]
        ),

        # Spawn robot in Gazebo
	Node(
	    package='gazebo_ros',
	    executable='spawn_entity.py',
	    arguments=[
		'-topic', 'robot_description',
		'-entity', 'mini_model',
		'-x', '0', '-y', '0', '-z', '0.5',
		'-R', '1.5708'   # rotate 90° to fix Y-up vs Z-up
	    ],
	    output='screen'
	),
    ])
