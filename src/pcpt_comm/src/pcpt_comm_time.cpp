#include "pcpt_comm/pcpt_comm_time.h"

/**************************************************************************************
功能描述: get time stamp
修改记录:
**************************************************************************************/
time_t PCPT_TIME_C::GetTimeStamp()
{
    time_t timeStamp;
    time(&timeStamp);
    return timeStamp;
}

/**************************************************************************************
功能描述: convert to stardard time format (GMT)
修改记录:
**************************************************************************************/
char* PCPT_TIME_C::GetGMTFormat()
{
    time_t timeStamp = GetTimeStamp();
    tm* gmt = gmtime(&timeStamp);
    char buffer[sizeof(acTimeFormat_)] = {0};
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", gmt);
    strcpy(acTimeFormat_, buffer);
    return acTimeFormat_;
}

/**************************************************************************************
功能描述: system local timeStamp convert to stardard time format (CST)
修改记录:
**************************************************************************************/
char* PCPT_TIME_C::GetCSTFormat()
{
    time_t timeStamp = GetTimeStamp();
    tm* cst = localtime(&timeStamp);
    char buffer[sizeof(acTimeFormat_)] = {0};
    sprintf(buffer, "%04u-%02u-%02u %02u:%02u:%02u",
            cst->tm_year + 1900,
            cst->tm_mon + 1,
            cst->tm_mday,
            cst->tm_hour,
            cst->tm_min,
            cst->tm_sec);

    strcpy(acTimeFormat_, buffer);
    return acTimeFormat_;
}

/**************************************************************************************
功能描述: time format convert to time stamp
          note: time format默认是CST时间，即当前系统时间
修改记录:
**************************************************************************************/
time_t PCPT_TIME_C::Format2Stamp(const char* pTimeFormat)
{
    /* time format should be such as "2022-11-01 18:02:31" */
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(struct tm));
    strptime(pTimeFormat, "%Y-%m-%d %H:%M:%S", &timeinfo);
    time_t timeStamp = mktime(&timeinfo);
    return timeStamp;
}

/**************************************************************************************
功能描述: time stamp convert to stardard time (GMT)
修改记录:
**************************************************************************************/
tm* PCPT_TIME_C::Stamp2GMTTime(time_t stamp)
{
    struct tm* pTimeinfo = gmtime(&stamp);  //GMT
    pTimeinfo->tm_year += 1900;
    pTimeinfo->tm_mon  += 1;
    timeStamp_ = *pTimeinfo;
    return &timeStamp_;
}

tm* PCPT_TIME_C::Stamp2GMTTime(double stamp)
{
    return Stamp2GMTTime(time_t(stamp));
}

/**************************************************************************************
功能描述: time stamp convert to stardard time (CST)
修改记录:
**************************************************************************************/
tm* PCPT_TIME_C::Stamp2CSTTime(time_t stamp)
{
    stamp += 28800; //GMT->CST(8个小时)
    return Stamp2GMTTime(stamp);
}

tm* PCPT_TIME_C::Stamp2CSTTime(double stamp)
{
    return Stamp2CSTTime(time_t(stamp));
}

/**************************************************************************************
功能描述: time stamp convert to stardard time format (GMT)
修改记录:
**************************************************************************************/
char* PCPT_TIME_C::Stamp2GMTFormat(time_t stamp)
{
    struct tm* pTimeinfo = gmtime(&stamp);  //GMT
    char buffer[sizeof(acTimeFormat_)] = {0};
    int length = strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", pTimeinfo);
    if (!length) return nullptr;
    strcpy(acTimeFormat_, buffer);
    return acTimeFormat_;
}

char* PCPT_TIME_C::Stamp2GMTFormat(double stamp)
{
    return Stamp2GMTFormat(time_t(stamp));
}

/**************************************************************************************
功能描述: time stamp convert to stardard time format (CST)
修改记录:
**************************************************************************************/
char* PCPT_TIME_C::Stamp2CSTFormat(time_t stamp)
{
    stamp += 28800; //GMT->CST(8个小时)
    return Stamp2GMTFormat(stamp);
}

char* PCPT_TIME_C::Stamp2CSTFormat(double stamp)
{
    return Stamp2CSTFormat(time_t(stamp));
}

/**************************************************************************************
功能描述: 转换为自1970年1月1日以来的秒数
修改记录:
**************************************************************************************/
time_t PCPT_TIME_C::MkSecTime(const int year, const int month, const int day)
{
    tm time = {};                   // 必须初始化
    time.tm_year = year  - 1900;    // tm_year是从1900年开始的
    time.tm_mon  = month - 1;       // tm_mon是从0开始的
    time.tm_mday = day;

    return mktime(&time);           // 转换为自1970年1月1日以来的秒数
}

/**************************************************************************************
功能描述: 转换为自1970年1月1日以来的天数
修改记录:
**************************************************************************************/
time_t PCPT_TIME_C::MkDayTime(const int year, const int month, const int day)
{
    time_t sec = MkSecTime(year, month, day);
    return sec / (24 * 3600);
}
