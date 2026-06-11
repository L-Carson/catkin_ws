#include "base_layer/vehicle_model/car_body.h"

#include <unordered_set>
#include "comm_layer/comm_math/comm_math.h"

/**
 * NOTE: 如果添加新机型，请通知感知检查:
 *    1.comm/comm_function.cpp是否适配
 *    2.comm/comm_car_body_file.cpp是否适配
 *    3.comm_lidar_3d/sensor_lidar_availability.cpp是否适配
 *    4.comm/maintain_param.cpp是否适配
 */
const CarBodyBaseParam CarBody::all_car_body_base_param_[] =
{
    //                                                      后轮   车尾到后 扫刷收 扫刷收 扫刷展  扫刷展  扫刷结  刷盘   刷毛   刷盘   最大转
    //                   车型    车长    车宽   车高    轴距  轮距   轴距离   起长度 起宽度 开长度  开宽度  构高度  半径   长度   高度   向角度
    {CarTypeEnum::S310, "S310",  0.78,  1.26,  0.50,  0.50,  0.88,  1.45,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S320, "S320",  0.82,  1.27,  0.97,  0.40,  0.80,  1.72,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S323, "S323",  0.82,  1.69,  0.85,  0.41,  0.79,  1.72,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S324, "S324",  0.82,  1.26,  0.90,  0.40,  0.79,  1.72,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S325, "S325",  0.82,  1.10,  0.40,  0.40,  0.79,  1.55,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S326, "S326",  0.82,  1.26,  0.90,  0.40,  0.79,  1.72,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S330, "S330",  0.82,  1.11,  0.48,  0.61,  0.89,  1.68,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S332, "S332",  1.03,  1.17,  0.81,  0.72,  0.87,  1.67,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S333, "S333",  1.03,  1.18,  0.81,  0.62,  0.87,  1.67,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S334, "S334",  1.03,  1.18,  0.80,  0.62,  0.87,  1.67,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S335, "S335",  1.03,  1.18,  0.80,  2.51,  0.87,  1.67,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S350, "S350",  0.91,  0.85,  0.41,  0.48,  0.73,  1.61,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S520, "S520",  1.44,  1.56,  1.00,  0.50,  1.18,  2.10,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
    {CarTypeEnum::S530, "S530",  1.47,  1.55,  2.08,  0.65,  1.13,  2.59,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.64},
};

void CarBody::InitModel(void) {
    const CarBodyBaseParam *car_body_base_ptr = GetCarBodyBasePtr("S330");
    if (nullptr == car_body_base_ptr) {
        return;
    }
    car_body_base_param_ = *car_body_base_ptr;
}

const CarBodyBaseParam *CarBody::GetCarBodyBasePtr(const std::string &car_type_name) {
    for (const CarBodyBaseParam &car_body_base_info : all_car_body_base_param_) {
        if (0 != car_type_name.compare(car_body_base_info.car_type_name)) {
            continue;
        }
        return &car_body_base_info;
    }
    return nullptr;
}

bool CarBody::IsS310(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S310);
}

// 判断是否是S320车型
bool CarBody::IsS320(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S320);
}

// 判断是否是S323车型
bool CarBody::IsS323(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S323);
}

// 判断是否是S324车型
bool CarBody::IsS324(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S324);
}

// 判断是否是S325车型
bool CarBody::IsS325(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S325);
}

// 判断是否是S326车型
bool CarBody::IsS326(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S326);
}


// 判断是否是S330车型
bool CarBody::IsS330(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S330);
}

// 判断是否是S332车型
bool CarBody::IsS332(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S332);
}

// 判断是否是S333车型
bool CarBody::IsS333(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S333);
}


// 判断是否是S334车型
bool CarBody::IsS334(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S334);
}


// 判断是否是S335车型

bool CarBody::IsS335(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S335);
}


// 判断是否是S350车型
bool CarBody::IsS350(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S350);
}

// 判断是否是S520车型
bool CarBody::IsS520(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S520);
}

// 判断是否是S530车型
bool CarBody::IsS530(void) const
{
    return (car_body_base_param_.car_type == CarTypeEnum::S530);
}

void CarBody::SetEgoPose(const Pose2D &ego_pose)
{
    ego_pose_ = ego_pose;
}

Pose2D CarBody::GetEgoPose() const
{
    return ego_pose_;
}


float CarBody::GetCarBodyHeight() const 
{
    return car_body_base_param_.ego_height;
}

float CarBody::GetBrushBodyHeight() const
{
    return car_body_base_param_.brush_body_height;
}


// 获取车体的宽度
float CarBody::GetCarBodyWidth() const
{
    return car_body_base_param_.ego_width;
}

float CarBody::GetTailToRearAxleLength() const
{
    return car_body_base_param_.tail_to_rear_axle_lenght;
}


float CarBody::GetWheelBaseLength() const
{
    return car_body_base_param_.wheel_base_length;
}

float CarBody::GetMinTurnRadius() const
{
    return car_body_base_param_.wheel_base_length / std::tan(car_body_base_param_.max_steer_angle);
}

