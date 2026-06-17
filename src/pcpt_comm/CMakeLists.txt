cmake_minimum_required(VERSION 2.8.3)

project(pcpt_comm)

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
add_compile_options(-ffunction-sections -fdata-sections)                                # 生成目标文件时按照函数分段
add_compile_options(-fPIC)                                                              # 位置无关，used when make .so file.
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")         # Del Not Use Content When Linking so Lib
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL")    # 将所有链接的静态文件中符号设置为隐藏
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")               # Del Not Use Content When Linking Exe File

find_package(
    catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    sensor_msgs
    pcl_ros
    image_transport
    cv_bridge
    comm_msg
    comm_srvs
    comm
    comm_extend
    comm_ros_extend
    core_lib
)

# find_package(PCL REQUIRED)
find_package(Eigen3 QUIET)

catkin_package(
    INCLUDE_DIRS
        include
        ${EIGEN3_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}

    LIBRARIES
        pcpt_comm
        ${Eigen3_LIBRARIES}
        ${catkin_LIBRARIES}

#  CATKIN_DEPENDS xxx
#  DEPENDS xxx
)

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
    include
    /usr/local/include
)

link_directories(
    ${CATKIN_DEVEL_PREFIX}/lib
    ${PCL_LIBRARY_DIRS}
    /usr/lib
)

add_library(
    pcpt_comm STATIC
    src/pcpt_comm_abnormal.cpp
    src/pcpt_comm_alarm.cpp
    src/pcpt_comm_base.cpp
    src/pcpt_comm_bspline.cpp
    src/pcpt_comm_car.cpp
    src/pcpt_comm_camera.cpp
    src/pcpt_comm_cfg.cpp
    src/pcpt_comm_curve.cpp
    src/pcpt_comm_points.cpp
    src/pcpt_comm_debug.cpp
    src/pcpt_comm_file.cpp
    src/pcpt_comm_format.cpp
    src/pcpt_comm_hmi.cpp
    src/pcpt_comm_hungarian.cpp
    src/pcpt_comm_iou.cpp
    src/pcpt_comm_marker.cpp
    src/pcpt_comm_msg.cpp
    src/pcpt_comm_polygon.cpp
    src/pcpt_comm_plane.cpp
    src/pcpt_comm_region.cpp
    src/pcpt_comm_ros.cpp
    src/pcpt_comm_server.cpp
    src/pcpt_comm_fitting.cpp
    src/pcpt_comm_subgraph.cpp
    src/pcpt_comm_tf.cpp
    src/pcpt_comm_time.cpp
    src/pcpt_comm_undistortion.cpp
    src/pcpt_comm_data_collection.cpp
    src/pcpt_comm_vision.cpp
    src/pcpt_comm_image.cpp
    src/pcpt_comm_singleton.cpp
    src/pcpt_comm_car_obst_view.cpp
)



add_executable(test_data_collect          test/test_data_collection.cpp)

target_link_libraries(test_data_collect             pcpt_comm   ${catkin_LIBRARIES})
