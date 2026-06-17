#include "pcpt_comm/pcpt_comm_ros.h"

/**************************************************************************************
功能描述: sensor_msgs::PointCloud2 转 pcl::PCLPointCloud2
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::MsgRosToMsgPcl(const sensor_msgs::PointCloud2 &inPut, pcl::PCLPointCloud2 &outPut)
{
    pcl_conversions::toPCL(inPut, outPut);
}

/**************************************************************************************
功能描述:sensor_msgs::PointCloud2 转 pcl::PointCloud< PointT>
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::MsgRosToMsgPcl(const sensor_msgs::PointCloud2 &inPut, pcl::PointCloud<PointXYZI> &outPut)
{
    pcl::fromROSMsg(inPut, outPut);
}

/**************************************************************************************
功能描述: pcl::PCLPointCloud2 转 sensor_msgs::PointCloud2
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::MsgPclToMgRos(const pcl::PCLPointCloud2& inPut, sensor_msgs::PointCloud2 &outPut)
{
    pcl_conversions::fromPCL(inPut, outPut);
}

/**************************************************************************************
功能描述: pcl::PCLPointCloud2 转 pcl::PointCloud< PointT>
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::MsgPclToMsgPcl(const pcl::PCLPointCloud2 &inPut, pcl::PointCloud<PointXYZI> &outPut)
{
    pcl::fromPCLPointCloud2(inPut, outPut);
}

/**************************************************************************************
功能描述: pcl::PointCloud<PointT> 转 snesor_msgs::PointCloud2
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::MsgPclToMsgRos(const pcl::PointCloud<PointXYZI> &inPut, sensor_msgs::PointCloud2 &outPut)
{
    pcl::toROSMsg(inPut, outPut);
}

/**************************************************************************************
功能描述: pcl::PointCloud< PointT> 转 pcl::PCLPointCloud2
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::MsgPclToMsgPcl(const pcl::PointCloud<PointXYZI> &inPut, pcl::PCLPointCloud2 &outPut)
{
    pcl::toPCLPointCloud2(inPut, outPut);
}

/**************************************************************************************
功能描述: sensor_msgs::PointCloud -> sensor_msgs::PointCloud2
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::ConvertPointCloudToPointCloud2(const sensor_msgs::PointCloud &inPut, sensor_msgs::PointCloud2 &outPut)
{
    sensor_msgs::convertPointCloudToPointCloud2(inPut, outPut);
}

/**************************************************************************************
功能描述: sensor_msgs::PointCloud2 -> sensor_msgs::PointCloud
修改记录:
**************************************************************************************/
void POINT_CLOUD_MSG_TRANSFORM_C::ConvertPointCloud2ToPointCloud(const sensor_msgs::PointCloud2 &inPut, sensor_msgs::PointCloud &outPut)
{
    sensor_msgs::convertPointCloud2ToPointCloud(inPut, outPut);
}

/**************************************************************************************
功能描述: 2D坐标数组发布
修改记录:
**************************************************************************************/
void COOR_2D_ARRAY_MSG_SENDER_C::Send(comm_msg::coor2DArray &coor2DArray) const
{
    if (!publisher_.getNumSubscribers()) return;
    publisher_.publish(coor2DArray);
}

/**************************************************************************************
功能描述: 带类别标签的2D坐标数组发布器
修改记录:
**************************************************************************************/
void COOR_2D_ARRAY_LA_MSG_SENDER_C::Send(comm_msg::coor2DArrayLA &coor2DArrayLA) const
{
    if (!publisher_.getNumSubscribers()) return;
    publisher_.publish(coor2DArrayLA);
}

