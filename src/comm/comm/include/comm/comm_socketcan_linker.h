#ifndef __COMM_SOCKETCAN_LINKER_H__
#define __COMM_SOCKETCAN_LINKER_H__

#include "comm/comm_socketcan.h"
#include "comm/comm_socketcan_passer.h"
#include "comm/comm_socketcan_sender.h"
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
#define SOCKETCAN_LINKER_SEND_INTERVAL_MAX             0.005

/**************************************************************************************
功能描述: 串口约定波特率
修改记录:
**************************************************************************************/
#define SOCKETCAN_LINKER_BANDRATE                      250000

/**************************************************************************************
功能描述: 串口接收消息统计
修改记录:
**************************************************************************************/
struct SOCKETCAN_LINKER_RECV_DBG_INFO_STRU {
    UINT32 readDataAbnCount;                        /* 读数据异常次数 */
    UINT32 discardDataLen;                          /* 丢弃数据的长度 */
    UINT32 selfAppMsgLen;                           /* 消息总长 */
    UINT32 pushFrameListCount;                      /* 压入队列的帧数 */
    UINT32 totalFrameListSize;                      /* 压入队列时队列长度累加值 */
    UINT32 maxRecvFrameListSize;                    /* 接收帧队列最大长度 */
    double lastReadCycleTime;                       /* 上一个读周期时间 */
    float  maxReadCycleLen;                         /* 最大读周期时长 */

    UINT32 frameCount;
    UINT32 frameCountOfCheckFail;
    UINT32 frameCountOfSelf;
    UINT32 frameCountOfSelfAck;
    UINT32 frameCountOfSelfApp;
    UINT32 frameCountOfSelfAppMsg;                  /* 应用消息数 */
    UINT32 frameCountOfSeltAppRepeat;               /* 应用重复帧数 */
    UINT32 frameCountOfSelfHandshake;               /* 握手帧数 */
    UINT32 frameCountOfPassByApp;                   /* 本地处理转发帧数量 */
    UINT32 frameCountOfPassByRos;                   /* 转发帧数量 */
    UINT32 frameCountOfPassAddrFalsify;             /* 转发帧地址伪造数量 */
    UINT32 frameCountOfPassSendingFail;             /* 转发发送失败数量 */

    SOCKETCAN_LINKER_RECV_DBG_INFO_STRU(void)
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
        frameCountOfPassByApp           = 0;
        frameCountOfPassByRos           = 0;
        frameCountOfPassAddrFalsify     = 0;
        frameCountOfPassSendingFail     = 0;
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
                                ".PassByApp=%u, .PassByRos=%u, .PassAddrFalsify=%u, .PassSendingFail=%u.",
                    readDataAbnCount, discardDataLen, selfAppMsgLen, checkFailFrameRate,
                    maxRecvFrameListSize, avgRecvFrameListSize,
                    maxReadCycleLen * 1000.0,
                    frameCount, frameCountOfCheckFail,
                    frameCountOfSelf, frameCountOfSelfAck, frameCountOfSelfApp, frameCountOfSelfAppMsg, frameCountOfSeltAppRepeat, frameCountOfSelfHandshake,
                    frameCountOfPassByApp, frameCountOfPassByRos, frameCountOfPassAddrFalsify, frameCountOfPassSendingFail);

        Clear();
    }
};

struct SOCKETCAN_LINKER_RECV_DATA_STRU
{
    UINT16 dataLen;
    UINT08 aucData[CAN_MSG_MAX_LEN * 2];
    double lastRecvDataTime;

    SOCKETCAN_LINKER_RECV_DATA_STRU(void)
    {
        dataLen          = 0;
        lastRecvDataTime = 0;
    }

    void ClrData(UINT16 clrDataLen)
    {
        R_ASSERT_DO(clrDataLen <= dataLen, dataLen = 0, NOTHING);

        UINT32 leftDataLen = dataLen - clrDataLen;
        if (leftDataLen > 0) {
            memmove(aucData, &aucData[clrDataLen], leftDataLen);
        }

        dataLen = leftDataLen;
    }
};

struct SOCKETCAN_LINKER_RECV_FRAME_STRU
{
    BOOL   isCheckSucc;
    UINT16 framelen;
    UINT08 aucFrameBuff[0];
};

using SOCKETCAN_LINKER_RECV_MSG_FUNC_C      = std::function<void(SERIAL_ADDR_ENUM, UINT08, const UINT08 *, UINT32)>;
using SOCKETCAN_LINKER_RECV_PASS_MSG_FUNC_C = std::function<void(const UINT08 *, UINT32)>;

