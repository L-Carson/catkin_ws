

#ifndef __COMM_MSG_MOVE_INFO_H__
#define __COMM_MSG_MOVE_INFO_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/move_info.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

struct DCU_MOVE_INFO_STRU {
    float leftWheelVel;
    float rightWheelVel;
    float moveLinearVel;
    float yawAngle;
    float yawAngleVel;
    float steeringAngle;            /* 单位弧度 */
    float accelerationX;            /* X轴加速度，方向为车体向右，单位g */
    float accelerationY;            /* Y轴加速度，方向为车体向前，单位g */
    float accelerationZ;            /* Z轴加速度，方向为车体向上，单位g */

    void SetZero(void)
    {
        leftWheelVel  = 0.0;
        rightWheelVel = 0.0;
        moveLinearVel = 0.0;
        yawAngle      = 0.0;
        yawAngleVel   = 0.0;
        steeringAngle = 0.0;
        accelerationX = 0.0;
        accelerationY = 0.0;
        accelerationZ = 0.0;
    }

};

struct DCU_MOVE_INFO_EX_STRU {
    double             time;
    DCU_MOVE_INFO_STRU stInfo;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class MOVE_INFO_SENDER_C
{
public:
    MOVE_INFO_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::move_info>(TOPIC_NAME_MOVE_INFO, 1);
    }

    void Send(const DCU_MOVE_INFO_STRU &stMoveInfo)
    {
        comm_msg::move_info stMsg;

        stMsg.header.stamp  = ros::Time::now();

        stMsg.steeringAngle = stMoveInfo.steeringAngle;
        stMsg.moveLinearVel = stMoveInfo.moveLinearVel;
        stMsg.yawAngle      = stMoveInfo.yawAngle;
        stMsg.yawAngleVel   = stMoveInfo.yawAngleVel;
        stMsg.accX          = stMoveInfo.accelerationX;
        stMsg.accY          = stMoveInfo.accelerationY;
        stMsg.accZ          = stMoveInfo.accelerationZ;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class MOVE_INFO_RECEIVER_C
{
public:
    MOVE_INFO_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_MOVE_INFO, 1, &MOVE_INFO_RECEIVER_C::ReceiveMsgCallBack, this);

        objDataValidTimer_.SetOverTime(0.15);

        stMoveInfo_.time                    = 0.0;
        stMoveInfo_.stInfo.leftWheelVel     = 0.0;
        stMoveInfo_.stInfo.rightWheelVel    = 0.0;
        stMoveInfo_.stInfo.moveLinearVel    = 0.0;
        stMoveInfo_.stInfo.yawAngle         = 0.0;
        stMoveInfo_.stInfo.yawAngleVel      = 0.0;
        stMoveInfo_.stInfo.steeringAngle    = 0.0;
        stMoveInfo_.stInfo.accelerationX    = 0.0;
        stMoveInfo_.stInfo.accelerationY    = 0.0;
        stMoveInfo_.stInfo.accelerationZ    = 0.0;
    }

    BOOL  IsPrepareReady(void) const
    {
        return isPrepareReady_;
    }

    INT32 Get(DCU_MOVE_INFO_EX_STRU &stMoveInfo) const
    {
        stMoveInfo = stMoveInfo_;

        return objDataValidTimer_.IsTimeOut() ? -1 : 0;
    }

private:
    ros::Subscriber         objSubscriber_;
    TIMER_C                 objDataValidTimer_;
    BOOL                    isPrepareReady_{false};
    DCU_MOVE_INFO_EX_STRU   stMoveInfo_;

