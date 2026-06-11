
#ifndef __COMM_DBG_SW_CPP__
#define __COMM_DBG_SW_CPP__

#include "comm/comm_dbg_sw.h"
#include "comm/comm_ros_base.h"
#include "comm/comm_base.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 获取指定开关
修改记录:
**************************************************************************************/
bool DBG_SW_PROTECTED_C::Get(DBG_SW_TYPE_ENUM enSwType)
{
    DBG_SW_INFO_STRU *pstSwInfo = GetSwInfo(enSwType);
    R_ASSERT(pstSwInfo != NULL, false);

    if (pstSwInfo->enActMode == DBG_SW_ACT_MODE_ACT_ONCE) {
        /* 该类型开关，被置位后值返回一次ON */
        if (pstSwInfo->isOn) {
            pstSwInfo->isOn = false;
            return true;
        } else {
            return false;
        }
    } else {
        return pstSwInfo->isOn;
    }
}

/**************************************************************************************
功能描述: 注册命令信息
修改记录:
**************************************************************************************/
void DBG_SW_PROTECTED_C::RegCmdInfo(CAR_CTRL_CMD_ENUM enCtrlCmd, const char *pcCtrlCmd, DBG_SW_TYPE_ENUM enSwType, DBG_SW_CTRL_CMD_ACT_MODE_ENUM enCmdActMode)
{
    R_ASSERT(regCmdInfoNum_ < ARRAY_SIZE(astCmdInfo), NOTHING);

    DBG_SW_CTRL_CMD_INFO_STRU *pstNew = &astCmdInfo[regCmdInfoNum_];
    pstNew->enCtrlCmd   = enCtrlCmd;
    pstNew->pcCtrlCmd   = pcCtrlCmd + CAR_CTRL_CMD_ENUM_NAME_PREFIX_LEN;
    pstNew->enDbgSwType = enSwType;
    pstNew->enActMode   = enCmdActMode;

    ++regCmdInfoNum_;
}

/**************************************************************************************
功能描述: 注册命令信息1
修改记录:
**************************************************************************************/
void DBG_SW_PROTECTED_C::RegCmdInfo(CAR_CTRL_CMD_ENUM enBaseCmd,CAR_CTRL_CMD_ENUM enCtrlCmd, const char *pcCtrlCmd, DBG_SW_TYPE_ENUM enSwType, DBG_SW_CTRL_CMD_ACT_MODE_ENUM enCmdActMode)
{
    R_ASSERT(regCmdInfoNum1_ < ARRAY_SIZE(astCombCmdInfo), NOTHING);

    DBG_SW_CTRL_COMB_CMD_INFO_STRU *pstNew = &astCombCmdInfo[regCmdInfoNum1_];
    pstNew->enBaseCmd  = enBaseCmd;
    pstNew->enCtrlCmd  = enCtrlCmd;
    pstNew->pcCtrlCmd   = pcCtrlCmd + CAR_CTRL_CMD_ENUM_NAME_PREFIX_LEN;
    pstNew->enDbgSwType = enSwType;
    pstNew->enActMode   = enCmdActMode;

    ++regCmdInfoNum1_;
}

/**************************************************************************************
功能描述: 注册开关信息
修改记录:
**************************************************************************************/
void DBG_SW_PROTECTED_C::RegSwInfo(DBG_SW_TYPE_ENUM enSwType, const char *pcSwType, BOOL initVal, DBG_SW_ACT_MODE_ENUM enActMode)
{
    R_ASSERT(enSwType < ARRAY_SIZE(astSwInfo), NOTHING);

    DBG_SW_INFO_STRU *pstSw = &astSwInfo[enSwType];
    pstSw->pcSwType  = pcSwType + DBG_SW_TYPE_ENUM_NAME_PREFIX_LEN;
    pstSw->isOn      = initVal;
    pstSw->enActMode = enActMode;
}

/**************************************************************************************
功能描述: 获取开关信息
修改记录:
**************************************************************************************/
DBG_SW_INFO_STRU *DBG_SW_PROTECTED_C::GetSwInfo(DBG_SW_TYPE_ENUM enSwType)
{
    R_ASSERT(enSwType < ARRAY_SIZE(astSwInfo), NULL);

    return &astSwInfo[enSwType];
}

/**************************************************************************************
功能描述: 处理开关
修改记录:
**************************************************************************************/
void DBG_SW_PROTECTED_C::SetSw(DBG_SW_CTRL_CMD_ACT_MODE_ENUM enCmdActMode, DBG_SW_INFO_STRU &stSwInfo)
{
    bool oldVal = stSwInfo.isOn;

    switch (enCmdActMode) {
        case DBG_SW_CTRL_CMD_ACT_MODE_CHANGE:
            stSwInfo.isOn = !stSwInfo.isOn;
            break;

        case DBG_SW_CTRL_CMD_ACT_MODE_SET_ON:
            stSwInfo.isOn = true;
            break;

        case DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF:
            stSwInfo.isOn = false;
            break;

        default:
            break;
    }

    ST_LOG_INFO("Set Sw(%s) Old Val(%d) To New Val(%d).", stSwInfo.pcSwType, oldVal, stSwInfo.isOn);
}

