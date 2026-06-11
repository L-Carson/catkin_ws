#include "comm/comm_serial.h"
#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

/**************************************************************************************
功能描述: 串口仿真接口函数
修改记录:
**************************************************************************************/
struct SIMULATION_SERIAL_INFO_STRU
{
    const CHAR *pucLocDevName;
    const CHAR *pucPeerDevName;
    FILE       *pfOutput;
    FILE       *pfInput;
    int         inputFileSizeOrigin;

    void AdjustFileName(std::string &strFileName)
    {
        for (UINT32 loop = 0; loop < strFileName.size(); ++loop) {
            if (strFileName[loop] == '/') {
                strFileName[loop] = '_';
            }
        }
    }

    std::string GetOutputFilePath(void)
    {
        std::string strFileName = std::string(pucLocDevName) + "_to_" + std::string(pucPeerDevName) + ".serial";
        AdjustFileName(strFileName);

        FILE_MNG_C objFileMng;
        return objFileMng.GetUsrRootDir() + strFileName;
    }

    std::string GetInputFilePath(void)
    {
        std::string strFileName = std::string(pucPeerDevName) + "_to_" + std::string(pucLocDevName) + ".serial";
        AdjustFileName(strFileName);

        FILE_MNG_C objFileMng;
        return objFileMng.GetUsrRootDir() + strFileName;
    }
};

SIMULATION_SERIAL_INFO_STRU g_astSimulationSerialInfo[] = {
    {SIMULATION_NAV_HMI_SERIAL_DEV_NAME, SIMULATION_HMI_NAV_SERIAL_DEV_NAME, NULL, NULL, -1},
    {SIMULATION_HMI_NAV_SERIAL_DEV_NAME, SIMULATION_NAV_HMI_SERIAL_DEV_NAME, NULL, NULL, -1},
    {SIMULATION_NAV_DCU_SERIAL_DEV_NAME, SIMULATION_DCU_NAV_SERIAL_DEV_NAME, NULL, NULL, -1},
    {SIMULATION_DCU_NAV_SERIAL_DEV_NAME, SIMULATION_NAV_DCU_SERIAL_DEV_NAME, NULL, NULL, -1},
    {SIMULATION_DCU_PCB_SERIAL_DEV_NAME, SIMULATION_PCB_DCU_SERIAL_DEV_NAME, NULL, NULL, -1},
    {SIMULATION_PCB_DCU_SERIAL_DEV_NAME, SIMULATION_DCU_PCB_SERIAL_DEV_NAME, NULL, NULL, -1},
};

/**************************************************************************************
功能描述: 查找配对表索引
修改记录:
**************************************************************************************/
int simulationSerial_GetDevDes(const char *pcDevName)
{
    for (int loop  = 0; loop < (int)ARRAY_SIZE(g_astSimulationSerialInfo); ++loop) {
        const SIMULATION_SERIAL_INFO_STRU &stSerialInfo = g_astSimulationSerialInfo[loop];
        if (0 == strcmp(stSerialInfo.pucLocDevName, pcDevName)) {
            return loop;
        }
    }

    return -1;
}

SIMULATION_SERIAL_INFO_STRU *simulationSerial_GetInfo(int devDes)
{
    R_ASSERT_1((devDes >= 0) && (devDes <  (int)ARRAY_SIZE(g_astSimulationSerialInfo)), NULL, devDes);

    return &g_astSimulationSerialInfo[devDes];
}

int simulationSerial_GetFileSize(const std::string &strFilePath)
{
    FILE_OPRT_C objFileOprt;
    if (!objFileOprt.IsExist(strFilePath)) {
        return 0;
    }

    FILE *pf = fopen(strFilePath.c_str(), "rb");
    R_ASSERT(pf != NULL, -1);

    fseek(pf, 0L, SEEK_END);
    int fileSize = ftell(pf);
    fclose(pf);
    R_ASSERT(fileSize >= 0, -1);

    return fileSize;
}

