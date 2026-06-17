#pragma once

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_define.h"

/**
 * @brief 体素Marker发布器
 *
 */
class PUB_BOX_MARKER_C
{
public:
    PUB_BOX_MARKER_C(ros::NodeHandle& nh, const char* pTopicName) : nh_(nh), preBoxMarkerSize_(0)
    {
        boxMarkerPub_ = nh.advertise<visualization_msgs::MarkerArray>(pTopicName, 1);
    }

    ~PUB_BOX_MARKER_C(){}

    void PubBoxMarker(const vector<BOX_INFO_STRU> &vstBoxInfo, const std_msgs::Header& header, const char* ns = "box");
    void PubBoxMarker(const vector<BOX_INFO_STRU> &vstBoxInfo, double stamp, const char* frameId = PCPT_FRAME_ID_CAR, const char* ns = "box");
    void PubBoxMarker(const vector<BOX_INFO_STRU> &vstBoxInfo);
    void PubBoxMarker(const BOX_INFO_STRU &stBoxInfo);

private:
    ros::NodeHandle&    nh_;
    ros::Publisher      boxMarkerPub_;   //发布box Marker
    uint32_t            preBoxMarkerSize_;
};



