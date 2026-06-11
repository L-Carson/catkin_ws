

#ifndef WEBSOCKET_SSL_API_H
#define WEBSOCKET_SSL_API_H

#include "simple_common.h"
#include <my_std_thread.h>

class WebsocketSslApi 
{
public:
    /**
     * 构造函数
     */
    WebsocketSslApi();
    /**
     * 析构函数
     */
    ~WebsocketSslApi();
    
    
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
     * @param[in] certification_file 将给定的证书文件发送到客户端
     * @param[in] private_key_file   指定包含证书文件私钥的文件
     * @param[in] verify_file        如果非空 则使用此证书颁发机构文件根据RFC 2818对客户端的证书和主机名执行验证
     * @param[in] func 接收数据回调函数 void func(const char*, unsigned int)
     * @return 
     */
    void CreateServer(int port, 
                      string url, 
                      DataCallBack func, 
                      string certification_file, 
                      string private_key_file, 
                      string verify_file = string());
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
     * @param[in] verify_certificate 设置为true（默认）以根据RFC 2818验证服务器的证书和主机名
     * @param[in] certification_file 如果非空 则将给定的证书文件发送到服务器 需要私钥文件
     * @param[in] private_key_file   如果非空 则指定包含证书文件私钥的文件 需要认证文件 
     * @param[in] verify_file        如果非空 请使用此证书颁发机构文件执行验证 
     * @param[in] func 接收数据回调函数 void func(const char*, unsigned int)
     * @return 
     */
    void CreateClient(string url, 
                      unsigned int time, 
                      DataCallBack func,
                      bool verify_certificate = true, 
                      string certification_file = string(), 
                      string private_key_file = string(), 
                      string verify_file = string());
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
    void SaveWebsocketConnect(shared_ptr<SimpleWssServer::Connection> connection);
    void DeleteWebsocketConnect(shared_ptr<SimpleWssServer::Connection> connection);
    void SendData(shared_ptr<SimpleWssServer::Connection> connect, const char* data, int data_len);
    
    
    void SetClientConnect(shared_ptr<SimpleWssClient::Connection> con);
    shared_ptr<SimpleWssClient::Connection> GetClientConnect();
    
    SimpleWssServer* m_ServerHandle = NULL;
    SimpleWssClient* m_ClientHandle = NULL;
    
    unsigned int m_ReconnectTime = 1; 
    map <string, WebSocketSslConnectInfo> m_ConnectList;
    shared_ptr<SimpleWssClient::Connection> m_ClientConnection = NULL;
    
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
