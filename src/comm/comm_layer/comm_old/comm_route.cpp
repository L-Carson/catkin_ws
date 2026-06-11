
#include "comm_route.h"
#include "comm_dbg_log.h"
#include "comm_geometry.h"

#include <algorithm>
#include <list>
#include <vector>
#include <string>

bool Route::IsValid(void) const
{
    return (GetRouteSize() >= 2);
}

bool Route::IsPointIdxValid(unsigned int point_idx) const
{
    return (point_idx < GetRouteSize());
}

bool Route::IsPointIdxAtEnd(unsigned int point_idx) const
{
    return ((point_idx + 1) >= GetRouteSize());
}

bool Route::IsPointOnRoutePointLeft(const Point2D &point, unsigned int route_point_idx)
{
    R_ASSERT_2(route_point_idx < GetRouteSize(), false, route_point_idx, GetRouteSize());
    RoutePointC &route_point = GetPointWithDir(route_point_idx);

    DirectedLine tangent_line(route_point.pose, std::sin(route_point.heading), std::cos(route_point.heading));
    return tangent_line.IsPointOnLeft(point);
}

/**************************************************************************************
功能描述: 设置路径
修改记录:
**************************************************************************************/
void Route::SetRoute(const std::vector<Point2D> &vect_route, int trj_no = 0, unsigned int seg_no = 0)
{
    Clear();

    AddPoints(vect_route, trj_no, seg_no);
}

/**************************************************************************************
功能描述: 将模拟的小车运行的位姿记录到路径对应投影点
修改记录:
**************************************************************************************/
void Route::SetCarPose(unsigned int proj_idx, Pose2D &car_pose, unsigned int origin_route_proj_idx)
{
    R_ASSERT_2(proj_idx < GetRouteSize(), NOTHING, proj_idx, GetRouteSize());

    /* 如果在该点之前的一些点还未设置，则设置为默认位姿。 */
    if (proj_idx > set_car_pose_point_num_)
    {
        const Pose2D *default_pose_ptr;
        unsigned int default_origin_route_proj_idx;
        if (set_car_pose_point_num_ == 0)
        {
            default_pose_ptr = &car_pose;
            default_origin_route_proj_idx = origin_route_proj_idx;
        }
        else
        {
            const RoutePointC &last_set_pose_point = GetPointConst(set_car_pose_point_num_ - 1);
            default_pose_ptr = &last_set_pose_point.car_pose;
            default_origin_route_proj_idx = last_set_pose_point.proj_idx_by_origin_route;
        }

        for (unsigned int loop = set_car_pose_point_num_; loop < proj_idx; ++loop)
        {
            RoutePointC &point = GetPoint(loop);
            point.car_pose = *default_pose_ptr;
            point.proj_idx_by_origin_route = default_origin_route_proj_idx;
        }
    }

    RoutePointC &cur_set_point = GetPoint(proj_idx);
    cur_set_point.car_pose = car_pose;
    cur_set_point.proj_idx_by_origin_route = origin_route_proj_idx;

    set_car_pose_point_num_ = proj_idx + 1;
}

int Route::SetTrjNo(unsigned int point_idx, int trj_no)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(point_idx < route_size, -1, point_idx, route_size);

    SetTrjNoWithNoProtect(point_idx, trj_no);
    return 0;
}

int Route::GetPointPos(unsigned int point_idx, Point2D &point) const
{
    R_ASSERT_2(point_idx < GetRouteSize(), -1, point_idx, GetRouteSize());

    const RoutePointC &cur_point = GetPointConst(point_idx);
    point = cur_point.point;

    return 0;
}

int Route::GetPointTrj(unsigned int point_idx, int &trj_no) const
{
    R_ASSERT_2(point_idx < GetRouteSize(), -1, point_idx, GetRouteSize());

    const RoutePointC &cur_point = GetPointConst(point_idx);
    trj_no = cur_point.traj_no;

    return 0;
}

int Route::GetPointSegNo(unsigned int point_idx, unsigned int &seg_no) const
{
    R_ASSERT_2(point_idx < GetRouteSize(), -1, point_idx, GetRouteSize());

    const RoutePointC &cur_point = GetPointConst(point_idx);
    seg_no = cur_point.route_seg_no;

    return 0;
}

int Route::GetPointOriginRouteProjectIdx(unsigned int point_idx, unsigned int &origin_route_proj_idx) const
{
    R_ASSERT_3(point_idx < set_car_pose_point_num_, -1, point_idx, set_car_pose_point_num_, GetRouteSize());

    const RoutePointC &cur_point = GetPointConst(point_idx);
    origin_route_proj_idx = cur_point.proj_idx_by_origin_route;

    return 0;
}

int Route::GetPointPosAndOriginRouteProjectIdx(unsigned int point_idx, Point2D &point,
                                               unsigned int &origin_route_proj_idx) const
{
    R_ASSERT_3(point_idx < set_car_pose_point_num_, -1, point_idx, set_car_pose_point_num_, GetRouteSize());

    const RoutePointC &cur_point = GetPointConst(point_idx);
    point = cur_point.point;
    origin_route_proj_idx = cur_point.proj_idx_by_origin_route;

    return 0;
}

