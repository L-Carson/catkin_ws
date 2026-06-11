#include "base_layer/vehicle_model/dynamic_car.h"

#include <unordered_set>
#include "comm_layer/comm_math/comm_math.h"

DynamicCar::DynamicCar(void)
{
}

DynamicCar::SetBrushTheta(const float &theta)
{
    brush_theta_  = theta;
}

// 获取左刷盘的圆柱形模型(车身坐标系)
const BrushDiskCylinder DynamicCar::GetLeftDiskCylinderByCar(void) const 
{
    float brush_length, brush_width;
    GetBrushBodyParam(brush_length, brush_width);
    BrushDiskCylinder brush_disk_cylinder;
    brush_disk_cylinder.center_pose.x = car_body_base_param_.ego_length - 
                                        car_body_base_param_.tail_to_rear_axle_lenght + 
                                        brush_length -
                                        car_body_base_param_.brush_disk_rudia;
    brush_disk_cylinder.center_pose.y = brush_width / 2.0;
    brush_disk_cylinder.height        = car_body_base_param_.brush_disk_height;
    brush_disk_cylinder.rudia         = car_body_base_param_.brush_disk_rudia;
    return brush_disk_cylinder;
}

// 获取左刷盘的圆柱形模型(全局地图坐标系)
const BrushDiskCylinder DynamicCar::GetLeftDiskCylinderByMap(void) const 
{
    BrushDiskCylinder cylinder_by_car = GetLeftDiskCylinderByCar();
    Pose2D center_by_car, center_by_map;
    center_by_car.x = cylinder_by_car.center_pose.x;
    center_by_car.y = cylinder_by_car.center_pose.y;
    center_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, center_by_car, center_by_map);
    BrushDiskCylinder cylinder_by_map;
    cylinder_by_map.center_pose.x = center_by_map.x;
    cylinder_by_map.center_pose.y = center_by_map.y;
    cylinder_by_map.height = cylinder_by_car.height;
    cylinder_by_map.rudia = cylinder_by_car.rudia;
    return cylinder_by_map;
}

// 获取右刷盘的圆柱形模型(车身坐标系)
const BrushDiskCylinder DynamicCar::GetRightDiskCylinderByCar(void) const 
{
    float brush_length, brush_width;
    GetBrushBodyParam(brush_length, brush_width);
    BrushDiskCylinder brush_disk_cylinder;
    brush_disk_cylinder.center_pose.x = car_body_base_param_.ego_length - 
                                        car_body_base_param_.tail_to_rear_axle_lenght + 
                                        brush_length -
                                        car_body_base_param_.brush_disk_rudia;
    brush_disk_cylinder.center_pose.y = -brush_width / 2.0;
    brush_disk_cylinder.height        = car_body_base_param_.brush_disk_height;
    brush_disk_cylinder.rudia         = car_body_base_param_.brush_disk_rudia;
    return brush_disk_cylinder;
}

// 获取右刷盘的圆柱形模型(全局地图坐标系)
const BrushDiskCylinder DynamicCar::GetRightDiskCylinderByMap(void) const 
{
    BrushDiskCylinder cylinder_by_car = GetRightDiskCylinderByCar();
    Pose2D center_by_car, center_by_map;
    center_by_car.x = cylinder_by_car.center_pose.x;
    center_by_car.y = cylinder_by_car.center_pose.y;
    center_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, center_by_car, center_by_map);
    BrushDiskCylinder cylinder_by_map;
    cylinder_by_map.center_pose.x = center_by_map.x;
    cylinder_by_map.center_pose.y = center_by_map.y;
    cylinder_by_map.height = cylinder_by_car.height;
    cylinder_by_map.rudia = cylinder_by_car.rudia;
    return cylinder_by_map;
}

CarPartsCube DynamicCar::GetBrushCubeByCar(void) const {
    float brush_length, brush_width;
    GetBrushBodyParam(brush_length, brush_width);
    CarPartsCube brush_body_cube;
    brush_body_cube.center_pose.x = car_body_base_param_.ego_length - 
                                    car_body_base_param_.tail_to_rear_axle_lenght + 
                                    brush_length / 2.0;
    brush_body_cube.center_pose.y = 0.0;
    brush_body_cube.cube.length = brush_length;
    brush_body_cube.cube.width  = brush_width - 2 * car_body_base_param_.brush_disk_rudia;
    brush_body_cube.cube.height = car_body_base_param_.brush_body_height;
    return brush_body_cube;
}


