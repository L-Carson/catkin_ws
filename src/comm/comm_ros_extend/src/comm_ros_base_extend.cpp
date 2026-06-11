#include "comm_ros_extend/comm_ros_base_extend.h"

POINT_TYPE_ENUM GetPointType(const sensor_msgs::PointCloud2::ConstPtr &pstMsg)
{
    for (size_t i = 0; i < pstMsg->fields.size(); ++i) {
        if (pstMsg->fields[i].name == "dis")
            return MY_POINT_TYPE_XYZIRCDT;
    }

    for (size_t i = 0; i < pstMsg->fields.size(); ++i) {
        if (pstMsg->fields[i].name == "timestamp")
            return MY_POINT_TYPE_XYZIRT;
    }

    for (size_t i = 0; i < pstMsg->fields.size(); ++i) {
        if (pstMsg->fields[i].name == "intensity")
            return MY_POINT_TYPE_XYZI;
    }

    return MY_POINT_TYPE_XYZ;
}

INT32 POINTCLOUD_MSG_RECEIVER_C::Get(CLOUD_DATA_STRU &stCloud, BOOL isNodelay)
{
    if (isNodelay) {
        if (objInvalidTimer_.IsTimeOut())   return -1;
    }
    else {
        if (!isExist_)   return -1;
    }

    stCloud.time     = stCloud_.time;
    *stCloud.pPoints = *stCloud_.pPoints;

    isExist_ = false;
    objInvalidTimer_.Clear();

    return 0;
}

void  POINTCLOUD_MSG_RECEIVER_C::ReceiveMsgCallBack(const sensor_msgs::PointCloud2::ConstPtr &pstMsg)
{
    stCloud_.time = pstMsg->header.stamp.toSec();
    if (enPointType_ == MY_POINT_TYPE_UNKNOW) {
        enPointType_ = GetPointType(pstMsg);
        if (   enPointType_ != MY_POINT_TYPE_XYZI && enPointType_ != MY_POINT_TYPE_XYZIRCDT
            && enPointType_ != MY_POINT_TYPE_XYZIRT)
            ST_LOG_ERR("Point Type Is Err %d.", (INT32)enPointType_);
    }

    if (enPointType_ == MY_POINT_TYPE_XYZIRCDT) {
        PointCloudXYZIRCDT pointCloud;
        pcl::fromROSMsg(*pstMsg, pointCloud);
        stCloud_.pPoints->header = pointCloud.header;
        stCloud_.pPoints->points.resize(pointCloud.size());
        for (size_t i = 0; i < pointCloud.size(); i++) {
            stCloud_.pPoints->points[i].x = pointCloud.points[i].x;
            stCloud_.pPoints->points[i].y = pointCloud.points[i].y;
            stCloud_.pPoints->points[i].z = pointCloud.points[i].z;
            stCloud_.pPoints->points[i].intensity = pointCloud.points[i].intensity;
        }
    }
    else if (enPointType_ == MY_POINT_TYPE_XYZI || enPointType_ == MY_POINT_TYPE_XYZIRT){
        pcl::fromROSMsg(*pstMsg, *stCloud_.pPoints);
    } else {
        return ;
    }

    isExist_ = true;
    objInvalidTimer_.Reset();
}

void  POINTCLOUD_MSG_SENDER_C::Send(const PointCloudXYZI::Ptr pCloud)
{
    if (publisher_.getNumSubscribers() != 0) {
        sensor_msgs::PointCloud2 pointCloudMsg;
        pcl::toROSMsg(*pCloud, pointCloudMsg);
        pointCloudMsg.header.stamp    = ros::Time::now();
        pointCloudMsg.header.frame_id = pCloud->header.frame_id;
        publisher_.publish(pointCloudMsg);
    }
}

INT32 POINTCLOUD_XYZIRT_MSG_RECEIVER_C::Get(CLOUD_XYZIRT_DATA_STRU &stCloud)
{
    if (objInvalidTimer_.IsTimeOut()) return -1;

    stCloud.time     = stCloud_.time;
    *stCloud.pPoints = *stCloud_.pPoints; //需要优化一下

    objInvalidTimer_.Clear();

    return 0;
}

