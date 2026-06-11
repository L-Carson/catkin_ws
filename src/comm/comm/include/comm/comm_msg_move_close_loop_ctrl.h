#ifndef __COMM_MSG_MOVE_CLOSE_LOOP_CTRL_H__
#define __COMM_MSG_MOVE_CLOSE_LOOP_CTRL_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/move_close_loop_ctrl.h>
#include <comm_msg/move_info.h>

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class MOVE_CLOSE_LOOP_CTRL_SHOWER_C
{
public:
    MOVE_CLOSE_LOOP_CTRL_SHOWER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        objPublisher_ = nh.advertise<comm_msg::move_close_loop_ctrl>(strTopicName.c_str(), 1);
    }

    void Show(const MOVE_CTRL_INFO_STRU &stPlannedCtrl, const MOVE_CTRL_INFO_STRU &stOutputCtrl, float measureSteeringAngle, float measureMoveLinearVel)
    {
        comm_msg::move_close_loop_ctrl stMsg;

        stMsg.plannedSteeringAngle = stPlannedCtrl.turnAngle;
        stMsg.plannedMoveLinearVel = stPlannedCtrl.linearVel;
        stMsg.controlSteeringAngle = stOutputCtrl.turnAngle;
        stMsg.controlMoveLinearVel = stOutputCtrl.linearVel;
        stMsg.measureSteeringAngle = measureSteeringAngle;
        stMsg.measureMoveLinearVel = measureMoveLinearVel;

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

#endif

