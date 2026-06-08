cmake_minimum_required(VERSION 3.10)
project(lidar_det_node)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
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
#  Paths are auto-detected relative to this package (portable across machines).
#  Overrides (relative paths resolve from this CMakeLists.txt):
#    -DTRANSFUSION_ROOT=../../../3d_target_detection/deploy/.../TransFusion
#    -DTRANSFUSION_BACKEND_LIB=.../build/libtransfusion_backend.so
#    -DLIBSPCONV_DIR=.../CenterPoint/3rdparty/libspconv/lib/aarch64
#  Build order: lidar_det_interface -> (TransFusion) transfusion_backend -> this.
# ---------------------------------------------------------------------------
set(TRANSFUSION_ROOT "" CACHE PATH "TransFusion project root; empty = auto-detect")
set(TRANSFUSION_BACKEND_LIB "" CACHE FILEPATH "libtransfusion_backend.so; empty = auto")
set(LIBSPCONV_DIR "" CACHE PATH "libspconv.so directory for rpath; empty = auto")

set(_tf_arch "${CMAKE_HOST_SYSTEM_PROCESSOR}")

# --- resolve TransFusion root ---
set(_tf_root_user "${TRANSFUSION_ROOT}")
if(_tf_root_user)
  if(NOT IS_ABSOLUTE "${_tf_root_user}")
    get_filename_component(_tf_root_user
      "${CMAKE_CURRENT_SOURCE_DIR}/${_tf_root_user}" ABSOLUTE)
  else()
    get_filename_component(_tf_root_user "${_tf_root_user}" ABSOLUTE)
  endif()
  if(NOT EXISTS "${_tf_root_user}/CMakeLists.txt")
    message(WARNING "TRANSFUSION_ROOT=${_tf_root_user} invalid; auto-detecting")
    set(_tf_root_user "")
    unset(TRANSFUSION_ROOT CACHE)
  endif()
endif()

set(TRANSFUSION_ROOT_DIR "")
if(_tf_root_user)
  set(TRANSFUSION_ROOT_DIR "${_tf_root_user}")
else()
  set(_tf_root_candidates
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../3d_target_detection/deploy/Lidar_orin_Solution/TransFusion"
  )
  foreach(_cand IN LISTS _tf_root_candidates)
    get_filename_component(_cand_abs "${_cand}" ABSOLUTE)
    if(EXISTS "${_cand_abs}/CMakeLists.txt")
      set(TRANSFUSION_ROOT_DIR "${_cand_abs}")
      break()
    endif()
  endforeach()
endif()

# --- resolve libtransfusion_backend.so ---
set(_tf_backend_user "${TRANSFUSION_BACKEND_LIB}")
if(_tf_backend_user)
  if(NOT IS_ABSOLUTE "${_tf_backend_user}")
    get_filename_component(_tf_backend_user
      "${CMAKE_CURRENT_SOURCE_DIR}/${_tf_backend_user}" ABSOLUTE)
  else()
    get_filename_component(_tf_backend_user "${_tf_backend_user}" ABSOLUTE)
  endif()
  if(NOT EXISTS "${_tf_backend_user}")
    message(WARNING "TRANSFUSION_BACKEND_LIB=${_tf_backend_user} invalid; auto-detecting")
    set(_tf_backend_user "")
    unset(TRANSFUSION_BACKEND_LIB CACHE)
  endif()
endif()

set(TRANSFUSION_BACKEND_LIB_RESOLVED "")
if(_tf_backend_user)
  set(TRANSFUSION_BACKEND_LIB_RESOLVED "${_tf_backend_user}")
elseif(TRANSFUSION_ROOT_DIR)
  set(TRANSFUSION_BACKEND_LIB_RESOLVED
    "${TRANSFUSION_ROOT_DIR}/build/libtransfusion_backend.so")
endif()

