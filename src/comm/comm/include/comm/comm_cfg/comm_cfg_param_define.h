#ifndef __COMM_CFG_PARAM_DEFINE_H__
#define __COMM_CFG_PARAM_DEFINE_H__

#include "../comm_base.h"

#include <list>
#include <vector>
#include <unordered_map>

/**
 * @brief 安装位姿参数结构
 *
 */
struct CFG_PARAM_INSTALL_STRU {
    float tx;        // [unit: m]
    float ty;        // [unit: m]
    float tz;        // [unit: m]
    float roll;      // [unit: deg]
    float pitch;     // [unit: deg]
    float yaw;       // [unit: deg]

    bool IsZero() const {
        if (SIG(tx) == 0   || SIG(ty) == 0    || SIG(tz) == 0 ||
            SIG(roll) == 0 || SIG(pitch) == 0 || SIG(yaw) == 0) {
            return true;
        } else {
            return false;
        }
    }
};

/**
 * @brief 安装位置参数结构
 *
 */
struct CFG_PARAM_INSTALL_POS_STRU {
    float tx;
    float ty;
    float tz;
};

/**
 * @brief 拟合的平面参数结构
 *
 */
struct CFG_PARAM_PLANE_FIT_STRU {
    float coeffY;
    float coeffZ;
    float constH;
};

typedef enum GNSS_PROTOCOL_TYPE_ENUM
{
    GNSS_PROTOCOL_TYPE_NMEA = 0,
    GNSS_PROTOCOL_TYPE_UBX  = 1,
} GNSS_PROTOCOL_TYPE_ENUM;

struct CFG_PARAM_GNSS_STRU {
    BOOL                        isSupport;
    GNSS_PROTOCOL_TYPE_ENUM     enProtocolType;
    BOOL                        isDualAntenna;
    std::string                 strDevName;
    INT32                       baudRate;
    CFG_PARAM_INSTALL_STRU      stInstallPose;
};

typedef enum IMU_TYPE_ENUM
{
    IMU_TYPE_YIS   = 0,
    IMU_TYPE_CH    = 1,
    IMU_TYPE_LIVOX = 2,
    IMU_TYPE_AIRY  = 3,
} IMU_TYPE_ENUM;

struct CFG_PARAM_IMU_STRU {
    BOOL                    isSupport;
    IMU_TYPE_ENUM           enType;
    std::string             strDevName;
    INT32                   baudRate;
    BOOL                    isModifyGyroZ;
    CFG_PARAM_INSTALL_STRU  stInstallPose;
    std::string             strImuRawTopic;
};

struct CFG_PARAM_ULTRASONIC_STRU {
    bool                    isSupport;
    uint32_t                id;
    std::string             strName;
    std::string             strType;
    CFG_PARAM_INSTALL_STRU  stInstallPose;
};

struct CFG_PARAM_ULTRASONICS_STRU {
    BOOL                    isSupport{false};
    std::string             strDevName;
    INT32                   baudRate;
    vector<CFG_PARAM_ULTRASONIC_STRU> vstUs;
};

struct CFG_PARAM_TOF_STRU {
    bool                    isSupport;
    uint32_t                baudRate;
    std::string             strName;
    std::string             strPortName;
    CFG_PARAM_INSTALL_STRU  stInstallPose;
};

struct CFG_PARAM_TOFS_STRU {
    BOOL                        isSupport{false};
    vector<CFG_PARAM_TOF_STRU>  vstTof;
};

struct CFG_PARAM_RGB_CAMERA_STRU {
    bool            isSupport{false};
    std::string     strName;
    std::string     strPortName;
    uint32_t        imgHeight;
    uint32_t        imgWidth;
    uint32_t        frameHz;
    vector<double>  cameraMatrix;           // 内参
    vector<double>  distortionCoefficients; // 畸变参数
    CFG_PARAM_INSTALL_STRU stInstallPose;   // 外参：安装参数

    BOOL IsCalibration() const {
        if (stInstallPose.IsZero()) return false;   //外参全为0，未标定

        //内参是否全为0
        bool isCameraMatrixZero(true);
        for (const double& value : cameraMatrix) {
            if (SIG(value) != 0) {
                isCameraMatrixZero = false;
                break;
            }
        }
        if (isCameraMatrixZero) return false;       // 内参全为0，未标定

        //畸变是否全为0
        bool isDistortionZero(true);
        for (const double& value : distortionCoefficients) {
            if (SIG(value) != 0) {
                isDistortionZero = false;
                break;
            }
        }
        if (isDistortionZero) return false;         // 畸变全为0，未标定

        return true;
    }
};

struct CFG_PARAM_RGB_CAMERAS_STRU {
    vector<CFG_PARAM_RGB_CAMERA_STRU> vstCamera;
};

struct CFG_PARAM_LOC_LIDAR_STRU {
    BOOL                    isSupport;
    CFG_PARAM_INSTALL_STRU  stInstallPose;
};

/**
 * @brief 尾部深度相机参数结构
 *
 */
struct CFG_PARAM_BACK_DEPTH_CAMERA_STRU {
    BOOL                        isSupport;
    CFG_PARAM_INSTALL_POS_STRU  stInstallPos;     // 安装位置
    CFG_PARAM_PLANE_FIT_STRU    stPlaneFitParam;  // 拟合的平面参数
};

