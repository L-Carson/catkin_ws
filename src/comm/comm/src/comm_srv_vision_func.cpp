
#include "comm_srv_vision_func.h"


INT32 VISION_FUNC_C::WaitAndGet(VISION_FUNCS_CFG_STRU &stFuncsCfg)
{
    ST_LOG_INFO("Begin...");

    INT32 rslt= WaitServiceAvailable();
    if (rslt != 0) {
        ST_LOG_ERR("WaitServiceAvailable() Fail.");
        return -1;
    }

    rslt = GetParam(stFuncsCfg.vectFuncCfg, stFuncsCfg.stLabelCfg);
    if (rslt != 0) {
        ST_LOG_ERR("GetParam() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 VISION_FUNC_C::WaitServiceAvailable(void)
{
    ST_LOG_INFO("Begin Waiting.");

    UINT32 waitTime      = 0;
    BOOL   isServiceAvailable = false;
    while (ros::ok() && !isServiceAvailable)
    {
        ++waitTime;

        isServiceAvailable = objSrvsClint_.waitForExistence(ros::Duration(1.0));
        ST_LOG_ERR_SAMPLE_IF_TRUE(!isServiceAvailable && (waitTime >= 10), 10, "Waiting %us. Service Not Available Now.", waitTime);
    }

    if (!isServiceAvailable) {
        ST_LOG_ERR("Waiting End With Exception.");
        return -1;
    }

    ST_LOG_INFO("Wait Succ.");
    return 0;
}

INT32 VISION_FUNC_C::GetParam(std::vector<VISION_FUNC_CFG_STRU> &vectFuncCfg, VISION_LABEL_CFG_STRU &stLabelCfg)
{
    ST_LOG_INFO("Begin Calling Service.");

    comm_srvs::visionFunc::Request  stReq;                  /* 为空 */
    comm_srvs::visionFunc::Response stRsp;
    BOOL isCallSucc = objSrvsClint_.call(stReq, stRsp);     /* 阻塞式，无限时间等待响应 */
    if (!isCallSucc) {
        ST_LOG_ERR("Call Service Fail.");
        return -1;
    }

    ST_LOG_INFO("Call Service Succ.");

    if (!stRsp.success) {
        ST_LOG_ERR("Service Proc Fail.");
        return -1;
    }

    ST_LOG_INFO("Service Proc Succ.");

    for (auto cfg : stRsp.visionFuncCfgArray) {
        vectFuncCfg.push_back(VISION_FUNC_CFG_STRU(cfg.funcId, cfg.ipu, cfg.hz));
    }

    stLabelCfg.vectDetLabel       = stRsp.visionLabelCfg.vectDetLabel;
    stLabelCfg.vectSegLabel       = stRsp.visionLabelCfg.vectSegLabel;
    stLabelCfg.isSupportThickPipe = stRsp.visionLabelCfg.isSupportThickPipe;
    stLabelCfg.thickPipeWidth     = stRsp.visionLabelCfg.thickPipeWidth;

    ST_LOG_INFO("Succ.");
    return 0;
}




