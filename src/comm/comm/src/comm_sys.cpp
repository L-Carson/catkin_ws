
#include "comm/comm_sys.h"
#include "comm/comm_dbg_log.h"

#include <algorithm>

#include <execinfo.h>                   /* for backtrace() */
#include <signal.h>                     /* for signal */
#include <new>                          /* for std::set_new_handler */
#include <cxxabi.h>                     /* for abi::__cxa_demangle */

/**************************************************************************************
功能描述: 和__PRETTY_FUNCTION__配合获取函数名全称
修改记录:
**************************************************************************************/
void Comm_GetFuncName(const char *pcFuncDes, CHAR acFuncName[], UINT32 maxFuncNameLen)
{
    UINT32 index = 0;
    for (const char *pcSrc = pcFuncDes; *pcSrc != '\0'; ++pcSrc) {
        if (index >= (maxFuncNameLen - 1)) {
            break;
        }

        if        (*pcSrc == ' ') {
            index = 0;
            continue;
        } else if (*pcSrc == '(') {
            break;
        } else {
            acFuncName[index] = *pcSrc;
            ++index;
        }
    }

    acFuncName[index] = '\0';
}

/**************************************************************************************
功能描述: 信号处理函数
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void Comm_HandleSignal(int sigNo)
{
    static struct
    {
        INT32       sigNo;
        const CHAR *pcSigDes;
    } astSigInfo[] =
    {
        {SIGSEGV,   "SIGSEGV   11 Segmentation Violation"},
        {SIGFPE,    "SIGFPE    08 Floating-point Exception"},
        {SIGABRT,   "SIGABRT   06 Abort"},
        {SIGTERM,   "SIGTERM   15 Termination"},
        {SIGSTKFLT, "SIGSTKFLT 16 Stack Fault"},
    };

    CHAR        acDefaultDes[20];       /* 确保一定够长 */
    const CHAR *pcSigDes = nullptr;

    for (UINT32 loop = 0; loop < ARRAY_SIZE(astSigInfo); ++loop) {
        if (astSigInfo[loop].sigNo == sigNo) {
            pcSigDes = astSigInfo[loop].pcSigDes;
            break;
        }
    }

    if (pcSigDes == nullptr) {
        (void)sprintf(acDefaultDes, "Sig-%u", sigNo);
        pcSigDes = acDefaultDes;
    }

    BACKTRACE_PRINTER_C::Print(pcSigDes, true);

    usleep(50 * 1000);                  /* 为了让异步的信息打印完成，等一会。 */
    fflush(stdout);                     /* 快要退出了，打印完成后立即刷新，避免丢失打印信息。 */

    signal(sigNo, SIG_DFL);             /* 恢复默认处理，从新发送信号 */
    raise(sigNo);
}

/**************************************************************************************
功能描述: 信号处理函数
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void Comm_HandBadAlloc(void)
{
    BACKTRACE_PRINTER_C::Print("Occur Bad Alloc", true);

    usleep(50 * 1000);                  /* 为了让异步的信息打印完成，等一会。 */
    fflush(stdout);                     /* 快要退出了，打印完成后立即刷新，避免丢失打印信息。 */

    std::set_new_handler(NULL);         /* 恢复默认处理 */
    throw std::bad_alloc();
}

void Comm_InitSys(void)
{
    BACKTRACE_PRINTER_C::Init();

    signal(SIGSEGV,   Comm_HandleSignal);           /* 段错误 */
    signal(SIGFPE,    Comm_HandleSignal);           /* 浮点错误 */
    signal(SIGABRT,   Comm_HandleSignal);           /* 强制退出 */
    signal(SIGTERM,   Comm_HandleSignal);           /* 终止 */
    signal(SIGSTKFLT, Comm_HandleSignal);           /* 堆栈错误 */

    std::set_new_handler(Comm_HandBadAlloc);        /* bad_alloc */

    ST_LOG_INFO("Succ.");
}

