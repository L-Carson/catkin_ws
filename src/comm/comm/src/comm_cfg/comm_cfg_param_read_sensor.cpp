
#include "comm/comm_cfg/comm_cfg_param_read_sensor.h"
#include "comm/comm_file.h"

#include <iostream>


namespace COMM
{

namespace CFG
{


// 构造函数
MachineParamManager::MachineParamManager(void)
{
    // 初始化urdf文件目录
    std::string objMachineDir;
    FILE_MNG_C objFileMng;
    objFileMng.GetMachineDir(objMachineDir);
    objUrdfDir_ = objMachineDir + objUrdfDir_;

    pobjUrdfApi_ = new UrdfOptApiC();
}

// 析构函数
MachineParamManager::~MachineParamManager(void)
{
    if (pobjUrdfApi_ != nullptr)
    {
        delete pobjUrdfApi_;
    }
}

// 读取指定joint的TF
int MachineParamManager::ReadGivenTFFromUrdf(const std::string &robot_type,
                                             const std::string &parent_frame_name,
                                             const std::string &child_frame_name,
                                             boost::array<float, 3> &rpy_value,
                                             boost::array<float, 3> &xyz_value)
{
    std::string objFileName = objUrdfDir_ + robot_type + ".urdf";

    // 读取指定joint的origin
    int read_ret = pobjUrdfApi_->ReadGivenJointOrigin(objFileName,
                                                      parent_frame_name, child_frame_name,
                                                      rpy_value, xyz_value);
    ST_LOG_INFO("加载TF: urdf文件[%s] frame[%s]->[%s] ret[%d] xyz(%f %f %f) rpy(%f %f %f)",
                objFileName.data(), child_frame_name.data(), parent_frame_name.data(), read_ret,
                xyz_value[0], xyz_value[1], xyz_value[2],
                rpy_value[0], rpy_value[1], rpy_value[2]);
    return read_ret;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadInstallParam(TiXmlElement *pobjCfgParam, CFG_PARAM_INSTALL_STRU &stInstallPose)
{
    INT32 rslt = 0;
    rslt += GetAttribute(pobjCfgParam, "Tx",    stInstallPose.tx);
    rslt += GetAttribute(pobjCfgParam, "Ty",    stInstallPose.ty);
    rslt += GetAttribute(pobjCfgParam, "Tz",    stInstallPose.tz);
    rslt += GetAttribute(pobjCfgParam, "Roll",  stInstallPose.roll);
    rslt += GetAttribute(pobjCfgParam, "Pitch", stInstallPose.pitch);
    rslt += GetAttribute(pobjCfgParam, "Yaw",   stInstallPose.yaw);

    //ST_LOG_INFO("Tx:%f Ty:%f Tz:%f Roll:%f Pitch:%f Yaw:%f", stInstallPose.tx, stInstallPose.ty, stInstallPose.tz, stInstallPose.roll, stInstallPose.pitch, stInstallPose.yaw);
    return rslt;
}

/**************************************************************************************
功能描述: 读取GPS配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadGnssCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_STRU &stGnssCfg)
{
    TiXmlElement *pobjGnssCfg = pobjCfgParam->FirstChildElement("Gnss");
    if (pobjGnssCfg == NULL) {
        ST_LOG_ERR("Get <Gnss> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjGnssCfg->QueryBoolAttribute("isSupport", &stGnssCfg.isSupport)) {
        ST_LOG_ERR("Get <Gnss> Cfg Attr <isSupport> Fail.");
        return -1;
    }

    INT32 protocolType;
    if (TIXML_SUCCESS != pobjGnssCfg->QueryIntAttribute("protocolType", &protocolType)) {
        ST_LOG_ERR("Get <Gnss> Cfg Attr <protocolType> Fail.");
        return -1;
    }
    stGnssCfg.enProtocolType = (GNSS_PROTOCOL_TYPE_ENUM)protocolType;
    if (TIXML_SUCCESS != pobjGnssCfg->QueryBoolAttribute("isDualAntenna", &stGnssCfg.isDualAntenna)) {
        ST_LOG_ERR("Get <Gnss> Cfg Attr <isDualAntenna> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjGnssCfg->QueryStringAttribute("devName", &stGnssCfg.strDevName)) {
        ST_LOG_ERR("Get <Gnss> Cfg Attr <devName> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjGnssCfg->QueryIntAttribute("baudRate", &stGnssCfg.baudRate)) {
        ST_LOG_ERR("Get <Gnss> Cfg Attr <baudRate> Fail.");
        return -1;
    }
    TiXmlElement *pobjInstallParamCfg = GetFirstChildElement(pobjGnssCfg, "InstallParam");
    if (pobjInstallParamCfg == NULL) return -1;

    INT32 rslt = ReadInstallParam(pobjInstallParamCfg, stGnssCfg.stInstallPose);
    if (rslt != 0) {
        ST_LOG_ERR("Read Gnss Install Pose Param Fail.");
        return -1;
    }

    ST_LOG_INFO("Cfg Param: <isSupport> = %d.",    stGnssCfg.isSupport);
    ST_LOG_INFO("Cfg Param: <protocolType> = %d.", (INT32)stGnssCfg.enProtocolType);
    return 0;
}


/**************************************************************************************
功能描述: 读取IMU配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadImuCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_IMU_STRU &stImuCfg)
{
    TiXmlElement *pobjImuCfg = pobjCfgParam->FirstChildElement("Imu");
    if (pobjImuCfg == NULL) {
        ST_LOG_ERR("Get <Imu> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjImuCfg->QueryBoolAttribute("isSupport", &stImuCfg.isSupport)) {
        ST_LOG_ERR("Get <Imu> Cfg Attr <isSupport> Fail.");
        return -1;
    }
    INT32 type;
    if (TIXML_SUCCESS != pobjImuCfg->QueryIntAttribute("type", &type)) {
        ST_LOG_ERR("Get <Imu> Cfg Attr <type> Fail.");
        return -1;
    }
    stImuCfg.enType = (IMU_TYPE_ENUM)type;

    if (TIXML_SUCCESS != pobjImuCfg->QueryStringAttribute("devName", &stImuCfg.strDevName)) {
        ST_LOG_ERR("Get <Imu> Cfg Attr <devName> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjImuCfg->QueryIntAttribute("baudRate", &stImuCfg.baudRate)) {
        ST_LOG_ERR("Get <Imu> Cfg Attr <baudRate> Fail.");
        return -1;
    }

    if (stImuCfg.enType == IMU_TYPE_AIRY) {
        stImuCfg.strImuRawTopic = "/raw_imu";
        if (TIXML_SUCCESS != pobjImuCfg->QueryStringAttribute("rawImuTopic", &stImuCfg.strImuRawTopic)) {
            ST_LOG_ERR("Get <Imu> Cfg Attr <rawImuTopic> Fail.");
        }
    }

    TiXmlElement *pobjInstallParamCfg = GetFirstChildElement(pobjImuCfg, "InstallParam");
    if (pobjInstallParamCfg == NULL)
        return -1;

    INT32 rslt = ReadInstallParam(pobjInstallParamCfg, stImuCfg.stInstallPose);
    if (rslt != 0) {
        ST_LOG_ERR("Read Imu Install Pose Param Fail.");
        return -1;
    }
    TiXmlElement *pobjOffsetParamCfg = GetFirstChildElement(pobjImuCfg, "Offset");
    if (pobjOffsetParamCfg == NULL)
        return -1;
    CFG_PARAM_INSTALL_STRU stOffsetCfg;
    rslt = ReadInstallParam(pobjOffsetParamCfg, stOffsetCfg);
    if (rslt == 0){
        ST_LOG_INFO("Read Imu Install Param, xyz(%f %f %f) rpy(%f %f %f).",
            stImuCfg.stInstallPose.tx,stImuCfg.stInstallPose.ty,stImuCfg.stInstallPose.tz,
            stImuCfg.stInstallPose.roll,stImuCfg.stInstallPose.pitch,stImuCfg.stInstallPose.yaw);
        ST_LOG_INFO("Read Imu Offset Param, xyz(%f %f %f) rpy(%f %f %f).",
            stOffsetCfg.tx,stOffsetCfg.ty,stOffsetCfg.tz, stOffsetCfg.roll,stOffsetCfg.pitch,stOffsetCfg.yaw);
        ATTITUDE_STRU stAtt0(stImuCfg.stInstallPose.roll,stImuCfg.stInstallPose.pitch,stImuCfg.stInstallPose.yaw);
        ATTITUDE_STRU stAtt1(stOffsetCfg.roll, stOffsetCfg.pitch, stOffsetCfg.yaw);
        Eigen::Matrix3f mRot  =  stAtt1.Matrix() * stAtt0.Matrix();;
        ATTITUDE_STRU stAtt;
        stAtt.Copy(mRot);
        stImuCfg.stInstallPose.roll  = stAtt.roll;
        stImuCfg.stInstallPose.pitch = stAtt.pitch;
        stImuCfg.stInstallPose.yaw   = stAtt.yaw; // 修正偏移
        ST_LOG_INFO("Read Imu Install Param(修正后), xyz(%f %f %f) rpy(%f %f %f).",
            stImuCfg.stInstallPose.tx,stImuCfg.stInstallPose.ty,stImuCfg.stInstallPose.tz,
            stImuCfg.stInstallPose.roll,stImuCfg.stInstallPose.pitch,stImuCfg.stInstallPose.yaw);
    }else{
        ST_LOG_INFO("No Imu Offset Param found, skip offset correction.");
        return -1;
    }

    ST_LOG_INFO("Cfg Param: <isSupport> = %d.", stImuCfg.isSupport);
    ST_LOG_INFO("Cfg Param: <type> = %d.",     (INT32)stImuCfg.enType);
    return 0;
}

/**************************************************************************************
功能描述: 读取超声波配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadUsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ULTRASONIC_STRU &stUsCfg)
{
    stUsCfg.strName = pobjCfgParam->Value();

    if (!IsValidSensor(Ultrasonic, stUsCfg.strName)) {
        ST_LOG_ERR("There is not exists the ultrasonic <%s>.", stUsCfg.strName.c_str());
        return -1;
    }

    if (GetAttribute(pobjCfgParam, "isSupport", stUsCfg.isSupport)) {
        ST_LOG_ERR("Failed to read <%s> property params.", stUsCfg.strName.c_str());
        return -1;
    }

    ST_LOG_INFO("<%s> Ultrasonic Cfg Param: <isSupport> = %d.", stUsCfg.strName.c_str(), stUsCfg.isSupport);
    if (!stUsCfg.isSupport) return 0;

    int rslt = 0;
    string strId;
    rslt += GetAttribute(pobjCfgParam, "Id",    strId);
    rslt += GetAttribute(pobjCfgParam, "Type",  stUsCfg.strType);
    rslt += GetAttribute(pobjCfgParam, "Tx",    stUsCfg.stInstallPose.tx);
    rslt += GetAttribute(pobjCfgParam, "Ty",    stUsCfg.stInstallPose.ty);
    rslt += GetAttribute(pobjCfgParam, "Roll",  stUsCfg.stInstallPose.roll);
    rslt += GetAttribute(pobjCfgParam, "Pitch", stUsCfg.stInstallPose.pitch);
    rslt += GetAttribute(pobjCfgParam, "Yaw",   stUsCfg.stInstallPose.yaw);
    stUsCfg.id = StringHex2Num(strId);
    stUsCfg.stInstallPose.tz = 0.0;

    if (rslt) return -1;
    ST_LOG_INFO("<%s> Ultrasonic Param: Id:%s(%u) Type:%s Tx:%.1f Ty:%.1f Roll:%.1f Pitch:%.1f Yaw:%.1f.",
                stUsCfg.strName.c_str(), strId.c_str(), stUsCfg.id, stUsCfg.strType.c_str(),
                stUsCfg.stInstallPose.tx, stUsCfg.stInstallPose.ty,
                stUsCfg.stInstallPose.roll, stUsCfg.stInstallPose.pitch, stUsCfg.stInstallPose.yaw);

    return 0;
}

/**************************************************************************************
功能描述: 读取Tof配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadTofCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_TOF_STRU &stTofCfg)
{
    stTofCfg.strName = pobjCfgParam->Value();

    if (!IsValidSensor(Tof, stTofCfg.strName)) {
        ST_LOG_ERR("There is not exists the tof <%s>.", stTofCfg.strName.c_str());
        return -1;
    }

    if (GetAttribute(pobjCfgParam, "isSupport", stTofCfg.isSupport)) {
        ST_LOG_ERR("Failed to read <%s> property params.", stTofCfg.strName.c_str());
        return -1;
    }

    ST_LOG_INFO("<%s> Tof Cfg Param: <isSupport> = %d.", stTofCfg.strName.c_str(), stTofCfg.isSupport);
    if (!stTofCfg.isSupport) return 0;

    int rslt = 0;
    rslt += GetAttribute(pobjCfgParam, "PortName", stTofCfg.strPortName);
    rslt += GetAttribute(pobjCfgParam, "BaudRate", stTofCfg.baudRate);
    rslt += GetAttribute(pobjCfgParam, "Tx",       stTofCfg.stInstallPose.tx);
    rslt += GetAttribute(pobjCfgParam, "Ty",       stTofCfg.stInstallPose.ty);
    rslt += GetAttribute(pobjCfgParam, "Tz",       stTofCfg.stInstallPose.tz);
    rslt += GetAttribute(pobjCfgParam, "Roll",     stTofCfg.stInstallPose.roll);
    rslt += GetAttribute(pobjCfgParam, "Pitch",    stTofCfg.stInstallPose.pitch);
    rslt += GetAttribute(pobjCfgParam, "Yaw",      stTofCfg.stInstallPose.yaw);

    if (rslt) return -1;
    ST_LOG_INFO("<%s> Tof Param: PortName:%s BaudRate:%u Tx:%.1f Ty:%.1f Tz:%.1f Roll:%.1f Pitch:%.1f Yaw:%.1f.",
                stTofCfg.strName.c_str(), stTofCfg.strPortName.c_str(), stTofCfg.baudRate,
                stTofCfg.stInstallPose.tx, stTofCfg.stInstallPose.ty, stTofCfg.stInstallPose.tz,
                stTofCfg.stInstallPose.roll, stTofCfg.stInstallPose.pitch, stTofCfg.stInstallPose.yaw);

    return 0;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadTofsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_TOFS_STRU &stCfg)
{
    TiXmlElement *pobjTofCfg = GetFirstChildElement(pobjCfgParam, "TofGroup");
    if (!pobjTofCfg) return -1;

    if (!GetAttribute(pobjTofCfg, "isSupport", stCfg.isSupport)) {
        ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "TofGroup", stCfg.isSupport);
        if (!stCfg.isSupport) return 0;
    } else return -1;

    int rslt = 0;

    TiXmlElement *pobjChild = pobjTofCfg->FirstChildElement();
    for (; pobjChild != nullptr; pobjChild = pobjChild->NextSiblingElement()) {
        CFG_PARAM_TOF_STRU stTofCfg;
        if (ReadTofCfg(pobjChild, stTofCfg)) {
            ST_LOG_ERR("Get <%s> Tof Cfg Element Fail.", stTofCfg.strName.c_str());
            rslt++;
        } else {
            if (stTofCfg.isSupport) stCfg.vstTof.push_back(std::move(stTofCfg));
        }
    }

    return rslt;
}

/**************************************************************************************
功能描述: 读取毫米波雷达配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadRadarDevCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RADAR_DEV_STRU &stRadarDevCfg)
{
    INT32 rslt = 0;

    rslt += GetAttribute(pobjCfgParam, "isSupport", stRadarDevCfg.isSupport);
    ST_LOG_INFO("%s Cfg Param: <isSupport> = %d.", pobjCfgParam->Value(), stRadarDevCfg.isSupport);
    if (!stRadarDevCfg.isSupport) return 0;

    TiXmlElement *pobjCollDetRegCfg = GetFirstChildElement(pobjCfgParam, "CollDetReg");
    if (pobjCollDetRegCfg == NULL) return -1;
    rslt += GetAttribute(pobjCollDetRegCfg, "RightBottomPoint_X", stRadarDevCfg.stDetectParam.rightBottomPointX);
    rslt += GetAttribute(pobjCollDetRegCfg, "RightBottomPoint_Y", stRadarDevCfg.stDetectParam.rightBottomPointY);
    rslt += GetAttribute(pobjCollDetRegCfg, "LeftUpPoint_X",      stRadarDevCfg.stDetectParam.leftUpPointX);
    rslt += GetAttribute(pobjCollDetRegCfg, "LeftUpPoint_Y",      stRadarDevCfg.stDetectParam.leftUpPointY);
    stRadarDevCfg.stDetectParam.maxObstCount = 63;

    TiXmlElement *pobjInstallParamCfg = GetFirstChildElement(pobjCfgParam, "InstallParam");
    if (pobjInstallParamCfg == NULL) return -1;
    rslt += GetAttribute(pobjInstallParamCfg, "Tx",   stRadarDevCfg.stInstallParam.tx);
    rslt += GetAttribute(pobjInstallParamCfg, "Ty",   stRadarDevCfg.stInstallParam.ty);
    rslt += GetAttribute(pobjInstallParamCfg, "Roll", stRadarDevCfg.stInstallParam.roll);
    rslt += GetAttribute(pobjInstallParamCfg, "Yaw",  stRadarDevCfg.stInstallParam.yaw);

    return rslt ;
}

/**************************************************************************************
功能描述: 读取所有毫米波雷达配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadRadarCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RADAR_STRU &stRadar)
{
    TiXmlElement *pobjRadarGroupCfg = GetFirstChildElement(pobjCfgParam, "RadarGroup");
    if (pobjRadarGroupCfg == NULL) return -1;

    stRadar.stLeftRadar.strRadarName = Radar[RADAR_ENUM::LEFT_RADAR];
    TiXmlElement *pobjLeftRadarCfg = GetFirstChildElement(pobjRadarGroupCfg, stRadar.stLeftRadar.strRadarName.c_str());
    if (pobjLeftRadarCfg == NULL)  return -1;
    INT32 rslt = 0;
    rslt += ReadRadarDevCfg(pobjLeftRadarCfg, stRadar.stLeftRadar);

    stRadar.stFrontRadar.strRadarName = Radar[RADAR_ENUM::FRONT_RADAR];
    TiXmlElement *pobjFrontRadarCfg = GetFirstChildElement(pobjRadarGroupCfg, stRadar.stFrontRadar.strRadarName.c_str());
    if (pobjFrontRadarCfg == NULL) return -1;
    rslt += ReadRadarDevCfg(pobjFrontRadarCfg, stRadar.stFrontRadar);

    stRadar.stRightRadar.strRadarName = Radar[RADAR_ENUM::RIGHT_RADAR];
    TiXmlElement *pobjRightRadarCfg = GetFirstChildElement(pobjRadarGroupCfg, stRadar.stRightRadar.strRadarName.c_str());
    if (pobjRightRadarCfg == NULL) return -1;
    rslt += ReadRadarDevCfg(pobjRightRadarCfg, stRadar.stRightRadar);

    stRadar.stBackRadar.strRadarName = Radar[RADAR_ENUM::BACK_RADAR];
    TiXmlElement *pobjBackRadarCfg = GetFirstChildElement(pobjRadarGroupCfg, stRadar.stBackRadar.strRadarName.c_str());
    if (pobjBackRadarCfg == NULL)  return -1;
    rslt += ReadRadarDevCfg(pobjBackRadarCfg, stRadar.stBackRadar);

    if (rslt != 0) {
        ST_LOG_ERR("Read Radar Group Config Fail.");
        return -1;
    }

    stRadar.stLeftRadar.stDetectParam.devID  = RADAR_ENUM::LEFT_RADAR;
    stRadar.stFrontRadar.stDetectParam.devID = RADAR_ENUM::FRONT_RADAR;
    stRadar.stRightRadar.stDetectParam.devID = RADAR_ENUM::RIGHT_RADAR;
    stRadar.stBackRadar.stDetectParam.devID  = RADAR_ENUM::BACK_RADAR;

    return 0;
}

/**************************************************************************************
功能描述: 读取rgb相机配置
修改记录:
**************************************************************************************/
void CFG_PARAM_READ_SENSOR_C::CameraMatrixSTR2DBL(string& strData, vector<double>& vData)
{
    istringstream iss(strData);
    string strRslt;
    while(getline(iss, strRslt, ',')) {
        vData.push_back(atof(strRslt.c_str()));
    }
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadUltrasioncsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ULTRASONICS_STRU &stCfg)
{
    TiXmlElement *pobjUsCfg = GetFirstChildElement(pobjCfgParam, "UltrasonicGroup");
    if (!pobjUsCfg) return -1;

    if (!GetAttribute(pobjUsCfg, "isSupport", stCfg.isSupport)) {
        ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "UltrasonicGroup", stCfg.isSupport);
        if (!stCfg.isSupport) return 0;
    } else return -1;

    INT32 rslt = 0;
    rslt += GetAttribute(pobjUsCfg, "devName",  stCfg.strDevName);
    rslt += GetAttribute(pobjUsCfg, "baudRate", stCfg.baudRate);
    if (0 == rslt) ST_LOG_INFO("<%s> Cfg Param: <devName> = %s, <baudRate> = %d.", "UltrasonicGroup", stCfg.strDevName.c_str(), stCfg.baudRate);

    TiXmlElement *pobjChild = pobjUsCfg->FirstChildElement();
    for (; pobjChild != nullptr; pobjChild = pobjChild->NextSiblingElement()) {
        CFG_PARAM_ULTRASONIC_STRU stUsCfg;
        if (ReadUsCfg(pobjChild, stUsCfg)) {
            ST_LOG_ERR("Get <%s> Ultrasonic Cfg Element Fail.\n", stUsCfg.strName.c_str());
            rslt++;
        } else {
            if (stUsCfg.isSupport) stCfg.vstUs.push_back(std::move(stUsCfg));
        }
    }

    return rslt;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadRgbCameraElement(TiXmlElement *pobjCfgParam, const char* element, vector<double>& vData, uint32_t num)
{
    TiXmlElement* pobjData = GetFirstChildElement(pobjCfgParam, element);
    if (!pobjData) return -1;

    string strName = pobjCfgParam->Value();
    string strData;
    if (GetAttribute(pobjData, "Data", strData)) {
        ST_LOG_ERR("Failed to read <%s-%s> params.", strName.c_str(), element);
        return -1;
    }

    CameraMatrixSTR2DBL(strData, vData);
    ST_LOG_INFO("<%s-%s> is:%s", strName.c_str(), element, strData.c_str());
    if (vData.size() != num) {
        ST_LOG_ERR("<%s-%s> error, params num is:%lu, not equal to %u.", strName.c_str(), element, vData.size(), num);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadRgbCameraCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RGB_CAMERA_STRU &stCameraCfg)
{
    stCameraCfg.strName = pobjCfgParam->Value();

    if (!IsValidSensor(RgbCamera, stCameraCfg.strName)) {
        ST_LOG_ERR("There is not exists the rgb camera <%s>.", stCameraCfg.strName.c_str());
        return -1;
    }

    if (GetAttribute(pobjCfgParam, "isSupport", stCameraCfg.isSupport)) {
        ST_LOG_ERR("Failed to read <%s> property params.", stCameraCfg.strName.c_str());
        return -1;
    }

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", stCameraCfg.strName.c_str(), stCameraCfg.isSupport);
    if (!stCameraCfg.isSupport) return 0;

    int rslt = 0;
    TiXmlElement* pobjProperty = GetFirstChildElement(pobjCfgParam, "Property");
    if (!pobjProperty) return -1;
    rslt += GetAttribute(pobjProperty, "PortName",  stCameraCfg.strPortName);
    rslt += GetAttribute(pobjProperty, "ImgHeight", stCameraCfg.imgHeight);
    rslt += GetAttribute(pobjProperty, "ImgWidth",  stCameraCfg.imgWidth);
    rslt += GetAttribute(pobjProperty, "FrameHz",   stCameraCfg.frameHz);

    TiXmlElement *pobjInstall = GetFirstChildElement(pobjCfgParam, "InstallParam");
    if (!pobjInstall) return -1;
    rslt += ReadInstallParam(pobjInstall, stCameraCfg.stInstallPose);
    rslt += ReadRgbCameraElement(pobjCfgParam, "CameraMatrix",           stCameraCfg.cameraMatrix,           9);
    rslt += ReadRgbCameraElement(pobjCfgParam, "DistortionCoefficients", stCameraCfg.distortionCoefficients, 5);

    ST_LOG_INFO("The <%s> camera install pose: x:%f y:%f z:%f roll:%f pitch:%f yaw:%f", stCameraCfg.strName.c_str(),
        stCameraCfg.stInstallPose.tx, stCameraCfg.stInstallPose.ty, stCameraCfg.stInstallPose.tz,
        stCameraCfg.stInstallPose.pitch, stCameraCfg.stInstallPose.roll, stCameraCfg.stInstallPose.yaw);

    return rslt;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadRgbCamerasCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RGB_CAMERAS_STRU &stCfg)
{
    TiXmlElement *pobjCameraCfg = GetFirstChildElement(pobjCfgParam, "CameraGroup");
    if (!pobjCameraCfg) return -1;

    int rslt = 0;
    TiXmlElement *pobjChild = pobjCameraCfg->FirstChildElement();
    for (; pobjChild != nullptr; pobjChild = pobjChild->NextSiblingElement()) {
        CFG_PARAM_RGB_CAMERA_STRU stCameraCfg;
        if (ReadRgbCameraCfg(pobjChild, stCameraCfg)) {
            ST_LOG_ERR("Get <%s> Cfg Element Fail.", stCameraCfg.strName.c_str());
            rslt++;
        } else {
            if (stCameraCfg.isSupport) stCfg.vstCamera.push_back(std::move(stCameraCfg));
        }
    }

    return rslt;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadPlaneFitParam(TiXmlElement *pobjCfgParam, CFG_PARAM_PLANE_FIT_STRU &stPlaneFitParam)
{
    INT32 rslt = 0;
    rslt += GetAttribute(pobjCfgParam, "Coeff_y", stPlaneFitParam.coeffY);
    rslt += GetAttribute(pobjCfgParam, "Coeff_z", stPlaneFitParam.coeffZ);
    rslt += GetAttribute(pobjCfgParam, "Const_h", stPlaneFitParam.constH);

    //ST_LOG_INFO("Coeff_y:%f Coeff_y:%f Coeff_y:%f", stHrzPlaneParam.coeffY, stHrzPlaneParam.coeffZ, stHrzPlaneParam.constH);

    return rslt;
}

/**************************************************************************************
功能描述: 读取车尾深度摄像头配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadBackDepthCameraCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_BACK_DEPTH_CAMERA_STRU &stBackDepthCameraCfg)
{
    string strCameraName = DepthCamera[D_CAMERA_ENUM::BACK_D_CAMERA];
    TiXmlElement *pobjBackDepthCameraCfg = GetFirstChildElement(pobjCfgParam, strCameraName.c_str());
    if (pobjBackDepthCameraCfg == NULL) {
        ST_LOG_ERR("Get <%s> Cfg Element Fail.", strCameraName.c_str());
        return -1;
    }

    if (TIXML_SUCCESS != pobjBackDepthCameraCfg->QueryBoolAttribute("isSupport", &stBackDepthCameraCfg.isSupport)) {
        ST_LOG_ERR("Get <%s> Cfg Attr <isSupport> Fail.", strCameraName.c_str());
        return -1;
    }

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", strCameraName.c_str(), stBackDepthCameraCfg.isSupport);
    if (!stBackDepthCameraCfg.isSupport) return 0;

    INT32 rslt = 0;
    TiXmlElement *pobjInstallParamCfg = GetFirstChildElement(pobjBackDepthCameraCfg, "InstallParam");
    if (pobjInstallParamCfg == NULL) return -1;
    rslt += GetAttribute(pobjInstallParamCfg, "Tx", stBackDepthCameraCfg.stInstallPos.tx);
    rslt += GetAttribute(pobjInstallParamCfg, "Ty", stBackDepthCameraCfg.stInstallPos.ty);

    TiXmlElement *pobjHrzPlaneParamCfg = GetFirstChildElement(pobjBackDepthCameraCfg, "HrzPlaneParam");
    if (pobjHrzPlaneParamCfg == NULL) return -1;
    rslt += ReadPlaneFitParam(pobjHrzPlaneParamCfg, stBackDepthCameraCfg.stPlaneFitParam);

    if (rslt != 0) {
        ST_LOG_ERR("Read <%s> Config Fail.", strCameraName.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取虚拟的定位激光雷达配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLocLidarCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LOC_LIDAR_STRU &stLocLidarCfg)
{
    TiXmlElement *pobjLocLidarCfg = pobjCfgParam->FirstChildElement("LocLidar");
    if (pobjLocLidarCfg == NULL) {
        ST_LOG_ERR("Get <LocLidar> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjLocLidarCfg->QueryBoolAttribute("isSupport", &stLocLidarCfg.isSupport)) {
        ST_LOG_ERR("Get <LocLidar> Cfg Attr <isSupport> Fail.");
        return -1;
    }

    TiXmlElement *pobjInstallParamCfg = GetFirstChildElement(pobjLocLidarCfg, "InstallParam");
    if (pobjInstallParamCfg == NULL)
        return -1;

    INT32 rslt = ReadInstallParam(pobjInstallParamCfg, stLocLidarCfg.stInstallPose);
    if (rslt != 0) {
        ST_LOG_ERR("Read LocLidar Install Pose Param Fail.");
        return -1;
    }

    ST_LOG_INFO("Cfg Param: <isSupport> = %d.", stLocLidarCfg.isSupport);
    return 0;
}

/**************************************************************************************
功能描述: 读取雷达基本属性
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarPropertyCfg(TiXmlElement *pobjLidarParam, const char *element, CFG_PARAM_LIDAR_PROPERTY_STRU &stProperty)
{
    stProperty.strLidarName = pobjLidarParam->Value();
    stProperty.strFrameId   = pobjLidarParam->Value();

    TiXmlElement *pobjProperty = GetFirstChildElement(pobjLidarParam, element);
    if (!pobjProperty) return -1;

    if (GetAttribute(pobjProperty, "LidarType", stProperty.strLidarType)) {
        ST_LOG_ERR("Failed to read <%s> property params.", stProperty.strLidarName.c_str());
        return -1;
    }

    if (GetAttribute(pobjProperty, "LidarSerialNum", stProperty.strLidarSerialNum)) {
        ST_LOG_ERR("Failed to read <%s> property params <LidarSerialNum>.", stProperty.strLidarName.c_str());
        return -1;
    }

    /* 速腾官方在最新的雷达驱动中将RSHELIOS_16改成了RSHELIOS_16P */
    if (stProperty.strLidarType == "RSHELIOS_16") stProperty.strLidarType = "RSHELIOS_16P";

    if (GetAttribute(pobjProperty, "IsSupportLoc", stProperty.isSupportLoc)) {
        ST_LOG_ERR("Failed to read <IsSupportLoc> property params.");
        return -1;
    }

    if (GetAttribute(pobjProperty, "IsSupportPnp", stProperty.isSupportPnp)) {
        ST_LOG_ERR("Failed to read <IsSupportPnp> property params.");
        return -1;
    }

    if (GetAttribute(pobjProperty, "IsSupportDustFilter", stProperty.isSupportDustFilter)) {
        ST_LOG_ERR("Failed to read <IsSupportDustFilter> property params.");
        return -1;
    }

    if (GetAttribute(pobjProperty, "IsSupportShadowFilter", stProperty.isSupportShadowFilter)) {
        ST_LOG_ERR("Failed to read <IsSupportShadowFilter> property params.");
        return -1;
    }
    if (GetAttribute(pobjProperty, "isSupportShadowLookupTableFilter", stProperty.isSupportShadowLookupTableFilter)) {
        ST_LOG_ERR("Failed to read <isSupportShadowLookupTableFilter> property params.");
        return -1;
    }

    if (!IsValidSensor(Lidar, stProperty.strLidarName)) {
        ST_LOG_ERR("There is not exists the lidar <%s>.", stProperty.strLidarName.c_str());
        return -1;
    }

    if (!IsValidSensor(LidarType, stProperty.strLidarType)) {
        ST_LOG_ERR("<%s> lidar type parameter <%s> error.", stProperty.strLidarName.c_str(), stProperty.strLidarType.c_str());
        return -1;
    }

    ST_LOG_INFO("The <%s> property params are as follows: lidarType(%s), frameId(%s), lidarSerialNum(%s), IsSupportLoc(%d), IsSupportPnp(%d), IsSupportDustFilter(%d).",
                stProperty.strLidarName.c_str(),
                stProperty.strLidarType.c_str(), stProperty.strFrameId.c_str(),
                stProperty.strLidarSerialNum.c_str(),
                stProperty.isSupportLoc, stProperty.isSupportPnp, stProperty.isSupportDustFilter);

    if (HasElement(BlindLidarType, stProperty.strLidarType) && !stProperty.isSupportPnp) {
        ST_LOG_ERR("%s未开启感知功能，请检查配置文件。", stProperty.strLidarName.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取雷达网路参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarNetworkCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_NET_STRU &stNetCfg)
{
    TiXmlElement *pobjNet = GetFirstChildElement(pobjLidarParam, element);
    if (!pobjNet) return -1;

    int rslt = 0;
    rslt += GetAttribute(pobjNet, "Ip",         stNetCfg.strIp);
    rslt += GetAttribute(pobjNet, "MsopPort",   stNetCfg.msopPort);
    rslt += GetAttribute(pobjNet, "DifopPort",  stNetCfg.difopPort);
    rslt += GetAttribute(pobjNet, "ImuPort",    stNetCfg.imuPort);

    if (!rslt) {
        ST_LOG_INFO("The <%s> network configuration is as follows: ip(%s), msop(%d), difop(%d), imu(%d).",
            strLidarName.c_str(), stNetCfg.strIp.c_str(), stNetCfg.msopPort, stNetCfg.difopPort, stNetCfg.imuPort);
    }

    return rslt;
}

/**************************************************************************************
功能描述: 读取雷达测距参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarDistParamCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_DIST_STRU &stDistCfg)
{
    TiXmlElement *pobjDist = GetFirstChildElement(pobjLidarParam, element);
    if (!pobjDist) return -1;

    if (GetAttribute(pobjDist, "MaxDist", stDistCfg.maxDist)) return -1;

    if (stDistCfg.maxDist < 5) {
        ST_LOG_ERR("The <%s> ranging parameters are set incorrectly, minDist=%f maxDist=%f.", strLidarName.c_str(), stDistCfg.minDist, stDistCfg.maxDist);
        return -1;
    }

    ST_LOG_INFO("The <%s> ranging parameters are as follows: minDist(%f), maxDist(%f).", strLidarName.c_str(), stDistCfg.minDist, stDistCfg.maxDist);

    return 0;
}

/**************************************************************************************
功能描述: 读取雷达安装位姿配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarInstallCfg(TiXmlElement *pobjLidarParam, const char *element, string& strFrameId, CFG_PARAM_INSTALL_STRU &stInstallParam)
{
    /* 若为室外清扫车，则从xml文件读取外参；否则，从urdf文件读取； */
    CAR_TYPE_PARSER_C::Ptr pobjCarTypeParser = CAR_TYPE_PARSER_C::GetInstance(); // 车体型号解析器

    if (pobjCarTypeParser->IsOutdoorSweeper() ||
        (!pobjCarTypeParser->IsOutdoorSweeper() && pobjCarTypeParser->GetRobotType()=="WH1-AV01")) {
        TiXmlElement *pobjInstall = GetFirstChildElement(pobjLidarParam, element);
        if (!pobjInstall) return -1;
        if (ReadInstallParam(pobjInstall, stInstallParam)) {
            ST_LOG_ERR("Read Lidar Install Cfg Param Error!");
            return -1;
        }
    } else {
        if (ReadLidarTFFromURDF(pobjCarTypeParser->GetRobotType(), strFrameId, stInstallParam)) {
            ST_LOG_ERR("Read Lidar Tf Cfg Param Error!");
            return -1;
        }
    }

    ST_LOG_INFO("%s: frameId[%s] xyz(%f %f %f) rpy(%f %f %f).",
                strFrameId.data(),
                strFrameId.data(),
                stInstallParam.tx,
                stInstallParam.ty,
                stInstallParam.tz,
                stInstallParam.roll,
                stInstallParam.pitch,
                stInstallParam.yaw);

    return 0;
}

INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarInstallOffsetCfg(TiXmlElement *pobjLidarParam, const char *element, string& strFrameId, CFG_PARAM_INSTALL_STRU &stInstallParam)
{
    CAR_TYPE_PARSER_C::Ptr pobjCarTypeParser = CAR_TYPE_PARSER_C::GetInstance(); // 车体型号解析器
    if (pobjCarTypeParser->IsOutdoorSweeper() ||
        (!pobjCarTypeParser->IsOutdoorSweeper() && pobjCarTypeParser->GetRobotType()=="WH1-AV01")) {
        //TiXmlElement *pobjInstall = GetNextSiblingElement(pobjLidarParam, element);
        TiXmlElement *pobjInstall = GetFirstChildElement(pobjLidarParam, element);

        if (!pobjInstall) return -1;
        if (ReadInstallParam(pobjInstall, stInstallParam)) {
            ST_LOG_ERR("Read Lidar Install-Offset Cfg Param Error!");
            return -1;
        }
    }
    else {
        if (ReadLidarTFFromURDF(pobjCarTypeParser->GetRobotType(), strFrameId, stInstallParam)) {
            ST_LOG_ERR("Read Lidar Tf-Offset Cfg Param Error!");
            return -1;
        }
    }
    ST_LOG_INFO("%s: frameId[%s](offset-param) xyz(%f %f %f) rpy(%f %f %f).",
                strFrameId.data(),
                strFrameId.data(),
                stInstallParam.tx,
                stInstallParam.ty,
                stInstallParam.tz,
                stInstallParam.roll,
                stInstallParam.pitch,
                stInstallParam.yaw);
    return 0;
}

/**************************************************************************************
功能描述: 从urdf文件中读取雷达tf
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarTFFromURDF(const std::string &carType, const std::string &laserFrame, CFG_PARAM_INSTALL_STRU &stInstallCfg)
{
    std::string base_frame = "base_link";
    boost::array<float, 3> rpy_value, xyz_value;

    // 读取指定joint的TF
    INT32 tfRet = objMacParamMng_.ReadGivenTFFromUrdf(carType,
                                                      base_frame, laserFrame,
                                                      rpy_value, xyz_value);
    if (tfRet < 0) {
        ST_LOG_ERR("读取lidar机器参数[TF]: 失败[%d].", tfRet);
        return tfRet;
    }

    stInstallCfg.tx = xyz_value[0];
    stInstallCfg.ty = xyz_value[1];
    stInstallCfg.tz = xyz_value[2];
    stInstallCfg.roll = RADIAN_TO_ANGLE(rpy_value[0]);
    stInstallCfg.pitch = RADIAN_TO_ANGLE(rpy_value[1]);
    stInstallCfg.yaw = RADIAN_TO_ANGLE(rpy_value[2]);
    return 0;
}

/**************************************************************************************
功能描述: 读取激光雷达激光扇区去除参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLaserSectionWipeCfg(TiXmlElement *pobjLidarParam, string& strLidarName, CFG_PARAM_LASER_SECTION_WIPE_STRU &stLaserSectionWipe)
{
    INT32 rslt = 0;
    rslt += GetAttribute(pobjLidarParam, "StartAngle", stLaserSectionWipe.startAngle);
    rslt += GetAttribute(pobjLidarParam, "EndAngle",   stLaserSectionWipe.endAngle);
    rslt += GetAttribute(pobjLidarParam, "MinDist",    stLaserSectionWipe.minDist);
    rslt += GetAttribute(pobjLidarParam, "MaxDist",    stLaserSectionWipe.maxDist);
    rslt += GetAttribute(pobjLidarParam, "MinHeight",  stLaserSectionWipe.minHeight);
    rslt += GetAttribute(pobjLidarParam, "MaxHeight",  stLaserSectionWipe.maxHeight);

    if (stLaserSectionWipe.startAngle > stLaserSectionWipe.endAngle) {
        ST_LOG_ERR("Section <%.2f~%.2f> error, please set the correct begin-end angle. Note: StartAngle <= EndAngle", stLaserSectionWipe.startAngle, stLaserSectionWipe.endAngle);
        return -1;
    }

    if (stLaserSectionWipe.minDist > stLaserSectionWipe.maxDist) {
        ST_LOG_ERR("Section <%.2f~%.2f> error, please set the correct min-max distance. Note: MinDist <= MaxDist", stLaserSectionWipe.minDist, stLaserSectionWipe.maxDist);
        return -1;
    }

    if (stLaserSectionWipe.minHeight > stLaserSectionWipe.maxHeight) {
        ST_LOG_ERR("Section <%.2f~%.2f> error, please set the correct min-max height. Note: MinHeight <= MaxHeight", stLaserSectionWipe.minHeight, stLaserSectionWipe.maxHeight);
        return -1;
    }

    ST_LOG_INFO("<%s> wipe section:%.2f~%.2f dist:%.2f~%.2f height:%.2f~%.2f", strLidarName.c_str(),
                stLaserSectionWipe.startAngle, stLaserSectionWipe.endAngle,
                stLaserSectionWipe.minDist,    stLaserSectionWipe.maxDist,
                stLaserSectionWipe.minHeight,  stLaserSectionWipe.maxHeight);
    return rslt;
}

/**************************************************************************************
功能描述: 读取安装在车中间的激光雷达激光扇区去除参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarSectionWipeCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_SECTION_WIPE_STRU &stLidarSectionWipe)
{
    TiXmlElement *pobjSection = pobjLidarParam->FirstChildElement(element);
    if (!pobjSection) {
        ST_LOG_INFO("The lidar <%s> does not support the section wiping function.", strLidarName.c_str());
        return 0;
    }

    int rslt = 0;
    CFG_PARAM_LASER_SECTION_WIPE_STRU stLaserSectionWipe;
    for(; pobjSection != nullptr; pobjSection = pobjSection->NextSiblingElement(element)) {
        rslt += ReadLaserSectionWipeCfg(pobjSection, strLidarName, stLaserSectionWipe);
        stLidarSectionWipe.vstLaserSectionWipe.push_back(stLaserSectionWipe);
    }

    if (rslt) {
        ST_LOG_ERR("Read lidar section wipe params error!");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取laser点云波束过滤参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLaserBeamWipeCfg(TiXmlElement *pobjBeams, CFG_PARAM_LASER_BEAM_WIPE_STRU &stLaserBeamWipe)
{
    INT32 rslt = 0;
    rslt += GetAttribute(pobjBeams, "BeginRing",   stLaserBeamWipe.beginRing);
    rslt += GetAttribute(pobjBeams, "EndRing",     stLaserBeamWipe.endRing);
    rslt += GetAttribute(pobjBeams, "BeginColumn", stLaserBeamWipe.beginColumn);
    rslt += GetAttribute(pobjBeams, "EndColumn",   stLaserBeamWipe.endColumn);

    if ((stLaserBeamWipe.beginRing > stLaserBeamWipe.endRing) || (stLaserBeamWipe.beginColumn > stLaserBeamWipe.endColumn)) {
        ST_LOG_ERR("Beam [%d~%d]<%d~%d> Error. Please Set the Correct Begin-End Beam Range.",
                   stLaserBeamWipe.beginRing, stLaserBeamWipe.endRing,
                   stLaserBeamWipe.beginColumn, stLaserBeamWipe.endColumn);
        return -1;
    }
    return rslt;
}

/**************************************************************************************
功能描述: 读取lidar点云波束过滤参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarBeamWipeCfg(TiXmlElement *pobjLidarParam, const char *element, string& strLidarName, CFG_PARAM_LIDAR_BEAM_WIPE_STRU &stLidarBeamWipe)
{
    TiXmlElement *pobjBeams = pobjLidarParam->FirstChildElement(element);
    if (pobjBeams == NULL) {
        stLidarBeamWipe.isSupport = false;
        ST_LOG_INFO("The lidar <%s> does not support the beam wiping function.", strLidarName.c_str());
        return 0;
    }

    stLidarBeamWipe.isSupport = true;

    int rslt = 0;
    CFG_PARAM_LASER_BEAM_WIPE_STRU stLaserBeamWipe;
    for (; pobjBeams != NULL; pobjBeams = pobjBeams->NextSiblingElement(element)) {
        rslt += ReadLaserBeamWipeCfg(pobjBeams, stLaserBeamWipe);
        stLidarBeamWipe.vstLaserBeamWipe.push_back(stLaserBeamWipe);
    }

    if (rslt) {
        ST_LOG_ERR("Read Lidar Laser Beam Wipe Cfg Param Error!");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取一个lidar配置信息
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_STRU &stLidarCfg)
{
    if (ReadLidarPropertyCfg(   pobjCfgParam, "Property",     stLidarCfg.stProperty)) return -1;
    if (ReadLidarNetworkCfg(    pobjCfgParam, "NetCfg",       stLidarCfg.stProperty.strLidarName, stLidarCfg.stNetParam)) return -1;
    if (ReadLidarDistParamCfg(  pobjCfgParam, "DistParam",    stLidarCfg.stProperty.strLidarName, stLidarCfg.stDistParam)) return -1;
    if (ReadLidarInstallCfg(    pobjCfgParam, "InstallParam", stLidarCfg.stProperty.strFrameId,   stLidarCfg.stInstallParam)) return -1;
    if (ReadLidarSectionWipeCfg(pobjCfgParam, "SectionWipe",  stLidarCfg.stProperty.strLidarName, stLidarCfg.stLidarSectionWipe)) return -1;
    if (ReadLidarBeamWipeCfg(   pobjCfgParam, "BeamWipe",     stLidarCfg.stProperty.strLidarName, stLidarCfg.stLidarBeamWipe)) return -1;

    return 0;
}

/**************************************************************************************
功能描述: 读取所有激光雷达配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_SENSOR_C::ReadLidarsCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDARS_STRU &stLidarsCfg)
{
    TiXmlElement *pobjLidars = GetFirstChildElement(pobjCfgParam, "LidarGroup");
    if (pobjLidars == NULL) {
        ST_LOG_ERR("Get <LidarGroup> Cfg Element Fail.");
        return -1;
    }

    CFG_PARAM_INSTALL_STRU stInstallOffsetParam;
    std::string strOffsetId = "Offset";
    if (0 != ReadLidarInstallOffsetCfg(pobjLidars, "Offset", strOffsetId, stInstallOffsetParam)){
        ST_LOG_ERR("读取激光器标定参数偏移量失败.");
        return -1;
    }
    stLidarsCfg.stInstallOffsetParam = stInstallOffsetParam;

    /* 遍历加载所有的雷达配置，若存在解析失败，则异常退出 */
    printf("\n");
    TiXmlElement *pobjChild = pobjLidars->FirstChildElement();
    for (; pobjChild != nullptr; pobjChild = pobjChild->NextSiblingElement()) {
        std::string strOffsetName = "Offset"; // 主激光雷达外参偏移
        std::string strLidarName = pobjChild->Value();
        if (strLidarName == strOffsetName){ // 跳过外参偏移
            ST_LOG_INFO("跳过主激光器外参项 <%s>", strLidarName.c_str());
            continue;
        }
        CFG_PARAM_LIDAR_STRU stLidarCfg;
        if (ReadLidarCfg(pobjChild, stLidarCfg)) {
            ST_LOG_ERR("Get <%s> Cfg Element Fail.", stLidarCfg.LidarName().c_str());
            return -1;
        } else {
            ST_LOG_INFO("<%s> is Supported.", stLidarCfg.LidarName().c_str());
            COOR_3D_POSE_STRU stLidarPose0(stLidarCfg.stInstallParam.tx,
                                           stLidarCfg.stInstallParam.ty,
                                           stLidarCfg.stInstallParam.tz,
                                           ANGLE_TO_RADIAN(stLidarCfg.stInstallParam.roll),
                                           ANGLE_TO_RADIAN(stLidarCfg.stInstallParam.pitch),
                                           ANGLE_TO_RADIAN(stLidarCfg.stInstallParam.yaw));
            COOR_3D_POSE_STRU stLidarPose1(stInstallOffsetParam.tx,
                                           stInstallOffsetParam.ty,
                                           stInstallOffsetParam.tz,
                                           ANGLE_TO_RADIAN(stInstallOffsetParam.roll),
                                           ANGLE_TO_RADIAN(stInstallOffsetParam.pitch),
                                           ANGLE_TO_RADIAN(stInstallOffsetParam.yaw));
            Eigen::Matrix4f mTf = stLidarPose1.GetTransform() * stLidarPose0.GetTransform();
            COOR_3D_POSE_STRU stLidarPose(mTf);
            stLidarCfg.stInstallParam.tx = stLidarPose.stPos.x;
            stLidarCfg.stInstallParam.ty = stLidarPose.stPos.y;
            stLidarCfg.stInstallParam.tz = stLidarPose.stPos.z;
            stLidarCfg.stInstallParam.roll  = RADIAN_TO_ANGLE(stLidarPose.stAtt.roll);
            stLidarCfg.stInstallParam.pitch = RADIAN_TO_ANGLE(stLidarPose.stAtt.pitch);
            stLidarCfg.stInstallParam.yaw   = RADIAN_TO_ANGLE(stLidarPose.stAtt.yaw);
            ST_LOG_INFO("%s(修正后): frameId[%s] xyz(%f %f %f) rpy(%f %f %f).",
                        stLidarCfg.LidarName().c_str(),stLidarCfg.stProperty.strFrameId.c_str(),
                        stLidarCfg.stInstallParam.tx,stLidarCfg.stInstallParam.ty,stLidarCfg.stInstallParam.tz,
                        stLidarCfg.stInstallParam.roll,stLidarCfg.stInstallParam.pitch,stLidarCfg.stInstallParam.yaw);
            stLidarsCfg.vstLidarsCfg.push_back(std::move(stLidarCfg));
        }
        printf("\n");
    }

    return 0;
}


}
}