/**************************************************************************************
功能描述: 仿真打开串口
          返回的配对表中的索引，也就是系统函数中的文件描述符
          定义与如下系统函数相同
          int open( const char * pathname, int oflags)
修改记录:
**************************************************************************************/
int SimulationSerial_Open(const char *pcDevName, int oflags, ...)
{
    R_ASSERT(pcDevName != NULL, -1);
    ST_LOG_INFO("Begin. Dev: %s.", pcDevName);

    int devDes = simulationSerial_GetDevDes(pcDevName);
    if (devDes < 0) {
        ST_LOG_FATAL("simulationSerial_GetDevDes(%s) Fail.", pcDevName);
        return -1;
    }

    SIMULATION_SERIAL_INFO_STRU *pstSerialInfo = simulationSerial_GetInfo(devDes);
    R_ASSERT_1(pstSerialInfo != NULL, -1, devDes);
    R_ASSERT_1(pstSerialInfo->pfOutput == NULL, -1, devDes);
    R_ASSERT_1(pstSerialInfo->pfInput == NULL, -1, devDes);
    R_ASSERT_1(pstSerialInfo->inputFileSizeOrigin < 0, -1, devDes);

    std::string strInputFilePath  = pstSerialInfo->GetInputFilePath();
    std::string strOutputFilePath = pstSerialInfo->GetOutputFilePath();

    pstSerialInfo->inputFileSizeOrigin = simulationSerial_GetFileSize(strInputFilePath);
    R_ASSERT_1(pstSerialInfo->inputFileSizeOrigin >= 0, -1, pstSerialInfo->inputFileSizeOrigin);

    pstSerialInfo->pfOutput = fopen(strOutputFilePath.c_str(), "wb");
    if (pstSerialInfo->pfOutput == NULL) {
        ST_LOG_ERR("fopen(%s) Fail.", strOutputFilePath.c_str());
        pstSerialInfo->inputFileSizeOrigin = -1;
        return -1;
    }

    ST_LOG_INFO("Succ. Dev: %s.", pcDevName);
    return devDes;
}

/**************************************************************************************
功能描述: 仿真串口操作
          定义与如下系统函数相同
          int fcntl(int fd, int cmd, long arg)
修改记录:
**************************************************************************************/
int SimulationSerial_Fcntl(int fd, int cmd, ...)
{
    return 0;
}

/**************************************************************************************
功能描述: 仿真串口操作
          定义与如下系统函数相同
          int tcgetattr(int fd, struct termios *termios_p)
修改记录:
**************************************************************************************/
int SimulationSerial_Tcgetattr(int fd, struct termios *termios_p)
{
    return 0;
}

/**************************************************************************************
功能描述: 仿真串口操作
          定义与如下系统函数相同
          int tcflush(int fd, int queue_selector)
修改记录:
**************************************************************************************/
int SimulationSerial_Tcflush(int fd, int queue_selector)
{
    return 0;
}

/**************************************************************************************
功能描述: 仿真串口操作
          定义与如下系统函数相同
          int tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
修改记录:
**************************************************************************************/
int SimulationSerial_Tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
{
    return 0;
}

