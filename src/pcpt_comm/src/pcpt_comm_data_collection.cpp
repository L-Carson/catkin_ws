
#include "pcpt_comm/pcpt_comm_data_collection.h"
#include "pcpt_comm/pcpt_comm_time.h"
#include "basic_manager.h"


INT32 DATA_COLLECTION_BASE_C::GetDataDir(std::string &strDataDir) const
{
    strDataDir = SaijieRunDataFolderName(strDataClass_);
    if (strDataDir.empty()) {
        ST_LOG_ERR("SaijieRunDataFolderName() Fail.");
        return -1;
    }

    if (strDataDir[strDataDir.size() - 1] != '/') {
        strDataDir.append("/");
    }

    return  0;
}

INT32 DATA_COLLECTION_BASE_C::GetTimeStr(double timeStamp, std::string &strTime) const
{
    PCPT_TIME_C objTimeProc;
    struct tm *pstTime = objTimeProc.Stamp2CSTTime(timeStamp);
    R_ASSERT(pstTime != nullptr, -1);

    UINT32 mSecond = UINT32((timeStamp - time_t(timeStamp)) * 1000);
    strTime = (  ToString(pstTime->tm_mon,  2, '0') + "-" + ToString(pstTime->tm_mday, 2, '0')
               + "_"
               + ToString(pstTime->tm_hour, 2, '0') + "-" + ToString(pstTime->tm_min,  2, '0') + "-" + ToString(pstTime->tm_sec, 2, '0')
               + "_"
               + ToString(mSecond,          3, '0'));

    return 0;
}



