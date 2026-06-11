/**
 * @file     wss_server.h
 * @brief    针对websockets作二次封装
 * @details  针对websockets作二次封装
 * @author   华仔
 * @date     9/7/21
 */

#ifndef DEMO_WEBSOCKETSHELPER_H
#define DEMO_WEBSOCKETSHELPER_H


#include "libwebsockets.h"
#include "my_std_thread.h"
#include <vector>
#include <queue>
using namespace std;

#define CONST_SERVER_TYPE_CMD                     1//命令服务器
#define CONST_SERVER_TYPE_FILE                    2//文件服务器
#define CONST_SERVER_TYPE_IMG                     3//图片服务器

typedef struct WsClientRecvInfoSt_
{
    struct lws *client;//客户端
    unsigned char *data;
    int data_len;
}WsClientRecvInfoSt;

typedef struct WsClientSendInfoSt_
{
    struct lws *client;//客户端
    unsigned char *data;
    int data_len;
    //==========以下是为了打印报文日志===============================
    int type;//服务器类型，多个服务器的情况下能区分
    bool is_print;//只是为了打印,为了解决一些定时上报频繁打印
    std::string print_file_name;//打印的文件标识
}WsClientSendInfoSt;

typedef struct WsClientInfoSt_
{
    struct lws *client;//客户端
    long long last_recv_time;//最后一次收到客户端数据时间
}WsClientInfoSt;

class WssServerHandler
{
public:
    /**
     * 构造函数
     */
    WssServerHandler();
    /**
     * 析构函数
     */
    ~WssServerHandler();
    /**
     * 启动服务器
     * @param[in] port 服务器端口
     * @param[in] ca_file 公钥证书文件
     * @param[in] cert_file 认证证书文件
     * @param[in] key_file 私钥证书文件
     * @return true：成功，false：失败
     */
    bool Start(int port,char *ca_file=NULL,char*cert_file=NULL,char* key_file=NULL);
    /**
     * 得到当前连接上的客户端的数量
     * @return 客户端的数量
     */
    int GetNum();
    /**
     * 判断当前客户端是否在链接列表中
     * @param[in] wsi
     * @return
     */
    bool IsLinkClient(struct lws *wsi);
    /**
     * 得到客户端的数据，此函数调用完记得释放分配的内存
     * @param[out] buf 返回客户端的数据，调用完记得free(buf.data)
     * @return true：成功，false：失败
     */
    bool GetClientData(WsClientRecvInfoSt& buf);
    /**
     * 发送数据到客户端
     * @param[in] wsi 客户端句柄
     * @param[in] type 类型，只是为了打印日志用。1：全部打印  2：只打印前面N字节
     * @param[in] data 发送数据
     * @param[in] data_len 发送数据长度
     * @param[in] print 是否打印日志
     */
    void Send(struct lws *wsi,unsigned char* data,int data_len,int type=1,bool print= true,std::string print_name="");
    /**
     * 发送数据到所有客户端
     * @param[in] type 类型，只是为了打印日志用。1：全部打印  2：只打印前面N字节
     * @param[in] data 发送数据
     * @param[in] data_len 发送数据长度
     * @param[in] print 是否打印日志
     */
    void Send(unsigned char* data,int data_len,int type=1,bool print= true,std::string print_name="");
    /**
     * 查看发送队列中有多少数据未发送出去，一般用来预警
     * @param[in] wsi 客户端句柄
     * @return 未发送个数
     */
    int NotSendDataNum(struct lws *wsi);
private:
    int ws_port_;
    struct lws_context* server_context_;
    MyStdThread my_thread_;//线程操作类
    MyStdThread::Thread thread_run_id_;//不停回调
    vector<WsClientInfoSt> ws_client_;//客户端信息
    pthread_mutex_t ws_client__metux_;
    queue<WsClientRecvInfoSt> ws_client_recv_;
    pthread_mutex_t ws_client_recv__metux_;
    queue<WsClientSendInfoSt> ws_send_data_;
    pthread_mutex_t ws_send_data_metux_;
private:
    unsigned char *recv_buf_;
    int recv_len_;
    int recv_buf_len_ ;
public:
    /**
     * 友元函数，wss回调
     * @param wsi
     * @param reason
     * @param user
     * @param in
     * @param len
     * @return
     */
    friend int WssCallback( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );
    /**
     * 得到服务器端口
     * @return 端口号
     */
    int GetPort();
private:
    /**
     * 强制lws_service退出阻塞
     */
    void CancelService();
    /**
     * 销毁wss
     * @return true：成功，false：失败
     */
    bool Destory();
    /**
     * 线程循环，等待客户端连接等
     * @param arg
     */
    void WssServerWhile(void*arg);
    /**
     * 回调发送数据
     * @param[in] wsi 客户端句柄
     */
    void SendCallback(struct lws *wsi);
    /**
     * 收到客户端数据放入队列中
     * @param[in] wsi 客户端句柄
     * @param[in] data 数据
     * @param[in] data_len 数据长度
     */
    void ServerRecv(struct lws *wsi,unsigned char*data,int data_len);
    /**
     * 缓存wss协议中一帧帧数据
     * @param[in] wsi 客户端句柄
     * @param[in] in 数据
     * @param[in] len 数据长度
     */
    void ServerRecvDeal(struct lws *wsi,unsigned char*in,int len);
    /**
     * 增加一个客户端链接
     * @param[in] wsi 客户端句柄
     */
    void AddClient(struct lws *wsi);
    /**
     * 一个客户端断开
     * @param[in] wsi 客户端句柄
     */
    void DeleteClient(struct lws *wsi);
    /**
     * 更新最后一次收到某个客户端数据的时间
     * @param[in] wsi 客户端句柄
     */
    void UpdateClientLastTime(struct lws *wsi);
    /**
     * 超时主动关闭某个客户端连接
     * @param[in] t 单位秒
     */
    void ClientTimeOutClose(int t);
};

#endif //DEMO_WEBSOCKETSHELPER_H