void Comm_UnInitSys(void)
{
    signal(SIGSEGV,   SIG_DFL);
    signal(SIGFPE,    SIG_DFL);
    signal(SIGABRT,   SIG_DFL);
    signal(SIGTERM,   SIG_DFL);
    signal(SIGSTKFLT, SIG_DFL);

    std::set_new_handler(NULL);

    BACKTRACE_PRINTER_C::UnInit();

    ST_LOG_INFO("Succ.");
}

void SYS_INFO_RECORDER_C::PeriodUpt(void)
{
    RecordTemperature();
    RecordCpuAndMemRate();
}

float SYS_INFO_RECORDER_C::GetCpuTemperature(UINT32 cpuNo) const
{
    /* 另一个可能路径 /proc/acpi/thermal_zone/TZS0/temperature */
    string strCpuTempFilePath = string("/sys/class/thermal/thermal_zone") + ToString(cpuNo) + string("/temp");
    FILE *fp = fopen(strCpuTempFilePath.c_str(), "r");
    if (fp == NULL) {
        return -1.0;
    }

    UINT32 cpuTmperatureEx;
    INT32  readNum = fscanf(fp, "%u", &cpuTmperatureEx);
    fclose(fp);
    if (readNum != 1) {
        return -1.0;
    }

    return float(double(cpuTmperatureEx) / 1000.0);
}

float SYS_INFO_RECORDER_C::GetCpusTemperature(void) const
{
    float maxCpuTemperature = 0.0;

    for (UINT32 loop = 0; loop < 8; ++loop) {
        float cpuTemperature = GetCpuTemperature(loop);
        maxCpuTemperature = MAX(maxCpuTemperature, cpuTemperature);
    }

    return maxCpuTemperature;
}

void SYS_INFO_RECORDER_C::RecordTemperature(void)
{
    if (!objReadTemperatureTimer.IsTimeOut()) {
        return;
    }

    objReadTemperatureTimer.Reset();
    float temperatureNow = GetCpusTemperature();
    smoothTemperature_ = (smoothTemperature_ == 0.0) ? temperatureNow : (smoothTemperature_ * 0.9 + temperatureNow * 0.1);

    if (   objOutputTemperatureTimer_.IsTimeOut()
        || (smoothTemperature_ > (lastRecordTemperature_ + 0.5))
        || ((smoothTemperature_ + 1.0) < lastRecordTemperature_)) {
        objOutputTemperatureTimer_.Reset();

        lastRecordTemperature_ = smoothTemperature_;

        ST_LOG_INFO("Cpu Temperature: %.2f.", smoothTemperature_);
    }
}

void SYS_INFO_RECORDER_C::RecordCpuAndMemRate(void)
{
    if (!objOutputCpuAndMemRateTimer_.IsTimeOut()) {
        return;
    }

    objOutputCpuAndMemRateTimer_.Reset();

    double startTime = Comm_GetTimeSec();

    /* 太耗时不分开显示了 */
    ST_LOG_INFO("");
    ST_LOG_INFO("");
    //ST_LOG_INFO("CPU Highest 10 Process:");
    //SYSTEM_EXEC("ps aux|head -1;ps aux|sort -rn -k3|head -10");       // CPU占用最多的前10个进程
    //ST_LOG_INFO("");
    //ST_LOG_INFO("");
    //ST_LOG_INFO("Memery Highest 10 Process:");
    //SYSTEM_EXEC("ps aux|head -1;ps aux|sort -rn -k4|head -10");       // 内存消耗最多的前10个进程
    //ST_LOG_INFO("");
    //ST_LOG_INFO("");
    //ST_LOG_INFO("Virtual Memery Highest 10 Process:");
    //SYSTEM_EXEC("ps aux|head -1;ps aux|sort -rn -k5|head -10");       // 虚拟内存使用最多的前10个进程
    SYSTEM_EXEC("ps -aux --sort -%cpu,-%mem | head -10");
    ST_LOG_INFO("");
    ST_LOG_INFO("Cost Time: %.2lfms.", (Comm_GetTimeSec() - startTime) * 1000.0);
    ST_LOG_INFO("");

}

