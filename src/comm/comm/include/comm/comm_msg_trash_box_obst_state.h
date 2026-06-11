#ifndef __COMM_MSG_TRASH_BOX_OBST_STATE_H__
#define __COMM_MSG_TRASH_BOX_OBST_STATE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"

#include <comm_msg/trash_box_obst_state.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 发布垃圾箱升降过程障碍状态消息
修改记录:
**************************************************************************************/
class TRASH_BOX_OBST_STATE_SENDER_C
{
public:
    TRASH_BOX_OBST_STATE_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::trash_box_obst_state>(TOPIC_NAME_TRASH_BOX_OBST_STATE, 1);
    }

    void Send(BOOL exisitObstUp, BOOL exisitObstDown)
    {
        comm_msg::trash_box_obst_state msg;
        msg.exisitObstUp   = exisitObstUp;
        msg.exisitObstDown = exisitObstDown;
        publisher_.publish(msg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 发布清扫状态消息
修改记录:
**************************************************************************************/
class TRASH_BOX_OBST_STATE_RECEIVER_C
{
public:
    TRASH_BOX_OBST_STATE_RECEIVER_C(ros::NodeHandle &nh)
    {
        exisitObstUp_   = true;
        exisitObstDown_ = true;

        objDataValidTimer_.SetOverTime(0.3);
        receiver_ = nh.subscribe(TOPIC_NAME_TRASH_BOX_OBST_STATE, 1, &TRASH_BOX_OBST_STATE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    BOOL IsExisitObstUp()
    {
        if (objDataValidTimer_.IsTimeOut()) {
            return true;
        }

        return exisitObstUp_;
    }

    BOOL IsExisitObstDown()
    {
        if (objDataValidTimer_.IsTimeOut()) {
            return true;
        }

        return exisitObstDown_;
    }

private:
    void ReceiveMsgCallBack(const comm_msg::trash_box_obst_state::ConstPtr &pstMsg)
    {
        objDataValidTimer_.Reset();

        exisitObstUp_   = pstMsg->exisitObstUp;
        exisitObstDown_ = pstMsg->exisitObstDown;
    }

    ros::Subscriber  receiver_;
    TIMER_C          objDataValidTimer_;

    BOOL exisitObstUp_;
    BOOL exisitObstDown_;
};

#endif


