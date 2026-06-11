#ifndef __COMM_MSG_TOOL_POINT_H__
#define __COMM_MSG_TOOL_POINT_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "geometry_msgs/PointStamped.h"
#include <ros/ros.h>
#include <functional>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class TOOL_POINT_RECEIVER_C
{
public:
    TOOL_POINT_RECEIVER_C(ros::NodeHandle &nh, BOOL isActOnce = true, const std::string &strTopicName = TOPIC_NAME_TOOL_POINT)
                            : isGetMode_(true),
                              isActOnce_(isActOnce),
                              objRecvFunc_(NULL)
    {
        isExist_       = false;
        objSubscriber_ = nh.subscribe(strTopicName, 1, &TOOL_POINT_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    TOOL_POINT_RECEIVER_C(ros::NodeHandle &nh, const std::function<void(const COORDINATE_2D_STRU &)> &objRecvFunc, const std::string &strTopicName = TOPIC_NAME_TOOL_POINT)
                            : isGetMode_(false),
                              isActOnce_(true),
                              objRecvFunc_(objRecvFunc)
    {
        isExist_       = false;
        objSubscriber_ = nh.subscribe(strTopicName, 1, &TOOL_POINT_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 Get(COORDINATE_2D_STRU &stPoint)
    {
        R_ASSERT(isGetMode_, -1);

        if (!isExist_) {
            return -1;
        }

        stPoint  = stPoint_;
        isExist_ = isActOnce_ ? false : true;
        return 0;
    }

private:
    const BOOL                                             isGetMode_;
    const BOOL                                             isActOnce_;
    const std::function<void(const COORDINATE_2D_STRU &)>  objRecvFunc_;
    ros::Subscriber                                        objSubscriber_;
    BOOL                                                   isExist_;
    COORDINATE_2D_STRU                                     stPoint_;

    void ReceiveMsgCallBack(const geometry_msgs::PointStamped::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);

        stPoint_.x  = pstMsg->point.x;
        stPoint_.y  = pstMsg->point.y;
        ST_LOG_INFO("x=%f, y=%f.", stPoint_.x, stPoint_.y);

        if (isGetMode_) {
            isExist_ = true;
        } else {
            R_ASSERT(objRecvFunc_ != NULL, NOTHING);
            objRecvFunc_(stPoint_);
        }
    }
};

#endif

