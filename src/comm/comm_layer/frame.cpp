/*
Author      : liuqunyang
Update Date : 2025/07/30
*/
#include "comm_layer/frame.h"

#include <algorithm>
#include <limits>

Frame::Frame(void)
{
}

Frame::Frame(const LocalView &local_view,
             const std::string &planning_start_point,
             const std::string &vehicle_state,
             std::list<std::string> &reference_lines)
    : local_view_(local_view),
      planning_start_point_(planning_start_point),
      vehicle_state_(vehicle_state),
      reference_lines_(reference_lines)
{
}

Frame::Frame(const LocalView &local_view,
             const std::string &planning_start_point,
             const std::string &vehicle_state)
    : Frame(local_view, planning_start_point, vehicle_state, std::list<std::string>())
{
}

const std::string &Frame::PlanningStartPoint() const
{
    return planning_start_point_;
}

const std::string &Frame::vehicle_state() const
{
    return vehicle_state_;
}

int32_t Frame::Init(const std::string *vehicle_state_provider, const std::list<std::string> &reference_lines)
{
    return 0;
}

const std::list<std::string> &Frame::ReferenceLines() const
{
    return reference_lines_;
}

std::list<std::string> *Frame::MutableReferenceLines()
{
    return &reference_lines_;
}

const std::string &Frame::GetCurrentReferenceLine() const
{
    return current_reference_line_;
}

const std::string *Frame::CreateStaticObstacle()
{
    std::string obstacle;
    return &obstacle;
}

std::string *MutablePlanningPath()
{
    return &planning_path_;
}

int32_t Frame::InitFrameData()
{
    return 0;
}

bool Frame::CreateReferenceLineInfo(const std::list<std::string> &reference_lines)
{
    return true;
}

const std::string *Frame::CreateStaticVirtualObstacle(const std::string &id)
{
    std::string obstacle;
    return &obstacle;
}

void Frame::AddObstacle(const std::string &obstacle)
{
    // 添加障碍物
}