void  POINTCLOUD_XYZIRT_MSG_RECEIVER_C::ReceiveMsgCallBack(const sensor_msgs::PointCloud2::ConstPtr &pstMsg)
{
    stCloud_.time = pstMsg->header.stamp.toSec();
    if (enPointType_ == MY_POINT_TYPE_UNKNOW) {
        enPointType_ = GetPointType(pstMsg);
        if (enPointType_ != MY_POINT_TYPE_XYZIRT && enPointType_ != MY_POINT_TYPE_XYZIRCDT)
            ST_LOG_ERR("Point Type Is Err %d.", (INT32)enPointType_);
    }

    if (enPointType_ == MY_POINT_TYPE_XYZIRCDT) {
        PointCloudXYZIRCDT pointCloud;
        pcl::fromROSMsg(*pstMsg, pointCloud);
        stCloud_.pPoints->header = pointCloud.header;
        stCloud_.pPoints->points.resize(pointCloud.size());
        for (size_t i = 0; i < pointCloud.size(); i++) {
            stCloud_.pPoints->points[i].x = pointCloud.points[i].x;
            stCloud_.pPoints->points[i].y = pointCloud.points[i].y;
            stCloud_.pPoints->points[i].z = pointCloud.points[i].z;
            stCloud_.pPoints->points[i].intensity = pointCloud.points[i].intensity;
            stCloud_.pPoints->points[i].ring      = pointCloud.points[i].ring;
            stCloud_.pPoints->points[i].timestamp = pointCloud.points[i].timestamp;
        }
    }
    else if (enPointType_ == MY_POINT_TYPE_XYZIRT){
        pcl::fromROSMsg(*pstMsg, *stCloud_.pPoints);
    } else {
        return ;
    }

    objInvalidTimer_.Reset();
}

void  POINTCLOUD_XYZIRT_MSG_SENDER_C::Send(const PointCloudXYZIRT::Ptr pCloud)
{
    if (publisher_.getNumSubscribers() != 0) {
        sensor_msgs::PointCloud2 pointCloudMsg;
        pcl::toROSMsg(*pCloud, pointCloudMsg);
        pointCloudMsg.header.stamp    = ros::Time::now();
        pointCloudMsg.header.frame_id = pCloud->header.frame_id;
        publisher_.publish(pointCloudMsg);
    }
}

INT32 LOC_POINTCLOUD_MSG_RECEIVER_C::Get(CLOUD_XYZIRT_DATA_STRU &stCloud)
{
    if (objInvalidTimer_.IsTimeOut())
        return -1;

    stCloud.time     = stCloud_.time;
    *stCloud.pPoints = *stCloud_.pPoints;

    objInvalidTimer_.Clear();

    return 0;
}

