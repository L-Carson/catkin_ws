/** 
 * @file     alarm_manager.h
 * @brief    告警管理
 * @details  告警管理，作为API提供给他人调用，调用者需要阅读《S330无人清扫车人机与导航通信接口规范》文档
 * @author   华仔
 * @date     9/15/21 
 */

#ifndef CORE_NODE_ALARM_MANAGER_H
#define CORE_NODE_ALARM_MANAGER_H

#include "basic_manager.h"
#include <mutex>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include "my_std_thread.h"
#include "commit_verison.h"

/** 告警核查频率，单位毫秒，发生和解除告警后核查一次，平时定时核查*/
constexpr int  kAlarmCheckTimeOut        = 5000;
constexpr char kPubAlarmModuleTopic[]    = "/alg_to_core_alarm";///< 算法模块 ---->core模块，算法模块
constexpr char kPubAlarmModuleCoreTopic[]= "/alg_to_core_alarm_check";///< 算法模块 ---->core模块
constexpr char kPubOccurenceTopic[] = "/alg_to_core_occurence";///< 算法模块 ---->core模块
//constexpr char kSubAlarmModuleTopic[]    = "/core_to_alg_alarm";///< core模块 ---->算法模块
//constexpr char kSubAlarmSelfTopic[]      = "/alg_to_core_alarm";///< 算法模块 ---->算法模块

//==================================周期性消息日志打印时长=====================================================================
/** 告警发生*/
constexpr int   kDebugAlarmManagerReport=5000;
constexpr int   kDebugAlarmManagerRecover=5000;


/** 告警管理类*/
class BasicAlarmManager
{
public:
    /**
     * 创建一个BasicTaskManager对象，如果存在则不重新创建，返回之前创建的对象
     * @return BasicTaskManager对象指针
     */
    static BasicAlarmManager* Create();
    /**
     * 销毁一个实例对象，在进程结束前的时候调用，其他情况不调用
     */
    static void Destory();
    /**
     * 给相关告警注册回调
     * @tparam T 类模板
     * @param[in] alarm_id 告警ID
     * @param[in] fp 告警发生回调函数指针
     * @param[in] obj 告警发生回调类指针，一般为this
     * @param[in] fp1 告警解除回调函数指针
     * @param[in] obj1 告警解除回调类指针，一般为this
     * @return 0：成功，其他失败
     */
    template <class T>
    int Reg(int alarm_id,void(T::*fp)(JsonValueConstPtr&), T* obj,void(T::*fp1)(JsonValueConstPtr&), T* obj1){
        boost::function<void(JsonValueConstPtr&)> fun;
        boost::function<void(JsonValueConstPtr&)> fun1;
        fun.clear();
        fun1.clear();
        if(fp && obj){
            fun = boost::bind(fp,obj,_1);
        }
        if(fp1 && obj1){
            fun1 = boost::bind(fp1,obj1,_1);
        }
        if(fun.empty() && fun1.empty()){
            return -1;
        }
        return ReqList(alarm_id,fun,fun1);
    }
    /**
     *给相关告警取消注册回调
     * @param alarm_id 告警ID，和注册时一致
     * @return 0：成功，其他失败
     */
    int UnReg(int alarm_id);

    /**
     * 告警发生
     * @param alarm_info 告警发生的详细信息，参考《S330无人清扫车人机与导航通信接口规范》文档
     * 例如：{"AlarmId":1}
     * @return 0：成功，其他失败
     */
    int Report(Json::Value& alarm_info);
    /**
     * 告警恢复
     * @param alarm_info 告警恢复的详细信息，参考《S330无人清扫车人机与导航通信接口规范》文档
     * 例如：{"AlarmId":1}
     * @return 0：成功，其他失败
     */
    int Recover(Json::Value& alarm_info);
    /**
     * @brief 事件发生
     * 
     * @param event_info 事件发生的详细信息 ，参考《S330无人清扫车人机与导航通信接口规范》文档
     * @return 0：成功，其他失败 
     */
    int Occurence(Json::Value& event_info);

private:
    /**
     * 构造函数
     */
    BasicAlarmManager();
    /**
     * 析构函数
     */
    ~BasicAlarmManager();
    /**
     * 拷贝构造函数
     * @param[in] signal
     */
    BasicAlarmManager(const BasicAlarmManager& signal);
    /**
     * 赋值函数
     * @param[in] signal
     * @return
     */
    const BasicAlarmManager& operator=(const BasicAlarmManager& signal);
private:
    static BasicAlarmManager* m_instance_;///< 对象指针
    static std::mutex m_instance_mutex_;///< 多线程安全
private:
    ros::NodeHandle                         nh_;
    ros::Publisher                          to_core_module_;///< 算法模块---->core模块，算法模块
    ros::Publisher                          to_core_module_check_;///< 算法模块---->core模块,发送数据核算
    ros::Publisher                          to_core_module_occurence_;///< 算法模块---->core模块,发送事件发生
    //ros::Subscriber                         from_core_module_;///< core模块---->算法模块
    ros::Subscriber                         from_alg_module_;///< 算法模块---->算法模块
    std::vector<Json::Value> alarm_list_;///< 保存现有告警列表
    std::mutex alarm_list_mutex_;///< 保护alarm_list_
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
    bool is_alarm_happen_ = false;//告警发生标志 用于处理告警发生时立马告警核查
private:
    /**
     * ros消息回调，core到算法的json消息
     * @param msg json字符串
     */
    void CallbackCoreMessage(const std_msgs::String::ConstPtr& msg);
    /**
     * 回调函数注册到列表中
     * @param fun 告警发生回调
     * @param fun1 告警解除回调
     * @return 0：成功，其他失败
     */
    int ReqList(int id,boost::function<void(JsonValueConstPtr&)>& fun,boost::function<void(JsonValueConstPtr&)>& fun1);
     /**
      * 告警发生和解除，维护当前的告警列表
      * @param report true：告警发生，false：告警解除
      */
    void AlarmHappen(bool report,Json::Value obj);
    /**
     * 是否有此告警
     * @param id 告警id
     * @return true：有，false：没有
     */
    bool IsHisAlarm(int id);
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg);
    /**
     * 返回告警核查字符串
     * @return 字符串，协议文档中MsgContent对象内容
     */
    std::string AlarmCheckString();
};




#endif //CORE_NODE_ALARM_MANAGER_H
