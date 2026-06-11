#ifndef __COMM_SERIAL_LINKER_H__
#define __COMM_SERIAL_LINKER_H__

#include "comm/comm_serial.h"
#include "comm/comm_serial_passer.h"
#include "comm/comm_serial_sender.h"
#include "comm/comm_serial_frame.h"
#include "comm/comm_base.h"

#include <string>
#include <thread>
#include <atomic>
#include <functional>

/**************************************************************************************
功能描述: 中断消息等待后续内容超时读取次数
          消息发送字节间时间间隔5ms
修改记录:
**************************************************************************************/
#define SERIAL_LINKER_SEND_INTERVAL_MAX             0.005

/**************************************************************************************
功能描述: 串口约定波特率
修改记录:
**************************************************************************************/
#define SERIAL_LINKER_BANDRATE                      115200

/**************************************************************************************
功能描述: 串口接收消息统计
修改记录:
**************************************************************************************/
struct SERIAL_LINKER_RECV_DBG_INFO_STRU {
    unsigned int readDataAbnCount;                        /* 读数据异常次数 */
    unsigned int discardDataLen;                          /* 丢弃数据的长度 */
    unsigned int selfAppMsgLen;                           /* 消息总长 */
    unsigned int pushFrameListCount;                      /* 压入队列的帧数 */
    unsigned int totalFrameListSize;                      /* 压入队列时队列长度累加值 */
    unsigned int maxRecvFrameListSize;                    /* 接收帧队列最大长度 */
    double lastReadCycleTime;                       /* 上一个读周期时间 */
    float  maxReadCycleLen;                         /* 最大读周期时长 */

    unsigned int frameCount;
    unsigned int frameCountOfCheckFail;
    unsigned int frameCountOfSelf;
    unsigned int frameCountOfSelfAck;
    unsigned int frameCountOfSelfApp;
    unsigned int frameCountOfSelfAppMsg;                  /* 应用消息数 */
    unsigned int frameCountOfSeltAppRepeat;               /* 应用重复帧数 */
    unsigned int frameCountOfSelfHandshake;               /* 握手帧数 */
    unsigned int frameCountOfTgtAddrErr;

    SERIAL_LINKER_RECV_DBG_INFO_STRU(void)
    {
        Clear();
    }

    void Clear()
    {
        readDataAbnCount                = 0;
        discardDataLen                  = 0;
        selfAppMsgLen                   = 0;
        pushFrameListCount              = 0;
        totalFrameListSize              = 0;
        maxRecvFrameListSize            = 0;
        lastReadCycleTime               = 0;
        maxReadCycleLen                 = 0;

        frameCount                      = 0;
        frameCountOfCheckFail           = 0;
        frameCountOfSelf                = 0;
        frameCountOfSelfAck             = 0;
        frameCountOfSelfApp             = 0;
        frameCountOfSelfAppMsg          = 0;
        frameCountOfSeltAppRepeat       = 0;
        frameCountOfSelfHandshake       = 0;
        frameCountOfTgtAddrErr          = 0;
    }

    void Output(void)
    {
        float checkFailFrameRate   = (frameCount == 0) ? 0.0 : (float(frameCountOfCheckFail) / float(frameCount));
        float avgRecvFrameListSize = (pushFrameListCount == 0) ? 0.0 : (float(totalFrameListSize) / float(pushFrameListCount));

        ST_LOG_INFO("[Statis].ReadDataAbnCount=%u, .DiscardDataLen=%u, .SelfAppMsgLen=%u, .CheckFailFrameRate=%f, "
                            ".maxRecvFrameListSize=%u, avgRecvFrameListSize=%.2f, "
                            ".maxReadCycleLen=%.2fms, "
                    "[frameCount].Total=%u, .CheckFail=%u, "
                                ".Self=%u, .SelfAck=%u, .SelfApp=%u, .SelfAppMsg=%u, .SeltAppRepeat=%u, .SelfHandshake=%u, "
                                ".TgtAddrErr=%u.",
                    readDataAbnCount, discardDataLen, selfAppMsgLen, checkFailFrameRate,
                    maxRecvFrameListSize, avgRecvFrameListSize,
                    maxReadCycleLen * 1000.0,
                    frameCount, frameCountOfCheckFail,
                    frameCountOfSelf, frameCountOfSelfAck, frameCountOfSelfApp, frameCountOfSelfAppMsg, frameCountOfSeltAppRepeat, frameCountOfSelfHandshake,
                    frameCountOfTgtAddrErr);

        Clear();
    }
};

