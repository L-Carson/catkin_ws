#ifndef __PCPT_COMM_ROS_H__
#define __PCPT_COMM_ROS_H__

#include <ros/ros.h>
#include "comm_msg/boxArray.h"
#include "comm_msg/coor2DArray.h"
#include "comm_msg/coor2DArrayLA.h"
#include "comm_msg/detect_result.h"
#include "comm_msg/movTargetsArray.h"
#include "comm_msg/radarTargetArray.h"
#include "comm_msg/roadmark.h"
#include "comm_msg/visionDepthCamera.h"
#include "comm_msg/visionFuncs.h"
#include "comm_msg/visionRects.h"
#include "comm_msg/visionAreas.h"
#include "comm/comm_msg.h"
#include "comm/comm_file.h"
#include "comm/comm_dbg_sw.h"
#include "comm/comm_ros_base.h"
#include "comm/comm_ros_trc.h"
#include "comm/comm_topic_name.h"
#include "pcpt_comm/pcpt_comm_car.h"
#include "pcpt_comm/pcpt_comm_log.h"
#include "pcpt_comm/pcpt_comm_msg.h"
#include "pcpt_comm/pcpt_comm_points.h"

#include "std_msgs/Header.h"
#include <std_msgs/String.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointCloud.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

#include <boost/thread.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>

// 屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include <sensor_msgs/point_cloud_conversion.h>
#pragma GCC diagnostic pop

using namespace std;

/**************************************************************************************
功能描述: 消息时效性
修改记录:
**************************************************************************************/
class MSG_TIMELINESS_C
{
public:
    MSG_TIMELINESS_C(string strTopicName, float dataInvalidTime, BOOL isDefaultTimeOut = false)
    : objDataInvalidTimer_(dataInvalidTime, isDefaultTimeOut)
    , strTopicName_(strTopicName)
    {}

    ~MSG_TIMELINESS_C(){}

    bool IsAbnormal() {
        if (g_isRestartedPnp) TimerReset();
        bool isDataInvalid = objDataInvalidTimer_.IsTimeOut();
        objAbnormalPrint_.DataInvalidPrint(isDataInvalid, PCPT_HZ_MAIN_FUN * 60, strTopicName_);
        if (isDataInvalid) return true;
        else               return false;
    }

    void TimerReset() {
        objDataInvalidTimer_.Reset();
    }

private:
    TIMER_C             objDataInvalidTimer_;
    ABNORMAL_PRINT_C    objAbnormalPrint_;
    std::string         strTopicName_;
};

/**************************************************************************************
功能描述: 点云消息格式相互转化
修改记录:
**************************************************************************************/
class POINT_CLOUD_MSG_TRANSFORM_C
{
public:
    POINT_CLOUD_MSG_TRANSFORM_C(void){}
    ~POINT_CLOUD_MSG_TRANSFORM_C(void){}

    void MsgRosToMsgPcl(const sensor_msgs::PointCloud2 &inPut , pcl::PCLPointCloud2 &outPut);
    void MsgRosToMsgPcl(const sensor_msgs::PointCloud2 &inPut , pcl::PointCloud<PointXYZI> &outPut);
    void MsgPclToMgRos(const pcl::PCLPointCloud2 &inPut, sensor_msgs::PointCloud2 &outPut);
    void MsgPclToMsgPcl(const pcl::PCLPointCloud2 &inPut, pcl::PointCloud<PointXYZI> &outPut);
    void MsgPclToMsgRos(const pcl::PointCloud<PointXYZI> &inPut, sensor_msgs::PointCloud2 &outPut);
    void MsgPclToMsgPcl(const pcl::PointCloud<PointXYZI> &inPut, pcl::PCLPointCloud2 &outPut);
    void ConvertPointCloudToPointCloud2(const sensor_msgs::PointCloud &inPut, sensor_msgs::PointCloud2 &outPut);
    void ConvertPointCloud2ToPointCloud(const sensor_msgs::PointCloud2 &inPut, sensor_msgs::PointCloud &outPut);

