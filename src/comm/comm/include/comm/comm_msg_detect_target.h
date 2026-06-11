#ifndef __COMM_MSG_DETECT_TARGET_H__
#define __COMM_MSG_DETECT_TARGET_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/detect_target.h>
#include <comm_msg/detect_result.h>
#include <ros/ros.h>

#include <mutex>


enum class DOOR_TYPE_ENUM : UINT32
{
    STORE_HOUSE = 0, // 库房门
    BARRIER_GATE = 1, // 闸门
};

/**************************************************************************************
功能描述: 发布检测门是否开的消息
修改记录:
**************************************************************************************/
class DETECT_TARGET_DOOR_SENDER_C
{
public:
    DETECT_TARGET_DOOR_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::detect_target>(TOPIC_NAME_DETECT_TARGET_DOOR, 1);
    }

    void DetectOpenDoor(const vector<COORDINATE_2D_STRU> &vertexes)
    {
        comm_msg::detect_target stMsg;
        stMsg.type = 1;
        for (const COORDINATE_2D_STRU &stPos : vertexes) {
            comm_msg::coor2D point;
            point.pos_x = stPos.x;
            point.pos_y = stPos.y;
            stMsg.vertexes.push_back(point);
        }

        objPublisher_.publish(stMsg);
    }

    void DetectCloseDoor(const vector<COORDINATE_2D_STRU> &vertexes)
    {
        comm_msg::detect_target stMsg;
        stMsg.type = 2;
        for (const COORDINATE_2D_STRU &stPos : vertexes) {
            comm_msg::coor2D point;
            point.pos_x = stPos.x;
            point.pos_y = stPos.y;
            stMsg.vertexes.push_back(point);
        }

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收检测门是否开的消息
修改记录:
**************************************************************************************/
class DETECT_TARGET_DOOR_RECEIVER_C
{
public:
    DETECT_TARGET_DOOR_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_DETECT_TARGET_DOOR, 1, &DETECT_TARGET_DOOR_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 GetDoorOpenStatus(UINT32 &type)
    {
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            return -1;
        }

        type = type_;
        return 0;
    }

private:
    ros::Subscriber objSubscriber_;
    TIMER_C         objValidTimer_{0.25};
    UINT32          type_;

    void ReceiveMsgCallBack(const comm_msg::detect_target::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != nullptr, NOTHING);

        objValidTimer_.Reset();

        type_ = pstMsg->type;
        ST_LOG_INFO_IF_CHANGE(type_, "type: %u.", type_);
    }
};


/**************************************************************************************
功能描述: 发布门是否开的消息
修改记录:
**************************************************************************************/
class DETECT_RESULT_DOOR_SENDER_C
{
public:
    DETECT_RESULT_DOOR_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::detect_result>(TOPIC_NAME_DETECT_RESULT_DOOR, 1);
    }

    void DetectDoor(const INT32 &doorOpenStatus)
    {
        comm_msg::detect_result stMsg;
        stMsg.result = doorOpenStatus;
        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收门是否开的消息
修改记录:
**************************************************************************************/
class DETECT_RESULT_DOOR_RECEIVER_C
{
public:
    DETECT_RESULT_DOOR_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_DETECT_RESULT_DOOR, 1, &DETECT_RESULT_DOOR_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 GetDoorOpenStatus(INT32 &result)
    {
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            return -1;
        }

        result = result_;
        return 0;
    }

private:
    ros::Subscriber objSubscriber_;
    TIMER_C         objValidTimer_{0.25};
    INT32           result_;

    void ReceiveMsgCallBack(const comm_msg::detect_result::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != nullptr, NOTHING);

        objValidTimer_.Reset();

        result_ = pstMsg->result;
        ST_LOG_INFO_IF_CHANGE(result_, "检测门是否开result: %i(0开1关).", result_);
    }
};

#endif

