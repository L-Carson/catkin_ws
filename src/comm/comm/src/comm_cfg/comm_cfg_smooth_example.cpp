
#include "comm/comm_cfg/comm_cfg_amend.h"
#include "comm/comm_cfg/comm_cfg_smoother.h"


namespace COMM
{

namespace CFG
{

namespace V01_11_237_001_VS_V01_11_237_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    INT32 rslt = objCfgAmend.SetAttribute(objCfg, {"Pnp", "ShortObstHold"}, "test1", 100);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_237_002 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    INT32 rslt = objCfgAmend.SetAttribute(objCfg, {"Pnp", "ShortObstHold"}, "test1", 200);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }

    rslt = objCfgAmend.SetAttribute(objCfg, {"Pnp", "ShortObstHold"}, "test2", 200);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

}

/**************************************************************************************
功能描述: 升级注册表
修改记录:
**************************************************************************************/
const UPGRADE_REG_STRU g_astUpgradeTabOfExample[] =
{
    /* 【重要说明】
            配置参数变更时，必须增加平滑处理，在此表中注册平滑处理信息。
            此表中的注册的版本，指的是配置版本号，配置版本号与软件版本不一样，详见当前配置版本 CFG_VERSION 定义。
            此表中的注册的函数，表示从表中上一个配置版本升级到新版本时需要调用注册函数对配置文件进行适配修改。

            本分支升级函数，表示从本分支低版本或基础版本升级过来的处理函数。
            收编分支升级函数，表示从其它分支升级过来的处理函数。

            每个分支的升级表中，第一行表示本分支配置数据的原始来源。
                    仅Master分支升级表中第一行，本分支升级函数不为空，且收编分支升级函数为空，表示来源于基础版本升级；
                    非Master分支升级表中第一行，本分支升级函数为空，且收编分支升级函数不为空，表示来源于其它分支收编。
            每个分支的升级表中，后续行本分支升级函数和收编其它分支升级函数至少有一个不为空。
            本表中升级目标版本必须严格从小到大排列，收编分支版本严格小于升级目标版本。

            修改本注册表有三种场景：
                （1）新建分支场景，修改步骤详见 g_astBranchUpgradeRegTab 的 【分支注册表新建说明】。
                （2）本分支内升级场景，修改步骤详见后续【配置升级注册说明】。
                （3）收编分支场景，修改步骤详见 g_astBranchUpgradeRegTab 的 【分支注册表收编说明】。
    */

    /*   升级目标版本                    本分支升级函数                     收编分支版本               收编分支升级函数          */
    {"V01.11.237.001",        nullptr,                          "V01.11.237.000",   V01_11_237_001_VS_V01_11_237_000::Upgrade},
    {"V01.11.237.002",        V01_11_237_002::Upgrade,          "",                 nullptr}

    /* 【配置升级注册说明】
            修改配置文件 checklist :
                (1) 将上一个配置注册版本的升级函数的域名 NEW_CFG_VER 修改为对应的版本号，比如 V01_01_001_000_001；
                (2) 将上一个配置注册版本的版本号 CFG_VERSION 修改为此时 CFG_VERSION 的具体的版本号字符串，比如 "V01.01.001.000.001"；
                (3) 新增加的新版本升级处理函数放到域名 NEW_CFG_VER 中；
                (4) 新增注册项的版本写为 CFG_VERSION ，即为自动识别最新配置版本；
                (5) 将 CFG_VERSION 的定义修改为 LOC_VERSION，即其定义变为
                    #define CFG_VERSION    LOC_VERSION
                (6) 将 \navZero\workspace\src\comm\comm\template\cfg_file\cfg_param.xml 修改为最新模板内容。

       注意：如果此时 CFG_VERSION 的定义本就是 LOC_VERSION，说明新版本已经有其他人增加注册了。
             此时不用新增注册项，直接在其注册函数中新增加变更内容即可。
             合并Merge时，应再次确认master最新代码中 CFG_VERSION 是否为 LOC_VERSION，如否则不上注册项。
    */
};

}

}


