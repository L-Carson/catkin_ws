#ifndef __COMM_MSG_DUSTBIN_STATE_H__
#define __COMM_MSG_DUSTBIN_STATE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/dustbinState.h>
#include <ros/ros.h>

/**************************************************************************************
功能描述: 发布垃圾箱状态消息
修改记录:
**************************************************************************************/
class DUSTBIN_STATE_SENDER_C
{
public:
    DUSTBIN_STATE_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::dustbinState>(TOPIC_NAME_DUSTBIN_STATE, 1);
    }

    void SendDustbinState(const BOOL &isFull)
    {
        comm_msg::dustbinState stMsg;
        stMsg.isFull = isFull;

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收垃圾箱状态消息
修改记录:
**************************************************************************************/
class DUSTBIN_STATE_RECEIVER_C
{
public:
    DUSTBIN_STATE_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_DUSTBIN_STATE, 1, &DUSTBIN_STATE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 GetDustbinState(BOOL &isDustbinFull)
    {
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            return -1;
        }

        isDustbinFull = isFull_;
        return 0;
    }

private:
    ros::Subscriber objSubscriber_;
    TIMER_C         objValidTimer_{0.25};
    BOOL            isFull_{false};

    void ReceiveMsgCallBack(const comm_msg::dustbinState::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != nullptr, NOTHING);

        objValidTimer_.Reset();
        isFull_  = pstMsg->isFull;

    }
};

#endif

