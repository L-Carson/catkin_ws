#ifndef __COMM_MSG_DCU_INFO_H__
#define __COMM_MSG_DCU_INFO_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm/comm_msg.h"
#include <comm_msg/dcu_info.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

struct DCU_DEV_INFO_STRU {
    UINT08 batQuantity;             /* 电量，百分比的值 */
    INT08  batTemp;
    float  batVol;

    bool   isAutoMode;
    bool   isRemoteControl;
    bool   isStopByFrontEdgeContact;
    bool   isStopByBackEdgeContact;
    bool   isStopByEmergeSw;

    bool   isRaining;
    bool   isWaterEnough;
    bool   isRubbishFull;

    bool   isUltraFault;
    bool   isGpsFault;

    bool   isMoveMotorFault;
    bool   isTurnMotorFault;
    bool   isBrakeSysFault;

    bool   isCleanBrushFault;
    bool   isCleanFanFault;
    bool   isCleanShakeFault;

    bool   isGarbageCapacityAbn;   /* 垃圾容量异常 */
};

struct DCU_STATE_STRU {
    bool   isDcuFault;                  /* 判断通信是否正常，如果不正常则认为故障 */
};

struct DCU_BRUSH_INFO_STRU {
    UINT08 leftStretchState       = MOTOR_STATE_NO_EXISIT;  /* 前刷左伸缩电机状态 */
    UINT08 rightStretchState      = MOTOR_STATE_NO_EXISIT;  /* 前刷右伸缩电机状态 */
    UINT08 backElevatorState      = MOTOR_STATE_NO_EXISIT;  /* 后刷升降电机状态 */
    UINT08 frontElevatorState     = MOTOR_STATE_NO_EXISIT;  /* 前刷升降电机状态 */
    UINT08 baffleState            = MOTOR_STATE_NO_EXISIT;  /* 主刷格挡板电机状态 */
    UINT08 rollBrushElevatorState = MOTOR_STATE_NO_EXISIT;  /* 滚刷升降电机状态 */
    bool   isFrontBrushOpen = false;  // 前边刷是否转动
    bool   isBackBrushOpen = false;   // 后边刷是否转动
    bool   isRollBrushOpen = false;   // 滚刷是否转动
};

struct DCU_TRASH_PACK_INFO_STRU
{
    UINT08 trashBinCoverState;              /* 垃圾打包结构中，垃圾斗盖状态， 详见                  DCU_TRASH_BIN_COVER_STATE_ENUM */
    UINT08 trashBagClampState;              /* 垃圾打包结构中，垃圾袋夹状态，详见                  DCU_TRASH_BAG_CLAMP_STATE_ENUM */
    UINT08 trashBagSubterreneState;         /* 垃圾打包结构中，垃圾袋热熔状态，详见                   DCU_TRASH_BAG_SUBTERRENE_STATE_ENUM */
    UINT08 trashBagDrawState;               /* 垃圾打包结构中，垃圾袋吸取状态，详见                   DCU_TRASH_BAG_DRAW_STATE_ENUM */
    BOOL   isTrashBagExist;                 /* 垃圾打包结构中，是否存在垃圾袋 */
    BOOL   isTrashBagThrownOut;             /* 垃圾打包结构中，垃圾袋是否掉落 */
    float  ultraDistToTrashBag;             // 垃圾打包结构中，垃圾盖上的超声波检测障碍距离，判断垃圾袋是否抛出
};


struct DCU_CHARGING_INFO_STRU
{
    UINT08 chargingDoorState;         /* 充电舱门状态位， 详见 DCU_CHARGE_DOOR_STATE_ENUM */
};

struct DCU_INFO_STRU {
    DCU_DEV_INFO_STRU        stDevInfo;
    DCU_STATE_STRU           stDcuState;
    DCU_BRUSH_INFO_STRU      stBrushInfo;
    DCU_TRASH_PACK_INFO_STRU stTrashPackInfo;
    DCU_CHARGING_INFO_STRU   stChargingInfo;
    
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class DCU_INFO_SENDER_C
{
public:
    DCU_INFO_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::dcu_info>(TOPIC_NAME_DCU_INFO, 1);
    }

