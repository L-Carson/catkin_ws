#include "pcpt_comm/pcpt_comm_msg.h"


BOOL LOCATION_3D_INFO_RECEIVER_C::IsNormal(void)
{
    return !objInvalidTimer_.IsTimeOut() && (enState_ >= 0);
}

BOOL LOCATION_3D_INFO_RECEIVER_C::IsNormalNew(void)
{
    return (enState_ >= 0);
}

BOOL LOCATION_3D_INFO_RECEIVER_C::IsChangingMap(void)
{
    return !objInvalidTimer_.IsTimeOut() && (enState_ == MAP_NODE_STATE_NAVIGATE_CHANGING_MAP);
}

BOOL LOCATION_3D_INFO_RECEIVER_C::IsCreatingMap(void)
{
    return !objInvalidTimer_.IsTimeOut() && (enState_ == MAP_NODE_STATE_GMAPPING_CREATING_MAP);
}

/**
 * @brief 获取3D定位信息
 *
 * @param stTsPose
 */
void LOCATION_3D_INFO_RECEIVER_C::Get(TIMESTAMP_POSE_STRU &stTsPose)
{
    stTsPose = stTsPose_;
}

/**
 * @brief 获取3D定位信息
 *
 * @param stTsPose
 * @return INT32: -1~定位异常；0-定位正常（包括导航、建图）
 */
INT32 LOCATION_3D_INFO_RECEIVER_C::GetNormal(TIMESTAMP_POSE_STRU &stTsPose)
{
    if (!IsNormal()) {
        return -1;
    }

    Get(stTsPose);
    return 0;
}

/**
 * @brief 获取3D定位信息
 *
 * @return TIMESTAMP_POSE_STRU*
 */
TIMESTAMP_POSE_STRU *LOCATION_3D_INFO_RECEIVER_C::GetNormal(void)
{
    if (!IsNormal()) {
        return NULL;
    }

    return &stTsPose_;
}

/**
 * @brief 获取2D定位信息
 *
 * @param stPose
 */
void LOCATION_3D_INFO_RECEIVER_C::Get(COOR_2D_POSE_STRU &stPose)
{
    stPose.stPos.x = stTsPose_.stPose.stPos.x;
    stPose.stPos.y = stTsPose_.stPose.stPos.y;
    stPose.dirAngle = stTsPose_.stPose.stAtt.yaw;
}

/**
 * @brief 获取2D定位信息
 *
 * @param stPose
 * @return int: -1~定位异常；0-定位正常（包括导航、建图）
 */
int LOCATION_3D_INFO_RECEIVER_C::GetNormal(COOR_2D_POSE_STRU &stPose)
{
    if (!IsNormal()) {
        return -1;
    }

    Get(stPose);
    return 0;
}

/**
 * @brief ROS话题回调: 3D定位信息
 *
 * @param pstMsg
 */
void LOCATION_3D_INFO_RECEIVER_C::ReceiveMsgCallBack(const comm_msg::PoseInfo::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();

    enState_ = static_cast<MAP_NODE_STATE_ENUM>(pstMsg->status);
    stTsPose_.time = RosStamp(pstMsg);

    stTsPose_.stPose.stPos.x     = pstMsg->x;
    stTsPose_.stPose.stPos.y     = pstMsg->y;
    stTsPose_.stPose.stPos.z     = pstMsg->z;
    stTsPose_.stPose.stAtt.roll  = pstMsg->roll;
    stTsPose_.stPose.stAtt.pitch = pstMsg->pitch;
    stTsPose_.stPose.stAtt.yaw   = pstMsg->yaw;
}

