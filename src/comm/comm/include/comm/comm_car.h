
#ifndef __COMM_CAR_H__
#define __COMM_CAR_H__

#include "comm/comm_geometry.h"
#include "comm/comm_base.h"
#include "comm/comm_cfg_param.h"
#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <atomic>
#include <unordered_map>

using namespace std;

#define AUTO_DRIVE_STEERING_ANGLE_FACTOR_MAX                0.95        /* 自动驾驶时采用的转向角最大系数 */
#define CAR_SIMULATION_MOVE_VEL                             0.98        /* 仿真速度，保证仿真周期不能跨过一个栅格的距离 */

/**************************************************************************************
功能描述: 车辆类型
修改记录:
**************************************************************************************/
enum CAR_TYPE_ENUM
{
    CAR_TYPE_S310,      // 小蜗牛车型

    CAR_TYPE_S320,      // 降本前车型
    CAR_TYPE_S323,      // 标准版：前扫刷不可左右伸缩
    CAR_TYPE_S324,      // 标准版：前扫刷可左右伸缩
    CAR_TYPE_S325,      // 侨银定制
    CAR_TYPE_S326,      // 垃圾打包车型（基于S324）

    CAR_TYPE_S330,      // 降本前车型
    CAR_TYPE_S332,      // 一字尾翼
    CAR_TYPE_S333,      // V 形尾翼
    CAR_TYPE_S334,      // V 形尾翼（展开宽度变窄）
    CAR_TYPE_S335,      // V 形尾翼（带有拖挂装置）
    CAR_TYPE_S336,      // 波普定制机型
    CAR_TYPE_S337,      // 深能定制机型

    CAR_TYPE_S350,      // 350车型

    CAR_TYPE_S520,      // 中型车（1吨）
    CAR_TYPE_S530,      // 中型车（3吨）

    CAR_TYPE_NULL       /* 必须在最后面 */
};

// 车身分区
enum class CAR_BODY_REGION_ENUM
{
    CENTER = 0,
    HEAD = 1,
    LEFT = 2,
    RIGHT = 3,
    BACK = 4,
};

// 车身分区结构
struct CAR_BODY_REGION_STRU
{
    CAR_BODY_REGION_ENUM region;
    COORDINATE_2D_STRU left_head;
    COORDINATE_2D_STRU right_head;
    COORDINATE_2D_STRU left_back;
    COORDINATE_2D_STRU right_back;
};

/**************************************************************************************
功能描述: 车信息
修改记录:
**************************************************************************************/
struct CAR_DYNAMIC_INFO_STRU
{
    float bodyWidthExpand;      /* 伸开后的不可触碰的车体宽度，不包含刷毛 */;
    float headLen;              /* 车头(含触边)最前沿到前轮的距离 */
};

/**************************************************************************************
功能描述: 车体信息
修改记录:
**************************************************************************************/
struct CAR_BODY_INFO_STRU
{
    CAR_TYPE_ENUM enCarType;            /* 车辆类型 */
    const CHAR   *pcCarType;

    float wheelBaseLen;                 /* 小车前后轮轴距 */
    float bodyWidth;                    /* 不可触碰的车体宽度，不包含刷毛 */
    float bodyLength;                   /* 车身长，不含扫刷结构臂架 */
    CAR_DYNAMIC_INFO_STRU stDynamicInfo;/* 小车随刷子运动变化的参数 */
    float tailLen;                      /* 车尾(含触边)最后沿到后轮的距离 */
    float wheelWidth;                   /* 后轮轮距 */
    float carHeight;                    /* 车高 */

    float maxForwardVel;                /* 最大前向速度 */
    float maxBackWardVelAbs;            /* 最大后向速度 */
    float minMoveVelAbs;                /* 小车最小有效速度 */
    float maxSteeringAngleAbs;          /* 最大转弯角度，绝对值 */

    float trcRouteAimDist;              /* 跟踪路线瞄准距离 */
    float closeEdgeAvoidDist;           /* 贴边避障距离，根据0.8倍最大转向角时车身右前位置能绕过正前方墙为准 */
    float brushFeatherLen;              /* 刷毛长度 */
};

