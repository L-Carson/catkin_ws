/*
Author      : gongzhihao
Update Date : 2025/07/30
*/
#ifndef COMM_BASE_H
#define COMM_BASE_H

// 系统类
#include <pwd.h>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <memory>
#include <condition_variable>

// io类
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

// 数学类
#include <cmath>
#include <limits>
#include <algorithm>

// 容器类
#include <map>
#include <set>
#include <list>
#include <deque>
#include <string>
#include <vector>
#include <unordered_map>

// ros类
#include <ros/ros.h>
#include <tf/tf.h>
#include <std_msgs/String.h>
#include <nav_msgs/Path.h>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PolygonStamped.h>

// 第三方库
#include "json/json.h"

constexpr int g_max_int = std::numeric_limits<int>::max();
constexpr int g_min_int = std::numeric_limits<int>::lowest();
constexpr long long g_max_long_long = std::numeric_limits<long long>::max();
constexpr long long g_min_long_long = std::numeric_limits<long long>::lowest();
constexpr unsigned int g_max_unsigned_int = std::numeric_limits<unsigned int>::max();
constexpr unsigned int g_min_unsigned_int = std::numeric_limits<unsigned int>::lowest();
constexpr unsigned long long g_max_unsigned_long_long = std::numeric_limits<unsigned long long>::max();
constexpr unsigned long long g_min_unsigned_long_long = std::numeric_limits<unsigned long long>::lowest();
constexpr float g_max_float = std::numeric_limits<float>::max();
constexpr float g_min_float = std::numeric_limits<float>::lowest();
constexpr double g_max_double = std::numeric_limits<double>::max();
constexpr double g_min_double = std::numeric_limits<double>::lowest();

constexpr float g_waypoint_interval = 0.1f;
// constexpr float g_waypoint_angle_interval = DegToRad(5.0f);

constexpr float map_resolution = 0.05f;

const std::string g_user_name = getpwuid(getuid())->pw_name;
const std::string g_user_dir = "/home/" + g_user_name + "/";
const std::string g_nav_zero_dir = g_user_dir + "/navZero/";

#endif
