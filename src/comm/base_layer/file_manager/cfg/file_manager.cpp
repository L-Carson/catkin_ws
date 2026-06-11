/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#include "base_layer/file_manager/cfg/file_manager.h"

namespace cfg
{

    FileManager *FileManager::p_instance_ = nullptr;
    std::mutex FileManager::instance_mutex_; // 单例锁

    // 获取对象指针
    FileManager *FileManager::GetInstancePointer(void)
    {
        if (p_instance_ == nullptr)
        {
            // 区域锁
            std::lock_guard<std::mutex> lock(instance_mutex_);

            if (p_instance_ == nullptr)
            {
                p_instance_ = new FileManager();
            }
        }

        return p_instance_;
    }

    // 构造函数
    FileManager::FileManager(void)
    {
        // 配置参数服务器
        p_cfg_param_server_ = ParamServer::GetInstancePointer();

        // 加载配置信息（从文件）
        LoadCfgInfo();
    }

    // 加载配置信息（从文件）
    bool FileManager::LoadCfgInfo(void)
    {
        std::string cfg_abs_file_name = g_nav_zero_dir + "cfg/pnc/cfg.json";

        // 读文件
        Json::Value json_root = StrToJson(ReadFile(cfg_abs_file_name));

        if (!json_root.isObject())
        {
            return false;
        }

        // 解析配置信息
        if (!ParseCfgInfo(json_root))
        {
            return false;
        }

        return true;
    }

    // 解析配置信息
    bool FileManager::ParseCfgInfo(const Json::Value &json_value)
    {
        // 解析速度配置
        if (!ParseVelCfg(json_value["vel_cfg"]))
        {
            return false;
        }

        // 解析安全配置
        if (!ParseSafeCfg(json_value["safe_cfg"]))
        {
            return false;
        }

        // 解析任务配置
        if (!ParseTaskCfg(json_value["task_cfg"]))
        {
            return false;
        }

        return true;
    }

    // 解析速度配置
    bool FileManager::ParseVelCfg(const Json::Value &json_value)
    {
        if (!json_value.isObject())
        {
            return false;
        }

        if (!json_value["max_vel_x"].isDouble() ||
            !json_value["min_vel_x"].isDouble() ||
            !json_value["max_vel_clean"].isDouble() ||
            !json_value["max_vel_yaw"].isDouble())
        {
            return false;
        }

        VelCfg vel_cfg;
        vel_cfg.max_vel_x = json_value["max_vel_x"].asDouble();
        vel_cfg.min_vel_x = json_value["min_vel_x"].asDouble();
        vel_cfg.max_vel_clean = json_value["max_vel_clean"].asDouble();
        vel_cfg.max_vel_yaw = json_value["max_vel_yaw"].asDouble();

        // 设置速度配置
        p_cfg_param_server_->SetVelCfg(vel_cfg);

        return true;
    }

    // 解析安全配置
    bool FileManager::ParseSafeCfg(const Json::Value &json_value)
    {
        if (!json_value.isObject())
        {
            return false;
        }

        if (!json_value["safe_dis_x"].isDouble() ||
            !json_value["safe_dis_y"].isDouble() ||
            !json_value["left_shorten_len"].isDouble() ||
            !json_value["right_shorten_len"].isDouble())
        {
            return false;
        }

        SafeCfg safe_cfg;
        safe_cfg.safe_dis_x = json_value["safe_dis_x"].asDouble();
        safe_cfg.safe_dis_y = json_value["safe_dis_y"].asDouble();
        safe_cfg.left_shorten_len = json_value["left_shorten_len"].asDouble();
        safe_cfg.right_shorten_len = json_value["right_shorten_len"].asDouble();

        // 设置安全配置
        p_cfg_param_server_->SetSafeCfg(safe_cfg);

        return true;
    }

    // 解析任务配置
    bool FileManager::ParseTaskCfg(const Json::Value &json_value)
    {
        if (!json_value.isObject())
        {
            return false;
        }

        if (!json_value["trash_offset"].isDouble() ||
            !json_value["empty_trash_dis"].isDouble() ||
            !json_value["enable_close_door"].isBool())
        {
            return false;
        }

        TaskCfg task_cfg;
        task_cfg.trash_offset = json_value["trash_offset"].asDouble();
        task_cfg.empty_trash_dis = json_value["empty_trash_dis"].asDouble();
        task_cfg.enable_close_door = json_value["enable_close_door"].asBool();

        // 设置任务配置
        p_cfg_param_server_->SetTaskCfg(task_cfg);

        return true;
    }

} // namespace
