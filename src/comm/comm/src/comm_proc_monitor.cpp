
#include "comm/comm_proc_monitor.h"
#include <malloc.h>                                 /* malloc_stats */


/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::Init()
{
    if (!strProcName_.empty()) {
        pid_ = GetPid_(strProcName_.c_str());   //监控指定进程
    } else {
        pid_ = GetTid_();
        strProcName_ = GetProcNameByPid(pid_);  //监控当前进程
    }

    if (strProcName_.empty()) return -1;

    char fileName[64] = {0};
    sprintf(fileName, "/proc/%d/task/", pid_);
    strTaskPath_ = fileName;

    cpuNum_ = GetCpuNum();

    isMonitorWork_ = true;
    monitoeThread_ = std::thread(&PROC_MONITOR_C::Monitor, this);

    auto position = strProcName_.find("_");
    if (position == string::npos) position = strProcName_.size();
    std::string strThreadName = strProcName_.substr(0, position) + "_monitor";
    pthread_setname_np(monitoeThread_.native_handle(), strThreadName.c_str());
    return 0;
}

/**************************************************************************************
功能描述: 获取CPU线程数
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::GetCpuNum()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

/**************************************************************************************
功能描述: 获取CPU占用时间信息
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::GetCpuOccupyInfo(CPU_TOTAL_OCCUPY_STRU &stCpu)
{
    return ParseCpuStatFile(stCpu);
}

/**************************************************************************************
功能描述: 获取总的CPU时间
修改记录:
**************************************************************************************/
unsigned long PROC_MONITOR_C::GetCpuOccupy()
{
    CPU_TOTAL_OCCUPY_STRU stCpu;
    if (GetCpuOccupyInfo(stCpu)) return 0;
    return stCpu.Total();
}

/**************************************************************************************
功能描述: 获取总的CPU占用率（Test）
修改记录:
**************************************************************************************/
float PROC_MONITOR_C::GetCpuOccupyPercentage()
{
    CPU_TOTAL_OCCUPY_STRU stCpu1;
    if (GetCpuOccupyInfo(stCpu1)) return 0;
    usleep(200000);

    CPU_TOTAL_OCCUPY_STRU stCpu2;
    if (GetCpuOccupyInfo(stCpu2)) return 0;

    unsigned long cpuOccupyTime = stCpu2.Total() - stCpu1.Total();
    if (!cpuOccupyTime) return 0;

    unsigned long cpuIdleTime = stCpu2.idle - stCpu1.idle;
    return (1 - 1.0 * cpuIdleTime / cpuOccupyTime) * 100.0 * cpuNum_;
}

/**************************************************************************************
功能描述: 获取进程占用CPU的时间信息
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::GetProcCpuOccupyInfo(const uint32_t pid, CPU_PROC_OCCUPY_STRU &stCpu)
{
    char fileName[64] = {0};
    sprintf(fileName, "/proc/%d/stat", pid);
    if (ParseProcStatFile(fileName, stCpu)) return -1;
    return 0;
}

/**************************************************************************************
功能描述: 获取进程的CPU时间
修改记录:
**************************************************************************************/
unsigned long PROC_MONITOR_C::GetProcCpuOccupy(const uint32_t pid)
{
    CPU_PROC_OCCUPY_STRU stCpu;
    if (GetProcCpuOccupyInfo(pid, stCpu)) return 0;
    return stCpu.Total();
}

/**************************************************************************************
功能描述: 获取进程的CPU占用率（Test）
修改记录:
**************************************************************************************/
float PROC_MONITOR_C::GetProcCpuPercentage(const uint32_t pid)
{
    unsigned long cpuOccupy1, cpuOccupy2;
    unsigned long cpuProcOccupy1, cpuProcOccupy2;

    cpuOccupy1     = GetCpuOccupy();
    cpuProcOccupy1 = GetProcCpuOccupy(pid);

    usleep(200000);

    cpuOccupy2     = GetCpuOccupy();
    cpuProcOccupy2 = GetProcCpuOccupy(pid);

    float pcpu = 0.0;
    if (0 != cpuOccupy2 - cpuOccupy1)
        pcpu = 100.0 * (cpuProcOccupy2 - cpuProcOccupy1) / (cpuOccupy2 - cpuOccupy1);

    return pcpu;
}

/**************************************************************************************
功能描述: 根据进程号获取进程名
修改记录:
**************************************************************************************/
string PROC_MONITOR_C::GetProcNameByPid(const uint32_t pid)
{
    CPU_PROC_OCCUPY_STRU stCpu;
    if (GetProcCpuOccupyInfo(pid, stCpu)) return "";
    return stCpu.strName;
}

