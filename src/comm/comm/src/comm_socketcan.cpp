#include "comm/comm_socketcan.h"

// 判断是否可用
BOOL SOCKETCAN_C::IsValid(void) const
{
    return (fileDes_ >= 0);
}

/**
 * 打开并初始化 CAN 设备
 * @pcDevName: CAN 设备名（如 "can0"）
 * @bitrate: 比特率（500000/250000/1000000）
 * @return: CAN 套接字描述符（失败返回-1）
 */
int SOCKETCAN_C::Open(const char *pcDevName, int bitrate)
{
    // 1. 创建 CAN 原始套接字
    fileDes_ = pstSocketCanOprt_->Socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (fileDes_ < 0)
    {
        ST_LOG_ERR("socket create failed !");
        perror("socket create failed");
        return -1;
    }

    // 2. 指定 CAN 设备（绑定到 can0/can1）
    struct ifreq ifr;
    strncpy(ifr.ifr_name, pcDevName, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    if (pstSocketCanOprt_->Ioctl(fileDes_, SIOCGIFINDEX, &ifr) < 0)
    {
        ST_LOG_ERR("ioctl SIOCGIFINDEX failed !");
        perror("ioctl SIOCGIFINDEX failed");
        close(fileDes_);
        return -1;
    }

    // 3. 绑定套接字到 CAN 设备
    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(fileDes_, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        ST_LOG_ERR("bind failed !");
        perror("bind failed");
        close(fileDes_);
        return -1;
    }
    // 4. 设置过滤规则：只接收 CAN ID = 0x123 的帧
    struct can_filter rfilter;
    rfilter.can_id = CAN_FRAME_ID;   // 目标 ID（十六进制）
    rfilter.can_mask = CAN_SFF_MASK; // 标准帧掩码（11位，0x7FF）
    // 应用过滤规则（1 表示只设置 1 条规则）
    pstSocketCanOprt_->Setsockopt(fileDes_, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    // 设置非阻塞模式，避免recv阻塞
    int flags = fcntl(fileDes_, F_GETFL, 0);
    fcntl(fileDes_, F_SETFL, flags | O_NONBLOCK);

    // 4. 可选：通过代码配置比特率（替代 ip link 命令）
    // 注：部分系统需要 root 权限，推荐先用命令配置
    // struct can_bitrate_config bc;
    // bc.bitrate = bitrate;
    // if (ioctl(fileDes_, SIOCSCANBITRATE, &bc) < 0)
    // {
    //     fprintf(stderr, "警告：比特率配置失败（建议用 ip link 命令配置）\n");
    // }

    // 5. 禁用回环（不接收自己发送的帧）
    int loopback = 0;
    if (pstSocketCanOprt_->Setsockopt(fileDes_, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback)) < 0)
    {
        ST_LOG_ERR("setsockopt loopback failed !");
        perror("setsockopt loopback failed");
    }

    return 0;
}

/**
 * 发送 CAN 帧
 * @pbuf: 数据内容（最多8字节）
 * @len: 数据长度（0~8）
 * @return: 发送字节数（失败返回-1）
 */
INT32 SOCKETCAN_C::SendData(const UINT08 *pbuf, int len) const
{
    if (pbuf == nullptr || len == 0)
    {
        std::cerr << "无效的发送参数" << std::endl;
        ST_LOG_ERR("无效的发送参数 !");
        return -1;
    }
    // 计算总帧数（向上取整）
    int total_frames = (len + 8 - 1) / 8;
    for (int frame_idx = 0; frame_idx < total_frames; frame_idx++)
    {
        struct can_frame frame;
        frame.can_id = CAN_FRAME_SEND_ID;
        int start = frame_idx * 8;
        int copy_len = std::min(8, len - start);
        // 设置数据长度和内容
        frame.can_dlc = copy_len;
        memset(frame.data, 0, 8); // 先清0
        memcpy(frame.data, pbuf + start, copy_len);
        // 发送帧
        int ret = pstSocketCanOprt_->Write(fileDes_, &frame, sizeof(frame));
        if (ret != sizeof(frame))
        {
            ST_LOG_ERR("CAN帧发送失败 !");
            perror("CAN帧发送失败");
            return -1;
        }
        // 控制发送速率，避免总线过载（可选）
        usleep(10000); // 间隔10ms
    }
    return 0;
}

INT32 SOCKETCAN_C::ReadData(UINT08 *buf, UINT32 nbytes) const
{
    R_ASSERT(fileDes_ >= 0, -1);
    int nleft = int(nbytes);
    struct can_frame recv_frame;
    // printf("readData start [%d]\n", nleft);
    while (nleft > 0)
    {
        /*recv until timeout or recv SERIAL_MSG_MAX_LEN data*/
        int nread = Read(&recv_frame, UINT32(nleft), timeout_);
        if (nread == 1)
        {
            if (recv_frame.can_id == CAN_FRAME_ID)
            {
                if (nleft == int(nbytes) && recv_frame.data[0] != 0x55)
                {
                    continue;
                }
                if (recv_frame.data[0] == 0x55 && recv_frame.data[1] == 0xAA)
                {
                    nbytes = recv_frame.data[2] + 0x0B;
                    nleft = int(nbytes);                    
                }
                memcpy(buf, recv_frame.data, recv_frame.can_dlc);
                nleft -= recv_frame.can_dlc;
                buf += recv_frame.can_dlc;
            }
        }
        else if (nread == 0)
        {
            // 超时，打印心跳
            printf("nread = 0\n");
            break;
        }
        else
        {
            printf("nread = [%d]\n", nread);
            if (nleft == int(nbytes))
            {
                return (-1); /* error, return -1 */
            }
            break; /* error, return amount read so far */
        }
    }
    // printf("readData end [%u] [%d]\n", nbytes, nleft);
    return (int(nbytes) - nleft); /* return >= 0 */
}

/**
 * 接收 CAN 帧
 * @s: CAN 套接字描述符
 * @frame: 接收帧的缓冲区（输出参数）
 * @timeout_ms: 超时时间（毫秒，-1=永久阻塞）
 * @return: 1=接收成功，0=超时，-1=失败
 */
INT32 SOCKETCAN_C::Read(struct can_frame *frame, UINT32 nbytes, int timeout_ms) const
{
    // 用 select 实现超时监听（复用前文 ReadSelect 逻辑）
    fd_set read_fds;
    struct timeval tv;
    FD_ZERO(&read_fds);
    FD_SET(fileDes_, &read_fds);
    if (timeout_ms >= 0)
    {
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
    }

    int ret = pstSocketCanOprt_->Select(fileDes_ + 1, &read_fds, NULL, NULL, (timeout_ms >= 0) ? &tv : NULL);
    if (ret < 0)
    {
        perror("select failed");
        return -1;
    }
    else if (ret == 0)
    {
        return 0; // 超时
    }

    // // 检查套接字是否就绪，读取 CAN 帧
    if (FD_ISSET(fileDes_, &read_fds))
    {
        ssize_t n = pstSocketCanOprt_->Read(fileDes_, frame, sizeof(struct can_frame));
        if (n != sizeof(struct can_frame))
        {
            perror("can recv failed");
            return -1;
        }

        return 1; // 接收成功
    }

    return 0;
}

/**
 * 关闭 CAN 设备
 * @s: CAN 套接字描述符
 */
void SOCKETCAN_C::Close(void)
{
    pstSocketCanOprt_->Close(fileDes_);
}

// 设置为仿真接口，在系统初始化时调用
void SOCKETCAN_C::SetSimulation(void)
{
    /* 不允许实例在运行的时候更改仿真模式 */
    if (instNum_ != 0)
    {
        ST_LOG_FATAL("Can Not Set Simulation When Existing Inst.");
        exit(0);
    }

    pstSocketCanOprt_ = &g_astSocketCanOprtTable[true];
    ST_LOG_INFO("Set Simulation Succ.");
}

BOOL SOCKETCAN_C::IsSimulation(void)
{
    return pstSocketCanOprt_->isSimulation;
}

int SimulationSocketCan_Socket(int domain, int type, int protocol) { return 0; }
int SimulationSocketCan_Ioctl(int fd, unsigned long int request, ...) { return 0; }
int SimulationSocketCan_Setsockopt(int devDes, int level, int optname, const void *optval, socklen_t optlen) { return 0; }
int SimulationSocketCan_Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) { return 0; }
ssize_t SimulationSocketCan_Read(int devDes, void *buf, size_t count) { return 0; }
ssize_t SimulationSocketCan_Write(int devDes, const void *buf, size_t nbytes) { return 0; }
int SimulationSocketCan_Close(int devDes) { return 0; }
