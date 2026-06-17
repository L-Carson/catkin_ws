#include "pcpt_comm/pcpt_comm_vision.h"

std::recursive_mutex   PCPT_VISION_FUNC_C::mutex_r_;
PCPT_VISION_FUNC_C*    PCPT_VISION_FUNC_C::pInstance_{nullptr};

VISION_FUNC_STRU PCPT_VISION_FUNC_C::GetVisionFunc(void) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return stFunc_;
}

BOOL  PCPT_VISION_FUNC_C::IsUsingDetLabel(void) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return !stFunc_.stDet.lstUsingLabel.empty();
}

BOOL  PCPT_VISION_FUNC_C::IsUsingSegLabel(void) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    return !stFunc_.stSeg.lstUsingLabel.empty();
}

std::vector<std::string> PCPT_VISION_FUNC_C::UsingCameraId(void) const
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    std::vector<std::string> vstrUsingCamera;
    std::vector<std::string> vstrDetUsingCamera = stFunc_.stDet.UsingCamera(); //检测任务实时使用的相机
    vstrUsingCamera.insert(vstrUsingCamera.end(), vstrDetUsingCamera.begin(), vstrDetUsingCamera.end());

    std::vector<std::string> vstrSegUsingCamera = stFunc_.stSeg.UsingCamera(); //分割任务实时使用的相机
    vstrUsingCamera.insert(vstrUsingCamera.end(), vstrSegUsingCamera.begin(), vstrSegUsingCamera.end());
    //TODO
    std::sort(vstrUsingCamera.begin(), vstrUsingCamera.end());
    auto last = std::unique(vstrUsingCamera.begin(), vstrUsingCamera.end());
    vstrUsingCamera.erase(last, vstrUsingCamera.end());

    return vstrDetUsingCamera;
}

void  PCPT_VISION_FUNC_C::RegFun(const VISION_FUNC_ID_ENUM& enFuncId, const BOOL isUsing)
{
    if (enFuncId == VI_NET_DET) {
        stFunc_.stDet.stAttr.isActive = isUsing ? true : false;
    } else if (enFuncId == VI_NET_SEG) {
        stFunc_.stSeg.stAttr.isActive = isUsing ? true : false;
    } else if (enFuncId == VI_QR_CODE_DET) {
         stFunc_.stQRCode.isActive = isUsing ? true : false;
    } else if (enFuncId == VI_SHORT_OBST_SEG){
         stFunc_.stShortObstSeg.stAttr.isActive = isUsing ? true : false;
    } else if (enFuncId == VI_TRAFFICLIGHT_DET){
         stFunc_.stTrafficLightDet.isActive = isUsing ? true : false;
    } else if (enFuncId == VI_TRASH_BOX_DET ) {
         stFunc_.stTrashBoxDet.isActive = isUsing ? true : false;
    } else if (enFuncId == VI_CLEANCLASS_DET) {
         stFunc_.stCleanClassDet.stAttr.isActive = isUsing ? true : false;
    }
}

void  PCPT_VISION_FUNC_C::RegLabel(const VISION_FUNC_ID_ENUM& enFuncId,
                                   const LABEL_ENUM enLabel, const BOOL isUsing)
{
    std::list<LABEL_ENUM>* pUsingLable = nullptr;
    if (enFuncId == VI_NET_DET) {
        pUsingLable = &stFunc_.stDet.lstUsingLabel;
    } else if (enFuncId == VI_NET_SEG) {
        pUsingLable = &stFunc_.stSeg.lstUsingLabel;
    } else if (enFuncId == VI_SHORT_OBST_SEG) {
        pUsingLable = &stFunc_.stShortObstSeg.lstUsingLabel;
    } else if (enFuncId == VI_TRAFFICLIGHT_DET) {
        //pUsingLable = &stFunc_.stTrafficLightDet.lstUsingLabel;
    } else if (enFuncId == VI_TRASH_BOX_DET) {
        //pUsingLable = &stFunc_.stTrashBoxDet.lstUsingLabel;
    } else if (enFuncId == VI_CLEANCLASS_DET) {
        pUsingLable = &stFunc_.stCleanClassDet.lstUsingLabel;
    }

    std::unique_lock<std::recursive_mutex> lock(mutex_);
    auto iter = find(pUsingLable->begin(), pUsingLable->end(), enLabel);
    if (isUsing) {
        if (iter == pUsingLable->end()) pUsingLable->push_back(enLabel);
    } else {
        if (iter != pUsingLable->end()) iter = pUsingLable->erase(iter);
    }
}

