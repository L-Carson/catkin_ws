/**
 * @file     boost_handler.h
 * @brief    常用boost函数整理封装
 * @details  常用boost函数整理封装
 * @author   华仔
 * @date     9/7/21
 */

#ifndef BASICS_BOOSTHANDLER_H
#define BASICS_BOOSTHANDLER_H

#include <iostream>

class BoostHandler
{
public:
    /**
     * 构造函数
     */
    BoostHandler();
    /**
     * 析构函数
     */
    ~BoostHandler();
public:
    /**
     * 判断某个字符串符合正则表达式
     * @param[in] str 被检查字符串
     * @param[in] reg 正则表达式
     * @return true:符合规则，false：不符合规则
     */
    static bool StringRegexMatch(std::string str,std::string reg);
};




#endif //NAV_BUSINESS_MODULE_BOOSTHANDLER_H
