#ifndef __COMM_SERIAL_SENDER_H__
#define __COMM_SERIAL_SENDER_H__

#include "comm/comm_serial_frame.h"
#include "comm/comm_serial.h"
#include "comm/comm_base.h"

#include <list>
#include <mutex>

/**************************************************************************************
功能描述: 握手间隔时长
          如果持续这么长的时间无发送消息，则发送握手消息
修改记录:
**************************************************************************************/
#define SERIAL_SENDER_HANDSHAKE_INTERVAL            1.0

/**************************************************************************************
功能描述: 判定串口异常的时间
          如果持续这么长的时间无法送成功消息，包括握手消息，则判定串口异常
          采用3次握手间隔时长
修改记录:
**************************************************************************************/
#define SERIAL_SENDER_FAULT_NO_SUCC_TIMEOUT         (SERIAL_SENDER_HANDSHAKE_INTERVAL * 3 + 0.1)

/**************************************************************************************
功能描述: 串口消息最大重发次数
修改记录:
**************************************************************************************/
#define SERIAL_SENDER_MAX_RETRY_SEND_TIMES          1

/**************************************************************************************
功能描述: 发送缓存消息数
修改记录:
**************************************************************************************/
#define SERIAL_SNEDER_MSG_BUFF_SIZE                 3

/**************************************************************************************
功能描述: ACK等待最大时长，单位s
修改记录:
**************************************************************************************/
#define SERIAL_SENDER_WAIT_ACK_TIMEOUT              0.05

/**************************************************************************************
功能描述: 串口发送消息统计
修改记录:
**************************************************************************************/
struct SERIAL_SENDER_DBG_INFO_STRU {
    UINT32 msgCount;                            /* 总的发送消息数，包含应用层消息和握手消息 */
    UINT32 msgCountOfApp;                       /* 应用方发送的消息数 */
    UINT32 msgConutOfAppFailAsDisconnect;       /* 因断链导致应用发送失败的消息数 */
    UINT32 msgCountOfHandshake;                 /* 发送的握手消息数 */
    UINT32 msgCountOfFailAsBusy;                /* 因链路忙导致丢弃而失败的消息数 */
    UINT32 msgCountOfFailOnSending;             /* 多次尝试最终仍失败的消息数 */

    UINT32 msgLenOfAppSucc;                     /* 发送成功的消息总长 */

    UINT32 frameCount;                          /* 总的发送帧数 */
    UINT32 frameCountOfSelf;                    /* 发送的自身帧数 */
    UINT32 frameCountOfPass;                    /* 发送的转发帧数 */
    UINT32 frameCountOfAck;                     /* 发送的ACK帧数 */
    UINT32 frameCountOfFailAsCheck;             /* 对方回复校验失败的发送帧数 */
    UINT32 frameCountOfFailAsAckTimeout;        /* 对方回复超时的发送帧数 */
    UINT32 frameCountOfDiscardRecvAck;          /* 丢弃的收到ACK数 */

    SERIAL_SENDER_DBG_INFO_STRU(void)
    {
        Clear();
    }

    void Clear(void)
    {
        msgCount                             = 0;
        msgCountOfApp                        = 0;
        msgConutOfAppFailAsDisconnect        = 0;
        msgCountOfHandshake                  = 0;
        msgCountOfFailAsBusy                 = 0;
        msgCountOfFailOnSending              = 0;

        msgLenOfAppSucc                      = 0;

        frameCount                           = 0;
        frameCountOfSelf                     = 0;
        frameCountOfPass                     = 0;
        frameCountOfAck                      = 0;
        frameCountOfFailAsCheck              = 0;
        frameCountOfFailAsAckTimeout         = 0;
        frameCountOfDiscardRecvAck           = 0;
    }

