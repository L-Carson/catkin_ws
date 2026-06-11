#ifndef __COMM_BASE_H__
#define __COMM_BASE_H__

#include "comm/comm_time.h"
#include "comm/comm_sys.h"
#include "comm/comm_dbg_log.h"
#include "comm/comm_data_type.h"
#include "comm/comm_label.h"

#include <flann/flann.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>           /* cv::eigen2cv */

#include <list>
#include <vector>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>

#include <dirent.h>
#include <sys/stat.h>

#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

#define PI 3.1415926535897932384626433832795

/* 地图分辨率 */
#define MAP_RESOLUTION                          0.05
#define SCAN_RANGE_FURTHER                      100.0

#define MAIN_CTRL_FREQUENCY                     20
#define MAIN_CTRL_CYCLE_LEN                     0.05

/* 角度 弧度 互转 */
#define ANGLE_TO_RADIAN(angle_)                 ((double)(angle_) * PI / 180.0)
#define RADIAN_TO_ANGLE(radian_)                ((double)(radian_) * 180.0 / PI)

/* sleep */
#define SLEEP_S(t_) std::this_thread::sleep_for(std::chrono::seconds(t_));      // 秒
#define SLEEP_M(t_) std::this_thread::sleep_for(std::chrono::milliseconds(t_)); // 毫秒
#define SLEEP_U(t_) std::this_thread::sleep_for(std::chrono::microseconds(t_)); // 微秒
#define SLEEP_N(t_) std::this_thread::sleep_for(std::chrono::nanoseconds(t_));  // 纳秒

typedef Eigen::Matrix<float, 2, 1> Vector2f;
typedef Eigen::Matrix<float, 3, 1> Vector3f;
typedef Eigen::Matrix<float, 4, 1> Vector4f;
typedef Eigen::Matrix<float, 6, 1> Vector6f;
typedef Eigen::Matrix<float, 3, 3> Matrix3f;
typedef Eigen::Matrix<float, 4, 4> Matrix4f;
typedef Eigen::AngleAxis<float>    AngleAxisf;
typedef Eigen::Quaternion<float>   Quaternionf;
typedef Eigen::Matrix<float, Eigen::Dynamic, 1> VectorXf;
typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> MatrixXf;

typedef Eigen::Matrix<double, 2, 1> Vector2d;
typedef Eigen::Matrix<double, 3, 1> Vector3d;
typedef Eigen::Matrix<double, 4, 1> Vector4d;
typedef Eigen::Matrix<double, 6, 1> Vector6d;
typedef Eigen::Matrix<double, 3, 3> Matrix3d;
typedef Eigen::Matrix<double, 4, 4> Matrix4d;
typedef Eigen::AngleAxis<double>    AngleAxisd;
typedef Eigen::Quaternion<double>   Quaterniond;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> VectorXd;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> MatrixXd;
typedef Eigen::Transform<double,3, Eigen::Isometry> Isometry3d;

/* 告警码 */
enum ALARM_ID_ENUM {
    ALARM_ID_LIDAR                        = 0x50000,    //激光雷达
    ALARM_ID_LIDAR_UNCALIBRATE            = 0x50009,    //激光雷达未标定告警
    ALARM_ID_LIDAR_MATCH_VERIFY           = 0x5000A,    //激光雷达点云校验异常（匹配度）
    ALARM_ID_LIDAR_POINTS_VERIFY          = 0x5000B,    //激光雷达点云校验异常（车身点数）
    ALARM_ID_RADAR                        = 0x50010,    //毫米波雷达
    ALARM_ID_IMU                          = 0x50020,    //IMU
    ALARM_ID_GPS                          = 0x50030,    //GPS
    ALARM_ID_ULTRASONIC                   = 0x50040,    //超声波雷达
    ALARM_ID_CAMERA                       = 0x50050,    //RGB相机
    ALARM_ID_LOC                          = 0x50060,    //无定位
    ALARM_ID_STAGNATE                     = 0x50070,    //停滞不前
    ALARM_ID_OUT_OF_WORK                  = 0x50080,    //不在工作区
    ALARM_ID_DCU                          = 0x50090,    //DCU
    ALARM_ID_OFF_TRACK                    = 0x500A0,    //机器人偏离路线
    ALARM_ID_TRAFFIC_JAM                  = 0x500B0,    //驶入易堵车路段
    ALARM_ID_IPU                          = 0x500C0,    //图像处理单元
    ALARM_ID_VISION_PIPELINE              = 0x50100,    //水管、管线
    ALARM_ID_VISION_THICKPIPE             = 0x50101,    //粗水管
    ALARM_ID_VISION_SLOPE_PAD             = 0x50110,    //斜坡垫
    ALARM_ID_VISINO_CORD                  = 0x50120,    //电线、缆绳
    ALARM_ID_DOOR_OPEN_AB                 = 0x50130,    //库房门开门异常
    ALARM_ID_DOOR_CLOSE_AB                = 0x50140,    //库房门关门异常
    ALARM_ID_CROSSING                     = 0x50150,    //通过路口告警
    ALARM_ID_TRAFFIC_LIGHT_AB             = 0x50160,    //红绿灯异常告警
    ALARM_ID_PANEL_UNCLOSED               = 0x50170,    //屏幕盖未合上
    ALARM_ID_DUSTBIN_FULL                 = 0x50180,    //站点垃圾箱已满
    ALARM_ID_DUSTBIN_NULL                 = 0x50181,    //站点垃圾箱不存在
    ALARM_ID_TRASHBOX_LIFT_MOTOR_STICK    = 0x50182,    //车载垃圾箱推出高度不到位
    ALARM_ID_TRASHBOX_ROLL_MOTOR_STICK    = 0x50183,    //车载垃圾箱翻斗推出不到位
    ALARM_ID_TRASHBOX_ROLL_MOTOR_WITHDRAW = 0x50184,    //车载垃圾箱翻斗收回不到位
};

template<typename T>
void Rot2Euler(const Eigen::Matrix<T, 3, 3> &R, T &roll, T &pitch, T &yaw)
{
    T sy = sqrt(R(0,0) * R(0,0) +  R(1,0) * R(1,0));
    BOOL  singular = sy < 1e-6;
    if (!singular) {
        roll  = atan2( R(2,1), R(2,2));
        pitch = atan2(-R(2,0), sy);
        yaw   = atan2( R(1,0), R(0,0));
    } else {
        roll  = atan2(-R(1,2), R(1,1));
        pitch = atan2(-R(2,0), sy);
        yaw   = 0.0;
    }
}

float CalcAngleDiffAbs(float angle1, float angle2);     /* 结构体内函数使用，提前申明 */

struct COORDINATE_2D_STRU {
    float x;
    float y;

    COORDINATE_2D_STRU(void) {}

    COORDINATE_2D_STRU(const float &srcX, const float &srcY)
    {
        SetValue(srcX, srcY);
    }

    void Copy(const COORDINATE_2D_STRU &stSrc)
    {
        x = stSrc.x;
        y = stSrc.y;
    }

    void SetValue(const float &srcX, const float &srcY)
    {
        x = srcX;
        y = srcY;
    }

    void SetZero(void)
    {
        x = 0.0;
        y = 0.0;
    }

    float CalcDistSqu(const COORDINATE_2D_STRU &stAnotherPoint) const
    {
        float diffX = x - stAnotherPoint.x;
        float diffY = y - stAnotherPoint.y;
        return (diffX * diffX + diffY * diffY);
    }

    bool IsAlmostEuqal(const COORDINATE_2D_STRU &stAnotherPoint) const
    {
        float distSqu = CalcDistSqu(stAnotherPoint);
        return IS_FLOAT_ALMOST_EQUAL_ZERO(distSqu);
    }

    float GetVectorLenSqu(void) const
    {
        return x * x + y * y;
    }

    COORDINATE_2D_STRU operator + (const COORDINATE_2D_STRU& a) const
    {
        COORDINATE_2D_STRU p;
        p.x = x + a.x;
        p.y = y + a.y;
        return p;
    }

    COORDINATE_2D_STRU operator - (const COORDINATE_2D_STRU& a) const
    {
        COORDINATE_2D_STRU p;
        p.x = x - a.x;
        p.y = y - a.y;
        return p;
    }

    float operator * (const COORDINATE_2D_STRU& a) const
    {
        float p = x * a.x + y * a.y;
        return p;
    }

    COORDINATE_2D_STRU operator * (const float& value) const
    {
        COORDINATE_2D_STRU p;
        p.x = x * value;
        p.y = y * value;
        return p;
    }

    COORDINATE_2D_STRU operator / (const float& value) const
    {
        COORDINATE_2D_STRU p;
        p.x = x / value;
        p.y = y / value;
        return p;
    }

