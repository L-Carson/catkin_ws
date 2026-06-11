#include "comm/comm_cfg_param.h"
#include "comm/comm_car.h"
#include "comm/comm_base.h"
#include <math.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**
 * NOTE: 如果添加新机型，请通知感知检查:
 *    1.comm/comm_function.cpp是否适配
 *    2.comm/comm_car_body_file.cpp是否适配
 *    3.comm_lidar_3d/sensor_lidar_availability.cpp是否适配
 *    4.comm/maintain_param.cpp是否适配
 */
const CAR_BODY_INFO_STRU CAR_BODY_C::astCarBodyInfo_[] =
{
    /*               车型    轴距  车宽  车身长 {伸开车宽 车头长} 车尾长  轮距  车高  最大前速 最大后速 最小速度 最大转向角 跟线预瞄距离 贴边避障距离 刷毛长度*/
    {CAR_TYPE_S310, "S310", 0.78, 1.26, 1.48, {1.26, 0.50},  0.50, 0.88, 1.45, 1.50,   0.50,   0.15,   0.50,     0.50,     1.57,    0.10},
    {CAR_TYPE_S320, "S320", 0.82, 1.27, 1.42, {1.70, 0.97},  0.40, 0.80, 1.72, 1.50,   0.50,   0.15,   0.50,     0.50,     1.59,    0.10},
    {CAR_TYPE_S323, "S323", 0.82, 1.69, 1.43, {1.69, 0.85},  0.41, 0.79, 1.72, 1.50,   1.50,   0.15,   0.75,     0.50,     1.37,    0.10},
    {CAR_TYPE_S324, "S324", 0.82, 1.26, 1.42, {1.69, 0.90},  0.40, 0.79, 1.72, 1.50,   1.50,   0.15,   0.84,     0.50,     1.33,    0.10},
    {CAR_TYPE_S325, "S325", 0.82, 1.10, 1.42, {1.10, 0.40},  0.40, 0.79, 1.55, 1.50,   1.50,   0.15,   0.84,     0.50,     1.33,    0.10},
    {CAR_TYPE_S326, "S326", 0.82, 1.26, 1.42, {1.69, 0.90},  0.40, 0.79, 1.72, 1.50,   1.50,   0.15,   0.84,     0.50,     1.33,    0.10},
    {CAR_TYPE_S330, "S330", 0.82, 1.11, 1.63, {1.11, 0.48},  0.61, 0.89, 1.68, 1.50,   0.50,   0.05,   0.50,     0.50,     1.56,    0.10},
    {CAR_TYPE_S332, "S332", 0.94, 1.17, 1.95, {1.72, 0.81},  0.72, 0.87, 1.67, 2.00,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S333, "S333", 0.94, 1.18, 1.90, {1.72, 0.81},  0.62, 0.87, 1.67, 2.00,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S334, "S334", 0.94, 1.18, 1.90, {1.62, 0.80},  0.62, 0.87, 1.67, 2.00,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S335, "S335", 0.94, 1.18, 3.74, {1.62, 0.80},  2.51, 0.87, 1.67, 2.00,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S336, "S336", 0.94, 1.27, 1.90, {1.72, 0.80},  0.58, 0.87, 1.72, 2.00,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S337, "S337", 0.94, 1.29, 1.90, {1.72, 0.80},  0.67, 0.87, 1.72, 2.00,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S350, "S350", 0.91, 1.05, 1.80, {1.13, 0.41},  0.48, 0.73, 1.39, 1.50,   1.50,   0.05,   1.10,     0.50,     1.42,    0.10},
    {CAR_TYPE_S520, "S520", 1.44, 1.56, 2.14, {1.56, 1.00},  0.50, 1.18, 2.10, 1.50,   1.50,   0.15,   0.50,     0.50,     2.18,    0.15},
    {CAR_TYPE_S530, "S530", 1.47, 1.55, 2.12, {1.86, 2.08},  0.65, 1.13, 2.59, 2.00,   1.50,   0.15,   0.50,     2.00,     1.89,    0.13},
};

CAR_BODY_PARAM_STRU CAR_BODY_C::astStatesParam_[2];
BOOL                CAR_BODY_C::isInitSucc_{false};
std::mutex          CAR_BODY_C::objInitMutex_;
std::atomic_bool    CAR_BODY_C::isExpand_{true};

/**************************************************************************************
功能描述: 初始化模型
修改记录:
**************************************************************************************/
void CAR_BODY_C::InitModel(void)
{
    std::unique_lock<std::mutex> objLocker(objInitMutex_);
    if (isInitSucc_) {
        return;
    }

    /* 车辆模型初始化失败则直接退出，因为模型错误可能导致高风险。 */
    CFG_PARAM_C objCfgParam;
    INT32 rslt = objCfgParam.Init();
    if (rslt != 0) {
        ST_LOG_ERR("objCfgParam.Init() Fail. To Exit.");
        exit(0);
    }

    const CFG_PARAM_VEL_LIMIT_STRU      &stVelLimitCfg   = objCfgParam.GetVelLimitCfg();
    const CFG_PARAM_STRUCTURE_INFO_STRU &stStructureInfo = objCfgParam.GetStructureInfoCfg();
    const CAR_BODY_INFO_STRU            *pstCarBodyInfo  = GetCarBodyInfo(stStructureInfo.strModel);
    if (pstCarBodyInfo == nullptr) {
        ST_LOG_ERR("Unknown Model: %s. To Exit.", stStructureInfo.strModel.c_str());
        exit(0);
    }

    SetNormalStateParam(*pstCarBodyInfo, stVelLimitCfg, astStatesParam_[false]);
    SetExpandStateParam(*pstCarBodyInfo, stVelLimitCfg, astStatesParam_[true]);

    isInitSucc_ = true;
    ST_LOG_INFO("Succ. Car Type: %s, %u.", stStructureInfo.strModel.c_str(), pstCarBodyInfo->enCarType);
}

/**************************************************************************************
功能描述: 设置车辆扩展状态
修改记录:
**************************************************************************************/
void CAR_BODY_C::SetExpandState(BOOL isExpand)
{
    ST_LOG_INFO_IF(BOOL(isExpand_) != isExpand, "New-%u, Old=%u.", isExpand, BOOL(isExpand_));
    isExpand_ = isExpand;
}

/**************************************************************************************
功能描述: 设置车辆的速度参数
修改记录:
**************************************************************************************/
void CAR_BODY_C::SetVelLimitHmi(float maxCleanSpeed, float maxPatrolSpeed, float maxSearchCleanSpeed)
{
    float maxCurCleanSpeed = astStatesParam_[false].stMotionParam.maxCleanVelAbs;
    float maxCurPatrolSpeed = astStatesParam_[false].stMotionParam.maxForwardVel;
    float maxCurSearchCleanSpeed = astStatesParam_[false].stMotionParam.maxSearchCleanVel;
    ST_LOG_INFO("当前最大清扫速度[%f], 最大巡逻速度[%f], 最大巡扫速度[%f]", maxCurCleanSpeed, maxCurPatrolSpeed, maxCurSearchCleanSpeed);
    ST_LOG_INFO("Hmi设置最大清扫速度[%f],  最大巡逻速度[%f], 最大巡扫速度[%f]", maxCleanSpeed, maxPatrolSpeed, maxSearchCleanSpeed);
    astStatesParam_[false].stMotionParam.maxForwardVel = MIN(maxCurPatrolSpeed, maxPatrolSpeed);
    astStatesParam_[false].stMotionParam.maxCleanVelAbs = MIN(maxCurCleanSpeed, maxCleanSpeed);
    astStatesParam_[false].stMotionParam.maxSearchCleanVel = MIN(maxCurSearchCleanSpeed, maxSearchCleanSpeed);
    astStatesParam_[true].stMotionParam.maxForwardVel = astStatesParam_[false].stMotionParam.maxForwardVel;
    astStatesParam_[true].stMotionParam.maxCleanVelAbs = astStatesParam_[false].stMotionParam.maxCleanVelAbs;
    astStatesParam_[true].stMotionParam.maxSearchCleanVel = astStatesParam_[false].stMotionParam.maxSearchCleanVel;
}

