#ifndef __PLANNED_TRJ_H__
#define __PLANNED_TRJ_H__

#include "comm_layer/comm_base/comm_type.h"

#include <vector>


// 路径点间距
const double g_path_point_dist            = 0.10;
const double g_path_point_dist_squ        = g_path_point_dist * g_path_point_dist;
const double g_path_point_dist_reciprocal = 1.0 / g_path_point_dist;

/* 控制周期，单位s */
const double g_control_cycle            = 0.05;
const double g_control_cycle_reciprocal = 1.0 / g_control_cycle;

// 根据路径点数计算路径长度
inline double CalcPathLenBySize(size_t path_size)
{
    return ((path_size <= 1) ? 0.0 : ((path_size - 1) * g_path_point_dist));
}

// 根据路径长度计算路径点数，长度为0时仍保留一个路径点
inline size_t CalcPathSizeByLen(double path_len)
{
    return (static_cast<size_t>(std::max(path_len, 0.0) * g_path_point_dist_reciprocal + 0.5) + 1);
}

struct PlannedPathPoint
{
    Point2D pos;
    double  max_vel_abs;    /* 速度小于等于0时表示需要在该点停车 */
};

// 规划路径，除最后一个点，其它点必须等间距，且最后一个点的间距只能小于正常间距
// 为了保证路径不会过短，可以在前面多一些点
// 最后一个点作为目标终点，不能在目标后多出其它点
struct PlannedPath
{
    bool                          is_forward;           /* 是否前向行驶，横向控制算法支持前向还是后向行驶需要确认 */
    bool                          is_backwheel_track;   /* 是否后轮跟踪，横向控制算法支持前轮还是后轮跟踪需要确认 */
    double                        slow_down_dec_abs;    /* 减速度，至少以该加速度减速到指定速度 */
    double                        slow_down_vel_abs;    /* 减速目标速度，至少减速到该速度 */
    std::vector<PlannedPathPoint> points;               /* 规划路径点 */
};

struct PlannedTrjPoint
{
    Point2D pos;                /* 平面位置 */
    double  lon_station;        /* 纵向位置 */
    double  tangent_dir;        /* 切线方向角 */
    double  curvature;          /* 曲率 */

    double  time_offset;        /* 时间偏移 */
    double  vel;                /* 速度 */
    double  acc;                /* 加速度 */
};

struct PlanedTrjTimePoint
{
    double  lon_station;        /* 纵向位置 */
    double  time_offset;        /* 时间偏移 */
    double  vel;                /* 速度 */
    double  acc;                /* 加速度 */
};

// 规划轨迹，车体坐标系
struct PlannedTrj
{
    bool                            is_forward;                /* 是否前进 */
    bool                            is_backwheel_track;        /* 是否使用后轮跟踪轨迹 */
    double                          plan_time;                 /* 规划时间 */
    bool                            is_stop_at_src_end;        /* 停车点是否是原终点 */
    Point2D                         stop_ref_pos;              /* 停车参考位置 */
    size_t                          stop_point_idx;            /* 用于指示停车轨迹点 */
    std::vector<PlannedTrjPoint>    points;                    /* 等间距轨迹点，间距为 g_path_point_dist */
    std::vector<PlanedTrjTimePoint> equ_time_points;           /* 等时距轨迹点，时距为 g_control_cycle */
};

// 将规划轨迹从地图坐标系转换到车辆坐标系
void ConvertPlannedTrjByMapToVehicle(const Pose2D &vehicle_pose_by_map, const PlannedTrj &trj_by_map, PlannedTrj &trj_by_vehicle);

class PlannedTrjAnalyzer
{
public:
    PlannedTrjAnalyzer(const PlannedTrj &planned_trj) : planned_trj_(planned_trj)
    {
    }

    bool   IsValid(void) const;

    /* 这些接口调用时必须确保轨迹合法 */
    bool   IsGoForward(void) const;
    bool   IsBackWheelTrack(void) const;
    bool   IsStopPointAtSrcEnd(void) const;
    bool   IsStopped(const PlannedTrjPoint &matched_point, const Point2D &pos) const;
    double GetPlanTime(void) const;

    const PlannedTrjPoint &GetEndTrjPoint(void) const;
    PlannedTrjPoint        GetNearestTrjPointByPos(const Point2D &pos) const;
    PlannedTrjPoint        GetNearestTrjPointByTime(double time) const;
    const PlannedTrjPoint *GetPreviewTrjPointByDist(const PlannedTrjPoint &matched_point, const Point2D &ref_pos, double preview_dist) const;

private:
    size_t                 GetEquDistPointIdxByStation(double lon_station) const;
    const PlannedTrjPoint &GetEquDistPointByStation(double lon_station) const;

private:
    const PlannedTrj &planned_trj_;
};


#endif

