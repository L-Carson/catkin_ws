/*
Author      : gongzhihao
Update Date : 2025/06/26
*/
#ifndef PROTOCOL_BASE_H
#define PROTOCOL_BASE_H

#include "comm_layer/comm_json/comm_json.h"

// 协议基类
class ProtocolBase
{
public:
    // 构造函数
    ProtocolBase(const std::string &ori_module, const std::string &dest_module, int submodule_id);

    // 析构函数
    ~ProtocolBase(void);

    // 打包指令头部
    void PackCommandHeader(Json::Value &json_header, const std::string &cmd, int time_thr);

    // 打包回复头部
    void PackResponseHeader(Json::Value &json_header, const std::string &cmd, const std::string &id);

    // 解析头部
    bool ParseHeader(const Json::Value &json_header, std::string &cmd, std::string &id);

    // 获取当前时间ms
    long long GetNowTimeMs(void);

private:
    // 组合id
    std::string ComposeId(int submodule_id, unsigned int &increased_id, long long time_ms);

    // 删除待响应id
    void DeleteId(const std::string &id);

    // 插入待响应id
    void InsertId(const std::string &id, int time_thr);

private:
    std::mutex id_mutex_;

    std::string ori_module_;
    std::string dest_module_;

    int submodule_id_;
    unsigned int increased_id_;

    std::unordered_map<std::string, int> id_wait_response_;
};

#endif
