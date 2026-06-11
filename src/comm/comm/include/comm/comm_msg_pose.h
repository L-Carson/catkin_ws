
#ifndef __COMM_MSG_POSE_H__
#define __COMM_MSG_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_datatypes.h>
#include <ros/ros.h>
#include <eigen3/Eigen/Core>
#include <functional>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class POSE_SENDER_C
{
public:
    POSE_SENDER_C(ros::NodeHandle &nh, const std::string &strTopicName, const std::string &strFrameId = "map") : strFrameId_(strFrameId)
    {
        objPublisher_ = nh.advertise<geometry_msgs::PoseStamped>(strTopicName, 10);
    }

    BOOL IsExistSubscriber(void)
    {
        return (objPublisher_.getNumSubscribers() > 0);
    }

    void Send(const Eigen::Matrix4d &objTfMat)
    {
        Eigen::Vector3d    objTranslVec = objTfMat.block<3, 1>(0, 3);
        Eigen::Quaterniond objQuaterniond(objTfMat.block<3, 3>(0, 0));

        geometry_msgs::PoseStamped stMsg;
        stMsg.header.stamp       = ros::Time::now();
        stMsg.header.frame_id    = strFrameId_;
        stMsg.pose.position.x    = objTranslVec(0);
        stMsg.pose.position.y    = objTranslVec(1);
        stMsg.pose.position.z    = objTranslVec(2);
        stMsg.pose.orientation.w = objQuaterniond.w();
        stMsg.pose.orientation.x = objQuaterniond.x();
        stMsg.pose.orientation.y = objQuaterniond.y();
        stMsg.pose.orientation.z = objQuaterniond.z();

        objPublisher_.publish(stMsg);
    }

private:
    const std::string   strFrameId_;
    ros::Publisher      objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/

#endif