/**************************************************************************************
功能描述: 判断是否是S310车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS310(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S310);
}

/**************************************************************************************
功能描述: 判断是否是S320车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS320(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S320);
}

/**************************************************************************************
功能描述: 判断是否是S323车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS323(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S323);
}

/**************************************************************************************
功能描述: 判断是否是S324车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS324(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S324);
}

/**************************************************************************************
功能描述: 判断是否是S325车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS325(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S325);
}

/**************************************************************************************
功能描述: 判断是否是S326车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS326(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S326);
}

/**************************************************************************************
功能描述: 判断是否是S330车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS330(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S330);
}

/**************************************************************************************
功能描述: 判断是否是S332车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS332(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S332);
}

/**************************************************************************************
功能描述: 判断是否是S333车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS333(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S333);
}

/**************************************************************************************
功能描述: 判断是否是S334车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS334(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S334);
}

/**************************************************************************************
功能描述: 判断是否是S335车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS335(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S335);
}

/**************************************************************************************
功能描述: 判断是否是S336车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS336(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S336);
}

/**************************************************************************************
功能描述: 判断是否是S337车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS337(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S337);
}

/**************************************************************************************
功能描述: 判断是否是S350车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS350(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S350);
}

/**************************************************************************************
功能描述: 判断是否是S520车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS520(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S520);
}

/**************************************************************************************
功能描述: 判断是否是S530车型
修改记录:
**************************************************************************************/
BOOL CAR_BODY_C::IsS530(void) const
{
    return (pstCarBodyParam_->enCarType == CAR_TYPE_S530);
}

BOOL CAR_BODY_C::Update(void)
{
    InitModel();
    BOOL expandState = isExpand_;
    if (expandState == expandState_) {
        return false;
    }

    expandState_ = expandState;
    pstCarBodyParam_ = &astStatesParam_[expandState_];

    return true;
}

BOOL CAR_BODY_C::IsExpand(void) const {
    return isExpand_ ? true : false;
}

/**************************************************************************************
功能描述: 获取车体左前点的位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetLeftHeadPosByCarWithProtect(float headProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model) const
{
    stPosByCar.x = astStatesParam_[is_use_expand_model].stExtendParam.stLeftHeadPosByCar_.x + headProtectWidth;
    stPosByCar.y = astStatesParam_[is_use_expand_model].stExtendParam.stLeftHeadPosByCar_.y + sideProtectWidth;
}

/**************************************************************************************
功能描述: 获取车体右前点的位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetRightHeadPosByCarWithProtect(float headProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model) const
{
    stPosByCar.x = astStatesParam_[is_use_expand_model].stExtendParam.stRightHeadPosByCar_.x + headProtectWidth;
    stPosByCar.y = astStatesParam_[is_use_expand_model].stExtendParam.stRightHeadPosByCar_.y - sideProtectWidth;
}

/**************************************************************************************
功能描述: 获取车体后轴左侧点位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetLeftBackAxisPosByCarWithProtect(float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model) const
{
    stPosByCar.x = astStatesParam_[is_use_expand_model].stExtendParam.stLeftBackAxisPosByCar_.x;
    stPosByCar.y = astStatesParam_[is_use_expand_model].stExtendParam.stLeftBackAxisPosByCar_.y + sideProtectWidth;
}

/**************************************************************************************
功能描述: 获取车体后轴右侧点位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetRightBackAxisPosByCarWithProtect(float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model) const
{
    stPosByCar.x = astStatesParam_[is_use_expand_model].stExtendParam.stRightBackAxisPosByCar_.x;
    stPosByCar.y = astStatesParam_[is_use_expand_model].stExtendParam.stRightBackAxisPosByCar_.y - sideProtectWidth;
}

/**************************************************************************************
功能描述: 获取车体左后点位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetLeftTailPosByCarWithProtect(float tailProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model) const
{
    stPosByCar.x = astStatesParam_[is_use_expand_model].stExtendParam.stLeftTailPosByCar_.x - tailProtectWidth;
    stPosByCar.y = astStatesParam_[is_use_expand_model].stExtendParam.stLeftTailPosByCar_.y + sideProtectWidth;
}

/**************************************************************************************
功能描述: 获取车体右后点位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetRightTailPosByCarWithProtect(float tailProtectWidth, float sideProtectWidth, COORDINATE_2D_STRU &stPosByCar, bool is_use_expand_model) const
{
    stPosByCar.x = astStatesParam_[is_use_expand_model].stExtendParam.stRightTailPosByCar_.x - tailProtectWidth;
    stPosByCar.y = astStatesParam_[is_use_expand_model].stExtendParam.stRightTailPosByCar_.y - sideProtectWidth;
}

/**************************************************************************************
功能描述: 获取模型宽度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetCarModelWidth(bool is_use_expand_model) const {
    return astStatesParam_[is_use_expand_model].stBaseParam.bodyWidth;
}

/**************************************************************************************
功能描述: 获取最小半径左转圆心位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetMaxAngleLeftTurnCenterPosByCar(COORDINATE_2D_STRU &stPosByCar) const
{
    stPosByCar = pstCarBodyParam_->stExtendParam.stMaxAngleLeftTurnCenterPosByCar_;
}

/**************************************************************************************
功能描述: 获取最小半径右转圆心位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetMaxAngleRightTurnCenterPosByCar(COORDINATE_2D_STRU &stPosByCar) const
{
    stPosByCar = pstCarBodyParam_->stExtendParam.stMaxAngleRightTurnCenterPosByCar_;
}

/**************************************************************************************
功能描述: 获取指定转向角时的转弯圆心位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetLeftTurnCenterPosByCar(float steeringAngleAbs, COORDINATE_2D_STRU &stPosByCar) const
{
    float steeringAngleAbsE = MIN(ABS(steeringAngleAbs), pstCarBodyParam_->stBaseParam.maxSteeringAngleAbs);
    float turnRadius        = GetTurnRadius(steeringAngleAbsE);

    stPosByCar.x  = 0.0;
    stPosByCar.y  = turnRadius;
}

/**************************************************************************************
功能描述: 获取指定转向角时的转弯圆心位置
修改记录:
**************************************************************************************/
void CAR_BODY_C::GetRightTurnCenterPosByCar(float steeringAngleAbs, COORDINATE_2D_STRU &stPosByCar) const
{
    float steeringAngleAbsE = MIN(ABS(steeringAngleAbs), pstCarBodyParam_->stBaseParam.maxSteeringAngleAbs);
    float turnRadius        = GetTurnRadius(steeringAngleAbsE);

    stPosByCar.x  = 0.0;
    stPosByCar.y  = turnRadius * -1.0;
}

