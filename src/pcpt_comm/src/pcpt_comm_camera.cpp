#include "pcpt_comm/pcpt_comm_camera.h"

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int PCPT_CAMERA_TF_C::Init()
{
#if 0
    if (InitFromYaml()) return -1;
#else
    if (InitFromCfg()) return -1;
#endif

    /* 获取相机视角等信息 */
    GetCameraViewAngle(cameraViewH_, cameraViewV_);

    ST_LOG_INFO("Init Success.");
    return 0;
}

/**************************************************************************************
功能描述: 像素坐标到车体坐标
修改记录:
**************************************************************************************/
cv::Point3f PCPT_CAMERA_TF_C::Pixel2Car(const cv::Point2f& imgPoint, float height) const
{
    /* 计算深度估计 */
    cv::Mat matImgPoint = (cv::Mat_<double>(3,1) << imgPoint.x, imgPoint.y, 1);
    cv::Mat matOut = matInTf_ * matImgPoint;
    double scale = (height + matExTf_.at<double>(2,0)) / matOut.at<double>(2,0);

    /* 计算车体坐标 */
    cv::Mat matPointByCar = matOut * scale - matExTf_;
    cv::Point3f result(matPointByCar.at<double>(0,0), matPointByCar.at<double>(1,0), matPointByCar.at<double>(2,0));

    return result;
}

/**************************************************************************************
功能描述: 车体坐标转像素坐标
修改记录:
**************************************************************************************/
cv::Point2f PCPT_CAMERA_TF_C::Car2Pixel(const cv::Point3f& pointByCar) const
{
    /* 旋转 & 平移 */
    cv::Mat pointsByCarMat = (cv::Mat_<double>(3,1) << pointByCar.x, pointByCar.y, pointByCar.z);
    cv::Mat cameraPoint = matR_ * pointsByCarMat;
    cameraPoint = cameraPoint + matT_;

    /* 乘上内参 */
    cv::Mat A = matIntr_ * cameraPoint;

    double u = A.at<double>(0,0);
    double v = A.at<double>(1,0);
    double d = A.at<double>(2,0);
    u = u / d;
    v = v / d;

    cv::Point2f result(u, v);

    return result;
}