/**************************************************************************************
功能描述: 坐标转点云
修改记录:
**************************************************************************************/
int FUSED_STATIC_OBST_SENDER_C::Coor2Msg(const PCPT_OBST_STRU& stObstByMap, comm_msg::coor2DArray &coor2DArray)
{
    if (stObstByMap.empty()) return 0;

    PCPT_TF_C objTf(COOR_2D_POSE_STRU(stObstByMap.x(), stObstByMap.y(), stObstByMap.yaw()));
    vector<OBST_LAYER_ENUM> vLayer = stObstByMap.Layers();

    vector<comm_msg::coor2D>* pvCoor2D = nullptr;
    for (const auto& pair : stObstByMap.umObst) {
        OBST_LABEL_ENUM label                   = pair.first;
        const MULTILAYER_OBST_STRU& stLayerObst = pair.second;
        for (const OBST_LAYER_ENUM& layer : vLayer) {
            if      (layer == OBST_LAYER_TOP)  pvCoor2D = &coor2DArray.coor2DArrayTop;
            else if (layer == OBST_LAYER_MID)  pvCoor2D = &coor2DArray.coor2DArrayMid;
            else if (layer == OBST_LAYER_DOWN) pvCoor2D = &coor2DArray.coor2DArrayDown;
            else continue;

            const LST_COOR* plstCoor = stLayerObst.LayerObst(layer);
            for (const OBST_STATE_STRU &stObstState : *plstCoor) {
                /* obst to coor msg */
                comm_msg::coor2D coor2D;
                coor2D.label.value = int(label);
                coor2D.pos_x       = stObstState.x();
                coor2D.pos_y       = stObstState.y();
                pvCoor2D->push_back(coor2D);

                /* point cloud by car view in rviz */
                if (objpCloudSenderByCar_.GetNumSubscribers() ||
                    objpCloudSenderByCarStatic_.GetNumSubscribers() ||
                    objpCloudSenderByCarDynamic_.GetNumSubscribers()) {
                    COORDINATE_2D_STRU stPosByCar;
                    objTf.TransFatherPosToSon(stObstState.stPos, stPosByCar);
                    pcl::PointXYZ point;
                    point.x = stPosByCar.x;
                    point.y = stPosByCar.y;
                    point.z = 0;
                    pCloudByCar_->points.push_back(point);
                    if (objpCloudSenderByCarStatic_.GetNumSubscribers() || objpCloudSenderByCarDynamic_.GetNumSubscribers()) {
                        if      (label == OBST_LABEL_STATIC || label == OBST_LABEL_UNKNOWN) pCloudByCarStatic_->points.push_back(point);
                        else if (label == OBST_LABEL_DYNAMIC)                               pCloudByCarDynamic_->points.push_back(point);
                    }
                }

                /* point cloud by map view in rviz */
                if (objpCloudSenderByMap_.GetNumSubscribers() ||
                    objpCloudSenderByMapStatic_.GetNumSubscribers() ||
                    objpCloudSenderByMapDynamic_.GetNumSubscribers()) {
                    pcl::PointXYZ point;
                    point.x = stObstState.x();
                    point.y = stObstState.y();
                    point.z = 0;
                    pCloudByMap_->points.push_back(point);
                    if (objpCloudSenderByMapStatic_.GetNumSubscribers() || objpCloudSenderByMapDynamic_.GetNumSubscribers()) {
                        if      (label == OBST_LABEL_STATIC || label == OBST_LABEL_UNKNOWN) pCloudByMapStatic_->points.push_back(point);
                        else if (label == OBST_LABEL_DYNAMIC)                               pCloudByMapDynamic_->points.push_back(point);
                    }
                }
            }
        }

    }

    /* 兼容旧消息，待删除 */
    coor2DArray.coor2DArray.insert(coor2DArray.coor2DArray.end(), coor2DArray.coor2DArrayMid.begin(), coor2DArray.coor2DArrayMid.end());
    coor2DArray.coor2DArray.insert(coor2DArray.coor2DArray.end(), coor2DArray.coor2DArrayDown.begin(),coor2DArray.coor2DArrayDown.end());

    return 0;
}

/**************************************************************************************
功能描述: 发布结果
修改记录:
**************************************************************************************/
void FUSED_STATIC_OBST_SENDER_C::Send(PCPT_OBST_STRU& stObstByMap)
{
    coor2DArray_.header.frame_id = PCPT_FRAME_ID_MAP;
    coor2DArray_.header.stamp = RosTime(stObstByMap.stamp);
    coor2DArray_.coor2DArray.clear();
    coor2DArray_.coor2DArrayTop.clear();
    coor2DArray_.coor2DArrayMid.clear();
    coor2DArray_.coor2DArrayDown.clear();

    /* 根据运动趋势，设置包含的障碍数据标志 */
    switch (g_move_trend) {
        case PLAN_MOVE_TREND_FORWARD:
            coor2DArray_.isContainForwardObst  = 1;
            coor2DArray_.isContainBackwardObst = 0;
            break;
        case PLAN_MOVE_TREND_BACKWARD:
            coor2DArray_.isContainForwardObst  = 0;
            coor2DArray_.isContainBackwardObst = 1;
            break;
        case PLAN_MOVE_TREND_FREE:
            coor2DArray_.isContainForwardObst  = 1;
            coor2DArray_.isContainBackwardObst = 1;
            break;
        case PLAN_MOVE_TREND_STOP:
        default:
            coor2DArray_.isContainForwardObst  = 0;
            coor2DArray_.isContainBackwardObst = 0;
            break;
    }

    pCloudByCar_->clear();
    pCloudByCarStatic_->clear();
    pCloudByCarDynamic_->clear();

    pCloudByMap_->clear();
    pCloudByMapStatic_->clear();
    pCloudByMapDynamic_->clear();

    if (Coor2Msg(stObstByMap, coor2DArray_)) return;

    /* pub */
    objCoor2DArrayMsgSender_.Send(coor2DArray_);
    objpCloudSenderByMap_.Send(       pCloudByMap_,        PCPT_FRAME_ID_MAP);
    objpCloudSenderByMapStatic_.Send( pCloudByMapStatic_,  PCPT_FRAME_ID_MAP);
    objpCloudSenderByMapDynamic_.Send(pCloudByMapDynamic_, PCPT_FRAME_ID_MAP);

    objpCloudSenderByCar_.Send(       pCloudByCar_,        PCPT_FRAME_ID_CAR);
    objpCloudSenderByCarStatic_.Send( pCloudByCarStatic_,  PCPT_FRAME_ID_CAR);
    objpCloudSenderByCarDynamic_.Send(pCloudByCarDynamic_, PCPT_FRAME_ID_CAR);
}

/**
 * @brief 发布带类别标签的2D障碍
 * @param vstStampPosArrayByMap
 */
