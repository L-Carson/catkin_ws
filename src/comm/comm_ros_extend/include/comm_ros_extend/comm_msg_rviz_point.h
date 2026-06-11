#ifndef __COMM_MSG_RVIZ_POINT_H__
#define __COMM_MSG_RVIZ_POINT_H__

#include <functional>
#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>

#include "comm/comm_base.h"

class RVIZ_POINT_RECEIVER_C
{
public:
    RVIZ_POINT_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/clicked_point", BOOL latch = false)
    {
        isExistRecFun_ = false;
        objSubscriber_ = nh.subscribe(pcTopicName, 1, &RVIZ_POINT_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    void ReceiveMsgCallBack(const geometry_msgs::PointStamped::ConstPtr& pstMsg)
    {
        COORDINATE_3D_STRU stPos(pstMsg->point.x, pstMsg->point.y, pstMsg->point.z);
        if (isExistRecFun_)
            objRecvFunc_(stPos); 
    }

    void RegRecFun(const std::function<void(const COORDINATE_3D_STRU &)> &objRecvFunc) 
    {
        objRecvFunc_   = objRecvFunc;
        isExistRecFun_ = true;
    }

private:
    ros::Subscriber                                  objSubscriber_;
    BOOL                                             isExistRecFun_;
    std::function<void(const COORDINATE_3D_STRU &)>  objRecvFunc_;   
};

#endif
