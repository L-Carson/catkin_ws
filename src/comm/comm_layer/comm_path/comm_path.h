/*
Author      : gongzhihao
Update Date : 2025/09/30
*/
#ifndef COMM_PATH_H
#define COMM_PATH_H

#include "comm_layer/comm_math/comm_math.h"

// 规则化路径
void NormalizePath(std::vector<Pose2D> &path);

// 规则化路径角度
void NormalizePathAngle(std::vector<Pose2D> &path);

// 搜索最近位姿
bool SearchNearestPose(const std::vector<Pose2D> &path,
                       const Pose2D &pose,
                       ProjectionInfo &projection_info);

// 搜索最近位姿
bool SearchNearestPose(const std::vector<Pose2D> &path,
                       const Pose2D &pose,
                       int begin_index,
                       int end_index,
                       ProjectionInfo &projection_info);

// 生成直线路径
void GenLinePath(const Pose2D &start_pose, const Pose2D &end_pose, std::vector<Pose2D> &line_path);

#endif
