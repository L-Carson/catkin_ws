#ifndef __COMM_MSG_SPOILER_DETECT_H__
#define __COMM_MSG_SPOILER_DETECT_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm/comm_msg.h"
#include <comm_msg/spoiler_detect.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**
 * @brief 检测尾翼范围内是否存在障碍物结构
 *
 */
struct SPOILER_ARANGE_DETECT_STRU
{
    double time;
    UINT08 detectLabel;          // 检测标签/0:未知 1:行人 2:骑车人 3:自行车 4:汽车 5:卡车 
    UINT08 detectValue;          // 检测值  是否检测到 0 否 1 是
};

/**************************************************************************************
功能描述: 检测尾翼范围内存在障碍物发布消息
修改记录:
**************************************************************************************/
class SPOILER_DETECT_SENDER_C
{
public:
    SPOILER_DETECT_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::spoiler_detect>(TOPIC_NAME_SPOILER_RANGE_DETECT, 1);
    }

    void Send(const SPOILER_ARANGE_DETECT_STRU &stDevInfo)
    {
        comm_msg::spoiler_detect stMsg;

        stMsg.header.stamp = ros::Time::now();
        stMsg.header.frame_id = "map";
        stMsg.label.value = stDevInfo.detectLabel;
        stMsg.detect_value = stDevInfo.detectValue;

        publisher_.publish(stMsg);
    }

private:
    ros::Publisher publisher_;
};

/**************************************************************************************
功能描述: 检测尾翼范围内存在障碍物接收器
修改记录:
**************************************************************************************/
class SPOILER_DETECT_RECEIVER_C
{
public:
    SPOILER_DETECT_RECEIVER_C(ros::NodeHandle &nh, double overTime = 1.0)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_SPOILER_RANGE_DETECT, 1, &SPOILER_DETECT_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(SPOILER_ARANGE_DETECT_STRU &stDetect) const {
        stDetect = stDetect_;
        return objInvalidTimer_.IsTimeOut() ? -1 : 0;
    }

    const SPOILER_ARANGE_DETECT_STRU *Get(void) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return nullptr;
        }

        return &stDetect_;
    }

private:
    ros::Subscriber receiver_;

    TIMER_C objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    SPOILER_ARANGE_DETECT_STRU stDetect_;

    void ReceiveMsgCallBack(const comm_msg::spoiler_detect::ConstPtr &pstMsg)
    {
        objInvalidTimer_.Reset();  // 重置数据失效定时器

        stDetect_.time = pstMsg->header.stamp.toSec();
        stDetect_.detectLabel = pstMsg->label.value;   // 提取检测标签
        stDetect_.detectValue = pstMsg->detect_value;  // 提取检测值
    }
};

#endif