    bool IsZero() const {
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(x)) return false;
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(y)) return false;
        return true;
    }
};

struct COORDINATE_3D_STRU {
    float x;
    float y;
    float z;

    COORDINATE_3D_STRU(void)
    {
        SetZero();
    }

    COORDINATE_3D_STRU(const float &srcX, const float &srcY, const float &srcZ)
    {
        x = srcX; y = srcY; z = srcZ;
    }

    COORDINATE_3D_STRU(const Eigen::Vector3f &vXYZ)
    {
        x = vXYZ(0); y = vXYZ(1); z = vXYZ(2);
    }

    void Copy(const COORDINATE_3D_STRU &stSrc)
    {
        SetValue(stSrc.x, stSrc.y, stSrc.z);
    }

    void SetValue(const float &srcX, const float &srcY, const float &srcZ)
    {
        x = srcX; y = srcY; z = srcZ;
    }

    void SetZero(void)
    {
        SetValue(0.0, 0.0, 0.0);
    }

    float CalcDistSqu(const COORDINATE_3D_STRU &stAnotherPoint) const
    {
        float diffX = x - stAnotherPoint.x;
        float diffY = y - stAnotherPoint.y;
        float diffZ = z - stAnotherPoint.z;
        return (diffX * diffX + diffY * diffY + diffZ*diffZ);
    }

    float CalcDistSquInXY(const COORDINATE_3D_STRU &stAnotherPoint) const
    {
        float diffX = x - stAnotherPoint.x;
        float diffY = y - stAnotherPoint.y;
        return (diffX * diffX + diffY * diffY);
    }

    BOOL IsAlmostEuqal(const COORDINATE_3D_STRU &stAnotherPoint, const float &threshold) const
    {
        float  distSqu = CalcDistSqu(stAnotherPoint);
        return distSqu < threshold*threshold;
    }

    BOOL IsAlmostEuqalInXY(const COORDINATE_3D_STRU &stAnotherPoint) const
    {
        float  distSqu = CalcDistSqu(stAnotherPoint);
        return distSqu < 0.1 * 0.1;
    }

    BOOL IsZero() const {
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(x)) return false;
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(y)) return false;
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(z)) return false;
        return true;
    }

    float GetVectorLenSqu(void) const
    {
        return x * x + y * y + z * z;
    }

    Eigen::Vector3f Vec(void) const
    {
        return Eigen::Vector3f(x, y, z);
    }
};

struct POINT_COORD_2D_STRU {
    POINT_COORD_2D_STRU() {}
    POINT_COORD_2D_STRU(const double x_, const double y_) : x(x_), y(y_) {}
    POINT_COORD_2D_STRU(const COORDINATE_2D_STRU& stCoor) : x(stCoor.x), y(stCoor.y) {}

    float CalcDistSqu(const POINT_COORD_2D_STRU &stAnotherPoint) const {
        float diffX = x - stAnotherPoint.x;
        float diffY = y - stAnotherPoint.y;
        return (diffX * diffX + diffY * diffY);
    }

    bool operator == (const POINT_COORD_2D_STRU& stPoint) const {
        return SIG(x - stPoint.x) == 0 && SIG(y - stPoint.y) == 0;
    }

    double x;
    double y;
};

struct POINT_COORD_3D_STRU {
    double x;
    double y;
    double z;
};

struct COOR_3D_DIR_STRU {
    POINT_COORD_3D_STRU stPos;
    float dirAngle;
};

struct IMAGE_COORDINATE_STRU {
    int x;
    int y;

    BOOL IsEqual(const IMAGE_COORDINATE_STRU &stAnother) const
    {
        return ((x == stAnother.x) && (y == stAnother.y));
    }
};

struct VOXEL_COORDINATE_STRU {
    int x;
    int y;
    int z;
};

struct LASER_POINT_STRU {
    float angle;
    float dist;
};

struct ATTITUDE_STRU {
    float roll;
    float pitch;
    float yaw;

    ATTITUDE_STRU(void){
        SetZero();
    }

    ATTITUDE_STRU(const float &srcRoll, const float &srcPitch, const float &srcYaw){
        SetValue(srcRoll, srcPitch, srcYaw);
    }

    void Copy(const ATTITUDE_STRU &stSrc){
        roll  = stSrc.roll; pitch = stSrc.pitch; yaw   = stSrc.yaw;
    }

    void Copy(const Matrix3f &R){
        SetValue(R);
    }

    void SetZero(void){
        SetValue(0.0, 0.0, 0.0);
    }

    void SetValue(const Matrix3f &R) {
        Rot2Euler(R, roll, pitch, yaw);
        SetValue(roll, pitch, yaw);
    }

    void SetValue(const float &srcRoll, const float &srcPitch, const float &srcYaw){
        roll = srcRoll; pitch = srcPitch; yaw = srcYaw;
    }

    Matrix3f Matrix(void) const{
        Eigen::AngleAxisf rotation_x( roll,  Vector3f::UnitX());
        Eigen::AngleAxisf rotation_y( pitch, Vector3f::UnitY());
        Eigen::AngleAxisf rotation_z( yaw,   Vector3f::UnitZ());
        Matrix3f R = (rotation_z * rotation_y * rotation_x) * Matrix3f::Identity();

        return R;
    }

    Matrix3f MatrixInv(void) const{
        return Matrix().transpose();
    }

    Quaternionf Quat(void) const{
        Quaternionf quat;
        quat = Matrix();
        return quat;
    }

    float CalcMaxAttDiff(const ATTITUDE_STRU &stAnotherAtt) const{
        float diffRoll  = CalcAngleDiffAbs(roll,  stAnotherAtt.roll);
        float diffPitch = CalcAngleDiffAbs(pitch, stAnotherAtt.pitch);
        float diffYaw   = CalcAngleDiffAbs(yaw,   stAnotherAtt.yaw);
        return MAX(MAX(diffRoll, diffPitch), diffYaw);
    }

    BOOL IsZero() const {
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(roll))  return false;
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(pitch)) return false;
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(yaw))   return false;
        return true;
    }
};

struct POSE_DIFF_STRU {
    float dist;
    float angle;

    POSE_DIFF_STRU(void){
        dist  = FLT_MAX;
        angle = PI;
    }
};

struct COOR_2D_POSE_STRU {
    COORDINATE_2D_STRU stPos;
    float dirAngle;

    COOR_2D_POSE_STRU(void)
    {
        SetZero();
    }

    COOR_2D_POSE_STRU(const COOR_2D_POSE_STRU &stPose)
    {
        SetValue(stPose.stPos.x, stPose.stPos.y, stPose.dirAngle);
    }

    COOR_2D_POSE_STRU(float x, float y, float angle)
    {
        SetValue(x, y, angle);
    }

    void Copy(const COOR_2D_POSE_STRU &stSrc)
    {
        SetValue(stSrc.stPos.x, stSrc.stPos.y, stSrc.dirAngle);
    }

    void SetZero(void)
    {
        SetValue(0, 0, 0);
    }

    void SetValue(float x, float y, float angle)
    {
        stPos.x  = x;
        stPos.y  = y;
        dirAngle = angle;
    }

    float x() const {
        return stPos.x;
    }

    float y() const {
        return stPos.y;
    }

    bool IsAboutEqual(COOR_2D_POSE_STRU stPose)
    {
        /* 如果两个姿态位置相差小于0.05m，角度相差小于0.5°，则认为姿态一样。 */
        return (   (stPos.CalcDistSqu(stPose.stPos) < 0.0025)
                && (CalcAngleDiffAbs(dirAngle, stPose.dirAngle) < ANGLE_TO_RADIAN(0.5)));
    }

    BOOL IsZero() {
        if (!stPos.IsZero()) return false;
        if (!IS_FLOAT_ALMOST_EQUAL_ZERO(dirAngle)) return false;
        return true;
    }

    float CalcDistSqu(const COORDINATE_2D_STRU &stAnotherPoint) const {
        return stPos.CalcDistSqu(stAnotherPoint);
    }

    float CalcDistSqu(const COOR_2D_POSE_STRU &stAnotherPoint) const {
        return stPos.CalcDistSqu(stAnotherPoint.stPos);
    }
};

/* 3D位姿数据结构 */
struct COOR_3D_POSE_STRU {
    COORDINATE_3D_STRU stPos;   // 3元Float位置坐标
    ATTITUDE_STRU      stAtt;   // 3元Float姿态: rpy

    COOR_3D_POSE_STRU(void) {
        SetZero();
    }

