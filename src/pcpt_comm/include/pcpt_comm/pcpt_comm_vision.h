#pragma once

#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_cfg.h"
#include "pcpt_comm/pcpt_comm_singleton.h"

/**
 * 视觉功能属性
 */
struct VISION_FUNC_ATTR_STRU {
    VISION_FUNC_ATTR_STRU(void) { }
    VISION_FUNC_ATTR_STRU(const BOOL isSupport, const BOOL isUseIpu, const UINT32 hz,
                          const std::vector<std::string>& vstrCameraId,
                          const VISION_FUNC_ID_ENUM funcId) {
        SetValue(isSupport, isUseIpu, hz, vstrCameraId, funcId);
    }

    void SetValue(const BOOL isSupport, const BOOL isUseIpu, const UINT32 hz,
                  const std::vector<std::string>& vstrCameraId,
                  const VISION_FUNC_ID_ENUM funcId) {
        this->isSupport    = isSupport;
        this->isUseIpu     = isUseIpu;
        this->hz           = hz;
        this->vstrCameraId = vstrCameraId;
        this->funcId       = funcId;
    }

    BOOL IsChange(VISION_FUNC_ATTR_STRU& stAttr) {
        if (this->isSupport != stAttr.isSupport) return true;
        if (this->hz        != stAttr.hz)        return true;
        return false;
    }

    BOOL IsActive(void)  const { return isActive; }

    VISION_FUNC_ID_ENUM FuncId(void) const {return funcId;}

    VISION_FUNC_ID_ENUM      funcId{VISION_FUNC_ID_ENUM::VI_NULL_FUNC};  //功能ID
    BOOL                     isSupport{false};                           //是否支持
    BOOL                     isActive{false};                            //功能是否激活
    BOOL                     isUseIpu{false};                            //是否使用图像处理单元
    UINT32                   hz{0};                                      //执行频率
    std::vector<std::string> vstrCameraId;                               //相机ID
};

/**
 * 视觉网络
 */
struct VISION_NET_STRU {
    BOOL IsSupport(void) const {return stAttr.isSupport;}
    BOOL IsActive(void)  const {return stAttr.isActive; }
    BOOL IsSupport(LABEL_ENUM enLabel) const {
        return std::find(vValidLable.begin(), vValidLable.end(), enLabel) != vValidLable.end();
    }
    VISION_FUNC_ID_ENUM FuncId(void) const {return stAttr.funcId;}

    /* 当前实时正在使用的相机 */
    std::vector<std::string> UsingCamera(void) const {
        if (!IsSupport()) return {};
        if (!IsActive())  return {};
        return stAttr.vstrCameraId;
    }

    VISION_FUNC_ATTR_STRU   stAttr;         //功能属性参数
    std::vector<LABEL_ENUM> vValidLable;    //可用的类别标签（配置文件中开启的类别标签）
    std::list<LABEL_ENUM>   lstUsingLabel;  //当前任务中正在使用的类别标签
};

/**
 * 视觉功能
 */
struct VISION_FUNC_STRU {
    BOOL                  isSupport{false};   //是否支持视觉检测功能
    BOOL                  isUseApi;           //是否调用API确认
    VISION_FUNC_ATTR_STRU stQRCode;           //二维码检测
    VISION_NET_STRU       stDet;              //网络检测
    VISION_NET_STRU       stSeg;              //网络分割
    VISION_NET_STRU       stShortObstSeg;     //低矮障碍分割
    VISION_FUNC_ATTR_STRU stTrafficLightDet;  //交通灯检测
    VISION_FUNC_ATTR_STRU stTrashBoxDet;      //垃圾箱检测
    VISION_NET_STRU       stCleanClassDet;    //洁净度检测
};

class PCPT_VISION_FUNC_C
{
public:
    PCPT_VISION_FUNC_C(void){ }
    VISION_FUNC_STRU GetVisionFunc(void) const;
    BOOL  IsUseIpu(void) const { return isUseIpu_;       } //是否使用图像处理单元
    BOOL  IsUseApi(void) const { return stFunc_.isUseApi;} //是否调用API
    BOOL  IsUsingDetLabel(void) const;                     //是否正在使用检测类别
    BOOL  IsUsingSegLabel(void) const;                     //是否正在使用分割类别
    std::vector<std::string> UsingCameraId(void) const;    //视觉任务实时使用的相机
    void  RegFun(const VISION_FUNC_ID_ENUM& enFuncId, const BOOL isUsing);  //激活/关闭某视觉功能
    void  RegLabel(const VISION_FUNC_ID_ENUM& enFuncId, const LABEL_ENUM enLabel, const BOOL isUsing);

    static PCPT_VISION_FUNC_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            return pInstance_;
        }

        pInstance_ = new PCPT_VISION_FUNC_C();
        INT32 rslt = pInstance_->InitVisionFunc();
        if (rslt != 0) {
            ST_LOG_ERR("Create Object Fail.");
            DelPtr(pInstance_);
            exit(EXIT_FAILURE);
        }

        ST_LOG_INFO("Create Object Succ.");
        PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

private:
    INT32 InitVisionFunc(void);
    BOOL  IsOptionalCamera(const std::string& strCamera) const;
    INT32 SetVisionFuncAttr(const CFG_PARAM_VISION_BASE_STRU& stCfg,
                            const VISION_FUNC_ID_ENUM& enFuncId,
                            VISION_FUNC_ATTR_STRU& stFuncAttr);

private:
    mutable std::recursive_mutex        mutex_;
    VISION_FUNC_STRU                    stFunc_;
    BOOL                                isUseIpu_{false};
    std::vector<std::string>            vstrOptionalCamera_; //配置文件中选配的相机
    static std::recursive_mutex         mutex_r_;            // 递归锁
    static PCPT_VISION_FUNC_C*          pInstance_;
};
