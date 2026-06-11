

#ifndef __COMM_MSG_SIMULATION_TOOL_POINT_H__
#define __COMM_MSG_SIMULATION_TOOL_POINT_H__

#include "comm/comm_base.h"
#include "geometry_msgs/PointStamped.h"
#include <ros/ros.h>
#include <functional>

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class SIMU_TOOL_POINT_RECEIVER_C
{
public:
    SIMU_TOOL_POINT_RECEIVER_C(ros::NodeHandle &nh, const std::string &strTopic, const std::function<void(const COORDINATE_2D_STRU &)> &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(strTopic.c_str(), 100, &SIMU_TOOL_POINT_RECEIVER_C::ReceiveMsgCallBack, this);
    }

private:
    const std::function<void(const COORDINATE_2D_STRU &)>   objRecvFunc_;
    ros::Subscriber                                         objSubscriber_;

    void ReceiveMsgCallBack(const geometry_msgs::PointStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        COORDINATE_2D_STRU stPoint;
        stPoint.x = float(pstMsg->point.x);
        stPoint.y = float(pstMsg->point.y);

        objRecvFunc_(stPoint);
    }
};

#endif

