#ifndef __COMM_PING_H__
#define __COMM_PING_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <atomic>

using namespace std;

class PING_IP_C
{
public:
    PING_IP_C(const string strIp, int deadLine)
    {
        strIp_    = strIp;
        deadLine_ = deadLine > 1 ? deadLine : 1;
        thread_   = std::thread(&PING_IP_C::Ping, this);
        pthread_setname_np(thread_.native_handle(), "ping_ip");
    }

    ~PING_IP_C()
    {
        isOk_ = false;
        if (thread_.joinable()) thread_.join();
    }

private:
    string              strIp_;
    int                 deadLine_;
    std::thread         thread_;
    std::atomic_bool    isOk_{true};

    string GetCmdResult(const string &strCmd)
    {
        char buf[10240] = {0};
        FILE *pf = NULL;
        if ((pf = popen(strCmd.c_str(), "r")) == NULL) return "";

        string strResult;
        while(fgets(buf, sizeof buf, pf)) strResult += buf;

        pclose(pf);
        unsigned int iSize = strResult.size();

        if (iSize > 0 && strResult[iSize - 1] == '\n') strResult = strResult.substr(0, iSize - 1);

        return strResult;
    }

    void Ping()
    {
        while (isOk_) {
            string strCmd = "ping " + strIp_ + " -w " + to_string(deadLine_);
            string stRcv = GetCmdResult(strCmd);
            cout << stRcv << endl;

            if ((stRcv.find("received") != string::npos && stRcv.find(", 0 received") == string::npos) ||
                (stRcv.find("已接收") != string::npos && stRcv.find("， 已接收 0 个包") == string::npos)) {
                ST_LOG_INFO("IPOK:%s.\n", strIp_.c_str());
            } else {
                ST_LOG_ERR("Unable to ping IP:%s.\n", strIp_.c_str());
            }
        }
    }
};

class IP_MONITOR_C
{
public:
    IP_MONITOR_C(const string strIp, int deadLine = 1) : strIp_(strIp), deadLine_(deadLine) {}
    ~IP_MONITOR_C(){}

    void Start() {
        if (!pobjPing_) pobjPing_ = new PING_IP_C(strIp_, deadLine_);
    }

    void Stop() {
        if (pobjPing_) {
            delete pobjPing_;
            pobjPing_ = nullptr;
        }
    }

private:
    PING_IP_C*  pobjPing_{nullptr};
    string      strIp_;
    int         deadLine_;
};

#endif
