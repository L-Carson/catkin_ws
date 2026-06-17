#include "pcpt_comm/pcpt_comm_debug.h"

std::recursive_mutex    PCPT_DEBUG_C::mutex_r_;
PCPT_DEBUG_C*           PCPT_DEBUG_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 加载参数
修改记录:
**************************************************************************************/
int PCPT_DEBUG_C::LodarParams() {
    PCPT_FILE_MNG_C objFileMng;
    string strParamsPath = objFileMng.GetUsrRootDir();
    strParamsPath += "map_config/pcpt_debug_params.yaml";

    ST_LOG_INFO("path:%s", strParamsPath.c_str());

    if (!objFileMng.IsFileExist(strParamsPath.c_str())) {
        ST_LOG_ERR("Debug parameter file is not exist.");
        return -1;
    }

    YAML::Node file   = YamlFileLoad(strParamsPath);
    YAML::Node parmas = YamlSubNodeAbort(file, "pcpt_debug_params");

    int rslt = 0;
    rslt += ReadSubNodeParams(parmas, "bool",   stParam.v_bool);
    rslt += ReadSubNodeParams(parmas, "int",    stParam.v_int);
    rslt += ReadSubNodeParams(parmas, "uint",   stParam.v_uint);
    rslt += ReadSubNodeParams(parmas, "float",  stParam.v_float);
    rslt += ReadSubNodeParams(parmas, "string", stParam.v_string);
    return rslt;
}

