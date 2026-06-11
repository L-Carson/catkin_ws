/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#include "comm_layer/protocol_api/protocol_api.h"

// 构造函数
template <typename T>
ProtocolApi<T>::ProtocolApi(const std::string &ori_module, const std::string &dest_module, int submodule_id)
    : ProtocolBase(ori_module, dest_module, submodule_id),
      SubCallbackInterface()
{
    // 话题发布订阅类
    p_pub_sub_ = new RosPubSub(nh_, ori_module, dest_module);

    // 设置订阅回调接口类
    p_pub_sub_->SetSubCallbackInterface(this);

    // 初始化参数
    InitParam();

    // 接收线程
    recv_thread_ = new std::thread(&ProtocolApi::RecvThread, this);
}

// 析构函数
template <typename T>
ProtocolApi<T>::~ProtocolApi(void)
{
    // 区域锁
    std::lock_guard<std::mutex> thread_lock(thread_mutex_);

    terminate_thread_ = true;

    if (recv_thread_ != nullptr)
    {
        delete recv_thread_;
    }
}

// 初始化参数
template <typename T>
void ProtocolApi<T>::InitParam(void)
{
    terminate_thread_ = false;
    loop_trigger_ = false;
    loop_cycle_ = 1.0f / 50.0f;
}

// 设置接收回调函数
template <typename T>
void ProtocolApi<T>::SetRecvFunc(RecvCallback func)
{
    recv_func_ = func;
}

// 更新指令名列表
template <typename T>
void ProtocolApi<T>::UpdateCmdStrTable(const T &cmd_type, const std::string &cmd_str)
{
    cmd_str_table_[cmd_type] = cmd_str;
}

// 更新指令类型列表
template <typename T>
void ProtocolApi<T>::UpdateCmdTypeTable(void)
{
    cmd_type_table_.clear();

    for (const auto &it : cmd_str_table_)
    {
        cmd_type_table_[it.second] = it.first;
    }
}

// 发送指令消息
template <typename T>
void ProtocolApi<T>::SendCmdMsg(const T &cmd_type, const Json::Value &json_data)
{
    Json::Value json_msg;

    // 打包指令头部
    PackCommandHeader(json_msg["header"], GetCmdStr(cmd_type), 3);

    // 额外数据段
    if (json_data.isObject())
    {
        json_msg["data"] = json_data;
    }

    // 被订阅时再发布
    p_pub_sub_->PubMsgNecessary(JsonToStr(json_msg));
}

// 发送结果消息
template <typename T>
void ProtocolApi<T>::SendResultMsg(const T &cmd_type,
                                   const std::string &id,
                                   int status,
                                   const std::string &error_code,
                                   const std::string &msg,
                                   const Json::Value &json_data_info)
{
    Json::Value json_msg;

    // 打包回复头部
    PackResponseHeader(json_msg["header"], GetCmdStr(cmd_type), id);

    // 打包数据段
    json_msg["data"]["status"] = status;
    json_msg["data"]["error_code"] = error_code;
    json_msg["data"]["msg"] = msg;

    // 额外数据段
    if (json_data_info.isObject())
    {
        json_msg["data"]["data_info"] = json_data_info;
    }

    // 被订阅时再发布
    p_pub_sub_->PubMsgNecessary(JsonToStr(json_msg));
}

// 接收线程
template <typename T>
void ProtocolApi<T>::RecvThread(void)
{
    std::unique_lock<std::mutex> thread_lock(thread_mutex_);

    // 线程循环
    while (!terminate_thread_ && nh_.ok())
    {
        // 等待唤醒标识置位或者启动标识复位
        while (!loop_trigger_)
        {
            // 若条件不满足，则释放锁，阻塞当前线程；若条件满足，则加锁；
            recv_cond_.wait(thread_lock);
        }

        // 解锁：任务锁
        thread_lock.unlock();

        // 启动的时间
        ros::WallTime begin_time = ros::WallTime::now();

        // 接收周期
        RecvCycle();

        // 统计时间代价
        double cycle_time_sec = (ros::WallTime::now() - begin_time).toSec();
        double rest_time_sec = loop_cycle_ - cycle_time_sec;

        if (rest_time_sec > 0)
        {
            ros::Duration(rest_time_sec).sleep();
        }

        // 加锁：任务锁
        thread_lock.lock();
    }
}

// 接收周期
template <typename T>
void ProtocolApi<T>::RecvCycle(void)
{
    // 弹出最早消息数据
    if (PopFirstMsgData(first_msg_data_))
    {
        // 接收回调函数
        recv_func_(first_msg_data_);
    }
}

// 订阅回调
template <typename T>
void ProtocolApi<T>::SubCallback(const std::string &msg)
{
    // 字符串->Json
    Json::Value json_msg = StrToJson(msg);

    if (!json_msg.isObject())
    {
        return;
    }

    CmdInfoSt<T> msg_data;
    if (ParseMsg(json_msg, msg_data))
    {
        // 加入消息数据
        PushMsgData(std::move(msg_data));
    }
}

// 解析消息
template <typename T>
bool ProtocolApi<T>::ParseMsg(const Json::Value &json_msg, CmdInfoSt<T> &msg_data)
{
    if (!json_msg["header"].isObject() || !json_msg["data"].isObject())
    {
        return false;
    }

    // 解析头部
    if (!ParseHeader(json_msg["header"], msg_data.cmd_str, msg_data.id))
    {
        return false;
    }

    // 获取指令类型
    if (!GetCmdType(msg_data.cmd_str, msg_data.cmd_type))
    {
        return false;
    }

    msg_data.json_data = json_msg["data"];
    return true;
}

// 加入消息数据
template <typename T>
void ProtocolApi<T>::PushMsgData(CmdInfoSt<T> &&msg_data)
{
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        msg_datas_.push_back(std::move(msg_data));
    }

    {
        // 区域锁
        std::lock_guard<std::mutex> thread_lock(thread_mutex_);

        if (!loop_trigger_)
        {
            loop_trigger_ = true;

            // 唤醒条件变量，唤醒接收线程
            recv_cond_.notify_one();
        }
    }
}

// 弹出最早消息数据
template <typename T>
bool ProtocolApi<T>::PopFirstMsgData(CmdInfoSt<T> &msg_data)
{
    {
        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        if (!msg_datas_.empty())
        {
            msg_data = std::move(msg_datas_.front());
            msg_datas_.pop_front();
            return true;
        }
    }

    {
        // 区域锁
        std::lock_guard<std::mutex> thread_lock(thread_mutex_);

        loop_trigger_ = false;
    }

    return false;
}

// 获取指令名
template <typename T>
std::string ProtocolApi<T>::GetCmdStr(const T &cmd_type)
{
    typename std::unordered_map<T, std::string>::iterator it = cmd_str_table_.find(cmd_type);
    if (it != cmd_str_table_.end())
    {
        return it->second;
    }

    return {};
}

// 获取指令类型
template <typename T>
bool ProtocolApi<T>::GetCmdType(const std::string &cmd_str, T &cmd_type)
{
    typename std::unordered_map<std::string, T>::iterator it = cmd_type_table_.find(cmd_str);
    if (it != cmd_type_table_.end())
    {
        cmd_type = it->second;
        return true;
    }

    return false;
}
