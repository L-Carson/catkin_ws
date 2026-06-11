/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#include "zone_manager/zone_manager.h"

ZoneManager *ZoneManager::p_instance_ = nullptr;
std::mutex ZoneManager::instance_mutex_; // 单例锁

// 获取对象指针
ZoneManager *ZoneManager::GetInstancePointer(void)
{
    if (p_instance_ == nullptr)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(instance_mutex_);

        if (p_instance_ == nullptr)
        {
            p_instance_ = new ZoneManager();
        }
    }

    return p_instance_;
}

// 构造函数
ZoneManager::ZoneManager(void)
{
    // 区域文件管理器
    p_zone_file_manager_ = zone::FileManager::GetInstancePointer();

    // 区域参数服务器
    p_zone_param_server_ = zone::ParamServer::GetInstancePointer();

    // 机器人系统基础设施
    p_robot_util_ = RobotSystemUtilClass::GetInstancePoint();

    // 初始化参数
    InitParam();

    // 初始化ros相关
    InitForROS();

    // 管理器线程
    manager_thread_ = new std::thread(&ZoneManager::ManagerThread, this);
}

// 析构函数
ZoneManager::~ZoneManager(void)
{
    // 区域锁
    std::lock_guard<std::mutex> thread_lock(thread_mutex_);

    terminate_thread_ = true;

    if (manager_thread_ != nullptr)
    {
        delete manager_thread_;
    }
}

// 初始化参数
void ZoneManager::InitParam(void)
{
    terminate_thread_ = false;
    loop_trigger_ = true;
    loop_cycle_ = 1.0f / 2.0f;

    map_frame_ = p_robot_util_->map_frame_;
    base_frame_ = p_robot_util_->base_frame_;
}

// 初始化ros相关
void ZoneManager::InitForROS(void)
{
    p_tf_api_ = new TFApiClass(gp_nav_safe_log);

    sub_odom_ = nh_.subscribe("/odom", 1, &ZoneManager::OdomCallback, this);
    sub_current_map_name_ = nh_.subscribe("/current_map_name", 1, &ZoneManager::CurrentMapNameCallback, this);
}

// 里程数据回调
void ZoneManager::OdomCallback(const nav_msgs::Odometry::ConstPtr &msg_data)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    curr_vel_ = msg_data->twist.twist;
}

// 当前地图名回调
void ZoneManager::CurrentMapNameCallback(const std_msgs::String::ConstPtr &msg_data)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    map_name_ = msg_data->data;
}

// 更新所有图层所有地图区域
void ZoneManager::UpdateAllZones(void)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    // 获取所有图层所有地图区域
    p_zone_param_server_->GetAllZones(all_zones_);
}

// 获取触发区域列表
void ZoneManager::GetTriggerZonesTable(std::map<ZoneTypeEnum, long long> &trigger_zones_table)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    trigger_zones_table = trigger_zones_table_;
}

// 获取离开区域列表
void ZoneManager::GetDepartZonesTable(std::map<ZoneTypeEnum, long long> &depart_zones_table)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    depart_zones_table = depart_zones_table_;
}

// 获取接近区域列表
void ZoneManager::GetApproachZonesTable(std::map<ZoneTypeEnum, long long> &approach_zones_table)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    approach_zones_table = approach_zones_table_;
}

// 获取接近距离列表
void ZoneManager::GetApproachDisTable(std::map<ZoneKeyType, float> &approach_dis_table)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    approach_dis_table = approach_dis_table_;
}

// 管理器线程
void ZoneManager::ManagerThread(void)
{
    LOG4_THREAD_P(gp_nav_safe_log);

    boost::unique_lock<std::mutex> thread_lock(thread_mutex_);

    // 线程循环
    while (!terminate_thread_ && nh_.ok())
    {
        // 等待唤醒标识置位或者启动标识复位
        while (!loop_trigger_)
        {
            // 若条件不满足，则释放锁，阻塞当前线程；若条件满足，则加锁；
            manager_cond_.wait(thread_lock);
        }

        // 解锁: 任务锁
        thread_lock.unlock();

        // 启动的时间
        ros::WallTime begin_time = ros::WallTime::now();

        // 管理器周期
        ManagerCycle();

        // 统计时间代价
        double cycle_time_sec = (ros::WallTime::now() - begin_time).toSec();

        double rest_time_sec = loop_cycle_ - cycle_time_sec;
        if (rest_time_sec > 0)
        {
            ros::Duration(rest_time_sec).sleep();
        }

        // 加锁: 任务锁
        thread_lock.lock();
    }
}

