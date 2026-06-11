#ifndef __COMM_MSG_TRASH_BOX_AREA_OBST_STATE_H__
#define __COMM_MSG_TRASH_BOX_AREA_OBST_STATE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"

#include <comm_msg/trash_box_area_obst_state.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 发布垃圾箱后方区域障碍状态消息
修改记录:
**************************************************************************************/
class TRASH_BOX_AREA_OBST_STATE_SENDER_C
{
public:
    TRASH_BOX_AREA_OBST_STATE_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::trash_box_area_obst_state>(TOPIC_NAME_TRASH_BOX_AREA_OBST_STATE, 1);
    }

    void Send(BOOL isExistHighObstInBoxArea, BOOL isExistHighObstOutBoxArea)
    {
        comm_msg::trash_box_area_obst_state msg;
        msg.isExistHighObstInBoxArea  = isExistHighObstInBoxArea;
        msg.isExistHighObstOutBoxArea = isExistHighObstOutBoxArea;
        publisher_.publish(msg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 发布清扫状态消息
修改记录:
**************************************************************************************/
class TRASH_BOX_AREA_OBST_STATE_RECEIVER_C
{
public:
    TRASH_BOX_AREA_OBST_STATE_RECEIVER_C(ros::NodeHandle &nh)
    {
        isExistHighObstInBoxArea_   = true;
        isExistHighObstOutBoxArea_  = false;

        objDataValidTimer_.SetOverTime(0.3);
        receiver_ = nh.subscribe(TOPIC_NAME_TRASH_BOX_AREA_OBST_STATE, 1, &TRASH_BOX_AREA_OBST_STATE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    BOOL IsExistHighObstInBoxArea()
    {
        if (objDataValidTimer_.IsTimeOut()) {
            return true;
        }

        return isExistHighObstInBoxArea_;
    }

    BOOL IsExistHighObstOutBoxArea()
    {
        if (objDataValidTimer_.IsTimeOut()) {
            return true;
        }

        return isExistHighObstOutBoxArea_;
    }

private:
    void ReceiveMsgCallBack(const comm_msg::trash_box_area_obst_state::ConstPtr &pstMsg)
    {
        objDataValidTimer_.Reset();

        isExistHighObstInBoxArea_  = pstMsg->isExistHighObstInBoxArea;
        isExistHighObstOutBoxArea_ = pstMsg->isExistHighObstOutBoxArea;
    }

    ros::Subscriber  receiver_;
    TIMER_C          objDataValidTimer_;

    BOOL isExistHighObstInBoxArea_;
    BOOL isExistHighObstOutBoxArea_;
};

#endif


