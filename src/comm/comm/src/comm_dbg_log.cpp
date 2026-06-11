
#ifndef __COMM_DBG_LOG_CPP__
#define __COMM_DBG_LOG_CPP__

#include "comm/comm_dbg_log.h"
#include "comm/comm_base.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 日志锁
修改记录:
**************************************************************************************/
mutex g_objLogMutex;

FILE *pLogFileHandle = nullptr;

INT32 InitXXXLog(const std::string &strLogFilePath)
{
    FILE *pLogFileHandleTmp = fopen(strLogFilePath.c_str(), "w");
    if (pLogFileHandleTmp == NULL) {
        ST_LOG_ERR("Open Log File <%s> Fail.", strLogFilePath.c_str());
        return -1;
    }

    g_objLogMutex.lock();
    pLogFileHandle = pLogFileHandleTmp;
    g_objLogMutex.unlock();

    return 0;
}

#endif


