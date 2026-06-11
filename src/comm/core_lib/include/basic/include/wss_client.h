/**
 * @file     wss_client.h
 * @brief    针对websockets 客户端作二次封装
 * @details  针对websockets 客户端作二次封装
 * @author   华仔
 * @date     9/7/21
 */

#ifndef WEBSOCKETSCLIENTHELPER_H
#define WEBSOCKETSCLIENTHELPER_H

#include "libwebsockets.h"
#include "my_std_thread.h"
#include <iostream>
using namespace std;
#include <queue>





typedef struct WssClientRecvInfoSt_
{
    unsigned char *data;
    int data_len;
}WssClientRecvInfoSt;

typedef struct WssClientSendInfoSt_
{
    bool is_print;//只是为了打印,为了解决一些定时上报频繁打印
    unsigned char *data;
    int data_len;
}WssClientSendInfoSt;

class WssClient
{
public:
    /**
     * 构造函数
     */
    WssClient();
    /**
     * 析构函数
     */
    ~WssClient();
    /**
     * 启动客户端
     * @param[in] url 服务端提供的URL，例如ws://127.0.0.1:2099
     * @param[in] ca_file 公钥证书文件
     * @param[in] cert_file 认证证书文件
     * @param[in] key_file 私钥证书文件
     * @return true：成功，false:失败
     */
    bool Start(char* url,char *ca_file=NULL,char*cert_file=NULL,char* key_file=NULL);
    /**
     * 得到服务端的数据，此函数调用完记得释放分配的内存
     * @param[out] buf 服务端数据结构,调用完后free(buf.data)
     * @return true：成功，false:失败
     */
    bool WssGetQueueData(WssClientRecvInfoSt& buf);
    /**
     * 发送数据给服务端
     * @param data 发送的数据
     * @param data_len 发送数据的长度
     * @param print 是否打印日志
     * @return 0：成功，其他失败
     */
    int WssSendToServer(unsigned char* data,int data_len,bool print= true);
    /**
     * 客户端是否连接服务端
     * @return true：连接成功，false:连接失败失败
     */
    bool IsLink();
public:
    /**
     * wss回调函数
     * @param wsi
     * @param reason
     * @param user
     * @param in
     * @param len
     * @return
     */
    friend int clientProtocolMyCallback( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );

private:
    struct lws_context*                 client_context_=NULL;
    std::mutex                          client_context_mutex_;
    struct lws_protocols *              m_protocols_=NULL;
    MyStdThread                         my_thread_;//线程操作类
    MyStdThread::Thread                 thread_run_id_=NULL;//不停回调
    int                                 m_link_step_=0;
    std::mutex                          m_link_step_mutex_;//加锁
    std::string                         m_url_;
    std::string                         key_file_;
    std::string                         ca_file_;
    std::string                         cert_file_;
    queue<WssClientRecvInfoSt>          ws_client_recv_;
    std::mutex                          ws_client_recv__metux_;
    queue<WssClientSendInfoSt>          ws_send_data_;
    std::mutex                          ws_send_data_metux_;
    struct lws *                        m_wsi_=NULL;
    std::mutex                          m_wsi_mutex_;//加锁
    bool                                destory_flag_=false;///< 对象释放标志
    std::mutex                          destory_flag_mutex_;///< destory_flag_变量线程保护
private:
    unsigned char *recv_buf_=NULL;
    int recv_len_=0;
    int recv_buf_len_ =0;
private:
    struct lws_context* GetClientContext();
    void SetClientContext(struct lws_context* lws);
    /**
     * 创建客户端
     * @return 0：成功，其他失败
     */
    int CreatClient();
    /**
     * 销毁客户端
     * @return 0：成功，其他失败
     */
    int DestoryClient();
    /**
     * 等待客户端连接上服务端
     * @param arg
     */
    void WssClientHelperWhile(void*arg);
    /**
     * 取得当前设备的毫秒值(1970 到现在)
     * @return 毫秒
     */
    long long GetCurTimeMsLonglong();
    /**
     * 设置客户端链接状态
     * @param step 步骤
     */
    void SetLinkStep(int step);
    /**
     * 得到和服务端连接状态
     * @return
     */
    int GetLinkStep();
    /**
     * 设置客户端句柄
     * @param lws_client 客户端句柄
     */
    void SetClientLws(struct lws * lws_client);
    /**
     * 得到客户端句柄
     * @return 客户端句柄
     */
    struct lws * GetClientLws();
    /**
     * 得到URL
     * @return URL
     */
    std::string GetURL();
    /**
     * 把每一帧数据保存起来，收到最后一帧就是一包数据
     * @param wsi 客户端句柄
     * @param in 一帧数据
     * @param len 一帧数据长度
     */
    void WssRecvDeal(struct lws *wsi,unsigned char*in,int len);
    /**
     * 把完整一包数据放入队列中
     * @param wsi 客户端句柄
     * @param data 数据
     * @param data_len 数据长度
     */
    void WssSaveRecvQueue(struct lws *wsi,unsigned char*data,int data_len);
    /**
     * 回调发送数据
     * @param wsi 客户端句柄
     */
    void WssSendDataCallback(struct lws *wsi);
    /**
     * 一个客户端连接断开
     * @param wsi 客户端句柄
     */
    void WssClientClose(struct lws *wsi);
};



#endif //WEBSOCKETSCLIENTHELPER_H
