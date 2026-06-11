/*
Author      : gongzhihao
Update Date : 2025/07/30
*/
#ifndef COMM_FUNCTION_H
#define COMM_FUNCTION_H

#include <cstdio>
#include <cstdarg>

#include "comm_layer/comm_math/comm_math.h"

// 十六进制->字符串
std::string HexToStr(const unsigned char *hex, int hex_size);

// Pose2D->PoseStamped
geometry_msgs::PoseStamped Pose2DToPoseStamped(const std::string &frame_id, const Pose2D &pose_2d);

// 格式化字符串
std::string FormatString(const char *format, ...);

// 生成矩形
void GenRectangle(char quadrant,
                  float min_x,
                  float max_x,
                  float min_y,
                  float max_y,
                  geometry_msgs::PolygonStamped &polygon_stamped);

// 添加多边形顶点
void AddPolygonVertex(geometry_msgs::PolygonStamped &polygon_stamped, float x, float y);

// 点是否在多边形内（凹凸多边形有效）
template <typename T>
bool IsInsidePolygon(const std::vector<T> &vertices, float x, float y);

// 计算包围边界
template <typename T>
void CalcBoundingBox(const std::vector<T> &vertices,
                     float &min_x,
                     float &max_x,
                     float &min_y,
                     float &max_y);

// 点是否在包围边界内
bool IsInsideBoundingBox(float x, float y, float min_x, float max_x, float min_y, float max_y);

// 线段是否相交
template <typename T>
bool IsIntersect(const T &line1_start,
                 const T &line1_end,
                 const T &line2_start,
                 const T &line2_end);

#endif
