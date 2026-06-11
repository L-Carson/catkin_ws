/*
Author      : gongzhihao
Update Date : 2025/10/10
*/
#include "comm_layer/comm_math/comm_math.h"

// 是否近似0
bool IsNearZero(float value)
{
    return (std::abs(value) < 0.001f);
}

// 角度制->弧度制
float DegToRad(float value)
{
    return (value * g_deg_to_rad);
}

// 弧度制->角度制
float RadToDeg(float value)
{
    return (value * g_rad_to_deg);
}

// 包裹角度[-180, 180]
float WrapAngleDeg(float angle_deg)
{
    angle_deg = std::fmod(angle_deg, 360.0f);
    angle_deg += (angle_deg < 0.0f) * 360.0f;
    return (angle_deg - (angle_deg > 180.0f) * 360.0f);
}

// 包裹角度[-PI, PI]
float WrapAngleRad(float angle_rad)
{
    angle_rad = std::fmod(angle_rad, 2.0f * M_PI);
    angle_rad += (angle_rad < 0.0f) * 2.0f * M_PI;
    return (angle_rad - (angle_rad > M_PI) * 2.0f * M_PI);
}

// 取符号
int Sign(float value)
{
    return (value > 0.0f) - (value < 0.0f);
}

// 约束值
float Constrain(float value, float low, float high)
{
    float out = 0.0f;

    if (std::isfinite(value) && std::isfinite(low) && std::isfinite(high))
    {
        if (low <= high)
        {
            out = (value < low ? low : (value > high ? high : value));
        }
    }

    return out;
}

// 是否值在范围内
bool IsInRange(float value, float low, float high)
{
    if (std::isfinite(value) && std::isfinite(low) && std::isfinite(high))
    {
        if (low <= high)
        {
            return (value >= low && value <= high);
        }
    }

    return false;
}

// 计算距离的平方
float CalcDisSquare(float x1, float y1, float x2, float y2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    return (dx * dx + dy * dy);
}

// 计算距离的平方
template <typename T>
float CalcDisSquare(const T &point1, const T &point2)
{
    return CalcDisSquare(point1.x, point1.y, point2.x, point2.y);
}

template float CalcDisSquare(const Pose2D &point1, const Pose2D &point2);
template float CalcDisSquare(const Point2D &point1, const Point2D &point2);

// 计算距离
float CalcDis(float x1, float y1, float x2, float y2)
{
    return std::sqrt(CalcDisSquare(x1, y1, x2, y2));
}

// 计算距离
template <typename T>
float CalcDis(const T &point1, const T &point2)
{
    return CalcDis(point1.x, point1.y, point2.x, point2.y);
}

template float CalcDis(const Pose2D &point1, const Pose2D &point2);
template float CalcDis(const Point2D &point1, const Point2D &point2);

// 计算两点连线方向角
template <typename T>
float CalcBearingAngle(const T &start, const T &end)
{
    return std::atan2(end.y - start.y, end.x - start.x);
}

template float CalcBearingAngle(const Pose2D &start, const Pose2D &end);
template float CalcBearingAngle(const Point2D &start, const Point2D &end);

// 局部坐标系转至全局坐标系 pa + ab -> pb
void LocalToGlobal(const Pose2D &pa, const Pose2D &ab, Pose2D &pb)
{
    float cos_angle = std::cos(pa.theta);
    float sin_angle = std::sin(pa.theta);

    pb.x = pa.x + ab.x * cos_angle - ab.y * sin_angle;
    pb.y = pa.y + ab.x * sin_angle + ab.y * cos_angle;
    pb.theta = WrapAngleRad(pa.theta + ab.theta);
}

// 局部坐标系转至全局坐标系 pa + ab -> pb
void LocalToGlobal(const Pose2D &pa, const std::vector<Pose2D> &ab, std::vector<Pose2D> &pb)
{
    float cos_angle = std::cos(pa.theta);
    float sin_angle = std::sin(pa.theta);

    size_t ab_size = ab.size();
    pb.resize(ab_size);

    for (size_t i = 0; i < ab_size; ++i)
    {
        const Pose2D &one_ab = ab[i];
        Pose2D &one_pb = pb[i];

        one_pb.x = pa.x + one_ab.x * cos_angle - one_ab.y * sin_angle;
        one_pb.y = pa.y + one_ab.x * sin_angle + one_ab.y * cos_angle;
        one_pb.theta = WrapAngleRad(pa.theta + one_ab.theta);
    }
}

