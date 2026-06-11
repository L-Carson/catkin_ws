#ifndef __COMM_MSG_DEV_INFO_H__
#define __COMM_MSG_DEV_INFO_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm/comm_msg.h"
#include <comm_msg/dev_info.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**
 * @brief 设备信息结构
 *
 */
struct DCU_MOVE_DEV_INFO_STRU
{
    UINT08  chargerMotorState;    // 充电电机状态
    UINT32  chargerMotorTravel;   //

    UINT08  liftMotorState;       // 举升电机状态
    UINT08  rollMotorState;       // 翻滚电机状态
    UINT16  leftLiftMotorTravel;  //
    UINT16  rightLiftMotorTravel; //
    UINT16  leftRollMotorTravel;  //
    UINT16  rightRollMotorTravel; //
    float   angleSensorOne;       //
    float   angleSensorTwo;       //

    BOOL    isStationEmergeStop;
    BOOL    isManualChargeMode;
    BOOL    proximitySw;
    BOOL    chargeSw;
    BOOL    waterSw;
    BOOL    isStationLockSw;
    BOOL    chargerTravelSw;
    BOOL    leftTravelSw;
    BOOL    rightTravelSw;

    float   trashBinCoverOpenAngle;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class DEV_INFO_SENDER_C
{
public:
    DEV_INFO_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::dev_info>(TOPIC_NAME_DEV_INFO, 1);
    }

    void Send(const DCU_MOVE_DEV_INFO_STRU &stDevInfo)
    {
        comm_msg::dev_info stMsg;

        stMsg.chargerMotorState      = stDevInfo.chargerMotorState;
        stMsg.chargerMotorTravel     = stDevInfo.chargerMotorTravel;
        stMsg.liftMotorState         = stDevInfo.liftMotorState;
        stMsg.rollMotorState         = stDevInfo.rollMotorState;
        stMsg.leftLiftMotorTravel    = stDevInfo.leftLiftMotorTravel;
        stMsg.rightLiftMotorTravel   = stDevInfo.rightLiftMotorTravel;
        stMsg.leftRollMotorTravel    = stDevInfo.leftRollMotorTravel;
        stMsg.rightRollMotorTravel   = stDevInfo.rightRollMotorTravel;
        stMsg.angleSensorOne         = stDevInfo.angleSensorOne;
        stMsg.angleSensorTwo         = stDevInfo.angleSensorTwo;
        stMsg.isStationEmergeStop    = stDevInfo.isStationEmergeStop;
        stMsg.isManualChargeMode     = stDevInfo.isManualChargeMode;
        stMsg.proximitySw            = stDevInfo.proximitySw;
        stMsg.chargeSw               = stDevInfo.chargeSw;
        stMsg.waterSw                = stDevInfo.waterSw;
        stMsg.stationLockSw          = stDevInfo.isStationLockSw;
        stMsg.chargerTravelSw        = stDevInfo.chargerTravelSw;
        stMsg.leftTravelSw           = stDevInfo.leftTravelSw;
        stMsg.rightTravelSw          = stDevInfo.rightTravelSw;
        stMsg.trashBinCoverOpenAngle = stDevInfo.trashBinCoverOpenAngle;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 设备信息接收器
修改记录:
**************************************************************************************/
class DEV_INFO_RECEIVER_C
{
public:
    DEV_INFO_RECEIVER_C(ros::NodeHandle &nh, double overTime = 0.5)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_DEV_INFO, 1, &DEV_INFO_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(DCU_MOVE_DEV_INFO_STRU &stDevInfo) const {
        stDevInfo = stDevInfo_;
        return objInvalidTimer_.IsTimeOut() ? -1 : 0;
    }

    const DCU_MOVE_DEV_INFO_STRU *Get(void) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return nullptr;
        }

        return &stDevInfo_;
    }

private:
    ros::Subscriber  receiver_;

    TIMER_C                 objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    DCU_MOVE_DEV_INFO_STRU  stDevInfo_;

    void ReceiveMsgCallBack(const comm_msg::dev_info::ConstPtr &pstMsg)
    {
        objInvalidTimer_.Reset();

        stDevInfo_.chargerMotorState      = pstMsg->chargerMotorState;
        stDevInfo_.chargerMotorTravel     = pstMsg->chargerMotorTravel;
        stDevInfo_.liftMotorState         = pstMsg->liftMotorState;
        stDevInfo_.rollMotorState         = pstMsg->rollMotorState;
        stDevInfo_.leftLiftMotorTravel    = pstMsg->leftLiftMotorTravel;
        stDevInfo_.rightLiftMotorTravel   = pstMsg->rightLiftMotorTravel;
        stDevInfo_.leftRollMotorTravel    = pstMsg->leftRollMotorTravel;
        stDevInfo_.rightRollMotorTravel   = pstMsg->rightRollMotorTravel;
        stDevInfo_.angleSensorOne         = pstMsg->angleSensorOne;
        stDevInfo_.angleSensorTwo         = pstMsg->angleSensorTwo;
        stDevInfo_.isStationEmergeStop    = pstMsg->isStationEmergeStop;
        stDevInfo_.isManualChargeMode     = pstMsg->isManualChargeMode;
        stDevInfo_.proximitySw            = pstMsg->proximitySw;
        stDevInfo_.chargeSw               = pstMsg->chargeSw;
        stDevInfo_.waterSw                = pstMsg->waterSw;
        stDevInfo_.isStationLockSw        = pstMsg->stationLockSw;
        stDevInfo_.chargerTravelSw        = pstMsg->chargerTravelSw;
        stDevInfo_.leftTravelSw           = pstMsg->leftTravelSw;
        stDevInfo_.rightTravelSw          = pstMsg->rightTravelSw;
        stDevInfo_.trashBinCoverOpenAngle = pstMsg->trashBinCoverOpenAngle;
    }
};

#endif

