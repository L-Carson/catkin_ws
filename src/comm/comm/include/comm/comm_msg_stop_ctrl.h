#ifndef __COMM_MSG_STOP_CTRL_H__
#define __COMM_MSG_STOP_CTRL_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"

#include <comm_msg/stop_ctrl.h>
#include <ros/ros.h>

/**************************************************************************************
功能描述: 发送
修改记录:
**************************************************************************************/
class STOP_CTRL_SENDER_C
{
public:
    STOP_CTRL_SENDER_C(ros::NodeHandle &nh)
    {
        objPub_ = nh.advertise<comm_msg::stop_ctrl>(TOPIC_NAME_REMOTE_STOP_CTRL, 1);
    }

    void Send(BOOL isAct)
    {
        if (isStopBefore_ != isAct) {
            ST_LOG_INFO("Send New Stop State: %u.", isAct);
            isStopBefore_ = isAct;
        }

        comm_msg::stop_ctrl stMsg;
        stMsg.isAct = (UINT08)isAct;
        objPub_.publish(stMsg);
    }

private:
    ros::Publisher objPub_;
    BOOL           isStopBefore_{false};
};

/**************************************************************************************
功能描述: 接收
修改记录:
**************************************************************************************/
class STOP_CTRL_RECEIVER_C
{
public:
    STOP_CTRL_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSub_ = nh.subscribe(TOPIC_NAME_REMOTE_STOP_CTRL, 1, &STOP_CTRL_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    BOOL IsStop(void) const
    {
        return isAct_;
    }

private:
    ros::Subscriber objSub_;
    BOOL            isAct_{false};

    void ReceiveMsgCallBack(const comm_msg::stop_ctrl::ConstPtr &pstMsg)
    {
        if (BOOL(pstMsg->isAct) != isAct_) {
            ST_LOG_INFO("Is Act Stop: %u.", pstMsg->isAct);
            isAct_ = BOOL(pstMsg->isAct);
        }
    }
};



#endif

