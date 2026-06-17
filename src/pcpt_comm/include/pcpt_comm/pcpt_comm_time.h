#ifndef __PCPT_COMM_TIME_H__
#define __PCPT_COMM_TIME_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <time.h>

/**
 * Unix 时间戳是从1970年1月1日（UTC/GMT的午夜）开始所经过的秒数，不考虑闰秒。
 * GMT(Greenwich Mean Time)代表格林尼治标准时间，时间戳用的就是GMT时间。
 * CST可以同时表示美国，澳大利亚，中国，古巴四个国家的标准时间。
 */

/**
struct tm {
    int tm_sec,     // 秒 (0-59)
    int tm_min,     // 分 (0-59)
    int tm_hour,    // 时 (0-23)
    int tm_mday,    // 天 (1-31)
    int tm_mon,     // 月 (0–11）
    int tm_year,    // 年 (当前年份减去1970年或1900)
    int tm_wday     // 星期 (0–6, 星期日=0)
    int tm_yday,    // 天 (0-365)
    int tm_isdst,   // 夏令时标识符
}
*/

// typedef long time_t

/**************************************************************************************
功能描述: time, stamp, format, etc. conversion
修改记录:
**************************************************************************************/
class PCPT_TIME_C
{
public:
    PCPT_TIME_C(){}
    ~PCPT_TIME_C(){}

    time_t GetTimeStamp();
    char*  GetGMTFormat();
    char*  GetCSTFormat();

    time_t Format2Stamp(const char* pTimeFormat);

    tm*  Stamp2GMTTime(time_t stamp);
    tm*  Stamp2GMTTime(double stamp);
    tm*  Stamp2CSTTime(time_t stamp);
    tm*  Stamp2CSTTime(double stamp);

    char* Stamp2GMTFormat(time_t stamp);
    char* Stamp2GMTFormat(double stamp);
    char* Stamp2CSTFormat(time_t stamp);
    char* Stamp2CSTFormat(double stamp);

    time_t MkSecTime(const int year, const int month, const int day);
    time_t MkDayTime(const int year, const int month, const int day);

private:
    tm   timeStamp_;
    char acTimeFormat_[20];
};

#endif
