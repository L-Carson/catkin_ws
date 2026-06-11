#ifndef COLLISION_CAR_H
#define COLLISION_CAR_H

#include <vector>
#include <string>
#include <unordered_set>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_geometry/comm_geometry.h"
#include "base_layer/vehicle_model/dynamic_car.h"
#include "base_layer/comm_math/comm_math.h"


class CollisionCar : public DynamicCar {
public: 
    CollisionCar(void);
     ~CollisionCar()
    {
    }

    // 刷盘模型是否和障碍点碰撞
    bool IsBrushCollide(const Point2D &obst);
    bool IsBrushCollide(const AnyPolygon &obst_area);
    bool IsBrushCollide(const DirectedLineSeg &obst_line_seg);

    // 刷毛模型是否和障碍点碰撞
    bool IsBrushFeatherCollide(const Point2D &obst);

    // 整体模型(刷盘和车体组成的立方体)是否和障碍点碰撞
    bool IsWholeCollide(const Point2D &obst);
    bool IsWholeCollide(const AnyPolygon &obst_area);
    bool IsWholeCollide(const DirectedLineSeg &obst_line_seg);

    // 车身是否和障碍碰撞
    bool IsCarBodyCollide(const Point2D &obst);
    bool IsCarBodyCollide(const AnyPolygon &obst_area);
    bool IsCarBodyCollide(const DirectedLineSeg &obst_line_seg);

    // 判断是否和障碍碰撞
    bool IsCollide(const Point2D &obst);
    bool IsCollide(const AnyPolygon &obst_area);
    bool IsCollide(const DirectedLineSeg &obst_line_seg);

    // 获取车体模型碰撞的5个区域
    void GetCollisionRegion(std::vector<CollideRegion> &collide_region_vec);

};


#endif // COLLISION_CAR_H

