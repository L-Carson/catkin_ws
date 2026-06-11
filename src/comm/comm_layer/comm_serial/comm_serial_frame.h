#ifndef __COMM_SERIAL_FRAME_H__
#define __COMM_SERIAL_FRAME_H__

#include "comm/comm_base.h"

#pragma pack(1)

/**************************************************************************************
功能描述: 串口消息起始标志
修改记录:
**************************************************************************************/
#define SERIAL_MSG_BEGIN_MARK               0xAA55

/**************************************************************************************
功能描述: 地址
修改记录:
**************************************************************************************/
enum SERIAL_ADDR_ENUM {
    SERIAL_ADDR_BOARDCAST                   = 0x00, /* 广播，所有设备接收 */
    SERIAL_ADDR_NET_CTRL                    = 0x01, /* 调度系统 */
    SERIAL_ADDR_HMI                         = 0x02, /* 人机系统 */
    SERIAL_ADDR_NAV                         = 0x03, /* 导航控制系统 */
    SERIAL_ADDR_DCU                         = 0x04, /* DCU控制板 */
    SERIAL_ADDR_PCB                         = 0x05, /* 电源控制板 */
    SERIAL_ADDR_CHARGER_CTRL                = 0x06, /* 充电控制板 */
    SERIAL_ADDR_DOOR_CTRL                   = 0x07, /* 开关门控制板 */
    SERIAL_ADDR_VISION                      = 0x08, /* 视觉系统 */
    SERIAL_ADDR_BUTT,                               /* 无效地址 *//* 所有有效地址必须在此之前定义 */
};

/**************************************************************************************
功能描述: 帧类型
修改记录:
**************************************************************************************/
enum SERIAL_MSG_FRAME_TYPE_ENUM
{
    SERIAL_MSG_FRAME_TYPE_ACK_SUCC          = 0x00, /* 接收成功应答帧 */
    SERIAL_MSG_FRAME_TYPE_ACK_FAIL          = 0x01, /* 接收失败应答帧 */
    SERIAL_MSG_FRAME_TYPE_HANDSHAKE         = 0x02, /* 握手帧 */
    SERIAL_MSG_FRAME_TYPE_REQ               = 0x03, /* 原始请求帧 */
};

/**************************************************************************************
功能描述: 通信帧格式
          帧头   起始标志     2字节 0x55AA
                 数据段长度   2字节 0~512
          命令段 源地址       1字节
                 目标地址     1字节
                 帧类型       1字节
                 功能码       1字节
                 帧编号       1字节
          数据段 消息内容     n字节
          校验端 CRC16        2字节

          字节序采用小端，由于本地主机就是小端序，所以不用转换字节序。
修改记录:
**************************************************************************************/
struct SERIAL_MSG_HEAD_STRU {
    unsigned short beginMark;               /* 定义详见 SERIAL_MSG_BEGIN_MARK */
    unsigned short dataLen;
    unsigned char srcAddr;                 /* 定义详见 SERIAL_ADDR_ENUM */
    unsigned char tgtAddr;                 /* 定义详见 SERIAL_ADDR_ENUM */
    unsigned char frameType;               /* 定义详见 SERIAL_MSG_FRAME_TYPE_ENUM */
    unsigned char msgType;                 /* 定义详见 SERIAL_MSG_TYPE_ENUM */
    unsigned char frameNo;
};

/**************************************************************************************
功能描述: 帧尾
修改记录:
**************************************************************************************/
struct SERIAL_MSG_TAIL_STRU {
    unsigned short crc16;
};

/**************************************************************************************
功能描述: 消息相关长度
修改记录:
**************************************************************************************/
#define SERIAL_MSG_DATA_MAX_LEN     512
#define SERIAL_MSG_MIN_LEN          (sizeof(SERIAL_MSG_HEAD_STRU) + sizeof(SERIAL_MSG_TAIL_STRU))
#define SERIAL_MSG_MAX_LEN          (SERIAL_MSG_MIN_LEN + SERIAL_MSG_DATA_MAX_LEN)


