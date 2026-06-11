#ifndef __COMM_MSG_EQUIPMENT_POSE_H__
#define __COMM_MSG_EQUIPMENT_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/equipment_pose.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 发送组合地图位置信息
修改记录:
**************************************************************************************/
class EQUIPMENT_POSE_SENDER_C
{
public:
    EQUIPMENT_POSE_SENDER_C(ros::NodeHandle &nh, string strTopicName = TOPIC_NAME_EQUIPMENT_POSE)
    {
        publisher_ = nh.advertise<comm_msg::equipment_pose>(strTopicName, 1);
    }

    void Send(BOOL isNormal, const COOR_2D_POSE_STRU &stPose)
    {
        comm_msg::equipment_pose stMsg;

        stMsg.status   = (UINT08)isNormal;
        stMsg.posX     = stPose.stPos.x;
        stMsg.posY     = stPose.stPos.y;
        stMsg.dirAngle = stPose.dirAngle;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 接收组合地图位置信息
修改记录:
**************************************************************************************/
class EQUIPMENT_POSE_RECEIVER_C
{
public:
    EQUIPMENT_POSE_RECEIVER_C(ros::NodeHandle &nh, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_  = nh.subscribe(TOPIC_NAME_EQUIPMENT_POSE, 1, &EQUIPMENT_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
        isNormal_  = false;
    }

    bool  IsAbnormal(void) const {
        return !isNormal_;
    }

    INT32 GetNormal(COOR_2D_POSE_STRU &stPose) const {
        if (!isNormal_ || objInvalidTimer_.IsTimeOut()) {
            return -1;
        }

        stPose.Copy(stPose_);

        return 0;
    }

private:
    TIMER_C             objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    ros::Subscriber     receiver_;
    BOOL                isNormal_;
    COOR_2D_POSE_STRU   stPose_;

    void ReceiveMsgCallBack(const comm_msg::equipment_pose::ConstPtr &pstMsg) {
        objInvalidTimer_.Reset();

        isNormal_        = (BOOL)pstMsg->status;
        stPose_.stPos.x  = pstMsg->posX;
        stPose_.stPos.y  = pstMsg->posY;
        stPose_.dirAngle = pstMsg->dirAngle;
    }
};

#endif

