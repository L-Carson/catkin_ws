#pragma once
#include <mutex>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <json/value.h>
#include "my_std_thread.h"
#include "basic_manager.h"

constexpr char kPubCommonModuleTopic[]    = "/alg_to_core_common";///< 算法模块 ---->core模块
constexpr char kSubCommonModuleTopic[]    = "/core_to_alg_common";///< core模块 ---->算法模块


class CommonManager
{
public:
    /**
     * 创建一个CommonManager对象，如果存在则不重新创建，返回之前创建的对象
     * @return CommonManager对象指针
     */
    static CommonManager* Create();
    /**
     * 销毁一个实例对象，在进程结束前的时候调用，其他情况不调用
     */
    static void Destory();
    /**
     * 获取当前节点信息
     * @return   json对象例如： {"NodeInfos":{"ExePath":"/home/saite/core","NodeName":"/core"}}
     */
    Json::Value GetNodeInfo();

        /**
     * 给人机过来的消息注册回调
     * @tparam T 类模板
     * @param[in] fp 回调函数指针
     * @param[in] obj 回调函数类指针
     * @param[in] msg_type 要回调的消息类型 -1: 表示所有消息
     */
    template <class T>
    int Reg(int msg_type, void(T::*fp)(JsonValueConstPtr&), T* obj){
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
        return ReqList(MsgDomain::EM_MSG_DOMAIN_GENERAL_FUNC_MANAGER, msg_type, fun, fp1, obj1);
    }
    /**
     * 取消注册回调
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
     * 取消管理所有注册
     * @return 0：成功，其他失败
     */
    int UnRegAll();
    /**
     * 发送任务管理相关消息到人机
     * @param[in] json 发送的json数据参考《S330无人清扫车人机与导航通信接口规范》文档
     * 例如：{"MsgType":6,"TransId":0,"MsgContent":{...}}
     * @return 0：成功，其他失败
     */
    int Send(Json::Value& json);
private:
    /**
     * 构造函数
     */
    CommonManager();
    /**
     * 析构函数
     */
    ~CommonManager();
    CommonManager(const CommonManager& signal) = delete;
    const CommonManager& operator=(const CommonManager& signal) = delete;
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg);

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
    int ReqList(int msg_domain, int msg_type, boost::function<void(JsonValueConstPtr&)>& fun,void* p_fun,void* p_class);

private:
    static CommonManager* m_instance_;///< 对象指针
    static std::mutex m_instance_mutex_;///< 多线程安全
private:
    ros::NodeHandle                         nh_;
    ros::Publisher                          to_core_module_;///< 算法模块---->core模块
    ros::Subscriber                         from_core_module_;///< core模块---->算法模块
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
};

