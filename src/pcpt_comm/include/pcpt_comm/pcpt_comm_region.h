#ifndef __PCPT_COMM_REGION_H__
#define __PCPT_COMM_REGION_H__

#include "comm/comm_base.h"
#include "pcpt_comm/pcpt_comm_tf.h"

/**************************************************************************************
功能描述: 区域处理类
修改记录:
**************************************************************************************/
class PCPT_REGION_C
{
public:
    PCPT_REGION_C() {}
    PCPT_REGION_C(RECTANGLE_STRU& stRectByCar) {}

    ~PCPT_REGION_C() {}

    void ExtendRectBySon2ByFather(const COOR_2D_POSE_STRU& stSonPoseByFather, const RECTANGLE_STRU& stRectBySon, RECTANGLE_STRU& stRectByFather) const;
    void ExtendRectByCar2ByMap(const COOR_2D_POSE_STRU& stCarPoseByMap, const RECTANGLE_STRU& stRectByCar, RECTANGLE_STRU& stRectByMap) const;
};

#endif
