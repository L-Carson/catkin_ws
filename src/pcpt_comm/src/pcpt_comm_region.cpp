#include <limits>
#include "pcpt_comm/pcpt_comm_region.h"

/**************************************************************************************
功能描述: 将子坐标系下的矩形框扩展到父坐标系下
修改记录:
**************************************************************************************/
void PCPT_REGION_C::ExtendRectBySon2ByFather(const COOR_2D_POSE_STRU& stSonPoseByFather,
                                             const RECTANGLE_STRU& stRectBySon,
                                             RECTANGLE_STRU& stRectByFather) const
{
    float minX = numeric_limits<float>::max();
    float minY = numeric_limits<float>::max();
    float maxX = numeric_limits<float>::lowest();
    float maxY = numeric_limits<float>::lowest();

    PCPT_TF_C objTf(stSonPoseByFather);
    COORDINATE_2D_STRU stPosBySon, stPosByFather;

#define PCPT_GET_BOUNDARY_VALUE()                           \
    objTf.TransSonPosToFather(stPosBySon, stPosByFather);   \
    minX = minX < stPosByFather.x ? minX : stPosByFather.x; \
    minY = minY < stPosByFather.y ? minY : stPosByFather.y; \
    maxX = maxX > stPosByFather.x ? maxX : stPosByFather.x; \
    maxY = maxY > stPosByFather.y ? maxY : stPosByFather.y; \

    stPosBySon.SetValue(stRectBySon.minX, stRectBySon.minY);
    PCPT_GET_BOUNDARY_VALUE();

    stPosBySon.SetValue(stRectBySon.minX, stRectBySon.maxY);
    PCPT_GET_BOUNDARY_VALUE();

    stPosBySon.SetValue(stRectBySon.maxX, stRectBySon.minY);
    PCPT_GET_BOUNDARY_VALUE();

    stPosBySon.SetValue(stRectBySon.maxX, stRectBySon.maxY);
    PCPT_GET_BOUNDARY_VALUE();

    stRectByFather.minX = minX;
    stRectByFather.minY = minY;
    stRectByFather.maxX = maxX;
    stRectByFather.maxY = maxY;
}

/**************************************************************************************
功能描述: 将子车体坐标系下的矩形框扩展到map坐标系下
修改记录:
**************************************************************************************/
void PCPT_REGION_C::ExtendRectByCar2ByMap(const COOR_2D_POSE_STRU& stCarPoseByMap,
                                          const RECTANGLE_STRU& stRectByCar,
                                          RECTANGLE_STRU& stRectByMap) const
{
    ExtendRectBySon2ByFather(stCarPoseByMap, stRectByCar, stRectByMap);
}

