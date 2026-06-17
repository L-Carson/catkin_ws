#ifndef __PCPT_COMM_BASE_H__
#define __PCPT_COMM_BASE_H__

#include "comm_extend/comm_base_extend.h"
#include "pcpt_comm/pcpt_comm_define.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <chrono>

#include <pcl_conversions/pcl_conversions.h>

extern std::atomic_int      g_move_trend;           //运动趋势
extern std::atomic_bool     g_detect_garbage;       //是否开启垃圾检测
extern std::atomic_bool     g_contact_edge;         //触边生成障碍

extern std::atomic_bool     g_butt_dustbin;         //是否对接垃圾箱
extern std::atomic_bool     g_dustbin_state;        //是否检测垃圾箱容量状态
extern std::atomic_bool     g_butt_charging_pile;   //是否对接充电桩
extern std::atomic_bool     g_butt_fixed_site;      //是否对接固定点（固定点倒垃圾）

extern std::atomic_int      g_blindLidarType;       //补盲雷达类型
extern std::atomic_ullong   g_lidar_stamp;          //雷达时间戳
extern std::atomic_bool     g_labelPile;            //是否标记充电桩
extern std::atomic_bool     g_isWork;               //是否工作
extern std::atomic_bool     g_isSim;                //是否仿真
extern std::atomic_bool     g_isRestartedPnp;       //是否重启了感知（规控）

extern double               g_gravity;              //重力加速度
extern POINT_TPYE_ENUM      g_pointType;            //点云数据类型

extern std::atomic_uchar    g_v_det_pipeline_state; //图像检测 - 水管管线存在状态
extern std::atomic_uchar    g_v_seg_pipeline_state; //图像分割 - 水管管线存在状态
extern std::atomic_bool     g_v_has_risk;           //当前图像视野是否存在风险

/**************************************************************************************
功能描述: 私有点云数据结构体（并不能带来性能的提升，反而有所下降）
修改记录:
**************************************************************************************/
template<typename PointT>
struct PCPT_CLOUD_STRU {
    typedef typename pcl::PointCloud<PointT> PointCloud;
    typedef typename pcl::PointCloud<PointT>::Ptr PointCloudPtr;

    void clear() {
        size_ = 0;
        if (pCloud) pCloud->clear();
    }

    void push_back(const PointT &point) {
        if (size_ < uint32_t(pCloud->size()))
            pCloud->points[size_] = point;
        else
            pCloud->push_back(point);
        size_++;
    }

    uint32_t size() {
        return size_;
    }

    void resize(uint32_t size_) {
        this->size_ = size_;
        pCloud->resize(size_);
    }

    PointT& operator [] (const uint32_t index) const {
        return pCloud->points[index];
    }

    uint32_t        size_{0};
    PointCloudPtr   pCloud{new PointCloud};
    pcl::PCLHeader& header{pCloud->header};
};

struct OBJ_INFO_STRU {
    int   id;
    int   type;
    float xDirection;
    float transversalPos;
    float longitudinalPos;
    float speed;
    float length;
    float width;
    float height;
};

struct FRAME_OBJ_INFO_STRU {
    double timestampGNSS;
    int    id;
    std::vector<OBJ_INFO_STRU> vstObjInfo;
};

struct TRAFFIC_LIGHT_STATE_INFO_STRU {
    double timestampGNSS;
    int    vehPhase;
    int    vehLight;
};


/**
 * @brief 并行for循环函数
 * @tparam Func 模板类
 */
