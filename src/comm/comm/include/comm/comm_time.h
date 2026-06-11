

#ifndef __COMM_TIME_H__
#define __COMM_TIME_H__
#include "comm/comm_data_type.h"

#include <string.h>
#include <atomic>
#include <mutex>
#include <list>
#include <functional>

using namespace std;

struct ST_TIME_STRU {
    unsigned short year;
    unsigned char  month;
    unsigned char  day;

    unsigned char  hour;
    unsigned char  min;
    unsigned char  sec;
};

INT32 Comm_InitTimer(int runFrequency);
void  Comm_UptTimer(void);

/**************************************************************************************
功能描述: 获取系统时间，避免频繁调用
修改记录:
**************************************************************************************/
double Comm_GetTimeSec(void);
void   Comm_GetTime(ST_TIME_STRU &stTime);
void   Comm_GetTimeStr(CHAR acTime[], UINT32 maxTimeLen);
std::string Comm_GetTimeStr(void);
std::string Comm_GetTimeStr1(void);

/**************************************************************************************
功能描述: 时间格式2处理
修改记录:
**************************************************************************************/
std::string Comm_GetTimeStr2(void);
std::string Comm_ConvertTimeSecToTimeStr2(double timeSec);
double      Comm_ConvertTimeStr2ToTimeSec(const std::string &strTimeStr);


class UPT_BASE_C
{
public:
    virtual void Upt(void) = 0;
    virtual ~UPT_BASE_C(){}
};

/**
 * @brief 更新管理器
 *
 */
class UPT_MNG_C
{
public:
    UPT_MNG_C(void)
    {
    }

    void Add(UPT_BASE_C *pobjUpt);
    void Rmv(UPT_BASE_C *pobjUpt);
    void UptAll(void);
    BOOL IsNull(void);
private:
    mutex              objMutex_;
    list<UPT_BASE_C *> lstUpt_;
};

/**
 * @brief 定时器（通过更新管理器维护周期迭代）
 *
 */
class TIMER_C : public UPT_BASE_C
{
public:
    TIMER_C(void);
    TIMER_C(int overTimeCycleNum, BOOL isDefaultTimeOut = true, BOOL isDefaultClosed = false);
    TIMER_C(double overTime, BOOL isDefaultTimeOut = true, BOOL isDefaultClosed = false);
    ~TIMER_C(void);

    TIMER_C(TIMER_C &&) = delete;                    // 禁止移动构造
    TIMER_C(const TIMER_C &) = delete;               // 禁止拷贝构造
    TIMER_C &operator=(const TIMER_C &) = delete;    // 禁止赋值构造

    void SetOverTime(int overTimeCycleNum);
    void SetOverTime(double overTime);
    void Upt(void);
    void Reset(void);
    void Reset(int overTimeCycleNum);
    void Reset(double overTime);
    void ResetAndAddTime(double addTime);
    void Clear(void);
    void Clear(int leftTimeCycleNum);
    void Clear(double leftTime);
    BOOL IsTimeOut(void) const;
    void Close(void);
    BOOL IsClosed(void) const;
protected:
private:
    double        overTime_;                   /* 定时器超时时间，为循环周期整倍数时准备，否则近似计算循环次数 */
    int           overTimeCycleNum_;           /* 定时器超时时间对应的循环周期数，优先使用 */
    atomic_int    leftTimeCycleNum_{0};        /* 多线程访问 *//* 离定时器超时剩余的循环周期数 */
};

/**************************************************************************************
功能描述: 周期时长统计
修改记录:
**************************************************************************************/
class CYCLE_LEN_STATIS_C
{
public:
    CYCLE_LEN_STATIS_C(const std::string &strDescription, double outputCycleLen, double expectCycleLen, double abnCycleDiffRate)
                                        : strDescription_(strDescription),
                                          outputCycleLen_(outputCycleLen),
                                          expectCycleLen_(expectCycleLen),
                                          abnCycleDiffMax_(expectCycleLen * abnCycleDiffRate),
                                          objOutputTimer_(outputCycleLen, false)
    {
        ClearStatis();
    }

