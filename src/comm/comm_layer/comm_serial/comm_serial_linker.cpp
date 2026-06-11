
#include "comm/comm_serial_linker.h"

/**************************************************************************************
功能描述: 串口管理 初始化
修改记录:
**************************************************************************************/
int SERIAL_LINKER_C::Init(const std::string &strDevName, const SERIAL_LINKER_RECV_MSG_FUNC_C &objRecvMsgFunc)
{
    ST_LOG_INFO("LocAddr=%u, PeerAddr=%u, DevName=%s.", enLocAddr_, enPeerAddr_, strDevName.c_str());

    int rslt = objSerial_.Open(strDevName.c_str(), SERIAL_LINKER_BANDRATE);
    if (rslt != 0) {
        ST_LOG_ERR("objSerial_.Open(%s) Fail.", strDevName.c_str());
        return -1;
    }

    rslt = CreateThread();
    if (rslt != 0) {
        ST_LOG_ERR("CreateThread() Fail.");
        return -1;
    }

    objProcMsgFunc_ = objRecvMsgFunc;

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 串口管理 发送消息
修改记录:
**************************************************************************************/
int SERIAL_LINKER_C::Send(SERIAL_ADDR_ENUM enTgtAddr, unsigned char msgType, const unsigned char *pucMsg, unsigned int msgLen)
{
    return objSender_.Send(enTgtAddr, msgType, pucMsg, unsigned short(msgLen));
}

int SERIAL_LINKER_C::SendPassMsg(const unsigned char *pucData, unsigned int dataLen)
{
    return objSender_.SendPass(pucData, dataLen);
}

/**************************************************************************************
功能描述: 串口管理 判断连接是否正常
修改记录:
**************************************************************************************/
bool SERIAL_LINKER_C::IsFault(void)
{
    return objSender_.IsConnectFault();
}

/**************************************************************************************
功能描述: 更新最近时间
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::StatisReadCycleLen(void)
{
    double curTime  = Comm_GetTimeSec();

    float curCycleLen = (stDbgInfo_.lastReadCycleTime > 0) ? float(curTime - stDbgInfo_.lastReadCycleTime) : 0.0;

    stDbgInfo_.maxReadCycleLen = std::max(stDbgInfo_.maxReadCycleLen, curCycleLen);

    stDbgInfo_.lastReadCycleTime = curTime;
}

/**************************************************************************************
功能描述: 输出统计信息
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::OutputDbgInfo(double curTime)
{
    if (lastOutputDbgInfoTime_ == 0.0) {
        lastOutputDbgInfoTime_ = curTime;
        return;
    }

    if ((curTime - lastOutputDbgInfoTime_) < (1.0 * 60.0)) {
        return;
    }

    lastOutputDbgInfoTime_ = curTime;

    stDbgInfo_.Output();
    objSender_.OutputDbgInfo();
}

/**************************************************************************************
功能描述: 判断本次收到的帧编号是否和上一次一样，通过此判断是否同一条消息
修改记录:
**************************************************************************************/
bool SERIAL_LINKER_C::IsFrameRepeat(const SERIAL_FRAME_C &objFrame)
{
    unsigned char frameNo = objFrame.GetFrameNo();
    if (frameNo != lastRecvFrameNo_) {
        lastRecvFrameNo_ = frameNo;
        return false;
    } else {
        return true;
    }
}

void SERIAL_LINKER_C::ProcSelfAppFrame(const SERIAL_FRAME_C &objFrame)
{
    if (IsFrameRepeat(objFrame)) {
        ++stDbgInfo_.frameCountOfSeltAppRepeat;
        return;
    }

    ++stDbgInfo_.frameCountOfSelfAppMsg;
    stDbgInfo_.selfAppMsgLen += objFrame.GetDataLen();

    /* 应用层消息处理 */
    R_ASSERT(objProcMsgFunc_ != NULL, NOTHING);

    SERIAL_FRAME_BUFF_P_STRU stMsgBuff;
    int rsltOfGetMsgBuff = objFrame.GetMsgBuff(stMsgBuff);
    R_ASSERT(rsltOfGetMsgBuff == 0, NOTHING);

    objProcMsgFunc_(objFrame.GetSrcAddr(), objFrame.GetMsgType(), stMsgBuff.pucBuff, stMsgBuff.buffLen);
}

/**************************************************************************************
功能描述: 消息数据处理
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::ProcSelfFrame(const SERIAL_FRAME_C &objFrame)
{
    ++stDbgInfo_.frameCountOfSelf;

    if (objFrame.IsAck()) {
        ++stDbgInfo_.frameCountOfSelfAck;
        objSender_.ProcAck(objFrame);

    } else if (objFrame.IsReq()) {
        ++stDbgInfo_.frameCountOfSelfApp;
        ProcSelfAppFrame(objFrame);

    } else if (objFrame.IsHandshake()) {
        ++stDbgInfo_.frameCountOfSelfHandshake;
        /* 握手帧等不需要处理 */
    } else {
    }
}

