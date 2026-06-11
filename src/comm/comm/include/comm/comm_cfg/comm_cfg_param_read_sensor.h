#ifndef __COMM_CFG_PARAM_READ_SENSOR_H__
#define __COMM_CFG_PARAM_READ_SENSOR_H__

#include "comm_cfg_param_define.h"
#include "comm_cfg_param_read_base.h"

#include "comm/comm_urdf_api.h"
#include "comm/comm_base.h"
#include "comm/loc_xml/tinyxml.h"

namespace COMM
{

namespace CFG
{


/**
 * @brief 机器参数管理器
 *
 */
class MachineParamManager
{
private:
    std::string objUrdfDir_ = "urdf/";     // urdf文件目录
    UrdfOptApiC *pobjUrdfApi_ = nullptr;   // urdf文件操作接口

public:
    // 构造函数
    MachineParamManager(void);

    // 析构函数
    ~MachineParamManager(void);

    // 读取指定joint的TF
    int ReadGivenTFFromUrdf(const std::string &robot_type,
                            const std::string &parent_frame_name,
                            const std::string &child_frame_name,
                            boost::array<float, 3> &rpy_value,
                            boost::array<float, 3> &xyz_value);
};

/**************************************************************************************
功能描述: 传感器配置参数读取
修改记录:
**************************************************************************************/
class CFG_PARAM_READ_SENSOR_C : public CFG_PARAM_READ_BASE_C
{
public:
    INT32 ReadGnssCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_STRU &stGnssCfg);
    INT32 ReadImuCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_IMU_STRU &stImuCfg);
    INT32 ReadUltrasioncsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ULTRASONICS_STRU &stCfg);
    INT32 ReadTofsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_TOFS_STRU &stCfg);
    INT32 ReadRadarCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RADAR_STRU &stRadar);
    INT32 ReadRgbCamerasCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RGB_CAMERAS_STRU &stCfg);
    INT32 ReadBackDepthCameraCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_BACK_DEPTH_CAMERA_STRU &stBackDepthCameraCfg);
    INT32 ReadLocLidarCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LOC_LIDAR_STRU &stLocLidarCfg);
    INT32 ReadLidarsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDARS_STRU &stLidarsCfg);

private:
    MachineParamManager objMacParamMng_; // 机器参数管理器

    INT32 ReadInstallParam(TiXmlElement *pobjCfgParam, CFG_PARAM_INSTALL_STRU &stInstallPose);

    /**************************************************************************************
    功能描述: 读取超声参数
    修改记录:
    **************************************************************************************/
    INT32 ReadUsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ULTRASONIC_STRU &stUsCfg);

    /**************************************************************************************
    功能描述: 读取TOF参数
    修改记录:
    **************************************************************************************/
    INT32 ReadTofCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_TOF_STRU &stTofCfg);

    /**************************************************************************************
    功能描述: 读取毫米波雷达参数
    修改记录:
    **************************************************************************************/
    INT32 ReadRadarDevCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RADAR_DEV_STRU &stRadarDevCfg);

    /**************************************************************************************
    功能描述: 读取摄像头参数
    修改记录:
    **************************************************************************************/
    void  CameraMatrixSTR2DBL(string& strData, vector<double>& vData);
    INT32 ReadRgbCameraElement(TiXmlElement *pobjCfgParam, const char* element, vector<double>& vData, uint32_t num);
    INT32 ReadRgbCameraCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RGB_CAMERA_STRU &stCameraCfg);

    INT32 ReadPlaneFitParam(TiXmlElement *pobjCfgParam, CFG_PARAM_PLANE_FIT_STRU &stPlaneFitParam);

    /**************************************************************************************
    功能描述: 读取雷达参数
    修改记录:
    **************************************************************************************/
    INT32 ReadLidarPropertyCfg(TiXmlElement *pobjLidarParam, const char *element, CFG_PARAM_LIDAR_PROPERTY_STRU &stProperty);
    INT32 ReadLidarNetworkCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_NET_STRU &stNetCfg);
    INT32 ReadLidarDistParamCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_DIST_STRU &stDistCfg);
    INT32 ReadLidarInstallCfg(TiXmlElement *pobjLidarParam, const char *element, string& strFrameId, CFG_PARAM_INSTALL_STRU &stInstallParam);
    INT32 ReadLidarInstallOffsetCfg(TiXmlElement *pobjLidarParam, const char *element, string& strFrameId, CFG_PARAM_INSTALL_STRU &stInstallParam);
    INT32 ReadLidarTFFromURDF(const std::string &carType, const std::string &laserFrame, CFG_PARAM_INSTALL_STRU &stInstallCfg);
    INT32 ReadLaserSectionWipeCfg(TiXmlElement *pobjLidarParam, string& strLidarName, CFG_PARAM_LASER_SECTION_WIPE_STRU &stLaserSectionWipe);
    INT32 ReadLidarSectionWipeCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_SECTION_WIPE_STRU &stLidarSectionWipe);
    INT32 ReadLaserBeamWipeCfg(TiXmlElement *pobjBeams, CFG_PARAM_LASER_BEAM_WIPE_STRU &stLaserBeamWipe);
    INT32 ReadLidarBeamWipeCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_BEAM_WIPE_STRU &stLidarBeamWipe);
    INT32 ReadLidarCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_STRU &stLidarCfg);
};

}

}

#endif

