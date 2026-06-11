
#ifndef __COMM_ROS_BASE_CPP__
#define __COMM_ROS_BASE_CPP__

#include "comm/comm_ros_base.h"
#include "comm/comm_show.h"
#include "comm/comm_base.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

void LOCATION_MSG_SENDER_C::Send(MAP_NODE_STATE_ENUM enState, int subMapNo, const COOR_2D_POSE_STRU &stPose)
{
    comm_msg::locationInfo stMsg;

    stMsg.status   = (INT32)enState;
    stMsg.subMapNo = subMapNo;
    stMsg.posX     = stPose.stPos.x;
    stMsg.posY     = stPose.stPos.y;
    stMsg.dirAngle = stPose.dirAngle;

    publisher_.publish(stMsg);
}

BOOL LOCATION_MSG_RECEIVER_C::IsPrepareReady(void) const
{
    return isPrepareReady_;
}

bool LOCATION_MSG_RECEIVER_C::IsNormal(void) const
{
    return !objDataValidTimer_.IsTimeOut() && (enState_ >= 0);
}

/**************************************************************************************
功能描述: 获取最新位置
          调用前需要先判断状态，否则获取的可能是错误的
修改记录:
**************************************************************************************/
void LOCATION_MSG_RECEIVER_C::Get(COOR_2D_POSE_STRU &stPose) const
{
    stPose.Copy(stPose_);
}

int LOCATION_MSG_RECEIVER_C::GetNormal(COOR_2D_POSE_STRU &stPose) const
{
    if (!IsNormal()) {
        return -1;
    }

    Get(stPose);
    return 0;
}

const COOR_2D_POSE_STRU *LOCATION_MSG_RECEIVER_C::GetNormal(void) const
{
    if (!IsNormal()) {
        return NULL;
    }

    return &stPose_;
}

void LOCATION_MSG_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::locationInfo::ConstPtr &pstMsg)
{
    isPrepareReady_ = true;

    objDataValidTimer_.Reset();

    enState_         = (MAP_NODE_STATE_ENUM)pstMsg->status;
    subMapNo_        = pstMsg->subMapNo;
    stPose_.stPos.x  = pstMsg->posX;
    stPose_.stPos.y  = pstMsg->posY;
    stPose_.dirAngle = pstMsg->dirAngle;
}

/**************************************************************************************
功能描述: 发送速度消息
修改记录:
**************************************************************************************/
void VEL_MSG_SENDER_C::Send(float leftWheelVel, float rightWheelVel)
{
    comm_msg::wheelSpeed stMsg;

    stMsg.leftwheel_speed  = leftWheelVel;
    stMsg.rightwheel_speed = rightWheelVel;

    publisher_.publish(stMsg);
}

int VEL_MSG_RECEIVER_C::Get(float &vel)
{
    if (objInvalidTimer_.IsTimeOut()) {
        return -1;
    }

    vel = vel_;
    return 0;
}

/**************************************************************************************
功能描述: 判断小车是否几乎停止了
          速度小于指定值就认为停止了
修改记录:
**************************************************************************************/
bool VEL_MSG_RECEIVER_C::IsAlmostStop(float stopVelLimit)
{
    return (!objInvalidTimer_.IsTimeOut() && (vel_ < stopVelLimit));
}

void VEL_MSG_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::wheelSpeed::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();

    vel_ = (pstMsg->leftwheel_speed + pstMsg->rightwheel_speed) * 0.5;
}

/**************************************************************************************
功能描述: 发送IMU数据
修改记录:
**************************************************************************************/
void IMU_DATA_MSG_SENDER_C::Send(float angleVel, float yawAngle, float accX, float accY, float accZ)
{
    comm_msg::imudata stMsg;

    stMsg.Temp = 25;

    stMsg.Ax   = accX;
    stMsg.Ay   = accY;
    stMsg.Az   = accZ;

    stMsg.Gx   = 0;
    stMsg.Gy   = 0;
    stMsg.Gz   = angleVel;

    stMsg.Yaw  = yawAngle;

    publisher_.publish(stMsg);
}

int IMU_DATA_MSG_RECEIVER_C::Get(float &angleVel, float &angleYaw)
{
    if (objInvalidTimer_.IsTimeOut()) {
        return -1;
    }

    angleVel = angleVel_;
    angleYaw = angleYaw_;
    return 0;
}

