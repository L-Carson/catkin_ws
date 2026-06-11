#ifndef __COMM_CFG_PARAM_INTERFACE_H__
#define __COMM_CFG_PARAM_INTERFACE_H__

#include "comm/comm_cfg/comm_cfg_param_define.h"
#include "comm/comm_base.h"
#include <string>
#include <mutex>



/**************************************************************************************
功能描述: 配置参数管理
修改记录:
**************************************************************************************/
class CFG_PARAM_MANAGER_C
{
public:
    CFG_PARAM_MANAGER_C(void) {};
    CFG_PARAM_MANAGER_C(const std::string &strCfgFileDir): strCfgParamFileDir_(strCfgFileDir) {};

    int Init(void);

    const CFG_PARAM_STRUCTURE_INFO_STRU        &GetStructureInfoCfg(void) const;
    const COORDINATE_2D_STRU                   &GetAlongEdgeRefPointCfg(void) const;
    const CFG_PARAM_DCU_INFO_STRU              &GetDcuInfoCfg(void) const;
    const CFG_PARAM_WHEEL_SPEED_SENSOR_STRU    &GetWheelSpeedSensorCfg(void) const;
    const CFG_PARAM_STEERING_ANGLE_SENSOR_STRU &GetSteeringAngleSensorCfg(void) const;
    const CFG_PARAM_VEL_SMOOTHER_STRU          &GetVelSmootherCfg(void) const;
    const CFG_PARAM_VEL_LIMIT_STRU             &GetVelLimitCfg(void) const;

    const CFG_PARAM_GLOBAL_PLAN_STRU           &GetGlobalPlanCfg(void) const;
    const CFG_PARAM_TASK_MNG_STRU              &GetTaskMngCfg(void) const;
    const CFG_PARAM_LOCAL_PLAN_STRU            &GetLocalPlanCfg(void) const;
    const CFG_PARAM_LINEAR_VEL_PLAN_STRU       &GetLinearVelPLanCfg(void) const;
    const CFG_PARAM_TELE_CTRL_STRU             &GetTeleCtrlCfg(void) const;
    const CFG_PARAM_PLAN_TIME_PARAM_STRU       &GetPlanTimeParamCfg(void) const;
    const CFG_PARAM_PLAN_FUNCTION_PARAM_STRU   &GetPlanFunctionParamCfg(void) const;

private:
    const std::string           strCfgParamFileDir_;

    static std::mutex           objInitMutex_;
    static bool                 isInitSucc_;
    static CFG_PARAM_STRU       stCfgParam_;
};



#endif

