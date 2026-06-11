#ifndef __COMM_MSG_LOOP_CONSTRAINT_H__
#define __COMM_MSG_LOOP_CONSTRAINT_H__

#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include "comm/comm_base.h"
#include "comm_extend/comm_base_extend.h"

class LOOP_CONSTRAINTS_MARKER_SENDER_C
{
public:
    LOOP_CONSTRAINTS_MARKER_SENDER_C(ros::NodeHandle &nh, const char *pcTopicName = "/loop_constraints", BOOL latch = false)
    {
        publisher_ = nh.advertise<visualization_msgs::MarkerArray>(pcTopicName, 1, latch);
    };

    void Send(const std::vector<LOOP_CONSTRAINT_STRU> &vstLoopConstraints)
    {
        visualization_msgs::MarkerArray markerArray;

        visualization_msgs::Marker markerNode;
        markerNode.header.frame_id = "map";
        markerNode.header.stamp = ros::Time::now();
        markerNode.action = visualization_msgs::Marker::ADD;
        markerNode.type   = visualization_msgs::Marker::SPHERE_LIST;
        markerNode.ns = "loop_nodes";
        markerNode.id = 0;
        markerNode.pose.orientation.w = 1;
        markerNode.scale.x = 0.3; markerNode.scale.y = 0.3; markerNode.scale.z = 0.3;
        markerNode.color.r = 0;   markerNode.color.g = 0.8; markerNode.color.b = 1;
        markerNode.color.a = 1;

        visualization_msgs::Marker markerEdge;
        markerEdge.header.frame_id = "map";
        markerEdge.header.stamp    = ros::Time::now();
        markerEdge.action = visualization_msgs::Marker::ADD;
        markerEdge.type   = visualization_msgs::Marker::LINE_LIST;
        markerEdge.ns = "loop_edges";
        markerEdge.id = 1;
        markerEdge.pose.orientation.w = 1;
        markerEdge.scale.x = 0.1; markerEdge.scale.y = 0.1; markerEdge.scale.z = 0.1;
        markerEdge.color.r = 0.9; markerEdge.color.g = 0.9; markerEdge.color.b = 0;
        markerEdge.color.a = 1;

        for (auto it = vstLoopConstraints.begin(); it != vstLoopConstraints.end(); ++it)
        {
            geometry_msgs::Point p;
            p.x = it->stPos1.x;  p.y = it->stPos1.y;  p.z = it->stPos1.z;
            markerNode.points.push_back(p);
            markerEdge.points.push_back(p);

            p.x = it->stPos2.x;  p.y = it->stPos2.y;  p.z = it->stPos2.z;
            markerNode.points.push_back(p);
            markerEdge.points.push_back(p);
        }

        markerArray.markers.push_back(markerNode);
        markerArray.markers.push_back(markerEdge);
        publisher_.publish(markerArray);
    }

private:
    ros::Publisher  publisher_;
};

#endif
