#ifndef __PCPT_MOTION_ESTIMATOR_H__
#define __PCPT_MOTION_ESTIMATOR_H__

#include <list>
#include <vector>
#include <string>
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_define.h"

#include "pcpt_det_dynamic/pcpt_kalman_filter.h"

struct OBJECT_STATE_STRU {
    BOX_INFO_STRU stBoxInfo;
    float         vx;
    float         vy;
};

/**************************************************************************************
功能描述: 目标运动状态估计器
修改记录:
**************************************************************************************/
class MOTION_ESTIMATOR_C
{
public:
    MOTION_ESTIMATOR_C(const BOX_INFO_STRU& stObjBoxInfo,
                       const double validTimeThr = 0.5)
        : validTimeThr_(validTimeThr)
    {
        stObjState_.stBoxInfo = stObjBoxInfo;
        stObjState_.vx = 0.0;
        stObjState_.vy = 0.0;
    }

    ~MOTION_ESTIMATOR_C(void) {  };

    void  Process(const BOX_INFO_STRU& stObjBoxInfo);
    INT32 GetObjState(const double time, OBJECT_STATE_STRU &stObjState);
    INT32 GetObjState(const double time, OBJECT_TRACK_STRU &stObjTrack);
    BOOL  IsLost(const double time)
    {
        return time - stObjState_.stBoxInfo.timeStamp > validTimeThr_;
    }

private:
    void  UptTraj(const double time, const double x, const double y);

private:
    KF_C                             objKf_;
    OBJECT_STATE_STRU                stObjState_;
    double                           validTimeThr_;
    std::list<TRAJECTORY_POINT_STRU> lstTrajectory_;
};

#endif

