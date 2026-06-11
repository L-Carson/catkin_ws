

#ifndef __COMM_DBG_SW_H__
#define __COMM_DBG_SW_H__

#include "comm/comm_base.h"
#include "comm/comm_msg.h"
#include <comm_msg/zhishancmd.h>
#include <ros/ros.h>
#include "ros/wall_timer.h"
#include <iostream>
#include <list>
#include <vector>
#include <atomic>

using namespace std;

#define DBG_SW_TYPE_ENUM_NAME_PREFIX_LEN    12          /* 下面定义的消息名称的前公共部分(DBG_SW_TYPE_)长度，如果该部分修改，则本长度要同步修改 */
#define K_UPDATE_TIME_PERIOD_SEC            3           /* 更新消息定时器 */
#define K_KEY_COMBINATION_MULTIPE           30          /* 组合键倍数 */

typedef enum {
    DBG_SW_TYPE_TELE_CTRL_CLEAN,                /* 手动控制清扫开关 */
    DBG_SW_TYPE_TELE_CTRL_CLEAN_STOP_MODE,      /* 手动控制清扫停止模式，暂停还是结束 */
    DBG_SW_TYPE_TELE_CTRL_CLEAN_IS_ACT,         /* 手动控制清扫是否生效 */
    DBG_SW_TYPE_SAVE_ROUTE,                     /* 保存路径 */

    DBG_SW_TYPE_CLEAR_OBST,                     /* 清除障碍物 */
    DBG_SW_TYPE_CLEAR_LIMIT_LINE,               /* 清除限制线 */

    DBG_SW_TYPE_DSP_SNED_STATIS_SHOW,           /* 发送DSP控制命令统计信息输出控制开关 */
    DBG_SW_TYPE_DSP_NRT_RPT_SHOW,               /* DSP非实时上报状态信息打印开关 */
    DBG_SW_TYPE_OUTPUT_VEL_PLAN_INFO_ONCE,      /* 输出速度规划信息一次开关 */
    DBG_SW_TYPE_OUTPUT_ST_PLAN_ROUTE_DBG_ONCE,  /* 多路径切换算法调试一次 */

    DBG_SW_TYPE_IS_RECORD_TOPIC_BAG,            /* 是否录制话题数据包 */
    DBG_SW_TYPE_IS_RECORD_TOPIC_NAV_BAG,        /* 是否录制话题导航数据包 */
    DBG_SW_TYPE_IS_RECORD_TOPIC_LOC_BAG,        /* 是否录制话题定位数据包 */
    DBG_SW_TYPE_IS_RECORD_TOPIC_PCPT_BAG,       /* 是否录制话题感知数据包 */
    DBG_SW_TYPE_IS_RECORD_TOPIC_CLBT_BAG,       /* 是否录制标定数据包 */
    DBG_SW_TYPE_IS_SAVE_GARBAGE_IMAGE,          /* 是否保存垃圾图片 */


    DBG_SW_TYPE_PLAY_AUDIO_1,                   /* 播放语音1 */
    DBG_SW_TYPE_PLAY_AUDIO_2,                   /* 播放语音2 */

    DBG_SW_TYPE_ALONG_CTRL_NEW,                 /* 使用新的跟踪控制 */
    DBG_SW_TYPE_TRC_REF_ROUTE,                  /* 跟踪原始参考路径 */
    DBG_SW_TYPE_TRC_BY_P_P,                     /* 使用pp算法算法 */
    DBG_SW_TYPE_SAVE_VEL_PLAN_INFO,             /* 保存速度规划信息 */
    DBG_SW_TYPE_SAVE_LON_CTRL_INFO,             /* 保存纵向控制信息 */
    DBG_SW_TYPE_VEL_PLAN_ACC_SMOOTH,            /* 速度规划加速度平滑 */
    DBG_SW_TYPE_SAVE_ALONG_TRC_INFO_SAVE,       /* 保存沿线跟踪信息 */

    DBG_SW_TYPE_DCU_OUT_TIME,                   /* 模拟dcu超时 */
    DBG_SW_TYPE_SET_VEL_INFO,                   /* 手动设置DCU反馈轮速 */

    DBG_SW_TYPE_BUTT,                           /* 不能在此之后加 */
} DBG_SW_TYPE_ENUM;