/**
 * @brief 车体基本参数结构
 *
 */
struct CAR_BODY_BASE_PARAM_STRU {
    float wheelBaseLen;                 /* 前后轴距 */
    float headToBackAxisLen;            /* 车头到后轴的距离 */
    float tailToBackAxisLen;            /* 车尾到后轴的距离 */
    float bodyWidth;                    /* 车身宽度 */
    float bodyLength;                   /* 车身长度, 不含扫刷臂结构 */
    float carHeight;                    /* 车身高度 */
    float backWheelWidth;               /* 后轮轮距 */
    float maxSteeringAngleAbs;          /* 最大转向角度 */
    float brushFeatherLen;              /* 刷毛长度 */
};

/**
 * @brief 车体扩展参数结构
 *
 */
struct CAR_BODY_EXTEND_PARAM_STRU {
                                                            // car坐标系: 原点在后轮轴心在地面上的投影，车头方向为X轴
    COOR_2D_POSE_STRU   stFrontWheelPoseByCar_;             // 前轮轴心位姿（car坐标系）

    COORDINATE_2D_STRU  stLeftHeadPosByCar_;                // 车身左前部坐标（car坐标系）
    COORDINATE_2D_STRU  stRightHeadPosByCar_;               // 车身右前部坐标（car坐标系）
    COORDINATE_2D_STRU  stLeftTailPosByCar_;                // 车身左后部坐标（car坐标系）
    COORDINATE_2D_STRU  stRightTailPosByCar_;               // 车身右后部坐标（car坐标系）
    COORDINATE_2D_STRU  stLeftBackAxisPosByCar_;            // 后轮轴左侧坐标（car坐标系）
    COORDINATE_2D_STRU  stRightBackAxisPosByCar_;           // 后轮轴右侧坐标（car坐标系）
    COORDINATE_2D_STRU  stLeftBackWheelPosByCar_;
    COORDINATE_2D_STRU  stRightBackWheelPosByCar_;

    COORDINATE_2D_STRU  stMaxAngleLeftTurnCenterPosByCar_;
    COORDINATE_2D_STRU  stMaxAngleRightTurnCenterPosByCar_;
};

/**
 * @brief 车体运动参数结构
 *
 */
struct CAR_MOTION_PARAM_STRU {
    float trcRouteAimDist;
    float closeEdgeAvoidDist;

    float minMoveVelAbs;
    float maxForwardVel;
    float maxBackwardVelAbs;
    float maxCleanVelAbs;
    float maxForwardVelWithoutLimit;
    float maxSearchCleanVel;
};

/**
 * @brief 车体参数结构
 *
 */
struct CAR_BODY_PARAM_STRU {
    CAR_TYPE_ENUM              enCarType;       // 车体型号
    CAR_BODY_BASE_PARAM_STRU   stBaseParam;     // 车体基本参数
    CAR_BODY_EXTEND_PARAM_STRU stExtendParam;   // 车体扩展参数
    CAR_MOTION_PARAM_STRU      stMotionParam;   // 车体运动参数
};

/**************************************************************************************
功能描述: 车身
修改记录:
**************************************************************************************/
class CAR_BODY_C
{
public:
    CAR_BODY_C(void)
    {
        InitModel();

        expandState_     = isExpand_;
        pstCarBodyParam_ = &astStatesParam_[expandState_];
        //Print();
    }

    virtual ~CAR_BODY_C(void)
    {
    }

    /**************************************************************************************
    功能描述: 处理车型信息
    修改记录:
    **************************************************************************************/
    static void  InitModel(void);
    static void  SetExpandState(BOOL isExpand);
    static void  SetVelLimitHmi(float maxCleanSpeed, float maxPatrolSpeed, float maxSearchCleanSpeed);

    static const CAR_BODY_INFO_STRU *GetCarBodyInfo(const std::string &strModel);
    static std::string               GetCarTypeStr(CAR_TYPE_ENUM enCarType);
    static float                     GetTurnRadiusByWheelBaseLen(float wheelBaseLen, float steeringAngle);