CarPartsCube DynamicCar::GetBrushCubeByCar(void) const {
    float brush_length, brush_width;
    GetBrushBodyParam(brush_length, brush_width);
    CarPartsCube brush_disk_cube;
    brush_disk_cube.center_pose.x = car_body_base_param_.ego_length - 
                                    car_body_base_param_.tail_to_rear_axle_lenght + 
                                    brush_length / 2.0;
    brush_disk_cube.center_pose.y = 0.0;
    brush_disk_cube.cube.length = brush_length;
    brush_disk_cube.cube.width  = brush_width;
    brush_disk_cube.cube.height = car_body_base_param_.brush_body_height;
    return brush_disk_cube;
}


CarPartsCube DynamicCar::GetBrushFeatherCubeByCar(void) const {
    float brush_length, brush_width;
    GetBrushBodyParam(brush_length, brush_width);
    CarPartsCube brush_feather_cube;
    brush_feather_cube.center_pose.x = car_body_base_param_.ego_length - 
                                    car_body_base_param_.tail_to_rear_axle_lenght + 
                                    brush_length / 2.0;
    brush_feather_cube.center_pose.y = 0.0;
    brush_feather_cube.cube.length = brush_length;
    brush_feather_cube.cube.width  = brush_width + car_body_base_param_.brush_feather_length * 2.0;
    brush_feather_cube.cube.height = car_body_base_param_.brush_body_height;
    return brush_feather_cube;
}

// 获取收起的扫刷结构模型四个角点(含刷盘半径， 车身坐标系)
const void DynamicCar::Get4CornerBrushByCar(Point2D &left_head_point, Point2D &left_back_point,
                                               Point2D &right_back_point, Point2D &right_head_point) const
{
    CarPartsCube brush_disk_cube = GetBrushCubeByCar();
    left_head_point.x = brush_disk_cube.center_pose.x + brush_disk_cube.cube.length / 2.0;
    left_head_point.y = brush_disk_cube.center_pose.y + brush_disk_cube.cube.width / 2.0;
    left_back_point.x = brush_disk_cube.center_pose.x - brush_disk_cube.cube.length / 2.0;
    left_back_point.y = brush_disk_cube.center_pose.y + brush_disk_cube.cube.width / 2.0;
    right_back_point.x = brush_disk_cube.center_pose.x - brush_disk_cube.cube.length / 2.0;
    right_back_point.y = brush_disk_cube.center_pose.y - brush_disk_cube.cube.width / 2.0;
    right_head_point.x = brush_disk_cube.center_pose.x + brush_disk_cube.cube.length / 2.0;
    right_head_point.y = brush_disk_cube.center_pose.y - brush_disk_cube.cube.width / 2.0;
}


// 获取收起的扫刷结构模型四个角点(含刷盘半径， 全局地图坐标系)
const void DynamicCar::Get4CornerBrushByMap(Point2D &left_head_point, Point2D &left_back_point,
                                               Point2D &right_back_point, Point2D &right_head_point) const
{
    Point2D left_head_point_by_car,  left_back_point_by_car;
    Point2D right_back_point_by_car,  right_head_point_by_car;
    Get4CornerBrushByCar(left_head_point_by_car, left_back_point_by_car,
                         right_back_point_by_car, right_head_point_by_car);

    Pose2D left_head_pose_by_car, left_head_pose_by_map;
    left_head_pose_by_car.x = left_head_point_by_car.x;
    left_head_pose_by_car.y = left_head_point_by_car.y;
    left_head_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, left_head_pose_by_car, left_head_pose_by_map);
    left_head_point.x = left_head_pose_by_map.x;
    left_head_point.y = left_head_pose_by_map.y;

    Pose2D left_back_pose_by_car, left_back_pose_by_map;
    left_back_pose_by_car.x = left_back_pose_by_car.x;
    left_back_pose_by_car.y = left_back_pose_by_car.y;
    left_back_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, left_back_pose_by_car, left_back_pose_by_map);
    left_back_point.x = left_back_pose_by_map.x;
    left_back_point.y = left_back_pose_by_map.y;

    Pose2D right_back_pose_by_car, right_back_pose_by_map;
    right_back_pose_by_car.x = right_back_pose_by_car.x;
    right_back_pose_by_car.y = right_back_pose_by_car.y;
    right_back_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, right_back_pose_by_car, right_back_pose_by_map);
    right_back_point.x = right_back_pose_by_map.x;
    right_back_point.y = right_back_pose_by_map.y;

    Pose2D right_head_pose_by_car, right_head_pose_by_map;
    right_head_pose_by_car.x = right_head_pose_by_car.x;
    right_head_pose_by_car.y = right_head_pose_by_car.y;
    right_head_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, right_head_pose_by_car, right_head_pose_by_map);
    right_head_point.x = right_head_pose_by_map.x;
    right_head_point.y = right_head_pose_by_map.y;
}

