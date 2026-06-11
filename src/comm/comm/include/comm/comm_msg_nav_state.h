
#ifndef __COMM_MSG_NAV_STATE_H__
#define __COMM_MSG_NAV_STATE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm/comm_label.h"
#include <comm_msg/nav_state.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 规划运动趋势
修改记录:
**************************************************************************************/
enum PLAN_MOVE_TREND_ENUM {
    PLAN_MOVE_TREND_STOP                = 0,        /* 停止运动 */
    PLAN_MOVE_TREND_FORWARD             = 1,        /* 向前运动 */
    PLAN_MOVE_TREND_BACKWARD            = 2,        /* 向后运动 */
    PLAN_MOVE_TREND_FREE                = 3,        /* 自由运动 */
};

/**************************************************************************************
功能描述: 导航状态
修改记录:
**************************************************************************************/
struct NAV_STATE_STRU {
    NAV_STATE_STRU(){}
    NAV_STATE_STRU(const PLAN_MOVE_TREND_ENUM enPlanMoveTrend) {
        this->enPlanMoveTrend = enPlanMoveTrend;
        vecNavCtrl.clear();
    }

    /* 添加导航控制指令 */
    void AddNavCtrl(const NAV_CTRL_ENUM enNavCtrl) {
        auto iter = find(vecNavCtrl.begin(), vecNavCtrl.end(), enNavCtrl);
        if (iter == vecNavCtrl.end()) vecNavCtrl.push_back(enNavCtrl);
    }

    /* 删除导航控制指令 */
    void DelNavCtrl(const NAV_CTRL_ENUM enNavCtrl) {
        do {
            auto iter = find(vecNavCtrl.begin(), vecNavCtrl.end(), enNavCtrl);
            if (iter == vecNavCtrl.end()) return;
            iter = vecNavCtrl.erase(iter);
        } while (1);
    }

    /* 清空导航控制指令 */
    void ClrNavCtrl() {vecNavCtrl.clear();}

    PLAN_MOVE_TREND_ENUM    enPlanMoveTrend = PLAN_MOVE_TREND_STOP;
    vector<NAV_CTRL_ENUM>   vecNavCtrl;
};

/**************************************************************************************
功能描述: 发布导航状态消息
修改记录:
**************************************************************************************/
class NAV_STATE_SENDER_C
{
public:
    NAV_STATE_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::nav_state>(TOPIC_NAME_NAV_STATE, 1);

        stLastNavState_.enPlanMoveTrend = PLAN_MOVE_TREND_STOP;
    }

    void Stop(void)
    {
        SendMsg(NAV_STATE_STRU(PLAN_MOVE_TREND_STOP));
    }

    void Send(const NAV_STATE_STRU &stNavState)
    {
        SendMsg(stNavState);
    }

private:
    ros::Publisher publisher_;
    NAV_STATE_STRU stLastNavState_;

    BOOL UptLastNavState(const NAV_STATE_STRU &stNavState)
    {
        if (   (stLastNavState_.enPlanMoveTrend   != stNavState.enPlanMoveTrend)
            || (stLastNavState_.vecNavCtrl.size() != stNavState.vecNavCtrl.size())) {
            stLastNavState_ = stNavState;
            return true;
        }

        for (const NAV_CTRL_ENUM& enNavCtrl : stNavState.vecNavCtrl) {
            auto iter = find(stNavState.vecNavCtrl.begin(), stNavState.vecNavCtrl.end(), enNavCtrl);
            if (iter == stNavState.vecNavCtrl.end()) return true;
        }

        return false;
    }

    void SendMsg(const NAV_STATE_STRU &stNavState)
    {
        comm_msg::nav_state stMsg;

        stMsg.planMoveTrend = (UINT08)stNavState.enPlanMoveTrend;
        for (const NAV_CTRL_ENUM& enCltr : stNavState.vecNavCtrl) {
            comm_msg::nav_ctrl stCtrlMsg;
            stCtrlMsg.value = UINT08(enCltr);
            stMsg.navCtrl.push_back(stCtrlMsg);
        }

        publisher_.publish(stMsg);

        /* 有变更时记录日志 */
        if (UptLastNavState(stNavState)) {
            ST_LOG_INFO("enTrend=%u (stop %u, forward %u, backward %u, free %u).",
                        stMsg.planMoveTrend,
                        PLAN_MOVE_TREND_STOP,
                        PLAN_MOVE_TREND_FORWARD,
                        PLAN_MOVE_TREND_BACKWARD,
                        PLAN_MOVE_TREND_FREE);

            if (stNavState.vecNavCtrl.empty()) return;

            std::stringstream ss_data;
            for (const NAV_CTRL_ENUM& enNavCtrl : stNavState.vecNavCtrl)
                ss_data << '[' << enNavCtrl << ']' << NavCtrl.at(enNavCtrl) << ", ";
            string str = ss_data.str();
            str.erase(str.end() - 2, str.end());
            ST_LOG_INFO("导航控制指令: %s。", str.c_str());
        }
    }
};

