/*
 * tcpServerHandlerApi.h
 *
 *  Created on: 2020年10月29日
 *      Author: ljh
 */

#ifndef TCP_SERVER_API_H_
#define TCP_SERVER_API_H_

#include <iostream>
#include <vector>
#include "my_std_thread.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace std;

#define CONST_DEBUG_SOCKET_TIME_OUT   0
#define CONST_DEBUG_SOCKET_BLOCK      1





typedef struct clientInfoApi
{
    string ip;
    unsigned short port;
    MyStdThread::Thread thread_id;
    int client_fd;
    MyStdThread::Mutex metux_w;

}clientInfoApiSt;

class TcpServerApi
{
public:
 
    /**
     * 构造函数
     * @param[in] ip ip地址
     * @param[in] port 端口
     * @param[in] maxLink 最大连接数
     */
    TcpServerApi(string ip, unsigned short port, int maxLink);
    /**
     * 析构函数
     */
    ~TcpServerApi();
    /**
     * 设置客户端超时时间
     * @param[in] t 超时时间
     * @return 
     */
     
    /**
     * 接受到客户端数据回调函数
     * @tparam T 模板
     * @param fp 回调函数指针
     * @param obj 类指针
     * @return 0成功
     */ 
    template<class T>
    void SetCallback(void(T::*fp)(int, unsigned char*, int), T* obj)
    {
        boost::function<void(int, unsigned char*, int)> fun;
        fun = boost::bind(fp,obj,_1,_2,_3);
        return SetRecvDataCallback(fun);
    }

    /**
     * 设置客户端数据超时时间
     * @param t 客户端超时时间，单位为秒。即t秒没有收到客户端数据，把客户端踢掉
     */
    void SetClientTimeout(int t);
    /**
     * 启动服务器
     * @param[in] timeOut 启动超时时间，单位ms.即timeOut毫秒内没有初始化成功，不断初始化
     * @return 
     */
    int Start(int timeOut);
    /**
     * 发送数据到所有连接的客户端
     * @param[in] data 数据首地址
     * @param[in] data_len 数据长度
     * @return 0成功，其他失败
     */
    int SendDataToAllClient(unsigned char* data, int data_len);
    /**
     * 发送数据给某个指定的客户端
     * @param[in] fd 描述符
     * @param[in] data 数据首地址
     * @param[in] data_len 数据长度
     * @return 0成功，其他失败
     */
    int SendDataToOneClient(int fd, unsigned char* data, int data_len);
    /**
     * 得到所有连接上的客户端个数
     * @return 个数
     */
    int GetClientSum();

private:
    void SetRecvDataCallback(boost::function<void(int, unsigned char*, int)> & fun);
    int CloseServerSocket();
    void TcpServerHandlerAccpet(void* arg);
    void ClientRecvData(void* arg);
    void AddOneClient(clientInfoApiSt c);
    void ReplaceClientThreadId(int fd, MyStdThread::Thread threadId);
    void DeleteOneClient(int fd, bool lock = true);
    bool IsOneClient(int fd);
    void DestoryAllClient();
    void* FindOneClientMutex(int fd);
    int SendDataToOneClient(int fd, void* mutex, unsigned char* data, int data_len, bool lockAllClient);
    bool SetSocketBufLen(int fd);
    long long GetCurTimeMS(void);
    int Start(void);


    int m_ClientMaxLink;                                                                    //客户端最大连接数
    string m_ServerIp;                                                                      //ip
    unsigned short m_ServerPort;                                                            //端口
    MyStdThread m_MyThread;                                                                 //线程操作类
    MyStdThread::Thread m_ThreadAcceptId;                                                   //accept线程对象 
    vector<clientInfoApiSt> m_AllClientInfo;
    MyStdThread::Mutex m_AllClientInfoMutex;                                               //保护all_client_info数据 
    int m_SocketFd;                                                                          //socket函数产生的fd
    int m_ClientTimeout;                                                                     //客户端超时时间

    boost::function<void(int, unsigned char*, int)> m_RecvDataFun;
};


#endif /* TCPSERVERHANDLERAPI_H_ */
