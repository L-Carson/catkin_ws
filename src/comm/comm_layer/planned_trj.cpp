
#include "comm_layer/planned_trj.h"
#include "comm_geometry/comm_geometry.h"
#include "comm_math/comm_math.h"
#include "base_layer/log_manager/log_manager.h"

#include <cstdio>

void ConvertPlannedTrjByMapToVehicle(const Pose2D &vehicle_pose_by_map, const PlannedTrj &trj_by_map, PlannedTrj &trj_by_vehicle)
{
    const double sin_vehicle_angle = std::sin(vehicle_pose_by_map.theta);
    const double cos_vehicle_angle = std::cos(vehicle_pose_by_map.theta);

    auto convert_pose_func = [&](double x_by_map, double y_by_map, double theta_by_map)
                             {
                                 const double dx = x_by_map - vehicle_pose_by_map.x;
                                 const double dy = y_by_map - vehicle_pose_by_map.y;

                                 Pose2D pose_by_vehicle;
                                 pose_by_vehicle.x     = cos_vehicle_angle * dx + sin_vehicle_angle * dy;
                                 pose_by_vehicle.y     = cos_vehicle_angle * dy - sin_vehicle_angle * dx;
                                 pose_by_vehicle.theta = WrapAngleRad(theta_by_map - vehicle_pose_by_map.theta);

                                 return pose_by_vehicle;
                             };

    auto convert_point_func = [&](const PlannedTrjPoint &point_by_map)
                              {
                                  Pose2D point_pose_by_vehicle = convert_pose_func(point_by_map.pos.x, point_by_map.pos.y, point_by_map.tangent_dir);

                                  PlannedTrjPoint point_by_vehicle(point_by_map);
                                  point_by_vehicle.pos.x       = point_pose_by_vehicle.x;
                                  point_by_vehicle.pos.y       = point_pose_by_vehicle.y;
                                  point_by_vehicle.tangent_dir = point_pose_by_vehicle.theta;

                                  return point_by_vehicle;
                              };

    const Pose2D stop_ref_pose_by_vehicle = convert_pose_func(trj_by_map.stop_ref_pos.x, trj_by_map.stop_ref_pos.y, 0.0);

    trj_by_vehicle.is_forward         = trj_by_map.is_forward;
    trj_by_vehicle.is_backwheel_track = trj_by_map.is_backwheel_track;
    trj_by_vehicle.plan_time          = trj_by_map.plan_time;
    trj_by_vehicle.is_stop_at_src_end = trj_by_map.is_stop_at_src_end;
    trj_by_vehicle.stop_ref_pos       = {stop_ref_pose_by_vehicle.x, stop_ref_pose_by_vehicle.y};
    trj_by_vehicle.stop_point_idx     = trj_by_map.stop_point_idx;

    trj_by_vehicle.points.resize(trj_by_map.points.size());
    std::transform(trj_by_map.points.begin(), trj_by_map.points.end(), trj_by_vehicle.points.begin(), convert_point_func);

    trj_by_vehicle.equ_time_points    = trj_by_map.equ_time_points;

}

bool PlannedTrjAnalyzer::IsValid(void) const
{
    if (planned_trj_.points.empty())
    {
        return false;
    }

    if (planned_trj_.equ_time_points.empty())
    {
        LOG_FATAL(gp_motion_log, "规划轨迹异常，等时距点列表为空");
        return false;
    }

    const PlannedTrjPoint    &end_equ_dist_point = planned_trj_.points.back();
    const PlanedTrjTimePoint &end_equ_time_point = planned_trj_.equ_time_points.back();
    const double              lon_station_error  = std::abs(end_equ_dist_point.lon_station - end_equ_time_point.lon_station);
    if (lon_station_error > (0.3 * g_path_point_dist))
    {
        LOG_FATAL(gp_motion_log, "规划轨迹异常，末端等时点和等距点里程[%.3lf]相差太大，等时距列表长度为[%lu], 等距列表长度为[%lu]", lon_station_error, planned_trj_.equ_time_points.size(), planned_trj_.points.size());
        return false;
    }

    if (planned_trj_.stop_point_idx >= planned_trj_.points.size())
    {
        LOG_FATAL(gp_motion_log, "规划轨迹异常，估计停止点[%lu]异常，轨迹长度为[%lu]", planned_trj_.stop_point_idx, planned_trj_.points.size());
        return false;
    }

    /* 这里判定异常的门限值，与上游计算投影点的容忍范围有关。 */
    const PlannedTrjPoint &begin_trj_point    = planned_trj_.points.front();
    const PlannedTrjPoint &stop_trj_point     = planned_trj_.points[planned_trj_.stop_point_idx];
    const double           max_stop_point_err = std::max(3.5 * begin_trj_point.lon_station, 3.5 * g_path_point_dist);
    if (CalcDisSquare(stop_trj_point.pos, planned_trj_.stop_ref_pos) > (max_stop_point_err * max_stop_point_err))
    {
        LOG_FATAL(gp_motion_log, "规划轨迹异常，目标停车位置(%.3lf, %.3lf)与对应轨迹点(%u, %.3lf, %.3lf, %.3lf)误差太大", planned_trj_.stop_ref_pos.x, planned_trj_.stop_ref_pos.y, planned_trj_.stop_point_idx, stop_trj_point.pos.x, stop_trj_point.pos.y, stop_trj_point.tangent_dir);
        return false;
    }

    return true;
}