    void Output(void)
    {
        float msgFailRate = (msgCount == 0) ? 0.0 : (float(msgCountOfFailAsBusy + msgCountOfFailOnSending) / float(msgCount));

        ST_LOG_INFO("[Statis].MsgFailRate=%f, "
                    "[MsgCount].Total=%u, .App=%u, .AppFailAsDisconnect=%u, .Handshake=%u, .FailAsBusy=%u, .FailOnSending=%u, "
                    "[MsgLen].AppSucc=%u, "
                    "[FrameCount].Total=%u, .Self=%u, .Pass=%u, .Ack=%u, .FailAsCheck=%u, .FailAsAckTimeout=%u, .DiscardRecvAck=%u.",
                    msgFailRate,
                    msgCount, msgCountOfApp, msgConutOfAppFailAsDisconnect, msgCountOfHandshake, msgCountOfFailAsBusy, msgCountOfFailOnSending,
                    msgLenOfAppSucc,
                    frameCount, frameCountOfSelf, frameCountOfPass, frameCountOfAck, frameCountOfFailAsCheck, frameCountOfFailAsAckTimeout, frameCountOfDiscardRecvAck);

        Clear();
    }
};


/**************************************************************************************
功能描述: 串口发送器
修改记录:
**************************************************************************************/
class SERIAL_SENDER_C
{
public:
    SERIAL_SENDER_C(SERIAL_C &objSerial, SERIAL_ADDR_ENUM enLocAddr, SERIAL_ADDR_ENUM enPeerAddr, UINT32 maxListSize = SERIAL_SNEDER_MSG_BUFF_SIZE)
                                                        : objSerial_(objSerial),
                                                          enLocAddr_(enLocAddr),
                                                          enPeerAddr_(enPeerAddr),
                                                          maxListSize_(maxListSize),
                                                          objSendingFrame_(enLocAddr)
    {
        memset(aucAddrFrameNo_, 0xFF, sizeof(aucAddrFrameNo_));
    }

    void  PeriodUpt(double curTime);
    INT32 Send(SERIAL_ADDR_ENUM enTgtAddr, UINT08 msgType, const UINT08 *pucMsg, UINT16 msgLen);
    void  SendAck(const SERIAL_FRAME_C &objRecvFrame, BOOL isCheckSucc);
    INT32 SendPass(const UINT08 *pucData, UINT16 dataLen);
    void  ProcAck(const SERIAL_FRAME_C &objAckFrame);
    BOOL  IsConnectFault(void);
    void  OutputDbgInfo(void);

private:
    const SERIAL_C                     &objSerial_;
    const SERIAL_ADDR_ENUM              enLocAddr_;
    const SERIAL_ADDR_ENUM              enPeerAddr_;
    const UINT32                        maxListSize_;

    std::mutex                          objMutex_;
    SERIAL_SENDER_DBG_INFO_STRU         stDbgInfo_;
    UINT08                              aucAddrFrameNo_[SERIAL_ADDR_BUTT];

    double                              curTime_                     = 0.0;          /* 采用更新时的时间，避免频繁调用时间接口 */
    double                              lastSendFrameSuccTime_       = 0.0;
    double                              lastTrySendSelfMsgTime_      = 0.0;          /* 上一次尝试发送消息的时间 */

    std::list<SERIAL_FRAME_BUFF_A_STRU> listFrame_;
    UINT32                              sendingFrameSendCount_       = 0;            /* 为0表示当前没有正在发送帧 */
    SERIAL_FRAME_C                      objSendingFrame_;                            /* 正在发送的帧，sendingFrameSendCount_ 为0时无效 */
    double                              sendingFrameSendTime_        = 0.0;          /* 发送帧的发送时间，sendingFrameSendCount_ 为0时无效 */

    UINT08 AllocFrameNo(SERIAL_ADDR_ENUM enTgtAddr);
    BOOL   IsConnectTimeout(void);

    void   SendFrameImmediately(const SERIAL_FRAME_C &objFrame);
    INT32  SendFrameSequentially(const SERIAL_FRAME_C &objFrame);
    void   SendHandshakeMsg(void);

    void   ReTrySendingFrame(void);
    void   EndSendingFrame(void);
    void   ProcSendingFrameAckFail(void);
    void   ProcSendingFrameAck(const SERIAL_FRAME_C &objAckFrame);
    void   ProcSendingFrameAckTimeout(void);

    void   UptSendingFrameState(void);
    void   TrySendNextFrame(void);
    void   TryHandshake(void);
};


#endif

