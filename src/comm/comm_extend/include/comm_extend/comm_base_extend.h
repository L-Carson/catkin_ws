#ifndef __COMM_BASE_EXTEND_H__
#define __COMM_BASE_EXTEND_H__

#define PCL_NO_PRECOMPILE
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <float.h>
#include <deque>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <pcl/pcl_macros.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>

#include "comm/comm_base.h"

using namespace std;

struct PointXYZIRT {
    PCL_ADD_POINT4D   //
    float    intensity;
    double   timestamp;
    uint16_t ring;                   ///< laser ring number
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW  // make sure our new allocators are aligned,
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZIRT,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)
    (double, timestamp, timestamp)(std::uint16_t, ring, ring))

struct PointXYZIRCDT {
    PCL_ADD_POINT4D;
    std::uint8_t  intensity;
    std::uint16_t ring = 0;
    std::uint16_t col  = 0;
    float    dis       = 0;
    double   timestamp = 0;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZIRCDT, (float, x, x)(float, y, y)(float, z, z)(std::uint8_t, intensity, intensity)
                                                 (std::uint16_t, ring, ring)(std::uint16_t, col, col)(float, dis, dis)(double, timestamp, timestamp))

typedef pcl::PointXYZ                  PointXYZ;
typedef pcl::PointXYZI                 PointXYZI;
typedef pcl::PointCloud<PointXYZ>      PointCloudXYZ;
typedef pcl::PointCloud<PointXYZI>     PointCloudXYZI;
typedef pcl::PointCloud<PointXYZIRT>   PointCloudXYZIRT;
typedef pcl::PointCloud<PointXYZIRCDT> PointCloudXYZIRCDT;

typedef enum POINT_TYPE_ENUM {
    MY_POINT_TYPE_UNKNOW   = -1,
    MY_POINT_TYPE_XYZ      =  0,
    MY_POINT_TYPE_XYZI     =  1,
    MY_POINT_TYPE_XYZIRT   =  2,
    MY_POINT_TYPE_XYZIRCDT =  3,
} POINT_TYPE_ENUM;

template<typename T>
T AngleNormalize(const T &angle)
{
    T normalizedAngle;
    if (angle > PI){
        normalizedAngle = angle - 2 * PI;
    }
    else if (angle < -PI){
        normalizedAngle = angle + 2 * PI;
    }
    else{
        normalizedAngle = angle;
    }

    return normalizedAngle;
}

template<typename T>
Eigen::Matrix<T, 3, 3> Hat(const Eigen::Matrix<T, 3, 1> &v)
{
    Eigen::Matrix<T, 3, 3> m = Eigen::Matrix<T, 3, 3>::Zero();
    m(0,1) = -v(2); m(1,0) = -m(0,1);
    m(0,2) =  v(1); m(2,0) = -m(0,2);
    m(1,2) = -v(0); m(2,1) = -m(1,2);
    return m;
}

template<typename T>
Eigen::Matrix<T, 3, 1> Vee(const Eigen::Matrix<T, 3, 3> &m)
{
    Eigen::Matrix<T, 3, 1> v = Eigen::Matrix<T, 3, 1>::Zero();
    v(0) = m(2,1);
    v(1) = m(0,2);
    v(2) = m(1,0);
    return v;
}

// reference from fast_lio v1.0
template<typename T>
Eigen::Matrix<T, 3, 3> RotVecExp(const Eigen::Matrix<T, 3, 1> &rotVec)
{
    T angleNorm = rotVec.norm();
    Eigen::Matrix<T, 3, 3> Eye3 = Eigen::Matrix<T, 3, 3>::Identity();

    if (angleNorm > 0.0000001) {
        Eigen::Matrix<T, 3, 1> axis = rotVec / angleNorm;
        Eigen::Matrix<T, 3, 3> K    = Hat(axis);
        return Eye3 + std::sin(angleNorm) * K + (1.0 - cos(angleNorm)) * K * K;
    }
    else {
        return Eye3;
    }
}