/**************************************************************************************
功能描述: 仿真测试串口状态
          定义与如下系统函数相同
          int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
修改记录:
**************************************************************************************/
int SimulationSerial_ReadSelect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    /* 先查找到设备描述符和设备 */
    int devDes = -1;
    for (int loop = 0; loop < nfds; ++loop) {
        if (0 != FD_ISSET(loop, readfds)) {
            devDes = loop;
            break;
        }
    }

    R_ASSERT(devDes >= 0, -1);
    SIMULATION_SERIAL_INFO_STRU *pstSerialInfo = simulationSerial_GetInfo(devDes);
    R_ASSERT_1(pstSerialInfo != NULL, -1, devDes);
    R_ASSERT_1(pstSerialInfo->inputFileSizeOrigin >= 0, -1, devDes);

    /* 此处忽略秒。不会这么大时间 */
    /* 等待文件内容发生变更 */
    int waitTimeUs = 100;
    for (int waitTimeAll = 0; waitTimeAll < timeout->tv_usec; waitTimeAll += waitTimeUs) {
        if (pstSerialInfo->pfInput == NULL) {
            std::string strInputFilePath  = pstSerialInfo->GetInputFilePath();
            int inputFileSizeNow = simulationSerial_GetFileSize(strInputFilePath);
            R_ASSERT(inputFileSizeNow >= 0, -1);

            /* 如果当前文件大小和打开串口时文件大小不一样，说明存在输入数据，准备开始读取数据，并且从当前末尾开始读 */
            if (pstSerialInfo->inputFileSizeOrigin != inputFileSizeNow) {
                pstSerialInfo->pfInput = fopen(strInputFilePath.c_str(), "rb");
                R_ASSERT(pstSerialInfo->pfInput != NULL, -1);
                fseek(pstSerialInfo->pfInput, 0L, SEEK_END);
                ST_LOG_INFO("Open Input File Succ.");
            }
        }

        if (pstSerialInfo->pfInput != NULL) {
            int inputFileSizeRead = ftell(pstSerialInfo->pfInput);
            R_ASSERT(inputFileSizeRead >= 0, -1);
            fseek(pstSerialInfo->pfInput, 0L, SEEK_END);
            int inputFileSizeNow = ftell(pstSerialInfo->pfInput);
            R_ASSERT(inputFileSizeNow >= 0, -1);

            /* 有新数据输入 */
            if (inputFileSizeNow > inputFileSizeRead) {
                fseek(pstSerialInfo->pfInput, inputFileSizeRead, SEEK_SET);
                return 1;
            }
        }

        usleep(waitTimeUs);
    }

    /* 处理正常，但无数据可以读 */
    return 0;
}

/**************************************************************************************
功能描述: 仿真测试读串口数据
          定义与如下系统函数相同
          ssize_t read(int fd, void *buf, size_t count)
修改记录:
**************************************************************************************/
ssize_t SimulationSerial_Read(int devDes, void *buf, size_t count)
{
    SIMULATION_SERIAL_INFO_STRU *pstSerialInfo = simulationSerial_GetInfo(devDes);
    R_ASSERT_1(pstSerialInfo != NULL, -1, devDes);
    R_ASSERT_1(pstSerialInfo->pfInput != NULL, -1, devDes);

    int readDataLen = fread(buf, 1, count, pstSerialInfo->pfInput);
    R_ASSERT_1(readDataLen >= 0, -1, devDes);

    return readDataLen;
}

/**************************************************************************************
功能描述: 仿真写串口
          定义与如下系统函数相同
          size_t write(int fd,const void *buf,size_t nbytes)
修改记录:
**************************************************************************************/
ssize_t SimulationSerial_Write(int devDes, const void *buf, size_t nbytes)
{
    SIMULATION_SERIAL_INFO_STRU *pstSerialInfo = simulationSerial_GetInfo(devDes);
    R_ASSERT_1(pstSerialInfo != NULL, -1, devDes);
    R_ASSERT_1(pstSerialInfo->pfOutput != NULL, -1, devDes);

    UINT32 writeDataLen = fwrite(buf, 1, nbytes, pstSerialInfo->pfOutput);
    R_ASSERT_2(writeDataLen == nbytes, -1, writeDataLen, nbytes);

    fflush(pstSerialInfo->pfOutput);

    return writeDataLen;
}

/**************************************************************************************
功能描述: 仿真关闭串口
          定义与如下系统函数相同
          int close(int fd)
修改记录:
**************************************************************************************/
int SimulationSerial_Close(int devDes)
{
    SIMULATION_SERIAL_INFO_STRU *pstSerialInfo = simulationSerial_GetInfo(devDes);
    R_ASSERT_1(pstSerialInfo != NULL, -1, devDes);

    if (pstSerialInfo->pfOutput != NULL) {
        fsync(fileno(pstSerialInfo->pfOutput));
        fclose(pstSerialInfo->pfOutput);
        pstSerialInfo->pfOutput = NULL;
    }

    if (pstSerialInfo->pfInput != NULL) {
        fclose(pstSerialInfo->pfInput);
        pstSerialInfo->pfInput = NULL;
    }

    pstSerialInfo->inputFileSizeOrigin = -1;

    ST_LOG_INFO("Succ. DevDes=%d.", devDes);
    return 0;
}

