#include "comm/comm_function.h"

std::recursive_mutex    FUN_C::mutex_r_;
FUN_C*                  FUN_C::pInstance_{nullptr};

const FUN_STRU FUN_C::astFun_[] =
{
    /* Function                         S310    S320    S323    S324    S325    S326    S330    S332    S333    S334    S335    S336    S337    S350    S520    S530 */
    //mnl
    //pnc
    //pnp
    {FUN_RAINFALL_DET,                  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false},     // 雨量检测
    {FUN_LIDAR_SEG_OFFSET,              false,  true,   true,   true,   true,   true,   false,  false,  false,  false,  false,  false,  false,  false,  false,  false},     // 雷达射线分割时进行位置偏移
    {FUN_CHARGING_GUN_DET,              false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false},     // 充电枪连接状态检测（基于激光雷达检测，但是雷达看不到）
    {FUN_CHARGING_PLUG_FILTER,          false,  false,  false,  false,  false,  false,  false,  true,   true,   true,   true,   true,   true,   false,  false,  false},     // 充电头过滤（车载）
    {FUN_EQUIPMENT_DET,                 false,  false,  false,  false,  false,  false,  true,   true,   true,   true,   true,   true,   true,   true,   false,  true},      // 外部设备定位检测（垃圾箱、充电桩、固定点）
    {FUN_ANCHOR_STICK_COMPLETE,         false,  false,  false,  false,  false,  false,  false,  true,   true,   true,   true,   true,   true,   false,  false,  false},     // 垃圾箱推杆电机完全伸出时，是否对垃圾箱位姿进行锚定
    {FUN_DUSTBIN_CAPACITY,              false,  false,  false,  false,  false,  false,  false,  true,   true,   true,   true,   true,   true,   false,  false,  false},     // 站点垃圾箱容量检测
    {FUN_TRASH_BOX_FILTER,              false,  false,  false,  false,  false,  false,  true,   true,   true,   true,   true,   true,   true,    true,  false,  false},     // 车载垃圾箱过滤
    {FUN_TRASH_BOX_OBST_STATE,          false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  true},      // 车载垃圾箱区域障碍评估
    {FUN_PARKING_LOCATOR_FILTER,        false,  false,  false,  false,  false,  false,  true,   false,  false,  false,  false,  false,  false,   true,  false,  false},     // 停车定位器过滤（自动充电时用）
    {FUN_MUTUAL_LIDAR_VERIFY,           false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  true,   false},     // 多雷达检测相互校验
    {FUN_REFLECT_SIDE_IGNORE,           false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  true,   false},     // 反光校验忽略车身侧面点
    {FUN_OBST_DENOISE,                  true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true},      // 感知结果降噪
    {FUN_SHORT_OBST_HOLD,               true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true},      // 低矮障碍维持
    {FUN_MULTI_2D_OBST,                 true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true,   true},      // 多帧2D障碍叠加
    {FUN_FRONT_BRUSH_POINTS_PROC,       false,  true,   true,   true,   true,   true,   false,  true,   true,   true,   true,   true,   true,   false,  true,   true},      // 前扫刷处点云处理（拖尾等）
    {FUN_LIDAR_BRACKET_REGION_FILTER,   false,  true,   true,   true,   false,  true,   false,  false,  false,  false,  false,  false,  false,  false,  false,  false},     // 雷达支架区域过滤（减少阳光干扰）
    {FUN_1_CONTACT_EDGE_VIRT_OBST,      true,   true,   true,   true,   false,  true,   false,  false,  false,  false,  false,  false,  false,  true,   true,   false},     // 1条触边虚拟障碍
    {FUN_2_CONTACT_EDGE_VIRT_OBST,      false,  false,  false,  false,  false,  false,  false,  true,   true,   true,   false,  true,   true,   false,  false,  false},     // 2条触边虚拟障碍
    {BLIND_SPOT_OBST_HOLD,              false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,   false,  false},     // 盲区障碍维持

    //sensor
    {FUN_FRONT_BRUSH_ELEVATE_FILTER,    false,  true,   true,   true,   false,  true,   false,  false,  false,  false,  false,  false,  false,  false,  false,  false},     // 前扫刷升降过滤
    {FUN_FRONT_BRUSH_STRETCH_FILTER,    false,  true,   true,   true,   false,  true,   false,  true,   true,   true,   true,   true,   true,   false,  false,  true},      // 前扫刷伸缩过滤
    {FUN_IGNORE_ABNORMAL_WHEN_DUMP,     false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  true},      // 倒垃圾时屏蔽雷达异常
};

/**************************************************************************************
功能描述: 是否支持该功能
修改记录:
**************************************************************************************/
bool FUN_C::IsSupport(FUN_ENUM enFun)
{
    const FUN_STRU* pstFun = GetFunCfg(enFun);
    if (!pstFun) return false;

    switch(enCarType_) {
        case CAR_TYPE_S310: return pstFun->isS310Support;
        case CAR_TYPE_S320: return pstFun->isS320Support;
        case CAR_TYPE_S323: return pstFun->isS323Support;
        case CAR_TYPE_S324: return pstFun->isS324Support;
        case CAR_TYPE_S325: return pstFun->isS325Support;
        case CAR_TYPE_S326: return pstFun->isS326Support;
        case CAR_TYPE_S330: return pstFun->isS330Support;
        case CAR_TYPE_S332: return pstFun->isS332Support;
        case CAR_TYPE_S333: return pstFun->isS333Support;
        case CAR_TYPE_S334: return pstFun->isS334Support;
        case CAR_TYPE_S335: return pstFun->isS335Support;
        case CAR_TYPE_S336: return pstFun->isS336Support;
        case CAR_TYPE_S337: return pstFun->isS337Support;
        case CAR_TYPE_S350: return pstFun->isS350Support;
        case CAR_TYPE_S520: return pstFun->isS520Support;
        case CAR_TYPE_S530: return pstFun->isS530Support;
        default:
            ST_LOG_ERR("Car type is invalid.");
            break;
    }
    return false;
}

/**************************************************************************************
功能描述: 获取车体类型
修改记录:
**************************************************************************************/
int FUN_C::GetCarType(string strModel, CAR_TYPE_ENUM& enCarType)
{
    auto iter = umModelType_.find(strModel);
    if (iter == umModelType_.end()) return -1;
    enCarType = iter->second;
    return 0;
}

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int FUN_C::Init()
{
    CFG_PARAM_C objCfgParam;
    if (objCfgParam.Init()) {
        ST_LOG_ERR("objCfgParam_.Init() Fail.");
        return -1;
    }
    const CFG_PARAM_STRUCTURE_INFO_STRU &stStructureInfo = objCfgParam.GetStructureInfoCfg();

    objCarBody_.GetCarModelTypeMap(umModelType_);

    if (GetCarType(stStructureInfo.strModel, enCarType_)) {
        ST_LOG_ERR("Failed to get car <%s> type.", stStructureInfo.strModel.c_str());
        return -1;
    }

    ST_LOG_INFO("Init Success.");
    return 0;
}

/**************************************************************************************
功能描述: 获取功能配置
修改记录:
**************************************************************************************/
const FUN_STRU* FUN_C::GetFunCfg(FUN_ENUM enFun) const
{
    for (const FUN_STRU &stFun : astFun_)
        if (stFun.enFun == enFun) return &stFun;

    ST_LOG_ERR("Failed to get valid function cfg.");
    return nullptr;
}