int Route::GetPointPose(unsigned int point_idx, Pose2D &pose)
{
    R_ASSERT_2(point_idx < GetRouteSize(), -1, point_idx, GetRouteSize());

    RoutePointC &cur_point = GetPointWithDir(point_idx);
    pose = cur_point.pose;

    return 0;
}

int Route::GetPointPoseAndTrjNo(unsigned int point_idx, Pose2D &pose, int &trj_no)
{
    R_ASSERT_2(point_idx < GetRouteSize(), -1, point_idx, GetRouteSize());

    RoutePointC &cur_point = GetPointWithDir(point_idx);
    pose = cur_point.pose;
    trj_no = cur_point.traj_no;

    return 0;
}

int Route::GetPointDirAngle(unsigned int point_idx, float &angle)
{
    R_ASSERT_2(point_idx < GetRouteSize(), -1, point_idx, GetRouteSize());

    RoutePointC &cur_point = GetPointWithDir(point_idx);
    angle = cur_point.heading;

    return 0;
}

int Route::GetPointIdxByOriginRouteProjectIdx(unsigned int origin_route_proj_idx) const
{
    bool is_exist_bigger_proj_idx = false;

    for (int loop = int(set_car_pose_point_num_ - 1); loop >= 0; --loop)
    {
        const RoutePointC &cur_point = GetPointConst((unsigned int)(loop));
        if (cur_point.proj_idx_by_origin_route > origin_route_proj_idx)
        {
            is_exist_bigger_proj_idx = true;
            continue;
        }

        if (cur_point.proj_idx_by_origin_route == origin_route_proj_idx)
        {
            return loop;
        }

        if (is_exist_bigger_proj_idx)
        {
            return loop;
        }

        break;
    }

    return -1;
}

int Route::GetEndPointPos(Point2D &point)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT(route_size > 0, -1);

    const RoutePointC &cur_point = GetPointConst(route_size - 1);
    point.x = cur_point.pose.x;
    point.y = cur_point.pose.y;

    return 0;
}

int Route::GetEndPointPose(Pose2D &pose)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT(route_size > 0, -1);

    RoutePointC &cur_point = GetPointWithDir(route_size - 1);
    pose = cur_point.pose;

    return 0;
}

int Route::GetEndPointPoseAndTrjNo(Pose2D &pose, int &trj_no)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT(route_size > 0, -1);

    RoutePointC &cur_point = GetPointWithDir(route_size - 1);
    pose = cur_point.pose;
    trj_no = cur_point.traj_no;

    return 0;
}

int Route::GetEndPointIdx(void) const
{
    int route_size = int(GetRouteSize());
    R_ASSERT_1(route_size > 0, -1, route_size);

    return route_size - 1;
}

unsigned int Route::GetCarPose(Pose2D &car_pose, unsigned int last_point_idx) const
{
    R_ASSERT_2(set_car_pose_point_num_ <= GetRouteSize(),
               std::numeric_limits<unsigned int>::max(),
               set_car_pose_point_num_, GetRouteSize());

    if (set_car_pose_point_num_ == 0)
    {
        return std::numeric_limits<unsigned int>::max();
    }

    unsigned int last_point_idx_e = std::min(last_point_idx, set_car_pose_point_num_ - 1);
    const RoutePointC &last_set_pose_point = GetPointConst(last_point_idx_e);
    car_pose = last_set_pose_point.car_pose;

    return last_point_idx_e;
}

