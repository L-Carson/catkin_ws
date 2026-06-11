/*
Author      : gongzhihao
Update Date : 2025/07/30
*/
#ifndef LOG_API_H
#define LOG_API_H

#include <regex>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/syscall.h>

#include "log4cplus/log4cplus.h"
#include "comm_layer/comm_function/comm_function.h"

// 日志类型
enum class LogTypeEnum
{
    RUNNING_LOG,   // 运行日志
    OPERATION_LOG, // 操作日志
};

#define LOG_TRACE(logger, logEvent, ...) logger->LogTrace(FormatString("" logEvent "", ##__VA_ARGS__), __FILE__, __LINE__);
#define LOG_DEBUG(logger, logEvent, ...) logger->LogDebug(FormatString("" logEvent "", ##__VA_ARGS__), __FILE__, __LINE__);
#define LOG_INFO(logger, logEvent, ...) logger->LogInfo(FormatString("" logEvent "", ##__VA_ARGS__), __FILE__, __LINE__);
#define LOG_WARN(logger, logEvent, ...) logger->LogWarn(FormatString("" logEvent "", ##__VA_ARGS__), __FILE__, __LINE__);
#define LOG_ERROR(logger, logEvent, ...) logger->LogError(FormatString("" logEvent "", ##__VA_ARGS__), __FILE__, __LINE__);
#define LOG_FATAL(logger, logEvent, ...) logger->LogFatal(FormatString("" logEvent "", ##__VA_ARGS__), __FILE__, __LINE__);

#define LOG_TRACE_HEX(logger, hex_data, hex_len, logEvent, ...) logger->LogTrace(FormatString("" logEvent "", ##__VA_ARGS__) + HexToStr(hex_data, hex_len), __FILE__, __LINE__);
#define LOG_DEBUG_HEX(logger, hex_data, hex_len, logEvent, ...) logger->LogDebug(FormatString("" logEvent "", ##__VA_ARGS__) + HexToStr(hex_data, hex_len), __FILE__, __LINE__);
#define LOG_INFO_HEX(logger, hex_data, hex_len, logEvent, ...) logger->LogInfo(FormatString("" logEvent "", ##__VA_ARGS__) + HexToStr(hex_data, hex_len), __FILE__, __LINE__);
#define LOG_WARN_HEX(logger, hex_data, hex_len, logEvent, ...) logger->LogWarn(FormatString("" logEvent "", ##__VA_ARGS__) + HexToStr(hex_data, hex_len), __FILE__, __LINE__);
#define LOG_ERROR_HEX(logger, hex_data, hex_len, logEvent, ...) logger->LogError(FormatString("" logEvent "", ##__VA_ARGS__) + HexToStr(hex_data, hex_len), __FILE__, __LINE__);
#define LOG_FATAL_HEX(logger, hex_data, hex_len, logEvent, ...) logger->LogFatal(FormatString("" logEvent "", ##__VA_ARGS__) + HexToStr(hex_data, hex_len), __FILE__, __LINE__);

// 日志接口类
class LogApi
{
public:
    // 构造函数
    LogApi(const std::string &file_dir,
           const std::string &file_name,
           const LogTypeEnum &log_type,
           int max_file_size,
           int max_file_num,
           int max_day,
           int max_dir_size);

    // 析构函数
    ~LogApi(void);

    // 记录Trace
    void LogTrace(const std::string &data, const std::string &abs_file_name, int line);

    // 记录Debug
    void LogDebug(const std::string &data, const std::string &abs_file_name, int line);

    // 记录Info
    void LogInfo(const std::string &data, const std::string &abs_file_name, int line);

    // 记录Warn
    void LogWarn(const std::string &data, const std::string &abs_file_name, int line);

    // 记录Error
    void LogError(const std::string &data, const std::string &abs_file_name, int line);

    // 记录Fatal
    void LogFatal(const std::string &data, const std::string &abs_file_name, int line);

private:
    // 创建目录
    void CreateDir(void);

    // 创建日志
    void CreateLogger(void);

    // 获取日志头
    std::string GetHeader(const std::string &abs_file_name, int line);

    // 检查文件
    void CheckFiles(void);

    // 删除文件每小时
    void RemoveFilesPerHour(void);

    // 扫描文件
    void ScanFiles(std::unordered_map<std::string, struct stat> &file_properties);

    // 是否文件名合法
    bool IsFileNameLegal(const std::string &file_name);

    // 删除旧文件
    void RemoveOldFiles(std::unordered_map<std::string, struct stat> &file_properties);

    // 累加文件大小
    int SumFilesSize(const std::unordered_map<std::string, struct stat> &file_properties);

    // 删除最早文件
    void RemoveFirstFile(std::unordered_map<std::string, struct stat> &file_properties);

private:
    std::mutex data_mutex_; // 数据锁

    log4cplus::Logger logger_;

    LogTypeEnum log_type_; // 日志类型

    std::string file_dir_;      // 文件目录
    std::string file_name_;     // 文件名
    std::string abs_file_name_; // 绝对文件名

    int max_file_size_; // 最大文件大小
    int max_file_num_;  // 最大文件数量
    int max_day_;       // 最大保存天数
    int max_dir_size_;  // 最大目录大小

    int last_hour_;
    int last_day_;
};

#endif
