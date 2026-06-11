#ifndef __COMM_MSG_DETECT_INFO_H__
#define __COMM_MSG_DETECT_INFO_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/coor2DArrayLA.h>
#include <ros/ros.h>

#include <mutex>
/**************************************************************************************
功能描述: 垃圾信息
修改记录:
**************************************************************************************/
struct DETECT_INFO_GARBAGE_STRU
{
    UINT32             id;
    COORDINATE_2D_STRU stPos;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class DETECT_INFO_SENDER_C
{
public:
    DETECT_INFO_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::coor2DArrayLA>(TOPIC_NAME_PCPT_OBST_WITH_LABEL, 1);
    }

    void Send(const std::vector<DETECT_INFO_GARBAGE_STRU> &vectGarbage, const std::vector<COORDINATE_2D_STRU> &vectPipePoint, const std::vector<COORDINATE_2D_STRU> &vectWirePoint, BOOL isExistRisk)
    {
        comm_msg::coor2DArrayLA stMsg;

        /* 添加垃圾 */
        for (const DETECT_INFO_GARBAGE_STRU &stGarbage : vectGarbage) {
            comm_msg::coor2D stMsgGarbagePos;
            stMsgGarbagePos.pos_x = stGarbage.stPos.x;
            stMsgGarbagePos.pos_y = stGarbage.stPos.y;

            comm_msg::coor2DArrayL stMsgGarbage;
            stMsgGarbage.header.stamp      = ros::Time::now();
            stMsgGarbage.header.frame_id   = "map";
            stMsgGarbage.label.value       = comm_msg::label::GARBAGE;
            stMsgGarbage.id                = stGarbage.id;
            stMsgGarbage.coor2DArray.push_back(stMsgGarbagePos);

            stMsg.coor2DArrayLA.push_back(stMsgGarbage);
        }

        /* 添加水管 */
        comm_msg::coor2DArrayL stMsgPipe;
        stMsgPipe.header.stamp      = ros::Time::now();
        stMsgPipe.header.frame_id   = "map";
        stMsgPipe.label.value       = comm_msg::label::PIPELINE;
        stMsgPipe.id                = 0;
        for (const COORDINATE_2D_STRU &stPipePoint : vectPipePoint) {
            comm_msg::coor2D stMsgPipePointPos;
            stMsgPipePointPos.pos_x = stPipePoint.x;
            stMsgPipePointPos.pos_y = stPipePoint.y;
            stMsgPipe.coor2DArray.push_back(stMsgPipePointPos);
        }

        stMsg.coor2DArrayLA.push_back(stMsgPipe);

        /* 添加电线 */
        comm_msg::coor2DArrayL stMsgWire;
        stMsgWire.header.stamp      = ros::Time::now();
        stMsgWire.header.frame_id   = "map";
        stMsgWire.label.value       = comm_msg::label::CORD;
        stMsgWire.id                = 0;
        for (const COORDINATE_2D_STRU &stWirePoint : vectWirePoint) {
            comm_msg::coor2D stMsgWirePointPos;
            stMsgWirePointPos.pos_x = stWirePoint.x;
            stMsgWirePointPos.pos_y = stWirePoint.y;
            stMsgWire.coor2DArray.push_back(stMsgWirePointPos);
        }

        stMsg.coor2DArrayLA.push_back(stMsgWire);

        stMsg.hasRisk = isExistRisk;

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class DETECT_GARBAGE_RECEIVER_C
{
public:
    DETECT_GARBAGE_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_PCPT_OBST_WITH_LABEL, 1, &DETECT_GARBAGE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    const std::vector<DETECT_INFO_GARBAGE_STRU> &Get(void)
    {
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            vectGarbage_.clear();
        }

        return vectGarbage_;
    }

private:
    ros::Subscriber                       objSubscriber_;
    TIMER_C                               objValidTimer_{0.25};
    std::vector<DETECT_INFO_GARBAGE_STRU> vectGarbage_;

    void ReceiveMsgCallBack(const comm_msg::coor2DArrayLA::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        objValidTimer_.Reset();

        vectGarbage_.clear();
        vectGarbage_.reserve(pstMsg->coor2DArrayLA.size());

        for (const comm_msg::coor2DArrayL &stObj : pstMsg->coor2DArrayLA) {
            if (stObj.label.value != comm_msg::label::GARBAGE) {
                continue;
            }

            if (stObj.coor2DArray.empty()) {
                continue;
            }

            const comm_msg::coor2D &stMsgGarbagePos = stObj.coor2DArray.front();

            DETECT_INFO_GARBAGE_STRU stGarbage;
            stGarbage.id      = stObj.id;
            stGarbage.stPos.x = stMsgGarbagePos.pos_x;
            stGarbage.stPos.y = stMsgGarbagePos.pos_y;

            vectGarbage_.push_back(stGarbage);
        }

        UINT32 garbageCount = UINT32(vectGarbage_.size());
        ST_LOG_INFO_IF_CHANGE(garbageCount, "Exist Garbage Count: %u.", garbageCount);
    }
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class DETECT_PIPE_RECEIVER_C
{
public:
    DETECT_PIPE_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_PCPT_OBST_WITH_LABEL, 1, &DETECT_PIPE_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 GetPipeLineObsts(std::vector<COORDINATE_2D_STRU> &pipeline_obsts)
    {
        std::lock_guard<mutex> lock(mutex_);
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            return -1;
        }

        pipeline_obsts = pipeline_obsts_;

        return 0;
    }

    INT32 GetThickPipeObsts(std::vector<COORDINATE_2D_STRU> &thick_pipe_obsts)
    {
        std::lock_guard<mutex> lock(mutex_);
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            return -1;
        }

        thick_pipe_obsts = thick_pipe_obsts_;

        return 0;
    }

