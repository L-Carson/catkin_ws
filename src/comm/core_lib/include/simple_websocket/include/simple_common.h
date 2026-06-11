

#ifndef COMMMON_H
#define COMMMON_H

#include "client_ws.hpp"
#include "server_ws.hpp"
#include "client_wss.hpp"
#include "server_wss.hpp"
#include <future>

using namespace std;

using SimpleWsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using SimpleWsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

using SimpleWssServer = SimpleWeb::SocketServer<SimpleWeb::WSS>;
using SimpleWssClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;

typedef std::function<void (const char*, unsigned int)> DataCallBack;

#include <future>

template <typename F, typename... Args>
auto SimpleRealAsync(F&& f, Args&&... args)-> std::future<typename std::result_of<F(Args...)>::type>
{
    using _Ret = typename std::result_of<F(Args...)>::type;
    auto _func = std::bind(std::forward<F>(f),std::forward<Args>(args)...);
    std::packaged_task<_Ret()> tsk(std::move(_func));
    auto _fut = tsk.get_future();
    std::thread thd(std::move(tsk));
    thd.detach();
    return _fut;
}


struct WebSocketConnectInfo
{
    shared_ptr<SimpleWsServer::Connection> connection;
    string path; 
    string key;
};
  
struct WebSocketSslConnectInfo
{
    shared_ptr<SimpleWssServer::Connection> connection;
    string path; 
    string key;
};










#endif 
