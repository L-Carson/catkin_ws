
#include "maintain_param.h"

const RGB_CAMERA_CLBT_POS_STRU RGB_CAMERA_CLBT_PARAM_C::astCameraPos_[] =
{
    /*              front_left        front_mid_left    front_mid        front_mid_right    front_right        rear_left        rear_mid           rear_right       left_mid         right_mid */
    {CAR_TYPE_S310, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S320, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S323, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S324, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S325, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S326, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S330, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {-0.50, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S332, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {-0.50, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S333, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {-0.50, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S334, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {-0.50, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S335, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {-0.50, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S336, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S337, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S350, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, 0.0, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S520, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
    {CAR_TYPE_S530, { NAN, NAN, NAN}, { NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {  NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}, {NAN, NAN, NAN}},
};

/**************************************************************************************
功能描述: 修改配置文件
修改记录:
**************************************************************************************/
INT32 CLBT_PARAM_BASE_C::ModifyCfgParamFile(const std::function<INT32(TiXmlElement &)> &pfModifyCfg) const
{
    /* 此处修改的文件为修改版配置文件，生效还需要审核。 */
    const std::string strCfgParamFilePath = objCfgParamFileMng_.GetChangeCfgParamFilePath();
    ST_LOG_INFO("修改配置文件路径为: %s.", strCfgParamFilePath.c_str());

    TiXmlDocument objXmlCfgFile;
    INT32 rslt = objCfgParamFileMng_.ReadChangeCfgParamFile(strCfgParamFilePath, objXmlCfgFile);
    if (rslt != 0) {
        ST_LOG_ERR("加载配置文件失败。");
        return -1;
    }

    TiXmlElement *pobjCfgParam = objXmlCfgFile.RootElement();
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("objXmlCfgFile.RootElement() Fail.");
        return -1;
    }

    rslt = pfModifyCfg(*pobjCfgParam);
    if (rslt != 0) {
        ST_LOG_ERR("修改配置内容失败。");
        return -1;
    }

    rslt = objCfgParamFileMng_.SaveCfgParamFile(strCfgParamFilePath, objXmlCfgFile);
    if (rslt != 0) {
        ST_LOG_ERR("保存配置文件失败。");
        return -1;
    }

    ST_LOG_ERR("修改配置文件成功。");
    return 0;
}

/**************************************************************************************
功能描述: 获取相机的安装位置
修改记录:
**************************************************************************************/
int RGB_CAMERA_CLBT_PARAM_C::GetCameraInstallPos(const string& strModel, const string& strCameraName, COORDINATE_3D_STRU& stCameraPos)
{
    CAR_BODY_C objCarBody;
    std::unordered_map<string, CAR_TYPE_ENUM> umModelType; //<strModel - carType>
    objCarBody.GetCarModelTypeMap(umModelType);

    auto iter = umModelType.find(strModel);
    if (iter == umModelType.end()) return -1;
    CAR_TYPE_ENUM enCarType = iter->second;

    bool hasMatch = false;
    for (const RGB_CAMERA_CLBT_POS_STRU &stCameraClbtPos : astCameraPos_) {
        if (enCarType != stCameraClbtPos.enCarType) continue;
        hasMatch = true;

        int cameraId = ElementId(RgbCamera, strCameraName);
        if (cameraId == -1) return -1;
        switch(cameraId) {
            case RGB_CAMERA_ENUM::FRONT_LEFT:      stCameraPos = stCameraClbtPos.stFrontLeftCameraPos;     break;
            case RGB_CAMERA_ENUM::FRONT_MID_LEFT:  stCameraPos = stCameraClbtPos.stFrontMidLeftCameraPos;  break;
            case RGB_CAMERA_ENUM::FRONT_MID:       stCameraPos = stCameraClbtPos.stFrontMidCameraPos;      break;
            case RGB_CAMERA_ENUM::FRONT_MID_RIGHT: stCameraPos = stCameraClbtPos.stFrontMidRightCameraPos; break;
            case RGB_CAMERA_ENUM::FRONT_RIGHT:     stCameraPos = stCameraClbtPos.stFrontRightCameraPos;    break;
            case RGB_CAMERA_ENUM::REAR_LEFT:       stCameraPos = stCameraClbtPos.stRearLeftCameraPos;      break;
            case RGB_CAMERA_ENUM::REAR_MID:        stCameraPos = stCameraClbtPos.stRearMidCameraPos;       break;
            case RGB_CAMERA_ENUM::REAR_RIGHT:      stCameraPos = stCameraClbtPos.stRearRightCameraPos;     break;
            case RGB_CAMERA_ENUM::LEFT_MID:        stCameraPos = stCameraClbtPos.stLeftMidCameraPos;       break;
            case RGB_CAMERA_ENUM::RIGHT_MID:       stCameraPos = stCameraClbtPos.stRightMIdCameraPos;      break;
            default: return -1;
        }
    }

    if (!hasMatch) return -1;
    return 0;
}

/**************************************************************************************
功能描述: 服务响应
修改记录:
**************************************************************************************/
bool RGB_CAMERA_CLBT_PARAM_C::SrvCallback(comm_srvs::cameraCalibration::Request &req, comm_srvs::cameraCalibration::Response &res)
{
    auto pfModifyCfg = [&](TiXmlElement &objCfgRoot)
    {
        COMM::CFG::CFG_AMEND_C objCfgAmend;
        const char            *cameraName = req.camera_name.c_str();

        string strModel = "";
        if (objCfgAmend.GetAttribute(objCfgRoot, {"VehicleBody", "StructureInfo"}, "model", strModel)) return -1;

        ST_LOG_INFO("[%s-%s]相机标定位置参数: [x:%f y:%f z:%f]。", strModel.c_str(), cameraName, req.tx, req.ty, req.tz);

        COORDINATE_3D_STRU stCameraPos;
        if (0 == GetCameraInstallPos(strModel, req.camera_name, stCameraPos)) {
            ST_LOG_INFO("[%s-%s]相机预设位置参数: [x:%f y:%f z:%f]。", strModel.c_str(), cameraName, stCameraPos.x, stCameraPos.y, stCameraPos.z);
            if (std::isnan(stCameraPos.x)) stCameraPos.x = req.tx;
            if (std::isnan(stCameraPos.y)) stCameraPos.y = req.ty;
            if (std::isnan(stCameraPos.z)) stCameraPos.z = req.tz;
        } else {
            ST_LOG_INFO("[%s-%s]相机预设位置参数不存在。", strModel.c_str(), cameraName);
            stCameraPos.x = req.tx;
            stCameraPos.y = req.ty;
            stCameraPos.z = req.tz;
        }

        ST_LOG_INFO("[%s-%s]相机写入位置参数: [x:%f y:%f z:%f]。", strModel.c_str(), cameraName, stCameraPos.x, stCameraPos.y, stCameraPos.z);

        int rslt = 0;
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "CameraMatrix"},           "Data",  req.camera_matrix);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "DistortionCoefficients"}, "Data",  req.distortion_coefficients);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "InstallParam"},           "Tx",    stCameraPos.x);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "InstallParam"},           "Ty",    stCameraPos.y);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "InstallParam"},           "Tz",    stCameraPos.z);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "InstallParam"},           "Roll",  req.roll);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "InstallParam"},           "Pitch", req.pitch);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"CameraGroup", cameraName, "InstallParam"},           "Yaw",   req.yaw);
        if (rslt != 0) {
            ST_LOG_ERR("相机[%s]标定参数写入配置失败。", cameraName);
            return -1;
        }

        ST_LOG_INFO("相机[%s]标定参数写入配置成功。", cameraName);
        return 0;
    };

    INT32 rslt = ModifyCfgParamFile(pfModifyCfg);
    if (rslt != 0) {
        ST_LOG_ERR("修改配置文件失败。");
        res.success = false;
        return false;
    }

    ST_LOG_INFO("修改配置文件成功。");
    res.success = true;
    return true;
}