    BOOL   IsS310(void) const;                /* 判断是否是某种车型，待优化，应用方不能按照类型来进行区分处理，应该修改按能力和配置进行处理。 */
    BOOL   IsS320(void) const;
    BOOL   IsS323(void) const;
    BOOL   IsS324(void) const;
    BOOL   IsS325(void) const;
    BOOL   IsS326(void) const;
    BOOL   IsS330(void) const;
    BOOL   IsS332(void) const;
    BOOL   IsS333(void) const;
    BOOL   IsS334(void) const;
    BOOL   IsS335(void) const;
    BOOL   IsS336(void) const;
    BOOL   IsS337(void) const;
    BOOL   IsS350(void) const;
    BOOL   IsS520(void) const;
    BOOL   IsS530(void) const;

    BOOL   Update(void);    /* 返回值，是否进行了更新 */
    BOOL   IsExpand(void) const;

    void   GetLeftHeadPosByCarWithProtect(float headProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model = false) const;
    void   GetRightHeadPosByCarWithProtect(float headProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model = false) const;
    void   GetLeftBackAxisPosByCarWithProtect(float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model = false) const;
    void   GetRightBackAxisPosByCarWithProtect(float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model = false) const;
    void   GetLeftTailPosByCarWithProtect(float tailProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model = false) const;
    void   GetRightTailPosByCarWithProtect(float tailProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model = false) const;
    float  GetCarModelWidth(bool is_use_expand_model) const;

    void   GetMaxAngleLeftTurnCenterPosByCar(COORDINATE_2D_STRU &stPosByCar) const;
    void   GetMaxAngleRightTurnCenterPosByCar(COORDINATE_2D_STRU &stPosByCar) const;
    void   GetLeftTurnCenterPosByCar(float steeringAngleAbs, COORDINATE_2D_STRU &stPosByCar) const;
    void   GetRightTurnCenterPosByCar(float steeringAngleAbs, COORDINATE_2D_STRU &stPosByCar) const;

    float  GetHeadHalfBodyLen(void) const;
    float  GetBackHalfBodyLen(void) const;
    float  GetBodyLenBeforeFrontWheel(void) const;
    float  GetWheelTrack(void) const;
    float  GetBodyLen(void) const;
    float  GetBrushModelLen(void) const; // 获取扫刷模型的长度
    float  GetBodyWidth(void) const;
    float  GetBodyWidthExpand(void) const;
    float  GetBodyWidthClosed(void) const;
    float  GetCarHeight(void) const;
    float  GetMaxSteeringAngleAbs(void) const;
    float  GetMaxForwardVel(void) const;
    float  GetMaxForwardVelWithoutLimit(void) const;
    float  GetMaxBackwardVelAbs(void) const;
    float  GetMaxCleanVelAbs(void) const;
    float  GetMaxSearchCleanVelAbs(void) const;
    float  GetMinVelAbs(void) const;
    float  GetTurnRadius(float steeringAngle) const;
    float  GetMinTurnRadius(void) const;
    float  GetSteeringAngle(float turnRadius) const;
    float  GetWheelBaseLen(void) const;
    float  GetTrcRouteAimDist(void) const;
    float  GetCloseEdgeAvoidDist(void) const;
    float  GetBrushFeatherLen(void) const;
    float  GetDiffWidthExpandAndNormal(void) const;

    void   GetCarModelTypeMap(std::unordered_map<string, CAR_TYPE_ENUM>& umModelType) const;

protected:
    CAR_BODY_PARAM_STRU            *pstCarBodyParam_{nullptr};      /* 车体参数 */
    BOOL                            expandState_{true};
private:
    static const CAR_BODY_INFO_STRU astCarBodyInfo_[];
    static CAR_BODY_PARAM_STRU      astStatesParam_[2];             /* 正常和扩展两种状态下车体参数 */
    static BOOL                     isInitSucc_;
    static std::mutex               objInitMutex_;
    static std::atomic_bool         isExpand_;