void IMU_DATA_MSG_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::imudata::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();

    angleVel_ = ANGLE_TO_RADIAN(pstMsg->Gz);
    angleYaw_ = ANGLE_TO_RADIAN(pstMsg->Yaw);
}

void ODOM_MSG_SENDER_C::Send(COOR_2D_POSE_STRU &stPose, float vel, float angleVel)
{
    nav_msgs::Odometry stOdomMsg;

    stOdomMsg.header.stamp    = ros::Time::now();
    stOdomMsg.header.frame_id = "odom";
    stOdomMsg.child_frame_id  = "base_footprint";

    stOdomMsg.pose.pose.position.x = stPose.stPos.x;
    stOdomMsg.pose.pose.position.y = stPose.stPos.y;
    stOdomMsg.pose.pose.position.z =  0;
    stOdomMsg.pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(0, 0, stPose.dirAngle);
    // Pose covariance
    for(int i = 0; i < 6; i++) {
        stOdomMsg.pose.covariance[i*6+i] = 0.1;
    }

    stOdomMsg.twist.twist.linear.x  = vel;
    stOdomMsg.twist.twist.linear.y  = 0.0;
    stOdomMsg.twist.twist.linear.z  = 0.0;
    stOdomMsg.twist.twist.angular.x = 0;
    stOdomMsg.twist.twist.angular.y = 0;
    stOdomMsg.twist.twist.angular.z = angleVel;
    // Twist covariance
    for(int i = 0; i < 6; i++) {
        stOdomMsg.twist.covariance[6*i+i] = 0.1;
    }

    publisher_.publish(stOdomMsg);
}

/**************************************************************************************
功能描述: 获取ODOM位姿
修改记录:
**************************************************************************************/
INT32 ODOM_MSG_RECEIVER_C::Get(COOR_2D_POSE_STRU &stPose)
{
    if (objInvalidTimer_.IsTimeOut()) {
        return -1;
    }

    stPose.Copy(stPose_);
    return 0;
}

/**************************************************************************************
功能描述: 接收ODOM话题消息处理
修改记录:
**************************************************************************************/
void ODOM_MSG_RECEIVER_C::ReceiveMsgCallBack(const nav_msgs::Odometry::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();

    stPose_.stPos.x  = pstMsg->pose.pose.position.x;
    stPose_.stPos.y  = pstMsg->pose.pose.position.y;
    stPose_.dirAngle = AdjustAngle(tf::getYaw(pstMsg->pose.pose.orientation));
}

void TF_SENDER_C::SendOdomToBaseFootTf(const COOR_2D_POSE_STRU &stBaseFootPoseByOdom)
{
    geometry_msgs::TransformStamped stTfMsg;

    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "odom";
    stTfMsg.child_frame_id  = "base_footprint";

    stTfMsg.transform.translation.x = stBaseFootPoseByOdom.stPos.x;
    stTfMsg.transform.translation.y = stBaseFootPoseByOdom.stPos.y;
    stTfMsg.transform.translation.z = 0;
    stTfMsg.transform.rotation      = tf::createQuaternionMsgFromYaw(stBaseFootPoseByOdom.dirAngle);

    objBroadcaster_.sendTransform(stTfMsg);
}

void TF_SENDER_C::SendMapToOdomTf(const COOR_2D_POSE_STRU &stOdomOriginPoseByMap)
{
    geometry_msgs::TransformStamped stTfMsg;

    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "map";
    stTfMsg.child_frame_id  = "odom";

    stTfMsg.transform.translation.x = stOdomOriginPoseByMap.stPos.x;
    stTfMsg.transform.translation.y = stOdomOriginPoseByMap.stPos.y;
    stTfMsg.transform.translation.z = 0;
    stTfMsg.transform.rotation      = tf::createQuaternionMsgFromYaw(stOdomOriginPoseByMap.dirAngle);

    objBroadcaster_.sendTransform(stTfMsg);
}

void TF_SENDER_C::SendBaseFootToBaseLinkTf(const COOR_2D_POSE_STRU &stBaseLinkPoseByFootPrint)
{
    geometry_msgs::TransformStamped stTfMsg;

    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "base_footprint";
    stTfMsg.child_frame_id  = "base_link";

    stTfMsg.transform.translation.x = stBaseLinkPoseByFootPrint.stPos.x;
    stTfMsg.transform.translation.y = stBaseLinkPoseByFootPrint.stPos.y;
    stTfMsg.transform.translation.z = 0;
    stTfMsg.transform.rotation      = tf::createQuaternionMsgFromYaw(stBaseLinkPoseByFootPrint.dirAngle);

    objBroadcaster_.sendTransform(stTfMsg);
}