/**************************************************************************************
功能描述: 更新定位数据
修改记录:
**************************************************************************************/
void PCPT_LOC_2D_MSG_C::UptLocationMsg()
{
    mutex_.lock();
    if ((0 == objLocMsgReceiver_.GetNormal(stCarPoseByMap_)) ||
        (0 == objLocInfoReceiver_.GetNormal(stCarPoseByMap_))) {
        isNormal_ = true;
        stamp_ = RosStampNow();
        if (isCache_) CacheLocationMsg(stCarPoseByMap_, stamp_);
    } else
        isNormal_ = false;

    //ST_LOG_ERR_SAMPLE_IF_TRUE(!isNormal_, MAIN_FUN_HZ * 60, "Location Info Abnormal.");
    ST_LOG_ERR_SAMPLE_IF_TRUE(!isNormal_, PCPT_HZ_MAIN_FUN * 60, "定位数据异常.");
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取定位数据
修改记录:
**************************************************************************************/
int PCPT_LOC_2D_MSG_C::GetLocationMsg(COOR_2D_POSE_STRU& stPose)
{
    mutex_.lock();
    if (!isNormal_) {
        mutex_.unlock();
        return -1;
    } else {
        stPose = stCarPoseByMap_;
        mutex_.unlock();
        return 0;
    }
}

int PCPT_LOC_2D_MSG_C::GetLocationMsg(LOC_2D_STRU& stLoc)
{
    mutex_.lock();
    stLoc.stamp = stamp_;
    mutex_.unlock();
    return GetLocationMsg(stLoc.stPose);
}

/**************************************************************************************
功能描述: 根据时间戳获取匹配的定位数据
修改记录:
**************************************************************************************/
int PCPT_LOC_2D_MSG_C::GetLocationMsg(const double stamp, COOR_2D_POSE_STRU& stLoc)
{
    mutex_.lock();
    if (!isNormal_) {
        mutex_.unlock();
        return -1;
    }

    /* 删除旧的不用的定位数据 */
    while (!listLoc_.empty()) {
        const LOC_2D_STRU &stHeadLocInfo = listLoc_.front();
        if (stHeadLocInfo.stamp >= stamp) break;
        listLoc_.pop_front();
    }

    if (listLoc_.empty()) {
        mutex_.unlock();
        ST_LOG_ERR("Not match loc info. Detecting time: %lf, to now time diff: %lf.", stamp, Comm_GetTimeSec() - stamp);
        return -1;
    }

    const LOC_2D_STRU &stMatchLoc = listLoc_.front();
    BOOL isTimeDiffTooBig = ((stMatchLoc.stamp - stamp) > 0.1);
    ST_LOG_ERR_TIMER_IF(isTimeDiffTooBig, objTimerLog_, "Time diff(%lf) of detecting(%lf) and match loc(%lf) is too big.", stMatchLoc.stamp - stamp, stamp, stMatchLoc.stamp);
    if (isTimeDiffTooBig) {
        mutex_.unlock();
        return -1;
    }

    stLoc = stMatchLoc.stPose;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 缓存定位数据
修改记录:
**************************************************************************************/
void PCPT_LOC_2D_MSG_C::CacheLocationMsg(COOR_2D_POSE_STRU& stPose, double& stamp)
{
    LOC_2D_STRU stLoc(stamp, stPose);

    while (!listLoc_.empty()) {
        const LOC_2D_STRU &stHeadLocInfo = listLoc_.front();
        if (stamp > stHeadLocInfo.stamp && stamp - stHeadLocInfo.stamp <= cacheTime_) break;
        listLoc_.pop_front();
    }

    listLoc_.push_back(stLoc);
}

/**************************************************************************************
功能描述: 更新定位数据
修改记录:
**************************************************************************************/
void PCPT_LOC_3D_MSG_C::UptLocationMsg()
{
    // 兼容3个定位接口的查询，优先级依次为: 清扫车3D接口、导航3.0接口、导航2.0接口
    mutex_.lock();
    if ((0 == objLocMsgReceiver_.GetNormal(stCarPoseByMap_)) ||
        (0 == objLocInfoReceiver_.GetNormal(stCarPoseByMap_))) {
        isNormal_ = true;
        if (isCache_) CacheLocationMsg(stCarPoseByMap_);
    }  else if (pobjTfApi_ != nullptr) {
        if (UptLocationMsgByTf(stCarPoseByMap_)) {
            isNormal_ = true;
            if (isCache_) CacheLocationMsg(stCarPoseByMap_);
        }
    }  else {
        isNormal_ = false;
    }

    //ST_LOG_ERR_SAMPLE_IF_TRUE(!isNormal_, MAIN_FUN_HZ * 60, "Location Info Abnormal.");
    ST_LOG_ERR_SAMPLE_IF_TRUE(!isNormal_, PCPT_HZ_MAIN_FUN * 60, "定位数据异常.");
    mutex_.unlock();
}

bool PCPT_LOC_3D_MSG_C::UptLocationMsgByTf(TIMESTAMP_POSE_STRU &stCarPoseByMap)
{
    if (pobjTfApi_->UpdateTFImple(PCPT_FRAME_ID_MAP, PCPT_FRAME_ID_CAR, tfCar2MapStamped_) == true) {
        double roll, pitch, yaw;
        double tx = tfCar2MapStamped_.getOrigin().x();
        double ty = tfCar2MapStamped_.getOrigin().y();
        double tz = tfCar2MapStamped_.getOrigin().z();
        tf::Matrix3x3(tfCar2MapStamped_.getRotation()).getRPY(roll, pitch, yaw);

        stCarPoseByMap.stPose.SetValue(tx, ty, tz, roll, pitch, yaw);
        stCarPoseByMap.time = tfCar2MapStamped_.stamp_.toSec();
        return true;
    }
    return false;
}

/**
 * @brief 获取3D定位信息
 *
 * @param stPose
 * @return int: -1~定位异常；0~定位正常
 */
int PCPT_LOC_3D_MSG_C::GetLocationMsg(COOR_3D_POSE_STRU& stPose) const
{
    mutex_.lock();
    if (!isNormal_) {
        mutex_.unlock();
        //ST_LOG_ERR("Failed to get location msg of <%s>.", strMark_.c_str());
        return -1;
    } else {
        stPose = stCarPoseByMap_.stPose;
        mutex_.unlock();
        return 0;
    }
}

/**
 * @brief 获取3D定位信息
 *
 * @param stLoc
 * @return int: -1~定位异常；0~定位正常
 */
int PCPT_LOC_3D_MSG_C::GetLocationMsg(LOC_3D_STRU& stLoc) const
{
    mutex_.lock();
    stLoc.stamp = stCarPoseByMap_.time;
    mutex_.unlock();
    return GetLocationMsg(stLoc.stPose);
}

/**
 * @brief 获取3D定位信息
 *
 * @param stPose
 * @return int: -1~定位异常；0~定位正常
 */
int PCPT_LOC_3D_MSG_C::GetLocationMsg(const double stamp, COOR_3D_POSE_STRU& stPose)
{
    mutex_.lock();
    if (!isCache_) {
        ST_LOG_ERR("The current object does not support locating data cache.");
        mutex_.unlock();
        return -1;
    };

    if (!isNormal_) {
        mutex_.unlock();
        return -1;
    }

    if (listLoc_.empty()) {
        mutex_.unlock();
        ST_LOG_ERR("The cached location data is empty.");
        return -1;
    }

    /* 删除旧的不用的定位数据，至少留一帧最新的定位数据 */
    bool hasPreLoc = false; //是否存在给定时间戳前一时刻的定位信息
    LOC_3D_STRU stPreLoc;   //给定时间戳前一时刻的定位信息
    while (listLoc_.size() > 1) {
        const LOC_3D_STRU &stHeadLocInfo = listLoc_.front();
        if (stHeadLocInfo.stamp >= stamp) break;
        stPreLoc = listLoc_.front();
        hasPreLoc = true;
        listLoc_.pop_front();
    }

    /* 判断匹配数据的有效性 */
    const LOC_3D_STRU &stMatchLoc = listLoc_.front();
    BOOL isTimeDiffTooBig = (fabs(stMatchLoc.stamp - stamp) > 0.15);
    BOOL isLocListOnlyOne = (listLoc_.size() == 1);
    ST_LOG_ERR_TIMER_IF(isTimeDiffTooBig &&  isLocListOnlyOne, objTimerLog1_, "Time diff(%lf) of <%s> detecting(%lf) and latest loc(%lf) is too big.", stMatchLoc.stamp - stamp, strMark_.c_str(), stamp, stMatchLoc.stamp);
    ST_LOG_ERR_TIMER_IF(isTimeDiffTooBig && !isLocListOnlyOne, objTimerLog2_, "Time diff(%lf) of <%s> detecting(%lf) and match loc(%lf) is too big.",  stMatchLoc.stamp - stamp, strMark_.c_str(), stamp, stMatchLoc.stamp);
    if (isTimeDiffTooBig) {
        mutex_.unlock();
        return -1;
    }

    /* 输出定位结果的加权值 */
    if (!hasPreLoc || SIG(stMatchLoc.stamp - stPreLoc.stamp) == 0) {
        stPose = stMatchLoc.stPose;
    } else {
        /* yaw 可能会出现-PI/PI临界跳变的情况 */
        float matchLocYaw = stMatchLoc.stPose.stAtt.yaw;
        if (matchLocYaw < 0) matchLocYaw = fmod(matchLocYaw + PI2, PI2);
        if (stPreLoc.stPose.stAtt.yaw < 0) stPreLoc.stPose.stAtt.yaw = fmod(stPreLoc.stPose.stAtt.yaw + PI2, PI2);

        float rate = (stamp - stPreLoc.stamp) / (stMatchLoc.stamp - stPreLoc.stamp);
        stPose.stPos.x     = stPreLoc.stPose.stPos.x     + rate * (stMatchLoc.stPose.stPos.x     - stPreLoc.stPose.stPos.x);
        stPose.stPos.y     = stPreLoc.stPose.stPos.y     + rate * (stMatchLoc.stPose.stPos.y     - stPreLoc.stPose.stPos.y);
        stPose.stPos.z     = stPreLoc.stPose.stPos.z     + rate * (stMatchLoc.stPose.stPos.z     - stPreLoc.stPose.stPos.z);
        stPose.stAtt.roll  = stPreLoc.stPose.stAtt.roll  + rate * (stMatchLoc.stPose.stAtt.roll  - stPreLoc.stPose.stAtt.roll);
        stPose.stAtt.pitch = stPreLoc.stPose.stAtt.pitch + rate * (stMatchLoc.stPose.stAtt.pitch - stPreLoc.stPose.stAtt.pitch);
        stPose.stAtt.yaw   = stPreLoc.stPose.stAtt.yaw   + rate * (matchLocYaw                   - stPreLoc.stPose.stAtt.yaw);

        if (stPose.stAtt.yaw > PI) stPose.stAtt.yaw -= PI2;
    }

    mutex_.unlock();
    return 0;
}

/**
 * @brief 获取2D定位信息
 *
 * @param stPose
 * @return int: -1~定位异常；0~定位正常
 */
int PCPT_LOC_3D_MSG_C::GetLocationMsg(COOR_2D_POSE_STRU& stPose) const
{
    COOR_3D_POSE_STRU stPose3d;
    if (GetLocationMsg(stPose3d)) return -1;
    stPose.stPos.x  = stPose3d.x();
    stPose.stPos.y  = stPose3d.y();
    stPose.dirAngle = stPose3d.yaw();
    return 0;
}

/**
 * @brief 获取2D定位信息
 *
 * @param stLoc
 * @return int: -1~定位异常；0~定位正常
 */
int PCPT_LOC_3D_MSG_C::GetLocationMsg(LOC_2D_STRU& stLoc) const
{
    mutex_.lock();
    stLoc.stamp = stCarPoseByMap_.time;
    mutex_.unlock();
    return GetLocationMsg(stLoc.stPose);
}

/**
 * @brief 获取2D定位信息
 *
 * @param stPose
 * @return int: -1~定位异常；0~定位正常
 */
int PCPT_LOC_3D_MSG_C::GetLocationMsg(const double stamp, COOR_2D_POSE_STRU& stPose)
{
    COOR_3D_POSE_STRU stPose3d;
    if (GetLocationMsg(stamp, stPose3d)) return -1;
    stPose.stPos.x  = stPose3d.x();
    stPose.stPos.y  = stPose3d.y();
    stPose.dirAngle = stPose3d.yaw();
    return 0;
}

/**************************************************************************************
功能描述: 缓存定位数据
修改记录:
**************************************************************************************/
void PCPT_LOC_3D_MSG_C::CacheLocationMsg(TIMESTAMP_POSE_STRU& stCarPoseByMap)
{
    LOC_3D_STRU stLoc(stCarPoseByMap.time, stCarPoseByMap.stPose);

    while (!listLoc_.empty()) {
        const LOC_3D_STRU &stHeadLocInfo = listLoc_.front();
        if (stLoc.stamp > stHeadLocInfo.stamp && stLoc.stamp- stHeadLocInfo.stamp <= cacheTime_) break;
        listLoc_.pop_front();
    }

    listLoc_.push_back(stLoc);
}

std::recursive_mutex    PCPT_CAR_POSE_C::mutex_r_;
PCPT_CAR_POSE_C*        PCPT_CAR_POSE_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新车体位姿
修改记录:
**************************************************************************************/
void PCPT_CAR_POSE_C::UptCarPose(const bool isValid, const LOC_3D_STRU& stCarPose)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    isValid_   = isValid;
    stCarPose_ = stCarPose;
}

/**************************************************************************************
功能描述: 获取车体位姿
修改记录:
**************************************************************************************/
int PCPT_CAR_POSE_C::Get2dCarPose(LOC_2D_STRU& stCarPose)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (!isValid_) return -1;
    stCarPose.stamp = stCarPose_.stamp;
    stCarPose.stPose.SetValue(stCarPose_.x(), stCarPose_.y(), stCarPose_.yaw());
    return 0;
}

