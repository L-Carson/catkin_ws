#include "pcpt_comm/pcpt_comm_cfg.h"

std::recursive_mutex    PCPT_CFG_C::mutex_r_;
PCPT_CFG_C*             PCPT_CFG_C::pInstance_{nullptr};

int PCPT_CFG_C::Init()
{
    if (objCfgParam_.Init() != 0) {
        ST_LOG_ERR("objCfgParam_.Init() Fail.");
        return -1;
    }

    ST_LOG_INFO("Init Success.");
    return 0;
}

const CFG_PARAM_C& PCPT_CFG_C::GetCfgParam() const
{
    return objCfgParam_;
}