    void PeriodUpt(void);
    void Add(void);
    void Clear(void);

private:
    const std::string   strDescription_;        // 描述信息
    const double        outputCycleLen_;        /* 输出统计信息周期时长 */
    const double        expectCycleLen_;        /* 预期周期时长，为0时不统计周期偏差比例 */
    const double        abnCycleDiffMax_;       /* 遗产周期误差限值 */

    TIMER_C             objOutputTimer_;        // 统计周期定时器
    double              lastCycleTime_{0.0};    // 上一周期时间戳[unit: s]

    UINT32              cycleCount_;            // 周期计数
    double              maxCycleLen_;           // 统计周期内最大周期时长[unit: s]
    double              minCycleLen_;           // 统计周期内最小周期时长[unit: s]
    double              totalCycleLen_;         // 统计周期内总计周期时长[unit: s]
    UINT32              abnCycleCount_;         // 统计周期内异常周期计数

    void ClearStatis(void);
};

/**************************************************************************************
功能描述: 耗费时长统计
修改记录:
**************************************************************************************/
class COST_TIME_STATIS_C
{
public:
    COST_TIME_STATIS_C(const std::string &strDes, double outputCycle, BOOL isOutputBySec = true) : strDes_(strDes),
                                                                                                   outputCycle_(outputCycle),
                                                                                                   outputCount_(UINT32_NULL),
                                                                                                   isOutputBySec_(isOutputBySec)
    {
        lastOutputTime_ = Comm_GetTimeSec();
    }

    COST_TIME_STATIS_C(const std::string &strDes, UINT32 outputCount, BOOL isOutputBySec = true) : strDes_(strDes),
                                                                                                   outputCycle_(24.0 * 3600.0),
                                                                                                   outputCount_(outputCount),
                                                                                                   isOutputBySec_(isOutputBySec)
    {
        lastOutputTime_ = Comm_GetTimeSec();
    }

    void Begin(double curTime = -1.0);
    void End(double curTime = -1.0);

private:
    const std::string strDes_;         // 描述信息
    const double      outputCycle_;    // 打印间隔时长[unit: s]
    const double      outputCount_;    // 打印间隔迭代次数
    const BOOL        isOutputBySec_;  // s单位打印标识

    double lastOutputTime_{0.0};       // 上次打印时间戳[unit: s]
    double beginTime_{0.0};            // 统计开始时间戳[unit: s]

    double minCostTime_{1.0e10};       // 最小耗费时长[unit: s]
    double maxCostTime_{0.0};          // 最大耗费时长[unit: s]
    double sumOfCostTime_{0.0};        // 总计耗费时长[unit: s]
    UINT32 repeatCount_{0};            // 迭代计数

    void Clear(void);
    void Output(double curTime);
};

/**************************************************************************************
功能描述: 运行周期控制
修改记录:
**************************************************************************************/
class OPERATION_CYCLE_CTRL_C
{
public:
    OPERATION_CYCLE_CTRL_C(const string &strModule, double periodTime = 1.0, double stepSleepTime = 0.005)
    {
        strModule_      = strModule;
        workPeriodTime_ = periodTime;
        logPeriodTime_  = 60.0;
        stepSleepTime_  = stepSleepTime;

        lastLogTime_    = Comm_GetTimeSec();
        startWorkTime_  = 0.0;

        ClearStatis();
    }

    void Start(void);
    void Stop(BOOL isSucc);
    void Wait(void);

    BOOL IsWorkPeriodTimeout(void);

private:
    string strModule_;                      /* 统计信息的模块名 */
    double workPeriodTime_;                 /* 周期时长 */
    double logPeriodTime_;                  /* 日志记录周期时长 */
    double stepSleepTime_;                  /* 单次睡眠时长 */

    double lastLogTime_;                    /* 上次日志的时间 */
    double startWorkTime_;

    /* 统计信息 */
    UINT32 periodCount_;
    UINT32 periodCountSucc_;
    double succPeriodSpentTimeTotal_;
    double succPeriodSpentTimeMax_;
    double failPeriodSpentTimeMax_;
    double maxStartTimeDiff_;

    void ClearStatis(void);
    void AddStatis(double curTime, BOOL isSucc);
    void OutputStatis(double curTime);
};



#endif

