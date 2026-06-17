#include "pcpt_comm/pcpt_comm_points.h"

std::recursive_mutex    PCPT_LIDAR_DATA_C::mutex_r_;
PCPT_LIDAR_DATA_C*      PCPT_LIDAR_DATA_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新雷达数据
修改记录:
**************************************************************************************/
void PCPT_LIDAR_DATA_C::UptLidarData(string strLidarName, SEGMENT_RESULT_STRU& stLidarSeg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    umLidarSeg_[strLidarName].Copy(stLidarSeg);
}

/**************************************************************************************
功能描述: 更新雷达击中标识
修改记录:
**************************************************************************************/
void PCPT_LIDAR_DATA_C::UptLidarData(string strLidarName, const std::vector<HIT_MARK_IDX> &vtrHitMarkIdx)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    umLidarHitMarkIdx_[strLidarName] = vtrHitMarkIdx;
}

/**************************************************************************************
功能描述: 获取雷达数据
修改记录:
**************************************************************************************/
void PCPT_LIDAR_DATA_C::GetAllLidarData(std::unordered_map<string, SEGMENT_RESULT_STRU>& umLidarSeg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    umLidarSeg = umLidarSeg_;
}

void PCPT_LIDAR_DATA_C::GetAllLidarData(PointCloudXYZI::Ptr pCloud)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    for (const auto& pair : umLidarSeg_) {
        *pCloud += *(pair.second.pGroundPoints);
        *pCloud += *(pair.second.pNoGroundPoints);
    }
}

void PCPT_LIDAR_DATA_C::GetAllLidarGroundData(PointCloudXYZI::Ptr pCloud)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    for (const auto& pair : umLidarSeg_) {
        *pCloud += *(pair.second.pGroundPoints);
    }
}

void PCPT_LIDAR_DATA_C::GetAllLidarNoGroundData(PointCloudXYZI::Ptr pCloud)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    for (const auto& pair : umLidarSeg_) {
        *pCloud += *(pair.second.pNoGroundPoints);
    }
}

int PCPT_LIDAR_DATA_C::GetLidarData(string strLidarName, SEGMENT_RESULT_STRU& stLidarSeg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    auto iter = umLidarSeg_.find(strLidarName);
    bool isInvalid = iter == umLidarSeg_.end();

    ST_LOG_ERR_SAMPLE_IF_TRUE(isInvalid, 60 * PCPT_HZ_10, "Invalid lidar: %s.", strLidarName.c_str());
    if (isInvalid || iter->second.IsEmpty()) return -1; //雷达超时时，会将数据清空

    stLidarSeg.Copy(iter->second);
    return 0;
}

int PCPT_LIDAR_DATA_C::GetLidarData(string strLidarName, std::vector<HIT_MARK_IDX>& vtrHitMarkIdx)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    auto iter = umLidarHitMarkIdx_.find(strLidarName);
    bool isInvalid = iter == umLidarHitMarkIdx_.end();

    ST_LOG_ERR_SAMPLE_IF_TRUE(isInvalid, 60 * PCPT_HZ_10, "Invalid lidar: %s.", strLidarName.c_str());
    if (isInvalid || iter->second.empty()) return -1; //雷达超时时，会将数据清空

    vtrHitMarkIdx.insert(vtrHitMarkIdx.end(), iter->second.begin(), iter->second.end());

    return 0;
}

int PCPT_LIDAR_DATA_C::GetLidarData(string strLidarName, PointCloudXYZI::Ptr pCloud)
{
    SEGMENT_RESULT_STRU stLidarData;
    if (GetLidarData(strLidarName, stLidarData)) return -1;
    *pCloud += *stLidarData.pGroundPoints;
    *pCloud += *stLidarData.pNoGroundPoints;
    return 0;
}

int PCPT_LIDAR_DATA_C::GetLidarGroundData(string strLidarName, PointCloudXYZI::Ptr pCloud)
{
    SEGMENT_RESULT_STRU stLidarData;
    if (GetLidarData(strLidarName, stLidarData)) return -1;
    *pCloud = *stLidarData.pGroundPoints;
    return 0;
}

