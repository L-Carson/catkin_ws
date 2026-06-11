/*
Author      : gongzhihao
Update Date : 2025/07/30
*/
#include "base_layer/log_manager/log_manager.h"

LogApi *gp_hmi_log = nullptr;    // 人机通信日志
LogApi *gp_dcu_log = nullptr;    // DCU通信日志
LogApi *gp_task_log = nullptr;   // 任务日志
LogApi *gp_safe_log = nullptr;   // 安全日志
LogApi *gp_motion_log = nullptr; // 运动日志
LogApi *gp_config_log = nullptr; // 配置日志

LogManager *LogManager::p_instance_ = nullptr;
std::mutex LogManager::instance_mutex_; // 单例锁

// 获取对象指针
LogManager *LogManager::GetInstancePointer(void)
{
    if (p_instance_ == nullptr)
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(instance_mutex_);

        if (p_instance_ == nullptr)
        {
            p_instance_ = new LogManager();
        }
    }

    return p_instance_;
}

// 构造函数
LogManager::LogManager(void)
{
    // 初始化参数
    InitParam();

    // 初始化Pnc日志
    InitPncLog();

    // 初始化Msg日志
    InitMsgLog();
}

// 初始化参数
void LogManager::InitParam(void)
{
    max_file_size_ = 10 * 1024 * 1024; // 10MB
    max_file_num_ = 10;
    max_day_ = 7;
    max_dir_size_ = 512 * 1024 * 1024; // 512MB
}

// 初始化Pnc日志
void LogManager::InitPncLog(void)
{
    static std::string pnc_log_dir = g_user_dir + "map_config/log/debug/pnc/";

    gp_hmi_log = CreateLogPointer(pnc_log_dir, "hmi", LogTypeEnum::RUNNING_LOG);
    gp_dcu_log = CreateLogPointer(pnc_log_dir, "dcu", LogTypeEnum::RUNNING_LOG);
    gp_task_log = CreateLogPointer(pnc_log_dir, "task", LogTypeEnum::RUNNING_LOG);
    gp_safe_log = CreateLogPointer(pnc_log_dir, "safe", LogTypeEnum::RUNNING_LOG);
    gp_motion_log = CreateLogPointer(pnc_log_dir, "motion", LogTypeEnum::RUNNING_LOG);
    gp_config_log = CreateLogPointer(pnc_log_dir, "config", LogTypeEnum::RUNNING_LOG);
}

// 初始化Msg日志
void LogManager::InitMsgLog(void)
{
    static std::string msg_log_dir = g_user_dir + "map_config/log/msg/";
}

// 创建日志指针
LogApi *LogManager::CreateLogPointer(const std::string &file_dir, const std::string &file_name, const LogTypeEnum &log_type)
{
    return new LogApi(file_dir, file_name, log_type, max_file_size_, max_file_num_, max_day_, max_dir_size_);
}