void  LOC_POINTCLOUD_MSG_RECEIVER_C::ReceiveMsgCallBack(const sensor_msgs::PointCloud2::ConstPtr &pstMsg)
{
    if (enPointType_ == MY_POINT_TYPE_UNKNOW) {
        enPointType_ = GetPointType(pstMsg);
        if (enPointType_ != MY_POINT_TYPE_XYZIRCDT && enPointType_ != MY_POINT_TYPE_XYZI)
            ST_LOG_ERR("Point Type Is Err %d.", (INT32)enPointType_);
    }

    stCloud_.time = pstMsg->header.stamp.toSec();
    if (enPointType_ == MY_POINT_TYPE_XYZIRCDT) {
        PointCloudXYZIRCDT pointCloud;
        pcl::fromROSMsg(*pstMsg, pointCloud);
        stCloud_.pPoints->points.resize(pointCloud.size());
        for (size_t i = 0; i < stCloud_.pPoints->size(); i++) {
            stCloud_.pPoints->points[i].x = pointCloud[i].x;
            stCloud_.pPoints->points[i].y = pointCloud[i].y;
            stCloud_.pPoints->points[i].z = pointCloud[i].z;
            stCloud_.pPoints->points[i].intensity = pointCloud[i].intensity;
            stCloud_.pPoints->points[i].ring      = pointCloud[i].ring;
            stCloud_.pPoints->points[i].timestamp = pointCloud[i].timestamp;
        }
        stCloud_.pPoints->height   = 1;
        stCloud_.pPoints->width    = pointCloud.size();
        stCloud_.pPoints->is_dense = true;
    } else if (enPointType_ == MY_POINT_TYPE_XYZI) {
        // 兼容以前的雷达驱动
        PointCloudXYZI pointCloud;
        pcl::fromROSMsg(*pstMsg, pointCloud);
        stCloud_.pPoints->points.resize(pointCloud.size());
        for (size_t i = 0; i < stCloud_.pPoints->size(); i++) {
            stCloud_.pPoints->points[i].x = pointCloud[i].x;
            stCloud_.pPoints->points[i].y = pointCloud[i].y;
            stCloud_.pPoints->points[i].z = pointCloud[i].z;
            stCloud_.pPoints->points[i].intensity = 10000.0 * (pointCloud[i].intensity - floorf(pointCloud[i].intensity));
            stCloud_.pPoints->points[i].ring      = (INT32)(pointCloud[i].intensity);
            stCloud_.pPoints->points[i].timestamp = stCloud_.time;
        }
        stCloud_.pPoints->height   = 1;
        stCloud_.pPoints->width    = pointCloud.size();
        stCloud_.pPoints->is_dense = true;
    } else {
        ST_LOG_ERR("Point Type Is Err %d.", (INT32)enPointType_);
        return ;
    }

    objInvalidTimer_.Reset();
}

void  SCE_MAP_MSG_SENDER_C::Send(COORDINATE_2D_STRU &stOriginPos, float resolution, UINT32 sizeX, UINT32 sizeY, UINT08 *pucMap)
{
    R_ASSERT(pucMap != NULL, NOTHING);

    nav_msgs::OccupancyGrid stMapMsg;

    stMapMsg.header.stamp    = ros::Time::now();
    stMapMsg.header.frame_id = "map";

    stMapMsg.info.resolution            = resolution;
    stMapMsg.info.origin.position.x     = stOriginPos.x;
    stMapMsg.info.origin.position.y     = stOriginPos.y;
    stMapMsg.info.origin.position.z     = 0.0;
    stMapMsg.info.origin.orientation.x  = 0.0;
    stMapMsg.info.origin.orientation.y  = 0.0;
    stMapMsg.info.origin.orientation.z  = 0.0;
    stMapMsg.info.origin.orientation.w  = 1.0;
    stMapMsg.info.width                 = sizeX;
    stMapMsg.info.height                = sizeY;

    stMapMsg.data.resize(stMapMsg.info.width * stMapMsg.info.height);
    for (UINT32 loop = 0; loop < stMapMsg.data.size(); ++loop) {
        switch (pucMap[loop]) {
            case 255:
                stMapMsg.data[loop] = 255;
                break;

            default:
                stMapMsg.data[loop] = 0;
                break;
        }
    }

    publisher_.publish(stMapMsg);
}

void  PATH_3D_MSG_SENDER_C::Send(const vector<Matrix4f> &vmPose)
{
    nav_msgs::Path stPathMsg;
    stPathMsg.header.frame_id = "map";
    stPathMsg.header.stamp    = ros::Time::now();
    for (size_t i = 0; i < vmPose.size(); i++){
        geometry_msgs::PoseStamped stPoseStamped;
        stPoseStamped.header.frame_id = "map";
        stPoseStamped.header.stamp    = ros::Time::now();
        stPoseStamped.pose.position.x = vmPose[i](0,3);
        stPoseStamped.pose.position.y = vmPose[i](1,3);
        stPoseStamped.pose.position.z = vmPose[i](2,3);

        Quaternionf quat(vmPose[i].block<3, 3>(0, 0));
        stPoseStamped.pose.orientation.x = quat.x();
        stPoseStamped.pose.orientation.y = quat.y();
        stPoseStamped.pose.orientation.z = quat.z();
        stPoseStamped.pose.orientation.w = quat.w();
        stPathMsg.poses.push_back(stPoseStamped);
    }
    publisher_.publish(stPathMsg);
}

