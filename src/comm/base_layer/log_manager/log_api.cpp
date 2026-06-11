/*
Author      : gongzhihao
Update Date : 2025/07/30
*/
#include "base_layer/log_manager/log_api.h"

// 构造函数
LogApi::LogApi(const std::string &file_dir,
               const std::string &file_name,
               const LogTypeEnum &log_type,
               int max_file_size,
               int max_file_num,
               int max_day,
               int max_dir_size)
{
    // max_file_size: 最大文件大小，超出大小则分割文件，test_20250721.log.N，其中N代表1、2、3等
    // max_file_num: 最大文件数量，test_20210104.log.N的最大个数，超过则轮询覆盖
    // max_day: 最大保存天数，以当前时间为例，删除当前时间之前的max_day天创建的文件
    // max_dir_size: 最大目录大小，file_dir文件夹内file_name类型的文件超过容量，则删除创建时间最早的文件
    file_dir_ = file_dir;
    file_name_ = file_name;
    log_type_ = log_type;
    max_file_size_ = std::min(max_file_size, 10 * 1024 * 1024);
    max_file_num_ = std::min(max_file_num, 10);
    max_day_ = std::max(max_day, 1);
    max_dir_size_ = std::max(max_dir_size, 10 * 1024 * 1024);

    abs_file_name_ = "";
    last_hour_ = -1;
    last_day_ = -1;

    // 创建目录
    CreateDir();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    // 创建日志
    CreateLogger();
}

// 析构函数
LogApi::~LogApi(void)
{
    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_WARN(logger_, LOG4CPLUS_TEXT("日志文件结束[" + abs_file_name_ + "]"));

    logger_.removeAllAppenders();
    logger_.closeNestedAppenders();
}

// 创建目录
void LogApi::CreateDir(void)
{
    struct stat dir_info;
    if (stat(file_dir_.data(), &dir_info) != 0 || (dir_info.st_mode & S_IFDIR) == 0)
    {
        mkdir(file_dir_.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
}

// 创建日志
void LogApi::CreateLogger(void)
{
    // 获取当前时间
    struct tm *p;
    time_t timep;
    time(&timep);
    p = localtime(&timep);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << 1900 + p->tm_year << std::setw(2) << 1 + p->tm_mon << std::setw(2) << p->tm_mday;
    std::string str_time = (log_type_ == LogTypeEnum::OPERATION_LOG ? "" : ss.str());
    abs_file_name_ = file_dir_ + file_name_ + "_" + str_time + ".log";

    if (!log4cplus::Logger::exists(LOG4CPLUS_TEXT(file_name_)))
    {
        logger_ = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(file_name_));
    }

    if (log4cplus::Logger::exists(LOG4CPLUS_TEXT(file_name_)))
    {
        log4cplus::tstring pattern = LOG4CPLUS_TEXT("%-5p %D{%Y-%m-%d %H:%M:%S,%Q} %m%n");
        log4cplus::SharedAppenderPtr appender(new log4cplus::RollingFileAppender(LOG4CPLUS_TEXT(abs_file_name_),
                                                                                 max_file_size_,
                                                                                 max_file_num_,
                                                                                 true,
                                                                                 true));
        appender->setName(LOG4CPLUS_TEXT(file_name_ + "_file"));
        appender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));

        logger_.setLogLevel(log4cplus::ALL_LOG_LEVEL);
        logger_.removeAllAppenders();
        logger_.addAppender(appender);
    }
}

// 记录Trace
void LogApi::LogTrace(const std::string &data, const std::string &abs_file_name, int line)
{
    // 检查文件
    CheckFiles();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_TRACE(logger_, LOG4CPLUS_TEXT(GetHeader(abs_file_name, line) + data));
}

// 记录Debug
void LogApi::LogDebug(const std::string &data, const std::string &abs_file_name, int line)
{
    // 检查文件
    CheckFiles();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_DEBUG(logger_, LOG4CPLUS_TEXT(GetHeader(abs_file_name, line) + data));
}

// 记录Info
void LogApi::LogInfo(const std::string &data, const std::string &abs_file_name, int line)
{
    // 检查文件
    CheckFiles();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT(GetHeader(abs_file_name, line) + data));
}

// 记录Warn
void LogApi::LogWarn(const std::string &data, const std::string &abs_file_name, int line)
{
    // 检查文件
    CheckFiles();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_WARN(logger_, LOG4CPLUS_TEXT(GetHeader(abs_file_name, line) + data));
}

// 记录Error
void LogApi::LogError(const std::string &data, const std::string &abs_file_name, int line)
{
    // 检查文件
    CheckFiles();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_ERROR(logger_, LOG4CPLUS_TEXT(GetHeader(abs_file_name, line) + data));
}

// 记录Fatal
void LogApi::LogFatal(const std::string &data, const std::string &abs_file_name, int line)
{
    // 检查文件
    CheckFiles();

    // 区域锁
    std::lock_guard<std::mutex> lock(data_mutex_);

    LOG4CPLUS_FATAL(logger_, LOG4CPLUS_TEXT(GetHeader(abs_file_name, line) + data));
}