float CarBody::GetMaxSteerAngle() const
{
    return car_body_base_param_.max_steer_angle;
}

Cube CarBody::GetCarWheelCubeByCar(void) const {
    Cube car_wheel_cube;
    car_wheel_cube.length = car_body_base_param_.wheel_base_length;
    car_wheel_cube.width  = car_body_base_param_.back_wheel_width;
    car_wheel_cube.height = car_body_base_param_.ego_height;
    return car_wheel_cube;
}

Cube CarBody::GetCarBodyCubeByCar(void) const {
    Cube car_body_cube;
    car_body_cube.length = car_body_base_param_.ego_length;
    car_body_cube.width  = car_body_base_param_.ego_width;
    car_body_cube.height = car_body_base_param_.ego_height;
    return car_body_cube;
}

CarPartsCube CarBody::GetFeatherFoldCubeByCar(void) const {
    CarPartsCube brush_disk_cube;
    brush_disk_cube.center_pose.x = car_body_base_param_.ego_length - 
                                    car_body_base_param_.tail_to_rear_axle_lenght + 
                                    car_body_base_param_.brush_fold_length / 2.0;
    brush_disk_cube.center_pose.y = 0.0;
    brush_disk_cube.cube.length = car_body_base_param_.brush_fold_length;
    brush_disk_cube.cube.width  = car_body_base_param_.brush_fold_width + 
                                  car_body_base_param_.brush_feather_length * 2.0;
    brush_disk_cube.cube.height = car_body_base_param_.brush_body_height;
    return brush_disk_cube;
}


CarPartsCube CarBody::GetFeatherSpreadCubeByCar(void) const {
    CarPartsCube brush_disk_cube;
    brush_disk_cube.center_pose.x = car_body_base_param_.ego_length - 
                                    car_body_base_param_.tail_to_rear_axle_lenght + 
                                    car_body_base_param_.brush_spread_length / 2.0;
    brush_disk_cube.center_pose.y = 0.0;
    brush_disk_cube.cube.length = car_body_base_param_.brush_spread_length;
    brush_disk_cube.cube.width  = car_body_base_param_.brush_spread_width + 
                                  car_body_base_param_.brush_feather_length * 2.0;
    brush_disk_cube.cube.height = car_body_base_param_.brush_body_height;
    return brush_disk_cube;
}


// 获取车轮模型的四个角度（车身坐标系）
void CarBody::Get4CornerWheelModelByCar(Point2D &left_head_point, Point2D &left_back_point,
                                              Point2D &right_back_point, Point2D &right_head_point) const
{
    left_head_point.x = car_body_base_param_.wheel_base_length;
    left_head_point.y = car_body_base_param_.back_wheel_width / 2.0;
    left_back_point.x = 0.0;
    left_back_point.y = car_body_base_param_.back_wheel_width / 2.0;
    right_back_point.x = 0.0;
    right_back_point.y = -car_body_base_param_.back_wheel_width / 2.0;
    right_head_point.x = car_body_base_param_.wheel_base_length;
    right_head_point.y = -car_body_base_param_.back_wheel_width / 2.0;
}

// 获取车轮模型的四个角度（全局地图坐标系）
void CarBody::Get4CornerWheelModelByMap(Point2D &left_head_point, Point2D &left_back_point,
                                              Point2D &right_back_point, Point2D &right_head_point) const
{
    Point2D left_head_point_by_car,  left_back_point_by_car;
    Point2D right_back_point_by_car,  right_head_point_by_car;
    Get4CornerWheelModelByCar(left_head_point_by_car, left_back_point_by_car,
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

// 获取车体模型的四个角度（车身坐标系）
void CarBody::Get4CornerCarModelByCar(Point2D &left_head_point, Point2D &left_back_point,
                                            Point2D &right_back_point, Point2D &right_head_point) const
{
    left_head_point.x = car_body_base_param_.ego_length - car_body_base_param_.tail_to_rear_axle_lenght;
    left_head_point.y = car_body_base_param_.ego_width / 2.0;
    left_back_point.x = -car_body_base_param_.tail_to_rear_axle_lenght;
    left_back_point.y = car_body_base_param_.ego_width / 2.0;
    right_back_point.x = -car_body_base_param_.tail_to_rear_axle_lenght;
    right_back_point.y = -car_body_base_param_.ego_width / 2.0;
    right_head_point.x = car_body_base_param_.ego_length - car_body_base_param_.tail_to_rear_axle_lenght;
    right_head_point.y = -car_body_base_param_.ego_width / 2.0;
}

// 获取车体模型的四个角度（全局地图坐标系）
void CarBody::Get4CornerCarModelByMap(Point2D &left_head_point, Point2D &left_back_point,
                                            Point2D &right_back_point, Point2D &right_head_point) const
{
    Point2D left_head_point_by_car,  left_back_point_by_car;
    Point2D right_back_point_by_car,  right_head_point_by_car;
    Get4CornerCarModelByCar(left_head_point_by_car, left_back_point_by_car,
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
