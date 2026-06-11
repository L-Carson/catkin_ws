#ifndef __COMM_SOCKETCAN_H__
#define __COMM_SOCKETCAN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/netlink.h>
#include <errno.h>
#include "comm/comm_base.h"

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

#define CAN_FRAME_ID        0x533           // nav recv filter CAN帧ID
#define CAN_FRAME_SEND_ID   0x633           // nav send CAN帧ID


/**************************************************************************************
功能描述: 串口接口函数表
修改记录:
**************************************************************************************/
struct SOCKETCAN_OPRT_STRU
{
    const BOOL  isSimulation;
    const CHAR *pcState;

    int     (* Socket)(int, int, int);
    int     (* Ioctl)(int, unsigned long int, ...);
    int     (* Setsockopt)(int, int, int, const void *, socklen_t);
    int     (* Select)(int, fd_set *, fd_set *, fd_set *, struct timeval *);
    ssize_t (* Read)(int, void *, size_t);
    ssize_t (* Write)(int, const void *, size_t);
    int     (* Close)(int);
};

int SimulationSocketCan_Socket(int domain, int type, int protocol);
int SimulationSocketCan_Ioctl(int fd, unsigned long int request, ...);
int SimulationSocketCan_Setsockopt(int devDes, int level, int optname, const void *optval, socklen_t optlen);
int SimulationSocketCan_Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
ssize_t SimulationSocketCan_Read(int devDes, void *buf, size_t count);
ssize_t SimulationSocketCan_Write(int devDes, const void *buf, size_t nbytes);
int SimulationSocketCan_Close(int devDes);

class SOCKETCAN_C
{
public:
    SOCKETCAN_C(unsigned int timeout = SERIAL_READ_TIMEOUT_LEN_US) : timeout_(timeout)
    {
        fileDes_ = -1;
        ++instNum_;
        ST_LOG_INFO("%s State.", pstSocketCanOprt_->pcState);
    }

    ~SOCKETCAN_C(void)
    {
        Close();
        --instNum_;
    }

    // static void  SetSimulation(void);
    // static BOOL  IsSimulation(void);

    BOOL  IsValid(void) const;
    INT32 Open(const char *pcDevName, int bitrate);
    void  Close(void);    
    INT32 Read(struct can_frame *frame, UINT32 nbytes, int timeout_ms) const;
    INT32 ReadData(UINT08 *buf, UINT32 nbytes) const;
    INT32 SendData(const UINT08 *pbuf, int len) const;
public:
    void SetSimulation(void);
    BOOL IsSimulation(void);


private:
    const unsigned int       timeout_;
    INT32                    fileDes_;

    const SOCKETCAN_OPRT_STRU g_astSocketCanOprtTable[2] =
    {
        {
            false,
            "Normal",
            socket,
            ioctl,
            setsockopt,
            select,
            read,
            write,
            close
        },
        {
            true,
            "Simulation",
            SimulationSocketCan_Socket,
            SimulationSocketCan_Ioctl,
            SimulationSocketCan_Setsockopt,
            SimulationSocketCan_Select,
            SimulationSocketCan_Read,
            SimulationSocketCan_Write,
            SimulationSocketCan_Close
        }
    };

    const SOCKETCAN_OPRT_STRU *pstSocketCanOprt_ = &g_astSocketCanOprtTable[false];
    atomic_int              instNum_{0};


    // int Set(int fd, int nSpeed, int nBits, char nEvent, int nStop);
    // int tread(int fd, UINT08 *buf, size_t nbytes, unsigned int timout) const;
};

#endif

