#ifndef MOVE_CAR_H
#define MOVE_CAR_H

#include <vector>
#include <string>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_geometry/comm_geometry.h"
#include "base_layer/vehicle_model/dynamic_car.h"



// 移动车辆
class MoveCar : public DynamicCar
{
public:
    MoveCar(float timePerMove = 0.05, float maxAccAbs = 1.0e10)
    {
        timeLenPerMove_           = timePerMove;
        maxAccAbs_                = maxAccAbs;
        maxVelDiffOfPerPeriodAbs_ = maxAccAbs_ * timeLenPerMove_;
        inverseOfTimeLenPerMove_  = 1.0 / timePerMove;
        inverseOfWheelBaseLen_    = 1.0 / GetWheelBaseLen();

        ResetMoveInfo();
    }

    virtual ~MoveCar (void)
    {
    }

    float Move(float steeringAngle, float linearVel);
    float MoveBackWheelDrive(float steeringAngle, float linearVel);
    float MoveWithAccLimit(float steeringAngle, float linearVel);

    void  ResetMoveInfo(void);

    const CAR_MOVE_INFO_STRU &GetMoveInfo(void) const;

private:
    float               timeLenPerMove_;
    float               maxAccAbs_;
    float               maxVelDiffOfPerPeriodAbs_;
    float               inverseOfTimeLenPerMove_;
    float               inverseOfWheelBaseLen_;

    CAR_MOVE_INFO_STRU  stMoveInfo_;     /* 最新运动信息 */

    float GetLinearVelWithAccLimit(float linearVelCmd) const;
};

#endif // DRIVER_CAR_H