/**************************************************************************************
功能描述: 获取前半身长度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetHeadHalfBodyLen(void) const
{
    return pstCarBodyParam_->stBaseParam.headToBackAxisLen;
}

/**************************************************************************************
功能描述: 获取后半身长度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetBackHalfBodyLen(void) const
{
    return pstCarBodyParam_->stBaseParam.tailToBackAxisLen;
}

/**************************************************************************************
功能描述: 获取前轮之前的车身长度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetBodyLenBeforeFrontWheel(void) const
{
    return pstCarBodyParam_->stBaseParam.headToBackAxisLen - pstCarBodyParam_->stBaseParam.wheelBaseLen;
}

/**************************************************************************************
功能描述: 获取左右轮距
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetWheelTrack(void) const
{
    return pstCarBodyParam_->stBaseParam.backWheelWidth;
}

float CAR_BODY_C::GetBodyLen(void) const
{
    return pstCarBodyParam_->stBaseParam.headToBackAxisLen + pstCarBodyParam_->stBaseParam.tailToBackAxisLen;
}

float CAR_BODY_C::GetBrushModelLen(void) const
{
    float brush_len = pstCarBodyParam_->stBaseParam.headToBackAxisLen + pstCarBodyParam_->stBaseParam.tailToBackAxisLen - pstCarBodyParam_->stBaseParam.bodyLength;
    if (IsS350()) {
        brush_len = 0.2f;
    }
    return brush_len;
}

/**************************************************************************************
功能描述: 获取车身宽度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetBodyWidth(void) const
{
    return pstCarBodyParam_->stBaseParam.bodyWidth;
}

float CAR_BODY_C::GetBodyWidthExpand(void) const
{
    return astStatesParam_[true].stBaseParam.bodyWidth;
}

float CAR_BODY_C::GetBodyWidthClosed(void) const
{
    return astStatesParam_[false].stBaseParam.bodyWidth;
}

/**************************************************************************************
功能描述: 获取车身高度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetCarHeight(void) const
{
    return pstCarBodyParam_->stBaseParam.carHeight;
}

/**************************************************************************************
功能描述: 获取最大转向角
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetMaxSteeringAngleAbs(void) const
{
    return pstCarBodyParam_->stBaseParam.maxSteeringAngleAbs;
}

/**************************************************************************************
功能描述: 获取最大前向速度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetMaxForwardVel(void) const
{
    return pstCarBodyParam_->stMotionParam.maxForwardVel;
}

/**************************************************************************************
功能描述: 获取无限制最大前向速度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetMaxForwardVelWithoutLimit(void) const
{
    return pstCarBodyParam_->stMotionParam.maxForwardVelWithoutLimit;
}

/**************************************************************************************
功能描述: 获取最大后向速度
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetMaxBackwardVelAbs(void) const
{
    return pstCarBodyParam_->stMotionParam.maxBackwardVelAbs;
}

float CAR_BODY_C::GetMaxCleanVelAbs(void) const
{
    return pstCarBodyParam_->stMotionParam.maxCleanVelAbs;
}

float CAR_BODY_C::GetMaxSearchCleanVelAbs(void) const
{
    return pstCarBodyParam_->stMotionParam.maxSearchCleanVel;
}

float CAR_BODY_C::GetMinVelAbs(void) const
{
    return pstCarBodyParam_->stMotionParam.minMoveVelAbs;
}

/**************************************************************************************
功能描述: 获取给定转向角下的转弯半径
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetTurnRadius(float steeringAngle) const
{
    return GetTurnRadiusByWheelBaseLen(pstCarBodyParam_->stBaseParam.wheelBaseLen, steeringAngle);
}

float CAR_BODY_C::GetMinTurnRadius(void) const
{
    return GetTurnRadius(pstCarBodyParam_->stBaseParam.maxSteeringAngleAbs);
}

/**************************************************************************************
功能描述: 获取给定转弯半径(指后轴中心处)下的转向角
          半径为正，圆心在左边，转向角大于0
          半径为负，圆心在右边，转向角小于0
修改记录:
**************************************************************************************/
float CAR_BODY_C::GetSteeringAngle(float turnRadius) const
{
    return SIGN(turnRadius) * atan2(pstCarBodyParam_->stBaseParam.wheelBaseLen, ABS(turnRadius));
}

float CAR_BODY_C::GetWheelBaseLen(void) const
{
    return pstCarBodyParam_->stBaseParam.wheelBaseLen;
}

float CAR_BODY_C::GetTrcRouteAimDist(void) const
{
    return pstCarBodyParam_->stMotionParam.trcRouteAimDist;
}

float CAR_BODY_C::GetCloseEdgeAvoidDist(void) const
{
    return pstCarBodyParam_->stMotionParam.closeEdgeAvoidDist;
}

float CAR_BODY_C::GetBrushFeatherLen(void) const
{
    return pstCarBodyParam_->stBaseParam.brushFeatherLen;
}

float CAR_BODY_C::GetDiffWidthExpandAndNormal(void) const {
    return astStatesParam_[true].stBaseParam.bodyWidth - astStatesParam_[false].stBaseParam.bodyWidth;
}

void CAR_BODY_C::GetCarModelTypeMap(std::unordered_map<string, CAR_TYPE_ENUM>& umModelType) const
{
    for (const CAR_BODY_INFO_STRU &stCarBodyInfo : astCarBodyInfo_)
        umModelType.emplace(stCarBodyInfo.pcCarType, stCarBodyInfo.enCarType);
}

const CAR_BODY_INFO_STRU *CAR_BODY_C::GetCarBodyInfo(const std::string &strModel)
{
    for (const CAR_BODY_INFO_STRU &stCarBodyInfo : astCarBodyInfo_) {
        if (0 != strModel.compare(stCarBodyInfo.pcCarType)) {
            continue;
        }

        return &stCarBodyInfo;
    }

    return nullptr;
}

std::string CAR_BODY_C::GetCarTypeStr(CAR_TYPE_ENUM enCarType)
{
    for (const CAR_BODY_INFO_STRU &stCarBodyInfo : astCarBodyInfo_) {
        if (stCarBodyInfo.enCarType == enCarType) {
            return stCarBodyInfo.pcCarType;
        }
    }

    return "";
}

float CAR_BODY_C::GetTurnRadiusByWheelBaseLen(float wheelBaseLen, float steeringAngle)
{
    return (wheelBaseLen * tan(0.5 * PI - ABS(steeringAngle)));
}

void CAR_BODY_C::SetCarBodyExtendParam(const CAR_BODY_BASE_PARAM_STRU &stBaseParam, CAR_BODY_EXTEND_PARAM_STRU &stExtendParam)
{
    stExtendParam.stFrontWheelPoseByCar_.stPos.x  = stBaseParam.wheelBaseLen;
    stExtendParam.stFrontWheelPoseByCar_.stPos.y  = 0.0;
    stExtendParam.stFrontWheelPoseByCar_.dirAngle = 0.0;

    stExtendParam.stLeftHeadPosByCar_.x       = stBaseParam.headToBackAxisLen;
    stExtendParam.stLeftHeadPosByCar_.y       = 0.5 * stBaseParam.bodyWidth;
    stExtendParam.stRightHeadPosByCar_.x      = stBaseParam.headToBackAxisLen;
    stExtendParam.stRightHeadPosByCar_.y      = -0.5 * stBaseParam.bodyWidth;
    stExtendParam.stLeftTailPosByCar_.x       = -1.0 * stBaseParam.tailToBackAxisLen;
    stExtendParam.stLeftTailPosByCar_.y       = 0.5 * stBaseParam.bodyWidth;
    stExtendParam.stRightTailPosByCar_.x      = -1.0 * stBaseParam.tailToBackAxisLen;
    stExtendParam.stRightTailPosByCar_.y      = -0.5 * stBaseParam.bodyWidth;
    stExtendParam.stLeftBackAxisPosByCar_.x   = 0.0;
    stExtendParam.stLeftBackAxisPosByCar_.y   = 0.5 * stBaseParam.bodyWidth;
    stExtendParam.stRightBackAxisPosByCar_.x  = 0.0;
    stExtendParam.stRightBackAxisPosByCar_.y  = -0.5 * stBaseParam.bodyWidth;
    stExtendParam.stLeftBackWheelPosByCar_.x  = 0.0;
    stExtendParam.stLeftBackWheelPosByCar_.y  = 0.5 * stBaseParam.backWheelWidth;
    stExtendParam.stRightBackWheelPosByCar_.x = 0.0;
    stExtendParam.stRightBackWheelPosByCar_.y = -0.5 * stBaseParam.backWheelWidth;

    float minTurnRadius = GetTurnRadiusByWheelBaseLen(stBaseParam.wheelBaseLen, stBaseParam.maxSteeringAngleAbs);
    stExtendParam.stMaxAngleLeftTurnCenterPosByCar_.x  = 0.0;
    stExtendParam.stMaxAngleLeftTurnCenterPosByCar_.y  = minTurnRadius;
    stExtendParam.stMaxAngleRightTurnCenterPosByCar_.x = 0.0;
    stExtendParam.stMaxAngleRightTurnCenterPosByCar_.y = -1.0 * minTurnRadius;
}

