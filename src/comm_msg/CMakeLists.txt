cmake_minimum_required(VERSION 2.8.3)
project(comm_msg)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(
    catkin REQUIRED
    COMPONENTS
    std_msgs
    geometry_msgs
    message_generation
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )
add_message_files(
    DIRECTORY msg
    FILES body_pose.msg
          box.msg
          boxArray.msg
          carBodyFilter.msg
          coorLabel.msg
          coor2D.msg
          coor2DArray.msg
          coor2DArrayL.msg
          coor2DArrayLA.msg
          coor3D.msg
          coor3DArray.msg
          daq_ctrl.msg
          dcu_info.msg
          dev_info.msg
          detect_result.msg
          detect_target.msg
          dustbinState.msg
          equipment_info.msg
          equipment_pose.msg
          gpsInfo.msg
          ipu.msg
          label.msg
          label_state.msg
          locationInfo.msg
          locationInfo3D.msg
          mapping_ctrl.msg
          marker_visible.msg
          module_abnormal_info.msg
          move_close_loop_ctrl.msg
          move_ctrl.msg
          move_info.msg
          movPred.msg
          movTarget.msg
          movTargetsArray.msg
          nav_algo_info.msg
          nav_ctrl.msg
          nav_state.msg
          point_id.msg
          pose.msg
          pose2D.msg
          pose3D.msg
          pose_score.msg
          PoseInfo.msg
          qrcode_pose.msg
          radarTarget.msg
          radarTargetArray.msg
          roadmark.msg
          serialPassData.msg
          simulation_circular_obst.msg
          simulation_obsts.msg
          stop_ctrl.msg
          traffic_light_info.msg
          traffic_light_result.msg
          traffic_light_target.msg
          trash_box_obst_state.msg
          trash_box_area_obst_state.msg
          ultrasonicData.msg
          ultrasonicsData.msg
          visionDepthCamera.msg
          visionFunc.msg
          visionFuncs.msg
          visionRect.msg
          visionRects.msg
          visionPixel.msg
          visionArea.msg
          visionAreas.msg
          wheelSpeed.msg
          imudata.msg
          Mode.msg
          SaveMap.msg
          security_redundancy.msg
          hmiStatus.msg
          zhishancmd.msg
          customRoute.msg
          sensorExtriParam.msg
          roborExtriParam.msg
          spoiler_detect.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )
generate_messages(
    DEPENDENCIES
    std_msgs
    geometry_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES comm_msg
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)
catkin_package(
    CATKIN_DEPENDS
    message_runtime
    std_msgs
)


###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp library
# add_library(comm_msg
#   src/${PROJECT_NAME}/comm_msg.cpp
# )

## Declare a cpp executable
# add_executable(comm_msg_node src/comm_msg_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(comm_msg_node comm_msg_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(comm_msg_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS comm_msg comm_msg_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_comm_msg.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
