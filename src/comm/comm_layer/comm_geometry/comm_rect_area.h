
#ifndef COMM_RECT_AREA_H
#define COMM_RECT_AREA_H

#include <algorithm>
#include <vector>
//#include "comm_coordinate.h"
#include "comm_layer/comm_base/comm_type.h"


/**************************************************************************************
功能描述: 矩形区域
修改记录:
**************************************************************************************/
class RectArea
{
public:
    RectArea(void)
    {
        Reset();
    }

    void Reset(void)
    {
        is_init_ = false;
        left_back_point_ = {0.0f, 0.0f};
        right_top_point_ = {0.0f, 0.0f};
    }

    bool IsValid(void) const
    {
        return is_init_;
    }

    float GetMeasure(void) const
    {
        return is_init_ ? ((right_top_point_.x - left_back_point_.x) * (right_top_point_.y - left_back_point_.y)) : 0.0;
    }

    float GetSizeX(void) const
    {
        return is_init_ ? (right_top_point_.x - left_back_point_.x) : 0.0;
    }

    float GetSizeY(void) const
    {
        return is_init_ ? (right_top_point_.y - left_back_point_.y) : 0.0;
    }

    Point2D LeftBackPoint() const
    {
        return left_back_point_;
    }

    Point2D RightTopPoint() const
    {
        return right_top_point_;
    }

    void IncludePoint(const Point2D &point)
    {
        if (!is_init_)
        {
            is_init_ = true;
            left_back_point_ = point;
            right_top_point_ = point;
        }
        else
        {
            left_back_point_.x = std::min(left_back_point_.x, point.x);
            left_back_point_.y = std::min(left_back_point_.y, point.y);
            right_top_point_.x = std::max(right_top_point_.x, point.x);
            right_top_point_.y = std::max(right_top_point_.y, point.y);
        }
    }

    void IncludePoints(const std::vector<Point2D> &vec_point)
    {
        for (const Point2D &point : vec_point)
        {
            IncludePoint(point);
        }
    }

    void IncludePoint(float point_x, float point_y)
    {
        Point2D point = {point_x, point_y};

        IncludePoint(point);
    }

    void IncludeArea(const RectArea &other_area)
    {
        if (!other_area.is_init_)
        {
            return;
        }

        IncludePoint(other_area.left_back_point_);
        IncludePoint(other_area.right_top_point_);
    }

    void Expand(float expand_x, float expand_y)
    {
        if ((expand_x < 0) || (expand_y < 0))
        {
            return;
        }

        if (is_init_)
        {
            left_back_point_.x -= expand_x;
            left_back_point_.y -= expand_y;

            right_top_point_.x += expand_x;
            right_top_point_.y += expand_y;
        }
    }

    void Expand(float expand_dist)
    {
        Expand(expand_dist, expand_dist);
    }

    bool IsIn(const Point2D &point) const
    {
        return (is_init_
                && (point.x >= left_back_point_.x) && (point.y >= left_back_point_.y)
                && (point.x <= right_top_point_.x) && (point.y <= right_top_point_.y));
    }

    bool IsExistOverlapArea(const RectArea &other_area) const
    {
        /* 重叠一个点也算重叠 */
        if (!is_init_ || !other_area.is_init_)
        {
            return false;
        }

        return !(   (other_area.right_top_point_.x < this->left_back_point_.x)
                 || (other_area.right_top_point_.y < this->left_back_point_.y)
                 || (other_area.left_back_point_.x > this->right_top_point_.x)
                 || (other_area.left_back_point_.y > this->right_top_point_.y));
    }

    /* 获取与另一个区域的重叠区域 */
    void GetOverlapArea(const RectArea &other_area, RectArea &overlap_area) const
    {
        if (!this->is_init_ || !other_area.is_init_)
        {
            overlap_area.Reset();
        }
        else
        {
            overlap_area.left_back_point_.x = std::max(this->left_back_point_.x, other_area.left_back_point_.x);
            overlap_area.left_back_point_.y = std::max(this->left_back_point_.y, other_area.left_back_point_.y);
            overlap_area.right_top_point_.x = std::min(this->right_top_point_.x, other_area.right_top_point_.x);
            overlap_area.right_top_point_.y = std::min(this->right_top_point_.y, other_area.right_top_point_.y);
            if ((overlap_area.left_back_point_.x <= overlap_area.right_top_point_.x) &&
                (overlap_area.left_back_point_.y <= overlap_area.right_top_point_.y))
            {
                overlap_area.is_init_ = true;
            }
        }
    }

    /* 获取与另一个区域的非重叠区域 */
    void GetNonOverlapAreas(const RectArea &other_area, std::vector<RectArea> &vec_area) const
    {
        if (!this->is_init_)
        {
            return;
        }

        RectArea overlap_area;
        this->GetOverlapArea(other_area, overlap_area);
        if (!overlap_area.IsValid())
        {
            vec_area.push_back(*this);
            return;
        }

        if (overlap_area.left_back_point_.x > this->left_back_point_.x)
        {
            RectArea area;
            area.IncludePoint(this->left_back_point_);
            area.IncludePoint(overlap_area.left_back_point_.x, this->right_top_point_.y);
            vec_area.push_back(area);
        }

        if (overlap_area.right_top_point_.x < this->right_top_point_.x)
        {
            RectArea area;
            area.IncludePoint(this->right_top_point_);
            area.IncludePoint(overlap_area.right_top_point_.x, this->left_back_point_.y);
            vec_area.push_back(area);
        }

        if (overlap_area.left_back_point_.y > this->left_back_point_.y)
        {
            RectArea area;
            area.IncludePoint(overlap_area.left_back_point_.x, this->left_back_point_.y);
            area.IncludePoint(overlap_area.right_top_point_.x, overlap_area.left_back_point_.y);
            vec_area.push_back(area);
        }

        if (overlap_area.right_top_point_.y < this->right_top_point_.y)
        {
            RectArea area;
            area.IncludePoint(overlap_area.left_back_point_.x, overlap_area.right_top_point_.y);
            area.IncludePoint(overlap_area.right_top_point_.x, this->right_top_point_.y);
            vec_area.push_back(area);
        }
    }

    int GetLTPoint(Point2D &point) const
    {
        if (!IsValid())
        {
            return -1;
        }

        point.x = left_back_point_.x;
        point.y = right_top_point_.y;
        return 0;
    }

    int GetRBPoint(Point2D &point) const
    {
        if (!IsValid())
        {
            return -1;
        }

        point.x = right_top_point_.x;
        point.y = left_back_point_.y;
        return 0;
    }

    int GetBorder(std::vector<Point2D> &vec_border_point) const
    {
        if (!IsValid())
        {
            return -1;
        }

        vec_border_point.push_back(left_back_point_);
        vec_border_point.push_back({left_back_point_.x, right_top_point_.y});
        vec_border_point.push_back(right_top_point_);
        vec_border_point.push_back({right_top_point_.x, left_back_point_.y});
        return 0;
    }

private:
    bool is_init_;
    Point2D left_back_point_;   // 左下角坐标点
    Point2D right_top_point_;   // 右上角坐标点
};


#endif   // COMM_RECT_AREA_H

