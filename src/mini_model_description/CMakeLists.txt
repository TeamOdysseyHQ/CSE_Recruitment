cmake_minimum_required(VERSION 3.8)
project(mini_model_description)

find_package(ament_cmake REQUIRED)

# Install urdf and meshes so $(find mini_model_description) works
install(
  DIRECTORY urdf meshes
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
