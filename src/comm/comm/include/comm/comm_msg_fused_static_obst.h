#ifndef __COMM_MSG_FUSED_STATIC_OBST_H__
#define __COMM_MSG_FUSED_STATIC_OBST_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include <comm_msg/coor2DArray.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/* 和感知消息的分类保持一致 */
enum FUSED_OBST_TYPE_ENUM {
    FUSED_OBST_TYPE_UNKNOWN = 0,         // 未知
    FUSED_OBST_TYPE_STATIC = 1,          // 静态障碍物      
    FUSED_OBST_TYPE_DYNAMIC = 2,         // 动态障碍物
    FUSED_OBST_TYPE_HOLLOW_OBST = 3,     // 空洞障碍物
    FUSED_OBST_TYPE_SHORT_HOLD = 4,      // 低矮维持障碍物
    FUSED_OBST_TYPE_COMMON_HOLD = 5,     // 常规维持障碍物
    FUSED_OBST_TYPE_BLIND_SPOT   = 6,    // 盲区障碍
    FUSED_OBST_TYPE_SLOPE_PAD    = 7,    // 斜坡垫
    FUSED_OBST_TYPE_CONTACT_EDGE = 8,    // 触边障碍
    FUSED_OBST_TYPE_TREELAWN     = 9,    // 绿植障碍
    FUSED_OBST_TYPE_ULTRASONIC   = 10,   // 超声
};

/**************************************************************************************
功能描述: 融合静态障碍物消息接收器
修改记录:
**************************************************************************************/
class FUSED_STATIC_OBST_RECEIVER_C
{
public:
    FUSED_STATIC_OBST_RECEIVER_C(ros::NodeHandle &nh)
    {
        subscriber_ = nh.subscribe(TOPIC_NAME_FUSED_STATIC_OBST, 1, &FUSED_STATIC_OBST_RECEIVER_C::ReceiveMsgCallBack, this);

        objInvalidTimer_.SetOverTime(0.25);
    }

    INT32 Get(BOOL isMustIncludeForwardObsts, BOOL isMustIncludeBackwardObsts, vector<COORDINATE_2D_STRU> &vectObsts, vector<COORDINATE_2D_STRU> &vectObstsTop) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return -1;
        }

        BOOL isMsgNotMatch = (isMustIncludeForwardObsts && !isIncludeForwardObsts_) || (isMustIncludeBackwardObsts && !isIncludeBackwardObsts_);
        ST_LOG_ERR_SAMPLE_IF_TRUE(isMsgNotMatch, 20 * 60, "Not Match: NeedForward=%d, NeedBackward=%d, ExistForward=%d, ExistBackward=%d.", isMustIncludeForwardObsts, isMustIncludeBackwardObsts, isIncludeForwardObsts_, isIncludeBackwardObsts_);
        if (isMsgNotMatch) {
            return -1;
        }

        /* 没有最新数据来之前保持原先的数据 */
        vectObsts.clear();
        vectObsts.reserve(vectObstsMid_.size() + vectObstsDown_.size() + vectUltrasonicObsts_.size());
        vectObsts.insert(vectObsts.end(), vectObstsMid_.begin(), vectObstsMid_.end());
        vectObsts.insert(vectObsts.end(), vectObstsDown_.begin(), vectObstsDown_.end());
        vectObsts.insert(vectObsts.end(), vectUltrasonicObsts_.begin(), vectUltrasonicObsts_.end());
        vectObstsTop = vectObstsTop_;
        return 0;
    }

    INT32 GetDynamicObstacle(vector<COORDINATE_2D_STRU> &vectDyncObsts) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return -1;
        }

        vectDyncObsts = vectDyncObsts_;
        return 0;
    }

    INT32 GetNoneUltrasonicObstacle(vector<COORDINATE_2D_STRU> &vectObsts, vector<COORDINATE_2D_STRU> &vectObstsTop) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return -1;
        }

        /* 没有最新数据来之前保持原先的数据 */
        vectObsts.clear();
        vectObsts.reserve(vectObstsMid_.size() + vectObstsDown_.size());
        vectObsts.insert(vectObsts.end(), vectObstsMid_.begin(), vectObstsMid_.end());
        vectObsts.insert(vectObsts.end(), vectObstsDown_.begin(), vectObstsDown_.end());
        vectObstsTop = vectObstsTop_;
        return 0;
    }

