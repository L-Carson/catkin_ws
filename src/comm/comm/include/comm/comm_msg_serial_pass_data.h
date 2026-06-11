#ifndef __COMM_MSG_SERIAL_PASS_DATA_H__
#define __COMM_MSG_SERIAL_PASS_DATA_H__

#include "comm/comm_base.h"
#include "comm/comm_topic_name.h"
#include <comm_msg/serialPassData.h>
#include <ros/ros.h>
#include <functional>


/**************************************************************************************
功能描述: 发送转发串口数据
修改记录:
**************************************************************************************/
class SERIAL_PASS_DATA_SENDER_C
{
public:
    SERIAL_PASS_DATA_SENDER_C(ros::NodeHandle &nh, const std::string &strTopicName)
    {
        objPublisher_ = nh.advertise<comm_msg::serialPassData>(strTopicName.c_str(), 1000);
    }

    void Send(const UINT08 *pucData, UINT32 dataLen)
    {
        R_ASSERT((pucData != NULL), NOTHING);

        comm_msg::serialPassData stMsg;

        stMsg.dataStream.resize(dataLen);
        for (UINT32 loop = 0; loop < dataLen; ++loop) {
            stMsg.dataStream[loop] = pucData[loop];
        }

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收转发串口数据
修改记录:
**************************************************************************************/
class SERIAL_PASS_DATA_RECEIVER_C
{
public:
    SERIAL_PASS_DATA_RECEIVER_C(ros::NodeHandle &nh, const std::string &strTopicName, const std::function<void(const UINT08 *, UINT32)> &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(strTopicName.c_str(), 1000, &SERIAL_PASS_DATA_RECEIVER_C::ReceiveMsgCallBack, this);
    }

private:
    const std::function<void(const UINT08 *, UINT32)> objRecvFunc_;
    ros::Subscriber                                   objSubscriber_;

    void ReceiveMsgCallBack(const comm_msg::serialPassData::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);
        R_ASSERT(pstMsg->dataStream.size() > 0, NOTHING);

        objRecvFunc_(&pstMsg->dataStream[0], UINT32(pstMsg->dataStream.size()));
    }
};

#endif

