#ifndef __COMM_CFG_PARAM_INTERFACE_H__
#define __COMM_CFG_PARAM_INTERFACE_H__

#include "comm/comm_cfg/comm_cfg_param_define.h"
#include "comm/comm_base.h"
#include <string>
#include <mutex>



/**************************************************************************************
功能描述: 配置参数（提供参数缓存的读取接口，与文件无关）
修改记录:
**************************************************************************************/
class CFG_PARAM_C
{
public:
    CFG_PARAM_C(void) {};
    CFG_PARAM_C(const std::string &strCfgFileDir): strCfgParamFileDir_(strCfgFileDir) {};

    INT32 Init(void);
    INT32 InitLocMappingCfg(void);
    INT32 InitExtrinsicCheckCfg(void);

    BOOL  IsSupportGnss(void) const;
    BOOL  IsGnssDualAntenna(void) const;

    BOOL  IsSupportImu(void) const;
    BOOL  IsSupportLocLidar(void) const;

    const CFG_PARAM_LIDARS_STRU  &GetLidarsCfgInfo(void) const;
    const CFG_PARAM_LIDAR_STRU   *GetLidarCfgParam(const std::string &strLidarName) const;
    const CFG_PARAM_INSTALL_STRU &GetLidarsInstallOffsetParam(void) const;

    BOOL  IsSupportBackDepthCamera(void) const;
    BOOL  IsSupportDoubleLidarReflectVerify(void) const;
    BOOL  IsSupportEmergenceReflectVerify(void) const;
    BOOL  IsSupportRoadmarkSalientVerify(void) const;
    BOOL  IsSupportShortObstHold(void) const;
    BOOL  IsSupportCommonObstHold(void) const;
    BOOL  IsSupportCluster(void) const;
    BOOL  IsSupportPcdNet(void) const;
    BOOL  IsSupportCarBodyDenoising(void) const;
    BOOL  IsSupportCleanliness(void) const;

    GNSS_PROTOCOL_TYPE_ENUM GetGnssProtocolType(void) const;
    std::string             GetGnssDevName(void)    const;
    INT32                   GetGnssBaudRate(void)   const;

    IMU_TYPE_ENUM GetImuType(void) const;
    std::string   GetImuDevName(void)    const;
    INT32         GetImuBaudRate(void)   const;
    std::string   GetImuRawTopic(void)   const;
    BOOL          IsModifyImuGyroZ(void) const;

    const CFG_PARAM_INSTALL_STRU               &GetGnssInstallPose(void) const;
    const CFG_PARAM_INSTALL_STRU               &GetImuInstallPose(void) const;
    const CFG_PARAM_INSTALL_STRU               &GetLocLidarInstallPose(void) const;

    const CFG_PARAM_INSTALL_POS_STRU           &GetBackDepthCameraInstallCfg(void) const;
    const CFG_PARAM_PLANE_FIT_STRU             &GetBackDepthCameraPlaneFitCfg(void) const;

    const CFG_PARAM_ULTRASONICS_STRU           &GetUltrasonicsCfg(void) const;
    const CFG_PARAM_TOFS_STRU                  &GetTofsCfg(void) const;
    const CFG_PARAM_RGB_CAMERAS_STRU           &GetRgbCamerasCfg(void) const;
    const CFG_PARAM_RADAR_STRU                 &GetRadarCfg(void) const;

