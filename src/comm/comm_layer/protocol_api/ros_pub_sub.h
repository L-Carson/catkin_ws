/*
Author      : gongzhihao
Update Date : 2025/06/26
*/
#ifndef ROS_PUB_SUB_H
#define ROS_PUB_SUB_H

#include "comm_layer/comm_base/comm_base.h"

// 订阅回调接口类
class SubCallbackInterface
{
public:
    // 订阅回调
    virtual void SubCallback(const std::string &msg) = 0;
};

// 话题发布订阅类
class RosPubSub
{
public:
    // 构造函数
    RosPubSub(ros::NodeHandle &nh, const std::string &ori_module, const std::string &dest_module);

    // 设置订阅回调接口类
    void SetSubCallbackInterface(SubCallbackInterface *p_interface);

    // 发布消息
    void PubMsg(const std::string &msg);

    // 被订阅时再发布
    void PubMsgNecessary(const std::string &msg);

private:
    // 回调函数
    void SubCallback(const std_msgs::String::ConstPtr &msg_data);

private:
    ros::Publisher pub_;
    ros::Subscriber sub_;

    SubCallbackInterface *p_sub_interface_ = nullptr; // 订阅回调接口类
};

#endif
