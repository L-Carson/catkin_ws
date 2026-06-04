cmake_minimum_required(VERSION 3.10)
project(lidar_det_interface)

find_package(catkin REQUIRED)

# Header-only package: just export the include directory so both the ROS node
# and (out-of-tree) backends can pick up lidar_detector_api.hpp.
catkin_package(
  INCLUDE_DIRS include
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.hpp"
)
