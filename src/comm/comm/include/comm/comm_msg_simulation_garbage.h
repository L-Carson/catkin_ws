
#ifndef __COMM_MSG_SIMULATION_GARBAGE_H__
#define __COMM_MSG_SIMULATION_GARBAGE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "geometry_msgs/PointStamped.h"
#include <ros/ros.h>
#include <functional>

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class SIMULATION_INC_GARBAGE_RECEIVER_C
{
public:
    SIMULATION_INC_GARBAGE_RECEIVER_C(ros::NodeHandle &nh, const std::function<void(const COORDINATE_2D_STRU &)> &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_SIMULATION_INC_GARBAGE, 100, &SIMULATION_INC_GARBAGE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

private:
    const std::function<void(const COORDINATE_2D_STRU &)>   objRecvFunc_;
    ros::Subscriber                                         objSubscriber_;

    void ReceiveMsgCallBack(const geometry_msgs::PointStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        COORDINATE_2D_STRU stGarbageCenter;
        stGarbageCenter.x = float(pstMsg->point.x);
        stGarbageCenter.y = float(pstMsg->point.y);
        ST_LOG_INFO("Change Garbage. x = %f, y = %f.", stGarbageCenter.x, stGarbageCenter.y);

        objRecvFunc_(stGarbageCenter);
    }
};


#endif

