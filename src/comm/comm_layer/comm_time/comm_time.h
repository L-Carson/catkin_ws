/*
Author      : gongzhihao
Update Date : 2025/07/31
*/
#ifndef COMM_TIME_H
#define COMM_TIME_H

#include "comm_layer/comm_base/comm_base.h"

// 定时器
class Timer
{
public:
    // 构造函数
    Timer(float time_thr_high);

    // 构造函数
    Timer(float time_thr_low, float time_thr_high);

    // 析构函数
    ~Timer(void);

    // 设置时间阈值
    void SetTimeThr(float time_thr_high);

    // 设置时间阈值
    void SetTimeThr(float time_thr_low, float time_thr_high);

    void SetTimeOut(void);

    // 重置
    void Reset(void);

    // 是否时间不足
    bool IsTimeLack(void);

    // 是否超时
    bool IsTimeOut(void) const;

private:
    ros::Time last_time_;
    ros::Duration time_thr_low_;
    ros::Duration time_thr_high_;
};

#endif
