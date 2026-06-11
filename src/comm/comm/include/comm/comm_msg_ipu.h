#ifndef __COMM_MSG_IPU_H__
#define __COMM_MSG_IPU_H__

#include "comm/comm_base.h"
#include "comm/comm_topic_name.h"
#include <comm_msg/ipu.h>
#include <ros/ros.h>

using namespace std;

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class IPU_SENDER_C
{
public:
    IPU_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::ipu>(TOPIC_NAME_IPU, 1);
    }

    void Send()
    {
        comm_msg::ipu msg;
        msg.header.stamp = ros::Time::now();
        msg.header.frame_id = "IPU";
        objPublisher_.publish(msg);
    }

private:
    ros::Publisher     objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class IPU_RECEIVER_C
{
public:
    IPU_RECEIVER_C(ros::NodeHandle &nh, double overTime = 30.0) : objDataValidTimer_(overTime)
    {
        objReceiver_ = nh.subscribe(TOPIC_NAME_IPU, 1, &IPU_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    bool IsIpuFault()
    {
        return objDataValidTimer_.IsTimeOut();
    }

private:
    ros::Subscriber  objReceiver_;
    TIMER_C          objDataValidTimer_;       /* 数据失效定时器，超时后数据无效 */

    void ReceiveMsgCallBack(const comm_msg::ipu::ConstPtr &pstMsg)
    {
        objDataValidTimer_.Reset();
    }
};


#endif