void  PATH_3D_MSG_SENDER_C::Send(const deque<COORDINATE_3D_STRU> &vstPos)
{
    double time = ros::Time::now().toSec();
    Send(vstPos, time);
}

void  PATH_3D_MSG_SENDER_C::Send(const deque<COORDINATE_3D_STRU> &vstPos, const double &time)
{
    if (publisher_.getNumSubscribers() != 0) {
        nav_msgs::Path stPathMsg;
        ConvPos2PathMsg(time, vstPos, stPathMsg);
        publisher_.publish(stPathMsg);
   }

}

void  PATH_3D_MSG_SENDER_C::ConvPos2PathMsg(const double &time, const deque<COORDINATE_3D_STRU> &vstPos,
                                           nav_msgs::Path &stPathMsg)
{
    ros::Time rosTime((double)time);
    stPathMsg.header.frame_id = "map";
    stPathMsg.header.stamp    = rosTime;
    for (size_t i = 0; i < vstPos.size(); i++){
        geometry_msgs::PoseStamped stPoseStamped;
        stPoseStamped.header.frame_id = "map";
        stPoseStamped.header.stamp    = rosTime;
        stPoseStamped.pose.position.x = vstPos[i].x;
        stPoseStamped.pose.position.y = vstPos[i].y;
        stPoseStamped.pose.position.z = vstPos[i].z;
        stPoseStamped.pose.orientation.x = 0.0;
        stPoseStamped.pose.orientation.y = 0.0;
        stPoseStamped.pose.orientation.z = 0.0;
        stPoseStamped.pose.orientation.w = 1.0;
        stPathMsg.poses.push_back(stPoseStamped);
    }
}


INT32 MOVE_INFO_MSG_RECEIVER_C::Get(deque<MOVE_INFO_DATA_STRU>  &vstMoveInfo)
{
    if (vstMoveInfo_.size() > 0) {
        vstMoveInfo.insert(vstMoveInfo.end(), vstMoveInfo_.begin(), vstMoveInfo_.end());
        vstMoveInfo_.clear();
        return 0;
    }

    return -1;
}

void  MOVE_INFO_MSG_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::move_info::ConstPtr& pMsg)
{
    MOVE_INFO_DATA_STRU stMoveInfo;

    stMoveInfo.time          = pMsg->header.stamp.toSec();
    stMoveInfo.steeringAngle = pMsg->steeringAngle;
    stMoveInfo.moveLinearVel = pMsg->moveLinearVel;
    stMoveInfo.yawAngle      = pMsg->yawAngle;
    stMoveInfo.yawAngleVel   = pMsg->yawAngleVel;
    stMoveInfo.acc = Vector3f(pMsg->accX, pMsg->accY, pMsg->accZ);
    vstMoveInfo_.push_back(stMoveInfo);
}


void  ODOM_3D_MSG_RECEIVER_C::Get(deque<ODOM_DATA_STRU>  &vOdomData)
{
    if (vOdomData_.size() > 0) {
        vOdomData.insert(vOdomData.end(), vOdomData_.begin(), vOdomData_.end());
        vOdomData_.clear();
    }
}

INT32 ODOM_3D_MSG_RECEIVER_C::Get(ODOM_DATA_STRU  &stOdomData)
{
    if (vOdomData_.size() > 0) {
        stOdomData = vOdomData_.back();
        return 0;
    }
    return -1;
}

void  ODOM_3D_MSG_RECEIVER_C::ReceiveMsgCallBack(const nav_msgs::Odometry::ConstPtr& pMsg)
{
    ODOM_DATA_STRU stOdomData;
    stOdomData.time = pMsg->header.stamp.toSec();
    stOdomData.stVelData.time = stOdomData.time;

    stOdomData.stVelData.linearVel = Eigen::Vector3f(pMsg->twist.twist.linear.x,
                                                     pMsg->twist.twist.linear.y,
                                                     pMsg->twist.twist.linear.z);
    stOdomData.stVelData.angleVel  = Eigen::Vector3f(pMsg->twist.twist.angular.x,
                                                     pMsg->twist.twist.angular.y,
                                                     pMsg->twist.twist.angular.z);

    Matrix4f    mPose = Matrix4f::Identity();
    Quaternionf quat  = Quaternionf(pMsg->pose.pose.orientation.w,
                                    pMsg->pose.pose.orientation.x,
                                    pMsg->pose.pose.orientation.y,
                                    pMsg->pose.pose.orientation.z);
    mPose.block<3, 1>(0, 3) = Vector3f(pMsg->pose.pose.position.x,
                                       pMsg->pose.pose.position.y,
                                       pMsg->pose.pose.position.z);
    mPose.block<3, 3>(0, 0) = quat.normalized().toRotationMatrix();
    stOdomData.stPose.Copy(mPose);

    vOdomData_.push_back(stOdomData);
    while(vOdomData_.size() > buffSize_){
        vOdomData_.pop_front();
    }
}

