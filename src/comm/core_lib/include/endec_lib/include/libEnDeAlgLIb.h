//
// Created by ljh on 6/1/21.
//

#ifndef DEMO_LIBENDEALGLIB_H
#define DEMO_LIBENDEALGLIB_H

#include <iostream>
#if _WINDOWS
#define LIBOUT_API extern __declspec(dllexport)
#else
#define LIBOUT_API extern


#endif

/***********************************************************************************
* 函数名：
* 描述  ：得到用户名密码
* 输入  ：in_time：修改密码时间，格式固定为 yyyy-mm-dd hh:mm:ss；in_mac：系统的 mac 地址;in_passwd_len:out_passwd缓存的长度
* 输出  ：out_passwd：密码
* 返回值 ：true:成功；false失败
************************************************************************************/
LIBOUT_API bool GetUserPasswd(char* in_time,char* in_mac,int in_passwd_len,char* out_passwd);

/***********************************************************************************
* 函数名：
* 描述  ： 用户密码加解密
* 输入  ：in_en_de：true:加密，false:解密；in_passwd：加密/解密数据；out_passwd_len：out_passwd缓存长度
* 输出  ：out_passwd：加密/解密数据
* 返回值 ：true:成功；false失败
************************************************************************************/
LIBOUT_API bool EnDePasswd(bool in_en_de,char * in_passwd,int out_passwd_len,char *out_passwd);

/***********************************************************************************
* 函数名：
* 描述  ： 创建证书
* 输入  ：filename：证书存放地址加文件名（绝对路径）；type 证书类型： 0：普通证书 1： 能使用暴力破解功能；
 *        start_time 证书有效起始时间，格式固定为 yyyy-mm-dd hh:mm:ss；end_time 证书失效时间，格式固定为 yyyy-mm-dd hh:mm:ss
 *        mac windows 电脑 wifi 的 mac 地址
* 输出  ：无
* 返回值 ：0:成功，其他失败；1：证书过期  2：证书非法
************************************************************************************/
LIBOUT_API bool CreatCer(char *filename,int type,char* start_time,char* end_time,char* mac);

/***********************************************************************************
* 函数名：
* 描述  ： 验证证书
* 输入  ：cur_mac:当前mac,in_time：当前时间，格式固定为 yyyy-mm-dd hh:mm:ss；filename：证书存放地址加文件名（绝对路径）
* 输出  ：无
* 返回值 ：0:成功，其他失败；1：证书非法  2：证书过期  3:证书时间没到 4：类型不匹配 5:mac不匹配
************************************************************************************/
LIBOUT_API int VerifyCer(char* cur_mac,char* in_time,int in_type,char *filename);

/**
 * 日志加解密
 * @param[in] in_en_de true:加密，false:解密
 * @param[in] in_data 加/解密数据
 * @param[in,out] out_data 加/解密后数据
 * @return 0：成功，其他失败
 */
LIBOUT_API int EnDeLog(bool in_en_de,std::string in_data,std::string& out_data);
LIBOUT_API int EnDeCmd(bool in_en_de, std::string in_data, std::string & out_data);
#endif //DEMO_LIBENDEALGLIB_H
