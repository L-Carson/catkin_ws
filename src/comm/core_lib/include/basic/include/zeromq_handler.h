/**
 * @file     zero_handler.h
 * @brief    针对zmq作二次封装
 * @details  针对zmq作二次封装,zeromq的模式有以下几种    参考：http://api.zeromq.org/master:zmq-socket
请求/应答模式：ZMQ_REQ、ZMQ_REP、ZMQ_DEALER、ZMQ_ROUTER   参考：https://www.freesion.com/article/3768915958/
			ZMQ_REQ、ZMQ_REP：请求-应答模式，REQ-REP必须同步的进行，即，REQ套接字必须先发送请求，然后等待应答；REP必须先接受请求，然后发送应答。
			一旦打乱这个顺序，将造成错误，套接字将无法继续工作。ZMQ_DEALER和ZMQ_ROUTER没有发送顺序之说
			ZMQ_DEALER、ZMQ_ROUTER这两种模式是对请求应答模式的扩展，一般的请求应答模式是同步的，服务端发送了数据，要等到客户端接收到数据后再发送回服务端才可以。
			而这个新模式是异步的，不需要等到客户端返回数据，就可以继续发送数据。
发布/订阅模式：ZMQ_PUB、ZMQ_SUB、ZMQ_XPUB、ZMQ_XSUB
			备注：PUB-SUB具有”slow joiner”症状。”slow joiner”的症状是：即使先启动订阅，稍等片刻再启动发布者，
			订阅者也可能错过发布者发送的第一条消息。建立TCP连接需要花费时间，具体取决于网络状况，以及主机到服务端的路由，
			所以即使多个订阅者同时启动，它们可能也不会收到同样的消息；
管道模式：ZMQ_PUSH、ZMQ_PULL
		这个模型里，管道是单向的，从 PUSH 端单向的向 PULL 端单向的推送数据流。
配对模式：ZMQ_PAIR
		独家对模式（Exclusive pair）用于将一个对等点精确地连接到另一个对等点。此模式用于跨inproc传输的线程间通信
本地模式：ZMQ_STREAM
		使用TCP://transport时，ZMQ_STREAM类型的套接字用于从非ØMQ对等方发送和接收TCP数据。ZMQ_流套接字可以充当客户端和/或服务器，异步发送和/或接收TCP数据。
		当接收到TCP数据时，ZMQ_流套接字应在将消息传递给应用程序之前，在消息前向消息预先添加包含发起对等方的路由id的消息部分。接收到的消息在所有连接的对等方之间公平排队。
		当发送TCP数据时，ZMQ_流套接字应删除消息的第一部分，并使用它来确定消息应路由到的对等方的路由id，不可导出的消息将导致EHOSTUNREACH或EAGAIN错误。
		要打开到服务器的连接，请使用zmq_connect调用，然后使用带有zmq_routing_id选项的zmq_getsockopt调用获取套接字路由id。
		若要关闭特定连接，请发送路由id帧，然后发送零长度消息（请参阅示例部分）。
		当建立连接时，应用程序将接收到长度为零的消息。类似地，当对等端断开连接（或连接丢失）时，应用程序将接收到长度为零的消息。
		必须先发送一个路由id帧，然后再发送一个数据帧。数据帧上的路由标志被忽略。
 * @author   华仔
 * @date     9/7/21
 */

#ifndef ZEROMQHANDLER_H_
#define ZEROMQHANDLER_H_


#include "zmq.h"


#define ZEROMQ_HANDLER_ZMQ_REQ			ZMQ_REQ
#define ZEROMQ_HANDLER_ZMQ_REP			ZMQ_REP
#define ZEROMQ_HANDLER_ZMQ_DEALER		ZMQ_DEALER
#define ZEROMQ_HANDLER_ZMQ_ROUTER		ZMQ_ROUTER
#define ZEROMQ_HANDLER_ZMQ_PUB			ZMQ_PUB
#define ZEROMQ_HANDLER_ZMQ_SUB			ZMQ_SUB
#define ZEROMQ_HANDLER_ZMQ_PUSH			ZMQ_PUSH
#define ZEROMQ_HANDLER_ZMQ_PULL			ZMQ_PULL


class ZmqHandler
{
public:
	/**
	 * 构造函数
	 */
	ZmqHandler();
	/**
	 * 析构函数
	 */
	~ZmqHandler();
	/**
	 * 初始化
	 * @param[in] in_type 类型，参考见上面定义的宏。
	 * @param[in] in_endpoint 端点。提供进程内、进程间、机器间、广播等四种通信协议。通信协议配置简单，用类似于URL形式的字符串指定即可，格式分别为inproc://、ipc://、tcp://、pgm://。ZeroMQ会自动根据指定的字符串解析出协议、地址、端口号等信息。
	 * @param[in] in_identity 标识
	 * @return
	 */
	bool Init(int in_type,const char*in_endpoint,const char *in_identity=NULL);
	/**
	 * 发送数据
	 * @param[in] in_buf 发送数据缓存
	 * @param[in] in_buf_len 发送数据长度
	 * @param[in] flags ZMQ_DONTWAIT非阻塞，当REQ没有连上REQ，发送阻塞
	 * @return true:成功  false：失败
	 */
	bool Send(unsigned char*in_buf,int in_buf_len,int flags=ZMQ_DONTWAIT);
	/**
	 * 发送数据,此接口需要初始化的时候传入标识
	 * @param[in] in_buf 发送数据缓存
	 * @param[in] in_buf_len 发送数据长度
	 * @param[in] in_dst 目标机
	 * @param[in] in_src 源机
	 * @return true:成功  false：失败
	 */
	bool Send(unsigned char*in_buf,int in_buf_len,const char *in_dst,const char* in_src);
	/**
	 * 接收数据
	 * @param[out] out_buf 接收数据存放的缓存
	 * @param[in] in_buf_len 预期希望接收数据长度
	 * @param[in] flags ZMQ_DONTWAIT非阻塞
	 * @return 收到数据长度
	 */
	int Recv(unsigned char*out_buf,int in_buf_len,int flags=ZMQ_DONTWAIT);
	/**
	 * 接收数据,此接口需要初始化的时候传入标识
	 * @param[out] out_buf 接收数据存放的缓存
	 * @param in_buf_len 预期希望接收数据长度
	 * @param out_src 源机
	 * @param out_dst 目标机
	 * @return 收到数据长度
	 */
	int Recv(unsigned char*out_buf,int in_buf_len,char* out_src,char *out_dst);
private:
	void* zeromq_ctx;
	void* zeromq_socket;
	int zeromq_type;
};


#endif /* ZEROMQHANDLER_H_ */
