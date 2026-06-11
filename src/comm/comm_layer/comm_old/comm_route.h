#ifndef __COMM_ROUTE_H__
#define __COMM_ROUTE_H__

#include "comm_coordinate.h"
#include "comm_angle.h"
#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_math/comm_math.h"

#include <list>
#include <vector>

#define ROUTE_SIZE_TO_LEN(route_size) float((route_size)*g_waypoint_interval) /* 简化计算 每个点就认为是一段 */
#define ROUTE_LEN_TO_SIZE(route_len) (unsigned int)(((route_len) + 0.5 * g_waypoint_interval) * (1.0 / g_waypoint_interval))

struct RouteSegInfo
{
    bool isGoForward; /* 该路径是否前进 */
};

/**************************************************************************************
功能描述: 点和路径相对信息
修改记录:
**************************************************************************************/
struct PoseRouteRelativeInfo
{
    float dist_to_route; // 指定位置到路径的垂直距离
    bool is_point_on_left;
    unsigned int proj_idx; // 指定位置在路径投影点的索引
};

/**************************************************************************************
功能描述: 路径点
修改记录:
**************************************************************************************/
class RoutePointC
{
public:
    Pose2D pose;               // 位置
    int traj_no;               // 平行路径号，全局路径为0，左侧为正，右侧为负
    unsigned int route_seg_no; // 路径段号

    bool is_dir_valid;
    float heading; // 前进方向

    Pose2D car_pose;                       // 小车经过该点位姿
    unsigned int proj_idx_by_origin_route; // 在原始路径上的投影点

    unsigned int sum_squ_of_front_trj_no; // 前部分路段的trjNO的平方和

    RoutePointC(void)
    {
    }

    RoutePointC(const Pose2D &pose_in, int trj_no_in, unsigned int seg_no_in)
    {
        pose = pose_in;
        traj_no = trj_no_in;
        route_seg_no = seg_no_in;
        is_dir_valid = false;
        proj_idx_by_origin_route = std::numeric_limits<unsigned int>::max();
        sum_squ_of_front_trj_no = 0;
    }
};

/**************************************************************************************
功能描述: 路径
修改记录:
**************************************************************************************/
class Route
{
public:
    Route(void)
    {
        Clear();
    }

    Route(const Route &src_route)
    {
        *this = src_route;
    }

    Route(const std::vector<Point2D> &vect_route, int trj_no = 0, unsigned int seg_no = 0)
    {
        SetRoute(vect_route, trj_no, seg_no);
    }

    const Route &operator=(const Route &src_route)
    {
        is_tail_points_exist_dir_ = src_route.is_tail_points_exist_dir_;
        set_dir_point_num_ = src_route.set_dir_point_num_;
        set_car_pose_point_num_ = src_route.set_car_pose_point_num_;
        set_sum_of_front_trj_no_squ_num_ = src_route.set_sum_of_front_trj_no_squ_num_;
        vec_route_ = src_route.vec_route_;

        return *this;
    }

    bool IsValid(void) const;
    bool IsPointIdxValid(unsigned int point_idx) const;
    bool IsPointIdxAtEnd(unsigned int point_idx) const;
    bool IsPointOnRoutePointLeft(const Point2D &point, unsigned int route_point_idx);

    void SetRoute(const std::vector<Point2D> &vect_route, int trj_no = 0, unsigned int seg_no = 0);
    void SetCarPose(unsigned int proj_idx, Pose2D &car_pose, unsigned int origin_route_proj_idx);
    int SetTrjNo(unsigned int point_idx, int trj_no);

    int GetPointPos(unsigned int point_idx, Point2D &point) const;
    int GetPointTrj(unsigned int point_idx, int &trj_no) const;
    int GetPointSegNo(unsigned int point_idx, unsigned int &seg_no) const;
    int GetPointOriginRouteProjectIdx(unsigned int point_idx, unsigned int &origin_route_proj_idx) const;
    int GetPointPosAndOriginRouteProjectIdx(unsigned int point_idx, Point2D &point,
                                            unsigned int &origin_route_proj_idx) const;
    int GetPointPose(unsigned int point_idx, Pose2D &pose);
    int GetPointPoseAndTrjNo(unsigned int point_idx, Pose2D &pose, int &trj_no);
    int GetPointDirAngle(unsigned int point_idx, float &angle);
    int GetPointIdxByOriginRouteProjectIdx(unsigned int origin_route_proj_idx) const;

    int GetEndPointPos(Point2D &point);
    int GetEndPointPose(Pose2D &pose);
    int GetEndPointPoseAndTrjNo(Pose2D &pose, int &trj_no);
    int GetEndPointIdx(void) const;

    unsigned int GetCarPose(Pose2D &car_pose, unsigned int last_point_idx = std::numeric_limits<unsigned int>::max()) const;

    int GetAimPos(unsigned int proj_idx, float aim_dist, Point2D &aim_point);

    int GetRelativeInfo(Pose2D &pose, PoseRouteRelativeInfo &relative_info,
                        unsigned int search_begin_idx = 0,
                        float max_search_len = 0.0,
                        bool is_must_match_dir = false);
    int GetRelativeInfo(const Point2D &point, PoseRouteRelativeInfo &relative_info,
                        unsigned int search_begin_idx = 0, float max_search_len = 0.0) const;
    unsigned int GetLocNearestPointIdx(const Point2D &point, unsigned int loc_range_seed_idx);
    int GetFirstPointIdxOfFrontAndSameDir(Pose2D &pose, unsigned int &point_idx,
                                          unsigned int search_begin_idx = 0,
                                          unsigned int search_end_idx = g_max_unsigned_int);
    float GetLen(void) const;
    float GetLenAfter(unsigned int idx) const;
    float GetRateAfter(unsigned int idx) const;
    unsigned int GetSize(void) const;
    unsigned int GetAvgOfTrjNoSqu(void);