struct SERIAL_LINKER_RECV_DATA_STRU
{
    unsigned short dataLen;
    unsigned char aucData[SERIAL_MSG_MAX_LEN * 2];     /* 为了能合并连续两次接收的消息，所以该最大长度翻倍 */;
    double lastRecvDataTime;

    SERIAL_LINKER_RECV_DATA_STRU(void)
    {
        dataLen          = 0;
        lastRecvDataTime = 0;
    }

    void ClrData(unsigned short clrDataLen)
    {
        R_ASSERT_DO(clrDataLen <= dataLen, dataLen = 0, NOTHING);

        unsigned int leftDataLen = dataLen - clrDataLen;
        if (leftDataLen > 0) {
            memmove(aucData, &aucData[clrDataLen], leftDataLen);
        }

        dataLen = leftDataLen;
    }
};

struct SERIAL_LINKER_RECV_FRAME_STRU
{
    bool   isCheckSucc;
    unsigned short framelen;
    unsigned char aucFrameBuff[0];
};

using SERIAL_LINKER_RECV_MSG_FUNC_C      = std::function<void(SERIAL_ADDR_ENUM, unsigned char, const unsigned char *, unsigned int)>;

/**************************************************************************************
功能描述: 串口管理
          注意，当前一个进程只能有一个该对象
修改记录:
**************************************************************************************/
class SERIAL_LINKER_C
{
public:


    SERIAL_LINKER_C(ros::NodeHandle &nh, SERIAL_ADDR_ENUM enLocAddr, SERIAL_ADDR_ENUM enPeerAddr)
                                            : nh_(nh),
                                              enLocAddr_(enLocAddr),
                                              enPeerAddr_(enPeerAddr),
                                              objSerial_(SERIAL_READ_TIMEOUT_LEN_US),
                                              objSender_(objSerial_, enLocAddr, enPeerAddr)
    {
    }

    ~SERIAL_LINKER_C(void)
    {
        DeleteThread();
        objSerial_.Close();
    }

    int Init(const std::string &strDevName, const SERIAL_LINKER_RECV_MSG_FUNC_C &objRecvMsgFunc);
    int Send(SERIAL_ADDR_ENUM enTgtAddr, unsigned char msgType, const unsigned char *pucMsg, unsigned int msgLen);
    int SendPassMsg(const unsigned char *pucData, unsigned int dataLen);
    bool  IsFault(void);

private:
    ros::NodeHandle                            &nh_;
    const SERIAL_ADDR_ENUM                      enLocAddr_;
    const SERIAL_ADDR_ENUM                      enPeerAddr_;

    SERIAL_C                                    objSerial_;
    SERIAL_SENDER_C                             objSender_;
    SERIAL_LINKER_RECV_MSG_FUNC_C               objProcMsgFunc_;

    double                                      lastOutputDbgInfoTime_{0.0};
    SERIAL_LINKER_RECV_DBG_INFO_STRU            stDbgInfo_;
    unsigned char                               lastRecvFrameNo_{0xFF};

    std::thread                                *pobjReadDataThread_{nullptr};
    std::thread                                *pobjProcFrameThread_{nullptr};
    std::list<SERIAL_LINKER_RECV_FRAME_STRU *>  listRecvFrame_;
    std::mutex                                  objRecvFrameListMutex_;
    std::atomic_bool                            isThreadWork_{false};

    void   StatisReadCycleLen(void);
    void   OutputDbgInfo(double curTime);

    bool   IsFrameRepeat(const SERIAL_FRAME_C &objFrame);
    void   ProcSelfAppFrame(const SERIAL_FRAME_C &objFrame);
    void   ProcSelfFrame(const SERIAL_FRAME_C &objFrame);
    void   ProcFrame(const SERIAL_FRAME_C &objFrame, bool isCheckSucc);

    bool   ReadData(SERIAL_LINKER_RECV_DATA_STRU &stRecvDataInfo);
    unsigned short ProcData(const unsigned char *pucData, unsigned short dataLen);

    void   PushFrame(const SERIAL_FRAME_C &objFrame, bool isCheckSucc);
    const SERIAL_LINKER_RECV_FRAME_STRU *PopFrame(void);

    void   ReadDataTask(void);
    void   ProcFrameTask(void);

    int    CreateThread(void);
    void   DeleteThread(void);
};

#endif

