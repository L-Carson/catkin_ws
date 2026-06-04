#ifndef __MULTI_FRAME_PROCESS_H__
#define __MULTI_FRAME_PROCESS_H__

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/impl/point_types.hpp>
#include <nav_msgs/Odometry.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>

#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>

#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>

#define MAT_FROM_ARRAY(v) v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8]

struct BpPointXYZIRT
{
   PCL_ADD_POINT4D;
   uint8_t  intensity;
   uint16_t ring      = 0;
   uint16_t col       = 0;
   float    dis       = 0;
   double   timestamp = 0;
   EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT(BpPointXYZIRT,
                                  (float, x, x)
                                  (float, y, y)
                                  (float, z, z)
                                  (unsigned char,  intensity, intensity)
                                  (unsigned short, ring, ring)
                                  (unsigned short, col, col)
                                  (float, dis, dis)
                                  (double, timestamp, timestamp))

typedef pcl::PointCloud<BpPointXYZIRT>  BpCloudXYZIRT;
typedef pcl::PointXYZI PointType;
typedef pcl::PointCloud<PointType> CloudType;
typedef struct MULTI_FRAME_PARAM_STRU
{
    std::string saveBinDir;
    std::string savePcdDir;
    bool isSaveMulti;
    int  startFrameId;
    bool isLidarNewDrive;
    std::string lidarName;
    std::string pcptMap;

} MULTI_FRAME_PARAM_STRU;
/**************************************************************************************
功能描述:MULTI_FRAME_PROCESS_C
修改记录:
**************************************************************************************/
class MULTI_FRAME_PROCESS_C
{
public:
    MULTI_FRAME_PROCESS_C(ros::NodeHandle& nh,MULTI_FRAME_PARAM_STRU &multiFrameParam)
    : nh_(nh)
    , objMultiFrameParam_(multiFrameParam)
    {
       ROS_INFO("lidarName_ : %s ",objMultiFrameParam_.lidarName.c_str());
       subCloudBp_              = nh_.subscribe(objMultiFrameParam_.lidarName, 1, &MULTI_FRAME_PROCESS_C::RsBpLidarCallback,this);
       pubMultiFramePointCloud_ = nh_.advertise<sensor_msgs::PointCloud2>(objMultiFrameParam_.lidarName + "/multiFrame", 10);

       mkdir(objMultiFrameParam_.saveBinDir.c_str(),S_IRWXU);
       mkdir(objMultiFrameParam_.savePcdDir.c_str(),S_IRWXU);
       ROS_INFO("MULTI_FRAME_PROCESS_C Succ");

    };
    ~MULTI_FRAME_PROCESS_C(void) {};

    bool Init();
    bool SavePCDFileBinary(std::string path,pcl::PointCloud<pcl::PointXYZRGBL> pointCloud);
    bool SavePCDFileASCII(std::string path,pcl::PointCloud<pcl::PointXYZI> pointCloud);
    bool SavePLYFile(std::string path,pcl::PointCloud<pcl::PointXYZI> pointCloud);
    bool SaveBinFile(std::string path,pcl::PointCloud<pcl::PointXYZRGBL> pointCloud);
    bool SaveMultiFrame(pcl::PointCloud<pcl::PointXYZI>::Ptr pInCloud);

private:
    bool MultiFramePointsTransform(pcl::PointCloud<pcl::PointXYZI>::Ptr pInCloud,pcl::PointCloud<pcl::PointXYZI>::Ptr pOutCloud);
    void RsBpLidarCallback(const sensor_msgs::PointCloud2::ConstPtr &cloudMsg);

    ros::NodeHandle                 nh_;
    MULTI_FRAME_PARAM_STRU          objMultiFrameParam_;
    pcl::VoxelGrid<pcl::PointXYZI>  downSizeFilter_;
    ros::Publisher                  pubMultiFramePointCloud_;
    ros::Subscriber                 subCloudBp_;
};

#endif




