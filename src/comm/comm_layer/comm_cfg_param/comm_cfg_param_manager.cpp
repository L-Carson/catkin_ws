
#include "comm/comm_cfg/comm_cfg_param_file.h"
#include "comm/comm_cfg/comm_cfg_param_manager.h"
#include <string>

/**************************************************************************************
功能描述: 静态变量定义
修改记录:
**************************************************************************************/
std::mutex     CFG_PARAM_MANAGER_C::objInitMutex_;
bool           CFG_PARAM_MANAGER_C::isInitSucc_               = false;
CFG_PARAM_STRU CFG_PARAM_MANAGER_C::stCfgParam_               = {{0}};

/**************************************************************************************
功能描述: 初始化
          多线程时如何处理? 初始化中读文件本身有锁，此处暂处理
修改记录:
**************************************************************************************/
int CFG_PARAM_MANAGER_C::Init(void)
{
    ST_LOG_INFO("Begin...");
    std::lock_guard<std::mutex> objLocker(objInitMutex_);

    if (isInitSucc_) {
        return 0;
    }

    COMM::CFG::CFG_PARAM_FILE_C objCfgParamFile(strCfgParamFileDir_);
    int rslt = objCfgParamFile.Read(stCfgParam_);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgParamFile.Read() Fail.");
        return -1;
    }

    isInitSucc_ = true;
    ST_LOG_INFO("Succ.");
    return 0;
}


const COORDINATE_2D_STRU &CFG_PARAM_MANAGER_C::GetAlongEdgeRefPointCfg(void) const
{
    return stCfgParam_.stVehicleBody.stAlongEdgeRefPoint;
}

const CFG_PARAM_DCU_INFO_STRU &CFG_PARAM_MANAGER_C::GetDcuInfoCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stDcuInfo;
}

const CFG_PARAM_WHEEL_SPEED_SENSOR_STRU &CFG_PARAM_MANAGER_C::GetWheelSpeedSensorCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stWheelSpeedSensor;
}

const CFG_PARAM_STEERING_ANGLE_SENSOR_STRU &CFG_PARAM_MANAGER_C::GetSteeringAngleSensorCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stSteeringAngleSensor;
}

const CFG_PARAM_VEL_SMOOTHER_STRU &CFG_PARAM_MANAGER_C::GetVelSmootherCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stVelSmoother;
}

const CFG_PARAM_VEL_LIMIT_STRU &CFG_PARAM_MANAGER_C::GetVelLimitCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stVelLimit;
}

const CFG_PARAM_GLOBAL_PLAN_STRU &CFG_PARAM_MANAGER_C::GetGlobalPlanCfg(void) const
{
    return stCfgParam_.stPlan.stGlobalPlan;
}

const CFG_PARAM_TASK_MNG_STRU &CFG_PARAM_MANAGER_C::GetTaskMngCfg(void) const
{
    return stCfgParam_.stPlan.stTaskMng;
}

const CFG_PARAM_LOCAL_PLAN_STRU &CFG_PARAM_MANAGER_C::GetLocalPlanCfg(void) const
{
    return stCfgParam_.stPlan.stLocalPlan;
}

const CFG_PARAM_LINEAR_VEL_PLAN_STRU &CFG_PARAM_MANAGER_C::GetLinearVelPLanCfg(void) const
{
    return stCfgParam_.stPlan.stLinearVelPlan;
}

const CFG_PARAM_TELE_CTRL_STRU &CFG_PARAM_MANAGER_C::GetTeleCtrlCfg(void) const
{
    return stCfgParam_.stPlan.stTeleCtrl;
}

const CFG_PARAM_PLAN_TIME_PARAM_STRU &CFG_PARAM_MANAGER_C::GetPlanTimeParamCfg(void) const
{
    return stCfgParam_.stPlan.stTimeParam;
}

const CFG_PARAM_PLAN_FUNCTION_PARAM_STRU &CFG_PARAM_MANAGER_C::GetPlanFunctionParamCfg(void) const
{
    return stCfgParam_.stPlan.stFunctionParam;
}