/**************************************************************************************
功能描述: 根据矩形框计算相机坐标系下图片视角
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::GetImageViewAngleByCamera(const cv::Rect &rect, CONE_STRU &stCone) const
{
    cv::Point2d imagePoint(rect.x, rect.y);
    GetPixelAngleByCamera(imagePoint, stCone.angleH_min, stCone.angleV_min);

    imagePoint.x += rect.width;
    imagePoint.y += rect.height;
    GetPixelAngleByCamera(imagePoint, stCone.angleH_max, stCone.angleV_max);

    stCone.stOrigin.x = matTRInv_.at<double>(0, 3);
    stCone.stOrigin.y = matTRInv_.at<double>(1, 3);
    stCone.stOrigin.z = matTRInv_.at<double>(2, 3);
}

/**************************************************************************************
功能描述: 获取指定坐标系下指定深度的图片视角
           注意：cone的原点相对于与车体坐标系
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::GetImageViewAngleByFrame(const cv::Rect &rect, const double depthByCamera, const cv::Mat &matTRByCar, CONE_STRU &stCone) const
{
    CONE_STRU stConeByCamera;
    GetImageViewAngleByCamera(rect, stConeByCamera);

    cv::Mat matTR = matTRByCar.inv() * matTRInv_; //camera -> car -> frame

    cv::Point3d pointByCamera;
    cv::Point3d ltPointByFrame, lbPointByFrame, rtPointByFrame, rbPointByFrame; //left right top bottom

#define BOUNDRY_POINT_BY_CAMERA_TO_BY_FRAME(angleH, angleV, pointByFrame)   \
    pointByCamera.x = tan(angleH) * depthByCamera;                          \
    pointByCamera.y = tan(angleV) * depthByCamera;                          \
    pointByCamera.z = depthByCamera;                                        \
    TransPointByCamera2ByFrame(pointByCamera, pointByFrame, matTR);         \

    BOUNDRY_POINT_BY_CAMERA_TO_BY_FRAME(stConeByCamera.angleH_min, stConeByCamera.angleV_min, ltPointByFrame);
    BOUNDRY_POINT_BY_CAMERA_TO_BY_FRAME(stConeByCamera.angleH_min, stConeByCamera.angleV_max, lbPointByFrame);
    BOUNDRY_POINT_BY_CAMERA_TO_BY_FRAME(stConeByCamera.angleH_max, stConeByCamera.angleV_min, rtPointByFrame);
    BOUNDRY_POINT_BY_CAMERA_TO_BY_FRAME(stConeByCamera.angleH_max, stConeByCamera.angleV_max, rbPointByFrame);

    double minX(DBL_MAX), minY(DBL_MAX), minZ(DBL_MAX), maxX(-DBL_MAX), maxY(-DBL_MAX), maxZ(-DBL_MAX);

#define BOUNDRY_VALUE_BY_FRAME(point)   \
    minX = MIN(minX, point.x);          \
    minY = MIN(minY, point.y);          \
    minZ = MIN(minZ, point.z);          \
    maxX = MAX(maxX, point.x);          \
    maxY = MAX(maxY, point.y);          \
    maxZ = MAX(maxZ, point.z);          \

    BOUNDRY_VALUE_BY_FRAME(ltPointByFrame);
    BOUNDRY_VALUE_BY_FRAME(lbPointByFrame);
    BOUNDRY_VALUE_BY_FRAME(rtPointByFrame);
    BOUNDRY_VALUE_BY_FRAME(rbPointByFrame);

    stCone.angleH_min = FLT_MAX;
    stCone.angleH_max = -FLT_MAX;

#define BOUNDRY_ANGLE_BY_FRAME(angle)                   \
    stCone.angleH_min = MIN(stCone.angleH_min, angle);  \
    stCone.angleH_max = MAX(stCone.angleH_max, angle);  \

    BOUNDRY_ANGLE_BY_FRAME(atan2(minY, minX));
    BOUNDRY_ANGLE_BY_FRAME(atan2(minY, maxX));
    BOUNDRY_ANGLE_BY_FRAME(atan2(maxY, minX));
    BOUNDRY_ANGLE_BY_FRAME(atan2(maxY, maxX));

    stCone.angleV_min = atan(minZ / hypot(maxX, maxY));
    stCone.angleV_max = atan(maxZ / hypot(minX, minY));

    stCone.stOrigin.x = matTRByCar.at<double>(0, 3);
    stCone.stOrigin.y = matTRByCar.at<double>(1, 3);
    stCone.stOrigin.z = matTRByCar.at<double>(2, 3);
}

/**************************************************************************************
功能描述: 获取车体坐标系下指定深度的图片视角
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::GetImageViewAngleByCar(const cv::Rect &rect, const double depthByCamera, CONE_STRU &stCone) const
{
    cv::Mat matTR = cv::Mat::eye(4, 4, CV_64FC1);
    GetImageViewAngleByFrame(rect, depthByCamera, matTR, stCone);
    PubVisionConeByCar(stCone);
}

/**************************************************************************************
功能描述: 根据相机内参计算相机视角
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::GetCameraViewAngle(float &viewH, float &viewV) const
{
    CONE_STRU stConeByCamera;
    cv::Rect rect(0, 0, PIXEL_COL_1280, PIXEL_ROW_720);
    GetImageViewAngleByCamera(rect, stConeByCamera);

    float midAngleH(0), midAngleV(0);
    GetPixelAngleByCamera(0.5 * PIXEL_COL_1280, 0.5 * PIXEL_ROW_720, midAngleH, midAngleV);

    viewH = stConeByCamera.GetViewAngleH();
    viewV = stConeByCamera.GetViewAngleV();
}

/**************************************************************************************
功能描述: 获取车体坐标系下指定深度depth的相机视角
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::GetCameraViewAngleByCar(const float depthByCamera, CONE_STRU& stCone) const
{
    cv::Rect rect(0, 0, PIXEL_COL_1280, PIXEL_ROW_720);
    GetImageViewAngleByCar(rect, depthByCamera, stCone);
}

/**************************************************************************************
功能描述: 从yaml文件初始化
修改记录:
**************************************************************************************/
int PCPT_CAMERA_TF_C::InitFromYaml()
{
    /* 获取相机参数文件路径 */
    string strCameraParams("");                         //相机参数：内参、外参、畸变等

    objFileMng_.GetCameraConfigDir(strFilePath_);
    strCameraParams = strFilePath_ + strCameraName_ + ".yaml";

    if (!objFileMng_.IsFileExist(strCameraParams.c_str())) {
        ST_LOG_ERR("The camera parameter file does not exist: %s.", strCameraParams.c_str());
        return 1;
    }

#if 0   //W2
    int rslt = ParseParamsMat(strCameraParams);
#else   //Sweeper
    int rslt = ParseParamsRT(strCameraParams);
#endif

    if (rslt) {
        ST_LOG_ERR("Parsing camera <%s> parameters fail.", strCameraName_.c_str());
        return -1;
    } else {
        ST_LOG_INFO("Parsing camera <%s> parameters success.", strCameraName_.c_str());
    }

    return 0;
}