// 获取收起的扫刷结构模型四个角点(含刷盘半径和刷毛长度, 车身坐标系)
const void DynamicCar::Get4CornerBrushFeatherByCar(Point2D &left_head_point, Point2D &left_back_point,
                                                   Point2D &right_back_point, Point2D &right_head_point) const
{
    CarPartsCube brush_fea_cube = GetBrushFeatherCubeByCar();
    left_head_point.x = brush_fea_cube.center_pose.x + brush_fea_cube.cube.length / 2.0;
    left_head_point.y = brush_fea_cube.center_pose.y + brush_fea_cube.cube.width / 2.0;
    left_back_point.x = brush_fea_cube.center_pose.x - brush_fea_cube.cube.length / 2.0;
    left_back_point.y = brush_fea_cube.center_pose.y + brush_fea_cube.cube.width / 2.0;
    right_back_point.x = brush_fea_cube.center_pose.x - brush_fea_cube.cube.length / 2.0;
    right_back_point.y = brush_fea_cube.center_pose.y - brush_fea_cube.cube.width / 2.0;
    right_head_point.x = brush_fea_cube.center_pose.x + brush_fea_cube.cube.length / 2.0;
    right_head_point.y = brush_fea_cube.center_pose.y - brush_fea_cube.cube.width / 2.0;
}

// 获取收起的扫刷结构模型四个角点(含刷盘半径和刷毛长度, 全局地图坐标系)
const void DynamicCar::Get4CornerBrushFeatherByMap(Point2D &left_head_point, Point2D &left_back_point,
                                                   Point2D &right_back_point, Point2D &right_head_point) const
{
    Point2D left_head_point_by_car, left_back_point_by_car;
    Point2D right_back_point_by_car, right_head_point_by_car;
    Get4CornerBrushFeatherByCar(left_head_point_by_car, left_back_point_by_car,
                                right_back_point_by_car, right_head_point_by_car);

    Pose2D left_head_pose_by_car, left_head_pose_by_map;
    left_head_pose_by_car.x = left_head_point_by_car.x;
    left_head_pose_by_car.y = left_head_point_by_car.y;
    left_head_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, left_head_pose_by_car, left_head_pose_by_map);
    left_head_point.x = left_head_pose_by_map.x;
    left_head_point.y = left_head_pose_by_map.y;

    Pose2D left_back_pose_by_car, left_back_pose_by_map;
    left_back_pose_by_car.x = left_back_pose_by_car.x;
    left_back_pose_by_car.y = left_back_pose_by_car.y;
    left_back_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, left_back_pose_by_car, left_back_pose_by_map);
    left_back_point.x = left_back_pose_by_map.x;
    left_back_point.y = left_back_pose_by_map.y;

    Pose2D right_back_pose_by_car, right_back_pose_by_map;
    right_back_pose_by_car.x = right_back_pose_by_car.x;
    right_back_pose_by_car.y = right_back_pose_by_car.y;
    right_back_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, right_back_pose_by_car, right_back_pose_by_map);
    right_back_point.x = right_back_pose_by_map.x;
    right_back_point.y = right_back_pose_by_map.y;

    Pose2D right_head_pose_by_car, right_head_pose_by_map;
    right_head_pose_by_car.x = right_head_pose_by_car.x;
    right_head_pose_by_car.y = right_head_pose_by_car.y;
    right_head_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, right_head_pose_by_car, right_head_pose_by_map);
    right_head_point.x = right_head_pose_by_map.x;
    right_head_point.y = right_head_pose_by_map.y;
}


