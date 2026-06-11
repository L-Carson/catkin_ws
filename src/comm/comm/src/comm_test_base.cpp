
#ifndef __COMM_TEST_BASE_CPP__
#define __COMM_TEST_BASE_CPP__

#include "comm/comm_test_base.h"

#include <math.h>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

/**************************************************************************************
功能描述: 获取界面输出键值
修改记录:
**************************************************************************************/
CHAR GetInputKey(void)
{
    static BOOL isCloseTerminalBuff = false;

    if (!isCloseTerminalBuff) {
        //关闭缓存区，使从终端接收一个字符不用按回车
        SYSTEM_EXEC("stty -icanon");

        isCloseTerminalBuff = true;
    }

    //fd_set 为long型数组
    //其每个元素都能和打开的文件句柄建立联系
    fd_set rfds;
    //将　rfds数组清零
    FD_ZERO(&rfds);
    //将rfds的第0位置为１，这样fd=1的文件描述符就添加到了rfds中
    //最初　rfds为00000000,添加后变为10000000
    FD_SET(0, &rfds);

    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 1 * 1000; //设置等待超时时间

    //检测键盘是否有输入
    //由内核根据io状态修改rfds的内容，来判断执行了select的进程哪个句柄可读
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        return getchar();
    }

    //没有数据返回n
    return 0;
}

INT32 WaitAndGetInputVal(const INPUT_INFO_STRU astInputInfo[], UINT32 numOfInputInfo)
{
    ST_LOG_INFO("Input Info: ");
    for (UINT32 loop = 0; loop < numOfInputInfo; ++loop) {
        const INPUT_INFO_STRU &stInputInfo = astInputInfo[loop];
        ST_LOG_INFO("    %c: %s", stInputInfo.inputKey, stInputInfo.strDes.c_str());
    }

    ST_LOG_INFO("Please Input: ");

    while (true) {
        CHAR inputKey = GetInputKey();
        if (inputKey ==0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        for (UINT32 loop = 0; loop < numOfInputInfo; ++loop) {
            const INPUT_INFO_STRU &stInputInfo = astInputInfo[loop];
            if (stInputInfo.inputKey == inputKey) {
                return stInputInfo.val;
            }
        }

        ST_LOG_ERR("Invalid Input: %c", inputKey);
    }
}

BOOL IsSerialSimulationMode(int argc, char** argv)
{
    R_ASSERT(argv != nullptr, false);

    auto iter = std::find_if(argv, argv + argc, [](const char *pstInputParam) { return (std::string(pstInputParam) == std::string("-simulation")); });
    if (iter != (argv + argc)) {
        ST_LOG_INFO("Serial Simulation Mode.");
        return true;
    }

    return false;
}

/**************************************************************************************
功能描述: 判断是否忽略配置文件防修改校验
修改记录:
**************************************************************************************/
BOOL IsIgnoreCfgParamFileModChecking(int argc, char** argv)
{
    R_ASSERT(argv != nullptr, false);

    auto iter = std::find_if(argv, argv + argc, [](const char *pstInputParam) { return (std::string(pstInputParam) == std::string("-ignore_cfg_checking")); });
    if (iter != (argv + argc)) {
        ST_LOG_INFO("Ignore Cfg Param File Modified Checking.");
        return true;
    }

    return false;
}


#endif