struct CFG_PARAM_HEAD_LIDAR_CAR_FILTER_STRU {
    TRI_PRISM_STRU  stCarHeadFilterParam;
    CUBOID_STRU     stCarTailtopFilterParam;
    CUBOID_STRU     stCarTailbottomFilterParam;
};

struct CFG_PARAM_MIDDLE_LIDAR_CAR_FILTER_STRU {
    PYRAMID_STRU       stCarFilterParam;
    CUBOID_STRU        stLidarRackFilterParam;
};

/**
 * @brief laser点云扇形过滤参数结构
 *
 */
struct CFG_PARAM_LASER_SECTION_WIPE_STRU {
    float startAngle;
    float endAngle;
    float minDist;
    float maxDist;
    float minHeight;
    float maxHeight;
};

struct CFG_PARAM_LIDAR_SECTION_WIPE_STRU {
    vector<CFG_PARAM_LASER_SECTION_WIPE_STRU> vstLaserSectionWipe;
};

/**
 * @brief laser点云波束过滤参数结构
 *
 */
struct CFG_PARAM_LASER_BEAM_WIPE_STRU {
    unsigned int beginRing;
    unsigned int endRing;
    unsigned int beginColumn;
    unsigned int endColumn;
};

struct CFG_PARAM_LIDAR_BEAM_WIPE_STRU {
    BOOL  isSupport;
    vector<CFG_PARAM_LASER_BEAM_WIPE_STRU> vstLaserBeamWipe;
};

/**
 * @brief lidar基本属性
 *
 */
struct CFG_PARAM_LIDAR_PROPERTY_STRU {
    std::string strLidarName;
    std::string strLidarType;
    std::string strFrameId;
    std::string strLidarSerialNum;
    BOOL        isSupportLoc;
    BOOL        isSupportPnp;
    BOOL        isSupportDustFilter;    //true-开启扬尘滤波，false-关闭扬尘滤波，默认false
    BOOL        isSupportShadowFilter;  //true-开启拖尾滤波，false-关闭拖尾滤波，默认true
    BOOL        isSupportShadowLookupTableFilter; //true-开启基于水平、垂直角度拖尾滤波，false-关闭滤波，默认false
};

/**
 * @brief lidar网络配置
 *
 */
struct CFG_PARAM_LIDAR_NET_STRU {
    std::string strIp;
    int         msopPort;
    int         difopPort;
    int         imuPort;
};

/**
 * @brief lidar测距参数
 *
 */
struct CFG_PARAM_LIDAR_DIST_STRU {
    float minDist{0.1};
    float maxDist;
};

/**
 * @brief lidar参数结构
 *
 */
struct CFG_PARAM_LIDAR_STRU {
    const std::string &LidarName() const {
        return stProperty.strLidarName;
    }

    const std::string &LidarType() const {
        return stProperty.strLidarType;
    }

    const std::string &LidarSerialNum() const {
        return stProperty.strLidarSerialNum;
    }


    const BOOL IsSupportLoc() const {
         return stProperty.isSupportLoc;
    }

    const BOOL IsSupportPnp() const {
         return stProperty.isSupportPnp;
    }

    const BOOL IsSupportDustFilter() const {
        return stProperty.isSupportDustFilter;
    }

    const BOOL IsSupportShadowFilter() const {
        return stProperty.isSupportShadowFilter;
    }
    const BOOL IsSupportShadowLookupTableFilter() const {
        return stProperty.isSupportShadowLookupTableFilter;
    }

    CFG_PARAM_LIDAR_PROPERTY_STRU       stProperty;         // 基本属性
    CFG_PARAM_LIDAR_NET_STRU            stNetParam;         // 网络配置
    CFG_PARAM_LIDAR_DIST_STRU           stDistParam;        // 距离参数
    CFG_PARAM_INSTALL_STRU              stInstallParam;     // 安装参数
    CFG_PARAM_LIDAR_SECTION_WIPE_STRU   stLidarSectionWipe; // 点云扇区过滤参数
    CFG_PARAM_LIDAR_BEAM_WIPE_STRU      stLidarBeamWipe;    // 点云波束过滤参数
};

/**
 * @brief lidar组参数结构
 *
 */
struct CFG_PARAM_LIDARS_STRU {
    CFG_PARAM_INSTALL_STRU            stInstallOffsetParam;
    std::vector<CFG_PARAM_LIDAR_STRU> vstLidarsCfg; // 雷达配置信息
};

struct CFG_PARAM_SCAN_CONTEXT_STRU {
    float maxRadius;
    INT32 numCandidate;
    float searchRatio;
    float scDistThreshold;
};

struct CFG_PARAM_RAW_RELOC_STRU {
    BOOL isSupportStationReloc;
    BOOL isSupportRecordReloc;
    BOOL isSupportGnssReloc;
    BOOL isSupportScanContextReloc;
    BOOL isSupportImuAxisZReloc;
    CFG_PARAM_SCAN_CONTEXT_STRU stScanContext;
};

struct CFG_PARAM_NDT_MATCHER_STRU {
    INT32  maxIter;
    float  stepSize;
    float  transEps;
    float  resolution;
};

struct CFG_PARAM_SEARCH_REG_STRU {
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minYaw;
    float maxYaw;
};

struct CFG_PARAM_SEARCH_STEP_STRU {
    float stepX;
    float stepY;
    float stepYaw;
};

