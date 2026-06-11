/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#ifndef FILE_MANAGER__CFG__FILE_MANAGER_H
#define FILE_MANAGER__CFG__FILE_MANAGER_H

#include "comm_layer/comm_json/comm_json.h"
#include "base_layer/parameter_server/cfg/param_server.h"

namespace cfg
{

    // 配置文件管理器
    class FileManager
    {
    public:
        // 获取对象指针
        static FileManager *GetInstancePointer(void);

    private:
        // 构造函数
        FileManager(void);

        // 禁止使用拷贝构造函数
        FileManager(const FileManager &other) = delete;

        // 禁止使用拷贝赋值运算符
        FileManager &operator=(const FileManager &) = delete;

        // 加载配置信息（从文件）
        bool LoadCfgInfo(void);

        // 解析配置信息
        bool ParseCfgInfo(const Json::Value &json_value);

        // 解析速度配置
        bool ParseVelCfg(const Json::Value &json_value);

        // 解析安全配置
        bool ParseSafeCfg(const Json::Value &json_value);

        // 解析任务配置
        bool ParseTaskCfg(const Json::Value &json_value);

    private:
        static FileManager *p_instance_;
        static std::mutex instance_mutex_; // 单例锁

        ParamServer *p_cfg_param_server_ = nullptr; // 配置参数服务器
    };

} // namespace

#endif