/**************************************************************************************
功能描述: 获取进程占用的内存
修改记录:
**************************************************************************************/
unsigned int PROC_MONITOR_C::GetProcMem(const uint32_t pid)
{
    char file_name[64] = {0};
    FILE *fd;
    char line_buff[512] = {0};
    sprintf(file_name, "/proc/%d/status", pid);

    fd =fopen(file_name, "r");
    if(nullptr == fd) return 0;

    char name[64];
    int vmrss;
    for (int i = 0; i < VMRSS_LINE - 1; ++i)
        if (nullptr == fgets(line_buff, sizeof(line_buff), fd))
            continue;

    if (nullptr == fgets(line_buff, sizeof(line_buff), fd)) return 0;
    sscanf(line_buff, "%s %d", name, &vmrss);
    fclose(fd);

    return vmrss;
}

/**************************************************************************************
功能描述: 获取进程占用的虚拟内存
修改记录:
**************************************************************************************/
unsigned int PROC_MONITOR_C::GetProcVirtualMem(const uint32_t pid)
{
    char file_name[64] = {0};
    FILE *fd;
    char line_buff[512] = {0};
    sprintf(file_name, "/proc/%d/status", pid);

    fd = fopen(file_name, "r");
    if (nullptr == fd) return 0;

    char name[64];
    int vmsize;
    for (int i = 0; i < VMSIZE_LINE - 1; ++i)
        if (nullptr == fgets(line_buff, sizeof(line_buff), fd))
            continue;

    if (nullptr == fgets(line_buff, sizeof(line_buff), fd)) return 0;
    sscanf(line_buff, "%s %d", name, &vmsize);
    fclose(fd);

    return vmsize;
}

/**************************************************************************************
功能描述: 获取线程占用CPU的时间信息
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::GetThreadCpuOccupyInfo(const uint32_t tid, CPU_PROC_OCCUPY_STRU &stCpu)
{
    char fileName[64] = {0};
    sprintf(fileName, "/proc/%d/task/%d/stat", pid_, tid);
    //or sprintf(fileName, "/proc/%d/task/%d/stat", tid, tid);

    if (ParseProcStatFile(fileName, stCpu)) return -1;
    return 0;
}

/**************************************************************************************
功能描述: 获取线程的CPU时间
修改记录:
**************************************************************************************/
unsigned long PROC_MONITOR_C::GetThreadCpuOccupy(const uint32_t tid)
{
    CPU_PROC_OCCUPY_STRU stCpu;
    if (GetThreadCpuOccupyInfo(tid, stCpu))
        return 0;
    else
        return stCpu.Total();
}

/**************************************************************************************
功能描述: 获取线程的CPU占用率（Test）
修改记录:
**************************************************************************************/
float PROC_MONITOR_C::GetThreadCpuPercentage(const uint32_t tid)
{
    unsigned long cpuOccupy1, cpuOccupy2;
    unsigned long cpuProcOccupy1, cpuProcOccupy2;

    cpuOccupy1     = GetCpuOccupy();
    cpuProcOccupy1 = GetThreadCpuOccupy(tid);

    usleep(200000);

    cpuOccupy2     = GetCpuOccupy();
    cpuProcOccupy2 = GetThreadCpuOccupy(tid);

    float tcpu = 0.0;
    if (0 != cpuOccupy2 - cpuOccupy1)
        tcpu = 100.0 * (cpuProcOccupy2 - cpuProcOccupy1) / (cpuOccupy2 - cpuOccupy1) * cpuNum_;

    return tcpu;
}

/**************************************************************************************
功能描述: 根据线程号获取线程名
修改记录:
**************************************************************************************/
string PROC_MONITOR_C::GetThreadNameByTid(const uint32_t tid)
{
    CPU_PROC_OCCUPY_STRU stCpu;
    if (GetThreadCpuOccupyInfo(tid, stCpu)) return "";
    return stCpu.strName;
}

/**************************************************************************************
功能描述: 根据进程名，获取进程号
修改记录:
**************************************************************************************/
uint32_t PROC_MONITOR_C::GetPid_(const char* process_name, const char* user)
{
    if (user == nullptr) user = getlogin();

    char cmd[512];
    if (user) sprintf(cmd, "pgrep %s -u %s", process_name, user);

    FILE *pstr = popen(cmd, "r");
    if (pstr == nullptr) return 0;
    char buff[512];
    ::memset(buff, 0, sizeof(buff));
    if (nullptr == fgets(buff, 512, pstr)) return 0;

    return atoi(buff);
}

