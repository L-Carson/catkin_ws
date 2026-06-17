#pragma once

#include "comm/comm_file.h"
#include "comm/comm_yaml.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_cfg.h"
#include "pcpt_comm/pcpt_comm_file.h"
#include <opencv2/opencv.hpp>
#include <visualization_msgs/MarkerArray.h>

using namespace std;

/**************************************************************************************
功能描述: 相机坐标变化
修改记录:
**************************************************************************************/
class PCPT_CAMERA_TF_C
{
public:
    PCPT_CAMERA_TF_C(){}
    PCPT_CAMERA_TF_C(ros::NodeHandle& nh, const CFG_PARAM_RGB_CAMERA_STRU &stCameraParam)
    : strCameraName_(stCameraParam.strName + "_camera")
    , stCameraParam_(stCameraParam)
    {
        string strTopic = "/pcpt/marker_" + strCameraName_ + "_vision_cone";
        pubMarker_ = nh.advertise<visualization_msgs::MarkerArray>(strTopic, 1);
    }

    ~PCPT_CAMERA_TF_C(){}

    int Init();
    cv::Point3f Pixel2Car(const cv::Point2f& imgPoint, float height = 0) const;
    cv::Point2f Car2Pixel(const cv::Point3f& pointByCar) const;

    void GetImageViewAngleByCamera(const cv::Rect &rect, CONE_STRU &stCone) const;
    void GetImageViewAngleByFrame(const cv::Rect &rect, const double depthByCamera, const cv::Mat &matTRByCar, CONE_STRU &stCone) const;
    void GetImageViewAngleByCar(const cv::Rect &rect, const double depthByCamera, CONE_STRU &stCone) const;

    void GetCameraViewAngle(float &viewH, float &viewV) const;
    void GetCameraViewAngleByCar(const float depthByCamera, CONE_STRU& stCone) const;

    const CFG_PARAM_RGB_CAMERA_STRU& GetCameraParam() const {return stCameraParam_;}

private:
    PCPT_FILE_MNG_C objFileMng_;
    string          strFilePath_;

    cv::Mat matIntr_;       //相机内参 3*3
    cv::Mat matIntrInv_;    //相机内参的逆矩阵 3*3
    cv::Mat matT_;          //相机外参：平移向量 3*1
    cv::Mat matR_;          //相机外参：旋转矩阵 3*3
    cv::Mat matTR_;         //相机外参：平移+旋转 4*4
    cv::Mat matTRInv_;      //相机外参：平移+旋转 4*4
    cv::Mat matInTf_;       //cam2car 内参变换 intrinsic 3*3
    cv::Mat matExTf_;       //cam2car 外参变化 extrinsic 3*1

    float cameraViewH_;     //相机水平视角
    float cameraViewV_;     //相机垂直视角

    ros::Publisher      pubMarker_;

    string                      strCameraName_;
    CFG_PARAM_RGB_CAMERA_STRU   stCameraParam_;

    int  InitFromYaml();
    int  InitFromCfg();
    void SetMatRT(cv::Mat& matR, cv::Mat& matT);
    void TransMatRT(cv::Mat& matRects, cv::Mat& matTvecs);
    void TransMatRT(const COOR_3D_POSE_STRU& stCarPoseByCamera);
    int  ParseParamsMat(string &strCameraParams);
    int  ParseParamsRT(string &strCameraParams);
    int  ConvertVector2Mat(const vector<double>& v, const int rows, cv::Mat &mat) const;
    void GetPixelAngleByCamera(const double imageX, const double imageY, float& angleH, float& angleV) const;
    void GetPixelAngleByCamera(const cv::Point2d &imagePoint, float& angleH, float& angleV) const;
    void TransPointByCamera2ByFrame(const cv::Point3d &pointByCamera, cv::Point3d &pointByCar, const cv::Mat &matTRByFrame) const;
    void TransPointByCamera2ByCar(const cv::Point3d &pointByCamera, cv::Point3d &pointByCar) const;
    void PubVisionCone(const CONE_STRU &stCone, const char* pNameSpace) const;
    void PubVisionConeByCar(const CONE_STRU &stCone) const;
};