cmake_minimum_required(VERSION 2.8.3)
project(core_lib)

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

#配置当前的系统
set(MY_INCLUDES_DIR "include")

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    set(MY_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/arm64)
else()
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        # 64位区分系统版本，当前支持Ubuntu 18.04和20.04
        cmake_host_system_information(RESULT LOC_OS_VERSION QUERY OS_VERSION)
        if(LOC_OS_VERSION MATCHES ".*18.04.*Ubuntu.*")
            message(STATUS "System Is Ubuntu 18.04")
            set(MY_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/64_18.04)
            
        elseif(LOC_OS_VERSION MATCHES ".*20.04.*Ubuntu.*")
            message(STATUS "System Is Ubuntu 20.04")
            set(MY_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/64_20.04)
            
        else()
            message(FATAL_ERROR "Unkown System ${OS_VERSION}")
        endif()
    else()
        message(STATUS "System Is Ubuntu 14.04")
        set(MY_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/32)
    endif()
endif()

message(STATUS "My Lib Dir Is ${MY_LIBRARIES_DIR}")

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)
find_package(CURL  REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex system)

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    saijie_lib/include
    saijie_lib/src
    ${MY_INCLUDES_DIR}/basic/include
    ${MY_INCLUDES_DIR}/websockets/include
    ${MY_INCLUDES_DIR}/saijie/include
    ${MY_INCLUDES_DIR}/tiny_xml/include
    ${MY_INCLUDES_DIR}/json/include
    ${MY_INCLUDES_DIR}/log4cplus/include
)

#message(SATUS " ${Boost_LIBRARIES}")
#message(SATUS " ${Boost_LIBRARY_DIRS}")


# 该处要顺序要求，被依赖的库要放在后面，否则链接会有问题
catkin_package(
    INCLUDE_DIRS
        ./include
        ${MY_INCLUDES_DIR}/basic/include
        ${MY_INCLUDES_DIR}/websockets/include
        ${MY_INCLUDES_DIR}/saijie/include
        ${MY_INCLUDES_DIR}/tiny_xml/include
        ${MY_INCLUDES_DIR}/json/include
        ${MY_INCLUDES_DIR}/log4cplus/include
        ${catkin_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIRS}

    LIBRARIES
        ${MY_LIBRARIES_DIR}/libsaijie_basics.a
        ${MY_LIBRARIES_DIR}/libbasics.a
        # libdl.so
        ${CMAKE_DL_LIBS}
        ${catkin_LIBRARIES}
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES}
)

#add_executable(saijie_demo main.cpp)

#set_target_properties(saijie_demo PROPERTIES LINK_FLAGS "-Wl,-rpath,/home/saite/navZero/self_exe_so:/home/zhishan/navZero/self_exe_so")

#target_link_libraries(saijie_demo
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/saijie/lib/libsaijie_basics.so
#    ${catkin_LIBRARIES}
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/basic/lib/libbasics.a
#    #${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/so/liblog_handler_no_console.so
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/basic/lib/liblog_handler.a
#    ${Boost_LIBRARY_DIRS}/libboost_regex.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/json/lib/libjsoncpp_static.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/log4cplus/lib/liblog4cplus.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/websockets/lib/libwebsockets.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/sqlite3/lib/libsqlite3.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/websockets/lib/libssl.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/websockets/lib/libcrypto.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/zemq/lib/libzmq.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/tiny_xml/lib/libtiny_xml.a
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/share_memory/lib/libfastcommon.so
#    ${CMAKE_CURRENT_SOURCE_DIR}/lib/${MY_LIBRARIES_DIR}/share_memory/lib/libshmcache.so
#    -ldl
#    -pthread
#)





















