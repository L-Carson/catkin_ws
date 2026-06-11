/** 
 * @file     dev_manager.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     9/15/21 
 */

#ifndef CORE_NODE_DEV_MANAGER_H
#define CORE_NODE_DEV_MANAGER_H


#include "basic_manager.h"
#include <mutex>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <ros/ros.h>
#include <std_msgs/String.h>

constexpr char kPubDevModuleTopic[]             = "/alg_to_core_dev";///< 算法模块 ---->core模块
constexpr char kSubDevModuleTopic[]             = "/core_to_alg_dev";///< core模块 ---->算法模块
constexpr char kPubDevStatusModuleTopic[]       = "/alg_to_core_dev_status";///< 算法模块 ---->core模块 订阅者能订阅到发布者最后一次记录
constexpr char kSubDevStatusModuleTopic[]       = "/core_to_alg_dev_status";///< core模块 ---->算法模块 订阅者能订阅到发布者最后一次记录
constexpr char kPubDevDefaultStatusModuleTopic[]  = "/alg_to_core_dev_DefaultStatus";///< 算法模块 ---->core模块 订阅者能订阅到发布者最后一次记录 注册默认状态的
//==================================周期性消息日志打印时长=====================================================================
/** 设备管理状态上报*/
constexpr int   kDebugDevStateReport=5000;
constexpr int   kDebugDevCoreStatusMessage=5000;
constexpr int   kDebugDevCoreMessage=5000;

/** 设备管理类*/
class BasicDevManager
{
public:
    /**
     * 创建一个BasicTaskManager对象，如果存在则不重新创建，返回之前创建的对象
     * @return BasicTaskManager对象指针
     */
    static BasicDevManager* Create();
    /**
     * 销毁一个实例对象，在进程结束前的时候调用，其他情况不调用
     */
    static void Destory();
    /**
     * 给人机过来设备管理消息注册回调
     * @tparam T 类模板
     * @param[in] msg_type 消息类型
     * @param[in] fp 回调函数指针
     * @param[in] obj 回调函数类指针
     */
    template <class T>
    int Reg(int msg_type,void(T::*fp)(JsonValueConstPtr&), T* obj){
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
        return ReqList(msg_type,fun,fp1,obj1);
    }
    /**
     * 取消注册，按消息类型
     * @param[in] msg_type 消息类型
     * @return 0：成功，其他失败
     */
    int UnReg(int msg_type);
    /**
     * 清空注册
     * @return 0：成功，其他失败
     */
    int UnRegClear();
    /**
     * 取消注册，按回调函数
     * @tparam T 类模板
     * @param[in] fp 回调函数指针
     * @param[in] obj 回调函数类指针
     * @return 0：成功，其他失败
     */
    template <class T>
    int UnReg(void(T::*fp)(JsonValueConstPtr&), T* obj){
        void *fp1=NULL;
        void *obj1=NULL;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
        fp1 = (void*)fp;
        obj1=(void*)obj;
#pragma GCC diagnostic pop
        return UnRegList(fp1, obj1);
    }
    /**
     * 发送数据到人机,一般用于响应，主动上报命令
     * @param[in] info 参考《S330无人清扫车人机与导航通信接口规范》文档
     * 例如：{"MsgType":1,"TransId":0,"MsgContent":{...}}
     * @return 0：成功，其他失败
     */
    int Send(Json::Value& info);
    /**
     * 获取人机状态，采用json格式易于扩展，json内容如：
     * {"linkState":1}
     * linkState-->0:没有与人机连接上，1：人机连接成功
     * @return json对象
     */
    Json::Value GetRobotStatus();
    /**
     * 启动通知，算法模块初始化完毕后调用此接口
     * @return 0：成功，其他失败
     */
    int StartStateReport();
    /**
     * 状态上报
     * @param state_info 状态信息,参考《S330无人清扫车人机与导航通信接口规范》文档
     * 例如：{"PoseInfo":{"MapX":0.2,"MapY":0.2,"MapZ":0.2,"MapYaw":0.2,"GpsLon":0.2,"GpsLat":0.2,"GpsAlt":0.2,"GpsYaw":0.2},"MoveInfo":{"Vel":0.9,"SteeringAngle":0.3,"TurnDir":0}}
     * @return 0：成功，其他失败
     */
    int StateReport(Json::Value& state_info);

    /**
     * 调用方注册状态上报默认内容
     * @param state_info 状态信息,参考《S330无人清扫车人机与导航通信接口规范》文档
     * 例如：{"PoseInfo":{"MapX":0.2,"MapY":0.2,"MapZ":0.2,"MapYaw":0.2,"GpsLon":0.2,"GpsLat":0.2,"GpsAlt":0.2,"GpsYaw":0.2},"MoveInfo":{"Vel":0.9,"SteeringAngle":0.3,"TurnDir":0}}
     * @return 0：成功，其他失败
     */
    int SetStatusDefaults(Json::Value& state_info);


private:
    /**
     * 构造函数
     */
    BasicDevManager();
    /**
     * 析构函数
     */
    ~BasicDevManager();
    /**
     * 拷贝构造函数
     * @param[in] signal
     */
    BasicDevManager(const BasicDevManager& signal);
    /**
     * 赋值函数
     * @param[in] signal
     * @return
     */
    const BasicDevManager& operator=(const BasicDevManager& signal);
private:
    static BasicDevManager* m_instance_;///< 对象指针
    static std::mutex m_instance_mutex_;///< 多线程安全
private:
    ros::NodeHandle                         nh_;
    ros::Publisher                          to_core_module_;///< 算法模块---->core模块
    ros::Subscriber                         from_core_module_;///< core模块---->算法模块
    ros::Publisher                          status_to_core_module_;///< 算法模块---->core模块
    ros::Publisher                          DefaultStatus_to_core_module_;///< 算法模块---->core模块 注册默认信息
    ros::Subscriber                         status_core_module_;///< core模块---->算法模块
    Json::Value                             robot_status_;///< 人机状态
    std::mutex                              robot_status_mutex_;
private:
    /**
     * ros消息回调，core到算法的json消息
     * @param msg json字符串
     */
    void CallbackCoreMessage(const std_msgs::String::ConstPtr& msg);
    /**
     * ros消息回调，core到算法的状态消息
     * @param msg json字符串
     */
    void CallbackCoreStatusMessage(const std_msgs::String::ConstPtr& msg);
    /**
     * 增加回调函数到注册列表中
     * @param[in] msg_id 消息类型
     * @param[in] fun 回调函数
     * @param[in] p_fun 函数指针
     * @param[in] p_class 类指针
     * @return 0：成功，其他失败
     */
    int ReqList(int msg_id,boost::function<void(JsonValueConstPtr&)>& fun,void* p_fun,void* p_class);
    /**
     * 减少回调函数到注册列表中
     * @param[in] fun 函数指针
     * @param[in] obj 类指针
     * @return 0：成功，其他失败
     */
    int UnRegList(void* fun, void* obj);
};







#endif //CORE_NODE_DEV_MANAGER_H