void CAR_BODY_C::SetCarMotionParam(const CAR_BODY_INFO_STRU &stBodyInfo, const CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg, CAR_MOTION_PARAM_STRU &stMotionParam)
{
    float maxForwardVelCfg     = MAX(stBodyInfo.minMoveVelAbs, stVelLimitCfg.maxForwardVel);
    float maxBackwardVelAbsCfg = MAX(stBodyInfo.minMoveVelAbs, stVelLimitCfg.maxBackwardVelAbs);
    float maxCleanVelAbsCfg    = MAX(stBodyInfo.minMoveVelAbs, stVelLimitCfg.maxCleanVelAbs);

    stMotionParam.trcRouteAimDist     = stBodyInfo.trcRouteAimDist;
    stMotionParam.closeEdgeAvoidDist  = stBodyInfo.closeEdgeAvoidDist;

    stMotionParam.minMoveVelAbs             = stBodyInfo.minMoveVelAbs;
    stMotionParam.maxForwardVel             = MIN(maxForwardVelCfg,     stBodyInfo.maxForwardVel);
    stMotionParam.maxForwardVelWithoutLimit = stBodyInfo.maxForwardVel;
    stMotionParam.maxBackwardVelAbs         = MIN(maxBackwardVelAbsCfg, stBodyInfo.maxBackWardVelAbs);
    stMotionParam.maxCleanVelAbs            = MIN(maxCleanVelAbsCfg,    stBodyInfo.maxForwardVel);
    stMotionParam.maxSearchCleanVel         = stMotionParam.maxCleanVelAbs;
}

void CAR_BODY_C::SetNormalStateParam(const CAR_BODY_INFO_STRU &stBodyInfo, const CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg, CAR_BODY_PARAM_STRU &stBodyParam)
{
    stBodyParam.enCarType                       = stBodyInfo.enCarType;

    stBodyParam.stBaseParam.wheelBaseLen        = stBodyInfo.wheelBaseLen;
    stBodyParam.stBaseParam.headToBackAxisLen   = stBodyInfo.wheelBaseLen + stBodyInfo.stDynamicInfo.headLen;
    stBodyParam.stBaseParam.tailToBackAxisLen   = stBodyInfo.tailLen;
    stBodyParam.stBaseParam.bodyWidth           = stBodyInfo.bodyWidth;
    stBodyParam.stBaseParam.bodyLength          = stBodyInfo.bodyLength;
    stBodyParam.stBaseParam.carHeight           = stBodyInfo.carHeight;
    stBodyParam.stBaseParam.backWheelWidth      = stBodyInfo.wheelWidth;
    stBodyParam.stBaseParam.maxSteeringAngleAbs = stBodyInfo.maxSteeringAngleAbs;
    stBodyParam.stBaseParam.brushFeatherLen     = stBodyInfo.brushFeatherLen;

    SetCarBodyExtendParam(stBodyParam.stBaseParam, stBodyParam.stExtendParam);
    SetCarMotionParam(stBodyInfo, stVelLimitCfg, stBodyParam.stMotionParam);
};

void CAR_BODY_C::SetExpandStateParam(const CAR_BODY_INFO_STRU &stBodyInfo, const CFG_PARAM_VEL_LIMIT_STRU &stVelLimitCfg, CAR_BODY_PARAM_STRU &stBodyParam)
{
    stBodyParam.enCarType                       = stBodyInfo.enCarType;

    stBodyParam.stBaseParam.wheelBaseLen        = stBodyInfo.wheelBaseLen;
    stBodyParam.stBaseParam.headToBackAxisLen   = stBodyInfo.wheelBaseLen + stBodyInfo.stDynamicInfo.headLen;
    stBodyParam.stBaseParam.tailToBackAxisLen   = stBodyInfo.tailLen;
    stBodyParam.stBaseParam.bodyWidth           = stBodyInfo.stDynamicInfo.bodyWidthExpand;
    stBodyParam.stBaseParam.bodyLength          = stBodyInfo.bodyLength;
    stBodyParam.stBaseParam.carHeight           = stBodyInfo.carHeight;
    stBodyParam.stBaseParam.backWheelWidth      = stBodyInfo.wheelWidth;
    stBodyParam.stBaseParam.maxSteeringAngleAbs = stBodyInfo.maxSteeringAngleAbs;
    stBodyParam.stBaseParam.brushFeatherLen     = stBodyInfo.brushFeatherLen;

    SetCarBodyExtendParam(stBodyParam.stBaseParam, stBodyParam.stExtendParam);
    SetCarMotionParam(stBodyInfo, stVelLimitCfg, stBodyParam.stMotionParam);
}

void CAR_BODY_C::Print(void) const
{
    ST_LOG_INFO("CarType = %u.", pstCarBodyParam_->enCarType);
    ST_LOG_INFO("stBaseParam.wheelBaseLen = %f.", pstCarBodyParam_->stBaseParam.wheelBaseLen);
    ST_LOG_INFO("stBaseParam.maxSteeringAngleAbs = %f.", pstCarBodyParam_->stBaseParam.maxSteeringAngleAbs);

    ST_LOG_INFO("GetMinTurnRadius() = %f.", GetMinTurnRadius());

    ST_LOG_INFO("trcRouteAimDist = %f.",    pstCarBodyParam_->stMotionParam.trcRouteAimDist);
    ST_LOG_INFO("closeEdgeAvoidDist = %f.", pstCarBodyParam_->stMotionParam.closeEdgeAvoidDist);
}

void DYNC_CAR_C::SetCarPose(const COOR_2D_POSE_STRU &stCarPoseByMap)
{
    stCarPoseByMap_ = stCarPoseByMap;

    objMapToCarTf_.SetTransRelationship(stCarPoseByMap_);

    objMapToCarTf_.TransSonPoseToFather(pstCarBodyParam_->stExtendParam.stFrontWheelPoseByCar_, stCarFrontWheelPoseByMap_);
}

void DYNC_CAR_C::SetFrontWheelPose(const COOR_2D_POSE_STRU &stPoseByMap)
{
    stCarFrontWheelPoseByMap_ = stPoseByMap;

    objMapToCarTf_.SetTransRelationship(stCarFrontWheelPoseByMap_, pstCarBodyParam_->stExtendParam.stFrontWheelPoseByCar_);

    objMapToCarTf_.GetSonOriginPoseByFather(stCarPoseByMap_);
}

const COORDINATE_2D_STRU &DYNC_CAR_C::GetCarPos(void) const
{
    return stCarPoseByMap_.stPos;
}

void DYNC_CAR_C::GetCarPose(COOR_2D_POSE_STRU &stCarPoseByMap) const
{
    stCarPoseByMap = stCarPoseByMap_;
}