    static void  SetCarBodyExtendParam(const CAR_BODY_BASE_PARAM_STRU &stBaseParam, CAR_BODY_EXTEND_PARAM_STRU &stExtendParam);
    static void  SetCarMotionParam(const CAR_BODY_INFO_STRU &stBodyInfo, const CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg, CAR_MOTION_PARAM_STRU &stMotionParam);
    static void  SetNormalStateParam(const CAR_BODY_INFO_STRU &stBodyInfo, const CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg, CAR_BODY_PARAM_STRU &stBodyParam);
    static void  SetExpandStateParam(const CAR_BODY_INFO_STRU &stBodyInfo, const CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg, CAR_BODY_PARAM_STRU &stBodyParam);

    void Print(void) const;
};

/**************************************************************************************
功能描述: 动态车辆
修改记录:
**************************************************************************************/
class DYNC_CAR_C : public CAR_BODY_C
{
public:
    DYNC_CAR_C(void)
    {
    }

    virtual ~DYNC_CAR_C(void)
    {
    }

    void  SetCarPose(const COOR_2D_POSE_STRU &stCarPoseByMap);
    void  SetFrontWheelPose(const COOR_2D_POSE_STRU &stPoseByMap);

    const COORDINATE_2D_STRU &GetCarPos(void) const;
    void  GetCarPose(COOR_2D_POSE_STRU &stCarPoseByMap) const;
    const COOR_2D_POSE_STRU &GetCarPose(void) const;
    const COORDINATE_2D_STRU &GetFrontWheelPos(void) const;
    void  GetFrontWheelPose(COOR_2D_POSE_STRU &stPoseByMap) const;
    void  GetFrontWheelPose(POSE_2D_C &objPoseByMap) const;
    const COOR_2D_POSE_STRU &GetFrontWheelPose(void) const;

    // 获取扫刷结构矩形的4个角点
    void  Get4CornerBrushPos(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                             COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                            float sideProtectLen = 0.0, float headProectLen = 0.0) const;

    // 获取车体本身矩形的4个角点(不含扫刷)
    void  Get4CornerBodyPos(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                             COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                            float sideProtectLen = 0.0, float headProtectLen = 0.0, float tailProectLen = 0.0) const;

    // 获取整体结构矩形的4个角点(含车身和扫刷)
    void  Get4CornerWholePos(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                             COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                            float sideProtectLen = 0.0, float headProectLen = 0.0, float tailProectLen = 0.0) const;

    void  GetCarBodyRegion(std::vector<CAR_BODY_REGION_STRU> &vecRegion) const;
    void  Get4CornerPosByCar(COORDINATE_2D_STRU &stLeftHeadPosByCar, COORDINATE_2D_STRU &stRightHeadPosByCar,
                             COORDINATE_2D_STRU &stLeftTailPosByCar, COORDINATE_2D_STRU &stRightTailPosByCar,
                             float sideProtectLen = 0.0, float faceProectLen = 0.0) const;
    void  Get4CornerPosByMap(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                             COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                             float sideProtectLen = 0.0, float faceProectLen = 0.0) const;
    void  Get4CornerHeadCarExpandModel(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                                       COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap) const;
    void  Get4CornerCarModelExpand(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                                       COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                                       float sideProtectLen = 0.0, float faceProectLen = 0.0) const;
    void  GetWheelPosByMap(COORDINATE_2D_STRU &stFrontWheelPos, COORDINATE_2D_STRU &stLeftBackWheel, COORDINATE_2D_STRU &stRightBackWheel) const;

    void  TransformPosByMapToByCar(const COORDINATE_2D_STRU &stPosByMap, COORDINATE_2D_STRU &stPosByCar) const;
    void  TransformPosByCarToByMap(const COORDINATE_2D_STRU &stPosByCar, COORDINATE_2D_STRU &stPosByMap) const;
    void  TransformPoseByMapToByCar(const COOR_2D_POSE_STRU &stPoseByMap, COOR_2D_POSE_STRU &stPoseByCar) const;
    void  TransformPoseByCarToByMap(const COOR_2D_POSE_STRU &stPoseByCar, COOR_2D_POSE_STRU &stPoseByMap) const;

