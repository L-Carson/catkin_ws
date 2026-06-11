
#ifndef __COMM_MSG_IMAGE_AREAS_H__
#define __COMM_MSG_IMAGE_AREAS_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/visionAreas.h"
#include <ros/ros.h>
#include <functional>
#include <string>
#include <vector>


/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class IMAGE_AREAS_SENDER_C
{
public:
    IMAGE_AREAS_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::visionAreas>(TOPIC_NAME_VISION_SEG_INFO, 10);
    }


    /* 此处为提高效率，像素点入参不要const，直接交换过来使用。 */
    void Send(const ros::Time &objImageTime, const std::string &strCameraName, std::vector<comm_msg::visionArea> &vectArea, const LABEL_STATE_ENUM& state = LABEL_STATE_YES)
    {
        comm_msg::visionAreas stMsg;
        stMsg.header.stamp     = objImageTime;
        stMsg.header.frame_id  = "base_link";
        stMsg.cameraId         = strCameraName;
        stMsg.pipeline.value   = uint8_t(state);
        stMsg.areas.swap(vectArea);

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

