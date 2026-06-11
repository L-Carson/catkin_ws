#ifndef __COMM_ROS_BASE_EXTEND_H__
#define __COMM_ROS_BASE_EXTEND_H__

#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>

#include <nav_msgs/Path.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/PolygonStamped.h>
#include <geometry_msgs/Point32.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

#include "comm/comm_msg.h"
#include "comm/comm_base.h"
#include "comm_msg/move_info.h"
#include "comm_msg/locationInfo3D.h"
#include "comm_msg/pose2D.h"
#include "comm_msg/pose_score.h"
#include "comm_msg/qrcode_pose.h"
#include "comm_msg/equipment_pose.h"

#include "comm/comm_topic_name.h"
#include "comm_extend/comm_base_extend.h"

POINT_TYPE_ENUM GetPointType(const sensor_msgs::PointCloud2::ConstPtr &pstMsg);

class POINTCLOUD_MSG_RECEIVER_C
{
public:
    POINTCLOUD_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/points", const INT32 buffSize = 1)
    {
        receiver_ = nh.subscribe(pcTopicName, buffSize, &POINTCLOUD_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        enPointType_ = MY_POINT_TYPE_UNKNOW;
        isExist_ = false;
        objInvalidTimer_.SetOverTime(0.2);
    }

    INT32 Get(CLOUD_DATA_STRU &stCloud, BOOL isNodelay = true);

private:
    ros::Subscriber     receiver_;
    BOOL                isExist_;
    TIMER_C             objInvalidTimer_;
    POINT_TYPE_ENUM     enPointType_;
    CLOUD_DATA_STRU     stCloud_;

    void ReceiveMsgCallBack(const sensor_msgs::PointCloud2::ConstPtr &pstMsg);
};

class POINTCLOUD_MSG_SENDER_C
{
public:
    POINTCLOUD_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = "/currPoints", bool latch = false){
        publisher_ = nh.advertise<sensor_msgs::PointCloud2>(pcTopicName, 1, latch);
    };
    void Send(const PointCloudXYZI::Ptr pCloud);

private:
    ros::Publisher  publisher_;
};

class POINTCLOUD_XYZIRT_MSG_RECEIVER_C
{
public:
    POINTCLOUD_XYZIRT_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/points", const INT32 buffSize = 1)
    {
        receiver_ = nh.subscribe(pcTopicName, buffSize, &POINTCLOUD_XYZIRT_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        enPointType_ = MY_POINT_TYPE_UNKNOW;
        objInvalidTimer_.SetOverTime(0.2);
    }

    INT32 Get(CLOUD_XYZIRT_DATA_STRU &stCloud);

private:
    ros::Subscriber        receiver_;
    TIMER_C                objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    POINT_TYPE_ENUM        enPointType_;
    CLOUD_XYZIRT_DATA_STRU stCloud_;

    void ReceiveMsgCallBack(const sensor_msgs::PointCloud2::ConstPtr &pstMsg);
};

class POINTCLOUD_XYZIRT_MSG_SENDER_C
{
public:
    POINTCLOUD_XYZIRT_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = "/currPoints", bool latch = false)
    {
        publisher_ = nh.advertise<sensor_msgs::PointCloud2>(pcTopicName, 1, latch);
    };
    void Send(const PointCloudXYZIRT::Ptr pCloud);

private:
    ros::Publisher  publisher_;
};

class MAP_POINTCLOUD_MSG_RECEIVER_C : public POINTCLOUD_MSG_RECEIVER_C
{
public:
    MAP_POINTCLOUD_MSG_RECEIVER_C(ros::NodeHandle &nh)
        : POINTCLOUD_MSG_RECEIVER_C(nh, TOPIC_NAME_MAP_POINTS)
    {
    };

private:
};

class MAP_POINTCLOUD_MSG_SENDER_C : public POINTCLOUD_MSG_SENDER_C
{
public:
    MAP_POINTCLOUD_MSG_SENDER_C(ros::NodeHandle &nh):POINTCLOUD_MSG_SENDER_C(nh, TOPIC_NAME_MAP_POINTS, true)
    {

    };
private:
};

class ALIGNED_POINTCLOUD_MSG_RECEIVER_C : public POINTCLOUD_MSG_RECEIVER_C
{
public:
    ALIGNED_POINTCLOUD_MSG_RECEIVER_C(ros::NodeHandle &nh)
        : POINTCLOUD_MSG_RECEIVER_C(nh, TOPIC_NAME_ALIGNED_POINTS)
    {
    };
private:
};

class ALIGNED_POINTCLOUD_MSG_SENDER_C : public POINTCLOUD_MSG_SENDER_C
{
public:
    ALIGNED_POINTCLOUD_MSG_SENDER_C(ros::NodeHandle &nh)
        : POINTCLOUD_MSG_SENDER_C(nh, TOPIC_NAME_ALIGNED_POINTS, false)
    {
    };
private:
};

