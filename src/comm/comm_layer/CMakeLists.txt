cmake_minimum_required(VERSION 2.8.3)
project(comm_layer)

# 选择编译器版本，优先选择高版本
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if (COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else ()
    message(STATUS "编译器不支持C++11及C++14")
endif ()

add_compile_options(-fPIC)

# 构建类型为RELEASE
set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-s")

find_package(catkin REQUIRED
    COMPONENTS
        roscpp
        rospy
        core_lib
)

include_directories(
    ${catkin_INCLUDE_DIRS}
    ../
)

catkin_package(
    INCLUDE_DIRS
        ../
    LIBRARIES
        comm_layer
)

add_library(comm_layer STATIC
    comm_function/comm_function.cpp
    comm_geometry/comm_geometry.cpp
    comm_json/comm_json.cpp
    comm_math/comm_math.cpp
    comm_path/comm_path.cpp
    comm_time/comm_time.cpp
    comm_visual/comm_visual.cpp
    protocol_api/protocol_base.cpp
    protocol_api/ros_pub_sub.cpp
    planned_trj.cpp
)
target_link_libraries(comm_layer
    PUBLIC
        ${catkin_LIBRARIES}
)
