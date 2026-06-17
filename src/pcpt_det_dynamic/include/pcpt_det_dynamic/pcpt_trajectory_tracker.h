#ifndef __PCPT_TRAJECTORY_TRACHER_H__
#define __PCPT_TRAJECTORY_TRACHER_H__

#include <unordered_set>
#include <unordered_map>

#include "comm/comm_base.h"

#include "pcpt_comm/pcpt_comm_define.h"
#include "pcpt_comm/pcpt_comm_polygon.h"
#include "pcpt_comm/pcpt_comm_hungarian.h"

#include "pcpt_det_dynamic/pcpt_motion_estimator.h"

/**************************************************************************************
功能描述: 目标轨迹跟踪器
修改记录:
**************************************************************************************/
class TRAJECTORY_TRACKER_C
{
public:
    TRAJECTORY_TRACKER_C(void)
    {

    }

    ~TRAJECTORY_TRACKER_C(void) {}

    INT32 GetObjTrackTraj(const LIDAR_CLUSTER_STRU&       stDectObjCurr,
                          std::vector<OBJECT_TRACK_STRU>& vstObjTrack);

private:
    void  DectObjAssociation(LIDAR_CLUSTER_STRU& stDectObjCurr);
    void  UptDectObjMotionEstimator(LIDAR_CLUSTER_STRU& stDectObjCurr);
    INT32 GetObjTrackTraj(const double time,
                          std::vector<OBJECT_TRACK_STRU>& vstObjTrack);
    void  CalcIouDistMatrix(std::vector<BOX_INFO_STRU>& vstBoxInfoCur,
                            std::vector<BOX_INFO_STRU>& vstBoxInfoPre,
                            std::vector<std::vector<double>>& vvCostMatrix);
private:
    HUNGARIAN_ALGORITHM_C objHungarian_;      // 匈牙利算法
    PCPT_POLYGON_C        objAnyPolygon_;     // 任意多边形
    std::unordered_map<UINT32, MOTION_ESTIMATOR_C*> umObjMotionEstimator_; // 目标ID - 运动估计器
};

#endif


