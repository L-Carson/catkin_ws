
#ifndef __COMM_TRC_CPP__
#define __COMM_TRC_CPP__

#include "comm/comm_trc.h"
#include "comm/comm_base.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

void TRACE_SAVE_BASE_C::Start(void)
{
    Stop();

    string strFileDir;
    objFileMng_.GetTraceFileDir(strFileDir);
    string strFilePath = strFileDir + strFileName_;
    
    pfTraceFile_ = fopen(strFilePath.c_str(), "w");
    if (pfTraceFile_ == NULL) {
        ST_LOG_ERR("Open File Fail.");
        return;
    }

    ST_LOG_INFO("Succ.");
}

void TRACE_SAVE_BASE_C::Stop(void)
{
    if (pfTraceFile_ != NULL) {
        objFileOprt_.SyncBuffToFile(pfTraceFile_);
        objFileOprt_.SyncFileToDisk(pfTraceFile_);
        fclose(pfTraceFile_);
        pfTraceFile_ = NULL;
        ST_LOG_INFO("Succ.");
    }
}

FILE *TRACE_SAVE_BASE_C::GetFileHandle(void)
{
    return pfTraceFile_;
}

void CAR_ROUTE_SAVE_C::Push(const COORDINATE_2D_STRU &stPos)
{
    /* 记录条件: 开关打开，且距离大于0.05或者超过5s */
    FILE *pfFile = GetFileHandle();
    if (pfFile == NULL) {
        return;
    }

    if ((stLastPoint_.CalcDistSqu(stPos) < (0.05 * 0.05)) && !objOutputTimer_.IsTimeOut()) {
        return;
    }

    objOutputTimer_.Reset();

    fprintf(pfFile, "%8.2f %8.2f\n", stPos.x, stPos.y);
    
    stLastPoint_.Copy(stPos);
}

void CAR_POSE_TRJ_SAVE_C::Push(const COOR_2D_POSE_STRU &stPose)
{
    /* 记录条件: 开关打开，且距离大于0.05，或者超过5s */
    FILE *pfFile = GetFileHandle();
    if (pfFile == NULL) {
        return;
    }

    if (   (stLastPose_.stPos.CalcDistSqu(stPose.stPos) < (0.05 * 0.05)) 
        && !objOutputTimer_.IsTimeOut()) {
        return;
    }

    objOutputTimer_.Reset();

    fprintf(pfFile, "%10.4f %10.4f %8.4f\n", stPose.stPos.x, stPose.stPos.y, stPose.dirAngle);
    
    stLastPose_.Copy(stPose);
}

void DSP_CTRL_INFO_SAVER_C::Push(const MOVE_CTRL_INFO_STRU &stSrcCtrl, const MOVE_CTRL_INFO_STRU &stSmoothCtrl, float actualLinearVel, float actualTurnAngle)
{
    FILE *pfFile = GetFileHandle();
    if (pfFile == NULL) {
        return;
    }
    
    fprintf(pfFile, "SrcCtrl.vel=%5.2f .turnAngle=%5.2f Smooth.vel=%5.2f .turnAngle=%5.2f MoveInfo.vel=%5.2f turnAngle=%5.2f\n", 
               stSrcCtrl.linearVel,    stSrcCtrl.turnAngle,
            stSmoothCtrl.linearVel, stSmoothCtrl.turnAngle,
                   actualLinearVel,        actualTurnAngle);
}

void POS_TRJ_BUFF_C::AddNewPos(const COORDINATE_2D_STRU &stPos)
{
    if (listTrj_.size() >= maxTrjPointNum_) {
        listTrj_.pop_front();
    }
    
    if (   (listTrj_.size() == 0)
        || (stLastPos_.CalcDistSqu(stPos) > minDistSqu_)) {
        listTrj_.push_back(stPos);
        stLastPos_.Copy(stPos);
        return;
    }
}

const list<COORDINATE_2D_STRU> &POS_TRJ_BUFF_C::GetTrj(void)
{
    return listTrj_;
}



#endif


