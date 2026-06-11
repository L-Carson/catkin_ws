
#include "comm/comm_cfg/comm_cfg_param_file.h"

#include "comm/comm_cfg/comm_cfg_param_read_sensor.h"
#include "comm/comm_cfg/comm_cfg_param_read_mnl.h"
#include "comm/comm_cfg/comm_cfg_param_read_pnp.h"
#include "comm/comm_cfg/comm_cfg_param_read_pnc.h"

#include "config_manager.h"



namespace COMM
{

namespace CFG
{

std::atomic_bool CFG_PARAM_FILE_MNG_C::isIgnorCfgCheckingWhenDbg_{false};

/**************************************************************************************
功能描述: 设置忽略工作区配置文件防修改检查
修改记录:
**************************************************************************************/
void CFG_PARAM_FILE_MNG_C::SetIgnoreCfgParamChecking(void)
{
    isIgnorCfgCheckingWhenDbg_ = true;

    ST_LOG_INFO("Succ.");
}

/**************************************************************************************
功能描述: 获取工作区目录
修改记录:
**************************************************************************************/
std::string CFG_PARAM_FILE_MNG_C::GetWorkCfgParamFileDir(void) const
{
    return strWorkCfgParamFileDir_;
}

/**************************************************************************************
功能描述: 获取修改区目录
修改记录:
**************************************************************************************/
std::string CFG_PARAM_FILE_MNG_C::GetChangeCfgParamFileDir(void) const
{
    return FILE_OPRT_C::ToStyledDir(GetLoginUserRootDirPath() + std::string(kNavAlgoChangeConfigPath));
}

/**************************************************************************************
功能描述: 获取变更记录文件目录
修改记录:
**************************************************************************************/
std::string CFG_PARAM_FILE_MNG_C::GetChangeRecordCfgParamFileDir(void) const
{
    return (GetChangeCfgParamFileDir() + "history_record/");
}

/**************************************************************************************
功能描述: 获取新的工作版配置文件
修改记录:
**************************************************************************************/
std::string CFG_PARAM_FILE_MNG_C::GetWorkCfgParamFilePath(void) const
{
    return (strWorkCfgParamFileDir_ + strCfgParamFileNameEx_);
}

/**************************************************************************************
功能描述: 获取新的历史版本配置文件
修改记录:
**************************************************************************************/
std::string CFG_PARAM_FILE_MNG_C::GetWorkResvCfgParamFilePath(const std::string &strCfgVer) const
{
    return (strWorkCfgParamFileDir_ + strCfgParamFileName_ + "_" + strCfgVer + ".xml");
}

/**************************************************************************************
功能描述: 获取修改版配置文件路径
修改记录:
**************************************************************************************/
std::string CFG_PARAM_FILE_MNG_C::GetChangeCfgParamFilePath(void) const
{
    return (GetChangeCfgParamFileDir() + strCfgParamFileNameEx_);
}

/**************************************************************************************
功能描述: 读配置文件
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_MNG_C::ReadWorkCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const
{
    bool isCheck = isCheckWorkCfgParamFileModManually_ && !isIgnorCfgCheckingWhenDbg_;

    int rslt = ConfigManager::ReadTiXmlElementFromNavCfg(objXmlCfgFile, strCfgParamFilePath, isCheck);

    switch (rslt) {
        case 0:  break;
        case 1:  ST_LOG_ERR("配置文件不存在: %s.", strCfgParamFilePath.c_str()); break;
        case 2:  ST_LOG_ERR("配置文件内容格式错误: %s.", strCfgParamFilePath.c_str()); break;
        case 3:  ST_LOG_ERR("配置文件被手动修改过: %s.", strCfgParamFilePath.c_str()); break;
        default: ST_LOG_ERR("配置文件异常: %s.", strCfgParamFilePath.c_str());        break;
    }

    if (rslt != 0) {
        ST_LOG_ERR("ConfigManager::ReadTiXmlElementFromNavCfg(%s) Fail.", strCfgParamFilePath.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读修改区配置文件
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_MNG_C::ReadChangeCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const
{
    /* 不需要检查人工修改。 */
    int rslt = ConfigManager::ReadTiXmlElementFromNavCfg(objXmlCfgFile, strCfgParamFilePath, false);
    if (rslt != 0) {
        ST_LOG_ERR("ConfigManager::ReadTiXmlElementFromNavCfg(%s) Fail.", strCfgParamFilePath.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读老的配置文件
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_MNG_C::ReadOldCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const
{
    /* 不需要检查人工修改。 */
    bool isSucc = objXmlCfgFile.LoadFile(strCfgParamFilePath.c_str());
    if (!isSucc) {
        ST_LOG_ERR("objXmlCfgFile.LoadFile(%s) Fail.", strCfgParamFilePath.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 保存配置文件
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_MNG_C::SaveCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const
{
    /* 保存文件时都调用基础架构提供的接口，添加校验信息。 */
    int rslt = ConfigManager::SaveTiXmlDocumentToNavCfg(objXmlCfgFile, strCfgParamFilePath);
    if (rslt != 0) {
        ST_LOG_ERR("ConfigManager::SaveTiXmlDocumentToFile(%s) Fail.", strCfgParamFilePath.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 备份修改记录
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_MNG_C::BackupWorkCfgParamFile(void) const
{
    int rslt = ConfigManager::BackCupWorkAlgoConfigFile();
    if (rslt != 0) {
        ST_LOG_ERR("ConfigManager::BackCupWorkAlgoConfigFile() Fail.");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 设置导航配置参数文件目录
修改记录:
**************************************************************************************/
void CFG_PARAM_FILE_MNG_C::SetWorkCfgParamFileDir(void)
{
    /* 优先使用传入的目录，传入目录时表示不是正常车辆运行场景，不需要进行修改确认。 */
    if (!strInputCfgParamFileDir_.empty()) {
        isCheckWorkCfgParamFileModManually_ = false;
        strWorkCfgParamFileDir_             = FILE_OPRT_C::ToStyledDir(strInputCfgParamFileDir_);
    } else {
        isCheckWorkCfgParamFileModManually_ = true;
        strWorkCfgParamFileDir_             = FILE_OPRT_C::ToStyledDir(GetLoginUserRootDirPath() + std::string(kNavAlgoWorkConfigPath));
    }
}

/**************************************************************************************
功能描述: 读取配置文件
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_C::Read(CFG_PARAM_STRU &stParam)
{
    ST_LOG_INFO("Begin...");

    const std::string strCfgParamFilePath = objCfgParamFileMng_.GetWorkCfgParamFilePath();
    ST_LOG_INFO("Cfg Param File Path: %s.", strCfgParamFilePath.c_str());

    TiXmlDocument objXmlCfgFile;
    int rslt = objCfgParamFileMng_.ReadWorkCfgParamFile(strCfgParamFilePath, objXmlCfgFile);
    if (rslt != 0) {
        ST_LOG_ERR("Load File(%s) Fail.", strCfgParamFilePath.c_str());
        ST_LOG_ERR("加载配置文件(%s) 失败.", strCfgParamFilePath.c_str());
        return -1;
    }

    TiXmlElement *pobjCfgParam = objXmlCfgFile.RootElement();
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get Root Element Fail.");
        return -1;
    }

    CFG_PARAM_READER_C    objPncCfgReader;

    READ_CFG(objPncCfgReader.ReadVehicleBodyCfg,        pobjCfgParam, stParam.stVehicleBody);
    READ_CFG(objPncCfgReader.ReadVehicleChassisCfg,     pobjCfgParam, stParam.stVehicleChassis);
    READ_CFG(objPncCfgReader.ReadPlanCfg,               pobjCfgParam, stParam.stPlan);

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 写入配置文件
修改记录:
**************************************************************************************/
int CFG_PARAM_FILE_C::Save(const CFG_PARAM_STRU &stParam)
{
    return -1;
}


}

}



