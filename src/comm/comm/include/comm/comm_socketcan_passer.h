#ifndef __COMM_SOCKETCAN_PASSER_H__
#define __COMM_SOCKETCAN_PASSER_H__

#include "comm/comm_socketcan_sender.h"
#include "comm/comm_msg_serial_pass_data.h"
#include "comm/comm_base.h"
#include <string>
#include <vector>

struct SOCKETCAN_PASS_IN_SENDER_INFO_STRU
{
    SERIAL_ADDR_ENUM           enTgtAddr;
    SERIAL_PASS_DATA_SENDER_C *pobjSender;
};

/**************************************************************************************
功能描述: 串口转发
修改记录:
**************************************************************************************/
class SOCKETCAN_PASSER_C
{
public:
    SOCKETCAN_PASSER_C(ros::NodeHandle &nh, SOCKETCAN_SENDER_C &objSocketcanSender) : nh_(nh), objSocketcanSender_(objSocketcanSender)
    {
    }

    ~SOCKETCAN_PASSER_C(void)
    {
        ClearPassInDataSenders();
        ClearPassOutDataReceivers();
    }

    INT32 Init(void);
    void  RegPassIn(SERIAL_ADDR_ENUM enTgtAddr, const std::string &strDataTopic);
    void  RegPassOut(const std::string &strDataTopic);
    INT32 Pass(SERIAL_ADDR_ENUM enTgtAddr, const UINT08 *pucMsg, UINT32 msgLen);

private:
    ros::NodeHandle                              &nh_;
    SOCKETCAN_SENDER_C                              &objSocketcanSender_;
    std::vector<SOCKETCAN_PASS_IN_SENDER_INFO_STRU>  vectPassInSenderInfo_;
    std::vector<SERIAL_PASS_DATA_RECEIVER_C *>    vectPassOutDataReceiver_;

    void  ClearPassInDataSenders(void);
    void  ClearPassOutDataReceivers(void);

    void  RecvPassOutData(const UINT08 *pucMsg, UINT32 msgLen);

    SERIAL_PASS_DATA_SENDER_C *GetPassInDataSender(SERIAL_ADDR_ENUM enTgtAddr);
};


#endif

