

#ifndef COMM_GEOMETRY_H
#define COMM_GEOMETRY_H

#include <cmath>
#include <cstdlib>
#include <list>
#include <vector>

#include "comm_layer/comm_geometry/comm_rect_area.h"
#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_math/comm_math.h"

// 二维向量
class Vec2D
{
public:
    Vec2D(void);
    Vec2D(const Point2D &start_point, const Point2D &end_point);
    Vec2D(float len, float angle);

    bool IsDirConsistent(Vec2D &other); // 判断方向是否一致，即夹角小于90°

    float GetLen(void) const; // 获取向量长度

    float GetDirAngle(void);

    float GetLenSqu(void) const;

    float DotMult(Vec2D &other); // 获取点乘

private:
    inline void ToPoint(void);
    inline void ToDirLen(void);

private:
    bool is_valid_;
    bool is_exist_point_;
    Point2D point_;

    bool is_exist_dir_len_;
    float len_;
    float angle_;
};

// 有向直线, 采用一个点和一个向量来表示
class DirectedLine
{
public:
    DirectedLine(void);
    DirectedLine(const Point2D &start_point, const Point2D &end_point);
    DirectedLine(const Point2D &start_point, float angle);
    DirectedLine(const Point2D &start_point, float angle_sin, float angle_cos);
    DirectedLine(const Pose2D &pose);

    void Reset(const Point2D &start_point, const Point2D &end_point);
    void Reset(const Point2D &start_point, float angle);
    void Reset(const Point2D &start_point, float angle_sin, float angle_cos);
    void Clear(void);

    bool IsValid(void) const;

    int GetPointSide(const Point2D &point) const;
    bool IsPointOnLeft(const Point2D &point) const;
    bool IsPointOnLine(const Point2D &point) const;
    bool IsPointOnRight(const Point2D &point) const;

    int GetIntersectionWithAnotherLine(const DirectedLine &other_line, Point2D &insect_point) const;
    int GetProjectPoint(const Point2D &point, Point2D &project_point) const;
    float GetDistSquToPoint(const Point2D &point) const;
    float GetDirAngle(void) const;

protected:
    Point2D start_point_;
    Point2D vec2d_;
};

// 二维射线
class Ray2D : public DirectedLine
{
public:
    Ray2D(const Point2D &start_point, const Point2D &forward_point) : DirectedLine(start_point, forward_point)
    {
    }

    Ray2D(const Point2D &point, float angle) : DirectedLine(point, angle)
    {
    }

    Ray2D(const Pose2D &pose) : DirectedLine(pose)
    {
    }

    const Point2D &GetStartPoint(void) const;

    bool IsPointOnFront(const Point2D &point) const;
    bool IsPointOnRay(const Point2D &point) const;

    bool IsIntersectWithArea(const RectArea &rect_area) const;
};

// 有向线段
class DirectedLineSeg : public Ray2D
{
public:
    DirectedLineSeg(const Point2D &start_point, const Point2D &end_point)
        : Ray2D(start_point, end_point)
    {
        end_point_ = end_point;
    }

    bool IsIntersectedWithRay(const Ray2D &ray2d) const;
    bool IsIntersectedWithOtherLineSeg(const DirectedLineSeg &other) const;
    bool IsPointOnLineSeg(const Point2D &point) const;

    const Point2D &GetEndPoint(void) const;
    void GetRectArea(RectArea &rect_area) const;
    int GetIntersectionWithRay(const Ray2D &ray2d, Point2D &intersection_point) const;
    float GetDistanceToPoint(const Point2D &point) const;

private:
    Point2D end_point_;
};

// 多段有向直线。有序的多个点，两两依次组成多个有向直线；处理多边形时，仅用于处理凸多边形。
class MultiDerectedLine
{
public:
    MultiDerectedLine(void);
    ~MultiDerectedLine(void);

    void AddExtendPoint(const Point2D &new_point);
    void AddExtendPoint(float point_x, float point_y);
    void Clear(void);
    bool IsPointOnLeft(const Point2D &point); /* 暂未使用，接口待优化 */

private:
    bool is_first_point_exist_;
    Point2D last_point_;
    std::list<DirectedLine *> directed_line_list_;
};

// 任意多边形
class AnyPolygon
{
public:
    AnyPolygon(const std::vector<Point2D> &vec_vertice)
    {
        (void)Reset(vec_vertice);
    }

    AnyPolygon(void)
    {
    }

    ~AnyPolygon(void)
    {
    }