/**************************************************************************************
功能描述: 获取当前线程的线程号
修改记录:
**************************************************************************************/
uint32_t PROC_MONITOR_C::GetTid_()
{
    return syscall(SYS_gettid);
}

/**************************************************************************************
功能描述: 打印内存申请信息
修改记录:
**************************************************************************************/
void PROC_MONITOR_C::PrintMemoryInfo(void)
{
    ST_LOG_INFO("");
    ST_LOG_INFO("Begin...");

    malloc_stats();         /* 非标接口，将动态内存使用情况输出到stderr。 */
    fflush(stderr);         /* 尽快刷新防止与stdout内容乱序。 */

    ST_LOG_INFO("End.");
    ST_LOG_INFO("");
}

/**************************************************************************************
功能描述: 打印线程信息
修改记录:
**************************************************************************************/
void PROC_MONITOR_C::PrintThreadInfo(void)
{
    ST_LOG_INFO("");
    ST_LOG_INFO("Begin...");

    std::string strCmd = "ps -T -p " + ToString(GetPid());
    ST_LOG_INFO("%s.", strCmd.c_str());
    SYSTEM_EXEC(strCmd.c_str());

    ST_LOG_INFO("End.");
    ST_LOG_INFO("");
}

/**************************************************************************************
功能描述: 打印资源使用信息
修改记录:
**************************************************************************************/
void PROC_MONITOR_C::PrintResourceInfo(void)
{
    double time1 = Comm_GetTimeSec();

    PrintMemoryInfo();
    PrintThreadInfo();

    ST_LOG_INFO("Spent Time: %.2fms.", (Comm_GetTimeSec() - time1) * 1000.0);
}

/**************************************************************************************
功能描述: 获取第N项开始的指针
修改记录:
**************************************************************************************/
const char* PROC_MONITOR_C::GetItems(const char*buffer, int item) const
{
    const char *p = buffer;
    int len = strlen(buffer);
    int count = 0;

    for (int i = 0; i < len; ++i) {
        if (' ' == *p) {
            count++;
            if (count == item - 1) {
                p++;
                break;
            }
        }
        p++;
    }
    return p;
}

/**************************************************************************************
功能描述: 解析总占用CPU时间的stat文件
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::ParseCpuStatFile(CPU_TOTAL_OCCUPY_STRU& stCpu)
{
    FILE *fd = fopen("/proc/stat", "r");
    if (!fd) return -1;

    char buff[1024];
    const char *p = fgets(buff, sizeof(buff), fd);
    fclose(fd);
    if (!p) return -1;
    buff[sizeof(buff) - 1] = 0;

    char name[1024];
    if (EOF == sscanf(buff, "%s %lu %lu %lu %lu %lu %lu %lu", name, &stCpu.user, &stCpu.nice, &stCpu.system, &stCpu.idle, &stCpu.lowait, &stCpu.irq, &stCpu.softirq))
        return -1;

    return 0;
}

/**************************************************************************************
功能描述: 解析进程或线程占用CPU时间的stat文件
修改记录:
**************************************************************************************/
int PROC_MONITOR_C::ParseProcStatFile(const char* statFile, CPU_PROC_OCCUPY_STRU& stCpu)
{
    FILE *fd = fopen(statFile, "r");
    if (!fd) return -1;

    char buff[1024];
    const char *p = fgets(buff, sizeof(buff), fd);
    fclose(fd);
    if (!p) return -1;
    buff[sizeof(buff) - 1] = 0;

    if (EOF == sscanf(buff, "%u", &stCpu.tid)) return -1;

    if (stCpu.strName.empty()) {
        char name[1024];
        const char *k = GetItems(buff, THREAD_NAME_ITEM);
        if (EOF == sscanf(k, "%s", name)) return -1;
        string strThreadName = name;
        if (!strThreadName.empty()) {
            strThreadName = strThreadName.substr(1, strThreadName.size() - 2);
            stCpu.strName = strThreadName;
        }
    }

    const char *q = GetItems(buff, PID_ITEM);
    if (EOF == sscanf(q, "%u", &stCpu.pid)) return -1;

    q = GetItems(buff, OCCUPY_ITEM);
    if (EOF == sscanf(q, "%lu %lu %lu %lu", &stCpu.utime, &stCpu.stime, &stCpu.cutime, &stCpu.cstime)) return -1;

    return 0;
}