    BOOL  IsPointBehind(const COORDINATE_2D_STRU &stPoint) const;

protected:
    COOR_2D_POSE_STRU stCarPoseByMap_;            // car位姿（map坐标系）
    COOR_2D_POSE_STRU stCarFrontWheelPoseByMap_;  // car前轮轴心位姿（map坐标系）

private:
    COOR_TF_C         objMapToCarTf_;
};

struct CAR_MOVE_INFO_STRU {
    float steeringAngle;       /* 转向角 */
    float moveLinearVel;       /* 线速度 */
    float imuAngularVel;       /* 角速度 */
    float imuAngle;
};

/**************************************************************************************
功能描述: 移动车辆
修改记录:
**************************************************************************************/
class MOVE_CAR_C : public DYNC_CAR_C
{
public:
    MOVE_CAR_C(float timePerMove = 0.05, float maxAccAbs = 1.0e10)
    {
        timeLenPerMove_           = timePerMove;
        maxAccAbs_                = maxAccAbs;
        maxVelDiffOfPerPeriodAbs_ = maxAccAbs_ * timeLenPerMove_;
        inverseOfTimeLenPerMove_  = 1.0 / timePerMove;
        inverseOfWheelBaseLen_    = 1.0 / GetWheelBaseLen();

        ResetMoveInfo();
    }

    virtual ~MOVE_CAR_C (void)
    {
    }

    float Move(float steeringAngle, float linearVel);
    void MoveLen(float steeringAngle, float len);
    float MoveBackWheelDrive(float steeringAngle, float linearVel);
    float MoveWithAccLimit(float steeringAngle, float linearVel);

    void  ResetMoveInfo(void);

    const CAR_MOVE_INFO_STRU &GetMoveInfo(void) const;

private:
    float               timeLenPerMove_;
    float               maxAccAbs_;
    float               maxVelDiffOfPerPeriodAbs_;
    float               inverseOfTimeLenPerMove_;
    float               inverseOfWheelBaseLen_;

    CAR_MOVE_INFO_STRU  stMoveInfo_;     /* 最新运动信息 */

    float GetLinearVelWithAccLimit(float linearVelCmd) const;
};

/**************************************************************************************
功能描述: 车辆驾驶器
修改记录:
**************************************************************************************/
class CAR_DRIVER_C
{
public:
    CAR_DRIVER_C(DYNC_CAR_C &objCar, BOOL isGoForward = true, BOOL isUseFrontWheelToGoForwardTrc = true)
                    : objCar_(objCar),
                      isGoForward_(isGoForward),
                      isUseFrontWheelToGoForwardTrc_(isUseFrontWheelToGoForwardTrc)
    {
    }

    virtual ~CAR_DRIVER_C(void)
    {
    }

    void  SetMoveDir(BOOL isGoForward, BOOL isUseFrontWheelToGoForwardTrc);
    BOOL  IsGoForward(void) const;

    void  SetRoutePose(const COOR_2D_POSE_STRU &stCarRoutePose);
    void  SetRoutePose(const POSE_2D_C &objRoutePose);
    void  GetRoutePose(COOR_2D_POSE_STRU &stCarRoutePose) const;
    void  GetFrontWheelRoutePose(POSE_2D_C &objRoutePose) const;
    void  GetRoutePose(POSE_2D_C &objRoutePose) const;

    float GetSteeringAngleOfFrontWheelTrc(const COORDINATE_2D_STRU &stAimPosByMap) const;
    float GetSteeringAngleOfBackWheelTrc(const COORDINATE_2D_STRU &stAimPosByMap) const;
    float GetSteeringAngleOfBackWheelDrive(const COORDINATE_2D_STRU &stAimPosByMap) const;
    float GetAimPointSteeringAngle(const COORDINATE_2D_STRU &stAimPosByMap) const;

    float GetSimulationVel(void) const;
    float GetMaxMoveVelAbs(void) const;

private:
    DYNC_CAR_C &objCar_;

    BOOL        isGoForward_;
    BOOL        isUseFrontWheelToGoForwardTrc_;
};


#endif