    COOR_3D_POSE_STRU(const float srcX,    const float srcY,     const float srcZ,
                      const float srcRoll, const float srcPitch, const float srcYaw) {
        SetValue(srcX, srcY, srcZ, srcRoll, srcPitch, srcYaw);
    }

    COOR_3D_POSE_STRU(const COORDINATE_3D_STRU &stSrcPos,
                      const ATTITUDE_STRU      &stSrcAtt) {
        SetValue(stSrcPos.x,    stSrcPos.y,     stSrcPos.z,
                 stSrcAtt.roll, stSrcAtt.pitch, stSrcAtt.yaw);
    }

    COOR_3D_POSE_STRU(const Matrix4f &mPose) {
        SetValue(mPose);
    }

    void Copy(const COOR_3D_POSE_STRU &stSrc) {
        SetValue(stSrc.stPos.x,    stSrc.stPos.y,     stSrc.stPos.z,
                 stSrc.stAtt.roll, stSrc.stAtt.pitch, stSrc.stAtt.yaw);
    }

    void Copy(const Matrix4f &mPose) {
        stAtt.SetValue(mPose.block(0,0,3,3));
        stPos.SetValue(mPose(0,3), mPose(1,3),  mPose(2,3));
    }

    void SetValue(const float &srcX,    const float &srcY,     const float &srcZ,
                  const float &srcRoll, const float &srcPitch, const float &srcYaw) {
        stPos.SetValue(srcX,    srcY,     srcZ);
        stAtt.SetValue(srcRoll, srcPitch, srcYaw);
    }

    void SetValue(const Matrix4f &mPose) {
        stAtt.SetValue(mPose.block(0,0,3,3));
        stPos.SetValue(mPose(0,3), mPose(1,3),  mPose(2,3));
    }

