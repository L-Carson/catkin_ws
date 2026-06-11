/**
 * @file     public_system.h
 * @brief    系统函数二次封装
 * @details  系统函数二次封装
 * @author   华仔
 * @date     9/7/21
 */

#ifndef BASICS_PUBLICSYSTEM_H
#define BASICS_PUBLICSYSTEM_H



#include <iostream>
using namespace std;
#include <mutex>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include <file_system.h>



/**判断是否超时,time单位ms,isTimeOut:为bool类型变量 */
#define IS_TIME_OUT(time, isTimeOut) \
do{\
   static long long last_time = 0;\
   long long cur_time = PublisSystem::GetCurTimeMsLonglong();\
    if(last_time==0 || (cur_time - last_time) >= time){\
		last_time = PublisSystem::GetCurTimeMsLonglong();\
        isTimeOut = true;\
    }else{\
        isTimeOut = false;\
    }\
\
}while(0);

/* 变化了立刻打印，没有变化定时打印*/
/*#define LOG_CHANGE(logger,timeout,logEvent,...) \
do{\
   static std::string last = "";\
   static long long repeat_time = 0;\
   long long cur_time = GetCurTimeMsLonglongLog();\
   std::string value = FormatTostring("" logEvent "",##__VA_ARGS__);\
    if(last != value){\
        LogOutToFile(logger,value,LOGLEVEL_INFO,__func__,__LINE__ );\
		last = value;\
        repeat_time = cur_time;\
    }\
    else if(timeout>0 && cur_time-repeat_time>timeout){\
        LogOutToFile(logger,value,LOGLEVEL_INFO,__func__,__LINE__ );\
        repeat_time = cur_time;\
    }\
\
}while(0);*/

