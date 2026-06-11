
#include "comm/comm_define.h"
#include "comm/comm_cfg/comm_cfg_smoother.h"

#include "comm_cfg_smooth_master.cpp"
#include "comm_cfg_smooth_example.cpp"
#include "comm_cfg_smooth_release_203.cpp"
#include "comm_cfg_smooth_release_247.cpp"

namespace COMM
{

namespace CFG
{

struct BRANCH_UPGRADE_REG_TAB_STRU
{
    const UPGRADE_REG_STRU *pstUpgradeTab;
    UINT32                  upgradeTabSize;
};

const std::string STR_BASE_CFG_VER = "V00.00.000.000";

/**************************************************************************************
功能描述: 各分支升级汇总表
修改记录:
**************************************************************************************/
const BRANCH_UPGRADE_REG_TAB_STRU g_astBranchUpgradeRegTab[] =
{
    /* 【重要说明】
            （1）每个分支，如果在该分支上存在配置参数变更，都需要维护一个升级注册表，来进行参数平滑升级。
            （2）每个分支的升级注册表都必须汇总注册填写到本汇总表中，本汇总表中各分支表无顺序要求。
            （3）每个分支的升级注册表及其相关升级函数都维护在一个文件中，方便后续进行分支间升级合并操作。
    */

    /* 【分支注册表新建说明】
            （1）新建条件：新的分支已经拉出，且配置需要进行升级变更。如果拉了分支，暂无参数变更，不要新建对应注册表。

            （2）新建步骤：
                    （a）修改新分支的父分支升级注册表。
                            父分支升级注册表中最后一行，其版本一般还是默认的 CFG_VERSION ，升级函数命名域也还是默认的 NEW_CFG_VER。
                            将它们修改成对应版本的字符串和名称，与其它行要求一致。

                    （b）拷贝升级模板文件。
                            升级模板文件为 comm_cfg_smooth_example.cpp，拷贝模板文件，并按照新分支命名。
                            新分支升级模板文件命名规则为 comm_cfg_smooth_xxx.cpp，xxx为分支简化名。
                            比如203分支的为 comm_cfg_smooth_release_203.cpp。

                    （c）填写初始注册内容。
                            参考模板的内容填写第一行，并删除模板中多余内容。
                            分支注册表全局变量建议命名规则为 g_astUpgradeTabOfxxx，xxx为分支简化名，与文件名一致。
                            比如203分支的为 g_astUpgradeTabOfRelease203.
                            注册表第一行，表示配置升级来源，都只需要注册分支收编升级信息。
                            注册表修改详见升级表出的【配置升级注册说明】。

                    （d）将分支注册表文件include到本文件。
                            为了简化处理，当前直接讲各分支注册表文件直接include到本文件，一起编译，不用修改cmake文件。
                            在本文件最上方，添加 include 语句，比如: #include comm_cfg_smooth_release_203.cpp

                    （e）汇总表注册。
                            将新的注册表全局变量注册到汇总表。
    */

    /* 【分支注册表收编说明】
            （1）收编条件：
                    这里收编指支持从一个分支版本升级到另一个分支版本。（非新拉分支的场景）
                    收编的目标版本一定要比被收编版本的版本号大，不允许小版本号收编大版本号。
                    一般都是     master 分支收编其它分支。建议不做或少做非master分支间收编，不好维护。
                    两个分支都支持本收编机制，且按照本机制进行了模块和代码改造。

            （2）收编步骤：
                    （a）修改本分支升级注册表。
                            升级注册表中最后一行，其版本一般还是默认的 CFG_VERSION ，升级函数命名域也还是默认的 NEW_CFG_VER。
                            将它们修改成对应版本的字符串和名称，与其它行要求一致。

                    （b）合并本模块。
                            将被收编的分支中本模块（comm_cfg目录）内容合并到目标分支的本模块中。
                            有新文件，则将文件拷贝过来；有老文件新内容，则将新内容拷贝过来；老文件修改问题则根据修改内容进行同步；
                            本步骤的目的，及确保收编目标分支代码同时支持两个分支下的版本升级。

                    （c）修改收编分支升级注册表。
                            升级注册表中最后一行，其版本一般还是默认的 CFG_VERSION ，升级函数命名域也还是默认的 NEW_CFG_VER。
                            将它们修改成对应版本的字符串和名称，与其它行要求一致。

                    （d）添加收编注册项。
                            新增加一个版本升级注册项，与分支内升级注册项的差别，就是分支收编注册信息不为空。
                            如果配置相对没有变更，则升级函数中的处理可以为空，直接返回成功。
                            注册表修改详见升级表出的【配置升级注册说明】。

                    （e）重复收编。
                            同一个分支可以被另一个分支收编多次。
                            被收编分支存在后续版本且配置变更，可以再次被收编。
                            重复步骤a~d。
    */

    /****** 分支注册表首地址                       分支注册表中注册项个数 ***********************/
    {g_astUpgradeTabOfMaster,           ARRAY_SIZE(g_astUpgradeTabOfMaster)},
    {g_astUpgradeTabOfExample,          ARRAY_SIZE(g_astUpgradeTabOfExample)},
    {g_astUpgradeTabOfRelease203,       ARRAY_SIZE(g_astUpgradeTabOfRelease203)},
    {g_astUpgradeTabOfRelease247,       ARRAY_SIZE(g_astUpgradeTabOfRelease247)}
};

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::Init(void)
{
    ST_LOG_INFO("Begin...");

    INT32 rslt = CheckBranchUpgradeRegTabs();
    if (rslt != 0) {
        ST_LOG_FATAL("CheckBranchUpgradeRegTabs() Fail.");
        return -1;
    }

    rslt = CheckBaseVerUpgradeRegInfo();
    if (rslt != 0) {
        ST_LOG_FATAL("CheckBaseVerUpgradeRegInfo() Fail.");
        return -1;
    }

    rslt = CheckCurVerUpgradeRegInfo();
    if (rslt != 0) {
        ST_LOG_FATAL("CheckCurVerUpgradeRegInfo() Fail.");
        return -1;
    }

    rslt = InitCfgVerIdTab(mapCfgVerId_);
    if (rslt != 0) {
        ST_LOG_FATAL("InitCfgVerIdTab() Fail.");
        return -1;
    }

    rslt = InitUpgradeTab(mapCfgVerId_, vectUpgradeNode_);
    if (rslt != 0) {
        ST_LOG_FATAL("InitUpgradeTab() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 获取所有版本
修改记录:
**************************************************************************************/
void UPGRADE_PATH_MNG_C::GetCfgVers(std::vector<std::string> &vectCfgVer) const
{
    vectCfgVer.clear();
    vectCfgVer.reserve(mapCfgVerId_.size());

    for (const auto &objPair : mapCfgVerId_) {
        vectCfgVer.emplace_back(objPair.first);
    }
}

/**************************************************************************************
功能描述: 获取升级路径
          返回成功，仅表示过程没发生异常，不表示搜索到了可行的路径。
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::SearchPath(const std::vector<std::string> &vectSrcCfgVer, const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const
{
    ST_LOG_INFO("Begin...");

    /* 将版本字符串转换成ID */
    std::vector<UINT32> vectSrcCfgVerId;
    for (const std::string &strSrcCfgVer : vectSrcCfgVer) {
        UINT32 srcCfgVerId = GetCfgVerId(strSrcCfgVer);
        if (srcCfgVerId == UINT32_NULL) {
            ST_LOG_FATAL("GetCfgVerId(SrcCfgVer = %s) Fail. TgtCfgVer = %s.", strSrcCfgVer.c_str(), strTgtCfgVer.c_str());
            return -1;
        }

        vectSrcCfgVerId.push_back(srcCfgVerId);
    }

    UINT32 tgtCfgVerId = GetCfgVerId(strTgtCfgVer);
    if (tgtCfgVerId == UINT32_NULL) {
        ST_LOG_FATAL("GetCfgVerId(TgtCfgVer = %s) Fail.", strTgtCfgVer.c_str());
        return -1;
    }

    /* 搜索 */
    std::vector<STEP_STRU> vectUpgradeStepTmp;
    INT32 rslt = Search(vectSrcCfgVerId, tgtCfgVerId, vectUpgradeStepTmp);
    if (rslt != 0) {
        ST_LOG_FATAL("Search(SrcCfgNum = %u, TgtCfgVer = %s, TgtCfgVerId = %u) Fail.", UINT32(vectSrcCfgVer.size()), strTgtCfgVer.c_str(), tgtCfgVerId);
        return -1;
    }

    /* 将版本字符串ID转换成原始字符串 */
    for (const STEP_STRU &stStepTmp : vectUpgradeStepTmp) {
        std::string strSrcCfgVer = GetCfgVerById(stStepTmp.srcCfgVerId);
        if (strSrcCfgVer == "") {
            ST_LOG_FATAL("GetCfgVerById(%u) Fail. TgtCfgVer = %s, TgtCfgVerId = %u.", stStepTmp.srcCfgVerId, strTgtCfgVer.c_str(), tgtCfgVerId);
            return -1;
        }

        vectUpgradePath.emplace_back(UPGRADE_STEP_STRU{strSrcCfgVer, stStepTmp.pfUpgrade});
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 检查各分支注册表
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::CheckBranchUpgradeRegTabs(void) const
{
    for (UINT32 loopOfBranch = 0; loopOfBranch < ARRAY_SIZE(g_astBranchUpgradeRegTab); ++loopOfBranch) {
        const BRANCH_UPGRADE_REG_TAB_STRU &stBranchUpgradeRegTab = g_astBranchUpgradeRegTab[loopOfBranch];
        R_ASSERT_2(stBranchUpgradeRegTab.pstUpgradeTab != nullptr, -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        R_ASSERT_2(stBranchUpgradeRegTab.upgradeTabSize > 0,       -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

        /* 分支注册表中，第一行表示分支配置升级来源，要么从基础版本升级过来，要么从其他版本收编过来，二者只能且必取其一。 */
        const UPGRADE_REG_STRU &stFirstRegItem = stBranchUpgradeRegTab.pstUpgradeTab[0];
        R_ASSERT_2((stFirstRegItem.pfUpgrade != nullptr) != (stFirstRegItem.pfMergeUpgrade != nullptr), -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

        if (stFirstRegItem.pfUpgrade != nullptr) {
            R_ASSERT_2(stFirstRegItem.strTgtVer > STR_BASE_CFG_VER,           -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        } else {
            R_ASSERT_2(stFirstRegItem.strMergeVer > STR_BASE_CFG_VER,         -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
            R_ASSERT_2(stFirstRegItem.strTgtVer > stFirstRegItem.strMergeVer, -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        }

        /* 分支注册表中，其它行，分支内升级函数不能为空，分支间收编可选。 */
        const std::string *pstrPreVer = &stFirstRegItem.strTgtVer;
        for (UINT32 loopOfItem = 1; loopOfItem < stBranchUpgradeRegTab.upgradeTabSize; ++loopOfItem) {
            const UPGRADE_REG_STRU &stUpgradeRegItem = stBranchUpgradeRegTab.pstUpgradeTab[loopOfItem];

            /* 版本号必须升序排列 */
            R_ASSERT_3(stUpgradeRegItem.strTgtVer > *pstrPreVer,    -1, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
            R_ASSERT_3(stUpgradeRegItem.pfUpgrade != nullptr,       -1, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

            if (stUpgradeRegItem.pfMergeUpgrade != nullptr) {
                R_ASSERT_3(stUpgradeRegItem.strMergeVer > STR_BASE_CFG_VER,           -1, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
                R_ASSERT_3(stUpgradeRegItem.strTgtVer > stUpgradeRegItem.strMergeVer, -1, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
            }

            pstrPreVer = &stUpgradeRegItem.strTgtVer;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 检查基础版本升级注册信息
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::CheckBaseVerUpgradeRegInfo(void) const
{
    /* 每个表的第一行，就是该分支的来源，其中只有主分支是从基础版本升级过来，其它分支都是从另一个分支收编过来的。 */
    UINT32 baseVerUpgradeRegNum = 0;

    for (UINT32 loopOfBranch = 0; loopOfBranch < ARRAY_SIZE(g_astBranchUpgradeRegTab); ++loopOfBranch) {
        const BRANCH_UPGRADE_REG_TAB_STRU &stBranchUpgradeRegTab = g_astBranchUpgradeRegTab[loopOfBranch];
        R_ASSERT_2(stBranchUpgradeRegTab.pstUpgradeTab != nullptr, -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        R_ASSERT_2(stBranchUpgradeRegTab.upgradeTabSize > 0,       -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

        const UPGRADE_REG_STRU &stFirstUpgradeRegItem = stBranchUpgradeRegTab.pstUpgradeTab[0];
        if (stFirstUpgradeRegItem.pfUpgrade != nullptr) {
            ++baseVerUpgradeRegNum;
        }
    }

    R_ASSERT_1(baseVerUpgradeRegNum == 1, -1, baseVerUpgradeRegNum);
    return 0;
}

/**************************************************************************************
功能描述: 检查当前版本升级注册信息
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::CheckCurVerUpgradeRegInfo(void) const
{
    /* 当前软件的配置版本的升级注册信息必须是在某个分支注册表的最后一行。 */
    for (UINT32 loopOfBranch = 0; loopOfBranch < ARRAY_SIZE(g_astBranchUpgradeRegTab); ++loopOfBranch) {
        const BRANCH_UPGRADE_REG_TAB_STRU &stBranchUpgradeRegTab = g_astBranchUpgradeRegTab[loopOfBranch];
        R_ASSERT_2(stBranchUpgradeRegTab.pstUpgradeTab != nullptr, -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        R_ASSERT_2(stBranchUpgradeRegTab.upgradeTabSize > 0,       -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

        const UPGRADE_REG_STRU &stLastUpgradeRegItem = stBranchUpgradeRegTab.pstUpgradeTab[stBranchUpgradeRegTab.upgradeTabSize - 1];
        if (stLastUpgradeRegItem.strTgtVer == CFG_VERSION) {
            return 0;
        }
    }

    return -1;
}

/**************************************************************************************
功能描述: 初始化配置版本号表
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::InitCfgVerIdTab(std::map<std::string, UINT32> &mapCfgVerId)
{
    ST_LOG_INFO("Begin...");

    mapCfgVerId.clear();
    mapCfgVerId[STR_BASE_CFG_VER] = 0;

    for (UINT32 loopOfBranch = 0; loopOfBranch < ARRAY_SIZE(g_astBranchUpgradeRegTab); ++loopOfBranch) {
        const BRANCH_UPGRADE_REG_TAB_STRU &stBranchUpgradeRegTab = g_astBranchUpgradeRegTab[loopOfBranch];
        R_ASSERT_2(stBranchUpgradeRegTab.pstUpgradeTab != nullptr, -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        R_ASSERT_2(stBranchUpgradeRegTab.upgradeTabSize > 0,       -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

        for (UINT32 loopOfItem = 0; loopOfItem < stBranchUpgradeRegTab.upgradeTabSize; ++loopOfItem) {
            const UPGRADE_REG_STRU &stUpgradeRegItem = stBranchUpgradeRegTab.pstUpgradeTab[loopOfItem];

            /* 版本号注册项全局不能重复 */
            auto iter = mapCfgVerId.find(stUpgradeRegItem.strTgtVer);
            R_ASSERT_3(iter == mapCfgVerId.end(), -1, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

            mapCfgVerId[stUpgradeRegItem.strTgtVer] = UINT32(mapCfgVerId.size());
        }
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 初始化升级表
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::InitUpgradeTab(const std::map<std::string, UINT32> &mapCfgVerId, std::vector<NODE_STRU> &vectUpgradeNode)
{
    ST_LOG_INFO("Begin...");

    /* 初始化基础版本的升级节点 */
    UINT32 upgradeNodeNum = UINT32(mapCfgVerId.size());
    vectUpgradeNode.resize(upgradeNodeNum, NODE_STRU{UINT32_NULL, "", std::vector<STEP_STRU>()});

    auto iterOfBaseCfgVerId = mapCfgVerId.find(STR_BASE_CFG_VER);
    R_ASSERT(iterOfBaseCfgVerId != mapCfgVerId.end(), -1);
    UINT32 baseCfgVerId = iterOfBaseCfgVerId->second;
    R_ASSERT_2(baseCfgVerId < upgradeNodeNum, -1, baseCfgVerId, upgradeNodeNum);
    NODE_STRU &stBaseVerNode = vectUpgradeNode[baseCfgVerId];
    stBaseVerNode.cfgVerId  = baseCfgVerId;
    stBaseVerNode.strCfgVer = STR_BASE_CFG_VER;

    /* 初始化其它版本的升级节点 */
    for (UINT32 loopOfBranch = 0; loopOfBranch < ARRAY_SIZE(g_astBranchUpgradeRegTab); ++loopOfBranch) {
        const BRANCH_UPGRADE_REG_TAB_STRU &stBranchUpgradeRegTab = g_astBranchUpgradeRegTab[loopOfBranch];
        R_ASSERT_2(stBranchUpgradeRegTab.pstUpgradeTab != nullptr, -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);
        R_ASSERT_2(stBranchUpgradeRegTab.upgradeTabSize > 0,       -1, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

        for (UINT32 loopOfItem = 0; loopOfItem < stBranchUpgradeRegTab.upgradeTabSize; ++loopOfItem) {
            const UPGRADE_REG_STRU &stUpgradeRegItem = stBranchUpgradeRegTab.pstUpgradeTab[loopOfItem];

            auto iterOfTgtCfgVerId = mapCfgVerId.find(stUpgradeRegItem.strTgtVer);
            R_ASSERT_4(iterOfTgtCfgVerId != mapCfgVerId.end(), -1, upgradeNodeNum, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

            UINT32 tgtCfgVerId = iterOfTgtCfgVerId->second;
            R_ASSERT_4(tgtCfgVerId < upgradeNodeNum, -1, tgtCfgVerId, upgradeNodeNum, loopOfItem, loopOfBranch);

            NODE_STRU &stTgtCfgVerNode = vectUpgradeNode[tgtCfgVerId];
            R_ASSERT_4(stTgtCfgVerNode.cfgVerId == UINT32_NULL, -1, tgtCfgVerId, stTgtCfgVerNode.cfgVerId, loopOfItem, loopOfBranch);

            stTgtCfgVerNode.cfgVerId  = tgtCfgVerId;
            stTgtCfgVerNode.strCfgVer = stUpgradeRegItem.strTgtVer;

            if (stUpgradeRegItem.pfUpgrade != nullptr) {
                const std::string &strSrcCfgVer = (loopOfItem == 0) ? STR_BASE_CFG_VER : stBranchUpgradeRegTab.pstUpgradeTab[loopOfItem - 1].strTgtVer;

                auto iter = mapCfgVerId.find(strSrcCfgVer);
                R_ASSERT_4(iter != mapCfgVerId.end(), -1, tgtCfgVerId, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

                UINT32 srcCfgVerId = iter->second;
                R_ASSERT_4(srcCfgVerId < upgradeNodeNum, -1, srcCfgVerId, upgradeNodeNum, loopOfItem, loopOfBranch);

                stTgtCfgVerNode.vectStep.emplace_back(STEP_STRU{srcCfgVerId, stUpgradeRegItem.pfUpgrade});
            }

            if (stUpgradeRegItem.pfMergeUpgrade != nullptr) {
                auto iter = mapCfgVerId.find(stUpgradeRegItem.strMergeVer);
                R_ASSERT_4(iter != mapCfgVerId.end(), -1, tgtCfgVerId, loopOfItem, loopOfBranch, stBranchUpgradeRegTab.upgradeTabSize);

                UINT32 mergeCfgVerId = iter->second;
                R_ASSERT_4(mergeCfgVerId < upgradeNodeNum, -1, mergeCfgVerId, upgradeNodeNum, loopOfItem, loopOfBranch);

                stTgtCfgVerNode.vectStep.emplace_back(STEP_STRU{mergeCfgVerId, stUpgradeRegItem.pfMergeUpgrade});
            }
        }
    }

    /* 校验节点是否正常 */
    for (UINT32 loop = 0; loop < upgradeNodeNum; ++loop) {
        const NODE_STRU &stNode = vectUpgradeNode[loop];
        R_ASSERT_3(stNode.cfgVerId < upgradeNodeNum, -1, loop, stNode.cfgVerId, upgradeNodeNum);
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 搜索
修改记录:
**************************************************************************************/
INT32 UPGRADE_PATH_MNG_C::Search(const std::vector<UINT32> &vectSrcCfgVerId, UINT32 tgtCfgVerId, std::vector<STEP_STRU> &vectUpgradeStep) const
{
    ST_LOG_INFO("Begin...");

    UINT32 upgradeNodeNum = GetUpgradeNodeNum();
    R_ASSERT_3(tgtCfgVerId < upgradeNodeNum,  -1, tgtCfgVerId, upgradeNodeNum, vectSrcCfgVerId.size());

    vectUpgradeStep.clear();

    /* 初始化状态表 */
    std::vector<BOOL> vectIsCfgVerSearched(upgradeNodeNum, false);
    std::vector<BOOL> vectIsSrcCfgVer(upgradeNodeNum, false);

    for (UINT32 srcCfgVerId : vectSrcCfgVerId) {
        R_ASSERT_4(srcCfgVerId < upgradeNodeNum, -1, srcCfgVerId, tgtCfgVerId, upgradeNodeNum, vectSrcCfgVerId.size());
        R_ASSERT_4(srcCfgVerId != tgtCfgVerId,   -1, srcCfgVerId, tgtCfgVerId, upgradeNodeNum, vectSrcCfgVerId.size());
        vectIsSrcCfgVer[srcCfgVerId] = true;
    }

    /* 初始化搜索表 */
    std::vector<SEARCH_NODE_STRU> vectSearchNode;

    vectSearchNode.reserve(upgradeNodeNum);
    vectSearchNode.emplace_back(SEARCH_NODE_STRU{UINT32_NULL, tgtCfgVerId, nullptr});
    vectIsCfgVerSearched[tgtCfgVerId] = true;

    /* 搜索步骤最少的升级路径 */
    UINT32 nodeIdxOfSrcCfg = UINT32_NULL;
    for (UINT32 nodeIdx = 0; nodeIdx < vectSearchNode.size(); ++nodeIdx) {
        const SEARCH_NODE_STRU &stSearchNode = vectSearchNode[nodeIdx];
        R_ASSERT_4(stSearchNode.srcCfgVerId < upgradeNodeNum, -1, stSearchNode.srcCfgVerId, upgradeNodeNum, nodeIdx, stSearchNode.tgtNodeIdx);
        if (vectIsSrcCfgVer[stSearchNode.srcCfgVerId]) {
            nodeIdxOfSrcCfg = nodeIdx;
            break;
        }

        const NODE_STRU &stUpgradeNode = GetUpgradeNode(stSearchNode.srcCfgVerId);
        R_ASSERT_4(stUpgradeNode.cfgVerId == stSearchNode.srcCfgVerId, -1, stUpgradeNode.cfgVerId, stSearchNode.srcCfgVerId, upgradeNodeNum, nodeIdx);

        for (const STEP_STRU &stStep : stUpgradeNode.vectStep) {
            R_ASSERT_4(stStep.srcCfgVerId < upgradeNodeNum, -1, stStep.srcCfgVerId, upgradeNodeNum, stUpgradeNode.cfgVerId, nodeIdx);
            if (vectIsCfgVerSearched[stStep.srcCfgVerId]) {
                continue;
            }

            vectSearchNode.emplace_back(SEARCH_NODE_STRU{nodeIdx, stStep.srcCfgVerId, stStep.pfUpgrade});
            vectIsCfgVerSearched[stStep.srcCfgVerId] = true;
        }
    }

    /* 提取搜索结果，不含目标版本 */
    UINT32 searchNodeNum = UINT32(vectSearchNode.size());
    if (nodeIdxOfSrcCfg < searchNodeNum) {
        vectUpgradeStep.reserve(nodeIdxOfSrcCfg);

        for (const SEARCH_NODE_STRU *pstNode = &vectSearchNode[nodeIdxOfSrcCfg]; pstNode->tgtNodeIdx < searchNodeNum; pstNode = &vectSearchNode[pstNode->tgtNodeIdx]) {
            vectUpgradeStep.emplace_back(STEP_STRU{pstNode->srcCfgVerId, pstNode->pfUpgrade});
        }
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::Init(void)
{
    ST_LOG_INFO("Begin...");

    INT32 rslt = objUpgradePathMng_.Init();
    if (rslt != 0) {
        ST_LOG_FATAL("objUpgradePathMng_.Init() Fail.");
        ST_LOG_FATAL("升级表存在问题，请联系开发同事。");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 平滑升级执行
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::Exec(BOOL &isUpdate) const
{
    ST_LOG_INFO("Begin...");
    isUpdate = false;

    /* 如果版本匹配，不用进行升级 */
    std::string strSoftwareCfgVer = CFG_VERSION;
    std::string strOldActCfgVer;
    INT32 rslt = GetActCfgFileVer(strOldActCfgVer);
    if (rslt != 0) {
        ST_LOG_ERR("GetActCfgFileVer() Fail.");
        ST_LOG_ERR("获取老的激活配置文件的版本号失败。");
        return -1;
    }

    ST_LOG_INFO("老的激活配置文件的版本号是: %s，软件的配置版本是: %s.", strOldActCfgVer.c_str(), strSoftwareCfgVer.c_str());
    if (strOldActCfgVer == strSoftwareCfgVer) {
        ST_LOG_INFO("配置文件的版本和软件配置版本一致，不需要升级。");
        return 0;
    }

    /* 执行升级 */
    rslt = Upgrade(strOldActCfgVer, strSoftwareCfgVer);
    if (rslt != 0) {
        ST_LOG_ERR("Upgrade(%s->%s) Fail.", strOldActCfgVer.c_str(), strSoftwareCfgVer.c_str());
        ST_LOG_ERR("升级配置失败。");
        return -1;
    }

    isUpdate = true;
    ST_LOG_INFO("配置平滑升级成功。");
    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 判断指定版本的配置文件是否存在
修改记录:
**************************************************************************************/
BOOL CFG_SMOOTHER_C::IsRsvCfgFileExist(const std::string &strCfgVer) const
{
    const std::string strRsvCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strCfgVer);

    return objFileOprt_.IsExist(strRsvCfgFilePath);
}

/**************************************************************************************
功能描述: 获取配置文件版本号
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::GetCfgFileVer(const std::string &strCfgFilePath, std::string &strCfgVer) const
{
    TiXmlDocument objXmlCfgFile;
    INT32 rslt = objCfgParamFileMng_.ReadWorkCfgParamFile(strCfgFilePath, objXmlCfgFile);
    if (rslt != 0) {
        ST_LOG_ERR("Load File(%s) Fail.", strCfgFilePath.c_str());
        return -1;
    }

    TiXmlElement *pobjCfgRoot = objXmlCfgFile.RootElement();
    if (pobjCfgRoot == NULL) {
        ST_LOG_ERR("Get Root Element Fail.");
        return -1;
    }

    /* 基础版本中没有版本字段，只要没有该属性，都按照基本版本处理。 */
    rslt = pobjCfgRoot->QueryStringAttribute("ver", &strCfgVer);
    if (TIXML_SUCCESS != rslt) {
        ST_LOG_INFO("Attribute < ver > Not Exist, Set Base Ver.");
        strCfgVer = STR_BASE_CFG_VER;
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取激活配置文件的版本号
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::GetActCfgFileVer(std::string &strCfgVer) const
{
    strCfgVer = "";

    const std::string strActCfgFilePath = objCfgParamFileMng_.GetWorkCfgParamFilePath();
    if (!objFileOprt_.IsExist(strActCfgFilePath)) {
        ST_LOG_INFO("不存在激活配置文件(%s)。", strActCfgFilePath.c_str());
        return 0;
    }

    INT32 rslt = GetCfgFileVer(strActCfgFilePath, strCfgVer);
    if (rslt != 0) {
        ST_LOG_ERR("GetCfgFileVer(Path=%s) Fail.", strActCfgFilePath.c_str());
        ST_LOG_ERR("加载激活配置文件(%s)失败。", strActCfgFilePath.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 检查备份配置文件中版本与文件名是否一致
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::CheckRsvCfgFile(const std::string &strCfgVer) const
{
    const std::string strRsvCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strCfgVer);

    std::string strRsvCfgFileVer;
    INT32 rslt = GetCfgFileVer(strRsvCfgFilePath, strRsvCfgFileVer);
    if (rslt != 0) {
        ST_LOG_ERR("GetCfgFileVer(%s) Fail.", strRsvCfgFilePath.c_str());
        ST_LOG_ERR("读取备份配置文件(%s)版本失败，请检查该文件。", strRsvCfgFilePath.c_str());
        return -1;
    }

    if (strRsvCfgFileVer != strCfgVer) {
        ST_LOG_ERR("备份配置文件(%s)的版本(%s)与文件名不一致，请检查该文件。", strRsvCfgFilePath.c_str(), strRsvCfgFileVer.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 备份激活配置文件
          就是将当前导航正在使用的配置文件，拷贝一份，并且以其版本号重命名。
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::RsvActCfgFile(const std::string &strRsvActCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const
{
    ST_LOG_INFO("Begin...");

    /* 如果本来就没有激活配置文件，则不用备份 */
    if (strRsvActCfgVer == "") {
        ST_LOG_INFO("不用备份激活配置文件。");
        return 0;
    }

    const std::string strActCfgFilePath = objCfgParamFileMng_.GetWorkCfgParamFilePath();
    R_ASSERT(objFileOprt_.IsExist(strActCfgFilePath), -1);
    ST_LOG_INFO("备份激活配置文件< %s >。", strActCfgFilePath.c_str());

    /* 如果备份的文件名已经被占用，先删除该文件，再拷贝生成新的备份文件。 */
    const std::string strRsvCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strRsvActCfgVer);
    ST_LOG_INFO("激活配置文件的备份文件路径为: %s.", strRsvCfgFilePath.c_str());
    if (objFileOprt_.IsExist(strRsvCfgFilePath)) {
        ST_LOG_ERR("激活配置文件的备份文件已经存在，删除该文件。");

        INT32 rslt = objFileOprt_.DelFile(strRsvCfgFilePath);
        if (rslt != 0) {
            ST_LOG_ERR("激活配置文件的老备份文件< %s >无法删除，请检查其权限，或确认该文件无用后手动删除该问题。", strRsvCfgFilePath.c_str());
            return -1;
        }
    }

    INT32 rslt = objFileOprt_.Rename(strActCfgFilePath, strRsvCfgFilePath, true);
    if (rslt != 0) {
        ST_LOG_ERR("objFileOprt_.Copy(%s, %s) Fail.", strActCfgFilePath.c_str(), strRsvCfgFilePath.c_str());
        ST_LOG_ERR("备份文件(%s)为(%s)失败。请检查文件权限或文件是否损坏。", strActCfgFilePath.c_str(), strRsvCfgFilePath.c_str());
        return -1;
    }

    stUpgradeState.isRsvOldVerActCfgFile = true;
    ST_LOG_INFO("备份激活配置文件< %s >为< %s >成功。", strActCfgFilePath.c_str(), strRsvCfgFilePath.c_str());
    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 激活指定的备份配置文件
          就是删除当前激活配置文件，然后将将指定版本的备份的配置文件名修改为导航可以读取的文件名。
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::ActRsvCfgFile(const std::string &strOldActCfgVer, const std::string &strNewActCfgVer) const
{
    ST_LOG_INFO("Begin...");

    const std::string strActCfgFilePath = objCfgParamFileMng_.GetWorkCfgParamFilePath();
    R_ASSERT(!objFileOprt_.IsExist(strActCfgFilePath), -1);

    const std::string strRsvCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strNewActCfgVer);
    R_ASSERT(objFileOprt_.IsExist(strRsvCfgFilePath), -1);

    INT32 rslt = objFileOprt_.Rename(strRsvCfgFilePath, strActCfgFilePath, true);
    if (rslt != 0) {
        ST_LOG_ERR("objFileOprt_.Rename(%s, %s) Fail.", strRsvCfgFilePath.c_str(), strActCfgFilePath.c_str());
        ST_LOG_ERR("激活配置文件(%s)为(%s)失败。", strRsvCfgFilePath.c_str(), strActCfgFilePath.c_str());
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 恢复
修改记录:
**************************************************************************************/
void CFG_SMOOTHER_C::Recover(const std::string &strOldActCfgVer, const std::string &strNewActCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const
{
    if (stUpgradeState.isRsvOldVerActCfgFile) {
        /* 恢复老的激活配置文件 */
        const std::string strActCfgFilePath = objCfgParamFileMng_.GetWorkCfgParamFilePath();
        if (objFileOprt_.IsExist(strActCfgFilePath)) {
            INT32 rslt = objFileOprt_.DelFile(strActCfgFilePath);
            ST_LOG_ERR("Del Act Cfg File(%s) Rslt: %d.", strActCfgFilePath.c_str(), rslt);
        }

        const std::string strOldActRsvCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strOldActCfgVer);
        INT32 rslt = objFileOprt_.Rename(strOldActRsvCfgFilePath, strActCfgFilePath, true);
        ST_LOG_ERR("Recover Old Act File(%s) Rslt: %d.", strOldActRsvCfgFilePath.c_str(), rslt);
    }

    if (stUpgradeState.isGenNewVerRsvCfgFile) {
        /* 清除新生成的目标版本的备份配置文件 */
        const std::string strNewResvCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strNewActCfgVer);
        INT32 rslt = objFileOprt_.DelFile(strNewResvCfgFilePath);
        ST_LOG_ERR("Del New Rsv Cfg File(%s) Rslt: %d.", strNewResvCfgFilePath.c_str(), rslt);
    }

    stUpgradeState.isRsvOldVerActCfgFile = false;
    stUpgradeState.isGenNewVerRsvCfgFile = false;
}

/**************************************************************************************
功能描述: 打印升级路径
修改记录:
**************************************************************************************/
void CFG_SMOOTHER_C::PrintUpgradePath(const std::string &strTgtCfgVer, const std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const
{
    ST_LOG_INFO("升级步骤数为%u，具体步骤如下:", UINT32(vectUpgradePath.size()));

    for (const UPGRADE_STEP_STRU &stUpgradeStep : vectUpgradePath) {
        ST_LOG_INFO("%s -->", stUpgradeStep.strSrcVer.c_str());
    }

    ST_LOG_INFO("--> %s", strTgtCfgVer.c_str());
    ST_LOG_INFO("升级步骤结束。");
}

/**************************************************************************************
功能描述: 获取备份的版本
修改记录:
**************************************************************************************/
void CFG_SMOOTHER_C::GetRsvCfgFileVers(std::vector<std::string> &vectRsvCfgVer) const
{
    /* 查看升级表中所有存在升级注册的，并且存在备份文件的版本。 */
    std::vector<std::string> vectCfgVer;
    objUpgradePathMng_.GetCfgVers(vectCfgVer);

    for (const std::string &strCfgVer : vectCfgVer) {
        if (!IsRsvCfgFileExist(strCfgVer)) {
            continue;
        }

        INT32 rslt = CheckRsvCfgFile(strCfgVer);
        if (rslt != 0) {
            ST_LOG_ERR("CheckRsvCfgFile() Fail.");
            continue;
        }

        vectRsvCfgVer.emplace_back(strCfgVer);
    }

    ST_LOG_INFO("Rsv Cfg Ver Num Is %u:", UINT32(vectRsvCfgVer.size()));
    for (const std::string &strRsvCfgVer : vectRsvCfgVer) {
        ST_LOG_INFO(" -- %s", strRsvCfgVer.c_str());
    }
}

INT32 CFG_SMOOTHER_C::GetUpgradePathFromSrcCfgVer(const std::string &strSrcCfgVer, const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const
{
    std::vector<std::string> vectSrcCfgVer = {strSrcCfgVer};

    INT32 rslt = objUpgradePathMng_.SearchPath(vectSrcCfgVer, strTgtCfgVer, vectUpgradePath);
    if (rslt != 0) {
        ST_LOG_FATAL("objUpgradePathMng_.SearchPath(%s -> %s) Fail.", strSrcCfgVer.c_str(), strTgtCfgVer.c_str());
        return -1;
    }

    if (vectUpgradePath.empty()) {
        ST_LOG_ERR("Not Exist Upgrade Path From %s To %s.", strSrcCfgVer.c_str(), strTgtCfgVer.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 打印升级路径
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::GetUpgradePathFromRsvCfgVers(const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const
{
    std::vector<std::string> vectRsvCfgVer;
    GetRsvCfgFileVers(vectRsvCfgVer);
    if (vectRsvCfgVer.empty()) {
        ST_LOG_ERR("无可用的备份配置文件，无法从历史版本升级，请参考版本模板制作对应版本的配置文件。");
        return -1;
    }

    INT32 rslt = objUpgradePathMng_.SearchPath(vectRsvCfgVer, strTgtCfgVer, vectUpgradePath);
    if (rslt != 0) {
        ST_LOG_ERR("objUpgradePathMng_.SearchPath(%u -> %s) Fail.", UINT32(vectRsvCfgVer.size()), strTgtCfgVer.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取升级路径
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::GetUpgradePath(const std::string &strOldActCfgVer, const std::string &strTgtCfgVer, std::vector<UPGRADE_STEP_STRU> &vectUpgradePath) const
{
    ST_LOG_INFO("Begin...");

    /* 升级路径优先顺序为： */
    /*      1、如果目标备份配置文件存在，优先使用； */
    /*      2、如果老的激活配置文件存在，且目标版本号大于老版本号，优先选用从该版本开始升级； */
    /*      3、从其它历史备份配置文件开始升级； */
    if (IsRsvCfgFileExist(strTgtCfgVer)) {
        ST_LOG_INFO("升级目标配置版本的备份文件存在，升级路径为空。");

        vectUpgradePath.clear();
        INT32 rslt = CheckRsvCfgFile(strTgtCfgVer);
        if (rslt != 0) {
            ST_LOG_ERR("CheckRsvCfgFile(%s) Fail.", strTgtCfgVer.c_str());
            ST_LOG_ERR("目标版本的备份配置文件(%s)检查存在问题。", strTgtCfgVer.c_str());
            return -1;
        }

    } else if ((strOldActCfgVer != "") && (strTgtCfgVer > strOldActCfgVer)) {
        ST_LOG_INFO("存在老的激活配置文件，从老的激活配置文件升级。");

        INT32 rslt = GetUpgradePathFromSrcCfgVer(strOldActCfgVer, strTgtCfgVer, vectUpgradePath);
        if (rslt != 0) {
            ST_LOG_ERR("GetUpgradePathFromSrcCfgVer(%s -> %s) Fail.", strOldActCfgVer.c_str(), strTgtCfgVer.c_str());
            ST_LOG_ERR("搜索从老激活配置版本(%s)到新版本(%s)的升级路径失败，请联系开发确认是否能够平滑升级。如果不支持平滑升级且又要升级，请先手动删除或备份该激活配置文件，再重启。", strOldActCfgVer.c_str(), strTgtCfgVer.c_str());
            return -1;
        }

    } else {
        ST_LOG_INFO("从其它非激活版本的历史备份配置文件升级。");

        INT32 rslt = GetUpgradePathFromRsvCfgVers(strTgtCfgVer, vectUpgradePath);
        if (rslt != 0) {
            ST_LOG_ERR("GetUpgradePathFromRsvCfgVers(->%s) Fail.", strTgtCfgVer.c_str());
            ST_LOG_ERR("搜索从其它备份配置文件升级到新版本(%s)的升级路径失败，请确认是否存在合法的历史版本配置文件。如无，请手动制作新版本的配置文件。", strTgtCfgVer.c_str());
            return -1;
        }
    }

    PrintUpgradePath(strTgtCfgVer, vectUpgradePath);

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 升级配置
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::UpgradeCfgByPath(const std::vector<UPGRADE_STEP_STRU> &vectUpgradePath, const std::string &strTgtCfgVer, TiXmlElement &objXmlCfg) const
{
    ST_LOG_INFO("Begin...");

    /* 按照升级路径依次执行升级函数 */
    UINT32 upgradePathSize = INT32(vectUpgradePath.size());
    for (UINT32 loop = 0; loop < upgradePathSize; ++loop) {
        const UPGRADE_STEP_STRU &stUpgradeStep = vectUpgradePath[loop];
        R_ASSERT_2(stUpgradeStep.pfUpgrade != nullptr, -1, loop, upgradePathSize);

        std::string strStepTgtVer = ((loop + 1) < upgradePathSize) ? ((const UPGRADE_STEP_STRU &)vectUpgradePath[loop + 1]).strSrcVer : strTgtCfgVer;
        ST_LOG_INFO("Begin To Upgrade Step From < %s > To < %s >.", stUpgradeStep.strSrcVer.c_str(), strStepTgtVer.c_str());

        INT32 rslt = stUpgradeStep.pfUpgrade(objXmlCfg);
        if (rslt != 0) {
            ST_LOG_ERR("stUpgradeStep.pfUpgrade() Fail.");
            return -1;
        }

        ST_LOG_INFO("Upgrade Step Succ.");
    }

    objXmlCfg.SetAttribute("ver", strTgtCfgVer);

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 升级备份配置文件
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::UpgradeRsvCfgFileByPath(const std::vector<UPGRADE_STEP_STRU> &vectUpgradePath, const std::string &strTgtCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const
{
    ST_LOG_INFO("Begin...");

    /* 升级路径为空，表示不用升级操作。 */
    if (vectUpgradePath.empty()) {
        R_ASSERT(IsRsvCfgFileExist(strTgtCfgVer), -1);
        ST_LOG_INFO("Upgrade Path Is Null, No Need To Upgrade Rsv Cfg File.");
        return 0;
    }

    R_ASSERT(!IsRsvCfgFileExist(strTgtCfgVer), -1);

    const std::string strSrcCfgVer = ((const UPGRADE_STEP_STRU &)vectUpgradePath.front()).strSrcVer;
    ST_LOG_INFO("Src Cfg Ver %s, Tgt Cfg Ver %s.", strSrcCfgVer.c_str(), strTgtCfgVer.c_str());

    /* 加载原始版本配置文件 */
    TiXmlDocument     objXmlCfgFile;
    const std::string strSrcCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strSrcCfgVer);
    INT32 rsltOfReadCfgFile = objCfgParamFileMng_.ReadWorkCfgParamFile(strSrcCfgFilePath, objXmlCfgFile);
    R_ASSERT(rsltOfReadCfgFile == 0, -1);

    TiXmlElement *pobjCfgRoot = objXmlCfgFile.RootElement();
    R_ASSERT(pobjCfgRoot != nullptr, -1);

    /* 升级配置 */
    INT32 rslt = UpgradeCfgByPath(vectUpgradePath, strTgtCfgVer, *pobjCfgRoot);
    if (rslt != 0) {
        ST_LOG_ERR("UpgradeCfgByPath() Fail.");
        return -1;
    }

    /* 保存目标版本配置文件 */
    const std::string strTgtCfgFilePath = objCfgParamFileMng_.GetWorkResvCfgParamFilePath(strTgtCfgVer);
    rslt = objCfgParamFileMng_.SaveCfgParamFile(strTgtCfgFilePath, objXmlCfgFile);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgParamFileMng_.SaveCfgParamFile(%s) Fail.", strTgtCfgFilePath.c_str());
        return -1;
    }

    stUpgradeState.isGenNewVerRsvCfgFile = true;
    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 生成指定版本的备份配置文件
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::UpgradeRsvCfgFile(const std::string &strOldActCfgVer, const std::string &strTgtCfgVer, UPGRADE_STATE_STRU &stUpgradeState) const
{
    ST_LOG_INFO("Begin...");

    /* 找一条升级路径，然后使用该升级路径升级并生成指定目标版本的备份配置文件。 */
    std::vector<UPGRADE_STEP_STRU> vectUpgradePath;
    INT32 rslt = GetUpgradePath(strOldActCfgVer, strTgtCfgVer, vectUpgradePath);
    if (rslt != 0) {
        ST_LOG_ERR("GetUpgradePath(TgtCfgVer=%s) Fail.", strTgtCfgVer.c_str());
        return -1;
    }

    rslt = UpgradeRsvCfgFileByPath(vectUpgradePath, strTgtCfgVer, stUpgradeState);
    if (rslt != 0) {
        ST_LOG_ERR("UpgradeRsvCfgFileByPath(StepSize=%u, TgtCfgVer=%s) Fail.", UINT32(vectUpgradePath.size()), strTgtCfgVer.c_str());
        ST_LOG_ERR("升级配置文件(%s)失败，请联系开发工程师。", strTgtCfgVer.c_str());
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 升级
修改记录:
**************************************************************************************/
INT32 CFG_SMOOTHER_C::Upgrade(const std::string &strOldActCfgVer, const std::string &strTgtCfgVer) const
{
    ST_LOG_INFO("Begin...");

    UPGRADE_STATE_STRU stUpgradeState;

    /* 升级就是把老的配置文件备份，生成新的备份配置文件，然后把激活配置文件换成新版本的。 */
    INT32 rslt = RsvActCfgFile(strOldActCfgVer, stUpgradeState);
    if (rslt != 0) {
        ST_LOG_ERR("RsvActCfgFile() Fail.");
        ST_LOG_ERR("备份老的激活配置文件失败。");
        Recover(strOldActCfgVer, strTgtCfgVer, stUpgradeState);
        return -1;
    }

    rslt = UpgradeRsvCfgFile(strOldActCfgVer, strTgtCfgVer, stUpgradeState);
    if (rslt != 0) {
        ST_LOG_ERR("UpgradeRsvCfgFile(%s, %s) Fail.", strOldActCfgVer.c_str(), strTgtCfgVer.c_str());
        ST_LOG_ERR("生成软件配置版本< %s >的备份配置文件失败。", strTgtCfgVer.c_str());
        Recover(strOldActCfgVer, strTgtCfgVer, stUpgradeState);
        return -1;
    }

    rslt = ActRsvCfgFile(strOldActCfgVer, strTgtCfgVer);
    if (rslt != 0) {
        ST_LOG_ERR("ActRsvCfgFile(%s, %s) Fail.", strOldActCfgVer.c_str(), strTgtCfgVer.c_str());
        ST_LOG_ERR("激活软件配置文件< %s >失败。", strTgtCfgVer.c_str());
        Recover(strOldActCfgVer, strTgtCfgVer, stUpgradeState);
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}




}

}


