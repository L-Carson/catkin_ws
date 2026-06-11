/*
Author      : liuqunyang
Update Date : 2025/07/30
*/
#ifndef OPEN_SCPE_INFO_H
#define OPEN_SCPE_INFO_H

#include <list>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "comm_layer/comm_geometry.h"
#include "comm_layer/comm_base/comm_type.h"

class OpenSpaceInfo
{
public:
    explicit OpenSpaceInfo();
    virtual ~OpenSpaceInfo() = default;

    const Point2D &GetStartPoint() const
    {
        return start_point_;
    }

    const Point2D &GetEndPoint() const
    {
        return end_point_;
    }

    const std::list<Point2D> &GetPathPoints() const
    {
        return path_points_;
    }

    std::list<Point2D> *MutablePathPoints()
    {
        return &path_points_;
    }

    const std::list<Point2D> &GetChosedPathPoints() const
    {
        return chosed_path_points_;
    }

    std::list<Point2D> *MutablePathPoints()
    {
        return &chosed_path_points_;
    }

private:
    Point2D start_point_;                   // 开放空间起始点
    Point2D end_point_;                     // 开放空间结束点
    bool is_plan_success_ = false;          // 是否规划成功
    std::list<Point2D> path_points_;        // 规划的路径
    std::list<Point2D> chosed_path_points_; // 本次执行的路径
};

#endif
