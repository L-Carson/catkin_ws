
#include "comm/comm_serial_frame.h"

INT32 SERIAL_FRAME_C::EncodeReq(SERIAL_ADDR_ENUM enTgtAddr, UINT08 frameNo, UINT08 msgType, const UINT08 *pucMsg, UINT16 msgLen, SERIAL_FRAME_BUFF_A_STRU &stBuff)
{
    INT32 rsltOfFill = Fill(enLocAddr_, enTgtAddr, SERIAL_MSG_FRAME_TYPE_REQ, frameNo, msgType, pucMsg, msgLen, stBuff);
    R_ASSERT_1(rsltOfFill == 0, -1, msgLen);

    return 0;
}

void SERIAL_FRAME_C::EncodeHandshake(SERIAL_ADDR_ENUM enTgtAddr, UINT08 frameNo, SERIAL_FRAME_BUFF_A_STRU &stBuff)
{
    (void)Fill(enLocAddr_, enTgtAddr, SERIAL_MSG_FRAME_TYPE_HANDSHAKE, frameNo, 0xFF, NULL, 0, stBuff);
}

void SERIAL_FRAME_C::EncodeAck(const SERIAL_FRAME_C &objRecvFrame, SERIAL_FRAME_BUFF_A_STRU &stBuff, BOOL isCheckSucc)
{
    SERIAL_MSG_FRAME_TYPE_ENUM enFrameType = isCheckSucc ? SERIAL_MSG_FRAME_TYPE_ACK_SUCC : SERIAL_MSG_FRAME_TYPE_ACK_FAIL;

    (void)Fill(objRecvFrame.GetTgtAddr(), objRecvFrame.GetSrcAddr(), enFrameType, objRecvFrame.GetFrameNo(), objRecvFrame.GetMsgType(), NULL, 0, stBuff);
}

INT32 SERIAL_FRAME_C::Decode(const UINT08 *pucData, UINT16 dataLen)
{
    if ((pucData == NULL) || (dataLen < sizeof(*pstHead_))) {
        return -1;
    }

    pstHead_ = (SERIAL_MSG_HEAD_STRU *)pucData;
    dataLen_ = dataLen;
    return 0;
}

void SERIAL_FRAME_C::Clear(void)
{
    pstHead_ = NULL;
}

/**************************************************************************************
功能描述: 判断串口数据是否为数据帧
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsFrame(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return ((pstHead_->beginMark == SERIAL_MSG_BEGIN_MARK) && (pstHead_->dataLen <= SERIAL_MSG_DATA_MAX_LEN));
}

/**************************************************************************************
功能描述: 判断串口数据帧是否完整
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsComplete(void) const
{
    return (dataLen_ >= GetFrameLen());
}

BOOL SERIAL_FRAME_C::IsCheckSucc(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    UINT16                lenBeforeTail = UINT16(sizeof(*pstHead_) + pstHead_->dataLen);
    SERIAL_MSG_TAIL_STRU *pstTail       = (SERIAL_MSG_TAIL_STRU *)((UINT08 *)pstHead_ + lenBeforeTail);

    return (pstTail->crc16 == GetCRC16((UINT08 *)pstHead_, lenBeforeTail, 0));
}

/**************************************************************************************
功能描述: 判断是否ACK
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsAck(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return (   (pstHead_->frameType == SERIAL_MSG_FRAME_TYPE_ACK_SUCC)
            || (pstHead_->frameType == SERIAL_MSG_FRAME_TYPE_ACK_FAIL));
}

BOOL SERIAL_FRAME_C::IsAckSucc(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return (pstHead_->frameType == SERIAL_MSG_FRAME_TYPE_ACK_SUCC);
}

/**************************************************************************************
功能描述: 判断是否请求
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsReq(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return (pstHead_->frameType == SERIAL_MSG_FRAME_TYPE_REQ);
}

/**************************************************************************************
功能描述: 判断是否握手消息
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsHandshake(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return (pstHead_->frameType == SERIAL_MSG_FRAME_TYPE_HANDSHAKE);
}

/**************************************************************************************
功能描述: 判断是否转发
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsToLoc(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return (pstHead_->tgtAddr == enLocAddr_);
}

/**************************************************************************************
功能描述: 判断源地址是否为本地
修改记录:
**************************************************************************************/
BOOL SERIAL_FRAME_C::IsFromLoc(void) const
{
    R_ASSERT(pstHead_ != NULL, false);

    return (pstHead_->srcAddr == enLocAddr_);
}

/**************************************************************************************
功能描述: 获取完整帧长度
修改记录:
**************************************************************************************/
UINT16 SERIAL_FRAME_C::GetFrameLen(void) const
{
    R_ASSERT(pstHead_ != NULL, 0);

    return UINT16(sizeof(*pstHead_) + pstHead_->dataLen + sizeof(SERIAL_MSG_TAIL_STRU));
}

const UINT08 *SERIAL_FRAME_C::GetFrameHead(void) const
{
    return (const UINT08 *)pstHead_;
}

