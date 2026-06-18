cmake_minimum_required(VERSION 2.8.3)
project(comm)

# 选择编译器版本，优先选择高版本
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11/14 support. Please use a different C++ compiler.")
endif()

add_definitions(-Wall)                                                                  # Report All Compile Warn
SET(CMAKE_BUILD_TYPE release)                                                           # PCL Exe Occur Seg Fault Without This Item
add_compile_options(-g)
add_compile_options(-ffunction-sections -fdata-sections)                                # 生成目标文件时按照函数分段
add_compile_options(-fPIC)                                                              # 位置无关，used when make .so file.
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")         # Del Not Use Content When Linking so Lib
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL")    # 将所有链接的静态文件中符号设置为隐藏
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")               # Del Not Use Content When Linking Exe File

# 根据UBUNTU系统版本指定依赖包版本
if (1)
    file(READ "/etc/os-release" OS_RELEASE_CONTENT)
    string(REGEX MATCH "VERSION_ID=\"([0-9.]+)\"" _ ${OS_RELEASE_CONTENT})
    set(UBUNTU_VERSION ${CMAKE_MATCH_1})
    if(UBUNTU_VERSION STREQUAL "18.04")
        message(STATUS "System Is Ubuntu 18.04")
        set(OpenCV_VERSION "3.2")
    elseif(UBUNTU_VERSION STREQUAL "20.04")
        message(STATUS "System Is Ubuntu 20.04")
        if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
            set(OpenCV_VERSION "4.2")
        else()
            set(OpenCV_VERSION "4.2")
        endif()
    else()
        message(FATAL_ERROR "Unkown System ${UBUNTU_VERSION}")
    endif()
else()
    cmake_host_system_information(RESULT LOC_OS_VERSION QUERY OS_VERSION)
    if(LOC_OS_VERSION MATCHES ".*18.04.*Ubuntu.*")
        message(STATUS "System Is Ubuntu 18.04")
        set(OpenCV_VERSION "3.2")
    elseif(LOC_OS_VERSION MATCHES ".*20.04.*Ubuntu.*")
        message(STATUS "System Is Ubuntu 20.04")
        set(OpenCV_VERSION "4.2")
    else()
        message(FATAL_ERROR "Unkown System ${OS_VERSION}")
    endif()
endif()

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(
    catkin REQUIRED COMPONENTS
    roscpp
    tf
    comm_srvs
    comm_msg
    pcl_ros
    pcl_conversions
    std_msgs
    core_lib
)

find_package(
    Boost REQUIRED COMPONENTS
    system
)

find_package(OpenCV ${OpenCV_VERSION} EXACT REQUIRED)   # 限定只能查找对应版本的库
find_package(Eigen3 QUIET)

message(STATUS "OpenCV_INCLUDE_DIRS： ${OpenCV_INCLUDE_DIRS}")

# 加如下项，否则可能导致使用的yaml库不是ros对应的版本，而是系统自带的老版本
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAMLCPP yaml-cpp REQUIRED)
if(YAMLCPP_VERSION VERSION_GREATER "0.5.0")
    add_definitions(-DHAVE_YAMLCPP_GT_0_5_0)
endif()
link_directories(${YAMLCPP_LIBRARY_DIRS})
#message("YAMLCPP_LIBRARY_DIRS=${YAMLCPP_LIBRARY_DIRS}")

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
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
#   std_msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

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
    INCLUDE_DIRS
        include
        ${EIGEN3_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}

    LIBRARIES
        comm
        ${Eigen3_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${Boost_LIBRARIES}
        ${catkin_LIBRARIES}

#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS xxx
)

###########
## Build ##
###########

# To Define This MACRO In Code
#ADD_DEFINITIONS(-DTIXML_USE_STL)

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
    ${EIGEN3_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}                  # 需要包含，不然找不到消息头文件
    include
    include/comm
    /usr/local/include
)

link_directories(
    ${CATKIN_DEVEL_PREFIX}/lib              # 需要包含，不然找不到本地生成的库
    /usr/lib
)

