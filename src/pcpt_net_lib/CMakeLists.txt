cmake_minimum_required(VERSION 2.8.3)

project(pcpt_net_lib)

SET(CMAKE_BUILD_TYPE release)

find_package(
    catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
)

catkin_package(
  INCLUDE_DIRS include
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
   
find_library(NVINFER NAMES nvinfer)
find_library(NVPARSERS NAMES nvparsers)
find_library(NVONNXPARSERS NAMES nvonnxparser)

list(APPEND LINK_LIBRARY
            ${NVINFER}
            ${NVONNXPARSERS}
            ${CUDA_LIBRARIES}
            )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D_MWAITXINTRIN_H_INCLUDED -D_FORCE_INLINES ")

if (CMAKE_VERSION VERSION_LESS "3.1")
    add_compile_options("-std=c++14 -fext-numeric-literals")
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

include_directories(include)
file(GLOB SRC src/*.cc src/*.cu  src/*.cpp )
cuda_add_library(${PROJECT_NAME} ${SRC} OPTIONS -DYAML_BUILD_SHARED_LIBS=ON ) 
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARY} )