void TF_SENDER_C::SendBaseLinkToScanTf(const COOR_2D_POSE_STRU &stScanPoseByBaseLink)
{
    geometry_msgs::TransformStamped stTfMsg;

    stTfMsg.header.stamp    = ros::Time::now();
    stTfMsg.header.frame_id = "base_link";
    stTfMsg.child_frame_id  = "scan";

    stTfMsg.transform.translation.x = stScanPoseByBaseLink.stPos.x;
    stTfMsg.transform.translation.y = stScanPoseByBaseLink.stPos.y;
    stTfMsg.transform.translation.z = 0;
    stTfMsg.transform.rotation      = tf::createQuaternionMsgFromYaw(stScanPoseByBaseLink.dirAngle);

    objBroadcaster_.sendTransform(stTfMsg);
}

/**************************************************************************************
功能描述: 发送GPS信息
修改记录:
**************************************************************************************/
void GPS_INFO_MSG_SENDER_C::Send(INT08 satelliteNum, INT08 rsltType, float hdop, double longitude, double latitude, float altitude, float yawAngle)
{
    comm_msg::gpsInfo stMsg;

    stMsg.header.stamp  = ros::Time::now();
    stMsg.satelliteNum  = satelliteNum;
    stMsg.rsltType      = rsltType;
    stMsg.longitude     = longitude;
    stMsg.latitude      = latitude;
    stMsg.altitude      = altitude;
    stMsg.yamAngle      = yawAngle;

    publisher_.publish(stMsg);
}

/**************************************************************************************
功能描述: 获取GPS信息
修改记录:
**************************************************************************************/
INT32 GPS_INFO_MSG_RECERVER_C::Get(GPS_INFO_STRU &stGpsInfo)
{
    if (objInvalidTimer_.IsTimeOut()) {
        return -1;
    }

    stGpsInfo.Copy(stGpsInfo_);

    /* 只能有效一次 */
    objInvalidTimer_.Clear();
    return 0;
}

/**************************************************************************************
功能描述: 接收GPS信息
修改记录:
**************************************************************************************/
void GPS_INFO_MSG_RECERVER_C::ReceiveMsgCallBack(const comm_msg::gpsInfo::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();

    stGpsInfo_.time                     = pstMsg->header.stamp.toSec();
    stGpsInfo_.satelliteNum             = pstMsg->satelliteNum;
    stGpsInfo_.rsltType                 = pstMsg->rsltType;
    stGpsInfo_.stPose.stCoord.lon       = pstMsg->longitude;
    stGpsInfo_.stPose.stCoord.lat       = pstMsg->latitude;
    stGpsInfo_.stPose.stCoord.alt       = pstMsg->altitude;
    stGpsInfo_.stPose.yawAngle          = pstMsg->yamAngle;
}

/**************************************************************************************
功能描述: 发送毫米波雷达信息
修改记录:
**************************************************************************************/
void RADAR_MSG_SENDER_C::Send(RADAR_INFO_STRU &stRadarInfo)
{
    comm_msg::radarTargetArray             msgTargetArray;
    comm_msg::radarTarget                  msgTarget;
    vector<POS_VEL_STRU>::iterator  iter;

    ros::Time time(stRadarInfo.timeStamp);

    msgTargetArray.header.stamp = time;
    msgTargetArray.radarID = stRadarInfo.radarID;

    for (iter = stRadarInfo.vstPosVel.begin(); iter != stRadarInfo.vstPosVel.end(); iter++) {
        msgTarget.targetDistx = iter->posX;
        msgTarget.targetDisty = iter->posY;
        msgTarget.targetVelx  = iter->velX ;
        msgTarget.targetVely  = iter->velY ;
        msgTargetArray.allTargets.push_back(msgTarget);
    }

    if (publisher_.getNumSubscribers() != 0) {
        publisher_.publish(msgTargetArray);
    }
}

