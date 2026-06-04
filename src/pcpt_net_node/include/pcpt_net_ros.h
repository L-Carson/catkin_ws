#ifndef __PCPT_ROS_H__
#define __PCPT_ROS_H__
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseStamped.h>
#include <pcl_conversions/pcl_conversions.h>
using namespace std;

enum PCD_LABEL_ENUM {
    EN_CAR        = 0,
    EN_BICYCLE    = 1,
    EN_PEDESTRIAN = 2,
};

typedef struct lidarDataType {
  uint8_t label;
  float   x;
  float   y;
  float   z;
  float   width;    //x
  float   length;   //y
  float   height;   //z
  float   angle;
  float   score;
} lidarDataType;

struct PointXYZIRT {
    PCL_ADD_POINT4D;
    uint8_t  intensity;
    uint16_t ring      = 0;
    uint16_t col       = 0;
    float    dis       = 0;
    double   timestamp = 0;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZIRT,
                                  (float, x, x)
                                  (float, y, y)
                                  (float, z, z)
                                  (unsigned char,  intensity, intensity)
                                  (unsigned short, ring, ring)
                                  (unsigned short, col, col)
                                  (float, dis, dis)
                                  (double, timestamp, timestamp))

typedef pcl::PointCloud<pcl::PointXYZI> pclCloud;
typedef pcl::PointCloud<PointXYZIRT> CloudXYZIRT;

/**************************************************************************************
功能描述: PCPT_ROS_C
修改记录:
**************************************************************************************/
class PCPT_ROS_C
{
public:
    PCPT_ROS_C(ros::NodeHandle& nh, const float lidarHeight) : lidarHeight_(lidarHeight)
    {
        pubDetectBoxMarkers_  = nh.advertise<visualization_msgs::MarkerArray>("/detect_box_markers", 1,true);
        pubDetectTextMarkers_ = nh.advertise<visualization_msgs::MarkerArray>("/detect_text_markers", 1,true);
        pointCloudPublisher_  = nh.advertise<sensor_msgs::PointCloud2>("pointCloud_16", 1, true);
    }

    ~PCPT_ROS_C(void){}

    void Init();
    void PublishPoints(sensor_msgs::PointCloud2::Ptr pointsMessage, ros::Time rosTime);
    void ShowNetDetectMsgs(vector<lidarDataType> resultOut, ros::Time rosTime);

private:
    ros::Publisher                  pubDetectBoxMarkers_;
    ros::Publisher                  pubDetectTextMarkers_;
    ros::Publisher                  pointCloudPublisher_;
    float                           lidarHeight_;
    visualization_msgs::MarkerArray makerArrayBoxPre;
    visualization_msgs::MarkerArray makerArrayTextPre;
    std_msgs::ColorRGBA             carColor;           //浅蓝色
    std_msgs::ColorRGBA             biCycleColor_;      //紫色
    std_msgs::ColorRGBA             pedestrialColor_;   //绿色
    std_msgs::ColorRGBA             textColor_;         //白色

    void InitColor();
};

#endif