/**************************************************************************************
功能描述: 从参数配置初始化
修改记录:
**************************************************************************************/
int PCPT_CAMERA_TF_C::InitFromCfg()
{
    /* 相机内参转换 */
    if (ConvertVector2Mat(stCameraParam_.cameraMatrix, 3, matIntr_)) return -1;

    /* 相机外参转换 */
    CFG_PARAM_INSTALL_STRU &stPose = stCameraParam_.stInstallPose;
    COOR_3D_POSE_STRU stCameraPoseByCar(stPose.tx, stPose.ty, stPose.tz, stPose.roll, stPose.pitch, stPose.yaw);
    COOR_3D_POSE_STRU stCarPoseByCamera = stCameraPoseByCar.GetPoseInv();

    TransMatRT(stCarPoseByCamera);
    return 0;
}

/**************************************************************************************
功能描述: 解析相机参数（W2室内外一体机）
修改记录:
**************************************************************************************/
int PCPT_CAMERA_TF_C::ParseParamsMat(string &strCameraParams)
{
    /**
     * 参考系：相机坐标系；
     * 相机坐标系：x轴指向右 y轴指向下 z轴指向前;
     * 相机外参记录的是车体坐标系到相机坐标系的平移、旋转参数；
     */

    /* 读取相机参数 */
    YAML::Node params  = YamlFileLoad(strCameraParams);

    /* 相机内参转换 */
    if (!YamlIsSubNodeExist(params, "CameraMat")) return -1;
    if (ConvertVector2Mat(params["CameraMat"]["data"].as<vector<double>>(), 3, matIntr_)) return -1;

    /* 相机外参转换 */
    cv::Mat matExtrinsic;
    if (!YamlIsSubNodeExist(params, "CameraExtrinsicMat")) return -1;
    if (ConvertVector2Mat(params["CameraExtrinsicMat"]["data"].as<vector<double>>(), 4, matTR_)) return -1;

    matTRInv_ = matTR_.inv();
    matR_ = matTR_(cv::Range(0, 3), cv::Range(0, 3));
    matT_ = matTR_(cv::Range(0, 3), cv::Range(3, 4));

    /* 整合内外参 */
    matIntrInv_ = matIntr_.inv();
    matInTf_    = matR_.inv() * matIntrInv_;
    matExTf_    = matR_.inv() * matT_;

    return 0;
}

/**************************************************************************************
功能描述: 设置MatRT
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::SetMatRT(cv::Mat& matR, cv::Mat& matT)
{
    /* 整合内外参 */
    matIntrInv_ = matIntr_.inv();
    matInTf_    = matR.inv() * matIntrInv_;
    matExTf_    = matR.inv() * matT;

    /* 平移 & 旋转 */
    matTR_ = (cv::Mat_<double>(4, 4) << matR.at<double>(0,0), matR.at<double>(0,1), matR.at<double>(0,2), matT.at<double>(0,0),
                                        matR.at<double>(1,0), matR.at<double>(1,1), matR.at<double>(1,2), matT.at<double>(1,0),
                                        matR.at<double>(2,0), matR.at<double>(2,1), matR.at<double>(2,2), matT.at<double>(2,0),
                                                           0,                    0,                    0,                   1);
    matTRInv_ = matTR_.inv();
}