const INT32 OCC_MAP_MSG_SENDER_C::aucCostToOcc_[256] =
{
    100, 200,  90, 200, 200, 200, 200, 200, 200, 200, /* 000 - 009 */  80, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 010 - 019 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 020 - 029 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 030 - 039 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 040 - 049 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 050 - 059 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 060 - 069 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 070 - 079 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 080 - 089 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 090 - 099 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 100 - 109 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 110 - 119 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 120 - 129 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 130 - 139 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 140 - 149 */  40, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 150 - 159 */
     40, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 160 - 169 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 170 - 179 */
     20, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 180 - 189 */  70, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 190 - 199 */
    200, 200, 200, 200, 200,  -1, 200, 200, 200, 200, /* 200 - 209 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 210 - 219 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 220 - 229 */ 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 230 - 239 */
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /* 240 - 249 */ 200, 200, 200,  10,   0, 200                      /* 250 - 255 */
};

UINT32 OCC_MAP_MSG_SENDER_C::GetSubscriberNum(void) const
{
    return UINT32(publisher_.getNumSubscribers());
}

BOOL OCC_MAP_MSG_SENDER_C::IsExistSubscriber(void) const
{
    return (publisher_.getNumSubscribers() > 0);
}

/**************************************************************************************
功能描述: 发布地图数据
修改记录:
**************************************************************************************/
void OCC_MAP_MSG_SENDER_C::SendMsg(COORDINATE_2D_STRU &stOriginPos, float resolution, UINT32 sizeX, UINT32 sizeY, UINT08 *pucMap) const
{
    R_ASSERT(pucMap != NULL, NOTHING);
    if (!isMsgBuffered_ && !IsExistSubscriber()) {
        return;
    }

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
        stMapMsg.data[loop] = aucCostToOcc_[pucMap[loop]];
    }

    publisher_.publish(stMapMsg);
}

void OCC_MAP_MSG_SENDER_C::SendMsg(const GRAY_MAP_C &objMap)
{
    if (!isMsgBuffered_ && !IsExistSubscriber()) {
        return;
    }

    COORDINATE_2D_STRU stMapOriginPos;
    float              resolution;
    UINT32             mapSizeX;
    UINT32             mapSizeY;
    UINT08            *pucMap = NULL;
    R_ASSERT(0 == objMap.GetMapInfo(stMapOriginPos, resolution, mapSizeX, mapSizeY, &pucMap), NOTHING);

    SendMsg(stMapOriginPos, resolution, mapSizeX, mapSizeY, pucMap);
}

void OCC_MAP_MSG_SENDER_C::Clear(void)
{
    /* 清除之前的显示时，只是使用默认的将之前的覆盖掉 */
    GRAY_MAP_C objNullMap;
    objNullMap.CreateMap(1, 1, objNullMap.GetCostOfNull());

    SendMsg(objNullMap);
}

const GRAY_MAP_C &OCC_MAP_MSG_RECEIVER_C::Get(void) const
{
    return objMap_;
}

void OCC_MAP_MSG_RECEIVER_C::ReceiveMsgCallBack(const nav_msgs::OccupancyGrid::ConstPtr &pstMsg)
{
    COORDINATE_2D_STRU stOriginPos;
    UINT32             mapSizeX;
    UINT32             mapSizeY;
    float              resolution;
    stOriginPos.x = pstMsg->info.origin.position.x;
    stOriginPos.y = pstMsg->info.origin.position.y;
    mapSizeX      = pstMsg->info.width;
    mapSizeY      = pstMsg->info.height;
    resolution    = pstMsg->info.resolution;
    R_ASSERT_3(pstMsg->data.size() == (mapSizeX * mapSizeY), NOTHING, UINT32(pstMsg->data.size()), mapSizeX, mapSizeY);

    objMap_.CreateMap(stOriginPos, mapSizeX, mapSizeY, true, resolution);

    UINT32  mapSizeXNew;
    UINT32  mapSizeYNew;
    UINT08 *pucMap = NULL;
    INT32 rslt = objMap_.GetMapInfo(stOriginPos, resolution, mapSizeXNew, mapSizeYNew, &pucMap);
    if ((rslt != 0) || (mapSizeXNew != mapSizeX) || (mapSizeYNew != mapSizeY) || (pucMap == NULL)) {
        ST_LOG_ERR("GetMapInfo Rslt = %u, mapSize(%u, %u), mapSizeNew(%u, %u), pucMap = 0x%p.", rslt, mapSizeX, mapSizeY, mapSizeXNew, mapSizeYNew, pucMap);
        objMap_.DeleteMap();
        return;
    }

    for (UINT32 loop = 0; loop < UINT32(pstMsg->data.size()); ++loop) {
        switch (pstMsg->data[loop]) {
            case 100:       /* 障碍，待优化 */
                pucMap[loop] = 0;
                break;

            case 0:         /* 空白，待优化 */
                pucMap[loop] = 254;
                break;

            case -1:        /* 未知，待优化 */
                pucMap[loop] = 205;
                break;

            default:        /* 异常 */
                pucMap[loop] = 205;
                break;
        }
    }
}

