cmake_minimum_required(VERSION 2.8.3)

project(pcpt_net_node)
add_definitions(-Wall)      

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

SET(CMAKE_BUILD_TYPE release)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)" )
  include(ProcessorCount)
  ProcessorCount(N)
  message("Processer number:  ${N}")
  if(N GREATER 4)
    add_definitions(-DMP_EN)
    add_definitions(-DMP_PROC_NUM=3)
    message("core for MP: 3")
  elseif(N GREATER 3)
    add_definitions(-DMP_EN)
    add_definitions(-DMP_PROC_NUM=2)
    message("core for MP: 2")
  else()
    add_definitions(-DMP_PROC_NUM=1)
  endif()
else()
  add_definitions(-DMP_PROC_NUM=1)
endif()

find_package(
    catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    pcl_ros
    tf
    comm_msg
    pcl_conversions
    geometry_msgs
    nav_msgs
    sensor_msgs
    rospy
    message_generation
    visualization_msgs
    eigen_conversions
    pcpt_net_lib
    comm
)
find_package(Eigen3 REQUIRED)
find_package(PCL 1.8 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)


include_directories(
  include
  ${catkin_INCLUDE_DIRS} 
  ${EIGEN3_INCLUDE_DIR}
  ${PCL_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  )

include_directories("/usr/include/eigen3/")

catkin_package(
  INCLUDE_DIRS
    include
    ${Boost_INCLUDE_DIRS}

  LIBRARIES
    std_msgs
    nav_msgs
    geometry_msgs
    sensor_msgs
    roscpp
    rospy
    PCL
    libpcpt_net_lib.so
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
  ${EIGEN3_INCLUDE_DIR}
  ${PCL_INCLUDE_DIRS}
)

link_directories(
    ${CATKIN_DEVEL_PREFIX}/lib
    ${PCL_LIBRARY_DIRS}
)
find_package(CUDA REQUIRED QUIET)
include_directories(${CUDA_INCLUDE_DIRS})
if (CUDA_VERSION VERSION_LESS "11.0")
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -std=c++11 -Wno-deprecated-gpu-targets)
else ()
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -std=c++14 -Wno-deprecated-gpu-targets)
endif ()

# set(CMAKE_INCLUDE_PATH  /home/aero/software/TensorRT-8.0.0.3.Linux.x86_64-gnu.cuda-11.0.cudnn8.2/TensorRT-8.0.0.3/include)
# set(CMAKE_LIBRARY_PATH  /home/aero/software/TensorRT-8.0.0.3.Linux.x86_64-gnu.cuda-11.0.cudnn8.2/TensorRT-8.0.0.3/lib)
set(CMAKE_INCLUDE_PATH $ENV{HOME}/software/TensorRT-8.0.0.3/include)
set(CMAKE_LIBRARY_PATH $ENV{HOME}/software/TensorRT-8.0.0.3/lib)

include_directories(${CMAKE_INCLUDE_PATH} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D_MWAITXINTRIN_H_INCLUDED -D_FORCE_INLINES ")

if (CMAKE_VERSION VERSION_LESS "3.1")
    add_compile_options("-std=c++14 -fext-numeric-literals")
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()
            
file(GLOB SRC src/*.cc  src/*.cpp )

add_executable(pcpt_net_node ${SRC} )
target_link_libraries(pcpt_net_node
    libpcpt_net_lib.so
    ${catkin_LIBRARIES}
    ${PCL_LIBRARIES} 
    yaml-cpp
)