const SERIAL_OPRT_STRU g_astSerialOprtTable[2] =
{
    {
        false,
        "Normal",
        open,
        fcntl,
        tcgetattr,
        tcflush,
        tcsetattr,
        select,
        read,
        write,
        close
    },
    {
        true,
        "Simulation",
        SimulationSerial_Open,
        SimulationSerial_Fcntl,
        SimulationSerial_Tcgetattr,
        SimulationSerial_Tcflush,
        SimulationSerial_Tcsetattr,
        SimulationSerial_ReadSelect,
        SimulationSerial_Read,
        SimulationSerial_Write,
        SimulationSerial_Close
    }
};

/**************************************************************************************
功能描述: 默认为正式接口
修改记录:
**************************************************************************************/
const SERIAL_OPRT_STRU *SERIAL_C::pstSerialOprt_ = &g_astSerialOprtTable[false];
atomic_int              SERIAL_C::instNum_{0};

/**************************************************************************************
功能描述: 设置为仿真接口，在系统初始化时调用
修改记录:
**************************************************************************************/
void SERIAL_C::SetSimulation(void)
{
    /* 不允许实例在运行的时候更改仿真模式 */
    if (instNum_ != 0) {
        ST_LOG_FATAL("Can Not Set Simulation When Existing Inst.");
        exit(0);
    }

    pstSerialOprt_ = &g_astSerialOprtTable[true];
    ST_LOG_INFO("Set Simulation Succ.");
}

/**************************************************************************************
功能描述: 当前是否仿真接口
修改记录:
**************************************************************************************/
BOOL SERIAL_C::IsSimulation(void)
{
    return pstSerialOprt_->isSimulation;
}

/**************************************************************************************
功能描述: 串口 判断是否可用
修改记录:
**************************************************************************************/
BOOL SERIAL_C::IsValid(void) const
{
    return (fileDes_ >= 0);
}

