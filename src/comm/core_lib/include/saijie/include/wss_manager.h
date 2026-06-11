/** 
 * @file     wss_manager.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     10/19/21 
 */

#ifndef WS_CLENT_DEMO_WS_CLIENT_H
#define WS_CLENT_DEMO_WS_CLIENT_H

#include "wss_client.h"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <json/value.h>
#include "my_std_thread.h"


class WssClientLib
{
public:
    /**
     * 构造函数
     */
    WssClientLib();
    /**
     * 析构函数
     */
    ~WssClientLib();
    /**
     * 启动客户端，并设置收到服务端的数据回调
     * @tparam T
     * @param fp
     * @param obj
     * @return
     */
    template <class T>
    int Start(void(T::*fp)(std::string&), T* obj,char* url=(char*)"ws://127.0.0.1:2030"){
        if(!fp || !obj){
            return -1;
        }
        std::unique_lock<std::mutex> lock(fun_mutex_);
        fun_ = boost::bind(fp,obj,_1);
        if(!WssStart(url)){
            return -2;
        }
        return 0;
    }
    /**
     * 发送数据到服务端
     * @param str 发送的字符串
     * @return 0：成功，其他失败
     */
    int Send(std::string str);
    /**
     * 发送数据到服务端
     * @param json 发送的json对象
     * @return 0：成功，其他失败
     */
    int Send(Json::Value json);
private:
    WssClient* client_=NULL;
    boost::function<void(std::string&)> fun_=NULL;
    std::mutex fun_mutex_;
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
    bool                                destory_flag_=false;///< 对象释放标志
    std::mutex                          destory_flag_mutex_;///< destory_flag_变量线程保护
private:
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg);
    /**
     * 启动wss客户端
     * @param url wss的URL地址
     * @return
     */
    bool WssStart(char* url);
    /**
     * 设置destory_flag_标志
     * @param flag
    */
    void SetDestoryFlag(bool flag);
    /**
     * 获取destory_flag_标志
     * @return
     */
    bool GetDestoryFlag();
};

#endif //WS_CLENT_DEMO_WS_CLIENT_H
