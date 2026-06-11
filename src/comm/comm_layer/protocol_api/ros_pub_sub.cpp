/*
Author      : gongzhihao
Update Date : 2025/06/26
*/
#include "comm_layer/protocol_api/ros_pub_sub.h"

// 构造函数
RosPubSub::RosPubSub(ros::NodeHandle &nh, const std::string &ori_module, const std::string &dest_module)
{
    const std::string pub_topic_name = ori_module + "_to_" + dest_module;
    const std::string sub_topic_name = dest_module + "_to_" + ori_module;

    pub_ = nh.advertise<std_msgs::String>(pub_topic_name, 5, true);

    sub_ = nh.subscribe(sub_topic_name, 5, &RosPubSub::SubCallback, this);
}

// 回调函数
void RosPubSub::SubCallback(const std_msgs::String::ConstPtr &msg_data)
{
    // 订阅回调
    p_sub_interface_->SubCallback(msg_data->data);
}

// 设置订阅回调接口类
void RosPubSub::SetSubCallbackInterface(SubCallbackInterface *p_interface)
{
    p_sub_interface_ = p_interface;
}

// 发布消息
void RosPubSub::PubMsg(const std::string &msg)
{
    std_msgs::String msg_data;
    msg_data.data = msg;

    pub_.publish(msg_data);
}

// 被订阅时再发布
void RosPubSub::PubMsgNecessary(const std::string &msg)
{
    if (pub_.getNumSubscribers() > 0)
    {
        PubMsg(msg);
    }
}