/**************************************************************************************
功能描述: 转换matRects matTvecs
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::TransMatRT(cv::Mat& matRects, cv::Mat& matTvecs)
{
    cv::Rodrigues(matRects, matR_); //罗德里格斯，输入旋转向量，输出旋转矩阵
    matT_ = matTvecs;
    SetMatRT(matR_, matT_);
}

void PCPT_CAMERA_TF_C::TransMatRT(const COOR_3D_POSE_STRU& stCarPoseByCamera)
{
    Vector3f vTrans  = stCarPoseByCamera.GetTranslation();
    Matrix3f mRotate = stCarPoseByCamera.GetRotation();

    matR_ = (cv::Mat_<double>(3, 3) << mRotate(0,0), mRotate(0,1), mRotate(0,2),
                                       mRotate(1,0), mRotate(1,1), mRotate(1,2),
                                       mRotate(2,0), mRotate(2,1), mRotate(2,2));
    matT_ = (cv::Mat_<double>(3, 1) << vTrans(0), vTrans(1), vTrans(2));

    SetMatRT(matR_, matT_);
}

/**************************************************************************************
功能描述: 解析相机参数（清扫车）
修改记录:
**************************************************************************************/
int PCPT_CAMERA_TF_C::ParseParamsRT(string &strCameraParams)
{
    /**
     * 参考系：相机坐标系；
     * 相机坐标系：x轴指向右 y轴指向下 z轴指向前;
     * 相机外参记录的是车体坐标系到相机坐标系的平移、旋转参数；
     * 平移：x y z;
     * 旋转：注意是旋转向量，而非欧拉角(roll pitch yaw);
     */

    /* 读取相机参数 */
    YAML::Node params  = YamlFileLoad(strCameraParams);

    /* 相机内参转换 */
    if (!YamlIsSubNodeExist(params, "camera_matrix")) return -1;
    if (ConvertVector2Mat(params["camera_matrix"]["data"].as<vector<double>>(), 3, matIntr_)) return -1;

    /* 相机外参转换 */
    if (!YamlIsSubNodeExist(params, "rvecs")) return -1;
    if (!YamlIsSubNodeExist(params, "tvecs")) return -1;
    cv::Mat matRects, matTvecs; //旋转向量、平移向量
    if (ConvertVector2Mat(params["rvecs"]["data"].as<vector<double>>(), 1, matRects)) return -1;
    if (ConvertVector2Mat(params["tvecs"]["data"].as<vector<double>>(), 3, matTvecs)) return -1;
    matTvecs *= 0.001;          //转换为标准单位（米）

    TransMatRT(matRects, matTvecs);
    return 0;
}

/**************************************************************************************
功能描述: 将读取到的相机参数向量转化为矩阵
修改记录:
**************************************************************************************/
int PCPT_CAMERA_TF_C::ConvertVector2Mat(const vector<double>& v, const int rows, cv::Mat &mat) const
{
    if (v.size() % rows != 0) {
        ST_LOG_ERR("Params error, input data size:%u, rows:%d", uint32_t(v.size()), rows);
        return -1;
    }

    cv::Mat matT = cv::Mat(v);              //将vector变成单列的mat
    mat = matT.reshape(1, rows).clone();
    return 0;
}