void FUSED_STATIC_OBST_SENDER_C::Send(const vector<VISION_USAGE_STRU>& vstUsageByMap)
{
    comm_msg::coor2DArrayLA coor2DArrayLA;
    for (const VISION_USAGE_STRU& stUsage: vstUsageByMap) {
        //if (!stUsage.size()) continue;
        comm_msg::coor2DArrayL coor2DArrayL;
        coor2DArrayL.header.stamp    = RosTime(stUsage.stamp);
        coor2DArrayL.header.frame_id = PCPT_FRAME_ID_MAP;
        coor2DArrayL.label.value     = uint8_t(stUsage.label);
        coor2DArrayL.id              = stUsage.id;
        coor2DArrayL.coor2DArray.reserve(stUsage.size());
        for (const COORDINATE_2D_STRU& stCoor : stUsage.data) {
            comm_msg::coor2D msgCoor;
            msgCoor.pos_x = stCoor.x;
            msgCoor.pos_y = stCoor.y;
            coor2DArrayL.coor2DArray.push_back(msgCoor);
        }
        coor2DArrayLA.coor2DArrayLA.push_back(coor2DArrayL);
    }

    if (hasRisk_ != g_v_has_risk) hasRisk_ = g_v_has_risk;
    ST_LOG_INFO_SAMPLE_IF_TRUE(hasRisk_, PCPT_HZ_MAIN_FUN, "当前图像视野存在潜在风险，需要规控减速确认。");

    coor2DArrayLA.hasRisk = hasRisk_;
    objCoor2DArrayLAMsgSender_.Send(coor2DArrayLA);
}

/**************************************************************************************
功能描述: 发布统计
修改记录:
**************************************************************************************/
void FUSED_STATIC_OBST_SENDER_C::DelayStatistics(const double obstStamp)
{
    /* 缓存数据 */
    static UINT64 lidarStampInt = 0;
    vstStamp_.push_back(PUB_STAMP_STRU(obstStamp, RosStampNow()));
    if (lidarStampInt == UINT64(obstStamp)) return;

#if 1   //打印数据时间戳
    std::stringstream ss;
    ss << to_string(UINT64(obstStamp)) << '(' << vstStamp_.size() << ')';
    ss << ": ";
    for (const PUB_STAMP_STRU& stStamp : vstStamp_) {
        ss << '(' << DBL_DECIMAL(float(stStamp.obstStamp - UINT64(stStamp.obstStamp)), 3);
        ss << ' ' << DBL_DECIMAL(float(stStamp.pubStamp   - UINT64(stStamp.pubStamp)),   3) << ')';
    }
    ST_LOG_INFO("%s:", ss.str().c_str());
#endif

#if 1   //延时统计
    double min = DBL_MAX;
    double max = -DBL_MAX;
    double avg = 0;
    for (const PUB_STAMP_STRU& stStamp : vstStamp_) {
        double delayTime = stStamp.DelayTime();
        min = MIN(min, delayTime);
        max = MAX(max, delayTime);
        avg += delayTime;
    }
    avg /= vstStamp_.size();
    ST_LOG_INFO("基于激光雷达的障碍数据延时统计：最小(%.3f) 最大(%.3f) 平均(%.3f)", min, max, avg);
#endif

    /* 置位 */
    lidarStampInt = UINT64(obstStamp);
    vstStamp_.clear();
}

// 判断数据是否异常
BOOL LIDAR_SCAN_RECEIVER_C::IsDataInvalid() const
{
    return objDataInvalidTimer_.IsTimeOut();
}

// 判断传感器是否故障
BOOL LIDAR_SCAN_RECEIVER_C::IsSensorFault() const
{
    return objSensorFaultTimer_.IsTimeOut();
}

// 获取有效scan数据
int LIDAR_SCAN_RECEIVER_C::GetNormalData(sensor_msgs::LaserScan::Ptr &ptrScan)
{
    /* 当数据超时时，返回-1 */
    if (objDataInvalidTimer_.IsTimeOut())
        return -1;

    std::unique_lock<std::recursive_mutex> lock(cacheMutex_);

    /* 当没有新数据时，返回1，减少数据拷贝 */
    if (!hasNewData_)
        return 1;

    swap(ptrScan, pScanCache_);
    hasNewData_ = false;
    return 0;
}

// ROS话题回调: scan数据
void LIDAR_SCAN_RECEIVER_C::MsgReceiveCallBack(const sensor_msgs::LaserScan::ConstPtr &pstMsg)
{
    if ((pstMsg != nullptr) && (pstMsg->ranges.size() > 0))
    {
        {
            std::unique_lock<std::recursive_mutex> lock(cacheMutex_);
            if (pScanCache_ != nullptr)
            {
                *pScanCache_ = *pstMsg;
            }
            hasNewData_ = true;
        }

        objDataInvalidTimer_.Reset();
        objSensorFaultTimer_.Reset();
    }
}

/**************************************************************************************
功能描述: 判断是否异常
修改记录:
**************************************************************************************/
BOOL LIDAR_DATA_RECEIVER_C::IsDataInvalid()
{
    return IsAbnormal();
}