/**************************************************************************************
功能描述: 服务响应
修改记录:
**************************************************************************************/
bool LIDAR_CLBT_PARAM_C::SrvCallback(comm_srvs::lidarCalibration::Request &req, comm_srvs::lidarCalibration::Response &res)
{
    auto pfModifyCfg = [&](TiXmlElement &objCfgRoot)
    {
        COMM::CFG::CFG_AMEND_C objCfgAmend;

        for (auto &lidarParam : req.lidarsClbtParam) {
            const char *lidarName = lidarParam.name.c_str();
            std::string strLidarSerialNum = lidarParam.serialNum.c_str();

            int rslt = 0;
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "InstallParam"}, "Tx",    lidarParam.x);
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "InstallParam"}, "Ty",    lidarParam.y);
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "InstallParam"}, "Tz",    lidarParam.z);
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "InstallParam"}, "Roll",  Rad2Deg(lidarParam.roll));
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "InstallParam"}, "Pitch", Rad2Deg(lidarParam.pitch));
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "InstallParam"}, "Yaw",   Rad2Deg(lidarParam.yaw));
            rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", lidarName, "Property"}, "LidarSerialNum", strLidarSerialNum);
            if (rslt != 0) {
                ST_LOG_ERR("雷达[%s]标定参数写入配置失败。", lidarName);
                return -1;
            }

            ST_LOG_INFO("雷达[%s]标定参数写入配置成功。", lidarName);
        }

        ST_LOG_INFO("所有雷达标定参数写入配置成功。");
        return 0;
    };

    INT32 rslt = ModifyCfgParamFile(pfModifyCfg);
    if (rslt != 0) {
        ST_LOG_ERR("修改配置文件失败。");
        res.success = false;
        return false;
    }

    ST_LOG_INFO("修改配置文件成功。");
    res.success = true;
    return true;
}

