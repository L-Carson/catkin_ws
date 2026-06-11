
#ifndef __COMM_ROS_TRC_CPP__
#define __COMM_ROS_TRC_CPP__

#include "comm/comm_ros_trc.h"
#include "comm/comm_base.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

void TRACE_SAVE_BASE_ROS_C::Upt(void)
{
    if (objDbgSw_.Get(enSwType_)) {
        if (NULL == GetFileHandle()) {
            Start();
        }
    } else {
        if (NULL != GetFileHandle()) {
            Stop();
        }
    }
}

void CAR_ROUTE_SAVE_ROS_C::PushEx(const COORDINATE_2D_STRU &stPos)
{
    Upt();
    Push(stPos);
}

void CAR_POSE_TRJ_SAVE_ROS_C::PushEx(const COOR_2D_POSE_STRU &stPose)
{
    Upt();
    Push(stPose);
}

void ROUTES_SAVER_C::PeriodUpt(void)
{
    const COOR_2D_POSE_STRU *pstCarPose   = objLocReceiver_.GetNormal();
    BOOL                     isGetPosFail = (pstCarPose == NULL);
    ST_LOG_ERR_SAMPLE_IF_TRUE(isGetPosFail, 60 * 20, "objLocReceiver_.GetNormal() Fail.");
    if (isGetPosFail) {
        return;
    }

    objCar_.SetCarPose(*pstCarPose);

    objFrontWheelRouteSaver_.PushEx(objCar_.GetFrontWheelPos());
    objBackWheelRouteSaver_.PushEx(objCar_.GetCarPos());
    objCarPoseTrjSaver_.PushEx(objCar_.GetCarPose());
}

void DSP_CTRL_INFO_ROS_SAVER_C::PushEx(const MOVE_CTRL_INFO_STRU &stSrcCtrl, const MOVE_CTRL_INFO_STRU &stSmoothCtrl, float actualLinearVel, float actualTurnAngle)
{
    Upt();
    Push(stSrcCtrl, stSmoothCtrl, actualLinearVel, actualTurnAngle);
}

#endif


