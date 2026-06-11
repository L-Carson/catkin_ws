

#ifndef __COMM_MSG_MOVE_CTRL_H__
#define __COMM_MSG_MOVE_CTRL_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/move_ctrl.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;


/**************************************************************************************
功能描述: 规划模式，有贴边、跟线等，该枚举必须和move_ctrl.msg里面的planningModel保持一致
修改记录:
**************************************************************************************/
enum PLANNING_MODEL_ENUM {
    PLANNING_MODEL_NONE         = 0,        // 无效
    PLANNING_MODEL_CLEAR_EDGE   = 1,        // 贴边模式
    PLANNING_MODEL_FOLLOW_ROUTE = 2,        // 跟线模式
    PLANNING_MODEL_POINT_REMOTE_CTRL = 3,   // 发点远程遥控模式
    PLANNING_MODEL_ANGLE_REMOTE_CTRL = 4,   // 方向远程遥控模式
    PLANNING_MODEL_DEBUG_TOOL_CTRL   = 5,   // 调试工具控制模式
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class MOVE_CTRL_SENDER_C
{
public:
    MOVE_CTRL_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::move_ctrl>(TOPIC_NAME_MOVE_CTRL, 1);
    }

    void Send(BOOL isExistTask, BOOL isTeleCtrl, BOOL isCleaning, 
              const MOVE_CTRL_INFO_STRU &stMoveCtrl, 
              const MOVE_CTRL_SMOOTH_INFO_STRU &stCtrlSmooth,
              const MOVE_CTRL_SAFE_INFO_STRU &stCtrlSafe)
    {
        comm_msg::move_ctrl stMsg;

        stMsg.header.stamp  = ros::Time::now();
        stMsg.isExistTask   = isExistTask;
        stMsg.isTeleCtrl    = isTeleCtrl;
        stMsg.isCleaning    = isCleaning;
        stMsg.steeringAngle = stMoveCtrl.turnAngle;
        stMsg.moveLinearVel = stMoveCtrl.linearVel;
        stMsg.isMoveSmooth  = stCtrlSmooth.isSmooth;
        stMsg.isStopSmooth  = stCtrlSmooth.isSmoothStop;
        stMsg.isUseSecurityRedundancy = stCtrlSafe.isUseSecurityRedundancy;
        stMsg.isIgnoreLaserData       = stCtrlSafe.isIgnoreLaserData;
        stMsg.isIgnoreUltrasonicData  = stCtrlSafe.isIgnoreUltrasonicData;
        stMsg.planningModel           = stCtrlSafe.planningModel;
        stMsg.brushModel              = stCtrlSafe.brushModel;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class MOVE_CTRL_RECEIVER_C
{
public:
    MOVE_CTRL_RECEIVER_C(ros::NodeHandle &nh)
    {
        subscriber_ = nh.subscribe(TOPIC_NAME_MOVE_CTRL, 1, &MOVE_CTRL_RECEIVER_C::ReceiveMsgCallBack, this);

        objInvalidTimer_.SetOverTime(0.25);
        isExistTask_               = false;
        isTeleCtrl_                = false;
        isCleaning_                = false;
        stMoveCtrl_.turnAngle      = 0.0;
        stMoveCtrl_.linearVel      = 0.0;
        stCtrlSmooth_.isSmooth     = false;
        stCtrlSmooth_.isSmoothStop = false;
        stCtrlSafe_.Reset();

    }

    INT32 Get(BOOL &isExistTask, BOOL &isTeleCtrl, BOOL &isCleaning, MOVE_CTRL_INFO_STRU &stMoveCtrl, 
              MOVE_CTRL_SMOOTH_INFO_STRU &stCtrlSmooth, MOVE_CTRL_SAFE_INFO_STRU &stCtrlSafe) const
    {
        isExistTask     = isExistTask_;
        isTeleCtrl      = isTeleCtrl_;
        isCleaning      = isCleaning_;
        stMoveCtrl      = stMoveCtrl_;
        stCtrlSmooth    = stCtrlSmooth_;
        stCtrlSafe      = stCtrlSafe_;

        return objInvalidTimer_.IsTimeOut() ? -1 : 0;
    }

private:
    ros::Subscriber             subscriber_;

    TIMER_C                     objInvalidTimer_;
    BOOL                        isExistTask_;
    BOOL                        isTeleCtrl_;
    BOOL                        isCleaning_;
    MOVE_CTRL_INFO_STRU         stMoveCtrl_;
    MOVE_CTRL_SMOOTH_INFO_STRU  stCtrlSmooth_;
    MOVE_CTRL_SAFE_INFO_STRU    stCtrlSafe_;
    ros::Time                   lastT_;

    void ReceiveMsgCallBack(const comm_msg::move_ctrl::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        ros::Time nowT = ros::Time::now();
        ros::Duration d(nowT - lastT_);
        float intervalMs = d.toSec();
        ST_LOG_INFO_IF(intervalMs > 0.14, "接收速度花费时间s: %.3f, last接收s:%.3f, now接收s:%.3f", 
                       intervalMs, lastT_.toSec(), nowT.toSec());
        ST_LOG_INFO_IF(intervalMs > 0.14, "速度接收-发布花费时间s:%.3f, 发布时间戳s:%.3f", 
                       nowT.toSec() - pstMsg->header.stamp.toSec(), pstMsg->header.stamp.toSec());
        lastT_ = nowT;
        objInvalidTimer_.Reset();

        isExistTask_               = pstMsg->isExistTask;
        isTeleCtrl_                = pstMsg->isTeleCtrl;
        isCleaning_                = pstMsg->isCleaning;
        stMoveCtrl_.turnAngle      = pstMsg->steeringAngle;
        stMoveCtrl_.linearVel      = pstMsg->moveLinearVel;
        stCtrlSmooth_.isSmooth     = pstMsg->isMoveSmooth;
        stCtrlSmooth_.isSmoothStop = pstMsg->isStopSmooth;
        stCtrlSafe_.isUseSecurityRedundancy = pstMsg->isUseSecurityRedundancy;
        stCtrlSafe_.isIgnoreLaserData       = pstMsg->isIgnoreLaserData;
        stCtrlSafe_.isIgnoreUltrasonicData  = pstMsg->isIgnoreUltrasonicData;
        stCtrlSafe_.planningModel           = pstMsg->planningModel;
        stCtrlSafe_.brushModel              = pstMsg->brushModel;
    }
};


#endif