bool IMU_CLBT_PARAM_C::SrvCallback(comm_srvs::imuCalibration::Request &req, comm_srvs::imuCalibration::Response &res)
{
    auto pfModifyCfg = [&](TiXmlElement &objCfgRoot)
    {
        COMM::CFG::CFG_AMEND_C objCfgAmend;
        auto                   imuParam = req.imuClbtParam;

        int rslt = 0;
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "InstallParam"}, "Tx",    imuParam.x);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "InstallParam"}, "Ty",    imuParam.y);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "InstallParam"}, "Tz",    imuParam.z);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "InstallParam"}, "Roll",  imuParam.roll);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "InstallParam"}, "Pitch", imuParam.pitch);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "InstallParam"}, "Yaw",   imuParam.yaw);
        if (rslt != 0) {
            ST_LOG_ERR("IMU标定参数写入配置失败。");
            return -1;
        }

        ST_LOG_INFO("IMU标定参数写入配置成功。");
        return 0;
    };

    INT32 rslt = ModifyCfgParamFile(pfModifyCfg);
    if (rslt != 0) {
        ST_LOG_ERR("修改配置文件失败。");
        res.success = false;
        return false;
    }

    ST_LOG_INFO("修改配置文件成功。");
    res.success = true;
    return true;
}

/**************************************************************************************
功能描述: 服务响应
修改记录:
**************************************************************************************/
bool LIDAR_OFFSET_CLBT_PARAM_C::SrvCallback(comm_srvs::lidarOffsetCalibration::Request &req, comm_srvs::lidarOffsetCalibration::Response &res)
{
    auto pfModifyCfg = [&](TiXmlElement &objCfgRoot)
    {
        COMM::CFG::CFG_AMEND_C objCfgAmend;
        const char            *lidarName = req.lidarOffsetParam.name.c_str();

        int rslt = 0;
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", "Offset"}, "Tx",    req.lidarOffsetParam.x);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", "Offset"}, "Ty",    req.lidarOffsetParam.y);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", "Offset"}, "Tz",    req.lidarOffsetParam.z);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", "Offset"}, "Roll",  Rad2Deg(req.lidarOffsetParam.roll));
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", "Offset"}, "Pitch", Rad2Deg(req.lidarOffsetParam.pitch));
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"LidarGroup", "Offset"}, "Yaw",   Rad2Deg(req.lidarOffsetParam.yaw));
        if (rslt != 0) {
            ST_LOG_ERR("雷达[%s]标定参数偏移量写入配置失败。", lidarName);
            return -1;
        }

        ST_LOG_INFO("雷达[%s]标定参数偏移量写入配置成功。", lidarName);
        return 0;
    };

    INT32 rslt = ModifyCfgParamFile(pfModifyCfg);
    if (rslt != 0) {
        ST_LOG_ERR("修改配置文件失败。");
        res.success = false;
        return false;
    }

    ST_LOG_INFO("修改配置文件成功。");
    res.success = true;
    return true;
}

bool IMU_OFFSET_CLBT_PARAM_C::SrvCallback(comm_srvs::imuOffsetCalibration::Request &req, comm_srvs::imuOffsetCalibration::Response &res)
{
    auto pfModifyCfg = [&](TiXmlElement &objCfgRoot)
    {
        COMM::CFG::CFG_AMEND_C objCfgAmend;

        int rslt = 0;
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "Offset"}, "Tx",    req.imuOffsetParam.x);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "Offset"}, "Ty",    req.imuOffsetParam.y);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "Offset"}, "Tz",    req.imuOffsetParam.z);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "Offset"}, "Roll",  req.imuOffsetParam.roll);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "Offset"}, "Pitch", req.imuOffsetParam.pitch);
        rslt += objCfgAmend.SetAttribute(objCfgRoot, {"Imu", "Offset"}, "Yaw",   req.imuOffsetParam.yaw);
        if (rslt != 0) {
            ST_LOG_ERR("Imu标定参数偏移量写入配置失败");
            return -1;
        }

        ST_LOG_INFO("Imu标定参数偏移量写入配置成功");
        return 0;
    };

    INT32 rslt = ModifyCfgParamFile(pfModifyCfg);
    if (rslt != 0) {
        ST_LOG_ERR("修改配置文件失败。");
        res.success = false;
        return false;
    }

    ST_LOG_INFO("修改配置文件成功。");
    res.success = true;
    return true;
}

