#ifndef DRIVER_CAR_H
#define DRIVER_CAR_H

#include <vector>
#include <string>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_geometry/comm_geometry.h"
#include "base_layer/vehicle_model/dynamic_car.h"
#include "comm/comm_data_type.h"


// 车辆驾驶器
class DriverCar : public DynamicCar
{
public:
    DriverCar(bool is_go_forward_ = true, const float &step_move_time)
        : is_go_forward_(is_go_forward_), step_move_time_(step_move_time)
    {
    }

    virtual ~DriverCar(void)
    {
    }

    void  SetMoveDir(const bool &is_go_forward);
    bool  IsGoForward(void) const;
    float MoveCar(const float &steer_angle, const float &vel);
    void  MoveCar(const float &steer_angle, const float &move_len);

    float GetSteerAngle(const Point2D &aim_point) const;

private:
    float step_move_time_;
    bool is_go_forward_;
};

#endif // DRIVER_CAR_H

