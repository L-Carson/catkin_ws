
#ifndef __COMM_MSG_IMAGE_RECTS_H__
#define __COMM_MSG_IMAGE_RECTS_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/visionRects.h"
#include <ros/ros.h>
#include <functional>
#include <string>
#include <vector>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class IMAGE_RECTS_SENDER_C
{
public:
    IMAGE_RECTS_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::visionRects>(TOPIC_NAME_VISION_DET_INFO, 10);
    }


    /* 此处为提高效率，对象框入参不要const，直接交换过来使用。 */
    void Send(const ros::Time &objImageTime, const std::string &strCameraName, std::vector<comm_msg::visionRect> &vectRect, const LABEL_STATE_ENUM& state = LABEL_STATE_YES)
    {
        comm_msg::visionRects stMsg;
        stMsg.header.stamp     = objImageTime;
        stMsg.header.frame_id  = "base_link";
        stMsg.cameraId         = strCameraName;
        stMsg.pipeline.value   = uint8_t(state);
        stMsg.rects.swap(vectRect);

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};



/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/


#endif

