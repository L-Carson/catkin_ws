#ifndef __COMM_CFG_PARAM_FILE_H__
#define __COMM_CFG_PARAM_FILE_H__

#include "comm_cfg_param_define.h"

#include "loc_xml/tinyxml.h"

#include <string>



namespace COMM
{

namespace CFG
{

/**************************************************************************************
功能描述: 导航配置参数文件目录
修改记录:
**************************************************************************************/
class CFG_PARAM_FILE_MNG_C
{
public:
    CFG_PARAM_FILE_MNG_C(const std::string &strInputCfgParamFileDir = "") : strInputCfgParamFileDir_(strInputCfgParamFileDir)
    {
        SetWorkCfgParamFileDir();
    }

    static void SetIgnoreCfgParamChecking(void);

    std::string GetWorkCfgParamFileDir(void) const;
    std::string GetChangeCfgParamFileDir(void) const;
    std::string GetChangeRecordCfgParamFileDir(void) const;

    std::string GetWorkCfgParamFilePath(void) const;
    std::string GetWorkResvCfgParamFilePath(const std::string &strCfgVer) const;

    std::string GetChangeCfgParamFilePath(void) const;

    int ReadWorkCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    int ReadChangeCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    int ReadOldCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    int SaveCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    int BackupWorkCfgParamFile(void) const;

private:
    static std::atomic_bool isIgnorCfgCheckingWhenDbg_;

    const std::string       strInputCfgParamFileDir_;
    const std::string       strCfgParamFileName_{"cfg_param"};
    const std::string       strCfgParamFileNameEx_{"cfg_param.xml"};

    bool                    isCheckWorkCfgParamFileModManually_{false};              /* 是否校验人工修改 */
    std::string             strWorkCfgParamFileDir_;

    void SetWorkCfgParamFileDir(void);
};

/**************************************************************************************
功能描述: 配置参数文件（提供清扫车参数文件的读写接口）
修改记录:
**************************************************************************************/
class CFG_PARAM_FILE_C
{
public:
    CFG_PARAM_FILE_C(const std::string &strCfgParamFileDir = ""): objCfgParamFileMng_(strCfgParamFileDir)
    {
    }

    int Read(CFG_PARAM_STRU &stParam);
    int Save(const CFG_PARAM_STRU &stParam);

private:
    const CFG_PARAM_FILE_MNG_C objCfgParamFileMng_;
};




}

}



#endif