int PCPT_CAR_POSE_C::Get3dCarPose(LOC_3D_STRU& stCarPose)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (!isValid_) return -1;
    stCarPose = stCarPose_;
    return 0;
}

int PCPT_CAR_POSE_C::Get2dCarPose(COOR_2D_POSE_STRU& stCarPose)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (!isValid_) return -1;
    stCarPose.SetValue(stCarPose_.x(), stCarPose_.y(), stCarPose_.yaw());
    return 0;
}

int PCPT_CAR_POSE_C::Get3dCarPose(COOR_3D_POSE_STRU& stCarPose)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (!isValid_) return -1;
    stCarPose.SetValue(stCarPose_.x(), stCarPose_.y(), stCarPose_.z(),
                       stCarPose_.roll(), stCarPose_.pitch(), stCarPose_.yaw());
    return 0;
}

int PCPT_CAR_POSE_C::Get2dCarPos(COORDINATE_2D_STRU& stCarPos)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (!isValid_) return -1;
    stCarPos.SetValue(stCarPose_.x(), stCarPose_.y());
    return 0;
}

int PCPT_CAR_POSE_C::Get3dCarPos(COORDINATE_3D_STRU& stCarPos)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (!isValid_) return -1;
    stCarPos.SetValue(stCarPose_.x(), stCarPose_.y(), stCarPose_.z());
    return 0;
}