# --- resolve libspconv directory (runtime rpath for backend .so) ---
set(_libspconv_user "${LIBSPCONV_DIR}")
if(_libspconv_user)
  if(NOT IS_ABSOLUTE "${_libspconv_user}")
    get_filename_component(_libspconv_user
      "${CMAKE_CURRENT_SOURCE_DIR}/${_libspconv_user}" ABSOLUTE)
  else()
    get_filename_component(_libspconv_user "${_libspconv_user}" ABSOLUTE)
  endif()
  if(NOT EXISTS "${_libspconv_user}/libspconv.so")
    message(WARNING "LIBSPCONV_DIR=${_libspconv_user} invalid; auto-detecting")
    set(_libspconv_user "")
    unset(LIBSPCONV_DIR CACHE)
  endif()
endif()

set(LIBSPCONV_DIR_RESOLVED "")
if(_libspconv_user)
  set(LIBSPCONV_DIR_RESOLVED "${_libspconv_user}")
elseif(TRANSFUSION_ROOT_DIR)
  set(_libspconv_lib_root
    "${TRANSFUSION_ROOT_DIR}/../CenterPoint/3rdparty/libspconv/lib")
  if(EXISTS "${_libspconv_lib_root}/${_tf_arch}/libspconv.so")
    set(LIBSPCONV_DIR_RESOLVED "${_libspconv_lib_root}/${_tf_arch}")
  else()
    file(GLOB _spconv_cuda_dirs "${_libspconv_lib_root}/${_tf_arch}_cuda*")
    foreach(_d IN LISTS _spconv_cuda_dirs)
      if(EXISTS "${_d}/libspconv.so")
        set(LIBSPCONV_DIR_RESOLVED "${_d}")
        break()
      endif()
    endforeach()
  endif()
endif()

if(TRANSFUSION_ROOT_DIR)
  message(STATUS "lidar_det_node: TRANSFUSION_ROOT=${TRANSFUSION_ROOT_DIR}")
endif()
if(LIBSPCONV_DIR_RESOLVED)
  message(STATUS "lidar_det_node: LIBSPCONV_DIR=${LIBSPCONV_DIR_RESOLVED}")
endif()

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} src/lidar_det_node.cpp)
add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})

if(NOT TRANSFUSION_BACKEND_LIB_RESOLVED OR
    NOT EXISTS "${TRANSFUSION_BACKEND_LIB_RESOLVED}")
  set(_tf_build_hint "${TRANSFUSION_ROOT_DIR}")
  if(NOT _tf_build_hint)
    set(_tf_build_hint
      "<auto: ../../../3d_target_detection/deploy/Lidar_orin_Solution/TransFusion>")
  endif()
  message(FATAL_ERROR
    "transfusion_backend not found at ${TRANSFUSION_BACKEND_LIB_RESOLVED}\n"
    "Build it first:\n"
    "  cd ${_tf_build_hint} && mkdir -p build && cd build\n"
    "  cmake .. -DCMAKE_BUILD_TYPE=Release && make -j transfusion_backend\n"
    "Or pass -DTRANSFUSION_BACKEND_LIB=<relative-or-absolute-path>")
endif()

get_filename_component(_backend_dir "${TRANSFUSION_BACKEND_LIB_RESOLVED}" DIRECTORY)

set(_lidar_det_rpath "${_backend_dir}")
if(LIBSPCONV_DIR_RESOLVED)
  set(_lidar_det_rpath "${_backend_dir};${LIBSPCONV_DIR_RESOLVED}")
elseif(TRANSFUSION_ROOT_DIR)
  message(WARNING
    "lidar_det_node: libspconv not found under "
    "${TRANSFUSION_ROOT_DIR}/../CenterPoint/3rdparty/libspconv/lib; "
    "runtime may fail to load transfusion_backend")
endif()

target_link_libraries(${PROJECT_NAME}
  ${TRANSFUSION_BACKEND_LIB_RESOLVED}
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
)

# Find the backend .so and its transitive libspconv at runtime.
set_target_properties(${PROJECT_NAME} PROPERTIES
  BUILD_RPATH   "${_lidar_det_rpath}"
  INSTALL_RPATH "${_lidar_det_rpath}"
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