BACKTRACE_PRINTER_C::ASYNC_PRINTER_C BACKTRACE_PRINTER_C::objAsyncPrinter_;

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::Init(void)
{
    ST_LOG_INFO("Begin...");

    objAsyncPrinter_.Start();

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 去初始化
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::UnInit(void)
{
    ST_LOG_INFO("Begin...");

    objAsyncPrinter_.Stop();

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 打印调用栈
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::Print(const CHAR *pcCause, BOOL isEmergency)
{
    objAsyncPrinter_.Print(pcCause, isEmergency);
}

/**************************************************************************************
功能描述: 开始
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::Start(void)
{
    ST_LOG_INFO("Begin...");

    PrintProcessMaps();

    objPrintThread_ = std::thread(&BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::PrintTask, this);
    pthread_setname_np(objPrintThread_.native_handle(), "c_bt_printer");

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 停止
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::Stop(void)
{
    ST_LOG_INFO("Begin...");

    objPrintInfoPipe_.Stop();

    if (objPrintThread_.joinable()) {
        objPrintThread_.join();
    }

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 异步打印调用栈
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::Print(const CHAR *pcCause, BOOL isEmergency)
{
    if (pcCause == NULL)
    {
        return;
    }
    PRINT_INFO_STRU stThreadBtPrintInfo;

    GetLocBackTrace(pcCause, isEmergency, stThreadBtPrintInfo);

    /* 紧急情况下，也就是异常处理时，为防止无法完成异步打印，先在本处理中打印一遍简要信息，可能导致阻塞也无所谓。 */
    if (isEmergency) {
        SIMPLE_LOG(stThreadBtPrintInfo.threadId, "Print Emergency BackTrace.");
        PrintBriefInfo(stThreadBtPrintInfo);
    }

    /* 如果异步打印机制都关闭了，就在此处打印调用栈信息，此时已经发生严重异常了，可能导致阻塞也无所谓。 */
    if (!objPrintInfoPipe_.IsWorking()) {
        SIMPLE_LOG(stThreadBtPrintInfo.threadId, "Async Printer Is Stopped.");
        PrintInfo(stThreadBtPrintInfo.threadId, stThreadBtPrintInfo);
    }

    objPrintInfoPipe_.Push(stThreadBtPrintInfo);
}

/**************************************************************************************
功能描述: 打印进程内存空间映射信息
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::PrintProcessMaps(void)
{
    ST_LOG_INFO("Begin...");
    ST_LOG_INFO("");

    /* cat /proc/6084/maps */
    UINT32      locPid = GetPid();
    std::string strCmd = "cat /proc/" + ToString(locPid) + "/maps";

    SYSTEM_EXEC(strCmd.c_str());

    ST_LOG_INFO("");
    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 获取本线程调用栈
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::GetLocBackTrace(const CHAR *pcCause, BOOL isEmergency, PRINT_INFO_STRU &stPrintInfo)
{
    stPrintInfo.threadId = GetTid();

    (void)strncpy(stPrintInfo.acCause, pcCause, ARRAY_SIZE(stPrintInfo.acCause));
    stPrintInfo.acCause[ARRAY_SIZE(stPrintInfo.acCause) - 1] = '\0';

    INT32 backTraceSize = backtrace(stPrintInfo.apvBtAddr, ARRAY_SIZE(stPrintInfo.apvBtAddr));
    if ((backTraceSize <= 0) || (backTraceSize > INT32(ARRAY_SIZE(stPrintInfo.apvBtAddr)))) {
        stPrintInfo.btSize = 0;

        (void)strncpy(stPrintInfo.acFailDes, "backtrace() Fail", ARRAY_SIZE(stPrintInfo.acFailDes));
        stPrintInfo.acFailDes[ARRAY_SIZE(stPrintInfo.acFailDes) - 1] = '\0';
    } else {
        stPrintInfo.btSize = UINT32(backTraceSize);
    }
}

/**************************************************************************************
功能描述: 打印调用栈简要信息
          注意：本函数阻塞可能小较小
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::PrintBriefInfo(const PRINT_INFO_STRU &stPrintInfo)
{
    SIMPLE_LOG(stPrintInfo.threadId, "Begin...");

    SIMPLE_LOG(stPrintInfo.threadId, "Cause=%.*s.", INT32(ARRAY_SIZE(stPrintInfo.acCause)), stPrintInfo.acCause);
    SIMPLE_LOG(stPrintInfo.threadId, "BackTrace Size Is %u:", stPrintInfo.btSize);
    if (stPrintInfo.btSize == 0) {
        SIMPLE_LOG(stPrintInfo.threadId, "Fail: %.*s.", INT32(ARRAY_SIZE(stPrintInfo.acFailDes)), stPrintInfo.acFailDes);
        return;
    }

    for (UINT32 loop  = 0; loop < stPrintInfo.btSize; ++loop) {
        SIMPLE_LOG(stPrintInfo.threadId, "%02d %p", loop, stPrintInfo.apvBtAddr[loop]);
    }

    SIMPLE_LOG(stPrintInfo.threadId, "End.");
}

/**************************************************************************************
功能描述: 解析符号
          注意：本函数有资源竞争，有较大可能导致阻塞。
修改记录:
**************************************************************************************/
string BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::DemangleSymbol(UINT32 threadId, const CHAR *pcSymbol)
{
    if (pcSymbol == nullptr) {
        SIMPLE_LOG(threadId, "Symbol Is Null.");
        return std::string("SymbolNull");
    }

    /* 尝试按照C++规则解析，解析失败则返回原始名称 */
    CHAR  acMangledFuncName[128];
    if (1 == sscanf(pcSymbol, "%*[^(]%*[^_]%127[^)+]", acMangledFuncName)) {
        size_t demangledFuncNameLen;
        int    demangStatus;
        CHAR  *pcDemangledFuncName = abi::__cxa_demangle(acMangledFuncName, NULL, &demangledFuncNameLen, &demangStatus);
        if (pcDemangledFuncName != NULL) {
            std::string strFuncName(pcDemangledFuncName);
            free(pcDemangledFuncName);
            return strFuncName;
        }
    }

    return std::string(pcSymbol);
}

/**************************************************************************************
功能描述: 打印函数调用栈
          注意：本函数有资源竞争，有较大可能导致阻塞。
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::PrintInfo(UINT32 threadId, const PRINT_INFO_STRU &stPrintInfo)
{
    SIMPLE_LOG(threadId, "Begin...");

    double beginTime = Comm_GetTimeSec();

    SIMPLE_LOG(threadId, "ThreadId=%u, Cause=%.*s.", stPrintInfo.threadId, INT32(ARRAY_SIZE(stPrintInfo.acCause)), stPrintInfo.acCause);
    SIMPLE_LOG(threadId, "BackTrace Size Is %u:", stPrintInfo.btSize);
    if (stPrintInfo.btSize == 0) {
        SIMPLE_LOG(threadId, "Fail: %.*s.", INT32(ARRAY_SIZE(stPrintInfo.acFailDes)), stPrintInfo.acFailDes);
        return;
    }

    CHAR **ppcSymbols = backtrace_symbols(stPrintInfo.apvBtAddr, stPrintInfo.btSize);
    if (ppcSymbols == NULL) {
        SIMPLE_LOG(threadId, "backtrace_symbols() Fail.");
        return;
    }

    for (UINT32 loop  = 0; loop < stPrintInfo.btSize; ++loop) {
        std::string strFuncName = DemangleSymbol(threadId, ppcSymbols[loop]);
        SIMPLE_LOG(threadId, "%02d %p %s", loop, stPrintInfo.apvBtAddr[loop], strFuncName.c_str());
    }

    free(ppcSymbols);
    SIMPLE_LOG(threadId, "End. Spent Time: %.3fs.", Comm_GetTimeSec() - beginTime);
}

/**************************************************************************************
功能描述: 打印任务
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::PrintTask(void)
{
    ST_LOG_INFO("Begin...");

    UINT32 threadId = GetTid();

    /* 为了减少死锁的可能性，将待打印的调用栈信息拷贝出来再处理。 */
    while (objPrintInfoPipe_.IsWorking()) {
        PRINT_INFO_STRU stBackTraceInfo;

        objPrintInfoPipe_.Pop(stBackTraceInfo);
        if (stBackTraceInfo.threadId == UINT32_NULL) {
            continue;
        }

        PrintInfo(threadId, stBackTraceInfo);
    }

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 压入打印信息
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::INFO_PIPE_C::Push(const PRINT_INFO_STRU &stPrintInfo)
{
    if (!isWorking_) {
        SIMPLE_LOG(stPrintInfo.threadId, "Not Working Now.");
        return;
    }

    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    /* 如果当前有调用栈没有被打印，覆盖前打印调用栈信息。 */
    if (stPrintInfo_.threadId != UINT32_NULL) {
        PrintOverwriteInfo(stPrintInfo_);
    }

    (void)memcpy(&stPrintInfo_, &stPrintInfo, sizeof(stPrintInfo_));

    objInfoCondVal_.notify_all();
}

/**************************************************************************************
功能描述: 弹出打印信息
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::INFO_PIPE_C::Pop(PRINT_INFO_STRU &stPrintInfo)
{
    if (!isWorking_) {
        SIMPLE_LOG(GetTid(), "Not Working Now.");
        return;
    }

    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    objInfoCondVal_.wait(objLocker);
    if (stPrintInfo_.threadId == UINT32_NULL) {
        stPrintInfo.threadId = UINT32_NULL;
        return;
    }

    (void)memcpy(&stPrintInfo, &stPrintInfo_, sizeof(stPrintInfo));

    stPrintInfo_.threadId = UINT32_NULL;
}

/**************************************************************************************
功能描述: 判断是否工作
修改记录:
**************************************************************************************/
BOOL BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::INFO_PIPE_C::IsWorking(void)
{
    return isWorking_;
}

/**************************************************************************************
功能描述: 停止管道
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::INFO_PIPE_C::Stop(void)
{
    ST_LOG_INFO("Begin.");

    isWorking_ = false;

    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    stPrintInfo_.threadId = UINT32_NULL;

    objInfoCondVal_.notify_all();

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 打印信息
          注意：该函数不能执行任何复杂操作，包括日志记录和内存申请等，防止死锁等。紧急情况除外。
修改记录:
**************************************************************************************/
void BACKTRACE_PRINTER_C::ASYNC_PRINTER_C::INFO_PIPE_C::PrintOverwriteInfo(const PRINT_INFO_STRU &stPrintInfo) const
{
    UINT32 locThreadId = GetTid();

    SIMPLE_LOG(locThreadId, "Overwrite Exist Thread(Id=%u) Cause(%.*s).", stPrintInfo.threadId, INT32(ARRAY_SIZE(stPrintInfo.acCause)), stPrintInfo.acCause);
    SIMPLE_LOG(locThreadId, "BackTrace Size Is %u:", stPrintInfo.btSize);
    if (stPrintInfo.btSize == 0) {
        SIMPLE_LOG(locThreadId, "Fail: %.*s.", INT32(ARRAY_SIZE(stPrintInfo.acFailDes)), stPrintInfo.acFailDes);
        return;
    }

    for (UINT32 loop  = 0; loop < stPrintInfo.btSize; ++loop) {
        SIMPLE_LOG(locThreadId, "%02d %p", loop, stPrintInfo.apvBtAddr[loop]);
    }
}

/**************************************************************************************
功能描述: 静态变量
修改记录:
**************************************************************************************/
THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C THREAD_BLOCKING_MONITOR_C::objThreadsMonitor_;

/**************************************************************************************
功能描述: 设置监控的线程信息
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::SetThreadInfo(const std::string &strThreadDescription, double blockingTimeThr)
{
    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    ST_LOG_INFO("Set Thread Info: Des = %s, Thr = %lf.", strThreadDescription.c_str(), blockingTimeThr);
    if (locThreadId_ != UINT32_NULL) {
        ST_LOG_FATAL("Thread(Id=%u, Des=%s) Is Monitored Now.", locThreadId_, strThreadDescription.c_str());
        return;
    }

    strDescription_  = strThreadDescription;
    blockingTimeThr_ = blockingTimeThr;
}

/**************************************************************************************
功能描述: 周期更新
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::PeriodUpt(void)
{
    if (!IsMonitoringThread()) {
        AddThisThreadToMonitor();
    }

    objThreadsMonitor_.UptThread(GetMonitoringThreadId());
}

/**************************************************************************************
功能描述: 当前是否在监控线程
修改记录:
**************************************************************************************/
BOOL THREAD_BLOCKING_MONITOR_C::IsMonitoringThread(void)
{
    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    return (locThreadId_ != UINT32_NULL);
}

/**************************************************************************************
功能描述: 获取监控线程ID
修改记录:
**************************************************************************************/
UINT32 THREAD_BLOCKING_MONITOR_C::GetMonitoringThreadId(void)
{
    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    return locThreadId_;
}

/**************************************************************************************
功能描述: 添加当前线程来监控
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::AddThisThreadToMonitor(void)
{
    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    locThreadId_ = GetTid();
    ST_LOG_INFO("Add ThreadId %u.", locThreadId_);

    objThreadsMonitor_.AddThread(locThreadId_, pthread_self(), strDescription_, blockingTimeThr_);
}

/**************************************************************************************
功能描述: 删除当前线程的监控
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::RmvThisThreadToMonitor(void)
{
    std::unique_lock<std::mutex> objLocker(objInfoMutex_);

    if (locThreadId_ == UINT32_NULL) {
        return;
    }

    ST_LOG_INFO("Rmv ThreadId %u.", locThreadId_);

    objThreadsMonitor_.RmvThread(locThreadId_);
}

std::atomic_bool THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::isPrintingBackTraceInfo_{false};

/**************************************************************************************
功能描述: 添加监控线程
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::AddThread(UINT32 threadId, pthread_t threadPtr, const std::string &strThreadDescription, double blockingTimeThr)
{
    UINT32 monitoredThreadNum = AddThreadInfoInList(threadId, threadPtr, strThreadDescription, blockingTimeThr);
    if (monitoredThreadNum == 0) {
        return;
    }

    StartMonitoring();
}

/**************************************************************************************
功能描述: 删除监控线程
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::RmvThread(UINT32 threadId)
{
    UINT32 monitoredThreadNum = RmvThreadInfoInList(threadId);
    if (monitoredThreadNum > 0) {
        return;
    }

    StopMonitoring();
}

/**************************************************************************************
功能描述: 周期更新，设置线程状态正常
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::UptThread(UINT32 threadId)
{
    UptThreadWorking(threadId);
}

/**************************************************************************************
功能描述: 处理信号，打印调用栈，不能有复杂处理，否则可能死锁
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::HandleSigOfBackTrace(INT32 sigOfBackTrace)
{
    isPrintingBackTraceInfo_ = true;

    BACKTRACE_PRINTER_C::Print("Thread Blocking", false);

    isPrintingBackTraceInfo_ = false;
}

/**************************************************************************************
功能描述: 添加监控的线程信息
修改记录:
**************************************************************************************/
UINT32 THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::AddThreadInfoInList(UINT32 threadId, pthread_t threadPtr, const std::string &strThreadDescription, double blockingTimeThr)
{
    std::unique_lock<std::mutex> objLocker(objThreadInfoMutex_);

    auto iter = std::find_if(listThreadInfo_.begin(), listThreadInfo_.end(), [&](const THREAD_INFO_STRU &stThreadInfo) { return (stThreadInfo.threadId == threadId); });
    if (iter != listThreadInfo_.end()) {
        ST_LOG_FATAL("Thread(Id=%u, Des=%s) Is Exist.", threadId, strThreadDescription.c_str());
        return UINT32(listThreadInfo_.size());
    }

    if (blockingTimeThr < minBlockingTimeLen_) {
        ST_LOG_ERR("Thread(Id=%u, Des=%s) Blocking Time(%.3fs) Is Too Short.", threadId, strThreadDescription.c_str(), blockingTimeThr);
    }

    THREAD_INFO_STRU stThreadInfo;
    stThreadInfo.threadId        = threadId;
    stThreadInfo.threadPtr       = threadPtr;
    stThreadInfo.strDescription  = strThreadDescription;
    stThreadInfo.blockingTimeThr = std::max(blockingTimeThr, minBlockingTimeLen_);
    stThreadInfo.lastNormalTime  = Comm_GetTimeSec();
    stThreadInfo.blockingTimeLen = stThreadInfo.blockingTimeThr;
    ST_LOG_INFO("ThreadId=%u, ThreadPtr=%llu, Des=%s, BlockTimeThr=%.3fs.", threadId, UINT64(threadPtr), strThreadDescription.c_str(), stThreadInfo.blockingTimeThr);

    listThreadInfo_.emplace_back(std::move(stThreadInfo));

    UINT32 monitoredThreadNum = UINT32(listThreadInfo_.size());
    ST_LOG_INFO("Monitored Thread Num Is %u Now.", monitoredThreadNum);
    return monitoredThreadNum;
}

/**************************************************************************************
功能描述: 删除监控的线程信息
修改记录:
**************************************************************************************/
UINT32 THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::RmvThreadInfoInList(UINT32 threadId)
{
    std::unique_lock<std::mutex> objLocker(objThreadInfoMutex_);

    auto iter = std::find_if(listThreadInfo_.begin(), listThreadInfo_.end(), [&](const THREAD_INFO_STRU &stThreadInfo) { return (stThreadInfo.threadId == threadId); });
    if (iter == listThreadInfo_.end()) {
        ST_LOG_FATAL("Thread(Id=%u) Is Not Exist.", threadId);
        return UINT32(listThreadInfo_.size());
    }

    const THREAD_INFO_STRU &stLocThreadInfo = *iter;
    ST_LOG_INFO("ThreadId=%u, Des=%s.", threadId, stLocThreadInfo.strDescription.c_str());

    listThreadInfo_.erase(iter);

    UINT32 monitoredThreadNum = UINT32(listThreadInfo_.size());
    ST_LOG_INFO("Monitored Thread Num Is %u Now.", monitoredThreadNum);
    return monitoredThreadNum;
}

/**************************************************************************************
功能描述: 更新线程工作状态
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::UptThreadWorking(UINT32 threadId)
{
    std::unique_lock<std::mutex> objLocker(objThreadInfoMutex_);

    auto iter = std::find_if(listThreadInfo_.begin(), listThreadInfo_.end(), [&](const THREAD_INFO_STRU &stThreadInfo) { return (stThreadInfo.threadId == threadId); });
    if (iter == listThreadInfo_.end()) {
        ST_LOG_FATAL("Thread(Id=%u) Is Not Exist.", threadId);
        return;
    }

    THREAD_INFO_STRU &stLocThreadInfo = *iter;

    double curTime = Comm_GetTimeSec();
    if (curTime > (stLocThreadInfo.lastNormalTime + stLocThreadInfo.blockingTimeThr)) {
        ST_LOG_INFO("Thread(Id=%u, Des=%s) Blocking Recover Now.", threadId, stLocThreadInfo.strDescription.c_str());
    }

    stLocThreadInfo.lastNormalTime  = curTime;
    stLocThreadInfo.blockingTimeLen = stLocThreadInfo.blockingTimeThr;
}

/**************************************************************************************
功能描述: 获取需要跟踪的线程列表
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::GetThreadsToBackTrace(std::vector<pthread_t> &vectThreadPtr)
{
    std::unique_lock<std::mutex> objLocker(objThreadInfoMutex_);

    vectThreadPtr.clear();
    const double curTime = Comm_GetTimeSec();

    std::for_each(listThreadInfo_.begin(),
                  listThreadInfo_.end(),
                  [&](THREAD_INFO_STRU &stThreadInfo)
                  {
                        if (curTime > (stThreadInfo.lastNormalTime + stThreadInfo.blockingTimeLen)) {
                            SIMPLE_LOG(GetTid(), "Thread(Id=%u, Des=%s) Blocking Continue %.3lfs. Is Printing BackTrace: %u.", stThreadInfo.threadId, stThreadInfo.strDescription.c_str(), stThreadInfo.blockingTimeLen, UINT32(isPrintingBackTraceInfo_));

                            /* 阻塞超过5分钟才触发检控，频繁监控更容易触发自锁。 */
                            if (stThreadInfo.blockingTimeLen > minBlockTimeLenOfBt_) {
                                SIMPLE_LOG(GetTid(), "Thread(Id=%u, Des=%s) Output BackTraceInfo.", stThreadInfo.threadId, stThreadInfo.strDescription.c_str());
                                vectThreadPtr.emplace_back(stThreadInfo.threadPtr);
                            }

                            stThreadInfo.blockingTimeLen += std::min(stThreadInfo.blockingTimeLen * 0.5, 5.0);
                        }
                  });
}

