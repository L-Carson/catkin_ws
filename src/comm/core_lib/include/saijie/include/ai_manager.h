/** 
 * @file     ai_manager.h
 * @brief    大模型管理接口
 * @details  
 * @author   华仔
 * @date    
 */

#ifndef CORE_NODE_AI_MANAGER_H
#define CORE_NODE_AI_MANAGER_H

#include "basic_manager.h"
#include <mutex>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <ros/ros.h>
#include <std_msgs/String.h>


constexpr char kPubAiModuleTopic[]    = "/alg_to_core_ai";///< 算法模块 ---->core模块
constexpr char kSubAiModuleTopic[]    = "/core_to_alg_ai";///< core模块 ---->算法模块
//==================================周期性消息日志打印时长=====================================================================


/** 任务管理类*/
class BasicAiManager
{
public:
    /**
     * 创建一个BasicAiManager对象，如果存在则不重新创建，返回之前创建的对象
     * @return BasicAiManager对象指针
     */
    static BasicAiManager* Create();
    /**
     * 销毁一个实例对象，在进程结束前的时候调用，其他情况不调用
     */
    static void Destory();
    /**
     * 给人机过来的消息任务注册回调
     * @tparam T 类模板
     * @param[in] fp 回调函数指针
     * @param[in] obj 回调函数类指针
     */
    template <class T>
    int Reg(void(T::*fp)(JsonValueConstPtr&), T* obj){
        if(!fp || !obj){
            return -1;
        }
        boost::function<void(JsonValueConstPtr&)> fun;
        fun = boost::bind(fp,obj,_1);
        void *fp1=NULL;
        void *obj1=NULL;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
        fp1 = (void*)fp;
        obj1=(void*)obj;
#pragma GCC diagnostic pop
        return ReqList(fun,fp1,obj1);
    }
    /**
     * 取消任务注册回调
     * @tparam T
     * @param[in] fp 回调函数指针，和注册时一致
     * @param[in] obj 回调函数类指针，和注册时一致
     * @return 0：成功，其他失败
     */
    template <class T>
    int UnReq(void(T::*fp)(JsonValueConstPtr&), T* obj){
        void *fp1=NULL;
        void *obj1=NULL;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
        fp1 = (void*)fp;
        obj1=(void*)obj;
#pragma GCC diagnostic pop
        return UnReqList(fp1,obj1);
    }
    /**
     * 取消任务管理所有注册
     * @return 0：成功，其他失败
     */
    int UnReg();
    /**
     * 发送任务管理相关消息到人机
     * @param[in] json 发送的json数据参考 https://alidocs.dingtalk.com/i/nodes/3NwLYZXWynd9N3q3IjZoR2o7VkyEqBQm?iframeQuery=utm_source%3Dportal%26utm_medium%3Dportal_recent&rnd=0.3146938623960511
     *  @param[in] file_data 文件地址
     *  @param[in] file_len 文件长度
     * @return 0：成功，其他失败
     */
    int Send(Json::Value& json,unsigned char* file_data,int file_len);
private:
    /**
     * 构造函数
     */
    BasicAiManager();
    /**
     * 析构函数
     */
    ~BasicAiManager();
    /**
     * 拷贝构造函数
     * @param[in] signal
     */
    BasicAiManager(const BasicAiManager& signal);
    /**
     * 赋值函数
     * @param[in] signal
     * @return
     */
    const BasicAiManager& operator=(const BasicAiManager& signal);
private:
    static BasicAiManager* m_instance_;///< 对象指针
    static std::mutex m_instance_mutex_;///< 多线程安全
private:
    ros::NodeHandle                         nh_;
    ros::Publisher                          to_core_module_;///< 算法模块---->core模块
    ros::Subscriber                         from_core_module_;///< core模块---->算法模块
private:
    /**
     * ros消息回调，core到算法的json消息
     * @param msg json字符串
     */
    void CallbackCoreMessage(const std_msgs::String::ConstPtr& msg);
    /**
     * 从注册信息到列表中删除相关注册
     * @param[in] p_fun 函数指针
     * @param[in] p_class 对象指针
     * @return 0：成功，其他失败
     */
    int UnReqList(void* p_fun,void* p_class);
    /**
     * 增加回调函数到注册列表中
     * @param[in] fun 回调函数
     * @param[in] p_fun 函数指针
     * @param[in] p_class 对象指针
     * @return 0：成功，其他失败
     */
    int ReqList(boost::function<void(JsonValueConstPtr&)>& fun,void* p_fun,void* p_class);
};


#endif //CORE_NODE_TASK_MANAGER_H