int Route::GetAimPos(unsigned int proj_idx, float aim_dist, Point2D &aim_point)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(IsPointIdxValid(proj_idx), -1, proj_idx, route_size);

    unsigned int aim_route_point_idx = proj_idx + static_cast<unsigned int>(aim_dist / g_waypoint_interval);
    if (aim_route_point_idx < route_size)
    {
        const RoutePointC &aim_route_point = GetPointConst(aim_route_point_idx);
        aim_point.x = aim_route_point.pose.x;
        aim_point.y = aim_route_point.pose.y;
    }
    else
    {
        RoutePointC &end_point = GetPointWithDir(route_size - 1);

        float extend_dist = float(aim_route_point_idx - route_size + 1) * g_waypoint_interval;
        aim_point.x = end_point.pose.x + std::cos(end_point.heading) * extend_dist;
        aim_point.y = end_point.pose.y + std::sin(end_point.heading) * extend_dist;
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取给定位置和路线的相对位姿信息
          如果不给定搜索范围，则从头找到尾
修改记录:
**************************************************************************************/
int Route::GetRelativeInfo(Pose2D &pose, PoseRouteRelativeInfo &relative_info, unsigned int search_begin_idx,
                           float max_search_len, bool is_must_match_dir)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(IsPointIdxValid(search_begin_idx), -1, route_size, search_begin_idx);

    unsigned int max_point_idx = route_size - 1;
    unsigned int search_end_idx;
    if (max_search_len <= 0.0)
    {
        search_end_idx = max_point_idx;
    }
    else
    {
        search_end_idx = search_begin_idx + static_cast<unsigned int>(max_search_len / g_waypoint_interval);
        search_end_idx = std::min(search_end_idx, max_point_idx);
    }

    float min_dist_squ = 1.0e10;
    unsigned int min_dist_idx = search_begin_idx;
    bool is_match_dir = false;
    for (unsigned int loop = search_begin_idx; loop <= search_end_idx; ++loop)
    {
        RoutePointC &route_point = GetPointWithDir(loop);

        float dist_squ = CalcDisSquare(route_point.pose, pose);
        float diff_theta = NormalizeAngle(route_point.pose.heading - pose.heading);

        /* 如果存在方向一致的点，采用方向一致且路径最近点；如果不存在则采用距离最近点 */
        bool is_cur_match_dir = (diff_theta < (0.5 * M_PI));
        if (is_match_dir && !is_cur_match_dir)
        {
            continue;
        }
        else if (!is_match_dir && is_cur_match_dir)
        {
            min_dist_squ = dist_squ;
            min_dist_idx = loop;
            is_match_dir = true;
            continue;
        }
        else
        {
            if (dist_squ < min_dist_squ)
            {
                min_dist_squ = dist_squ;
                min_dist_idx = loop;
            }
        }
    }

    /* 不存在方向匹配的点，且又要求强制匹配方向，则直接失败，不获取次要的不匹配方向的结果 */
    if (is_must_match_dir && !is_match_dir)
    {
        return -1;
    }

    const Pose2D &pre_route_pose = (min_dist_idx == 0) ? GetPointConst(min_dist_idx).pose
                                                       : GetPointConst(min_dist_idx - 1).pose;
    const Pose2D &next_route_pose = (min_dist_idx == max_point_idx) ? GetPointConst(min_dist_idx).pose
                                                                    : GetPointConst(min_dist_idx + 1).pose;

    Point2D pre_route_point(pre_route_pose.x, pre_route_pose.y);
    Point2D next_route_point(next_route_pose.x, next_route_pose.y);
    Point2D point_in(pose.x, pose.y);
    relative_info.dist_to_route = sqrt(min_dist_squ);
    relative_info.is_point_on_left = DirectedLine(pre_route_point, next_route_point).IsPointOnLeft(point_in);
    relative_info.proj_idx = min_dist_idx;
    return 0;
}

int Route::GetRelativeInfo(const Point2D &point, PoseRouteRelativeInfo &relative_info,
                           unsigned int search_begin_idx, float max_search_len) const
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(IsPointIdxValid(search_begin_idx), -1, route_size, search_begin_idx);

    unsigned int max_point_idx = route_size - 1;
    unsigned int search_end_idx;
    if (max_search_len <= 0.0)
    {
        search_end_idx = max_point_idx;
    }
    else
    {
        search_end_idx = search_begin_idx + static_cast<unsigned int>(max_search_len / g_waypoint_interval);
        search_end_idx = std::min(search_end_idx, max_point_idx);
    }

    float min_dist_squ = 1.0e10;
    unsigned int min_dist_idx = search_begin_idx;
    for (unsigned int loop = search_begin_idx; loop <= search_end_idx; ++loop)
    {
        const RoutePointC &route_point = GetPointConst(loop);
        float dist_squ = CalcDisSquare(route_point.pose.x, route_point.pose.y, point.x, point.y);
        if (dist_squ >= min_dist_squ)
        {
            continue;
        }

        min_dist_squ = dist_squ;
        min_dist_idx = loop;
    }

    const Pose2D &pre_route_pose = (min_dist_idx == 0) ? GetPointConst(min_dist_idx).pose
                                                       : GetPointConst(min_dist_idx - 1).pose;
    const Pose2D &next_rotue_pose = (min_dist_idx == max_point_idx) ? GetPointConst(min_dist_idx).pose
                                                                    : GetPointConst(min_dist_idx + 1).pose;

    Point2D pre_route_point(pre_route_pose.x, pre_route_pose.y);
    Point2D next_route_point(next_rotue_pose.x, next_rotue_pose.y);
    relative_info.dist_to_route = sqrt(min_dist_squ);
    relative_info.is_point_on_left = DirectedLine(pre_route_point, next_route_point).IsPointOnLeft(point);
    relative_info.proj_idx = min_dist_idx;
    return 0;
}

unsigned int Route::GetLocNearestPointIdx(const Point2D &point, unsigned int loc_range_seed_idx)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(loc_range_seed_idx < route_size, g_max_unsigned_int, route_size, loc_range_seed_idx);
    const RoutePointC &seed_point = GetPointConst(loc_range_seed_idx);

    unsigned int min_dist_idx_on_left = loc_range_seed_idx;
    float min_dist_squ_on_left = CalcDisSquare(point.x, point.y, seed_point.pose.x, seed_point.pose.y);
    unsigned int min_dist_idx_on_right = min_dist_idx_on_left;
    float min_dist_squ_on_right = min_dist_squ_on_left;

    for (unsigned int loop = loc_range_seed_idx + 1; loop < route_size; ++loop)
    {
        const RoutePointC &route_point = GetPointConst(loop);
        float dist_squ = CalcDisSquare(point.x, point.y, route_point.pose.x, route_point.pose.y);
        if (dist_squ > min_dist_squ_on_left)
        {
            break;
        }

        min_dist_idx_on_left = loop;
        min_dist_squ_on_left = dist_squ;
    }

    for (int loop = int(loc_range_seed_idx) - 1; loop >= 0; --loop)
    {
        const RoutePointC &route_point = GetPointConst((unsigned int)(loop));
        float dist_squ = CalcDisSquare(point.x, point.y, route_point.pose.x, route_point.pose.y);
        if (dist_squ > min_dist_squ_on_right)
        {
            break;
        }

        min_dist_idx_on_right = (unsigned int)(loop);
        min_dist_squ_on_right = dist_squ;
    }

    return (min_dist_squ_on_right < min_dist_squ_on_left) ? min_dist_idx_on_right : min_dist_idx_on_left;
}