int PCPT_LIDAR_DATA_C::GetLidarNoGroundData(string strLidarName, PointCloudXYZI::Ptr pCloud)
{
    SEGMENT_RESULT_STRU stLidarData;
    if (GetLidarData(strLidarName, stLidarData)) return -1;
    *pCloud = *stLidarData.pNoGroundPoints;
    return 0;
}

/**************************************************************************************
功能描述: 获取雷达有效的分割数据，参数stLidarSeg必须缓存有上次数据的时间戳
修改记录: -1:异常 0:正常 1:无新数据
**************************************************************************************/
int PCPT_LIDAR_DATA_C::GetLidarValidData(const string& strLidarName, SEGMENT_RESULT_STRU& stLidarSeg)
{
    /* 获取指定雷达数据 */
    SEGMENT_RESULT_STRU stLidarSegTmp;
    int rslt = GetLidarData(strLidarName, stLidarSegTmp);

    /* 异常打印 */
    auto iter = umLidarAbLog_.find(strLidarName);
    if (iter != umLidarAbLog_.end()) {
        ABNORMAL_PRINT_C& objAbLog = iter->second;
        string strLog = "Failed to get " + strLidarName + " segment result data.";
        objAbLog.AbnormalPrint(rslt == -1, 60 * PCPT_HZ_10, strLog);
    }

    /* 更新数据及返回状态 */
    if (rslt == -1) return -1;  //无对应雷达或数据为空（数据超时）
    if (PclStampInt(stLidarSegTmp.pNoGroundPoints) == PclStampInt(stLidarSeg.pNoGroundPoints)) return 1;   //时间戳不变，无新数据
    stLidarSeg.Copy(stLidarSegTmp);
    return 0;
}

int PCPT_LIDAR_DATA_C::GetLidarValidData(const string& strLidarName, std::vector<HIT_MARK_IDX>& vtrHitMarkIdx)
{
     /* 获取指定雷达数据 */
    std::vector<HIT_MARK_IDX> vtrTempHitMarkIdx;
    int rslt = GetLidarData(strLidarName, vtrTempHitMarkIdx);

    /* 异常打印 */
    auto iter = umLidarAbLog_.find(strLidarName);
    if (iter != umLidarAbLog_.end()) {
        ABNORMAL_PRINT_C& objAbLog = iter->second;
        string strLog = "Failed to get " + strLidarName + " hit mark data.";
        objAbLog.AbnormalPrint(rslt == -1, 60 * PCPT_HZ_10, strLog);
    }

    /* 更新数据及返回状态 */
    if (rslt == -1) return -1;  //无对应雷达或数据为空（数据超时）
    //if (PclStampInt(stLidarSegTmp.pNoGroundPoints) == PclStampInt(stLidarSeg.pNoGroundPoints)) return 1;   //时间戳不变，无新数据
    //stLidarSeg.Copy(stLidarSegTmp);
    vtrHitMarkIdx.swap(vtrTempHitMarkIdx);
    return 0;
}

