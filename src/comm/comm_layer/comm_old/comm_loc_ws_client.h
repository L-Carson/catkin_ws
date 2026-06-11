#ifndef __COMM_LOC_WS_CLIENT_H__
#define __COMM_LOC_WS_CLIENT_H__

#include "nav_comm/loc_websocket/client_ws.hpp"

#include "comm/comm_base.h"

#include <list>
#include <string>
#include <mutex>
#include <atomic>
#include <functional>
#include <memory>

using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

#pragma pack(push)
#pragma pack(4)

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
class LOC_WS_CLIENT_C
{
public:
    LOC_WS_CLIENT_C(void) : objWsClient_("127.0.0.1:2030")
    {
        objRecvMsgFunc_ = NULL;
        pobjThread_ = NULL;
        isThreadWork_ = false;
        pobjStartThread_ = NULL;
        objReStartTimer_.SetOverTime(1.0);
        isStarting_ = false;
        pobjConnection_ = NULL;
    }

    ~LOC_WS_CLIENT_C(void)
    {
        DeleteThread();
        ST_LOG_INFO("End.");
    }

    int Init(const std::function<void(const std::string &)> &objRecvMsgFunc);
    int Send(const std::string &strMessage);

private:
    const unsigned int maxMsgListSize_ = 1000;
    WsClient objWsClient_;
    std::function<void(const std::string &)> objRecvMsgFunc_;
    std::thread *pobjThread_;
    std::atomic_bool isThreadWork_;

    /* ws启停在同一个线程处理 */
    std::thread *pobjStartThread_;
    TIMER_C objReStartTimer_;
    std::atomic_bool isStarting_;

    std::mutex objConnectionMutex_;
    std::shared_ptr<WsClient::Connection> pobjConnection_;

    std::mutex objMessageMutex_;
    std::list<std::string> listMessage_;

    void SaveConnection(std::shared_ptr<WsClient::Connection> &pobjConnection);
    void ClrConnection(void);

    void PushMessage(const std::string &strMessage);
    bool PopMessage(std::string &strMessage);
    void ClrMessage(void);
    void TryProcMessage(void);

    void RecvOpen(std::shared_ptr<WsClient::Connection> pobjConnection);
    void RecvClose(std::shared_ptr<WsClient::Connection> pobjConnection, int status, const std::string &strReason);
    void RecvError(std::shared_ptr<WsClient::Connection> pobjConnection, const SimpleWeb::error_code &objErr);
    void RecvMessage(std::shared_ptr<WsClient::Connection> pobjConnection, std::shared_ptr<WsClient::InMessage> pobjMsg);

    void StartThread(void);
    void Start(void);
    void Stop(void);
    void TryConnect(void);

    void ProcPeriodic(void);

    /**************************************************************************************
    功能描述: 线程
    修改记录:
    **************************************************************************************/
    void Thread(void);
    int CreateThread(void);
    void DeleteThread(void);
};

#pragma pack(pop)

#endif
