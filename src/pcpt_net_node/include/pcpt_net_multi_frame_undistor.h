#ifndef __MOTION_DISTORTION__
#define __MOTION_DISTORTION__

#pragma once
#include <ros/ros.h>

#include <std_msgs/Header.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <mutex>

#include <Eigen/Dense>
#include <deque>
#include <Eigen/Dense>
#include <pcpt_net_multi_frame_process.h>

using namespace std;

typedef struct LIDAR_IMU_PARAM_STRU {

    int     channel;        //线束
    int     pointsNum;      //点数
    string  sensorNameRs;   //"/HeadRightRsBpLidar/car_filtered_points";
    vector<double> imu2LidarRot;
    vector<double> imu2LidarRPY;

} LIDAR_IMU_PARAM_STRU;

typedef struct LIDAR_TYPE_STRU {

    float   lidarMinRange;
    float   lidarMaxRange;
    string  imuTopic;
    string  lidarFrame;
    int     queueLengt;

    LIDAR_TYPE_STRU(void) {
         lidarMinRange         = 1;
         lidarMaxRange         = 1000;
         imuTopic              = "/yis_imu";
         lidarFrame            = "base_link";
         queueLengt            = 2000;
    }

} LIDAR_TYPE_STRU;


/**************************************************************************************
功能描述:运动去畸变
修改记录:
**************************************************************************************/
class MULTI_FRAME_UNDISTORTION_C
{
public:
    MULTI_FRAME_UNDISTORTION_C(ros::NodeHandle &nh,LIDAR_IMU_PARAM_STRU& lidarImuParam);
    ~MULTI_FRAME_UNDISTORTION_C();

    void Init();
    void ResetParameters();
    sensor_msgs::Imu ImuConverter(const sensor_msgs::Imu& imuIn);
    void ImuHandler(const sensor_msgs::Imu::ConstPtr& imuMsg);
    void CloudHandler(const sensor_msgs::PointCloud2ConstPtr& msg);
    bool ImuDeskewInfo();
    BpPointXYZIRT DeskewPoint(BpPointXYZIRT *point, double relTime);
    void MotionUnDistortion(BpCloudXYZIRT::Ptr pointCloudIn);

private:

    ros::NodeHandle                         nh_;
    std_msgs::Header                        cloudHeader_;

    std::mutex                              imuLock_;
    ros::Subscriber                         subImu_;
    ros::Subscriber                         subLidarCloud_;

    ros::Publisher                          pubCloudUndistortion_;
    ros::Publisher                          pubCloudLidar_;

    LIDAR_IMU_PARAM_STRU                    lidarImuParam_;
    LIDAR_TYPE_STRU                         stLidarType_;

    double                                  timeScanCur_;
    double                                  timeScanEnd_;
    bool                                    imuAvailable_;

    int                                     imuCur_;
    bool                                    firstPointFlag_;

    double                                  *imuTime_ = new double[stLidarType_.queueLengt];
    double                                  *imuRotX_ = new double[stLidarType_.queueLengt];
    double                                  *imuRotY_ = new double[stLidarType_.queueLengt];
    double                                  *imuRotZ_ = new double[stLidarType_.queueLengt];
    std::deque<sensor_msgs::Imu>            imuQueue_;
    std::deque<sensor_msgs::PointCloud2>    cloudQueue_;

    //Imu->lidar
    Eigen::Matrix3d                         imu2LidarRot_;
    Eigen::Matrix3d                         imu2LidarRPY_;
    Eigen::Quaterniond                      imu2LidarQRPY_;

    Eigen::Affine3f                         transStartInverse_ = Eigen::Affine3f::Identity();
};

#endif //