/**************************************************************************************
功能描述: 判断是否故障
修改记录:
**************************************************************************************/
BOOL LIDAR_DATA_RECEIVER_C::IsSensorFault() const
{
    return objSensorFaultTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 获取点云
修改记录:
**************************************************************************************/
int LIDAR_DATA_RECEIVER_C::GetNormalPointsCloud(PointCloudXYZIRCDT::Ptr &ptrCloud)
{
    /* 当数据超时时，返回-1 */
    if (IsDataInvalid()) return -1;

    /* 当没有新数据时，返回1，减少数据拷贝 */
    if (!hasNewData_) return 1;

    /* 数据实时性判断 */
    if (!IsTimelinessOk(RosStamp(msg_.header))) return -1;

    if (msg_.point_step == 32) {
        pcl::fromROSMsg(msg_, *pCloudXYZI_);
        g_pointType = TYPE_OLD;
    } else {
        pcl::fromROSMsg(msg_, *pCloudXYZIRCDT_);
        g_pointType = TYPE_NEW;
    }

    /* 若点云为新类型，则直接拷贝；否则，转为新类型再输出 */
    if (g_pointType == TYPE_NEW) {
        swap(ptrCloud, pCloudXYZIRCDT_);
    } else if (g_pointType == TYPE_OLD) {
        ptrCloud->clear();
        ptrCloud->header = pCloudXYZI_->header;
        for (auto &point : pCloudXYZI_->points) {
            PointXYZIRCDT pointT;
            pointT.x = point.x;
            pointT.y = point.y;
            pointT.z = point.z;
            pointT.ring = uint16_t(point.intensity);
            pointT.intensity = uint8_t((point.intensity - pointT.ring) * 1000);
            ptrCloud->push_back(pointT);
        }
    }
    hasNewData_ = false;

    return 0;
}

/**************************************************************************************
功能描述: 雷达点云接收
修改记录:
**************************************************************************************/
void LIDAR_DATA_RECEIVER_C::MsgReceiveCallBack(const sensor_msgs::PointCloud2ConstPtr &ptrMsg)
{
    if (ptrMsg == nullptr || ptrMsg->data.empty()) return;

    msg_ = *ptrMsg;
    hasNewData_ = true;
    TimerReset();
    objSensorFaultTimer_.Reset();
}

/**************************************************************************************
功能描述: 雷达数据实时性是否正常
修改记录:
**************************************************************************************/
bool LIDAR_DATA_RECEIVER_C::IsTimelinessOk(const double msgTime)
{
    if (g_isSim) return true;
    if (SIG(msgTime) == 0) return true;

    double curTime = RosStampNow();
    double difTime = fabs(curTime - msgTime);
    float  refTime = 0.5;

    if (difTime > refTime) {
        if (SIG(abMsgTime_ - msgTime) != 0) {       //避免重复打印同一数据的超时日志
            ST_LOG_ERR("话题[%s]实时性异常, 当前时间[%.2f], 话题时间[%.2f], 时间差[%.2f(ref:%.2f)].",
                strTopicName_.c_str(), curTime, msgTime, difTime, refTime);
            abMsgTime_ = msgTime;
        }
        return false;
    }
    return true;
}

/**************************************************************************************
功能描述: 判断是否异常
修改记录:
**************************************************************************************/
BOOL RADAR_DATA_RECEIVER_C::IsDataInvalid() const
{
    return objDataInvalidTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 判断是否故障
修改记录:
**************************************************************************************/
BOOL RADAR_DATA_RECEIVER_C::IsSensorFault() const
{
    return objSensorFaultTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 获取毫米波雷达最新消息内容
修改记录:
**************************************************************************************/
const RADAR_INFO_STRU *RADAR_DATA_RECEIVER_C::Get(void)
{
    if (objDataInvalidTimer_.IsTimeOut()) {
        return NULL;
    }

    return &stRadarInfo_;
}

/**************************************************************************************
功能描述: 毫米波雷达信息
修改记录:
**************************************************************************************/
void RADAR_DATA_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::radarTargetArray::ConstPtr &ptrMsg)
{
    if (!ptrMsg) return;
    TimerReset();

    stRadarInfo_.radarID = ptrMsg->radarID;
    stRadarInfo_.timeStamp = RosStamp(ptrMsg);
    stRadarInfo_.vstPosVel.clear();

    vector<comm_msg::radarTarget>::const_iterator iter;
    POS_VEL_STRU stPosVel;
    for (iter = ptrMsg->allTargets.begin(); iter != ptrMsg->allTargets.end(); ++iter) {
        stPosVel.posX = iter->targetDistx;
        stPosVel.posY = iter->targetDisty;
        stPosVel.velX = iter->targetVelx ;
        stPosVel.velY = iter->targetVely ;
        stRadarInfo_.vstPosVel.push_back(stPosVel);
    }
}

/**************************************************************************************
功能描述: 定时器复位
修改记录:
**************************************************************************************/
void RADAR_DATA_RECEIVER_C::TimerReset()
{
    objDataInvalidTimer_.Reset();
    objSensorFaultTimer_.Reset();
}

/**************************************************************************************
功能描述: 深度相机数据接收
修改记录:
**************************************************************************************/
void DEPTH_CAMERA_POINTS1_RECEIVER_C::MsgReceiveCallBack(const sensor_msgs::PointCloud::ConstPtr &ptrMsg)
{
    if (!ptrMsg || !ptrMsg->points.size()) return;
    sensor_msgs::convertPointCloudToPointCloud2 (*ptrMsg, objPoints2_);
    std::unique_lock<std::recursive_mutex> lock(cacheMutex_);
    pcl::fromROSMsg(objPoints2_, *ptrCloud_);
    objPoints_ = *ptrMsg;
    TimerReset();
}

/**************************************************************************************
功能描述: 判断是否超时
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_POINTS1_RECEIVER_C::IsDataInvalid() const
{
    return objDataInvalidTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 传感器数据超时复位状态
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_POINTS1_RECEIVER_C::IsSensorReset() const
{
    return objSensorResetTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 是否有故障
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_POINTS1_RECEIVER_C::IsSensorFault() const
{
    return objSensorFaultTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 复位
修改记录:
**************************************************************************************/
void DEPTH_CAMERA_POINTS1_RECEIVER_C::TimerReset()
{
    objDataInvalidTimer_.Reset();
    objSensorResetTimer_.Reset();
    objSensorFaultTimer_.Reset();
}

/**************************************************************************************
功能描述: 获取普通点云
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_POINTS1_RECEIVER_C::GetNormalPointsCloud(pcl::PointCloud<pcl::PointXYZI>::Ptr &ptrCloud)
{
    if (objDataInvalidTimer_.IsTimeOut()) {
        ptrCloud->clear();
        return false;
    }

    std::unique_lock<std::recursive_mutex> lock(cacheMutex_);
    *ptrCloud = *ptrCloud_;
    return true;
}

/**************************************************************************************
功能描述: 获取普通点云
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_POINTS1_RECEIVER_C::GetNormalPointsCloud(sensor_msgs::PointCloud &depthPoints)
{
    if (objDataInvalidTimer_.IsTimeOut()) {
        return false;
    }

    std::unique_lock<std::recursive_mutex> lock(cacheMutex_);
    depthPoints = objPoints_;
    return true;
}

/**************************************************************************************
功能描述: 深度相机数据接收
修改记录:
**************************************************************************************/
void DEPTH_CAMERA_MSG_RECEIVER_C::MsgReceiveCallBack(const sensor_msgs::PointCloud2ConstPtr &ptrMsg)
{
    if (!ptrMsg || !ptrMsg->data.size()) return;
    pcl::fromROSMsg(*ptrMsg, *ptrCloud_);
    TimerReset();
}

/**************************************************************************************
功能描述: 获取相机深度点云数据
修改记录:
**************************************************************************************/
void DEPTH_CAMERA_MSG_RECEIVER_C::GetPointsCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &ptrCloud) const
{
    *ptrCloud = *ptrCloud_;
}

/**************************************************************************************
功能描述: 判断是否超时
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_MSG_RECEIVER_C::IsDataInvalid() const
{
    return objDataInvalidTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 是否有故障
修改记录:
**************************************************************************************/
BOOL DEPTH_CAMERA_MSG_RECEIVER_C::IsSensorFault() const
{
    return objSensorFaultTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 复位
修改记录:
**************************************************************************************/
void DEPTH_CAMERA_MSG_RECEIVER_C::TimerReset()
{
    objDataInvalidTimer_.Reset();
    objSensorFaultTimer_.Reset();
}

/**************************************************************************************
功能描述: 获取普通点云
修改记录:
**************************************************************************************/
void DEPTH_CAMERA_MSG_RECEIVER_C::GetNormalPointsCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &ptrCloud) const
{
    if (objDataInvalidTimer_.IsTimeOut()) {
        ptrCloud->clear();
        return;
    }

    GetPointsCloud(ptrCloud);
}

/**************************************************************************************
功能描述: 选择深度相机enum
修改记录:
**************************************************************************************/
void SELECT_DEPTH_CAMERA_MSG_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::visionDepthCamera::ConstPtr &ptrMsg)
{
    if (!ptrMsg) return;
    enSelectDepthCamera_ = D_CAMERA_ENUM(ptrMsg->selectDepthCamera);
    ST_LOG_INFO("Receive Select Depth Camera: %d.", ptrMsg->selectDepthCamera);
}

