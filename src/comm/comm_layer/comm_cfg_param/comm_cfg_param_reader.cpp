
#include "comm/comm_cfg/comm_cfg_param_read_pnc.h"


namespace COMM
{

namespace CFG
{



int CFG_PARAM_READER_C::ReadStructureInfoCfg(const TiXmlElement *pobjVehicleBodyCfgParam, CFG_PARAM_STRUCTURE_INFO_STRU &stStuctrueInfo) const
{
    const TiXmlElement *pobjStructureInfo = pobjVehicleBodyCfgParam->FirstChildElement("StructureInfo");
    if (pobjStructureInfo == NULL) {
        ST_LOG_ERR("Get <StructureInfo> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_STRING(pobjStructureInfo, "StructureInfo", "model", stStuctrueInfo.strModel);

    ST_LOG_INFO("<StructureInfo> Cfg: <model> = %s.", stStuctrueInfo.strModel.c_str());
    return 0;
}

/**************************************************************************************
功能描述: 读取沿边参考点配置
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadAlongEdgeRefPointCfg(const TiXmlElement *pobjVehicleBodyCfgParam, COORDINATE_2D_STRU &stAlongEdgeRefPointCfg) const
{
    const TiXmlElement *pobjAlongEdgeRefPointCfg = pobjVehicleBodyCfgParam->FirstChildElement("AlongEdgeRefPoint");
    if (pobjAlongEdgeRefPointCfg == NULL) {
        ST_LOG_ERR("Get <AlongEdgeRefPoint> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjAlongEdgeRefPointCfg, "AlongEdgeRefPoint", "x", stAlongEdgeRefPointCfg.x);
    READ_ATTR_FLOAT(pobjAlongEdgeRefPointCfg, "AlongEdgeRefPoint", "y", stAlongEdgeRefPointCfg.y);

    ST_LOG_INFO("<AlongEdgeRefPoint> Cfg: <x> = %f, <y> = %f.", stAlongEdgeRefPointCfg.x, stAlongEdgeRefPointCfg.y);
    return 0;
}

/**************************************************************************************
功能描述: 读取读取车身参数配置
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadVehicleBodyCfg(const TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_BODY_STRU &stVehicleBodyCfg) const
{
    const TiXmlElement *pobjVehicleBodyCfg = pobjCfgParam->FirstChildElement("VehicleBody");
    if (pobjVehicleBodyCfg == NULL) {
        ST_LOG_ERR("Get <VehicleBody> Cfg Element Fail.");
        return -1;
    }

    READ_CFG(ReadStructureInfoCfg,     pobjVehicleBodyCfg, stVehicleBodyCfg.stStructrueInfo);
    READ_CFG(ReadAlongEdgeRefPointCfg, pobjVehicleBodyCfg, stVehicleBodyCfg.stAlongEdgeRefPoint);

    ST_LOG_INFO("Read <VehicleBody> Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 读取计算平台-导航工控机类型
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadNavIpcTypeCfg(TiXmlElement *pobjVehicleChassisCfgParam, string& strNavIpcType) const
{
    TiXmlElement *pobjNavIpcTypeCfg = pobjVehicleChassisCfgParam->FirstChildElement("ComputingPlatform");
    if (pobjNavIpcTypeCfg == NULL) {
        ST_LOG_ERR("Get <ComputingPlatform> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_STRING(pobjNavIpcTypeCfg, "ComputingPlatform", "NavIpcType", strNavIpcType);

    ST_LOG_INFO("<ComputingPlatform> Cfg: <NavIpcType> = %s.", strNavIpcType.c_str());
    return 0;
}

/**************************************************************************************
功能描述: 读取DCU信息
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadDcuInfoCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_DCU_INFO_STRU &stDcuInfoCfg) const
{
    TiXmlElement *pobjDcuInfoCfg = pobjVehicleChassisCfgParam->FirstChildElement("DcuInfo");
    if (pobjDcuInfoCfg == NULL) {
        ST_LOG_ERR("Get <DcuInfo> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_STRING(pobjDcuInfoCfg, "DcuInfo", "SerialDevName", stDcuInfoCfg.strSerialDevName);
    if (TIXML_SUCCESS != pobjWheelSpeedSensorCfg->QueryBoolAttribute("EnableCarPitch", &stDcuInfoCfg.enbleCarPitch)) {
        stDcuInfoCfg.enbleCarPitch = true;
        ST_LOG_ERR("Get <DcuInfo> Cfg Attr <EnableCarPitch> Fail.");
    }

    ST_LOG_INFO("<DcuInfo> Cfg: <SerialDevName> = %s  <EnableCarPitch> = %d.", stDcuInfoCfg.strSerialDevName.c_str(), stDcuInfoCfg.enbleCarPitch);
    return 0;
}

/**************************************************************************************
功能描述: 读取底盘轮速计参数配置
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadWheelSpeedSensorCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_WHEEL_SPEED_SENSOR_STRU &stWheelSpeedSensorCfg) const
{
    TiXmlElement *pobjWheelSpeedSensorCfg = pobjVehicleChassisCfgParam->FirstChildElement("WheelSpeedSensor");
    if (pobjWheelSpeedSensorCfg == NULL) {
        ST_LOG_ERR("Get <WheelSpeedSensor> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjWheelSpeedSensorCfg->QueryFloatAttribute("ScaleFactor", &stWheelSpeedSensorCfg.scaleFactor)) {
        ST_LOG_ERR("Get <WheelSpeedSensor> Cfg Attr <ScaleFactor> Fail.");
        return -1;
    }

    ST_LOG_INFO("<WheelSpeedSensor> Cfg: <ScaleFactor> = %f.", stWheelSpeedSensorCfg.scaleFactor);
    return 0;
}

/**************************************************************************************
功能描述: 读取转向角传感器参数配置
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadSteeringAngleSensorCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_STEERING_ANGLE_SENSOR_STRU &stSteeringAngleSensorCfg) const
{
    TiXmlElement *pobjSteeringAngleSensorCfg = pobjVehicleChassisCfgParam->FirstChildElement("SteeringAngleSensor");
    if (pobjSteeringAngleSensorCfg == NULL) {
        ST_LOG_ERR("Get <SteeringAngleSensor> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjSteeringAngleSensorCfg->QueryFloatAttribute("ScaleFactor", &stSteeringAngleSensorCfg.scaleFactor)) {
        ST_LOG_ERR("Get <SteeringAngleSensor> Cfg Attr <ScaleFactor> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjSteeringAngleSensorCfg->QueryFloatAttribute("OffsetFactor", &stSteeringAngleSensorCfg.offsetFactor)) {
        ST_LOG_ERR("Get <SteeringAngleSensor> Cfg Attr <OffsetFactor> Fail.");
        return -1;
    }

    ST_LOG_INFO("<SteeringAngleSensor> Cfg: <ScaleFactor> = %f, <OffsetFactor> = %f.", stSteeringAngleSensorCfg.scaleFactor, stSteeringAngleSensorCfg.offsetFactor);
    return 0;
}

/**************************************************************************************
功能描述: 读取速度平滑器参数
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadVelSmootherCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_VEL_SMOOTHER_STRU &stVelSmootherCfg) const
{
    TiXmlElement *pobjVelSmootherCfg = pobjVehicleChassisCfgParam->FirstChildElement("VelSmoother");
    if (pobjVelSmootherCfg == NULL) {
        ST_LOG_ERR("Get <VelSmoother> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjVelSmootherCfg, "VelSmoother", "forwardAcc",    stVelSmootherCfg.forwardAcc);
    READ_ATTR_FLOAT(pobjVelSmootherCfg, "VelSmoother", "forwardDec",    stVelSmootherCfg.forwardDec);
    READ_ATTR_FLOAT(pobjVelSmootherCfg, "VelSmoother", "backwardAcc",   stVelSmootherCfg.backwardAcc);
    READ_ATTR_FLOAT(pobjVelSmootherCfg, "VelSmoother", "backwardDec",   stVelSmootherCfg.backwardDec);

    ST_LOG_INFO("<VelSmoother> Cfg: <forwardAcc> = %f, <forwardDec> = %f, <backwardAcc> = %f, <backwardDec> = %f.",
                stVelSmootherCfg.forwardAcc,
                stVelSmootherCfg.forwardDec,
                stVelSmootherCfg.backwardAcc,
                stVelSmootherCfg.backwardDec);
    return 0;
}

/**************************************************************************************
功能描述: 读限速配置
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadVelLimitCfg(TiXmlElement *pobjVehicleChassisCfgParam, CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg) const
{
    TiXmlElement *pobjVelLimitCfg = pobjVehicleChassisCfgParam->FirstChildElement("VelLimit");
    if (pobjVelLimitCfg == NULL) {
        ST_LOG_ERR("Get <VelLimit> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjVelLimitCfg, "VelLimit", "maxForwardVel",     stVelLimitCfg.maxForwardVel);
    READ_ATTR_FLOAT(pobjVelLimitCfg, "VelLimit", "maxBackwardVelAbs", stVelLimitCfg.maxBackwardVelAbs);
    READ_ATTR_FLOAT(pobjVelLimitCfg, "VelLimit", "maxCleanVelAbs",    stVelLimitCfg.maxCleanVelAbs);

    ST_LOG_INFO("<VelLimit> Cfg: <maxForwardVel> = %f, <maxBackwardVelAbs> = %f, <maxCleanVelAbs> = %f.",
                stVelLimitCfg.maxForwardVel,
                stVelLimitCfg.maxBackwardVelAbs,
                stVelLimitCfg.maxCleanVelAbs);
    return 0;
}


/**************************************************************************************
功能描述: 读取底盘参数配置
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadVehicleChassisCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_CHASSIS_STRU &stVehicleChssisCfg) const
{
    TiXmlElement *pobjVehicleChassisCfg = pobjCfgParam->FirstChildElement("VehicleChassis");
    if (pobjVehicleChassisCfg == NULL) {
        ST_LOG_ERR("Get <VehicleChassis> Cfg Element Fail.");
        return -1;
    }

    READ_CFG(ReadNavIpcTypeCfg,          pobjVehicleChassisCfg, stVehicleChssisCfg.strNavIpcType);
    READ_CFG(ReadDcuInfoCfg,             pobjVehicleChassisCfg, stVehicleChssisCfg.stDcuInfo);
    READ_CFG(ReadWheelSpeedSensorCfg,    pobjVehicleChassisCfg, stVehicleChssisCfg.stWheelSpeedSensor);
    READ_CFG(ReadSteeringAngleSensorCfg, pobjVehicleChassisCfg, stVehicleChssisCfg.stSteeringAngleSensor);
    READ_CFG(ReadVelSmootherCfg,         pobjVehicleChassisCfg, stVehicleChssisCfg.stVelSmoother);
    READ_CFG(ReadVelLimitCfg,            pobjVehicleChassisCfg, stVehicleChssisCfg.stVelLimit);

    ST_LOG_INFO("Read <VehicleChassis> Succ.");
    return 0;
}

int CFG_PARAM_READER_C::ReadGlobalPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_GLOBAL_PLAN_STRU &stGlobalPlanCfg) const
{
    TiXmlElement *pobjGlobalPlanCfg = pobjPlanCfgParam->FirstChildElement("GloalPlan");
    if (pobjGlobalPlanCfg == NULL) {
        ST_LOG_ERR("Get <GloalPlan> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjGlobalPlanCfg, "GloalPlan", "transDistOfCloseEdgeRouteForNonCleaning", stGlobalPlanCfg.transDistOfCloseEdgeRouteForNonCleaning);
    READ_ATTR_BOOL(pobjGlobalPlanCfg,  "GloalPlan", "isSupportTurnAround",                     stGlobalPlanCfg.isSupportTurnAround);
    READ_ATTR_BOOL(pobjGlobalPlanCfg,  "GloalPlan", "isSupportRoadPortTurnAround",             stGlobalPlanCfg.isSupportRoadPortTurnAround);
    READ_ATTR_FLOAT(pobjGlobalPlanCfg, "GloalPlan", "skipExecuteLen",                          stGlobalPlanCfg.skipExecuteLen);
    if (stGlobalPlanCfg.skipExecuteLen < 1.0) {
        ST_LOG_ERR("skipExecuteLen %f is too small, use min value %f.", stGlobalPlanCfg.skipExecuteLen, 1.0);
        stGlobalPlanCfg.skipExecuteLen = 1.0;
    }

    READ_ATTR_FLOAT(pobjGlobalPlanCfg, "GloalPlan", "minTurnAroundRoadWidth",                  stGlobalPlanCfg.minTurnAroundRoadWidth);

    ST_LOG_INFO("<GloalPlan> Cfg: <transDistOfCloseEdgeRouteForNonCleaning> = %f.", stGlobalPlanCfg.transDistOfCloseEdgeRouteForNonCleaning);
    ST_LOG_INFO("<GloalPlan> Cfg: <isSupportTurnAround> = %u.",                     stGlobalPlanCfg.isSupportTurnAround);
    ST_LOG_INFO("<GloalPlan> Cfg: <isSupportRoadPortTurnAround> = %u.",             stGlobalPlanCfg.isSupportRoadPortTurnAround);
    ST_LOG_INFO("<GloalPlan> Cfg: <skipExecuteLen> = %f.",                          stGlobalPlanCfg.skipExecuteLen);
    ST_LOG_INFO("<GloalPlan> Cfg: <minTurnAroundRoadWidth> = %f.",                  stGlobalPlanCfg.minTurnAroundRoadWidth);

    return 0;
}

int CFG_PARAM_READER_C::ReadTaskMngCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_TASK_MNG_STRU &stTaskMngCfg) const
{
    TiXmlElement *pobjTaskMngCfg = pobjPlanCfgParam->FirstChildElement("TaskMng");
    if (pobjTaskMngCfg == NULL) {
        ST_LOG_ERR("Get <TaskMng> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_UINT32(pobjTaskMngCfg, "TaskMng", "defaultAlgoOfPatrol", stTaskMngCfg.defaultAlgoOfPatrol);
    if (TIXML_SUCCESS != pobjTaskMngCfg->QueryFloatAttribute("emptyTrashMoveDist", &stTaskMngCfg.emptyTrashMoveDist))
    {
        ST_LOG_ERR("缺省 <TaskMng> emptyTrashMoveDist.");
        stTaskMngCfg.emptyTrashMoveDist = 0.0f;
    }
    ST_LOG_INFO("<TaskMng> Cfg: defaultAlgoOfPatrol[%u] emptyTrashMoveDist[%f].", stTaskMngCfg.defaultAlgoOfPatrol, stTaskMngCfg.emptyTrashMoveDist);
    return 0;
}

/**************************************************************************************
功能描述: 读取局部规划参数
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadLocalPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_LOCAL_PLAN_STRU &stLocalPlanCfg) const
{
    TiXmlElement *pobjLocalPlanCfg = pobjPlanCfgParam->FirstChildElement("LocalPlan");
    if (pobjLocalPlanCfg == NULL) {
        ST_LOG_ERR("Get <LocalPlan> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjLocalPlanCfg, "LocalPlan", "stopFaceSafeDist",                   stLocalPlanCfg.stopFaceSafeDist);
    READ_ATTR_FLOAT(pobjLocalPlanCfg, "LocalPlan", "parkFaceSafeDist",                   stLocalPlanCfg.parkFaceSafeDist);
    READ_ATTR_FLOAT(pobjLocalPlanCfg, "LocalPlan", "maxSideSafeDistWhenAvoidAlongRoute", stLocalPlanCfg.maxSideSafeDistWhenAvoidAlongRoute);
    READ_ATTR_FLOAT(pobjLocalPlanCfg, "LocalPlan", "rangeDistGarbageBinPose",            stLocalPlanCfg.rangeDistGarbageBinPose);
    READ_ATTR_FLOAT(pobjLocalPlanCfg, "LocalPlan", "trashBoxCtrlThresh",                 stLocalPlanCfg.trashBoxCtrlThresh);
    READ_ATTR_FLOAT(pobjLocalPlanCfg, "LocalPlan", "brushShortenLen",                    stLocalPlanCfg.brushShortenLen);

    ST_LOG_INFO("<LocalPlan> Cfg: <stopFaceSafeDist> = %f, <parkFaceSafeDist> = %f, <maxSideSafeDistWhenAvoidAlongRoute> = %f,"
                "<rangeDistGarbageBinPose> = %f, <trashBoxCtrlThresh> = %f, <brushShortenLen> = %f.",
                stLocalPlanCfg.stopFaceSafeDist,
                stLocalPlanCfg.parkFaceSafeDist,
                stLocalPlanCfg.maxSideSafeDistWhenAvoidAlongRoute,
                stLocalPlanCfg.rangeDistGarbageBinPose,
                stLocalPlanCfg.trashBoxCtrlThresh,
                stLocalPlanCfg.brushShortenLen);
    return 0;
}

/**************************************************************************************
功能描述: 读取线速度规划参数
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadLinearVelPlanCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_LINEAR_VEL_PLAN_STRU &stLinearVelPlanCfg) const
{
    TiXmlElement *pobjLinearVelPlanCfg = pobjPlanCfgParam->FirstChildElement("LinearVelPlan");
    if (pobjLinearVelPlanCfg == NULL) {
        ST_LOG_ERR("Get <LinearVelPlan> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjLinearVelPlanCfg, "LinearVelPlan", "MaxAngularVel",      stLinearVelPlanCfg.maxAngularVel);
    READ_ATTR_FLOAT(pobjLinearVelPlanCfg, "LinearVelPlan", "SmoothDec",          stLinearVelPlanCfg.smoothDec);
    READ_ATTR_FLOAT(pobjLinearVelPlanCfg, "LinearVelPlan", "VelCtrlDelayFactor", stLinearVelPlanCfg.velCtrlDelayFactor);

    ST_LOG_INFO("<LinearVelPlan> Cfg: <MaxAngularVel> = %f, <SmoothDec> = %f, <VelCtrlDelayFactor> = %f.",
                stLinearVelPlanCfg.maxAngularVel,
                stLinearVelPlanCfg.smoothDec,
                stLinearVelPlanCfg.velCtrlDelayFactor);
    return 0;
}

int CFG_PARAM_READER_C::ReadTeleCtrlCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_TELE_CTRL_STRU &stTeleCtrlCfg) const
{
    TiXmlElement *pobjTeleCtrlCfg = pobjPlanCfgParam->FirstChildElement("TeleCtrl");
    if (pobjTeleCtrlCfg == NULL) {
        ST_LOG_ERR("Get <TeleCtrl> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_BOOL(pobjTeleCtrlCfg, "TeleCtrl", "isEmergeStopSmooth", stTeleCtrlCfg.isEmergeStopSmooth);

    ST_LOG_INFO("<TeleCtrl> Cfg: <isEmergeStopSmooth> = %d.", stTeleCtrlCfg.isEmergeStopSmooth);
    return 0;
}

int CFG_PARAM_READER_C::ReadPlanTimeParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_TIME_PARAM_STRU &stTimeParamCfg) const
{
    TiXmlElement *pobjTimeParamCfg = pobjPlanCfgParam->FirstChildElement("TimeParam");
    if (pobjTimeParamCfg == NULL) {
        ST_LOG_ERR("Get <TimeParam> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_FLOAT(pobjTimeParamCfg, "TimeParam", "waitEscapeTime", stTimeParamCfg.waitEscapeTime);

    ST_LOG_INFO("<TimeParam> Cfg: <waitEscapeTime> = %.1f.", stTimeParamCfg.waitEscapeTime);
    return 0;
}

int CFG_PARAM_READER_C::ReadPlanFunctionParamCfg(TiXmlElement *pobjPlanCfgParam, CFG_PARAM_PLAN_FUNCTION_PARAM_STRU &stFunctionParamCfg) const
{
    TiXmlElement *pobjFunctionParamCfg = pobjPlanCfgParam->FirstChildElement("FunctionParam");
    if (pobjFunctionParamCfg == NULL) {
        ST_LOG_ERR("Get <FunctionParam> Cfg Element Fail.");
        return -1;
    }

    READ_ATTR_BOOL(pobjFunctionParamCfg, "FunctionParam", "isDoorCloseRequire", stFunctionParamCfg.isDoorCloseRequire);
    READ_ATTR_BOOL(pobjFunctionParamCfg, "FunctionParam", "isDynamicObstStop", stFunctionParamCfg.isDynamicObstStop);
    READ_ATTR_BOOL(pobjFunctionParamCfg, "FunctionParam", "isSmartCtrlBrush", stFunctionParamCfg.isSmartCtrlBrush);

    ST_LOG_INFO("<FunctionParam> Cfg: <isDoorCloseRequire> = %u.", stFunctionParamCfg.isDoorCloseRequire);
    ST_LOG_INFO("<FunctionParam> Cfg: <isDynamicObstStop> = %u.", stFunctionParamCfg.isDynamicObstStop);
    ST_LOG_INFO("<FunctionParam> Cfg: <isSmartCtrlBrush> = %u.", stFunctionParamCfg.isSmartCtrlBrush);
    return 0;
}

/**************************************************************************************
功能描述: 读取规划配置参数
修改记录:
**************************************************************************************/
int CFG_PARAM_READER_C::ReadPlanCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PLAN_STRU &stPlanCfg) const
{
    TiXmlElement *pobjPlanCfg = pobjCfgParam->FirstChildElement("Plan");
    if (pobjPlanCfg == NULL) {
        ST_LOG_ERR("Get <Plan> Cfg Element Fail.");
        return -1;
    }

    READ_CFG(ReadGlobalPlanCfg,    pobjPlanCfg, stPlanCfg.stGlobalPlan);
    READ_CFG(ReadTaskMngCfg,       pobjPlanCfg, stPlanCfg.stTaskMng);
    READ_CFG(ReadLocalPlanCfg,     pobjPlanCfg, stPlanCfg.stLocalPlan);
    READ_CFG(ReadLinearVelPlanCfg, pobjPlanCfg, stPlanCfg.stLinearVelPlan);
    READ_CFG(ReadTeleCtrlCfg,      pobjPlanCfg, stPlanCfg.stTeleCtrl);
    READ_CFG(ReadPlanTimeParamCfg, pobjPlanCfg, stPlanCfg.stTimeParam);
    READ_CFG(ReadPlanFunctionParamCfg, pobjPlanCfg, stPlanCfg.stFunctionParam);


    ST_LOG_INFO("Read <Plan> Succ.");
    return 0;
}



}

}