void SERIAL_LINKER_C::ProcFrame(const SERIAL_FRAME_C &objFrame, bool isCheckSucc)
{
    if (!objFrame.IsToLoc()) {
        ++stDbgInfo_.frameCountOfTgtAddrErr;
        return;
    }

    /* 本地处理的非ACK帧需要回复ACK */
    if (!objFrame.IsAck()) {
        objSender_.SendAck(objFrame, isCheckSucc);
    }

    /* 校验失败的消息不处理 */
    if (!isCheckSucc) {
        return;
    }

    ProcSelfFrame(objFrame);
}

bool SERIAL_LINKER_C::ReadData(SERIAL_LINKER_RECV_DATA_STRU &stRecvDataInfo)
{
    if (stRecvDataInfo.dataLen >= sizeof(stRecvDataInfo.aucData)) {
        ++stDbgInfo_.readDataAbnCount;
        stRecvDataInfo.dataLen = 0;
    }

    unsigned int freeBuffLen = sizeof(stRecvDataInfo.aucData) - stRecvDataInfo.dataLen;
    int  readDataLen = objSerial_.ReadData(&stRecvDataInfo.aucData[stRecvDataInfo.dataLen], freeBuffLen);
    if ((readDataLen < 0) || (readDataLen > int(freeBuffLen))) {
        ++stDbgInfo_.readDataAbnCount;
        stRecvDataInfo.dataLen = 0;
        return false;

    } else if (readDataLen == 0) {
        /* 如果持续一段时间没有读取到数据，则说明之前遗留的数据无后续数据，去掉帧头一个字节继续往下解 */
        if ((stRecvDataInfo.dataLen > 0) && ((Comm_GetTimeSec() - stRecvDataInfo.lastRecvDataTime) > SERIAL_LINKER_SEND_INTERVAL_MAX)) {
            PrintHexStream("Recv Timeout.", stRecvDataInfo.aucData, stRecvDataInfo.dataLen);
            stRecvDataInfo.ClrData(1);
            stDbgInfo_.discardDataLen += 1;
        }
        return false;

    } else {
        stRecvDataInfo.dataLen += readDataLen;
        stRecvDataInfo.lastRecvDataTime = Comm_GetTimeSec();
        return true;
    }
}

/**************************************************************************************
功能描述: 处理接收的数据
          返回已处理的数据的长度
修改记录:
**************************************************************************************/
unsigned short SERIAL_LINKER_C::ProcData(const unsigned char *pucData, unsigned short dataLen)
{
    SERIAL_FRAME_C objFrame(enLocAddr_);
    unsigned short         skipDataLen         = 0;
    bool           isLastProcDataFrame = true;

    while (skipDataLen < dataLen) {
        const unsigned char *pucLeftProcData = pucData + skipDataLen;
        unsigned int        leftProcDataLen = dataLen - skipDataLen;

        /* 如果剩余数据无法解码，则等后续数据一起解码 */
        int rslt = objFrame.Decode(pucLeftProcData, leftProcDataLen);
        if (rslt != 0) {
            break;
        }

        /* 如果当前不是帧，则跳过一个字节继续解码 */
        if (!objFrame.IsFrame()) {
            if (isLastProcDataFrame) {
                PrintHexStream("Not Frame.", pucLeftProcData, leftProcDataLen);
            }

            isLastProcDataFrame = false;
            ++skipDataLen;
            ++stDbgInfo_.discardDataLen;
            continue;
        }

        isLastProcDataFrame = true;

        /* 如果当前帧没有结束，则等后续数据一起解码 */
        if (!objFrame.IsComplete()) {
            break;
        }

        /* 如果是完整帧，则压入队列 */
        ++stDbgInfo_.frameCount;
        bool isFrameCheckSucc = objFrame.IsCheckSucc();
        PushFrame(objFrame, isFrameCheckSucc);

        /* 如果帧处理失败，可能当前解析是错误的，从下一个字节重新尝试 */
        if (!isFrameCheckSucc) {
            PrintHexStream("Check Fail.", pucLeftProcData, leftProcDataLen);
            ++stDbgInfo_.frameCountOfCheckFail;
            ++skipDataLen;
            ++stDbgInfo_.discardDataLen;
            continue;
        }

        skipDataLen += objFrame.GetFrameLen();
    }

    return skipDataLen;
}

