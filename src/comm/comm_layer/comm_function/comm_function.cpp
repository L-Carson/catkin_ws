/*
Author      : gongzhihao
Update Date : 2025/07/29
*/
#include "comm_layer/comm_function/comm_function.h"

// 十六进制->字符串
std::string HexToStr(const unsigned char *hex, int hex_size)
{
    std::string str_data;

    if (hex == nullptr || hex_size <= 0)
    {
        return str_data;
    }

    static std::string hex_table = "0123456789ABCDEF";
    str_data.reserve(2 * hex_size);

    for (int i = 0; i < hex_size; ++i)
    {
        str_data += hex_table[hex[i] >> 4];
        str_data += hex_table[hex[i] & 0x0F];
    }

    return str_data;
}

// Pose2D->PoseStamped
geometry_msgs::PoseStamped Pose2DToPoseStamped(const std::string &frame_id, const Pose2D &pose_2d)
{
    geometry_msgs::PoseStamped pose_stamped;

    pose_stamped.header.frame_id = frame_id;
    pose_stamped.header.stamp = ros::Time::now();

    pose_stamped.pose.position.x = pose_2d.x;
    pose_stamped.pose.position.y = pose_2d.y;
    pose_stamped.pose.position.z = 0.0;
    pose_stamped.pose.orientation = tf::createQuaternionMsgFromYaw(pose_2d.theta);

    return pose_stamped;
}

// 格式化字符串
std::string FormatString(const char *format, ...)
{
    va_list args;
    std::string str_data;

    va_start(args, format);
    int str_size = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (str_size > 0)
    {
        str_data.resize(str_size);
        va_start(args, format);
        vsnprintf(&str_data[0], str_size + 1, format, args);
        va_end(args);
    }

    return str_data;
}

// 生成矩形
void GenRectangle(char quadrant,
                  float min_x,
                  float max_x,
                  float min_y,
                  float max_y,
                  geometry_msgs::PolygonStamped &polygon_stamped)
{
    // 根据象限的顺序添加端点
    switch (quadrant)
    {
    case 0x14:
    {
        AddPolygonVertex(polygon_stamped, min_x, (min_y + max_y) / 2.0f); // 下侧中点
        AddPolygonVertex(polygon_stamped, min_x, min_y);                  // 右下角
        AddPolygonVertex(polygon_stamped, max_x, min_y);                  // 右上角
        AddPolygonVertex(polygon_stamped, max_x, max_y);                  // 左上角
        AddPolygonVertex(polygon_stamped, min_x, max_y);                  // 左下角
        AddPolygonVertex(polygon_stamped, min_x, (min_y + max_y) / 2.0f); // 下侧中点
        break;
    }
    case 0x23:
    {
        AddPolygonVertex(polygon_stamped, max_x, (min_y + max_y) / 2.0f); // 上侧中点
        AddPolygonVertex(polygon_stamped, max_x, max_y);                  // 左上角
        AddPolygonVertex(polygon_stamped, min_x, max_y);                  // 左下角
        AddPolygonVertex(polygon_stamped, min_x, min_y);                  // 右下角
        AddPolygonVertex(polygon_stamped, max_x, min_y);                  // 右上角
        AddPolygonVertex(polygon_stamped, max_x, (min_y + max_y) / 2.0f); // 上侧中点
        break;
    }
    case 0x12:
    {
        AddPolygonVertex(polygon_stamped, min_y, (min_x + max_x) / 2.0f); // 右侧中点
        AddPolygonVertex(polygon_stamped, max_x, min_y);                  // 右上角
        AddPolygonVertex(polygon_stamped, max_x, max_y);                  // 左上角
        AddPolygonVertex(polygon_stamped, min_x, max_y);                  // 左下角
        AddPolygonVertex(polygon_stamped, min_x, min_y);                  // 右下角
        AddPolygonVertex(polygon_stamped, min_y, (min_x + max_x) / 2.0f); // 右侧中点
        break;
    }
    case 0x34:
    {
        AddPolygonVertex(polygon_stamped, max_y, (min_x + max_x) / 2.0f); // 左侧中点
        AddPolygonVertex(polygon_stamped, min_x, max_y);                  // 左下角
        AddPolygonVertex(polygon_stamped, min_x, min_y);                  // 右下角
        AddPolygonVertex(polygon_stamped, max_x, min_y);                  // 右上角
        AddPolygonVertex(polygon_stamped, max_x, max_y);                  // 左上角
        AddPolygonVertex(polygon_stamped, max_y, (min_x + max_x) / 2.0f); // 左侧中点
        break;
    }
    default:
    {
        break;
    }
    }
}