template<typename T>
Eigen::Matrix<T, 3, 1> RotMatLog(const Eigen::Matrix<T, 3, 3> &rotMat)
{
    T &&theta = acos(0.5 * (rotMat.trace() - 1));
    Eigen::Matrix<T,3,1> K(rotMat(2,1) - rotMat(1,2), rotMat(0,2) - rotMat(2,0), rotMat(1,0) - rotMat(0,1));
    return (abs(theta) < 0.001) ? (0.5 * K) : (0.5 * theta / sin(theta) * K);
}

/* 传感器相关数据结构　*/
struct CLOUD_DATA_STRU {
    double              time;
    PointCloudXYZI::Ptr pPoints;

    CLOUD_DATA_STRU(void) {
        SetZero();
    };

    void SetZero(void) {
        time = 0.0;
        pPoints.reset(new PointCloudXYZI());
    }
};

struct CLOUD_XYZIRT_DATA_STRU {
    double                time;
    PointCloudXYZIRT::Ptr pPoints;

    CLOUD_XYZIRT_DATA_STRU(void){
        SetZero();
    };

    void SetZero(void) {
        time = 0.0;
        pPoints.reset(new PointCloudXYZIRT());
    }
};

/**
 * @brief imu数据结构
 *
 */
struct IMU_DATA_STRU {
    double      time;          // 数据时间戳[unit: s]
    Vector3f    acc;
    Vector3f    gyro;
    Quaternionf orientation;

    IMU_DATA_STRU(void) {
        SetZero();
    }

    IMU_DATA_STRU(const IMU_DATA_STRU &stImuData) {
        SetValue(stImuData.time, stImuData.acc, stImuData.gyro, stImuData.orientation);
    }

    void Copy(const IMU_DATA_STRU &stImuData){
        SetValue(stImuData.time, stImuData.acc, stImuData.gyro, stImuData.orientation);
    }

    Matrix3f GetRotation(void) const {
        Matrix3f rotation = orientation.matrix();
        return rotation;
    }

    Vector3f GetEuler(void) const {
        Matrix3f rotation = GetRotation();

        Vector3f eulerAngle;
        Rot2Euler(rotation, eulerAngle(0), eulerAngle(1), eulerAngle(2));

        return eulerAngle;
    }

    Quaternionf GetQuat(void) const {
        return orientation;
    }

    void SetZero(void){
        SetValue(0.0, Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 0.0),
                 Quaternionf(1.0, 0.0, 0.0, 0.0));
    }

    void SetValue(const double   &srcTime, const Vector3f    &srcAcc,
                  const Vector3f &srcGyro, const Quaternionf &srcQuat) {
        time = srcTime;
        acc  = srcAcc;
        gyro = srcGyro;
        orientation = srcQuat;
    }

};

struct CTRL_INPUT_STRU {
    double     time;
    Vector6f   ctrlInput;

    CTRL_INPUT_STRU(void)
    {
        time      = 0.0;
        ctrlInput = Vector6f::Zero();
    }

    CTRL_INPUT_STRU Interpolation(const double &xTime, const CTRL_INPUT_STRU &stCtrlInputE) const
    {
        double deltaT     = stCtrlInputE.time - time;
        float  ratio      = deltaT < 0.005 ? 0.5 : (xTime - time) / deltaT;

        CTRL_INPUT_STRU   stCtrlInputX;
        stCtrlInputX.time      = xTime;
        stCtrlInputX.ctrlInput = (1 - ratio) * ctrlInput + ratio * stCtrlInputE.ctrlInput;

        return stCtrlInputX;
    }

};

/**
 * @brief 带时间戳3D位姿结构
 *
 */
struct TIMESTAMP_POSE_STRU {
    double            time;       // 时间戳[unit: s]
    COOR_3D_POSE_STRU stPose;     // 3D位姿

