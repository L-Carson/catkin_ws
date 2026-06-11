/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#ifndef PROTOCOL_API_H
#define PROTOCOL_API_H

#include "comm_layer/protocol_api/ros_pub_sub.h"
#include "comm_layer/protocol_api/protocol_base.h"

// 指令信息
template <typename T>
struct CmdInfoSt
{
    T cmd_type;
    std::string cmd_str;
    std::string id;
    long long time;
    Json::Value json_data;
};

// 协议接口类
template <typename T>
class ProtocolApi : public ProtocolBase, public SubCallbackInterface
{
public:
    // 接收回调函数
    typedef std::function<bool(const CmdInfoSt<T> &)> RecvCallback;

public:
    // 构造函数
    ProtocolApi(const std::string &ori_module, const std::string &dest_module, int submodule_id);

    // 析构函数
    ~ProtocolApi(void);

    // 设置接收回调函数
    void SetRecvFunc(RecvCallback func);

    // 更新指令名列表
    void UpdateCmdStrTable(const T &cmd_type, const std::string &cmd_str);

    // 更新指令类型列表
    void UpdateCmdTypeTable(void);

    // 发送指令消息
    void SendCmdMsg(const T &cmd_type, const Json::Value &json_data);

    // 发送结果消息
    void SendResultMsg(const T &cmd_type,
                       const std::string &id,
                       int status,
                       const std::string &error_code,
                       const std::string &msg,
                       const Json::Value &json_data_info);

private:
    // 初始化参数
    void InitParam(void);

    // 接收线程
    void RecvThread(void);

    // 接收周期
    void RecvCycle(void);

    // 订阅回调
    virtual void SubCallback(const std::string &msg) override;

    // 解析消息
    bool ParseMsg(const Json::Value &json_msg, CmdInfoSt<T> &msg_data);

    // 加入消息数据
    void PushMsgData(CmdInfoSt<T> &&msg_data);

    // 弹出最早消息数据
    bool PopFirstMsgData(CmdInfoSt<T> &msg_data);

    // 获取指令名
    std::string GetCmdStr(const T &cmd_type);

    // 获取指令类型
    bool GetCmdType(const std::string &cmd_str, T &cmd_type);

private:
    std::thread *recv_thread_;          // 接收线程
    std::condition_variable recv_cond_; // 接收条件变量
    std::mutex thread_mutex_;           // 线程锁
    std::mutex data_mutex_;             // 数据锁
    bool terminate_thread_;             // 线程终止标识
    bool loop_trigger_;                 // 线程启动标识
    float loop_cycle_;                  // 线程循环周期

    ros::NodeHandle nh_;

    RosPubSub *p_pub_sub_ = nullptr;   // 话题发布订阅类
    RecvCallback recv_func_ = nullptr; // 接收回调函数

    CmdInfoSt<T> first_msg_data_; // 最早消息数据

    std::deque<CmdInfoSt<T>> msg_datas_; // 消息数据

    std::unordered_map<T, std::string> cmd_str_table_;  // 指令名列表
    std::unordered_map<std::string, T> cmd_type_table_; // 指令类型列表
};

#include "protocol_api/protocol_api_impl.h"

#endif