typedef enum {
    DBG_SW_CTRL_CMD_ACT_MODE_CHANGE,            /* 每次收到该开关的消息时，将开关装填翻转 */
    DBG_SW_CTRL_CMD_ACT_MODE_SET_ON,            /* 每次收到消息时将开关设置为ON */
    DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF,           /* 每次收到消息时将开关设置为OFF */
} DBG_SW_CTRL_CMD_ACT_MODE_ENUM;

typedef enum {
    DBG_SW_ACT_MODE_ACT_CONTINUE,               /* 状态持续保存 */
    DBG_SW_ACT_MODE_ACT_ONCE,                   /* 被设置为ON后，第一获取为ON，然后立马变回OFF */
} DBG_SW_ACT_MODE_ENUM;

struct DBG_SW_CTRL_CMD_INFO_STRU {
    CAR_CTRL_CMD_ENUM             enCtrlCmd;
    const char                   *pcCtrlCmd;
    DBG_SW_TYPE_ENUM              enDbgSwType;
    DBG_SW_CTRL_CMD_ACT_MODE_ENUM enActMode;
};

struct DBG_SW_CTRL_COMB_CMD_INFO_STRU {
    CAR_CTRL_CMD_ENUM             enBaseCmd;
    CAR_CTRL_CMD_ENUM             enCtrlCmd;
    const char                   *pcCtrlCmd;
    DBG_SW_TYPE_ENUM              enDbgSwType;
    DBG_SW_CTRL_CMD_ACT_MODE_ENUM enActMode;
};

struct DBG_SW_INFO_STRU {
    const char          *pcSwType;
    bool                 isOn;
    DBG_SW_ACT_MODE_ENUM enActMode;
};

