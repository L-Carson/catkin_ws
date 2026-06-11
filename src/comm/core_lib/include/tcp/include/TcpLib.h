

#ifndef TCP_LIB_H
#define TCP_LIB_H
#include <functional>


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
private:                     \
	TypeName(const TypeName&);\
	TypeName& operator=(const TypeName&)

#include<string>
using namespace std;


typedef enum 
{
    TCP_SERVER_ERR_CODE_OK = 0,             //成功
    TCP_SERVER_ERR_CODE_INIT_FAIL,          //服务器启动初始化失败
    TCP_SERVER_ERR_CODE_BIND_FAIL,          //服务器绑定端口失失败
    TCP_SERVER_ERR_CODE_LISTEN_FAIL,        //服务器监听失败
    TCP_SERVER_ERR_CODE_STARTED,            //服务器已经启动
    TCP_SERVER_ERR_CODE_STARTING            //服务器正在启动
}EnumTcpServerErrCode;


typedef enum 
{
    TCP_CLIENT_ERR_CODE_OK = 0,             //成功
    TCP_CLIENT_ERR_CODE_INIT_FAIL,          //客户端启动初始化失败
    TCP_CLIENT_ERR_CODE_CONNECTING,         //客户端正在连接中 
    TCP_CLIENT_ERR_CODE_CLOSING,            // 客户端正在关闭中
    TCP_CLIENT_ERR_CODE_SERVERADDR_ERR,     // 连接服务器地址有错
    TCP_CLIENT_ERR_CODE_CONNECT_FAIL,       //连接服务器失败
    TCP_CLIENT_ERR_CODE_CONNECTED,          //已经连接上服务器，重复连
    TCP_CLIENT_ERR_CODE_BINDIP_ERR,         //客户端绑定ip地址出错 
}EnumTcpClientErrCode;


class TcpConnection;
class TcpClient;

//内部实现类声明
class TcpConnectionImpl;
class TcpServerImpl;
class TcpClientImpl;

/*
//服务连接的回调函数定义, nStatus =0 成功 ,注意：应先判断status =0 才能使用conn，否则conn可能NULL(比如当客户端连接服务器不成功时)
typedef void  (*pConnectionCallback)(TcpConnection* conn,int nStatus,void* pUserData);

//读取数据的回调函数定义
typedef void  (*pMessageCallback)(TcpConnection* conn,
                                    void* message, 
                                    size_t len,
                                    void* pUserData);

//发送完成的回调函数定义 , nStatus =0 成功，其他失败
typedef void  (*pSendCompleteCallback)(TcpConnection* conn,int nStatus,void* pUserData);


//网络关闭后回调，注意：在回调以后不能再用相关的conn。这个回调发生在连接被动断开或者主动断开连接之后。
typedef void  (*pShutdownCallback)(TcpConnection* conn,void* pUserData);
*/
typedef  std::function<void (TcpConnection*, int ,void *)>  pConnectionCallback;
typedef  std::function<void (TcpConnection*, void* ,int , void *)>	pMessageCallback;
typedef  std::function<void (TcpConnection*, int ,void *)>  pSendCompleteCallback;
typedef  std::function<void (TcpConnection* ,void *)>  pShutdownCallback;


typedef struct InetAddress
{
    std::string ip;     //对端IP
    int  port;          //端口
}s_inet_adr_t;


//调用其他类之前必须调用该函数
bool InitNetLib();

//其他类析构完成后可调用该函数
void FreeNetLib();



//中间连接类，用户不需要自己实例化此类。一般在回调函数里面传给用户。
class TcpConnection
{
public:
    TcpConnection(void* loop);
    ~TcpConnection(void);

public:
    //获取本地ip地址和端口
    const InetAddress& LocalAddress();
    //获取对端ip地址和端口
    const InetAddress& PeerAddress();
    bool Connected();
    void Shutdown();
    //注意如果isNeedCopyData=false，则表示缓冲由外部提供，
    //message的内存至少要等到pSendCompleteCallback回调才能释放
    void Send(const void* message, size_t len,bool isNeedCopyData = true);
    void SetContext(void* context);
    void* GetContext() const;
    
    //*************************注意：下面的方法为内部实现调用，外部不需要调用这些方法**********************
    void* GetHandle() const;
    //*************************注意：上面的方法为内部实现调用，外部不需要调用这些方法**********************


private:
    TcpConnectionImpl  *m_pImpl;
    DISALLOW_COPY_AND_ASSIGN(TcpConnection);
};




//网络库的TCP服务器接口类
class TcpServer
{
public:
    TcpServer(const std::string &bindIp, int bindPort,const string& name);

    ~TcpServer(void);

    //返回设定的服务器名称
    const string& Name() const;

    //启动服务接口
    EnumTcpServerErrCode Start();

    void SetConnectionCallback(pConnectionCallback pConnectCB,void* pUserData);
    
    void SetMessageCallback(pMessageCallback pMessageCB,void* pUserData);

    void SetSendCompleteCallback(pSendCompleteCallback pSendCompleteCB,void* pUserData);

    void SetShutdownCallback(pShutdownCallback pShutdownCB,void* pUserData);

private:
    TcpServerImpl *m_pImpl;
   
    DISALLOW_COPY_AND_ASSIGN(TcpServer);
};


//网络库的TCP客户端接口类
class TcpClient
{
public:
    TcpClient(const std::string &serverIp, int serverPort);
    ~TcpClient(void);

public:
    //客户端连接接口
    EnumTcpClientErrCode Connect();
    //断开连接接口
    void Disconnect();
    //绑定客户端地址端口，不调用系统默认绑定
    bool BindAddr(const std::string &localIp, int localPort);
    void SetConnectionCallback(pConnectionCallback pConnectCB,void* pUserData);
    void SetMessageCallback(pMessageCallback pMessageCB,void* pUserData);
    void SetSendCompleteCallback(pSendCompleteCallback pSendCompleteCB,void* pUserData);
    void SetShutdownCallback(pShutdownCallback pShutdownCB,void* pUserData);


private:
    TcpClientImpl *m_pImpl;

    DISALLOW_COPY_AND_ASSIGN(TcpClient);

};

#endif 





