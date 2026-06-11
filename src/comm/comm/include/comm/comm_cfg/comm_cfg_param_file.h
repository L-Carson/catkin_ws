#ifndef __COMM_CFG_PARAM_FILE_H__
#define __COMM_CFG_PARAM_FILE_H__

#include "comm_cfg_param_define.h"

#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/loc_xml/tinyxml.h"

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
    CFG_PARAM_FILE_MNG_C(const std::string &strInputCfgParamFileDir = "") : strInputCfgParamFileDir_(strInputCfgParamFileDir),
                                                                            objFileOprt_(),
                                                                            objFileMng_(),
                                                                            objCfgFileMng_()
    {
        SetWorkCfgParamFileDir();
    }

    static void SetIgnoreCfgParamChecking(void);

    std::string GetWorkCfgParamFileDir(void) const;
    std::string GetChangeCfgParamFileDir(void) const;
    std::string GetChangeRecordCfgParamFileDir(void) const;

    std::string GetWorkCfgParamFilePath(void) const;
    std::string GetWorkResvCfgParamFilePath(const std::string &strCfgVer) const;
    std::string GetOldWorkCfgParamFilePath(void) const;
    std::string GetOldWorkResvCfgParamFilePath(const std::string &strCfgVer) const;

    std::string GetChangeCfgParamFilePath(void) const;

    INT32 ReadWorkCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    INT32 ReadChangeCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    INT32 ReadOldCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    INT32 SaveCfgParamFile(const std::string &strCfgParamFilePath, TiXmlDocument &objXmlCfgFile) const;
    INT32 BackupWorkCfgParamFile(void) const;

private:
    static std::atomic_bool isIgnorCfgCheckingWhenDbg_;

    const std::string       strInputCfgParamFileDir_;
    const std::string       strCfgParamFileName_{"cfg_param"};
    const std::string       strCfgParamFileNameEx_{"cfg_param.xml"};
    const FILE_OPRT_C       objFileOprt_;
    const FILE_MNG_C        objFileMng_;
    const CFG_FILE_MNG_C    objCfgFileMng_;

    BOOL                    isCheckWorkCfgParamFileModManually_{false};              /* 是否校验人工修改 */
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

    INT32 Read(CFG_PARAM_STRU &stParam);
    INT32 ReadLocMappingCfg(CFG_PARAM_STRU &stParam);
    INT32 ReadLidarCameraExCfg(CFG_PARAM_STRU &stParam);

    INT32 Save(const CFG_PARAM_STRU &stParam);

private:
    const CFG_PARAM_FILE_MNG_C objCfgParamFileMng_;
};




}

}



#endif

