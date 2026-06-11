#include "nav_comm/nav_simulation_hmi_ws.h"

int LOC_WS_CLIENT_C::Init(const std::function<void(const std::string &)> &objRecvMsgFunc)
{
    objRecvMsgFunc_ = objRecvMsgFunc;

    /* 尝试重连时长 */ /* 不对，应该不是这个含义 */
    objWsClient_.config.timeout_request = 1.0;

    objWsClient_.on_open = std::bind(&LOC_WS_CLIENT_C::RecvOpen, this, std::placeholders::_1);
    objWsClient_.on_close = std::bind(&LOC_WS_CLIENT_C::RecvClose, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    objWsClient_.on_error = std::bind(&LOC_WS_CLIENT_C::RecvError, this, std::placeholders::_1, std::placeholders::_2);
    objWsClient_.on_message = std::bind(&LOC_WS_CLIENT_C::RecvMessage, this, std::placeholders::_1, std::placeholders::_2);

    int rslt = CreateThread();
    if (rslt != 0)
    {
        ST_LOG_ERR("CreateThread() Fail.");
        return -1;
    }

    ST_LOG_INFO("Default Not Start Work.");
    ST_LOG_INFO("Succ.");
    return 0;
}

int LOC_WS_CLIENT_C::Send(const std::string &strMessage)
{
    std::unique_lock<std::mutex> objLocker(objConnectionMutex_);

    if (pobjConnection_ == NULL)
    {
        ST_LOG_INFO("Connect Invalid.");
        return -1;
    }

    pobjConnection_->send(strMessage);
    return 0;
}

void LOC_WS_CLIENT_C::SaveConnection(std::shared_ptr<WsClient::Connection> &pobjConnection)
{
    std::unique_lock<std::mutex> objLocker(objConnectionMutex_);
    pobjConnection_ = pobjConnection;
}

void LOC_WS_CLIENT_C::ClrConnection(void)
{
    std::unique_lock<std::mutex> objLocker(objConnectionMutex_);
    pobjConnection_ = NULL;
}

void LOC_WS_CLIENT_C::PushMessage(const std::string &strMessage)
{
    std::unique_lock<std::mutex> objLocker(objMessageMutex_);

    if (listMessage_.size() > maxMsgListSize_)
    {
        ST_LOG_ERR("Msg List Is Full. Msg Is Dropped.");
        return;
    }

    listMessage_.push_back(strMessage);
}

bool LOC_WS_CLIENT_C::PopMessage(std::string &strMessage)
{
    std::unique_lock<std::mutex> objLocker(objMessageMutex_);

    if (0 == listMessage_.size())
    {
        return false;
    }

    strMessage.swap(listMessage_.front());
    listMessage_.pop_front();

    return true;
}

void LOC_WS_CLIENT_C::ClrMessage(void)
{
    std::unique_lock<std::mutex> objLocker(objMessageMutex_);

    listMessage_.clear();
    ST_LOG_INFO("Succ.");
}

void LOC_WS_CLIENT_C::TryProcMessage(void)
{
    std::string strMessage;
    if (!PopMessage(strMessage))
    {
        return;
    }

    //ST_LOG_INFO("Msg: %s.", strMessage.c_str());
    R_ASSERT(objRecvMsgFunc_ != NULL, NOTHING);
    objRecvMsgFunc_(strMessage);
}

void LOC_WS_CLIENT_C::RecvOpen(std::shared_ptr<WsClient::Connection> pobjConnection)
{
    SaveConnection(pobjConnection);

    ST_LOG_INFO("Connect Open.");
}

void LOC_WS_CLIENT_C::RecvClose(std::shared_ptr<WsClient::Connection> pobjConnection, int status, const std::string &strReason)
{
    ClrConnection();

    ST_LOG_INFO("Connect Close. Reason=%s.", strReason.c_str());
}

void LOC_WS_CLIENT_C::RecvError(std::shared_ptr<WsClient::Connection> pobjConnection, const SimpleWeb::error_code &objErr)
{
    ClrConnection();

    ST_LOG_ERR("Connect Err : %s.", objErr.message().c_str());
}

void LOC_WS_CLIENT_C::RecvMessage(shared_ptr<WsClient::Connection> pobjConnection, shared_ptr<WsClient::InMessage> pobjMsg)
{
    R_ASSERT(objRecvMsgFunc_ != NULL, NOTHING);

    PushMessage(pobjMsg->string());
}

void LOC_WS_CLIENT_C::StartThread(void)
{
    ST_LOG_INFO("Begin.");

    objWsClient_.start();

    isStarting_ = false;

    ST_LOG_INFO("End.");
}

void LOC_WS_CLIENT_C::Start(void)
{
    ST_LOG_INFO("Begin.");

    pobjStartThread_ = new thread(&LOC_WS_CLIENT_C::StartThread, this);
    if (pobjStartThread_ == NULL)
    {
        ST_LOG_ERR("New StartThread() Fail.");
        return;
    }

    isStarting_ = true;
    ST_LOG_INFO("Succ.");
}

void LOC_WS_CLIENT_C::Stop(void)
{
    ST_LOG_INFO("Begin.");

    objWsClient_.stop();

    if (pobjStartThread_ != NULL)
    {
        pobjStartThread_->join();

        delete pobjStartThread_;
        pobjStartThread_ = NULL;
    }

    isStarting_ = false;

    ClrConnection();
    ClrMessage();

    ST_LOG_INFO("Succ.");
}

void LOC_WS_CLIENT_C::TryConnect(void)
{
    if (isStarting_ && (pobjStartThread_ == NULL))
    {
        ST_LOG_FATAL("Starting, But No Thread.");
        isStarting_ = false;
    }

    if (!isStarting_)
    {
        /* 防止出现start后异常退出了无法再起 */
        if (pobjStartThread_ != NULL)
        {
            ST_LOG_INFO("Start Thread Abnormal, To Close.");
            Stop();
            objReStartTimer_.Reset();
        }

        /* 防止出现异常时高频重连，上次关了后间隔一段时间才启动 */
        if (objReStartTimer_.IsTimeOut())
        {
            ST_LOG_INFO("Start Thread Not Run, To Start.");
            Start();
        }
    }
}

void LOC_WS_CLIENT_C::ProcPeriodic(void)
{
    TryConnect();
    TryProcMessage();
}

void LOC_WS_CLIENT_C::Thread(void)
{
    ST_LOG_INFO("Begin.");

    CYCLE_LEN_CTRLER_C objCycleLenCtrler("WsClientThread", 0.001, 0.001);
    while (isThreadWork_)
    {
        if (objCycleLenCtrler.IsWorkPeriodTimeout())
        {
            objCycleLenCtrler.Start();

            ProcPeriodic();

            objCycleLenCtrler.Stop(true);
        }
        else
        {
            objCycleLenCtrler.Wait();
        }
    }

    if (pobjStartThread_ != NULL)
    {
        Stop();
    }

    ST_LOG_INFO("End.");
}

int LOC_WS_CLIENT_C::CreateThread(void)
{
    R_ASSERT(pobjThread_ == NULL, -1);

    isThreadWork_ = true;

    pobjThread_ = new thread(&LOC_WS_CLIENT_C::Thread, this);
    if (pobjThread_ == NULL)
    {
        ST_LOG_ERR("New Thread Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

void LOC_WS_CLIENT_C::DeleteThread(void)
{
    ST_LOG_INFO("Begin.");

    /* 停止循环，等待自然结束 */
    isThreadWork_ = false;

    if (pobjThread_ != NULL)
    {
        pobjThread_->join();

        delete pobjThread_;
        pobjThread_ = NULL;
    }

    ST_LOG_INFO("Succ.");
}
