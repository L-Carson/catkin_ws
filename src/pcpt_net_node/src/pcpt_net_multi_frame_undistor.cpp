#include "pcpt_net_multi_frame_undistor.h"

/**************************************************************************************
功能描述:MULTI_FRAME_UNDISTORTION_C
修改记录:
**************************************************************************************/
MULTI_FRAME_UNDISTORTION_C::MULTI_FRAME_UNDISTORTION_C(ros::NodeHandle &nh,LIDAR_IMU_PARAM_STRU& lidarImuParam)
    : nh_(nh),
    lidarImuParam_(lidarImuParam)
{
    subImu_                 = nh_.subscribe<sensor_msgs::Imu>(stLidarType_.imuTopic, 200, &MULTI_FRAME_UNDISTORTION_C::ImuHandler, this);
    subLidarCloud_          = nh_.subscribe<sensor_msgs::PointCloud2>(lidarImuParam_.sensorNameRs, 5, &MULTI_FRAME_UNDISTORTION_C::CloudHandler, this);
    pubCloudUndistortion_   = nh_.advertise<sensor_msgs::PointCloud2> (lidarImuParam_.sensorNameRs + "/undistor", 1);
    pubCloudLidar_          = nh_.advertise<sensor_msgs::PointCloud2> (lidarImuParam_.sensorNameRs + "/src", 1);
    ResetParameters();
    pcl::console::setVerbosityLevel(pcl::console::L_ERROR);
    // imu2LidarRot_ << -1, 0, 0, 0, -1, 0, 0, 0,1;
    // imu2LidarRPY_ << 1, 0, 0,0, 1, 0, 0, 0, 1;
    imu2LidarRot_ << MAT_FROM_ARRAY(lidarImuParam_.imu2LidarRot);
    imu2LidarRPY_ << MAT_FROM_ARRAY(lidarImuParam_.imu2LidarRPY);

    imu2LidarQRPY_ = Eigen::Quaterniond(imu2LidarRPY_);
    ROS_INFO("MULTI_FRAME_UNDISTORTION_C Succ ");
}

MULTI_FRAME_UNDISTORTION_C::~MULTI_FRAME_UNDISTORTION_C()
{

}

/**************************************************************************************
功能描述:resetParameters
修改记录:
**************************************************************************************/
void MULTI_FRAME_UNDISTORTION_C::ResetParameters()
{
    imuCur_             = 0;
    firstPointFlag_     = true;

    for (int i = 0; i < stLidarType_.queueLengt; ++i)
    {
        imuTime_[i] = 0;
        imuRotX_[i] = 0;
        imuRotY_[i] = 0;
        imuRotZ_[i] = 0;
    }
}

/**************************************************************************************
功能描述:回调imu
修改记录:
**************************************************************************************/
void MULTI_FRAME_UNDISTORTION_C::ImuHandler(const sensor_msgs::Imu::ConstPtr& imuMsg)
{
    std::lock_guard<std::mutex> lock1(imuLock_);
    sensor_msgs::Imu imuOut = *imuMsg;
    Eigen::Vector3d acc(imuMsg->linear_acceleration.x, imuMsg->linear_acceleration.y, imuMsg->linear_acceleration.z);
    acc = imu2LidarRot_ * acc;
    imuOut.linear_acceleration.x = acc.x();
    imuOut.linear_acceleration.y = acc.y();
    imuOut.linear_acceleration.z = acc.z();
    Eigen::Vector3d gyr(imuMsg->angular_velocity.x, imuMsg->angular_velocity.y, imuMsg->angular_velocity.z);
    gyr = imu2LidarRot_ * gyr;
    imuOut.angular_velocity.x = gyr.x();
    imuOut.angular_velocity.y = gyr.y();
    imuOut.angular_velocity.z = gyr.z();
    Eigen::Quaterniond qFrom(imuMsg->orientation.w, imuMsg->orientation.x, imuMsg->orientation.y, imuMsg->orientation.z);
    Eigen::Quaterniond q_final = qFrom * imu2LidarQRPY_;
    imuOut.orientation.x = q_final.x();
    imuOut.orientation.y = q_final.y();
    imuOut.orientation.z = q_final.z();
    imuOut.orientation.w = q_final.w();

    if (sqrt(qFrom.x()*qFrom.x() + qFrom.y()*qFrom.y() + qFrom.z()*qFrom.z() + qFrom.w()*qFrom.w()) < 0.1)
    {
        printf("Invalid quaternion, please use a 9-axis IMU!\n");
        ros::shutdown();
    }
    imuQueue_.push_back(imuOut);
}