class LOC_POINTCLOUD_MSG_RECEIVER_C
{
public:
    LOC_POINTCLOUD_MSG_RECEIVER_C(ros::NodeHandle &nh)
    {
        receiver_ = nh.subscribe(TOPIC_NAME_LOC_POINTS, 2, &LOC_POINTCLOUD_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        enPointType_ = MY_POINT_TYPE_UNKNOW;
        objInvalidTimer_.SetOverTime(0.2);
    }

    INT32 Get(CLOUD_XYZIRT_DATA_STRU &stCloud);

private:
    ros::Subscriber         receiver_;
    TIMER_C                 objInvalidTimer_;
    CLOUD_XYZIRT_DATA_STRU  stCloud_;
    POINT_TYPE_ENUM         enPointType_;

    void ReceiveMsgCallBack(const sensor_msgs::PointCloud2::ConstPtr &pstMsg);
};

class SCE_MAP_MSG_SENDER_C
{
public:
    SCE_MAP_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = "/scene_map", bool latch = true)
    {
        publisher_ = nh.advertise<nav_msgs::OccupancyGrid>(pcTopicName, 1, latch);
    };
    void Send(COORDINATE_2D_STRU &stOriginPos, float resolution, UINT32 sizeX, UINT32 sizeY, UINT08 *pucMap);

private:
    ros::Publisher  publisher_;
};

class PATH_3D_MSG_SENDER_C
{
public:
    PATH_3D_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = "/path", bool latch = false)
    {
        publisher_ = nh.advertise<nav_msgs::Path>(pcTopicName, 1, latch);
    };
    void Send(const std::vector<Matrix4f> &vmPose);
    void Send(const std::deque<COORDINATE_3D_STRU> &vstPos);
    void Send(const std::deque<COORDINATE_3D_STRU> &vstPos, const double &time);

private:
    void ConvPos2PathMsg(const double &time, const std::deque<COORDINATE_3D_STRU> &vstPos, nav_msgs::Path &stPathMsg);

private:
    ros::Publisher                  publisher_;

};

class MOVE_INFO_MSG_RECEIVER_C
{
public:
    MOVE_INFO_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/move_info")
    {
        receiver_ = nh.subscribe(pcTopicName, 200, &MOVE_INFO_MSG_RECEIVER_C::ReceiveMsgCallBack, this, ros::TransportHints().tcpNoDelay());
    };
    INT32 Get(std::deque<MOVE_INFO_DATA_STRU>  &vstMoveInfo);

private:
    void ReceiveMsgCallBack(const comm_msg::move_info::ConstPtr& pMsg);

private:
    ros::Subscriber      receiver_;

    std::deque<MOVE_INFO_DATA_STRU>  vstMoveInfo_;
};

class ODOM_3D_MSG_RECEIVER_C
{
public:
    ODOM_3D_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/odom_3d", const size_t buffSize = 1000)
         : buffSize_(buffSize)
    {
        receiver_ = nh.subscribe(pcTopicName, 1000, &ODOM_3D_MSG_RECEIVER_C::ReceiveMsgCallBack, this, ros::TransportHints().tcpNoDelay());
    };
    void  Get(deque<ODOM_DATA_STRU>  &vOdomData);
    INT32 Get(ODOM_DATA_STRU  &stOdomData);

private:
    void ReceiveMsgCallBack(const nav_msgs::Odometry::ConstPtr& pMsg);

private:
    ros::Subscriber        receiver_;
    size_t                 buffSize_;
    deque<ODOM_DATA_STRU>  vOdomData_;
};

class ODOM_3D_MSG_SENDER_C
{
public:
    ODOM_3D_MSG_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = "/odom_3d",
                         const std::string strParentsFrame = "odom",
                         const std::string strChildFrame   = "base_link")
    {
        publisher_ = nh.advertise<nav_msgs::Odometry>(pcTopicName, 1000);
        strParentsFrame_ = strParentsFrame;
        strChildFrame_   = strChildFrame;
    };
    void Send(const Matrix4f &mPose);
    void Send(const Matrix4f &mPose,  const double &time);
    void Send(ODOM_DATA_STRU stOdomData);

private:
    ros::Publisher  publisher_;
    std::string     strParentsFrame_;
    std::string     strChildFrame_;
};

/**
 * @brief imu数据接收器
 *
 */
class IMU_MSG_RECEIVER_C
{
public:
    IMU_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/yis_imu", const size_t buffSize = 1000)
        : buffSize_(buffSize)
    {
        receiver_ = nh.subscribe(pcTopicName, 1000, &IMU_MSG_RECEIVER_C::ReceiveMsgCallBack, this, ros::TransportHints().tcpNoDelay());
    };
    INT32 Get(deque<IMU_DATA_STRU>  &vImuData);

private:
    void ReceiveMsgCallBack(const sensor_msgs::ImuConstPtr& pMsg);

private:
    ros::Subscriber        receiver_;
    deque<IMU_DATA_STRU>   vImuData_;  // imu数据缓存队列
    size_t                 buffSize_;  // 缓存队列尺寸
};

class IMU_MSG_SENDER_C
{
public:
    IMU_MSG_SENDER_C(ros::NodeHandle &nh, string pcTopicName = string("/yis_imu"))
    {
        publisher_ = nh.advertise<sensor_msgs::Imu>(pcTopicName, 1000);
    };
    void Send(const IMU_DATA_STRU &stImuData);

private:
    ros::Publisher  publisher_;
};

