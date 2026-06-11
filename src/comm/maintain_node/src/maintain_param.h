#pragma once

#include <ros/ros.h>
#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include "comm/comm_topic_name.h"
#include "comm/comm_cfg_param.h"
#include "comm/comm_file.h"
#include "comm/comm_cfg/comm_cfg_amend.h"
#include "comm_srvs/cameraCalibration.h"
#include "comm_srvs/lidarCalibration.h"
#include "comm_srvs/imuCalibration.h"
#include "comm_srvs/lidarOffsetCalibration.h"
#include "comm_srvs/imuOffsetCalibration.h"
#include "comm_msg/roborExtriParam.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;

struct RGB_CAMERA_CLBT_POS_STRU {
    CAR_TYPE_ENUM       enCarType;                  // 车辆类型
    COORDINATE_3D_STRU  stFrontLeftCameraPos;       // 前左相机
    COORDINATE_3D_STRU  stFrontMidLeftCameraPos;    // 前中左相机
    COORDINATE_3D_STRU  stFrontMidCameraPos;        // 前中相机
    COORDINATE_3D_STRU  stFrontMidRightCameraPos;   // 前中右相机
    COORDINATE_3D_STRU  stFrontRightCameraPos;      // 前右相机
    COORDINATE_3D_STRU  stRearLeftCameraPos;        // 后左相机
    COORDINATE_3D_STRU  stRearMidCameraPos;         // 后中相机
    COORDINATE_3D_STRU  stRearRightCameraPos;       // 后右相机
    COORDINATE_3D_STRU  stLeftMidCameraPos;         // 左中相机
    COORDINATE_3D_STRU  stRightMIdCameraPos;        // 右中相机
};

class CLBT_PARAM_BASE_C
{
public:

protected:
    INT32 ModifyCfgParamFile(const std::function<INT32(TiXmlElement &)> &pfModifyCfg) const;

private:
    COMM::CFG::CFG_PARAM_FILE_MNG_C objCfgParamFileMng_;
};

/**************************************************************************************
功能描述: 相机标定参数
修改记录:
**************************************************************************************/
class RGB_CAMERA_CLBT_PARAM_C : public CLBT_PARAM_BASE_C
{
public:
    RGB_CAMERA_CLBT_PARAM_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_CLBT_RGB_CAMERA_PARAM, &RGB_CAMERA_CLBT_PARAM_C::SrvCallback, this);
    }

private:
    static const RGB_CAMERA_CLBT_POS_STRU astCameraPos_[];
    ros::ServiceServer  server_;

    int  GetCameraInstallPos(const string& strModel, const string& strCameraName, COORDINATE_3D_STRU& stCameraPos);
    bool SrvCallback(comm_srvs::cameraCalibration::Request &req, comm_srvs::cameraCalibration::Response &res);
};

/**************************************************************************************
功能描述: 激光雷达标定参数
修改记录:
**************************************************************************************/
class LIDAR_CLBT_PARAM_C : public CLBT_PARAM_BASE_C
{
public:
    LIDAR_CLBT_PARAM_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_CLBT_LIDAR_PARAM, &LIDAR_CLBT_PARAM_C::SrvCallback, this);
    }

private:
    ros::ServiceServer  server_;
    bool SrvCallback(comm_srvs::lidarCalibration::Request &req, comm_srvs::lidarCalibration::Response &res);
};

/**************************************************************************************
功能描述: IMU标定参数
修改记录:
**************************************************************************************/
class IMU_CLBT_PARAM_C : public CLBT_PARAM_BASE_C
{
public:
    IMU_CLBT_PARAM_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_CLBT_IMU_PARAM, &IMU_CLBT_PARAM_C::SrvCallback, this);
    }

private:
    ros::ServiceServer  server_;
    bool SrvCallback(comm_srvs::imuCalibration::Request &req, comm_srvs::imuCalibration::Response &res);
};

/**************************************************************************************
功能描述: 激光雷达标定补偿参数
修改记录:
**************************************************************************************/
class LIDAR_OFFSET_CLBT_PARAM_C : public CLBT_PARAM_BASE_C
{
public:
    LIDAR_OFFSET_CLBT_PARAM_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_CLBT_LIDAR_OFFSET_PARAM, &LIDAR_OFFSET_CLBT_PARAM_C::SrvCallback, this);
    }

    int SaveParam();