/**************************************************************************************
功能描述: 获取车体位姿
修改记录:
**************************************************************************************/
const LOC_2D_STRU PCPT_CAR_POSE_C::Get2dLocInfo()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return LOC_2D_STRU(stCarPose_.stamp, COOR_2D_POSE_STRU(stCarPose_.x(), stCarPose_.y(), stCarPose_.yaw()));
}

const LOC_3D_STRU PCPT_CAR_POSE_C::Get3dLocInfo()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return stCarPose_;
}

const COOR_2D_POSE_STRU PCPT_CAR_POSE_C::Get2dCarPose()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return COOR_2D_POSE_STRU(stCarPose_.x(), stCarPose_.y(), stCarPose_.yaw());
}

const COOR_3D_POSE_STRU PCPT_CAR_POSE_C::Get3dCarPose()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return COOR_3D_POSE_STRU(stCarPose_.x(), stCarPose_.y(), stCarPose_.z(),
                             stCarPose_.roll(), stCarPose_.pitch(), stCarPose_.yaw());
}

const COORDINATE_2D_STRU PCPT_CAR_POSE_C::Get2dCarPos()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return COORDINATE_2D_STRU(stCarPose_.x(), stCarPose_.y());
}

const COORDINATE_3D_STRU PCPT_CAR_POSE_C::Get3dCarPos()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return COORDINATE_3D_STRU(stCarPose_.x(), stCarPose_.y(), stCarPose_.z());
}

