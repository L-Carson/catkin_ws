#include <pcpt_net_multi_frame_process.h>

/**************************************************************************************
功能描述: Init()
修改记录:
**************************************************************************************/
bool MULTI_FRAME_PROCESS_C::Init()
{
    return true;
}

/**************************************************************************************
功能描述: SavePCDFileBinary
修改记录:
**************************************************************************************/
bool MULTI_FRAME_PROCESS_C::SavePCDFileBinary(std::string path,pcl::PointCloud<pcl::PointXYZRGBL> pointCloud)
{
    ROS_INFO("save to pcd path: %s ",path.c_str());
    pcl::io::savePCDFileBinary(path, pointCloud);
    return true;
}

/**************************************************************************************
功能描述: SavePLYFile
修改记录:
**************************************************************************************/
bool MULTI_FRAME_PROCESS_C::SavePLYFile(std::string path,pcl::PointCloud<pcl::PointXYZI> pointCloud)
{
    ROS_INFO("save to ply path: %s ",path.c_str());
    pcl::io::savePLYFile(path, pointCloud);
    return true;
}

bool MULTI_FRAME_PROCESS_C::SavePCDFileASCII(std::string path,pcl::PointCloud<pcl::PointXYZI> pointCloud)
{
    ROS_INFO("save to pcd ASCII path: %s ",path.c_str());
    pcl::io::savePCDFileASCII(path, pointCloud);
    return true;
}

/**************************************************************************************
功能描述: SaveBinFile
修改记录:
**************************************************************************************/
bool MULTI_FRAME_PROCESS_C::SaveBinFile(std::string path,pcl::PointCloud<pcl::PointXYZRGBL> pointCloud)
{
    ROS_INFO("save to bin path: %s ",path.c_str());
    std::ofstream out;
    out.open(path, std::ios::out | std::ios::binary);
    int cloudSize = pointCloud.points.size();
    for (int i = 0; i < cloudSize; ++i)
    {
        float point_x   = pointCloud.points[i].x;
        float point_y   = pointCloud.points[i].y;
        float point_z   = pointCloud.points[i].z ;
        float intensity = pointCloud.points[i].rgba; //int64->float 防止溢出
        float channel   = pointCloud.points[i].label;
        //printf("intensity: %f \n",intensity);
        out.write(reinterpret_cast<const char *>(&point_x), sizeof(point_x));
        out.write(reinterpret_cast<const char *>(&point_y), sizeof(point_y));
        out.write(reinterpret_cast<const char *>(&point_z), sizeof(point_z));
        out.write(reinterpret_cast<const char *>(&intensity), sizeof(intensity));
        out.write(reinterpret_cast<const char *>(&channel), sizeof(channel));
    }
    out.close();
    return true;
}

