#ifndef __COMM_SERIAL_H__
#define __COMM_SERIAL_H__

#include "comm/comm_base.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <atomic>

/**************************************************************************************
功能描述: 仿真串口定义
修改记录:
**************************************************************************************/
#define SIMULATION_NAV_HMI_SERIAL_DEV_NAME      "/dev/ttyS1"           /* 本地测试，NAV侧模拟NAV-HMI的串口设备名，假的 */
#define SIMULATION_NAV_DCU_SERIAL_DEV_NAME      "/dev/ttyS3"           /* 本地测试，NAV侧模拟NAV-DCU的串口设备名，假的 */
#define SIMULATION_DCU_NAV_SERIAL_DEV_NAME      "/dev/ttyS100"         /* 本地测试，DCU侧模拟DCU-NAV的串口设备名，假的 */
#define SIMULATION_HMI_NAV_SERIAL_DEV_NAME      "/dev/ttyS101"         /* 本地测试，HMI侧模拟HMI-NAV的串口设备名，假的 */
#define SIMULATION_DCU_PCB_SERIAL_DEV_NAME      "/dev/ttyS102"         /* 本地测试，DCU侧模拟DCU-PCB的串口设备名，假的 */
#define SIMULATION_PCB_DCU_SERIAL_DEV_NAME      "/dev/ttyS103"         /* 本地测试，PCB侧模拟PCB-DCU的串口设备名，假的 */

/**************************************************************************************
功能描述: 出口读取超时时长，单位us
修改记录:
**************************************************************************************/
#define SERIAL_READ_TIMEOUT_LEN_US                     (1 * 1000)

/**************************************************************************************
功能描述: 串口接口函数表
修改记录:
**************************************************************************************/
struct SERIAL_OPRT_STRU
{
    const BOOL  isSimulation;
    const CHAR *pcState;

    int     (* Open)(const char *, int, ...);
    int     (* Fcntl)(int, int, ...);
    int     (* TcGetAttr)(int, struct termios *);
    int     (* TcFlush)(int, int);
    int     (* TcSetAttr)(int, int, const struct termios *);
    int     (* ReadSelect)(int, fd_set *, fd_set *, fd_set *, struct timeval *);
    ssize_t (* Read)(int, void *, size_t);
    ssize_t  (* Write)(int, const void *, size_t);
    int     (* Close)(int);
};

/**************************************************************************************
功能描述: 串口类
修改记录:
**************************************************************************************/
class SERIAL_C
{
public:
    SERIAL_C(unsigned int timeout = SERIAL_READ_TIMEOUT_LEN_US) : timeout_(timeout)
    {
        fileDes_ = -1;
        ++instNum_;
        ST_LOG_INFO("%s State.", pstSerialOprt_->pcState);
    }

    ~SERIAL_C(void)
    {
        Close();
        --instNum_;
    }

    static void  SetSimulation(void);
    static BOOL  IsSimulation(void);

    BOOL  IsValid(void) const;
    INT32 Open(const char *pcDevName, int speed);
    void  Close(void);
    INT32 ReadData(UINT08 *buf, UINT32 nbytes) const;
    void  SendData(const UINT08 *pbuf, int len) const;

private:
    static const SERIAL_OPRT_STRU *pstSerialOprt_;
    static atomic_int              instNum_;

    const unsigned int       timeout_;
    INT32                    fileDes_;


    int Set(int fd, int nSpeed, int nBits, char nEvent, int nStop);
    int tread(int fd, UINT08 *buf, size_t nbytes, unsigned int timout) const;
};

#endif

