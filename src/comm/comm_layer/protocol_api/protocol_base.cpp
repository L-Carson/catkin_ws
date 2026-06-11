/*
Author      : gongzhihao
Update Date : 2025/06/26
*/
#include "comm_layer/protocol_api/protocol_base.h"

// 构造函数
ProtocolBase::ProtocolBase(const std::string &ori_module, const std::string &dest_module, int submodule_id)
{
    ori_module_ = ori_module;
    dest_module_ = dest_module;

    submodule_id_ = submodule_id;
    increased_id_ = 0;

    id_wait_response_.clear();
}

// 析构函数
ProtocolBase::~ProtocolBase(void)
{
}

// 打包指令头部
void ProtocolBase::PackCommandHeader(Json::Value &json_header, const std::string &cmd, int time_thr)
{
    // 获取当前时间ms
    long long now_time_ms = GetNowTimeMs();

    std::string id = ComposeId(submodule_id_, increased_id_, now_time_ms);

    json_header.clear();
    json_header["ori"] = ori_module_;
    json_header["dest"] = dest_module_;
    json_header["cmd"] = cmd;
    json_header["id"] = id;
    json_header["time"] = static_cast<Json::UInt64>(now_time_ms);

    // 插入待响应id
    InsertId(id, time_thr);
}

// 打包回复头部
void ProtocolBase::PackResponseHeader(Json::Value &json_header, const std::string &cmd, const std::string &id)
{
    // 获取当前时间ms
    long long now_time_ms = GetNowTimeMs();

    json_header.clear();
    json_header["ori"] = ori_module_;
    json_header["dest"] = dest_module_;
    json_header["cmd"] = cmd;
    json_header["id"] = id.empty() ? ComposeId(submodule_id_, increased_id_, now_time_ms) : id;
    json_header["time"] = static_cast<Json::UInt64>(now_time_ms);
}

// 解析头部
bool ProtocolBase::ParseHeader(const Json::Value &json_header, std::string &cmd, std::string &id)
{
    if (!json_header.isObject() ||
        !json_header["ori"].isString() ||
        !json_header["dest"].isString() ||
        !json_header["cmd"].isString() ||
        !json_header["id"].isString() ||
        !json_header["time"].isUInt64())
    {
        return false;
    }

    if (json_header["ori"].asString() != dest_module_ || json_header["dest"].asString() != ori_module_)
    {
        return false;
    }

    cmd = json_header["cmd"].asString();
    id = json_header["id"].asString();

    // 删除待响应id
    DeleteId(id);

    return true;
}

// 获取当前时间ms
long long ProtocolBase::GetNowTimeMs(void)
{
    long long now_time_ms = ros::Time::now().toSec() * 1000;
    return now_time_ms;
}

// 组合id
std::string ProtocolBase::ComposeId(int submodule_id, unsigned int &increased_id, long long time_ms)
{
    std::stringstream id_string;

    id_string << std::setw(2) << std::setfill('0') << submodule_id << "_" << increased_id << "_" << time_ms;
    increased_id++;

    return id_string.str();
}

// 删除待响应id
void ProtocolBase::DeleteId(const std::string &id)
{
    std::unique_lock<std::mutex> lock(id_mutex_);

    std::unordered_map<std::string, int>::iterator it = id_wait_response_.find(id);
    if (it != id_wait_response_.end())
    {
        id_wait_response_.erase(it);
    }
}

// 插入待响应id
void ProtocolBase::InsertId(const std::string &id, int time_thr)
{
    std::unique_lock<std::mutex> lock(id_mutex_);

    id_wait_response_[id] = time_thr;
}