void  ODOM_3D_MSG_SENDER_C::Send(const Matrix4f &mPose)
{
    double time = ros::Time::now().toSec();
    Send(mPose, time);
}

void  ODOM_3D_MSG_SENDER_C::Send(const Matrix4f &mPose,  const double &time)
{
    nav_msgs::Odometry odometry;
    ros::Time rosTime((double)time);
    odometry.header.stamp    = rosTime;
    odometry.header.frame_id = strParentsFrame_;
    odometry.child_frame_id  = strChildFrame_;

    odometry.pose.pose.position.x = mPose(0,3);
    odometry.pose.pose.position.y = mPose(1,3);
    odometry.pose.pose.position.z = mPose(2,3);

    Quaternionf quat(mPose.block<3, 3>(0, 0));
    odometry.pose.pose.orientation.x = quat.x();
    odometry.pose.pose.orientation.y = quat.y();
    odometry.pose.pose.orientation.z = quat.z();
    odometry.pose.pose.orientation.w = quat.w();

    publisher_.publish(odometry);
}

void  ODOM_3D_MSG_SENDER_C::Send(ODOM_DATA_STRU stOdomData)
{
    nav_msgs::Odometry odometry;
    ros::Time rosTime(stOdomData.time);
    odometry.header.stamp    = rosTime;
    odometry.header.frame_id = strParentsFrame_;
    odometry.child_frame_id  = strChildFrame_;

    Matrix4f mPose = stOdomData.stPose.GetTransform();
    odometry.pose.pose.position.x = mPose(0,3);
    odometry.pose.pose.position.y = mPose(1,3);
    odometry.pose.pose.position.z = mPose(2,3);

    Quaternionf quat(mPose.block<3, 3>(0, 0));
    odometry.pose.pose.orientation.x = quat.x();
    odometry.pose.pose.orientation.y = quat.y();
    odometry.pose.pose.orientation.z = quat.z();
    odometry.pose.pose.orientation.w = quat.w();

    /* 车体坐标系下的线速度和角速度　*/
    odometry.twist.twist.linear.x  = stOdomData.stVelData.linearVel(0);
    odometry.twist.twist.linear.y  = stOdomData.stVelData.linearVel(1);
    odometry.twist.twist.linear.z  = stOdomData.stVelData.linearVel(2);
    odometry.twist.twist.angular.x = stOdomData.stVelData.angleVel(0);
    odometry.twist.twist.angular.y = stOdomData.stVelData.angleVel(1);
    odometry.twist.twist.angular.z = stOdomData.stVelData.angleVel(2);

    publisher_.publish(odometry);
}


INT32 IMU_MSG_RECEIVER_C::Get(deque<IMU_DATA_STRU>  &vImuData)
{
    if (vImuData_.size() > 0) {
        vImuData.insert(vImuData.end(), vImuData_.begin(), vImuData_.end());
        vImuData_.clear();
        return 0;
    }
    return -1;
}