## Declare a C++ library
# add_library(xxx
#   src/${PROJECT_NAME}/xxx.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
set(GEOGRAPHICLIB_PRECISION 2 CACHE STRING "Precision: 1 = float, 2 = double, 3 = extended, 4 = quadruple, 5 = variable")
set_property(CACHE GEOGRAPHICLIB_PRECISION PROPERTY STRINGS 1 2 3 4 5)


## Declare a C++ executable
add_library(
    comm STATIC
    src/comm_base.cpp
    src/comm_car.cpp
    src/comm_car_body_file.cpp
    src/comm_dbg_log.cpp
    src/comm_dbg_sw.cpp
    src/comm_file.cpp
    src/comm_function.cpp
    src/comm_geometry.cpp
    src/comm_json_api.cpp
    src/comm_lidar_type_parser.cpp
    src/comm_map.cpp
    src/comm_module_abnormal.cpp
    src/comm_proc_monitor.cpp
    src/comm_ros_base.cpp
    src/comm_ros_trc.cpp
    src/comm_serial.cpp

    src/comm_socketcan.cpp
    src/comm_socketcan_sender.cpp
    src/comm_socketcan_passer.cpp
    src/comm_socketcan_linker.cpp
    
    src/comm_serial_frame.cpp
    src/comm_serial_linker.cpp
    src/comm_serial_passer.cpp
    src/comm_serial_sender.cpp
    src/comm_show.cpp
    src/comm_srv_camera_param.cpp
    src/comm_srv_vision_func.cpp
    src/comm_sys.cpp
    src/comm_test_base.cpp
    src/comm_time.cpp
    src/comm_trc.cpp
    src/comm_urdf_api.cpp
    src/comm_yaml.cpp

    src/comm_cfg/comm_cfg_param_file.cpp
    src/comm_cfg/comm_cfg_param_interface.cpp
    src/comm_cfg/comm_cfg_param_read_base.cpp
    src/comm_cfg/comm_cfg_param_read_mnl.cpp
    src/comm_cfg/comm_cfg_param_read_pnc.cpp
    src/comm_cfg/comm_cfg_param_read_pnp.cpp
    src/comm_cfg/comm_cfg_param_read_sensor.cpp
    src/comm_cfg/comm_cfg_amend.cpp
    src/comm_cfg/comm_cfg_smoother.cpp

    src/comm_hd_map/comm_road_network_data.cpp
    src/comm_hd_map/comm_road_network_file.cpp
    src/comm_hd_map/comm_road_network_show.cpp
    src/comm_hd_map/comm_geo_coord_transformer.cpp
    src/comm_hd_map/comm_dync_map_file.cpp

    src/loc_xml/tinystr.cpp
    src/loc_xml/tinyxml.cpp
    src/loc_xml/tinyxmlerror.cpp
    src/loc_xml/tinyxmlparser.cpp

    src/loc_json/json_reader.cpp
    src/loc_json/json_value.cpp
    src/loc_json/json_writer.cpp

    src/loc_geocentric/Geocentric.cpp
    src/loc_geocentric/LocalCartesian.cpp
    src/loc_geocentric/Math.cpp
)


add_executable(test_any_polygon           test/test_any_polygon.cpp)
add_executable(test_map                   test/test_map.cpp)
add_executable(test_time                  test/test_time.cpp)
add_executable(test_geo_coord_transformer test/test_geo_coord_transformer.cpp)
add_executable(test_dync_map_file         test/test_dync_map_file.cpp)
add_executable(test_geometry              test/test_geometry.cpp)
add_executable(test_module_abnormal       test/test_module_abnormal.cpp)

target_link_libraries(test_any_polygon              comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(test_map                      comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(test_time                     comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(test_geo_coord_transformer    comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(test_dync_map_file            comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(test_geometry                 comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(test_module_abnormal          comm ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(xx ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
## Dependent library need to be placed at the back

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
# install(TARGETS xx xx
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
# catkin_add_gtest(${PROJECT_NAME}-test test/test_xx.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
