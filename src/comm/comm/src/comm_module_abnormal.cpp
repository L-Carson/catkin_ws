
#include "comm/comm_module_abnormal.h"

namespace COMM
{

void MODULE_ABNORMAL_PUBLISHER_C::SendAbnormalInfo(INT32 errCode)
{
    ST_LOG_ERR_IF(errCode != oldErrCode_, "[%d]ErrCode %d --> %d.", enModule_, oldErrCode_, errCode);
    oldErrCode_ = errCode;

    SendMsg(errCode);
}


void MODULE_ABNORMAL_PUBLISHER_C::SendMsg(INT32 errCode)
{
    comm_msg::module_abnormal_info stMsg;
    stMsg.module  = enModule_;
    stMsg.errCode = errCode;

    objPublisher_.publish(stMsg);
}

BOOL MODULE_ABNORMAL_SUBSCRIBER_C::IsAnyModulesAbnormal(void)
{
    if (!isInitSucc_) {
        return true;
    }

    UINT32 abnormalModuleNum = 0;
    for (const MODULE_ABNORMAL_INFO_STRU &stAbnormalInfo : astModuleAbnormalInfo_) {
        if (stAbnormalInfo.errCode != 0) {
            ++abnormalModuleNum;
        }
    }

    ST_LOG_INFO_IF(abnormalModuleNum != abnormalModuleNum_, "Abnormal Module Num %u --> %u.", abnormalModuleNum_, abnormalModuleNum);
    abnormalModuleNum_ = abnormalModuleNum;

    return (abnormalModuleNum > 0);
}

void MODULE_ABNORMAL_SUBSCRIBER_C::InitModuleAbnormalInfo(void)
{
    isInitSucc_ = false;

    for (MODULE_ABNORMAL_INFO_STRU &stAbnormalInfo : astModuleAbnormalInfo_) {
        stAbnormalInfo.module = ABNORMAL_MODULE_BUTT;
    }

    for (const ABNORMAL_MODULE_INFO_STRU &stModuleInfo : g_astAbnormalModuleInfo) {
        R_ASSERT_2(stModuleInfo.module < ARRAY_SIZE(astModuleAbnormalInfo_), NOTHING, stModuleInfo.module, ARRAY_SIZE(astModuleAbnormalInfo_));

        MODULE_ABNORMAL_INFO_STRU &stAbnormalInfo = astModuleAbnormalInfo_[stModuleInfo.module];
        R_ASSERT_2(stAbnormalInfo.module == ABNORMAL_MODULE_BUTT, NOTHING, stModuleInfo.module, ARRAY_SIZE(astModuleAbnormalInfo_));

        stAbnormalInfo.module  = stModuleInfo.module;
        stAbnormalInfo.strDes  = stModuleInfo.strDes;
        stAbnormalInfo.errCode = 0;
    }

    for (UINT32 loop = 0; loop < ARRAY_SIZE(astModuleAbnormalInfo_); ++loop) {
        MODULE_ABNORMAL_INFO_STRU &stAbnormalInfo = astModuleAbnormalInfo_[loop];

        R_ASSERT_2(stAbnormalInfo.module == loop, NOTHING, stAbnormalInfo.module, loop);
    }

    isInitSucc_ = true;
    ST_LOG_INFO("Succ.");
}

void MODULE_ABNORMAL_SUBSCRIBER_C::ReceiveMsgCallBack(const comm_msg::module_abnormal_info::ConstPtr &pstMsg)
{
    UINT32 module = pstMsg->module;
    INT32 errCode = pstMsg->errCode;

    R_ASSERT_2(module < ARRAY_SIZE(astModuleAbnormalInfo_), NOTHING, module, ARRAY_SIZE(astModuleAbnormalInfo_));

    MODULE_ABNORMAL_INFO_STRU &stAbnormalInfo = astModuleAbnormalInfo_[module];
    ST_LOG_INFO_IF(errCode != stAbnormalInfo.errCode, "Module(%d, %s) ErrCode: %d -> %d.", module, stAbnormalInfo.strDes.c_str(), stAbnormalInfo.errCode, errCode);

    stAbnormalInfo.errCode = errCode;
}









}