int Route::GetFirstPointIdxOfFrontAndSameDir(Pose2D &pose, unsigned int &point_idx,
                                             unsigned int search_begin_idx, unsigned int search_end_idx)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(search_begin_idx < route_size, -1, route_size, search_begin_idx);
    R_ASSERT_2(search_begin_idx <= search_end_idx, -1, search_begin_idx, search_end_idx);
    unsigned int search_end_idx_ex = std::min(route_size - 1, search_end_idx);

    Ray2D pose_ray(pose);

    point_idx = g_max_unsigned_int;
    for (unsigned int loop = search_begin_idx; loop <= search_end_idx_ex; ++loop)
    {
        RoutePointC &route_point = GetPointWithDir(loop);

        if (!pose_ray.IsPointOnFront(Point2D(route_point.pose.x, route_point.pose.y)))
        {
            continue;
        }

        float diff_theta = WrapAngleRad(pose.heading - route_point.pose.heading);
        if (std::fabs(diff_theta) > (0.5 * M_PI))
        {
            continue;
        }

        point_idx = loop;
        break;
    }

    return 0;
}

float Route::GetLen(void) const
{
    return float(GetRouteSize() * g_waypoint_interval);
}

float Route::GetLenAfter(unsigned int idx) const
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(idx < route_size, 0.0, idx, route_size);

    return (float(route_size - idx - 1) * g_waypoint_interval);
}

float Route::GetRateAfter(unsigned int idx) const
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(idx < route_size, 0.0, idx, route_size);

    return float(route_size - idx - 1) / float(route_size);
}

unsigned int Route::GetSize(void) const
{
    return GetRouteSize();
}

unsigned int Route::GetAvgOfTrjNoSqu(void)
{
    unsigned int src_route_size = GetRouteSize();
    R_ASSERT(src_route_size > 0, g_max_unsigned_int);
    R_ASSERT_2(set_sum_of_front_trj_no_squ_num_ <= src_route_size, g_max_unsigned_int,
               src_route_size, set_sum_of_front_trj_no_squ_num_);

    unsigned int sum_of_front_trj_no_squ = 0;
    if (set_sum_of_front_trj_no_squ_num_ > 0)
    {
        RoutePointC &route_point = GetPoint(set_sum_of_front_trj_no_squ_num_ - 1);
        sum_of_front_trj_no_squ = route_point.sum_of_front_trj_no_squ;
    }

    for (unsigned int loop = set_sum_of_front_trj_no_squ_num_; loop < src_route_size; ++loop)
    {
        RoutePointC &route_point = GetPoint(loop);
        sum_of_front_trj_no_squ += route_point.traj_no * route_point.traj_no;
        route_point.sum_of_front_trj_no_squ = sum_of_front_trj_no_squ;
    }

    return sum_of_front_trj_no_squ / src_route_size;
}

void Route::GetRoute(std::vector<Point2D> &vec_route) const
{
    unsigned int src_route_size = GetRouteSize();
    vec_route.clear();
    vec_route.reserve(src_route_size);

    for (unsigned int loop = 0; loop < src_route_size; ++loop)
    {
        const RoutePointC &route_point = GetPointConst(loop);
        vec_route.push_back(Point2D(route_point.pose.x, route_point.pose.y));
    }
}

/**************************************************************************************
功能描述: 获取子路径
修改记录:
**************************************************************************************/
int Route::GetSubRoute(Route &sub_route, unsigned int begin_idx, unsigned int end_idx) const
{
    unsigned int max_src_idx = GetRouteSize();
    R_ASSERT(max_src_idx > 0, -1);

    unsigned int end_idx_e = std::min(max_src_idx - 1, end_idx);
    R_ASSERT(begin_idx <= end_idx_e, -1);

    sub_route.is_tail_points_exist_dir_ = false;
    sub_route.set_dir_point_num_ = (set_dir_point_num_ > begin_idx) ? (set_dir_point_num_ - begin_idx) : 0;
    sub_route.set_car_pose_point_num_ = 0;
    sub_route.set_sum_of_front_trj_no_squ_num_ = 0;

    sub_route.vec_route_.clear();
    sub_route.vec_route_.reserve(end_idx_e - begin_idx + 1);
    for (unsigned int loop = begin_idx; loop <= end_idx_e; ++loop)
    {
        sub_route.vec_route_.push_back(vec_route_[loop]);
    }

    sub_route.ClrTailPointsDir();

    return 0;
}

