#pragma once

#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_filter.h"
#include "pcpt_comm/pcpt_comm_tf.h"
#include "pcpt_comm/pcpt_comm_log.h"
#include "pcpt_comm/pcpt_comm_cfg.h"
#include "pcpt_comm/pcpt_comm_singleton.h"

using namespace std;

/**************************************************************************************
功能描述: 雷达数据
修改记录:
**************************************************************************************/
class PCPT_LIDAR_DATA_C
{
public:
    PCPT_LIDAR_DATA_C(){}
    PCPT_LIDAR_DATA_C(const PCPT_LIDAR_DATA_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_LIDAR_DATA_C &operator=(const PCPT_LIDAR_DATA_C &) = delete;   // 禁止使用拷贝赋值运算符

    static PCPT_LIDAR_DATA_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            return pInstance_;
        }

        pInstance_ = new PCPT_LIDAR_DATA_C();
        INT32 rslt = pInstance_->Init();
        if (rslt != 0) {
            ST_LOG_ERR("Create Object Fail.");
            delete pInstance_;
            pInstance_ = nullptr;
            return nullptr;
        }

        ST_LOG_INFO("Create Object Succ.");
        PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

    void UptLidarData(string strLidarName, SEGMENT_RESULT_STRU& stLidarSeg);

    

    void GetAllLidarData(std::unordered_map<string, SEGMENT_RESULT_STRU>& umLidarSeg);
    void GetAllLidarData(PointCloudXYZI::Ptr pCloud);
    void GetAllLidarGroundData(PointCloudXYZI::Ptr pCloud);
    void GetAllLidarNoGroundData(PointCloudXYZI::Ptr pCloud);

    int  GetLidarData(string strLidarName, SEGMENT_RESULT_STRU& stLidarSeg);
    int  GetLidarData(string strLidarName, PointCloudXYZI::Ptr pCloud);
    int  GetLidarGroundData(string strLidarName, PointCloudXYZI::Ptr pCloud);
    int  GetLidarNoGroundData(string strLidarName, PointCloudXYZI::Ptr pCloud);
    int  GetLidarValidData(const string& strLidarName, SEGMENT_RESULT_STRU& stLidarSeg);

    int  MergeLidarStamp(const unordered_map<string, SEGMENT_RESULT_STRU>& umLidarSegs, uint64_t &stamp, const int type);

    /* cloud pertistence */
    void UptLidarData(string strLidarName, const std::vector<HIT_MARK_IDX> &vtrHitMarkIdx);
    int  GetLidarData(string strLidarName, std::vector<HIT_MARK_IDX>& vtrHitMarkIdx);
    int  GetLidarValidData(const string& strLidarName, std::vector<HIT_MARK_IDX>& vtrHitMarkIdx);


private:
    std::recursive_mutex            mutex_;                 // 递归锁
    static std::recursive_mutex     mutex_r_;
    static PCPT_LIDAR_DATA_C*       pInstance_;
    std::unordered_map<string, SEGMENT_RESULT_STRU>         umLidarSeg_;
    std::unordered_map<string, ABNORMAL_PRINT_C>            umLidarAbLog_;

    /* cloud pertistence */
    std::unordered_map<string, std::vector<HIT_MARK_IDX>>   umLidarHitMarkIdx_;

    int Init();
};

/**************************************************************************************
功能描述: 感知融合数据
修改记录:
**************************************************************************************/
class PCPT_FUSED_DATA_C
{
public:
    PCPT_FUSED_DATA_C(){}
    PCPT_FUSED_DATA_C(const PCPT_FUSED_DATA_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_FUSED_DATA_C &operator=(const PCPT_FUSED_DATA_C &) = delete;   // 禁止使用拷贝赋值运算符

    static PCPT_FUSED_DATA_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_FUSED_DATA_C();
            PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
            ST_LOG_INFO("Create Object Succ.");
        }

        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

    void UptPcdNoGround(PointCloudXYZI::Ptr pCloud);
    int  GetPointCloud(PointCloudXYZ::Ptr &pCloud);
    int  GetPointCloud(PointCloudXYZI::Ptr &pCloud);

private:
    PointCloudXYZI                  objFusedPointCloud_;
    TIMER_C                         objTimer_{1.0, false};
    std::recursive_mutex            mutex_;         // 递归锁
    static std::recursive_mutex     mutex_r_;
    static PCPT_FUSED_DATA_C*       pInstance_;
};

/**************************************************************************************
功能描述: 感知障碍数据
修改记录:
**************************************************************************************/
class PCPT_OBST_DATA_C
{
public:
    PCPT_OBST_DATA_C(void){ }
    PCPT_OBST_DATA_C(const PCPT_OBST_DATA_C &other)       = delete;   // 禁止使用拷贝构造函数
    PCPT_OBST_DATA_C &operator=(const PCPT_OBST_DATA_C &) = delete;   // 禁止使用拷贝赋值运算符

    static PCPT_OBST_DATA_C *GetInstance(UINT32 frameNum = 1)
    {
        if (frameNum_ < frameNum) {
            frameNum_ = frameNum;
            dueTime_  = uint64_t(2.0 * frameNum_ / PCPT_HZ_DET_STATIC * TEN_6);
        }

        std::unique_lock<std::recursive_mutex> lock(mutex_r_);
        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_OBST_DATA_C();
            PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
            ST_LOG_INFO("Create Object Succ.");
        }

        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);
        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

    void  Upt2dObstData(PCPT_OBST_STRU& stObstByMap);
    void  Get2dObstData(PCPT_OBST_STRU& stObstByMap) const;

private:
    void UptBySingleFrame(PCPT_OBST_STRU& stObstByMap);
    void UptByMultiFrame(PCPT_OBST_STRU& stObstByMap);
    void CacheData(const PCPT_OBST_STRU& stObstByMap,
                   std::list<PCPT_OBST_STRU>& lstObstByMap) const;
    void UptLabelVector(const PCPT_OBST_STRU& stObstByMap,
                        std::vector<OBST_LABEL_ENUM>& vstLabel) const;
    void MergeData(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer,
                   const std::list<PCPT_OBST_STRU>& lstObstByMap,
                   MULTILAYER_OBST_STRU& stLayerObstByMap);

private:
    PCPT_OBST_STRU                  stObstByMap_;
    PCPT_OBST_STRU                  stObstByMapUpt_;
    std::list<PCPT_OBST_STRU>       lstObstByMap_;
    std::vector<OBST_LABEL_ENUM>    vstLabel_;      //数据类别
    PointCloudXYZ::Ptr              pCloudObst_{new PointCloudXYZ};
    mutable std::recursive_mutex    mutex_;              // 递归锁

    static std::atomic_uint         frameNum_;           // 缓存帧数
    static std::atomic_ullong       dueTime_;            // 数据有效时常(us)
    static std::recursive_mutex     mutex_r_;
    static PCPT_OBST_DATA_C*        pInstance_;
};