/**************************************************************************************
功能描述: 导航状态接收器
修改记录:
**************************************************************************************/
class NAV_STATE_RECEIVER_C
{
public:
    NAV_STATE_RECEIVER_C(ros::NodeHandle &nh)
    {
        subscriber_ = nh.subscribe(TOPIC_NAME_NAV_STATE, 1, &NAV_STATE_RECEIVER_C::ReceiveMsgCallBack, this);

        stNavState_.enPlanMoveTrend = PLAN_MOVE_TREND_STOP;
    }

    void Get(NAV_STATE_STRU &stNavState) const
    {
        stNavState = stNavState_;
    }

private:
    ros::Subscriber subscriber_;
    NAV_STATE_STRU  stNavState_;

    void ReceiveMsgCallBack(const comm_msg::nav_state::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        /* 以最新状态为准，所以直接覆盖老状态即可 */
        NAV_STATE_STRU stNewState;
        stNewState.enPlanMoveTrend = PLAN_MOVE_TREND_ENUM(pstMsg->planMoveTrend);
        for (const auto& ctrlMsg : pstMsg->navCtrl)
            stNewState.vecNavCtrl.push_back(NAV_CTRL_ENUM(ctrlMsg.value));

        /* 判断导航状态是否发生变化 */
        bool isNavStateChange = stNewState.enPlanMoveTrend   != stNavState_.enPlanMoveTrend ||
                                stNewState.vecNavCtrl.size() != stNavState_.vecNavCtrl.size();
        if (!isNavStateChange) {
            for (const NAV_CTRL_ENUM& enNavCtrl : stNewState.vecNavCtrl) {
                auto iter = find(stNavState_.vecNavCtrl.begin(), stNavState_.vecNavCtrl.end(), enNavCtrl);
                if (iter == stNavState_.vecNavCtrl.end()) isNavStateChange = true;
            }
        }

        /* 输出日志 */
        if (isNavStateChange) {
            ST_LOG_INFO("New Move Trend: %u (stop %u, forward %u, backward %u, free %u).",
                        stNewState.enPlanMoveTrend,
                        PLAN_MOVE_TREND_STOP,
                        PLAN_MOVE_TREND_FORWARD,
                        PLAN_MOVE_TREND_BACKWARD,
                        PLAN_MOVE_TREND_FREE);

            if (!stNewState.vecNavCtrl.empty()) {
                std::stringstream ss_data;
                for (const NAV_CTRL_ENUM& enNavCtrl : stNewState.vecNavCtrl)
                    ss_data << '[' << enNavCtrl << ']' << NavCtrl.at(enNavCtrl) << ", ";
                string str = ss_data.str();
                str.erase(str.end() - 2, str.end());
                ST_LOG_INFO("导航控制指令: %s。", str.c_str());
            }
        }

        stNavState_ = stNewState;
    }
};


#endif