private:
    ros::ServiceServer  server_;
    bool SrvCallback(comm_srvs::lidarOffsetCalibration::Request &req, comm_srvs::lidarOffsetCalibration::Response &res);
};

/**************************************************************************************
功能描述: IMU标定补偿参数
修改记录:
**************************************************************************************/
class IMU_OFFSET_CLBT_PARAM_C : public CLBT_PARAM_BASE_C
{
public:
    IMU_OFFSET_CLBT_PARAM_C(ros::NodeHandle& nh)
    {
        server_ = nh.advertiseService(TOPIC_NAME_SERVER_CLBT_IMU_OFFSET_PARAM, &IMU_OFFSET_CLBT_PARAM_C::SrvCallback, this);
    }

private:
    ros::ServiceServer  server_;
    bool SrvCallback(comm_srvs::imuOffsetCalibration::Request &req, comm_srvs::imuOffsetCalibration::Response &res);
};

/**************************************************************************************
功能描述: 监控cfg文件
修改记录:
**************************************************************************************/
class CFG_PARAM_FILE_MONITOR_C
{
public:
    CFG_PARAM_FILE_MONITOR_C(void)
    {
        /* 获取配置文件路径，此处监控修改版配置文件，因为它才会被手动修改 */
        strFilePath_ = COMM::CFG::CFG_PARAM_FILE_MNG_C().GetChangeCfgParamFilePath();

        /* 打印cfg文件内容 */
        ShowCfgParam(strFilePath_);

        /* 启动文件修改监测及同步线程 */
        objMonitorThread_ = std::thread(&CFG_PARAM_FILE_MONITOR_C::MonitorThreadFunc, this);
    }

    ~CFG_PARAM_FILE_MONITOR_C(void)
    {
        if (objMonitorThread_.joinable())
            objMonitorThread_.join();
    }

private:
    /* 打印cfg参数表到终端 */
    void ShowCfgParam(const std::string &strCfgFilePath);

    /**
     * cfg监控函数
     * 监控文件的修改时间，有时间更新则将内容强制写入内存
     */
    void MonitorThreadFunc(void);

private:
    std::string strFilePath_;       // cfg配置文件的路径
    std::thread objMonitorThread_;  // 线程
};

/**************************************************************************************
功能描述: 以1Hz的频率发布gnss、imu、lidar的外参和支持标志位
修改记录:
**************************************************************************************/
class EXTRI_PUBLISHER_C
{
public:
    EXTRI_PUBLISHER_C(ros::NodeHandle& nh)
    {
        pub_ = nh.advertise<comm_msg::roborExtriParam>(TOPIC_NAME_EXTRI_PARAM, 10);
        objThread_ = std::thread(&EXTRI_PUBLISHER_C::PubExtriParamTopic, this);
    }

    ~EXTRI_PUBLISHER_C(void)
    {
        if (objThread_.joinable())
            objThread_.join();
    }

private:
    void PubExtriParamTopic(void);

private:
    ros::Publisher pub_;     // 发布器
    std::thread objThread_;  // 线程

};

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
class MAINTAIN_C
{
public:
    MAINTAIN_C(ros::NodeHandle& nh)
    : objCameraClbtParam_(nh)
    , objLidarClbtParam_(nh)
    , objImuClbtParam_(nh)
    , objLidarOffsetClbtParam_(nh)
    , objImuOffsetClbtParam_(nh)
    , objSensorExtriSender_(nh)
    {
    }

private:
    RGB_CAMERA_CLBT_PARAM_C   objCameraClbtParam_;
    LIDAR_CLBT_PARAM_C        objLidarClbtParam_;
    IMU_CLBT_PARAM_C          objImuClbtParam_;
    LIDAR_OFFSET_CLBT_PARAM_C objLidarOffsetClbtParam_;
    IMU_OFFSET_CLBT_PARAM_C   objImuOffsetClbtParam_;
    CFG_PARAM_FILE_MONITOR_C  objCfgParamFileMonitor_;
    EXTRI_PUBLISHER_C         objSensorExtriSender_;
};