class DBG_SW_PROTECTED_C
{
public:
    DBG_SW_PROTECTED_C(ros::NodeHandle &nh)
    {
        regCmdInfoNum_              = 0;
        regCmdInfoNum1_             = 0;
        cmdInfoCombinationKey_      = CAR_CTRL_CMD_BUTT;
        subZhiShanCmd_              = nh.subscribe("/zhishanAndroid/zhishancmd", 10, &DBG_SW_PROTECTED_C::ReceiveZhiShanCmdCallBack, this);
        updateTimer_                = nh.createWallTimer(ros::WallDuration(K_UPDATE_TIME_PERIOD_SEC), &DBG_SW_PROTECTED_C::UpdateTimer, this, false, false);
        std::for_each(std::begin(astSwInfo),
                      std::end(astSwInfo),
                      [](DBG_SW_INFO_STRU &stSwInfo)
                      {
                          stSwInfo.pcSwType = "unknown";
                          stSwInfo.isOn = false;
                          stSwInfo.enActMode = DBG_SW_ACT_MODE_ACT_CONTINUE;
                      });

        /* 注册命令和开关的对应开关，支持多对多 */
#define REG_CMD_INFO(CtrlCmd_, SwType_, CmdActMode_)                        RegCmdInfo(CtrlCmd_, #CtrlCmd_, SwType_, CmdActMode_)
#define REG_COMB_CMD_INFO(BaseCmd_,CtrlCmd_, SwType_, CmdActMode_)          RegCmdInfo(BaseCmd_,CtrlCmd_, #CtrlCmd_, SwType_, CmdActMode_)

        REG_CMD_INFO(CAR_CTRL_CMD_TELE_CTRL_CLEAN_CHANGE,                   DBG_SW_TYPE_TELE_CTRL_CLEAN,             DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_TELE_CTRL_CLEAN_CHANGE,                   DBG_SW_TYPE_TELE_CTRL_CLEAN_IS_ACT,      DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_TELE_CTRL_CLEAN_PAUSE_STOP,               DBG_SW_TYPE_TELE_CTRL_CLEAN_STOP_MODE,   DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_TELE_CTRL_CLEAN_DEACT,                    DBG_SW_TYPE_TELE_CTRL_CLEAN_IS_ACT,      DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);
        REG_CMD_INFO(CAR_CTRL_CMD_TELE_CTRL_CLEAN_DEACT,                    DBG_SW_TYPE_TELE_CTRL_CLEAN,             DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);
        REG_CMD_INFO(CAR_CTRL_CMD_TELE_CTRL_CLEAN_DEACT,                    DBG_SW_TYPE_TELE_CTRL_CLEAN_STOP_MODE,   DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);

        REG_CMD_INFO(CAR_CTRL_CMD_START_SAVE_ROUTE,                         DBG_SW_TYPE_SAVE_ROUTE,                  DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_STOP_SAVE_ROUTE,                          DBG_SW_TYPE_SAVE_ROUTE,                  DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);

        REG_CMD_INFO(CAR_CTRL_CMD_CLEAR_OBST,                               DBG_SW_TYPE_CLEAR_OBST,                  DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_CLEAR_LIMIT_LINE,                         DBG_SW_TYPE_CLEAR_LIMIT_LINE,            DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_CLEAR_OBST_AND_LIMIT_LINE,                DBG_SW_TYPE_CLEAR_OBST,                  DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_CLEAR_OBST_AND_LIMIT_LINE,                DBG_SW_TYPE_CLEAR_LIMIT_LINE,            DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );

        REG_CMD_INFO(CAR_CTRL_CMD_START_RECORD_TOPIC_BAG,                   DBG_SW_TYPE_IS_RECORD_TOPIC_BAG,         DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_STOP_RECORD_TOPIC_BAG,                    DBG_SW_TYPE_IS_RECORD_TOPIC_BAG,         DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);

        REG_CMD_INFO(CAR_CTRL_CMD_PLAY_AUDIO_1,                             DBG_SW_TYPE_PLAY_AUDIO_1,                DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_PLAY_AUDIO_2,                             DBG_SW_TYPE_PLAY_AUDIO_2,                DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );

        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_ALONG_CTRL_NEW,                    DBG_SW_TYPE_ALONG_CTRL_NEW,              DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_TRC_REF_ROUTE,                     DBG_SW_TYPE_TRC_REF_ROUTE,               DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_TRC_BY_P_P_SW,                     DBG_SW_TYPE_TRC_BY_P_P,                  DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_SAVE_VEL_PLAN_INFO,                DBG_SW_TYPE_SAVE_VEL_PLAN_INFO,          DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_SAVE_LON_CTRL_INFO,                DBG_SW_TYPE_SAVE_LON_CTRL_INFO,          DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_ACC_SMOOTH_SW,                     DBG_SW_TYPE_VEL_PLAN_ACC_SMOOTH,         DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_ALONG_TRC_INFO_SAVE_SW,            DBG_SW_TYPE_SAVE_ALONG_TRC_INFO_SAVE,    DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );

        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_LOC_BAG_START,       DBG_SW_TYPE_IS_RECORD_TOPIC_LOC_BAG,   DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_LOC_BAG_STOP,        DBG_SW_TYPE_IS_RECORD_TOPIC_LOC_BAG,   DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_PCPT_BAG_START,      DBG_SW_TYPE_IS_RECORD_TOPIC_PCPT_BAG,  DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_PCPT_BAG_STOP,       DBG_SW_TYPE_IS_RECORD_TOPIC_PCPT_BAG,  DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_CLBT_BAG_START,      DBG_SW_TYPE_IS_RECORD_TOPIC_CLBT_BAG,  DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_CLBT_BAG_STOP,       DBG_SW_TYPE_IS_RECORD_TOPIC_CLBT_BAG,  DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_NAV_BAG_START,       DBG_SW_TYPE_IS_RECORD_TOPIC_NAV_BAG,   DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_RECORD_NAV_BAG_STOP,        DBG_SW_TYPE_IS_RECORD_TOPIC_NAV_BAG,   DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_START_SAVE_GARBAGE_IMAGE,   DBG_SW_TYPE_IS_SAVE_GARBAGE_IMAGE,     DBG_SW_CTRL_CMD_ACT_MODE_SET_ON);
        REG_COMB_CMD_INFO(CAR_CTRL_CMD_COMB_KEY_BASE, CAR_CTRL_CMD_COMB_KEY_STOP_SAVE_GARBAGE_IMAGE,    DBG_SW_TYPE_IS_SAVE_GARBAGE_IMAGE,     DBG_SW_CTRL_CMD_ACT_MODE_SET_OFF);

        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_DSP_STATIS_SHOW_SW,                DBG_SW_TYPE_DSP_SNED_STATIS_SHOW,           DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_DSP_NRT_RPT_SHOW_SW,               DBG_SW_TYPE_DSP_NRT_RPT_SHOW,               DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_OUTPUT_VEL_PLAN_INFO_ONCE,                DBG_SW_TYPE_OUTPUT_VEL_PLAN_INFO_ONCE,      DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_OUTPUT_ST_PLAN_ROUTE_DBG_ONCE,            DBG_SW_TYPE_OUTPUT_ST_PLAN_ROUTE_DBG_ONCE,  DBG_SW_CTRL_CMD_ACT_MODE_SET_ON );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_DCU_OUT_TIME,                      DBG_SW_TYPE_DCU_OUT_TIME,                   DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );
        REG_CMD_INFO(CAR_CTRL_CMD_CHANGE_SET_VEL_INFO,                      DBG_SW_TYPE_SET_VEL_INFO,                   DBG_SW_CTRL_CMD_ACT_MODE_CHANGE );


