cmake_minimum_required(VERSION 2.8.3)
project(maintain_node)

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

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(
    catkin REQUIRED COMPONENTS
    roscpp
    comm
    comm_srvs
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
#   INCLUDE_DIRS include
#  LIBRARIES xxx
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS xxx
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
    ${catkin_INCLUDE_DIRS}
    include
    /usr/local/include
)

link_directories(
    ${CATKIN_DEVEL_PREFIX}/lib
    /usr/lib
)

## Declare a C++ executable
add_executable(
    maintain_node
    src/maintain_node.cpp
    src/maintain_param.cpp
)

## Specify libraries to link a library or executable target against
## Dependent library need to be placed at the back
target_link_libraries(
    maintain_node
    libcomm.a
    ${catkin_LIBRARIES}
)

add_executable(test_lidar_clbt           test/test_lidar_clbt.cpp)
add_executable(modify_serial_num         test/modify_serial_num.cpp)
add_executable(modify_calib_lidar_yaw    test/modify_calib_lidar_yaw.cpp)

target_link_libraries(test_lidar_clbt              comm ${catkin_LIBRARIES})
target_link_libraries(modify_serial_num            comm ${catkin_LIBRARIES})
target_link_libraries(modify_calib_lidar_yaw       comm ${catkin_LIBRARIES})
