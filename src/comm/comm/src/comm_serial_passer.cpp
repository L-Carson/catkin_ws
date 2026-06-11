
#include "comm/comm_serial_passer.h"

INT32 SERIAL_PASSER_C::Init(void)
{
    ST_LOG_INFO("Succ.");
    return 0;
}

void SERIAL_PASSER_C::RegPassIn(SERIAL_ADDR_ENUM enTgtAddr, const std::string &strDataTopic)
{
    SERIAL_PASS_IN_SENDER_INFO_STRU stSenderInfo;
    stSenderInfo.enTgtAddr  = enTgtAddr;
    stSenderInfo.pobjSender = new SERIAL_PASS_DATA_SENDER_C(nh_, strDataTopic);
    R_ASSERT(stSenderInfo.pobjSender != nullptr, NOTHING);

    vectPassInSenderInfo_.push_back(stSenderInfo);
    ST_LOG_INFO("Succ. TgtAddr = %u, DataTopic = %s.", enTgtAddr, strDataTopic.c_str());
}

void SERIAL_PASSER_C::RegPassOut(const std::string &strDataTopic)
{
    SERIAL_PASS_DATA_RECEIVER_C *pobjReceiver = new SERIAL_PASS_DATA_RECEIVER_C(nh_, strDataTopic, std::bind(&SERIAL_PASSER_C::RecvPassOutData, this, std::placeholders::_1, std::placeholders::_2));
    R_ASSERT(pobjReceiver != nullptr, NOTHING);

    vectPassOutDataReceiver_.push_back(pobjReceiver);
    ST_LOG_INFO("Succ. DataTopic = %s.", strDataTopic.c_str());
}

INT32 SERIAL_PASSER_C::Pass(SERIAL_ADDR_ENUM enTgtAddr, const UINT08 *pucMsg, UINT32 msgLen)
{
    SERIAL_PASS_DATA_SENDER_C *pobjSender = GetPassInDataSender(enTgtAddr);
    if (pobjSender == nullptr) {
        ST_LOG_ERR_SAMPLE(20 * 5, "Not Reg Sender Info. enTgtAddr=%u.", enTgtAddr);
        return -1;
    }

    pobjSender->Send(pucMsg, msgLen);
    return 0;
}

void SERIAL_PASSER_C::ClearPassInDataSenders(void)
{
    for (SERIAL_PASS_IN_SENDER_INFO_STRU &stSenderInfo : vectPassInSenderInfo_) {
        if (stSenderInfo.pobjSender != nullptr) {
            delete stSenderInfo.pobjSender;
            stSenderInfo.pobjSender = nullptr;
        }
    }

    vectPassInSenderInfo_.clear();
    ST_LOG_INFO("End.");
}

void SERIAL_PASSER_C::ClearPassOutDataReceivers(void)
{
    for (SERIAL_PASS_DATA_RECEIVER_C *pobjReceiver : vectPassOutDataReceiver_) {
        if (pobjReceiver != nullptr) {
            delete pobjReceiver;
            pobjReceiver = nullptr;
        }
    }

    vectPassOutDataReceiver_.clear();
    ST_LOG_INFO("End.");
}

void SERIAL_PASSER_C::RecvPassOutData(const UINT08 *pucMsg, UINT32 msgLen)
{
    INT32 rslt = objSerialSender_.SendPass(pucMsg, UINT16(msgLen));
    ST_LOG_ERR_SAMPLE_IF_FAIL(rslt, 100, "objSerialSender_.SendPass() Fail.");
}

SERIAL_PASS_DATA_SENDER_C *SERIAL_PASSER_C::GetPassInDataSender(SERIAL_ADDR_ENUM enTgtAddr)
{
    for (SERIAL_PASS_IN_SENDER_INFO_STRU &stSenderInfo : vectPassInSenderInfo_) {
        if (stSenderInfo.enTgtAddr == enTgtAddr) {
            R_ASSERT(stSenderInfo.pobjSender != nullptr, nullptr);

            return stSenderInfo.pobjSender;
        }
    }

    return nullptr;
}