void ROS_POSE_SENDER_C::SendMsg(const COOR_2D_POSE_STRU &stPose)
{
    geometry_msgs::PoseStamped stMsg;

    stMsg.header.stamp    = ros::Time::now();
    stMsg.header.frame_id = "map";

    stMsg.pose.position.x = stPose.stPos.x;
    stMsg.pose.position.y = stPose.stPos.y;
    stMsg.pose.position.z = 0.0;

    stMsg.pose.orientation = tf::createQuaternionMsgFromYaw(stPose.dirAngle);

    publisher_.publish(stMsg);
}

BOOL ROS_POLYGON_SENDER_C::IsEixstSubscriber(void)
{
    return (publisher_.getNumSubscribers() > 0);
}

void ROS_POLYGON_SENDER_C::SendMultiSideShapeMsg(const vector<COORDINATE_2D_STRU> &vctPos)
{
    geometry_msgs::PolygonStamped stMsg;

    stMsg.header.stamp    = ros::Time::now();
    stMsg.header.frame_id = "map";

    for (UINT32 loop = 0; loop < UINT32(vctPos.size()); ++loop) {
        const COORDINATE_2D_STRU &stPos = vctPos[loop];

        geometry_msgs::Point32 stMsgPoint;
        stMsgPoint.x = stPos.x;
        stMsgPoint.y = stPos.y;
        stMsgPoint.z = 0.0;
        stMsg.polygon.points.push_back(stMsgPoint);
    }

    publisher_.publish(stMsg);
    isShowing_ = true;
}

void ROS_POLYGON_SENDER_C::Send4SideShapeMsg(const COORDINATE_2D_STRU &stP1, const COORDINATE_2D_STRU &stP2, const COORDINATE_2D_STRU &stP3, const COORDINATE_2D_STRU &stP4)
{
    geometry_msgs::PolygonStamped stMsg;

    stMsg.header.stamp    = ros::Time::now();
    stMsg.header.frame_id = "map";

    geometry_msgs::Point32 stPoint;

    stPoint.x = stP1.x;
    stPoint.y = stP1.y;
    stPoint.z = 0.0;
    stMsg.polygon.points.push_back(stPoint);

    stPoint.x = stP2.x;
    stPoint.y = stP2.y;
    stPoint.z = 0.0;
    stMsg.polygon.points.push_back(stPoint);

    stPoint.x = stP3.x;
    stPoint.y = stP3.y;
    stPoint.z = 0.0;
    stMsg.polygon.points.push_back(stPoint);

    stPoint.x = stP4.x;
    stPoint.y = stP4.y;
    stPoint.z = 0.0;
    stMsg.polygon.points.push_back(stPoint);

    publisher_.publish(stMsg);
    isShowing_ = true;
}

void ROS_POLYGON_SENDER_C::Clear(void)
{
    if (!isShowing_) {
        return;
    }

    geometry_msgs::PolygonStamped stMsg;
    stMsg.header.stamp    = ros::Time::now();
    stMsg.header.frame_id = "map";
    publisher_.publish(stMsg);

    isShowing_ = false;
}

const AREA_C &ROS_AREA_RECEIVER_C::Get(void)
{
    return objArea_;
}

void ROS_AREA_RECEIVER_C::ReceiveMsgCallBack(const geometry_msgs::PolygonStamped::ConstPtr &pstMsg)
{
    objArea_.Reset();

    R_ASSERT_1(pstMsg->polygon.points.size() >= 2, NOTHING, pstMsg->polygon.points.size());

    COORDINATE_2D_STRU stLBPoint;
    stLBPoint.x = pstMsg->polygon.points[0].x;
    stLBPoint.y = pstMsg->polygon.points[0].y;
    objArea_.IncludePoint(stLBPoint);

    COORDINATE_2D_STRU stRTPoint;
    stRTPoint.x = pstMsg->polygon.points[1].x;
    stRTPoint.y = pstMsg->polygon.points[1].y;
    objArea_.IncludePoint(stRTPoint);
}

