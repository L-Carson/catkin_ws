#ifndef __COMM_SYS_H__
#define __COMM_SYS_H__
#include "comm/comm_data_type.h"
#include "comm/comm_time.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <string>
#include <vector>
#include <list>


#include <signal.h>         /* for signal */
#include <unistd.h>         /* getpid */
#include <sys/syscall.h>    /* ubuntu中syscall定义是放在头文件unistd.h中的。 */
#include <exception>        /* 异常处理 */

/**************************************************************************************
功能描述: 函数名
          VS下不支持该宏
修改记录:
**************************************************************************************/
#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

/**************************************************************************************
功能描述: 设置动态库函数接口非隐藏属性，编译时默认所有隐藏
修改记录:
**************************************************************************************/
#define DLL_PUBLIC __attribute__((visibility("default")))

/**************************************************************************************
功能描述: 忽略system返回值，其返回值比较难判断处理
修改记录:
**************************************************************************************/
#define SYSTEM_EXEC(cmd_)     if (-1 == system(cmd_)) {}

/**************************************************************************************
功能描述: 获取线程ID，该线程ID全系统唯一
修改记录:
**************************************************************************************/
#define GetTid()                                (UINT32(syscall(__NR_gettid)))

/**************************************************************************************
功能描述: 获取进程ID，该线程ID全系统唯一，也就是主线程的线程ID
修改记录:
**************************************************************************************/
#define GetPid()                                (UINT32(getpid()))

/**************************************************************************************
功能描述: 简要日志接口
          注意：日志接口要尽可能简单，避免容易出问题，特别是资源竞争卡死
修改记录:
**************************************************************************************/
#define SIMPLE_LOG(_tid, _strFmt, ...)  printf("[%8u] " _strFmt "\n", _tid, ##__VA_ARGS__)

/**************************************************************************************
功能描述: 和__PRETTY_FUNCTION__配合获取函数名全称
修改记录:
**************************************************************************************/
void Comm_GetFuncName(const char *pcFuncDes, CHAR acFuncName[], UINT32 maxFuncNameLen);

void Comm_InitSys(void);
void Comm_UnInitSys(void);

/**************************************************************************************
功能描述: 系统信息记录
修改记录:
**************************************************************************************/
class SYS_INFO_RECORDER_C
{
public:
    SYS_INFO_RECORDER_C(void)
    {
    }

    void PeriodUpt(void);

private:
    TIMER_C objReadTemperatureTimer{1.0};
    TIMER_C objOutputTemperatureTimer_{60.0};
    TIMER_C objOutputCpuAndMemRateTimer_{60.0};
    float   smoothTemperature_{0.0};
    float   lastRecordTemperature_{0.0};

    float GetCpuTemperature(UINT32 cpuNo) const;
    float GetCpusTemperature(void) const;
    void  RecordTemperature(void);
    void  RecordCpuAndMemRate(void);
};


/**************************************************************************************
功能描述: 调用栈打印
修改记录:
**************************************************************************************/
class BACKTRACE_PRINTER_C
{
public:
    static void Init(void);
    static void UnInit(void);

    static void Print(const CHAR *pcCause, BOOL isEmergency);

private:
    /**************************************************************************************
    功能描述: 打印线程调用栈
              该过程为异步打印，即将调用栈传递给了另一个线程打印，防止终端处理中复杂处理死锁。
    修改记录:
    **************************************************************************************/
    class ASYNC_PRINTER_C
    {
    public:
        ~ASYNC_PRINTER_C(void)
        {
            Stop();
        }

        void Start(void);
        void Stop(void);

        void Print(const CHAR *pcCause, BOOL isEmergency);

    private:
        struct PRINT_INFO_STRU
        {
            UINT32  threadId;           /* 线程号 *//* 不是无效值时其它字段有效 */
            CHAR    acCause[50];        /* 触发原因 */
            UINT32  btSize;             /* 调用栈深度，可能为0 */
            void   *apvBtAddr[50];      /* 调用栈地址 *//*      btSize不为0有效     */
            CHAR    acFailDes[50];      /* 失败信息 *//*      btSize为0有效     */
        };

        /**************************************************************************************
        功能描述: 打印信息管道
        修改记录:
        **************************************************************************************/
        class INFO_PIPE_C
        {
        public:
            INFO_PIPE_C(void)
            {
                stPrintInfo_.threadId = UINT32_NULL;
            }

            ~INFO_PIPE_C(void)
            {
                Stop();
            }

            void Push(const PRINT_INFO_STRU &stPrintInfo);
            void Pop(PRINT_INFO_STRU &stPrintInfo);

            BOOL IsWorking(void);
            void Stop(void);

        private:
            std::atomic_bool        isWorking_{true};
            std::mutex              objInfoMutex_;
            std::condition_variable objInfoCondVal_;
            PRINT_INFO_STRU         stPrintInfo_;

            void PrintOverwriteInfo(const PRINT_INFO_STRU &stPrintInfo) const;
        };

        std::thread objPrintThread_;
        INFO_PIPE_C objPrintInfoPipe_;

        /**************************************************************************************
        功能描述: 初始化
        修改记录:
        **************************************************************************************/
        void        PrintProcessMaps(void);