    INT32 GetWireObsts(std::vector<COORDINATE_2D_STRU> &wire_obsts)
    {
        std::lock_guard<mutex> lock(mutex_);
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            return -1;
        }

        wire_obsts = wire_obsts_;

        return 0;
    }

    const BOOL &IsHasRisk()
    {
        std::lock_guard<mutex> lock(mutex_);
        return is_has_risk_;
    }

private:
    ros::Subscriber                            objSubscriber_;
    TIMER_C                                    objValidTimer_{0.25};
    std::vector<COORDINATE_2D_STRU>            pipeline_obsts_;
    std::vector<COORDINATE_2D_STRU>            thick_pipe_obsts_;
    std::vector<COORDINATE_2D_STRU>            wire_obsts_;
    BOOL                                       is_has_risk_;  // 是否存在风险
    std::mutex                                 mutex_;

    void ReceiveMsgCallBack(const comm_msg::coor2DArrayLA::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        std::lock_guard<mutex> lock(mutex_);
        objValidTimer_.Reset();

        pipeline_obsts_.clear();
        thick_pipe_obsts_.clear();
        wire_obsts_.clear();

        is_has_risk_ = pstMsg->hasRisk;
        for (const comm_msg::coor2DArrayL &stObj : pstMsg->coor2DArrayLA) {
            // 读取水管数据
            if (stObj.label.value == comm_msg::label::PIPELINE) {
                for (const comm_msg::coor2D &stMsgPipePos : stObj.coor2DArray)
                {
                    pipeline_obsts_.emplace_back(COORDINATE_2D_STRU{stMsgPipePos.pos_x, stMsgPipePos.pos_y});
                }
            }

            // 读取粗水管数据
            if (stObj.label.value == comm_msg::label::THICKPIPE) {
                for (const comm_msg::coor2D &stMsgPipePos : stObj.coor2DArray)
                {
                    thick_pipe_obsts_.emplace_back(COORDINATE_2D_STRU{stMsgPipePos.pos_x, stMsgPipePos.pos_y});
                }
            }

            // 读取电缆数据
            if (stObj.label.value == comm_msg::label::CORD) {
                for (const comm_msg::coor2D &stMsgPipePos : stObj.coor2DArray)
                {
                    wire_obsts_.emplace_back(COORDINATE_2D_STRU{stMsgPipePos.pos_x, stMsgPipePos.pos_y});
                }
            }
        }
    }
};

class CHARGING_GUN_CHECK_RECEIVER_C
{
public:
    CHARGING_GUN_CHECK_RECEIVER_C(ros::NodeHandle &nh) : isCharging_(false)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_PCPT_OBST_WITH_LABEL, 1, &CHARGING_GUN_CHECK_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    BOOL IsCharging()
    {
        if (!isCharging_) {
            return isCharging_;
        }

        std::lock_guard<mutex> lock(mutex_);
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            isCharging_ = false;
        }

        return isCharging_;
    }

private:
    ros::Subscriber                       objSubscriber_;
    TIMER_C                               objValidTimer_{0.25};
    BOOL                                  isCharging_;
    std::mutex                            mutex_;

    void ReceiveMsgCallBack(const comm_msg::coor2DArrayLA::ConstPtr &pstMsg)
    {
        std::lock_guard<mutex> lock(mutex_);
        objValidTimer_.Reset();

        for (const comm_msg::coor2DArrayL &stObj : pstMsg->coor2DArrayLA) {
            if (stObj.label.value == comm_msg::label::CHARGING_GUN) {
                isCharging_ = true;
                break;
            }
        }
    }
};

#endif