#define REG_SW_INFO(SwType_, initVal_, SwActMode_)            RegSwInfo(SwType_, #SwType_, initVal_, SwActMode_)

        REG_SW_INFO(DBG_SW_TYPE_TELE_CTRL_CLEAN,                false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_TELE_CTRL_CLEAN_STOP_MODE,      false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_TELE_CTRL_CLEAN_IS_ACT,         false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_SAVE_ROUTE,                     false, DBG_SW_ACT_MODE_ACT_CONTINUE);

        REG_SW_INFO(DBG_SW_TYPE_CLEAR_OBST,                     false, DBG_SW_ACT_MODE_ACT_ONCE    );
        REG_SW_INFO(DBG_SW_TYPE_CLEAR_LIMIT_LINE,               false, DBG_SW_ACT_MODE_ACT_ONCE    );

        REG_SW_INFO(DBG_SW_TYPE_PLAY_AUDIO_1,                   false, DBG_SW_ACT_MODE_ACT_ONCE    );
        REG_SW_INFO(DBG_SW_TYPE_PLAY_AUDIO_2,                   false, DBG_SW_ACT_MODE_ACT_ONCE    );

        REG_SW_INFO(DBG_SW_TYPE_DSP_SNED_STATIS_SHOW,           false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_DSP_NRT_RPT_SHOW,               false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_OUTPUT_VEL_PLAN_INFO_ONCE,      false, DBG_SW_ACT_MODE_ACT_ONCE    );
        REG_SW_INFO(DBG_SW_TYPE_OUTPUT_ST_PLAN_ROUTE_DBG_ONCE,  false, DBG_SW_ACT_MODE_ACT_ONCE    );

        REG_SW_INFO(DBG_SW_TYPE_ALONG_CTRL_NEW,                 false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_TRC_REF_ROUTE,                  false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_TRC_BY_P_P,                     true,  DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_SAVE_VEL_PLAN_INFO,             false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_SAVE_LON_CTRL_INFO,             false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_VEL_PLAN_ACC_SMOOTH,            true,  DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_SAVE_ALONG_TRC_INFO_SAVE,       false, DBG_SW_ACT_MODE_ACT_CONTINUE);

        REG_SW_INFO(DBG_SW_TYPE_IS_RECORD_TOPIC_BAG,            false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_IS_RECORD_TOPIC_NAV_BAG,        false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_IS_RECORD_TOPIC_LOC_BAG,        false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_IS_RECORD_TOPIC_PCPT_BAG,       false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_IS_RECORD_TOPIC_CLBT_BAG,       false, DBG_SW_ACT_MODE_ACT_CONTINUE);
        REG_SW_INFO(DBG_SW_TYPE_IS_SAVE_GARBAGE_IMAGE,          false, DBG_SW_ACT_MODE_ACT_CONTINUE);
    }

