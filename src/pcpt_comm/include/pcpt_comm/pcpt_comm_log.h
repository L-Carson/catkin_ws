#ifndef __PCPT_COMM_LOG_H__
#define __PCPT_COMM_LOG_H__

#include "comm/comm_dbg_log.h"

/**
 * @brief 异常信息打印器
 *
 */
class ABNORMAL_PRINT_C
{
public:
    ABNORMAL_PRINT_C() : dataSwOldVal_(false), dataOutputTimes_(0){}

    void DataInvalidPrint(BOOL sw, UINT32 sampleTimes, const string strPtint)
    {
        if (dataSwOldVal_ != sw) {
            if (sw) {
                ST_LOG("ERR", "Topic <%s> Data Time Out. Begin.", strPtint.c_str());
                dataOutputTimes_ = 0;
            } else {
                ST_LOG("ERR", "Topic <%s> Data Time Out. End.", strPtint.c_str());
            }
            dataSwOldVal_ = sw;
        }
        if (dataSwOldVal_) {
            ++dataOutputTimes_;
            if (dataOutputTimes_ >= sampleTimes) {
                dataOutputTimes_ = 0;
                ST_LOG("ERR", "Topic <%s> Data TimeOut.", strPtint.c_str());
            }
        }
    }

    void PeriodPrint(BOOL sw, UINT32 sampleTimes, const string strPtint)
    {
        if (dataSwOldVal_ != sw) {
            if (sw) {
                ST_LOG("ERR", "%s Begin.", strPtint.c_str());
                dataOutputTimes_ = 0;
            } else {
                ST_LOG("ERR", "%s End.", strPtint.c_str());
            }
            dataSwOldVal_ = sw;
        }
        if (dataSwOldVal_) {
            ++dataOutputTimes_;
            if (dataOutputTimes_ >= sampleTimes) {
                dataOutputTimes_ = 0;
                ST_LOG("ERR", "%s", strPtint.c_str());
            }
        }
    }

    void AbnormalPrint(BOOL sw, UINT32 sampleTimes, const string strPtint)
    {
        if (dataSwOldVal_ != sw) {
            if (sw) {
                ST_LOG("ERR", "%s Begin.", strPtint.c_str());
                dataOutputTimes_ = 0;
            } else {
                ST_LOG("ERR", "%s End.", strPtint.c_str());
            }
            dataSwOldVal_ = sw;
        }
        if (dataSwOldVal_) {
            ++dataOutputTimes_;
            if (dataOutputTimes_ >= sampleTimes) {
                dataOutputTimes_ = 0;
                ST_LOG("ERR", "%s", strPtint.c_str());
            }
        }
    }

private:
    BOOL   dataSwOldVal_;
    UINT32 dataOutputTimes_;
};

#endif