struct CFG_PARAM_FINE_RELOC_STRU {
    CFG_PARAM_NDT_MATCHER_STRU stNdtMatcher;
    CFG_PARAM_SEARCH_REG_STRU  stSearchRegion;
    CFG_PARAM_SEARCH_STEP_STRU stSearchStep;
};

struct CFG_PARAM_RELOC_STRU {
    float voxelGridSize;
    float fitnessScoreThreshold;
    CFG_PARAM_RAW_RELOC_STRU  stRawReloc;
    CFG_PARAM_FINE_RELOC_STRU stFineReloc;
};

typedef enum MATCH_METHOD_ENUM {
    NDT    = 0,
    ICP    = 1,
} MATCH_METHOD_ENUM;

struct CFG_PARAM_LIDAR_LOC_STRU {
    std::vector<std::string>   vstrLidars;
    MATCH_METHOD_ENUM          enMatchMethod;
    CFG_PARAM_NDT_MATCHER_STRU stNdtMatcher;
    float                      voxelGridSize;
    float                      fitnessScoreThreshold;
};

typedef enum FUSION_METHOD_ENUM {
    MOVE_AVG_FILTER           =  0,
    ERROR_STATE_KALMAN_FILTER =  1,
    ERROR_STATE_KALMAN_FILTER_CA =  2,
} FUSION_METHOD_ENUM;

typedef enum MOTION_MODEL_ENUM {
    MOTION_MODEL_CV  = 0,
    MOTION_MODEL_CA  = 1,
} MOTION_MODEL_ENUM;

typedef enum OBS_MODEL_ENUM {
    OBS_MODEL_DIRECT   = 0,
    OBS_MODEL_INDIRECT = 1,
} OBS_MODEL_ENUM;

struct CFG_PARAM_POSE_ESTIMATOR_STRU {
    FUSION_METHOD_ENUM      enFusionMethod;
    MOTION_MODEL_ENUM       enMotionModel;
    OBS_MODEL_ENUM          enLidarObsModel;
    OBS_MODEL_ENUM          enGnssObsModel;
};

struct CFG_PARAM_LOC_STRU {
    CFG_PARAM_POSE_ESTIMATOR_STRU  stPoseEstimator;
    CFG_PARAM_LIDAR_LOC_STRU       stLidarLoc;
    CFG_PARAM_RELOC_STRU           stReloc;
};

//新定位模块新增
struct CFG_PARAM_COARSE_RELOC_STRU {
    BOOL enableRelocStation;
    BOOL enableRecord;
    BOOL enableGnss;
    std::vector<std::string> vstrLidarTopicList;
};

struct CFG_PARAM_VOXEL_MATCH_STRU {
    float srcCloudRes;
    float voxelMapRes;
    float minScore;
    INT32 maxSearchPointNum;
};

struct CFG_PARAM_OMPNDT_STRU {
    INT32  maxIter;
    float  stepSize;
    float  transEps;
    float  resolution;
    INT32  threadNum;
};

struct CFG_PARAM_SMALL_GICP_STRU {
    float voxelRes = 1.0;
    float transEps = 0.01;
    INT32 maxIter = 50;
    INT32 threadNum = 1;
    float maxCorrespondDist = 1.0;
    INT32 correspondenceRand = 16;
    BOOL useVGICP = false;
    BOOL verbose = false;
};

struct CFG_PARAM_NDT_STRU {
    INT32  maxIter;
    float  stepSize;
    float  transEps;
    float  resolution;
};

struct CFG_PARAM_MIXNDT_STRU {
    INT32  maxIter;
    float  stepSize;
    float  transEps;
    float  rawResolution;
    float  fineResolution;
    float  outlierThr;
    INT32  threadNum;
    float  srcCloudDsRes;
};

struct CFG_PARAM_REGISTERER_STRU {
    INT32 type;
    float srcCloudRes;
    float minScore;
    float matchThrK = 0.0;
    float matchThrB = 0.315;
    CFG_PARAM_OMPNDT_STRU stOmpNdt;
    CFG_PARAM_NDT_STRU    stNdt;
    CFG_PARAM_MIXNDT_STRU stMixNdt;
    CFG_PARAM_SMALL_GICP_STRU stSmallGicp;
};

struct CFG_PARAM_FINE_RELOC_V3_STRU {
    CFG_PARAM_SEARCH_STEP_STRU stSearchStep;
    CFG_PARAM_VOXEL_MATCH_STRU stVoxelMatch;
    CFG_PARAM_REGISTERER_STRU  stRegisterer;
};

struct CFG_PARAM_RELOC_V3_STRU {
    CFG_PARAM_COARSE_RELOC_STRU  stCoarseReloc;
    CFG_PARAM_FINE_RELOC_V3_STRU stFineReloc;
};

struct CFG_PARAM_ESKF_STRU {
    float ng;
    float na;
    float nbg;
    float nba;
};

struct CFG_PARAM_GNSS_LOC_GENERAL_STRU {
    BOOL isDualAntenna;
    CFG_PARAM_ESKF_STRU stEskf;
};

struct CFG_PARAM_GNSS_LOC_CUSTOMIZED_STRU {

};

struct CFG_PARAM_GNSS_LOC_STRU {
    INT32 type;
    CFG_PARAM_GNSS_LOC_GENERAL_STRU    stGnssLocGeneral;
    CFG_PARAM_GNSS_LOC_CUSTOMIZED_STRU stGnssLocCustomized;
};