/**************************************************************************************
功能描述: 监控一次
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::MonitorOnce(void)
{
    std::vector<pthread_t> vectThreadPtr;
    GetThreadsToBackTrace(vectThreadPtr);

    // 发送信号给被监控的线程，触发其打印调用栈
    std::for_each(vectThreadPtr.begin(),
                  vectThreadPtr.end(),
                  [&](pthread_t threadPtr)
                  {
                        INT32 rslt = pthread_kill(threadPtr, sigForBackTrace_);
                        if (rslt != 0) {
                            SIMPLE_LOG(GetTid(), "pthread_kill(ThreadPtr=%llu) Fail.", UINT64(threadPtr));
                        }
                  });
}

/**************************************************************************************
功能描述: 监控线程任务
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::MonitorTask(void)
{
    ST_LOG_INFO("Begin...");

    usleep(1000 * 1000);

    OPERATION_CYCLE_CTRL_C objCircleCtrl("Thread_Block_Monitor", blockingMonitorCircle_);

    while (isMonitorThreadWorking_) {
        if (objCircleCtrl.IsWorkPeriodTimeout()) {
            objCircleCtrl.Start();

            MonitorOnce();

            objCircleCtrl.Stop(true);
        } else {
            objCircleCtrl.Wait();
        }
    }

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 开始监控
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::StartMonitoring(void)
{
    ST_LOG_INFO("Begin...");

    std::unique_lock<std::mutex> objLocker(objMonitorThreadMutex_);

    if (isMonitorThreadWorking_) {
        return;
    }

    signal(sigForBackTrace_, THREADS_MONITOR_C::HandleSigOfBackTrace);

    isMonitorThreadWorking_ = true;

    objMonitorThread_ = std::thread(&THREADS_MONITOR_C::MonitorTask, this);
    pthread_setname_np(objMonitorThread_.native_handle(), "c_block_monitor");

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 停止监控
修改记录:
**************************************************************************************/
void THREAD_BLOCKING_MONITOR_C::THREADS_MONITOR_C::StopMonitoring(void)
{
    ST_LOG_INFO("Begin...");

    std::unique_lock<std::mutex> objLocker(objMonitorThreadMutex_);

    if (!isMonitorThreadWorking_) {
        return;
    }

    signal(sigForBackTrace_, SIG_DFL);      /* 恢复默认 */

    isMonitorThreadWorking_ = false;

    if (objMonitorThread_.joinable()) {
        objMonitorThread_.join();
    }

    ST_LOG_INFO("End.");
}


