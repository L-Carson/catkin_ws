#ifndef __COMM_SRV_CAMERA_PARAM_H__
#define __COMM_SRV_CAMERA_PARAM_H__

#include "comm/comm_base.h"
#include "comm/comm_label.h"
#include "comm/comm_topic_name.h"
#include "comm_srvs/camerasParam.h"
#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <ros/ros.h>


struct CAMERA_PARAM_STRU
{
    std::string             strName;
    SENSOR_FACE_H_DIR_ENUM  enFaceDirH;
    SENSOR_FACE_V_DIR_ENUM  enFaceDirV;
    std::string             strDevPath;
    INT32                   imageWidth;
    INT32                   imageHeight;
    INT32                   frameRate;
    cv::Mat                 objIntrinsicMat;
    cv::Mat                 objDistCoeffMat;
    cv::Mat                 objRemap1;
    cv::Mat                 objRemap2;
    Eigen::Matrix4d         objTfMatOfV4C;

    CAMERA_PARAM_STRU(void)
    {
    }

    CAMERA_PARAM_STRU(const CAMERA_PARAM_STRU &stOther)
    {
        DepthCopyFrom(stOther);
    }

    const CAMERA_PARAM_STRU &operator=(const CAMERA_PARAM_STRU &stOther)
    {
        DepthCopyFrom(stOther);

        return *this;
    }

    void DepthCopyFrom(const CAMERA_PARAM_STRU &stOther)
    {
        strName         = stOther.strName;
        enFaceDirH      = stOther.enFaceDirH;
        enFaceDirV      = stOther.enFaceDirV;
        strDevPath      = stOther.strDevPath;
        imageWidth      = stOther.imageWidth;
        imageHeight     = stOther.imageHeight;
        frameRate       = stOther.frameRate;
        objIntrinsicMat = stOther.objIntrinsicMat.clone();
        objDistCoeffMat = stOther.objDistCoeffMat.clone();
        objRemap1       = stOther.objRemap1.clone();
        objRemap2       = stOther.objRemap2.clone();
        objTfMatOfV4C   = stOther.objTfMatOfV4C;
    }

    void Print(void) const
    {
        ST_LOG_INFO("Print Camera < %s > Begin.", strName.c_str());

        ST_LOG_INFO("DevPath: %s, face(%d, %d), %d * %d, %dHz.", strDevPath.c_str(), static_cast<int>(enFaceDirH), static_cast<int>(enFaceDirV), imageWidth, imageHeight, frameRate);
        Comm_PrintCvMatInfo("IntrinsicMat", objIntrinsicMat);
        Comm_PrintCvMatInfo("DistCoeffMat", objDistCoeffMat);
        Comm_PrintCvMatInfo("RemapMat1",    objRemap1);
        Comm_PrintCvMatInfo("RemapMat2",    objRemap2);
        Comm_PrintEgMatInfo("TfMatOfV4C",   objTfMatOfV4C);

        ST_LOG_INFO("Print Camera < %s > End.", strName.c_str());
    }
};

class CAMERA_PARAM_CLIENT_C
{
public:
    CAMERA_PARAM_CLIENT_C(ros::NodeHandle &nh)
    {
        objSrvsClient_ = nh.serviceClient<comm_srvs::camerasParam>(TOPIC_NAME_SERVER_RGB_CAMERA_PARAM);
    }

    /* 阻塞式等待 */
    INT32 WaitAndGet(std::vector<CAMERA_PARAM_STRU> &vectCamerasParam);

private:
    /* 此处不能用 TIMER_C 类       */
    ros::ServiceClient objSrvsClient_;

    INT32 WaitServiceAvailable(void);
    INT32 ConvertMsgCamera(const comm_srvs::cameraParam &objMsgCamera, CAMERA_PARAM_STRU &stCameraParam) const;
    INT32 ConvertMsgCameras(const std::vector<comm_srvs::cameraParam> &vectMsgCamaera, std::vector<CAMERA_PARAM_STRU> &vectCamerasParam) const;
    INT32 GetParam(std::vector<CAMERA_PARAM_STRU> &vectCamerasParam);
};



#endif