/**************************************************************************************
功能描述: 多帧点云 (tf由  base_link -> map -> 当前帧的base_link)
修改记录:
**************************************************************************************/
bool MULTI_FRAME_PROCESS_C::MultiFramePointsTransform(pcl::PointCloud<pcl::PointXYZI>::Ptr pInCloud,pcl::PointCloud<pcl::PointXYZI>::Ptr pOutCloud)
{
    static std::vector<pcl::PointCloud<pcl::PointXYZI>::Ptr>  vectorMapPoints;
    static tf::TransformListener            listenerTf;
    static tf::StampedTransform             transform;
    pcl::PointCloud<pcl::PointXYZI>::Ptr    pSensorPoints(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::PointCloud<pcl::PointXYZI>::Ptr    pMapPoints(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::PointCloud<pcl::PointXYZI>::Ptr    pGlobalCloud(new pcl::PointCloud<pcl::PointXYZI>);
    std::vector<Eigen::Matrix4f>            tfDataBuff;

    *pSensorPoints = *pInCloud;
    try {
        listenerTf.waitForTransform(objMultiFrameParam_.pcptMap, "base_link", ros::Time(0), ros::Duration(1.0));
        listenerTf.lookupTransform(objMultiFrameParam_.pcptMap, "base_link", ros::Time(0), transform);
    } catch (tf::TransformException &ex) {
        ROS_INFO("Transform error of sensor data: %s ", ex.what());
        return false;
    }

    Eigen::Matrix4f sensorToWorld = Eigen::Matrix4f::Identity();
    pcl_ros::transformAsMatrix(transform, sensorToWorld);   //直接得到矩阵
    pcl::transformPointCloud(*pSensorPoints, *pMapPoints, sensorToWorld);
    tfDataBuff.insert(tfDataBuff.begin(),sensorToWorld);
    vectorMapPoints.insert(vectorMapPoints.begin(),pMapPoints);
    if (vectorMapPoints.size() > 10) {
       vectorMapPoints.pop_back();
       //取出多帧点云
       for (size_t i = 0 ; i < vectorMapPoints.size(); ++i) {
            Eigen::Matrix4f baseTfData = tfDataBuff[0].inverse();
            pcl::transformPointCloud(*vectorMapPoints[i], *pGlobalCloud, baseTfData);
            *pOutCloud += *pGlobalCloud;
        }
    }
    return true;
}

/**************************************************************************************
功能描述:保存Rs雷达数据
修改记录:
**************************************************************************************/
void MULTI_FRAME_PROCESS_C::RsBpLidarCallback(const sensor_msgs::PointCloud2::ConstPtr &cloudMsg)
{
    // ROS_INFO("RsBp Lidar Callback");
    double time0 = ros::Time::now().toSec();
    double time1 = ros::Time::now().toSec();
    double time2 = ros::Time::now().toSec();
    double time3 = ros::Time::now().toSec();

    //ros::Time time = cloudMsg->header.stamp;
    pcl::PointCloud<pcl::PointXYZI>::Ptr    rsBpLidar(new pcl::PointCloud<pcl::PointXYZI>());
    rsBpLidar->clear();
    if (objMultiFrameParam_.isLidarNewDrive) {
        BpCloudXYZIRT PointCloudBp;
        pcl::fromROSMsg(*cloudMsg,PointCloudBp);
        for (size_t i = 0; i < PointCloudBp.size(); ++i) {
            pcl::PointXYZI point;
            point.x = PointCloudBp.points[i].x;
            point.y = PointCloudBp.points[i].y;
            point.z = PointCloudBp.points[i].z;
            point.intensity = PointCloudBp.points[i].intensity;
            // int channel = PointCloudBp.points[i].intensity;
            // point.intensity = (PointCloudBp.points[i].intensity-channel)*10000;
            if (point.x < 8 && point.x >-5 && point.y > -5 && point.y < 5 && point.z < 2.0) {
                rsBpLidar->points.push_back(point);
            }
        }
    } else {
        CloudType PointCloudBp;
        pcl::fromROSMsg(*cloudMsg,PointCloudBp);
        for (size_t i = 0; i < PointCloudBp.size(); ++i) {
            pcl::PointXYZI point;
            point.x         = PointCloudBp.points[i].x;
            point.y         = PointCloudBp.points[i].y;
            point.z         = PointCloudBp.points[i].z;
            int channel     = PointCloudBp.points[i].intensity;
            point.intensity = (PointCloudBp.points[i].intensity-channel)*10000;
            if (point.x < 8 && point.x >-5 && point.y > -5 && point.y < 5 && point.z < 2.0) {
                rsBpLidar->points.push_back(point);
            }
        }
    }

    time1 = ros::Time::now().toSec() - time0;
    pcl::PointCloud<pcl::PointXYZI>::Ptr inCloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::PointCloud<pcl::PointXYZI>::Ptr outCloud(new pcl::PointCloud<pcl::PointXYZI>);
    *inCloud = *rsBpLidar ;
    float LeafSize = 0.04;
    downSizeFilter_.setLeafSize(LeafSize, LeafSize, LeafSize);
    downSizeFilter_.setInputCloud(inCloud);
    downSizeFilter_.filter(*inCloud);

    time2 = ros::Time::now().toSec() - time0;
    if (!MultiFramePointsTransform(inCloud, outCloud)) {
       ROS_INFO("MultiFrame PointsTransform Fail ... ");
       return ;
    }

    time3 = ros::Time::now().toSec() - time0;

    if (outCloud->size()) {
        sensor_msgs::PointCloud2 pointCloudMsg;
        pcl::toROSMsg(*outCloud, pointCloudMsg);
        pointCloudMsg.header.stamp = ros::Time::now();
        pointCloudMsg.header.frame_id = "base_link";
        pubMultiFramePointCloud_.publish(pointCloudMsg);

        if(objMultiFrameParam_.isSaveMulti) SaveMultiFrame(outCloud);
    }

    ROS_INFO("Time: %f %f %f %f",time0, time1, time2, time3);
}

/**************************************************************************************
功能描述:保存多帧
修改记录:
**************************************************************************************/
bool MULTI_FRAME_PROCESS_C::SaveMultiFrame(pcl::PointCloud<pcl::PointXYZI>::Ptr pInCloud)
{
    pcl::PointCloud<pcl::PointXYZRGBL>::Ptr outRGBL(new pcl::PointCloud<pcl::PointXYZRGBL>);
    for (size_t i = 0; i < pInCloud->size(); ++i) {
        pcl::PointXYZRGBL point;
        point.x = pInCloud->points[i].x;
        point.y = pInCloud->points[i].y;
        point.z = pInCloud->points[i].z;
        point.rgba = pInCloud->points[i].intensity;
        point.label = 1 ;
        outRGBL->points.push_back(point);
    }
    // ROS_INFO("pInCloud: %d %d ",pInCloud->size(),outRGBL->size());

    static int n = 0;
    std::string savePcdName;
    std::string saveBinName;
    char fileNum[256];
    n = n + 1;
    if ((n % 10) == 0) {
        objMultiFrameParam_.startFrameId  = objMultiFrameParam_.startFrameId + 1;
        sprintf(fileNum, "%04d", objMultiFrameParam_.startFrameId);

        savePcdName         = objMultiFrameParam_.savePcdDir +  fileNum + ".pcd";
        saveBinName         = objMultiFrameParam_.saveBinDir +  fileNum + ".bin";
        // ROS_INFO("savePcdName: %s",savePcdName.c_str());
        // ROS_INFO("saveBinName: %s",saveBinName.c_str());
        SavePCDFileBinary(savePcdName,*outRGBL);
        // SavePCDFileASCII(savePcdASSCIIName,pOutBaseCloud);
        // SavePLYFile(savePlyName,pOutBaseCloud);
        SaveBinFile(saveBinName,*outRGBL);
    }

    return true;
}