/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#ifndef PARAMETER_SERVER__CFG__PARAM_SERVER_H
#define PARAMETER_SERVER__CFG__PARAM_SERVER_H

#include "base_layer/log_manager/log_manager.h"

namespace cfg
{

    // 配置参数服务器
    class ParamServer
    {
    public:
        // 获取对象指针
        static ParamServer *GetInstancePointer(void);

        // 设置速度配置
        void SetVelCfg(const VelCfg &vel_cfg);

        // 设置安全配置
        void SetSafeCfg(const SafeCfg &safe_cfg);

        // 设置任务配置
        void SetTaskCfg(const TaskCfg &task_cfg);

        // 获取速度配置
        void GetVelCfg(VelCfg &vel_cfg);

        // 获取安全配置
        void GetSafeCfg(SafeCfg &safe_cfg);

        // 获取任务配置
        void GetTaskCfg(TaskCfg &task_cfg);

    private:
        // 构造函数
        ParamServer(void);

        // 禁止使用拷贝构造函数
        ParamServer(const ParamServer &other) = delete;

        // 禁止使用拷贝赋值运算符
        ParamServer &operator=(const ParamServer &) = delete;

        // 可视化速度配置
        void VisualVelCfg(const VelCfg &vel_cfg);

        // 可视化安全配置
        void VisualSafeCfg(const SafeCfg &safe_cfg);

        // 可视化任务配置
        void VisualTaskCfg(const TaskCfg &task_cfg);

    private:
        static ParamServer *p_instance_;
        static std::mutex instance_mutex_; // 单例锁

        std::mutex data_mutex_; // 数据锁

        VelCfg vel_cfg_;   // 速度配置
        SafeCfg safe_cfg_; // 安全配置
        TaskCfg task_cfg_; // 任务配置
    };

} // namespace

#endif