// 获取扫刷和车体组成的整个矩形模型的四个角点(车身坐标系)
const void DynamicCar::Get4CornerWholeByCar(Point2D &left_head_point, Point2D &left_back_point,
                                            Point2D &right_back_point, Point2D &right_head_point) const
{
    float brush_length, brush_width;
    GetBrushBodyParam(brush_length, brush_width);
    brush_width += 2 * car_body_base_param_.brush_disk_rudia;
    Cube car_cube = GetCarBodyCube();
    float width = std::max(car_cube.width, brush_disk_cube.width);
    left_head_point.x = car_body_base_param_.ego_length + brush_length 
                        - car_body_base_param_.tail_to_rear_axle_lenght;
    left_head_point.y = width / 2.0;
    left_back_point.x = -car_body_base_param_.tail_to_rear_axle_lenght;
    left_back_point.y = width / 2.0;
    right_back_point.x = -car_body_base_param_.tail_to_rear_axle_lenght;
    right_back_point.y = -width / 2.0;
    right_head_point.x = car_body_base_param_.ego_length + brush_length 
                         - car_body_base_param_.tail_to_rear_axle_lenght;
    right_head_point.y = -width / 2.0;
}

// 获取扫刷展开theta时，扫刷和车体组成的整个矩形模型的四个角点(全局地图坐标系)
const void DynamicCar::Get4CornerWholeByMap(Point2D &left_head_point, Point2D &left_back_point,
                                            Point2D &right_back_point, Point2D &right_head_point) const
{
    Point2D left_head_point_by_car, left_back_point_by_car;
    Point2D right_back_point_by_car, right_head_point_by_car;
    Get4CornerWholeByCar(left_head_point_by_car, left_back_point_by_car,
                         right_back_point_by_car, right_head_point_by_car);

    Pose2D left_head_pose_by_car, left_head_pose_by_map;
    left_head_pose_by_car.x = left_head_point_by_car.x;
    left_head_pose_by_car.y = left_head_point_by_car.y;
    left_head_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, left_head_pose_by_car, left_head_pose_by_map);
    left_head_point.x = left_head_pose_by_map.x;
    left_head_point.y = left_head_pose_by_map.y;

    Pose2D left_back_pose_by_car, left_back_pose_by_map;
    left_back_pose_by_car.x = left_back_pose_by_car.x;
    left_back_pose_by_car.y = left_back_pose_by_car.y;
    left_back_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, left_back_pose_by_car, left_back_pose_by_map);
    left_back_point.x = left_back_pose_by_map.x;
    left_back_point.y = left_back_pose_by_map.y;

    Pose2D right_back_pose_by_car, right_back_pose_by_map;
    right_back_pose_by_car.x = right_back_pose_by_car.x;
    right_back_pose_by_car.y = right_back_pose_by_car.y;
    right_back_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, right_back_pose_by_car, right_back_pose_by_map);
    right_back_point.x = right_back_pose_by_map.x;
    right_back_point.y = right_back_pose_by_map.y;

    Pose2D right_head_pose_by_car, right_head_pose_by_map;
    right_head_pose_by_car.x = right_head_pose_by_car.x;
    right_head_pose_by_car.y = right_head_pose_by_car.y;
    right_head_pose_by_car.theta = 0.0;
    LocalToGlobal(ego_pose_, right_head_pose_by_car, right_head_pose_by_map);
    right_head_point.x = right_head_pose_by_map.x;
    right_head_point.y = right_head_pose_by_map.y;
}

const void DynamicCar::GetBrushBodyParam(float &brush_body_length, float &brush_body_width) const
{
    float arm_length = car_body_base_param_.brush_body_length - car_body_base_param_.brush_disk_rudia;
    brush_body_length = arm_length * std::cos(brush_theta_) + car_body_base_param_.brush_disk_rudia;
    brush_body_width = car_body_base_param_.brush_body_width + 2 * arm_length * std::sin(brush_theta_);
}
