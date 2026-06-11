/** 
 * @file     map_manager.h
 * @brief    地图管理
 * @details  地图管理
 * @author   华仔
 * @date     10/10/21 
 */

#ifndef S330_BASICS_MAP_MANAGER_H
#define S330_BASICS_MAP_MANAGER_H
#include "basic_manager.h"
#include <iostream>
#include <mutex>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <ros/ros.h>
#include <std_msgs/String.h>

constexpr char kSubMapModuleTopic[]    = "/core_to_alg_map";///< core模块 ---->算法模块
constexpr char kPubMapModuleTopic[]    = "/alg_to_core_map";///< < 算法模块 ---->core模块

/** 地图管理类*/
class BasicMapManager
{
public:
    /**
     * 创建一个BasicTaskManager对象，如果存在则不重新创建，返回之前创建的对象
     * @return BasicTaskManager对象指针
     */
    static BasicMapManager* Create();
    /**
     * 销毁一个实例对象
     */
    static void Destory();
    /**
     * 地图变更注册回调
     * @tparam T 类模板
     * @param[in] fp 回调函数指针
     * @param[in] obj 回调函数类指针
     */
    template <class T>
    int RegMapChange(void(T::*fp)(std::string ), T* obj){
        if(!fp || !obj){
            return -1;
        }
        boost::function<void(std::string)> fun;
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
     * 取消地图变更注册回调
     * @tparam T
     * @param[in] fp 回调函数指针，和注册时一致
     * @param[in] obj 回调函数类指针，和注册时一致
     * @return 0：成功，其他失败
     */
    template <class T>
    int UnReqMapChange(void(T::*fp)(int), T* obj){
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
     * 已废弃
     * 获取当前地图版本，当版本发生变化，去双方约定好的文件夹（～/map_config/map/work_scene）拿地图数据
     * @return 地图名版本
     */
    std::string GetCurMap();
    /**
     * 获取当前地图路径
     * @return 地图路径 默认返回/home/用户/map_config/work_scene
     */
    std::string GetWorkMapPath();

    /**
     * 给人机过来地图管理消息注册回调
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
private:
    /**
     * 构造函数
     */
    BasicMapManager();
    /**
     * 析构函数
     */
    ~BasicMapManager();
    /**
     * 拷贝构造函数
     * @param[in] signal
     */
    BasicMapManager(const BasicMapManager& signal);
    /**
     * 赋值函数
     * @param[in] signal
     * @return
     */
    const BasicMapManager& operator=(const BasicMapManager& signal);
private:
    static BasicMapManager* m_instance_;///< 对象指针
    static std::mutex m_instance_mutex_;///< 多线程安全
private:
    ros::NodeHandle                         nh_;
    ros::Subscriber                         from_core_module_;///< core模块---->算法模块
    ros::Publisher                          to_core_module_;///< 算法模块---->core模块
    std::string                                     cur_map_="";///< 当前地图信息
    std::mutex                              cur_map_mutex_;///< 多线程安全
private:
    /**
     * 设置当前使用地图
     * @param map 地图信息
     */
    void SetCurMapInfo(std::string map);
    /**
     * 获取当前正在使用的地图
     * @return 地图信息
     */
    std::string GetCurMapInfo();
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
    int ReqList(boost::function<void(std::string)>& fun,void* p_fun,void* p_class);
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
    /**
     * 获取本地地图文件
     * @param ver 地图版本
     * @return 对应地图存放位置，绝对路径
     */
    //std::string IntVerToMapPath(int ver);
};



#endif //S330_BASICS_MAP_MANAGER_H