// 管理器周期
void ZoneManager::ManagerCycle(void)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    // 查询机器人位姿
    if (!p_tf_api_->UpdateTFImple(map_frame_, base_frame_, robot_pose_))
    {
        return;
    }

    // 检测触发及离开区域
    DetectTriggerDepartZones(map_name_, robot_pose_);

    // 检测接近区域
    DetectApproachZones(map_name_, robot_pose_, curr_vel_.linear.x, curr_vel_.angular.z);
}

// 检测触发及离开区域
void ZoneManager::DetectTriggerDepartZones(const std::string &map_name,
                                           const geometry_msgs::Pose2D &curr_pose)
{
    // 给定位姿检测触发区域
    DetectTriggerZones(map_name, curr_pose, trigger_zones_table_);

    // 清空离开区域列表
    depart_zones_table_.clear();

    // 离开: 上次触发但当前未触发
    for (auto &one_zone : last_trigger_zones_table_)
    {
        if (trigger_zones_table_.find(one_zone.first) == trigger_zones_table_.end())
        {
            depart_zones_table_[one_zone.first] = one_zone.second;
            LOG4_INFO_P(gp_nav_safe_log, "离开[%s] 区域id[%lld]",
                        p_zone_param_server_->GetZoneName(one_zone.first).data(), one_zone.second);
        }
    }

    // 触发: 当前触发但上次未触发
    for (auto &one_zone : trigger_zones_table_)
    {
        if (last_trigger_zones_table_.find(one_zone.first) == last_trigger_zones_table_.end())
        {
            LOG4_INFO_P(gp_nav_safe_log, "触发[%s] 区域id[%lld]",
                        p_zone_param_server_->GetZoneName(one_zone.first).data(), one_zone.second);
        }
    }

    // 缓存触发区域列表
    last_trigger_zones_table_ = trigger_zones_table_;
}

// 检测接近区域
void ZoneManager::DetectApproachZones(const std::string &map_name,
                                      const geometry_msgs::Pose2D &curr_pose,
                                      float vel_linear,
                                      float vel_angular)
{
    // 清空接近区域列表及接近距离列表
    approach_zones_table_.clear();
    approach_dis_table_.clear();

    RouteClass sim_route;
    float sign = (fabs(vel_linear) < 0.01f) ? 1.0f : ((vel_linear < 0) ? -1.0f : 1.0f);
    float vel_x = sign * std::max(fabs(vel_linear), 0.3f);

    // 生成仿真路径
    GenerateTrajectory(vel_x, vel_angular, 3.0f, sim_route);

    // 转换目标路径到global坐标系
    sim_route.CalcGlobalRoute(curr_pose);

    // 遍历所有路径点（从远到近，首路径点除外）
    int route_size = sim_route.GetSize();
    for (int cnt = route_size - 1; cnt > 0; --cnt)
    {
        const WayPointClass *p_waypoint = sim_route.GetWayPoint(cnt);

        geometry_msgs::Pose2D pose;
        pose.x = p_waypoint->position_.x;
        pose.y = p_waypoint->position_.y;

        // 估算当前路径点与首点距离
        float dis = cnt * kRoutePointsDistance;

        std::map<ZoneTypeEnum, long long> trigger_zones_table;

        // 给定位姿检测触发区域
        DetectTriggerZones(map_name, pose, trigger_zones_table);

        for (auto &one_zone : trigger_zones_table)
        {
            // 若已触发，则舍弃，避免区域同时触发及接近
            std::map<ZoneTypeEnum, long long>::iterator it = trigger_zones_table_.find(one_zone.first);
            if (it != trigger_zones_table_.end() && it->second == one_zone.second)
            {
                continue;
            }

            approach_zones_table_[one_zone.first] = one_zone.second;

            ZoneKeyType zone_key(one_zone.first, one_zone.second);
            approach_dis_table_[zone_key] = dis;
        }
    }

    // 离开: 上次接近但当前未接近且未触发
    for (auto &one_zone : last_approach_zones_table_)
    {
        if (approach_zones_table_.find(one_zone.first) == approach_zones_table_.end() &&
            trigger_zones_table_.find(one_zone.first) == trigger_zones_table_.end())
        {
            depart_zones_table_[one_zone.first] = one_zone.second;
            LOG4_INFO_P(gp_nav_safe_log, "离开[%s] 区域id[%lld]",
                        p_zone_param_server_->GetZoneName(one_zone.first).data(), one_zone.second);
        }
    }

    // 接近: 仿真路径触发或距离不同
    for (auto &one_zone : approach_zones_table_)
    {
        ZoneKeyType zone_key(one_zone.first, one_zone.second);

        std::map<ZoneKeyType, float>::iterator it = last_approach_dis_table_.find(zone_key);
        if (it == last_approach_dis_table_.end() || it->second != approach_dis_table_[zone_key])
        {
            LOG4_INFO_P(gp_nav_safe_log, "接近[%s] 区域id[%lld] 距离[%.3f]",
                        p_zone_param_server_->GetZoneName(one_zone.first).data(),
                        one_zone.second,
                        approach_dis_table_[zone_key]);
        }
    }

    // 缓存接近区域列表及接近距离列表
    last_approach_zones_table_ = approach_zones_table_;
    last_approach_dis_table_ = approach_dis_table_;
}

