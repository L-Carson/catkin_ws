#pragma once

#include "comm/comm_base.h"
#include "pcpt_comm/pcpt_comm_define.h"
#include "pcpt_comm/pcpt_comm_curve.h"

class PCPT_TRACK
{
public:
    PCPT_TRACK(const float PredDurationTime) : PredDurationTime_(PredDurationTime)
    {
        predPointsNum_ = PredDurationTime_ * predPointsNumPreSecond_;
    }
    ~PCPT_TRACK(){}

    int Track(const OBJECT_TRACK_STRU &stObjTrack, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
private:
    PCPT_CURVE_C objFitCurve_;                  // 曲线拟合器

    float       PredDurationTime_;              //预测时长
    int         predPointsNumPreSecond_{20};    //每秒预测的点数
    int         predPointsNum_;                 //预测点数量

    int  SmoothFilter(uint32_t halfWinSize, vector<COORDINATE_2D_STRU>& vstPosIn, vector<COORDINATE_2D_STRU>& vstPosOut);
    int  TrajOptimize(const std::vector<COORDINATE_2D_STRU>& vstPosIn, const double distInterval,
                      std::vector<COORDINATE_2D_STRU>& vstPosOut);
    int  FittingTrajectory(const list<TRAJECTORY_POINT_STRU>& lstTrajectory, LABEL_ENUM enClass, vector<double>& vCoeff);
    void PredictTrajectory(const POS_VEL_STRU& stPosVel, double timeStamp, vector<double>& vCoeff, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    void PredictTrajectoryByMultiOrderCurve(const POS_VEL_STRU& stPosVel, double timeStamp, vector<double>& vCoeff, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    void PredictTrajectoryByCircle(const POS_VEL_STRU& stPosVel, double timeStamp, vector<double>& vCoeff, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
    void PredictTrajectoryByVel(const POS_VEL_STRU& stPosVel, double timeStamp, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory);
};