/**************************************************************************************
功能描述: 串口 打开
修改记录:
**************************************************************************************/
INT32 SERIAL_C::Open(const char *pcDevName, int speed)
{
    R_ASSERT(fileDes_ < 0, -1);

    fileDes_ = pstSerialOprt_->Open(pcDevName, O_RDWR|O_NOCTTY|O_NDELAY);
    if (fileDes_ < 0) {
        ST_LOG_ERR("Open Seial Port(%s) Fail.", pcDevName);
        return -1;
    }

    int rslt = pstSerialOprt_->Fcntl(fileDes_, F_SETFL, 0);
    if (rslt < 0) {
        ST_LOG_ERR("fcntl(fileDes_=%d) Fail.", rslt);
        return -1;
    }

    /* 8 databits, 1 stopbit, no parity */
    if (Set(fileDes_, speed, 8, 1, 'n') != 0) {
        ST_LOG_ERR("Set Port Attr Fail. Speed=%d.", speed);
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 串口 关闭
修改记录:
**************************************************************************************/
void SERIAL_C::Close(void)
{
    if (fileDes_ >= 0) {
        pstSerialOprt_->Close(fileDes_);
        fileDes_ = -1;
    }
}

/*******************************************************************************
 * Function: treadn
 * Identifier:  (Trace to: )
 * Description: continuiosly received nbytes data to buf until receive nbytes or timout
 *
 * Input:  fd: serial device   buf: recv buffer   nbytes:num of bytes desired to recv     timout:recv timeout value
 * Output: buf:recv buffer
 * Return: number of bytes we received
 * Others:
 * Log: 2015/10/21 Laizl create.
 *******************************************************************************/
INT32 SERIAL_C::ReadData(UINT08 *buf, UINT32 nbytes) const
{
    R_ASSERT(fileDes_ >= 0, -1);

    int nleft;
    int nread;

    nleft = int(nbytes);
    while (nleft > 0)
    {
        /*recv until timeout or recv SERIAL_MSG_MAX_LEN data*/
        //10000 usec (10ms) character interval. we define the frame interval is 100ms and the character interval in a frame   should be less than 5ms at 115200bps
        if ((nread = tread(fileDes_, buf, UINT32(nleft), timeout_)) < 0)
        {
             if (nleft == int(nbytes))
             {
                return(-1); /* error, return -1 */
             }
             else
             {
                break;      /* error, return amount read so far */
             }
        } else if (nread == 0)
        {
             break;          /* EOF */
        }

        //Comm_PrintStream(buf, nread);

        nleft -= nread;
        buf += nread;
    }

    return (int(nbytes) - nleft);      /* return >= 0 */
}

/*******************************************************************************
 * Function: serialPutData
 * Identifier:  (Trace to: )
 * Description: send len of bytes to serial port
 *
 * Input:  pbuf:data to send     len:bytes num to send
 * Output: none
 * Return: void
 * Others:
 * Log: 2015/10/21 Laizl create.
 *******************************************************************************/
void SERIAL_C::SendData(const UINT08 *pbuf, int len) const
{
    R_ASSERT(fileDes_ >= 0, NOTHING);
    R_ASSERT_1((pbuf != NULL) && (len != 0), NOTHING, len);

    const UINT08 *ptmp = pbuf;
    for (int i = 0; i < len; i++)
    {
        if (1 != pstSerialOprt_->Write(fileDes_, ptmp, 1)) {
            ST_LOG_ERR("No Send 1 Byte.");
        }
        ptmp++;
    }
}

/**************************************************************************************
功能描述: 串口 设置参数
          param  fd     serial device file description
          param  nBits value should be 7 or 8
          param  nStop value should be 1 or 2
          param  nEvent  value should be N,E,O,,S
修改记录:
**************************************************************************************/
int SERIAL_C::Set(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;

    if (pstSerialOprt_->TcGetAttr(fd, &oldtio) != 0) {
        ST_LOG_ERR("Get Attr Fail.");
        return -1;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;

        case 8:
            newtio.c_cflag |= CS8;
            break;

        default:
            break;
    }

    switch (nEvent)
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;

        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;

        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;

        default:
            break;
    }

    switch (nSpeed)
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;

        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;

        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;

        case 19200:
            cfsetispeed(&newtio, B19200);
            cfsetospeed(&newtio, B19200);
            break;

        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;

        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;

        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    if (nStop == 1) {
        newtio.c_cflag &= ~CSTOPB;
    } else if ( nStop == 2 ) {
        newtio.c_cflag |= CSTOPB;
    } else {
    }

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN]  = 0;

    pstSerialOprt_->TcFlush(fd, TCIFLUSH);
    if(pstSerialOprt_->TcSetAttr(fd,TCSANOW,&newtio) !=0 )
    {
        ST_LOG_ERR("Set Attr Fail.");
        return -1;
    }

    return 0;
}

/*******************************************************************************
 * Function: tread
 * Identifier:  (Trace to: )
 * Description: received nbytes data to buf, return if receive succeed or timout
 *
 * Input:  fd: serial device   buf: recv buffer   nbytes:num of bytes desired to recv     timout:recv timeout value
 * Output: buf:recv buffer
 * Return: number of bytes we received
 * Others: only receive one time
 * Log: 2015/10/21 lzl create.
 *******************************************************************************/
int SERIAL_C::tread(int fd, UINT08 *buf, size_t nbytes, unsigned int timout) const
{
    int   nfds;
    fd_set  readfds;
    struct timeval  tv;

    tv.tv_sec = 0;
    tv.tv_usec = timout;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    nfds = pstSerialOprt_->ReadSelect(fd+1, &readfds, NULL, NULL, &tv);
    if (nfds < 0) {
        return -1;
    } else if (nfds == 0) {
        return 0;
    } else {
        return int(pstSerialOprt_->Read(fd, buf, nbytes));
    }
}

