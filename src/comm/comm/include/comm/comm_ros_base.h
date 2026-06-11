

#ifndef __COMM_ROS_BASE_H__
#define __COMM_ROS_BASE_H__

#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include "comm/comm_msg.h"
#include "comm/comm_serial.h"
#include "comm/comm_map.h"
#include "comm/comm_topic_name.h"
#include <comm_msg/locationInfo.h>
#include <comm_msg/wheelSpeed.h>
#include <comm_msg/imudata.h>
#include <comm_msg/zhishancmd.h>
#include <comm_msg/gpsInfo.h>
#include <comm_msg/radarTargetArray.h>
#include <comm_msg/carBodyFilter.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PolygonStamped.h>
#include <tf/transform_broadcaster.h>
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
#include <visualization_msgs/Marker.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 发送组合地图位置信息
修改记录:
**************************************************************************************/
class LOCATION_MSG_SENDER_C
{
public:
    LOCATION_MSG_SENDER_C(ros::NodeHandle &nh, const string &strTopicName = TOPIC_NAME_LOCATION_INFO)
    {
        publisher_ = nh.advertise<comm_msg::locationInfo>(strTopicName.c_str(), 1);
    }

    void Send(MAP_NODE_STATE_ENUM enState, int subMapNo, const COOR_2D_POSE_STRU &stPose);

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 接收组合地图位置信息
修改记录:
**************************************************************************************/
class LOCATION_MSG_RECEIVER_C
{
public:
    LOCATION_MSG_RECEIVER_C(ros::NodeHandle &nh, double overTime = 0.3)
    {
        objDataValidTimer_.SetOverTime(overTime);
        objSubscriber_ = nh.subscribe(TOPIC_NAME_LOCATION_INFO, 1, &LOCATION_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        enState_       = MAP_NODE_STATE_IDLE;
    }

    BOOL IsPrepareReady(void) const;
    bool IsNormal(void) const;

    void Get(COOR_2D_POSE_STRU &stPose) const;
    int  GetNormal(COOR_2D_POSE_STRU &stPose) const;
    const COOR_2D_POSE_STRU *GetNormal(void) const;

private:
    ros::Subscriber     objSubscriber_;
    TIMER_C             objDataValidTimer_;       /* 数据失效定时器，超时后数据无效 */
    BOOL                isPrepareReady_{false};
    MAP_NODE_STATE_ENUM enState_;
    int                 subMapNo_;
    COOR_2D_POSE_STRU   stPose_;

    void ReceiveMsgCallBack(const comm_msg::locationInfo::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 发送速度信息
修改记录:
**************************************************************************************/
class VEL_MSG_SENDER_C
{
public:
    VEL_MSG_SENDER_C(ros::NodeHandle &nh, const string &strTopicName = TOPIC_NAME_WHEEL_SPEED)
    {
        publisher_ = nh.advertise<comm_msg::wheelSpeed>(strTopicName.c_str(), 1);
    }

    void Send(float leftWheelVel, float rightWheelVel);

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收速度信息
修改记录:
**************************************************************************************/
class VEL_MSG_RECEIVER_C
{
public:
    VEL_MSG_RECEIVER_C(ros::NodeHandle &nh, double overTime = 0.2)
    {
        receiver_ = nh.subscribe(TOPIC_NAME_WHEEL_SPEED, 1, &VEL_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        objInvalidTimer_.SetOverTime(overTime);
        vel_ = 0.0;
    }

    int  Get(float &vel);
    bool IsAlmostStop(float stopVelLimit = 0.01);

private:
    ros::Subscriber receiver_;
    TIMER_C         objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    float           vel_;

    void ReceiveMsgCallBack(const comm_msg::wheelSpeed::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 发送IMU信息
修改记录:
**************************************************************************************/
class IMU_DATA_MSG_SENDER_C
{
public:
    IMU_DATA_MSG_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::imudata>("/imu_data", 1);
    }

    void Send(float angleVel, float yawAngle, float accX, float accY, float accZ);

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收IMU信息
修改记录:
**************************************************************************************/
class IMU_DATA_MSG_RECEIVER_C
{
public:
    IMU_DATA_MSG_RECEIVER_C(ros::NodeHandle &nh)
    {
        receiver_ = nh.subscribe("/imu_data", 1, &IMU_DATA_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        objInvalidTimer_.SetOverTime(0.2);
    }

    int Get(float &angleVel, float &angleYaw);

private:
    ros::Subscriber receiver_;
    TIMER_C         objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    float           angleVel_;
    float           angleYaw_;

    void ReceiveMsgCallBack(const comm_msg::imudata::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 发送ODOM话题
修改记录:
**************************************************************************************/
class ODOM_MSG_SENDER_C
{
public:
    ODOM_MSG_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<nav_msgs::Odometry>("/odom", 1);
    }

    void Send(COOR_2D_POSE_STRU &stPose, float vel, float angleVel);

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 接收ODOM话题
修改记录:
**************************************************************************************/
class ODOM_MSG_RECEIVER_C
{
public:
    ODOM_MSG_RECEIVER_C(ros::NodeHandle &nh)
    {
        receiver_ = nh.subscribe("/odom", 1, &ODOM_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        objInvalidTimer_.SetOverTime(0.15);
    }

    INT32 Get(COOR_2D_POSE_STRU &stPose);

private:
    ros::Subscriber     receiver_;

    TIMER_C             objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    COOR_2D_POSE_STRU   stPose_;

    void ReceiveMsgCallBack(const nav_msgs::Odometry::ConstPtr &pstMsg);
};

class TF_SENDER_C
{
public:
    void SendOdomToBaseFootTf(const COOR_2D_POSE_STRU &stBaseFootPoseByOdom);
    void SendMapToOdomTf(const COOR_2D_POSE_STRU &stOdomOriginPoseByMap);
    void SendBaseFootToBaseLinkTf(const COOR_2D_POSE_STRU &stBaseLinkPoseByFootPrint);
    void SendBaseLinkToScanTf(const COOR_2D_POSE_STRU &stScanPoseByBaseLink);

private:
    tf::TransformBroadcaster objBroadcaster_;
};

/**************************************************************************************
功能描述: 发送GPS
修改记录:
**************************************************************************************/
class GPS_INFO_MSG_SENDER_C
{
public:
    GPS_INFO_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = TOPIC_NAME_GNSS_INFO)
    {
        publisher_ = nh.advertise<comm_msg::gpsInfo>(pcTopicName, 1);
    }

    void Send(INT08 satelliteNum, INT08 rsltType, float hdop, double longitude, double latitude, float altitude, float yawAngle);
private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 接收GPS
          GPS数据更新比较慢
修改记录:
**************************************************************************************/
class GPS_INFO_MSG_RECERVER_C
{
public:
    GPS_INFO_MSG_RECERVER_C(ros::NodeHandle &nh, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_GNSS_INFO, 1, &GPS_INFO_MSG_RECERVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(GPS_INFO_STRU &stGpsInfo);

private:
    ros::Subscriber      receiver_;

    TIMER_C              objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    GPS_INFO_STRU        stGpsInfo_;

    void ReceiveMsgCallBack(const comm_msg::gpsInfo::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 发送毫米波雷达消息
修改记录:
**************************************************************************************/
class RADAR_MSG_SENDER_C
{
public:
    RADAR_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName)
    {
        publisher_ = nh.advertise<comm_msg::radarTargetArray>(pcTopicName, 1);
    }
    void Send(RADAR_INFO_STRU &stRadarInfo);
private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 发布地图数据
          缓存地图，无订阅时也需要发送；不缓存地图，需订阅才能发送
修改记录:
**************************************************************************************/
class OCC_MAP_MSG_SENDER_C
{
public:
    OCC_MAP_MSG_SENDER_C(ros::NodeHandle &nh, const string &strTopicName, BOOL isMsgBuffered = true) : isMsgBuffered_(isMsgBuffered)
    {
        publisher_ = nh.advertise<nav_msgs::OccupancyGrid>(strTopicName.c_str(), 1, isMsgBuffered);
    }

    ~OCC_MAP_MSG_SENDER_C(void)
    {
        if (!isMsgBuffered_) {
            Clear();
        }
    }

    UINT32 GetSubscriberNum(void) const;
    BOOL   IsExistSubscriber(void) const;

    void   SendMsg(COORDINATE_2D_STRU &stOriginPos, float resolution, UINT32 sizeX, UINT32 sizeY, UINT08 *pucMap) const;
    void   SendMsg(const GRAY_MAP_C &objMap);
    void   Clear(void);

private:
    static const INT32 aucCostToOcc_[256];

    ros::Publisher publisher_;
    BOOL           isMsgBuffered_;
};

class OCC_MAP_MSG_RECEIVER_C
{
public:
    OCC_MAP_MSG_RECEIVER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        receiver_ = nh.subscribe(strTopicName.c_str(), 1, &OCC_MAP_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    const GRAY_MAP_C &Get(void) const;

private:
    ros::Subscriber receiver_;
    GRAY_MAP_C      objMap_;

    void ReceiveMsgCallBack(const nav_msgs::OccupancyGrid::ConstPtr &pstMsg);
};

class ROS_POSE_SENDER_C
{
public:
    ROS_POSE_SENDER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        publisher_ = nh.advertise<geometry_msgs::PoseStamped>(strTopicName.c_str(), 1, true);
    }

    void SendMsg(const COOR_2D_POSE_STRU &stPose);

private:
    ros::Publisher publisher_;
};

class ROS_POLYGON_SENDER_C
{
public:
    ROS_POLYGON_SENDER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        publisher_ = nh.advertise<geometry_msgs::PolygonStamped>(strTopicName.c_str(), 1, true);
        isShowing_ = false;
    }

    BOOL IsEixstSubscriber(void);
    void SendMultiSideShapeMsg(const vector<COORDINATE_2D_STRU> &vctPos);
    void Send4SideShapeMsg(const COORDINATE_2D_STRU &stP1, const COORDINATE_2D_STRU &stP2, const COORDINATE_2D_STRU &stP3, const COORDINATE_2D_STRU &stP4);
    void Clear(void);

private:
    ros::Publisher publisher_;
    BOOL           isShowing_;
};

class ROS_AREA_RECEIVER_C
{
public:
    ROS_AREA_RECEIVER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        receiver_ = nh.subscribe(strTopicName.c_str(), 1, &ROS_AREA_RECEIVER_C::ReceiveMsgCallBack, this);

        objArea_.Reset();
    }

    const AREA_C &Get(void);

private:
    ros::Subscriber receiver_;
    AREA_C          objArea_;

    void ReceiveMsgCallBack(const geometry_msgs::PolygonStamped::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 发布路径
修改记录:
**************************************************************************************/
class PATH_SENDER_C
{
public:
    PATH_SENDER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        publisher_ = nh.advertise<nav_msgs::Path>(strTopicName.c_str(), 1, true);
    }

    BOOL IsExistSubscriber(void);
    void Send(const vector<COORDINATE_2D_STRU> &vctRoute);
    void Send(const list<COORDINATE_2D_STRU> &listRoute);

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 接收路径
修改记录:
**************************************************************************************/
class PATH_RECEIVER_C
{
public:
    PATH_RECEIVER_C(ros::NodeHandle &nh, const string &strTopicName, float timeoutDuration = 0.2)
    {
        receiver_ = nh.subscribe(strTopicName.c_str(), 1, &PATH_RECEIVER_C::ReceiveMsgCallBack, this);
        objInvalidTimer_.SetOverTime(timeoutDuration);
    }

    const vector<COORDINATE_2D_STRU> &Get(void);

private:
    ros::Subscriber            receiver_;
    TIMER_C                    objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    vector<COORDINATE_2D_STRU> vectPath_;

    void ReceiveMsgCallBack(const nav_msgs::Path::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 发布调试文字显示
修改记录:
**************************************************************************************/
class DSP_TEXT_SENDER_C
{
public:
    DSP_TEXT_SENDER_C(ros::NodeHandle &nh, const string &strTopicName)
    {
        publisher_ = nh.advertise<visualization_msgs::Marker>(strTopicName.c_str(), 1);
    }

    void Send(const COORDINATE_2D_STRU &stPos, const string &strDsp);

private:
    const float     textHeight_{0.5};
    ros::Publisher  publisher_;
};

#endif

