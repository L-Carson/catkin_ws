// 屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "pcpt_net_ros.h"
#pragma GCC diagnostic pop


void PCPT_ROS_C::Init() {
    InitColor();
}

void PCPT_ROS_C::InitColor()
{
    carColor.r = 0;
    carColor.g = 229;
    carColor.b = 238;
    carColor.a = 0.8;

    biCycleColor_.r = 127;
    biCycleColor_.g = 0;
    biCycleColor_.b = 255;
    biCycleColor_.a = 0.8;

    pedestrialColor_.r = 0;
    pedestrialColor_.g = 200;
    pedestrialColor_.b = 0;
    pedestrialColor_.a = 0.8;

    textColor_.r = 255;
    textColor_.g = 255;
    textColor_.b = 255;
    textColor_.a = 0.9;
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void PCPT_ROS_C::PublishPoints(sensor_msgs::PointCloud2::Ptr pointsMessage, ros::Time rosTime)
{
  pointsMessage->header.frame_id = "base_link";
  pointsMessage->header.stamp    = rosTime;
  pointCloudPublisher_.publish(pointsMessage);
}

/**************************************************************************************
功能描述:网络输出 数据转Boxes 并发布
修改记录:
**************************************************************************************/
void PCPT_ROS_C::ShowNetDetectMsgs(vector<lidarDataType> resultOut, ros::Time rosTime)
{
    if (!pubDetectBoxMarkers_.getNumSubscribers() &&
        !pubDetectTextMarkers_.getNumSubscribers()) {
        return;
    }

    visualization_msgs::MarkerArray markerBox;
    visualization_msgs::MarkerArray textMarkerArray;

    for (size_t i = 0; i < resultOut.size(); ++i) {
        const lidarDataType &box = resultOut[i];
        visualization_msgs::Marker markBox;
        visualization_msgs::Marker markText;
        std_msgs::Header header;
        header.stamp    = rosTime;
        header.frame_id = "base_link";
        markBox.header  = header;
        markText.header = header;

        switch (box.label) {
            case PCD_LABEL_ENUM::EN_CAR:
                markBox.type    = visualization_msgs::Marker::CUBE;
                markBox.scale.x = box.width;
                markBox.scale.y = box.length;
                markBox.scale.z = box.height;
                markBox.color   = carColor;
                break;
            case PCD_LABEL_ENUM::EN_BICYCLE:
                markBox.type    = visualization_msgs::Marker::CUBE;
                markBox.scale.x = box.width;
                markBox.scale.y = box.length;
                markBox.scale.z = box.height;
                markBox.color   = biCycleColor_;
                break;
            case PCD_LABEL_ENUM::EN_PEDESTRIAN:
                markBox.type    = visualization_msgs::Marker::CYLINDER;
                markBox.scale.x = box.width>box.length ? box.length : box.width;
                markBox.scale.y = box.width>box.length ? box.length : box.width;
                markBox.scale.z = box.height;
                markBox.color   = pedestrialColor_;
                break;
            default:
                markBox.type    = visualization_msgs::Marker::CUBE;
                markBox.scale.x = box.width;
                markBox.scale.y = box.length;
                markBox.scale.z = box.height;
                markBox.color   = carColor;
                break;
        }

        Eigen::AngleAxisd rotationVector(box.angle, Eigen::Vector3d(0, 0, 1));
        Eigen::Quaterniond quaternion(rotationVector);

        markBox.action             = visualization_msgs::Marker::ADD;
        markBox.pose.position.x    = box.x;
        markBox.pose.position.y    = box.y;
        markBox.pose.position.z    = box.z + lidarHeight_;
        markBox.pose.orientation.x = quaternion.x();
        markBox.pose.orientation.y = quaternion.y();
        markBox.pose.orientation.z = quaternion.z();
        markBox.pose.orientation.w = quaternion.w();
        markBox.ns                 = "/box_markers";
        markBox.id                 = 200 + i;
        markerBox.markers.push_back(markBox);

        markText.type               = visualization_msgs::Marker::TEXT_VIEW_FACING;
        markText.color              = textColor_;
        markText.scale.z            = 1.0;
        markText.action             = visualization_msgs::Marker::ADD;
        markText.pose.position.x    = box.x;
        markText.pose.position.y    = box.y;
        markText.pose.position.z    = box.z + box.height / 2 ;
        markText.pose.orientation.x = quaternion.x();
        markText.pose.orientation.y = quaternion.y();
        markText.pose.orientation.z = quaternion.z();
        markText.pose.orientation.w = quaternion.w();

        char s_score[50];
        sprintf(s_score, "%.2f", box.score);
        markText.text = string(s_score);
        markText.ns   = "/text_markers";
        markText.id   = 200 + i;
        textMarkerArray.markers.push_back(markText);
    }

    visualization_msgs::MarkerArray netBoxtMsgs;
    visualization_msgs::MarkerArray netTextMsgs;
    netBoxtMsgs.markers.insert(netBoxtMsgs.markers.end(), markerBox.markers.begin(), markerBox.markers.end());
    netTextMsgs.markers.insert(netTextMsgs.markers.end(), textMarkerArray.markers.begin(), textMarkerArray.markers.end());

    visualization_msgs::MarkerArray netBoxtMsgsTemp = netBoxtMsgs;
    visualization_msgs::MarkerArray netTextMsgsTemp = netTextMsgs;

    for (int i = 0; i < (int)makerArrayBoxPre.markers.size(); ++i) {
        visualization_msgs::Marker markBox_pre, markerText_pre;
        markBox_pre    = makerArrayBoxPre.markers[i];
        markerText_pre = makerArrayTextPre.markers[i];
        int isDelete = 1;
        for (int j = 0; j < (int)netBoxtMsgs.markers.size(); ++j){
            visualization_msgs::Marker markBox_new;
            markBox_new = netBoxtMsgs.markers[j];
            if (markBox_pre.id == markBox_new.id){
                isDelete = 0;
                break;
            }
        }

        if (isDelete) {
            markBox_pre.action = visualization_msgs::Marker::DELETE;
            netBoxtMsgs.markers.push_back(markBox_pre);
            markerText_pre.action = visualization_msgs::Marker::DELETE;
            netTextMsgs.markers.push_back(markerText_pre);
        }
    }
    pubDetectBoxMarkers_.publish(netBoxtMsgs);
    pubDetectTextMarkers_.publish(netTextMsgs);

    makerArrayBoxPre  = netBoxtMsgsTemp ;
    makerArrayTextPre = netTextMsgsTemp;
}
