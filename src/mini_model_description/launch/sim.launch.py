from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    pkg_share = FindPackageShare('mini_model_description')
    xacro_file = PathJoinSubstitution([pkg_share, 'urdf', 'mini_model.xacro'])

    robot_description = {
        'robot_description': ParameterValue(
            Command(['xacro ', xacro_file]),
            value_type=str,
        )
    }

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare('ros_gz_sim'),
                'launch',
                'gz_sim.launch.py',
            ])
        ),
        launch_arguments={'gz_args': '-r empty.sdf'}.items(),
    )

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[robot_description],
    )

    spawn_robot = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=[
            '-string', Command(['xacro ', xacro_file]),
            '-name', 'mini_model',
            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.45',
            '-R', '1.5708',
            '-P', '0.0',
            '-Y', '0.0',
        ],
    )

    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        output='screen',
        arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
    )

    diff_drive_spawner = Node(
        package='controller_manager',
        executable='spawner',
        output='screen',
        arguments=['diff_drive_controller', '--controller-manager', '/controller_manager'],
    )

    delayed_controllers = TimerAction(
        period=5.0,
        actions=[joint_state_broadcaster_spawner, diff_drive_spawner],
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        spawn_robot,
        delayed_controllers,
    ])
