#pragma once

#include "comm/comm_yaml.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_file.h"
#include "pcpt_comm/pcpt_comm_singleton.h"


struct PCPT_DEBUG_PARAM_STRU {
    vector<bool>        v_bool;
    vector<int>         v_int;
    vector<uint32_t>    v_uint;
    vector<float>       v_float;
    vector<string>      v_string;
};

class PCPT_DEBUG_C {
public:
    PCPT_DEBUG_C() {
        if (!LodarParams()) isLoadParam_ = true;
    }

    /* 返回对象指针 */
    static PCPT_DEBUG_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            return pInstance_;
        }

        pInstance_ = new PCPT_DEBUG_C();
        if (!pInstance_->isLoadParam_) {
            ST_LOG_ERR("Create Object Fail.");
            DelPtr(pInstance_);
            return nullptr;
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

    PCPT_DEBUG_PARAM_STRU stParam;

private:
    bool   isLoadParam_{false};
    static std::recursive_mutex     mutex_r_;       // 递归锁
    static PCPT_DEBUG_C*            pInstance_;

    int LodarParams();

    template <typename T>
    int ReadParma(const YAML::Node& yaml, const std::string& strNode, const std::string& strKey, vector<T>& vParam)
    {
        T value{};
        if (YamlRead(yaml, strKey, value, value)) {
            ST_LOG_ERR("Failed to read <%s-%s> param.", strNode.c_str(), strKey.c_str());
            return -1;
        }

        vParam.push_back(value);
        return 0;
    }

    template <typename T>
    int ReadSubNodeParams(const YAML::Node& yaml, const std::string& strNode, vector<T>& vParam)
    {
        YAML::Node parmas = YamlSubNodeAbort(yaml, strNode);

        int rslt = 0;
        rslt += ReadParma(parmas, strNode, "param0", vParam);
        rslt += ReadParma(parmas, strNode, "param1", vParam);
        rslt += ReadParma(parmas, strNode, "param2", vParam);
        rslt += ReadParma(parmas, strNode, "param3", vParam);
        rslt += ReadParma(parmas, strNode, "param4", vParam);
        return rslt;
    }
};
