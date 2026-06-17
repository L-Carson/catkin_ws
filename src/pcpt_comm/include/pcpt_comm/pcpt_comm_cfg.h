#ifndef __PCPT_COMM_CFG_H__
#define __PCPT_COMM_CFG_H__

#include "comm/comm_cfg_param.h"
#include "pcpt_comm/pcpt_comm_singleton.h"

using namespace std;

/**************************************************************************************
功能描述: 感知配置
修改记录:
**************************************************************************************/
class PCPT_CFG_C
{
public:
    PCPT_CFG_C() {}
    PCPT_CFG_C(const PCPT_CFG_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_CFG_C &operator=(const PCPT_CFG_C &) = delete;   // 禁止使用拷贝赋值运算符

    static PCPT_CFG_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            return pInstance_;
        }

        pInstance_ = new PCPT_CFG_C();
        INT32 rslt = pInstance_->Init();
        if (rslt != 0) {
            ST_LOG_ERR("Create Object Fail.");
            delete pInstance_;
            pInstance_ = nullptr;
            exit(EXIT_FAILURE);
        }

        PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
        ST_LOG_INFO("Create Object Succ.");
        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

    const CFG_PARAM_C& GetCfgParam() const;

private:
    CFG_PARAM_C                 objCfgParam_;
    static std::recursive_mutex mutex_r_;
    static PCPT_CFG_C*          pInstance_;

    int Init();
};

#endif
