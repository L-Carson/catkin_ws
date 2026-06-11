/*
Author      : gongzhihao
Update Date : 2025/07/30
*/
#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include "base_layer/log_manager/log_api.h"

extern LogApi *gp_hmi_log;
extern LogApi *gp_dcu_log;
extern LogApi *gp_task_log;
extern LogApi *gp_safe_log;
extern LogApi *gp_motion_log;
extern LogApi *gp_config_log;

// 日志管理器
class LogManager
{
public:
    // 获取对象指针
    static LogManager *GetInstancePointer(void);

private:
    // 构造函数
    LogManager(void);

    // 禁止使用拷贝构造函数
    LogManager(const LogManager &other) = delete;

    // 禁止使用拷贝赋值运算符
    LogManager &operator=(const LogManager &) = delete;

    // 初始化参数
    void InitParam(void);

    // 初始化Pnc日志
    void InitPncLog(void);

    // 初始化Msg日志
    void InitMsgLog(void);

    // 创建日志指针
    LogApi *CreateLogPointer(const std::string &file_dir, const std::string &file_name, const LogTypeEnum &log_type);

private:
    static LogManager *p_instance_;
    static std::mutex instance_mutex_; // 单例锁

    int max_file_size_; // 最大文件大小
    int max_file_num_;  // 最大文件数量
    int max_day_;       // 最大保存天数
    int max_dir_size_;  // 最大目录大小
};

#endif
