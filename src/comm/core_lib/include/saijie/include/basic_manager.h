/** 
 * @file     basic_manager.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     9/14/21 
 */

#ifndef CORE_NODE_BASIC_MANAGER_H
#define CORE_NODE_BASIC_MANAGER_H

#include <json/json.h>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

/**  core地图文件存放的路径 */
constexpr char  kCoreMapSaveFileFlag[] = "/map_config/map";


//消息域
enum MsgDomain{
    EM_MSG_DOMAIN_DEVICE_MANAGER = 1,           //设备管理
    EM_MSG_DOMAIN_TASK_MANAGER = 2,             //任务管理
    EM_MSG_DOMAIN_ALARM_MANAGER = 3,            //告警管理
    EM_MSG_DOMAIN_SOFTWARE_MANAGER = 4,         //软件管理
    EM_MSG_DOMAIN_MAP_MANAGER = 5,              //地图管理
    EM_MSG_DOMAIN_GENERAL_FUNC_MANAGER = 6,     //通用功能管理
    EM_MSG_DOMAIN_CONFIG_MANAGER = 7,           //配置功能管理
    EM_MSG_DOMAIN_UNKOWN
};

/**  json对象常量指针*/
typedef boost::shared_ptr< Json::Value const> JsonValueConstPtr;

/**
 * 初始化SaijieBasics库
 * @param[bool] ScreenOutput_flag 设置日志屏幕输出  true日志屏幕输出 false日志不屏幕输出
 * @return 0：成功，其他失败
 */
int SaijieBasicsLibInit(bool ScreenOutput_flag = true);
/**
 * 获取SaijieBasics库的版本
 * @return SaijieBasics库的版本
 */
std::string GetSaijieBasicsLibVer();
/**
 * 获取s330库日志文件标志
 * @return s330库日志文件标志
 */
std::string GetSaijieBasicsLibLogFlag();
/**
 * 判断指定文件夹下是否存在当前时间文件夹，不存在则创建
 * @param name 指定文件夹
 * @return 当前时间文件夹路径--绝对路径
 */
std::string SaijieRunDataFolderName(const std::string name);

#endif //CORE_NODE_BASIC_MANAGER_H
