#include "pcpt_det_dynamic/pcpt_motion_estimator.h"

void  MOTION_ESTIMATOR_C::Process(const BOX_INFO_STRU& stObjBoxInfo)
{
    stObjState_.stBoxInfo = stObjBoxInfo;

    MEASURE_PACKAGE_C stMeasData;
    stMeasData.sensorType  = MEASURE_PACKAGE_C::LIDAR;
    stMeasData.timeStamp   = stObjBoxInfo.timeStamp;
    stMeasData.measureData = Eigen::VectorXd(2);
    stMeasData.measureData << stObjBoxInfo.stRotatedRect.x, stObjBoxInfo.stRotatedRect.y;
    objKf_.Process(stMeasData);

    Eigen::VectorXd motionState = objKf_.GetState();
    stObjState_.stBoxInfo.stRotatedRect.x = motionState(0);
    stObjState_.stBoxInfo.stRotatedRect.y = motionState(1);
    stObjState_.vx = motionState(2);
    stObjState_.vy = motionState(3);

    UptTraj(stMeasData.timeStamp, motionState(0), motionState(1));
}

INT32 MOTION_ESTIMATOR_C::GetObjState(const double time, OBJECT_STATE_STRU &stObjState)
{
    double eps = 0.001;
    double dt  = time - stObjState_.stBoxInfo.timeStamp;
    if (dt < -eps) {
        return -1;
    } else if (dt < eps) {
        stObjState = stObjState_;
    } else {
        stObjState = stObjState_;
        stObjState.stBoxInfo.timeStamp = time;
        stObjState.stBoxInfo.stRotatedRect.x += stObjState_.vx * dt;
        stObjState.stBoxInfo.stRotatedRect.y += stObjState_.vy * dt;
    }

    return 0;
}

INT32 MOTION_ESTIMATOR_C::GetObjState(const double time, OBJECT_TRACK_STRU &stObjTrack)
{
    OBJECT_STATE_STRU stObjState;
    if (0 != GetObjState(time, stObjState)) {
        ST_LOG_ERR("Get Obj State Fail.");
        return -1;
    }

    stObjTrack.stBox = stObjState.stBoxInfo;
    stObjTrack.stPosVel.objectID = stObjTrack.stBox.boxID;
    stObjTrack.stPosVel.posX = stObjState.stBoxInfo.x();
    stObjTrack.stPosVel.posY = stObjState.stBoxInfo.y();
    stObjTrack.stPosVel.velX = stObjState.vx;
    stObjTrack.stPosVel.velY = stObjState.vy;
    stObjTrack.lstTrajectory = lstTrajectory_;

    return 0;
}

void  MOTION_ESTIMATOR_C::UptTraj(const double time, const double x, const double y)
{
    TRAJECTORY_POINT_STRU stTrajectoryPoint;
    stTrajectoryPoint.timeStamp = time;
    stTrajectoryPoint.x         = x;
    stTrajectoryPoint.y         = y;
    lstTrajectory_.emplace_back(stTrajectoryPoint);
    while (fabs(time - lstTrajectory_.front().timeStamp) > 3.0) {
        lstTrajectory_.erase(lstTrajectory_.begin());
    }
}

