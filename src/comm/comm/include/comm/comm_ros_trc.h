

#ifndef __COMM_ROS_TRC_H__
#define __COMM_ROS_TRC_H__

#include "comm/comm_dbg_sw.h"
#include "comm/comm_ros_base.h"
#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/comm_trc.h"
#include "comm/comm_car.h"
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 跟踪信息保存文件
修改记录:
**************************************************************************************/
class TRACE_SAVE_BASE_ROS_C : virtual protected TRACE_SAVE_BASE_C
{
public:
    TRACE_SAVE_BASE_ROS_C(ros::NodeHandle &nh, DBG_SW_TYPE_ENUM enSwType, const string &strFileName) : TRACE_SAVE_BASE_C(strFileName), objDbgSw_(nh)
    {
        enSwType_    = enSwType;
    }

    void Upt(void);
    
private:
    DBG_SW_C         objDbgSw_;
    DBG_SW_TYPE_ENUM enSwType_;
};

/**************************************************************************************
功能描述: 保存小车路径
修改记录:
**************************************************************************************/
class CAR_ROUTE_SAVE_ROS_C : protected TRACE_SAVE_BASE_ROS_C, protected CAR_ROUTE_SAVE_C
{
public:
    CAR_ROUTE_SAVE_ROS_C(ros::NodeHandle &nh, const string &strFileName = "route.txt") 
                        : TRACE_SAVE_BASE_C(strFileName),
                          TRACE_SAVE_BASE_ROS_C(nh, DBG_SW_TYPE_SAVE_ROUTE, strFileName),
                          CAR_ROUTE_SAVE_C(strFileName)
    {
    }

    void PushEx(const COORDINATE_2D_STRU &stPos);
    
private:
};

/**************************************************************************************
功能描述: 保存小车位姿轨迹
修改记录:
**************************************************************************************/
class CAR_POSE_TRJ_SAVE_ROS_C : protected TRACE_SAVE_BASE_ROS_C, protected CAR_POSE_TRJ_SAVE_C
{
public:
    CAR_POSE_TRJ_SAVE_ROS_C(ros::NodeHandle &nh, const string &strFileName = "pose_trj.txt") 
                        : TRACE_SAVE_BASE_C(strFileName),
                          TRACE_SAVE_BASE_ROS_C(nh, DBG_SW_TYPE_SAVE_ROUTE, strFileName),
                          CAR_POSE_TRJ_SAVE_C(strFileName)
    {
    }

    void PushEx(const COOR_2D_POSE_STRU &stPose);
    
private:
};

/**************************************************************************************
功能描述: 保存小车路径
修改记录:
**************************************************************************************/
class ROUTES_SAVER_C
{
public:
    ROUTES_SAVER_C(ros::NodeHandle &nh) : objLocReceiver_(nh),
                                          objFrontWheelRouteSaver_(nh, "route_f.txt"),
                                          objBackWheelRouteSaver_( nh, "route_b.txt"),
                                          objCarPoseTrjSaver_( nh, "pose_trj.txt")
    {
    }
    
    void PeriodUpt(void);
    
private:
    DYNC_CAR_C              objCar_;
    LOCATION_MSG_RECEIVER_C objLocReceiver_;
    CAR_ROUTE_SAVE_ROS_C    objFrontWheelRouteSaver_;
    CAR_ROUTE_SAVE_ROS_C    objBackWheelRouteSaver_;
    CAR_POSE_TRJ_SAVE_ROS_C objCarPoseTrjSaver_;
};

/**************************************************************************************
功能描述: 保存底盘控制信息
修改记录:
**************************************************************************************/
class DSP_CTRL_INFO_ROS_SAVER_C : protected TRACE_SAVE_BASE_ROS_C, protected DSP_CTRL_INFO_SAVER_C
{
public:
    DSP_CTRL_INFO_ROS_SAVER_C(ros::NodeHandle &nh, const string &strFileName = "trc_dsp_ctrl.txt")
                        : TRACE_SAVE_BASE_C(strFileName),
                          TRACE_SAVE_BASE_ROS_C(nh, DBG_SW_TYPE_SAVE_ROUTE, strFileName),
                          DSP_CTRL_INFO_SAVER_C(strFileName)
    {
    }

    void PushEx(const MOVE_CTRL_INFO_STRU &stSrcCtrl, const MOVE_CTRL_INFO_STRU &stSmoothCtrl, float actualLinearVel, float actualTurnAngle);
    
};

#endif