/**************************************************************************************
功能描述: 获取选择深度相机enum
修改记录:
**************************************************************************************/
void SELECT_DEPTH_CAMERA_MSG_RECEIVER_C::Get(D_CAMERA_ENUM &enSelectDepthCamera)
{
    enSelectDepthCamera = enSelectDepthCamera_;
}

/**************************************************************************************
功能描述: 发布深度相机点云
修改记录:
**************************************************************************************/
void CAMERA_PCD_SENDER_C::Send(pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud)
{
    sensor_msgs::PointCloud2 msg;

    pcl::toROSMsg(*pCloud, msg);
    msg.header.frame_id = PCPT_FRAME_ID_CAR;
    msg.header.stamp    = RosTimeNow();

    if (0 != publisher_.getNumSubscribers()) {
        publisher_.publish(msg);
    }
}

/**************************************************************************************
功能描述: 发送视觉任务控制消息
修改记录:
**************************************************************************************/
void VISION_FUNC_SENDER_C::Send(const std::string &strVisionFunc)
{
    std_msgs::String msg;
    msg.data = strVisionFunc;
    if (publisher_.getNumSubscribers()) {
        publisher_.publish(msg);
    }
}

void VISION_FUNC_RECEIVER_C::Get(std::string &strVisionFunc)
{
    strVisionFunc = msg_.data;
}

void VISION_FUNC_RECEIVER_C::MsgReceiveCallback(const std_msgs::String &msg)
{
    msg_ = msg;
}

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
void VISION_DETECT_SENDER_C::Send(double detectTime, const std::vector<COORDINATE_2D_STRU> &vstPos)
{
    sensor_msgs::PointCloud msg;
    msg.header.frame_id = "base_footprint";
    msg.header.stamp    = ros::Time(detectTime);

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        geometry_msgs::Point32 point;
        point.x = stPos.x;
        point.y = stPos.y;
        point.z = 0;

        msg.points.push_back(point);
    }

    objPublisher_.publish(msg);
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
const VISION_OBJ_STRU<V_COOR_STRU>* VISION_COOR_RECEIVER_C::Get() {
    int rslt = IsAbnormal();
    ST_LOG_ERR_SAMPLE_IF_TRUE(rslt, 60 * PCPT_HZ_MAIN_FUN, "图像网络检测坐标数据异常。");
    if (rslt || !hasNewData_) return nullptr;
    hasNewData_ = false;
    return &stVisionObj_;
}