/**************************************************************************************
功能描述: 融合雷达数据的时间戳
修改记录:
**************************************************************************************/
int PCPT_LIDAR_DATA_C::MergeLidarStamp(const unordered_map<string, SEGMENT_RESULT_STRU>& umLidarSegs, uint64_t &stamp, const int type)
{
    uint64_t timeStampMin         = ULLONG_MAX; // 最小时间戳[unit: us]
    uint64_t timeStampMax         = 0;          // 最大时间戳
    uint64_t timeStampAvg         = 0;          // 平均时间戳
    uint64_t timeStampBlindLidar  = 0;          // 补盲雷达时间戳
    int      validLidarCount      = 0;          // 当前有效雷达点云数量
    int      validBlindLidarCount = 0;          // 当前有效补盲雷达点云数量

    /* 统计所有3D点云的时间 */
    for (auto iter = umLidarSegs.begin(); iter != umLidarSegs.end(); ++iter) {
        const SEGMENT_RESULT_STRU &stSeg = iter->second;
        if (stSeg.IsClear()) continue;
        uint64_t timeStamp = PclStampInt(stSeg.pNoGroundPoints);
        if (timeStamp != 0) {
            timeStampAvg += timeStamp;
            timeStampMin = MIN(timeStampMin, timeStamp);
            timeStampMax = MAX(timeStampMax, timeStamp);
            validLidarCount++;

            /* 判断是否为补盲类型雷达 */
            if (HasElement(BlindLidarType, stSeg.strLidarType)) {
                validBlindLidarCount++;
                timeStampBlindLidar += timeStamp;
            }
        }
    }

    if (validLidarCount == 0) return -1;

    double maxDiffTime = (timeStampMax - timeStampMin) * 0.000001; // us-->s
    if (maxDiffTime > 0.2) ST_LOG_ERR("The maximum deviation of multi lidar timestamp is %f.", maxDiffTime);

    /* 获取综合点云数据的时间戳 */
    switch(type) {
        case 0:     // 使用平均时间戳
            stamp = timeStampAvg / validLidarCount;
            break;
        case 1:     // 使用最大时间戳（最新）
            stamp = timeStampMax;
            break;
        case 2:     // 如果有补盲雷达数据，使用补盲雷达（平均）时间戳，否则使用最新时间戳
            if (validBlindLidarCount) stamp = timeStampBlindLidar / validBlindLidarCount;
            else                      stamp = timeStampMax;
            break;
    }

    return 0;
}

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int PCPT_LIDAR_DATA_C::Init()
{
    PCPT_CFG_C* pobjCfg = PCPT_CFG_C::GetInstance();
    const CFG_PARAM_C& objCfgParam = pobjCfg->GetCfgParam();

    const CFG_PARAM_LIDARS_STRU &stLidarsCfg = objCfgParam.GetLidarsCfgInfo();

    for (const CFG_PARAM_LIDAR_STRU& stLidar : stLidarsCfg.vstLidarsCfg) {
        umLidarSeg_.emplace(stLidar.LidarName(), SEGMENT_RESULT_STRU());
        umLidarAbLog_.emplace(stLidar.LidarName(), ABNORMAL_PRINT_C());
    }

    ST_LOG_INFO("Init Success.");
    return 0;
}

std::recursive_mutex    PCPT_FUSED_DATA_C::mutex_r_;
PCPT_FUSED_DATA_C*      PCPT_FUSED_DATA_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 更新点云数据
修改记录:
**************************************************************************************/
void PCPT_FUSED_DATA_C::UptPcdNoGround(PointCloudXYZI::Ptr pCloud)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    objFusedPointCloud_ = *pCloud;
    objTimer_.Reset();
}

/**************************************************************************************
功能描述: 获取点云数据
修改记录:
**************************************************************************************/
int PCPT_FUSED_DATA_C::GetPointCloud(PointCloudXYZ::Ptr &pCloud)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (objTimer_.IsTimeOut()) return -1;

    //pcl::copyPointCloud(*pFusedCloud_, *pCloud);
    pCloud->clear();
    pCloud->reserve(objFusedPointCloud_.size());
    pCloud->header = objFusedPointCloud_.header;
    for (auto& point : objFusedPointCloud_.points) {
        PointXYZ pointTmp;
        pointTmp.x = point.x;
        pointTmp.y = point.y;
        pointTmp.z = point.z;
        pCloud->push_back(pointTmp);
    }

    return 0;
}

int PCPT_FUSED_DATA_C::GetPointCloud(PointCloudXYZI::Ptr &pCloud)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    if (objTimer_.IsTimeOut()) return -1;
    *pCloud = objFusedPointCloud_;
    return 0;
}

std::atomic_uint        PCPT_OBST_DATA_C::frameNum_{1};
std::atomic_ullong      PCPT_OBST_DATA_C::dueTime_{0};
std::recursive_mutex    PCPT_OBST_DATA_C::mutex_r_;
PCPT_OBST_DATA_C*       PCPT_OBST_DATA_C::pInstance_{nullptr};

void PCPT_OBST_DATA_C::Upt2dObstData(PCPT_OBST_STRU& stObstByMap)
{
    if (frameNum_ < 2) {
        UptBySingleFrame(stObstByMap);   //使用单帧
    } else {
        UptByMultiFrame(stObstByMap);    //使用多帧
    }
}

void PCPT_OBST_DATA_C::Get2dObstData(PCPT_OBST_STRU &stObstByMap) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stObstByMap = stObstByMap_;
}

