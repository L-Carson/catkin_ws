#ifndef __COMM_CFG_SMOOTHER_H__
#define __COMM_CFG_SMOOTHER_H__

#include "comm/comm_cfg/comm_cfg_param_file.h"
#include "comm/comm_file.h"
#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include "comm/loc_xml/tinyxml.h"

#include <string>
#include <list>
#include <vector>

namespace COMM
{

namespace CFG
{

using UPGRADE_FUNC = INT32(*)(TiXmlElement &);

struct UPGRADE_REG_STRU
{
    const std::string  strTgtVer;           /* 升级目标版本 */
    const UPGRADE_FUNC pfUpgrade;           /* 本分支升级函数 */
    const std::string  strMergeVer;         /* 被收编版本 */
    const UPGRADE_FUNC pfMergeUpgrade;      /* 收编升级函数 */
};

struct UPGRADE_STEP_STRU
{
    std::string  strSrcVer;
    UPGRADE_FUNC pfUpgrade;
};


/**************************************************************************************
功能描述: 升级关系管理
修改记录:
**************************************************************************************/
class UPGRADE_PATH_MNG_C
{
public:
    INT32 Init(void);

    void  GetCfgVers(std::vector<std::string> &vectCfgVer) const;
    INT32 SearchPath(const std::vector<std::string> &vectSrcCfgVer, const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const;

private:
    struct STEP_STRU
    {
        UINT32       srcCfgVerId;
        UPGRADE_FUNC pfUpgrade;
    };

    struct NODE_STRU
    {
        UINT32                 cfgVerId;
        std::string            strCfgVer;
        std::vector<STEP_STRU> vectStep;
    };

    struct SEARCH_NODE_STRU
    {
        UINT32       tgtNodeIdx;        /* 升级目标版本的节点索引 */
        UINT32       srcCfgVerId;
        UPGRADE_FUNC pfUpgrade;
    };

    std::map<std::string, UINT32>   mapCfgVerId_;
    std::vector<NODE_STRU>          vectUpgradeNode_;

    INT32 CheckBranchUpgradeRegTabs(void) const;
    INT32 CheckBaseVerUpgradeRegInfo(void) const;
    INT32 CheckCurVerUpgradeRegInfo(void) const;

    INT32 InitCfgVerIdTab(std::map<std::string, UINT32> &mapCfgVerId);
    INT32 InitUpgradeTab(const std::map<std::string, UINT32> &mapCfgVerId, std::vector<NODE_STRU> &vectUpgradeNode);

    inline UINT32           GetCfgVerId(const std::string &strCfgVer) const { auto iter = mapCfgVerId_.find(strCfgVer); return (iter == mapCfgVerId_.end()) ? UINT32_NULL : iter->second; }
    inline UINT32           GetUpgradeNodeNum(void)                   const { return UINT32(vectUpgradeNode_.size()); }
    inline const NODE_STRU &GetUpgradeNode(UINT32 cfgVerId)           const { return vectUpgradeNode_[cfgVerId]; }
    inline std::string      GetCfgVerById(UINT32 cfgVerId)            const { return (cfgVerId < GetUpgradeNodeNum()) ? GetUpgradeNode(cfgVerId).strCfgVer : ""; }

    INT32 Search(const std::vector<UINT32> &vectSrcCfgVerId, UINT32 tgtCfgVerId, std::vector<STEP_STRU> &vectUpgradeStep) const;

};


/**************************************************************************************
功能描述: 配置文件平滑器
          在版本升级和降级过程中，平滑配置文件
修改记录:
**************************************************************************************/
class CFG_SMOOTHER_C
{
public:
    INT32 Init(void);
    INT32 Exec(BOOL &isUpdate) const;

private:
    struct SEARCH_NODE_STRU
    {
        UINT32                   nodeIdxOfTgtVer;     /* 升级目标版本的节点索引 */
        const UPGRADE_STEP_STRU *pstUpgradeStep;
    };

    struct UPGRADE_STATE_STRU
    {
        BOOL isRsvOldVerActCfgFile = false;
        BOOL isGenNewVerRsvCfgFile = false;
    };

    using UPGTADE_TAB_MAP = std::map<std::string, std::vector<UPGRADE_STEP_STRU>>;

    FILE_OPRT_C          objFileOprt_;
    CFG_PARAM_FILE_MNG_C objCfgParamFileMng_;
    UPGRADE_PATH_MNG_C   objUpgradePathMng_;

    /**************************************************************************************
    功能描述: 配置文件操作
    修改记录:
    **************************************************************************************/
    BOOL  IsRsvCfgFileExist(const std::string &strCfgVer) const;
    INT32 GetCfgFileVer(const std::string &strCfgFilePath, std::string &strCfgVer) const;
    INT32 GetActCfgFileVer(std::string &strCfgVer) const;
    INT32 CheckRsvCfgFile(const std::string &strCfgVer) const;
    INT32 RsvActCfgFile(const std::string &strRsvActCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const;
    INT32 ActRsvCfgFile(const std::string &strOldActCfgVer, const std::string &strNewActCfgVer) const;
    void  Recover(const std::string &strOldActCfgVer, const std::string &strNewActCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const;

    /**************************************************************************************
    功能描述: 升级关系管理
    修改记录:
    **************************************************************************************/
    void  PrintUpgradePath(const std::string &strTgtCfgVer, const std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const;
    void  GetRsvCfgFileVers(std::vector<std::string> &vectRsvCfgVer) const;
    INT32 GetUpgradePathFromSrcCfgVer(const std::string &strSrcCfgVer, const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const;
    INT32 GetUpgradePathFromRsvCfgVers(const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const;
    INT32 GetUpgradePath(const std::string &strOldActCfgVer, const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const;

    /**************************************************************************************
    功能描述: 升级配置文件
    修改记录:
    **************************************************************************************/
    INT32 UpgradeCfgByPath(const std::vector<UPGRADE_STEP_STRU> &vectUpgradePath, const std::string &strTgtCfgVer, TiXmlElement &objXmlCfg) const;
    INT32 UpgradeRsvCfgFileByPath(const std::vector<UPGRADE_STEP_STRU> &vectUpgradePath, const std::string &strTgtCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const;
    INT32 UpgradeRsvCfgFile(const std::string &strOldActCfgVer, const std::string &strTgtCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const;
    INT32 Upgrade(const std::string &strOldActCfgVer, const std::string &strTgtCfgVer) const;
};


}

}

#endif

