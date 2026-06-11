#ifndef __COMM_CFG_PARAM_READ_PNC_H__
#define __COMM_CFG_PARAM_READ_PNC_H__

#include "comm_cfg_param_define.h"
#include "comm_cfg_param_read_base.h"

#include "comm/comm_base.h"
#include "comm/loc_xml/tinyxml.h"



namespace COMM
{

namespace CFG
{


/**************************************************************************************
功能描述: 规划配置参数读取
修改记录:
**************************************************************************************/
class CFG_PARAM_READ_PNC_C : public CFG_PARAM_READ_BASE_C
{
public:
    INT32 ReadVehicleBodyCfg(const TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_BODY_STRU &stVehicleBodyCfg) const;
    INT32 ReadVehicleChassisCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_CHASSIS_STRU &stVehicleChssisCfg) const;
    INT32 ReadPlanCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PLAN_STRU &stPlanCfg) const;

private:
    /**************************************************************************************
    功能描述: 读取车身结构参数
    修改记录:
    **************************************************************************************/
    INT32 ReadStructureInfoCfg(const TiXmlElement *pobjVehicleBodyCfgParam, CFG_PARAM_STRUCTURE_INFO_STRU &stStuctrueInfo) const;
    INT32 ReadAlongEdgeRefPointCfg(const TiXmlElement *pobjVehicleBodyCfgParam, COORDINATE_2D_STRU &stAlongEdgeRefPointCfg) const;

    /**************************************************************************************
    功能描述: 读取底盘硬件参数
    修改记录:
    **************************************************************************************/
    INT32 ReadNavIpcTypeCfg(TiXmlElement *pobjVehicleChassisCfgParam, string& strNavIpcType) const;
    INT32 ReadDcuInfoCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_DCU_INFO_STRU &stDcuInfoCfg) const;
    INT32 ReadWheelSpeedSensorCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_WHEEL_SPEED_SENSOR_STRU &stWheelSpeedSensorCfg) const;
    INT32 ReadSteeringAngleSensorCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_STEERING_ANGLE_SENSOR_STRU &stSteeringAngleSensorCfg) const;
    INT32 ReadVelSmootherCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_VEL_SMOOTHER_STRU &stVelSmootherCfg) const;
    INT32 ReadVelLimitCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg) const;

    /**************************************************************************************
    功能描述: 读取规划算法参数
    修改记录:
    **************************************************************************************/
    INT32 ReadGlobalPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_GLOBAL_PLAN_STRU &stGlobalPlanCfg) const;
    INT32 ReadTaskMngCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_TASK_MNG_STRU &stTaskMngCfg) const;
    INT32 ReadLocalPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_LOCAL_PLAN_STRU &stLocalPlanCfg) const;
    INT32 ReadLinearVelPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_LINEAR_VEL_PLAN_STRU &stLinearVelPlanCfg) const;
    INT32 ReadTeleCtrlCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_TELE_CTRL_STRU &stTeleCtrlCfg) const;
    INT32 ReadPlanTimeParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_TIME_PARAM_STRU &stTimeParamCfg) const;
    INT32 ReadPlanFunctionParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_FUNCTION_PARAM_STRU &stFunctionParamCfg) const;
    INT32 ReadPlanChargeParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_CHARGE_PARAM_STRU &stChargeParamCfg) const;
};


}

}



#endif

