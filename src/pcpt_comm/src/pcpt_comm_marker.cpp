#include "pcpt_comm/pcpt_comm_marker.h"

void PUB_BOX_MARKER_C::PubBoxMarker(const vector<BOX_INFO_STRU> &vstBoxInfo, const std_msgs::Header& header, const char* ns)
{
    if (!boxMarkerPub_.getNumSubscribers()) return;

    visualization_msgs::MarkerArray markerArray;
    visualization_msgs::Marker box;

    box.header  = header;
    box.ns      = ns;
    box.type    = visualization_msgs::Marker::CUBE;
    box.action  = visualization_msgs::Marker::ADD;
    box.color.r = 1.0f;
    box.color.g = 1.0f;
    box.color.b = 0.0f;
    box.color.a = 0.5;

    int id = 0;

    for (const BOX_INFO_STRU &stBoxInfo : vstBoxInfo) {
        Eigen::Quaterniond quaternion;
        quaternion = Eigen::AngleAxisd(stBoxInfo.stRotatedRect.angle, Eigen::Vector3d::UnitZ()) *
                     Eigen::AngleAxisd(0, Eigen::Vector3d::UnitY()) *
                     Eigen::AngleAxisd(0, Eigen::Vector3d::UnitX());

        box.id                 = id;
        box.pose.position.x    = stBoxInfo.stRotatedRect.x;
        box.pose.position.y    = stBoxInfo.stRotatedRect.y;
        box.pose.position.z    = (stBoxInfo.minZ + stBoxInfo.maxZ) * 0.5;
        box.pose.orientation.x = quaternion.x();
        box.pose.orientation.y = quaternion.y();
        box.pose.orientation.z = quaternion.z();
        box.pose.orientation.w = quaternion.w();
        box.scale.x            = stBoxInfo.stRotatedRect.width;
        box.scale.y            = stBoxInfo.stRotatedRect.height;
        box.scale.z            = stBoxInfo.maxZ - stBoxInfo.minZ;

        markerArray.markers.push_back(box);
        id++;
    }

    /* 清除无效的Marker在rviz上的显示 */
    if (vstBoxInfo.size() < preBoxMarkerSize_) {
        box.action = visualization_msgs::Marker::DELETE;
        for (uint32_t i = 0; i < preBoxMarkerSize_ - vstBoxInfo.size(); ++i) {
            box.id = id;
            markerArray.markers.push_back(box);
            id++;
        }
    }
    preBoxMarkerSize_ = vstBoxInfo.size();

    boxMarkerPub_.publish(markerArray);
}

void PUB_BOX_MARKER_C::PubBoxMarker(const vector<BOX_INFO_STRU> &vstBoxInfo, double stamp, const char* frameId, const char* ns)
{
    std_msgs::Header header;
    header.stamp    = RosTime(stamp);
    header.frame_id = frameId;
    PubBoxMarker(vstBoxInfo, header, ns);
}

void PUB_BOX_MARKER_C::PubBoxMarker(const vector<BOX_INFO_STRU> &vstBoxInfo)
{
    PubBoxMarker(vstBoxInfo, RosStampNow());
}

void PUB_BOX_MARKER_C::PubBoxMarker(const BOX_INFO_STRU &stBoxInfo)
{
    vector<BOX_INFO_STRU> vstBoxInfo;
    vstBoxInfo.push_back(stBoxInfo);
    PubBoxMarker(vstBoxInfo);
}


