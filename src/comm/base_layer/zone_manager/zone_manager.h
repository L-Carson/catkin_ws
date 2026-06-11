/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#ifndef ZONE_MANAGER_H
#define ZONE_MANAGER_H

#include <nav_msgs/Odometry.h>

using namespace comm;
using namespace common_lib;

// 区域管理器
class ZoneManager
{
public:
    // 获取对象指针
    static ZoneManager *GetInstancePointer(void);

    // 析构函数
    ~ZoneManager(void);

    // 更新所有图层所有地图区域
    void UpdateAllZones(void);

    // 获取触发区域列表
    void GetTriggerZonesTable(std::map<ZoneTypeEnum, long long> &trigger_zones_table);

    // 获取离开区域列表
    void GetDepartZonesTable(std::map<ZoneTypeEnum, long long> &depart_zones_table);

    // 获取接近区域列表
    void GetApproachZonesTable(std::map<ZoneTypeEnum, long long> &approach_zones_table);

    // 获取接近距离列表
    void GetApproachDisTable(std::map<ZoneKeyType, float> &approach_dis_table);

private:
    // 构造函数
    ZoneManager(void);

    // 禁止使用拷贝构造函数
    ZoneManager(const ZoneManager &other) = delete;

    // 禁止使用拷贝赋值运算符
    ZoneManager &operator=(const ZoneManager &) = delete;

    // 初始化参数
    void InitParam(void);

    // 初始化ros相关
    void InitForROS(void);

    // 里程数据回调
    void OdomCallback(const nav_msgs::Odometry::ConstPtr &msg_data);

    // 当前地图名回调
    void CurrentMapNameCallback(const std_msgs::String::ConstPtr &msg_data);

    // 管理器线程
    void ManagerThread(void);

    // 管理器周期
    void ManagerCycle(void);

    // 检测触发及离开区域
    void DetectTriggerDepartZones(const std::string &map_name,
                                  const geometry_msgs::Pose2D &curr_pose);

    // 检测接近区域
    void DetectApproachZones(const std::string &map_name,
                             const geometry_msgs::Pose2D &curr_pose,
                             float vel_linear,
                             float vel_angular);

    // 给定位姿检测触发区域
    void DetectTriggerZones(const std::string &map_name,
                            const geometry_msgs::Pose2D &pose,
                            std::map<ZoneTypeEnum, long long> &trigger_zones_table);

    // 生成仿真路径
    bool GenerateTrajectory(float vel_linear, float vel_angular, float sim_time, RouteClass &route);

private:
    static ZoneManager *p_instance_;
    static std::mutex instance_mutex_; // 单例锁

    std::thread *manager_thread_;          // 管理器线程
    std::condition_variable manager_cond_; // 管理器条件变量
    std::mutex thread_mutex_;              // 线程锁
    std::mutex data_mutex_;                // 数据锁
    bool terminate_thread_;                // 线程终止标识
    bool loop_trigger_;                    // 线程启动标识
    float loop_cycle_;                     // 线程循环周期

    ros::NodeHandle nh_;
    ros::Subscriber sub_current_map_name_; // 当前地图名订阅器
    ros::Subscriber sub_odom_;             // 里程订阅器

    TFApiClass *p_tf_api_ = nullptr;                   // TF接口
    zone::FileManager *p_zone_file_manager_ = nullptr; // 区域文件管理器
    zone::ParamServer *p_zone_param_server_ = nullptr; // 区域参数服务器
    RobotSystemUtilClass *p_robot_util_ = nullptr;     // 机器人系统基础设施

    std::string map_name_;
    std::string map_frame_;  // 地图坐标系
    std::string base_frame_; // 载体坐标系

    geometry_msgs::Pose2D robot_pose_; // 当前机器人位姿(map)
    geometry_msgs::Twist curr_vel_;    // 当前机器人速度

    std::map<ZoneTypeEnum, AllMapZonesType> all_zones_; // 所有图层所有地图区域

    std::map<ZoneTypeEnum, long long> last_trigger_zones_table_;  // 上次触发区域列表
    std::map<ZoneTypeEnum, long long> trigger_zones_table_;       // 触发区域列表
    std::map<ZoneTypeEnum, long long> depart_zones_table_;        // 离开区域列表
    std::map<ZoneTypeEnum, long long> last_approach_zones_table_; // 上次接近区域列表
    std::map<ZoneTypeEnum, long long> approach_zones_table_;      // 接近区域列表

    std::map<ZoneKeyType, float> last_approach_dis_table_; // 上次接近距离列表
    std::map<ZoneKeyType, float> approach_dis_table_;      // 接近距离列表
};

#endif