    void ReceiveMsgCallBack(const comm_msg::move_info::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        isPrepareReady_ = true;

        objDataValidTimer_.Reset();

        stMoveInfo_.time                    = pstMsg->header.stamp.toSec();
        stMoveInfo_.stInfo.leftWheelVel     = pstMsg->moveLinearVel;
        stMoveInfo_.stInfo.rightWheelVel    = pstMsg->moveLinearVel;
        stMoveInfo_.stInfo.moveLinearVel    = pstMsg->moveLinearVel;
        stMoveInfo_.stInfo.yawAngle         = pstMsg->yawAngle;
        stMoveInfo_.stInfo.yawAngleVel      = pstMsg->yawAngleVel;
        stMoveInfo_.stInfo.steeringAngle    = pstMsg->steeringAngle;
        stMoveInfo_.stInfo.accelerationX    = pstMsg->accX;
        stMoveInfo_.stInfo.accelerationY    = pstMsg->accY;
        stMoveInfo_.stInfo.accelerationZ    = pstMsg->accZ;
    }
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class DCU_MOVE_CTRL_SENDER_C
{
public:
    DCU_MOVE_CTRL_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::move_info>(TOPIC_NAME_DCU_MOVE_CTRL, 1);
    }

    void Send(const DCU_MOVE_INFO_STRU &stMoveInfo)
    {
        comm_msg::move_info stMsg;

        stMsg.header.stamp  = ros::Time::now();

        stMsg.steeringAngle = stMoveInfo.steeringAngle;
        stMsg.moveLinearVel = stMoveInfo.moveLinearVel;
        stMsg.yawAngle      = stMoveInfo.yawAngle;
        stMsg.yawAngleVel   = stMoveInfo.yawAngleVel;
        stMsg.accX          = stMoveInfo.accelerationX;
        stMsg.accY          = stMoveInfo.accelerationY;
        stMsg.accZ          = stMoveInfo.accelerationZ;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class DCU_MOVE_CTRL_RECEIVER_C
{
public:
    DCU_MOVE_CTRL_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_DCU_MOVE_CTRL, 1, &DCU_MOVE_CTRL_RECEIVER_C::ReceiveMsgCallBack, this);

        objDataValidTimer_.SetOverTime(0.15);

        stMoveInfo_.time                    = 0.0;
        stMoveInfo_.stInfo.leftWheelVel     = 0.0;
        stMoveInfo_.stInfo.rightWheelVel    = 0.0;
        stMoveInfo_.stInfo.moveLinearVel    = 0.0;
        stMoveInfo_.stInfo.yawAngle         = 0.0;
        stMoveInfo_.stInfo.yawAngleVel      = 0.0;
        stMoveInfo_.stInfo.steeringAngle    = 0.0;
        stMoveInfo_.stInfo.accelerationX    = 0.0;
        stMoveInfo_.stInfo.accelerationY    = 0.0;
        stMoveInfo_.stInfo.accelerationZ    = 0.0;
    }

    BOOL  IsPrepareReady(void) const
    {
        return isPrepareReady_;
    }

    INT32 Get(DCU_MOVE_INFO_EX_STRU &stMoveInfo) const
    {
        stMoveInfo = stMoveInfo_;

        return objDataValidTimer_.IsTimeOut() ? -1 : 0;
    }

private:
    ros::Subscriber         objSubscriber_;
    TIMER_C                 objDataValidTimer_;
    BOOL                    isPrepareReady_{false};
    DCU_MOVE_INFO_EX_STRU   stMoveInfo_;

    void ReceiveMsgCallBack(const comm_msg::move_info::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        isPrepareReady_ = true;

        objDataValidTimer_.Reset();

        stMoveInfo_.time                    = pstMsg->header.stamp.toSec();
        stMoveInfo_.stInfo.leftWheelVel     = pstMsg->moveLinearVel;
        stMoveInfo_.stInfo.rightWheelVel    = pstMsg->moveLinearVel;
        stMoveInfo_.stInfo.moveLinearVel    = pstMsg->moveLinearVel;
        stMoveInfo_.stInfo.yawAngle         = pstMsg->yawAngle;
        stMoveInfo_.stInfo.yawAngleVel      = pstMsg->yawAngleVel;
        stMoveInfo_.stInfo.steeringAngle    = pstMsg->steeringAngle;
        stMoveInfo_.stInfo.accelerationX    = pstMsg->accX;
        stMoveInfo_.stInfo.accelerationY    = pstMsg->accY;
        stMoveInfo_.stInfo.accelerationZ    = pstMsg->accZ;
    }
};

#endif