struct CFG_PARAM_LIO_STRU {
    std::vector<std::string> vstrLidarTopicList;
};

struct CFG_PARAM_WIO_STRU {

};

struct CFG_PARAM_SMOOTHER_STRU {
    INT32 windowSize;
    float weight;
    float diffDistance = 0.65;
};

struct CFG_PARAM_VOXEL_GRID_FILTER_STRU
{
    float leafSizeX = 2.0;
    float leafSizeY = 2.0;
    float leafSizeZ = 2.0;
};

struct CFG_PARAM_ADAPTIVE_VOXEL_GRID_FILTER_STRU
{
    INT32 voxelNum = 1000;
    float leafSize = 0.5;
};

struct CFG_PARAM_DOWN_SAMPLE_FILTER_STRU {
    INT32 type = 0;
    CFG_PARAM_VOXEL_GRID_FILTER_STRU           stVgf;
    CFG_PARAM_ADAPTIVE_VOXEL_GRID_FILTER_STRU  stAvgf;
};

struct CFG_PARAM_ACTIVE_MAP_STRU {
    bool enable = false;
    float voxelMapSize = 0.0;
    int maxVoxelSize = 0;
};

struct CFG_PARAM_SCAN_MATCHER_STRU {
    float srcCloudMinRange;
    float srcCloudMaxRange;
    CFG_PARAM_ACTIVE_MAP_STRU stActMap;
    CFG_PARAM_DOWN_SAMPLE_FILTER_STRU stDsf;
    float mapLeafSize = 0.5;
    float diffDistanceThr = 0.65;

    INT32 type;
    float minScore;
    float matchThrK = 0.0;
    float matchThrB = 0.315;
    CFG_PARAM_OMPNDT_STRU stOmpNdt;
    CFG_PARAM_NDT_STRU    stNdt;
    CFG_PARAM_MIXNDT_STRU stMixNdt;
    CFG_PARAM_SMALL_GICP_STRU stSmallGicp;
};

struct CFG_PARAM_LIDAR_LOC_LIO_STRU {
    float                       correctPeriod;
    CFG_PARAM_LIO_STRU          stLio;
    CFG_PARAM_SMOOTHER_STRU     stSmoother;
    CFG_PARAM_SCAN_MATCHER_STRU stScanMatcher;
};

struct CFG_PARAM_LIDAR_LOC_DATA_PROCESS_STRU {
    float inputCloudMinRange;
    float inputCloudMaxRange;
    int inputCloudJumpNum;
};

struct CFG_PARAM_LIDAR_LOC_WIO_STRU {
    float                       correctPeriod;
    CFG_PARAM_LIDAR_LOC_DATA_PROCESS_STRU stDataProcess;
    CFG_PARAM_WIO_STRU          stWio;
    CFG_PARAM_SMOOTHER_STRU     stSmoother;
    CFG_PARAM_SCAN_MATCHER_STRU stScanMatcher;
    std::vector<std::string>    vstrLidarTopicList;
};

struct CFG_PARAM_LIDAR_LOC_V3_STRU {
    INT32 type;
    CFG_PARAM_LIDAR_LOC_LIO_STRU stLidarLocLio;
    CFG_PARAM_LIDAR_LOC_WIO_STRU stLidarLocWio;
};

struct CFG_PARAM_COMB_LOC_STRU {
    CFG_PARAM_LIDAR_LOC_V3_STRU stLidarLoc;
    CFG_PARAM_GNSS_LOC_STRU     stGnssLoc;
};

struct CFG_PARAM_VEHICLE_LOC_STRU {
    BOOL enable;
    CFG_PARAM_COMB_LOC_STRU  stCombLoc;
    CFG_PARAM_RELOC_V3_STRU  stReloc;
};

struct CFG_PARAM_MATCH_STRU {
    MATCH_METHOD_ENUM          enMatchMethod;
    float                      matchThreshold;
    CFG_PARAM_NDT_MATCHER_STRU stNdtMatcher;
};

struct CFG_PARAM_LO_FRONTEND_STRU {
    CFG_PARAM_MATCH_STRU       stMatcher;
};

struct CFG_PARAM_DATAPROC_STRU {
    INT32  satelliteNum;
    float  continueTime;
};

struct CFG_PARAM_LIO_FRONTEND_STRU {
    INT32 lineFilterSize;
    float planeThreshold;
    float lidarNoise;
};

struct CFG_PARAM_FRONTEND_STRU {
    float                       mapIncDistThreshold;
    float                       mapIncAngleThreshold;
    INT32                       mapSize;
    CFG_PARAM_LO_FRONTEND_STRU  stLOFrontEnd;
    CFG_PARAM_LIO_FRONTEND_STRU stLIOFrontEnd;
};

struct CFG_PARAM_LO_FACTOR_STRU {
    float addDistThreshold;
    float addTimeThreshold;
    float noisePos;
    float noiseAtt;
};

struct CFG_PARAM_LC_FACTOR_STRU {
    float distThreshold;
    float noisePos;
    float noiseAtt;
};

struct CFG_PARAM_GNSS_FACTOR_STRU {
    float addDistThreshold;
    float addTimeThreshold;
    float trajErrThreshold;
    float noiseXY;
    float noiseZ;
};

