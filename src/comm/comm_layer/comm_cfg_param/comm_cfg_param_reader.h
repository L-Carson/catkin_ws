#ifndef __COMM_CFG_PARAM_READ_PNC_H__
#define __COMM_CFG_PARAM_READ_PNC_H__

#include "comm_cfg_param_define.h"
#include "comm_cfg_param_base_reader.h"

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
class CFG_PARAM_READER_C : public CFG_PARAM_READER_BASE_C
{
public:
    int ReadVehicleBodyCfg(const TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_BODY_STRU &stVehicleBodyCfg) const;
    int ReadVehicleChassisCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_CHASSIS_STRU &stVehicleChssisCfg) const;
    int ReadPlanCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PLAN_STRU &stPlanCfg) const;

private:
    /**************************************************************************************
    功能描述: 读取车身结构参数
    修改记录:
    **************************************************************************************/
    int ReadStructureInfoCfg(const TiXmlElement *pobjVehicleBodyCfgParam, CFG_PARAM_STRUCTURE_INFO_STRU &stStuctrueInfo) const;
    int ReadAlongEdgeRefPointCfg(const TiXmlElement *pobjVehicleBodyCfgParam, COORDINATE_2D_STRU &stAlongEdgeRefPointCfg) const;

    /**************************************************************************************
    功能描述: 读取底盘硬件参数
    修改记录:
    **************************************************************************************/
    int ReadNavIpcTypeCfg(TiXmlElement *pobjVehicleChassisCfgParam, string& strNavIpcType) const;
    int ReadDcuInfoCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_DCU_INFO_STRU &stDcuInfoCfg) const;
    int ReadWheelSpeedSensorCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_WHEEL_SPEED_SENSOR_STRU &stWheelSpeedSensorCfg) const;
    int ReadSteeringAngleSensorCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_STEERING_ANGLE_SENSOR_STRU &stSteeringAngleSensorCfg) const;
    int ReadVelSmootherCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_VEL_SMOOTHER_STRU &stVelSmootherCfg) const;
    int ReadVelLimitCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg) const;

    /**************************************************************************************
    功能描述: 读取规划算法参数
    修改记录:
    **************************************************************************************/
    int ReadGlobalPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_GLOBAL_PLAN_STRU &stGlobalPlanCfg) const;
    int ReadTaskMngCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_TASK_MNG_STRU &stTaskMngCfg) const;
    int ReadLocalPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_LOCAL_PLAN_STRU &stLocalPlanCfg) const;
    int ReadLinearVelPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_LINEAR_VEL_PLAN_STRU &stLinearVelPlanCfg) const;
    int ReadTeleCtrlCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_TELE_CTRL_STRU &stTeleCtrlCfg) const;
    int ReadPlanTimeParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_TIME_PARAM_STRU &stTimeParamCfg) const;
    int ReadPlanFunctionParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_FUNCTION_PARAM_STRU &stFunctionParamCfg) const;
};


}

}



#endif

