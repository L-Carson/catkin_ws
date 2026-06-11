
#ifndef __COMM_SRV_INIT_POSE_H__
#define __COMM_SRV_INIT_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"

#include "comm_srvs/InitPose.h"

#include <ros/ros.h>
#include <functional>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class INIT_POSE_CLIENT_C
{
public:
    INIT_POSE_CLIENT_C(ros::NodeHandle &nh)
    {
        objClient_ = nh.serviceClient<comm_srvs::InitPose>(SERVICE_NAME_INIT_POSE);
    }

    BOOL Call(const COOR_2D_POSE_STRU &stPose)
    {
        /* 等待2.5s */
        if(!ros::service::waitForService(SERVICE_NAME_INIT_POSE, 2500)) {
            ST_LOG_ERR("WaitForService %s, timeout.", SERVICE_NAME_INIT_POSE);
            return false;
        }

        comm_srvs::InitPose initPoseSrv;
        initPoseSrv.request.x   = stPose.stPos.x;
        initPoseSrv.request.y   = stPose.stPos.y;
        initPoseSrv.request.yaw = stPose.dirAngle;

        return objClient_.call(initPoseSrv);
    }

private:

    ros::ServiceClient objClient_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class INIT_POSE_SERVER_C
{
public:
    INIT_POSE_SERVER_C(ros::NodeHandle &nh)
    {
        isUpdated_ = false;
        objServer_ = nh.advertiseService(SERVICE_NAME_INIT_POSE, &INIT_POSE_SERVER_C::InitPoseCallback, this);
    }

    BOOL Get(COOR_2D_POSE_STRU &stInitPose) {
        BOOL isUpdated = isUpdated_;
        stInitPose     = stInitPose_;

        isUpdated_ = false;

        return isUpdated;
    }

private:

    BOOL InitPoseCallback(comm_srvs::InitPose::Request &req, comm_srvs::InitPose::Response &res)
    {
        stInitPose_.stPos.x  = req.x;
        stInitPose_.stPos.y  = req.y;
        stInitPose_.dirAngle = req.yaw;

        res.success = true;

        isUpdated_ = true;
        return true;
    }

    COOR_2D_POSE_STRU  stInitPose_;
    BOOL               isUpdated_;
    ros::ServiceServer objServer_;
};

#endif

