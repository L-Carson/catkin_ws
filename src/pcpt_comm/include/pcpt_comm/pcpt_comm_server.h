#ifndef __PCPT_COMM_SERVER_H__
#define __PCPT_COMM_SERVER_H__

#include "ros/ros.h"
#include "comm/comm_dbg_log.h"
#include "comm/comm_topic_name.h"
#include "comm_srvs/PcdNet.h"
#include "comm_srvs/camerasParam.h"
#include "comm_srvs/visionFunc.h"
#include "comm_srvs/visionFuncSim.h"
#include "pcpt_comm/pcpt_comm_cfg.h"
#include "pcpt_comm/pcpt_comm_define.h"
#include "pcpt_comm/pcpt_comm_vision.h"

/**
 * @brief 点云网络参数服务器
 */
class PCD_NET_SERVER_C
{
public:
    PCD_NET_SERVER_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_PCD_NET, &PCD_NET_SERVER_C::SrvCallback, this);
    }

    int Init();

private:
    ros::ServiceServer  server_;
    bool                isInitSucc_{false};
    bool                isOpen_{false};         // 点云网络启用标识
    vector<comm_srvs::LidarInstallParam> vmsg_; // 雷达安装参数

    bool SrvCallback(comm_srvs::PcdNet::Request &req, comm_srvs::PcdNet::Response &res);
};

/**
 * @brief RGB相机参数服务器
 */
class RGB_CAMERA_PARAM_SERVER_C
{
public:
    RGB_CAMERA_PARAM_SERVER_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_RGB_CAMERA_PARAM, &RGB_CAMERA_PARAM_SERVER_C::SrvCallback, this);
    }

    int Init();

private:
    bool                isInitSucc_{false};
    ros::ServiceServer  server_;
    vector<comm_srvs::cameraParam> vmsg_;   // RGB相机参数

    bool SrvCallback(comm_srvs::camerasParam::Request &req, comm_srvs::camerasParam::Response &res);
};

/**
 * @brief 图像功能服务器
 */
class VISION_FUNC_SERVER_C
{
public:
    VISION_FUNC_SERVER_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_VISION_FUNC, &VISION_FUNC_SERVER_C::SrvCallback, this);
    }

    INT32 Init(void);

private:
    BOOL                            isInitSucc_{false};
    ros::ServiceServer              server_;
    comm_srvs::visionFunc::Response res_;

    BOOL SrvCallback(comm_srvs::visionFunc::Request &req, comm_srvs::visionFunc::Response &res);
};

/**
 * @brief 图像功能仿真服务器
 */
class VISION_FUNC_SIM_SERVER_C
{
public:
    VISION_FUNC_SIM_SERVER_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_VISION_FUNC_SIM, &VISION_FUNC_SIM_SERVER_C::SrvCallback, this);
    }

    INT32 GetVisionFuncAttr(VISION_FUNC_ATTR_STRU& stFuncAttr);

private:
    TIMER_C                 objServerValidTimer_{5.0};  //服务有效时长
    ros::ServiceServer      server_;
    VISION_FUNC_ATTR_STRU   stFuncAttr_;

    BOOL SrvCallback(comm_srvs::visionFuncSim::Request &req, comm_srvs::visionFuncSim::Response &res);
};

/**
 * @brief 感知参数服务器
 */
class PCPT_ROS_SERVER_C
{
public:
    PCPT_ROS_SERVER_C(ros::NodeHandle& nh)
    : objPcdNetServer_(nh)
    , objRgbCameraParamServer_(nh)
    , objVisionFuncServer_(nh)
    {
        clientTest_ = nh.serviceClient<comm_srvs::camerasParam>(TOPIC_NAME_SERVER_RGB_CAMERA_PARAM);
    }

    INT32 Init(void);
    INT32 ClientTest(void);

private:
    PCD_NET_SERVER_C            objPcdNetServer_;
    RGB_CAMERA_PARAM_SERVER_C   objRgbCameraParamServer_;
    VISION_FUNC_SERVER_C        objVisionFuncServer_;
    ros::ServiceClient          clientTest_;
};

#endif

