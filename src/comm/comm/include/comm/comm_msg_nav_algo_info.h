#ifndef __COMM_MSG_NAV_ALGO_INFO_H__
#define __COMM_MSG_NAV_ALGO_INFO_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/nav_algo_info.h>
#include <ros/ros.h>


struct NAV_ALGO_INFO_STRU
{
    UINT08 algoType;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class NAV_ALGO_INFO_SENDER_C
{
public:
    NAV_ALGO_INFO_SENDER_C(ros::NodeHandle &nh)
    {
        /* 保存最新状态 */
        objPublisher_ = nh.advertise<comm_msg::nav_algo_info>(TOPIC_NAME_NAV_ALGO_INFO, 1);
    }

    void Send(const NAV_ALGO_INFO_STRU &stNavAlgoInfo)
    {
        comm_msg::nav_algo_info stMsg;
        stMsg.algoType = stNavAlgoInfo.algoType;

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class NAV_ALGO_INFO_RECEIVER_C
{
public:
    NAV_ALGO_INFO_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_NAV_ALGO_INFO, 1, &NAV_ALGO_INFO_RECEIVER_C::ReceiveMsgCallBack, this);

        objValidTimer_.SetOverTime(1.0);
    }

    INT32 Get(NAV_ALGO_INFO_STRU &stNavAlgoInfo) const
    {
        stNavAlgoInfo = stNavAlgoInfo_;

        return objValidTimer_.IsTimeOut() ? -1 : 0;
    }

private:
    ros::Subscriber    objSubscriber_;

    TIMER_C            objValidTimer_;
    NAV_ALGO_INFO_STRU stNavAlgoInfo_;

    void ReceiveMsgCallBack(const comm_msg::nav_algo_info::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        stNavAlgoInfo_.algoType = pstMsg->algoType;
        objValidTimer_.Reset();
    }
};


#endif