#pragma pack()

/**************************************************************************************
功能描述: 帧数据缓存
修改记录:
**************************************************************************************/
struct SERIAL_FRAME_BUFF_A_STRU {
    unsigned short buffLen;
    unsigned char aucBuff[SERIAL_MSG_MAX_LEN];
};

struct SERIAL_FRAME_BUFF_P_STRU
{
    unsigned short  buffLen;
    unsigned char *pucBuff;
};

/**************************************************************************************
功能描述: 串口帧
修改记录:
**************************************************************************************/
class SERIAL_FRAME_C
{
public:
    SERIAL_FRAME_C(SERIAL_ADDR_ENUM enLocAddr) : enLocAddr_(enLocAddr)
    {
        pstHead_ = NULL;
    }

    int  EncodeReq(SERIAL_ADDR_ENUM enTgtAddr, unsigned char frameNo, unsigned char msgType, const unsigned char *pucMsg, unsigned short msgLen, SERIAL_FRAME_BUFF_A_STRU &stBuff);
    void   EncodeHandshake(SERIAL_ADDR_ENUM enTgtAddr, unsigned char frameNo, SERIAL_FRAME_BUFF_A_STRU &stBuff);
    void   EncodeAck(const SERIAL_FRAME_C &objRecvFrame, SERIAL_FRAME_BUFF_A_STRU &stBuff, bool isCheckSucc);
    int  Decode(const unsigned char *pucData, unsigned short dataLen);
    void   Clear(void);

    bool   IsFrame(void) const;
    bool   IsComplete(void) const;
    bool   IsCheckSucc(void) const;
    bool   IsAck(void) const;
    bool   IsAckSucc(void) const;
    bool   IsReq(void) const;
    bool   IsHandshake(void) const;
    bool   IsToLoc(void) const;
    bool   IsFromLoc(void) const;

    unsigned short GetFrameLen(void) const;
    const unsigned char *GetFrameHead(void) const;
    unsigned short GetDataLen(void) const;
    int  GetBuff(SERIAL_FRAME_BUFF_P_STRU &stBuff) const;
    int  GetBuff(SERIAL_FRAME_BUFF_A_STRU &stBuff) const;
    int  GetMsgBuff(SERIAL_FRAME_BUFF_P_STRU &stMsgBuff) const;
    SERIAL_ADDR_ENUM GetSrcAddr(void) const;
    SERIAL_ADDR_ENUM GetTgtAddr(void) const;
    unsigned char GetFrameNo(void) const;
    unsigned char GetMsgType(void) const;

private:
    const SERIAL_ADDR_ENUM enLocAddr_;
    const unsigned short           ausCcittTable_[256] = {  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
                                                    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
                                                    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
                                                    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
                                                    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
                                                    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
                                                    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
                                                    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
                                                    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
                                                    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
                                                    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
                                                    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
                                                    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
                                                    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
                                                    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
                                                    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
                                                    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
                                                    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
                                                    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
                                                    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
                                                    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
                                                    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
                                                    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
                                                    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
                                                    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
                                                    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
                                                    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
                                                    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
                                                    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
                                                    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
                                                    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
                                                    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};

    SERIAL_MSG_HEAD_STRU       *pstHead_;
    unsigned int                      dataLen_;

    int  Fill(SERIAL_ADDR_ENUM enSrcAddr, SERIAL_ADDR_ENUM enTgtAddr, SERIAL_MSG_FRAME_TYPE_ENUM enFrameType, unsigned char frameNo, unsigned char msgType, const unsigned char *pucMsg, unsigned short msgLen, SERIAL_FRAME_BUFF_A_STRU &stBuff);
    unsigned short GetCRC16(const unsigned char *pucData, unsigned short dataLen, unsigned short oldCRC16) const;

};


#endif