void IMU_MSG_RECEIVER_C::ReceiveMsgCallBack(const sensor_msgs::ImuConstPtr &pMsg)
{
    IMU_DATA_STRU stImuData;
    stImuData.time = pMsg->header.stamp.toSec();
    stImuData.acc = Vector3f( pMsg->linear_acceleration.x,
                              pMsg->linear_acceleration.y,
                              pMsg->linear_acceleration.z );
    stImuData.gyro = Vector3f( pMsg->angular_velocity.x,
                               pMsg->angular_velocity.y,
                               pMsg->angular_velocity.z) ;
    stImuData.orientation = Quaternionf( pMsg->orientation.w,
                                         pMsg->orientation.x,
                                         pMsg->orientation.y,
                                         pMsg->orientation.z );
    vImuData_.push_back(stImuData);
    while(vImuData_.size() > buffSize_){
        vImuData_.pop_front();
    }
}

void  IMU_MSG_SENDER_C::Send(const IMU_DATA_STRU &stImuData)
{
    sensor_msgs::Imu imuMsg;
    imuMsg.header.stamp = ros::Time::now();
    imuMsg.header.frame_id = "base_link";

    imuMsg.orientation.x = stImuData.orientation.x();
    imuMsg.orientation.y = stImuData.orientation.y();
    imuMsg.orientation.z = stImuData.orientation.z();
    imuMsg.orientation.w = stImuData.orientation.w();

    imuMsg.linear_acceleration.x = stImuData.acc(0);
    imuMsg.linear_acceleration.y = stImuData.acc(1);
    imuMsg.linear_acceleration.z = stImuData.acc(2);

    imuMsg.angular_velocity.x = stImuData.gyro(0);
    imuMsg.angular_velocity.y = stImuData.gyro(1);
    imuMsg.angular_velocity.z = stImuData.gyro(2);

    publisher_.publish(imuMsg);
}


void  LOCATION_3D_MSG_SENDER_C::Send(MAP_NODE_STATE_ENUM enState, int subMapNo,
                                     const COOR_3D_POSE_STRU &stPose)
{
    comm_msg::locationInfo3D stMsg;
    stMsg.header.stamp = ros::Time::now();
    stMsg.header.frame_id = "base_link";

    stMsg.status   = (INT32)enState;
    stMsg.subMapNo = subMapNo;
    stMsg.posX     = stPose.x();
    stMsg.posY     = stPose.y();
    stMsg.posZ     = stPose.z();
    stMsg.roll     = stPose.roll();
    stMsg.pitch    = stPose.pitch();
    stMsg.yaw      = stPose.yaw();

    publisher_.publish(stMsg);
}

BOOL  LOCATION_3D_MSG_RECEIVER_C::IsNormal(void)
{
    return !objInvalidTimer_.IsTimeOut() && (enState_ >= 0);
}

BOOL  LOCATION_3D_MSG_RECEIVER_C::IsNormalNew(void)
{
    return (enState_ >= 0);
}

BOOL  LOCATION_3D_MSG_RECEIVER_C::IsChangingMap(void)
{
    return !objInvalidTimer_.IsTimeOut() && (enState_ == MAP_NODE_STATE_NAVIGATE_CHANGING_MAP);
}

BOOL  LOCATION_3D_MSG_RECEIVER_C::IsCreatingMap(void)
{
    return !objInvalidTimer_.IsTimeOut() && (enState_ == MAP_NODE_STATE_GMAPPING_CREATING_MAP);
}

void  LOCATION_3D_MSG_RECEIVER_C::Get(TIMESTAMP_POSE_STRU &stTsPose)
{
    stTsPose = stTsPose_;
}

INT32 LOCATION_3D_MSG_RECEIVER_C::GetNormal(TIMESTAMP_POSE_STRU &stTsPose)
{
    if (!IsNormal()) {
        return -1;
    }

    Get(stTsPose);
    return 0;
}

TIMESTAMP_POSE_STRU *LOCATION_3D_MSG_RECEIVER_C::GetNormal(void)
{
    if (!IsNormal()) {
        return NULL;
    }

    return &stTsPose_;
}

INT32 LOCATION_3D_MSG_RECEIVER_C::GetSubMapNo(void)
{
    return subMapNo_;
}

