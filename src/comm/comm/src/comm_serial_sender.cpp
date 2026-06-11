
#include "comm_serial_sender.h"

void SERIAL_SENDER_C::PeriodUpt(double curTime)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    curTime_ = curTime;

    UptSendingFrameState();
    TrySendNextFrame();
    TryHandshake();
}

INT32 SERIAL_SENDER_C::Send(SERIAL_ADDR_ENUM enTgtAddr, UINT08 msgType, const UINT08 *pucMsg, UINT16 msgLen)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    ++stDbgInfo_.msgCountOfApp;

    if (IsConnectTimeout()) {
        ++stDbgInfo_.msgConutOfAppFailAsDisconnect;
        return -1;
    }

    SERIAL_FRAME_BUFF_A_STRU stFrameBuff;
    SERIAL_FRAME_C           objFrame(enLocAddr_);
    INT32 rsltOfEncode = objFrame.EncodeReq(enTgtAddr, AllocFrameNo(enTgtAddr), msgType, pucMsg, msgLen, stFrameBuff);
    R_ASSERT_3(rsltOfEncode == 0, -1, enTgtAddr, msgType, msgLen);

    return SendFrameSequentially(objFrame);
}

void SERIAL_SENDER_C::SendAck(const SERIAL_FRAME_C &objRecvFrame, BOOL isCheckSucc)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    ++stDbgInfo_.frameCountOfAck;

    SERIAL_FRAME_BUFF_A_STRU stFrameBuff;
    SERIAL_FRAME_C           objFrame(enLocAddr_);
    objFrame.EncodeAck(objRecvFrame, stFrameBuff, isCheckSucc);

    SendFrameImmediately(objFrame);
}

INT32 SERIAL_SENDER_C::SendPass(const UINT08 *pucData, UINT16 dataLen)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    ++stDbgInfo_.frameCountOfPass;

    SERIAL_FRAME_C objFrame(enLocAddr_);
    INT32 rsltOfDecode = objFrame.Decode(pucData, dataLen);
    R_ASSERT(rsltOfDecode == 0, -1);

    SendFrameImmediately(objFrame);
    return 0;
}

void SERIAL_SENDER_C::ProcAck(const SERIAL_FRAME_C &objAckFrame)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    /* 判断该帧是否匹配正在发送的帧 */
    if (   (sendingFrameSendCount_ == 0)
        || (objAckFrame.GetSrcAddr() != objSendingFrame_.GetTgtAddr())
        || (objAckFrame.GetTgtAddr() != objSendingFrame_.GetSrcAddr())
        || (objAckFrame.GetFrameNo() != objSendingFrame_.GetFrameNo())) {
        ++stDbgInfo_.frameCountOfDiscardRecvAck;
        return;
    }

    ProcSendingFrameAck(objAckFrame);
}

BOOL SERIAL_SENDER_C::IsConnectFault(void)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    return IsConnectTimeout();
}

void SERIAL_SENDER_C::OutputDbgInfo(void)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    stDbgInfo_.Output();
}

UINT08 SERIAL_SENDER_C::AllocFrameNo(SERIAL_ADDR_ENUM enTgtAddr)
{
    R_ASSERT_1(enTgtAddr < ARRAY_SIZE(aucAddrFrameNo_), UINT08_NULL, enTgtAddr);

    return ++aucAddrFrameNo_[enTgtAddr];
}

BOOL SERIAL_SENDER_C::IsConnectTimeout(void)
{
    return ((curTime_ - lastSendFrameSuccTime_) > SERIAL_SENDER_FAULT_NO_SUCC_TIMEOUT);
}

void SERIAL_SENDER_C::SendFrameImmediately(const SERIAL_FRAME_C &objFrame)
{
    SERIAL_FRAME_BUFF_P_STRU stBuff;
    objFrame.GetBuff(stBuff);

    objSerial_.SendData(stBuff.pucBuff, stBuff.buffLen);
    ++stDbgInfo_.frameCount;
}

/**************************************************************************************
功能描述: 按队列发送帧
修改记录:
**************************************************************************************/
INT32 SERIAL_SENDER_C::SendFrameSequentially(const SERIAL_FRAME_C &objFrame)
{
    ++stDbgInfo_.msgCount;

    if (listFrame_.size() >= maxListSize_) {
        ++stDbgInfo_.msgCountOfFailAsBusy;
        return -1;
    }

    SERIAL_FRAME_BUFF_A_STRU stBuff;
    INT32 rsltOfBuff = objFrame.GetBuff(stBuff);
    R_ASSERT(rsltOfBuff == 0, -1);
    listFrame_.push_back(stBuff);

    lastTrySendSelfMsgTime_ = curTime_;

    TrySendNextFrame();
    return 0;
}

