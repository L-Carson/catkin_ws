#include "base_layer/vehicle_model/driver_car.h"


//设置行进方向
void DriverCar::SetMoveDir(const bool &is_go_forward)
{
    is_go_forward_ = is_go_forward;
}

bool DriverCar::IsGoForward(void) const
{
    return is_go_forward_;
}

float DriverCar::MoveCar(const float &steer_angle, const float &vel)
{
    float move_len = vel * step_move_time_;
    MoveCar(steer_angle, move_len);
    return std::fabs(move_len);
}

void DriverCar::MoveCar(const float &steer_angle, const float &move_len)
{
    const Pose2D &old_ego_pose = GetEgoPose();

    float delta_dir_angle = move_len * std::tan(steer_angle) / GetWheelBaseLength();
    float move_dir = old_ego_pose.theta + delta_dir_angle / 2.0;

    Pose2D new_ego_pose;
    new_ego_pose.x = old_ego_pose.x + std::cos(move_dir) * move_len;
    new_ego_pose.y = old_ego_pose.y + std::sin(move_dir) * move_len;
    new_ego_pose.theta = WrapAngleRad(old_ego_pose.theta + delta_dir_angle);
    SetEgoPose(new_ego_pose);
}


// 纯跟踪算法获取跟踪目标的转向角
float DriverCar::GetSteerAngle(const Point2D &aim_point) const
{
    const Pose2D &ego_pose = GetEgoPose();
    float max_steer_angle = M_PI_2;

    // 假设打转方向盘后，后轮经过一个圆弧到达目标点，则该圆弧的圆心点必在后轮轴上
    float left_dir_uint_vec_x = std::cos(ego_pose.theta + 0.5 * M_PI);
    float left_dir_uint_vec_y = std::sin(ego_pose.theta + 0.5 * M_PI);
    float wheel_to_aim_vec_x = aim_point.x - ego_pose.x;
    float wheel_to_aim_vec_y = aim_point.y - ego_pose.y;

    // 上述两向量点乘，如果结果为0，表示目标和后轮在一条直线上直接后退即可
    float vec_dot_product = wheel_to_aim_vec_x * left_dir_uint_vec_x + 
                            wheel_to_aim_vec_y * left_dir_uint_vec_y;
    if (std::fabs(vec_dot_product) < 1.0e-20) {
        return 0.0;
    }

    // 计算后轮转向半径，大于0表示圆心在车辆左侧，小于0表示圆心在右侧
    float turn_radius = 0.5 * (wheel_to_aim_vec_x * wheel_to_aim_vec_x + 
                               wheel_to_aim_vec_y * wheel_to_aim_vec_y) / vec_dot_product;

    // 根据后轮转弯半径，计算前轮转向角
    float steer_angle = Sign(turn_radius) * std::atan2(GetWheelBaseLength(), std::fabs(turn_radius));
    return LIMIT(steer_angle, -1.0 * max_steer_angle, max_steer_angle);
}

#endif // DRIVER_CAR_H