/**************************************************************************************
功能描述: 从指定位置截取指定长度的子路径
修改记录:
**************************************************************************************/
int Route::GetSubRoute(Route &sub_route, unsigned int begin_idx, float max_sub_route_len) const
{
    unsigned int max_src_idx = GetRouteSize();
    R_ASSERT((max_src_idx > 0) && (max_sub_route_len >= 0), -1);

    unsigned int end_idx = begin_idx + static_cast<unsigned int>(max_sub_route_len / g_waypoint_interval);

    return GetSubRoute(sub_route, begin_idx, end_idx);
}

/**************************************************************************************
功能描述: 在路径最后延伸一个点
修改记录:
**************************************************************************************/
void Route::ExtendEnd(const Point2D &new_point, int trj_no, bool is_add_closer, unsigned int seg_no)
{
    if (GetRouteSize() == 0)
    {
        AddPoint(new_point, trj_no, seg_no);
        return;
    }

    const RoutePointC &route_end_point = GetPointConst(GetRouteSize() - 1);
    Point2D end_point(route_end_point.pose.x, route_end_point.pose.y);

    /* 如果延伸点和之前最后一个点的距离，已标准距离误差不超过5%则直接添加。 */
    float dist_squ = CalcDisSquare(end_point, new_point);
    float delta_of_dist_squ = dist_squ - g_waypoint_interval * g_waypoint_interval;
    float delta_of_dist = delta_of_dist_squ * 0.5 / g_waypoint_interval;
    if (std::fabs(delta_of_dist) < (g_waypoint_interval * 0.05))
    {
        AddPoint(new_point, trj_no, seg_no);
        return;
    }

    /* 长度达不到最大舍弃值，则不用处理 */
    float extend_factor = is_add_closer ? 0.5 : 0.0;
    float discard_factor = 1.0 - extend_factor; /* 该系数须大于0 */
    float max_dist_squ_of_discard = g_waypoint_interval * g_waypoint_interval * discard_factor * discard_factor;
    if (dist_squ <= max_dist_squ_of_discard)
    { /* 该条件和后面循环的条件是一致的 */
        return;
    }

    /* 后续分支至少插值一次 */
    float to_point_angle = std::atan2(end_point.y - new_point.y, end_point.x - new_point.x);
    Point2D delta_point(float(g_waypoint_interval * std::cos(to_point_angle)),
                        float(g_waypoint_interval * std::sin(to_point_angle)));
    Point2D lmt_point(float(new_point.x + delta_point.x * extend_factor),
                      float(new_point.y + delta_point.y * extend_factor));

    /* 使用较大的分量进行比较，防止该分量delta为0 */
    Point2D add_point{end_point.x + delta_point.x, end_point.y + delta_point.y};

    bool is_compare_x = std::fabs(delta_point.x) > std::fabs(delta_point.y);
    float &add_point_compare = is_compare_x ? add_point.x : add_point.y;
    float &lmt_point_compare = is_compare_x ? lmt_point.x : lmt_point.y;
    bool is_compare_delta_positive = is_compare_x ? (delta_point.x > 0) : (delta_point.y > 0);

    do
    {
        AddPoint(add_point, trj_no, seg_no);

        add_point.x += delta_point.x;
        add_point.y += delta_point.y;

    } while (is_compare_delta_positive == (add_point_compare < lmt_point_compare));
}

void Route::ExtendEnd(const std::vector<Point2D> &vec_new_points, int trj_no, bool is_add_closer, unsigned int seg_no)
{
    unsigned int route_size_old = GetRouteSize();
    unsigned int new_point_size = vec_new_points.size();
    vec_route_.reserve(route_size_old + new_point_size);

    for (unsigned int loop = 0; loop < new_point_size; ++loop)
    {
        bool is_add_closer_e = ((loop + 1) == new_point_size) && is_add_closer;
        ExtendEnd(vec_new_points[loop], trj_no, is_add_closer_e, seg_no);
    }
}

void Route::ExtendEndByPointsOfSmoothTrjNo(const std::vector<Point2D> &vec_new_points,
                                           int trj_no_begin, int trj_no_end)
{
    unsigned int route_size_old = GetRouteSize();

    vec_route_.reserve(route_size_old + vec_new_points.size());
    for (const Point2D &new_point : vec_new_points)
    {
        ExtendEnd(new_point, trj_no_begin, false, 0);
    }

    unsigned int route_size_new = GetRouteSize();
    if (route_size_new <= route_size_old)
    {
        return;
    }

    if (trj_no_end == trj_no_begin)
    {
        return;
    }

    float delta_of_trj_no = float(trj_no_end - trj_no_begin) / float(route_size_new - route_size_old);
    float trj_no_float = float(trj_no_begin);
    for (unsigned int loop = route_size_old + 1; loop < route_size_new; ++loop)
    {
        trj_no_float += delta_of_trj_no;
        int trj_no = std::lround(trj_no_float);

        SetTrjNoWithNoProtect(loop, trj_no);
    }
}