/**************************************************************************************
功能描述: 发送握手帧
修改记录:
**************************************************************************************/
void SERIAL_SENDER_C::SendHandshakeMsg(void)
{
    ++stDbgInfo_.msgCountOfHandshake;

    SERIAL_FRAME_BUFF_A_STRU stFrameBuff;
    SERIAL_FRAME_C           objFrame(enLocAddr_);
    objFrame.EncodeHandshake(enPeerAddr_, AllocFrameNo(enPeerAddr_), stFrameBuff);

    INT32 rsltOfSend = SendFrameSequentially(objFrame);
    R_ASSERT(rsltOfSend == 0, NOTHING);
}

void SERIAL_SENDER_C::ReTrySendingFrame(void)
{
    SERIAL_FRAME_BUFF_P_STRU stBuff;
    INT32 rsltOfGetFrameBuff = objSendingFrame_.GetBuff(stBuff);
    R_ASSERT(rsltOfGetFrameBuff == 0, NOTHING);

    objSerial_.SendData(stBuff.pucBuff, stBuff.buffLen);
    ++stDbgInfo_.frameCount;
    ++stDbgInfo_.frameCountOfSelf;

    ++sendingFrameSendCount_;
    sendingFrameSendTime_ = curTime_;
}

void SERIAL_SENDER_C::EndSendingFrame(void)
{
    sendingFrameSendCount_ = 0;
    objSendingFrame_.Clear();
    listFrame_.pop_front();
}

void SERIAL_SENDER_C::ProcSendingFrameAckFail(void)
{
    /* 如果发送次数已经达到最大次数则不再重试，否则继续重试 */
    if (sendingFrameSendCount_ >= SERIAL_SENDER_MAX_RETRY_SEND_TIMES) {
        ++stDbgInfo_.msgCountOfFailOnSending;
        EndSendingFrame();
        TrySendNextFrame();

    } else {
        ReTrySendingFrame();
    }
}

void SERIAL_SENDER_C::ProcSendingFrameAck(const SERIAL_FRAME_C &objAckFrame)
{
    if (objAckFrame.IsAckSucc()) {
        stDbgInfo_.msgLenOfAppSucc += objSendingFrame_.GetDataLen();
        lastSendFrameSuccTime_ = curTime_;
        EndSendingFrame();
        TrySendNextFrame();

    } else {
        ++stDbgInfo_.frameCountOfFailAsCheck;
        ProcSendingFrameAckFail();
    }
}

void SERIAL_SENDER_C::ProcSendingFrameAckTimeout(void)
{
    ++stDbgInfo_.frameCountOfFailAsAckTimeout;

    ProcSendingFrameAckFail();
}

void SERIAL_SENDER_C::UptSendingFrameState(void)
{
    if (   (sendingFrameSendCount_ == 0)
        || ((Comm_GetTimeSec() - sendingFrameSendTime_) < SERIAL_SENDER_WAIT_ACK_TIMEOUT)) {
        return;
    }

    ProcSendingFrameAckTimeout();
}

/**************************************************************************************
功能描述: 处理帧队列
修改记录:
**************************************************************************************/
void SERIAL_SENDER_C::TrySendNextFrame(void)
{
    if ((listFrame_.size() == 0) || (sendingFrameSendCount_ != 0)) {
        return;
    }

    SERIAL_FRAME_BUFF_A_STRU &stFirstFrameBuff = listFrame_.front();
    objSerial_.SendData(stFirstFrameBuff.aucBuff, stFirstFrameBuff.buffLen);

    ++stDbgInfo_.frameCount;
    ++stDbgInfo_.frameCountOfSelf;

    (void)objSendingFrame_.Decode(stFirstFrameBuff.aucBuff, stFirstFrameBuff.buffLen);
    sendingFrameSendTime_  = curTime_;
    sendingFrameSendCount_ = 1;
}

/**************************************************************************************
功能描述: 尝试握手
修改记录:
**************************************************************************************/
void SERIAL_SENDER_C::TryHandshake(void)
{
    /* 一段时间没有发送消息，则尝试握手 */
    if ((curTime_ - lastTrySendSelfMsgTime_) > SERIAL_SENDER_HANDSHAKE_INTERVAL) {
        SendHandshakeMsg();
    }
}