// 添加多边形顶点
void AddPolygonVertex(geometry_msgs::PolygonStamped &polygon_stamped, float x, float y)
{
    geometry_msgs::Point32 vertex;

    vertex.x = x;
    vertex.y = y;
    vertex.z = 0.0f;

    polygon_stamped.polygon.points.push_back(vertex);
}

// 点是否在多边形内（凹凸多边形有效）
template <typename T>
bool IsInsidePolygon(const std::vector<T> &vertices, float x, float y)
{
    float min_x;
    float max_x;
    float min_y;
    float max_y;

    // 计算包围边界
    CalcBoundingBox(vertices, min_x, max_x, min_y, max_y);

    // 点是否在包围边界内-快速排除
    if (!IsInsideBoundingBox(x, y, min_x, max_x, min_y, max_y))
    {
        return false;
    }

    T line1_start;
    T line1_end;
    T line2_start;
    T line2_end;

    // 取多边形外一点，与目标点连线，如果与多边形边交点为偶数则目标点多边形外面，反之则在里面
    line1_start.x = max_x + 10.0f;
    line1_start.y = max_y + 10.0f;
    line1_end.x = x;
    line1_end.y = y;

    int count = 0;
    int vertices_size = static_cast<int>(vertices.size());

    for (int i = 0; i < vertices_size; ++i)
    {
        line2_start.x = vertices[i].x;
        line2_start.y = vertices[i].y;
        line2_end.x = vertices[(i + 1) % vertices_size].x;
        line2_end.y = vertices[(i + 1) % vertices_size].y;

        // 线段是否相交
        if (IsIntersect(line1_start, line1_end, line2_start, line2_end))
        {
            count++;
        }
    }

    // 如果与多边形边交点为偶数则目标点多边形外面，反之则在里面
    return ((count & 1) == 1);
}

template bool IsInsidePolygon(const std::vector<Pose2D> &vertices, float x, float y);
template bool IsInsidePolygon(const std::vector<Point2D> &vertices, float x, float y);

// 计算包围边界
template <typename T>
void CalcBoundingBox(const std::vector<T> &vertices,
                     float &min_x,
                     float &max_x,
                     float &min_y,
                     float &max_y)
{
    min_x = g_max_float;
    max_x = g_min_float;
    min_y = g_max_float;
    max_y = g_min_float;

    for (const T &vertex : vertices)
    {
        min_x = std::min(min_x, vertex.x);
        max_x = std::max(max_x, vertex.x);
        min_y = std::min(min_y, vertex.y);
        max_y = std::max(max_y, vertex.y);
    }
}

template void CalcBoundingBox(const std::vector<Pose2D> &vertices,
                              float &min_x,
                              float &max_x,
                              float &min_y,
                              float &max_y);
template void CalcBoundingBox(const std::vector<Point2D> &vertices,
                              float &min_x,
                              float &max_x,
                              float &min_y,
                              float &max_y);

// 点是否在包围边界内
bool IsInsideBoundingBox(float x, float y, float min_x, float max_x, float min_y, float max_y)
{
    return (x >= min_x && x <= max_x && y >= min_y && y <= max_y);
}

// 线段是否相交
template <typename T>
bool IsIntersect(const T &line1_start,
                 const T &line1_end,
                 const T &line2_start,
                 const T &line2_end)
{
    // 快速排除
    if (std::min(line1_start.x, line1_end.x) > std::max(line2_start.x, line2_end.x) ||
        std::min(line1_start.y, line1_end.y) > std::max(line2_start.y, line2_end.y) ||
        std::max(line1_start.x, line1_end.x) < std::min(line2_start.x, line2_end.x) ||
        std::max(line1_start.y, line1_end.y) < std::min(line2_start.y, line2_end.y))
    {
        return false;
    }

    double line1_dx = line1_end.x - line1_start.x;
    double line1_dy = line1_end.y - line1_start.y;
    double line2_dx = line2_end.x - line2_start.x;
    double line2_dy = line2_end.y - line2_start.y;

    double u = (line2_start.x - line1_start.x) * line1_dy - line1_dx * (line2_start.y - line1_start.y);
    double v = (line2_end.x - line1_start.x) * line1_dy - line1_dx * (line2_end.y - line1_start.y);

    if (u * v > 1e-6)
    {
        return false;
    }

    double w = (line1_start.x - line2_start.x) * line2_dy - line2_dx * (line1_start.y - line2_start.y);
    double z = (line1_end.x - line2_start.x) * line2_dy - line2_dx * (line1_end.y - line2_start.y);

    return (w * z <= 1e-6);
}

template bool IsIntersect(const Pose2D &line1_start,
                          const Pose2D &line1_end,
                          const Pose2D &line2_start,
                          const Pose2D &line2_end);
template bool IsIntersect(const Point2D &line1_start,
                          const Point2D &line1_end,
                          const Point2D &line2_start,
                          const Point2D &line2_end);
