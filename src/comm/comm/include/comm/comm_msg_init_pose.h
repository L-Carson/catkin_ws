

#ifndef __COMM_MSG_INIT_POSE_H__
#define __COMM_MSG_INIT_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <tf/transform_datatypes.h>
#include <ros/ros.h>
#include <functional>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class INIT_POSE_SENDER_C
{
public:
    INIT_POSE_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>(TOPIC_NAME_INIT_POSE, 1);
    }

    void Send(const COOR_2D_POSE_STRU &stPose)
    {
        geometry_msgs::PoseWithCovarianceStamped stMsg;
        stMsg.header.stamp = ros::Time::now();
        stMsg.header.frame_id = "map";
        stMsg.pose.pose.position.x  = stPose.stPos.x;
        stMsg.pose.pose.position.y  = stPose.stPos.y;
        stMsg.pose.pose.position.z  = 0.0;
        stMsg.pose.pose.orientation = tf::createQuaternionMsgFromYaw(stPose.dirAngle);

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class INIT_POSE_RECEIVER_C
{
public:
    INIT_POSE_RECEIVER_C(ros::NodeHandle &nh, BOOL isActOnce = true)
                            : isGetMode_(true),
                              isActOnce_(isActOnce),
                              objRecvFunc_(NULL)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_INIT_POSE, 1, &INIT_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INIT_POSE_RECEIVER_C(ros::NodeHandle &nh, std::function<void(const COOR_2D_POSE_STRU &)> objRecvFunc)
                            : isGetMode_(false),
                              isActOnce_(true),
                              objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_INIT_POSE, 1, &INIT_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(COOR_2D_POSE_STRU &stInitPose)
    {
        R_ASSERT(isGetMode_, -1);

        if (!isExist_) {
            return -1;
        }

        stInitPose = stInitPose_;
        isExist_   = isActOnce_ ? false : true;
        return 0;
    }

private:
    const BOOL                                             isGetMode_;
    const BOOL                                             isActOnce_;
    const std::function<void(const COOR_2D_POSE_STRU &)>   objRecvFunc_;
    ros::Subscriber                                        objSubscriber_;
    BOOL                                                   isExist_{false};
    COOR_2D_POSE_STRU                                      stInitPose_;

    void ReceiveMsgCallBack(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        stInitPose_.stPos.x  = pstMsg->pose.pose.position.x;
        stInitPose_.stPos.y  = pstMsg->pose.pose.position.y;
        stInitPose_.dirAngle = tf::getYaw(pstMsg->pose.pose.orientation);
        ST_LOG_INFO("x=%f, y=%f, yaw=%f.", stInitPose_.stPos.x, stInitPose_.stPos.y, stInitPose_.dirAngle);

        if (isGetMode_) {
            isExist_ = true;
        } else {
            R_ASSERT(objRecvFunc_ != NULL, NOTHING);
            objRecvFunc_(stInitPose_);
        }
    }
};


#endif

