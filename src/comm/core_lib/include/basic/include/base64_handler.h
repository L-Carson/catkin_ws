
#ifndef MY_COMM_BASE64_H
#define MY_COMM_BASE64_H

#include <string>

/**
 * base64加密
 * @param str 加密前数据
 * @param len 加密数据长度
 * @return 加密后数据
 */
std::string Base64Encode(const char* str, unsigned int len);
/**
 * base64解密
 * @param s 加密后数据
 * @return 解密后数据
 */
std::string Base64Decode(std::string const& s);

#endif //MY_COMM_BASE64_H