// 给定位姿检测触发区域
void ZoneManager::DetectTriggerZones(const std::string &map_name,
                                     const geometry_msgs::Pose2D &pose,
                                     std::map<ZoneTypeEnum, long long> &trigger_zones_table)
{
    // 清空触发区域列表
    trigger_zones_table.clear();

    // 遍历所有图层
    for (auto &all_map_zones : all_zones_)
    {
        // 区域类型
        ZoneTypeEnum zone_type = all_map_zones.first;

        AllMapZonesType::iterator one_map_zones = all_map_zones.second.find(map_name);
        if (one_map_zones == all_map_zones.second.end())
        {
            continue;
        }

        // 遍历单地图所有区域
        for (auto &one_zone : one_map_zones->second)
        {
            if (IsInsidePolygon(one_zone.second, pose.x, pose.y))
            {
                long long zone_id = one_zone.first;
                trigger_zones_table[zone_type] = zone_id;
                break;
            }
        }
    }
}

// 生成仿真路径
bool ZoneManager::GenerateTrajectory(float vel_linear, float vel_angular, float sim_time, RouteClass &route)
{
    route.Init();

    if (IsNearZero(vel_linear) && IsNearZero(vel_angular))
    {
        return false;
    }

    float sim_distance = fabs(vel_linear) * sim_time;
    float sim_angle = fabs(vel_angular) * sim_time;
    int num_steps = std::ceil(std::max(sim_distance / kRoutePointsDistance, sim_angle / kWaypointAngleInterval));
    num_steps = std::max(num_steps, 3);

    // 计算仿真时间步长
    float dt = sim_time / num_steps;

    // 圆弧运动增量
    float dx, dy, dtheta;
    dtheta = vel_angular * dt;
    if (!IsNearZero(vel_angular))
    {
        float r = vel_linear / vel_angular;
        dx = std::sin(dtheta) * r;
        dy = (1.0f - std::cos(dtheta)) * r;
    }
    else
    {
        dx = vel_linear * dt;
        dy = 0.0f;
    }

    WayPointClass waypoint(0.0f, 0.0f, 0.0f);
    std::vector<WayPointClass> waypoints;

    // 迭代生成路径
    for (int i = 0; i < num_steps; ++i)
    {
        waypoints.push_back(waypoint);

        float cos_angle = std::cos(waypoint.direction_.Val());
        float sin_angle = std::sin(waypoint.direction_.Val());

        float new_x = waypoint.position_.x + dx * cos_angle - dy * sin_angle;
        float new_y = waypoint.position_.y + dy * cos_angle + dx * sin_angle;
        float new_theta = waypoint.direction_.Val() + dtheta;
        NormalizeAngleForRad(new_theta);

        waypoint.position_.x = new_x;
        waypoint.position_.y = new_y;
        waypoint.direction_ = new_theta;
    }

    route.SetWayPoints(waypoints, 0, 0);

    return true;
}
