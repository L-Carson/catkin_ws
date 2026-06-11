
#ifndef __COMM_TIME_CPP__
#define __COMM_TIME_CPP__

#include "comm/comm_time.h"
#include "comm/comm_dbg_log.h"

#include "time.h"
#include <sys/time.h>
#include <stdio.h>
#include <iostream>

#include <math.h>
#include <string.h>
#include <list>
#include <vector>

using namespace std;

/**
 * @brief 运行周期数据结构
 *
 */
struct RUN_PERIOD_STRU {
    int    runFrequency;
    double runPeriod;

    /**************************************************************************************
    功能描述: 根据时长获取运行周期数，时长单位秒
    修改记录:
    **************************************************************************************/
    int GetCycleNum(double timeSec) const
    {
        return (int)((timeSec + 0.5 * runPeriod) * runFrequency);
    }

    /**************************************************************************************
    功能描述: 根据周期数获取时长
    修改记录:
    **************************************************************************************/
    double GetCyclesLen(int cycleNum) const
    {
        return cycleNum * runPeriod;
    }
};

RUN_PERIOD_STRU g_objRunPeriod = {1, 1.0};   // 运行周期数据缓存（用于方便定时器计算）
UPT_MNG_C       g_objUptMng;                 // 更新管理器（用于维护所有定时器）

/**************************************************************************************
功能描述: 初始化定时器
修改记录:
**************************************************************************************/
INT32 Comm_InitTimer(int runFrequency)
{
    R_ASSERT_1(runFrequency > 0, -1, runFrequency);
    R_ASSERT(g_objUptMng.IsNull(), -1);

    g_objRunPeriod.runFrequency = runFrequency;
    g_objRunPeriod.runPeriod    = 1.0 / (double)runFrequency;

    return 0;
}

/**************************************************************************************
功能描述: 更新定时器
修改记录:
**************************************************************************************/
void Comm_UptTimer(void)
{
    g_objUptMng.UptAll();
}

/**************************************************************************************
功能描述: 获取时间，用到1970.1.1.0.0.0的秒来计算
修改记录:
**************************************************************************************/
double Comm_GetTimeSec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;
}

/**************************************************************************************
功能描述: 获取系统时间，避免频繁调用
修改记录:
**************************************************************************************/
void Comm_GetTime(ST_TIME_STRU &stTime)
{
    time_t t_time;
    time(&t_time);
    //struct tm *pstTm = gmtime(&t_time);
    struct tm *pstTm = localtime(&t_time);
    if (pstTm == NULL) {
        stTime.year  = 0;
        stTime.month = 0;
        stTime.day   = 0;
        stTime.hour  = 0;
        stTime.min   = 0;
        stTime.sec   = 0;
        return;
    }

    stTime.year  = pstTm->tm_year + 1900;
    stTime.month = pstTm->tm_mon + 1;
    stTime.day   = pstTm->tm_mday;
    stTime.hour  = pstTm->tm_hour;
    stTime.min   = pstTm->tm_min;
    stTime.sec   = pstTm->tm_sec;
}

/**************************************************************************************
功能描述: 获取系统时间字符串，避免频繁调用
修改记录:
**************************************************************************************/
void Comm_GetTimeStr(CHAR acTime[], UINT32 maxTimeLen)
{
    ST_TIME_STRU stTime;
    Comm_GetTime(stTime);

    snprintf(acTime, maxTimeLen, "%02u%02u-%02u:%02u:%02u", stTime.month, stTime.day, stTime.hour, stTime.min, stTime.sec);
}

/**************************************************************************************
功能描述: 获取系统时间字符串，避免频繁调用
修改记录:
**************************************************************************************/
std::string Comm_GetTimeStr(void)
{
    CHAR acTime[100];
    Comm_GetTimeStr(acTime, sizeof(acTime));

    return acTime;
}

std::string Comm_GetTimeStr1(void)
{
    ST_TIME_STRU stTime;
    Comm_GetTime(stTime);

    CHAR acTime[100];
    snprintf(acTime, sizeof(acTime), "%02u_%02u-%02u_%02u_%02u", stTime.month, stTime.day, stTime.hour, stTime.min, stTime.sec);

    return acTime;
}

std::string Comm_GetTimeStr2(void)
{
    ST_TIME_STRU stTime;
    Comm_GetTime(stTime);

    CHAR acTime[100];
    snprintf(acTime, sizeof(acTime), "%4u-%02u-%02u %02u:%02u:%02u", stTime.year, stTime.month, stTime.day, stTime.hour, stTime.min, stTime.sec);

    return acTime;
}