// 获取日志头
std::string LogApi::GetHeader(const std::string &abs_file_name, int line)
{
    static std::string str_pid = std::to_string(syscall(SYS_gettid));
    std::string file_name = abs_file_name.substr(abs_file_name.find_last_of('/') + 1);
    std::string str_line = std::to_string(line);

    return ("[" + str_pid + ":" + file_name + ":" + str_line + "] ");
}

// 检查文件
void LogApi::CheckFiles(void)
{
    // 操作日志不处理
    if (log_type_ == LogTypeEnum::OPERATION_LOG)
    {
        return;
    }

    // 删除文件每小时
    RemoveFilesPerHour();

    // 获取当前时间
    struct tm *p;
    time_t timep;
    time(&timep);
    p = localtime(&timep);

    // 若日志文件跨天
    if (last_day_ > 0 && last_day_ != p->tm_mday)
    {
        // 创建目录
        CreateDir();

        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        // 创建日志
        CreateLogger();

        LOG4CPLUS_WARN(logger_, LOG4CPLUS_TEXT("日志文件跨天[" + abs_file_name_ + "]"));
    }

    last_day_ = p->tm_mday;

    // 若日志文件被手动删除
    if (abs_file_name_.size() <= 0 || access(abs_file_name_.data(), F_OK) != 0)
    {
        // 创建目录
        CreateDir();

        // 区域锁
        std::lock_guard<std::mutex> lock(data_mutex_);

        // 创建日志
        CreateLogger();

        LOG4CPLUS_WARN(logger_, LOG4CPLUS_TEXT("日志文件被手动删除[" + abs_file_name_ + "]"));
    }
}

// 删除文件每小时
void LogApi::RemoveFilesPerHour(void)
{
    // 获取当前时间
    struct tm *p;
    time_t timep;
    time(&timep);
    p = localtime(&timep);

    // 每小时执行一次
    if (p->tm_hour != last_hour_)
    {
        std::unordered_map<std::string, struct stat> file_properties;

        // 扫描文件
        ScanFiles(file_properties);

        // 删除旧文件
        RemoveOldFiles(file_properties);

        // 累加文件大小
        while (SumFilesSize(file_properties) > max_dir_size_)
        {
            // 删除最早文件
            RemoveFirstFile(file_properties);
        }

        last_hour_ = p->tm_hour;
    }
}

// 扫描文件
void LogApi::ScanFiles(std::unordered_map<std::string, struct stat> &file_properties)
{
    file_properties.clear();

    if (access(file_dir_.data(), F_OK) == 0)
    {
        DIR *p_dir = opendir(file_dir_.data());
        struct dirent *p_entry = nullptr;

        while ((p_entry = readdir(p_dir)) != nullptr)
        {
            // 排除.和..
            if (strlen(p_entry->d_name) > 2)
            {
                struct stat file_info;
                if (stat((file_dir_ + p_entry->d_name).data(), &file_info) == 0)
                {
                    // 是否文件名合法
                    if (IsFileNameLegal(p_entry->d_name))
                    {
                        file_properties[p_entry->d_name] = file_info;
                    }
                }
            }
        }

        closedir(p_dir);
    }
}

// 是否文件名合法
bool LogApi::IsFileNameLegal(const std::string &file_name)
{
    static std::regex regex(R"(^(.+)_(\d{8})\.log(\.\w+)?$)", std::regex::icase | std::regex::optimize);
    return std::regex_match(file_name, regex);
}

// 删除旧文件
void LogApi::RemoveOldFiles(std::unordered_map<std::string, struct stat> &file_properties)
{
    time_t timep;
    time(&timep);
    timep -= max_day_ * 24 * 3600;
    if (timep < 0)
    {
        return;
    }

    for (std::unordered_map<std::string, struct stat>::iterator it = file_properties.begin(); it != file_properties.end();)
    {
        if (it->second.st_mtime < timep)
        {
            // 删除文件
            remove((file_dir_ + it->first).data());

            it = file_properties.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// 累加文件大小
int LogApi::SumFilesSize(const std::unordered_map<std::string, struct stat> &file_properties)
{
    int sum = 0;

    for (const auto &it : file_properties)
    {
        sum += it.second.st_size;
    }

    return sum;
}

// 删除最早文件
void LogApi::RemoveFirstFile(std::unordered_map<std::string, struct stat> &file_properties)
{
    struct tm *p;
    int year_days = g_max_int;
    std::string first_file_name = "";

    for (const auto &it : file_properties)
    {
        p = localtime(&it.second.st_mtime);

        if (p->tm_yday < year_days)
        {
            year_days = p->tm_yday;
            first_file_name = it.first;
        }
    }

    std::unordered_map<std::string, struct stat>::iterator it = file_properties.find(first_file_name);
    if (it != file_properties.end())
    {
        // 删除文件
        remove((file_dir_ + it->first).data());

        file_properties.erase(it);
    }
}