void  LOCATION_3D_MSG_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::locationInfo3D::ConstPtr &pstMsg)
{
    ros::Time nowT = ros::Time::now();
    ros::Duration d(nowT - lastT_);
    float intervalMs = d.toSec();
    ST_LOG_INFO_IF(intervalMs > 0.15, "接收定位花费时间s: %.3f, last接收s:%.3f, now接收s:%.3f", 
                    intervalMs, lastT_.toSec(), nowT.toSec());
    ST_LOG_INFO_IF(intervalMs > 0.15, "定位接收-发布花费时间s:%.3f, 发布时间戳s:%.3f", 
                    nowT.toSec() - pstMsg->header.stamp.toSec(), pstMsg->header.stamp.toSec());
    lastT_ = nowT;

    objInvalidTimer_.Reset();

    enState_  = (MAP_NODE_STATE_ENUM)pstMsg->status;
    subMapNo_ = pstMsg->subMapNo;
    stTsPose_.time = pstMsg->header.stamp.toSec();
    stTsPose_.stPose.stPos.x     = pstMsg->posX;
    stTsPose_.stPose.stPos.y     = pstMsg->posY;
    stTsPose_.stPose.stPos.z     = pstMsg->posZ;
    stTsPose_.stPose.stAtt.roll  = pstMsg->roll;
    stTsPose_.stPose.stAtt.pitch = pstMsg->pitch;
    stTsPose_.stPose.stAtt.yaw   = pstMsg->yaw;
}

void  POSE2D_SENDER_C::Send(const COOR_2D_POSE_STRU &stPose)
{
    comm_msg::pose2D stMsg;
    stMsg.posX = stPose.stPos.x;
    stMsg.posY = stPose.stPos.y;
    stMsg.yaw  = stPose.dirAngle;

    publisher_.publish(stMsg);
}

void  POSE_SCORE_SENDER_C::Send(const float score, const Matrix4f &mPose)
{
    comm_msg::pose_score stMsg;
    stMsg.score = score;

    COOR_3D_POSE_STRU stPose(mPose);
    stMsg.pose.posX  = stPose.x();
    stMsg.pose.posY  = stPose.y();
    stMsg.pose.posZ  = stPose.z();
    stMsg.pose.roll  = stPose.roll();
    stMsg.pose.pitch = stPose.pitch();
    stMsg.pose.yaw   = stPose.yaw();

    publisher_.publish(stMsg);
}

void  POSE_SCORE_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::pose_score::ConstPtr& pstMsg)
{
    objInvalidTimer_.Reset();
    stPoseScore_.score  = pstMsg->score;

    COOR_3D_POSE_STRU stPose(pstMsg->pose.posX, pstMsg->pose.posY,  pstMsg->pose.posZ,
                             pstMsg->pose.roll, pstMsg->pose.pitch, pstMsg->pose.yaw);
    stPoseScore_.mPose  = stPose.GetTransform();
}

INT32 POSE_SCORE_RECEIVER_C::Get(POSE_SCORE_STRU &stPoseScore)
{
    if(objInvalidTimer_.IsTimeOut()){
        return -1;
    }
    stPoseScore.Copy(stPoseScore_);
    objInvalidTimer_.Clear();

    return 0;
}


void  POLYGON_SENDER_C::Send(std::vector<Vector2f> &vertexes)
{
    geometry_msgs::PolygonStamped  stMsg;
    stMsg.header.frame_id = "/map";
    stMsg.header.stamp    = ros::Time::now();
    stMsg.polygon.points.clear();
    for(size_t i = 0; i < vertexes.size(); i++)
    {
        geometry_msgs::Point32 stPoint;
        stPoint.x = vertexes[i](0);
        stPoint.y = vertexes[i](1);
        stPoint.z = 0.0;
        stMsg.polygon.points.push_back(stPoint);
    }

    publisher_.publish(stMsg);
}


void  QRCODE_POSE_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::qrcode_pose::ConstPtr& pstMsg)
{
    objInvalidTimer_.Reset();
    stPoseId_.time = pstMsg->header.stamp.toSec();
    stPoseId_.id   = pstMsg->id;

    Quaternionf quat(pstMsg->pose.orientation.w, pstMsg->pose.orientation.x,
                     pstMsg->pose.orientation.y, pstMsg->pose.orientation.z);

    Vector3f pos = Vector3f(pstMsg->pose.position.x,
                            pstMsg->pose.position.y,
                            pstMsg->pose.position.z);
    Matrix4f mPose = Matrix4f::Identity();
    mPose.block<3,3>(0,0) = quat.toRotationMatrix();
    mPose.block<3,1>(0,3) = pos;

    stPoseId_.stPose.Copy(mPose);
}

