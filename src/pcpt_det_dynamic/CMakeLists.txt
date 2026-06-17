cmake_minimum_required(VERSION 2.8.3)

project(pcpt_det_dynamic)

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
        set(OpenCV_VERSION "4.2")
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

find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    pcl_ros
    comm_msg
    comm
    comm_extend
    comm_ros_extend
    pcpt_comm
)

find_package(OpenCV ${OpenCV_VERSION} REQUIRED)

catkin_package(
    INCLUDE_DIRS
        include
        ${OpenCV_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}

    LIBRARIES
        pcpt_det_dynamic
        ${OpenCV_LIBRARIES}
        ${catkin_LIBRARIES}

#  CATKIN_DEPENDS xxx
#  DEPENDS xxx
)

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    include
    /usr/local/include
)

link_directories(
    ${CATKIN_DEVEL_PREFIX}/lib
    ${OpenCV_LIBRARIES}
    /usr/lib
)

add_library(
    pcpt_det_dynamic STATIC
    src/pcpt_det_dynamic.cpp
    src/pcpt_trajectory_tracker.cpp
    src/pcpt_motion_estimator.cpp
    src/pcpt_kalman_filter.cpp
    src/pcpt_track.cpp
)

add_executable(
    pcpt_det_dynamic_node
    src/pcpt_det_dynamic_test_node.cpp
)

target_link_libraries(
    pcpt_det_dynamic_node
    pcpt_det_dynamic
    ${catkin_LIBRARIES}
    yaml-cpp
)