    TIMESTAMP_POSE_STRU Interpolation(const double &xTime, const TIMESTAMP_POSE_STRU &stTimePoseE) const
    {
        Matrix4f mPoseX   = Matrix4f::Identity();
        double deltaT     = stTimePoseE.time - time;
        float  ratio      = deltaT < 0.005 ? 0.5 : (xTime - time) / deltaT;
        Vector3f vPosX    = (1 - ratio)*stPose.GetTranslation()  + ratio*stTimePoseE.stPose.GetTranslation();
        Quaternionf quatX = stPose.stAtt.Quat().slerp(ratio, stTimePoseE.stPose.stAtt.Quat());
        mPoseX.block<3,1>(0,3) = vPosX;
        mPoseX.block<3,3>(0,0) = quatX.normalized().matrix();

        TIMESTAMP_POSE_STRU  stTimePoseX;
        stTimePoseX.time = xTime;
        stTimePoseX.stPose.Copy(mPoseX);

        return stTimePoseX;
    }
};

struct MOVE_INFO_DATA_STRU {
    double   time;
    float    steeringAngle;
    float    moveLinearVel;
    float    yawAngle;
    float    yawAngleVel;
    Vector3f acc;

    void Copy(const MOVE_INFO_DATA_STRU &stMoveInfo){
        time          = stMoveInfo.time;
        steeringAngle = stMoveInfo.steeringAngle;
        moveLinearVel = stMoveInfo.moveLinearVel;
        yawAngle      = stMoveInfo.yawAngle;
        yawAngleVel   = stMoveInfo.yawAngleVel;
        acc           = stMoveInfo.acc;
    }

};

struct POSE_SCORE_STRU {
    float     score;
    Matrix4f  mPose;

    POSE_SCORE_STRU(void) {
        score = DBL_MAX;
        mPose = Matrix4f::Identity();
    }

    void Copy(POSE_SCORE_STRU &stPoseScore)
    {
       score = stPoseScore.score;
       mPose = stPoseScore.mPose;
    }

};

struct LOOP_CONSTRAINT_STRU {
    float              score;
    COORDINATE_3D_STRU stPos1;
    COORDINATE_3D_STRU stPos2;
};


template<class T> class DATA_CHANNEL_C
{
public:
    DATA_CHANNEL_C (INT32 maxSize = 10000) : maxSize_(maxSize)
    {
       vItems_.clear();
    };

    void Send(const T &item)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        vItems_.emplace_back(item);
        while (vItems_.size() > maxSize_) {
            vItems_.pop_front();
        }
    }

    INT32 Get(std::deque<T> &vItems)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!vItems_.empty()) {
            vItems.insert(vItems.end(), vItems_.begin(), vItems_.end());
            vItems_.clear();
            return 0;
        }
        return -1;
    }

    INT32 Get(T &item)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!vItems_.empty()) {
            item = vItems_.front();
            vItems_.pop_front();
            return 0;
        }
        return -1;
    }

    BOOL IsBlock(void)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return !vItems_.empty();
    }

private:
    size_t         maxSize_;
    std::deque<T>  vItems_;
    std::mutex     mutex_;
};

PointCloudXYZI::Ptr PointXyzirt2Xyzi(const PointCloudXYZIRT::Ptr pPoints);

template <class F>
void TimeConsumeEvaluate(F&& func, const std::string& strFuncName, const double threshold)
{
    std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
    std::forward<F>(func)();
    std::chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
    if (time > threshold) {
        ST_LOG_INFO("%s Exe Time %f, Expect time %f .", strFuncName.c_str(), time, threshold);
    }
}

INT32 GetLoginUserName(string &strUserName);
INT32 RunSystem(const string &strCmd);
std::vector<std::string> GetFileNames(const std::string &strDir, const std::string &strSuffix);
BOOL  SaveKittiTraj(std::ostream &os, const std::vector<Matrix4f> &vmPoses);


#endif
