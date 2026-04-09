cmake_minimum_required(VERSION 3.8)
project(mini_model_description)

find_package(ament_cmake REQUIRED)

# Install the directories so ROS can find them
install(DIRECTORY
  meshes
  urdf
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)
ament_package()