bool PlannedTrjAnalyzer::IsGoForward(void) const
{
    return planned_trj_.is_forward;
}

bool PlannedTrjAnalyzer::IsBackWheelTrack(void) const
{
    return planned_trj_.is_backwheel_track;
}

bool PlannedTrjAnalyzer::IsStopPointAtSrcEnd(void) const
{
    return planned_trj_.is_stop_at_src_end || (planned_trj_.stop_point_idx >= (planned_trj_.points.size() - 1));
}

bool PlannedTrjAnalyzer::IsStopped(const PlannedTrjPoint &matched_point, const Point2D &pos) const
{
    const PlannedTrjPoint &stop_point   = planned_trj_.points[planned_trj_.stop_point_idx];
    const bool             is_near_stop = matched_point.lon_station > (stop_point.lon_station - g_path_point_dist * 3.0);
    if (!is_near_stop) {
        return false;
    }

    return Ray2D(planned_trj_.stop_ref_pos, stop_point.tangent_dir).IsPointOnFront(pos);
}

double PlannedTrjAnalyzer::GetPlanTime(void) const
{
    return planned_trj_.plan_time;
}

const PlannedTrjPoint &PlannedTrjAnalyzer::GetEndTrjPoint(void) const
{
    return planned_trj_.points.back();
}

PlannedTrjPoint PlannedTrjAnalyzer::GetNearestTrjPointByPos(const Point2D &pos) const
{
    // 取距离最近点做控制参考点
    const PlannedTrjPoint *p_nearest_point  = &planned_trj_.points.front();
    double                 nearest_dist_squ = CalcDisSquare(p_nearest_point->pos, pos);

    std::for_each(planned_trj_.points.begin(),
                  planned_trj_.points.end(),
                  [&](const PlannedTrjPoint &trj_point)
                  {
                      const double dist_squ = CalcDisSquare(trj_point.pos, pos);
                      if (dist_squ < nearest_dist_squ)
                      {
                          p_nearest_point  = &trj_point;
                          nearest_dist_squ = dist_squ;
                      }
                  });

    return *p_nearest_point;
}

PlannedTrjPoint PlannedTrjAnalyzer::GetNearestTrjPointByTime(double time) const
{
    const double tgt_time_offset            = time - planned_trj_.plan_time;
    const double tgt_time_offset_adj        = std::max(tgt_time_offset, 0.0);
    const size_t nearest_time_point_idx     = static_cast<size_t>(tgt_time_offset_adj * g_control_cycle_reciprocal + 0.5);
    const size_t nearest_time_point_idx_adj = std::min(nearest_time_point_idx, planned_trj_.equ_time_points.size() - 1);

    const PlanedTrjTimePoint &equ_time_point_of_nearest = planned_trj_.equ_time_points[nearest_time_point_idx_adj];
    const PlannedTrjPoint    &equ_dist_point_of_match   = GetEquDistPointByStation(equ_time_point_of_nearest.lon_station);

    PlannedTrjPoint new_point_of_match;
    new_point_of_match.pos         = equ_dist_point_of_match.pos;
    new_point_of_match.lon_station = equ_time_point_of_nearest.lon_station;
    new_point_of_match.tangent_dir = equ_dist_point_of_match.tangent_dir;
    new_point_of_match.curvature   = equ_dist_point_of_match.curvature;
    new_point_of_match.time_offset = equ_time_point_of_nearest.time_offset;
    new_point_of_match.vel         = equ_time_point_of_nearest.vel;
    new_point_of_match.acc         = equ_time_point_of_nearest.acc;

    return new_point_of_match;
}

const PlannedTrjPoint *PlannedTrjAnalyzer::GetPreviewTrjPointByDist(const PlannedTrjPoint &matched_point, const Point2D &ref_pos, double preview_dist) const
{
    const size_t matched_point_idx = GetEquDistPointIdxByStation(matched_point.lon_station);
    const double preview_dist_squ  = preview_dist * preview_dist;

    // 遍历路径中的后续路径点，搜索在前视距离之外的最近路径点
    for (size_t loop = matched_point_idx; loop < planned_trj_.points.size(); ++loop)
    {
        const PlannedTrjPoint &trj_point = planned_trj_.points[loop];
        if (CalcDisSquare(ref_pos, trj_point.pos) >= preview_dist_squ)
        {
            return &trj_point;
        }
    }

    return nullptr;
}

size_t PlannedTrjAnalyzer::GetEquDistPointIdxByStation(double lon_station) const
{
    const PlannedTrjPoint &first_equ_dist_point  = planned_trj_.points.front();
    return CalcPathSizeByLen(lon_station - first_equ_dist_point.lon_station) - 1;
}

const PlannedTrjPoint &PlannedTrjAnalyzer::GetEquDistPointByStation(double lon_station) const
{
    const size_t equ_dist_idx_of_match = GetEquDistPointIdxByStation(lon_station);
    return planned_trj_.points[equ_dist_idx_of_match];
}