class LOCATION_3D_MSG_RECEIVER_C
{
public:
    LOCATION_3D_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = TOPIC_NAME_3D_LOCATION_INFO_SAIJIE, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(pcTopicName, 1, &LOCATION_3D_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        enState_ = MAP_NODE_STATE_IDLE;
    };

    BOOL  IsNormal(void);
    BOOL  IsNormalNew(void);
    BOOL  IsChangingMap(void);
    BOOL  IsCreatingMap(void);

    void  Get(TIMESTAMP_POSE_STRU &stTsPose);
    INT32 GetNormal(TIMESTAMP_POSE_STRU &stTsPose);
    TIMESTAMP_POSE_STRU *GetNormal(void);
    INT32 GetSubMapNo(void);
    void  ReceiveMsgCallBack(const comm_msg::locationInfo3D::ConstPtr& pMsg);

private:
    TIMER_C              objInvalidTimer_;
    ros::Subscriber      receiver_;
    MAP_NODE_STATE_ENUM  enState_;
    INT32                subMapNo_;
    TIMESTAMP_POSE_STRU  stTsPose_;
    ros::Time            lastT_;
};

class LOCATION_3D_MSG_SENDER_C
{
public:
    LOCATION_3D_MSG_SENDER_C(ros::NodeHandle &nh, string pcTopicName = string(TOPIC_NAME_3D_LOCATION_INFO_SAIJIE))
    {
        publisher_ = nh.advertise<const comm_msg::locationInfo3D>(pcTopicName, 1);
    };
    void Send(MAP_NODE_STATE_ENUM enState, int subMapNo, const COOR_3D_POSE_STRU &stPose);

private:
    ros::Publisher  publisher_;
};

class POSE_SCORE_RECEIVER_C
{
public:
    POSE_SCORE_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/match_result")
    {
        objInvalidTimer_.SetOverTime(0.1);
        receiver_ = nh.subscribe(pcTopicName, 1, &POSE_SCORE_RECEIVER_C::ReceiveMsgCallBack, this);
    };
    void  ReceiveMsgCallBack(const comm_msg::pose_score::ConstPtr& pstMsg);
    INT32 Get(POSE_SCORE_STRU &stPoseScore);

private:
    TIMER_C            objInvalidTimer_;
    ros::Subscriber    receiver_;
    POSE_SCORE_STRU    stPoseScore_;
};

class POSE2D_SENDER_C
{
public:
    POSE2D_SENDER_C(ros::NodeHandle &nh, string pcTopicName = string("/pose2D"))
    {
         publisher_ = nh.advertise<const comm_msg::pose2D>(pcTopicName, 1);
    }
    void Send(const COOR_2D_POSE_STRU &stPose);

private:
    ros::Publisher  publisher_;
};

class POSE_SCORE_SENDER_C
{
public:
    POSE_SCORE_SENDER_C(ros::NodeHandle &nh, string pcTopicName = string("/match_result"))
    {
         publisher_ = nh.advertise<const comm_msg::pose_score>(pcTopicName, 1);
    }
    void Send(const float score, const Matrix4f &mPose);

private:
    ros::Publisher  publisher_;
};

class POLYGON_SENDER_C
{
public:
    POLYGON_SENDER_C(ros::NodeHandle &nh, string pcTopicName = string("/polygon"))
    {
         publisher_ = nh.advertise<const geometry_msgs::PolygonStamped>(pcTopicName, 1);
    }
    void Send(std::vector<Vector2f> &vertexes);

private:
    ros::Publisher  publisher_;
};

class QRCODE_POSE_RECEIVER_C
{
public:
    QRCODE_POSE_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/qrcode_pose")
    {
        objInvalidTimer_.SetOverTime(0.2);
        receiver_ = nh.subscribe(pcTopicName, 1, &QRCODE_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    };
    void  ReceiveMsgCallBack(const comm_msg::qrcode_pose::ConstPtr& pstMsg);
    INT32 Get(POSE_ID_STRU &stPoseId);

private:
    TIMER_C            objInvalidTimer_;
    ros::Subscriber    receiver_;
    POSE_ID_STRU       stPoseId_;
};

class ROP_MSG_RECEIVER_C
{
public:
    ROP_MSG_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = "/rate_of_progress")
    {
        receiver_ = nh.subscribe(pcTopicName, 1, &ROP_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
        isExist_  = false;
    };
    INT32 Get(float &rop);

private:
    ros::Subscriber     receiver_;
    bool                isExist_;
    float               rop_;

private:
    void ReceiveMsgCallBack(const std_msgs::Float32::ConstPtr &pstMsg);
};

class TF_SENDER_3D_C
{
public:
    void SendTfMap2Base(const  Matrix4f &mPose);
    void SendTfOdom2Base(const Matrix4f &mPose);
    void SendTfMap2Odom(const  Matrix4f &mPose);
    void SendTf(const std::string &strFatherFrameId, const std::string &strChildFrameId,  const Matrix4f &mPose);

private:
    tf::TransformBroadcaster broadcaster_;
};

#endif
