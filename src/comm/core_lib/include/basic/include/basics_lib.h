/** 
 * @file     basics_lib.h
 * @brief    基础库头文件
 * @details  基础库头文件，定义一些基础库变量，函数，宏等
 * @author   ljh
 * @date     9/7/21 
 */

#ifndef BASICS_LIB_BASICSLIB_H
#define BASICS_LIB_BASICSLIB_H

#include "log_handler.h"

/**
 * 设置架构组基础库日志文件标识
 * @param name 文件标识
 */
void SetBasicLibLogFlag(const char* name);
/**
 * 获取架构组基础库版本
 * @return
 */
std::string GetBasicLibVer();
/**
 * 获取架构组基础库日志文件标识
 * @return
 */
std::string GetBasicLibLogFlag();

#endif //BASICS_LIB_BASICSLIB_H