/**************************************************************************************
功能描述: 根据像素坐标获取相机坐标系下的视角
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::GetPixelAngleByCamera(const double imageX, const double imageY, float& angleH, float& angleV) const
{
    cv::Mat PointByImage  = (cv::Mat_<double>(3, 1) << imageX, imageY, 1);
    cv::Mat pointByCamera = matIntrInv_ * PointByImage;
    angleH = atan(pointByCamera.at<double>(0, 0));
    angleV = atan(pointByCamera.at<double>(1, 0));
}

void PCPT_CAMERA_TF_C::GetPixelAngleByCamera(const cv::Point2d &imagePoint, float& angleH, float& angleV) const
{
    GetPixelAngleByCamera(imagePoint.x, imagePoint.y, angleH, angleV);
}

/**************************************************************************************
功能描述: 将相机坐标系下的点转换到指定坐标系
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::TransPointByCamera2ByFrame(const cv::Point3d &pointByCamera, cv::Point3d &pointByCar, const cv::Mat &matTRByFrame) const
{
    cv::Mat point  = (cv::Mat_<double>(4, 1) << pointByCamera.x, pointByCamera.y, pointByCamera.z, 1);
    cv::Mat pointT =  matTRByFrame * point;

    pointByCar.x = pointT.at<double>(0, 0);
    pointByCar.y = pointT.at<double>(1, 0);
    pointByCar.z = pointT.at<double>(2, 0);
}

/**************************************************************************************
功能描述: 将相机坐标系下的点转换到车体坐标系
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::TransPointByCamera2ByCar(const cv::Point3d &pointByCamera, cv::Point3d &pointByCar) const
{
    TransPointByCamera2ByFrame(pointByCamera, pointByCar, matTRInv_);
}

/**************************************************************************************
功能描述: 发布视锥
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::PubVisionCone(const CONE_STRU &stCone, const char* pNameSpace) const
{
    if (!pubMarker_.getNumSubscribers()) return;

    visualization_msgs::Marker line;
    line.header.stamp       = RosTimeNow();
    line.header.frame_id    = PCPT_FRAME_ID_CAR;
    line.id                 = 0;
    line.ns                 = pNameSpace;
    line.action             = visualization_msgs::Marker::ADD;
    line.pose.orientation.w = 1.0;
    line.type               = visualization_msgs::Marker::LINE_LIST;
    line.scale.x            = 0.02;
    line.color.r            = 1.0;
    line.color.g            = 0.0;
    line.color.b            = 0.0;
    line.color.a            = 1.0;

    geometry_msgs::Point orignPoint;
    geometry_msgs::Point ltPoint, lpPoint, rtPoint, rpPoint;
    orignPoint.x = stCone.stOrigin.x;
    orignPoint.y = stCone.stOrigin.y;
    orignPoint.z = stCone.stOrigin.z;

#define RAY_LINE_POINT(angleH, angleV, depth, endPoint) \
    endPoint.x = cos(angleH) * depth;                   \
    endPoint.y = sin(angleH) * depth;                   \
    endPoint.z = tan(angleV) * depth;                   \
    line.points.push_back(orignPoint);                  \
    line.points.push_back(endPoint);                    \

    float coneDepth = 10;
    RAY_LINE_POINT(stCone.angleH_min, stCone.angleV_min, coneDepth, ltPoint);
    RAY_LINE_POINT(stCone.angleH_min, stCone.angleV_max, coneDepth, lpPoint);
    RAY_LINE_POINT(stCone.angleH_max, stCone.angleV_min, coneDepth, rtPoint);
    RAY_LINE_POINT(stCone.angleH_max, stCone.angleV_max, coneDepth, rpPoint);

#define RAY_LINE_MARKER(sPoint, ePoint) \
        line.points.push_back(sPoint);  \
        line.points.push_back(ePoint);  \

    RAY_LINE_MARKER(orignPoint, ltPoint);
    RAY_LINE_MARKER(orignPoint, lpPoint);
    RAY_LINE_MARKER(orignPoint, rtPoint);
    RAY_LINE_MARKER(orignPoint, rpPoint);

    RAY_LINE_MARKER(ltPoint, lpPoint);
    RAY_LINE_MARKER(lpPoint, rpPoint);
    RAY_LINE_MARKER(rpPoint, rtPoint);
    RAY_LINE_MARKER(rtPoint, ltPoint);

    visualization_msgs::MarkerArray markerArray;
    markerArray.markers.push_back(line);
    pubMarker_.publish(markerArray);
}

/**************************************************************************************
功能描述: 发布车体坐标系的视锥
修改记录:
**************************************************************************************/
void PCPT_CAMERA_TF_C::PubVisionConeByCar(const CONE_STRU &stCone) const
{
    PubVisionCone(stCone, "car");
}
