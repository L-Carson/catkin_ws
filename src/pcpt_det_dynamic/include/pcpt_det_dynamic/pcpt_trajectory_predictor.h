#ifndef __PCPT_TRAJECTORY_PREDICTOR_H__
#define __PCPT_TRAJECTORY_PREDICTOR_H__

#include "comm/comm_base.h"
#include "pcpt_comm/pcpt_comm_define.h"
#include "pcpt_comm/pcpt_comm_curve.h"

class TRAJECTORY_PREDICTOR_C
{
public:
    TRAJECTORY_PREDICTOR_C(const float PredDurationTime)
        : PredDurationTime_(PredDurationTime)
    {
        predPointsNum_ = PredDurationTime_ * predPointsNumPreSecond_;
    }

    ~TRAJECTORY_PREDICTOR_C(void){ };

    INT32 GetPredictTraj(const OBJECT_TRACK_STRU& stObjTrack,
                         std::vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);

private:
    INT32 FittingTraj(const std::list<TRAJECTORY_POINT_STRU>& lstTrajectory,
                      LABEL_ENUM enClass, std::vector<double>& vCoeff);
    BOOL  IsOkTrackTraj(const OBJECT_TRACK_STRU& stObjTrack);
    void  PredictTrajectory(const POS_VEL_STRU& stPosVel, double timeStamp,
                            std::vector<double>& vCoeff,
                            std::vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    void  PredictTrajectoryByMultiOrderCurve(const POS_VEL_STRU& stPosVel, double timeStamp,
                                             std::vector<double>& vCoeff,
                                             std::vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    void  PredictTrajectoryByCircle(const POS_VEL_STRU& stPosVel, double timeStamp,
                                    std::vector<double>& vCoeff,
                                    std::vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    void  PredictTrajectoryByVel(const POS_VEL_STRU& stPosVel, double timeStamp,
                                 std::vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    INT32 SmoothFilter(UINT32 halfWinSize, std::vector<COORDINATE_2D_STRU>& vstPosIn,
                       std::vector<COORDINATE_2D_STRU>& vstPosOut);
    INT32 TrajOptimize(const std::vector<COORDINATE_2D_STRU>& vstPosIn, const double distInterval,
                      std::vector<COORDINATE_2D_STRU>& vstPosOut);
private:
    PCPT_CURVE_C objFitCurve_;            // 曲线拟合器
    float PredDurationTime_;              // 预测时长
    INT32 predPointsNumPreSecond_{20};    // 每秒预测的点数
    INT32 predPointsNum_;                 // 预测点数量
};

#endif
