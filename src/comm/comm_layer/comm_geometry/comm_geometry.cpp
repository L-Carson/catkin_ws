

#include "comm_layer/comm_geometry/comm_geometry.h"

#include <string>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <type_traits>

Vec2D::Vec2D(void)
{
    is_valid_ = false;
}

Vec2D::Vec2D(const Point2D &start_point, const Point2D &end_point)
{
    point_.x = end_point.x - start_point.x;
    point_.y = end_point.y - start_point.y;

    is_exist_point_ = true;
    is_exist_dir_len_ = false;
    is_valid_ = true;
}

Vec2D::Vec2D(float len, float angle)
{
    len_ = len;
    angle_ = angle;

    is_exist_point_ = false;
    is_exist_dir_len_ = true;
    is_valid_ = true;
}

// 判断方向是否一致
bool Vec2D::IsDirConsistent(Vec2D &other)
{
    // // R_ASSERT(is_valid_, false);

    return (DotMult(other) > 0);
}

float Vec2D::GetDirAngle(void)
{
    // // R_ASSERT(is_valid_, 0.0);

    if (!is_exist_dir_len_)
    {
        ToDirLen();
    }

    return angle_;
}

float Vec2D::GetLenSqu(void) const
{
    // // R_ASSERT(is_valid_, -1);

    if (is_exist_point_)
    {
        return (point_.x * point_.x + point_.y * point_.y);
    }
    else
    {
        return len_ * len_;
    }
}

float Vec2D::DotMult(Vec2D &other)
{
    // // R_ASSERT(is_valid_ && other.is_valid_, 0.0);

    /* 优先坐标计算，再次长度和角度计算，最后转成坐标进行计算 */
    if (is_exist_point_ && other.is_exist_point_)
    {
        return (point_.x * other.point_.x + point_.y * other.point_.y);
    }
    else if (is_exist_dir_len_ && is_exist_dir_len_)
    {
        return (len_ * other.len_ * cos(angle_ - other.angle_));
    }
    else
    {
        if (is_exist_point_)
        {
            other.ToPoint();
        }
        else
        {
            ToPoint();
        }

        return (point_.x * other.point_.x + point_.y * other.point_.y);
    }
}

inline void Vec2D::ToPoint(void)
{
    point_.x = len_ * std::cos(angle_);
    point_.y = len_ * std::sin(angle_);

    is_exist_point_ = true;
}

inline void Vec2D::ToDirLen(void)
{
    len_ = std::sqrt(point_.x * point_.x + point_.y * point_.y);
    angle_ = std::atan2(point_.y, point_.x);

    is_exist_dir_len_ = true;
}

DirectedLine::DirectedLine(void)
{
    Clear();
}

DirectedLine::DirectedLine(const Point2D &start_point, const Point2D &end_point)
{
    Reset(start_point, end_point);
}

DirectedLine::DirectedLine(const Point2D &start_point, float angle)
{
    Reset(start_point, angle);
}

DirectedLine::DirectedLine(const Point2D &start_point, float angle_sin, float angle_cos)
{
    Reset(start_point, angle_sin, angle_cos);
}

DirectedLine::DirectedLine(const Pose2D &pose)
{
    Reset({pose.x, pose.y}, std::sin(pose.theta), std::cos(pose.theta));
}

void DirectedLine::Reset(const Point2D &start_point, const Point2D &end_point)
{
    start_point_ = start_point;
    vec2d_.x = end_point.x - start_point_.x;
    vec2d_.y = end_point.y - start_point_.y;
}

void DirectedLine::Reset(const Point2D &start_point, float angle)
{
    start_point_ = start_point;

    vec2d_.x = std::cos(angle);
    vec2d_.y = std::sin(angle);
}

void DirectedLine::Reset(const Point2D &start_point, float angle_sin, float angle_cos)
{
    start_point_ = start_point;

    vec2d_.x = angle_cos;
    vec2d_.y = angle_sin;
}

void DirectedLine::Clear(void)
{
    start_point_ = {0.0f, 0.0f};
    vec2d_ = {0.0f, 0.0f};
}

bool DirectedLine::IsValid(void) const
{
    return (vec2d_.x * vec2d_.x + vec2d_.y * vec2d_.y > std::numeric_limits<float>::epsilon());
}

