#include "pcpt_comm/pcpt_comm_car.h"


/**************************************************************************************
功能描述: 设置车体在map下的位置
修改记录:
**************************************************************************************/
void PCPT_CAR_C::SetPose(const COOR_2D_POSE_STRU& stCarPoseByMap)
{
    SetCarPose(stCarPoseByMap);
    Get4CornerPosByMap(stLeftHeadPosByMap_, stRightHeadPosByMap_, stLeftTailPosByMap_, stRightTailPosByMap_, 0.0, 0.0);
}

/**************************************************************************************
功能描述: 车体坐标系下，二维点坐标是否在车体范围内
修改记录:
**************************************************************************************/
BOOL PCPT_CAR_C::IsPointInCar(const COORDINATE_2D_STRU &stPoint) const
{
    if (stPoint.x > stLeftHeadPosByCar_.x  || stPoint.y > stLeftHeadPosByCar_.y  ||
        stPoint.x < stLeftTailPosByCar_.x  || stPoint.y > stLeftTailPosByCar_.y  ||
        stPoint.x > stRightHeadPosByCar_.x || stPoint.y < stRightHeadPosByCar_.y ||
        stPoint.x < stRightTailPosByCar_.x || stPoint.y < stRightTailPosByCar_.y)
        return false;

    return true;
}

/**************************************************************************************
功能描述: map坐标系下，二维点坐标是否在车头后方
修改记录:
**************************************************************************************/
bool PCPT_CAR_C::IsPointBehindCarHeadByMap(const COORDINATE_2D_STRU &stPoint) const
{
    PCPT_2D_VECTOR_STRU stVector(stLeftHeadPosByCar_, stRightHeadPosByMap_);
    return !stVector.IsOnLeft(stPoint);
}

/**************************************************************************************
功能描述: map坐标系下，二维点坐标是否在车体尾后方
修改记录:
**************************************************************************************/
bool PCPT_CAR_C::IsPointBehindCarTailByMap(const COORDINATE_2D_STRU &stPoint) const
{
    PCPT_2D_VECTOR_STRU stVector(stLeftTailPosByCar_, stRightTailPosByMap_);
    return !stVector.IsOnLeft(stPoint);
}

/**************************************************************************************
功能描述: 扫刷展开时宽度的一半
修改记录:
**************************************************************************************/
float PCPT_CAR_C::HalfBodyWidthWithExtend()
{
    SetExpandState(true);
    Update();
    return GetBodyWidth() * 0.5;
}

/**************************************************************************************
功能描述: 扫刷未展开时宽度的一半
修改记录:
**************************************************************************************/
float PCPT_CAR_C::HalfBodyWidthNoExtend()
{
    SetExpandState(false);
    Update();
    return GetBodyWidth() * 0.5;
}