template<typename Func>
void parallel_for(int start, int end, Func func, int num_threads = 0) {
    // 确定线程数（默认使用硬件支持的最大线程数）
    if (num_threads <= 0) {
        num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 2; // 回退到默认值
    }

    // 计算每个线程处理的迭代范围
    const int total_iterations = end - start + 1;
    const int chunk_size = std::max(1, total_iterations / num_threads);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        // 计算当前线程的起始和结束索引
        int thread_start = start + t * chunk_size;
        int thread_end = std::min(thread_start + chunk_size - 1, end);

        // 创建线程处理当前区间的迭代
        threads.emplace_back([=]() {
            for (int i = thread_start; i <= thread_end; ++i) {
                func(i); // 执行用户定义的循环体
            }
        });
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

/**
 * @brief 删除指针
 * @tparam T 模板类
 * @param ptr 指针
 */
template<typename T>
void DelPtr(T*& ptr) {
    if (ptr) {
        delete ptr;
        ptr = nullptr;
    }
}

template<typename T>
void DelArrayPtr(T*& ptr) {
    if (ptr) {
        delete[] ptr;
        ptr = nullptr;
    }
}

/**
 * @brief 返回多雷达时间戳的融合结果
 * @return double 时间戳[unit: s]
 */
double LidarStamp();

/**
 * @brief 返回点云数据的时间戳
 * @tparam T 模板类
 * @param pclPtr pcl点云智能指针
 * @return double 时间戳[unit: s]
 */
template<typename T>
double PclStamp(const T& pclPtr) {
    if (pclPtr) return pclPtr->header.stamp * ZERO_6; // us-->s
    else        return NAN;
}

template<typename T>
uint64_t PclStampInt(const T& pclPtr) {
    if (pclPtr) return pclPtr->header.stamp;
    else        return 0;
}

double   PclStamp(const pcl::PCLHeader &header);
uint64_t PclStampInt(const pcl::PCLHeader &header);

/**
 * @brief 返回ros消息的时间戳
 * @tparam T 模板类
 * @param rosPtr ros消息智能指针
 * @return double 时间戳[unit: s]
 */
template<typename T>
double RosStamp(const T& rosPtr) {
    if (rosPtr) return rosPtr->header.stamp.toSec();
    else        return NAN;
}

template<typename T>
uint64_t RosStampInt(const T& rosPtr) {
    if (rosPtr) return uint64_t(rosPtr->header.stamp.toSec() * TEN_6);
    else        return 0;
}
uint64_t  RosStampInt(const std_msgs::Header& header);

double    RosStamp(const ros::Time stamp);
double    RosStamp(const std_msgs::Header& header);
double    RosStampNow();

ros::Time RosTime (const double stamp);
ros::Time RosTimeNow();

/**
 * @brief 返回ros header
 * @tparam T 模板类
 * @param pclPtr pcl点云智能指针
 * @return double 时间戳[unit: s]
 */
template<typename T>
std_msgs::Header RosHeaderFromPcl(const T& pclPtr) {
    std_msgs::Header header;
    pcl_conversions::fromPCL(pclPtr->header, header);
    return header;
}

std_msgs::Header RosHeader(const char* frame_id = PCPT_FRAME_ID_CAR);

void PrintCostTime(const std::vector<double> &vecTimes, const std::string &strMark, const float printThr);
void PrintCostTime(const std::vector<std::chrono::time_point<std::chrono::steady_clock> > &vecTimes, 
                   const std::string &strMark, const float printThr);

class DELAY_STATISTICS_C
{
public:
    DELAY_STATISTICS_C(const std::string &strMark, const double statisticsPeriod = 60.0)
        : strMark_(strMark), statisticsPeriod_(statisticsPeriod)
    {

    };

    void DelayStatistics(const double currXxxTime)
    {
        double currTime = RosStampNow();
        if (prevPrintTime_ < 0.0) {
            prevPrintTime_ = currTime;
        }
        vecDelayTime_.push_back(currTime - currXxxTime);

        if (currTime - prevPrintTime_ > statisticsPeriod_) {
            prevPrintTime_ = currTime;
            double min =  DBL_MAX;
            double max = -DBL_MAX;
            double avg = 0;
            for (double delayTime : vecDelayTime_) {
                min = MIN(min, delayTime);
                max = MAX(max, delayTime);
                avg += delayTime;
            }
            avg /= vecDelayTime_.size();
            ST_LOG_INFO("[%s] 延时统计(%lu/%f s)：最小(%.3f) 最大(%.3f) 平均(%.3f)",
                strMark_.c_str(), vecDelayTime_.size(), statisticsPeriod_, min, max, avg);
            vecDelayTime_.clear();
        }
    }

private:
    std::string         strMark_;
    double              statisticsPeriod_{0.0};
    double              prevPrintTime_{-1.0};
    std::vector<double> vecDelayTime_;
};


#endif