    template<typename T> void AddHeaderToPCL(T &pCloud) const
    {
        std_msgs::Header header;
        header.stamp = RosTimeNow();
        header.frame_id = PCPT_FRAME_ID_CAR;
        pcl_conversions::toPCL(header, pCloud->header);
    }

    template<typename T> void AddHeaderFromPCL(T &pCloud) const
    {
        std_msgs::Header header = RosHeaderFromPcl(pCloud);
    }

private:
};

/**************************************************************************************
功能描述: 点云数据发布器
修改记录:
**************************************************************************************/
class POINT_CLOUD_MSG_SENDER_C
{
public:
    POINT_CLOUD_MSG_SENDER_C(ros::NodeHandle &nh, string strTopicName)
    {
        publisher_ = nh.advertise<sensor_msgs::PointCloud2>(strTopicName, 10);
    }

    template<typename T> void Send(T &pCloud, const char* coorType) const
    {
        if (!pCloud) return;
        if (0 == publisher_.getNumSubscribers()) return;

        sensor_msgs::PointCloud2 pointCloudMsg;
        pcl::toROSMsg(*pCloud, pointCloudMsg);
        pointCloudMsg.header.frame_id = coorType;
        if (0 == SIG(PclStamp(pCloud))) pointCloudMsg.header.stamp = RosTimeNow();
        publisher_.publish(pointCloudMsg);
    }

    template<typename T> void Send(T &pCloud, string coorType) const
    {
        Send(pCloud, coorType.c_str());
    }

    template<typename T> void Send(T &pCloud) const
    {
        if (pCloud->header.frame_id.empty()) Send(pCloud, PCPT_FRAME_ID_CAR);
        else                                 Send(pCloud, pCloud->header.frame_id);
    }

    void Send(sensor_msgs::PointCloud2 &cloud) const
    {
        if (publisher_.getNumSubscribers()) publisher_.publish(cloud);
    }

    uint32_t GetNumSubscribers() const
    {
        return publisher_.getNumSubscribers();
    }

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 点云1数据发布器
修改记录:
**************************************************************************************/
class POINT_CLOUD1_MSG_SENDER_C
{
private:
    ros::Publisher publisher_;

public:
    POINT_CLOUD1_MSG_SENDER_C(ros::NodeHandle &nh, string strTopicName)
    {
        publisher_ = nh.advertise<sensor_msgs::PointCloud>(strTopicName, 10);
    }

    /**************************************************************************************
    功能描述: 点云发布
    修改记录:
    **************************************************************************************/
    template <typename T>
    void Send(T &pCloud, const char *coorType,
              const ros::Time &dataStamp = RosTimeNow()) const
    {
        if (!pCloud) return;
        if (publisher_.getNumSubscribers())
        {
            sensor_msgs::PointCloud cloud_msg;
            cloud_msg.header.stamp = dataStamp;
            cloud_msg.header.frame_id = coorType;
            cloud_msg.points.resize(pCloud->points.size());
            int points_cnt = 0;
            for (auto one_point : pCloud->points)
            {
                geometry_msgs::Point32 &new_point = cloud_msg.points[points_cnt];
                new_point.x = one_point.x;
                new_point.y = one_point.y;
                new_point.z = one_point.z;
                points_cnt++;
            }
            publisher_.publish(cloud_msg);
        }
    }

    template <typename T>
    void Send(T &pCloud, string coorType,
              const ros::Time &dataStamp = RosTimeNow()) const
    {
        Send(pCloud, coorType.data(), dataStamp);
    }

    template <typename T>
    void Send(T &pCloud) const
    {
        if (pCloud->header.frame_id.empty())
            Send(pCloud, PCPT_FRAME_ID_CAR);
        else
            Send(pCloud, pCloud->header.frame_id, pCloud->header.stamp);
    }

    void Send(sensor_msgs::PointCloud &cloud) const
    {
        if (publisher_.getNumSubscribers())
        {
            if (cloud.header.frame_id.empty())
            {
                cloud.header.stamp = RosTimeNow();
                cloud.header.frame_id = PCPT_FRAME_ID_CAR;
            }
            publisher_.publish(cloud);
        }
    }