void PCPT_OBST_DATA_C::CacheData(const PCPT_OBST_STRU& stObstByMap,
                                       std::list<PCPT_OBST_STRU>& lstObstByMap) const
{
    if (lstObstByMap.empty() || (stObstByMap.stamp - lstObstByMap.front().stamp) > 0) {
        lstObstByMap.push_back(stObstByMap);
    }

    while(lstObstByMap.size() > 0) {
        PCPT_OBST_STRU& stObst = lstObstByMap.front();
        if ((stObstByMap.stamp - stObst.stamp) * TEN_6 > dueTime_) {
            lstObstByMap.pop_front();    //清除超时数据
            continue;
        }
        break;
    }

    while (lstObstByMap.size() > frameNum_)
        lstObstByMap.pop_front();
}

void PCPT_OBST_DATA_C::UptLabelVector(const PCPT_OBST_STRU& stObstByMap,
                                      std::vector<OBST_LABEL_ENUM>& vstLabel) const
{
    for (const auto& pair : stObstByMap.umObst) {
        if (vstLabel.end() == find(vstLabel.begin(), vstLabel.end(), pair.first)) {
            vstLabel.push_back(pair.first);
        }
    }
}

void PCPT_OBST_DATA_C::MergeData(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer,
                                 const std::list<PCPT_OBST_STRU>& lstObstByMap,
                                 MULTILAYER_OBST_STRU& stLayerObstByMap)
{
    /* 转成点云 */
    pCloudObst_->clear();
    for (const PCPT_OBST_STRU& stPcptObst : lstObstByMap) {
        const MULTILAYER_OBST_STRU* pstLayerObst = stPcptObst.LabelData(label);
        if (!pstLayerObst) continue;

        const LST_COOR* pLstCoor = pstLayerObst->LayerObst(layer);
        if (!pLstCoor) continue;

        for (const COORDINATE_2D_STRU& stPos : *pLstCoor)
            pCloudObst_->push_back(PointXYZ(stPos.x, stPos.y, 0));
    }

    /* 转成障碍数据 */
    if (pCloudObst_->size() > 0) {
        /* 降采样 */
        POINT_CLOUD_FILTER_C<PointXYZ> objFilter;
        objFilter.VoxelGridFilter(0.03, pCloudObst_, pCloudObst_);

        /* 整合到2D障碍 */
        LST_COOR lstCoorTmp;
        for (const auto &point : pCloudObst_->points)
            lstCoorTmp.push_back(COORDINATE_2D_STRU(point.x, point.y));
        stLayerObstByMap.insert(layer, lstCoorTmp);
    }
}

void PCPT_OBST_DATA_C::UptBySingleFrame(PCPT_OBST_STRU& stObstByMap)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stObstByMap_ = stObstByMap;
}

void PCPT_OBST_DATA_C::UptByMultiFrame(PCPT_OBST_STRU& stObstByMap)
{
    /* 数据缓存 */
    CacheData(stObstByMap, lstObstByMap_);

    /* 获取障碍层编号 */
    std::vector<OBST_LAYER_ENUM> vLayer = stObstByMap.Layers();
    if (vLayer.empty()) return;

    /* 更新类别标签 */
    UptLabelVector(stObstByMap, vstLabel_);

    /* 数据叠加 */
    for (const OBST_LABEL_ENUM& label : vstLabel_) {
        /* 根据类别判断是否需要叠加 */
        BOOL isAdd = false;
        switch(label) {
            case OBST_LABEL_UNKNOWN:
            case OBST_LABEL_STATIC:
            case OBST_LABEL_DYNAMIC:
                isAdd = true; break;
            default:
                isAdd = false; break;
        }

        /* 清除历史数据 */
        stObstByMapUpt_.clear(label);

        if (!isAdd) { /* 不需要叠加 */
            if (stObstByMap.isExist(label))
                stObstByMapUpt_[label] = stObstByMap[label];
        } else {      /* 需要叠加 */
            for (const OBST_LAYER_ENUM& layer : vLayer) {
                MergeData(label, layer, lstObstByMap_, stObstByMapUpt_[label]);
            }
        }
    }

    /* 更新时间和位姿 */
    stObstByMapUpt_.frameID = stObstByMap.frameID;
    stObstByMapUpt_.stamp     = stObstByMap.stamp;
    stObstByMapUpt_.stCarPose = stObstByMap.stCarPose;

    /* 更新2d障碍数据 */
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stObstByMap_ = stObstByMapUpt_;
}


