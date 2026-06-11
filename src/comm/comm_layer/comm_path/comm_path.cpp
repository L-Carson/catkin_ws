/*
Author      : gongzhihao
Update Date : 2025/09/30
*/
#include "comm_layer/comm_path/comm_path.h"

// 规则化路径
void NormalizePath(std::vector<Pose2D> &path)
{
    // 路径点数
    int path_size = static_cast<int>(path.size());
    if (path_size <= 1)
    {
        return;
    }

    std::vector<Pose2D> normalized_path;
    normalized_path.reserve(2 * path_size);
    normalized_path.push_back(path[0]);

    for (int i = 1; i < path_size;)
    {
        Pose2D &first_pose = normalized_path.back();
        Pose2D &second_pose = path[i];

        // 计算距离
        float dis = CalcDis(first_pose, second_pose);

        // 计算两点连线方向角
        float angle = CalcBearingAngle(first_pose, second_pose);

        second_pose.theta = angle;

        // 是否近似0
        if (IsNearZero(dis - g_waypoint_interval))
        {
            first_pose.theta = angle;
            normalized_path.push_back(second_pose);
            i++;
            continue;
        }

        if (dis < g_waypoint_interval)
        {
            if (i == path_size - 1 && !IsNearZero(dis))
            {
                first_pose.theta = angle;
                normalized_path.push_back(second_pose);
            }
            i++;
        }
        else
        {
            Pose2D new_pose = {first_pose.x + g_waypoint_interval * std::cos(angle),
                               first_pose.y + g_waypoint_interval * std::sin(angle),
                               angle};

            first_pose.theta = angle;
            normalized_path.push_back(new_pose);
        }
    }

    path = std::move(normalized_path);
}

// 规则化路径角度
void NormalizePathAngle(std::vector<Pose2D> &path)
{
    // 路径点数
    int path_size = static_cast<int>(path.size());
    if (path_size <= 1)
    {
        return;
    }

    for (int i = 1; i < path_size; ++i)
    {
        // 计算两点连线方向角
        float angle = CalcBearingAngle(path[i - 1], path[i]);

        path[i - 1].theta = angle;
        path[i].theta = angle;
    }
}

// 搜索最近位姿
bool SearchNearestPose(const std::vector<Pose2D> &path,
                       const Pose2D &pose,
                       ProjectionInfo &projection_info)
{
    // 路径点数
    int path_size = static_cast<int>(path.size());
    if (path_size <= 0)
    {
        return false;
    }

    int begin_index;
    int end_index;

    if (projection_info.index < 0)
    {
        begin_index = 0;
        end_index = path_size - 1;
    }
    else
    {
        begin_index = projection_info.index - 50;
        end_index = projection_info.index + 50;
    }

    // 搜索最近位姿
    return SearchNearestPose(path, pose, begin_index, end_index, projection_info);
}

// 搜索最近位姿
bool SearchNearestPose(const std::vector<Pose2D> &path,
                       const Pose2D &pose,
                       int begin_index,
                       int end_index,
                       ProjectionInfo &projection_info)
{
    // 路径点数
    int path_size = static_cast<int>(path.size());
    if (path_size <= 0)
    {
        return false;
    }

    begin_index = std::max(begin_index, 0);
    end_index = std::min(end_index, path_size - 1);

    if (end_index - begin_index < 0)
    {
        return false;
    }

    float min_dis = g_max_float;

    for (int i = begin_index; i <= end_index; ++i)
    {
        // 计算距离的平方
        float dis_square = CalcDisSquare(pose, path[i]);

        if (dis_square < min_dis)
        {
            min_dis = dis_square;
            projection_info.index = i;
        }
    }

    projection_info.dis = std::sqrt(min_dis);
    return true;
}

// 生成直线路径
void GenLinePath(const Pose2D &start_pose, const Pose2D &end_pose, std::vector<Pose2D> &line_path)
{
    // 计算距离
    float dis = CalcDis(start_pose, end_pose);

    // 计算两点连线方向角
    float angle = CalcBearingAngle(start_pose, end_pose);

    int num_steps = dis / g_waypoint_interval;
    float dx = g_waypoint_interval * std::cos(angle);
    float dy = g_waypoint_interval * std::sin(angle);

    // 插入起点
    line_path.resize(num_steps + 2);
    line_path[0] = start_pose;
    line_path[0].theta = angle;

    for (int i = 1; i <= num_steps; ++i)
    {
        line_path[i].x = line_path[i - 1].x + dx;
        line_path[i].y = line_path[i - 1].y + dy;
        line_path[i].theta = angle;
    }

    // 插入终点
    line_path[num_steps + 1] = end_pose;
    line_path[num_steps + 1].theta = angle;
}
