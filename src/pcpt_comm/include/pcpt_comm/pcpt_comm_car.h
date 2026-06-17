#ifndef __PCPT_COMM_CAR_H__
#define __PCPT_COMM_CAR_H__

#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include "pcpt_comm/pcpt_comm_define.h"

using namespace std;

/**************************************************************************************
功能描述: 动态车辆执行器
修改记录:
**************************************************************************************/
class PCPT_CAR_C : public DYNC_CAR_C
{
public:
    PCPT_CAR_C() : DYNC_CAR_C()
    {
        Get4CornerPosByCar(stLeftHeadPosByCar_, stRightHeadPosByCar_, stLeftTailPosByCar_, stRightTailPosByCar_, 0.0, 0.0);
    }

    ~PCPT_CAR_C(){}

    void SetPose(const COOR_2D_POSE_STRU& stCarPose);

    bool IsPointInCar(const COORDINATE_2D_STRU &stPoint) const;

    bool IsPointBehindCarHeadByMap(const COORDINATE_2D_STRU &stPoint) const;
    bool IsPointBehindCarTailByMap(const COORDINATE_2D_STRU &stPoint) const;

    float HalfBodyWidthWithExtend();
    float HalfBodyWidthNoExtend();

private:
    COORDINATE_2D_STRU  stLeftHeadPosByCar_;    // 车身左前部坐标（car坐标系）
    COORDINATE_2D_STRU  stRightHeadPosByCar_;   // 车身右前部坐标（car坐标系）
    COORDINATE_2D_STRU  stLeftTailPosByCar_;    // 车身左后部坐标（car坐标系）
    COORDINATE_2D_STRU  stRightTailPosByCar_;   // 车身右后部坐标（car坐标系）

    COORDINATE_2D_STRU  stLeftHeadPosByMap_;    // 车身左前部坐标（map坐标系）
    COORDINATE_2D_STRU  stRightHeadPosByMap_;   // 车身右前部坐标（map坐标系）
    COORDINATE_2D_STRU  stLeftTailPosByMap_;    // 车身左后部坐标（map坐标系）
    COORDINATE_2D_STRU  stRightTailPosByMap_;   // 车身右后部坐标（map坐标系）

    void InitDirectedLine();
};

#endif
