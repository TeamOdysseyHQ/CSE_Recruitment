import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node

def generate_launch_description():
    pkg_share = get_package_share_directory('mini_model_description')
    
    # 1. Process the URDF with Xacro
    urdf_file = os.path.join(pkg_share, 'urdf', 'mini_model.xacro')
    robot_description_content = Command(['xacro ', urdf_file])

    # 2. Robot State Publisher Node
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='both',
        parameters=[{'robot_description': robot_description_content, 'use_sim_time': True}]
    )

    # 3. Include Gazebo launch
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('ros_gz_sim'), 'launch', 'gz_sim.launch.py')]),
        launch_arguments={'gz_args': '-r empty.sdf'}.items() # Using default empty world
    )

    # 4. Spawn Robot in Gazebo
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=['-string', robot_description_content,
                   '-name', 'mini_model',
                   '-allow_renaming', 'true']
    )

    # 5. Connect Gazebo and ROS through a bridge
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=['/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock'],
        output='screen'
    )

    # 6. ros2_control controller spawners
    diff_drive_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["diff_drive_controller"],
    )

    joint_broad_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
    )

    #rviz node
    rviz_config_file = getattr(LaunchConfiguration('rvizconfig'), 'perform', None) # Provide optionally
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', os.path.join(pkg_share, 'rviz', 'urdf.rviz')],
        parameters=[{'use_sim_time': True}]
    )
    
    return LaunchDescription([
        gazebo,
        bridge,
        robot_state_publisher_node,
        spawn_entity,
        rviz_node,
        diff_drive_spawner,
        joint_broad_spawner
    ])
