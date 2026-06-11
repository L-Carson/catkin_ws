#include "base_layer/vehicle_model/collision_car.h"

#include "comm_layer/comm_math/comm_math.h"


CollisionCar::CollisionCar(void)
{ 
}

bool CollisionCar::IsBrushCollide(const Point2D &obst)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerBrushByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsPointIn(obst))
    {
        return true;
    }
    return false;
}

bool CollisionCar::IsBrushCollide(const AnyPolygon &obst_area)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerBrushByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsExistOverlapWithOtherPolygon(obst_area))
    {
        return true;
    }
    return false;
}


bool CollisionCar::IsBrushCollide(const DirectedLineSeg &obst_line_seg)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerBrushByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsIntersectWithLineSeg(obst_line_seg))
    {
        return true;
    }
    return false;
}

bool CollisionCar::IsBrushFeatherCollide(const Point2D &obst)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerBrushFeatherByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsPointIn(obst))
    {
        return true;
    }
    return false;
}

bool CollisionCar::IsWholeCollide(const Point2D &obst)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerWholeByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsPointIn(obst))
    {
        return true;
    }
    return false;
}


bool CollisionCar::IsWholeCollide(const AnyPolygon &obst_area)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerWholeByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsIntersectWithOtherPolygon(obst_area))
    {
        return true;
    }
    return false;
}


bool CollisionCar::IsWholeCollide(const DirectedLineSeg &obst_line_seg)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerWholeByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsIntersectWithLineSeg(obst_line_seg))
    {
        return true;
    }
    return false;
}


bool CollisionCar::IsCarBodyCollide(const Point2D &obst)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerCarBodyByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsPointIn(obst))
    {
        return true;
    }
    return false;
}


bool CollisionCar::IsCarBodyCollide(const AnyPolygon &obst_area)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerCarBodyByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsExistOverlapWithOtherPolygon(obst_area))
    {
        return true;
    }
    return false;
}


bool CollisionCar::IsCarBodyCollide(const DirectedLineSeg &obst_line_seg)
{
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    Get4CornerCarBodyByCar(left_head_point, left_back_point, right_back_point, right_head_point);
    std::vector<Point2D> points{left_head_point, left_back_point, right_back_point, right_head_point};
    AnyPolygon polygon(points);
    if (polygon.IsIntersectWithLineSeg(obst_line_seg))
    {
        return true;
    }
    return false;
}

bool CollisionCar::IsCollide(const Point2D &obst)
{
    if (IsWholeCollide(obst))
    {
        if (IsBrushCollide(obst) || IsCarBodyCollide(obst))
        {
            return true;
        }
    }
    return false;
}


bool CollisionCar::IsCollide(const AnyPolygon &obst_area)
{
    if (IsWholeCollide(obst_area))
    {
        if (IsBrushCollide(obst_area) || IsCarBodyCollide(obst_area))
        {
            return true;
        }
    }
    return false;
}


bool CollisionCar::IsCollide(const DirectedLineSeg &obst_line_seg)
{
    if (IsWholeCollide(obst_line_seg))
    {
        if (IsBrushCollide(obst_line_seg) || IsCarBodyCollide(obst_line_seg))
        {
            return true;
        }
    }
    return false;
}


// 获取扫刷结构和车体模型的9个区域是否有障碍，如果有则输出该区域
void CollisionCar::GetCollisionRegion(std::vector<CollideRegion> &collide_region_vec)
{
    collide_region_vec.resize(5);
    
    float car_x_min = -car_body_base_param_.tail_to_rear_axle_lenght;
    float car_x_max = car_body_base_param_.ego_length - car_body_base_param_.tail_to_rear_axle_lenght;
    float car_x_mid1 = std::min(car_body_base_param_.wheel_base_length, car_x_max-0.1);
    float car_x_mid2 = 0.0;
    float car_y_min = -car_body_base_param_.ego_width / 2.0;
    float car_y_max = car_body_base_param_.ego_width / 2.0;
    float car_y_mid1 = car_y_max - 0.1;
    float car_y_mid2 = car_y_min + 0.1;

    CollideRegion head_region;
    head_region.region_type = CollideRegionTypeEnum::CAR_HEAD;
    head_region.vectexs.emplace_back(Point2D(car_x_max, car_y_mid1));
    head_region.vectexs.emplace_back(Point2D(car_x_max, car_y_mid2));
    head_region.vectexs.emplace_back(Point2D(car_x_mid1, car_y_mid2));
    head_region.vectexs.emplace_back(Point2D(car_x_mid1, car_y_mid1));
    collide_region_vec.emplace_back(head_region);

    CollideRegion left_region;
    left_region.region_type = CollideRegionTypeEnum::CAR_LEFT;
    left_region.vectexs.emplace_back(Point2D(car_x_max, car_y_max));
    left_region.vectexs.emplace_back(Point2D(car_x_max, car_y_mid1));
    left_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_mid1));
    left_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_max));
    collide_region_vec.emplace_back(left_region);

    CollideRegion right_region;
    right_region.region_type = CollideRegionTypeEnum::CAR_RIGHT;
    right_region.vectexs.emplace_back(Point2D(car_x_max, car_y_mid2));
    right_region.vectexs.emplace_back(Point2D(car_x_max, car_y_min));
    right_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_min));
    right_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_mid2));
    collide_region_vec.emplace_back(right_region);

    CollideRegion center_region;
    center_region.region_type = CollideRegionTypeEnum::CAR_CENTER;
    center_region.vectexs.emplace_back(Point2D(car_x_mid1, car_y_mid1));
    center_region.vectexs.emplace_back(Point2D(car_x_mid1, car_y_mid2));
    center_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_mid2));
    center_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_mid1));
    collide_region_vec.emplace_back(center_region);

    CollideRegion back_region;
    back_region.region_type = CollideRegionTypeEnum::CAR_BACK;
    back_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_max));
    back_region.vectexs.emplace_back(Point2D(car_x_mid2, car_y_min));
    back_region.vectexs.emplace_back(Point2D(car_x_min, car_y_min));
    back_region.vectexs.emplace_back(Point2D(car_x_min, car_y_max));
    collide_region_vec.emplace_back(back_region);

}