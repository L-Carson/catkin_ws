/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#include "base_layer/parameter_server/cfg/param_server.h"

namespace cfg
{

    ParamServer *ParamServer::p_instance_ = nullptr;
    std::mutex ParamServer::instance_mutex_; // 单例锁

    // 获取对象指针
    ParamServer *ParamServer::GetInstancePointer(void)
    {
        if (p_instance_ == nullptr)
        {
            // 区域锁
            std::lock_guard<std::mutex> lock(instance_mutex_);

            if (p_instance_ == nullptr)
            {
                p_instance_ = new ParamServer();
            }
        }

        return p_instance_;
    }

    // 构造函数
    ParamServer::ParamServer(void)
    {
    }

    // 设置速度配置
    void ParamServer::SetVelCfg(const VelCfg &vel_cfg)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        vel_cfg_ = vel_cfg;

        // 可视化速度配置
        VisualVelCfg(vel_cfg);
    }

    // 设置安全配置
    void ParamServer::SetSafeCfg(const SafeCfg &safe_cfg)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        safe_cfg_ = safe_cfg;

        // 可视化安全配置
        VisualSafeCfg(safe_cfg);
    }

    // 设置任务配置
    void ParamServer::SetTaskCfg(const TaskCfg &task_cfg)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        task_cfg_ = task_cfg;

        // 可视化任务配置
        VisualTaskCfg(task_cfg);
    }

    // 获取速度配置
    void ParamServer::GetVelCfg(VelCfg &vel_cfg)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        vel_cfg = vel_cfg_;
    }

    // 获取安全配置
    void ParamServer::GetSafeCfg(SafeCfg &safe_cfg)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        safe_cfg = safe_cfg_;
    }

    // 获取任务配置
    void ParamServer::GetTaskCfg(TaskCfg &task_cfg)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        task_cfg = task_cfg_;
    }

    // 可视化速度配置
    void ParamServer::VisualVelCfg(const VelCfg &vel_cfg)
    {
        LOG_INFO(gp_config_log, "--------------- 加载配置: 速度配置 ---------------");

        LOG_INFO(gp_config_log, "最大X速度[%.3f]", vel_cfg.max_vel_x);
        LOG_INFO(gp_config_log, "最小X速度[%.3f]", vel_cfg.min_vel_x);
        LOG_INFO(gp_config_log, "最大清扫速度[%.3f]", vel_cfg.max_vel_clean);
        LOG_INFO(gp_config_log, "最大Yaw速度[%.3f]", vel_cfg.max_vel_yaw);

        LOG_INFO(gp_config_log, "----------------------------------------------------\n");
    }

    // 可视化安全配置
    void ParamServer::VisualSafeCfg(const SafeCfg &safe_cfg)
    {
        LOG_INFO(gp_config_log, "--------------- 加载配置: 安全配置 ---------------");

        LOG_INFO(gp_config_log, "X安全距离[%.3f]", safe_cfg.safe_dis_x);
        LOG_INFO(gp_config_log, "Y安全距离[%.3f]", safe_cfg.safe_dis_y);
        LOG_INFO(gp_config_log, "左收缩长度[%.3f]", safe_cfg.left_shorten_len);
        LOG_INFO(gp_config_log, "右收缩长度[%.3f]", safe_cfg.right_shorten_len);

        LOG_INFO(gp_config_log, "----------------------------------------------------\n");
    }

    // 可视化任务配置
    void ParamServer::VisualTaskCfg(const TaskCfg &task_cfg)
    {
        LOG_INFO(gp_config_log, "--------------- 加载配置: 任务配置 ---------------");

        LOG_INFO(gp_config_log, "倒垃圾偏移[%.3f]", task_cfg.trash_offset);
        LOG_INFO(gp_config_log, "清空垃圾距离[%.3f]", task_cfg.empty_trash_dis);
        LOG_INFO(gp_config_log, "使能关门[%d]", task_cfg.enable_close_door);

        LOG_INFO(gp_config_log, "----------------------------------------------------\n");
    }

} // namespace
