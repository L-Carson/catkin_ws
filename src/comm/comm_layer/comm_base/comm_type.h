/*
Author      : gongzhihao
Update Date : 2025/07/08
*/
#ifndef COMM_TYPE_H
#define COMM_TYPE_H

#include "comm_layer/comm_base/comm_base.h"

// 按顺序归类，1、enum类，2、struct类，3、typedef类，4、函数类

// 运动部件状态枚举
enum class MotorStateEnum
{
    NORMAL = 0x00,            // 状态正常
    STICK_COMPLETE = 0x01,    // 伸出完成
    WITHDRAW_COMPLETE = 0x02, // 收回完成
    STICKING = 0x03,          // 伸出中...
    WITHDRAWING = 0x04,       // 收回中...
    NO_EXISIT = 0xEE,         // 状态不存在
    ABNORMAL = 0xFF,          // 状态异常
};

// 热熔部件状态枚举
enum class SubterreneStateEnum
{
    STOPPING = 0x00,  // 停止
    COMPLETE = 0x01,  // 热熔完成
    WORKING = 0x02,   // 热熔功能运行中
    NO_EXISIT = 0xEE, // 状态不存在
    ABNORMAL = 0xFF,  // 状态异常
};

// 运动部件状态执行状态枚举，人机-导航交互使用
enum class DevCtrlMotionStateEnum
{
    NORMAL = 1,          // 正常
    NORMAL_COMPLETE = 2, // 正常完成
    ABNORMAL = 3,        // 异常
    RESUME = 4,          // 重试
    RESUME_COMPLETE = 5, // 重试完成
    CANCEL_COMPLETE = 6, // 取消完成
};

// 开放空间目标点类型
enum class OpenSpaceGoalTypeEnum
{
    GARBAGE = 0,                  // 对准任务的目标点，垃圾点
    CHARGE = 1,                   // 对准任务的目标点，充电点或加水点
    PARKING = 2,                  // 库房或临时停靠点
    ALIGNING = 3,                 // 对准任务的起始点
    GARBAGE_NO_BOX = 4,           // 对准任务的目标点，垃圾点(无垃圾筒)
    THROW_RUBBISH = 5,            // 对准任务的目标点，扔垃圾点
    THROW_RUBBISH_NO_STATION = 6, // 对准任务的目标点， 原地扔垃圾点
    START_NARROW = 7,             // 窄道起始点
    MAP_CHANGE = 8,               // 地图切换点
    EMPTY_RUBBISH_NO_STATION = 9, // 原地倒垃圾点
    EMPTY_RUBBISH_SELF_BOX = 10,  // 对准任务的目标点，垃圾点(自带垃圾筒)
    TYPE_BUTT
};

// 告警类型
enum class AlarmTypeEnum
{
    PACING = 0,                 // 原地徘徊
    OUT_BOUNDARY = 1,           // 工作边界外
    DCU_LOST = 2,               // DCU通信丢失（超时）
    TRAFFIC_HOTSPOT = 3,        // 交通热点（易拥堵）
    PIPE_DETECTED = 4,          // 检出管道
    WIRE_DETECTED = 5,          // 检出电线
    OPEN_DOOR_FAIL = 6,         // 开门失败
    CLOSE_DOOR_FAIL = 7,        // 关门失败
    CROSSING = 8,               // 穿越路口
    TRAFFIC_LIGHT_ABNORMAL = 9, // 红绿灯检测异常
    SITE_BLOCKED = 10,          // 站点被阻挡
    EJECT_TRASH_FAIL = 11,      // 扔出垃圾失败
};

// 障碍点类型
enum class ObstPointTypeEnum
{
    UNKNOWN = 0,     // 未知
    STATIC = 1,      // 静态
    DYNAMIC = 2,     // 动态
    HOLLOW = 3,      // 空洞
    LOW = 4,         // 低矮
    HOLD = 5,        // 维持
    BLIND_SPOT = 6,  // 盲区
    SLOPE = 7,       // 斜坡
    BUMPER = 8,      // 触边
    TREELAWN = 9,    // 绿植
    ULTRASONIC = 10, // 超声
    EDGE = 101,      // 路沿
};

// 二维点
struct Point2D
{
    float x;
    float y;
};

// 三维点
struct Point3D
{
    float x;
    float y;
    float z;
};

// 二维位姿
struct Pose2D
{
    float x;
    float y;
    float theta;
};

// 投影信息
struct ProjectionInfo
{
    int index;
    float dis;
};

struct PathPose2D
{
    Pose2D pose;
    float kappa; // 曲率
    float s;     // 路程
};

struct PlanningPath
{
    std::vector<PathPose2D> path;
    bool is_go_forward;
};

struct TrajPoint
{
    PathPose2D path_pose; // 坐标
    float vel;            // 速度
    float t;              // 相对时间，一般起点的相对时间为0
};

struct Trajectory
{
    std::vector<TrajPoint> traj_points;
    bool is_go_forward;
};

struct PredictObst
{
    Pose2D pose;  // 位姿
    float length; // 障碍物长度
    float width;  // 障碍物宽度
    float vel;    // 障碍物速度
};

// ST点
struct STPoint
{
    float s;
    float t;
};

// 速度配置
struct VelCfg
{
    float max_vel_x;
    float min_vel_x;
    float max_vel_clean;
    float max_vel_yaw;
};

// 安全配置
struct SafeCfg
{
    float safe_dis_x;
    float safe_dis_y;
    float left_shorten_len;
    float right_shorten_len;
};

// 任务配置
struct TaskCfg
{
    float trash_offset;
    float empty_trash_dis;
    bool enable_close_door;
};

#endif