// 局部坐标系转至全局坐标系 pa + ba -> pb
void LocalToGlobal2(const Pose2D &pa, const Pose2D &ba, Pose2D &pb)
{
    float pb_theta = WrapAngleRad(pa.theta - ba.theta);
    float cos_angle = std::cos(pb_theta);
    float sin_angle = std::sin(pb_theta);

    pb.x = pa.x - ba.x * cos_angle + ba.y * sin_angle;
    pb.y = pa.y - ba.x * sin_angle - ba.y * cos_angle;
    pb.theta = pb_theta;
}

// 全局坐标系转至局部坐标系 pa + pb -> ab
void GlobalToLocal(const Pose2D &pa, const Pose2D &pb, Pose2D &ab)
{
    float cos_angle = std::cos(pa.theta);
    float sin_angle = std::sin(pa.theta);
    float dx = pb.x - pa.x;
    float dy = pb.y - pa.y;

    ab.x = dx * cos_angle + dy * sin_angle;
    ab.y = dy * cos_angle - dx * sin_angle;
    ab.theta = WrapAngleRad(pb.theta - pa.theta);
}

// 全局坐标系转至局部坐标系 pa + pb -> ab
void GlobalToLocal(const Pose2D &pa, const std::vector<Pose2D> &pb, std::vector<Pose2D> &ab)
{
    float cos_angle = std::cos(pa.theta);
    float sin_angle = std::sin(pa.theta);

    size_t pb_size = pb.size();
    ab.resize(pb_size);

    for (size_t i = 0; i < pb_size; ++i)
    {
        const Pose2D &one_pb = pb[i];
        Pose2D &one_ab = ab[i];

        float dx = one_pb.x - pa.x;
        float dy = one_pb.y - pa.y;

        one_ab.x = dx * cos_angle + dy * sin_angle;
        one_ab.y = dy * cos_angle - dx * sin_angle;
        one_ab.theta = WrapAngleRad(one_pb.theta - pa.theta);
    }
}

// 反转坐标系 ab -> ba
void InvertPose(const Pose2D &ab, Pose2D &ba)
{
    float cos_angle = std::cos(ab.theta);
    float sin_angle = std::sin(ab.theta);

    ba.x = 0.0f - ab.x * cos_angle - ab.y * sin_angle;
    ba.y = 0.0f + ab.x * sin_angle - ab.y * cos_angle;
    ba.theta = 0.0f - ab.theta;
}

// 相加向量
Point2D AddVector(const Point2D &vector1, const Point2D &vector2)
{
    return {vector1.x + vector2.x, vector1.y + vector2.y};
}

// 相减向量
Point2D SubVector(const Point2D &vector1, const Point2D &vector2)
{
    return {vector1.x - vector2.x, vector1.y - vector2.y};
}

// 缩放向量
Point2D ScaleVector(float scalar, const Point2D &vector)
{
    return {scalar * vector.x, scalar * vector.y};
}

// 计算向量点积
float CalcVectorDotProduct(const Point2D &vector1, const Point2D &vector2)
{
    return (vector1.x * vector2.x + vector1.y * vector2.y);
}

// 计算向量叉积
float CalcVectorCrossProduct(const Point2D &vector1, const Point2D &vector2)
{
    return (vector1.x * vector2.y - vector2.x * vector1.y);
}

// 计算向量模的平方
float CalcVectorNormSquare(const Point2D &vector)
{
    return (vector.x * vector.x + vector.y * vector.y);
}

// 计算向量模
float CalcVectorNorm(const Point2D &vector)
{
    return std::sqrt(CalcVectorNormSquare(vector));
}

// 计算向量夹角
float CalcVectorAngle(const Point2D &vector1, const Point2D &vector2)
{
    float dot_product = CalcVectorDotProduct(vector1, vector2);
    float norm1 = CalcVectorNorm(vector1);
    float norm2 = CalcVectorNorm(vector2);

    if (IsNearZero(norm1) || IsNearZero(norm2))
    {
        return 0.0f;
    }

    return std::acos(dot_product / (norm1 * norm2));
}