private:
    ros::Subscriber             subscriber_;
    TIMER_C                     objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    BOOL                        isIncludeForwardObsts_;
    BOOL                        isIncludeBackwardObsts_;
    vector<COORDINATE_2D_STRU>  vectObstsTop_;
    vector<COORDINATE_2D_STRU>  vectObstsMid_;
    vector<COORDINATE_2D_STRU>  vectObstsDown_;
    vector<COORDINATE_2D_STRU>  vectDyncObsts_;
    vector<COORDINATE_2D_STRU>  vectUltrasonicObsts_;

    ros::Time lastT_;

    void ReceiveMsgCallBack(const comm_msg::coor2DArray::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        ros::Time nowT = ros::Time::now();
        ros::Duration d(nowT - lastT_);
        lastT_ = nowT;
        float intervalMs = d.toSec() * 1000;
        ST_LOG_INFO_IF(intervalMs > 160.0, "recv obs interval in ms: %f", intervalMs);

        ros::Duration msgD(nowT - pstMsg->header.stamp);
        const float delayMsgTime = 500.0;    //单位ms
        if (msgD.toSec() * 1000 > delayMsgTime) {
            ST_LOG_INFO("感知给的数据是延迟超过%.2fms不使用.", delayMsgTime);
            return;
        }

        objInvalidTimer_.Reset();

        isIncludeForwardObsts_  = pstMsg->isContainForwardObst;
        isIncludeBackwardObsts_ = pstMsg->isContainBackwardObst;

        vectDyncObsts_.clear();
        vectDyncObsts_.reserve(pstMsg->coor2DArrayTop.size() + pstMsg->coor2DArrayMid.size() + pstMsg->coor2DArrayDown.size());

        vectObstsTop_.clear();
        vectObstsTop_.reserve(pstMsg->coor2DArrayTop.size());
        for (UINT32 loop = 0; loop < pstMsg->coor2DArrayTop.size(); ++loop) {
            COORDINATE_2D_STRU stObst;
            stObst.x = pstMsg->coor2DArrayTop[loop].pos_x;
            stObst.y = pstMsg->coor2DArrayTop[loop].pos_y;

            vectObstsTop_.push_back(stObst);

            if (pstMsg->coor2DArrayTop[loop].label.value == FUSED_OBST_TYPE_DYNAMIC) {
                vectDyncObsts_.push_back(stObst);
            }
        }

        vectObstsMid_.clear();
        vectObstsMid_.reserve(pstMsg->coor2DArrayMid.size());
        vectUltrasonicObsts_.clear();
        for (UINT32 loop = 0; loop < pstMsg->coor2DArrayMid.size(); ++loop) {
            COORDINATE_2D_STRU stObst;
            stObst.x = pstMsg->coor2DArrayMid[loop].pos_x;
            stObst.y = pstMsg->coor2DArrayMid[loop].pos_y;
            if (pstMsg->coor2DArrayMid[loop].label.value == FUSED_OBST_TYPE_ULTRASONIC) {
                vectUltrasonicObsts_.push_back(stObst);
            } else {
                vectObstsMid_.push_back(stObst);
            }

            if (pstMsg->coor2DArrayMid[loop].label.value == FUSED_OBST_TYPE_DYNAMIC) {
                vectDyncObsts_.push_back(stObst);
            }
        }

        vectObstsDown_.clear();
        vectObstsDown_.reserve(pstMsg->coor2DArrayDown.size());
        for (UINT32 loop = 0; loop < pstMsg->coor2DArrayDown.size(); ++loop) {
            COORDINATE_2D_STRU stObst;
            stObst.x = pstMsg->coor2DArrayDown[loop].pos_x;
            stObst.y = pstMsg->coor2DArrayDown[loop].pos_y;
            if (pstMsg->coor2DArrayDown[loop].label.value == FUSED_OBST_TYPE_ULTRASONIC) {
                vectUltrasonicObsts_.push_back(stObst);
            } else {
                vectObstsDown_.push_back(stObst);
            }

            if (pstMsg->coor2DArrayDown[loop].label.value == FUSED_OBST_TYPE_DYNAMIC) {
                vectDyncObsts_.push_back(stObst);
            }
        }
    }
};

#endif

