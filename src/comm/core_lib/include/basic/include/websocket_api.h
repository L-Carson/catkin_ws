

#ifndef WEBSOCKET_API_H
#define WEBSOCKET_API_H

#include "simple_common.h"
#include <my_std_thread.h>

class WebsocketApi 
{
public:
    /**
     * 构造函数
     */
    WebsocketApi();
    /**
     * 析构函数
     */
    ~WebsocketApi();
    /**
     * 设置发送格式
     * @param[in] fin_rsv_opcode 129=txt 130=binary
     * @return 
     */
    void SetFinRsvOpcode(unsigned char fin_rsv_opcode);
    
    
    /**
     * 启动服务端
     * @param[in] port 服务器端口
     * @param[in] url 正则表达式
     * @param[in] func 接收数据回调函数 void func(const char*, unsigned int)
     * @return 
     */
    void CreateServer(int port, string url, DataCallBack func);
    /**
     * 发送给客户端数据
     * @param[in] data 数据
     * @param[in] data_len 数据长度
     * @return 
     */
    void SendClientData(const char* data, int data_len);
    /**
     * 获取连接的客户端数量
     * @return 连接的客户端数量
     */
    int GetClientNum();
    /**
     * 启动客户端
     * @param[in] url 正则表达式
     * @param[in] time 重连时间单位秒
     * @param[in] func 接收数据回调函数 void func(const char*, unsigned int)
     * @return 
     */
    void CreateClient(string url, unsigned int time, DataCallBack func);
    /**
     * 发送给服务端数据
     * @param[in] data 数据
     * @param[in] data_len 数据长度
     * @return 
     */
    void SendServerData(const char* data, int data_len);
    /**
     * 获取客户端连接状态
     * @return 客户端连接状态
     */
    bool GetConnectStatus();
private:
    void SaveWebsocketConnect(shared_ptr<SimpleWsServer::Connection> connection);
    void DeleteWebsocketConnect(shared_ptr<SimpleWsServer::Connection> connection);
    void SendData(shared_ptr<SimpleWsServer::Connection> connect, const char* data, int data_len);
    
    void SetClientConnect(shared_ptr<SimpleWsClient::Connection> con);
    shared_ptr<SimpleWsClient::Connection> GetClientConnect();
    SimpleWsServer* m_ServerHandle = NULL;
    SimpleWsClient* m_ClientHandle = NULL;
    
    unsigned int m_ReconnectTime = 1; 
    map <string, WebSocketConnectInfo> m_ConnectList;
    shared_ptr<SimpleWsClient::Connection> m_ClientConnection = NULL;
    
    DataCallBack m_ServerDataCallBack;
    DataCallBack m_ClientDataCallBack;
    
    std::mutex m_ServerMutex;
    std::mutex m_ClientMutex;
    
    unsigned long m_PingTime = 0; 
    unsigned char m_FinRsvOpcode = 130;
    
    MyStdThread m_ThreadStartServer;
    MyStdThread m_ThreadPingServer;
    MyStdThread m_ThreadStartClient;
    MyStdThread m_ThreadStopClient;
    
    
    void ThreadStartServer(void *arg);
    void ThreadPingServer(void *arg);
    void ThreadStartClient(void *arg);
    void ThreadStopClient(void *arg);
};



















#endif 
