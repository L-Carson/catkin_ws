/*
Author      : gongzhihao
Update Date : 2025/07/09
*/
#ifndef COMM_JSON_H
#define COMM_JSON_H

#include "comm_layer/comm_base/comm_base.h"

// 读文件
std::string ReadFile(const std::string &abs_file_name);

// 写文件
bool WriteFile(const std::string &abs_file_name, const std::string &file_data);

// 字符串->Json
Json::Value StrToJson(const std::string &str_data);

// Json->字符串
std::string JsonToStr(const Json::Value &json_data);

#endif
