/*
Author      : gongzhihao
Update Date : 2025/08/04
*/
#include "comm_layer/comm_visual/comm_visual.h"

// 可视化PoseStamped
void VisualPoseStamped(const ros::Publisher &pub, const std::string &frame_id, const Pose2D &pose_2d)
{
    if (pub.getNumSubscribers() <= 0)
    {
        return;
    }

    // Pose2D->PoseStamped
    geometry_msgs::PoseStamped pose_stamped = Pose2DToPoseStamped(frame_id, pose_2d);

    pub.publish(pose_stamped);
}

// 可视化Path
void VisualPath(const ros::Publisher &pub, const std::string &frame_id, const std::vector<Pose2D> &route)
{
    if (pub.getNumSubscribers() <= 0)
    {
        return;
    }

    // 路径点数
    size_t route_size = route.size();
    if (route_size <= 0)
    {
        return;
    }

    nav_msgs::Path path;

    path.header.frame_id = frame_id;
    path.header.stamp = ros::Time::now();

    path.poses.resize(route_size);

    for (size_t i = 0; i < route_size; ++i)
    {
        // Pose2D->PoseStamped
        path.poses[i] = Pose2DToPoseStamped(frame_id, route[i]);
    }

    pub.publish(path);
}

// 可视化PolygonStamped
void VisualPolygonStamped(const ros::Publisher &pub,
                          const std::string &frame_id,
                          const std::vector<std::vector<Pose2D>> &polygons)
{
    if (pub.getNumSubscribers() <= 0)
    {
        return;
    }

    // 多边形数
    size_t polygons_size = polygons.size();
    if (polygons_size <= 0)
    {
        return;
    }

    // 多边形顶点数
    size_t vertices_size = polygons[0].size();
    if (vertices_size <= 0)
    {
        return;
    }

    geometry_msgs::PolygonStamped polygon_stamped;

    polygon_stamped.header.frame_id = frame_id;
    polygon_stamped.header.stamp = ros::Time::now();

    polygon_stamped.polygon.points.resize(polygons_size * (vertices_size + 1) + polygons_size);

    // 多边形右上角顶点栈
    std::vector<Pose2D> upper_right_points;
    upper_right_points.resize(polygons_size);

    int point_cnt = 0;
    int upper_right_point_cnt = 0;

    // 遍历所有多边形
    for (const auto &vertices : polygons)
    {
        // 遍历当前多边形的所有顶点
        for (const auto &vertex : vertices)
        {
            polygon_stamped.polygon.points[point_cnt].x = vertex.x;
            polygon_stamped.polygon.points[point_cnt].y = vertex.y;
            polygon_stamped.polygon.points[point_cnt++].z = 0.0f;
        }

        // 最后再次插入第一个点闭合轮廓
        polygon_stamped.polygon.points[point_cnt].x = vertices[0].x;
        polygon_stamped.polygon.points[point_cnt].y = vertices[0].y;
        polygon_stamped.polygon.points[point_cnt++].z = 0.0f;

        // 将右上顶点入栈
        upper_right_points[upper_right_point_cnt].x = vertices.back().x;
        upper_right_points[upper_right_point_cnt++].y = vertices.back().y;
    }

    // 倒序插入右上顶点
    for (int cnt = upper_right_point_cnt - 1; cnt >= 0; --cnt)
    {
        polygon_stamped.polygon.points[point_cnt].x = upper_right_points[cnt].x;
        polygon_stamped.polygon.points[point_cnt].y = upper_right_points[cnt].y;
        polygon_stamped.polygon.points[point_cnt++].z = 0.0f;
    }

    pub.publish(polygon_stamped);
}

// 可视化OccupancyGrid
void VisualOccupancyGrid(const ros::Publisher &pub,
                         const std::string &frame_id,
                         float resolution,
                         int width,
                         int height,
                         float origin_x,
                         float origin_y,
                         const std::vector<signed char> &data)
{
    if (pub.getNumSubscribers() <= 0)
    {
        return;
    }

    if (width * height != data.size())
    {
        return;
    }

    nav_msgs::OccupancyGrid occupancy_grid;

    occupancy_grid.header.frame_id = frame_id;
    occupancy_grid.header.stamp = ros::Time::now();

    occupancy_grid.info.resolution = resolution;
    occupancy_grid.info.width = width;
    occupancy_grid.info.height = height;
    occupancy_grid.info.origin.position.x = origin_x;
    occupancy_grid.info.origin.position.y = origin_y;
    occupancy_grid.info.origin.position.z = 0.0;
    occupancy_grid.info.origin.orientation.x = 0.0;
    occupancy_grid.info.origin.orientation.y = 0.0;
    occupancy_grid.info.origin.orientation.z = 0.0;
    occupancy_grid.info.origin.orientation.w = 1.0;
    occupancy_grid.data = data;

    pub.publish(occupancy_grid);
}