BOOL PATH_SENDER_C::IsExistSubscriber(void)
{
    return (publisher_.getNumSubscribers() > 0);
}

void PATH_SENDER_C::Send(const vector<COORDINATE_2D_STRU> &vctRoute)
{
    if (!IsExistSubscriber()) {
        return;
    }

    nav_msgs::Path stPathMsg;
    stPathMsg.header.stamp    = ros::Time::now();
    stPathMsg.header.frame_id = "map";

    geometry_msgs::PoseStamped stPathPoint;
    stPathPoint.header.stamp    = ros::Time::now();
    stPathPoint.header.frame_id = "map";

    /* 此处发布的路径仅用于显示，不含有准确的时间和方向信息 */
    for (vector<COORDINATE_2D_STRU>::const_iterator iter = vctRoute.begin();
        iter != vctRoute.end();
        ++iter) {
        const COORDINATE_2D_STRU &stPoint = *iter;

        stPathPoint.pose.position.x  = stPoint.x;
        stPathPoint.pose.position.y  = stPoint.y;
        stPathPoint.pose.position.z  = 0;
        stPathPoint.pose.orientation = tf::createQuaternionMsgFromYaw(0.0);

        stPathMsg.poses.push_back(stPathPoint);
    }

    publisher_.publish(stPathMsg);
}

void PATH_SENDER_C::Send(const list<COORDINATE_2D_STRU> &listRoute)
{
    if (!IsExistSubscriber()) {
        return;
    }

    nav_msgs::Path stPathMsg;
    stPathMsg.header.stamp    = ros::Time::now();
    stPathMsg.header.frame_id = "map";

    geometry_msgs::PoseStamped stPathPoint;
    stPathPoint.header.stamp    = ros::Time::now();
    stPathPoint.header.frame_id = "map";

    /* 此处发布的路径仅用于显示，不含有准确的时间和方向信息 */
    for (list<COORDINATE_2D_STRU>::const_iterator iter = listRoute.begin();
        iter != listRoute.end();
        ++iter) {
        const COORDINATE_2D_STRU &stPoint = *iter;

        stPathPoint.pose.position.x  = stPoint.x;
        stPathPoint.pose.position.y  = stPoint.y;
        stPathPoint.pose.position.z  =  0;
        stPathPoint.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(0, 0, 0.0);

        stPathMsg.poses.push_back(stPathPoint);
    }

    publisher_.publish(stPathMsg);
}

const vector<COORDINATE_2D_STRU> &PATH_RECEIVER_C::Get(void)
{
    if (objInvalidTimer_.IsTimeOut()) {
        vectPath_.clear();
    }

    return vectPath_;
}

void PATH_RECEIVER_C::ReceiveMsgCallBack(const nav_msgs::Path::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();

    UINT32 pathSize = UINT32(pstMsg->poses.size());
    vectPath_.clear();
    vectPath_.reserve(pathSize);

    for (UINT32 loop = 0; loop < pathSize; ++loop) {
        const geometry_msgs::PoseStamped &stMsgPoint = pstMsg->poses[loop];

        COORDINATE_2D_STRU stPoint;
        stPoint.x = stMsgPoint.pose.position.x;
        stPoint.y = stMsgPoint.pose.position.y;
        vectPath_.push_back(stPoint);
    }
}

void DSP_TEXT_SENDER_C::Send(const COORDINATE_2D_STRU &stPos, const string &strDsp)
{
    visualization_msgs::Marker marker;

    marker.header.frame_id="map";
    marker.header.stamp = ros::Time::now();
    marker.ns = "basic_shapes";

    marker.id     = 0;
    marker.type   = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x    = stPos.x;
    marker.pose.position.y    = stPos.y;
    marker.pose.position.z    = 0.0;        /* 悬浮高度 */
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    marker.text = strDsp;

    marker.scale.x = 1.0;           /* 无效 */
    marker.scale.y = 1.0;           /* 无效 */
    marker.scale.z = textHeight_;   /* 字体大小 */

    marker.color.b = GET_PRI_COLOR_B(SHOW_COLOR_RED);
    marker.color.g = GET_PRI_COLOR_G(SHOW_COLOR_RED);
    marker.color.r = GET_PRI_COLOR_R(SHOW_COLOR_RED);
    marker.color.a = 0.5;

    publisher_.publish(marker);
}

#endif


