

#ifndef SHARE_MEMORY_API_H
#define SHARE_MEMORY_API_H

#include <iostream>
#include <dlfcn.h>
#include <chrono>
#include <sys/time.h>
#include <vector>
#include "shmcache/shmcache_types.h"

using namespace std;


typedef struct ShareMemoryDataSt
{
    //int
}ShareMemoryDataSt;


struct KeyInfo
{
    string time;
    string pid;
};

class ShareMemoryApi 
{
public:
    /**
     * 构造函数
     * 共享内存包含多个内存块，内存块与内存块之间有连接关系。
     * @param[in] mode 模式    1-共享内存数据满时，在往共享内存中写入数据失败（追加写入）   2-共享内存数据满时，在往共享内存中写入数据，轮训覆盖最早的内存块（追加写入）   3-覆盖写入共享内存数据
     * @param[in] memory_key 共享内存标识，同一系统唯一
     * @param[in] max_key_count 内存块个数
     * @param[in] max_value_size 单块内存块大小，单位字节
     */
    ShareMemoryApi(int mode, string memory_key, int max_key_count = 80, int max_value_size = 256*1024);
    /**
     * 析构函数
     */
    ~ShareMemoryApi();
    /**
     * 初始化
     * @return 
     */
    int Init();
    /**
     * 卸载
     * @return 0-成功 非0-失败
     */
    int Remove();
    /**
     * 共享内存中追加/覆盖内容（取决于模式）
     * @param[in] data 数据首地址
     * @param[in] data_len 数据长度
     * @return 0-成功 非0-失败
     */
    int Set(char* data, unsigned int data_len);
    /**
     * 获取共享内存内容，内容带有格式：4字节长度+数据+4字节长度+数据.......4字节长度+数据
     * @param[out] data 数据首地址
     * @param[in] data_len data的长度
     * @param[out] get_len 接收长度
     * @return 0-成功 非0-失败
     */
    int Get(char* data, unsigned int data_len, unsigned int & get_len);
    
private:
    long long GetMilliSecondTime()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    bool LoadConfig();
    int GetKeyCount();
    void GetKeyInfo(string key_str, string & key_time, string & key_pid);
    
    int SetData(char* data, unsigned int data_len, string key_str);
    int SetRegular(char* data, unsigned int data_len);
    int SetLoop(char* data, unsigned int data_len);
    int SetSingle(char* data, unsigned int data_len);
    
    struct shmcache_config m_Config;
    struct shmcache_context m_Context;
    
    int m_MaxKeyCount;
    int m_MaxValueSize;
    int m_Mode;
    string m_FileName;
};





/*******************demo*****************/
/*

int main()
{  
    // 设置共享内存内容
    ShareMemoryApi a;
    a.Init();

    string str = "share memory test";
    if(a.Set(str) == 0)
    {
    }
    
    // 获取共享内存内容
    ShareMemoryApi a;
    a.Init();

    string str = "";
    if(a.Get(str) == 0)
    {
    }
}

*/













#endif 