        /**************************************************************************************
        功能描述: 获取调用栈信息
        修改记录:
        **************************************************************************************/
        void        GetLocBackTrace(const CHAR *pcCause, BOOL isEmergency, PRINT_INFO_STRU &stPrintInfo);

        /**************************************************************************************
        功能描述: 打印调用栈
        修改记录:
        **************************************************************************************/
        void        PrintBriefInfo(const PRINT_INFO_STRU &stPrintInfo);
        std::string DemangleSymbol(UINT32 threadId, const CHAR *pcSymbol);
        void        PrintInfo(UINT32 threadId, const PRINT_INFO_STRU &stPrintInfo);

        /**************************************************************************************
        功能描述: 打印调用栈线程
        修改记录:
        **************************************************************************************/
        void        PrintTask(void);
    };

    static ASYNC_PRINTER_C objAsyncPrinter_;
};

/**************************************************************************************
功能描述: 线程阻塞监控
          在被监控线程中实例化，且按时调用 SetNormal(), 不调用认为阻塞了。
          超过一定时间阻塞不循环，则出发记录被监控线程当时的调用栈。
          阻塞判定时长不能过短，否则很容易误触发，频繁误差发影响程序正常执行效率。
          误触发判定时间精度为 blockingMonitorCircle_，当前为 10ms。
          该类不依赖其它模块，任何线程都可以使用本类进行监控，包括COMM中部分线程。
修改记录:
**************************************************************************************/
class THREAD_BLOCKING_MONITOR_C
{
public:
    THREAD_BLOCKING_MONITOR_C(void)
    {
    }

    THREAD_BLOCKING_MONITOR_C(const std::string &strThreadDescription, double blockingTimeThr, BOOL isMonitorThisThread = true)
    {
        SetThreadInfo(strThreadDescription, blockingTimeThr);

        if (isMonitorThisThread) {
            AddThisThreadToMonitor();
        }
    }

    ~THREAD_BLOCKING_MONITOR_C(void)
    {
        RmvThisThreadToMonitor();
    }

    void SetThreadInfo(const std::string &strThreadDescription, double blockingTimeThr);

    void PeriodUpt(void);

private:
    class THREADS_MONITOR_C
    {
    public:
        ~THREADS_MONITOR_C(void)
        {
            StopMonitoring();
        }

        void AddThread(UINT32 threadId, pthread_t threadPtr, const std::string &strThreadDescription, double blockingTimeThr);
        void RmvThread(UINT32 threadId);
        void UptThread(UINT32 threadId);

    private:
        struct THREAD_INFO_STRU
        {
            UINT32      threadId;          /* 线程ID，作为线程信息的唯一标识 */
            pthread_t   threadPtr;         /* 线程信息指针，pthread库中线程标识 */
            std::string strDescription;    /* 线程描述 */
            double      blockingTimeThr;   /* 线程阻塞门限 */
            double      lastNormalTime;    /* 上一次正常时间点 */
            double      blockingTimeLen;   /* 线程阻塞时长 */
        };

        const double                minBlockTimeLenOfBt_   = 60.0 * 5.0;
        const double                blockingMonitorCircle_ = 0.01;
        const double                minBlockingTimeLen_    = 0.05;
        const INT32                 sigForBackTrace_       = SIGUSR1;

        std::atomic_bool            isMonitorThreadWorking_{false};

        std::mutex                  objMonitorThreadMutex_;
        std::thread                 objMonitorThread_;

        std::mutex                  objThreadInfoMutex_;
        std::list<THREAD_INFO_STRU> listThreadInfo_;              /* 非性能关键，使用list，便于内存能及时释放。 */
        static std::atomic_bool     isPrintingBackTraceInfo_;

        static void  HandleSigOfBackTrace(INT32 sigOfBackTrace);

        /**************************************************************************************
        功能描述: 数据管理
        修改记录:
        **************************************************************************************/
        UINT32 AddThreadInfoInList(UINT32 threadId, pthread_t threadPtr, const std::string &strThreadDescription, double blockingTimeThr);
        UINT32 RmvThreadInfoInList(UINT32 threadId);
        void   UptThreadWorking(UINT32 threadId);
        void   GetThreadsToBackTrace(std::vector<pthread_t> &vectThreadPtr);

        /**************************************************************************************
        功能描述: 监控
        修改记录:
        **************************************************************************************/
        void  MonitorOnce(void);
        void  MonitorTask(void);

        /**************************************************************************************
        功能描述: 监控控制
        修改记录:
        **************************************************************************************/
        void  StartMonitoring(void);
        void  StopMonitoring(void);
    };

    std::mutex               objInfoMutex_;
    std::string              strDescription_{"Null"};
    double                   blockingTimeThr_{1.0};
    UINT32                   locThreadId_{UINT32_NULL};

    static THREADS_MONITOR_C objThreadsMonitor_;

    BOOL   IsMonitoringThread(void);
    UINT32 GetMonitoringThreadId(void);
    void   AddThisThreadToMonitor(void);
    void   RmvThisThreadToMonitor(void);
};



#endif

