#pragma once

#include <ros/ros.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "comm/comm_file.h"

#define VMRSS_LINE          17
#define VMSIZE_LINE         13
#define THREAD_NAME_ITEM    2
#define PID_ITEM            5
#define OCCUPY_ITEM         14

struct CPU_TOTAL_OCCUPY_STRU {
    unsigned long Total() {
        return user + nice + system + idle + lowait + irq + softirq;
    }

    unsigned long user{0};
    unsigned long nice{0};
    unsigned long system{0};
    unsigned long idle{0};
    unsigned long lowait{0};
    unsigned long irq{0};
    unsigned long softirq{0};
};

struct CPU_PROC_OCCUPY_STRU {
    unsigned long Total() {
        return utime + stime + cutime + cstime;
    }

    uint32_t      pid{0};       //process id
    uint32_t      tid{0};       //thread id
    string        strName{""};  //thread name
    unsigned long utime{0};     //user time
    unsigned long stime{0};     //kernel time
    unsigned long cutime{0};    //all user time
    unsigned long cstime{0};    //all dead time
};

class PROC_MONITOR_C
{
public:
    PROC_MONITOR_C(const string& strProcName="") : strProcName_(strProcName)
    {
        (void)Init();
    }

    ~PROC_MONITOR_C()
    {
        isMonitorWork_ = false;

        if (monitoeThread_.joinable()) {
            monitoeThread_.join();
        }
    }

    static void PrintResourceInfo(void);

private:
    int           Init();
    int           GetCpuNum();

    int           GetCpuOccupyInfo(CPU_TOTAL_OCCUPY_STRU &stCpu);
    unsigned long GetCpuOccupy();
    float         GetCpuOccupyPercentage();

    int           GetProcCpuOccupyInfo(const uint32_t pid, CPU_PROC_OCCUPY_STRU &stCpu);
    unsigned long GetProcCpuOccupy(const uint32_t pid);
    float         GetProcCpuPercentage(const uint32_t pid);
    string        GetProcNameByPid(const uint32_t pid);

    unsigned int  GetProcMem(const uint32_t pid);
    unsigned int  GetProcVirtualMem(const uint32_t pid);

    int           GetThreadCpuOccupyInfo(const uint32_t tid, CPU_PROC_OCCUPY_STRU &stCpu);
    unsigned long GetThreadCpuOccupy(const uint32_t tid);
    float         GetThreadCpuPercentage(const uint32_t tid);
    string        GetThreadNameByTid(const uint32_t pid);

    uint32_t      GetPid_(const char* process_name, const char* user = nullptr);
    uint32_t      GetTid_();

    static void PrintMemoryInfo(void);
    static void PrintThreadInfo(void);

private:
    TIMER_C         objTimer_{5.0};
    FILE_OPRT_C     objFileMng_;

    uint32_t        pid_{0};
    uint32_t        cpuNum_{0};

    std::string     strProcName_;
    std::string     strTaskPath_;

    std::atomic_bool        isMonitorWork_{false};
    std::thread             monitoeThread_;

    CPU_TOTAL_OCCUPY_STRU   stCpuPre_;
    float                   cpuPercentagePre_{0.0};
    unsigned int            procCpuTimerPre_{0};
    float                   procCpuPercentagePre_{0.0};
    vector<uint32_t>        vThreadId_;

    std::unordered_map<uint32_t, unsigned long> mapThreadCpuTimePre_; //<tid-cpuOccupy>
    std::unordered_map<uint32_t, unsigned long> mapThreadCpuTimeCur_; //<tid-cpuOccupy>

    TIMER_C         objPrintResourceTimer{60.0, false};


    const char* GetItems(const char*buffer, int item) const;
    int         ParseCpuStatFile(CPU_TOTAL_OCCUPY_STRU& stCpu);
    int         ParseProcStatFile(const char* statFile, CPU_PROC_OCCUPY_STRU& stCpu);
    void        Monitor();
};

