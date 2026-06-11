

#ifndef __COMM_MSG_GOAL_POSE_H__
#define __COMM_MSG_GOAL_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_datatypes.h>
#include <ros/ros.h>
#include <functional>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class GOAL_POSE_SENDER_C
{
public:
    GOAL_POSE_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<geometry_msgs::PoseStamped>(TOPIC_NAME_GOAL_POSE, 1);
    }

    void Send(const COOR_2D_POSE_STRU &stPose)
    {
        geometry_msgs::PoseStamped stMsg;
        stMsg.header.stamp = ros::Time::now();
        stMsg.header.frame_id = "map";
        stMsg.pose.position.x  = stPose.stPos.x;
        stMsg.pose.position.y  = stPose.stPos.y;
        stMsg.pose.position.z  = 0.0;
        stMsg.pose.orientation = tf::createQuaternionMsgFromYaw(stPose.dirAngle);

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class GOAL_POSE_RECEIVER_C
{
public:
    GOAL_POSE_RECEIVER_C(ros::NodeHandle &nh, BOOL isActOnce = true)
                            : isGetMode_(true),
                              isActOnce_(isActOnce),
                              objRecvFunc_(NULL)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_GOAL_POSE, 1, &GOAL_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    GOAL_POSE_RECEIVER_C(ros::NodeHandle &nh, std::function<void(const COOR_2D_POSE_STRU &)> objRecvFunc)
                            : isGetMode_(false),
                              isActOnce_(true),
                              objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_GOAL_POSE, 1, &GOAL_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(COOR_2D_POSE_STRU &stInitPose)
    {
        R_ASSERT(isGetMode_, -1);

        if (!isExist_) {
            return -1;
        }

        stInitPose = stGoalPose_;
        isExist_   = isActOnce_ ? false : true;
        return 0;
    }

private:
    const BOOL                                             isGetMode_;
    const BOOL                                             isActOnce_;
    const std::function<void(const COOR_2D_POSE_STRU &)>   objRecvFunc_;
    ros::Subscriber                                        objSubscriber_;
    BOOL                                                   isExist_{false};
    COOR_2D_POSE_STRU                                      stGoalPose_;

    void ReceiveMsgCallBack(const geometry_msgs::PoseStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        stGoalPose_.stPos.x  = pstMsg->pose.position.x;
        stGoalPose_.stPos.y  = pstMsg->pose.position.y;
        stGoalPose_.dirAngle = tf::getYaw(pstMsg->pose.orientation);
        ST_LOG_INFO("x=%f, y=%f, yaw=%f.", stGoalPose_.stPos.x, stGoalPose_.stPos.y, stGoalPose_.dirAngle);

        if (isGetMode_) {
            isExist_ = true;
        } else {
            R_ASSERT(objRecvFunc_ != NULL, NOTHING);
            objRecvFunc_(stGoalPose_);
        }
    }
};


#endif