const COOR_2D_POSE_STRU &DYNC_CAR_C::GetCarPose(void) const
{
    return stCarPoseByMap_;
}

const COORDINATE_2D_STRU &DYNC_CAR_C::GetFrontWheelPos(void) const
{
    return stCarFrontWheelPoseByMap_.stPos;
}

void DYNC_CAR_C::GetFrontWheelPose(COOR_2D_POSE_STRU &stPoseByMap) const
{
    stPoseByMap = stCarFrontWheelPoseByMap_;
}

void DYNC_CAR_C::GetFrontWheelPose(POSE_2D_C &objPoseByMap) const
{
    objPoseByMap = stCarFrontWheelPoseByMap_;
}

const COOR_2D_POSE_STRU &DYNC_CAR_C::GetFrontWheelPose(void) const
{
    return stCarFrontWheelPoseByMap_;
}

void  DYNC_CAR_C::Get4CornerBrushPos(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                                     COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                                     float sideProtectLen, float headProectLen) const
{
    COORDINATE_2D_STRU stLeftHeadPosByCar, stRightHeadPosByCar, stLeftTailPosByCar, stRightTailPosByCar;
    float carBodyHeadX = pstCarBodyParam_->stBaseParam.bodyLength - pstCarBodyParam_->stBaseParam.tailToBackAxisLen;
    carBodyHeadX = (IsS350() && IsExpand()) ? carBodyHeadX - 0.6f : carBodyHeadX; // 如果是350并且展刷的模式下，则扫刷边要缩回0.6m
    bool is_s3 = IsS330() || IsS332() || IsS333() || IsS334() || IsS335() || IsS336() || IsS337();
    float delta_head_len = (is_s3 && IsExpand()) ? 0.1f : 0.0f; // 如果是330并且展开的模式下,边刷前面的边要短0.1m(实际是0.2m，因为感知过滤模型更大，所以只设置短0.1m)
    delta_head_len = (IsS350() && IsExpand()) ? 0.3f : delta_head_len; // 如果是350并且展刷的模式下，边刷前面的边要短0.3m
    float brushHeadX = pstCarBodyParam_->stBaseParam.headToBackAxisLen - delta_head_len + headProectLen;
    float brushY = pstCarBodyParam_->stBaseParam.bodyWidth / 2.0f + sideProtectLen;
    stLeftHeadPosByCar.SetValue(brushHeadX, brushY);
    stRightHeadPosByCar.SetValue(brushHeadX, -brushY);
    stLeftTailPosByCar.SetValue(carBodyHeadX, brushY);
    stRightTailPosByCar.SetValue(carBodyHeadX, -brushY);
    TransformPosByCarToByMap(stLeftHeadPosByCar, stLeftHeadPosByMap);
    TransformPosByCarToByMap(stRightHeadPosByCar, stRightHeadPosByMap);
    TransformPosByCarToByMap(stLeftTailPosByCar, stLeftTailPosByMap);
    TransformPosByCarToByMap(stRightTailPosByCar, stRightTailPosByMap);
}


void  DYNC_CAR_C::Get4CornerBodyPos(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                                    COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                                    float sideProtectLen, float headProtectLen, float tailProectLen) const
{
    float body_width = GetBodyWidth();
    if (IsExpand())
    {
        body_width -= GetDiffWidthExpandAndNormal();
    }
    COORDINATE_2D_STRU stLeftHeadPosByCar, stRightHeadPosByCar, stLeftTailPosByCar, stRightTailPosByCar;
    float carBodyHeadX = pstCarBodyParam_->stBaseParam.bodyLength - pstCarBodyParam_->stBaseParam.tailToBackAxisLen;
    carBodyHeadX += headProtectLen;
    float carBodyTailX = -pstCarBodyParam_->stBaseParam.tailToBackAxisLen - tailProectLen;
    float bodyY = body_width / 2.0f + sideProtectLen;
    stLeftHeadPosByCar.SetValue(carBodyHeadX, bodyY);
    stRightHeadPosByCar.SetValue(carBodyHeadX, -bodyY);
    stLeftTailPosByCar.SetValue(carBodyTailX, bodyY);
    stRightTailPosByCar.SetValue(carBodyTailX, -bodyY);
    TransformPosByCarToByMap(stLeftHeadPosByCar, stLeftHeadPosByMap);
    TransformPosByCarToByMap(stRightHeadPosByCar, stRightHeadPosByMap);
    TransformPosByCarToByMap(stLeftTailPosByCar, stLeftTailPosByMap);
    TransformPosByCarToByMap(stRightTailPosByCar, stRightTailPosByMap);
}

void  DYNC_CAR_C::Get4CornerWholePos(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap,
                                    COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
                                    float sideProtectLen, float headProectLen, float tailProectLen) const
{
    COORDINATE_2D_STRU stPosByCar;
    bool is_use_expand_model = IsExpand();

    GetLeftHeadPosByCarWithProtect(headProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftHeadPosByMap);

    GetRightHeadPosByCarWithProtect(headProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightHeadPosByMap);

    GetLeftTailPosByCarWithProtect(tailProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftTailPosByMap);

    GetRightTailPosByCarWithProtect(tailProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightTailPosByMap);
}

void  DYNC_CAR_C::GetCarBodyRegion(std::vector<CAR_BODY_REGION_STRU> &vecRegion) const
{
    vecRegion.reserve(5);
    float body_width = GetBodyWidth();
    if (IsExpand())
    {
        body_width -= GetDiffWidthExpandAndNormal();
    }
    float car_x_min = -pstCarBodyParam_->stBaseParam.tailToBackAxisLen;
    float car_x_max = pstCarBodyParam_->stBaseParam.bodyLength - pstCarBodyParam_->stBaseParam.tailToBackAxisLen;
    float car_x_mid1 = std::min(pstCarBodyParam_->stBaseParam.wheelBaseLen, car_x_max-0.1f);
    float car_x_mid2 = 0.0;
    float car_y_min = -body_width / 2.0f;
    float car_y_max = body_width / 2.0f;
    float car_y_mid1 = car_y_max - 0.1f;
    float car_y_mid2 = car_y_min + 0.1f;
    COORDINATE_2D_STRU left_head_local, right_head_local, left_tail_local, right_tail_local;

    CAR_BODY_REGION_STRU head_region;
    head_region.region = CAR_BODY_REGION_ENUM::HEAD;
    left_head_local.SetValue(car_x_max, car_y_mid1);
    right_head_local.SetValue(car_x_max, car_y_mid2);
    left_tail_local.SetValue(car_x_mid1, car_y_mid1);
    right_tail_local.SetValue(car_x_mid1, car_y_mid2);
    TransformPosByCarToByMap(left_head_local, head_region.left_head);
    TransformPosByCarToByMap(right_head_local, head_region.right_head);
    TransformPosByCarToByMap(left_tail_local, head_region.left_back);
    TransformPosByCarToByMap(right_tail_local, head_region.right_back);
    vecRegion.emplace_back(head_region);

    CAR_BODY_REGION_STRU left_region;
    left_region.region = CAR_BODY_REGION_ENUM::LEFT;
    left_head_local.SetValue(car_x_max, car_y_max);
    right_head_local.SetValue(car_x_max, car_y_mid1);
    left_tail_local.SetValue(car_x_mid2, car_y_max);
    right_tail_local.SetValue(car_x_mid2, car_y_mid1);
    TransformPosByCarToByMap(left_head_local, left_region.left_head);
    TransformPosByCarToByMap(right_head_local, left_region.right_head);
    TransformPosByCarToByMap(left_tail_local, left_region.left_back);
    TransformPosByCarToByMap(right_tail_local, left_region.right_back);
    vecRegion.emplace_back(left_region);

    CAR_BODY_REGION_STRU right_region;
    right_region.region = CAR_BODY_REGION_ENUM::RIGHT;
    left_head_local.SetValue(car_x_max, car_y_mid2);
    right_head_local.SetValue(car_x_max, car_y_min);
    left_tail_local.SetValue(car_x_mid2, car_y_mid2);
    right_tail_local.SetValue(car_x_mid2, car_y_min);
    TransformPosByCarToByMap(left_head_local, right_region.left_head);
    TransformPosByCarToByMap(right_head_local, right_region.right_head);
    TransformPosByCarToByMap(left_tail_local, right_region.left_back);
    TransformPosByCarToByMap(right_tail_local, right_region.right_back);
    vecRegion.emplace_back(right_region);

    CAR_BODY_REGION_STRU center_region;
    center_region.region = CAR_BODY_REGION_ENUM::CENTER;
    left_head_local.SetValue(car_x_mid1, car_y_mid1);
    right_head_local.SetValue(car_x_mid1, car_y_mid2);
    left_tail_local.SetValue(car_x_mid2, car_y_mid1);
    right_tail_local.SetValue(car_x_mid2, car_y_mid2);
    TransformPosByCarToByMap(left_head_local, center_region.left_head);
    TransformPosByCarToByMap(right_head_local, center_region.right_head);
    TransformPosByCarToByMap(left_tail_local, center_region.left_back);
    TransformPosByCarToByMap(right_tail_local, center_region.right_back);
    vecRegion.emplace_back(center_region);

    CAR_BODY_REGION_STRU back_region;
    back_region.region = CAR_BODY_REGION_ENUM::BACK;
    left_head_local.SetValue(car_x_mid2, car_y_max);
    right_head_local.SetValue(car_x_mid2, car_y_min);
    left_tail_local.SetValue(car_x_min, car_y_max);
    right_tail_local.SetValue(car_x_min, car_y_min);
    TransformPosByCarToByMap(left_head_local, back_region.left_head);
    TransformPosByCarToByMap(right_head_local, back_region.right_head);
    TransformPosByCarToByMap(left_tail_local, back_region.left_back);
    TransformPosByCarToByMap(right_tail_local, back_region.right_back);
    vecRegion.emplace_back(back_region);

}

