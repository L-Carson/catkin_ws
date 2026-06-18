
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

    std::string strVisionFuncCfg = stRsp.visionFuncCfg;

    ST_LOG_INFO("%s", strVisionFuncCfg.c_str());

    Json::Value  objJsonVisionFuncCfgs;
    JSONCPP_STRING   strReaderErr;
    Json::CharReaderBuilder objReaderBuiler;
    Json::CharReader  *pobjJsonReader = objReaderBuiler.newCharReader();
    BOOL isSucc = pobjJsonReader->parse(strVisionFuncCfg.c_str(), strVisionFuncCfg.c_str() + strVisionFuncCfg.length(), 
                                        &objJsonVisionFuncCfgs, &strReaderErr);
    delete pobjJsonReader;
    pobjJsonReader = NULL;
    if (!isSucc) {
        ST_LOG_ERR("objReader.parse() Fail. Err: %s.", strReaderErr.c_str());
        return -1;
    }

    if (objJsonVisionFuncCfgs["visionFuncCfg"].isArray()) {
        INT32 num = objJsonVisionFuncCfgs["visionFuncCfg"].size();
        for (INT32 i = 0; i < num; i++) {
            VISION_FUNC_CFG_STRU stVisionFuncCfg;
            Json::Value objJsonVisionFuncCfg = objJsonVisionFuncCfgs["visionFuncCfg"][i];
            stVisionFuncCfg.funcId   = objJsonVisionFuncCfg["funcId"].asUInt();
            stVisionFuncCfg.isUseIpu = objJsonVisionFuncCfg["isUseIpu"].asBool();
            stVisionFuncCfg.hz       = objJsonVisionFuncCfg["hz"].asUInt();
            vectFuncCfg.push_back(stVisionFuncCfg);

            if (stVisionFuncCfg.funcId == (UINT08)VI_NET_DET) {
                if (objJsonVisionFuncCfg["label"].isArray()) {
                    Json::Value objJsonLabel = objJsonVisionFuncCfg["label"];
                    INT32 numLabel = objJsonLabel.size();
                    for (INT32 j = 0; j < numLabel; j++) {
                        UINT32 id = objJsonLabel[j]["id"].asUInt();
                        stLabelCfg.vectDetLabel.push_back(id);
                    }
                }
            }

            if (stVisionFuncCfg.funcId == (UINT08)VI_NET_SEG) {
                if (objJsonVisionFuncCfg["label"].isArray()) {
                    Json::Value objJsonLabel = objJsonVisionFuncCfg["label"];
                    INT32 numLabel = objJsonLabel.size();
                    for (INT32 j = 0; j < numLabel; j++) {
                        UINT32 id = objJsonLabel[j]["id"].asUInt();
                        stLabelCfg.vectSegLabel.push_back(id);
                        if (id == UINT32(TAG_THICKPIPE)) {
                            stLabelCfg.isSupportThickPipe = true;
                            stLabelCfg.thickPipeWidth = objJsonLabel[j]["attribute"]["thickPipeWidth"].asFloat();
                        }
                    }
                }
            }
        }
    }
        
    ST_LOG_INFO("Succ.");
    return 0;
}