    const BOOL                                 &IsSupportPnp(void) const;
    const CFG_PARAM_CAR_BODY_VXL_FILTER_STRU   &GetCarBodyVxlFilterCfg(void) const;
    const CFG_PARAM_GROUND_SEG_STRU            &GetGroundSegCfg(void) const;
    const CFG_PARAM_PCPT_REGION_STRU           &GetPcptRegionCfg(void) const;
    const CFG_PARAM_DOUBLE_LIDAR_VERIFY_STRU   &GetDoubleLidarReflectVerifyCfg(void) const;
    const CFG_PARAM_EMERGENCE_VERIFY_STRU      &GetEmergenceReflectVerifyCfg(void) const;
    const CFG_PARAM_ROADMARK_VERIFY_STRU       &GetRoadmarkSalientVerifyCfg(void) const;
    const CFG_PARAM_SHORT_OBST_HOLD_STRU       &GetShortObstHoldCfg(void) const;
    const CFG_PARAM_COMMON_OBST_HOLD_STRU      &GetCommonObstHoldCfg(void) const;
    const CFG_PARAM_PLANE_SEG_STRU             &GetPlaneSegCfg(void) const;
    const CFG_PARAM_CLUSTER_STRU               &GetClusterCfg(void) const;
    const CFG_PARAM_LAYER_HEIGHT_STRU          &GetSeg2dObstCfg(void) const;
    const CFG_PARAM_MUTLTIFRAME_STRU           &GetMultiframeCfg(void) const;
    const CFG_PARAM_DETECT_PILE_STRU           &GetDetectPileCfg(void) const;
    const CFG_PARAM_OBST_STATE_STRU            &GetObstStateCfg(void) const;
    const CFG_PARAM_TRAFFIC_LIGHT_STRU         &GetTrafficLightCfg(void) const;
    const CFG_PARAM_ACCESSIBLE_AREA_STRU       &GetAccessibleAreaCfg(void) const;
    const CFG_PARAM_SLOPE_PAD_DET_STRU         &GetSlopePadDetCfg(void) const;
    const CFG_PARAM_ROAD_REGION_FILTER_STRU    &GetRoadRegionFilter(void) const;
    const CFG_PARAM_FILTER_POINTS_REBUILD_STRU &GetFilterPointsRebuildCfg(void) const;
    const CFG_PARAM_WAREHOUSE_STRU             &GetWarehouseCfg(void) const;
    const CFG_PARAM_CAR_BODY_DENOSING_STRU     &GetCarBodyDenosingCfg(void) const;
    const CFG_PARAM_CLEANLINESS_STRU           &GetCleanlinessCfg(void) const;
    

    const CFG_PARAM_VISION_FUNC_STRU           &GetVisionFuncCfg(void) const;

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
    const CFG_PARAM_PLAN_CHARGE_PARAM_STRU     &GetPlanChargeParamCfg(void) const;

    FUSION_METHOD_ENUM                         &GetFusionMethod(void) const;
    MOTION_MODEL_ENUM                          &GetMotionModel(void)   const;
    OBS_MODEL_ENUM                             &GetLidarObsModel(void) const;
    OBS_MODEL_ENUM                             &GetGnssObsModel(void)  const;

    MATCH_METHOD_ENUM                          &GetLidarLocMatchMethod(void) const;
    float                                       GetLidarLocVoxelGridSize(void) const;
    float                                       GetLidarLocFitnessScoreThreshold(void) const;
    const CFG_PARAM_NDT_MATCHER_STRU           &GetLidarLocNdtCfg(void) const;
    const std::vector<std::string>             &GetLidarLocLidarList(void) const;
    const CFG_PARAM_LOC_STRU                   &GetLocCfg(void) const;

    float                                       GetRelocFitnessScoreThreshold(void) const;
    float                                       GetRelocVoxelGridSize(void) const;
    BOOL                                        IsSupportStationReloc(void);
    BOOL                                        IsSupportRecordReloc(void);
    BOOL                                        IsSupportGnssReloc(void);
    BOOL                                        IsSupportScanContextReloc(void);
    BOOL                                        IsSupportImuAxisZReloc(void);
    const CFG_PARAM_SCAN_CONTEXT_STRU          &GetRelocScanContextCfg(void) const;
    const CFG_PARAM_NDT_MATCHER_STRU           &GetReLocNdtCfg(void) const;
    const CFG_PARAM_SEARCH_REG_STRU            &GetReLocSearchRegion(void) const;
    const CFG_PARAM_SEARCH_STEP_STRU           &GetReLocSearchStep(void) const;

    const CFG_PARAM_VEHICLE_LOC_STRU           &GetVehicleLocCfg(void) const;
    BOOL                                        IsEnableNewLocSys(void) const;

    const CFG_PARAM_DATAPROC_STRU              &GetDataProcCfg(void)    const;
    const CFG_PARAM_FRONTEND_STRU              &GetFrontEndCfg(void)    const;
    const CFG_PARAM_LO_FRONTEND_STRU           &GetLOFrontEndCfg(void)  const;
    const CFG_PARAM_LIO_FRONTEND_STRU          &GetLIOFrontEndCfg(void) const;
    const CFG_PARAM_BACKEND_STRU               &GetBackEndCfg(void)     const;
    const CFG_PARAM_LOOPCLOSE_STRU             &GetLoopCloseCfg(void)   const;

private:
    const std::string           strCfgParamFileDir_;

    static std::mutex           objInitMutex_;
    static BOOL                 isInitSucc_;
    static BOOL                 isInitLocMappingCfgSucc_;
    static BOOL                 isInitExtrinsicCheckCfgSucc_;
    static CFG_PARAM_STRU       stCfgParam_;                // 配置参数
};



#endif