struct CFG_PARAM_BACKEND_STRU {
    CFG_PARAM_LO_FACTOR_STRU   stLoFactor;
    CFG_PARAM_LC_FACTOR_STRU   stLcFactor;
    CFG_PARAM_GNSS_FACTOR_STRU stGnssFactor;
};

struct CFG_PARAM_LOOPCLOSE_STRU {
    INT32                 frameDist;
    float                 globalMatchThreshold;
    CFG_PARAM_MATCH_STRU  stMatcher;
};

struct CFG_PARAM_MAPPING_STRU {
    CFG_PARAM_DATAPROC_STRU  stDataProc;
    CFG_PARAM_FRONTEND_STRU  stFrontEnd;
    CFG_PARAM_BACKEND_STRU   stBackEnd;
    CFG_PARAM_LOOPCLOSE_STRU stLoopClose;
};

struct CFG_PARAM_RADAR_DETECT_STRU {
    UINT32  devID;
    UINT32  maxObstCount;
    float   rightBottomPointX;
    float   rightBottomPointY;
    float   leftUpPointX;
    float   leftUpPointY;

    bool operator == (const UINT32& devID_) const {
        return devID == devID_;
    }

    bool operator == (const CFG_PARAM_RADAR_DETECT_STRU& stCfg) const {
        return (stCfg.devID == devID &&
                stCfg.maxObstCount == maxObstCount &&
                stCfg.rightBottomPointX == rightBottomPointX &&
                stCfg.rightBottomPointY == rightBottomPointY &&
                stCfg.leftUpPointX == leftUpPointX &&
                stCfg.leftUpPointY == leftUpPointY);
    }
};

struct CFG_PARAM_RADAR_INSTALL_STRU {
    float tx;
    float ty;
    float roll;
    float yaw;
};

struct CFG_PARAM_RADAR_DEV_STRU {
    BOOL                            isSupport;
    string                          strRadarName;
    CFG_PARAM_RADAR_DETECT_STRU     stDetectParam;
    CFG_PARAM_RADAR_INSTALL_STRU    stInstallParam;
};

struct CFG_PARAM_RADAR_STRU {
    CFG_PARAM_RADAR_DEV_STRU        stLeftRadar;
    CFG_PARAM_RADAR_DEV_STRU        stFrontRadar;
    CFG_PARAM_RADAR_DEV_STRU        stRightRadar;
    CFG_PARAM_RADAR_DEV_STRU        stBackRadar;
};

/**
 * @brief 车身体素过滤参数结构
 *
 */
struct CFG_PARAM_CAR_BODY_VXL_FILTER_STRU {
    float       voxelSize;
    CUBOID_STRU stCuboid;
};

/**
 * @brief lidar点云射线地面分割参数结构
 *
 */
struct CFG_PARAM_SEG_STRU {
    float   maxLocalSlope;
    float   maxGeneralSlope;
    float   minHeightThreshold;
    float   reclassDistThreshold;
    float   radiusThreshold;
};

/**
 * @brief lidar点云扇区地面分割参数结构
 *
 */
struct CFG_PARAM_SEG_SEC_STRU {
    uint32_t secNums;
    uint32_t loopNums;
    float    minDiffHeight;
    float    coefficient;
};

/**
 * @brief lidar点云地面分割参数结构
 *
 */
struct CFG_PARAM_GROUND_SEG_STRU {
    bool                   isUseRPY;       // 是否使用rpy角度调整待分割点云位姿
    CFG_PARAM_SEG_STRU     stCfgSeg;
    CFG_PARAM_SEG_SEC_STRU stCfgSegSec;
};

/**
 * @brief 点云滤波区域参数结构
 *
 */
struct CFG_PARAM_PCPT_REGION_STRU {
    float       corseCell{0.2};                             // 粗粒度区域cell分辨率
    float       fineCell{0.05};                             // 细粒度区域cell分辨率
    CUBOID_STRU stCorseRegion{-30, 30, -30, 30, -5, 5};     // 粗粒度区域
    CUBOID_STRU stFineRegion{-4, 6, -5, 5, -5, 5};          // 细粒度区域
    CUBOID_STRU stObstRegion{-30, 30, -30, 30, -5, 5};      // 障碍结果区域
};

/**
 * @brief 点云维持参数结构
 */

struct CFG_PARAM_PCPT_CLOUD_PERSISTENCE_STRU {
    BOOL    isSupport;
    // 空间参数
    INT32 maxGroundVoxelNum{80000}; // 地面点云维持最大体素数量
    float voxelSize{0.5}; // 维持空间体素大小
    float minR{0.0}; // 最小半径 
    float maxR{150.0};  // 最大半径
    INT32 rBins{800}; // 半径方向分辨率 固定不可开放
    INT32 thetaBins{72}; // 航偏角分辨率
    INT32 phiBins{36}; // 俯仰角分辨率 
    float phiMin{-M_PI / 2}; // 最小俯仰角度
    float phiMax{M_PI / 2}; // 最大俯仰角度

    // 概率参数
    float loOcc{0.6}; // 击中累积概率 
    float loFree{-0.6}; // 空闲累积概率 
    float occThresh{0.6}; // 击中概率阈值 
    float freeThresh{0.5}; // 空闲概率阈值 
};