/**************************************************************************************
功能描述: 更新导航状态
修改记录:
**************************************************************************************/
void PCPT_NAV_STATE_MSG_C::UptNavStateMsg()
{
    mutex_.lock();
    objNavReceiver_.Get(stNavState_);
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取导航状态
修改记录:
**************************************************************************************/
void PCPT_NAV_STATE_MSG_C::GetNavStateMsg(NAV_STATE_STRU &stNavState)
{
    mutex_.lock();
    stNavState = stNavState_;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 更新IMU数据
修改记录:
**************************************************************************************/
void PCPT_IMU_DATA_C::UptImuData()
{
    mutex_.lock();
    if (objImuMsgReceiver_.Get(deqImuData_) &&
        objImuMsgReceiverW2_.Get(deqImuData_)) {
    } else {
        objTimer_.Reset();
        while(deqImuData_.size() > buffSize_) deqImuData_.pop_front();
    }

    ST_LOG_ERR_SAMPLE_IF_TRUE(objTimer_.IsTimeOut(), PCPT_HZ_MAIN_FUN * 60, "Imu数据异常.");
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取IMU数据
修改记录:
**************************************************************************************/
int PCPT_IMU_DATA_C::GetImuData(deque<IMU_DATA_STRU>& deqImuData)
{
    mutex_.lock();
    if (objTimer_.IsTimeOut()) {
        mutex_.unlock();
        return -1;
    }
    deqImuData = deqImuData_;
    mutex_.unlock();
    return 0;
}

int PCPT_IMU_DATA_C::GetImuData(IMU_DATA_STRU& stImuData)
{
    deque<IMU_DATA_STRU> deqImuData;
    if (GetImuData(deqImuData)) return -1;
    if (deqImuData.empty()) return -1;
    stImuData = deqImuData.back();
    return 0;
}

/**************************************************************************************
功能描述: 根据时间戳获取匹配的IMU数据
修改记录: 时间戳unit: s
**************************************************************************************/
int PCPT_IMU_DATA_C::GetImuData(const double stamp, IMU_DATA_STRU& stImuData)
{
    mutex_.lock();
    /* 删除老的不用的数据 */
    while (!deqImuData_.empty()) {
        const IMU_DATA_STRU &st = deqImuData_.front();
        if (st.time > stamp && deqImuData_.size() < buffSize_) break;
        deqImuData_.pop_front();
    }

    if (deqImuData_.empty()) {
        ST_LOG_ERR("Not match imu data. Detecting time: %lf, to now time diff: %lf.", stamp, Comm_GetTimeSec() - stamp);
        mutex_.unlock();
        return -1;
    }

    const IMU_DATA_STRU &stMatchImuData = deqImuData_.front();
    if ((stMatchImuData.time - stamp) > 0.1) {
        ST_LOG_ERR("Time diff(%lf) of detecting(%lf) and match imu(%lf) is too big.", stMatchImuData.time - stamp, stamp, stMatchImuData.time);
        mutex_.unlock();
        return -1;
    }

    stImuData = stMatchImuData;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新运动信息
修改记录:
**************************************************************************************/
void PCPT_MOVE_INFO_C::UptMoveInfo()
{
    mutex_.lock();

    DCU_MOVE_INFO_EX_STRU stMoveInfo;
    if (objMoveInfo_.Get(stMoveInfo)) {
        isDataValid_ = false;
    } else {
        isDataValid_ = true;
        deqMoveInfo_.push_back(stMoveInfo);
        while (deqMoveInfo_.size() > buffSize_) deqMoveInfo_.pop_front();
    }

    ST_LOG_ERR_SAMPLE_IF_TRUE(!isDataValid_, PCPT_HZ_MAIN_FUN * 60, "运动数据异常.");
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取运动信息队列
修改记录:
**************************************************************************************/
int PCPT_MOVE_INFO_C::GetMoveInfo(deque<DCU_MOVE_INFO_EX_STRU>& deqMoveInfo)
{
    mutex_.lock();
    if (!isDataValid_) {
        mutex_.unlock();
        return -1;
    }
    deqMoveInfo = deqMoveInfo_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 获取最新的运动信息
修改记录:
**************************************************************************************/
int PCPT_MOVE_INFO_C::GetMoveInfo(DCU_MOVE_INFO_EX_STRU& stMoveInfo)
{
    mutex_.lock();
    if (!isDataValid_ || deqMoveInfo_.empty()) {
        mutex_.unlock();
        return -1;
    }
    stMoveInfo = deqMoveInfo_.back();
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 根据时间戳获取运动信息
修改记录:
**************************************************************************************/
int PCPT_MOVE_INFO_C::GetMoveInfo(const double stamp, DCU_MOVE_INFO_EX_STRU& stMoveInfo)
{
    mutex_.lock();
    /* 删除老的不用的数据 */
    while (!deqMoveInfo_.empty()) {
        const DCU_MOVE_INFO_EX_STRU &st = deqMoveInfo_.front();
        if (st.time > stamp && deqMoveInfo_.size() < buffSize_) break;
        deqMoveInfo_.pop_front();
    }

    if (deqMoveInfo_.empty()) {
        ST_LOG_ERR("Not match imu data. Detecting time: %lf, to now time diff: %lf.", stamp, Comm_GetTimeSec() - stamp);
        mutex_.unlock();
        return -1;
    }

    const DCU_MOVE_INFO_EX_STRU &stMatchData = deqMoveInfo_.front();
    if ((stMatchData.time - stamp) > 0.1) {
        ST_LOG_ERR("Time diff(%lf) of detecting(%lf) and match imu(%lf) is too big.", stMatchData.time - stamp, stamp, stMatchData.time);
        mutex_.unlock();
        return -1;
    }

    stMoveInfo = stMatchData;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新Vel数据
修改记录:
**************************************************************************************/
void PCPT_VELOCITY_C::UptVelData()
{
    mutex_.lock();
    if (objVelMsgReceiver_.Get(vel_))
        isVelDataValid_ = false;
    else
        isVelDataValid_ = true;

    ST_LOG_ERR_SAMPLE_IF_TRUE(!isVelDataValid_, PCPT_HZ_MAIN_FUN * 60, "轮速编码器数据异常.");
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取vel数据
修改记录:
**************************************************************************************/
int PCPT_VELOCITY_C::GetVelData(float& vel)
{
    mutex_.lock();
    if (!isVelDataValid_) {
        mutex_.unlock();
        return -1;
    }
    vel = vel_;
    mutex_.unlock();
    return 0;
}

std::recursive_mutex     PCPT_VELOCITY_INST_C::mutex_r_;
PCPT_VELOCITY_INST_C*    PCPT_VELOCITY_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 获取vel数据
修改记录:
**************************************************************************************/
void PCPT_VELOCITY_INST_C::UptVelData(const bool isValid, const float& vel)
{
    mutex_.lock();
    isValid_ = isValid;
    vel_     = vel;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取vel数据
修改记录:
**************************************************************************************/
int PCPT_VELOCITY_INST_C::GetVelData(float& vel)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    vel = vel_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新数据
修改记录:
**************************************************************************************/
void PCPT_DEVICE_INFO_C::UptDevInfo()
{
    const DCU_MOVE_DEV_INFO_STRU* pstDevInfo = objDevInfoRecv_.Get();
    if (pstDevInfo == nullptr) {
        isDevInfoValid_ = false;
    } else {
        isDevInfoValid_ = true;
        stDevInfo_ = *pstDevInfo;
    }
    ST_LOG_ERR_SAMPLE_IF_TRUE(!isDevInfoValid_, PCPT_HZ_MAIN_FUN * 60, "设备信息异常.");
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
int PCPT_DEVICE_INFO_C::GetDevInfo(DCU_MOVE_DEV_INFO_STRU& stDevInfo)
{
    if (!isDevInfoValid_) {
        return -1;
    }
    stDevInfo = stDevInfo_;
    return 0;
}

std::recursive_mutex        PCPT_DEVICE_INFO_INST_C::mutex_r_;
PCPT_DEVICE_INFO_INST_C*    PCPT_DEVICE_INFO_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新数据
修改记录:
**************************************************************************************/
void PCPT_DEVICE_INFO_INST_C::UptDevInfo(const bool isValid, const DCU_MOVE_DEV_INFO_STRU& stDevInfo)
{
    mutex_.lock();
    isValid_ = isValid;
    stDevInfo_ = stDevInfo;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
int PCPT_DEVICE_INFO_INST_C::GetDevInfo(DCU_MOVE_DEV_INFO_STRU& stDevInfo)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    stDevInfo = stDevInfo_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新数据
修改记录:
**************************************************************************************/
void PCPT_EQUIPMENT_INFO_C::UptEquipmentInfo()
{
    const EQUIPMENT_INFO_STRU *pstEquipmentInfo = objEquInfoMsgReceiver_.Get();
    if (pstEquipmentInfo) {
        stEquipmentInfo_ = *pstEquipmentInfo;
        isDataValid_ = true;
    } else
        isDataValid_ = false;

    if (g_butt_dustbin || g_butt_charging_pile)
        ST_LOG_ERR_SAMPLE_IF_TRUE(!isDataValid_, PCPT_HZ_MAIN_FUN * 60, "获取装备信息异常（二维码、反光条等）。");
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
int PCPT_EQUIPMENT_INFO_C::GetEquipmentInfo(EQUIPMENT_INFO_STRU& stEquipmentInfo)
{
    if (!isDataValid_) return -1;
    stEquipmentInfo = stEquipmentInfo_;
    return 0;
}

std::recursive_mutex        PCPT_EQUIPMENT_INFO_INST_C::mutex_r_;
PCPT_EQUIPMENT_INFO_INST_C* PCPT_EQUIPMENT_INFO_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新数据
修改记录:
**************************************************************************************/
void PCPT_EQUIPMENT_INFO_INST_C::UptEquipmentInfo(const bool isValid, const EQUIPMENT_INFO_STRU& stEquipmentInfo)
{
    mutex_.lock();
    isValid_ = isValid;
    stEquipmentInfo_ = stEquipmentInfo;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取数据
修改记录:
**************************************************************************************/
int PCPT_EQUIPMENT_INFO_INST_C::GetEquipmentInfo(EQUIPMENT_INFO_STRU& stEquipmentInfo)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    stEquipmentInfo = stEquipmentInfo_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新设备位姿
修改记录:
**************************************************************************************/
void PCPT_EQUIPMENT_POSE_C::UptEquipmentPose()
{
    if (objRcv_.GetNormal(stEquipmentPose_))
        isDataValid_ = false;
    else
        isDataValid_ = true;
}

/**************************************************************************************
功能描述: 获取设备位姿
修改记录:
**************************************************************************************/
int PCPT_EQUIPMENT_POSE_C::GetEquipmentPose(COOR_2D_POSE_STRU& stEquipmentPose)
{
    if (isDataValid_) {
        stEquipmentPose = stEquipmentPose_;
        return 0;
    }
    return -1;
}

std::recursive_mutex        PCPT_EQUIPMENT_POSE_INST_C::mutex_r_;
PCPT_EQUIPMENT_POSE_INST_C* PCPT_EQUIPMENT_POSE_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新设备位姿
修改记录:
**************************************************************************************/
void PCPT_EQUIPMENT_POSE_INST_C::UptEquipmentPose(const bool isValid, const COOR_2D_POSE_STRU& stEquipmentPose)
{
    mutex_.lock();
    isValid_ = isValid;
    stEquipmentPose_ = stEquipmentPose;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取设备位姿
修改记录:
**************************************************************************************/
int PCPT_EQUIPMENT_POSE_INST_C::GetEquipmentPose(COOR_2D_POSE_STRU& stEquipmentPose)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    stEquipmentPose = stEquipmentPose_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新DCU信息
修改记录:
**************************************************************************************/
void PCPT_DCU_INFO_C::UptDcuInfo()
{
    if (objRcv_.Get(stDcuInfo_))
        isDataValid_ = false;
    else
        isDataValid_ = true;
}

/**************************************************************************************
功能描述: 获取DCU信息
修改记录:
**************************************************************************************/
int PCPT_DCU_INFO_C::GetDcuInfo(DCU_INFO_STRU& stDcuInfo)
{
    if (isDataValid_) {
        stDcuInfo = stDcuInfo_;
        return 0;
    }
    return -1;
}

std::recursive_mutex    PCPT_DCU_INFO_INST_C::mutex_r_;
PCPT_DCU_INFO_INST_C*   PCPT_DCU_INFO_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新DCU信息
修改记录:
**************************************************************************************/
void PCPT_DCU_INFO_INST_C::UptDcuInfo(const bool isValid, const DCU_INFO_STRU& stDcuInfo)
{
    mutex_.lock();
    isValid_ = isValid;
    stDcuInfo_ = stDcuInfo;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取DCU信息
修改记录:
**************************************************************************************/
int PCPT_DCU_INFO_INST_C::GetDcuInfo(DCU_INFO_STRU& stDcuInfo)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    stDcuInfo = stDcuInfo_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 更新二维码位姿
修改记录:
**************************************************************************************/
void PCPT_QR_CODE_POSE_C::UptQrCodePose()
{
    if (objRcv_.Get(stQrCodePose_))
        isDataValid_ = false;
    else
        isDataValid_ = true;
}

/**************************************************************************************
功能描述: 获取二维码位姿
修改记录:
**************************************************************************************/
int PCPT_QR_CODE_POSE_C::GetQrCodePose(POSE_ID_STRU& stQrCodePose)
{
    if (isDataValid_) {
        stQrCodePose = stQrCodePose_;
        return 0;
    }
    return -1;
}

std::recursive_mutex        PCPT_QR_CODE_POSE_INST_C::mutex_r_;
PCPT_QR_CODE_POSE_INST_C*   PCPT_QR_CODE_POSE_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新二维码位姿
修改记录:
**************************************************************************************/
void PCPT_QR_CODE_POSE_INST_C::UptQrCodePose(const bool isValid, const POSE_ID_STRU& stQrCodePose)
{
    mutex_.lock();
    isValid_ = isValid;
    stQrCodePose_ = stQrCodePose;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取二维码位姿
修改记录:
**************************************************************************************/
int PCPT_QR_CODE_POSE_INST_C::GetQrCodePose(POSE_ID_STRU& stQrCodePose)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    stQrCodePose = stQrCodePose_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 获取里程计数据
修改记录:
**************************************************************************************/
void PCPT_ODOM_3D_C::GetOdomData(deque<ODOM_DATA_STRU> &vOdomData)
{
    if (vOdomData_.empty()) return;
    vOdomData.insert(vOdomData.end(), vOdomData_.begin(), vOdomData_.end());
    vOdomData_.clear();
}

int PCPT_ODOM_3D_C::GetOdomData(ODOM_DATA_STRU &stOdomData)
{
    if (vOdomData_.empty()) return -1;
    stOdomData = vOdomData_.back();
    return 0;
}

/**************************************************************************************
功能描述: 话题数据回调
修改记录:
**************************************************************************************/
void PCPT_ODOM_3D_C::ReceiveMsgCallBack(const nav_msgs::Odometry::ConstPtr& pMsg)
{
    ODOM_DATA_STRU stOdomData;
    stOdomData.time = RosStamp(pMsg);
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

    while (vOdomData_.size() && vOdomData_.back().time >= stOdomData.time) vOdomData_.pop_back();
    vOdomData_.push_back(stOdomData);
    while(vOdomData_.size() > buffSize_) vOdomData_.pop_front();
}

std::recursive_mutex    PCPT_ODOM_3D_INST_C::mutex_r_;
PCPT_ODOM_3D_INST_C*    PCPT_ODOM_3D_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新里程计数据
修改记录:
**************************************************************************************/
void PCPT_ODOM_3D_INST_C::UptOdomData(const size_t buffSize, const deque<ODOM_DATA_STRU> &vOdomData)
{
    if (vOdomData.empty()) return;

    mutex_.lock();
    while (vOdomData_.size() && vOdomData_.back().time >= vOdomData.front().time) vOdomData_.pop_back();
    vOdomData_.insert(vOdomData_.end(), vOdomData.begin(), vOdomData.end());
    while (vOdomData_.size() > buffSize) vOdomData_.pop_front();
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取里程计数据
修改记录:
**************************************************************************************/
int PCPT_ODOM_3D_INST_C::GetOdomData(const double startStamp, deque<ODOM_DATA_STRU> &vOdomData)
{
    mutex_.lock();
    if (vOdomData_.empty()) {
        mutex_.unlock();
        return -1;
    }

    /**
     * 获取数据的起始时间与里程计数据的最新时间差别较大时(1s)，将startStamp视为无效
     * 可能是调试时循环播放bag包引起的
     */
    if (startStamp > vOdomData_.back().time + 1.0) {
        vOdomData = vOdomData_;
        mutex_.unlock();
        return -1;
    }

    auto iter = vOdomData_.begin();
    for (int i = int(vOdomData_.size() - 1); i >= 0; --i) {
        if (vOdomData_[i].time > startStamp) {
            if (i == 0) iter = vOdomData_.begin();
            else continue;
        } else {
            iter = vOdomData_.begin() + i + 1;
            break;
        }
    }

    if (iter == vOdomData_.end()) {
        mutex_.unlock();
        return -1;
    }

    vOdomData.insert(vOdomData.end(), iter, vOdomData_.end());

    mutex_.unlock();
    return 0;
}

int PCPT_ODOM_3D_INST_C::GetOdomData(ODOM_DATA_STRU &stOdomData)
{
    mutex_.lock();
    if (vOdomData_.empty()) {
        mutex_.unlock();
        return -1;
    }
    mutex_.unlock();
    stOdomData = vOdomData_.back();
    return 0;
}

/**************************************************************************************
功能描述: 获取GNSS信息
修改记录:
**************************************************************************************/
int PCPT_GNSS_C::GetGnssData(GPS_INFO_STRU &stGpsInfo)
{
    if (objInvalidTimer_.IsTimeOut()) return -1;
    stGpsInfo.Copy(stGpsInfo_);
    return 0;
}

/**************************************************************************************
功能描述: 话题数据回调
修改记录:
**************************************************************************************/
void PCPT_GNSS_C::ReceiveMsgCallBack(const comm_msg::gpsInfo::ConstPtr &pstMsg)
{
    objInvalidTimer_.Reset();
    stGpsInfo_.time               = RosStamp(pstMsg);
    stGpsInfo_.satelliteNum       = pstMsg->satelliteNum;
    stGpsInfo_.rsltType           = pstMsg->rsltType;
    stGpsInfo_.stPose.stCoord.lon = pstMsg->longitude;
    stGpsInfo_.stPose.stCoord.lat = pstMsg->latitude;
    stGpsInfo_.stPose.stCoord.alt = pstMsg->altitude;
    stGpsInfo_.stPose.yawAngle    = pstMsg->yamAngle;
}

std::recursive_mutex    PCPT_GNSS_INST_C::mutex_r_;
PCPT_GNSS_INST_C*       PCPT_GNSS_INST_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新GNSS数据
修改记录:
**************************************************************************************/
void PCPT_GNSS_INST_C::UptGnssData(const bool isValid, const GPS_INFO_STRU &stGpsInfo)
{
    mutex_.lock();
    isValid_ = isValid;
    stGpsInfo_ = stGpsInfo;
    mutex_.unlock();
}

/**************************************************************************************
功能描述: 获取GNSS数据
修改记录:
**************************************************************************************/
int PCPT_GNSS_INST_C::GetGnssData(GPS_INFO_STRU &stGpsInfo)
{
    mutex_.lock();
    if (!isValid_) {
        mutex_.unlock();
        return -1;
    }
    stGpsInfo = stGpsInfo_;
    mutex_.unlock();
    return 0;
}

/**************************************************************************************
功能描述: 获取检测请求信息
修改记录:
**************************************************************************************/
int PCPT_DETECT_TARGET_MSG_C::GetDetectTargetInfo(uint8_t& type, VST_COOR& vstCoors)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (objInvalidTimer_.IsTimeOut()) return -1;
    type     = type_;
    vstCoors = vstCoors_;
    return 0;
}

/**************************************************************************************
功能描述: 是否检测
修改记录:
**************************************************************************************/
bool PCPT_DETECT_TARGET_MSG_C::IsDetect()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return !objInvalidTimer_.IsTimeOut();
}

/**************************************************************************************
功能描述: 话题数据回调
修改记录:
**************************************************************************************/
void PCPT_DETECT_TARGET_MSG_C::ReceiveMsgCallBack(const comm_msg::detect_target::ConstPtr &pstMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    objInvalidTimer_.Reset();
    type_ = pstMsg->type;
    vstCoors_.clear();
    for (auto &pos : pstMsg->vertexes)
        vstCoors_.push_back(COORDINATE_2D_STRU(pos.pos_x, pos.pos_y));
}

/**************************************************************************************
功能描述: 话题数据回调
修改记录:
**************************************************************************************/
void PCPT_LABEL_CMD_C::ReceiveMsgCallBack(const std_msgs::Int8::ConstPtr pstCmdMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    isLabel_ = bool(pstCmdMsg->data);   //0 & 1
    objLabelTimer_.Reset();
}

/**************************************************************************************
功能描述: 是否标记充电桩
修改记录:
**************************************************************************************/
bool PCPT_LABEL_CMD_C::IsLabelChargingPile()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (objLabelTimer_.IsTimeOut()) return false;
    return isLabel_;
}

/**************************************************************************************
功能描述: 获取检测红绿灯信息
修改记录:
**************************************************************************************/
int PCPT_TRAFFIC_LIGHT_TARGET_MSG_C::GetTrafficLightTargetInfo(TRAFFIC_LIGHT_TARGET_STRU& stTrafficLight)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (objInvalidTimer_.IsTimeOut()) return -1;    // 超时未获取成功
    stTrafficLight = stTrafficLight_;
    return 0;
}

/**************************************************************************************
功能描述: 接收导航发送的检测红绿灯消息
修改记录:
**************************************************************************************/
void PCPT_TRAFFIC_LIGHT_TARGET_MSG_C::ReceiveMsgCallBack(const comm_msg::traffic_light_target::ConstPtr &pstMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    objInvalidTimer_.Reset();
    stTrafficLight_.isOpen             = pstMsg->is_open;
    stTrafficLight_.pixelBias          = pstMsg->pixel_bias;
    stTrafficLight_.rectBias           = pstMsg->rect_bias;

    stTrafficLight_.stTarget.type      = pstMsg->target.type;
    stTrafficLight_.stTarget.px        = pstMsg->target.pos_x;
    stTrafficLight_.stTarget.py        = pstMsg->target.pos_y;
    stTrafficLight_.stTarget.pz        = pstMsg->target.pos_z;
    stTrafficLight_.stTarget.rowLength = pstMsg->target.row_length;
    stTrafficLight_.stTarget.colLength = pstMsg->target.col_length;
    stTrafficLight_.ClearNears();

    for (const comm_msg::traffic_light_info& stMsgInfo : pstMsg->nears) {
        TRAFFIC_LIGHT_INFO_STRU stInfo;
        stInfo.type      = stMsgInfo.type;
        stInfo.px        = stMsgInfo.pos_x;
        stInfo.py        = stMsgInfo.pos_y;
        stInfo.pz        = stMsgInfo.pos_z;
        stInfo.rowLength = stMsgInfo.row_length;
        stInfo.colLength = stMsgInfo.col_length;
        stTrafficLight_.lstNear.push_back(stInfo);
    }
}
