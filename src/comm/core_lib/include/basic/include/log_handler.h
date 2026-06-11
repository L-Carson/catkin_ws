/** 
 * @file     log_handler.h
 * @brief    日志打印的封装
 * @details  提供日志封装接口
 *           文件名称格式：文件标识+日期+.log.N ---->例如 basics_lib20210907.log, basics_lib20210907.log.1
 *           文件内容，每行格式：行头+空格+实际内容
 *                            行头格式为：[时间 日志级别 线程ID 函数名 行号]不同字段之间以空格区分以便后续做统计分析，例如：[2021-08-13,11:28:28,866.676 INFO 2505 wsHelperServerSendCallback 322]
 *           日志采用宏的理由：1.设计遵循的原则是调用者使用方便，传入参数越少越好。__func__，__LINE__不能每次让调用者传入，C++默认参数也能做到，为什么不选c++类的原因后面说到
 *                          2.日志打印采用了可选参数，和printf参数一样打印。
 *                          3.如果采用c++类的方式调用，可选参数和默认参数都要放在最后面，做不到。调用者每次都要传入__func__，__LINE__，调用者也很麻烦。这就是为什么开源的日志库很多采用宏的原因，例如谷歌日志库glog
 * @author   华仔
 * @date     9/7/21 
 */

#ifndef BASICS_LIB_LOGHANDLER_H
#define BASICS_LIB_LOGHANDLER_H

#include <iostream>

using namespace std;


//zeromqHandler *zmq_pub;

/**
 * 日志级别
 */
typedef enum LOGLEVEL_ENUM_{
    LOGLEVEL_TRACE =0,     ///< 跟踪：指明程序运行轨迹，比DEBUG级别的粒度更细
    LOGLEVEL_DEBUG,        ///< 调试：指明细致的事件信息，对调试应用最有用
    LOGLEVEL_INFO,         ///< 信息：指明描述信息，从粗粒度上描述了应用运行过程
    LOGLEVEL_WARN,         ///< 警告：指明可能潜在的危险状况
    LOGLEVEL_ERROR,        ///< 错误：指明错误事件，但应用可能还能继续运行
    LOGLEVEL_FATAL,         ///< 致命：指明非常严重的可能会导致应用终止执行错误事件
    LOGLEVEL_ENCRY
}LOGLEVEL_ENUM;

/** 日志等待超时时间，单位微妙*/
#define LOG_WAITE_TIME  1000

/** 打印日志 */
#define LOG_ALL(logger,level, logEvent,...)                         LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),level,__func__,__LINE__ );
/** 打印跟踪日志 */
#define LOG_TRACE(logger, logEvent,...)                             LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_TRACE,__func__,__LINE__ );
/** 打印调试日志 */
#define LOG_DEBUG(logger, logEvent,...)                             LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_DEBUG,__func__,__LINE__ );
/** 打印信息日志 */
#define LOG_INFO(logger, logEvent,...)                              LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_INFO,__func__,__LINE__ );
/** 打印警告日志 */
#define LOG_WARN(logger, logEvent,...)                              LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_WARN,__func__,__LINE__ );
/** 打印错误日志 */
#define LOG_ERROR(logger, logEvent,...)                             LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_ERROR,__func__,__LINE__ );
/** 打印致命日志 */
#define LOG_FATAL(logger, logEvent,...)                             LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_FATAL,__func__,__LINE__ );
/** 打印加密日志 */
#define LOG_ENCRY(logger, logEvent,...)                             LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),LOGLEVEL_ENCRY,__func__,__LINE__ );

/** 打印日志 字节流*/
#define LOG_ALL_HEX(logger,level,hex_data,hex_len,logEvent,...)     LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),level,__func__,__LINE__ );
/** 打印跟踪日志 字节流*/
#define LOG_TRACE_HEX(logger,hex_data,hex_len,logEvent,...)         LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_TRACE,__func__,__LINE__ );
/** 打印调试日志 字节流*/
#define LOG_DEBUG_HEX(logger,hex_data,hex_len,logEvent,...)         LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_DEBUG,__func__,__LINE__ );
/** 打印信息日志 字节流*/
#define LOG_INFO_HEX(logger,hex_data,hex_len,logEvent,...)          LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_INFO,__func__,__LINE__ );
/** 打印警告日志 字节流*/
#define LOG_WARN_HEX(logger,hex_data,hex_len,logEvent,...)          LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_WARN,__func__,__LINE__ );
/** 打印错误日志 字节流*/
#define LOG_ERROR_HEX(logger,hex_data,hex_len,logEvent,...)         LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_ERROR,__func__,__LINE__ );
/** 打印致命日志 字节流*/
#define LOG_FATAL_HEX(logger,hex_data,hex_len,logEvent,...)         LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_FATAL,__func__,__LINE__ );
/** 打印加密日志 字节流*/
#define LOG_ENCRY_HEX(logger,hex_data,hex_len,logEvent,...)         LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),LOGLEVEL_ENCRY,__func__,__LINE__ );