    void SetZero(void) {
        SetValue(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    Matrix4f GetTransform(void) const {
        Matrix4f mPose = Matrix4f::Identity();
        mPose.block(0,3,3,1) = GetTranslation();
        mPose.block(0,0,3,3) = GetRotation();
        return mPose;
    }

    Matrix4f GetTransformInv(void) const {
        Matrix4f mPose = Matrix4f::Identity();
        mPose.block(0,3,3,1) = -GetRotation().transpose() * GetTranslation();
        mPose.block(0,0,3,3) =  GetRotation().transpose();
        return mPose;
    }

    COOR_3D_POSE_STRU GetPoseInv(void) const {
        Matrix4f mPoseInv = GetTransformInv();
        return COOR_3D_POSE_STRU(mPoseInv);
    }

    Matrix3f GetRotation(void) const {
        return stAtt.Matrix();
    }

    Vector3f GetTranslation(void) const {
        return stPos.Vec();
    }

    COOR_2D_POSE_STRU Get2DPose(void) const {
        return COOR_2D_POSE_STRU(stPos.x, stPos.y, stAtt.yaw);
    }

    COORDINATE_2D_STRU Get2DPos(void) const {
        return COORDINATE_2D_STRU(stPos.x, stPos.y);
    }

    float x(void) const {
        return stPos.x;
    }

    float y(void) const {
        return stPos.y;
    }

    float z(void) const {
        return stPos.z;
    }

    float roll(void) const {
        return stAtt.roll;
    }

    float pitch(void) const {
        return stAtt.pitch;
    }

    float yaw(void) const {
        return stAtt.yaw;
    }

    POSE_DIFF_STRU CalcPoseDiff(const Matrix4f &mAnotherPose) const {
        POSE_DIFF_STRU stPoseDiff;
        Matrix4f   mDiffPose = GetTransformInv() * mAnotherPose;
        AngleAxisf diffAngleAxis(mDiffPose.block<3, 3>(0, 0));
        stPoseDiff.dist  = mDiffPose.block<3, 1>(0, 3).norm();
        stPoseDiff.angle = diffAngleAxis.angle();

        return stPoseDiff;
    }

    POSE_DIFF_STRU CalcPoseDiff(const COOR_3D_POSE_STRU &stAnotherPose) const {
        return CalcPoseDiff(stAnotherPose.GetTransform());
    }

    BOOL IsAboutEqual(const COOR_3D_POSE_STRU &stAnotherPose) const {
        POSE_DIFF_STRU stPoseDiff = CalcPoseDiff(stAnotherPose);
        if (stPoseDiff.dist < 0.05 && stPoseDiff.angle < ANGLE_TO_RADIAN(0.5)) {
            return true;
        }
        return false;
    }

    BOOL IsZero() const {
        if (!stPos.IsZero()) return false;
        if (!stAtt.IsZero()) return false;
        return true;
    }
};

struct MOVE_CTRL_INFO_STRU {
    float turnAngle;        /* 转向控制量 */
    float linearVel;        /* 线速度控制量 */

    void SetZero(void)
    {
        turnAngle = 0.0;
        linearVel = 0.0;
    }

    void Copy(const MOVE_CTRL_INFO_STRU &stSrc)
    {
        turnAngle = stSrc.turnAngle;
        linearVel = stSrc.linearVel;
    }
};

struct MOVE_CTRL_SMOOTH_INFO_STRU {
    BOOL isSmooth;
    BOOL isSmoothStop;
};

struct MOVE_CTRL_SAFE_INFO_STRU {
    BOOL   isUseSecurityRedundancy;
    BOOL   isIgnoreLaserData;
    BOOL   isIgnoreUltrasonicData;
    UINT32 planningModel;
    UINT32 brushModel;

    void Reset(void) {
        isUseSecurityRedundancy    = true;
        isIgnoreLaserData          = false;
        isIgnoreUltrasonicData     = false;
        planningModel              = 0;
        brushModel                 = 0;
    }
};

struct GPS_COORD_STRU {
    double lon;                 /* 经度 */
    double lat;                 /* 纬度 */
    double alt;                 /* 高度　*/

    void SetZero(void)
    {
        lon = 0.0;
        lat = 0.0;
        alt = 0.0;
    }

    void Copy(const GPS_COORD_STRU &stSrc)
    {
        lon = stSrc.lon;
        lat = stSrc.lat;
        alt = stSrc.alt;
    }
};

struct GPS_POSE_STRU {
    GPS_COORD_STRU stCoord;
    float          yawAngle;

    void SetZero(void)
    {
        stCoord.SetZero();
        yawAngle = 0.0;
    }

    void Copy(const GPS_POSE_STRU &stSrc)
    {
        stCoord.Copy(stSrc.stCoord);
        yawAngle = stSrc.yawAngle;
    }

};

typedef enum {
  GPS_RSLT_TYPE_FIX     = 4,    /* 固定解 */
  GPS_RSLT_TYPE_FLOAT   = 5,    /* 浮点解 */
} GPS_RSLT_TYPE_ENUM;

struct GPS_INFO_STRU {
    double         time;
    UINT08         satelliteNum;            /* 卫星数 */
    UINT08         rsltType;                /* 解状态 详见 GPS_RSLT_TYPE_ENUM */
    float          hdop;
    GPS_POSE_STRU  stPose;

    void SetZero(void)
    {
        time         = 0.0;
        satelliteNum = 0;
        rsltType     = 0;
        stPose.SetZero();
    }

    void Copy(const GPS_INFO_STRU &stSrc)
    {
        time         = stSrc.time;
        satelliteNum = stSrc.satelliteNum;
        rsltType     = stSrc.rsltType;
        stPose.Copy(stSrc.stPose);
    }

    BOOL IsValid(void)
    {
        return ((satelliteNum >= 6) && ((rsltType == GPS_RSLT_TYPE_FIX) || (rsltType == GPS_RSLT_TYPE_FLOAT)));
    }

    BOOL IsValid2(void)
    {
        return ((satelliteNum >= 6) && (rsltType == GPS_RSLT_TYPE_FIX));
    }

};

struct CUBOID_STRU {
    CUBOID_STRU(){}

    CUBOID_STRU(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ)
    {
        SetValue(minX, maxX, minY, maxY, minZ, maxZ);
    }

    void SetValue(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ)
    {
        this->minX = minX; this->maxX = maxX;
        this->minY = minY; this->maxY = maxY;
        this->minZ = minZ; this->maxZ = maxZ;
    }

    bool IsValid() {
        if (maxX < minX || IS_FLOAT_ALMOST_EQUAL_ZERO(maxX - minX)) return false;
        if (maxY < minY || IS_FLOAT_ALMOST_EQUAL_ZERO(maxY - minY)) return false;
        if (maxZ < minZ || IS_FLOAT_ALMOST_EQUAL_ZERO(maxZ - minZ)) return false;
        return true;
    }

    bool IsInCubiod(const float x, const float y, const float z) const
    {
        if(x < minX || x > maxX) return false;
        if(y < minY || y > maxY) return false;
        if(z < minZ || z > maxZ) return false;
        return true;
    }

    template<typename T>
    bool IsInCubiod(const T& pos) const
    {
        return IsInCubiod(pos.x, pos.y, pos.z);
    }

    vector<COORDINATE_3D_STRU> CornerPoints() const {
        vector<COORDINATE_3D_STRU> vstCornerPoints;
        vstCornerPoints.push_back(COORDINATE_3D_STRU(minX, minY, minZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(minX, maxY, minZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(maxX, maxY, minZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(maxX, minY, minZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(minX, minY, maxZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(minX, maxY, maxZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(maxX, maxY, maxZ));
        vstCornerPoints.push_back(COORDINATE_3D_STRU(maxX, minY, maxZ));
        return vstCornerPoints;
    }

    COORDINATE_3D_STRU CenterPoints() const {
        return COORDINATE_3D_STRU((minX + maxX) * 0.5, (minY + maxY) * 0.5, (minZ + maxZ) * 0.5);
    }

    bool operator == (const CUBOID_STRU& stCuboid) const
    {
        if (fabsf(stCuboid.minX - minX) > 1e-8 ||
            fabsf(stCuboid.minY - minY) > 1e-8 ||
            fabsf(stCuboid.minZ - minZ) > 1e-8 ||
            fabsf(stCuboid.maxX - maxX) > 1e-8 ||
            fabsf(stCuboid.maxY - maxY) > 1e-8 ||
            fabsf(stCuboid.maxZ - maxZ) > 1e-8 )
            return false;
        else
            return true;
    }

    float minX{0};
    float minY{0};
    float minZ{0};
    float maxX{0};
    float maxY{0};
    float maxZ{0};
};

struct RECTANGLE_STRU {
    RECTANGLE_STRU(void) {}

    RECTANGLE_STRU(const float minX, const float maxX, const float minY, const float maxY) {
        SetValue(minX, maxX, minY, maxY);
    }

    void SetValue(const float minX, const float maxX, const float minY, const float maxY) {
        this->minX = minX; this->maxX = maxX;
        this->minY = minY; this->maxY = maxY;
    }

    bool IsValid() {
        if (maxX < minX || IS_FLOAT_ALMOST_EQUAL_ZERO(maxX - minX)) return false;
        if (maxY < minY || IS_FLOAT_ALMOST_EQUAL_ZERO(maxY - minY)) return false;
        return true;
    }

    bool IsIn(const float x, const float y) const {
        if (x < minX || x > maxX) return false;
        if (y < minY || y > maxY) return false;
        return true;
    }

    template<typename T>
    bool IsIn(const T& pos) const {
        return IsIn(pos.x, pos.y);
    }

    float Area() const {
        return (maxX - minX) * (maxY - minY);
    }

    float X() const {
        return std::move((minX + maxX) * 0.5);
    }

    float Y() const {
        return std::move((minY + maxY) * 0.5);
    }

    float Width() const {
        return std::move(maxX - minX);
    }

    float Length() const {
        return std::move(maxY - minY);
    }

    void Round(float unit) {
        minX = round(minX / unit) * unit;
        minY = round(minY / unit) * unit;
        maxX = round(maxX / unit) * unit;
        maxY = round(maxY / unit) * unit;
    }

    vector<COORDINATE_2D_STRU> CornerPoints() const {
        vector<COORDINATE_2D_STRU> vstCornerPoints;
        vstCornerPoints.push_back(COORDINATE_2D_STRU(minX, minY));
        vstCornerPoints.push_back(COORDINATE_2D_STRU(minX, maxY));
        vstCornerPoints.push_back(COORDINATE_2D_STRU(maxX, maxY));
        vstCornerPoints.push_back(COORDINATE_2D_STRU(maxX, minY));
        return vstCornerPoints;
    }

    COORDINATE_2D_STRU CenterPoints() const {
        return COORDINATE_2D_STRU((minX + maxX) * 0.5, (minY + maxY) * 0.5);
    }

    bool operator == (const RECTANGLE_STRU& stRect) const
    {
        if (fabsf(stRect.minX - minX) > 1e-8 ||
            fabsf(stRect.minY - minY) > 1e-8 ||
            fabsf(stRect.maxX - maxX) > 1e-8 ||
            fabsf(stRect.maxY - maxY) > 1e-8)
            return false;
        else
            return true;
    }

    bool operator != (const RECTANGLE_STRU& stRect) const {
        if (fabsf(stRect.minX - minX) > 1e-8 ||
            fabsf(stRect.minY - minY) > 1e-8 ||
            fabsf(stRect.maxX - maxX) > 1e-8 ||
            fabsf(stRect.maxY - maxY) > 1e-8)
            return true;
        else
            return false;
    }

    float minX{0};
    float minY{0};
    float maxX{0};
    float maxY{0};
};

struct SECTION_STRU {
    SECTION_STRU(void) {}
    SECTION_STRU(const float centerX, const float centerY, const float radius, const float startAngle, const float endAngle) {
        SetValue(COORDINATE_2D_STRU(centerX, centerY), radius, startAngle, endAngle);
    }

    SECTION_STRU(const COORDINATE_2D_STRU& stCenter, const float radius, const float startAngle, const float endAngle) {
        SetValue(stCenter, radius, startAngle, endAngle);
    }

    void SetValue(const float centerX, const float centerY, const float radius, const float startAngle, const float endAngle) {
        SetValue(COORDINATE_2D_STRU(centerX, centerY), radius, startAngle, endAngle);
    }

    void SetValue(const COORDINATE_2D_STRU& stCenter, const float radius, const float startAngle, const float endAngle) {
        this->radius         = radius;
        this->radiusSqu      = radius * radius;
        this->startAngle     = startAngle;
        this->endAngle       = endAngle;
        this->stCenter       = stCenter;
        this->stBoundingRect = BoundingRect(stCenter, radius, startAngle, endAngle);

        // -PI ~ PI
        if (this->startAngle > PI) {
            this->startAngle -= PI * 2;
            this->endAngle   -= PI * 2;
        } else if (this->endAngle < -PI) {
            this->startAngle += PI * 2;
            this->endAngle   += PI * 2;
        }
    }

    void UptByRadius(const float radius) {
        SetValue(stCenter, radius, startAngle, endAngle);
    }

    bool IsIn(const COORDINATE_2D_STRU& stCoor) const {
        // 是否在外接矩形框内
        if (!stBoundingRect.IsIn(stCoor)) return false;

        // 计算点到圆心的距离
        double distSqu = stCenter.CalcDistSqu(stCoor);

        // 检查点是否在半径范围内
        if (distSqu > radiusSqu) return false;

        // 计算点相对于圆心的极角
        double angle = std::atan2(stCoor.y - stCenter.y, stCoor.x - stCenter.x);    // (-PI~PI]

        // 检查点的极角是否在扇形的角度范围内
        if (                    angle >= startAngle          && angle <= endAngle)          return true;
        //if (endAngle   >  PI && angle >= startAngle - PI * 2 && angle <= endAngle - PI * 2) return true;
        //if (startAngle < -PI && angle >= startAngle + PI * 2 && angle <= endAngle + PI * 2) return true;
        return false;
    }

    bool IsIn(const float x, const float y) const {return IsIn(COORDINATE_2D_STRU(x, y));}

    /* 计算扇形的外接矩形 */
    RECTANGLE_STRU BoundingRect(const COORDINATE_2D_STRU& stCenter, const double radius, const double startAngle, const double endAngle) const {
        double minX = stCenter.x;
        double maxX = stCenter.x;
        double minY = stCenter.y;
        double maxY = stCenter.y;

        // 计算起始和终止角度对应的点
        POINT_COORD_2D_STRU startPoint = {stCenter.x + radius * std::cos(startAngle), stCenter.y + radius * std::sin(startAngle)};
        POINT_COORD_2D_STRU endPoint   = {stCenter.x + radius * std::cos(endAngle),   stCenter.y + radius * std::sin(endAngle)};

        // 更新边界值
        minX = std::min(minX, std::min(startPoint.x, endPoint.x));
        maxX = std::max(maxX, std::max(startPoint.x, endPoint.x));
        minY = std::min(minY, std::min(startPoint.y, endPoint.y));
        maxY = std::max(maxY, std::max(startPoint.y, endPoint.y));

        // 检查特殊角度（-90, 0, 90, 180 度）是否在扇形范围内
        auto checkAngle = [&](double angle) {
            if (startAngle <= angle && angle <= endAngle) {
                POINT_COORD_2D_STRU point = {stCenter.x + radius * std::cos(angle), stCenter.y + radius * std::sin(angle)};
                minX = std::min(minX, point.x);
                maxX = std::max(maxX, point.x);
                minY = std::min(minY, point.y);
                maxY = std::max(maxY, point.y);
            }
        };

        checkAngle(-PI / 2);
        checkAngle( 0);
        checkAngle( PI / 2);
        checkAngle( PI);

        return RECTANGLE_STRU(minX, maxX, minY, maxY);
    }

    float              radius;          // 半径
    float              radiusSqu;       // 半径的平方
    float              startAngle;      // 起始角
    float              endAngle;        // 终止角
    COORDINATE_2D_STRU stCenter;        // 圆心
    RECTANGLE_STRU     stBoundingRect;  // 外接矩形
};

struct TRIANGLE_STRU {
    TRIANGLE_STRU() {}
    TRIANGLE_STRU(const COORDINATE_2D_STRU& _vertexO,
                  const COORDINATE_2D_STRU& _vertexA,
                  const COORDINATE_2D_STRU& _vertexB) {
        SetValue(_vertexO, _vertexA, _vertexB);
    }
    TRIANGLE_STRU(const COORDINATE_2D_STRU& _vertexO,
                  const float& _startAngle,
                  const float& _startSide,
                  const float& _endAngle,
                  const float& _endSide) {
        SetValue(_vertexO, _startAngle, _startSide, _endAngle, _endSide);
    }

    /* 设置三角形，其中O为三角形的顶点，A、B为任一点，无顺序限制 */
    void SetValue(const COORDINATE_2D_STRU& _vertexO,
                  const COORDINATE_2D_STRU& _vertexA,
                  const COORDINATE_2D_STRU& _vertexB) {
        vertexO = _vertexO;
        vertexA = _vertexA;
        vertexB = _vertexB;

        float angleOA = atan2(vertexA.y - vertexO.y, vertexA.x - vertexO.x);    // -π ~ π
        float angleOB = atan2(vertexB.y - vertexO.y, vertexB.x - vertexO.x);    // -π ~ π
        float sideOA  = sqrt(vertexO.CalcDistSqu(vertexA));
        float sideOB  = sqrt(vertexO.CalcDistSqu(vertexB));

        angleOA = angleOA < 0 ? angleOA + PI * 2 : angleOA;   // 0 ~ 2π
        angleOB = angleOB < 0 ? angleOB + PI * 2 : angleOB;   // 0 ~ 2π

        if (angleOA < angleOB) {
            if (ABS(angleOA - angleOB) < PI) {
                startAngle = angleOA;
                startSide  = sideOA;
                endAngle   = angleOB;
                endSide    = sideOB;
            } else {
                startAngle = angleOB;
                startSide  = sideOB;
                endAngle   = angleOA;
                endSide    = sideOA;
            }
        } else {
            if (ABS(angleOA - angleOB) < PI) {
                startAngle = angleOB;
                startSide  = sideOB;
                endAngle   = angleOA;
                endSide    = sideOA;
            } else {
                startAngle = angleOA;
                startSide  = sideOA;
                endAngle   = angleOB;
                endSide    = sideOB;
            }
        }

        if (startAngle > PI) startAngle -= PI * 2;  // -π ~ π
        if (endAngle   > PI) endAngle   -= PI * 2;  // -π ~ π
    }

    /* 设置三角形，其中O为三角形的顶点，0 < endAngle - startAngle < PI（象限角度差） */
    void SetValue(const COORDINATE_2D_STRU& _vertexO,
                  const float& _startAngle,
                  const float& _startSide,
                  const float& _endAngle,
                  const float& _endSide) {
        vertexO    = _vertexO;
        startAngle = fmod(_startAngle, 2 * PI);
        startSide  = _startSide;
        endAngle   = fmod(_endAngle,   2 * PI);
        endSide    = _endSide;

        if (startAngle >  PI) startAngle -= PI * 2; // -π ~ 0
        if (startAngle < -PI) startAngle += PI * 2; //  0 ~ π
        if (endAngle   >  PI) endAngle   -= PI * 2; // -π ~ 0
        if (endAngle   < -PI) endAngle   += PI * 2; //  0 ~ π

        vertexA.x = vertexO.x + startSide * cos(startAngle);    // 此处将OA设置为起始边（逆时针）
        vertexA.y = vertexO.y + startSide * sin(startAngle);
        vertexB.x = vertexO.x + endSide   * cos(endAngle);      // 此处将OB设置为终止边（逆时针）
        vertexB.y = vertexO.y + endSide   * sin(endAngle);
    }

    /* 计算向量叉积的符号（正、负或零） */
    float CrossProduct(const COORDINATE_2D_STRU& O, const COORDINATE_2D_STRU& A, const COORDINATE_2D_STRU& B) const {
        return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
    }

    /* 判断点是否在三角形内部 */
    template<typename T>
    bool IsIn(const T& coor) const {
        float dOA2OP = CrossProduct(vertexO, vertexA, COORDINATE_2D_STRU(coor.x, coor.y));
        float dOP2OB = CrossProduct(vertexO, COORDINATE_2D_STRU(coor.x, coor.y), vertexB);
        float dOA2OB = CrossProduct(vertexO, vertexA, vertexB);
        /**
         * 如果点coor在三角形的每条边的同一侧（内部或外部），则dOA、dAB和dBO的符号应该相同
         * 如果它们都是正的、负的或零（但在这里零的情况只会在coor位于边上时发生，我们不考虑这种情况为内部）
         * 并且它们都不为零（因为零表示coor在边上，我们也不考虑这种情况为内部）
         */
        //bool hasNeg = (dOA < 0) || (dAB < 0) || (dBO < 0);
        //bool hasPos = (dOA > 0) || (dAB > 0) || (dBO > 0);

        //return !(hasNeg && hasPos); // 如果不是既有正又有负，则点P在三角形内部或边上（但我们只关心内部）
        if      (dOA2OB > 0) return dOA2OP >= 0 && dOP2OB >= 0;
        else if (dOA2OB < 0) return dOA2OP <= 0 && dOP2OB <= 0;
        else return false;
    }

    /* 三角形表示方式1：三个顶点O、A、B */
    COORDINATE_2D_STRU vertexO;
    COORDINATE_2D_STRU vertexA;
    COORDINATE_2D_STRU vertexB;

    /* 三角形表示方式2：一个顶点O、及顶角O处的起止角和边长 */
    float startAngle;   // 起始角(-π ~ π)
    float startSide;    // 起始边长
    float endAngle;     // 终止角(-π ~ π)
    float endSide;      // 终止边长
};

struct TRI_PRISM_STRU {
    void SetValue(float tempMinX, float tempMaxX, float tempMinY,
                  float tempMaxY, float tempMinZ, float tempMaxZ )
    {
        minX  = tempMinX;   maxX = tempMaxX;
        minY  = tempMinY;   maxY = tempMaxY;
        minZ  = tempMinZ;   maxZ = tempMaxZ;
        slope = (tempMaxZ - tempMinZ)/(tempMinX - tempMaxX);
        intercept = tempMaxZ - slope* tempMinX;
    }

    BOOL IsInTriPrism(float x, float y, float z)
    {
        if(x < minX || x > maxX) return false;
        if(y < minY || y > maxY) return false;
        if((z - slope*x - intercept)/sqrt(slope*slope + 1) > 0.05){
            return false;
        }
        return true;
    }

    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
    float slope;
    float intercept;
};

struct TETRAHEDRON_STRU {
    void SetValue(COORDINATE_3D_STRU vertex0, COORDINATE_3D_STRU vertex1,
                  COORDINATE_3D_STRU vertex2, COORDINATE_3D_STRU vertex3)
    {
        mVertex << vertex0.x, vertex0.y, vertex0.z, 1.0,
                   vertex1.x, vertex1.y, vertex1.z, 1.0,
                   vertex2.x, vertex2.y, vertex2.z, 1.0,
                   vertex3.x, vertex3.y, vertex3.z, 1.0;
    }

    BOOL IsInTetrahedron(float x, float y, float z)
    {
        float det0 = mVertex.determinant();

        for (INT32 i = 0; i < 4; i++){
            Eigen::Matrix4f m = mVertex;
            m.row(i)   = Eigen::Vector4f(x, y, z, 1.0);
            float det1 = m.determinant();
            if (det0 * det1 < 0){
                return false;
            }
        }
        return true;
    }

    Eigen::Matrix4f mVertex;
};

struct PYRAMID_STRU {
    void SetValue(float tempMinX, float tempMaxX, float tempMinY,
                  float tempMaxY, COORDINATE_3D_STRU tempVertex)
    {
        minX = tempMinX;  maxX = tempMaxX;
        minY = tempMinY;  maxY = tempMaxY;
        vertex = tempVertex;

        COORDINATE_3D_STRU vertex1, vertex2, vertex3, vertex4 ;
        vertex1.SetValue(minX, minY, -0.1);
        vertex2.SetValue(minX, maxY, -0.1);
        vertex3.SetValue(maxX, maxY, -0.1);
        vertex4.SetValue(maxX, minY, -0.1);
        tetrahedron0.SetValue(vertex,vertex1,vertex2,vertex3);
        tetrahedron1.SetValue(vertex,vertex3,vertex4,vertex1);
    }

    BOOL IsInPyramid(float x, float y, float z)
    {
        if(x < minX || x > maxX) return false;
        if(y < minY || y > maxY) return false;
        if(   !tetrahedron0.IsInTetrahedron(x, y, z)
           && !tetrahedron1.IsInTetrahedron(x, y, z)){
            return false;
        }

        return true;
    }

    float  minX;
    float  maxX;
    float  minY;
    float  maxY;
    COORDINATE_3D_STRU vertex;
    TETRAHEDRON_STRU   tetrahedron0;
    TETRAHEDRON_STRU   tetrahedron1;
};

struct POS_VEL_STRU {
    POS_VEL_STRU() : objectID(0), posX(0), posY(0), velX(0), velY(0) {}

    uint32_t objectID;
    float    posX;
    float    posY;
    float    velX;
    float    velY;
};

struct POSE_ID_STRU {
    double              time;
    UINT32              id;
    COOR_3D_POSE_STRU   stPose;

    POSE_ID_STRU(void) {
        SetZero();
    }

    void SetZero(void) {
        time = 0.0;
        id   = 10000;
        stPose.SetZero();
    }
};

struct VEL_DATA_STRU {
    double      time;
    Vector3f    linearVel;
    Vector3f    angleVel;

    VEL_DATA_STRU(void){
        SetZero();
    }

    VEL_DATA_STRU(const VEL_DATA_STRU &stSrcVelData){
        SetValue(stSrcVelData.time, stSrcVelData.linearVel, stSrcVelData.angleVel);
    }

    void Copy(const VEL_DATA_STRU &stSrcVelData){
        SetValue(stSrcVelData.time, stSrcVelData.linearVel, stSrcVelData.angleVel);
    }

    void SetZero(void){
        SetValue(0.0, Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 0.0));
    }

    void SetValue(const double srcTime, const Vector3f &srcLinearVel, const Vector3f &srcAngleVel){
        time      = srcTime;
        linearVel = srcLinearVel;
        angleVel  = srcAngleVel;
    }
};

struct WHEEL_VEL_DATA_STRU {
    double time;
    float  vel;
};

struct ODOM_DATA_STRU {
    double             time;
    COOR_3D_POSE_STRU  stPose;
    VEL_DATA_STRU      stVelData;

    ODOM_DATA_STRU(void){
        SetZero();
    }

    void Copy(const ODOM_DATA_STRU &stOdomData)
    {
        SetValue(stOdomData.time, stOdomData.stPose, stOdomData.stVelData);
    }

    void SetZero(void)
    {
        time = 0;;
        stPose.SetZero();
        stVelData.SetZero();
    }

    void SetValue(const double &srcTime, const COOR_3D_POSE_STRU &stSrcPose, const VEL_DATA_STRU &stSrcVelData)
    {
        time = srcTime;
        stPose.Copy(stSrcPose);
        stVelData.Copy(stSrcVelData);
    }
};

struct RADAR_INFO_STRU {
    RADAR_INFO_STRU() : radarID(0), timeStamp(0.0) {}

    UINT08 radarID;
    double timeStamp;
    std::vector<POS_VEL_STRU> vstPosVel;
};

// deg-->rad
inline double Deg2Rad(const double &value)
{
    const double kC = M_PI / 180.0;
    return (value * kC);
}

// rad-->deg
inline double Rad2Deg(const double &value)
{
    const double kC = 180.0 / M_PI;
    return (value * kC);
}

/**************************************************************************************
功能描述: 获取最大值
修改记录:
**************************************************************************************/
inline float GetMax(float data1, float data2, float data3, float data4)
{
    float max1 = MAX(data1, data2);
    float max2 = MAX(data3, data4);
    return MAX(max1, max2);
}

/**************************************************************************************
功能描述: 获取最小值
修改记录:
**************************************************************************************/
inline float GetMin(float data1, float data2, float data3, float data4)
{
    float min1 = MIN(data1, data2);
    float min2 = MIN(data3, data4);
    return MIN(min1, min2);
}

/**************************************************************************************
功能描述: 打印码流
修改记录:
**************************************************************************************/
void Comm_PrintStream(const CHAR *pucDes, const UINT08 *pucStream, UINT32 streamLen);
void Comm_PrintCvVec3dInfo(const std::string &strDes, const cv::Vec3d &objVec);
void Comm_PrintCvMatInfo(const std::string &strDes, const cv::Mat &objMat);
void Comm_PrintEgVec3dInfo(const std::string &strDes, const Eigen::Vector3d &objVec);
template <typename T>
void Comm_PrintEgMatInfo(const std::string &strDes, const T &objMat)
{
    cv::Mat objCvMat;
    cv::eigen2cv(objMat, objCvMat);
    Comm_PrintCvMatInfo(strDes, objCvMat);
}

template <typename UINT_WRAPPER>
void Comm_GetUintStrLines(const std::vector<UINT_WRAPPER> &vectUintWrapper, std::function<UINT32(const UINT_WRAPPER &)> objGetUintValFunc, UINT32 uintStrWidth, UINT32 maxUintNumPerLine, std::vector<std::string> &vectUintStrLine)
{
    std::string strUintLine;
    UINT32      uintNumJoined   = 0;
    UINT32      uintWrapperSize = UINT32(vectUintWrapper.size());

    for (UINT32 loop = 0; loop < uintWrapperSize; ++loop) {
        UINT32 uintVal = objGetUintValFunc(vectUintWrapper[loop]);
        strUintLine.append(ToString(uintVal, uintStrWidth, ' '));

        ++uintNumJoined;
        if ((uintNumJoined >= maxUintNumPerLine) || ((loop + 1) == uintWrapperSize)) {
            vectUintStrLine.push_back(strUintLine);
            strUintLine.clear();
            uintNumJoined = 0;
        }
    }
}


float CalcDistSqu(const COORDINATE_2D_STRU &stPos1, const COORDINATE_2D_STRU &stPos2);
float CalcAngleDiffAbs(float angle1, float angle2);
float Calc2PointLineAngle(const COORDINATE_2D_STRU *pstPos1, const COORDINATE_2D_STRU *pstPos2);
float AdjustAngle(float srcAngle);
float NormalizeAngle(float angle);
INT32 Comm_FitPlaneOfAxByEquZ(const std::vector<POINT_COORD_3D_STRU> &vectToFitPoint, POINT_COORD_2D_STRU &stParam);
bool  ComparePointToPointRelAngle(COORDINATE_2D_STRU *pstPosBase, COORDINATE_2D_STRU *pstPos1, COORDINATE_2D_STRU *pstPos2);

class ANGLE_C
{
public:
    ANGLE_C(float angle = 0.0)
    {
        Set(angle);
    }

    ANGLE_C(const ANGLE_C &objAngle)
    {
        *this = objAngle;
    }

    ANGLE_C &operator=(double angle)
    {
        Set(float(angle));
        return *this;
    }

    ANGLE_C &operator=(const ANGLE_C &objAngle)
    {
        /* 不判断是否自己，否则使用时报编译告警 */
        angle_      = objAngle.angle_;
        isExistSin_ = objAngle.isExistSin_;
        angleSin_   = objAngle.angleSin_;
        isExistCos_ = objAngle.isExistCos_;
        angleCos_   = objAngle.angleCos_;

        return *this;
    }

    ANGLE_C operator+(double angle) const
    {
        return ANGLE_C(angle_ + angle);
    }

    ANGLE_C operator+(const ANGLE_C &objAngle) const
    {
        ANGLE_C objNewAngle(*this);

        objNewAngle.Add(objAngle);
        return objNewAngle;
    }

    friend ANGLE_C operator+(double angle1, const ANGLE_C &objAngle2)
    {
        return ANGLE_C(angle1 + objAngle2.angle_);
    }

    ANGLE_C operator-(void) const
    {
        return ANGLE_C(-1.0 * angle_);
    }

    ANGLE_C operator-(double angle) const
    {
        return ANGLE_C(angle_ - angle);
    }

    ANGLE_C operator-(const ANGLE_C &objAngle) const
    {
        ANGLE_C objNewAngle(*this);

        objNewAngle.Sub(objAngle);
        return objNewAngle;
    }

    friend ANGLE_C operator-(double angle1, const ANGLE_C &objAngle2)
    {
        return ANGLE_C(angle1 - objAngle2.angle_);
    }

    BOOL operator>(double angle) const
    {
        return (angle_ > angle);
    }

    BOOL operator>(const ANGLE_C &objAngle) const
    {
        return (angle_ > objAngle.angle_);
    }

    friend BOOL operator>(double angle1, const ANGLE_C &objAngle2)
    {
        return (angle1 > objAngle2.angle_);
    }

    BOOL operator<(double angle) const
    {
        return (angle_ < angle);
    }

    BOOL operator<(const ANGLE_C &objAngle) const
    {
        return (angle_ < objAngle.angle_);
    }

    friend BOOL operator<(double angle1, const ANGLE_C &objAngle2)
    {
        return (angle1 < objAngle2.angle_);
    }

    ANGLE_C &Add(double angle)
    {
        return Set(angle_ + angle);
    }

    ANGLE_C &Add(const ANGLE_C &objAngle)
    {
        angle_ = AdjustAngle(angle_ + objAngle.angle_);

        if (isExistSin_ && isExistCos_ && objAngle.isExistSin_ && objAngle.isExistCos_) {
            float newSin = angleSin_ * objAngle.angleCos_ + angleCos_ * objAngle.angleSin_;
            float newCos = angleCos_ * objAngle.angleCos_ - angleSin_ * objAngle.angleSin_;
            angleSin_ = newSin;
            angleCos_ = newCos;
        } else {
            isExistSin_ = false;
            isExistCos_ = false;
        }

        return *this;
    }

    ANGLE_C &Sub(double angle)
    {
        return Set(angle_ - angle);
    }

    ANGLE_C &Sub(const ANGLE_C &objAngle)
    {
        angle_ = AdjustAngle(angle_ - objAngle.angle_);

        if (isExistSin_ && isExistCos_ && objAngle.isExistSin_ && objAngle.isExistCos_) {
            float newSin = angleSin_ * objAngle.angleCos_ - angleCos_ * objAngle.angleSin_;
            float newCos = angleCos_ * objAngle.angleCos_ + angleSin_ * objAngle.angleSin_;
            angleSin_ = newSin;
            angleCos_ = newCos;
        } else {
            isExistSin_ = false;
            isExistCos_ = false;
        }

        return *this;
    }

    float Val(void) const
    {
        return angle_;
    }

    float ValAbs(void) const
    {
        return ABS(angle_);
    }

    float Sin(void) const
    {
        if (!isExistSin_) {
            angleSin_ = sin(angle_);
            isExistSin_ = true;
        }

        return angleSin_;
    }

    float Cos(void) const
    {
        if (!isExistCos_) {
            angleCos_ = cos(angle_);
            isExistCos_ = true;
        }

        return angleCos_;
    }

    float SinOfAddHalfPi(void) const
    {
        return Cos();
    }

    float CosOfAddHalfPi(void) const
    {
        return -1.0 * Sin();
    }

    float SinOfSubHalfPi(void) const
    {
        return -1.0 * Cos();
    }

    float CosOfSubHalfPi(void) const
    {
        return Sin();
    }

    ANGLE_C &SetAbs(void)
    {
        if (angle_ < 0) {
            angle_ = -1.0 * angle_;

            if (isExistSin_) {
                angleSin_ = -1.0 * angleSin_;
            }
        }

        return *this;
    }

private:
    float angle_;

    mutable bool  isExistSin_;
    mutable float angleSin_;
    mutable bool  isExistCos_;
    mutable float angleCos_;

    inline ANGLE_C &Set(float angle)
    {
        angle_      = AdjustAngle(angle);
        isExistSin_ = false;
        angleSin_   = 0.0;
        isExistCos_ = false;
        angleCos_   = 0.0;

        return *this;
    }

    inline float AdjustAngle(float srcAngle) const
    {
        if        (srcAngle > PI) {
            return (srcAngle - 2.0 * PI);
        } else if (srcAngle < (-PI)) {
            return (srcAngle + 2.0 * PI);
        } else {
            return srcAngle;
        }
    }
};

class POSE_2D_C
{
public:
    COORDINATE_2D_STRU stPos;
    ANGLE_C            objDir;

    POSE_2D_C(void)
    {
        SetZero();
    }

    POSE_2D_C(const POSE_2D_C &objPose)
    {
        *this = objPose;
    }

    POSE_2D_C(const COOR_2D_POSE_STRU &stPose)
    {
        *this = stPose;
    }

    POSE_2D_C(const COORDINATE_2D_STRU &stSrcPos, const ANGLE_C &objSrcAngle)
    {
        stPos.Copy(stSrcPos);
        objDir = objSrcAngle;
    }

    const POSE_2D_C &operator=(const POSE_2D_C &objPose)
    {
        if (this != &objPose) {
            stPos.Copy(objPose.stPos);
            objDir = objPose.objDir;
        }

        return *this;
    }

    const POSE_2D_C &operator=(const COOR_2D_POSE_STRU &stPose)
    {
        stPos.Copy(stPose.stPos);
        objDir = stPose.dirAngle;

        return *this;
    }

    void SetZero(void)
    {
        stPos.SetZero();
        objDir = 0.0;
    }

    COOR_2D_POSE_STRU ToOld(void) const
    {
        return COOR_2D_POSE_STRU(stPos.x, stPos.y, objDir.Val());
    }

    void TransLeft(float dist, POSE_2D_C &objNewPose)
    {
        objNewPose.stPos.x = stPos.x + objDir.CosOfAddHalfPi() * dist;
        objNewPose.stPos.y = stPos.y + objDir.SinOfAddHalfPi() * dist;
        objNewPose.objDir  = objDir;
    }
};

class AREA_C
{
public:
    COORDINATE_2D_STRU stLBPoint;   /* 左下角坐标点 */
    COORDINATE_2D_STRU stRTPoint;   /* 右上角坐标点 */

    AREA_C(void)
    {
        Reset();
    }

    void Reset(void)
    {
        isInit_ = false;
        stLBPoint.SetZero();
        stRTPoint.SetZero();
    }

    BOOL IsValid(void) const
    {
        return isInit_;
    }

    float GetMeasure(void) const
    {
        return isInit_ ? ((stRTPoint.x - stLBPoint.x) * (stRTPoint.y - stLBPoint.y)) : 0.0;
    }

    float GetSizeX(void) const
    {
        return isInit_ ? (stRTPoint.x - stLBPoint.x) : 0.0;
    }

    float GetSizeY(void) const
    {
        return isInit_ ? (stRTPoint.y - stLBPoint.y) : 0.0;
    }

    void IncludePoint(const COORDINATE_2D_STRU &stPoint)
    {
        if (!isInit_) {
            isInit_ = true;
            stLBPoint.Copy(stPoint);
            stRTPoint.Copy(stPoint);
        } else {
            stLBPoint.x = MIN(stLBPoint.x, stPoint.x);
            stLBPoint.y = MIN(stLBPoint.y, stPoint.y);
            stRTPoint.x = MAX(stRTPoint.x, stPoint.x);
            stRTPoint.y = MAX(stRTPoint.y, stPoint.y);
        }
    }

    void IncludePoints(const std::vector<COORDINATE_2D_STRU> &vectPoint)
    {
        for (const COORDINATE_2D_STRU &stPoint : vectPoint) {
            IncludePoint(stPoint);
        }
    }

    void IncludePoint(float pointX, float pointY)
    {
        COORDINATE_2D_STRU stPoint = {pointX, pointY};

        IncludePoint(stPoint);
    }

    void IncludeArea(const AREA_C &objOtherArea)
    {
        if (!objOtherArea.isInit_) {
            return;
        }

        IncludePoint(objOtherArea.stLBPoint);
        IncludePoint(objOtherArea.stRTPoint);
    }

    void Expand(float expandX, float expandY)
    {
        if ((expandX < 0) || (expandY < 0)) {
            return;
        }

        if (isInit_) {
            stLBPoint.x -= expandX;
            stLBPoint.y -= expandY;

            stRTPoint.x += expandX;
            stRTPoint.y += expandY;
        }
    }

    void Expand(float expandDist)
    {
        Expand(expandDist, expandDist);
    }

    BOOL IsIn(const COORDINATE_2D_STRU &stPoint) const
    {
        return (   isInit_
                && (stPoint.x >= stLBPoint.x) && (stPoint.y >= stLBPoint.y)
                && (stPoint.x <= stRTPoint.x) && (stPoint.y <= stRTPoint.y));
    }

    BOOL IsExistOverlapArea(const AREA_C &objOtherArea) const
    {
        /* 重叠一个点也算重叠 */
        if (!isInit_ || !objOtherArea.isInit_) {
            return false;
        }

        return !(   (objOtherArea.stRTPoint.x < this->stLBPoint.x)
                 || (objOtherArea.stRTPoint.y < this->stLBPoint.y)
                 || (objOtherArea.stLBPoint.x > this->stRTPoint.x)
                 || (objOtherArea.stLBPoint.y > this->stRTPoint.y));
    }

    /* 获取与另一个区域的重叠区域 */
    void GetOverlapArea(const AREA_C &objOtherArea, AREA_C &objOverlapArea) const
    {
        if (!this->isInit_ || !objOtherArea.isInit_) {
            objOverlapArea.Reset();
        } else {
            objOverlapArea.stLBPoint.x = MAX(this->stLBPoint.x, objOtherArea.stLBPoint.x);
            objOverlapArea.stLBPoint.y = MAX(this->stLBPoint.y, objOtherArea.stLBPoint.y);
            objOverlapArea.stRTPoint.x = MIN(this->stRTPoint.x, objOtherArea.stRTPoint.x);
            objOverlapArea.stRTPoint.y = MIN(this->stRTPoint.y, objOtherArea.stRTPoint.y);
            if (   (objOverlapArea.stLBPoint.x <= objOverlapArea.stRTPoint.x)
                && (objOverlapArea.stLBPoint.y <= objOverlapArea.stRTPoint.y)) {
                objOverlapArea.isInit_ = true;
            }
        }
    }

    /* 获取与另一个区域的非重叠区域 */
    void GetNonOverlapAreas(const AREA_C &objOtherArea, std::vector<AREA_C> &vectArea) const
    {
        if (!this->isInit_) {
            return;
        }

        AREA_C objOverlapArea;
        this->GetOverlapArea(objOtherArea, objOverlapArea);
        if (!objOverlapArea.IsValid()) {
            vectArea.push_back(*this);
            return;
        }

        if (objOverlapArea.stLBPoint.x > this->stLBPoint.x) {
            AREA_C objArea;
            objArea.IncludePoint(this->stLBPoint);
            objArea.IncludePoint(objOverlapArea.stLBPoint.x, this->stRTPoint.y);
            vectArea.push_back(objArea);
        }

        if (objOverlapArea.stRTPoint.x < this->stRTPoint.x) {
            AREA_C objArea;
            objArea.IncludePoint(this->stRTPoint);
            objArea.IncludePoint(objOverlapArea.stRTPoint.x, this->stLBPoint.y);
            vectArea.push_back(objArea);
        }

        if (objOverlapArea.stLBPoint.y > this->stLBPoint.y) {
            AREA_C objArea;
            objArea.IncludePoint(objOverlapArea.stLBPoint.x, this->stLBPoint.y);
            objArea.IncludePoint(objOverlapArea.stRTPoint.x, objOverlapArea.stLBPoint.y);
            vectArea.push_back(objArea);
        }

        if (objOverlapArea.stRTPoint.y < this->stRTPoint.y) {
            AREA_C objArea;
            objArea.IncludePoint(objOverlapArea.stLBPoint.x, objOverlapArea.stRTPoint.y);
            objArea.IncludePoint(objOverlapArea.stRTPoint.x, this->stRTPoint.y);
            vectArea.push_back(objArea);
        }
    }

    INT32 GetLTPoint(COORDINATE_2D_STRU &stPoint) const
    {
        if (!IsValid()) {
            return -1;
        }

        stPoint.x = stLBPoint.x;
        stPoint.y = stRTPoint.y;
        return 0;
    }

    INT32 GetRBPoint(COORDINATE_2D_STRU &stPoint) const
    {
        if (!IsValid()) {
            return -1;
        }

        stPoint.x = stRTPoint.x;
        stPoint.y = stLBPoint.y;
        return 0;
    }

    INT32 GetBorder(std::vector<COORDINATE_2D_STRU> &vectBorderPoint) const
    {
        if (!IsValid()) {
            return -1;
        }

        vectBorderPoint.push_back(stLBPoint);
        vectBorderPoint.push_back(COORDINATE_2D_STRU(stLBPoint.x, stRTPoint.y));
        vectBorderPoint.push_back(stRTPoint);
        vectBorderPoint.push_back(COORDINATE_2D_STRU(stRTPoint.x, stLBPoint.y));
        return 0;
    }

    float GetDistToPoint(const COORDINATE_2D_STRU &stPoint) const
    {
        if (!IsValid()) {
            return -1.0;
        }

        COORDINATE_2D_STRU stRefPoint;
        stRefPoint.x = LIMIT(stPoint.x, stLBPoint.x, stRTPoint.x);
        stRefPoint.y = LIMIT(stPoint.y, stLBPoint.y, stRTPoint.y);
        return std::sqrt(stPoint.CalcDistSqu(stRefPoint));
    }

private:
    BOOL isInit_;
};

/**************************************************************************************
功能描述: 坐标系转换
修改记录:
**************************************************************************************/
class COOR_TF_C
{
public:
    COOR_TF_C(void);
    COOR_TF_C(const COORDINATE_2D_STRU &stSonOriginPosByFather, float sonDirAngleByFather);
    COOR_TF_C(const COOR_2D_POSE_STRU &stSonOriginPoseByFather);
    COOR_TF_C(float sonDirAngleByFather);

    void SetTransRelationship(const COORDINATE_2D_STRU &stSonOriginPosByFather, float sonDirAngleByFather);
    void SetTransRelationship(const COORDINATE_2D_STRU &stSonOriginPosByFather, float sonDirAngleByFather, float sonDirAngleByFatherSin, float sonDirAngleByFatherCos);
    void SetTransRelationship(const COOR_2D_POSE_STRU &stSonOriginPoseByFather);
    void SetTransRelationship(const COOR_2D_POSE_STRU &stPoseInFather, const COOR_2D_POSE_STRU &stPoseInSon);
    void SetTransRelationship(const COORDINATE_2D_STRU &stPos1InFather, const COORDINATE_2D_STRU &stPos1InSon,
                              const COORDINATE_2D_STRU &stPos2InFather, const COORDINATE_2D_STRU &stPos2InSon);

    void TransSonPosToFather(const COORDINATE_2D_STRU &stSonPos, COORDINATE_2D_STRU &stFatherPos) const;
    void TransFatherPosToSon(const COORDINATE_2D_STRU &stFatherPos, COORDINATE_2D_STRU &stSonPos) const;

    void TransSonPoseToFather(const COOR_2D_POSE_STRU &stSonPose, COOR_2D_POSE_STRU &stFatherPose) const;
    void TransFatherPoseToSon(const COOR_2D_POSE_STRU &stFatherPose, COOR_2D_POSE_STRU &stSonPose) const;

    float TransSonAngleToFather(float sonAngle) const;
    float TransFatherAngleToSon(float fatherAngle) const;

    void GetSonOriginPoseByFather(COOR_2D_POSE_STRU &stSonOriginPoseByFather) const;

private:
    COOR_2D_POSE_STRU stSonOriginPoseByFather_;
    float sonDirAngleByFatherSin_;
    float sonDirAngleByFatherCos_;
};

#ifndef _______Main___________
#endif

/**************************************************************************************
功能描述: COMM模块初始化 C接口，与C++接口二选一。
          Init和Uninit最好成对调用。
修改记录:
**************************************************************************************/
INT32 Comm_Init(int runFrequency, float abnCycleDiffRate = 0.1);
INT32 Comm_Init2(int argc, char** argv, int runFrequency, float abnCycleDiffRate = 0.1);
void  Comm_Uninit(void);
void  Comm_UptByPeriod(void);

/**************************************************************************************
功能描述: COMM模块初始化 C++接口，与C接口二选一。
修改记录:
**************************************************************************************/
class COMM_BASE_C
{
public:
    COMM_BASE_C(void)
    {
    }

    ~COMM_BASE_C(void)
    {
        Comm_Uninit();
    }

    COMM_BASE_C(const COMM_BASE_C &other) = delete;
    COMM_BASE_C &operator=(const COMM_BASE_C &) = delete;

    INT32 Init(int argc, char** argv, int runFrequency, float abnCycleDiffRate = 0.1)
    {
        return Comm_Init2(argc, argv, runFrequency, abnCycleDiffRate);
    }

    void  PeriodUpt(void)
    {
        return Comm_UptByPeriod();
    }

private:
};

/**************************************************************************************
功能描述: 是否仿真运行模式
修改记录:
**************************************************************************************/
BOOL  Comm_IsSimulationMode(void);


#endif