/**
 * @brief 点云障碍分层高度参数结构，分为上中下三层
 *
 */
struct CFG_PARAM_LAYER_HEIGHT_STRU {
    float midLayerMinHeight{0};
    float midLayerMaxHeight{0};
};

struct CFG_PARAM_DOUBLE_LIDAR_VERIFY_STRU {
    BOOL            isSupport;
    RECTANGLE_STRU  stVerifyReg;
};

struct CFG_PARAM_EMERGENCE_VERIFY_STRU {
    BOOL            isSupport;
    RECTANGLE_STRU  stVerifyReg;
    float           CompareRadius;
    float           keyFrameDist;
};

struct CFG_PARAM_ROADMARK_VERIFY_STRU {
    BOOL isSupport;
};

struct CFG_PARAM_REFLECT_VERIFY_STRU {
    CFG_PARAM_DOUBLE_LIDAR_VERIFY_STRU  stDoubleLidarVerify;
    CFG_PARAM_EMERGENCE_VERIFY_STRU     stEmergenceVerify;
    CFG_PARAM_ROADMARK_VERIFY_STRU      stRoadmarkVerify;
};

struct CFG_PARAM_SHORT_OBST_HOLD_STRU {
    BOOL           isSupport;
    RECTANGLE_STRU stRegion;
    float          maxHeight;
    float          hedgeRadius;
};

struct CFG_PARAM_COMMON_OBST_HOLD_STRU {
    BOOL           isSupport;
    RECTANGLE_STRU stMinRegion;
    RECTANGLE_STRU stMaxRegion;
};

struct CFG_PARAM_DROP_DET_STRU {
    bool            isSupport;
    float           height;
};

struct CFG_PARAM_PLANE_SEG_STRU {
    BOOL                    isSupport;
    bool                    isUseVelItem;
    int                     iterationTimes;
    float                   unitX;
    float                   maxAngleByX;
    float                   maxAngleByY;
    float                   heightThreshold;
    RECTANGLE_STRU          stRegion;
    CFG_PARAM_DROP_DET_STRU stDropDet;
};

struct CFG_PARAM_REGION_STRU {
    float   originDistance;
    float   clusterRadius;
    UINT32  minCluPointsNum;
    UINT32  maxCluPointsNum;
};

struct CFG_PARAM_NET_STRU {
    BOOL isSupport;
};

struct CFG_PARAM_DETECT_PILE_STRU {
    float qrCodeValidDist;
    int   binType;
    float distThresh;   // 判断站点垃圾箱是否已满的距离阈值
    float numThresh;    // 判断站点垃圾箱是否已满的点数阈值
    int   intensityThreshold;
    float chargingPileHeight;
    float offsetY;
};

struct CFG_PARAM_OBST_STATE_STRU {
    bool  isSupport{false};
    float staticThreshold{0};
};

struct CFG_PARAM_TRAFFIC_LIGHT_STRU {
    int   pixelBias;    // 像素偏差
    float rectBias;     // 最大偏差率
};

/**
 * @brief 点云聚类参数结构
 *
 */
struct CFG_PARAM_CLUSTER_STRU {
    BOOL    isSupport;
    float   voxelSize;                          // 体素分辨率
    vector<CFG_PARAM_REGION_STRU>   vstRegion;  // 聚类区域有序集
};

/**
 * @brief 点云多帧处理参数结构
 *
 */
struct CFG_PARAM_MUTLTIFRAME_STRU {
    BOOL    isSupport{false};
    int     frameNum{0};    // 帧数
};

/**
 * @brief 可通行区域参数结构
 *
 */
struct CFG_PARAM_ACCESSIBLE_AREA_STRU {
    BOOL    isSupport{false};
};

/**
 * @brief 斜坡垫检测参数结构
 *
 */
struct CFG_PARAM_SLOPE_PAD_DET_STRU {
    BOOL    isSupport{false};
    float   extandDist{0};
};

/**
 * @brief 道路区域外点云过滤
 *
 */
struct CFG_PARAM_ROAD_REGION_FILTER_STRU {
    BOOL    isSupport{false};
};

/**
 * @brief 感知过滤点云重建
 *
 */
struct CFG_PARAM_FILTER_POINTS_REBUILD_STRU {
    BOOL    isSupport{false};
};

/**
 * @brief 库房相关参数
 *
 */
struct CFG_PARAM_WAREHOUSE_STRU {
    float reflectiveStripHeight;
    int   intensityThreshold;
};

/**
 * @brief 车身噪点去除相关参数
 *
 */
struct CFG_PARAM_CAR_BODY_DENOSING_STRU {
    BOOL  isSupport{false};
    float resolution{0.};
};

/**
 * @brief 洁净度相关参数
 *
 */
struct CFG_PARAM_CLEANLINESS_STRU {
    BOOL  isSupport{false};
    float length{0.};
    float width{0.};
    float coefficient{0.};
};


/**
 * @brief 感知参数结构
 *
 */
