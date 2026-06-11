#ifndef __COMM_MSG_RVIZ_GOAL_POSE_H__
#define __COMM_MSG_RVIZ_GOAL_POSE_H__

#include <functional>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/PoseStamped.h>

#include "comm/comm_base.h"

class RVIZ_GOAL_POSE_RECEIVER_C
{
public:
    RVIZ_GOAL_POSE_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/move_base_simple/goal", BOOL latch = false)
    {
        isExistRecFun_ = false;
        objSubscriber_ = nh.subscribe(pcTopicName, 1, &RVIZ_GOAL_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    void ReceiveMsgCallBack(const geometry_msgs::PoseStamped::ConstPtr& pstMsg)
    {
        COOR_2D_POSE_STRU stInitPose;
        stInitPose.stPos.x  = pstMsg->pose.position.x;
        stInitPose.stPos.y  = pstMsg->pose.position.y;
        stInitPose.dirAngle = tf::getYaw(pstMsg->pose.orientation);
        if (isExistRecFun_)
            objRecvFunc_(stInitPose); 
    }

    void RegRecFun(const std::function<void(const COOR_2D_POSE_STRU &)> &objRecvFunc) 
    {
        objRecvFunc_   = objRecvFunc;
        isExistRecFun_ = true;
    }

private:
    ros::Subscriber                                 objSubscriber_;
    BOOL                                            isExistRecFun_;
    std::function<void(const COOR_2D_POSE_STRU &)>  objRecvFunc_;   
};

#endif