    void Send(const DCU_INFO_STRU &stDcuInfo)
    {
        comm_msg::dcu_info stMsg;

        stMsg.batteryPowerLeftRate     = stDcuInfo.stDevInfo.batQuantity;
        stMsg.batteryTemperature       = stDcuInfo.stDevInfo.batTemp;
        stMsg.batteryVoltage           = stDcuInfo.stDevInfo.batVol;

        stMsg.isAutoMode               = stDcuInfo.stDevInfo.isAutoMode;
        stMsg.isRemoteControl          = stDcuInfo.stDevInfo.isRemoteControl;
        stMsg.isStopByFrontEdgeContact = stDcuInfo.stDevInfo.isStopByFrontEdgeContact;
        stMsg.isStopByBackEdgeContact  = stDcuInfo.stDevInfo.isStopByBackEdgeContact;
        stMsg.isStopByEmergeSw         = stDcuInfo.stDevInfo.isStopByEmergeSw;

        stMsg.isRaining                = stDcuInfo.stDevInfo.isRaining;
        stMsg.isWaterEnough            = stDcuInfo.stDevInfo.isWaterEnough;
        stMsg.isRubbishFull            = stDcuInfo.stDevInfo.isRubbishFull;

        stMsg.isUltraFault             = stDcuInfo.stDevInfo.isUltraFault;
        stMsg.isGpsFault               = stDcuInfo.stDevInfo.isGpsFault;
        stMsg.isMoveMotorFault         = stDcuInfo.stDevInfo.isMoveMotorFault;
        stMsg.isTurnMotorFault         = stDcuInfo.stDevInfo.isTurnMotorFault;
        stMsg.isBrakeSysFault          = stDcuInfo.stDevInfo.isBrakeSysFault;
        stMsg.isBrushSysFault          = stDcuInfo.stDevInfo.isCleanBrushFault;
        stMsg.isFanSysFault            = stDcuInfo.stDevInfo.isCleanFanFault;
        stMsg.isShakeSysFault          = stDcuInfo.stDevInfo.isCleanShakeFault;

        stMsg.isDcuFault               = stDcuInfo.stDcuState.isDcuFault;

        stMsg.leftStretchState         = stDcuInfo.stBrushInfo.leftStretchState;
        stMsg.rightStretchState        = stDcuInfo.stBrushInfo.rightStretchState;
        stMsg.backElevatorState        = stDcuInfo.stBrushInfo.backElevatorState;
        stMsg.frontElevatorState       = stDcuInfo.stBrushInfo.frontElevatorState;
        stMsg.baffleState              = stDcuInfo.stBrushInfo.baffleState;
        stMsg.rollBrushElevatorState   = stDcuInfo.stBrushInfo.rollBrushElevatorState;
        stMsg.isFrontBrushOpen         = stDcuInfo.stBrushInfo.isFrontBrushOpen;
        stMsg.isBackBrushOpen          = stDcuInfo.stBrushInfo.isBackBrushOpen;
        stMsg.isRollBrushOpen          = stDcuInfo.stBrushInfo.isRollBrushOpen;

        stMsg.trashBinCoverState       = stDcuInfo.stTrashPackInfo.trashBinCoverState;
        stMsg.trashBagClampState       = stDcuInfo.stTrashPackInfo.trashBagClampState;
        stMsg.trashBagSubterreneState  = stDcuInfo.stTrashPackInfo.trashBagSubterreneState;
        stMsg.trashBagDrawState        = stDcuInfo.stTrashPackInfo.trashBagDrawState;
        stMsg.isTrashBagExist          = stDcuInfo.stTrashPackInfo.isTrashBagExist;
        stMsg.isTrashBagThrownOut      = stDcuInfo.stTrashPackInfo.isTrashBagThrownOut;
        stMsg.ultraDistToTrashBag      = stDcuInfo.stTrashPackInfo.ultraDistToTrashBag;
        stMsg.chargingDoorState        = stDcuInfo.stChargingInfo.chargingDoorState;
        stMsg.isGarbageCapacityAbn     = stDcuInfo.stDevInfo.isGarbageCapacityAbn;

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher     objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class DCU_INFO_RECEIVER_C
{
public:
    using Ptr = std::shared_ptr<DCU_INFO_RECEIVER_C>;
    using ConstPtr = std::shared_ptr<const DCU_INFO_RECEIVER_C>;

public:
    DCU_INFO_RECEIVER_C(ros::NodeHandle &nh, double overTime = 2.5) : objDataValidTimer_(overTime)
    {
        objReceiver_ = nh.subscribe(TOPIC_NAME_DCU_INFO, 1, &DCU_INFO_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(DCU_INFO_STRU &stDcuInfo) const
    {
        stDcuInfo = stDcuInfo_;
        return objDataValidTimer_.IsTimeOut() ? -1 : 0;
    }

    const DCU_INFO_STRU *Get(void) const
    {
        if (objDataValidTimer_.IsTimeOut()) {
            return NULL;
        }

        return &stDcuInfo_;
    }

private:
    ros::Subscriber  objReceiver_;
    TIMER_C          objDataValidTimer_;       /* 数据失效定时器，超时后数据无效 */
    DCU_INFO_STRU    stDcuInfo_{{0}};

    void ReceiveMsgCallBack(const comm_msg::dcu_info::ConstPtr &pstMsg)
    {
        objDataValidTimer_.Reset();

        stDcuInfo_.stDevInfo.batQuantity                = pstMsg->batteryPowerLeftRate;
        stDcuInfo_.stDevInfo.batTemp                    = pstMsg->batteryTemperature;
        stDcuInfo_.stDevInfo.batVol                     = pstMsg->batteryVoltage;

        stDcuInfo_.stDevInfo.isAutoMode                 = pstMsg->isAutoMode;
        stDcuInfo_.stDevInfo.isRemoteControl            = pstMsg->isRemoteControl;
        stDcuInfo_.stDevInfo.isStopByFrontEdgeContact   = pstMsg->isStopByFrontEdgeContact;
        stDcuInfo_.stDevInfo.isStopByBackEdgeContact    = pstMsg->isStopByBackEdgeContact;
        stDcuInfo_.stDevInfo.isStopByEmergeSw           = pstMsg->isStopByEmergeSw;

        stDcuInfo_.stDevInfo.isRaining                  = pstMsg->isRaining;
        stDcuInfo_.stDevInfo.isWaterEnough              = pstMsg->isWaterEnough;
        stDcuInfo_.stDevInfo.isRubbishFull              = pstMsg->isRubbishFull;

        stDcuInfo_.stDevInfo.isUltraFault               = pstMsg->isUltraFault;
        stDcuInfo_.stDevInfo.isGpsFault                 = pstMsg->isGpsFault;
        stDcuInfo_.stDevInfo.isMoveMotorFault           = pstMsg->isMoveMotorFault;
        stDcuInfo_.stDevInfo.isTurnMotorFault           = pstMsg->isTurnMotorFault;
        stDcuInfo_.stDevInfo.isBrakeSysFault            = pstMsg->isBrakeSysFault;
        stDcuInfo_.stDevInfo.isCleanBrushFault          = pstMsg->isBrushSysFault;
        stDcuInfo_.stDevInfo.isCleanFanFault            = pstMsg->isFanSysFault;
        stDcuInfo_.stDevInfo.isCleanShakeFault          = pstMsg->isShakeSysFault;
        stDcuInfo_.stDevInfo.isGarbageCapacityAbn       = pstMsg->isGarbageCapacityAbn;
        stDcuInfo_.stDcuState.isDcuFault                = pstMsg->isDcuFault;

        stDcuInfo_.stBrushInfo.leftStretchState         = pstMsg->leftStretchState;
        stDcuInfo_.stBrushInfo.rightStretchState        = pstMsg->rightStretchState;
        stDcuInfo_.stBrushInfo.backElevatorState        = pstMsg->backElevatorState;
        stDcuInfo_.stBrushInfo.frontElevatorState       = pstMsg->frontElevatorState;
        stDcuInfo_.stBrushInfo.baffleState              = pstMsg->baffleState;
        stDcuInfo_.stBrushInfo.rollBrushElevatorState   = pstMsg->rollBrushElevatorState;
        stDcuInfo_.stBrushInfo.isFrontBrushOpen         = pstMsg->isFrontBrushOpen;
        stDcuInfo_.stBrushInfo.isBackBrushOpen          = pstMsg->isBackBrushOpen;
        stDcuInfo_.stBrushInfo.isRollBrushOpen          = pstMsg->isRollBrushOpen;

        stDcuInfo_.stTrashPackInfo.trashBinCoverState      = pstMsg->trashBinCoverState;
        stDcuInfo_.stTrashPackInfo.trashBagClampState      = pstMsg->trashBagClampState;
        stDcuInfo_.stTrashPackInfo.trashBagSubterreneState = pstMsg->trashBagSubterreneState;
        stDcuInfo_.stTrashPackInfo.trashBagDrawState       = pstMsg->trashBagDrawState;
        stDcuInfo_.stTrashPackInfo.isTrashBagExist         = pstMsg->isTrashBagExist;
        stDcuInfo_.stTrashPackInfo.isTrashBagThrownOut     = pstMsg->isTrashBagThrownOut;
        stDcuInfo_.stTrashPackInfo.ultraDistToTrashBag     = pstMsg->ultraDistToTrashBag;
        stDcuInfo_.stChargingInfo.chargingDoorState        = pstMsg->chargingDoorState;
    }
};


#endif

