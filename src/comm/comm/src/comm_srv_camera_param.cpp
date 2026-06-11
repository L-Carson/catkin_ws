
#include "comm_srv_camera_param.h"


INT32 CAMERA_PARAM_CLIENT_C::WaitAndGet(std::vector<CAMERA_PARAM_STRU> &vectCamerasParam)
{
    ST_LOG_INFO("Begin...");

    INT32 rslt= WaitServiceAvailable();
    if (rslt != 0) {
        ST_LOG_ERR("WaitServiceAvailable() Fail.");
        return -1;
    }

    rslt = GetParam(vectCamerasParam);
    if (rslt != 0) {
        ST_LOG_ERR("GetParam() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 CAMERA_PARAM_CLIENT_C::WaitServiceAvailable(void)
{
    ST_LOG_INFO("Begin Waiting.");

    UINT32 waitTime      = 0;
    BOOL   isServiceAvailable = false;
    while (ros::ok() && !isServiceAvailable)
    {
        ++waitTime;

        isServiceAvailable = objSrvsClient_.waitForExistence(ros::Duration(1.0));
        ST_LOG_ERR_SAMPLE_IF_TRUE(!isServiceAvailable && (waitTime >= 10), 10, "Waiting %us. Service Not Available Now.", waitTime);
    }

    if (!isServiceAvailable) {
        ST_LOG_ERR("Waiting End With Exception.");
        return -1;
    }

    ST_LOG_INFO("Wait Succ.");
    return 0;
}

INT32 CAMERA_PARAM_CLIENT_C::ConvertMsgCamera(const comm_srvs::cameraParam &objMsgCamera, CAMERA_PARAM_STRU &stCameraParam) const
{
    ST_LOG_INFO("Begin Convert Camera < %s > Msg.", objMsgCamera.name_id.c_str());

    R_ASSERT_1(objMsgCamera.camera_matrix.size() == 9,           -1, UINT32(objMsgCamera.camera_matrix.size()));
    R_ASSERT_1(objMsgCamera.distortion_coefficients.size() == 5, -1, UINT32(objMsgCamera.distortion_coefficients.size()));

    const RGB_CAMERA_BASE_INFO_STRU *pstCameraBaseInfo = Comm_GetCameraBaseInfoByName(objMsgCamera.name_id);
    R_ASSERT(pstCameraBaseInfo != nullptr, -1);

    stCameraParam.strName         = objMsgCamera.name_id;
    stCameraParam.enFaceDirH      = pstCameraBaseInfo->enFaceDirH;
    stCameraParam.enFaceDirV      = pstCameraBaseInfo->enFaceDirV;
    stCameraParam.strDevPath      = objMsgCamera.port_name;
    stCameraParam.imageWidth      = objMsgCamera.width;
    stCameraParam.imageHeight     = objMsgCamera.height;
    stCameraParam.frameRate       = objMsgCamera.frame_hz;

    /* 处理内参，mat需要float数据类型，此处不要用vector构造Mat，有坑 */
    stCameraParam.objIntrinsicMat = (cv::Mat_<float>(3, 3) << objMsgCamera.camera_matrix[0], objMsgCamera.camera_matrix[1], objMsgCamera.camera_matrix[2],
                                                              objMsgCamera.camera_matrix[3], objMsgCamera.camera_matrix[4], objMsgCamera.camera_matrix[5],
                                                              objMsgCamera.camera_matrix[6], objMsgCamera.camera_matrix[7], objMsgCamera.camera_matrix[8]);

    stCameraParam.objDistCoeffMat = (cv::Mat_<float>(5, 1) << objMsgCamera.distortion_coefficients[0],
                                                              objMsgCamera.distortion_coefficients[1],
                                                              objMsgCamera.distortion_coefficients[2],
                                                              objMsgCamera.distortion_coefficients[3],
                                                              objMsgCamera.distortion_coefficients[4]);

    cv::initUndistortRectifyMap(stCameraParam.objIntrinsicMat,
                                stCameraParam.objDistCoeffMat,
                                cv::Mat(),
                                cv::Mat(),
                                cv::Size(stCameraParam.imageWidth, stCameraParam.imageHeight),
                                0,
                                stCameraParam.objRemap1,
                                stCameraParam.objRemap2);

    /* 处理外参，合成转换矩阵 */
    /* 此处摄像头外参为车体对于摄像头的旋转和平移 */
    Eigen::AngleAxisd rx(objMsgCamera.roll,  Vector3d::UnitX());
    Eigen::AngleAxisd ry(objMsgCamera.pitch, Vector3d::UnitY());
    Eigen::AngleAxisd rz(objMsgCamera.yaw,   Vector3d::UnitZ());
    Eigen::Matrix3d rotate = (rz * ry * rx) * Matrix3d::Identity();
    Eigen::Vector3d trans  = Eigen::Vector3d(objMsgCamera.tx, objMsgCamera.ty, objMsgCamera.tz);

    stCameraParam.objTfMatOfV4C << rotate(0, 0), rotate(0, 1), rotate(0, 2), trans[0],
                                   rotate(1, 0), rotate(1, 1), rotate(1, 2), trans[1],
                                   rotate(2, 0), rotate(2, 1), rotate(2, 2), trans[2],
                                            0.0,          0.0,          0.0,      1.0;

    stCameraParam.Print();
    ST_LOG_INFO("< %s > Succ.", stCameraParam.strName.c_str());
    return 0;
}

INT32 CAMERA_PARAM_CLIENT_C::ConvertMsgCameras(const std::vector<comm_srvs::cameraParam> &vectMsgCamaera, std::vector<CAMERA_PARAM_STRU> &vectCamerasParam) const
{
    ST_LOG_INFO("Begin Convert Msg Cameras Param.");

    vectCamerasParam.clear();
    vectCamerasParam.reserve(vectMsgCamaera.size());

    for (const comm_srvs::cameraParam &objMsgCamera : vectMsgCamaera) {
        CAMERA_PARAM_STRU stCameraParam;

        INT32 rslt = ConvertMsgCamera(objMsgCamera, stCameraParam);
        if (rslt != 0) {
            ST_LOG_INFO("ConvertMsgCamera() Fail.");
            return -1;
        }

        vectCamerasParam.emplace_back(stCameraParam);
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 CAMERA_PARAM_CLIENT_C::GetParam(std::vector<CAMERA_PARAM_STRU> &vectCamerasParam)
{
    ST_LOG_INFO("Begin Calling Service.");

    comm_srvs::camerasParam::Request  stReq;                /* 为空 */
    comm_srvs::camerasParam::Response stRsp;
    BOOL isCallSucc = objSrvsClient_.call(stReq, stRsp);     /* 阻塞式，无限时间等待响应 */
    if (!isCallSucc) {
        ST_LOG_ERR("Call Service Fail.");
        return -1;
    }

    ST_LOG_INFO("Call Service Succ.");

    if (!stRsp.success) {
        ST_LOG_ERR("Service Proc Fail.");
        return -1;
    }

    ST_LOG_INFO("Service Proc Succ.");

    INT32 rslt = ConvertMsgCameras(stRsp.cameraParamArray, vectCamerasParam);
    if (rslt != 0) {
        ST_LOG_ERR("ConvertMsgCameras() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}