    void Send(sensor_msgs::PointCloud &cloud, const string &coorType,
              const ros::Time &dataStamp = RosTimeNow()) const
    {
        if (publisher_.getNumSubscribers())
        {
            cloud.header.stamp = dataStamp;
            cloud.header.frame_id = coorType;
            publisher_.publish(cloud);
        }
    }

    uint32_t GetNumSubscribers() const
    {
        return publisher_.getNumSubscribers();
    }
};

/**************************************************************************************
功能描述: 2D坐标数组发布器
修改记录:
**************************************************************************************/
class COOR_2D_ARRAY_MSG_SENDER_C
{
public:
    COOR_2D_ARRAY_MSG_SENDER_C(ros::NodeHandle &nh, string pcTopicName)
    {
        publisher_ = nh.advertise<comm_msg::coor2DArray>(pcTopicName, 1);
    }

    ~COOR_2D_ARRAY_MSG_SENDER_C(){}

    void Send(comm_msg::coor2DArray &coor2DArray) const;

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 带类别标签的2D坐标数组发布器
修改记录:
**************************************************************************************/
class COOR_2D_ARRAY_LA_MSG_SENDER_C
{
public:
    COOR_2D_ARRAY_LA_MSG_SENDER_C(ros::NodeHandle &nh, string pcTopicName = TOPIC_NAME_PCPT_OBST_WITH_LABEL)
    {
        publisher_ = nh.advertise<comm_msg::coor2DArrayLA>(pcTopicName, 1);
    }

    ~COOR_2D_ARRAY_LA_MSG_SENDER_C(){}

    void Send(comm_msg::coor2DArrayLA &coor2DArrayLA) const;

private:
    ros::Publisher  publisher_;
};

/**************************************************************************************
功能描述: 融合后的静态障碍发布器
修改记录: 融合静态障碍发布器
**************************************************************************************/
class FUSED_STATIC_OBST_SENDER_C
{
struct PUB_STAMP_STRU {
    PUB_STAMP_STRU(){}
    PUB_STAMP_STRU(const double obstStamp, const double pubStamp) {
        this->obstStamp = obstStamp;
        this->pubStamp  = pubStamp;
    }
    double DelayTime() const {return pubStamp - obstStamp;}

    double obstStamp;
    double pubStamp;
};

public:
    FUSED_STATIC_OBST_SENDER_C(ros::NodeHandle &nh)
    : objCoor2DArrayMsgSender_(nh, TOPIC_NAME_FUSED_STATIC_OBST)
    , objCoor2DArrayLAMsgSender_(nh, TOPIC_NAME_PCPT_OBST_WITH_LABEL)
    , objpCloudSenderByMap_(nh, TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_MAP)
    , objpCloudSenderByMapStatic_(nh, string(TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_MAP) + "_static")
    , objpCloudSenderByMapDynamic_(nh, string(TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_MAP) + "_dynamic")
    , objpCloudSenderByCar_(nh, TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_CAR)
    , objpCloudSenderByCarStatic_(nh, string(TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_CAR) + "_static")
    , objpCloudSenderByCarDynamic_(nh, string(TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_CAR) + "_dynamic")
    {}

    ~FUSED_STATIC_OBST_SENDER_C(){}

    int  Coor2Msg(const PCPT_OBST_STRU& stObstByMap, comm_msg::coor2DArray &coor2DArray);
    void Send(PCPT_OBST_STRU& stObstByMap);
    void Send(const vector<VISION_USAGE_STRU>& vstUsageByMap);

private:
    COOR_2D_ARRAY_MSG_SENDER_C      objCoor2DArrayMsgSender_;       // 2D障碍发布器（map坐标系）
    COOR_2D_ARRAY_LA_MSG_SENDER_C   objCoor2DArrayLAMsgSender_;     // 带类别标签的2D坐标数组发布器

    POINT_CLOUD_MSG_SENDER_C        objpCloudSenderByMap_;          // 2D障碍点云发布器（综合）（map坐标系）
    POINT_CLOUD_MSG_SENDER_C        objpCloudSenderByMapStatic_;    // 2D障碍点云发布器（静态）（map坐标系）
    POINT_CLOUD_MSG_SENDER_C        objpCloudSenderByMapDynamic_;   // 2D障碍点云发布器（动态）（map坐标系）