std::string Comm_ConvertTimeSecToTimeStr2(double timeSec)
{
    time_t timeSecInt = static_cast<time_t>(timeSec);

    struct tm *pstTm = localtime(&timeSecInt);
    R_ASSERT(pstTm != nullptr, "0000-00-00 00:00:00");

    CHAR acTime[100];
    snprintf(acTime, sizeof(acTime), "%4u-%02u-%02u %02u:%02u:%02u", pstTm->tm_year + 1900, pstTm->tm_mon + 1, pstTm->tm_mday, pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);

    return std::string(acTime);
}

double Comm_ConvertTimeStr2ToTimeSec(const std::string &strTimeStr)
{
    /* 先解析时间格式 */
    /* 输入时间格式必须为 yyyy-MM-dd HH:mm:ss          */
    std::tm            stTime;
    std::istringstream objInputTimeStringStream(strTimeStr);
    objInputTimeStringStream >> std::get_time(&stTime, "%Y-%m-%d %H:%M:%S");
    if (objInputTimeStringStream.fail()) {
        return -1.0;
    }

    /* 再转换为1970年1月1日以来的秒数 */
    std::time_t timeSinceEpoch = std::mktime(&stTime);
    return double(timeSinceEpoch);
}

void UPT_MNG_C::Add(UPT_BASE_C *pobjUpt)
{
    if (pobjUpt == NULL) {
        return;
    }

    objMutex_.lock();
    lstUpt_.push_back(pobjUpt);
    ST_LOG_INFO("Obj Reg Count : %u.", UINT32(lstUpt_.size()));
    objMutex_.unlock();
}

void UPT_MNG_C::Rmv(UPT_BASE_C *pobjUpt)
{
    if (pobjUpt == NULL) {
        return;
    }

    objMutex_.lock();
    lstUpt_.remove(pobjUpt);
    ST_LOG_INFO("Obj Reg Count : %u.", UINT32(lstUpt_.size()));
    objMutex_.unlock();
}

void UPT_MNG_C::UptAll(void)
{
    objMutex_.lock();

    for (list<UPT_BASE_C *>::iterator itor = lstUpt_.begin();
         itor != lstUpt_.end();
         itor++) {
        UPT_BASE_C *pobjUpt = (UPT_BASE_C *)(*itor);
        pobjUpt->Upt();
    }

    objMutex_.unlock();
}

BOOL UPT_MNG_C::IsNull(void)
{
    return lstUpt_.empty();
}

TIMER_C::TIMER_C(void)
{
    g_objUptMng.Add(this);

    leftTimeCycleNum_ = 0;

    SetOverTime(0);
    Clear(0);
}

TIMER_C::TIMER_C(int overTimeCycleNum, BOOL isDefaultTimeOut, BOOL isDefaultClosed)
{
    g_objUptMng.Add(this);

    SetOverTime(overTimeCycleNum);

    if (isDefaultClosed) {
        Close();
    } else {
        if (isDefaultTimeOut) {
            Clear(0);
        } else {
            Reset();
        }
    }

}

TIMER_C::TIMER_C(double overTime, BOOL isDefaultTimeOut, BOOL isDefaultClosed)
{
    g_objUptMng.Add(this);

    SetOverTime(overTime);

    if (isDefaultClosed) {
        Close();
    } else {
        if (isDefaultTimeOut) {
            Clear(0);
        } else {
            Reset();
        }
    }
}

TIMER_C::~TIMER_C(void)
{
    g_objUptMng.Rmv(this);
}

void TIMER_C::SetOverTime(int overTimeCycleNum)
{
    overTime_         = g_objRunPeriod.GetCyclesLen(overTimeCycleNum);
    overTimeCycleNum_ = overTimeCycleNum;
}

void TIMER_C::SetOverTime(double overTime)
{
    overTime_         = overTime;
    overTimeCycleNum_ = g_objRunPeriod.GetCycleNum(overTime);
}

void TIMER_C::Upt(void)
{
    if (leftTimeCycleNum_ > 0) {
        leftTimeCycleNum_--;
    }
}

void TIMER_C::Reset(void)
{
    if (overTimeCycleNum_ == 0) {
        overTimeCycleNum_ = g_objRunPeriod.GetCycleNum(overTime_);
    }

    leftTimeCycleNum_ = overTimeCycleNum_;
}

