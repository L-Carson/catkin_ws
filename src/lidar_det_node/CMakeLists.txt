cmake_minimum_required(VERSION 3.10)
project(lidar_det_node)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  geometry_msgs
  visualization_msgs
  pcl_ros
  pcl_conversions
  comm_msg
  lidar_det_interface
)
find_package(PCL REQUIRED)

catkin_package(
  CATKIN_DEPENDS roscpp sensor_msgs comm_msg visualization_msgs lidar_det_interface
)

# ---------------------------------------------------------------------------
#  External backend shared library (built by the TransFusion CMake project).
#  Override with -DTRANSFUSION_BACKEND_LIB=/abs/path/libtransfusion_backend.so
#  Build order: lidar_det_interface -> (TransFusion) transfusion_backend -> this.
# ---------------------------------------------------------------------------
set(TRANSFUSION_ROOT
    "$ENV{HOME}/Saite/3D_target_detection/deploy/Lidar_orin_Solution/TransFusion"
    CACHE PATH "TransFusion project root")
set(TRANSFUSION_BACKEND_LIB
    "${TRANSFUSION_ROOT}/build/libtransfusion_backend.so"
    CACHE FILEPATH "Path to libtransfusion_backend.so")

# libspconv dir for runtime rpath (the backend .so depends on it transitively).
set(LIBSPCONV_DIR
    "${TRANSFUSION_ROOT}/../CenterPoint/3rdparty/libspconv/lib/x86_64"
    CACHE PATH "Directory containing libspconv.so for runtime rpath")

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} src/lidar_det_node.cpp)
add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})

if(NOT EXISTS "${TRANSFUSION_BACKEND_LIB}")
  message(FATAL_ERROR
    "transfusion_backend not found at ${TRANSFUSION_BACKEND_LIB}\n"
    "Build it first:\n"
    "  cd ${TRANSFUSION_ROOT} && mkdir -p build && cd build\n"
    "  cmake .. -DCMAKE_BUILD_TYPE=Release && make -j transfusion_backend\n"
    "Or pass -DTRANSFUSION_BACKEND_LIB=/abs/path/libtransfusion_backend.so")
endif()

get_filename_component(_backend_dir "${TRANSFUSION_BACKEND_LIB}" DIRECTORY)

target_link_libraries(${PROJECT_NAME}
  ${TRANSFUSION_BACKEND_LIB}
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
)

# Find the backend .so and its transitive libspconv at runtime.
set_target_properties(${PROJECT_NAME} PROPERTIES
  BUILD_RPATH   "${_backend_dir};${LIBSPCONV_DIR}"
  INSTALL_RPATH "${_backend_dir};${LIBSPCONV_DIR}"
)

# Disk .bin -> PointCloud2 replayer (no backend dependency; for offline testing).
add_executable(bin_cloud_publisher src/bin_cloud_publisher.cpp)
add_dependencies(bin_cloud_publisher ${catkin_EXPORTED_TARGETS})
target_link_libraries(bin_cloud_publisher ${catkin_LIBRARIES} ${PCL_LIBRARIES})

install(TARGETS ${PROJECT_NAME} bin_cloud_publisher
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
install(DIRECTORY config launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  OPTIONAL
)
