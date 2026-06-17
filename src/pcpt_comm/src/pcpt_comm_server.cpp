#include "pcpt_comm/pcpt_comm_server.h"

INT32 PCD_NET_SERVER_C::Init()
{
    PCPT_CFG_C* pobjCfg = PCPT_CFG_C::GetInstance();
    const CFG_PARAM_C& objCfgParam = pobjCfg->GetCfgParam();

    isOpen_ = objCfgParam.IsSupportPcdNet();
    if (!isOpen_) return 0;

    comm_srvs::LidarInstallParam msg;
    CFG_PARAM_LIDARS_STRU stLidars = objCfgParam.GetLidarsCfgInfo();

    for (const CFG_PARAM_LIDAR_STRU &stLidar : stLidars.vstLidarsCfg) {
        msg.name  = stLidar.LidarName();
        msg.x     = stLidar.stInstallParam.tx;
        msg.y     = stLidar.stInstallParam.ty;
        msg.z     = stLidar.stInstallParam.tz;
        msg.roll  = stLidar.stInstallParam.roll;
        msg.pitch = stLidar.stInstallParam.pitch;
        msg.yaw   = stLidar.stInstallParam.yaw;
        vmsg_.push_back(msg);
    }

    isInitSucc_ = true;
    ST_LOG_INFO("Init Success.");
    return 0;
}

BOOL  PCD_NET_SERVER_C::SrvCallback(comm_srvs::PcdNet::Request &req, comm_srvs::PcdNet::Response &res)
{
    if (!isInitSucc_) {
        res.success = false;
        return false;
    }

    res.success = true;
    res.isOpen = isOpen_;
    res.lidarInstallParamArray = vmsg_;
    return true;
}

INT32 RGB_CAMERA_PARAM_SERVER_C::Init()
{
    PCPT_CFG_C* pobjCfg = PCPT_CFG_C::GetInstance();
    const CFG_PARAM_C& objCfgParam = pobjCfg->GetCfgParam();

    comm_srvs::cameraParam msg;
    CFG_PARAM_RGB_CAMERAS_STRU stCameras = objCfgParam.GetRgbCamerasCfg();
    for (const CFG_PARAM_RGB_CAMERA_STRU &stCamera : stCameras.vstCamera) {
        msg.name_id                 = stCamera.strName;
        msg.port_name               = stCamera.strPortName;
        msg.frame_hz                = stCamera.frameHz;
        msg.height                  = stCamera.imgHeight;
        msg.width                   = stCamera.imgWidth;
        msg.camera_matrix           = stCamera.cameraMatrix;
        msg.distortion_coefficients = stCamera.distortionCoefficients;
        msg.tx                      = stCamera.stInstallPose.tx;
        msg.ty                      = stCamera.stInstallPose.ty;
        msg.tz                      = stCamera.stInstallPose.tz;
        msg.roll                    = stCamera.stInstallPose.roll;
        msg.pitch                   = stCamera.stInstallPose.pitch;
        msg.yaw                     = stCamera.stInstallPose.yaw;
        vmsg_.push_back(msg);
    }

    isInitSucc_ = true;
    ST_LOG_INFO("Init Success.");
    return 0;
}

BOOL  RGB_CAMERA_PARAM_SERVER_C::SrvCallback(comm_srvs::camerasParam::Request &req, comm_srvs::camerasParam::Response &res)
{
    if (!isInitSucc_) {
        res.success = false;
        return false;
    }

    res.success = true;
    res.cameraParamArray = vmsg_;
    return true;
}


