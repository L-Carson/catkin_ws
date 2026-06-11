
#include "comm/comm_serial_linker.h"

/**************************************************************************************
功能描述: 串口管理 初始化
修改记录:
**************************************************************************************/
INT32 SERIAL_LINKER_C::Init(const std::string &strDevName, const SERIAL_LINKER_RECV_MSG_FUNC_C &objRecvMsgFunc)
{
    ST_LOG_INFO("LocAddr=%u, PeerAddr=%u, DevName=%s.", enLocAddr_, enPeerAddr_, strDevName.c_str());

    INT32 rslt = objSerial_.Open(strDevName.c_str(), SERIAL_LINKER_BANDRATE);
    if (rslt != 0) {
        ST_LOG_ERR("objSerial_.Open(%s) Fail.", strDevName.c_str());
        return -1;
    }

    rslt = objPasser_.Init();
    if (rslt != 0) {
        ST_LOG_ERR("objPasser_.Init() Fail.");
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
功能描述: 注册消息代收处理
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::RegRecvPassMsg(SERIAL_ADDR_ENUM enTgtAddr, const SERIAL_LINKER_RECV_PASS_MSG_FUNC_C &objRecvMsgFunc)
{
    ST_LOG_INFO("Reg Recv Pass Msg Addr %u.", enTgtAddr);

    SERIAL_LINKER_RECV_PASS_MSG_REG_STRU stReg;
    stReg.enTgtAddr      = enTgtAddr;
    stReg.objRecvMsgFunc = objRecvMsgFunc;

    listRecvPassMsgReg_.push_back(stReg);
}

/**************************************************************************************
功能描述: 串口管理 注册转发入消息
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::RegPassInMsg(SERIAL_ADDR_ENUM enTgtAddr, const std::string &strDataTopic)
{
    objPasser_.RegPassIn(enTgtAddr, strDataTopic);
}

/**************************************************************************************
功能描述: 串口管理 注册转发出消息
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::RegPassOutMsg(const std::string &strDataTopic)
{
    objPasser_.RegPassOut(strDataTopic);
}

/**************************************************************************************
功能描述: 串口管理 发送消息
修改记录:
**************************************************************************************/
INT32 SERIAL_LINKER_C::Send(SERIAL_ADDR_ENUM enTgtAddr, UINT08 msgType, const UINT08 *pucMsg, UINT32 msgLen)
{
    return objSender_.Send(enTgtAddr, msgType, pucMsg, UINT16(msgLen));
}

INT32 SERIAL_LINKER_C::SendPassMsg(const UINT08 *pucData, UINT32 dataLen)
{
    return objSender_.SendPass(pucData, dataLen);
}

/**************************************************************************************
功能描述: 串口管理 判断连接是否正常
修改记录:
**************************************************************************************/
BOOL SERIAL_LINKER_C::IsFault(void)
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

    stDbgInfo_.maxReadCycleLen = MAX(stDbgInfo_.maxReadCycleLen, curCycleLen);

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

const SERIAL_LINKER_RECV_PASS_MSG_REG_STRU *SERIAL_LINKER_C::GetAppPassMsgReg(SERIAL_ADDR_ENUM enTgtAddr)
{
    for (const SERIAL_LINKER_RECV_PASS_MSG_REG_STRU &stRecvPassMsgReg : listRecvPassMsgReg_) {
        if (stRecvPassMsgReg.enTgtAddr == enTgtAddr) {
            return &stRecvPassMsgReg;
        }
    }

    return NULL;
}

BOOL SERIAL_LINKER_C::IsAppPassFrame(const SERIAL_FRAME_C &objFrame)
{
    return (NULL != GetAppPassMsgReg(objFrame.GetTgtAddr()));
}

/**************************************************************************************
功能描述: 判断本次收到的帧编号是否和上一次一样，通过此判断是否同一条消息
修改记录:
**************************************************************************************/
BOOL SERIAL_LINKER_C::IsFrameRepeat(const SERIAL_FRAME_C &objFrame)
{
    SERIAL_ADDR_ENUM enSrcAddr = objFrame.GetSrcAddr();
    R_ASSERT(enSrcAddr < ARRAY_SIZE(aucAddrFrameNo_), true);

    UINT08 frameNo = objFrame.GetFrameNo();
    if (frameNo != aucAddrFrameNo_[enSrcAddr]) {
        aucAddrFrameNo_[enSrcAddr] = frameNo;
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
    INT32 rsltOfGetMsgBuff = objFrame.GetMsgBuff(stMsgBuff);
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

void SERIAL_LINKER_C::ProcAppPassFrame(const SERIAL_FRAME_C &objFrame)
{
    ++stDbgInfo_.frameCountOfPassByApp;

    const SERIAL_LINKER_RECV_PASS_MSG_REG_STRU *pstPassMsgReg = GetAppPassMsgReg(objFrame.GetTgtAddr());
    R_ASSERT_1(pstPassMsgReg != NULL, NOTHING, objFrame.GetTgtAddr());
    R_ASSERT_1(pstPassMsgReg->objRecvMsgFunc != NULL, NOTHING, objFrame.GetTgtAddr());

    SERIAL_FRAME_BUFF_P_STRU stFrame;
    INT32 rsltOfGetBuff = objFrame.GetBuff(stFrame);
    R_ASSERT(rsltOfGetBuff == 0, NOTHING);

    pstPassMsgReg->objRecvMsgFunc(stFrame.pucBuff, stFrame.buffLen);
}

/**************************************************************************************
功能描述: 处理转发帧
修改记录:
**************************************************************************************/
void SERIAL_LINKER_C::ProcPassFrame(const SERIAL_FRAME_C &objFrame)
{
    /* 消息转发，包括ack消息，握手消息，应用消息等 */
    ++stDbgInfo_.frameCountOfPassByRos;

    /* 源不能是本地地址，防止其它端伪造本地消息 */
    if (objFrame.IsFromLoc()) {
        ++stDbgInfo_.frameCountOfPassAddrFalsify;
        return;
    }

    SERIAL_FRAME_BUFF_P_STRU stFrame;
    INT32 rsltOfGetBuff = objFrame.GetBuff(stFrame);
    R_ASSERT(rsltOfGetBuff == 0, NOTHING);

    INT32 rslt = objPasser_.Pass(objFrame.GetTgtAddr(), stFrame.pucBuff, stFrame.buffLen);
    if (rslt != 0) {
        ++stDbgInfo_.frameCountOfPassSendingFail;
    }
}

void SERIAL_LINKER_C::ProcFrame(const SERIAL_FRAME_C &objFrame, BOOL isCheckSucc)
{
    /* 本地处理的非ACK帧需要回复ACK */
    BOOL isLocAppFrame = objFrame.IsToLoc();
    if (!objFrame.IsAck() && isLocAppFrame) {
        objSender_.SendAck(objFrame, isCheckSucc);
    }

    /* 校验失败的消息不处理 */
    if (!isCheckSucc) {
        return;
    }

    /* 转发消息有两种处理方式：优先回调上层注册处理函数让上层转发或自处理，如果无回调则通过ros话题发送至同系统内其它串口节点转发 */
    if (isLocAppFrame) {
        ProcSelfFrame(objFrame);
    } else if (IsAppPassFrame(objFrame)) {
        ProcAppPassFrame(objFrame);
    } else {
        ProcPassFrame(objFrame);
    }
}

BOOL SERIAL_LINKER_C::ReadData(SERIAL_LINKER_RECV_DATA_STRU &stRecvDataInfo)
{
    if (stRecvDataInfo.dataLen >= sizeof(stRecvDataInfo.aucData)) {
        ++stDbgInfo_.readDataAbnCount;
        stRecvDataInfo.dataLen = 0;
    }

    UINT32 freeBuffLen = sizeof(stRecvDataInfo.aucData) - stRecvDataInfo.dataLen;
    INT32  readDataLen = objSerial_.ReadData(&stRecvDataInfo.aucData[stRecvDataInfo.dataLen], freeBuffLen);
    if ((readDataLen < 0) || (readDataLen > INT32(freeBuffLen))) {
        ++stDbgInfo_.readDataAbnCount;
        stRecvDataInfo.dataLen = 0;
        return false;

    } else if (readDataLen == 0) {
        /* 如果持续一段时间没有读取到数据，则说明之前遗留的数据无后续数据，去掉帧头一个字节继续往下解 */
        if ((stRecvDataInfo.dataLen > 0) && ((Comm_GetTimeSec() - stRecvDataInfo.lastRecvDataTime) > SERIAL_LINKER_SEND_INTERVAL_MAX)) {
            Comm_PrintStream("Recv Timeout.", stRecvDataInfo.aucData, stRecvDataInfo.dataLen);
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
UINT16 SERIAL_LINKER_C::ProcData(const UINT08 *pucData, UINT16 dataLen)
{
    SERIAL_FRAME_C objFrame(enLocAddr_);
    UINT16         skipDataLen         = 0;
    BOOL           isLastProcDataFrame = true;

    while (skipDataLen < dataLen) {
        const UINT08 *pucLeftProcData = pucData + skipDataLen;
        UINT32        leftProcDataLen = dataLen - skipDataLen;

        /* 如果剩余数据无法解码，则等后续数据一起解码 */
        INT32 rslt = objFrame.Decode(pucLeftProcData, leftProcDataLen);
        if (rslt != 0) {
            break;
        }

        /* 如果当前不是帧，则跳过一个字节继续解码 */
        if (!objFrame.IsFrame()) {
            if (isLastProcDataFrame) {
                Comm_PrintStream("Not Frame.", pucLeftProcData, leftProcDataLen);
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
        BOOL isFrameCheckSucc = objFrame.IsCheckSucc();
        PushFrame(objFrame, isFrameCheckSucc);

        /* 如果帧处理失败，可能当前解析是错误的，从下一个字节重新尝试 */
        if (!isFrameCheckSucc) {
            Comm_PrintStream("Check Fail.", pucLeftProcData, leftProcDataLen);
            ++stDbgInfo_.frameCountOfCheckFail;
            ++skipDataLen;
            ++stDbgInfo_.discardDataLen;
            continue;
        }

        skipDataLen += objFrame.GetFrameLen();
    }

    return skipDataLen;
}

void SERIAL_LINKER_C::PushFrame(const SERIAL_FRAME_C &objFrame, BOOL isCheckSucc)
{
    std::unique_lock<std::mutex> objLocker(objRecvFrameListMutex_);

    UINT16 frameLen         = objFrame.GetFrameLen();
    UINT16 recvFrameBuffLen = sizeof(SERIAL_LINKER_RECV_FRAME_STRU) + frameLen;
    SERIAL_LINKER_RECV_FRAME_STRU *pstRecvFrame = (SERIAL_LINKER_RECV_FRAME_STRU *)new UINT08[recvFrameBuffLen];
    R_ASSERT(pstRecvFrame != nullptr, NOTHING);
    pstRecvFrame->isCheckSucc = isCheckSucc;
    pstRecvFrame->framelen    = frameLen;
    (void)memcpy(pstRecvFrame->aucFrameBuff, objFrame.GetFrameHead(), frameLen);

    listRecvFrame_.push_back(pstRecvFrame);

    UINT32 listSize = UINT32(listRecvFrame_.size());
    ++stDbgInfo_.pushFrameListCount;
    stDbgInfo_.totalFrameListSize += listSize;
    stDbgInfo_.maxRecvFrameListSize = MAX(stDbgInfo_.maxRecvFrameListSize, listSize);
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
        BOOL hasReadData = ReadData(stRecvDataInfo);
        if (!hasReadData) {
            continue;
        }

        UINT16 dataLenProced = ProcData(stRecvDataInfo.aucData, stRecvDataInfo.dataLen);
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
        INT32 rslt = objFrame.Decode(pstRecvFrame->aucFrameBuff, pstRecvFrame->framelen);
        if (rslt != 0) {
            ST_LOG_FATAL("objFrame.Decode(FrameLen=%u) Fail.", pstRecvFrame->framelen);

        } else {
            ProcFrame(objFrame, pstRecvFrame->isCheckSucc);
        }

        delete [](const UINT08 *)pstRecvFrame;
    }

    /* 清除所有未处理完的消息 */
    while (true)
    {
        const SERIAL_LINKER_RECV_FRAME_STRU *pstRecvFrame = PopFrame();
        if (pstRecvFrame == nullptr) {
            break;
        }

        delete [](const UINT08 *)pstRecvFrame;
    }

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 串口管理 创建串口读取线程
修改记录:
**************************************************************************************/
INT32 SERIAL_LINKER_C::CreateThread(void)
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