/**************************************************************************************
功能描述:用于去畸变的相关关键信息或变量值计算
修改记录:
**************************************************************************************/
bool MULTI_FRAME_UNDISTORTION_C::ImuDeskewInfo()
{
    std::lock_guard<std::mutex> lock1(imuLock_);
    if (imuQueue_.empty())
    {
        ROS_INFO("Waiting For IMU Data ...");
        return false;
    }

    imuAvailable_ = false;
    while (!imuQueue_.empty())
    {
        /* 以0.01为阈值 舍弃较旧的imu数据 */
        if (imuQueue_.front().header.stamp.toSec() < timeScanCur_ - 0.01)
            imuQueue_.pop_front();
        else
            break;
    }
    if (imuQueue_.empty()){
        ROS_INFO(" imuQueue_ Enpty  ...");
        return false;
    }

    imuCur_ = 0;
    for (int i = 0; i < (int)imuQueue_.size(); ++i)
    {
        sensor_msgs::Imu thisImuMsg = imuQueue_[i];
        double currentImuTime = thisImuMsg.header.stamp.toSec();
        //如果当前Imu时间比下一帧时间大于0.01 退出
        if (currentImuTime > timeScanEnd_ + 0.01)
            break;
         //因为第一次初始化时以下值都是0
        if (imuCur_ == 0){
            imuRotX_[0] = 0;
            imuRotY_[0] = 0;
            imuRotZ_[0] = 0;
            imuTime_[0] = currentImuTime;
            ++imuCur_;
            continue;
        }
        //从imu信息中直接获得角速度
        double angularX, angularY, angularZ;
        angularX = thisImuMsg.angular_velocity.x;
        angularY = thisImuMsg.angular_velocity.y;
        angularZ = thisImuMsg.angular_velocity.z;

        double timeDiff = currentImuTime - imuTime_[imuCur_-1];
        if(timeDiff > 0.1 || timeDiff < -0.1)
        {
            ROS_INFO("timeDiff %f: ",timeDiff);
        }
        imuRotX_[imuCur_] = imuRotX_[imuCur_-1] + angularX * timeDiff;
        imuRotY_[imuCur_] = imuRotY_[imuCur_-1] + angularY * timeDiff;
        imuRotZ_[imuCur_] = imuRotZ_[imuCur_-1] + angularZ * timeDiff;
        imuTime_[imuCur_] = currentImuTime;
        // cout<<timeDiff<<endl;
        // cout<<angularX << " "<<angularY << " "<<angularZ << " "<<endl;
        // cout<<imuRotX_[imuCur_] << " "<<imuRotY_[imuCur_] << " "<<imuRotZ_[imuCur_] << " "<<endl;
        ++imuCur_;
    }
    --imuCur_;

    if (imuCur_ <= 0){
        ROS_INFO(" Cur Imu Pointer <= 0 , Cur Imu : %d ...",imuCur_);
        return false;
    }
    imuAvailable_ = true;
    return true;
}

