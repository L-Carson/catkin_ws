
#ifndef __COMM_MSG_SIMULATION_DYNC_MAP_H__
#define __COMM_MSG_SIMULATION_DYNC_MAP_H__

#include "comm/comm_base.h"
#include "comm/comm_topic_name.h"
#include "comm/loc_json/json.h"

#include "comm_msg/serialPassData.h"

#include <ros/ros.h>

#include <functional>
#include <vector>
#include <string>


using SIMULATION_DYNC_MAP_MSG_RECV_FUNC = std::function<void(UINT32, const Json::Value &)>;

/**************************************************************************************
功能描述: 动态地图
修改记录:
**************************************************************************************/
class SIMULATION_DYNC_MAP_MSG_BASE_C
{
public:
    SIMULATION_DYNC_MAP_MSG_BASE_C(ros::NodeHandle &nh, const std::string &strSendTopic, const std::string &strRecvTopic, const SIMULATION_DYNC_MAP_MSG_RECV_FUNC &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(strRecvTopic, 10, &SIMULATION_DYNC_MAP_MSG_BASE_C::ReceiveMsg, this);
        objPublisher_  = nh.advertise<comm_msg::serialPassData>(strSendTopic, 10);
    }

    void SendMsg(UINT32 msgType, const Json::Value &objJsonMsg)
    {
        Json::FastWriter objJsonWriter;
        std::string      strMsg = objJsonWriter.write(objJsonMsg);

        comm_msg::serialPassData stMsg;
        stMsg.dataStream.resize(strMsg.length() + 2);                   /* 存储格式，第一个字节为消息类型，第二个字节到倒数第二个字节为消息内容，最后一个字节为结束符。 */
        stMsg.dataStream[0]                   = UINT08(msgType);        /* 增加消息类型 */
        stMsg.dataStream[strMsg.length() + 1] = 0;                      /* 增加结束符 */
        for (UINT32 loop = 0; loop <= strMsg.length(); ++loop) {
            stMsg.dataStream[loop + 1] = strMsg[loop];
        }

        objPublisher_.publish(stMsg);
    }

private:
    const SIMULATION_DYNC_MAP_MSG_RECV_FUNC  objRecvFunc_;
    ros::Subscriber                          objSubscriber_;
    ros::Publisher                           objPublisher_;

    void ReceiveMsg(const comm_msg::serialPassData::ConstPtr &pstMsg)
    {
        R_ASSERT(objRecvFunc_ != nullptr,           NOTHING);
        R_ASSERT(pstMsg != NULL,                    NOTHING);
        R_ASSERT_1(pstMsg->dataStream.size() >= 2,  NOTHING, pstMsg->dataStream.size());
        R_ASSERT_2(pstMsg->dataStream.back() == 0,  NOTHING, pstMsg->dataStream.size(), pstMsg->dataStream.back());

        UINT32      msgType = pstMsg->dataStream[0];
        std::string strMsg  =(const char *)&pstMsg->dataStream[1];

        std::istringstream      objIss(strMsg);
        Json::Value             objJsonMsg;
        Json::CharReaderBuilder objBuilder;
        JSONCPP_STRING          strErrs;
        BOOL isSucc = Json::parseFromStream(objBuilder, objIss, &objJsonMsg, &strErrs);
        R_ASSERT_2(isSucc, NOTHING, msgType, pstMsg->dataStream.size());

        objRecvFunc_(msgType, objJsonMsg);
    }
};


/**************************************************************************************
功能描述: 动态地图
修改记录:
**************************************************************************************/
class SIMULATION_DYNC_MAP_MSG_CENTER_C : public SIMULATION_DYNC_MAP_MSG_BASE_C
{
public:
    SIMULATION_DYNC_MAP_MSG_CENTER_C(ros::NodeHandle &nh, const SIMULATION_DYNC_MAP_MSG_RECV_FUNC &objRecvFunc)
                            : SIMULATION_DYNC_MAP_MSG_BASE_C(nh, TOPIC_NAME_SIMULATION_DYNC_MAP_CENTER_TO_LOC, TOPIC_NAME_SIMULATION_DYNC_MAP_LOC_TO_CENTER, objRecvFunc)
    {
    }

private:
};

/**************************************************************************************
功能描述: 动态地图
修改记录:
**************************************************************************************/
class SIMULATION_DYNC_MAP_MSG_LOCAL_C : public SIMULATION_DYNC_MAP_MSG_BASE_C
{
public:
    SIMULATION_DYNC_MAP_MSG_LOCAL_C(ros::NodeHandle &nh, const SIMULATION_DYNC_MAP_MSG_RECV_FUNC &objRecvFunc)
                            : SIMULATION_DYNC_MAP_MSG_BASE_C(nh, TOPIC_NAME_SIMULATION_DYNC_MAP_LOC_TO_CENTER, TOPIC_NAME_SIMULATION_DYNC_MAP_CENTER_TO_LOC, objRecvFunc)
    {
    }

private:
};



#endif

