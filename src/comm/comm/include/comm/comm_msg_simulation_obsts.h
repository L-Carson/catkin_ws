

#ifndef __COMM_MSG_SIMULATION_OBSTS_H__
#define __COMM_MSG_SIMULATION_OBSTS_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/simulation_obsts.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include <tf/transform_datatypes.h>
#include <ros/ros.h>
#include <functional>

struct SIMULATION_CIRCULAR_OBST_STRU
{
    COORDINATE_2D_STRU stCenter;
    float              radius;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class SIMULATION_FULL_OBSTS_SENDER_C
{
public:
    SIMULATION_FULL_OBSTS_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::simulation_obsts>(TOPIC_NAME_SIMULATION_FULL_OBSTS, 1);
    }

    void Send(const std::vector<SIMULATION_CIRCULAR_OBST_STRU> &vectObst)
    {
        comm_msg::simulation_obsts stMsg;

        for (const SIMULATION_CIRCULAR_OBST_STRU &stObst : vectObst) {
            comm_msg::simulation_circular_obst stObstMsg;
            stObstMsg.x      = stObst.stCenter.x;
            stObstMsg.y      = stObst.stCenter.y;
            stObstMsg.radius = stObst.radius;

            stMsg.circularObsts.push_back(stObstMsg);
        }

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class SIMULATION_FULL_OBSTS_RECEIVER_C
{
public:
    SIMULATION_FULL_OBSTS_RECEIVER_C(ros::NodeHandle &nh, const std::function<void(const std::vector<SIMULATION_CIRCULAR_OBST_STRU> &)> &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_SIMULATION_FULL_OBSTS, 1, &SIMULATION_FULL_OBSTS_RECEIVER_C::ReceiveMsgCallBack, this);
    }

private:
    const std::function<void(const std::vector<SIMULATION_CIRCULAR_OBST_STRU> &)> objRecvFunc_;
    ros::Subscriber                                                               objSubscriber_;

    void ReceiveMsgCallBack(const comm_msg::simulation_obsts::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        std::vector<SIMULATION_CIRCULAR_OBST_STRU> vectObsts;
        vectObsts.reserve(pstMsg->circularObsts.size());
        for (const comm_msg::simulation_circular_obst &stObstMsg : pstMsg->circularObsts) {
            SIMULATION_CIRCULAR_OBST_STRU stObst;
            stObst.stCenter.x = stObstMsg.x;
            stObst.stCenter.y = stObstMsg.y;
            stObst.radius  = stObstMsg.radius;

            vectObsts.push_back(stObst);
        }

        objRecvFunc_(vectObsts);
    }
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class SIMULATION_INC_OBST_RECEIVER_C
{
public:
    SIMULATION_INC_OBST_RECEIVER_C(ros::NodeHandle &nh, const std::function<void(const COORDINATE_2D_STRU &)> &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_SIMULATION_INC_OBST, 100, &SIMULATION_INC_OBST_RECEIVER_C::ReceiveMsgCallBack, this);
    }

private:
    const std::function<void(const COORDINATE_2D_STRU &)>   objRecvFunc_;
    ros::Subscriber                                         objSubscriber_;

    void ReceiveMsgCallBack(const geometry_msgs::PointStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        COORDINATE_2D_STRU stObstCenter;
        stObstCenter.x = float(pstMsg->point.x);
        stObstCenter.y = float(pstMsg->point.y);
        ST_LOG_INFO("Change Obst. x = %f, y = %f.", stObstCenter.x, stObstCenter.y);

        objRecvFunc_(stObstCenter);
    }
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class SIMULATION_DYNC_OBST_RECEIVER_C
{
public:
    SIMULATION_DYNC_OBST_RECEIVER_C(ros::NodeHandle &nh, const std::function<void(const COOR_2D_POSE_STRU &)> &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_SIMULATION_DYNAMIC_OBST, 100, &SIMULATION_DYNC_OBST_RECEIVER_C::ReceiveMsgCallBack, this);
    }

private:
    const std::function<void(const COOR_2D_POSE_STRU &)>   objRecvFunc_;
    ros::Subscriber                                        objSubscriber_;

    void ReceiveMsgCallBack(const geometry_msgs::PoseStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        COOR_2D_POSE_STRU stPose;
        stPose.stPos.x  = float(pstMsg->pose.position.x);
        stPose.stPos.y  = float(pstMsg->pose.position.y);
        stPose.dirAngle = tf::getYaw(pstMsg->pose.orientation);
        ST_LOG_INFO("Add Dynamic Obst. x = %f, y = %f, yaw = %f.", stPose.stPos.x, stPose.stPos.y, stPose.dirAngle);

        objRecvFunc_(stPose);
    }
};

#endif