/**************************************************************************************
功能描述:运动补偿函数
修改记录:
**************************************************************************************/
BpPointXYZIRT MULTI_FRAME_UNDISTORTION_C::DeskewPoint(BpPointXYZIRT *point, double relTime)
{
    if (imuAvailable_ == false)
    {
        ROS_INFO(" imuAvailable_ false...");
        return *point;
    }
    /* 点在一帧中的具体时间 */
    //double pointTime = timeScanCur_ + relTime;
    // printf("time : %f \n",pointTime);
    double pointTime =  relTime;
    float rotXCur, rotYCur, rotZCur;
    float posXCur, posYCur, posZCur;

    /* 获得相关旋转量 */
    rotXCur         = 0;
    rotYCur         = 0;
    rotZCur         = 0;
    int imuFront    = 0;
    while (imuFront < imuCur_)
    {
        if (pointTime < imuTime_[imuFront])
            break;
        ++imuFront;
    }
    if (pointTime > imuTime_[imuFront] || imuFront == 0)
    {
        rotXCur = imuRotX_[imuFront];
        rotYCur = imuRotY_[imuFront];
        rotZCur = imuRotZ_[imuFront];
    } else {
        int imuBack         = imuFront - 1;
        double ratioFront   = (pointTime - imuTime_[imuBack]) / (imuTime_[imuFront] - imuTime_[imuBack]);
        double ratioBack    = (imuTime_[imuFront] - pointTime) / (imuTime_[imuFront] - imuTime_[imuBack]);
        rotXCur = imuRotX_[imuFront] * ratioFront + imuRotX_[imuBack] * ratioBack;
        rotYCur = imuRotY_[imuFront] * ratioFront + imuRotY_[imuBack] * ratioBack;
        rotZCur = imuRotZ_[imuFront] * ratioFront + imuRotZ_[imuBack] * ratioBack;
        //if (relTime < 0.005) cout<<"dfsfds"<<pointTime - imuTime_[imuBack]<<endl;
    }

    /* 获得相关平移，如果高速移动可能有用 低速车辆提升不大*/
    posXCur = 0;
    posYCur = 0;
    posZCur = 0;

    /* 如果是第一次收到数据 旋转矩阵初始化一次 后续迭代使用 */
    if (firstPointFlag_ == true)
    {
        transStartInverse_ = (pcl::getTransformation(posXCur, posYCur, posZCur, rotXCur, rotYCur, rotZCur)).inverse();
        firstPointFlag_ = false;
    }
    /* 把点投影到每一帧扫描的起始时刻 */
    Eigen::Affine3f transFinal  = pcl::getTransformation(posXCur, posYCur, posZCur, rotXCur, rotYCur, rotZCur);
    Eigen::Affine3f transBt     = transStartInverse_ * transFinal;


    /* 得到去畸变后的点云 */
    BpPointXYZIRT newPoint = *point;
    newPoint.x = transBt(0,0) * point->x + transBt(0,1) * point->y + transBt(0,2) * point->z + transBt(0,3);
    newPoint.y = transBt(1,0) * point->x + transBt(1,1) * point->y + transBt(1,2) * point->z + transBt(1,3);
    newPoint.z = transBt(2,0) * point->x + transBt(2,1) * point->y + transBt(2,2) * point->z + transBt(2,3);

    // if (relTime < 0.0005) {
    //     for(int i = 0; i<3 ; i++){
    //         printf("transBt :%f %f %f %f \n ",transBt(i,0),transBt(i,1),transBt(i,2), transBt(i,3));
    //     }

    //     for(int i = 0; i<3 ; i++){
    //         printf("transdfdsBt :%f %f %f %f\n ",transFinal(i,0),transFinal(i,1),transFinal(i,2), transFinal(i,3));
    //     }
    //     Eigen::Vector3f newp  = Eigen::Vector3f(newPoint.x, newPoint.y, newPoint.z);
    //     Eigen::Vector3f p     = Eigen::Vector3f(point->x, point->y, point->z);
    //     cout<<(newp - p).transpose()<< " "<<newp.transpose()<<" "<<p.transpose() <<endl;
    // }

    return newPoint;
}

/**************************************************************************************
功能描述: 去畸变
修改记录:
**************************************************************************************/
void MULTI_FRAME_UNDISTORTION_C::MotionUnDistortion(BpCloudXYZIRT::Ptr pointCloudIn)
{
    int cloudSize = pointCloudIn->points.size();
    /* 把点云数据按线束 按行列保存 */
    BpCloudXYZIRT::Ptr   cloudUndistortion(new BpCloudXYZIRT);
    for (int i = 0; i < cloudSize; ++i)
    {
        BpPointXYZIRT thisPoint = pointCloudIn->points[i];
        /* 去畸变  运动补偿 */
        thisPoint = DeskewPoint(&thisPoint, pointCloudIn->points[i].timestamp);
        cloudUndistortion->push_back(thisPoint);
    }

    sensor_msgs::PointCloud2 tempCloud;
    pcl::toROSMsg(*cloudUndistortion, tempCloud);
    tempCloud.header.stamp = cloudHeader_.stamp;
    tempCloud.header.frame_id = stLidarType_.lidarFrame;
    if (pubCloudUndistortion_.getNumSubscribers() != 0)
    {
        pubCloudUndistortion_.publish(tempCloud);
    }
}

/**************************************************************************************
功能描述:回调点云
修改记录:
**************************************************************************************/
void MULTI_FRAME_UNDISTORTION_C::CloudHandler(const sensor_msgs::PointCloud2ConstPtr& msg)
{
    /* 参数复位 */
    ResetParameters();
     /*缓存点云信息*/
    pcl::PointCloud<BpPointXYZIRT>::Ptr  pointCloudIn(new pcl::PointCloud<BpPointXYZIRT>());
    cloudQueue_.push_back(*msg);
    sensor_msgs::PointCloud2 currentCloudMsg;
    currentCloudMsg = std::move(cloudQueue_.front());
    cloudQueue_.pop_front();
    pcl::moveFromROSMsg(currentCloudMsg, *pointCloudIn);

    cloudHeader_        =  currentCloudMsg.header;
    timeScanCur_        =  pointCloudIn->points[0].timestamp;
    timeScanEnd_        =  timeScanCur_ + pointCloudIn->points.back().timestamp;
    /*发布点云*/
    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(*pointCloudIn, output);
    output.header.stamp = msg->header.stamp;
    output.header.frame_id = "base_link";
    pubCloudLidar_.publish(output);
    /*计算去畸变所需的参数*/
    if (!ImuDeskewInfo()){
        ROS_INFO("Imu Deskew Info Fail... ");
        return;
    }
    /*去畸变*/
    MotionUnDistortion(pointCloudIn);

}