/**************************************************************************************
功能描述: 获取给定点在直线的某一侧
          小于0表示右侧，大于0表示左侧，等于0表示在直线上
修改记录:
**************************************************************************************/
int DirectedLine::GetPointSide(const Point2D &point) const
{
    /* 计算直线起点到指定点的向量 */
    Point2D point_vector = {point.x - start_point_.x, point.y - start_point_.y};

    /* 叉乘: V1 X V2 = |V1|*|V2|*sin */
    float vector_cross_mult = vec2d_.x * point_vector.y - vec2d_.y * point_vector.x;
    float vector_len = std::fabs(vec2d_.x) + std::fabs(vec2d_.y);                   // 向量长度，近似计算
    float point_vector_len = std::fabs(point_vector.x) + std::fabs(point_vector.y); // 向量长度，近似计算
    float len_mult = vector_len * point_vector_len;
    if (len_mult <= std::numeric_limits<float>::epsilon())
    {
        return 0;
    }

    /* 计算两个向量夹角，以及点到直线的距离。 */
    /* 夹角和距离任何一个小于阈值就表示点在线上。 */
    /* 该门限不能太小，容易误判 */
    float sin_2vector = vector_cross_mult / len_mult;
    float dist_to_line = std::abs(sin_2vector * point_vector_len);
    /* 这里的距离阈值与 IsIntersectedWithOtherLineSeg 保持一致             */
    if ((std::abs(sin_2vector) < 1.0e-4) || (dist_to_line < 1.0e-3))
    {
        return 0;
    }
    else if (sin_2vector < 0.0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**************************************************************************************
功能描述: 判断给定点是否在该有向直线的左侧
修改记录:
**************************************************************************************/
bool DirectedLine::IsPointOnLeft(const Point2D &point) const
{
    return (GetPointSide(point) > 0);
}

/**************************************************************************************
功能描述: 判断给定点是否在该有向直线上
修改记录:
**************************************************************************************/
bool DirectedLine::IsPointOnLine(const Point2D &point) const
{
    return (GetPointSide(point) == 0);
}

/**************************************************************************************
功能描述: 判断给定点是否在该有向直线的右侧
修改记录:
**************************************************************************************/
bool DirectedLine::IsPointOnRight(const Point2D &point) const
{
    return (GetPointSide(point) < 0);
}

/**************************************************************************************
功能描述: 获取两条直线的交点
修改记录:
**************************************************************************************/
int DirectedLine::GetIntersectionWithAnotherLine(const DirectedLine &other_line, Point2D &insect_point) const
{
    /* 判断是否平行，平行无交点 */
    float denominator = other_line.vec2d_.x * vec2d_.y - other_line.vec2d_.y * vec2d_.x;
    if (std::abs(denominator) < 0.000001)
    {
        return -1;
    }

    float factor = ((start_point_.x - other_line.start_point_.x) * vec2d_.y -
                    (start_point_.y - other_line.start_point_.y) * vec2d_.x) /
                   denominator;
    insect_point.x = other_line.start_point_.x + factor * other_line.vec2d_.x;
    insect_point.y = other_line.start_point_.y + factor * other_line.vec2d_.y;
    return 0;
}

int DirectedLine::GetProjectPoint(const Point2D &point, Point2D &project_point) const
{
    DirectedLine line;
    line.start_point_ = point;
    line.vec2d_ = {-1.0f * this->vec2d_.y, this->vec2d_.x};

    return GetIntersectionWithAnotherLine(line, project_point);
}

/**************************************************************************************
功能描述: 获取给定点到直线的垂直距离的平方
修改记录:
**************************************************************************************/
float DirectedLine::GetDistSquToPoint(const Point2D &point) const
{
    float vec2d_len_squ = vec2d_.x * vec2d_.x + vec2d_.y * vec2d_.y;

    /* 如果表示直行反向的向量长度几乎为0，则无法确定直行方向，无法计算点到该直行的距离。 */
    if (IsNearZero(vec2d_len_squ))
    {
        return -1;
    }

    /* 根据两个叉乘的结果数字上等于对应平行四边形面积，来计算点到直线的距离 */
    Point2D point_vec2d = {point.x - start_point_.x, point.y - start_point_.y};
    float cross_multi = vec2d_.x * point_vec2d.y - point_vec2d.x * vec2d_.y;
    return (cross_multi * cross_multi) / vec2d_len_squ;
}

float DirectedLine::GetDirAngle(void) const
{
    return std::atan2(vec2d_.y, vec2d_.x);
}

const Point2D &Ray2D::GetStartPoint(void) const
{
    return start_point_;
}

bool Ray2D::IsPointOnFront(const Point2D &point) const
{
    Point2D point_vec2d = {point.x - start_point_.x, point.y - start_point_.y};

    /* 点乘大于0则在前方 */
    return ((vec2d_.x * point_vec2d.x + vec2d_.y * point_vec2d.y) >= 0.0);
}

bool Ray2D::IsPointOnRay(const Point2D &point) const
{
    if (!DirectedLine::IsPointOnLine(point))
    {
        return false;
    }

    /* 根据点乘符号判断是否在前方 */
    Point2D point_vec2d = {point.x - start_point_.x, point.y - start_point_.y};
    float dot_mult = vec2d_.x * point_vec2d.x + vec2d_.y * point_vec2d.y;
    return (dot_mult >= 0.0);
}

bool Ray2D::IsIntersectWithArea(const RectArea &rect_area) const
{
    if (!rect_area.IsValid())
    {
        return false;
    }

    Point2D area_vec2d = {rect_area.RightTopPoint().x - rect_area.LeftBackPoint().x,
                          rect_area.RightTopPoint().y - rect_area.LeftBackPoint().y};
    bool is_area_vec2d_x_zero = (std::abs(area_vec2d.x) <= std::numeric_limits<float>::epsilon());
    bool is_area_vec2d_y_zero = (std::abs(area_vec2d.y) <= std::numeric_limits<float>::epsilon());
    if (is_area_vec2d_x_zero || is_area_vec2d_y_zero)
    {
        /* 区域呈现为点或线段 */
        if (is_area_vec2d_x_zero && is_area_vec2d_y_zero)
        {
            return this->IsPointOnRay(rect_area.RightTopPoint());
        }
        else
        {
            DirectedLineSeg line_seg(rect_area.LeftBackPoint(), rect_area.RightTopPoint());
            return line_seg.IsIntersectedWithRay(*this);
        }
    }
    else
    {
        /* 正常矩形区域 */
        Point2D left_back_point{rect_area.LeftBackPoint().x, rect_area.LeftBackPoint().y};
        Point2D left_top_point{rect_area.LeftBackPoint().x, rect_area.RightTopPoint().y};
        Point2D right_top_point{rect_area.RightTopPoint().x, rect_area.RightTopPoint().y};
        Point2D right_back_point{rect_area.RightTopPoint().x, rect_area.LeftBackPoint().y};
        DirectedLineSeg line_seg1(left_back_point, left_top_point);
        if (line_seg1.IsIntersectedWithRay(*this))
        {
            return true;
        }

        DirectedLineSeg line_seg2(left_top_point, right_top_point);
        if (line_seg2.IsIntersectedWithRay(*this))
        {
            return true;
        }

        DirectedLineSeg line_seg3(right_top_point, right_back_point);
        if (line_seg3.IsIntersectedWithRay(*this))
        {
            return true;
        }

        DirectedLineSeg line_seg4(right_back_point, left_back_point);
        if (line_seg4.IsIntersectedWithRay(*this))
        {
            return true;
        }

        return false;
    }
}

/**************************************************************************************
功能描述: 判断该线段是否和给定射线相交
修改记录:
**************************************************************************************/
bool DirectedLineSeg::IsIntersectedWithRay(const Ray2D &ray2d) const
{
    const Point2D &ray_start_point = ray2d.GetStartPoint();
    if (this->IsPointOnLeft(ray_start_point))
    {
        return (!ray2d.IsPointOnLeft(this->GetStartPoint()) &&
                !ray2d.IsPointOnRight(this->end_point_));
    }
    else if (this->IsPointOnRight(ray_start_point))
    {
        return (!ray2d.IsPointOnRight(this->GetStartPoint()) &&
                !ray2d.IsPointOnLeft(this->end_point_));
    }
    else
    {
        return (this->IsPointOnLineSeg(ray_start_point) ||
                ray2d.IsPointOnRay(start_point_) ||
                ray2d.IsPointOnRay(end_point_));
    }
}

bool DirectedLineSeg::IsIntersectedWithOtherLineSeg(const DirectedLineSeg &other) const
{
    /* 有重叠的点，就认为时相交 */
    /* 计算两个线段4个端点相对于另一条线段的位置 */
    int side_other_start = this->GetPointSide(other.start_point_);
    int side_other_end = this->GetPointSide(other.end_point_);
    int side_this_start = other.GetPointSide(this->start_point_);
    int side_this_end = other.GetPointSide(this->end_point_);

    bool is_calc_colline_abnormal = false;

    /* 如果某个端点在另一条线段的直线上，则该点为共线点。计算总的共线点的个数。 */
    int colline_point_num = 4 - std::abs(side_other_start) - std::abs(side_other_end) - std::abs(side_this_start) - std::abs(side_this_end);
    switch (colline_point_num)
    {
    case 0:
    { /* 没有共线点，直接判是否交叉 */
        return (((side_other_start * side_other_end) < 0) &&
                ((side_this_start * side_this_end) < 0));
    }

    case 1:
    { /* 一个共线点，判断另一个线段两端点是否在该线段两侧。 */
        bool is_colline_point_on_other = (side_other_start == 0) || (side_other_end == 0);
        if (is_colline_point_on_other)
        {
            return ((side_this_start * side_this_end) < 0);
        }
        else
        {
            return ((side_other_start * side_other_end) < 0);
        }
    }

    case 2:
    { /* 两个共线点，分为两种情况，他们在同一个线段上和在不同线段上。 */
        bool is_colline_point_on_same = (std::abs(side_other_start) == std::abs(side_other_end));
        if (is_colline_point_on_same)
        {
            /* 他们在同一个线段上 */
            /* 这种情况，理论上不应该出现，应该只出现在两线段共线时，计算误差导致另外两个端点共线。此处还是当共线处理 */
            is_calc_colline_abnormal = true;
            break;
        }
        else
        {
            /* 他们在不同线段上 */ /* 又分为两种情况，两个共线点是重叠的，还有就是两个线段共线但计算误差导致 */
            const Point2D &colline_point_other = (side_other_start == 0) ? other.start_point_ : other.end_point_;
            const Point2D &colline_point_this = (side_this_start == 0) ? this->start_point_ : this->end_point_;
            /* 这里的距离阈值与 GetPointSide 保持一致             */
            if (CalcDisSquare(colline_point_other, colline_point_this) < 1.0e-6)
            {
                return true;
            }
            else
            {
                is_calc_colline_abnormal = true;
                break;
            }
        }
    }

    case 3:
    { /* 三个共线点，理论上不应该出现，应该只出现在两线段共线时，计算误差导致另外两个端点共线。此处还是当共线处理 */
        is_calc_colline_abnormal = true;
        break;
    }

    default:
    { /* 四个共线点，正常两线段共线 */
        break;
    }
    }

    /* 处理异常共线 */
    // if (is_calc_colline_abnormal) {
    //ST_LOG_INFO("Judge Line Colline Abnormal. Sides(%d %d %d %d), Other(%.2f, %.2f)-(%.2f, %.2f), This(%.2f, %.2f)-(%.2f, %.2f).",
    //            side_other_start,         side_other_end,           side_this_start,        side_this_end,
    //            other.start_point_.x, other.start_point_.y, other.end_point_.x, other.end_point_.y,
    //            this->start_point_.x,    this->start_point_.y,    this->end_point_.x,    this->end_point_.y);
    // }

    /* 共线时，一边两点比另一边两点都大或都小为不重叠交叉 */
    if (std::abs(this->start_point_.x - this->end_point_.x) > std::abs(this->start_point_.y - this->end_point_.y))
    {
        return !(((other.start_point_.x > this->start_point_.x) && (other.end_point_.x > this->start_point_.x) && (other.start_point_.x > this->end_point_.x) && (other.end_point_.x > this->end_point_.x)) || ((other.start_point_.x < this->start_point_.x) && (other.end_point_.x < this->start_point_.x) && (other.start_point_.x < this->end_point_.x) && (other.end_point_.x < this->end_point_.x)));
    }
    else
    {
        return !(((other.start_point_.y > this->start_point_.y) && (other.end_point_.y > this->start_point_.y) && (other.start_point_.y > this->end_point_.y) && (other.end_point_.y > this->end_point_.y)) || ((other.start_point_.y < this->start_point_.y) && (other.end_point_.y < this->start_point_.y) && (other.start_point_.y < this->end_point_.y) && (other.end_point_.y < this->end_point_.y)));
    }
}

bool DirectedLineSeg::IsPointOnLineSeg(const Point2D &point) const
{
    if (!IsPointOnLine(point))
    {
        return false;
    }

    /* 共线时 */
    if (std::abs(this->start_point_.x - this->end_point_.x) > std::abs(this->start_point_.y - this->end_point_.y))
    {
        return !(((point.x > this->start_point_.x) && (point.x > this->end_point_.x)) || ((point.x < this->start_point_.x) && (point.x < this->end_point_.x)));
    }
    else
    {
        return !(((point.y > this->start_point_.y) && (point.y > this->end_point_.y)) || ((point.y < this->start_point_.y) && (point.y < this->end_point_.y)));
    }
}

float DirectedLineSeg::GetDistanceToPoint(const Point2D &point) const
{
    float len = std::sqrt(std::pow(start_point_.x - end_point_.x, 2) + std::pow(start_point_.y - end_point_.y, 2));
    if (len < 0.5)
    {
        return std::sqrt(std::pow(point.x - start_point_.x, 2) + std::pow(point.y - start_point_.y, 2));
    }
    float t = ((point.x - start_point_.x) * (end_point_.x - start_point_.x) +
               (point.y - start_point_.y) * (end_point_.y - start_point_.y)) /
              (len * len);

    if (t < 0.0)
    {
        return std::sqrt(std::pow(point.x - start_point_.x, 2) + std::pow(point.y - start_point_.y, 2));
    }
    else if (t > 1.0)
    {
        return std::sqrt(std::pow(point.x - end_point_.x, 2) + std::pow(point.y - end_point_.y, 2));
    }
    Point2D closest_point = {start_point_.x + t * (end_point_.x - start_point_.x),
                             start_point_.y + t * (end_point_.y - start_point_.y)};
    return std::sqrt(std::pow(point.x - closest_point.x, 2) + std::pow(point.y - closest_point.y, 2));
}

const Point2D &DirectedLineSeg::GetEndPoint(void) const
{
    return end_point_;
}

void DirectedLineSeg::GetRectArea(RectArea &rect_area) const
{
    rect_area.Reset();
    rect_area.IncludePoint(start_point_);
    rect_area.IncludePoint(end_point_);
}

int DirectedLineSeg::GetIntersectionWithRay(const Ray2D &ray2d, Point2D &intersection_point) const
{
    if (!IsIntersectedWithRay(ray2d))
    {
        return -1;
    }

    return GetIntersectionWithAnotherLine(ray2d, intersection_point);
}

MultiDerectedLine::MultiDerectedLine(void)
{
    is_first_point_exist_ = false;
}

MultiDerectedLine::~MultiDerectedLine(void)
{
    Clear();
}

void MultiDerectedLine::AddExtendPoint(const Point2D &new_point)
{
    if (is_first_point_exist_)
    {
        DirectedLine *new_line_ptr = new DirectedLine(last_point_, new_point);
        if (new_line_ptr == nullptr)
        {
            // ST_LOG_ERR("new fail, x=%f, y=%f.", new_point.x, new_point.y);
            return;
        }

        directed_line_list_.push_back(new_line_ptr);
    }

    last_point_.x = new_point.x;
    last_point_.y = new_point.y;
    is_first_point_exist_ = true;
}

void MultiDerectedLine::AddExtendPoint(float point_x, float point_y)
{
    Point2D new_point = {point_x, point_y};

    AddExtendPoint(new_point);
}

void MultiDerectedLine::Clear(void)
{
    for (std::list<DirectedLine *>::iterator iter = directed_line_list_.begin();
         iter != directed_line_list_.end();
         iter++)
    {
        delete *iter;
    }

    directed_line_list_.clear();
}

bool MultiDerectedLine::IsPointOnLeft(const Point2D &point)
{
    if (directed_line_list_.size() == 0)
    {
        return false;
    }

    for (std::list<DirectedLine *>::iterator iter = directed_line_list_.begin();
         iter != directed_line_list_.end();
         iter++)
    {
        DirectedLine *directed_line_ptr = *iter;
        if (!directed_line_ptr->IsPointOnLeft(point))
        {
            return false;
        }
    }

    return true;
}

bool AnyPolygon::IsValid(void) const
{
    return (vec_edge_group_.size() > 0);
}

bool AnyPolygon::IsPointIn(const Point2D &point) const
{
    // // R_ASSERT(vec_edge_group_.size() > 0, false);

    if (!rect_area_.IsIn(point))
    {
        return false;
    }

    /* 根据计算从指定点开始沿x周正方向的射线与多边形相交的边的个数来判断是否在内部，
       个数为奇数表示在内部，个数为偶数表示在外部。 */
    /* 如果射线穿过顶点，该方法失效，判断出该场景后，尝试更换射线，如果多次后还是穿过顶点，返回false */
    static Point2D directed_vec[] = {{1.1, 0.0}, {0.0, -1.1}, {0.55, 0.5}, {0.55, -0.5}, {-1.1, 0.0}, {0.0, 1.1}, {-0.55, 0.5}, {-0.55, -0.5}};
    for (size_t loop = 0; loop < std::extent<decltype(directed_vec)>::value; ++loop)
    {
        Ray2D ray2d(point, {point.x + directed_vec[loop].x, point.y + directed_vec[loop].y});
        bool is_cross_vertex = false;
        bool is_in_polygon = false;

        for (const EdgeGroup &group : vec_edge_group_)
        {
            if (!ray2d.IsIntersectWithArea(group.area))
            {
                continue;
            }

            for (const DirectedLineSeg &edge : group.vec_edge)
            {
                if (ray2d.IsPointOnRay(edge.GetEndPoint()))
                {
                    is_cross_vertex = true;
                    break;
                }

                if (edge.IsIntersectedWithRay(ray2d))
                {
                    is_in_polygon = !is_in_polygon;
                }
            }

            if (is_cross_vertex)
            {
                break;
            }
        }

        if (!is_cross_vertex)
        {
            return is_in_polygon;
        }
    }

    return false;
}

bool AnyPolygon::IsIntersectWithLineSeg(const DirectedLineSeg &line_seg) const
{
    RectArea line_seg_rect_area;
    line_seg.GetRectArea(line_seg_rect_area);

    for (const EdgeGroup &edge_group : vec_edge_group_)
    {
        if (!edge_group.area.IsExistOverlapArea(line_seg_rect_area))
        {
            continue;
        }

        for (const DirectedLineSeg &edge_line_seg : edge_group.vec_edge)
        {
            if (edge_line_seg.IsIntersectedWithOtherLineSeg(line_seg))
            {
                return true;
            }
        }
    }

    return false;
}

bool AnyPolygon::IsIntersectWithOtherPolygon(const AnyPolygon &other) const
{
    // R_ASSERT(vec_edge_group_.size() > 0, false);
    // R_ASSERT(other.vec_edge_group_.size() > 0, false);

    /* 先group与group比较，再lineseg与group比较，最后再用lineseg和lineseg精确比较 */
    for (const EdgeGroup &other_edge_group : other.vec_edge_group_)
    {
        if (!other_edge_group.area.IsExistOverlapArea(rect_area_))
        {
            continue;
        }

        for (const EdgeGroup &local_edge_group : vec_edge_group_)
        {
            if (!local_edge_group.area.IsExistOverlapArea(other_edge_group.area))
            {
                continue;
            }

            for (const DirectedLineSeg &other_line_seg : other_edge_group.vec_edge)
            {
                RectArea objOtherLineSegArea;
                other_line_seg.GetRectArea(objOtherLineSegArea);
                if (!objOtherLineSegArea.IsExistOverlapArea(local_edge_group.area))
                {
                    continue;
                }

                for (const DirectedLineSeg &local_line_seg : local_edge_group.vec_edge)
                {
                    if (!local_line_seg.IsIntersectedWithOtherLineSeg(other_line_seg))
                    {
                        continue;
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

bool AnyPolygon::IsExistOverlapWithOtherPolygon(const AnyPolygon &other) const
{
    // R_ASSERT(vec_edge_group_.size() > 0, false);
    // R_ASSERT(other.vec_edge_group_.size() > 0, false);

    /* 判断两个多边形是否存在重叠 */
    /* 先判断两个多边形的边是否存在交叉，如果存在交叉则必然存在重叠 */
    if (IsIntersectWithOtherPolygon(other))
    {
        return true;
    }

    /* 再判断没有交叉时，还存在重叠，必然是一方被另一个全包围。所以任一顶点不在另一个多边形中则其不会被另一方包含。 */
    const EdgeGroup &loc_first_edge_group = this->vec_edge_group_.front();
    const DirectedLineSeg &loc_first_edge = loc_first_edge_group.vec_edge.front();
    const Point2D &loc_first_edge_end_point = loc_first_edge.GetEndPoint();
    if (other.IsPointIn(loc_first_edge_end_point))
    {
        return true;
    }

    const EdgeGroup &other_first_edge_group = other.vec_edge_group_.front();
    const DirectedLineSeg &other_first_edge = other_first_edge_group.vec_edge.front();
    const Point2D &other_first_edge_end_point = other_first_edge.GetEndPoint();
    if (IsPointIn(other_first_edge_end_point))
    {
        return true;
    }

    /* 即不交叉，一方又不被另一方包含，则不存在重叠 */
    return false;
}

bool AnyPolygon::IsCircleOut(const Point2D &center, float radius) const
{
    // R_ASSERT(vec_edge_group_.size() > 0, false);

    if (IsPointIn(center))
    {
        return false;
    }
    Circular curr_circle(center, radius);
    for (const EdgeGroup &group : vec_edge_group_)
    {
        for (const DirectedLineSeg &edge : group.vec_edge)
        {
            const Point2D &line_start_point = edge.GetStartPoint();
            const Point2D &line_end_point = edge.GetEndPoint();
            if (curr_circle.IsIntersectedLineSegment(line_start_point, line_end_point))
            {
                return false;
            }
        }
    }

    return true;
}

int AnyPolygon::Reset(const std::vector<Point2D> &vec_vertice)
{
    Clear();

    int rslt = MakePolygon(vec_vertice);
    if (rslt != 0)
    {
        Clear();
        // ST_LOG_ERR("生成任意多边形失败.");
        return -1;
    }

    rect_area_.Reset();
    for (const EdgeGroup &edge_group : vec_edge_group_)
    {
        rect_area_.IncludeArea(edge_group.area);
    }

    return 0;
}

int AnyPolygon::GetRectArea(RectArea &rect_area) const
{
    // R_ASSERT(IsValid(), -1);

    rect_area = rect_area_;
    return 0;
}

void AnyPolygon::DspAllVertice(const std::vector<Point2D> &vec_vertice) const
{
    unsigned int vertice_num = (unsigned int)(vec_vertice.size());
    // ST_LOG_INFO("Vertice顶点数量 = %u.", vertice_num);

    for (unsigned int loop = 0; loop < vertice_num; ++loop)
    {
        const Point2D &vertice = vec_vertice[loop];
        // ST_LOG_INFO("%3uth Verice x=%f, y=%f.", loop, vertice.x, vertice.y);
    }
}

void AnyPolygon::DivideEdgeGroup(const std::vector<Point2D> &vec_vertice, bool is_close,
                                 std::vector<EdgeGroup> &vec_group) const
{
    /* 对顶点分组进行各边是否相交判断，从而提高效率，按照估算判断次数来计算最优分组数，0.2为估算的组间重叠率，5为区域比较和相交比较效率比 */
    /* 边数较多时才需要进行分组，如果分组，组数必须大于等于5，每组必须多于3个 */
    unsigned int edge_num = is_close ? static_cast<unsigned int>(vec_vertice.size() - 1)
                                     : static_cast<unsigned int>(vec_vertice.size());
    unsigned int edge_group_num = (edge_num < 15) ? 1 : (unsigned int)(pow(double((0.5 + 0.2) * edge_num * edge_num * 5), 0.33333333));
    edge_group_num = (edge_group_num < 5) ? 1 : edge_group_num;
    unsigned int max_edge_num_in_group = edge_num / edge_group_num;
    max_edge_num_in_group = std::max(max_edge_num_in_group, 3U);

    vec_group.reserve(edge_num / max_edge_num_in_group + 1);

    const Point2D *last_vertice_ptr = &vec_vertice.back();
    unsigned int num_in_group = 0;
    EdgeGroup group;
    group.vec_edge.reserve(max_edge_num_in_group);
    group.area.IncludePoint(*last_vertice_ptr);

    for (unsigned int loop = is_close ? 1 : 0; loop < vec_vertice.size(); ++loop)
    {
        const Point2D &vertice = vec_vertice[loop];
        group.vec_edge.push_back(DirectedLineSeg(*last_vertice_ptr, vertice));
        group.area.IncludePoint(vertice);

        ++num_in_group;
        if (num_in_group >= max_edge_num_in_group)
        {
            num_in_group = 0;

            vec_group.push_back(group);

            group.vec_edge.clear();
            group.area.Reset();
            group.area.IncludePoint(vertice);
        }

        last_vertice_ptr = &vertice;
    }

    if (group.vec_edge.size() > 0)
    {
        vec_group.push_back(group);
    }
}

bool AnyPolygon::IsExistEdgesIntersectInGroup(const EdgeGroup &group, bool is_end_closed) const
{
    unsigned int edge_num = (unsigned int)(group.vec_edge.size());
    // R_ASSERT(edge_num > 0, false);

    unsigned int end_edge_idx = edge_num - 1;
    for (unsigned int loop = 0; loop <= end_edge_idx; ++loop)
    {
        const DirectedLineSeg &edge = group.vec_edge[loop];

        /* 处理相邻的，非最后一个或者封闭时的最后一个都要做相邻边判断，封闭时最后一条和第一条是相邻边 */
        if ((loop < end_edge_idx) || is_end_closed)
        {
            const DirectedLineSeg &next_edge = (loop < end_edge_idx) ? group.vec_edge[loop + 1] : group.vec_edge[0];
            if (edge.IsPointOnLine(next_edge.GetEndPoint()) &&
                (edge.IsPointOnLineSeg(next_edge.GetEndPoint()) || next_edge.IsPointOnLineSeg(edge.GetStartPoint())))
            {
                // ST_LOG_ERR("Neighbour Edge Overlap. Pre(%.2f, %.2f)-(%.2f, %.2f), Next(%.2f, %.2f)-(%.2f, %.2f).",
                //            edge.GetStartPoint().x, edge.GetStartPoint().y,
                //            edge.GetEndPoint().x, edge.GetEndPoint().y,
                //            next_edge.GetStartPoint().x, next_edge.GetStartPoint().y,
                //            next_edge.GetEndPoint().x, next_edge.GetEndPoint().y);
                return true;
            }
        }

        /* 处理非相邻的，首尾封闭时，第一个和最后一个不是非相邻 */
        unsigned int end_edge_idx_non_adjacent = (is_end_closed && (loop == 0)) ? (end_edge_idx - 1) : end_edge_idx;
        for (unsigned int loop_non_adjacent = loop + 2;
             loop_non_adjacent <= end_edge_idx_non_adjacent;
             ++loop_non_adjacent)
        {
            const DirectedLineSeg &non_adjacent_edge = group.vec_edge[loop_non_adjacent];
            if (edge.IsIntersectedWithOtherLineSeg(non_adjacent_edge))
            {
                // ST_LOG_ERR("NonAdjacent Edge Overlap. Pre(%.2f, %.2f)-(%.2f, %.2f), Next(%.2f, %.2f)-(%.2f, %.2f).",
                //            edge.GetStartPoint().x, edge.GetStartPoint().y,
                //            edge.GetEndPoint().x, edge.GetEndPoint().y,
                //            non_adjacent_edge.GetStartPoint().x, non_adjacent_edge.GetStartPoint().y,
                //            non_adjacent_edge.GetEndPoint().x, non_adjacent_edge.GetEndPoint().y);
                return true;
            }
        }
    }

    return false;
}

bool AnyPolygon::IsExistEdgesIntersectBetweenSeparatedGroup(const EdgeGroup &group1, const EdgeGroup &group2) const
{
    if (!group1.area.IsExistOverlapArea(group2.area))
    {
        return false;
    }

    for (const DirectedLineSeg &edge1 : group1.vec_edge)
    {
        for (const DirectedLineSeg &edge2 : group2.vec_edge)
        {
            if (edge1.IsIntersectedWithOtherLineSeg(edge2))
            {
                // ST_LOG_ERR("edge1(%.2f, %.2f)-(%.2f, %.2f), edge2(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
                //            edge1.GetStartPoint().x, edge1.GetStartPoint().y,
                //            edge1.GetEndPoint().x, edge1.GetEndPoint().y,
                //            edge2.GetStartPoint().x, edge2.GetStartPoint().y,
                //            edge2.GetEndPoint().x, edge2.GetEndPoint().y);
                return true;
            }
        }
    }

    return false;
}

bool AnyPolygon::IsExistEdgesIntersectBetweenConnectedGroup(const EdgeGroup &front_group,
                                                            const EdgeGroup &back_group,
                                                            bool is_end_closed) const
{
    unsigned int front_group_edge_num = (unsigned int)(front_group.vec_edge.size());
    unsigned int back_group_edge_num = (unsigned int)(back_group.vec_edge.size());
    // R_ASSERT((front_group_edge_num > 0) && (back_group_edge_num > 0), false);

    /* 前一个分组的最后一条和后一个分组的第一条为相邻边，需要判断是否共线重叠 */
    const DirectedLineSeg &front_group_back_edge = front_group.vec_edge.back();
    const DirectedLineSeg &back_group_back_edge = back_group.vec_edge.front();
    if (front_group_back_edge.IsPointOnLine(back_group_back_edge.GetEndPoint()) &&
        (front_group_back_edge.IsPointOnLineSeg(back_group_back_edge.GetEndPoint()) ||
         back_group_back_edge.IsPointOnLineSeg(front_group_back_edge.GetStartPoint())))
    {
        // ST_LOG_ERR("front_group_back_edge(%.2f, %.2f)-(%.2f, %.2f), back_group_back_edge(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
        //            front_group_back_edge.GetStartPoint().x, front_group_back_edge.GetStartPoint().y,
        //            front_group_back_edge.GetEndPoint().x, front_group_back_edge.GetEndPoint().y,
        //            back_group_back_edge.GetStartPoint().x, back_group_back_edge.GetStartPoint().y,
        //            back_group_back_edge.GetEndPoint().x, back_group_back_edge.GetEndPoint().y);
        return true;
    }

    /* 前一个分组的非最后一条和后一个分组进行比较 */
    for (unsigned int loop = 0; loop < (front_group_edge_num - 1); ++loop)
    {
        const DirectedLineSeg &front_group_edge = front_group.vec_edge[loop];

        for (const DirectedLineSeg &back_group_edge : back_group.vec_edge)
        {
            if (front_group_edge.IsIntersectedWithOtherLineSeg(back_group_edge))
            {
                // ST_LOG_ERR("front_group_edge(%.2f, %.2f)-(%.2f, %.2f), back_group_edge(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
                //            front_group_edge.GetStartPoint().x, front_group_edge.GetStartPoint().y,
                //            front_group_edge.GetEndPoint().x, front_group_edge.GetEndPoint().y,
                //            back_group_edge.GetStartPoint().x, back_group_edge.GetStartPoint().y,
                //            back_group_edge.GetEndPoint().x, back_group_edge.GetEndPoint().y);
                return true;
            }
        }
    }

    /* 前一个分组的最后一条和后一个分组的非最前一条进行比价 */
    for (unsigned int loop = 1; loop < back_group_edge_num; ++loop)
    {
        const DirectedLineSeg &back_group_edge = back_group.vec_edge[loop];
        if (front_group_back_edge.IsIntersectedWithOtherLineSeg(back_group_edge))
        {
            // ST_LOG_ERR("front_group_back_edge(%.2f, %.2f)-(%.2f, %.2f), back_group(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
            //            front_group_back_edge.GetStartPoint().x, front_group_back_edge.GetStartPoint().y,
            //            front_group_back_edge.GetEndPoint().x, front_group_back_edge.GetEndPoint().y,
            //            back_group_edge.GetStartPoint().x, back_group_edge.GetStartPoint().y,
            //            back_group_edge.GetEndPoint().x, back_group_edge.GetEndPoint().y);
            return true;
        }
    }

    return false;
}

bool AnyPolygon::IsExistEdgesIntersected(const std::vector<EdgeGroup> &vec_group) const
{
    unsigned int group_num = (unsigned int)(vec_group.size());
    // R_ASSERT(group_num > 0, false);

    /* 只有一组时，组需要检测组内相交 */
    if (group_num == 1)
    {
        return IsExistEdgesIntersectInGroup(vec_group[0], true);
    }

    /* 多组时，先检测组内相交，再检查组间相交 */
    /* 先检查每个分组内部是否存在相交 */
    for (const EdgeGroup &group : vec_group)
    {
        if (IsExistEdgesIntersectInGroup(group, false))
        {
            // ST_LOG_ERR("IsExistEdgesIntersectInGroup() True.");
            return true;
        }
    }

    /* 再检查分组间是否存在相交 */
    /* 对于只有两组，则首尾两两相连 */
    if (group_num == 2)
    {
        return IsExistEdgesIntersectBetweenConnectedGroup(vec_group[0], vec_group[1], true);
    }

    /* 对于多组，要考虑第一组和最后一组的相交情况 */
    unsigned int end_group_idx = group_num - 1;
    for (unsigned int loop = 0; loop <= end_group_idx; ++loop)
    {
        const EdgeGroup &group = vec_group[loop];

        /* 相邻组的 */
        const EdgeGroup &next_group = (loop < end_group_idx) ? vec_group[loop + 1] : vec_group[0];
        if (IsExistEdgesIntersectBetweenConnectedGroup(group, next_group, false))
        {
            // ST_LOG_ERR("IsExistEdgesIntersectBetweenConnectedGroup() True.");
            return true;
        }

        /* 非相邻的 */
        unsigned int endGroupIdxOfNonAdjacent = (loop == 0) ? (end_group_idx - 1) : end_group_idx;
        for (unsigned int loopOfCompare = loop + 2; loopOfCompare <= endGroupIdxOfNonAdjacent; ++loopOfCompare)
        {
            if (IsExistEdgesIntersectBetweenSeparatedGroup(group, vec_group[loopOfCompare]))
            {
                // ST_LOG_ERR("IsExistEdgesIntersectBetweenSeparatedGroup() True.");
                return true;
            }
        }
    }

    return false;
}

int AnyPolygon::MakePolygon(const std::vector<Point2D> &vec_vecrtice)
{
    const Point2D &first_vertice = vec_vecrtice.front();
    const Point2D &end_vertice = vec_vecrtice.back();
    bool is_close = IsNearZero(CalcDisSquare(first_vertice, end_vertice));
    unsigned int edge_num = is_close ? (unsigned int)(vec_vecrtice.size() - 1)
                                     : (unsigned int)(vec_vecrtice.size());
    if (edge_num < 3)
    {
        // ST_LOG_ERR("Edge num(%d) is not enough.", edge_num);
        DspAllVertice(vec_vecrtice);
        return -1;
    }

    /* 判断是否存在相交边时，为防止所有边都要进行两两比较，将边进行分组，并按照分组处理，减小比较次数，提高效率 */
    DivideEdgeGroup(vec_vecrtice, is_close, vec_edge_group_);

    if (IsExistEdgesIntersected(vec_edge_group_))
    {
        // ST_LOG_ERR("Exist intersected edges.");
        return -1;
    }

    return 0;
}

void AnyPolygon::Clear(void)
{
    vec_edge_group_.clear();
}

ConvexPolygon::ConvexPolygon(Point2D points[], int point_num)
{
    is_exist_first_vertice_ = false;
    last_vertice_ = {0.0f, 0.0f};

    int rslt = Make(points, point_num);
    if (rslt != 0)
    {
        // ST_LOG_ERR("Make Fail.");
    }
}

ConvexPolygon::ConvexPolygon(void)
{
    is_exist_first_vertice_ = false;
    last_vertice_ = {0.0f, 0.0f};
}

ConvexPolygon::~ConvexPolygon(void)
{
    Clear();
}

/**************************************************************************************
功能描述: 重新设置多边形
修改记录:
**************************************************************************************/
int ConvexPolygon::Reset(Point2D points[], int point_num)
{
    is_exist_first_vertice_ = false;
    last_vertice_ = {0.0f, 0.0f};

    Clear();

    return Make(points, point_num);
}

/**************************************************************************************
功能描述: 判断指定点是否在多边形内部
修改记录:
**************************************************************************************/
bool ConvexPolygon::IsPointIn(const Point2D &point) const
{
    if (directed_line_seg_list_.empty())
    {
        // ST_LOG_ERR("No Polygon.");
        return false;
    }

    for (std::list<DirectedLineSeg *>::const_iterator iter = directed_line_seg_list_.begin();
         iter != directed_line_seg_list_.end();
         iter++)
    {
        DirectedLineSeg *line_seg_ptr = *iter;
        if (!line_seg_ptr->IsPointOnLeft(point))
        {
            return false;
        }
    }

    return true;
}

/**************************************************************************************
功能描述: 获取多边形内点沿某个方向到多边形边的长度
          调用方保证给定点一定为内点
修改记录:
**************************************************************************************/
float ConvexPolygon::GetDistSquOfInternalPointAlongDirToEdge(const Point2D &internal_point, float angle)
{
    Ray2D ray2d(internal_point, angle);

    DirectedLineSeg *intersected_line_seg_ptr = GetLineSegIntersectedByInternalRay(ray2d);
    if (intersected_line_seg_ptr == nullptr)
    {
        // ST_LOG_ERR("Can Not Find Inersected Seg.");
        return -1.0;
    }

    Point2D intersection_point;
    if (0 != intersected_line_seg_ptr->GetIntersectionWithAnotherLine(ray2d, intersection_point))
    {
        // ST_LOG_ERR("Get Intersection Fail.");
        return -1.0;
    }

    return CalcDisSquare(internal_point, intersection_point);
}

/**************************************************************************************
功能描述: 获取指定内点到多边形最近的距离
          由调用方保证给定点为多边形内点
          返回值为负表示异常，计算失败
修改记录:
**************************************************************************************/
float ConvexPolygon::GetMinDistSquOfInternalPointToEdge(const Point2D &internal_point)
{
    float min_dist_squ = -1.0;

    for (std::list<DirectedLineSeg *>::iterator iter = directed_line_seg_list_.begin();
         iter != directed_line_seg_list_.end();
         iter++)
    {
        DirectedLineSeg *line_seg_ptr = *iter;
        float dist_squ = line_seg_ptr->GetDistSquToPoint(internal_point);
        if ((min_dist_squ < 0.0) || (dist_squ < min_dist_squ))
        {
            min_dist_squ = dist_squ;
        }
    }

    return min_dist_squ;
}

/**************************************************************************************
功能描述: 给多边形逆时针依次增加顶点
修改记录:
**************************************************************************************/
int ConvexPolygon::AddVertice(Point2D &point)
{
    if (!is_exist_first_vertice_)
    {
        last_vertice_ = point;
        is_exist_first_vertice_ = true;
        return 0;
    }

    /* 如果已存在边了，则需要校验下一个顶点是否合法，即在前一个边的左边，防止出现凹的，或不是顶点 */
    if (!directed_line_seg_list_.empty())
    {
        DirectedLineSeg *last_line_seg_ptr = directed_line_seg_list_.back();
        if (last_line_seg_ptr == nullptr)
        {
            // ST_LOG_ERR("Get Last Seg Fail. Seg Num = %u.", (unsigned int)(directed_line_seg_.size()));
            return -1;
        }

        if (!last_line_seg_ptr->IsPointOnLeft(point))
        {
            // ST_LOG_ERR("New Vertice Not No Left.");
            return -1;
        }
    }

    DirectedLineSeg *new_line_seg_ptr = new DirectedLineSeg(last_vertice_, point);
    if (new_line_seg_ptr == nullptr)
    {
        // ST_LOG_ERR("Alloc New Line Seg Fail.");
        return -1;
    }

    directed_line_seg_list_.push_back(new_line_seg_ptr);
    last_vertice_ = point;
    return 0;
}

/**************************************************************************************
功能描述: 检查该多边形是否合法
修改记录:
**************************************************************************************/
bool ConvexPolygon::Check(void)
{
    /* 至少是3条边 */
    if (directed_line_seg_list_.size() < 3)
    {
        // ST_LOG_ERR("Seg num(%u) is less.", (unsigned int)(directed_line_seg_.size()));
        return false;
    }

    /* 检查是否是封闭的 */
    DirectedLineSeg *first_line_seg = directed_line_seg_list_.front();
    DirectedLineSeg *last_line_seg_ptr = directed_line_seg_list_.back();
    const Point2D &first_vertice = first_line_seg->GetStartPoint();
    const Point2D &last_vertice = last_line_seg_ptr->GetEndPoint();
    if (!IsNearZero(CalcDisSquare(first_vertice, last_vertice)))
    {
        // ST_LOG_ERR("Not closed.");
        return false;
    }

    /* 检查是否是凸的，即判断所有点都不在所有边的右边 */
    for (std::list<DirectedLineSeg *>::iterator iter1 = directed_line_seg_list_.begin();
         iter1 != directed_line_seg_list_.end();
         iter1++)
    {
        DirectedLineSeg *line_seg_ptr1 = *iter1;
        for (std::list<DirectedLineSeg *>::iterator iter2 = directed_line_seg_list_.begin();
             iter2 != directed_line_seg_list_.end();
             iter2++)
        {
            DirectedLineSeg *line_seg_ptr2 = *iter2;
            const Point2D &line_seg_start_point = line_seg_ptr2->GetStartPoint();
            if (line_seg_ptr1->IsPointOnRight(line_seg_start_point))
            {
                // ST_LOG_ERR("Not Convex.");
                return false;
            }
        }
    }

    return true;
}

void ConvexPolygon::Clear(void)
{
    for (std::list<DirectedLineSeg *>::iterator iter = directed_line_seg_list_.begin();
         iter != directed_line_seg_list_.end();
         iter++)
    {
        delete *iter;
    }

    directed_line_seg_list_.clear();
}

/**************************************************************************************
功能描述: 构造凸多边形
          各个顶点逆时针顺序构成多边形
修改记录:
**************************************************************************************/
int ConvexPolygon::Make(Point2D points[], int point_num)
{
    // R_ASSERT_1(point_num >= 3, -1, point_num);

    for (int loop = 0; loop < point_num; loop++)
    {
        if (0 != AddVertice(points[loop]))
        {
            // ST_LOG_ERR("Add Vertice(Idx=%d) Fail.", loop);
            Clear();
            return -1;
        }
    }

    /* 如果最后一个点和第一个点不相等，则再增加第一个点，使多边形闭合 */
    if (!IsNearZero(CalcDisSquare(points[0], points[point_num - 1])))
    {
        if (0 != AddVertice(points[0]))
        {
            // ST_LOG_ERR("Add First Vertice Fail.");
            Clear();
            return -1;
        }
    }

    if (!Check())
    {
        // ST_LOG_ERR("Is Invalid.");
        Clear();
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取多边形内点沿某个方向与多边形的边相交，获取该边的线段
          调用方保证给定点一定为内点
修改记录:
**************************************************************************************/
DirectedLineSeg *ConvexPolygon::GetLineSegIntersectedByInternalRay(Ray2D &internal_ray)
{
    for (std::list<DirectedLineSeg *>::iterator iter = directed_line_seg_list_.begin();
         iter != directed_line_seg_list_.end();
         iter++)
    {
        DirectedLineSeg *line_seg_ptr = *iter;
        if (line_seg_ptr->IsIntersectedWithRay(internal_ray))
        {
            return line_seg_ptr;
        }
    }

    return nullptr;
}

/**************************************************************************************
功能描述: 判断是否为凸包
修改记录:
**************************************************************************************/
bool ConvexHull::IsConvexHull(const std::vector<Point2D> &points) const
{
    if (points.size() < 3)
        return false; // 至少需要3个顶点才能构成多边形
    if (points.size() == 3)
        return true; // 三角形一定为凸多边形

    int size = points.size();
    bool has_negative_turn = false;
    bool has_positive_turn = false;

    for (int i = 0; i < size; ++i)
    {
        int j = (i + 1) % size;
        int k = (i + 2) % size;

        /* 计算向量 points[i] -> points[j] 和 points[j] -> points[k] */
        double x1 = points[j].x - points[i].x;
        double y1 = points[j].y - points[i].y;
        double x2 = points[k].x - points[j].x;
        double y2 = points[k].y - points[j].y;

        /* 计算叉积 */
        double cross_product = x1 * y2 - y1 * x2;

        if (cross_product < 0)
        {
            if (has_positive_turn)
            {
                return false; // 如果已经检测到顺时针转向，则不是凸多边形
            }
            has_negative_turn = true;
        }
        else if (cross_product > 0)
        {
            if (has_negative_turn)
            {
                return false; // 如果已经检测到逆时针转向，则不是凸多边形
            }
            has_positive_turn = true;
        }
        // crossProduct == 0 的情况表示共线，在凸多边形中允许这种情况
    }

    /**
     * 如果没有检测到顺时针和逆时针的转向，则认为是凸多边形
     * 或者所有转向都是同向的（共线情况），也认为是凸多边形
     */
    return true;
}

/**************************************************************************************
功能描述: 计算凸包
修改记录:
**************************************************************************************/
std::vector<Point2D> ConvexHull::GetConvexHull(const std::vector<Point2D> &points) const
{
    std::vector<Point2D> vec_point;
    vec_point.reserve(points.size());
    for (const Point2D &point : points)
    {
        vec_point.emplace_back(point);
    }

    std::vector<Point2D> vec_point_rslt = ConvexHullTurn(vec_point);
    std::vector<Point2D> vec_coor;
    vec_coor.reserve(vec_point_rslt.size());
    for (const Point2D &point : vec_point_rslt)
    {
        vec_coor.emplace_back(point);
    }
    return vec_coor;
}

/**************************************************************************************
功能描述: 比较函数，用于极角排序
修改记录:
**************************************************************************************/
bool ConvexHull::Compare(const Point2D &a, const Point2D &b, const Point2D &origin) const
{
    double cross = (a.x - origin.x) * (b.y - origin.y) - (a.y - origin.y) * (b.x - origin.x);
    if (Sign(cross) == 0)
    {
        return CalcDisSquare(origin, a) < CalcDisSquare(origin, b); //处理共线情况
    }
    return cross > 0;
}

/**************************************************************************************
功能描述: 计算凸包（转成双精度进行计算）
修改记录:
**************************************************************************************/
std::vector<Point2D> ConvexHull::ConvexHullTurn(std::vector<Point2D> &points) const
{
    size_t n = points.size();
    if (n <= 3)
    {
        return points; // 凸包就是所有点
    }

    /* 找到最左下方的点 */
    int minIndex = 0;
    for (size_t i = 1; i < n; ++i)
    {
        if (points[i].y < points[minIndex].y ||
            (points[i].y == points[minIndex].y && points[i].x < points[minIndex].x))
        {
            minIndex = i;
        }
    }
    std::swap(points[0], points[minIndex]); // 将最左下方的点放到第一个位置

    /* 极角排序 */
    std::sort(points.begin() + 1, points.end(), [&](const Point2D &a, const Point2D &b)
              { return Compare(a, b, points[0]); });

    /* 构建凸包 */
    std::vector<Point2D> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    for (size_t i = 2; i < n; ++i)
    {
        while (hull.size() >= 2 && !Compare(points[i], hull[hull.size() - 2], hull[hull.size() - 1]))
            hull.pop_back();
        if (Sign(hull.back().x - points[i].x) == 0 && Sign(hull.back().y - points[i].y) == 0)
        {
            continue; // 出现重合的点
        }
        hull.push_back(points[i]);
    }

    /* 输出凸包 */
    return hull;
}

/**************************************************************************************
功能描述: 根据三角形的两条边和夹角求第三条边长的平方
修改记录:
**************************************************************************************/
float Triangle::Get3thSideLenSqu(const float &side_len1, const float &side_len2, const float &angle_2_side)
{
    return (side_len1 * side_len1 + side_len2 * side_len2 - 2.0 * side_len1 * side_len2 * std::cos(angle_2_side));
}

Circular::Circular(const Point2D &center, float radius)
{
    // R_ASSERT(radius > 0.0, NOTHING);

    center_ = center;
    radius_ = radius;
    radius_squ_ = radius * radius;
    is_exist_radius_ = true;
    is_valid_ = true;
}

Circular::Circular(const Point2D &point1, const Point2D &point2, const Point2D &point3)
{
    /* 三点不能太近 */
    Point2D vector12 = {point2.x - point1.x, point2.y - point1.y};
    Point2D vector23 = {point3.x - point2.x, point3.y - point2.y};
    Point2D vector13 = {point3.x - point1.x, point3.y - point1.y};
    if ((vector12.x * vector12.x + vector12.y * vector12.y < 1.0e-10) ||
        (vector23.x * vector23.x + vector23.y * vector23.y < 1.0e-10) ||
        (vector13.x * vector13.x + vector13.y * vector13.y < 1.0e-10))
    {
        return;
    }

    /* 三点不能共线 */
    float det = vector12.x * vector23.y - vector12.y * vector23.x;
    if (std::fabs(det) < 1.0e-10)
    {
        return;
    }

    /* 计算圆心 */
    float e = ((point2.x * point2.x - point1.x * point1.x) + (point2.y * point2.y - point1.y * point1.y)) * 0.5;
    float f = ((point2.x * point2.x - point3.x * point3.x) + (point2.y * point2.y - point3.y * point3.y)) * 0.5;
    float det_r = 1.0 / det;
    center_.x = (vector23.y * e + vector12.y * f) * det_r;
    center_.y = -(vector23.x * e + vector12.x * f) * det_r;

    /* 计算半径 */
    radius_squ_ = CalcDisSquare(center_, point1);

    is_exist_radius_ = false;
    is_valid_ = true;
}

bool Circular::IsValid(void) const
{
    return is_valid_;
}

int Circular::GetCenter(Point2D &center) const
{
    // R_ASSERT(is_valid_, -1);

    center = center_;
    return 0;
}

float Circular::GetRadiusSqu(void) const
{
    // R_ASSERT(is_valid_, -1);
    return radius_squ_;
}

float Circular::GetRadius(void)
{
    // R_ASSERT(is_valid_, -1);

    if (!is_exist_radius_)
    {
        radius_ = sqrt(radius_squ_);
        is_exist_radius_ = true;
    }

    return radius_;
}

int Circular::GetSamplePoints(const Point2D &start_point, const Point2D &end_point,
                              float delta_rad_abs, std::vector<Point2D> &vec_sample_points)
{
    // R_ASSERT(is_valid_, -1);

    Vec2D vector_s(center_, start_point);
    Vec2D vector_e(center_, end_point);

    float start_angle = vector_s.GetDirAngle();
    float end_angle = vector_e.GetDirAngle();
    float diff_angle = WrapAngleRad(end_angle - start_angle);
    float symbol = (diff_angle >= 0) ? 1.0 : -1.0;
    int sample_num = int(std::abs(diff_angle) / delta_rad_abs);
    float sin_start = sin(start_angle);
    float cos_start = cos(start_angle);
    float sin_delta = sin(delta_rad_abs);
    float cos_delta = cos(delta_rad_abs);
    float radius = GetRadius();

    vec_sample_points.reserve(sample_num + 1);

    float sin_sample = sin_start;
    float cos_sample = cos_start;
    for (int loop = 1; loop <= sample_num; ++loop)
    {
        float sin_sample_pre = sin_sample;
        float cos_sample_pre = cos_sample;
        sin_sample = sin_sample_pre * cos_delta + cos_sample_pre * sin_delta * symbol;
        cos_sample = cos_sample_pre * cos_delta - sin_sample_pre * sin_delta * symbol;

        Point2D sample_point;
        sample_point.x = center_.x + radius * cos_sample;
        sample_point.y = center_.y + radius * sin_sample;

        vec_sample_points.push_back(sample_point);
    }

    vec_sample_points.push_back(end_point);
    return 0;
}

bool Circular::IsPointIn(const Point2D &point) const
{
    if (is_valid_)
    {
        if ((point.x - center_.x) * (point.x - center_.x) +
                (point.y - center_.y) * (point.y - center_.y) - radius_squ_ <=
            0.001)
        {
            return true;
        }
    }
    return false;
}

bool Circular::IsIntersectedLineSegment(const Point2D &start_point,
                                        const Point2D &end_point) const
{
    if (!is_valid_)
    {
        return false;
    }
    if (IsPointIn(start_point) && IsPointIn(end_point))
    {
        return false;
    }
    if ((!IsPointIn(start_point) && IsPointIn(end_point)) ||
        (IsPointIn(start_point) && !IsPointIn(end_point)))
    {
        return true;
    }

    double a, b, c, dist1, dist2, angle1, angle2; // ax + by + c = 0;
    if (std::fabs(start_point.x - end_point.x) < 0.001)
    {
        a = 1;
        b = 0;
        c = -start_point.x;
    }
    else if (std::fabs(start_point.y - end_point.y) < 0.001)
    {
        a = 0;
        b = 1;
        c = -start_point.y;
    }
    else
    {
        a = start_point.y - end_point.y;
        b = end_point.x - start_point.x;
        c = start_point.x * end_point.y - start_point.y * end_point.x;
    }
    dist1 = a * center_.x + b * center_.y + c;
    dist1 *= dist1;
    dist2 = (a * a + b * b) * radius_squ_;
    if (dist1 > dist2)
    {
        return false;
    }
    angle1 = (center_.x - start_point.x) * (end_point.x - start_point.x) +
             (center_.y - start_point.y) * (end_point.y - start_point.y);
    angle2 = (center_.x - end_point.x) * (start_point.x - end_point.x) +
             (center_.y - end_point.y) * (start_point.y - end_point.y);
    if (angle1 > 0 && angle2 > 0)
    {
        return true; // 余弦都为正，则是锐角
    }
    return false;
}

bool Sector::IsPointIn(const Point2D &point)
{
    if ((radius_ > 0) && CalcDisSquare(center_, point) > (radius_ * radius_))
    {
        return false;
    }

    if (is_angle_range_positive_)
    {
        if (is_angle_range_over_pi_)
        {
            return begin_angle_line_.IsPointOnLeft(point) || end_angle_line_.IsPointOnRight(point);
        }
        else
        {
            return begin_angle_line_.IsPointOnLeft(point) && end_angle_line_.IsPointOnRight(point);
        }
    }
    else
    {
        if (is_angle_range_over_pi_)
        {
            return begin_angle_line_.IsPointOnRight(point) || end_angle_line_.IsPointOnLeft(point);
        }
        else
        {
            return begin_angle_line_.IsPointOnRight(point) && end_angle_line_.IsPointOnLeft(point);
        }
    }
}
