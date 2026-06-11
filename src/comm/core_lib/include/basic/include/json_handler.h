/**
 * @file     json_handler.h
 * @brief    常用json函数整理封装
 * @details  常用jsont函数整理封装
 * @author   华仔
 * @date     9/7/21
 */

#ifndef BASICS_JSONHANDLER_H
#define BASICS_JSONHANDLER_H

#include "json/json.h"

#include <iostream>
using namespace std;

class JsonHandler
{
public:
    /**
     * 构造函数
     */
    JsonHandler();
    /**
     * 析构函数
     */
    ~JsonHandler();
public:
    /**
     * 字符串解析成json结构
     * @param[in] str 字符串
     * @param[out] json json格式对象
     * @return true：成功 false:失败
     */
    static bool StringToJson(std::string& str,Json::Value& json);
    /**
     * 字节流解析成json结构
     * @param[in] start 字节流开始地址
     * @param[in] end 字节流结束地址
     * @param[out] json json格式对象
     * @return true：成功 false:失败
     */
    static bool StringToJson(char* start,char* end,Json::Value& json);
    /**
     * json结构转字符串
     * @param[in] json json格式对象
     * @param[in] pre  浮点数据小数个数
     * @param[in] indentation 是否去除空格，\n等
     * @return json字符串
     */
    static std::string JsonToString(Json::Value& json,int pre=3,bool indentation=true);
    /**
     * json结构转字符串,保留三位有效数字
     * @param json json格式对象
     * @return json字符串
     */
    static std::string JsonToStringFloat3(Json::Value& json);


};

#endif //NAV_BUSINESS_MODULE_OLD_JsonHandler_H