INT32 VISION_FUNC_SERVER_C::Init(void)
{
    PCPT_CFG_C* pobjCfg = PCPT_CFG_C::GetInstance();
    const CFG_PARAM_C& objCfgParam = pobjCfg->GetCfgParam();

    CFG_PARAM_VISION_FUNC_STRU stVisionCfg = objCfgParam.GetVisionFuncCfg();

    /* 获取视觉检测功能参数配置 */
    PCPT_VISION_FUNC_C* pobjVisionFunc = PCPT_VISION_FUNC_C::GetInstance();
    if (!pobjVisionFunc) {
        ST_LOG_ERR("Failed to Get PCPT_VISION_FUNC_C Instance.");
        return -1;
    }
    VISION_FUNC_STRU stVisionFunc = pobjVisionFunc->GetVisionFunc();

    Json::Value objJsonRes;
    objJsonRes["isUseApi"] = stVisionFunc.isUseApi;

    Json::Value objJsonVisionFuncs;
    if (stVisionFunc.stQRCode.isSupport){
        Json::Value objJsonCfg;
        objJsonCfg["funcId"]   = stVisionFunc.stQRCode.funcId;
        objJsonCfg["isUseIpu"] = stVisionFunc.stQRCode.isUseIpu;           
        objJsonCfg["hz"]      = stVisionFunc.stQRCode.hz;
        objJsonVisionFuncs.append(objJsonCfg);
    }

    if (stVisionFunc.stDet.stAttr.isSupport){
        Json::Value objJsonDetCfg;
        objJsonDetCfg["funcId"]   = stVisionFunc.stDet.stAttr.funcId;
        objJsonDetCfg["isUseIpu"] = stVisionFunc.stDet.stAttr.isUseIpu;           
        objJsonDetCfg["hz"]       = stVisionFunc.stDet.stAttr.hz;

        Json::Value objJsonLabels;
        for (const auto &label : stVisionCfg.stNetDet.vValidLabel) {
           Json::Value objJsonLabel;
           objJsonLabel["id"] = label;
           objJsonLabels.append(objJsonLabel);
        }

        if (objJsonLabels.size() > 0) {
            objJsonDetCfg["label"] = objJsonLabels;
        }
        objJsonVisionFuncs.append(objJsonDetCfg);
    }

    if (stVisionFunc.stSeg.stAttr.isSupport){
        Json::Value objJsonSegCfg;
        objJsonSegCfg["funcId"]   = stVisionFunc.stSeg.stAttr.funcId;
        objJsonSegCfg["isUseIpu"] = stVisionFunc.stSeg.stAttr.isUseIpu;           
        objJsonSegCfg["hz"]       = stVisionFunc.stSeg.stAttr.hz;

        Json::Value objJsonLabels;
        for (const auto &label : stVisionCfg.stNetSeg.vValidLabel) {
           Json::Value objJsonLabel;
           objJsonLabel["id"] = label;
           if (label == UINT32(TAG_THICKPIPE)) {
               Json::Value objJsonLabelAttr;
               objJsonLabelAttr["thickPipeWidth"] = stVisionCfg.stNetSeg.stLabelThickPipe.width;
               objJsonLabel["attribute"] = objJsonLabelAttr;
           }
           objJsonLabels.append(objJsonLabel);
        }

        if (objJsonLabels.size() > 0) {
            objJsonSegCfg["label"] = objJsonLabels;
        }
        objJsonVisionFuncs.append(objJsonSegCfg);
    }

    if (stVisionFunc.stTrafficLightDet.isSupport){
        Json::Value objJsonCfg;
        objJsonCfg["funcId"] = stVisionFunc.stTrafficLightDet.funcId;
        objJsonCfg["ipu"]    = stVisionFunc.stTrafficLightDet.isUseIpu;           \
        objJsonCfg["hz"]     = stVisionFunc.stTrafficLightDet.hz;
        objJsonVisionFuncs.append(objJsonCfg);
    }

    if (objJsonVisionFuncs.size() > 0) {
       objJsonRes["visionFuncCfg"] = objJsonVisionFuncs;
    }

    res_.success = true;
    res_.visionFuncCfg = objJsonRes.toStyledString();
    isInitSucc_ = true;

    ST_LOG_INFO("Init Success.");
    return 0;
}

bool  VISION_FUNC_SERVER_C::SrvCallback(comm_srvs::visionFunc::Request &req, comm_srvs::visionFunc::Response &res)
{
    if (!isInitSucc_) {
        res.success = false;
        return false;
    }

    res = res_;
    return true;
}

INT32 VISION_FUNC_SIM_SERVER_C::GetVisionFuncAttr(VISION_FUNC_ATTR_STRU& stFuncAttr)
{
    if (objServerValidTimer_.IsTimeOut()) return -1;
    if (stFuncAttr_.funcId == VISION_FUNC_ID_ENUM::VI_NULL_FUNC) return -1;
    stFuncAttr = stFuncAttr_;
    return 0;
}

BOOL  VISION_FUNC_SIM_SERVER_C::SrvCallback(comm_srvs::visionFuncSim::Request &req, comm_srvs::visionFuncSim::Response &res)
{
    stFuncAttr_.funcId   = VISION_FUNC_ID_ENUM(req.funcId);
    stFuncAttr_.isUseIpu = req.isUseIpu;
    stFuncAttr_.hz       = req.hz;
    stFuncAttr_.vstrCameraId.clear();
    stFuncAttr_.vstrCameraId.push_back(req.camera);

    objServerValidTimer_.Reset();

    res.success = true;
    return true;
}


INT32 PCPT_ROS_SERVER_C::Init(void)
{
    if (objPcdNetServer_.Init())         return -1;
    if (objRgbCameraParamServer_.Init()) return -1;
    if (objVisionFuncServer_.Init())     return -1;

    ST_LOG_INFO("Init Success.");
    return 0;
}


INT32 PCPT_ROS_SERVER_C::ClientTest(void)
{
    clientTest_.waitForExistence(); //等待服务启动成功
    comm_srvs::camerasParam srv;
    if (!clientTest_.call(srv)) {
        ST_LOG_ERR("Failed to call service.");
        return -1;
    }

    if (!srv.response.success) {
        ST_LOG_ERR("Parameter not initialized, please try again...");
        return 1;
    }

    ST_LOG_INFO("************************ camera param size:%lu", srv.response.cameraParamArray.size());
    for (const comm_srvs::cameraParam &camera : srv.response.cameraParamArray) {
        ST_LOG_INFO("name_id:%s",                                         camera.name_id.c_str());
        ST_LOG_INFO("port_name:%s",                                       camera.port_name.c_str());
        ST_LOG_INFO("frame_hz:%u",                                        camera.frame_hz);
        ST_LOG_INFO("image height:%u width:%u",                           camera.height, camera.width);
        ST_LOG_INFO("camera_matrix size:%lu",                             camera.camera_matrix.size());
        ST_LOG_INFO("distortion_coefficients size:%lu",                   camera.distortion_coefficients.size());
        ST_LOG_INFO("pose: x(%f) y(%f) z(%f) roll(%f) pitch(%f) yaw(%f)", camera.tx, camera.ty, camera.tz, camera.roll, camera.pitch, camera.yaw);
    }

    return 0;
}