INT32 QRCODE_POSE_RECEIVER_C::Get(POSE_ID_STRU &stPoseId)
{
    if(objInvalidTimer_.IsTimeOut()){
        return -1;
    }

    stPoseId = stPoseId_;
    //objInvalidTimer_.Clear();

    return 0;
}


INT32 ROP_MSG_RECEIVER_C::Get(float &rop)
{
    if (!isExist_)
        return -1;
    isExist_ = false;

    rop = rop_;
    return 0;
}

void  ROP_MSG_RECEIVER_C::ReceiveMsgCallBack(const std_msgs::Float32::ConstPtr &pstMsg)
{
    isExist_ = true;
    rop_     = pstMsg->data;
}


void  TF_SENDER_3D_C::SendTfMap2Base(const Matrix4f &mPose)
{
    geometry_msgs::TransformStamped stTfMsg;
    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "map";
    stTfMsg.child_frame_id  = "base_link";

    stTfMsg.transform.translation.x = mPose(0,3);
    stTfMsg.transform.translation.y = mPose(1,3);
    stTfMsg.transform.translation.z = mPose(2,3);

    Quaternionf quat(mPose.block<3,3>(0,0));
    stTfMsg.transform.rotation.x = quat.x();
    stTfMsg.transform.rotation.y = quat.y();
    stTfMsg.transform.rotation.z = quat.z();
    stTfMsg.transform.rotation.w = quat.w();
    broadcaster_.sendTransform(stTfMsg);
}

void  TF_SENDER_3D_C::SendTfOdom2Base(const Matrix4f &mPose)
{
    geometry_msgs::TransformStamped stTfMsg;
    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "odom";
    stTfMsg.child_frame_id  = "base_link";

    stTfMsg.transform.translation.x = mPose(0,3);
    stTfMsg.transform.translation.y = mPose(1,3);
    stTfMsg.transform.translation.z = mPose(2,3);

    Quaternionf quat(mPose.block<3,3>(0,0));
    stTfMsg.transform.rotation.x = quat.x();
    stTfMsg.transform.rotation.y = quat.y();
    stTfMsg.transform.rotation.z = quat.z();
    stTfMsg.transform.rotation.w = quat.w();
    broadcaster_.sendTransform(stTfMsg);
}

void  TF_SENDER_3D_C::SendTfMap2Odom(const Matrix4f &mPose)
{
    geometry_msgs::TransformStamped stTfMsg;
    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "map";
    stTfMsg.child_frame_id  = "odom";

    stTfMsg.transform.translation.x = mPose(0,3);
    stTfMsg.transform.translation.y = mPose(1,3);
    stTfMsg.transform.translation.z = mPose(2,3);

    Quaternionf quat(mPose.block<3,3>(0,0));
    stTfMsg.transform.rotation.x = quat.x();
    stTfMsg.transform.rotation.y = quat.y();
    stTfMsg.transform.rotation.z = quat.z();
    stTfMsg.transform.rotation.w = quat.w();
    broadcaster_.sendTransform(stTfMsg);
}

void  TF_SENDER_3D_C::SendTf(const string &strFatherFrameId, const string &strChildFrameId, const Matrix4f &mPose)
{
    geometry_msgs::TransformStamped stTfMsg;
    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = strFatherFrameId;
    stTfMsg.child_frame_id  = strChildFrameId;

    stTfMsg.transform.translation.x = mPose(0,3);
    stTfMsg.transform.translation.y = mPose(1,3);
    stTfMsg.transform.translation.z = mPose(2,3);

    Quaternionf quat(mPose.block<3,3>(0,0));
    stTfMsg.transform.rotation.x = quat.x();
    stTfMsg.transform.rotation.y = quat.y();
    stTfMsg.transform.rotation.z = quat.z();
    stTfMsg.transform.rotation.w = quat.w();
    broadcaster_.sendTransform(stTfMsg);
}