    POINT_CLOUD_MSG_SENDER_C        objpCloudSenderByCar_;          // 2D障碍点云发布器（综合）（car坐标系）
    POINT_CLOUD_MSG_SENDER_C        objpCloudSenderByCarStatic_;    // 2D障碍点云发布器（静态）（car坐标系）
    POINT_CLOUD_MSG_SENDER_C        objpCloudSenderByCarDynamic_;   // 2D障碍点云发布器（动态）（car坐标系）

    bool                            hasRisk_{false};                // 图像视野是否存在风险
    comm_msg::coor2DArray           coor2DArray_;
    vector<PUB_STAMP_STRU>          vstStamp_;

    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudByCar_{new PointCloudXYZ};        // 2D障碍点云（综合）（base坐标系）
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudByCarStatic_{new PointCloudXYZ};  // 2D障碍点云（静态）（base坐标系）
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudByCarDynamic_{new PointCloudXYZ}; // 2D障碍点云（动态）（base坐标系）

    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudByMap_{new PointCloudXYZ};        // 2D障碍点云（综合）（global坐标系）
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudByMapStatic_{new PointCloudXYZ};  // 2D障碍点云（静态）（global坐标系）
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudByMapDynamic_{new PointCloudXYZ}; // 2D障碍点云（动态）（global坐标系）

    void DelayStatistics(const double obstStamp);
};

/**************************************************************************************
功能描述: 2D Lidar消息接收器
修改记录:
**************************************************************************************/
class LIDAR_SCAN_RECEIVER_C
{
public:
    LIDAR_SCAN_RECEIVER_C(ros::NodeHandle &nh, string strTopicName)
    : cacheMutex_()
    , pScanCache_(new sensor_msgs::LaserScan)
    {
        receiver_ = nh.subscribe(strTopicName, 2, &LIDAR_SCAN_RECEIVER_C::MsgReceiveCallBack, this);
    }

    ~LIDAR_SCAN_RECEIVER_C() {}

    BOOL IsDataInvalid() const;
    BOOL IsSensorFault() const;
    int GetNormalData(sensor_msgs::LaserScan::Ptr &ptrScan);

private:
    ros::Subscriber receiver_;

    std::recursive_mutex   cacheMutex_;         // 递归锁
    sensor_msgs::LaserScan::Ptr pScanCache_;    // scan数据缓存
    BOOL hasNewData_{false};                    // 点云更新标识

