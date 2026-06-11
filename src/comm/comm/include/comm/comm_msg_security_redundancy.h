

#ifndef __COMM_MSG_RECURITY_REDUNDANCY_H__
#define __COMM_MSG_RECURITY_REDUNDANCY_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/security_redundancy.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class SECURITY_REDUNDANCY_SENDER_C
{
public:
    SECURITY_REDUNDANCY_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::security_redundancy>(TOPIC_NAME_SECURITY_REDUNDANCY, 1);
    }

    void Send(const BOOL &isStopLocationAbnormal, const BOOL &isStopCollision)
    {
        comm_msg::security_redundancy stMsg;

        stMsg.header.stamp  = ros::Time::now();
        stMsg.isStopLocationAbnormal   = isStopLocationAbnormal;
        stMsg.isStopCollision          = isStopCollision;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class SECURITY_REDUNDANCY_RECEIVER_C
{
public:
    SECURITY_REDUNDANCY_RECEIVER_C(ros::NodeHandle &nh)
    {
        subscriber_ = nh.subscribe(TOPIC_NAME_SECURITY_REDUNDANCY, 1, &SECURITY_REDUNDANCY_RECEIVER_C::ReceiveMsgCallBack, this);
        isStopLocationAbnormal_ = false;
        isStopCollision_        = false;
    }

    void Get(BOOL &isStopLocationAbnormal, BOOL &isStopCollision) const
    {
        isStopLocationAbnormal     = isStopLocationAbnormal_;
        isStopCollision            = isStopCollision_;
    }

private:
    ros::Subscriber             subscriber_;

    BOOL                        isStopLocationAbnormal_;
    BOOL                        isStopCollision_;

    void ReceiveMsgCallBack(const comm_msg::security_redundancy::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        isStopLocationAbnormal_    = pstMsg->isStopLocationAbnormal;
        isStopCollision_           = pstMsg->isStopCollision;
    }
};


#endif

