/*
Author      : gongzhihao
Update Date : 2025/08/04
*/
#ifndef COMM_VISUAL_H
#define COMM_VISUAL_H

#include "comm_layer/comm_function/comm_function.h"

// 可视化PoseStamped
void VisualPoseStamped(const ros::Publisher &pub, const std::string &frame_id, const Pose2D &pose_2d);

// 可视化Path
void VisualPath(const ros::Publisher &pub, const std::string &frame_id, const std::vector<Pose2D> &route);

// 可视化PolygonStamped
void VisualPolygonStamped(const ros::Publisher &pub,
                          const std::string &frame_id,
                          const std::vector<std::vector<Pose2D>> &polygons);

// 可视化OccupancyGrid
void VisualOccupancyGrid(const ros::Publisher &pub,
                         const std::string &frame_id,
                         float resolution,
                         int width,
                         int height,
                         float origin_x,
                         float origin_y,
                         const std::vector<signed char> &data);

#endif