class PublisSystem
{
public:
    /**
     * 构造函数
     */
    PublisSystem();
    /**
     * 析构函数
     */
    ~PublisSystem();
public://时间相关
    /**
     * 得到时间字符串,由于有些需求不支持空格，所以分不同格式
     * @param[in] type 1：格式为2021-09-07_15_01_01     2：格式为2021-09-07 15:01:01
     * @return 时间字符串
     */
    static std::string GetCurTimeStrYYYYMMDDhhmmss(int type=1);
    /**
     * 得到编译时间，格式为yyyy-MM-dd HH:mm:ss 例如：2021-04-12 21:23:21
     * @return 时间字符串
     */
    static std::string GetBuildTimeStrYYYYMMDDhhmmss();
    /**
     * 取得当前设备的毫秒值
     * @return 毫秒值
     */
    static long long GetCurTimeMsLonglong(void);
    /**
     * 取得当前设备的微秒值
     * @return 微秒值
     */
    static long long GetCurTimeUsLonglong(void);
    /**
     * 取得当前设备的秒值
     * @return 秒值
     */
    static long long GetCurTimeSLonglong(void);
    /**
     * 字符串转时间，字符串格式固定
     * @param[in] str 时间字符串，格式为yyyy-mm-dd hh:mm:ss 例如：2021-04-12 21:23:21
     * @return time_t类型时间结构体
     */
    static time_t StringToDatetime(string str);
    /**
     * 判断两个数是否相差N时间内,三者时间的单位必须一致
     * @param[in] t1 时间1
     * @param[in] t2 时间2
     * @param[in] t3 相隔时间
     * @return true:在时间内，false:不在时间内
     */
    static bool IsInTime(long long t1,long long t2,long long t3);
    /**
     * unix时间戳转字符串时间
     * @param s 自1970-1-1 0:0:0以来的秒数。
     * @return 格式为yyyy-MM-dd HH:mm:ss 例如：2021-04-12 21:23:21
     */
    static std::string UnixTimeStr2YYYYMMDDhhmmss(long long s);
public://文件相关
    /**
     * 读取文件内容，可以选择加不加文件锁
     * @param[in] fileName 文件名，绝对路径
     * @param[out] fileData 文件内容缓存地址
     * @param[in,out] fileBufLen 缓存长度,当为in时为fileData长度，out时为读取出来文件内容实际长度
     * @param[in] lock 是否加锁
     * @return 0:成功；其他失败;
     */
    static int ReadFileBinary(std::string fileName,unsigned char* fileData,int& fileBufLen,bool lock=false);
    /**
     * 读取文件内容,可以选择加不加文件锁,可以选择从文件任何地方读取
     * @param[in] fileName 文件名，绝对路径
     * @param[in] filePose 文件偏移
     * @param[out] fileData 文件内容缓存地址
     * @param[out] fileBufLen 读取文件内容长度
     * @param[in] lock 是否加锁
     * @return 0:成功；其他失败;
     */
    static int ReadFileBinary(std::string fileName,int filePose,unsigned char* fileData,int& fileBufLen,bool lock=false);
    /**
     * 可以选择互斥锁写数据到文件中
     * @param[in] fileName 文件名，绝对路径
     * @param[in] fileData 文件内容
     * @param[in] fileBufLen 文件长度
     * @param[in] lock 是否加锁
     * @return 0:成功；其他失败;
     */
    static int WriteFileBinary(std::string fileName,unsigned char* fileData,int fileBufLen,bool lock=false);
    /**
     * 可以选择互斥锁写数据到文件中
     * @param[in] fileName 文件名，绝对路径
     * @param[in] filePose 文件偏移
     * @param[in] fileData 文件内容
     * @param[in] fileBufLen 文件长度
     * @param[in] lock 是否加锁
     * @return 0:成功；其他失败;
     */
    static int WriteFileBinary(std::string fileName,int filePose,unsigned char* fileData,int fileBufLen,bool lock=false);
    /**
     * 读文本文件内容
     * @param[in] fileName 文件名，绝对路径
     * @param[in] data 文件内容
     * @param[in] lock 是否加锁
     * @return 0:成功；其他失败;
     */
    static int ReadFileText(std::string fileName,std::string& data,bool lock=false);
    /**
     * 写文本文件
     * @param[in] fileName 文件名，绝对路径
     * @param[in] data 文件内容
     * @param[in] lock 是否加锁
     * @return 0:成功；其他失败;
     */
    static int WriteFileText(std::string fileName,std::string data,bool lock=false);
    /**
     * 得到文件大小
     * @param[in] fileName 文件名，绝对路径
     * @return 0:成功；其他失败;
     */
    static long long GetFileLength(const char* fileName);
    /**
     * 得到文件夹所有文件信息
     * @param[in] path 文件夹路径
     * @param[out] list 文件信息列表
     * @param[in] son 是否遍历子文件夹
     * @return true:成功；其他失败
     */
    static bool GetFolderFileInfo(std::string path,vector<FileInfoSt>& list,bool son=true);
    /**
     * 得到文件夹所有文件夹信息
     * @param[in] path 文件夹路径
     * @param[out] list 文件信息列表
     * @param[in] son 是否遍历子文件夹
     * @return true:成功；其他失败
     */
    static bool GetFolderInfo(std::string path,vector<FileInfoSt>& list,bool son=true);
public:
    /**
     * 得到当前的用户名
     * @return 用户名;例如saite
     */
    static std::string GetSysUserName();
    /**
     * 得到用户绝对路径
     * @return 用户路径，例如/home/saite
     */
    static std::string GetSysUserPath();
    /**
     * 得到当前程序路径和程序名
     * @param[out] processdir 当前程序路径
     * @param[out] processname 程序名
     * @return 0:成功；其他失败;
     */
    static int GetExecutablePath(std::string& processdir,std::string& processname);
public:
    /**
     * 从一个绝对路径中得到文件名和路径
     * @param[in] path_addr 绝对路径 例如：/home/saite/a.txt
     * @param[out] path 文件路径 例如：/home/saite
     * @param[out] name 文件名 例如：a.txt
     * @return 0:成功；其他失败;
     */
    static int GetPathAndName(std::string path_addr,std::string& path, std::string& name);
public:
    /**
     * 是否一个数字字符串
     * @param[in] str 字符串
     * @return true:是，false:不是
     */
    static bool StringIsNum(std::string str);
    /**
     * hex转字符串
     * @param[in] hex 二进制数据
     * @param[in] hex_len 二进制数据长度
     * @return 字符串
     */
    static std::string HexTostring(unsigned char *hex,int hex_len);
    /**
     * 字符串转hex
     * @param[in] str 字符串
     * @param[out] hex 二进制数据
     * @param[in] hex_len 为hex的长度
     * @return true:成功，其他失败
     */
    static bool StringToHex(std::string& str,unsigned char *hex,int hex_len);
public:
    /**
     * 执行shell命令并获取命令执行结果
     * @param[in] cmd shell命令
     * @param[in] info shell返回的信息
     * @return true:成功，其他失败
     */
    static bool ExecuteCmd(std::string cmd,std::string& info);
};



#endif //NAV_TOOL_PUBLICSYSTEM_H