void SERIAL_LINKER_C::PushFrame(const SERIAL_FRAME_C &objFrame, bool isCheckSucc)
{
    std::unique_lock<std::mutex> objLocker(objRecvFrameListMutex_);

    unsigned short frameLen         = objFrame.GetFrameLen();
    unsigned short recvFrameBuffLen = sizeof(SERIAL_LINKER_RECV_FRAME_STRU) + frameLen;
    SERIAL_LINKER_RECV_FRAME_STRU *pstRecvFrame = (SERIAL_LINKER_RECV_FRAME_STRU *)new unsigned char[recvFrameBuffLen];
    R_ASSERT(pstRecvFrame != nullptr, NOTHING);
    pstRecvFrame->isCheckSucc = isCheckSucc;
    pstRecvFrame->framelen    = frameLen;
    (void)memcpy(pstRecvFrame->aucFrameBuff, objFrame.GetFrameHead(), frameLen);

    listRecvFrame_.push_back(pstRecvFrame);

    unsigned int listSize = unsigned int(listRecvFrame_.size());
    ++stDbgInfo_.pushFrameListCount;
    stDbgInfo_.totalFrameListSize += listSize;
    stDbgInfo_.maxRecvFrameListSize = std::max(stDbgInfo_.maxRecvFrameListSize, listSize);
}

const SERIAL_LINKER_RECV_FRAME_STRU *SERIAL_LINKER_C::PopFrame(void)
{
    std::unique_lock<std::mutex> objLocker(objRecvFrameListMutex_);

    if (listRecvFrame_.empty()) {
        return nullptr;
    }

    const SERIAL_LINKER_RECV_FRAME_STRU *pstFirst = listRecvFrame_.front();
    listRecvFrame_.pop_front();
    return pstFirst;
}

/**************************************************************************************
功能描述: 串口管理 收取消息线程任务处理函数
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::ReadDataTask(void)
{
    ST_LOG_INFO("Begin.");

    SERIAL_LINKER_RECV_DATA_STRU stRecvDataInfo;

    while (isThreadWork_)
    {
        StatisReadCycleLen();

        /* 读取数据并处理 */
        bool hasReadData = ReadData(stRecvDataInfo);
        if (!hasReadData) {
            continue;
        }

        unsigned short dataLenProced = ProcData(stRecvDataInfo.aucData, stRecvDataInfo.dataLen);
        stRecvDataInfo.ClrData(dataLenProced);
    }

    ST_LOG_INFO("End.");
}

void SERIAL_LINKER_C::ProcFrameTask(void)
{
    ST_LOG_INFO("Begin.");

    while (isThreadWork_)
    {
        /* 消息处理对时间戳精度要求不高，每次循环使用一个时间戳即可，如果影响可优化 */
        double curTime  = Comm_GetTimeSec();
        objSender_.PeriodUpt(curTime);
        OutputDbgInfo(curTime);

        const SERIAL_LINKER_RECV_FRAME_STRU *pstRecvFrame = PopFrame();
        if (pstRecvFrame == nullptr) {
            usleep(1000);
            continue;
        }

        SERIAL_FRAME_C objFrame(enLocAddr_);
        int rslt = objFrame.Decode(pstRecvFrame->aucFrameBuff, pstRecvFrame->framelen);
        if (rslt != 0) {
            ST_LOG_FATAL("objFrame.Decode(FrameLen=%u) Fail.", pstRecvFrame->framelen);

        } else {
            ProcFrame(objFrame, pstRecvFrame->isCheckSucc);
        }

        delete [](const unsigned char *)pstRecvFrame;
    }

    /* 清除所有未处理完的消息 */
    while (true)
    {
        const SERIAL_LINKER_RECV_FRAME_STRU *pstRecvFrame = PopFrame();
        if (pstRecvFrame == nullptr) {
            break;
        }

        delete [](const unsigned char *)pstRecvFrame;
    }

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 串口管理 创建串口读取线程
修改记录:
**************************************************************************************/
int SERIAL_LINKER_C::CreateThread(void)
{
    R_ASSERT(pobjReadDataThread_ == nullptr, -1);
    R_ASSERT(pobjProcFrameThread_ == nullptr, -1);

    isThreadWork_ = true;

    pobjReadDataThread_ = new std::thread(&SERIAL_LINKER_C::ReadDataTask, this);
    if (pobjReadDataThread_ == NULL) {
        ST_LOG_ERR("New ReadDataTask Fail.");
        return -1;
    }

    pobjProcFrameThread_ = new std::thread(&SERIAL_LINKER_C::ProcFrameTask, this);
    if (pobjProcFrameThread_ == NULL) {
        ST_LOG_ERR("New ProcFrameTask Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 串口管理 删除串口读取线程
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::DeleteThread(void)
{
    ST_LOG_INFO("Begin.");

    /* 停止循环，等待自然结束 */
    isThreadWork_ = false;

    if (pobjReadDataThread_ != NULL) {
        pobjReadDataThread_->join();

        delete pobjReadDataThread_;
        pobjReadDataThread_ = NULL;
    }

    if (pobjProcFrameThread_ != NULL) {
        pobjProcFrameThread_->join();

        delete pobjProcFrameThread_;
        pobjProcFrameThread_ = NULL;
    }

    ST_LOG_INFO("Succ.");
}

