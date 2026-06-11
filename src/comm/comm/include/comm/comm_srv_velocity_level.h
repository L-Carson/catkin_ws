#ifndef __COMM_SRV_VELOCITY_LEVEL_H__
#define __COMM_SRV_VELOCITY_LEVEL_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"

#include "comm_srvs/VelocityLevel.h"

#include <ros/ros.h>
#include <functional>

/**************************************************************************************
功能描述: 速度等级
修改记录:
**************************************************************************************/
enum VELOCITY_LEVEL_ENUM {
    VELOCITY_LEVEL_NORMAL = 0,        /* 正常速度 */
    VELOCITY_LEVEL_MAX    = 1,        /* 最大速度 */
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class VELOCITY_LEVEL_CLIENT_C
{
public:
    VELOCITY_LEVEL_CLIENT_C(ros::NodeHandle &nh)
    {
        objClient_ = nh.serviceClient<comm_srvs::VelocityLevel>(SERVICE_NAME_VELOCITY_LEVEL);
    }

    BOOL Call(VELOCITY_LEVEL_ENUM enVelocityLevel)
    {
        /* 等待2.5s */
        if(!ros::service::waitForService(SERVICE_NAME_VELOCITY_LEVEL, 2500)) {
            ST_LOG_ERR("WaitForService %s, timeout.", SERVICE_NAME_VELOCITY_LEVEL);
            return false;
        }

        comm_srvs::VelocityLevel velocityLevelSrv;
        velocityLevelSrv.request.level = (INT32)enVelocityLevel;

        return objClient_.call(velocityLevelSrv);
    }

private:

    ros::ServiceClient objClient_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class VELOCITY_LEVEL_SERVER_C
{
public:
    VELOCITY_LEVEL_SERVER_C(ros::NodeHandle &nh)
    {
        isUpdated_ = false;
        objServer_ = nh.advertiseService(SERVICE_NAME_VELOCITY_LEVEL, &VELOCITY_LEVEL_SERVER_C::velocityLevelCallback, this);
    }

    BOOL Get(VELOCITY_LEVEL_ENUM &enVelocityLevel) {
        BOOL isUpdated  = isUpdated_;
        enVelocityLevel = enVelocityLevel_;

        isUpdated_ = false;

        return isUpdated;
    }

private:

    BOOL velocityLevelCallback(comm_srvs::VelocityLevel::Request &req, comm_srvs::VelocityLevel::Response &res)
    {
        enVelocityLevel_ = (VELOCITY_LEVEL_ENUM)req.level;

        res.success = true;

        isUpdated_ = true;
        return true;
    }

    VELOCITY_LEVEL_ENUM enVelocityLevel_;
    BOOL                isUpdated_;
    ros::ServiceServer  objServer_;
};

#endif


