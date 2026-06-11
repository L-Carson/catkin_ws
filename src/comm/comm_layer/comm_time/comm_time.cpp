/*
Author      : gongzhihao
Update Date : 2025/07/31
*/
#include "comm_layer/comm_time/comm_time.h"

// 构造函数
Timer::Timer(float time_thr_high)
{
    // 设置时间阈值
    SetTimeThr(time_thr_high);

    // 重置
    Reset();
}

// 构造函数
Timer::Timer(float time_thr_low, float time_thr_high)
{
    // 设置时间阈值
    SetTimeThr(time_thr_low, time_thr_high);

    // 重置
    Reset();
}

// 析构函数
Timer::~Timer(void)
{
}

// 设置时间阈值
void Timer::SetTimeThr(float time_thr_high)
{
    time_thr_low_ = ros::Duration(0.0);
    time_thr_high_ = ros::Duration(time_thr_high);
}

// 设置时间阈值
void Timer::SetTimeThr(float time_thr_low, float time_thr_high)
{
    time_thr_low_ = ros::Duration(time_thr_low);
    time_thr_high_ = ros::Duration(time_thr_high);
}

// 设置为超时状态
void Timer::SetTimeOut(void)
{
    last_time_ = ros::Time::now() - time_thr_high_ * 2.0;
}

// 重置
void Timer::Reset(void)
{
    last_time_ = ros::Time::now();
}

// 是否时间不足
bool Timer::IsTimeLack(void)
{
    return (ros::Time::now() - last_time_ < time_thr_low_);
}

// 是否超时
bool Timer::IsTimeOut(void) const
{
    return (ros::Time::now() - last_time_ >= time_thr_high_);
}
