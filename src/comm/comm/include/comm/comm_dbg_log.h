#ifndef __COMM_DBG_LOG_H__
#define __COMM_DBG_LOG_H__
#include "comm/comm_data_type.h"
#include "comm/comm_sys.h"
#include "comm/comm_time.h"
#include <mutex>


using namespace std;

extern mutex g_objLogMutex;

extern FILE *pLogFileHandle;

#define CHECK_TRUE(condition_)                          ((condition_) ? 0 : -1)
#define CHECK_PTR(ptr_)                                 CHECK_TRUE(ptr_ != NULL)

INT32  InitXXXLog(const std::string &strLogFilePath);
/**************************************************************************************
功能描述: 普通日志
          日志格式: 线程号        时间   日志类型 函数名 日志信息
          打印后，清除缓存，保证能实时输出，注意其中逗号不要修改成分号，注意使用##去掉变参个数为0时前面的多余的逗号
          为保证多线程下日志输出不乱，需要加锁
修改记录:
**************************************************************************************/
#define ST_LOG(strLogType_, strFmt_, ...)                                                \
    do {                                                                                 \
        g_objLogMutex.lock();                                                            \
        CHAR acTime[100];                                                                \
        Comm_GetTimeStr(acTime, sizeof(acTime));                                         \
        CHAR acFuncName[200];                                                            \
        FILE *pLogFileHandletmp = (pLogFileHandle == nullptr) ? stdout : pLogFileHandle; \
        Comm_GetFuncName(__PRETTY_FUNCTION__, acFuncName, sizeof(acFuncName));           \
        fprintf(pLogFileHandletmp, "[%8u] %s [" strLogType_ "]%s: " strFmt_ "\n",        \
               GetTid(),                                                                 \
               acTime,                                                                   \
               acFuncName,                                                               \
               ##__VA_ARGS__);                                                           \
        fflush(pLogFileHandletmp);                                                       \
        g_objLogMutex.unlock();                                                          \
    } while (0)

#define ST_LOG_ERR(  strFmt_, ...)              ST_LOG("ERR",     strFmt_, ##__VA_ARGS__)
#define ST_LOG_INFO( strFmt_, ...)              ST_LOG("INFO",    strFmt_, ##__VA_ARGS__)
#define ST_LOG_WARN( strFmt_, ...)              ST_LOG("WARN",    strFmt_, ##__VA_ARGS__)
#define ST_LOG_FATAL(strFmt_, ...)              ST_LOG("FATAL",   strFmt_, ##__VA_ARGS__)

#define ST_LOG_RSLT(rslt_, strFmt_, ...)                        \
    if (rslt_ != 0) {                                           \
        ST_LOG_ERR(strFmt_" Fail.", ##__VA_ARGS__);             \
    } else {                                                    \
        ST_LOG_INFO(strFmt_" Succ.", ##__VA_ARGS__);            \
    }

/**************************************************************************************
功能描述: 条件日志
修改记录:
**************************************************************************************/
#define ST_LOG_INFO_IF( sw_, strFmt_, ...)      if (sw_) { ST_LOG_INFO( strFmt_, ##__VA_ARGS__); }
#define ST_LOG_WARN_IF( sw_, strFmt_, ...)      if (sw_) { ST_LOG_WARN( strFmt_, ##__VA_ARGS__); }
#define ST_LOG_ERR_IF(  sw_, strFmt_, ...)      if (sw_) { ST_LOG_ERR(  strFmt_, ##__VA_ARGS__); }
#define ST_LOG_FATAL_IF(sw_, strFmt_, ...)      if (sw_) { ST_LOG_FATAL(strFmt_, ##__VA_ARGS__); }


#define ST_LOG_INFO_IF_CHANGE(val_, strFmt_, ...)               \
    do {                                                        \
        static INT32 lastVal = 0;                               \
        if (INT32(val_) != lastVal) {                           \
            ST_LOG_INFO(strFmt_, ##__VA_ARGS__);                \
        }                                                       \
        lastVal = INT32(val_);                                  \
    } while (0)

/**************************************************************************************
功能描述: 采样日志
修改记录:
**************************************************************************************/
#define ST_LOG_SAMPLE(sampleTimes_, strLogType_, strFmt_, ...)      \
    do {                                                            \
        static UINT32 outputTimes_ = sampleTimes_;                  \
        ++outputTimes_;                                             \
        if (outputTimes_ >= (sampleTimes_)) {                       \
            outputTimes_ = 0;                                       \
            ST_LOG(strLogType_, strFmt_, ##__VA_ARGS__);            \
        }                                                           \
    } while(0)

#define ST_LOG_ERR_SAMPLE( sampleTimes_, strFmt_, ...)  ST_LOG_SAMPLE(sampleTimes_, "ERR",  strFmt_, ##__VA_ARGS__)
#define ST_LOG_INFO_SAMPLE(sampleTimes_, strFmt_, ...)  ST_LOG_SAMPLE(sampleTimes_, "INFO", strFmt_, ##__VA_ARGS__)




/**************************************************************************************
功能描述: 条件采样日志
修改记录:
**************************************************************************************/
#define ST_LOG_SAMPLE_IF_TRUE(sw_, sampleTimes_, strLogType_, strFmt_, ...)     \
    do {                                                                        \
        static BOOL   swOldVal     = false;                                     \
        static UINT32 outputTimes_ = (sampleTimes_);                            \
        BOOL swNewVal = (sw_);                                                  \
        if (swOldVal != swNewVal) {                                             \
            if (swNewVal) {                                                     \
                ST_LOG(strLogType_, strFmt_ " Begin.", ##__VA_ARGS__);          \
                outputTimes_ = 0;                                               \
            } else {                                                            \
                ST_LOG(strLogType_, strFmt_ " End.", ##__VA_ARGS__);            \
            }                                                                   \
            swOldVal = swNewVal;                                                \
        }                                                                       \
        if (swOldVal) {                                                         \
            ++outputTimes_;                                                     \
            if (outputTimes_ >= (sampleTimes_)) {                               \
                outputTimes_ = 0;                                               \
                ST_LOG(strLogType_, strFmt_, ##__VA_ARGS__);                    \
            }                                                                   \
        }                                                                       \
    } while(0)

#define ST_LOG_INFO_SAMPLE_IF_TRUE( sw_, sampleTimes_, strFmt_, ...)    ST_LOG_SAMPLE_IF_TRUE(sw_, sampleTimes_, "INFO",  strFmt_, ##__VA_ARGS__)
#define ST_LOG_ERR_SAMPLE_IF_TRUE(  sw_, sampleTimes_, strFmt_, ...)    ST_LOG_SAMPLE_IF_TRUE(sw_, sampleTimes_, "ERR",   strFmt_, ##__VA_ARGS__)
#define ST_LOG_FATAL_SAMPLE_IF_TRUE(sw_, sampleTimes_, strFmt_, ...)    ST_LOG_SAMPLE_IF_TRUE(sw_, sampleTimes_, "FATAL", strFmt_, ##__VA_ARGS__)

#define ST_LOG_ERR_SAMPLE_IF_FAIL(rslt_, sampleTimes_, strFmt_, ...)    ST_LOG_ERR_SAMPLE_IF_TRUE(rslt_ != 0, sampleTimes_, strFmt_, ##__VA_ARGS__)



/**************************************************************************************
功能描述: 定时周期日志
修改记录:
**************************************************************************************/
#define ST_LOG_TIMER(objTimer_, strLogType_, strFmt_, ...)          \
    if (objTimer_.IsTimeOut()) {                                    \
        objTimer_.Reset();                                          \
        ST_LOG(strLogType_, strFmt_, ##__VA_ARGS__);                \
    }                                                               \

#define ST_LOG_INFO_TIMER(objTimer_, strFmt_, ...)                      ST_LOG_TIMER(objTimer_, "INFO",  strFmt_, ##__VA_ARGS__)
#define ST_LOG_ERR_TIMER(objTimer_, strFmt_, ...)                       ST_LOG_TIMER(objTimer_, "ERR",   strFmt_, ##__VA_ARGS__)
#define ST_LOG_FATAL_TIMER(objTimer_, strFmt_, ...)                     ST_LOG_TIMER(objTimer_, "FATAL", strFmt_, ##__VA_ARGS__)



/**************************************************************************************
功能描述: 条件定时周期日志
修改记录:
**************************************************************************************/
#define ST_LOG_TIMER_IF(sw_, objTimer_, strLogType_, strFmt_, ...)          \
    if (sw_) {                                                              \
        if (objTimer_.IsClosed()) {                                         \
            objTimer_.Reset();                                              \
            ST_LOG(strLogType_, strFmt_ " Begin.", ##__VA_ARGS__);          \
        } else if (objTimer_.IsTimeOut()) {                                 \
            objTimer_.Reset();                                              \
            ST_LOG(strLogType_, strFmt_, ##__VA_ARGS__);                    \
        } else {                                                            \
        }                                                                   \
    } else {                                                                \
        if (!objTimer_.IsClosed()) {                                        \
            objTimer_.Close();                                              \
            ST_LOG(strLogType_, strFmt_ " End.", ##__VA_ARGS__);            \
        }                                                                   \
    }

#define ST_LOG_INFO_TIMER_IF(sw_, objTimer_, strFmt_, ...)              ST_LOG_TIMER_IF(sw_, objTimer_, "INFO",   strFmt_, ##__VA_ARGS__)
#define ST_LOG_ERR_TIMER_IF(sw_, objTimer_, strFmt_, ...)               ST_LOG_TIMER_IF(sw_, objTimer_, "ERR",    strFmt_, ##__VA_ARGS__)
#define ST_LOG_FATAL_TIMER_IF(sw_, objTimer_, strFmt_, ...)             ST_LOG_TIMER_IF(sw_, objTimer_, "FATAL",  strFmt_, ##__VA_ARGS__)




/**************************************************************************************
功能描述: 智能打印，避免周期频繁打印
修改记录:
**************************************************************************************/
#define ST_LOG_AI(newVal_, printStrWhenSucc_, printStrWhenFail_, printInterContinueTimes_)              \
    do {                                                                                                \
        static BOOL   isSuccPre_     = true;                                                            \
        static UINT32 continueTimes_ = 0;                                                               \
               BOOL   isSuccNow_     = ((newVal_) == 0);                                                \
        ++continueTimes_;                                                                               \
        if (isSuccNow_ == isSuccPre_) {                                                                 \
            if (continueTimes_ > (printInterContinueTimes_)) {                                          \
                if (isSuccNow_) {                                                                       \
                    ST_LOG_INFO(printStrWhenSucc_" Continue %u Times.", printInterContinueTimes_);      \
                } else {                                                                                \
                    ST_LOG_ERR(printStrWhenFail_" Continue %u Times.", printInterContinueTimes_);       \
                }                                                                                       \
                continueTimes_ = 0;                                                                     \
            }                                                                                           \
        } else {                                                                                        \
            if (isSuccNow_) {                                                                           \
                ST_LOG_INFO(printStrWhenSucc_);                                                         \
            } else {                                                                                    \
                ST_LOG_ERR(printStrWhenFail_);                                                          \
            }                                                                                           \
            isSuccPre_     = isSuccNow_;                                                                \
            continueTimes_ = 0;                                                                         \
        }                                                                                               \
    } while (0)

/**************************************************************************************
功能描述: 检查
修改记录:
**************************************************************************************/
#define NOTHING
#define CHECK_FAIL_R(  condition_, retVal_)                                 if (!(condition_)) { ST_LOG_ERR("Check(%s) Fail.",                                                                 #condition_);                                                             return retVal_; }
#define CHECK_FAIL_R_1(condition_, retVal_, para1_)                         if (!(condition_)) { ST_LOG_ERR("Check(%s) Fail. "#para1_" = %d.",                                                 #condition_, INT32(para1_));                                              return retVal_; }
#define CHECK_FAIL_R_2(condition_, retVal_, para1_, para2_)                 if (!(condition_)) { ST_LOG_ERR("Check(%s) Fail. "#para1_" = %d, "#para2_" = %d.",                                 #condition_, INT32(para1_), INT32(para2_));                               return retVal_; }
#define CHECK_FAIL_R_3(condition_, retVal_, para1_, para2_, para3_)         if (!(condition_)) { ST_LOG_ERR("Check(%s) Fail. "#para1_" = %d, "#para2_" = %d, "#para3_" = %d.",                 #condition_, INT32(para1_), INT32(para2_), INT32(para3_));                return retVal_; }
#define CHECK_FAIL_R_4(condition_, retVal_, para1_, para2_, para3_, para4_) if (!(condition_)) { ST_LOG_ERR("Check(%s) Fail. "#para1_" = %d, "#para2_" = %d, "#para3_" = %d, "#para4_" = %d.", #condition_, INT32(para1_), INT32(para2_), INT32(para3_), INT32(para4_)); return retVal_; }

/**************************************************************************************
功能描述: 断言
修改记录:
**************************************************************************************/
#define R_ASSERT(  condition_, retVal_)                                     if (!(condition_)) { ST_LOG_FATAL("Check(%s) Fail.",                                                                 #condition_);                                                             return retVal_; }
#define R_ASSERT_1(condition_, retVal_, para1_)                             if (!(condition_)) { ST_LOG_FATAL("Check(%s) Fail. "#para1_" = %d.",                                                 #condition_, INT32(para1_));                                              return retVal_; }
#define R_ASSERT_2(condition_, retVal_, para1_, para2_)                     if (!(condition_)) { ST_LOG_FATAL("Check(%s) Fail. "#para1_" = %d, "#para2_" = %d.",                                 #condition_, INT32(para1_), INT32(para2_));                               return retVal_; }
#define R_ASSERT_3(condition_, retVal_, para1_, para2_, para3_)             if (!(condition_)) { ST_LOG_FATAL("Check(%s) Fail. "#para1_" = %d, "#para2_" = %d, "#para3_" = %d.",                 #condition_, INT32(para1_), INT32(para2_), INT32(para3_));                return retVal_; }
#define R_ASSERT_4(condition_, retVal_, para1_, para2_, para3_, para4_)     if (!(condition_)) { ST_LOG_FATAL("Check(%s) Fail. "#para1_" = %d, "#para2_" = %d, "#para3_" = %d, "#para4_" = %d.", #condition_, INT32(para1_), INT32(para2_), INT32(para3_), INT32(para4_)); return retVal_; }

#define R_ASSERT_DO(condition_, doSomeThing_, retVal_)                      if (!(condition_)) { ST_LOG_FATAL("Check(%s) Fail.",                                                                 #condition_);                                                             doSomeThing_; return retVal_; }

#endif

