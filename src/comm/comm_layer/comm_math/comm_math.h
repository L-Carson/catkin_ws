/*
Author      : gongzhihao
Update Date : 2025/10/10
*/
#ifndef COMM_MATH_H
#define COMM_MATH_H

#include "comm_layer/comm_base/comm_type.h"

constexpr float g_deg_to_rad = M_PI / 180.0f;
constexpr float g_rad_to_deg = 180.0f / M_PI;

// 是否近似0
bool IsNearZero(float value);

// 角度制->弧度制
float DegToRad(float value);

// 弧度制->角度制
float RadToDeg(float value);

// 包裹角度[-180, 180]
float WrapAngleDeg(float angle_deg);

// 包裹角度[-PI, PI]
float WrapAngleRad(float angle_rad);

// 取符号
int Sign(float value);

// 约束值
float Constrain(float value, float low, float high);

// 是否值在范围内
bool IsInRange(float value, float low, float high);

// 计算距离的平方
float CalcDisSquare(float x1, float y1, float x2, float y2);

// 计算距离的平方
template <typename T>
float CalcDisSquare(const T &point1, const T &point2);

// 计算距离
float CalcDis(float x1, float y1, float x2, float y2);

// 计算距离
template <typename T>
float CalcDis(const T &point1, const T &point2);

// 计算两点连线方向角
template <typename T>
float CalcBearingAngle(const T &start, const T &end);

// 局部坐标系转至全局坐标系 pa + ab -> pb
void LocalToGlobal(const Pose2D &pa, const Pose2D &ab, Pose2D &pb);

// 局部坐标系转至全局坐标系 pa + ab -> pb
void LocalToGlobal(const Pose2D &pa, const std::vector<Pose2D> &ab, std::vector<Pose2D> &pb);

// 局部坐标系转至全局坐标系 pa + ba -> pb
void LocalToGlobal2(const Pose2D &pa, const Pose2D &ba, Pose2D &pb);

// 全局坐标系转至局部坐标系 pa + pb -> ab
void GlobalToLocal(const Pose2D &pa, const Pose2D &pb, Pose2D &ab);

// 全局坐标系转至局部坐标系 pa + pb -> ab
void GlobalToLocal(const Pose2D &pa, const std::vector<Pose2D> &pb, std::vector<Pose2D> &ab);

// 反转坐标系 ab -> ba
void InvertPose(const Pose2D &ab, Pose2D &ba);

// 相加向量
Point2D AddVector(const Point2D &vector1, const Point2D &vector2);

// 相减向量
Point2D SubVector(const Point2D &vector1, const Point2D &vector2);

// 缩放向量
Point2D ScaleVector(float scalar, const Point2D &vector);

// 计算向量点积
float CalcVectorDotProduct(const Point2D &vector1, const Point2D &vector2);

// 计算向量叉积
float CalcVectorCrossProduct(const Point2D &vector1, const Point2D &vector2);

// 计算向量模的平方
float CalcVectorNormSquare(const Point2D &vector);

// 计算向量模
float CalcVectorNorm(const Point2D &vector);

// 计算向量夹角
float CalcVectorAngle(const Point2D &vector1, const Point2D &vector2);

#endif
