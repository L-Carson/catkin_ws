#ifndef __PCPT_COMM_ABNORMAL_H__
#define __PCPT_COMM_ABNORMAL_H__

#include "comm/comm_dbg_log.h"
#include <atomic>

class PCPT_ABNORMAL_C
{
public:
    PCPT_ABNORMAL_C(const char* mark = nullptr) : mark_(mark) {}
    ~PCPT_ABNORMAL_C() {if (isAbnormal_) --abCount_;}

    bool IsAbnormal() {
        int abCount = abCount_;
        ST_LOG_INFO_IF_CHANGE(abCount, "Total abnormal: %d.", abCount);
        return abCount > 0;
    }

    void SetAbnormal(bool isAbnormal, const char* mark = nullptr) {
        if (isAbnormal_ != isAbnormal) {
            isAbnormal_ = isAbnormal;
            if (isAbnormal) {
                ++abCount_;
                int abCount = abCount_;
                if (mark == nullptr) mark = mark_;
                if (mark) {
                    ST_LOG_ERR("%s: set abnormal, residual abnormal: %d.", mark, abCount);
                } else {
                    ST_LOG_ERR("set abnormal, residual abnormal: %d.", abCount);
                }

            } else {
                --abCount_;
                int abCount = abCount_;
                if (mark == nullptr) mark = mark_;
                if (mark) {
                    ST_LOG_ERR("%s: reset abnormal, residual abnormal: %d.", mark, abCount);
                } else {
                    ST_LOG_ERR("reset abnormal, residual abnormal: %d.", abCount);
                }
            }
        }
    }

private:
    const char*             mark_{nullptr};
    bool                    isAbnormal_{false};
    static std::atomic_int  abCount_;
};

#endif
