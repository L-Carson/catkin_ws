#pragma once

#include "comm/comm_dbg_log.h"
#include "comm/comm_cfg_param.h"
#include "comm/comm_car.h"
#include <boost/thread.hpp>
#include <mutex>

/**
 * 感知功能
 */
enum FUN_ENUM {
    //mnl
    //pnc
    //pnp
    FUN_RAINFALL_DET,               // 雨量检测
    FUN_LIDAR_SEG_OFFSET,           // 雷达射线分割时进行位置偏移
    FUN_CHARGING_GUN_DET,           // 充电枪连接状态检测
    FUN_CHARGING_PLUG_FILTER,       // 充电头过滤（车载）
    FUN_EQUIPMENT_DET,              // 外部设备检测
    FUN_ANCHOR_STICK_COMPLETE,      // 垃圾箱推杆电机完全伸出时，是否对垃圾箱位姿进行锚定
    FUN_DUSTBIN_CAPACITY,           // 站点垃圾箱容量检测
    FUN_TRASH_BOX_FILTER,           // 车载垃圾箱过滤
    FUN_TRASH_BOX_OBST_STATE,       // 车载垃圾箱区域障碍状态
    FUN_PARKING_LOCATOR_FILTER,     // 停车定位器过滤（自动充电时用）
    FUN_MUTUAL_LIDAR_VERIFY,        // 多雷达检测相互校验
    FUN_REFLECT_SIDE_IGNORE,        // 反光校验忽略车身侧面点
    FUN_OBST_DENOISE,               // 感知结果降噪
    FUN_SHORT_OBST_HOLD,            // 低矮障碍维持
    FUN_MULTI_2D_OBST,              // 多帧2D障碍叠加
    FUN_FRONT_BRUSH_POINTS_PROC,    // 前扫刷处点云处理（拖尾等）
    FUN_LIDAR_BRACKET_REGION_FILTER,// 雷达支架区域过滤（减少阳光干扰）
    FUN_1_CONTACT_EDGE_VIRT_OBST,   // 1条触边虚拟障碍
    FUN_2_CONTACT_EDGE_VIRT_OBST,   // 2条触边虚拟障碍
    BLIND_SPOT_OBST_HOLD,           // 盲区障碍维持

    //sensor
    FUN_FRONT_BRUSH_ELEVATE_FILTER, // 前扫刷升降过滤
    FUN_FRONT_BRUSH_STRETCH_FILTER, // 前扫刷伸缩过滤
    FUN_IGNORE_ABNORMAL_WHEN_DUMP,  // 倒垃圾时屏蔽雷达异常

    //null
    FUN_NULL,                       // 缺省值
};

/**
 * 功能配置
 */
struct FUN_STRU {
    FUN_ENUM    enFun{FUN_NULL};
    bool        isS310Support{false};
    bool        isS320Support{false};
    bool        isS323Support{false};
    bool        isS324Support{false};
    bool        isS325Support{false};
    bool        isS326Support{false};
    bool        isS330Support{false};
    bool        isS332Support{false};
    bool        isS333Support{false};
    bool        isS334Support{false};
    bool        isS335Support{false};
    bool        isS336Support{false};
    bool        isS337Support{false};
    bool        isS350Support{false};
    bool        isS520Support{false};
    bool        isS530Support{false};
};

class FUN_C
{
public:
    FUN_C(){}
    FUN_C(const FUN_C &other) = delete;         // 禁止使用拷贝构造函数
    FUN_C &operator=(const FUN_C &) = delete;   // 禁止使用拷贝赋值运算符

    bool IsSupport(FUN_ENUM enFun);

    /* 返回对象指针 */
    static FUN_C *GetInstance() {
        if (pInstance_) return pInstance_;
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);
        pInstance_ = new FUN_C();
        if (!pInstance_ || pInstance_->Init()) {
            ST_LOG_ERR("Create FUN_C Object Fail.");
            delete pInstance_;
            pInstance_ = nullptr;
            exit(EXIT_FAILURE);
        }
        return pInstance_;
    }

private:
    CAR_BODY_C      objCarBody_;                    // 车身参数
    CAR_TYPE_ENUM   enCarType_;
    std::unordered_map<string, CAR_TYPE_ENUM> umModelType_; //<strModel - carType>

    static std::recursive_mutex mutex_r_;       // 递归锁
    static FUN_C*               pInstance_;
    static const FUN_STRU       astFun_[];

    int  GetCarType(string strModel, CAR_TYPE_ENUM& enCarType);
    int  Init();
    const FUN_STRU* GetFunCfg(FUN_ENUM enFun) const;
};
