#include "base_layer/vehicle_model/driver_car.h"


// 车辆驾驶器

class DriverCar
{
public:
    DriverCar(DYNC_CAR_C &objCar, BOOL isGoForward = true)
                    : objCar_(objCar),
                      isGoForward_(isGoForward),
    {
    }

    virtual ~DriverCar(void)
    {
    }

    void  SetMoveDir(BOOL isGoForward, BOOL isUseFrontWheelToGoForwardTrc);
    BOOL  IsGoForward(void) const;

    void  SetRoutePose(const COOR_2D_POSE_STRU &stCarRoutePose);
    void  SetRoutePose(const POSE_2D_C &objRoutePose);
    void  GetRoutePose(COOR_2D_POSE_STRU &stCarRoutePose) const;
    void  GetFrontWheelRoutePose(POSE_2D_C &objRoutePose) const;
    void  GetRoutePose(POSE_2D_C &objRoutePose) const;

    float GetSteeringAngleOfFrontWheelTrc(const COORDINATE_2D_STRU &stAimPosByMap) const;
    float GetSteeringAngleOfBackWheelTrc(const COORDINATE_2D_STRU &stAimPosByMap) const;
    float GetSteeringAngleOfBackWheelDrive(const COORDINATE_2D_STRU &stAimPosByMap) const;
    float GetAimPointSteeringAngle(const COORDINATE_2D_STRU &stAimPosByMap) const;

    float GetSimulationVel(void) const;
    float GetMaxMoveVelAbs(void) const;

private:
    DYNC_CAR_C &objCar_;

    bool is_go_forward_;
};

#endif // DRIVER_CAR_H