struct CFG_PARAM_PNP_STRU {
    BOOL                                    isSupport;
    CFG_PARAM_CAR_BODY_VXL_FILTER_STRU      stCarBodyVxlFilter; // 车身体素过滤参数
    CFG_PARAM_GROUND_SEG_STRU               stGroundSeg;
    CFG_PARAM_PCPT_REGION_STRU              stPcptRegion;       // 滤波区域参数
    CFG_PARAM_PCPT_CLOUD_PERSISTENCE_STRU   stPcptCloudPersistence; // 点云维持参数
    CFG_PARAM_LAYER_HEIGHT_STRU             stLayerHeight;      // 障碍分成参数
    CFG_PARAM_REFLECT_VERIFY_STRU           stReflectVerify;
    CFG_PARAM_SHORT_OBST_HOLD_STRU          stShortObstHold;
    CFG_PARAM_COMMON_OBST_HOLD_STRU         stCommonObstHold;   // 常规障碍维持参数
    CFG_PARAM_PLANE_SEG_STRU                stPlaneSeg;
    CFG_PARAM_CLUSTER_STRU                  stCluster;
    CFG_PARAM_NET_STRU                      stNet;
    CFG_PARAM_MUTLTIFRAME_STRU              stMultiframe;
    CFG_PARAM_DETECT_PILE_STRU              stDetectPile;
    CFG_PARAM_OBST_STATE_STRU               stObstState;
    CFG_PARAM_TRAFFIC_LIGHT_STRU            stTrafficLight;
    CFG_PARAM_ACCESSIBLE_AREA_STRU          stAccessibleArea;
    CFG_PARAM_SLOPE_PAD_DET_STRU            stSlopePadDet;
    CFG_PARAM_ROAD_REGION_FILTER_STRU       stRoadRegionFilter;
    CFG_PARAM_FILTER_POINTS_REBUILD_STRU    stFilterPointsRebuild;
    CFG_PARAM_WAREHOUSE_STRU                stWarehouse;
    CFG_PARAM_CAR_BODY_DENOSING_STRU        stCarBodyDenoising;  // 车身噪点去除相关参数
    CFG_PARAM_CLEANLINESS_STRU              stCleanliness;       // 洁净度相关参数
};

/**
 * @brief 视觉功能基本参数
 *
 */
struct CFG_PARAM_VISION_BASE_STRU {
    bool                      isSupport{false};
    bool                      isSupportIpu{false};
    uint32_t                  hz{0};
    std::vector<std::string>  vstrCamera;
};

/**
 * @brief 视觉功能-粗水管参数
 *
 */
struct CFG_PARAM_LABEL_THICKPIPE_STRU {
    bool  isSupport{false};
    float width{0.0};
};

/**
 * @brief 视觉功能-网络检测
 *
 */
struct CFG_PARAM_VISION_NET_DET_STRU {
    CFG_PARAM_VISION_BASE_STRU      stParam;
    vector<LABEL_ENUM>              vValidLabel;
};

/**
 * @brief 视觉功能-网络分割
 *
 */
struct CFG_PARAM_VISION_NET_SEG_STRU {
    CFG_PARAM_VISION_BASE_STRU      stParam;
    vector<LABEL_ENUM>              vValidLabel;
    CFG_PARAM_LABEL_THICKPIPE_STRU  stLabelThickPipe;   //粗水管参数
};

struct CFG_PARAM_VISION_TRAFFIC_LIGHT_DET_STRU {
    CFG_PARAM_VISION_BASE_STRU      stParam;
};

/**
 * @brief 洁净度功能-检测区域参数
 *
 */
struct CFG_PARAM_DETECT_AREA_STRU {
    float length{0.0};
    float width{0.0};
    float blindSpotThreshold{0.0};

};
struct CFG_PARAM_VISION_CLEAN_CLASS_DET_STRU {
    CFG_PARAM_VISION_BASE_STRU      stParam;
    CFG_PARAM_DETECT_AREA_STRU      stDetArea;   //检测区域参数
};

/**
 * @brief 视觉任务控制参数结构
 *
 */
struct CFG_PARAM_VISION_FUNC_STRU {
    bool isSupport{false};
    bool isUseApi{false};
    CFG_PARAM_VISION_BASE_STRU              stQRCode;          //二维码检测
    CFG_PARAM_VISION_NET_DET_STRU           stNetDet;          //网络检测
    CFG_PARAM_VISION_NET_SEG_STRU           stNetSeg;          //网络分割
    CFG_PARAM_VISION_TRAFFIC_LIGHT_DET_STRU stTrafficLightDet; //交通灯检测
    CFG_PARAM_VISION_BASE_STRU              stTrashBoxDet;     //垃圾箱检测
    CFG_PARAM_VISION_CLEAN_CLASS_DET_STRU   stCleanClassDet;   //洁净度检测
};

struct CFG_PARAM_DCU_INFO_STRU
{
    std::string strSerialDevName;
    bool enbleCarPitch;
};

struct CFG_PARAM_WHEEL_SPEED_SENSOR_STRU {
    float scaleFactor;
};

struct CFG_PARAM_STEERING_ANGLE_SENSOR_STRU {
    float scaleFactor;
    float offsetFactor;
};

struct CFG_PARAM_VEL_SMOOTHER_STRU {
    float forwardAcc;
    float forwardDec;
    float backwardAcc;
    float backwardDec;
};

/**
 * @brief 车体结构信息结构
 *
 */
struct CFG_PARAM_STRUCTURE_INFO_STRU {
    std::string strModel;
};

/**
 * @brief 车体参数结构
 *
 */
struct CFG_PARAM_VEHICLE_BODY_STRU {
    CFG_PARAM_STRUCTURE_INFO_STRU   stStructrueInfo;        // 车体结构信息
    COORDINATE_2D_STRU              stAlongEdgeRefPoint;
};