protected:
    bool Get(DBG_SW_TYPE_ENUM enSwType);

private:
    UINT32                           regCmdInfoNum_;
    UINT32                           regCmdInfoNum1_;
    CAR_CTRL_CMD_ENUM                cmdInfoCombinationKey_;
    DBG_SW_CTRL_CMD_INFO_STRU        astCmdInfo[50];                        /* 该数字大于上面的消息注册个数就行 */
    DBG_SW_CTRL_COMB_CMD_INFO_STRU   astCombCmdInfo[500];                   /* 该数字大于上面的消息注册个数就行 */
    DBG_SW_INFO_STRU                 astSwInfo[DBG_SW_TYPE_BUTT];
    ros::Subscriber                  subZhiShanCmd_;
    ros::WallTimer                   updateTimer_;                          //定时器

    void RegCmdInfo(CAR_CTRL_CMD_ENUM enCtrlCmd, const char *pcCtrlCmd, DBG_SW_TYPE_ENUM enSwType, DBG_SW_CTRL_CMD_ACT_MODE_ENUM enCmdActMode);
    void RegCmdInfo(CAR_CTRL_CMD_ENUM enBaseCmd,CAR_CTRL_CMD_ENUM enCtrlCmd, const char *pcCtrlCmd, DBG_SW_TYPE_ENUM enSwType, DBG_SW_CTRL_CMD_ACT_MODE_ENUM enCmdActMode);
    void RegSwInfo(DBG_SW_TYPE_ENUM enSwType, const char *pcSwType, BOOL initVal, DBG_SW_ACT_MODE_ENUM enActMode);

    DBG_SW_INFO_STRU *GetSwInfo(DBG_SW_TYPE_ENUM enSwType);
    void SetSw(DBG_SW_CTRL_CMD_ACT_MODE_ENUM enCmdActMode, DBG_SW_INFO_STRU &stSwInfo);

    void DspAllSwState(void);
    void ReceiveZhiShanCmdCallBack(const comm_msg::zhishancmd::ConstPtr &pstMsg);
    void UpdateTimer(const ros::WallTimerEvent& timerEvent);

    friend class DBG_SW_C;
};

class DBG_SW_C
{
public:
    DBG_SW_C(ros::NodeHandle &nh) : nh_(nh)
    {
        /* 实现单实例模式 */
        /* 该构造和析构为非高频操作，暂不考虑多线程互斥问题 */
        ++numOfInst_;
        if (1 == numOfInst_) {
            pobjDbgSwProtected_ = new DBG_SW_PROTECTED_C(nh_);
            if (NULL == pobjDbgSwProtected_) {
                ST_LOG_ERR("New DBG_SW_PROTECTED_C Fail.");
            } else {
                ST_LOG_INFO("New DBG_SW_PROTECTED_C Succ.");
            }
        }
    }

    ~DBG_SW_C(void)
    {
        --numOfInst_;
        if ((0 == numOfInst_) && (pobjDbgSwProtected_ != NULL)) {
            delete pobjDbgSwProtected_;
            pobjDbgSwProtected_ = NULL;
            ST_LOG_INFO("Delete DBG_SW_PROTECTED_C Succ.");
        }
    }

    bool Get(DBG_SW_TYPE_ENUM enSwType);

private:
    ros::NodeHandle           &nh_;

    static atomic_int          numOfInst_;
    static DBG_SW_PROTECTED_C *pobjDbgSwProtected_;
};

class DBG_SW_CTR_C
{
public:
    DBG_SW_CTR_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::zhishancmd>("/zhishanAndroid/zhishancmd", 10);
    }

    void SendCtrlCmd(CAR_CTRL_CMD_ENUM enCtrlCmd);

private:
    ros::Publisher publisher_;
};

#endif

