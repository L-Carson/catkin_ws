#ifndef __COMM_MSG_CMD_H__
#define __COMM_MSG_CMD_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"

#include <std_msgs/UInt8.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 发送命令
          注意：一个话题中只能同时（一个周期内）发送一个命令
修改记录:
**************************************************************************************/
class CMD_SENDER_BASE_C
{
public:
    CMD_SENDER_BASE_C(ros::NodeHandle &nh, const std::string &strTopicName)
    {
        objPublisher_ = nh.advertise<std_msgs::UInt8>(strTopicName, 1);
    }

    void Send(UINT08 cmd)
    {
        std_msgs::UInt8 stMsg;
        stMsg.data = cmd;
        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收命令
          注意：一个话题中只能同时（一个周期内）接收一个命令
修改记录:
**************************************************************************************/
class CMD_RECEIVER_BASE_C
{
public:
    CMD_RECEIVER_BASE_C(ros::NodeHandle &nh, const std::string &strTopicName)
    {
        objSubscriber_ = nh.subscribe(strTopicName, 1, &CMD_RECEIVER_BASE_C::ReceiveMsgCallBack, this);
    }

    UINT08 Get(void)
    {
        return objInvalidTimer_.IsTimeOut() ? UINT08_NULL : cmd_;
    }

private:
    ros::Subscriber  objSubscriber_;
    TIMER_C          objInvalidTimer_{0.15};
    UINT08           cmd_;

    void ReceiveMsgCallBack(const std_msgs::UInt8::ConstPtr &pstMsg)
    {
        cmd_ = pstMsg->data;

        objInvalidTimer_.Reset();
    }
};

/**************************************************************************************
功能描述: 发布充电加水控制消息
修改记录:
**************************************************************************************/
class CHARGE_CMD_SENDER_C : public CMD_SENDER_BASE_C
{
public:
    CHARGE_CMD_SENDER_C(ros::NodeHandle &nh) : CMD_SENDER_BASE_C(nh, TOPIC_NAME_CHARGE_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 接收充电加水控制状态消息
修改记录:
**************************************************************************************/
class CHARGE_CMD_RECEIVER_C : public CMD_RECEIVER_BASE_C
{
public:
    CHARGE_CMD_RECEIVER_C(ros::NodeHandle &nh) : CMD_RECEIVER_BASE_C(nh, TOPIC_NAME_CHARGE_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 发布垃圾箱倾倒控制消息
修改记录:
**************************************************************************************/
class TRASH_BIN_DUMP_CMD_SENDER_C : public CMD_SENDER_BASE_C
{
public:
    TRASH_BIN_DUMP_CMD_SENDER_C(ros::NodeHandle &nh) : CMD_SENDER_BASE_C(nh, TOPIC_NAME_TRASH_BIN_DUMP_CMD)
    {
    }

private:

};

/**************************************************************************************
功能描述: 接收垃圾箱倾倒控制状态消息
修改记录:
**************************************************************************************/
class TRASH_BIN_DUMP_CMD_RECEIVER_C : public CMD_RECEIVER_BASE_C
{
public:
    TRASH_BIN_DUMP_CMD_RECEIVER_C(ros::NodeHandle &nh) : CMD_RECEIVER_BASE_C(nh, TOPIC_NAME_TRASH_BIN_DUMP_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 发布垃圾斗盖控制命令
修改记录:
**************************************************************************************/
class TRASH_COVER_CTRL_CMD_SENDER_C : public CMD_SENDER_BASE_C
{
public:
    TRASH_COVER_CTRL_CMD_SENDER_C(ros::NodeHandle &nh) : CMD_SENDER_BASE_C(nh, TOPIC_NAME_TRASH_COVER_CTRL_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 接收垃圾斗盖控制命令
修改记录:
**************************************************************************************/
class TRASH_COVER_CTRL_CMD_RECEIVER_C : public CMD_RECEIVER_BASE_C
{
public:
    TRASH_COVER_CTRL_CMD_RECEIVER_C(ros::NodeHandle &nh) : CMD_RECEIVER_BASE_C(nh, TOPIC_NAME_TRASH_COVER_CTRL_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 发布垃圾打包控制命令
修改记录:
**************************************************************************************/
class TRASH_BAG_PACK_CMD_SENDER_C : public CMD_SENDER_BASE_C
{
public:
    TRASH_BAG_PACK_CMD_SENDER_C(ros::NodeHandle &nh) : CMD_SENDER_BASE_C(nh, TOPIC_NAME_TRASH_BAG_PACK_CMD)
    {
    }

private:
};


/**************************************************************************************
功能描述: 发布充电舱门控制命令
修改记录:
**************************************************************************************/
class CHARGING_DOOR_CMD_SENDER_C : public CMD_SENDER_BASE_C
{
public:
    CHARGING_DOOR_CMD_SENDER_C(ros::NodeHandle &nh) : CMD_SENDER_BASE_C(nh, TOPIC_NAME_CHARGING_DOOR_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 接收垃圾打包控制命令
修改记录:
**************************************************************************************/
class TRASH_BAG_PACK_CMD_RECEIVER_C : public CMD_RECEIVER_BASE_C
{
public:
    TRASH_BAG_PACK_CMD_RECEIVER_C(ros::NodeHandle &nh) : CMD_RECEIVER_BASE_C(nh, TOPIC_NAME_TRASH_BAG_PACK_CMD)
    {
    }

private:
};

/**************************************************************************************
功能描述: 接收充电舱门控制命令
修改记录:
**************************************************************************************/
class CHARGING_DOOR_CMD_RECEIVER_C : public CMD_RECEIVER_BASE_C
{
public:
    CHARGING_DOOR_CMD_RECEIVER_C(ros::NodeHandle &nh) : CMD_RECEIVER_BASE_C(nh, TOPIC_NAME_CHARGING_DOOR_CMD)
    {
    }

private:
};





#endif

