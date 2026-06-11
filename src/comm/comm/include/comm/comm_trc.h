

#ifndef __COMM_TRC_H__
#define __COMM_TRC_H__

#include "comm/comm_base.h"
#include "comm/comm_file.h"

#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 跟踪信息保存文件
修改记录:
**************************************************************************************/
class TRACE_SAVE_BASE_C
{
public:
    TRACE_SAVE_BASE_C(const string &strFileName)
    {
        strFileName_ = strFileName;
        pfTraceFile_ = NULL;
    }

    virtual ~TRACE_SAVE_BASE_C(void)
    {
        Stop();
    }

    void Start(void);
    void Stop(void);

protected:
    FILE *GetFileHandle(void);

private:
    FILE_MNG_C       objFileMng_;
    FILE_OPRT_C      objFileOprt_;
    string           strFileName_;
    FILE            *pfTraceFile_;
};

/**************************************************************************************
功能描述: 保存小车路径
修改记录:
**************************************************************************************/
class CAR_ROUTE_SAVE_C : virtual public TRACE_SAVE_BASE_C
{
public:
    CAR_ROUTE_SAVE_C(const string &strFileName = "route.txt") : TRACE_SAVE_BASE_C(strFileName)
    {
        objOutputTimer_.SetOverTime(5.0);
        stLastPoint_.SetZero();
    }

    void Push(const COORDINATE_2D_STRU &stPos);

private:
    TIMER_C            objOutputTimer_;
    COORDINATE_2D_STRU stLastPoint_;
};

/**************************************************************************************
功能描述: 保存小车位姿轨迹
修改记录:
**************************************************************************************/
class CAR_POSE_TRJ_SAVE_C : virtual protected TRACE_SAVE_BASE_C
{
public:
    CAR_POSE_TRJ_SAVE_C(const string &strFileName = "pose_trj.txt") : TRACE_SAVE_BASE_C(strFileName)
    {
        objOutputTimer_.SetOverTime(5.0);
        stLastPose_.SetZero();
    }

    void Push(const COOR_2D_POSE_STRU &stPose);

private:
    TIMER_C           objOutputTimer_;
    COOR_2D_POSE_STRU stLastPose_;
};

/**************************************************************************************
功能描述: 保存底盘控制信息
修改记录:
**************************************************************************************/
class DSP_CTRL_INFO_SAVER_C : virtual protected TRACE_SAVE_BASE_C
{
public:
    DSP_CTRL_INFO_SAVER_C(const string &strFileName) : TRACE_SAVE_BASE_C(strFileName)
    {
    }

    void Push(const MOVE_CTRL_INFO_STRU &stSrcCtrl, const MOVE_CTRL_INFO_STRU &stSmoothCtrl, float actualLinearVel, float actualTurnAngle);

private:
};

/**************************************************************************************
功能描述: 保存位置轨迹
修改记录:
**************************************************************************************/
class POS_TRJ_BUFF_C
{
public:
    POS_TRJ_BUFF_C(float minDist, float maxTrjLen = 1000.0)
    {
        float minDistAbs = MAX(ABS(minDist), 1.0e-3);

        minDistSqu_      = minDistAbs * minDistAbs;
        maxTrjPointNum_  = UINT32(maxTrjLen / minDist);
    }

    void AddNewPos(const COORDINATE_2D_STRU &stPos);
    const list<COORDINATE_2D_STRU> &GetTrj(void);

private:
    float                      minDistSqu_;
    UINT32                     maxTrjPointNum_;

    list<COORDINATE_2D_STRU>   listTrj_;
    COORDINATE_2D_STRU         stLastPos_;
};

#endif

