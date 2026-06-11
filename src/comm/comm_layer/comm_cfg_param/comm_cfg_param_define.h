// #ifndef __COMM_CFG_PARAM_DEFINE_H__
// #define __COMM_CFG_PARAM_DEFINE_H__

// #include "../comm_coordinate.h"

// #include <string>


// struct CFG_PARAM_DCU_INFO_STRU
// {
//     std::string strSerialDevName;
// };

// struct CFG_PARAM_WHEEL_SPEED_SENSOR_STRU {
//     float scaleFactor;
// };

// struct CFG_PARAM_STEERING_ANGLE_SENSOR_STRU {
//     float scaleFactor;
//     float offsetFactor;
// };

// struct CFG_PARAM_VEL_SMOOTHER_STRU {
//     float forwardAcc;
//     float forwardDec;
//     float backwardAcc;
//     float backwardDec;
// };

// /**
//  * @brief 车体结构信息结构
//  *
//  */
// struct CFG_PARAM_STRUCTURE_INFO_STRU {
//     std::string strModel;
// };

// /**
//  * @brief 车体参数结构
//  *
//  */
// struct CFG_PARAM_VEHICLE_BODY_STRU {
//     CFG_PARAM_STRUCTURE_INFO_STRU   stStructrueInfo;        // 车体结构信息
//     COORDINATE_2D_STRU              stAlongEdgeRefPoint;
// };

// struct CFG_PARAM_VEL_LIMIT_STRU {
//     float maxForwardVel;
//     float maxBackwardVelAbs;
//     float maxCleanVelAbs;
// };

// /**
//  * @brief 底盘参数结构
//  *
//  */
// struct CFG_PARAM_VEHICLE_CHASSIS_STRU {
//     std::string                          strNavIpcType;
//     CFG_PARAM_DCU_INFO_STRU              stDcuInfo;
//     CFG_PARAM_WHEEL_SPEED_SENSOR_STRU    stWheelSpeedSensor;
//     CFG_PARAM_STEERING_ANGLE_SENSOR_STRU stSteeringAngleSensor;
//     CFG_PARAM_VEL_SMOOTHER_STRU          stVelSmoother;
//     CFG_PARAM_VEL_LIMIT_STRU             stVelLimit;
// };

// struct CFG_PARAM_GLOBAL_PLAN_STRU {
//     float transDistOfCloseEdgeRouteForNonCleaning;
//     bool  isSupportTurnAround;
//     bool  isSupportRoadPortTurnAround;
//     float skipExecuteLen;
//     float minTurnAroundRoadWidth;
// };

// struct CFG_PARAM_TASK_MNG_STRU {
//     unsigned int defaultAlgoOfPatrol;
//     float emptyTrashMoveDist;
// };

// struct CFG_PARAM_LOCAL_PLAN_STRU
// {
//     float stopFaceSafeDist;
//     float parkFaceSafeDist;
//     float maxSideSafeDistWhenAvoidAlongRoute;
//     float rangeDistGarbageBinPose;
//     float trashBoxCtrlThresh;
//     float brushShortenLen;
// };

// struct CFG_PARAM_LINEAR_VEL_PLAN_STRU {
//     float maxAngularVel;
//     float smoothDec;
//     float velCtrlDelayFactor;
// };

// struct CFG_PARAM_TELE_CTRL_STRU {
//     bool isEmergeStopSmooth;
// };

// struct CFG_PARAM_PLAN_TIME_PARAM_STRU {
//     float waitEscapeTime;
// };

// struct CFG_PARAM_PLAN_FUNCTION_PARAM_STRU {
//     bool isDoorCloseRequire;
//     bool isDynamicObstStop;
//     bool isSmartCtrlBrush;
// };

// struct CFG_PARAM_PLAN_STRU {
//     CFG_PARAM_GLOBAL_PLAN_STRU          stGlobalPlan;
//     CFG_PARAM_TASK_MNG_STRU             stTaskMng;
//     CFG_PARAM_LOCAL_PLAN_STRU           stLocalPlan;
//     CFG_PARAM_LINEAR_VEL_PLAN_STRU      stLinearVelPlan;
//     CFG_PARAM_TELE_CTRL_STRU            stTeleCtrl;
//     CFG_PARAM_PLAN_TIME_PARAM_STRU      stTimeParam;
//     CFG_PARAM_PLAN_FUNCTION_PARAM_STRU  stFunctionParam;
// };

// /**
//  * @brief 配置参数结构
//  *
//  */
// struct CFG_PARAM_STRU {
//     CFG_PARAM_VEHICLE_BODY_STRU                 stVehicleBody;         // 车体参数
//     CFG_PARAM_VEHICLE_CHASSIS_STRU              stVehicleChassis;      // 底盘参数
//     CFG_PARAM_PLAN_STRU                         stPlan;                // 规划参数
// };



// #endif