/** 每隔N次打印一次日志  */
#define LOG_EVERY(logger,type,count, logEvent,...) \
do{\
   static int log_repeat_cont=0;\
    if(log_repeat_cont==0 || log_repeat_cont>=count){\
        LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),type,__func__,__LINE__ );\
    }\
    if(log_repeat_cont>=count){\
        log_repeat_cont = 0;\
    }\
    log_repeat_cont++;\
\
}while(0);

/** 每隔N次打印一次日志  字节流*/
#define LOG_EVERY_HEX(logger,type,count,hex_data,hex_len, logEvent,...) \
do{\
   static int log_repeat_cont=0;\
    if(log_repeat_cont==0 || log_repeat_cont>=count){\
        LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),type,__func__,__LINE__ );\
    }\
    if(log_repeat_cont>=count){\
        log_repeat_cont = 0;\
    }\
    log_repeat_cont++;\
\
}while(0);

/** 每隔N毫秒打印一次日志  */
#define LOG_TIMEOUT(logger,type,time, logEvent,...) \
do{\
   static long long repeat_time = 0;\
   long long cur_time = GetCurTimeMsLonglongLog();\
    if(repeat_time==0 || (cur_time - repeat_time) >= time){\
        LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),type,__func__,__LINE__ );\
		repeat_time = GetCurTimeMsLonglongLog();\
    }\
\
}while(0);

/** 每隔N毫秒打印一次日志  字节流*/
#define LOG_TIMEOUT_HEX(logger,type,time,hex_data,hex_len,logEvent,...) \
do{\
   static long long repeat_time = 0;\
   long long cur_time = GetCurTimeMsLonglongLog();\
    if(repeat_time==0 || (cur_time - repeat_time) >= time){\
        LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),type,__func__,__LINE__ );\
		repeat_time = GetCurTimeMsLonglongLog();\
    }\
\
}while(0);

/** 条件打印一次日志  */
#define LOG_ISOUT(logger,type,isPrint, logEvent,...) \
do{\
    if(isPrint){\
        LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__),type,__func__,__LINE__ );\
    }\
}while(0);

/** 条件打印一次日志  字节流*/
#define LOG_ISOUT_HEX(logger,type,isPrint,hex_data,hex_len,logEvent,...) \
do{\
    if(isPrint){\
        LogOutToFile(logger,FormatTostring("" logEvent "",##__VA_ARGS__)+HexTostring(hex_data,hex_len),type,__func__,__LINE__ );\
    }\
}while(0);

/**
 *日志打印初始化
 * @param[in] flag true:日志内容输出屏幕 false：日志内容不输出屏幕
 * @return 0：返回设置的值
 */
bool SetLogScreenOutput(bool flag=true);
/**
 *日志打印初始化
 * @param[in] flag true:直接发送信息给其他进程写入日志文件 false：在本进程把消息内容写入日志文件
 * @param[in] filepath:文件保存路径
 * @return 0：成功，其他失败
 */
int LogOutInit(std::string filepath,bool flag=false);
/**
 * 设置文件保存的路径。如果不设置默认保存在LogOutInit函数filepath下，增加此接口的目的是为了同一个进程可以保存日志在不同的文件夹下
 * @param[in] file_flag 文件标识
 * @param[in] file_path 对应的文件标识保存的文件路径
 * @return 0：成功，其他失败
 */
int SetLogSavePath(std::string file_flag,std::string file_path);
/**
 * 设置打印时间格式，是否打印纳秒
 * @param ns true:支持纳秒，false:不支持纳秒
 */
void SetTimeUS(bool ns);
/**
 * 释放日志模块申请的资源
 * @return
 */
int LogOutDestory();
/**
 *日志打印
 * @param[in] file_flag 文件标识，做为文件名的一部分
 * @param[in] msg 打印消息内容
 * @param[in] level 日志等级
 * @param[in] func 函数名
 * @param[in] line 行号
 * @return 0：成功，其他失败
 */
int LogOutToFile(string file_flag,string msg,LOGLEVEL_ENUM level,string func,int line);
/**
 *日志打印 msg数据原样写入日志文件中
 * @param[in] file_flag 文件标识，做为文件名的一部分
 * @param[in] msg 打印消息内容
 * @return 0：成功，其他失败
 */
int LogOutToFile(string file_flag,string msg);
/**
 *日志打印 msg数据原样写入日志文件中,log_manager专用
 * @param[in] file_flag 文件标识，做为文件名的一部分
 * @param[in] path_key 查找文件保存在哪个目录
 * @param[in] msg 打印消息内容
 * @return 0：成功，其他失败
 */
int LogOutToFileLogManager(string file_flag,string path_key,string msg);
/**
 *省略参数转字符串
 * @param fmt 格式化参数
 * @param ... 省略参数
 * @return 返回转好的字符串
 */
std::string FormatTostring(const char *fmt, ...);
/**
 * 二进制数据转字符串
 * @param hex 二进制数据
 * @param hex_len 二进制数据的长度
 * @return 返回转好的字符串
 */
std::string HexTostring(unsigned char *hex,int hex_len);
/**
 *获取1970-01-01 00:00:00 到当前时间的毫秒数
 * @return 返回1970-01-01 00:00:00 到当前时间的毫秒数
 */
long long GetCurTimeMsLonglongLog(void);

#endif //BASICS_LIB_LOGHANDLER_H