void TIMER_C::Reset(int overTimeCycleNum)
{
    SetOverTime(overTimeCycleNum);
    Reset();
}

void TIMER_C::Reset(double overTime)
{
    SetOverTime(overTime);
    Reset();
}

void TIMER_C::ResetAndAddTime(double addTime)
{
    int addTimeCycleNum = g_objRunPeriod.GetCycleNum(addTime);

    Reset(addTimeCycleNum + leftTimeCycleNum_);
}

void TIMER_C::Clear(void)
{
    leftTimeCycleNum_ = 0;
}

void TIMER_C::Clear(int leftTimeCycleNum)
{
    int leftTimeCycleNumTmp = MAX(leftTimeCycleNum, 0);

    if (leftTimeCycleNumTmp < leftTimeCycleNum_) {
        leftTimeCycleNum_ = leftTimeCycleNumTmp;
    }
}

void TIMER_C::Clear(double leftTime)
{
    int leftTimeCycleNum = g_objRunPeriod.GetCycleNum(leftTime);
    Clear(leftTimeCycleNum);
}

BOOL TIMER_C::IsTimeOut(void) const
{
    return (leftTimeCycleNum_ == 0);
}

void TIMER_C::Close(void)
{
    leftTimeCycleNum_ = -1;
}

BOOL TIMER_C::IsClosed(void) const
{
    return (leftTimeCycleNum_ < 0);
}

// 周期统计
void CYCLE_LEN_STATIS_C::PeriodUpt(void)
{
    if (!objOutputTimer_.IsTimeOut()) {
        return;
    }

    objOutputTimer_.Reset();

    if (cycleCount_ != 0) {
        // 打印：     描述  周期计数   平均周期时长   最小周期时长   最大周期时长    异常周期比率
        ST_LOG_INFO("[%s]Count=%u, avgLen=%.4f, minLen=%.4f, maxLen=%.4f, abnRate=%.2f.",
                    strDescription_.c_str(),
                    cycleCount_,
                    totalCycleLen_ / double(cycleCount_),
                    minCycleLen_,
                    maxCycleLen_,
                    double(abnCycleCount_) / double(cycleCount_));
    } else {
        ST_LOG_INFO("[%s]Count=%u.",
                    strDescription_.c_str(),
                    cycleCount_);
    }

    ClearStatis();
}

// 统计周期内迭代一次
void CYCLE_LEN_STATIS_C::Add(void)
{
    double curTime = Comm_GetTimeSec();
    if (lastCycleTime_ <= 0.0) {
        lastCycleTime_ = curTime;
        return;
    }

    double curCycleLen = curTime - lastCycleTime_;
    lastCycleTime_     = curTime;

    ++cycleCount_;
    maxCycleLen_       = MAX(maxCycleLen_, curCycleLen);
    minCycleLen_       = MIN(minCycleLen_, curCycleLen);
    totalCycleLen_    += curCycleLen;

    double curCycleDiff = ABS(curCycleLen - expectCycleLen_);
    if (curCycleDiff > abnCycleDiffMax_) {
        ++abnCycleCount_;
        ST_LOG_ERR("[%s]Expect Cycle Len Is %f, Cur Cycle Len(%f) Abnormal.", strDescription_.c_str(), expectCycleLen_, curCycleLen);
    }
}

void CYCLE_LEN_STATIS_C::Clear(void)
{
    objOutputTimer_.Reset();

    lastCycleTime_  = 0;

    ClearStatis();
}

// 复位统计周期参数
void CYCLE_LEN_STATIS_C::ClearStatis(void)
{
    cycleCount_     = 0;
    maxCycleLen_    = 0.0;
    minCycleLen_    = 1.0e10;
    totalCycleLen_  = 0.0;
    abnCycleCount_  = 0;
}

void COST_TIME_STATIS_C::Begin(double curTime)
{
    if (beginTime_ != 0.0) {
        ST_LOG_FATAL("[%s]Not End.", strDes_.c_str());
    }

    double curTimeE = (curTime > 0) ? curTime : Comm_GetTimeSec();

    beginTime_ = curTimeE;
}

void COST_TIME_STATIS_C::End(double curTime)
{
    double curTimeE = (curTime > 0) ? curTime : Comm_GetTimeSec();
    double curCostTime = curTimeE - beginTime_;
    beginTime_         = 0.0;

    minCostTime_       = MIN(minCostTime_, curCostTime);
    maxCostTime_       = MAX(maxCostTime_, curCostTime);
    sumOfCostTime_    += curCostTime;
    ++repeatCount_;

    Output(curTimeE);
}