struct CFG_PARAM_VEL_LIMIT_STRU {
    float maxForwardVel;
    float maxBackwardVelAbs;
    float maxCleanVelAbs;
};

/**
 * @brief 底盘参数结构
 *
 */
struct CFG_PARAM_VEHICLE_CHASSIS_STRU {
    std::string                          strNavIpcType;
    CFG_PARAM_DCU_INFO_STRU              stDcuInfo;
    CFG_PARAM_WHEEL_SPEED_SENSOR_STRU    stWheelSpeedSensor;
    CFG_PARAM_STEERING_ANGLE_SENSOR_STRU stSteeringAngleSensor;
    CFG_PARAM_VEL_SMOOTHER_STRU          stVelSmoother;
    CFG_PARAM_VEL_LIMIT_STRU             stVelLimit;
};

struct CFG_PARAM_GLOBAL_PLAN_STRU {
    float transDistOfCloseEdgeRouteForNonCleaning;
    BOOL  isSupportTurnAround;
    BOOL  isSupportRoadPortTurnAround;
    float skipExecuteLen;
    float minTurnAroundRoadWidth;
};

struct CFG_PARAM_TASK_MNG_STRU {
    UINT32 defaultAlgoOfPatrol;
    float emptyTrashMoveDist;
    float trafficControlCheckDist;
};

struct CFG_PARAM_LOCAL_PLAN_STRU
{
    float stopFaceSafeDist;
    float parkFaceSafeDist;
    float maxSideSafeDistWhenAvoidAlongRoute;
    float rangeDistGarbageBinPose;
    float trashBoxCtrlThresh;
    float brushShortenLen;
};

struct CFG_PARAM_LINEAR_VEL_PLAN_STRU {
    float maxAngularVel;
    float smoothDec;
    float velCtrlDelayFactor;
};

struct CFG_PARAM_TELE_CTRL_STRU {
    BOOL isEmergeStopSmooth;
};

struct CFG_PARAM_PLAN_TIME_PARAM_STRU {
    float waitEscapeTime;
};

struct CFG_PARAM_PLAN_FUNCTION_PARAM_STRU {
    BOOL isDoorCloseRequire;
    BOOL isDynamicObstStop;
    BOOL isSmartCtrlBrush;
    BOOL isAvoidObstRemotely;
    BOOL isAligningUseLastPose;
    BOOL isEscapeSideObst;  // 是否对车体内侧边障碍脱困
};

struct CFG_PARAM_PLAN_CHARGE_PARAM_STRU {
    float stopLenToEquipment; // 350自动充电停车车头最前面和充电桩反光条的距离
    float addWaterAdjustLen;  // 加水时调整的距离，最后停车位置调整距离
};

struct CFG_PARAM_PLAN_STRU {
    CFG_PARAM_GLOBAL_PLAN_STRU          stGlobalPlan;
    CFG_PARAM_TASK_MNG_STRU             stTaskMng;
    CFG_PARAM_LOCAL_PLAN_STRU           stLocalPlan;
    CFG_PARAM_LINEAR_VEL_PLAN_STRU      stLinearVelPlan;
    CFG_PARAM_TELE_CTRL_STRU            stTeleCtrl;
    CFG_PARAM_PLAN_TIME_PARAM_STRU      stTimeParam;
    CFG_PARAM_PLAN_FUNCTION_PARAM_STRU  stFunctionParam;
    CFG_PARAM_PLAN_CHARGE_PARAM_STRU    stChargeParam;
};

/**
 * @brief 配置参数结构
 *
 */
struct CFG_PARAM_STRU {
    CFG_PARAM_GNSS_STRU                         stGnss;                // Gnss参数
    CFG_PARAM_IMU_STRU                          stImu;                 // IMU参数
    CFG_PARAM_ULTRASONICS_STRU                  stUltrasonics;         // 超声波参数
    CFG_PARAM_TOFS_STRU                         stTofs;                // Tof参数
    CFG_PARAM_RGB_CAMERAS_STRU                  stRgbCameras;          // rgb相机参数
    CFG_PARAM_LOC_LIDAR_STRU                    stLocLidar;            // 定位雷达参数
    CFG_PARAM_BACK_DEPTH_CAMERA_STRU            stBackDepthCamera;     // 后视深度相机参数
    CFG_PARAM_LIDARS_STRU                       stLidars;              // Lidar参数
    CFG_PARAM_LOC_STRU                          stLoc;                 // 定位参数
    CFG_PARAM_VEHICLE_LOC_STRU                  stVehicleLoc;          // 车辆定位
    CFG_PARAM_MAPPING_STRU                      stMapping;             // 建图参数
    CFG_PARAM_RADAR_STRU                        stRadar;               // Radar参数
    CFG_PARAM_PNP_STRU                          stPnp;                 // 点云参数
    CFG_PARAM_VISION_FUNC_STRU                  stVisionFunc;          // 视觉任务控制参数
    CFG_PARAM_VEHICLE_BODY_STRU                 stVehicleBody;         // 车体参数
    CFG_PARAM_VEHICLE_CHASSIS_STRU              stVehicleChassis;      // 底盘参数
    CFG_PARAM_PLAN_STRU                         stPlan;                // 规划参数
};



#endif
