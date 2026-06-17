
#ifndef __PCPT_COMM_DATA_COLLECTION_H__
#define __PCPT_COMM_DATA_COLLECTION_H__

#include "pcpt_comm/pcpt_comm_base.h"
#include "comm/comm_base.h"
#include <string>

/**************************************************************************************
功能描述: 采集数据保存
修改记录:
**************************************************************************************/
class DATA_COLLECTION_BASE_C
{
public:
    DATA_COLLECTION_BASE_C(const std::string &strDataClass) : strDataClass_(strDataClass)
    {
    }

    virtual ~DATA_COLLECTION_BASE_C(void)
    {
    }


protected:
    INT32 GetDataDir(std::string &strDataDir) const;
    INT32 GetTimeStr(double timeStamp, std::string &strTime) const;

private:
    std::string strDataClass_;
};



#endif