void Route::ExtendEnd(Route &new_route)
{
    unsigned int route_size_old = GetRouteSize();
    unsigned int route_size_add = new_route.GetRouteSize();
    vec_route_.reserve(route_size_old + route_size_add);

    for (unsigned int loop = 0; loop < route_size_add; ++loop)
    {
        const RoutePointC &new_route_point = new_route.GetPointConst(loop);
        ExtendEnd(Point2D(new_route_point.pose.x, new_route_point.pose.y), new_route_point.traj_no);
    }
}

/**************************************************************************************
功能描述: 扩展路径
修改记录:
**************************************************************************************/
void Route::ExtendEnd(float forward_len)
{
    unsigned int route_size_old = GetRouteSize();
    R_ASSERT(route_size_old > 0, NOTHING);
    vec_route_.reserve(route_size_old + static_cast<unsigned int>(forward_len / g_waypoint_interval));

    RoutePointC end_point = GetPointWithDir(route_size_old - 1);
    for (float dist_to_last = g_waypoint_interval; dist_to_last <= forward_len; dist_to_last += g_waypoint_interval)
    {
        Point2D new_end_point;
        new_end_point.x = end_point.pose.x + std::cos(end_point.pose.heading) * dist_to_last;
        new_end_point.y = end_point.pose.y + std::sin(end_point.pose.heading) * dist_to_last;

        ExtendEnd(new_end_point, end_point.traj_no);
    }
}

/**************************************************************************************
功能描述: 延长路径，路径做一点平滑
修改记录:
**************************************************************************************/
void Route::ExtendEndWithSmooth(const Point2D &new_point, float min_turn_radius,
                                int trj_no, bool is_add_closer, unsigned int seg_no)
{
    unsigned int route_size_old = GetRouteSize();
    if (route_size_old <= 1)
    {
        ExtendEnd(new_point, trj_no, is_add_closer, seg_no);
        return;
    }

    const RoutePointC &last1_route_point = GetPointConst(route_size_old - 1);
    const RoutePointC &last2_route_point = GetPointConst(route_size_old - 2);
    Point2D last1_point(last1_route_point.pose.x, last1_route_point.pose.y);
    Point2D last2_point(last2_route_point.pose.x, last2_route_point.pose.y);

    /* 约束新加点构成的圆弧不会与上一段反向 */
    Vec2D vec2d_21(last2_point, last1_point);
    Vec2D vec2d_1n(last1_point, new_point);
    Vec2D vec2d_2n(last2_point, new_point);
    if ((vec2d_1n.DotMult(vec2d_2n) < 0.0) ||
        (vec2d_21.DotMult(vec2d_1n) < (-0.5 * vec2d_21.GetLenSqu())))
    {
        ClearEnd();
        ExtendEndWithSmooth(new_point, min_turn_radius, trj_no, is_add_closer, seg_no);
        return;
    }

    /* 判断新加点与最后两个点构成的圆的半径是否大于最小半径，不大于时删除最后一个点 */
    Circular circular(last2_point, last1_point, new_point);
    if (!circular.IsValid() || (circular.GetRadiusSqu() <= (min_turn_radius * min_turn_radius)))
    {
        ClearEnd();
        ExtendEndWithSmooth(new_point, min_turn_radius, trj_no, is_add_closer, seg_no);
        return;
    }

    /* 当新加段对应的弧度过大时，需要进行圆弧插值 */
    if (vec2d_1n.GetLenSqu() > (0.5 * 0.5))
    {
        std::vector<Point2D> vec_sample_points;
        int rslt_sample = circular.GetSamplePoints(last1_point, new_point, M_PI / 16.0, vec_sample_points);
        R_ASSERT(rslt_sample == 0, NOTHING);
        ExtendEnd(vec_sample_points, trj_no, is_add_closer, seg_no);
    }
    else
    {
        ExtendEnd(new_point, trj_no, is_add_closer, seg_no);
    }
}

void Route::ExtendEndWithSmooth(const std::vector<Point2D> &vec_new_points, float min_turn_radius,
                                int trj_no, bool is_add_closer, unsigned int seg_no)
{
    unsigned int route_size_old = GetRouteSize();
    unsigned int new_point_size = vec_new_points.size();
    vec_route_.reserve(route_size_old + new_point_size);

    for (unsigned int loop = 0; loop < new_point_size; ++loop)
    {
        bool is_add_close_e = ((loop + 1) == new_point_size) && is_add_closer;
        ExtendEndWithSmooth(vec_new_points[loop], min_turn_radius, trj_no, is_add_close_e, seg_no);
    }
}

void Route::ExtendEndWithSmooth(Route &new_route, float min_turn_radius)
{
    unsigned int route_size_old = GetRouteSize();
    unsigned int route_size_add = new_route.GetRouteSize();
    vec_route_.reserve(route_size_old + route_size_add);

    for (unsigned int loop = 0; loop < route_size_add; ++loop)
    {
        const RoutePointC &new_route_point = new_route.GetPointConst(loop);
        ExtendEndWithSmooth(Point2D(new_route_point.pose.x, new_route_point.pose.y),
                            min_turn_radius, new_route_point.traj_no);
    }
}