void COST_TIME_STATIS_C::Clear(void)
{
    minCostTime_   = 1.0e10;
    maxCostTime_   = 0.0;
    sumOfCostTime_ = 0.0;
    repeatCount_   = 0;
}

void COST_TIME_STATIS_C::Output(double curTime)
{
    if ((curTime < (lastOutputTime_ + outputCycle_)) && (repeatCount_ < outputCount_)) {
        return;
    }

    double avgCostTime = (repeatCount_ != 0) ? (sumOfCostTime_ / double(repeatCount_)) : 0.0;

    if (isOutputBySec_) {
        ST_LOG_INFO("[%s]Count=%3u, Avg=%6.3fs, Max=%6.3fs, Min=%6.3fs.", strDes_.c_str(), repeatCount_, avgCostTime, maxCostTime_, minCostTime_);
    } else {
        ST_LOG_INFO("[%s]Count=%3u, Avg=%8.3fms, Max=%8.3fms, Min=%8.3fms.", strDes_.c_str(), repeatCount_, avgCostTime * 1000.0, maxCostTime_ * 1000.0, minCostTime_ * 1000.0);
    }

    Clear();
    lastOutputTime_ = curTime;
}

void OPERATION_CYCLE_CTRL_C::Start(void)
{
    /* 统计最大起始工作时间间隔 */
    double curTime = Comm_GetTimeSec();
    if (startWorkTime_ != 0) {
        double startWorkTimeDiff = curTime - startWorkTime_;
        maxStartTimeDiff_ = MAX(maxStartTimeDiff_, startWorkTimeDiff);
    }

    startWorkTime_ = curTime;
}

void OPERATION_CYCLE_CTRL_C::Stop(BOOL isSucc)
{
    R_ASSERT_1(startWorkTime_ > 0, NOTHING, startWorkTime_);

    double endTime   = Comm_GetTimeSec();

    AddStatis(endTime, isSucc);

    OutputStatis(endTime);
}

BOOL OPERATION_CYCLE_CTRL_C::IsWorkPeriodTimeout(void)
{
    return ((Comm_GetTimeSec() - startWorkTime_) >= workPeriodTime_);
}

void OPERATION_CYCLE_CTRL_C::Wait(void)
{
    usleep(int(stepSleepTime_ * 1000 * 1000));
}

void OPERATION_CYCLE_CTRL_C::ClearStatis(void)
{
    periodCount_              = 0;
    periodCountSucc_          = 0;
    succPeriodSpentTimeTotal_ = 0;
    succPeriodSpentTimeMax_   = 0;
    failPeriodSpentTimeMax_   = 0;
    maxStartTimeDiff_         = 0;
}

void OPERATION_CYCLE_CTRL_C::AddStatis(double curTime, BOOL isSucc)
{
    double spentTime = curTime - startWorkTime_;

    periodCount_++;
    if (isSucc) {
        periodCountSucc_++;
        succPeriodSpentTimeTotal_ += spentTime;
        succPeriodSpentTimeMax_    = MAX(succPeriodSpentTimeMax_, spentTime);
    } else {
        failPeriodSpentTimeMax_    = MAX(failPeriodSpentTimeMax_, spentTime);
    }
}

void OPERATION_CYCLE_CTRL_C::OutputStatis(double curTime)
{
    /* 1分钟输出一次 */
    if ((curTime - lastLogTime_) > logPeriodTime_) {
        lastLogTime_ = curTime;

        ST_LOG_INFO("[%s] TotalCount=%u, SuccRate=%4.2f, AvgSuccTime=%4.4fs, MaxSuccTime=%4.4fs, MaxFailTime=%4.4fs, maxStartWorkTimeDiff=%4.4fs.",
                    strModule_.c_str(),
                    periodCount_,
                    (periodCount_ == 0) ? 0.0 : (float(periodCountSucc_) / float(periodCount_)),
                    (periodCountSucc_ == 0) ? 0.0 : float(succPeriodSpentTimeTotal_ / float(periodCountSucc_)),
                    float(succPeriodSpentTimeMax_),
                    float(failPeriodSpentTimeMax_),
                    float(maxStartTimeDiff_));

        ClearStatis();
    }
}


#endif