INT32 PCPT_VISION_FUNC_C::InitVisionFunc(void)
{
    PCPT_CFG_C* pobjCfg = PCPT_CFG_C::GetInstance();
    const CFG_PARAM_C& objCfgParam = pobjCfg->GetCfgParam();

    /* 获取所有选配相机的ID */
    const CFG_PARAM_RGB_CAMERAS_STRU& stCameras = objCfgParam.GetRgbCamerasCfg();
    for (const CFG_PARAM_RGB_CAMERA_STRU &stCamera : stCameras.vstCamera){
        vstrOptionalCamera_.push_back(stCamera.strName);
    }

    /* 初始化视觉功能参数 */
    CFG_PARAM_VISION_FUNC_STRU stCfgFunc = objCfgParam.GetVisionFuncCfg();
    stFunc_.isSupport = stCfgFunc.isSupport;
    stFunc_.isUseApi  = stCfgFunc.isUseApi;

    /* 二维码检测参数设置 */
    if (SetVisionFuncAttr(stCfgFunc.stQRCode, VI_QR_CODE_DET, stFunc_.stQRCode))     return -1;
    
    /* 检测参数设置 */
    if (SetVisionFuncAttr(stCfgFunc.stNetDet.stParam, VI_NET_DET, stFunc_.stDet.stAttr)) return -1;
    stFunc_.stDet.vValidLable = stCfgFunc.stNetDet.vValidLabel;

    /* 分割参数设置 */
    if (SetVisionFuncAttr(stCfgFunc.stNetSeg.stParam, VI_NET_SEG, stFunc_.stSeg.stAttr)) return -1;
    stFunc_.stSeg.vValidLable = stCfgFunc.stNetSeg.vValidLabel;

    /* 交通灯检测参数设置 */
    if (SetVisionFuncAttr(stCfgFunc.stTrafficLightDet.stParam, VI_TRAFFICLIGHT_DET, stFunc_.stTrafficLightDet)) return -1;

    /* 洁净度检测参数设置 */
    if (SetVisionFuncAttr(stCfgFunc.stCleanClassDet.stParam, VI_CLEANCLASS_DET, stFunc_.stCleanClassDet.stAttr)) return -1;
    
    return 0;
}

BOOL  PCPT_VISION_FUNC_C::IsOptionalCamera(const std::string& strCamera) const
{
    auto iter = find(vstrOptionalCamera_.begin(), vstrOptionalCamera_.end(), strCamera);
    if (iter == vstrOptionalCamera_.end()) return false;
    return true;
}

INT32 PCPT_VISION_FUNC_C::SetVisionFuncAttr(const CFG_PARAM_VISION_BASE_STRU& stCfg,
                                            const VISION_FUNC_ID_ENUM& enFuncId,
                                            VISION_FUNC_ATTR_STRU&     stFuncAttr)
{
    for (const string& strCamera : stCfg.vstrCamera) {
        if (!IsOptionalCamera(strCamera)) {
            ST_LOG_ERR("当前清扫车不支持<%s>相机，请检查导航配置文件。", strCamera.c_str());
            return -1;
        }
    }

    stFuncAttr.SetValue(stCfg.isSupport, stCfg.isSupportIpu, stCfg.hz, stCfg.vstrCamera, enFuncId);
    isUseIpu_ |= stCfg.isSupportIpu;

    return 0;
}