    void GetRoute(std::vector<Point2D> &vec_route) const;
    int GetSubRoute(Route &sub_route, unsigned int begin_idx = 0, unsigned int end_idx = g_max_unsigned_int) const;
    int GetSubRoute(Route &sub_route, unsigned int begin_idx, float max_sub_route_len) const;

    void ExtendEnd(const Point2D &new_point, int trj_no = 0, bool is_add_closer = false, unsigned int seg_no = 0);
    void ExtendEnd(const std::vector<Point2D> &vec_new_points, int trj_no = 0,
                   bool is_add_closer = false, unsigned int seg_no = 0);
    void ExtendEndByPointsOfSmoothTrjNo(const std::vector<Point2D> &vec_new_points, int trj_no_begin, int trj_no_end);
    void ExtendEnd(Route &new_route);
    void ExtendEnd(float forward_len);
    void ExtendEndWithSmooth(const Point2D &new_point, float min_turn_radius,
                             int trj_no = 0, bool is_add_closer = false, unsigned int seg_no = 0);
    void ExtendEndWithSmooth(const std::vector<Point2D> &vec_new_points, float min_turn_radius,
                             int trj_no = 0, bool is_add_closer = false, unsigned int seg_no = 0);
    void ExtendEndWithSmooth(Route &new_route, float min_turn_radius);

    int GenParallelRoute(float dist, float min_turn_radius, Route &new_route, unsigned int start_idx = 0);
    int GenParallelRouteInc(float dist, float min_turn_radius, Route &new_route,
                            unsigned int start_idx = 0, unsigned int end_idx = g_max_unsigned_int);

    void Reverse(void);
    void Swap(Route &other_route);

    void Clear(void);
    void ClearEnd(void);
    void ClearAfter(unsigned int idx);
    void ClearAtAndAfter(unsigned int idx);

    void Print(const std::string &str_des);

    void SetTailPointDir(float dir);

private:
    const unsigned int aim_dist_for_point_dir_ = 3;

    bool is_tail_points_exist_dir_;                /* 尾部的路径点中是否存在部分点已经计算方向角了 */
    unsigned int set_dir_point_num_;               /* 从起点连续的已经被计算方向角的数量 */
    unsigned int set_car_pose_point_num_;          /* RoutePointC.car_pose 和 RoutePointC.origin_route_proj_idx 的有效性受此限制 */
    unsigned int set_sum_of_front_trj_no_squ_num_; /* 求取过前段trjNo平方和的路径点的数量 */
    std::vector<RoutePointC> vec_route_;

    inline unsigned int GetRouteSize(void) const
    {
        return (unsigned int)(vec_route_.size());
    }

    inline void ClrTailPointsDir(void)
    {
        unsigned int route_size = (unsigned int)(vec_route_.size());
        unsigned int min_clr_idx = (route_size > aim_dist_for_point_dir_) ? (route_size - aim_dist_for_point_dir_) : 0;
        for (int loop = int(route_size) - 1; loop >= int(min_clr_idx); --loop)
        {
            RoutePointC &route_point = vec_route_[loop];
            route_point.is_dir_valid = false;
        }

        is_tail_points_exist_dir_ = false;
        set_dir_point_num_ = std::min(set_dir_point_num_, min_clr_idx);
    }

    inline void AddPoint(const Point2D &point, int traj_no, unsigned int seg_no)
    {
        if (is_tail_points_exist_dir_)
        {
            ClrTailPointsDir();
        }

        vec_route_.push_back(RoutePointC(point, traj_no, seg_no));
    }

    inline void AddPoints(const std::vector<Pose2D> &vect_point, int trj_no, unsigned int seg_no)
    {
        if (is_tail_points_exist_dir_)
        {
            ClrTailPointsDir();
        }

        unsigned int add_point_size = vect_point.size();
        vec_route_.reserve(GetRouteSize() + add_point_size);

        RoutePointC route_point(Point2D(), trj_no, seg_no);
        for (unsigned int loop = 0; loop < add_point_size; ++loop)
        {
            route_point.pose = vect_point[loop];
            vec_route_.push_back(route_point);
        }
    }

    inline const RoutePointC &GetPointConst(unsigned int idx) const
    {
        return vec_route_[idx];
    }

    inline RoutePointC &GetPoint(unsigned int idx)
    {
        return vec_route_[idx];
    }

    inline RoutePointC &GetPointWithDir(unsigned int idx)
    {
        RoutePointC &route_point = vec_route_[idx];
        if (!route_point.is_dir_valid)
        {
            SetPointDir(idx);
        }

        return route_point;
    }

    inline void SetTrjNoWithNoProtect(unsigned int point_idx, int trj_no)
    {
        RoutePointC &point = GetPoint(point_idx);
        point.traj_no = trj_no;

        set_sum_of_front_trj_no_squ_num_ = std::min(set_sum_of_front_trj_no_squ_num_, point_idx);
    }

    void SetPointDir(unsigned int idx);
    void SetAllPointsDir(void);
};

#endif