/**************************************************************************************
功能描述: 消息回调
修改记录:
**************************************************************************************/
void VISION_COOR_RECEIVER_C::MsgReceiveCallback(const sensor_msgs::PointCloud::ConstPtr& ptrMsg)
{
    if (!ptrMsg) return;
    TimerReset();
    hasNewData_ = true;

    stVisionObj_.data.clear();
    stVisionObj_.stamp = RosStamp(ptrMsg);
    for (auto &point : ptrMsg->points) {
        V_COOR_STRU stVisionCoor;
        stVisionCoor.stCoor.SetValue(point.x, point.y);
        stVisionObj_.data.push_back(stVisionCoor);
    }
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
const VISION_OBJ_STRU<V_AREA_STRU> *VISION_AREA_RECEIVER_C::Get()
{
    /* 数据超时异常时，返回空指针 */
    bool isAb = IsAbnormal();
    ST_LOG_ERR_SAMPLE_IF_TRUE(isAb, 60 * PCPT_HZ_MAIN_FUN, "图像网络分割数据异常。");
    if (isAb) return nullptr;

    /* 无新数据时，返回空数据 */
    if (!hasNewData_) {
        stVisionObj_.clear();
        return &stVisionObj_;
    }

    /* 数据实时性判断 */
    if (g_isRestartedPnp) stVisionObj_.reset(); //重启感知后的一段时间内没有图像数据，需将历史缓存的结果和时间重置
    if (!IsTimelinessOk(stVisionObj_.stamp)) return nullptr;

    /* 有新数据时，返回新数据 */
    hasNewData_ = false;
    return &stVisionObj_;
}

/**************************************************************************************
功能描述: 消息回调
修改记录:
**************************************************************************************/
void VISION_AREA_RECEIVER_C::MsgReceiveCallback(const comm_msg::visionAreas::ConstPtr &ptrMsg)
{
    if (!ptrMsg) return;
    TimerReset();
    hasNewData_ = true;

    stVisionObj_.clear();
    stVisionObj_.stamp      = RosStamp(ptrMsg);
    stVisionObj_.strCamera  = ptrMsg->cameraId;
    g_v_seg_pipeline_state  = ptrMsg->pipeline.value;

    for (auto &area : ptrMsg->areas) {
        if (area.pixels.empty()) continue;
        V_AREA_STRU stVisionArea;
        for (auto &pixel : area.pixels) {
            V_PIXEL_STRU stVisionPixel;
            stVisionPixel.label = (LABEL_ENUM)pixel.label.value;
            stVisionPixel.score = pixel.score;
            stVisionPixel.stImg.SetValue(pixel.x, pixel.y);
            stVisionArea.push_back(stVisionPixel);
        }
        stVisionArea.label = stVisionArea.lstPixel.front().label;
        stVisionObj_.push_back(stVisionArea);
    }
}

/**************************************************************************************
功能描述: 视觉数据实时性是否正常
修改记录:
**************************************************************************************/
bool VISION_AREA_RECEIVER_C::IsTimelinessOk(const double msgTime)
{
    if (g_isSim) return true;
    if (SIG(msgTime) == 0) return true;

    double curTime = RosStampNow();
    double difTime = fabs(curTime - msgTime);
    float  refTime = 2.0;

    if (difTime > refTime) {
        if (SIG(abMsgTime_ - msgTime) != 0) {       //避免重复打印同一数据的超时日志
            ST_LOG_ERR("话题[%s]实时性异常, 当前时间[%.2f], 话题时间[%.2f], 时间差[%.2f(ref:%.2f)].",
                strTopicName_.c_str(), curTime, msgTime, difTime, refTime);
            abMsgTime_ = msgTime;
        }
        return false;
    }
    return true;
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
const VISION_OBJ_STRU<V_RECT_STRU> *VISION_RECTANGLE_RECEIVER_C::Get()
{
    /* 数据超时异常时，返回空指针 */
    bool isAb = IsAbnormal();
    ST_LOG_ERR_SAMPLE_IF_TRUE(isAb, 60 * PCPT_HZ_MAIN_FUN, "图像网络检测数据异常");
    if (isAb) return nullptr;

    /* 无新数据时，返回空数据 */
    if (!hasNewData_) {
        stVisionObj_.clear();
        return &stVisionObj_;
    }

    /* 数据实时性判断 */
    if (g_isRestartedPnp) stVisionObj_.reset(); //重启感知后的一段时间内没有图像数据，需将历史缓存的结果和时间重置
    if (!IsTimelinessOk(stVisionObj_.stamp)) return nullptr;

    /* 有新数据时，返回新数据 */
    hasNewData_ = false;
    return &stVisionObj_;
}

/**************************************************************************************
功能描述: 消息回调
修改记录:
**************************************************************************************/
void VISION_RECTANGLE_RECEIVER_C::MsgReceiveCallback(const comm_msg::visionRects::ConstPtr &ptrMsg)
{
    if (!ptrMsg) return;
    TimerReset();
    hasNewData_ = true;

    stVisionObj_.clear();
    stVisionObj_.stamp      = RosStamp(ptrMsg);
    stVisionObj_.strCamera  = ptrMsg->cameraId;
    g_v_det_pipeline_state  = ptrMsg->pipeline.value;

    for (auto &object : ptrMsg->rects) {
        V_RECT_STRU stVisonRect;
        stVisonRect.label = LABEL_ENUM(object.label.value);
        stVisonRect.score = object.score;
        stVisonRect.stRect.SetValue(object.xmin, object.xmax, object.ymin, object.ymax);
        stVisonRect.feature = object.feature;
        stVisionObj_.data.push_back(stVisonRect);
    }

    if (strTopicName_ == TOPIC_NAME_VISION_TRAFFIC_LIGHT_DET_INFO) {
        ST_LOG_INFO("交通灯专项后处理收到检测框消息: 话题[%s], 相机[%s], rects数量[%lu], pipeline=%u, 时间戳[%.3f]。",
                    strTopicName_.c_str(),
                    stVisionObj_.strCamera.c_str(),
                    stVisionObj_.data.size(),
                    ptrMsg->pipeline.value,
                    stVisionObj_.stamp);
    }
}

/**************************************************************************************
功能描述: 视觉数据实时性是否正常
修改记录:
**************************************************************************************/
bool VISION_RECTANGLE_RECEIVER_C::IsTimelinessOk(const double msgTime)
{
    if (g_isSim) return true;
    if (SIG(msgTime) == 0) return true;

    double curTime = RosStampNow();
    double difTime = fabs(curTime - msgTime);
    float  refTime = 2.0;

    if (difTime > refTime) {
        if (SIG(abMsgTime_ - msgTime) != 0) {       //避免重复打印同一数据的超时日志
            ST_LOG_ERR("话题[%s]实时性异常, 当前时间[%.2f], 话题时间[%.2f], 时间差[%.2f(ref:%.2f)].",
                strTopicName_.c_str(), curTime, msgTime, difTime, refTime);
            abMsgTime_ = msgTime;
        }
        return false;
    }
    return true;
}

/**************************************************************************************
功能描述: 获取点云网络检测信息
修改记录:
**************************************************************************************/
const vector<BOX_INFO_STRU> *PCD_NET_MSG_RECEIVER_C::Get()
{
    int rslt = IsAbnormal();
    ST_LOG_ERR_SAMPLE_IF_TRUE(rslt, 60 * PCPT_HZ_MAIN_FUN, "点云网络检测数据异常");
    if (rslt) return nullptr;
    return &vstBoxes_;
}

/**************************************************************************************
功能描述: 点云网络检测信息消息回调
修改记录:
**************************************************************************************/
void PCD_NET_MSG_RECEIVER_C::MsgReceiverCallback(const comm_msg::boxArray::ConstPtr& ptrMsg)
{
    if (!ptrMsg) return;
    TimerReset();
    vstBoxes_.clear();

    double timeStame = RosStamp(ptrMsg);
    for (auto &msg : ptrMsg->boxes) {
        BOX_INFO_STRU box;
        box.timeStamp            = timeStame;
        box.enClass              = LABEL_ENUM(msg.label.value);
        box.minZ                 = 0;
        box.maxZ                 = msg.height;
        box.stRotatedRect.x      = msg.x;
        box.stRotatedRect.y      = msg.y;
        box.stRotatedRect.width  = msg.width;
        box.stRotatedRect.height = msg.length;
        box.stRotatedRect.angle  = msg.yaw;
        box.boxID                = id_;
        if (++id_ == UINT_MAX) id_ = 0;   //2^32-1 = 4294967295
        vstBoxes_.push_back(box);
    }
}

/**************************************************************************************
功能描述: 发送图片消息
修改记录:
**************************************************************************************/
void IMAGE_DATA_SENDER_C::Send(cv_bridge::CvImagePtr cvPtr)
{
    if (!cvPtr) {
        ST_LOG_ERR("Msg Pointer is nullptr, please check your msg.");
        return;
    }

    if (!publisher_.getNumSubscribers()) return;

    sensor_msgs::Image imageMsg;
    cvPtr->toImageMsg(imageMsg);
    publisher_.publish(imageMsg);
}

void IMAGE_DATA_SENDER_C::Send(cv::Mat &cvImage)
{
    if (cvImage.empty()) {
        ST_LOG_ERR("Msg is empty, please check your msg.");
        return;
    }

    cv_bridge::CvImagePtr cvPtr(new cv_bridge::CvImage);
    cvPtr->header.stamp = RosTimeNow();
    cvPtr->header.frame_id = PCPT_FRAME_ID_CAR;

    if (cvImage.channels() == 3)
        cvPtr->encoding = "bgr8";
    else if (cvImage.channels() == 1)
        cvPtr->encoding = "mono8";
    cvPtr->image = cvImage;

    Send(cvPtr);
}

/**************************************************************************************
功能描述: 是否有订阅
修改记录:
**************************************************************************************/
bool IMAGE_DATA_SENDER_C::HasSubscriber()
{
    return publisher_.getNumSubscribers();
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
int IMAGE_DATA_RCVER_C::Get(cv_bridge::CvImagePtr& cvPtr)
{
    if (IsAbnormal()) return -1;
    if (!hasNewData_) return 1;

    mutex_.lock();
    swap(cvPtr_, cvPtr);
    mutex_.unlock();

    hasNewData_ = false;
    return 0;
}

/**************************************************************************************
功能描述: 消息回调
修改记录:
**************************************************************************************/
void IMAGE_DATA_RCVER_C::MsgReceiverCallback(const sensor_msgs::ImageConstPtr& ptrMsg)
{
    if (!ptrMsg) return;
    TimerReset();
    hasNewData_ = true;

    try {
        mutex_.lock();
        cvPtr_ = cv_bridge::toCvCopy(ptrMsg, sensor_msgs::image_encodings::BGR8);
        mutex_.unlock();
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
}

/**************************************************************************************
功能描述: 发送路面标识数据
修改记录:
**************************************************************************************/
void ROADMARK_SENDER_C::Send(const vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double timeStamp)
{
    SendMsg(vvstCoor3d, timeStamp);
    SendMarker(vvstCoor3d, timeStamp);
}

/**************************************************************************************
功能描述: 发送路面标识msg
修改记录:
**************************************************************************************/
void ROADMARK_SENDER_C::SendMsg(const vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double timeStamp)
{
    if (pubMsg_.getNumSubscribers() == 0) return;

    comm_msg::roadmark roadmarkMsg;
    roadmarkMsg.header.stamp    = RosTime(timeStamp);
    roadmarkMsg.header.frame_id = PCPT_FRAME_ID_CAR;

    for (auto &vstCoor3d : vvstCoor3d) {
        geometry_msgs::Polygon polygon;
        for (auto &stCoor3d : vstCoor3d) {
            geometry_msgs::Point32 point;
            point.x = stCoor3d.x;
            point.y = stCoor3d.y;
            point.z = stCoor3d.z;
            polygon.points.push_back(point);
        }
        roadmarkMsg.polygons.push_back(polygon);
    }

    pubMsg_.publish(roadmarkMsg);
}

/**************************************************************************************
功能描述: 发送路面标识marker
修改记录:
**************************************************************************************/
void ROADMARK_SENDER_C::SendMarker(const vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double timeStamp)
{
    if (pubMarker_.getNumSubscribers() == 0) return;

    visualization_msgs::MarkerArray markerArray;
    visualization_msgs::Marker lines;

    lines.header.stamp        = RosTime(timeStamp);
    lines.header.frame_id     = PCPT_FRAME_ID_CAR;
    lines.ns                  = "roadmark";
    lines.action              = visualization_msgs::Marker::ADD;
    lines.pose.orientation.w  = 1.0;
    lines.type                = visualization_msgs::Marker::LINE_LIST;
    lines.scale.x             = 0.01;
    lines.color.r             = 1.0;
    lines.color.a             = 0.5;

    int id = 0;
    for (const auto &vstCoor3d : vvstCoor3d) {
        if (vstCoor3d.size() < 2) continue;

        lines.id = id++;
        for (size_t i = 0; i < vstCoor3d.size(); ++i) {
            geometry_msgs::Point point;

            const COORDINATE_3D_STRU &stCoor3d_p = vstCoor3d[i];
            point.x = stCoor3d_p.x;
            point.y = stCoor3d_p.y;
            point.z = stCoor3d_p.z;
            lines.points.push_back(point);

            const COORDINATE_3D_STRU &stCoor3d_b = vstCoor3d[(i + 1) % vstCoor3d.size()];
            point.x = stCoor3d_b.x;
            point.y = stCoor3d_b.y;
            point.z = stCoor3d_b.z;
            lines.points.push_back(point);
        }

        markerArray.markers.push_back(lines);
    }

    /* 清除无效的Marker在rviz上的显示 */
    if (vvstCoor3d.size() < preMarkerSize_) {
        lines.action = visualization_msgs::Marker::DELETE;
        for (uint32_t i = 0; i < preMarkerSize_ - vvstCoor3d.size(); ++i) {
            lines.id = id;
            markerArray.markers.push_back(lines);
            id++;
        }
    }
    preMarkerSize_ = vvstCoor3d.size();
    pubMarker_.publish(markerArray);
}

/**************************************************************************************
功能描述: 获取路面标识数据
修改记录:
**************************************************************************************/
int ROADMARK_RECEIVER_C::Get(vector<vector<COORDINATE_3D_STRU>>& vvstCoor3d, double& timeStamp)
{
    if (vvstCoor3d_.size()) {
        swap(vvstCoor3d, vvstCoor3d_);
        timeStamp = timeStamp_;
        vvstCoor3d_.clear();
        return 0;
    }
    return -1;
}

/**************************************************************************************
功能描述: 路面标识数据消息回调
修改记录:
**************************************************************************************/
void ROADMARK_RECEIVER_C::MsgReceiverCallback(const comm_msg::roadmarkPtr& ptrMsg)
{
    if (!ptrMsg) return;
    vvstCoor3d_.clear();
    timeStamp_ = RosStamp(ptrMsg);

    for (const auto &polygon : ptrMsg->polygons) {
        vector<COORDINATE_3D_STRU> vstCoor3d;
        for (const auto &point : polygon.points) {
            COORDINATE_3D_STRU stCoor3d;
            stCoor3d.x = point.x;
            stCoor3d.y = point.y;
            stCoor3d.z = point.z;
            vstCoor3d.push_back(stCoor3d);
        }
        vvstCoor3d_.push_back(vstCoor3d);
    }
}

void ROADMARK_RECEIVER_C::MarkerReceiverCallback(const visualization_msgs::MarkerArrayPtr& ptrMsg)
{
    if (!ptrMsg) return;
    vvstCoor3d_.clear();
    timeStamp_ = RosStamp(ptrMsg->markers.front().header);

    for (const visualization_msgs::Marker &lines : ptrMsg->markers) {
        vector<COORDINATE_3D_STRU> vstCoor3d;
        for (uint32_t i = 0; i < lines.points.size(); i+=2) {
            COORDINATE_3D_STRU stCoor3d;
            stCoor3d.x = lines.points[i].x;
            stCoor3d.y = lines.points[i].y;
            stCoor3d.z = lines.points[i].z;
            vstCoor3d.push_back(stCoor3d);
        }
        vvstCoor3d_.push_back(vstCoor3d);
    }
}

/**************************************************************************************
功能描述: 检测请求响应结果发布
修改记录:
**************************************************************************************/
void DETECT_RESULT_SENDER_C::Send(const int result)
{
    comm_msg::detect_result msg;
    msg.result = result;
    publisher_.publish(msg);
}
