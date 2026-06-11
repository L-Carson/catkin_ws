#ifndef __COMM_MSG_BODY_POSE_H__
#define __COMM_MSG_BODY_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/body_pose.h>
#include <ros/ros.h>

struct BODY_POSE_STRU {
    COOR_2D_POSE_STRU  stBackWheelPose;
    COORDINATE_2D_STRU stFrontWheelPos;
    COORDINATE_2D_STRU stAlongEdgeRefPointPos;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class BODY_POSE_SENDER_C
{
public:
    BODY_POSE_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::body_pose>(TOPIC_NAME_BODY_POSE, 1);
    }

    BOOL IsEixstSubscriber(void)
    {
        return (objPublisher_.getNumSubscribers() > 0);
    }

    void Send(const BODY_POSE_STRU &stBodyPose)
    {
        comm_msg::body_pose stMsg;

        stMsg.body_yaw            = stBodyPose.stBackWheelPose.dirAngle;
        stMsg.backWheel_x         = stBodyPose.stBackWheelPose.stPos.x;
        stMsg.backWheel_y         = stBodyPose.stBackWheelPose.stPos.y;
        stMsg.frontWheel_x        = stBodyPose.stFrontWheelPos.x;
        stMsg.frontWheel_y        = stBodyPose.stFrontWheelPos.y;
        stMsg.alongEdgeRefPoint_x = stBodyPose.stAlongEdgeRefPointPos.x;
        stMsg.alongEdgeRefPoint_y = stBodyPose.stAlongEdgeRefPointPos.y;

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

#endif