void DBG_SW_PROTECTED_C::DspAllSwState(void)
{
    ST_LOG_INFO("");
    ST_LOG_INFO("***************** All Sw State ********************");

    for (UINT32 loop = 0; loop < ARRAY_SIZE(astSwInfo); ++loop) {
        ST_LOG_INFO("%30s : %d", ToLower(astSwInfo[loop].pcSwType).c_str(), astSwInfo[loop].isOn);
    }

    ST_LOG_INFO("***************************************************");
    ST_LOG_INFO("");
}

/**************************************************************************************
功能描述: 消息处理函数
修改记录:
**************************************************************************************/
void DBG_SW_PROTECTED_C::ReceiveZhiShanCmdCallBack(const comm_msg::zhishancmd::ConstPtr &pstMsg)
{
    if (pstMsg->zhishanCommand != ZHISHANCMD_TYPE_CAR_CTRL) {
        return;
    }

    if (pstMsg->zhishanVoice == CAR_CTRL_CMD_DSP_ALL_DBG_SW_STATE) {
        DspAllSwState();
        return;
    }

    if (pstMsg->zhishanVoice == CAR_CTRL_CMD_COMB_KEY_BASE) {
        cmdInfoCombinationKey_ = CAR_CTRL_CMD_COMB_KEY_BASE;
        updateTimer_.start();
        ST_LOG_INFO("Rece CAR_CTRL_CMD_COMB_KEY_BASE.");
        return ;
    }

    if(cmdInfoCombinationKey_ == CAR_CTRL_CMD_COMB_KEY_BASE)
     {
        for (UINT32 loop = 0; loop < regCmdInfoNum1_; ++loop) {
            DBG_SW_CTRL_COMB_CMD_INFO_STRU *pstCmdReg = &astCombCmdInfo[loop];
            if (pstCmdReg->enCtrlCmd != pstMsg->zhishanVoice) {
                continue;
            }
            ST_LOG_INFO("Recv Cmd(%s) And Set Sw(%d).", pstCmdReg->pcCtrlCmd, pstCmdReg->enDbgSwType);
            DBG_SW_INFO_STRU *pstSwInfo = GetSwInfo(pstCmdReg->enDbgSwType);
            if (pstSwInfo == NULL) {
                ST_LOG_ERR("Sw(%d) Can Not Found.", pstCmdReg->enDbgSwType);
                continue;
            }
            SetSw(pstCmdReg->enActMode, *pstSwInfo);
        }
    } else {
        for (UINT32 loop = 0; loop < regCmdInfoNum_; ++loop) {
            DBG_SW_CTRL_CMD_INFO_STRU *pstCmdReg = &astCmdInfo[loop];
            if (pstCmdReg->enCtrlCmd != pstMsg->zhishanVoice) {
                continue;
            }
            ST_LOG_INFO("Recv Cmd(%s) And Set Sw(%d).", pstCmdReg->pcCtrlCmd, pstCmdReg->enDbgSwType);
            DBG_SW_INFO_STRU *pstSwInfo = GetSwInfo(pstCmdReg->enDbgSwType);
            if (pstSwInfo == NULL) {
                ST_LOG_ERR("Sw(%d) Can Not Found.", pstCmdReg->enDbgSwType);
                continue;
            }
            SetSw(pstCmdReg->enActMode, *pstSwInfo);
            }
       }
}

/**************************************************************************************
功能描述: 定时器
修改记录:
**************************************************************************************/
void DBG_SW_PROTECTED_C::UpdateTimer(const ros::WallTimerEvent& timerEvent)
{
    if (cmdInfoCombinationKey_ == CAR_CTRL_CMD_COMB_KEY_BASE) {
        cmdInfoCombinationKey_ = CAR_CTRL_CMD_BUTT;
        ST_LOG_INFO("CAR_CTRL_CMD_COMB_KEY_BASE Overtime.");
    }

    updateTimer_.stop();
}

/**************************************************************************************
功能描述: 静态变量定义
修改记录:
**************************************************************************************/
atomic_int          DBG_SW_C::numOfInst_(0);
DBG_SW_PROTECTED_C *DBG_SW_C::pobjDbgSwProtected_(NULL);

/**************************************************************************************
功能描述: 获取开关值
修改记录:
**************************************************************************************/
bool DBG_SW_C::Get(DBG_SW_TYPE_ENUM enSwType)
{
    return (pobjDbgSwProtected_ != NULL) ? pobjDbgSwProtected_->Get(enSwType) : false;
}

/**************************************************************************************
功能描述: 消息发送器
修改记录:
**************************************************************************************/
void DBG_SW_CTR_C::SendCtrlCmd(CAR_CTRL_CMD_ENUM enCtrlCmd)
{
    comm_msg::zhishancmd stMsg;

    stMsg.zhishanCommand = ZHISHANCMD_TYPE_CAR_CTRL;
    stMsg.zhishanVoice   = (UINT08)enCtrlCmd;
    stMsg.zhishanPara    = 0;
    stMsg.autoCharge     = 0;
    stMsg.rodControl     = 0;

    publisher_.publish(stMsg);
    ST_LOG_INFO("Send Ctrl Cmd: %u.", enCtrlCmd);
}

#endif