/**************************************************************************************
功能描述: 生成相距指定距离的平行路径
修改记录:
**************************************************************************************/
int Route::GenParallelRoute(float dist, float min_turn_radius, Route &new_route, unsigned int start_idx)
{
    unsigned int src_route_size = GetRouteSize();
    R_ASSERT_2(start_idx < src_route_size, -1, start_idx, src_route_size);

    new_route.Clear();
    new_route.vec_route_.reserve((unsigned int)(src_route_size * 1.2));

    SetAllPointsDir();

    int delta_of_trj_no = (dist >= 0) ? 1 : -1;

    for (unsigned int loop = start_idx; loop < src_route_size; ++loop)
    {
        RoutePointC &src_point = GetPointWithDir(loop);

        Point2D new_point;
        new_point.x = src_point.stPos.x + dist * std::cos(src_point.heading + M_PI_2);
        new_point.y = src_point.stPos.y + dist * std::sin(src_point.heading + M_PI_2);

        bool is_end_point = (loop == (src_route_size - 1));
        new_route.ExtendEndWithSmooth(new_point, min_turn_radius, src_point.traj_no + delta_of_trj_no, is_end_point);
    }

    return 0;
}

int Route::GenParallelRouteInc(float dist, float min_turn_radius, Route &new_route,
                               unsigned int start_idx, unsigned int end_idx)
{
    unsigned int src_route_size = GetRouteSize();
    R_ASSERT_2(start_idx < src_route_size, -1, start_idx, src_route_size);

    unsigned int end_idx_e = std::min(end_idx, src_route_size - 1);
    R_ASSERT_2(start_idx <= end_idx_e, -1, start_idx, end_idx);

    new_route.vec_route_.reserve((unsigned int)((end_idx_e - start_idx + 1) * 1.5) + new_route.GetRouteSize());

    SetAllPointsDir();

    int delta_of_trj_no = (dist >= 0) ? 1 : -1;

    for (unsigned int loop = start_idx; loop <= end_idx_e; ++loop)
    {
        RoutePointC &src_point = GetPointWithDir(loop);

        Point2D new_point;
        new_point.x = src_point.stPos.x + dist * std::cos(src_point.heading + M_PI_2);
        new_point.y = src_point.stPos.y + dist * std::sin(src_point.heading + M_PI_2);

        bool is_end_point = (loop == end_idx_e);
        new_route.ExtendEndWithSmooth(new_point, min_turn_radius, src_point.traj_no + delta_of_trj_no, is_end_point);
    }

    return 0;
}

/**************************************************************************************
功能描述: 将路径反向
修改记录:
**************************************************************************************/
void Route::Reverse(void)
{
    is_tail_points_exist_dir_ = false;
    set_dir_point_num_ = 0;
    set_car_pose_point_num_ = 0;
    set_sum_of_front_trj_no_squ_num_ = 0;

    reverse(vec_route_.begin(), vec_route_.end());

    for (RoutePointC &route_point : vec_route_)
    {
        route_point.is_dir_valid = false;
    }
}

/**************************************************************************************
功能描述: 交换两条路径的数据
修改记录:
**************************************************************************************/
void Route::Swap(Route &other_route)
{
    std::swap(this->is_tail_points_exist_dir_, other_route.is_tail_points_exist_dir_);
    std::swap(this->set_dir_point_num_, other_route.set_dir_point_num_);
    std::swap(this->set_car_pose_point_num_, other_route.set_car_pose_point_num_);
    std::swap(this->set_sum_of_front_trj_no_squ_num_, other_route.set_sum_of_front_trj_no_squ_num_);
    this->vec_route_.swap(other_route.vec_route_);
}

void Route::Clear(void)
{
    is_tail_points_exist_dir_ = false;
    set_dir_point_num_ = 0;
    set_car_pose_point_num_ = 0;
    set_sum_of_front_trj_no_squ_num_ = 0;
    vec_route_.clear();
}

void Route::ClearEnd(void)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT(route_size > 0, NOTHING);

    unsigned int left_size = route_size - 1;

    is_tail_points_exist_dir_ = false;
    set_dir_point_num_ = std::min(set_dir_point_num_, left_size);
    set_car_pose_point_num_ = std::min(set_car_pose_point_num_, left_size);
    set_sum_of_front_trj_no_squ_num_ = std::min(set_sum_of_front_trj_no_squ_num_, left_size);

    vec_route_.pop_back();

    ClrTailPointsDir();
}

void Route::ClearAfter(unsigned int idx)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(idx < route_size, NOTHING, idx, route_size);

    unsigned int left_size = idx + 1;
    if (left_size == route_size)
    {
        return;
    }

    is_tail_points_exist_dir_ = false;
    set_dir_point_num_ = std::min(set_dir_point_num_, left_size);
    set_car_pose_point_num_ = std::min(set_car_pose_point_num_, left_size);
    set_sum_of_front_trj_no_squ_num_ = std::min(set_sum_of_front_trj_no_squ_num_, left_size);

    /* 删除idx之后的，不包括idx */
    auto iter = vec_route_.begin();
    advance(iter, left_size);
    vec_route_.erase(iter, vec_route_.end());

    ClrTailPointsDir();
}