/**************************************************************************************
功能描述: 监控线程
修改记录:
**************************************************************************************/
void PROC_MONITOR_C::Monitor()
{
    float period = 1.0; //采样周期
    ros::Rate loop_rate(1 / period);

    while (isMonitorWork_) {
        /* 获取当前CPU占用信息 */
        CPU_TOTAL_OCCUPY_STRU stCpu;
        if (GetCpuOccupyInfo(stCpu)) {SLEEP_M(100); continue;}

        /* 获取当前时间段CPU总占用时间 */
        unsigned long cpuOccupyTime = stCpu.Total() - stCpuPre_.Total();
        if (!cpuOccupyTime) {SLEEP_M(100); continue;}

        /* 获取当前时间段CPU空闲时间 */
        unsigned long cpuIdleTime = stCpu.idle - stCpuPre_.idle;

        /* 计算当前时间段总的CPU占用率 */
        float cpuPercentage = (1 - 1.0 * cpuIdleTime / cpuOccupyTime) * 100.0;

        /* 获取进程的CPU占用率 */
        unsigned int procCpuTimer = GetProcCpuOccupy(pid_);
        float procCpuPercentage = 100.0 * (procCpuTimer - procCpuTimerPre_) / cpuOccupyTime * cpuNum_;

        /* 是否输出日志 */
        bool isPrint1 = fabsf(cpuPercentage - cpuPercentagePre_) > 5;           //总CPU占用率波动超过5%时才进行日志打印
        bool isPrint2 = fabsf(procCpuPercentage - procCpuPercentagePre_) > 10;  //进程的CPU占用率波动超过10%时才进行日志打印
        bool isPrint  = isPrint1 || isPrint2;

        /* 定时刷新线程ID */
        if (objTimer_.IsTimeOut()) {
            objTimer_.Reset();
            vector<std::string> vstrThreadId;
            objFileMng_.GetFilesOfDir(strTaskPath_, vstrThreadId);

            vThreadId_.clear();
            for (const string &str : vstrThreadId) {
                uint32_t tid = std::atoi(str.data());
                vThreadId_.push_back(tid);
            }
        }

        /* 获取进程中各线程的CPU占用率 */
        mapThreadCpuTimeCur_.clear();
        if (isPrint) {
            ST_LOG_INFO("*************** THREAD MONITORING RESULTS (%u CORES) ***************", cpuNum_);
            ST_LOG_INFO("The process <%s> currently has %u sub threads.", strProcName_.c_str(), uint32_t(vThreadId_.size()));       //进程的线程数
            ST_LOG_INFO("The CPU total utilization is:%.1f%%, and %.1f%% %.1fs ago.", cpuPercentage, cpuPercentagePre_, period);    //CPU总占用率
            ST_LOG_INFO("The CPU utilization of process <%d-%s> is:%.1f%%, and %.1f%% %.1fs ago.", pid_, strProcName_.c_str(), procCpuPercentage, procCpuPercentagePre_, period);   //进程的CPU占用率
        }

        for (auto &tid : vThreadId_) {
            CPU_PROC_OCCUPY_STRU stThreadCpu;
            if (GetThreadCpuOccupyInfo(tid, stThreadCpu)) continue;
            unsigned long threadCpuOccupy = stThreadCpu.Total();
            mapThreadCpuTimeCur_.emplace(tid, threadCpuOccupy);
            if (isPrint) {
                auto iterPre = mapThreadCpuTimePre_.find(tid);
                if (iterPre != mapThreadCpuTimePre_.end()) {
                    float pcpu = 100.0 * (threadCpuOccupy - iterPre->second) / cpuOccupyTime * cpuNum_;
                    if (pcpu > 3)   //进程的单CPU线程占用率超过3%时才进行日志打印
                        ST_LOG_INFO("The CPU utilization of thread <%d-%s> is:%.1f%%", tid, stThreadCpu.strName.c_str(), pcpu);     //线程的CPU占用率
                }
            }
        }

        if (isPrint) {printf("\n");}

        /* 保存检测结果 */
        stCpuPre_             = stCpu;
        cpuPercentagePre_     = cpuPercentage;
        procCpuTimerPre_      = procCpuTimer;
        procCpuPercentagePre_ = procCpuPercentage;
        swap(mapThreadCpuTimeCur_, mapThreadCpuTimePre_);

        if (objPrintResourceTimer.IsTimeOut()) {
            objPrintResourceTimer.Reset();

            PrintResourceInfo();
        }

        loop_rate.sleep();
    }
}