void DYNC_CAR_C::Get4CornerPosByCar(COORDINATE_2D_STRU &stLeftHeadPosByCar, COORDINATE_2D_STRU &stRightHeadPosByCar, COORDINATE_2D_STRU &stLeftTailPosByCar, COORDINATE_2D_STRU &stRightTailPosByCar, float sideProtectLen, float faceProectLen) const
{
    bool is_use_expand_model = IsExpand();
    GetLeftHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stLeftHeadPosByCar, is_use_expand_model);
    GetRightHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stRightHeadPosByCar, is_use_expand_model);
    GetLeftTailPosByCarWithProtect(faceProectLen, sideProtectLen, stLeftTailPosByCar, is_use_expand_model);
    GetRightTailPosByCarWithProtect(faceProectLen, sideProtectLen, stRightTailPosByCar, is_use_expand_model);
}

void DYNC_CAR_C::Get4CornerPosByMap(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap, COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap, float sideProtectLen, float faceProectLen) const
{
    COORDINATE_2D_STRU stPosByCar;
    bool is_use_expand_model = IsExpand();

    GetLeftHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftHeadPosByMap);

    GetRightHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightHeadPosByMap);

    GetLeftTailPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftTailPosByMap);

    GetRightTailPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightTailPosByMap);
}

void DYNC_CAR_C::Get4CornerHeadCarExpandModel(COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap, COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap) const
{
    COORDINATE_2D_STRU stPosByCar;
    BOOL is_use_expand_model = true;
    float faceProectLen = 0.0f;
    float sideProtectLen = 0.0f;
    GetLeftHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftHeadPosByMap);

    GetRightHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightHeadPosByMap);

    faceProectLen = -GetBodyLenBeforeFrontWheel();
    GetLeftHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftTailPosByMap);

    GetRightHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightTailPosByMap);
}

void DYNC_CAR_C::Get4CornerCarModelExpand(
    COORDINATE_2D_STRU &stLeftHeadPosByMap, COORDINATE_2D_STRU &stRightHeadPosByMap, 
    COORDINATE_2D_STRU &stLeftTailPosByMap, COORDINATE_2D_STRU &stRightTailPosByMap,
    float sideProtectLen, float faceProectLen) const {
    COORDINATE_2D_STRU stPosByCar;
    const BOOL is_use_expand_model = true;
    GetLeftHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftHeadPosByMap);

    GetRightHeadPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightHeadPosByMap);

    GetLeftTailPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stLeftTailPosByMap);

    GetRightTailPosByCarWithProtect(faceProectLen, sideProtectLen, stPosByCar, is_use_expand_model);
    TransformPosByCarToByMap(stPosByCar, stRightTailPosByMap);
}

void DYNC_CAR_C::GetWheelPosByMap(COORDINATE_2D_STRU &stFrontWheelPosByMap, COORDINATE_2D_STRU &stLeftBackWheelPosByMap, COORDINATE_2D_STRU &stRightBackWheelPosByMap) const
{
    stFrontWheelPosByMap = stCarFrontWheelPoseByMap_.stPos;
    TransformPosByCarToByMap(pstCarBodyParam_->stExtendParam.stLeftBackWheelPosByCar_, stLeftBackWheelPosByMap);
    TransformPosByCarToByMap(pstCarBodyParam_->stExtendParam.stRightBackWheelPosByCar_, stRightBackWheelPosByMap);
}

void DYNC_CAR_C::TransformPosByMapToByCar(const COORDINATE_2D_STRU &stPosByMap, COORDINATE_2D_STRU &stPosByCar) const
{
    objMapToCarTf_.TransFatherPosToSon(stPosByMap, stPosByCar);
}

void DYNC_CAR_C::TransformPosByCarToByMap(const COORDINATE_2D_STRU &stPosByCar, COORDINATE_2D_STRU &stPosByMap) const
{
    objMapToCarTf_.TransSonPosToFather(stPosByCar, stPosByMap);
}

void DYNC_CAR_C::TransformPoseByMapToByCar(const COOR_2D_POSE_STRU &stPoseByMap, COOR_2D_POSE_STRU &stPoseByCar) const
{
    objMapToCarTf_.TransFatherPoseToSon(stPoseByMap, stPoseByCar);
}

void DYNC_CAR_C::TransformPoseByCarToByMap(const COOR_2D_POSE_STRU &stPoseByCar, COOR_2D_POSE_STRU &stPoseByMap) const
{
    objMapToCarTf_.TransSonPoseToFather(stPoseByCar, stPoseByMap);
}

BOOL DYNC_CAR_C::IsPointBehind(const COORDINATE_2D_STRU &stPoint) const
{
    /* 判断是否在车身后轴后面 */
    COORDINATE_2D_STRU stPointByCar;
    TransformPosByMapToByCar(stPoint, stPointByCar);

    return (stPointByCar.x <= 0.0);

}