UINT16 SERIAL_FRAME_C::GetDataLen(void) const
{
    R_ASSERT(pstHead_ != NULL, 0);

    return pstHead_->dataLen;
}

INT32 SERIAL_FRAME_C::GetBuff(SERIAL_FRAME_BUFF_P_STRU &stBuff) const
{
    R_ASSERT(pstHead_ != NULL, -1);

    stBuff.buffLen = GetFrameLen();
    stBuff.pucBuff = (UINT08 *)pstHead_;

    return 0;
}

INT32 SERIAL_FRAME_C::GetBuff(SERIAL_FRAME_BUFF_A_STRU &stBuff) const
{
    R_ASSERT(pstHead_ != NULL, -1);

    stBuff.buffLen = GetFrameLen();
    R_ASSERT_1(stBuff.buffLen <= sizeof(stBuff.aucBuff), -1, stBuff.buffLen);

    if (stBuff.buffLen != 0) {
        memcpy(stBuff.aucBuff, pstHead_, stBuff.buffLen);
    }

    return 0;
}

INT32 SERIAL_FRAME_C::GetMsgBuff(SERIAL_FRAME_BUFF_P_STRU &stMsgBuff) const
{
    R_ASSERT(pstHead_ != NULL, -1);

    stMsgBuff.buffLen = pstHead_->dataLen;
    stMsgBuff.pucBuff = (UINT08 *)pstHead_ + sizeof(*pstHead_);

    return 0;
}

SERIAL_ADDR_ENUM SERIAL_FRAME_C::GetSrcAddr(void) const
{
    R_ASSERT(pstHead_ != NULL, SERIAL_ADDR_BUTT);

    return (SERIAL_ADDR_ENUM)pstHead_->srcAddr;
}

SERIAL_ADDR_ENUM SERIAL_FRAME_C::GetTgtAddr(void) const
{
    R_ASSERT(pstHead_ != NULL, SERIAL_ADDR_BUTT);

    return (SERIAL_ADDR_ENUM)pstHead_->tgtAddr;
}

UINT08 SERIAL_FRAME_C::GetFrameNo(void) const
{
    R_ASSERT(pstHead_ != NULL, UINT08_NULL);

    return pstHead_->frameNo;
}

UINT08 SERIAL_FRAME_C::GetMsgType(void) const
{
    R_ASSERT(pstHead_ != NULL, UINT08_NULL);

    return pstHead_->msgType;
}

INT32 SERIAL_FRAME_C::Fill(SERIAL_ADDR_ENUM enSrcAddr, SERIAL_ADDR_ENUM enTgtAddr, SERIAL_MSG_FRAME_TYPE_ENUM enFrameType, UINT08 frameNo, UINT08 msgType, const UINT08 *pucMsg, UINT16 msgLen, SERIAL_FRAME_BUFF_A_STRU &stBuff)
{
    R_ASSERT_1((msgLen == 0) || (pucMsg != NULL), -1, msgLen);

    UINT16 frameHeadAndDataLen = UINT16(sizeof(SERIAL_MSG_HEAD_STRU)) + msgLen;
    stBuff.buffLen = frameHeadAndDataLen + UINT16(sizeof(SERIAL_MSG_TAIL_STRU));
    R_ASSERT_1(stBuff.buffLen <= sizeof(stBuff), -1, msgLen);
    pstHead_ = (SERIAL_MSG_HEAD_STRU *)stBuff.aucBuff;

    pstHead_->beginMark = SERIAL_MSG_BEGIN_MARK;
    pstHead_->dataLen   = msgLen;
    pstHead_->srcAddr   = enSrcAddr;
    pstHead_->tgtAddr   = enTgtAddr;
    pstHead_->frameType = enFrameType;
    pstHead_->msgType   = msgType;
    pstHead_->frameNo   = frameNo;

    if (msgLen != 0) {
        memcpy(pstHead_ + 1, pucMsg, msgLen);
    }

    SERIAL_MSG_TAIL_STRU *pstTail = (SERIAL_MSG_TAIL_STRU *)((UINT08 *)pstHead_ + frameHeadAndDataLen);
    pstTail->crc16 = GetCRC16((UINT08 *)pstHead_, frameHeadAndDataLen, 0);

    return 0;
}

/**************************************************************************************
功能描述: 计算CRC16校验码
        * @param data          计算的数据
        * @param len           数据的长度
        * @param oldCRC16      上一个CRC16的值，用于循环计算大文件的CRC16。第一个数据的CRC16则传入0x0。
修改记录:
**************************************************************************************/
UINT16 SERIAL_FRAME_C::GetCRC16(const UINT08 *pucData, UINT16 dataLen, UINT16 oldCRC16) const
{
    UINT16 crc16 = oldCRC16;

    while (dataLen-- > 0) {
        crc16 = ausCcittTable_[(crc16 >> 8 ^ *pucData++) & 0xff] ^ (crc16 << 8);
    }

    return crc16;
}


