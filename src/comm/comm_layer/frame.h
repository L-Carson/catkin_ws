/*
Author      : liuqunyang
Update Date : 2025/07/30
*/
#ifndef FRAME_H
#define FRAME_H

#include <list>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "comm_layer/local_view.h"
#include "comm_layer/comm_geometry.h"
#include "comm_layer/open_space_info.h"
#include "comm_layer/comm_base/comm_type.h"

// 消息帧
class Frame
{
public:
    explicit Frame();

    Frame(const LocalView &local_view,
          const std::string &planning_start_point,
          const std::string &vehicle_state,
          std::list<std::string> &reference_lines);

    Frame(const LocalView &local_view,
          const std::string &planning_start_point,
          const std::string &vehicle_state);

    virtual ~Frame() = default;

    // 获取规划路径的起始点
    const std::string &PlanningStartPoint() const;

    const std::string &Frame::vehicle_state() const;

    int32_t Init(const std::string *vehicle_state_provider, const std::list<std::string> &reference_lines);

    const std::list<std::string> &ReferenceLines() const;

    std::list<std::string> *MutableReferenceLines();

    const std::string &GetCurrentReferenceLine() const;

    const std::string *CreateStaticObstacle();

    const LocalView &local_view() const
    {
        return local_view_;
    }

    const Pose2D &EgoPose() const
    {
        return ego_pose_;
    }

    std::string *MutablePlanningPath();

    const OpenSpaceInfo &GetOpenSpaceInfo() const
    {
        return open_space_info_;
    }

    OpenSpaceInfo *MutableOpenSpaceEndPoint()
    {
        return &open_space_info_;
    }

    const OpenSpaceGoalTypeEnum &GetOpenSpaceGoalType() const
    {
        return open_space_goal_type_;
    }

    const std::vector<Point2D> &GetThrowRubbishAreaVertexs() const
    {
        return throw_rubbish_area_vertexs_;
    }

private:
    int32_t InitFrameData();

    bool CreateReferenceLineInfo(const std::list<std::string> &reference_lines);

    /**
   * @brief create a static virtual obstacle
   */
    const std::string *CreateStaticVirtualObstacle(const std::string &id);

    void AddObstacle(const std::string &obstacle);

private:
    LocalView local_view_;                            // 所有规划需要用到的输入数据
    std::string global_map_;                          // 全局地图
    std::string local_map_;                           // 局部地图
    std::string local_obstacle_map_;                  // 局部障碍地图
    std::list<std::string> reference_lines_;          // 原始多条参考线
    std::string current_reference_line_;              // 当前参考线
    std::string vehicle_state_;                       // 当前车辆状态
    std::string planning_start_point_;                // 规划起始点
    std::string planning_trajectory_;                 // 规划的轨迹结果
    OpenSpaceInfo open_space_info_;                   // 开放空间信息
    Pose2D ego_pose_;                                 // 车体本身位姿
    OpenSpaceGoalTypeEnum open_space_goal_type_;      // 来自与路网信息，后续可以整合到整个路网信息类中
    std::vector<Point2D> throw_rubbish_area_vertexs_; // 扔垃圾区域顶点
};

#endif