float MOVE_CAR_C::Move(float steeringAngle, float linearVel)
{
    const COOR_2D_POSE_STRU &stCarPoseOld = GetCarPose();

    float moveLen       = linearVel * timeLenPerMove_;
    float deltaDirAgnle = moveLen * tan(steeringAngle) * inverseOfWheelBaseLen_;
    float moveDir       = stCarPoseOld.dirAngle + 0.5 * deltaDirAgnle;

    COOR_2D_POSE_STRU stCarPoseNew;
    stCarPoseNew.stPos.x  = stCarPoseOld.stPos.x + cos(moveDir) * moveLen;
    stCarPoseNew.stPos.y  = stCarPoseOld.stPos.y + sin(moveDir) * moveLen;
    stCarPoseNew.dirAngle = AdjustAngle(stCarPoseOld.dirAngle + deltaDirAgnle);
    SetCarPose(stCarPoseNew);

    stMoveInfo_.steeringAngle = steeringAngle;
    stMoveInfo_.moveLinearVel = linearVel;
    stMoveInfo_.imuAngularVel = deltaDirAgnle * inverseOfTimeLenPerMove_;
    stMoveInfo_.imuAngle      = stCarPoseNew.dirAngle;

    return ABS(moveLen);
}

void MOVE_CAR_C::MoveLen(float steeringAngle, float len)
{
    const COOR_2D_POSE_STRU &pose = GetCarPose();
    COOR_2D_POSE_STRU new_pose = pose;
    // 如果转向角很小，视为直线行驶
    if (std::fabs(steeringAngle) < 1e-6)
    {
        new_pose.stPos.x = pose.x() + len * std::cos(pose.dirAngle);
        new_pose.stPos.y = pose.y() + len * std::sin(pose.dirAngle);
        SetCarPose(new_pose);
        return;
    }

    // 自行车模型
    float radius = GetWheelBaseLen() / std::tan(steeringAngle); // 转弯半径
    float delta_theta = len / radius;                           // 角度变化
    float half_delta_theta = 0.5 * delta_theta;
    float move_dir_len = 2 * radius * std::sin(half_delta_theta); // 移动距离

    // 计算新位置
    float new_dir_angle = NormalizeAngle(pose.dirAngle + half_delta_theta);
    new_pose.stPos.x = pose.x() + move_dir_len * std::cos(new_dir_angle);
    new_pose.stPos.y = pose.y() + move_dir_len * std::sin(new_dir_angle);
    new_pose.dirAngle = NormalizeAngle(new_pose.dirAngle + delta_theta);

    SetCarPose(new_pose);
}

float MOVE_CAR_C::MoveBackWheelDrive(float steeringAngle, float linearVel)
{
    const COOR_2D_POSE_STRU &stCarPoseOld = GetFrontWheelPose();

    float moveLen       = linearVel * timeLenPerMove_;
    float deltaDirAgnle = moveLen * tan(steeringAngle) * inverseOfWheelBaseLen_;
    float moveDir       = stCarPoseOld.dirAngle + 0.5 * deltaDirAgnle;

    COOR_2D_POSE_STRU stCarPoseNew;
    stCarPoseNew.stPos.x  = stCarPoseOld.stPos.x + cos(moveDir) * moveLen;
    stCarPoseNew.stPos.y  = stCarPoseOld.stPos.y + sin(moveDir) * moveLen;
    stCarPoseNew.dirAngle = AdjustAngle(stCarPoseOld.dirAngle + deltaDirAgnle);
    SetFrontWheelPose(stCarPoseNew);

    stMoveInfo_.steeringAngle = steeringAngle;
    stMoveInfo_.moveLinearVel = linearVel;
    stMoveInfo_.imuAngularVel = deltaDirAgnle * inverseOfTimeLenPerMove_;
    stMoveInfo_.imuAngle      = stCarPoseNew.dirAngle;

    return ABS(moveLen);
}

float MOVE_CAR_C::MoveWithAccLimit(float steeringAngle, float linearVel)
{
    float linearVelWithAccLimit = GetLinearVelWithAccLimit(linearVel);

    return IsS530() ? MoveBackWheelDrive(steeringAngle, linearVelWithAccLimit) : Move(steeringAngle, linearVelWithAccLimit);
}

void MOVE_CAR_C::ResetMoveInfo(void)
{
    stMoveInfo_.steeringAngle = 0.0;
    stMoveInfo_.moveLinearVel = 0.0;
    stMoveInfo_.imuAngularVel = 0.0;
    stMoveInfo_.imuAngle      = 0.0;
}

const CAR_MOVE_INFO_STRU &MOVE_CAR_C::GetMoveInfo(void) const
{
    return stMoveInfo_;
}

float MOVE_CAR_C::GetLinearVelWithAccLimit(float linearVelCmd) const
{
    if ((stMoveInfo_.moveLinearVel * linearVelCmd) < 0.0) {
        /* 速度反向，先刹车减速到0.0 */
        return 0.0;

    } else if (IS_FLOAT_ALMOST_EQUAL(stMoveInfo_.moveLinearVel, linearVelCmd)) {
        /* 速度几乎不变，直接返回新速度 */
        return linearVelCmd;

    } else if ((stMoveInfo_.moveLinearVel >= 0) && (linearVelCmd > stMoveInfo_.moveLinearVel)) {
        /* 前向加速，按照最大加速度加速 */
        float velWithMaxAcc = stMoveInfo_.moveLinearVel + maxVelDiffOfPerPeriodAbs_;
        return MIN(velWithMaxAcc, linearVelCmd);

    } else if ((stMoveInfo_.moveLinearVel > 0) && (linearVelCmd < stMoveInfo_.moveLinearVel)) {
        /* 前向减速，不受减速度限制，直接返回新速度 */
        return linearVelCmd;

    } else if ((stMoveInfo_.moveLinearVel <= 0) && (linearVelCmd < stMoveInfo_.moveLinearVel)) {
        /* 后向加速，按照最大加速度加速 */
        float velWithMaxAcc = stMoveInfo_.moveLinearVel - maxVelDiffOfPerPeriodAbs_;
        return MAX(velWithMaxAcc, linearVelCmd);

    } else if ((stMoveInfo_.moveLinearVel < 0) && (linearVelCmd > stMoveInfo_.moveLinearVel)) {
        /* 后向减速，不受减速度限制，直接返回新速度 */
        return linearVelCmd;

    } else {
        ST_LOG_ERR("Asset!!!! preVel = %f, curVel = %f.", stMoveInfo_.moveLinearVel, linearVelCmd);
        return 0.0;
    }
}

/**************************************************************************************
功能描述: 设置行进方向
修改记录:
**************************************************************************************/
void CAR_DRIVER_C::SetMoveDir(BOOL isGoForward, BOOL isUseFrontWheelToGoForwardTrc)
{
    isGoForward_                   = isGoForward;
    isUseFrontWheelToGoForwardTrc_ = isUseFrontWheelToGoForwardTrc;
}

BOOL CAR_DRIVER_C::IsGoForward(void) const
{
    return isGoForward_;
}

/**************************************************************************************
功能描述: 设置车辆与路径对应的位姿
修改记录:
**************************************************************************************/
void CAR_DRIVER_C::SetRoutePose(const COOR_2D_POSE_STRU &stCarRoutePose)
{
    if (!isGoForward_) {
        /* 后退时，用户路径处理的位置为后轮中心，方向和车向相反。 */
        objCar_.SetCarPose(COOR_2D_POSE_STRU(stCarRoutePose.stPos.x, stCarRoutePose.stPos.y, AdjustAngle(stCarRoutePose.dirAngle + PI)));

    } else if (isUseFrontWheelToGoForwardTrc_) {
        objCar_.SetFrontWheelPose(stCarRoutePose);

    } else {
        objCar_.SetCarPose(stCarRoutePose);
    }
}

void CAR_DRIVER_C::SetRoutePose(const POSE_2D_C &objRoutePose)
{
    if (!isGoForward_) {
        /* 后退时，用户路径处理的位置为后轮中心，方向和车向相反。 */
        objCar_.SetCarPose(COOR_2D_POSE_STRU(objRoutePose.stPos.x, objRoutePose.stPos.y, AdjustAngle(objRoutePose.objDir.Val() + PI)));

    } else if (isUseFrontWheelToGoForwardTrc_) {
        objCar_.SetFrontWheelPose(objRoutePose.ToOld());

    } else {
        objCar_.SetCarPose(objRoutePose.ToOld());
    }
}