void CFG_PARAM_FILE_MONITOR_C::ShowCfgParam(const std::string &strCfgFilePath)
{
    std::ifstream xmlFile(strCfgFilePath);
    if (!xmlFile.is_open()) {
        ST_LOG_ERR("打开文件 %s 失败.", strCfgFilePath.c_str());
        return;
    }

    std::string strLine;
    std::vector<std::string> vstrLines;
    while (getline(xmlFile, strLine)) {
        vstrLines.push_back(strLine);
    }

    std::cout << "------------cfg_param.xml内容-------------" << std::endl<< std::endl;

    for (const auto &i : vstrLines)
        std::cout << i << std::endl;

    std::cout << "------------------------------------------" << std::endl<< std::endl;

    xmlFile.close();
}

void CFG_PARAM_FILE_MONITOR_C::MonitorThreadFunc(void)
{
    // 获取文件描述符, 以只写方式
    INT32 fileDescriptor = open(strFilePath_.c_str(), O_WRONLY);

    // 文件描述符有效判断
    if (fileDescriptor == -1) {
        ST_LOG_ERR("打开文件 %s 失败.", strFilePath_.c_str());
        return;
    }

    time_t stLastModifyTime = time_t(0);
    while (ros::ok()) {
        // 监测文件是否被修改，默认未修改
        BOOL isFileChange = false;

        // stat定义在stst.h中，这里有声明和定义双重作用
        struct stat stFileStat;
        // 这里是调用了stat函数，与上面的stat意义不一样
        if (stat(strFilePath_.c_str(), &stFileStat) == 0) {
            // 获取当前时间
            time_t stModifyTime = stFileStat.st_mtime;
            if (stModifyTime != stLastModifyTime) {
                stLastModifyTime = stModifyTime;
                isFileChange = true; // 文件已经被修改
            }
        }

        if (isFileChange) {
            if (fdatasync(fileDescriptor) == -1) {
                ST_LOG_ERR("同步文件到磁盘失败.");
            }else{
                ST_LOG_INFO("同步文件到磁盘成功.");
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    close(fileDescriptor);
}

void EXTRI_PUBLISHER_C::PubExtriParamTopic(void)
{
    ros::Rate objLoopRate(1.0);

    CFG_PARAM_C objCfgParam;
    if (0 != objCfgParam.Init()) {
        ST_LOG_ERR("objCfgParam.Init() 失败");
        return ;
    }

    while (ros::ok()) {
        comm_msg::roborExtriParam stMsg;

        // 获取gnss数据
        stMsg.gnss.isSupport = objCfgParam.IsSupportGnss();
        CFG_PARAM_INSTALL_STRU gnssExtrxParam = objCfgParam.GetGnssInstallPose();
        stMsg.gnss.x = gnssExtrxParam.tx;
        stMsg.gnss.y = gnssExtrxParam.ty;
        stMsg.gnss.z = gnssExtrxParam.tz;
        stMsg.gnss.roll  = gnssExtrxParam.roll;
        stMsg.gnss.pitch = gnssExtrxParam.pitch;
        stMsg.gnss.yaw   = gnssExtrxParam.yaw;

        // 获取imu数据
        stMsg.imu.isSupport = objCfgParam.IsSupportImu();
        CFG_PARAM_INSTALL_STRU imuExtriParam = objCfgParam.GetImuInstallPose();
        stMsg.imu.x = imuExtriParam.tx;
        stMsg.imu.y = imuExtriParam.ty;
        stMsg.imu.z = imuExtriParam.tz;
        stMsg.imu.roll  = imuExtriParam.roll;
        stMsg.imu.pitch = imuExtriParam.pitch;
        stMsg.imu.yaw   = imuExtriParam.yaw;

        // 获取lidar数据
        stMsg.lidar.isSupport = objCfgParam.IsSupportLocLidar();
        CFG_PARAM_INSTALL_STRU lidarExtriParam = objCfgParam.GetLocLidarInstallPose();
        stMsg.lidar.x = lidarExtriParam.tx;
        stMsg.lidar.y = lidarExtriParam.ty;
        stMsg.lidar.z = lidarExtriParam.tz;
        stMsg.lidar.roll = lidarExtriParam.roll;
        stMsg.lidar.pitch = lidarExtriParam.pitch;
        stMsg.lidar.yaw = lidarExtriParam.yaw;

        // 发布topic
        pub_.publish(stMsg);

        // 等待直到下一个发布周期
        objLoopRate.sleep();
    }
}