    TIMER_C objDataInvalidTimer_{0.5, false};   //超时表示数据无效
    TIMER_C objSensorFaultTimer_{30.0, false};  //超时表示传感器故障

private:
    void MsgReceiveCallBack(const sensor_msgs::LaserScan::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 激光雷达消息接收器
修改记录:
**************************************************************************************/
class LIDAR_DATA_RECEIVER_C : public MSG_TIMELINESS_C
{
public:
    LIDAR_DATA_RECEIVER_C(ros::NodeHandle &nh, string strTopicName)
    : MSG_TIMELINESS_C(strTopicName, 0.3, false)
    , strTopicName_(strTopicName)
    {
        receiver_ = nh.subscribe(strTopicName, 2, &LIDAR_DATA_RECEIVER_C::MsgReceiveCallBack, this);
    }

    ~LIDAR_DATA_RECEIVER_C(){}

    BOOL IsDataInvalid();
    BOOL IsSensorFault() const;
    int  GetNormalPointsCloud(PointCloudXYZIRCDT::Ptr &ptrCloud);

private:
    TIMER_C objSensorFaultTimer_{30.0, false};  // 超时表示传感器故障

    double  abMsgTime_{0};                      // 异常消息的时间
    BOOL    hasNewData_{false};                 // 点云更新标识
    string  strTopicName_;

    ros::Subscriber             receiver_;
    sensor_msgs::PointCloud2    msg_;
    PointCloudXYZI::Ptr         pCloudXYZI_{new PointCloudXYZI};            // 4元点云缓存（旧型）
    PointCloudXYZIRCDT::Ptr     pCloudXYZIRCDT_{new PointCloudXYZIRCDT};    // 8元点云缓存（新型）

    void MsgReceiveCallBack(const sensor_msgs::PointCloud2ConstPtr &ptrMsg);
    bool IsTimelinessOk(const double msgTime);
};

/**************************************************************************************
功能描述: 接收毫米波雷达消息
修改记录: Radar数据接收器
**************************************************************************************/
class RADAR_DATA_RECEIVER_C
{
public:
    RADAR_DATA_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName)
    {
        receiver_ = nh.subscribe(pcTopicName, 1, &RADAR_DATA_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    ~RADAR_DATA_RECEIVER_C(){}

    BOOL IsDataInvalid() const;
    BOOL IsSensorFault() const;
    const RADAR_INFO_STRU *Get(void);

private:
    TIMER_C objDataInvalidTimer_{0.2, false};   //超时表示数据无效
    TIMER_C objSensorFaultTimer_{30.0, false};  //超时表示传感器故障

    ros::Subscriber receiver_;
    RADAR_INFO_STRU stRadarInfo_;

    void ReceiveMsgCallBack(const comm_msg::radarTargetArray::ConstPtr &ptrMsg);
    void TimerReset();
};

/**************************************************************************************
功能描述: 深度相机点云1接收器
修改记录:
**************************************************************************************/
class DEPTH_CAMERA_POINTS1_RECEIVER_C
{
public:
    DEPTH_CAMERA_POINTS1_RECEIVER_C(ros::NodeHandle &nh)
    {
        pointsSub_ = nh.subscribe(TOPIC_NAME_DCAMERA_FUSED_POINTS, 1, &DEPTH_CAMERA_POINTS1_RECEIVER_C::MsgReceiveCallBack, this);
    }

    ~DEPTH_CAMERA_POINTS1_RECEIVER_C(){}

    BOOL IsDataInvalid() const;
    BOOL IsSensorReset() const;
    BOOL IsSensorFault() const;
    void TimerReset();
    BOOL GetNormalPointsCloud(pcl::PointCloud<pcl::PointXYZI>::Ptr &ptrCloud);
    BOOL GetNormalPointsCloud(sensor_msgs::PointCloud &depthPoints);

private:
    TIMER_C                     objDataInvalidTimer_{0.3, false};   //超时表示数据无效
    TIMER_C                     objSensorResetTimer_{30.0, false};  //超时表示需要进行传感器复位
    TIMER_C                     objSensorFaultTimer_{180.0, false}; //超时表示传感器故障
    ros::Subscriber             pointsSub_;
    sensor_msgs::PointCloud2    objPoints2_;  // 点云2临时缓存
private:
    std::recursive_mutex                 cacheMutex_;   // 缓存数据信号量
    pcl::PointCloud<pcl::PointXYZI>::Ptr ptrCloud_{new pcl::PointCloud<pcl::PointXYZI>};   // 深度点云缓存
    sensor_msgs::PointCloud              objPoints_;    // 深度点云缓存

private:
    void MsgReceiveCallBack(const sensor_msgs::PointCloud::ConstPtr &ptrMsg);
};

/**************************************************************************************
功能描述: 深度相机消息接收器
修改记录:
**************************************************************************************/
class DEPTH_CAMERA_MSG_RECEIVER_C
{
public:
    DEPTH_CAMERA_MSG_RECEIVER_C(ros::NodeHandle &nh) : nh_(nh)
    {
        pointsSub_ = nh.subscribe(TOPIC_NAME_VISION_PCLOUD_SRC, 1, &DEPTH_CAMERA_MSG_RECEIVER_C::MsgReceiveCallBack, this);
    }

    ~DEPTH_CAMERA_MSG_RECEIVER_C(){}

    BOOL IsDataInvalid() const;
    BOOL IsSensorFault() const;
    void TimerReset();
    void GetNormalPointsCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &ptrCloud) const;

private:
    ros::NodeHandle &nh_;
    TIMER_C         objDataInvalidTimer_{0.3, false};   //超时表示数据无效
    TIMER_C         objSensorFaultTimer_{30.0, false};  //超时表示传感器故障

    ros::Subscriber                     pointsSub_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr ptrCloud_{new pcl::PointCloud<pcl::PointXYZ>};   //深度点云缓存

    void MsgReceiveCallBack(const sensor_msgs::PointCloud2ConstPtr &ptrMsg);
    void GetPointsCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &ptrCloud) const;
};

/**************************************************************************************
功能描述: 选择深度相机接收器
修改记录: 深度相机可用通道接收器
**************************************************************************************/
class SELECT_DEPTH_CAMERA_MSG_RECEIVER_C
{
public:
    SELECT_DEPTH_CAMERA_MSG_RECEIVER_C(ros::NodeHandle &nh)
    {
        receiver_ = nh.subscribe(TOPIC_NAME_VISION_SELECT_DEPTH_CAMERA, 1, &SELECT_DEPTH_CAMERA_MSG_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    ~SELECT_DEPTH_CAMERA_MSG_RECEIVER_C(){}

    void Get(D_CAMERA_ENUM &enSelectDepthCamera);

private:
    ros::Subscriber     receiver_;
    D_CAMERA_ENUM       enSelectDepthCamera_{D_CAMERA_ENUM::NO_D_CAMERA};  // 当前选择的相机通道

    void ReceiveMsgCallBack(const comm_msg::visionDepthCamera::ConstPtr &ptrMsg);
};

/**************************************************************************************
功能描述: 相机点云发送器
修改记录:
**************************************************************************************/
class CAMERA_PCD_SENDER_C
{
public:
    CAMERA_PCD_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName)
    {
        publisher_ = nh.advertise<sensor_msgs::PointCloud2>(pcTopicName, 1);
    }

    ~CAMERA_PCD_SENDER_C(){}

    void Send(pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud);

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 视觉功能发布器
修改记录:
**************************************************************************************/
class VISION_FUNC_SENDER_C
{
public:
    VISION_FUNC_SENDER_C(ros::NodeHandle& nh)
    {
        publisher_ = nh.advertise<std_msgs::String>(TOPIC_NAME_VISION_FUNC, 1);
    }

    ~VISION_FUNC_SENDER_C(){}

    void Send(const std::string &strVisionFunc);

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 视觉功能接受器
修改记录:
**************************************************************************************/
class VISION_FUNC_RECEIVER_C
{
public:
    VISION_FUNC_RECEIVER_C(ros::NodeHandle& nh)
    {
        receiver_ = nh.subscribe(TOPIC_NAME_VISION_FUNC, 1, &VISION_FUNC_RECEIVER_C::MsgReceiveCallback, this);
    }

    ~VISION_FUNC_RECEIVER_C(){}

    void Get(std::string &strVisionFunc);

private:
    ros::Subscriber    receiver_;
    std_msgs::String   msg_;      // 视觉任务信息

    void MsgReceiveCallback(const std_msgs::String &msg);
};

/**************************************************************************************
功能描述: 基于图像检测的数据发布器
修改记录:
**************************************************************************************/
class VISION_DETECT_SENDER_C
{
public:
    VISION_DETECT_SENDER_C(ros::NodeHandle &nh, string strTopicName)
    {
        objPublisher_ = nh.advertise<sensor_msgs::PointCloud>(strTopicName, 10);
    }

    void Send(double detectTime, const std::vector<COORDINATE_2D_STRU> &vstPos);

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 基于图像检测的坐标数据接收器
修改记录:
**************************************************************************************/
class VISION_COOR_RECEIVER_C : public MSG_TIMELINESS_C
{
public:
    VISION_COOR_RECEIVER_C(ros::NodeHandle& nh, string strTopicName, float dataInvalidTime)
    : MSG_TIMELINESS_C(strTopicName, dataInvalidTime, false)
    , strTopicName_(strTopicName)
    {
        receiver_ = nh.subscribe(strTopicName_, 10, &VISION_COOR_RECEIVER_C::MsgReceiveCallback, this);
    }

    ~VISION_COOR_RECEIVER_C(){}

    const VISION_OBJ_STRU<V_COOR_STRU>* Get();

private:
    ros::Subscriber                 receiver_;
    std::string                     strTopicName_;
    bool                            hasNewData_{false};
    VISION_OBJ_STRU<V_COOR_STRU>    stVisionObj_;

    void MsgReceiveCallback(const sensor_msgs::PointCloud::ConstPtr &ptrMsg);
};

/**************************************************************************************
功能描述: 基于图像检测的像素区域数据接收器
修改记录:
**************************************************************************************/
class VISION_AREA_RECEIVER_C : public MSG_TIMELINESS_C
{
public:
    VISION_AREA_RECEIVER_C(ros::NodeHandle& nh, string strTopicName, float dataInvalidTime)
    : MSG_TIMELINESS_C(strTopicName, dataInvalidTime, false)
    , strTopicName_(strTopicName)
    {
        receiver_ = nh.subscribe(strTopicName_, 10, &VISION_AREA_RECEIVER_C::MsgReceiveCallback, this);
    }

    ~VISION_AREA_RECEIVER_C(){}

    const VISION_OBJ_STRU<V_AREA_STRU>* Get();

private:
    ros::Subscriber                 receiver_;
    std::string                     strTopicName_;
    bool                            hasNewData_{false};
    double                          abMsgTime_{0};  // 异常消息的时间
    VISION_OBJ_STRU<V_AREA_STRU>    stVisionObj_;

    void MsgReceiveCallback(const comm_msg::visionAreas::ConstPtr &ptrMsg);
    bool IsTimelinessOk(const double msgTime);
};

/**************************************************************************************
功能描述: 基于图像检测的框数据接收器
修改记录:
**************************************************************************************/
class VISION_RECTANGLE_RECEIVER_C : public MSG_TIMELINESS_C
{
public:
    VISION_RECTANGLE_RECEIVER_C(ros::NodeHandle& nh, string strTopicName, float dataInvalidTime)
    : MSG_TIMELINESS_C(strTopicName, dataInvalidTime, false)
    , strTopicName_(strTopicName)
    {
        receiver_ = nh.subscribe(strTopicName_, 10, &VISION_RECTANGLE_RECEIVER_C::MsgReceiveCallback, this);
    }

    ~VISION_RECTANGLE_RECEIVER_C(){}

    const VISION_OBJ_STRU<V_RECT_STRU>* Get();

private:
    ros::Subscriber                 receiver_;
    std::string                     strTopicName_;
    bool                            hasNewData_{false};
    double                          abMsgTime_{0};  // 异常消息的时间
    VISION_OBJ_STRU<V_RECT_STRU>    stVisionObj_;

    void MsgReceiveCallback(const comm_msg::visionRects::ConstPtr &ptrMsg);
    bool IsTimelinessOk(const double msgTime);
};

/**************************************************************************************
功能描述: 基于点云的网络检测消息接收
修改记录:
**************************************************************************************/
class PCD_NET_MSG_RECEIVER_C : public MSG_TIMELINESS_C
{
public:
    PCD_NET_MSG_RECEIVER_C(ros::NodeHandle& nh)
    : MSG_TIMELINESS_C(TOPIC_NAME_MARKER_PCD_NET_BOX, 0.5)
    {
        receiver_ = nh.subscribe(TOPIC_NAME_MARKER_PCD_NET_BOX, 1, &PCD_NET_MSG_RECEIVER_C::MsgReceiverCallback, this);
    }

    ~PCD_NET_MSG_RECEIVER_C(){}

    const vector<BOX_INFO_STRU> *Get();

private:
    ros::Subscriber         receiver_;
    vector<BOX_INFO_STRU>   vstBoxes_;  // Box有序集
    uint32_t                id_{0};     // Box ID计数值

    void MsgReceiverCallback(const comm_msg::boxArray::ConstPtr& ptrMsg);
};

/**************************************************************************************
功能描述: image msg sender
修改记录: 图像消息发送器
**************************************************************************************/
class IMAGE_DATA_SENDER_C
{
public:
    IMAGE_DATA_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName) : it_(nh)
    {
        publisher_ = it_.advertise(pcTopicName, 1);
    }

    ~IMAGE_DATA_SENDER_C(){}

    void Send(cv_bridge::CvImagePtr cvPtr);
    void Send(cv::Mat &cvImage);
    bool HasSubscriber();

private:
    image_transport::ImageTransport it_;
    image_transport::Publisher      publisher_;
};

/**************************************************************************************
功能描述: image msg receiver
修改记录: 图像消息接收器
**************************************************************************************/
class IMAGE_DATA_RCVER_C : public MSG_TIMELINESS_C
{
public:
    IMAGE_DATA_RCVER_C(ros::NodeHandle &nh, const char *pcTopicName, bool useCompressed = false)
    : MSG_TIMELINESS_C(pcTopicName, 2.0)
    , nh_(nh), it_(nh)
    {
#if 1
        image_transport::TransportHints hints(useCompressed ? "compressed" : "raw");
        imageReceiver_ = it_.subscribe(pcTopicName, 1, &IMAGE_DATA_RCVER_C::MsgReceiverCallback, this, hints);
#else
        if (useCompressed)
            imageReceiver_ = it_.subscribe(pcTopicName, 1, &IMAGE_DATA_RCVER_C::MsgReceiverCallback, this, image_transport::TransportHints("compressed"));
        else
            imageReceiver_ = it_.subscribe(pcTopicName, 1, &IMAGE_DATA_RCVER_C::MsgReceiverCallback, this);
#endif
    }

    int Get(cv_bridge::CvImagePtr& cvPtr);

private:
    ros::NodeHandle&                nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber     imageReceiver_;

    std::atomic_bool                hasNewData_{false}; // 数据更新标识
    cv_bridge::CvImagePtr           cvPtr_;             // 图像数据指针
    std::mutex                      mutex_;             // 线程锁
    //sensor_msgs::CompressedImage    compressedImage_; // 压缩的图像消息

    void MsgReceiverCallback(const sensor_msgs::ImageConstPtr& ptrMsg);
};

/**
 * @brief 路标轮廓发布器
 *
 */
class ROADMARK_SENDER_C
{
public:
    ROADMARK_SENDER_C(ros::NodeHandle& nh, const string strTopicName) : nh_(nh)
    {
        pubMsg_ = nh.advertise<comm_msg::roadmark>(strTopicName + "_msg", 1);
        pubMarker_ = nh.advertise<visualization_msgs::MarkerArray>(strTopicName + "_marker", 1);
    }

    ~ROADMARK_SENDER_C(){}

    void Send(const vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double timeStamp);

private:
    ros::NodeHandle&    nh_;
    ros::Publisher      pubMsg_;
    ros::Publisher      pubMarker_;
    uint32_t            preMarkerSize_{0};

    void SendMsg(const vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double timeStamp);
    void SendMarker(const vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double timeStamp);
};

/**
 * @brief 路标轮廓接收器
 *
 */
class ROADMARK_RECEIVER_C
{
public:
    ROADMARK_RECEIVER_C(ros::NodeHandle& nh, const string strTopicName)
    {
        subMsg_ = nh.subscribe(strTopicName + "_msg", 1, &ROADMARK_RECEIVER_C::MsgReceiverCallback, this);
        //subMarker_ = nh.subscribe(strTopicName + "_marker", 1, &ROADMARK_RECEIVER_C::MarkerReceiverCallback, this);
    }

    ~ROADMARK_RECEIVER_C(){}

    int Get(vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double& timeStamp);

private:
    ros::Subscriber subMsg_;
    ros::Subscriber subMarker_;
    double          timeStamp_;                       // 数据时间戳
    vector<vector<COORDINATE_3D_STRU>> vvstCoor3d_;   // 路标轮廓集

    void MsgReceiverCallback(const comm_msg::roadmarkPtr& ptrMsg);
    void MarkerReceiverCallback(const visualization_msgs::MarkerArrayPtr& ptrMsg);
};

/**************************************************************************************
功能描述: 检测请求响应结果发布器
修改记录:
**************************************************************************************/
class DETECT_RESULT_SENDER_C
{
public:
    DETECT_RESULT_SENDER_C(ros::NodeHandle& nh, const string strTopicName)
    {
        publisher_ = nh.advertise<comm_msg::detect_result>(strTopicName, 1);
    }

    ~DETECT_RESULT_SENDER_C(){}

    void Send(const int result);

private:
    ros::Publisher publisher_;
};

#endif