/**************************************************************************************
功能描述: 获取车辆与路径对应的位姿
修改记录:
**************************************************************************************/
void CAR_DRIVER_C::GetRoutePose(COOR_2D_POSE_STRU &stCarRoutePose) const
{
    if (!isGoForward_) {
        /* 后退时，用户路径处理的位置为后轮中心，方向和车向相反。 */
        objCar_.GetCarPose(stCarRoutePose);
        stCarRoutePose.dirAngle = AdjustAngle(stCarRoutePose.dirAngle + PI);

    } else if (isUseFrontWheelToGoForwardTrc_) {
        objCar_.GetFrontWheelPose(stCarRoutePose);

    } else {
        objCar_.GetCarPose(stCarRoutePose);
    }
}

/**************************************************************************************
功能描述: 获取车辆前轴中心与路径对应的位姿
修改记录:
**************************************************************************************/
void CAR_DRIVER_C::GetFrontWheelRoutePose(POSE_2D_C &objRoutePose) const
{
    objRoutePose = objCar_.GetFrontWheelPose();
    if (!isGoForward_) {
        /* 后退时，用户路径处理的位置为后轮中心，方向和车向相反。 */
        objRoutePose.objDir.Add(PI);
    }
}

void CAR_DRIVER_C::GetRoutePose(POSE_2D_C &objRoutePose) const
{
    if (!isGoForward_) {
        /* 后退时，用户路径处理的位置为后轮中心，方向和车向相反。 */
        objRoutePose = objCar_.GetCarPose();
        objRoutePose.objDir.Add(PI);

    } else if (isUseFrontWheelToGoForwardTrc_) {
        objRoutePose = objCar_.GetFrontWheelPose();

    } else {
        objRoutePose = objCar_.GetCarPose();
    }
}

/**************************************************************************************
功能描述: 获取跟踪目标的转向角(使用前轮跟踪)
修改记录:
**************************************************************************************/
float CAR_DRIVER_C::GetSteeringAngleOfFrontWheelTrc(const COORDINATE_2D_STRU &stAimPosByMap) const
{
    const COOR_2D_POSE_STRU &stCarFrontWheelPoseByMap = objCar_.GetFrontWheelPose();
    float                    maxSteeringAngleAbs      = objCar_.GetMaxSteeringAngleAbs();

    float steeringAngle = AdjustAngle(Calc2PointLineAngle(&stCarFrontWheelPoseByMap.stPos, &stAimPosByMap) - stCarFrontWheelPoseByMap.dirAngle);
    return LIMIT(steeringAngle, -1.0 * maxSteeringAngleAbs, maxSteeringAngleAbs);
}

/**************************************************************************************
功能描述: 获取后退时跟踪目标的转向角
修改记录:
**************************************************************************************/
float CAR_DRIVER_C::GetSteeringAngleOfBackWheelTrc(const COORDINATE_2D_STRU &stAimPosByMap) const
{
    const COOR_2D_POSE_STRU &stCarPose           = objCar_.GetCarPose();
    float                    maxSteeringAngleAbs = objCar_.GetMaxSteeringAngleAbs();

    /* 假设打转方向盘后，后轮经过一个圆弧到达目标点，则该圆弧的圆心点必在后轮轴上 */
    float backAxisLeftDirUnitVectorX = cos(stCarPose.dirAngle + 0.5 * PI);
    float backAxisLeftDirUnitVectorY = sin(stCarPose.dirAngle + 0.5 * PI);
    float wheelToAimVectorX = stAimPosByMap.x - stCarPose.stPos.x;
    float wheelToAimVectorY = stAimPosByMap.y - stCarPose.stPos.y;

    /* 上述两向量点乘，如果结果为0，表示目标和后轮在一条直线上直接后退即可 */
    float vectorDotProduct = (wheelToAimVectorX * backAxisLeftDirUnitVectorX + wheelToAimVectorY * backAxisLeftDirUnitVectorY);
    if (ABS(vectorDotProduct) < 1.0e-20) {
        return 0.0;
    }

    /* 计算后轮转向半径，大于0表示圆心在车辆左侧，小于0表示圆心在右侧 */
    float turnRadius = 0.5 * (wheelToAimVectorX * wheelToAimVectorX + wheelToAimVectorY * wheelToAimVectorY) / vectorDotProduct;

    /* 根据后轮转弯半径，计算前轮转向角 */
    float steeringAngle = objCar_.GetSteeringAngle(turnRadius);
    return LIMIT(steeringAngle, -1.0 * maxSteeringAngleAbs, maxSteeringAngleAbs);
}

/**************************************************************************************
功能描述: 获取后轮转向模型的转向角
修改记录:
**************************************************************************************/
float CAR_DRIVER_C::GetSteeringAngleOfBackWheelDrive(const COORDINATE_2D_STRU &stAimPosByMap) const
{
    const COOR_2D_POSE_STRU &stCarPose           = objCar_.GetFrontWheelPose();
    float                   maxSteeringAngleAbs = objCar_.GetMaxSteeringAngleAbs();

    /* 假设打转方向盘后，后轮经过一个圆弧到达目标点，则该圆弧的圆心点必在后轮轴上 */
    float backAxisLeftDirUnitVectorX = cos(stCarPose.dirAngle + 0.5 * PI);
    float backAxisLeftDirUnitVectorY = sin(stCarPose.dirAngle + 0.5 * PI);
    float wheelToAimVectorX = stAimPosByMap.x - stCarPose.stPos.x;
    float wheelToAimVectorY = stAimPosByMap.y - stCarPose.stPos.y;

    /* 上述两向量点乘，如果结果为0，表示目标和后轮在一条直线上直接后退即可 */
    float vectorDotProduct = (wheelToAimVectorX * backAxisLeftDirUnitVectorX + wheelToAimVectorY * backAxisLeftDirUnitVectorY);
    if (ABS(vectorDotProduct) < 1.0e-20) {
        return 0.0;
    }

    /* 计算后轮转向半径，大于0表示圆心在车辆左侧，小于0表示圆心在右侧 */
    float turnRadius = 0.5 * (wheelToAimVectorX * wheelToAimVectorX + wheelToAimVectorY * wheelToAimVectorY) / vectorDotProduct;

    /* 根据后轮转弯半径，计算前轮转向角 */
    float steeringAngle = objCar_.GetSteeringAngle(turnRadius);
    return LIMIT(steeringAngle, -1.0 * maxSteeringAngleAbs, maxSteeringAngleAbs);
}

/**************************************************************************************
功能描述: 获取跟踪目标的转向角
修改记录:
**************************************************************************************/
float CAR_DRIVER_C::GetAimPointSteeringAngle(const COORDINATE_2D_STRU &stAimPosByMap) const
{
    if (isGoForward_ && isUseFrontWheelToGoForwardTrc_) {
        return GetSteeringAngleOfFrontWheelTrc(stAimPosByMap);
    } else {
        return GetSteeringAngleOfBackWheelTrc(stAimPosByMap);
    }
}

float CAR_DRIVER_C::GetSimulationVel(void) const
{
    if (isGoForward_) {
        return CAR_SIMULATION_MOVE_VEL;
    } else {
        return CAR_SIMULATION_MOVE_VEL * -1.0;
    }
}

float CAR_DRIVER_C::GetMaxMoveVelAbs(void) const
{
    if (isGoForward_) {
        return objCar_.GetMaxForwardVel();
    } else {
        return objCar_.GetMaxBackwardVelAbs();
    }
}