void Route::ClearAtAndAfter(unsigned int idx)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(idx < route_size, NOTHING, idx, route_size);

    is_tail_points_exist_dir_ = false;
    set_dir_point_num_ = std::min(set_dir_point_num_, idx);
    set_car_pose_point_num_ = std::min(set_car_pose_point_num_, idx);
    set_sum_of_front_trj_no_squ_num_ = std::min(set_sum_of_front_trj_no_squ_num_, idx);

    /* 删除idx及之后的 */
    auto iter = vec_route_.begin();
    advance(iter, idx);
    vec_route_.erase(iter, vec_route_.end());

    ClrTailPointsDir();
}

void Route::Print(const std::string &str_des)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT(route_size > 0, NOTHING);

    ST_LOG_INFO("*********************** Begin ************************");

    ST_LOG_INFO("[%s] Route Size = %u, set_car_pose_point_num_ = %u, set_sum_of_front_trj_no_squ_num_ = %u.",
                str_des.c_str(), route_size, set_car_pose_point_num_, set_sum_of_front_trj_no_squ_num_);

    for (RoutePointC &route_point : vec_route_)
    {
        ST_LOG_INFO("x = %5.2f, y = %5.2f, isSetA = %d, a = %5.2f, 
                    trj_no = % 3d,
                    proj_idx_by_origin_route = % 3u, sum_of_front_trj_no_squ = % u.",
                                                                                 route_point.pose.x,
                    route_point.pose.y, route_point.heading, route_point.objDir.Val(),
                    route_point.traj_no, route_point.proj_idx_by_origin_route, route_point.sum_of_front_trj_no_squ);
    }

    ST_LOG_INFO("************************* End *************************");
}

void Route::SetTailPointDir(float dir)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT(route_size > 0, NOTHING);

    unsigned int tailIdx = route_size - 1;
    RoutePointC &cur_point = GetPoint(tailIdx);

    cur_point.heading = dir;
    cur_point.is_dir_valid = true;
}

void Route::SetPointDir(unsigned int idx)
{
    RoutePointC &cur_point = GetPoint(idx);

    unsigned int end_point_idx = GetRouteSize() - 1;
    if (end_point_idx == 0)
    {
        /* 只有一个点时，方向角直接为0 */
        cur_point.heading = 0.0;
    }
    else if (idx == end_point_idx)
    {
        /* 倒数第一个点方向角与前一个点保持一致 */
        RoutePointC &pre_point = GetPointWithDir(idx - 1);
        cur_point.heading = pre_point.heading;

        is_tail_points_exist_dir_ = true;
    }
    else
    {
        /* 方向角采用该点指向后面某个点的角度 */
        unsigned int aim_idx_t = idx + aim_dist_for_point_dir_;
        unsigned int aim_idx;
        if (aim_idx_t > end_point_idx)
        {
            is_tail_points_exist_dir_ = true;
            aim_idx = end_point_idx;
        }
        else
        {
            aim_idx = aim_idx_t;
        }

        const RoutePointC &aim_point = GetPointConst(aim_idx);
        cur_point.heading = std::atan2(aim_point.pose.y - cur_point.pose.y, aim_point.pose.x - cur_point.pose.x);
    }

    cur_point.is_dir_valid = true;
}

/**************************************************************************************
功能描述: 更新指定位置之后的路径点的行进方向
修改记录:
**************************************************************************************/
void Route::SetAllPointsDir(void)
{
    unsigned int route_size = GetRouteSize();
    R_ASSERT_2(set_dir_point_num_ <= route_size, NOTHING, set_dir_point_num_, route_size);
    if (set_dir_point_num_ == route_size)
    {
        return;
    }

    unsigned int loop = set_dir_point_num_;

    /* 存在正常瞄准点的情况 */
    unsigned int normal_aim_num = (route_size > aim_dist_for_point_dir_) ? (route_size - aim_dist_for_point_dir_) : 0;
    for (; loop < normal_aim_num; ++loop)
    {
        RoutePointC &cur_point = GetPoint(loop);
        if (cur_point.is_dir_valid)
        {
            continue;
        }

        RoutePointC &aim_point = GetPoint(loop + aim_dist_for_point_dir_);
        cur_point.heading = std::atan2(aim_point.pose.y - cur_point.pose.y, aim_point.pose.x - cur_point.pose.x);
        cur_point.is_dir_valid = true;
    }

    /* 瞄准最后一个点的情况 */
    unsigned int end_point_idx = route_size - 1;
    RoutePointC &end_point = GetPoint(end_point_idx);
    for (; loop < end_point_idx; ++loop)
    {
        RoutePointC &cur_point = GetPoint(loop);
        if (cur_point.is_dir_valid)
        {
            continue;
        }

        cur_point.heading = std::atan2(end_point.stPos.y - cur_point.stPos.y, end_point.stPos.x - cur_point.stPos.x);
        cur_point.is_dir_valid = true;
    }

    /* 最后一个点 */
    if (!end_point.is_dir_valid)
    {
        if (end_point_idx == 0)
        {
            end_point.heading = 0;
        }
        else
        {
            RoutePointC &last2_point = GetPoint(end_point_idx - 1);
            end_point.heading = last2_point.heading;
        }

        end_point.is_dir_valid = true;
    }

    is_tail_points_exist_dir_ = true;
    set_dir_point_num_ = route_size;
}
