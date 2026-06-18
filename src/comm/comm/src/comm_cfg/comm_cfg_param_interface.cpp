
#include "comm/comm_cfg/comm_cfg_param_file.h"
#include "comm/comm_cfg/comm_cfg_param_interface.h"
#include "comm/comm_base.h"
#include <string>

/**************************************************************************************
功能描述: 静态变量定义
修改记录:
**************************************************************************************/
std::mutex     CFG_PARAM_C::objInitMutex_;
BOOL           CFG_PARAM_C::isInitSucc_               = false;
BOOL           CFG_PARAM_C::isInitLocMappingCfgSucc_  = false;
BOOL           CFG_PARAM_C::isInitExtrinsicCheckCfgSucc_  = false;
CFG_PARAM_STRU CFG_PARAM_C::stCfgParam_               = {{0}};

/**************************************************************************************
功能描述: 初始化
          多线程时如何处理? 初始化中读文件本身有锁，此处暂处理
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_C::Init(void)
{
    ST_LOG_INFO("Begin...");
    std::lock_guard<std::mutex> objLocker(objInitMutex_);

    if (isInitSucc_) {
        return 0;
    }

    COMM::CFG::CFG_PARAM_FILE_C objCfgParamFile(strCfgParamFileDir_);
    INT32 rslt = objCfgParamFile.Read(stCfgParam_);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgParamFile.Read() Fail.");
        return -1;
    }

    isInitSucc_ = true;
    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 CFG_PARAM_C::InitLocMappingCfg(void)
{
    ST_LOG_INFO("Begin...");
    std::lock_guard<std::mutex> objLocker(objInitMutex_);

    if (isInitSucc_ || isInitLocMappingCfgSucc_) {
        ST_LOG_INFO("Inited Before.");
        return 0;
    }

    COMM::CFG::CFG_PARAM_FILE_C objCfgParamFile(strCfgParamFileDir_);
    INT32 rslt = objCfgParamFile.ReadLocMappingCfg(stCfgParam_);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgParamFile.ReadLocMappingCfg() Fail.");
        return -1;
    }

    isInitLocMappingCfgSucc_ = true;
    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 CFG_PARAM_C::InitExtrinsicCheckCfg(void)
{
    ST_LOG_INFO("Begin...");
    std::lock_guard<std::mutex> objLocker(objInitMutex_);

    if (isInitSucc_ || isInitExtrinsicCheckCfgSucc_) {
        ST_LOG_INFO("Inited Before.");
        return 0;
    }

    COMM::CFG::CFG_PARAM_FILE_C objCfgParamFile(strCfgParamFileDir_);
    INT32 rslt = objCfgParamFile.ReadLidarCameraExCfg(stCfgParam_);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgParamFile.ReadLidarCameraExCfg() Fail.");
        return -1;
    }

    isInitExtrinsicCheckCfgSucc_ = true;
    ST_LOG_INFO("Succ.");
    return 0;
}

BOOL CFG_PARAM_C::IsSupportGnss(void) const
{
    return stCfgParam_.stGnss.isSupport;
}

BOOL CFG_PARAM_C::IsGnssDualAntenna(void) const
{
    return stCfgParam_.stGnss.isDualAntenna;
}

BOOL CFG_PARAM_C::IsSupportImu(void) const
{
    return stCfgParam_.stImu.isSupport;
}

BOOL CFG_PARAM_C::IsSupportLocLidar(void) const
{
    return stCfgParam_.stLocLidar.isSupport;
}

const CFG_PARAM_LIDARS_STRU &CFG_PARAM_C::GetLidarsCfgInfo(void) const
{
    return stCfgParam_.stLidars;
}

// 获取指定雷达的配置信息
const CFG_PARAM_LIDAR_STRU *CFG_PARAM_C::GetLidarCfgParam(const std::string &strLidarName) const
{
    for (const CFG_PARAM_LIDAR_STRU &stLidarCfg : stCfgParam_.stLidars.vstLidarsCfg)
        if (strLidarName == stLidarCfg.LidarName())
            return &stLidarCfg;

    return nullptr;
}

const CFG_PARAM_INSTALL_STRU &CFG_PARAM_C::GetLidarsInstallOffsetParam(void) const
{
    return stCfgParam_.stLidars.stInstallOffsetParam;
}

BOOL CFG_PARAM_C::IsSupportBackDepthCamera(void) const
{
    return stCfgParam_.stBackDepthCamera.isSupport;
}

BOOL CFG_PARAM_C::IsSupportDoubleLidarReflectVerify(void) const
{
    return stCfgParam_.stPnp.stReflectVerify.stDoubleLidarVerify.isSupport;
}

BOOL CFG_PARAM_C::IsSupportEmergenceReflectVerify(void) const
{
    return stCfgParam_.stPnp.stReflectVerify.stEmergenceVerify.isSupport;
}

BOOL CFG_PARAM_C::IsSupportRoadmarkSalientVerify(void) const
{
    return stCfgParam_.stPnp.stReflectVerify.stRoadmarkVerify.isSupport;
}

BOOL CFG_PARAM_C::IsSupportShortObstHold(void) const
{
    return stCfgParam_.stPnp.stShortObstHold.isSupport;
}

BOOL CFG_PARAM_C::IsSupportCommonObstHold(void) const
{
    return stCfgParam_.stPnp.stCommonObstHold.isSupport;
}

BOOL CFG_PARAM_C::IsSupportCluster(void) const
{
    return stCfgParam_.stPnp.stCluster.isSupport;
}

BOOL CFG_PARAM_C::IsSupportPcdNet(void) const
{
    return stCfgParam_.stPnp.stNet.isSupport;
}

BOOL CFG_PARAM_C::IsSupportCarBodyDenoising(void) const
{
    return stCfgParam_.stPnp.stCarBodyDenoising.isSupport;
}

BOOL CFG_PARAM_C::IsSupportCleanliness(void) const
{
    return stCfgParam_.stPnp.stCleanliness.isSupport;
}


GNSS_PROTOCOL_TYPE_ENUM CFG_PARAM_C::GetGnssProtocolType(void) const
{
    return stCfgParam_.stGnss.enProtocolType;
}

std::string CFG_PARAM_C::GetGnssDevName(void) const
{
    return stCfgParam_.stGnss.strDevName;
}

INT32  CFG_PARAM_C::GetGnssBaudRate(void) const
{
    return stCfgParam_.stGnss.baudRate;
}

IMU_TYPE_ENUM CFG_PARAM_C::GetImuType(void) const
{
    return stCfgParam_.stImu.enType;
}

std::string CFG_PARAM_C::GetImuDevName(void) const
{
    return stCfgParam_.stImu.strDevName;
}

INT32 CFG_PARAM_C::GetImuBaudRate(void) const
{
    return stCfgParam_.stImu.baudRate;
}

std::string CFG_PARAM_C::GetImuRawTopic(void) const
{
    return stCfgParam_.stImu.strImuRawTopic;
}

BOOL CFG_PARAM_C::IsModifyImuGyroZ(void) const
{
    return stCfgParam_.stImu.isModifyGyroZ;
}

const CFG_PARAM_INSTALL_STRU &CFG_PARAM_C::GetGnssInstallPose(void) const
{
    return stCfgParam_.stGnss.stInstallPose;
}

const CFG_PARAM_INSTALL_STRU &CFG_PARAM_C::GetImuInstallPose(void) const
{
    return stCfgParam_.stImu.stInstallPose;
}

const CFG_PARAM_INSTALL_STRU &CFG_PARAM_C::GetLocLidarInstallPose(void) const
{
    return stCfgParam_.stLocLidar.stInstallPose;
}

const CFG_PARAM_INSTALL_POS_STRU &CFG_PARAM_C::GetBackDepthCameraInstallCfg(void) const
{
    return stCfgParam_.stBackDepthCamera.stInstallPos;
}

const CFG_PARAM_PLANE_FIT_STRU &CFG_PARAM_C::GetBackDepthCameraPlaneFitCfg(void) const
{
    return stCfgParam_.stBackDepthCamera.stPlaneFitParam;
}

const CFG_PARAM_ULTRASONICS_STRU &CFG_PARAM_C::GetUltrasonicsCfg(void) const
{
    return stCfgParam_.stUltrasonics;
}

const CFG_PARAM_TOFS_STRU &CFG_PARAM_C::GetTofsCfg(void) const
{
    return stCfgParam_.stTofs;
}

const CFG_PARAM_RGB_CAMERAS_STRU &CFG_PARAM_C::GetRgbCamerasCfg(void) const
{
    return stCfgParam_.stRgbCameras;
}

const CFG_PARAM_RADAR_STRU &CFG_PARAM_C::GetRadarCfg(void) const
{
    return stCfgParam_.stRadar;
}

const BOOL &CFG_PARAM_C::IsSupportPnp(void) const
{
    return stCfgParam_.stPnp.isSupport;
}

const CFG_PARAM_CAR_BODY_VXL_FILTER_STRU &CFG_PARAM_C::GetCarBodyVxlFilterCfg(void) const
{
    return stCfgParam_.stPnp.stCarBodyVxlFilter;
}

const CFG_PARAM_GROUND_SEG_STRU &CFG_PARAM_C::GetGroundSegCfg(void) const
{
    return stCfgParam_.stPnp.stGroundSeg;
}

const CFG_PARAM_PCPT_REGION_STRU &CFG_PARAM_C::GetPcptRegionCfg(void) const
{
    return stCfgParam_.stPnp.stPcptRegion;
}

const CFG_PARAM_PCPT_CLOUD_PERSISTENCE_STRU &CFG_PARAM_C::GetPcptCloudPersistenceCfg(void) const
{
    return stCfgParam_.stPnp.stPcptCloudPersistence;
}
const CFG_PARAM_LAYER_HEIGHT_STRU &CFG_PARAM_C::GetSeg2dObstCfg(void) const
{
    return stCfgParam_.stPnp.stLayerHeight;
}

const CFG_PARAM_MUTLTIFRAME_STRU &CFG_PARAM_C::GetMultiframeCfg(void) const
{
    return stCfgParam_.stPnp.stMultiframe;
}

const CFG_PARAM_DETECT_PILE_STRU &CFG_PARAM_C::GetDetectPileCfg(void) const
{
    return stCfgParam_.stPnp.stDetectPile;
}

const CFG_PARAM_OBST_STATE_STRU &CFG_PARAM_C::GetObstStateCfg(void) const
{
    return stCfgParam_.stPnp.stObstState;
}

const CFG_PARAM_TRAFFIC_LIGHT_STRU &CFG_PARAM_C::GetTrafficLightCfg(void) const
{
    return stCfgParam_.stPnp.stTrafficLight;
}

const CFG_PARAM_ACCESSIBLE_AREA_STRU &CFG_PARAM_C::GetAccessibleAreaCfg(void) const
{
    return stCfgParam_.stPnp.stAccessibleArea;
}

const CFG_PARAM_SLOPE_PAD_DET_STRU &CFG_PARAM_C::GetSlopePadDetCfg(void) const
{
    return stCfgParam_.stPnp.stSlopePadDet;
}

const CFG_PARAM_ROAD_REGION_FILTER_STRU &CFG_PARAM_C::GetRoadRegionFilter(void) const
{
    return stCfgParam_.stPnp.stRoadRegionFilter;
}

const CFG_PARAM_FILTER_POINTS_REBUILD_STRU &CFG_PARAM_C::GetFilterPointsRebuildCfg(void) const
{
    return stCfgParam_.stPnp.stFilterPointsRebuild;
}

const CFG_PARAM_WAREHOUSE_STRU &CFG_PARAM_C::GetWarehouseCfg(void) const
{
    return stCfgParam_.stPnp.stWarehouse;
}

const CFG_PARAM_CAR_BODY_DENOSING_STRU &CFG_PARAM_C::GetCarBodyDenosingCfg(void) const
{
    return stCfgParam_.stPnp.stCarBodyDenoising;
}

const CFG_PARAM_CLEANLINESS_STRU &CFG_PARAM_C::GetCleanlinessCfg(void) const
{
    return stCfgParam_.stPnp.stCleanliness;
}


const CFG_PARAM_DOUBLE_LIDAR_VERIFY_STRU &CFG_PARAM_C::GetDoubleLidarReflectVerifyCfg(void) const
{
    return stCfgParam_.stPnp.stReflectVerify.stDoubleLidarVerify;
}

const CFG_PARAM_EMERGENCE_VERIFY_STRU &CFG_PARAM_C::GetEmergenceReflectVerifyCfg(void) const
{
    return stCfgParam_.stPnp.stReflectVerify.stEmergenceVerify;
}

const CFG_PARAM_ROADMARK_VERIFY_STRU &CFG_PARAM_C::GetRoadmarkSalientVerifyCfg(void) const
{
    return stCfgParam_.stPnp.stReflectVerify.stRoadmarkVerify;
}

const CFG_PARAM_SHORT_OBST_HOLD_STRU &CFG_PARAM_C::GetShortObstHoldCfg(void) const
{
    return stCfgParam_.stPnp.stShortObstHold;
}

const CFG_PARAM_COMMON_OBST_HOLD_STRU &CFG_PARAM_C::GetCommonObstHoldCfg(void) const
{
    return stCfgParam_.stPnp.stCommonObstHold;
}

const CFG_PARAM_PLANE_SEG_STRU &CFG_PARAM_C::GetPlaneSegCfg(void) const
{
    return stCfgParam_.stPnp.stPlaneSeg;
}

const CFG_PARAM_CLUSTER_STRU &CFG_PARAM_C::GetClusterCfg(void) const
{
    return stCfgParam_.stPnp.stCluster;
}

const CFG_PARAM_VISION_FUNC_STRU &CFG_PARAM_C::GetVisionFuncCfg(void) const
{
    return stCfgParam_.stVisionFunc;
}

const CFG_PARAM_STRUCTURE_INFO_STRU &CFG_PARAM_C::GetStructureInfoCfg(void) const
{
    return stCfgParam_.stVehicleBody.stStructrueInfo;
}

const COORDINATE_2D_STRU &CFG_PARAM_C::GetAlongEdgeRefPointCfg(void) const
{
    return stCfgParam_.stVehicleBody.stAlongEdgeRefPoint;
}

const CFG_PARAM_DCU_INFO_STRU &CFG_PARAM_C::GetDcuInfoCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stDcuInfo;
}

const CFG_PARAM_WHEEL_SPEED_SENSOR_STRU &CFG_PARAM_C::GetWheelSpeedSensorCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stWheelSpeedSensor;
}

const CFG_PARAM_STEERING_ANGLE_SENSOR_STRU &CFG_PARAM_C::GetSteeringAngleSensorCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stSteeringAngleSensor;
}

const CFG_PARAM_VEL_SMOOTHER_STRU &CFG_PARAM_C::GetVelSmootherCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stVelSmoother;
}

const CFG_PARAM_VEL_LIMIT_STRU &CFG_PARAM_C::GetVelLimitCfg(void) const
{
    return stCfgParam_.stVehicleChassis.stVelLimit;
}

const CFG_PARAM_GLOBAL_PLAN_STRU &CFG_PARAM_C::GetGlobalPlanCfg(void) const
{
    return stCfgParam_.stPlan.stGlobalPlan;
}

const CFG_PARAM_TASK_MNG_STRU &CFG_PARAM_C::GetTaskMngCfg(void) const
{
    return stCfgParam_.stPlan.stTaskMng;
}

const CFG_PARAM_LOCAL_PLAN_STRU &CFG_PARAM_C::GetLocalPlanCfg(void) const
{
    return stCfgParam_.stPlan.stLocalPlan;
}

const CFG_PARAM_LINEAR_VEL_PLAN_STRU &CFG_PARAM_C::GetLinearVelPLanCfg(void) const
{
    return stCfgParam_.stPlan.stLinearVelPlan;
}

const CFG_PARAM_TELE_CTRL_STRU &CFG_PARAM_C::GetTeleCtrlCfg(void) const
{
    return stCfgParam_.stPlan.stTeleCtrl;
}

const CFG_PARAM_PLAN_TIME_PARAM_STRU &CFG_PARAM_C::GetPlanTimeParamCfg(void) const
{
    return stCfgParam_.stPlan.stTimeParam;
}

const CFG_PARAM_PLAN_FUNCTION_PARAM_STRU &CFG_PARAM_C::GetPlanFunctionParamCfg(void) const
{
    return stCfgParam_.stPlan.stFunctionParam;
}

const CFG_PARAM_PLAN_CHARGE_PARAM_STRU &CFG_PARAM_C::GetPlanChargeParamCfg(void) const
{
    return stCfgParam_.stPlan.stChargeParam;
}

const CFG_PARAM_LOC_STRU &CFG_PARAM_C::GetLocCfg(void) const
{
    return stCfgParam_.stLoc;
}

FUSION_METHOD_ENUM &CFG_PARAM_C::GetFusionMethod(void) const
{
    return stCfgParam_.stLoc.stPoseEstimator.enFusionMethod;
}

MOTION_MODEL_ENUM &CFG_PARAM_C::GetMotionModel(void)   const
{
    return stCfgParam_.stLoc.stPoseEstimator.enMotionModel;
}
OBS_MODEL_ENUM &CFG_PARAM_C::GetLidarObsModel(void) const
{
    return stCfgParam_.stLoc.stPoseEstimator.enLidarObsModel;
}
OBS_MODEL_ENUM &CFG_PARAM_C::GetGnssObsModel(void)  const
{
    return stCfgParam_.stLoc.stPoseEstimator.enGnssObsModel;
}

MATCH_METHOD_ENUM &CFG_PARAM_C::GetLidarLocMatchMethod(void) const
{
    return stCfgParam_.stLoc.stLidarLoc.enMatchMethod;
}

float CFG_PARAM_C::GetLidarLocVoxelGridSize(void) const
{
    return stCfgParam_.stLoc.stLidarLoc.voxelGridSize;
}

float CFG_PARAM_C::GetLidarLocFitnessScoreThreshold(void) const
{
    return stCfgParam_.stLoc.stLidarLoc.fitnessScoreThreshold;
}

const CFG_PARAM_NDT_MATCHER_STRU &CFG_PARAM_C::GetLidarLocNdtCfg(void) const
{
    return stCfgParam_.stLoc.stLidarLoc.stNdtMatcher;
}

const vector<string> &CFG_PARAM_C::GetLidarLocLidarList(void) const
{
    return stCfgParam_.stLoc.stLidarLoc.vstrLidars;
}

float  CFG_PARAM_C::GetRelocFitnessScoreThreshold(void) const
{
    return stCfgParam_.stLoc.stReloc.fitnessScoreThreshold;
}

float  CFG_PARAM_C::GetRelocVoxelGridSize(void) const
{
    return stCfgParam_.stLoc.stReloc.voxelGridSize;
}

BOOL CFG_PARAM_C::IsSupportStationReloc(void)
{
    return stCfgParam_.stLoc.stReloc.stRawReloc.isSupportStationReloc;
}

BOOL CFG_PARAM_C::IsSupportRecordReloc(void)
{
    return stCfgParam_.stLoc.stReloc.stRawReloc.isSupportRecordReloc;
}

BOOL CFG_PARAM_C::IsSupportGnssReloc(void)
{
    return stCfgParam_.stLoc.stReloc.stRawReloc.isSupportGnssReloc;
}

BOOL CFG_PARAM_C::IsSupportScanContextReloc(void)
{
    return stCfgParam_.stLoc.stReloc.stRawReloc.isSupportScanContextReloc;
}

BOOL CFG_PARAM_C::IsSupportImuAxisZReloc(void)
{
    return stCfgParam_.stLoc.stReloc.stRawReloc.isSupportImuAxisZReloc;
}

const CFG_PARAM_SCAN_CONTEXT_STRU &CFG_PARAM_C::GetRelocScanContextCfg(void) const
{
    return stCfgParam_.stLoc.stReloc.stRawReloc.stScanContext;
}

const CFG_PARAM_NDT_MATCHER_STRU &CFG_PARAM_C::GetReLocNdtCfg(void) const
{
    return stCfgParam_.stLoc.stReloc.stFineReloc.stNdtMatcher;
}

const CFG_PARAM_SEARCH_REG_STRU &CFG_PARAM_C::GetReLocSearchRegion(void) const
{
    return stCfgParam_.stLoc.stReloc.stFineReloc.stSearchRegion;
}

const CFG_PARAM_SEARCH_STEP_STRU &CFG_PARAM_C::GetReLocSearchStep(void) const
{
    return stCfgParam_.stLoc.stReloc.stFineReloc.stSearchStep;
}

const CFG_PARAM_VEHICLE_LOC_STRU &CFG_PARAM_C::GetVehicleLocCfg(void) const
{
    return stCfgParam_.stVehicleLoc;
}

BOOL CFG_PARAM_C::IsEnableNewLocSys(void) const
{
    return stCfgParam_.stVehicleLoc.enable;
}

const CFG_PARAM_DATAPROC_STRU &CFG_PARAM_C::GetDataProcCfg(void) const
{
    return stCfgParam_.stMapping.stDataProc;
}

const CFG_PARAM_FRONTEND_STRU &CFG_PARAM_C::GetFrontEndCfg(void) const
{
    return stCfgParam_.stMapping.stFrontEnd;
}

const CFG_PARAM_LO_FRONTEND_STRU &CFG_PARAM_C::GetLOFrontEndCfg(void) const
{
    return stCfgParam_.stMapping.stFrontEnd.stLOFrontEnd;
}

const CFG_PARAM_LIO_FRONTEND_STRU &CFG_PARAM_C::GetLIOFrontEndCfg(void) const
{
    return stCfgParam_.stMapping.stFrontEnd.stLIOFrontEnd;
}

const CFG_PARAM_BACKEND_STRU &CFG_PARAM_C::GetBackEndCfg(void) const
{
    return stCfgParam_.stMapping.stBackEnd;
}

const CFG_PARAM_LOOPCLOSE_STRU &CFG_PARAM_C::GetLoopCloseCfg(void) const
{
    return stCfgParam_.stMapping.stLoopClose;
}