/**************************************************************************************
功能描述: 消息代收，主要是给本地测试时使用
修改记录:
**************************************************************************************/
struct SOCKETCAN_LINKER_RECV_PASS_MSG_REG_STRU
{
    SERIAL_ADDR_ENUM                      enTgtAddr;
    SOCKETCAN_LINKER_RECV_PASS_MSG_FUNC_C objRecvMsgFunc;
};

/**************************************************************************************
功能描述: 串口管理
          注意，当前一个进程只能有一个该对象
修改记录:
**************************************************************************************/
class SOCKETCAN_LINKER_C
{
public:


    SOCKETCAN_LINKER_C(ros::NodeHandle &nh, SERIAL_ADDR_ENUM enLocAddr, SERIAL_ADDR_ENUM enPeerAddr)
                                            : nh_(nh),
                                              enLocAddr_(enLocAddr),
                                              enPeerAddr_(enPeerAddr),
                                              objSocketcan_(SERIAL_READ_TIMEOUT_LEN_US),
                                              objSender_(objSocketcan_, enLocAddr, enPeerAddr),
                                              objPasser_(nh, objSender_)
    {
        memset(aucAddrFrameNo_, 0xFF, sizeof(aucAddrFrameNo_));
    }

    ~SOCKETCAN_LINKER_C(void)
    {
        DeleteThread();
        objSocketcan_.Close();
    }

    INT32 Init(const std::string &strDevName, const SOCKETCAN_LINKER_RECV_MSG_FUNC_C &objRecvMsgFunc);
    void  RegRecvPassMsg(SERIAL_ADDR_ENUM enTgtAddr, const SOCKETCAN_LINKER_RECV_PASS_MSG_FUNC_C &objRecvMsgFunc);
    void  RegPassInMsg(SERIAL_ADDR_ENUM enTgtAddr, const std::string &strDataTopic);
    void  RegPassOutMsg(const std::string &strDataTopic);
    INT32 Send(SERIAL_ADDR_ENUM enTgtAddr, UINT08 msgType, const UINT08 *pucMsg, UINT32 msgLen);
    INT32 SendPassMsg(const UINT08 *pucData, UINT32 dataLen);
    BOOL  IsFault(void);

private:
    ros::NodeHandle                            &nh_;
    const SERIAL_ADDR_ENUM                      enLocAddr_;
    const SERIAL_ADDR_ENUM                      enPeerAddr_;

    SOCKETCAN_C                                 objSocketcan_;
    SOCKETCAN_SENDER_C                          objSender_;
    SOCKETCAN_PASSER_C                          objPasser_;
    SOCKETCAN_LINKER_RECV_MSG_FUNC_C            objProcMsgFunc_;
    list<SOCKETCAN_LINKER_RECV_PASS_MSG_REG_STRU>  listRecvPassMsgReg_;

    double                                      lastOutputDbgInfoTime_{0.0};
    SOCKETCAN_LINKER_RECV_DBG_INFO_STRU         stDbgInfo_;
    UINT08                                      aucAddrFrameNo_[SERIAL_ADDR_BUTT];

    std::thread                                *pobjReadDataThread_{nullptr};
    std::thread                                *pobjProcFrameThread_{nullptr};
    std::list<SOCKETCAN_LINKER_RECV_FRAME_STRU *>  listRecvFrame_;
    std::mutex                                  objRecvFrameListMutex_;
    std::atomic_bool                            isThreadWork_{false};

    void   StatisReadCycleLen(void);
    void   OutputDbgInfo(double curTime);

    const  SOCKETCAN_LINKER_RECV_PASS_MSG_REG_STRU *GetAppPassMsgReg(SERIAL_ADDR_ENUM enTgtAddr);
    BOOL   IsAppPassFrame(const SERIAL_FRAME_C &objFrame);

    BOOL   IsFrameRepeat(const SERIAL_FRAME_C &objFrame);
    void   ProcSelfAppFrame(const SERIAL_FRAME_C &objFrame);
    void   ProcSelfFrame(const SERIAL_FRAME_C &objFrame);
    void   ProcAppPassFrame(const SERIAL_FRAME_C &objFrame);
    void   ProcPassFrame(const SERIAL_FRAME_C &objFrame);
    void   ProcFrame(const SERIAL_FRAME_C &objFrame, BOOL isCheckSucc);

    BOOL   ReadData(SOCKETCAN_LINKER_RECV_DATA_STRU &stRecvDataInfo);
    UINT16 ProcData(const UINT08 *pucData, UINT16 dataLen);

    void   PushFrame(const SERIAL_FRAME_C &objFrame, BOOL isCheckSucc);
    const  SOCKETCAN_LINKER_RECV_FRAME_STRU *PopFrame(void);

    void   ReadDataTask(void);
    void   ProcFrameTask(void);

    INT32  CreateThread(void);
    void   DeleteThread(void);
};

#endif