    bool IsValid(void) const;
    bool IsPointIn(const Point2D &point) const;
    bool IsIntersectWithLineSeg(const DirectedLineSeg &line_seg) const;
    bool IsIntersectWithOtherPolygon(const AnyPolygon &other) const;
    bool IsExistOverlapWithOtherPolygon(const AnyPolygon &other) const;
    bool IsCircleOut(const Point2D &center, float radius) const;
    int Reset(const std::vector<Point2D> &vec_vertice);
    int GetRectArea(RectArea &rect_area) const;

private:
    struct EdgeGroup
    {
        RectArea area;
        std::vector<DirectedLineSeg> vec_edge;
    };

    void DspAllVertice(const std::vector<Point2D> &vec_vertice) const;
    void DivideEdgeGroup(const std::vector<Point2D> &vec_vertice, bool is_close,
                         std::vector<EdgeGroup> &vec_group) const;
    bool IsExistEdgesIntersectInGroup(const EdgeGroup &group, bool is_end_closed) const;
    bool IsExistEdgesIntersectBetweenSeparatedGroup(const EdgeGroup &group1, const EdgeGroup &group2) const;
    bool IsExistEdgesIntersectBetweenConnectedGroup(const EdgeGroup &front_group,
                                                    const EdgeGroup &back_group,
                                                    bool is_end_closed) const;
    bool IsExistEdgesIntersected(const std::vector<EdgeGroup> &vec_group) const;
    int MakePolygon(const std::vector<Point2D> &vec_vertice);

    void Clear(void);

private:
    RectArea rect_area_;
    std::vector<EdgeGroup> vec_edge_group_;
};

// 凸多边形。给定的多边形定点合法性由调用方保证。
class ConvexPolygon
{
public:
    ConvexPolygon(Point2D points[], int point_num);
    ConvexPolygon(void);
    ~ConvexPolygon(void);

    int Reset(Point2D points[], int point_num);

    bool IsPointIn(const Point2D &point) const;

    float GetDistSquOfInternalPointAlongDirToEdge(const Point2D &internal_point, float angle);
    float GetMinDistSquOfInternalPointToEdge(const Point2D &internal_point);

private:
    int AddVertice(Point2D &point);
    bool Check(void);
    void Clear(void);
    int Make(Point2D points[], int point_num);
    DirectedLineSeg *GetLineSegIntersectedByInternalRay(Ray2D &internal_ray);

private:
    bool is_exist_first_vertice_; // 临时变量，是否已存在第一个顶点
    Point2D last_vertice_;        // 临时变量，上次添加的顶点
    std::list<DirectedLineSeg *> directed_line_seg_list_;
};

// 计算凸包（基于Graham扫描法，极角排序）
class ConvexHull
{
public:
    ConvexHull() {}
    ~ConvexHull() {}

    bool IsConvexHull(const std::vector<Point2D> &points) const;
    std::vector<Point2D> GetConvexHull(const std::vector<Point2D> &points) const;

private:
    bool Compare(const Point2D &a, const Point2D &b, const Point2D &origin) const;
    std::vector<Point2D> ConvexHullTurn(std::vector<Point2D> &points) const;
};

// 三角形
class Triangle
{
public:
    float Get3thSideLenSqu(const float &side_len1, const float &side_len2, const float &angle_2_side);

private:
};

// 圆形
class Circular
{
public:
    Circular(const Point2D &center, float radius);
    Circular(const Point2D &point1, const Point2D &point2, const Point2D &point3);

    bool IsValid(void) const;
    int GetCenter(Point2D &center) const;
    float GetRadiusSqu(void) const;
    float GetRadius(void);
    int GetSamplePoints(const Point2D &start_point, const Point2D &end_point,
                        float delta_rad_abs, std::vector<Point2D> &vec_sample_points);
    bool IsPointIn(const Point2D &point) const;
    bool IsIntersectedLineSegment(const Point2D &start_point, const Point2D &end_point) const;

private:
    bool is_valid_ = false;
    Point2D center_;   // is_valid_ 为 true 时有效
    float radius_squ_; // is_valid_ 为 true 时有效
    bool is_exist_radius_ = false;
    float radius_; // isExistRadius_ 为 true 时有效
};

// 扇形
class Sector
{
public:
    Sector(const Point2D &center, float radius, float begin_angle, float angle_range)
        : center_(center),
          radius_(radius),
          is_angle_range_positive_(angle_range >= 0.0),
          is_angle_range_over_pi_(std::abs(angle_range) > M_PI),
          begin_angle_line_(center, begin_angle),
          end_angle_line_(center, begin_angle + angle_range)
    {
    }

    bool IsPointIn(const Point2D &point);

private:
    const Point2D center_;
    const float radius_;
    const bool is_angle_range_positive_;
    const bool is_angle_range_over_pi_;
    const DirectedLine begin_angle_line_;
    const DirectedLine end_angle_line_;
};

#endif // COMM_GEOMETRY_H
