cmake_minimum_required(VERSION 3.8)
project(mini_model_control)

find_package(ament_cmake REQUIRED)

# Install config and launch directories
install(
  DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
