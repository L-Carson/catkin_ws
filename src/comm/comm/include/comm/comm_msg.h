#ifndef __COMM_MSG_H__
#define __COMM_MSG_H__

enum ZHISHANCMD_TYPE_ENUM {
    ZHISHANCMD_TYPE_CAR_CTRL            = 8,
    ZHISHANCMD_TYPE_USED9               = 9,        /* 已经不再使用 */
    ZHISHANCMD_TYPE_USED10              = 10,       /* 已经不再使用 */
    ZHISHANCMD_TYPE_USED15              = 15,       /* 已经不再使用 */
    ZHISHANCMD_TYPE_USED21              = 21,       /* 已经不再使用 */
    ZHISHANCMD_TYPE_USED88              = 88,       /* 已经不再使用 */
};

#define CAR_CTRL_CMD_ENUM_NAME_PREFIX_LEN   13          /* 下面定义的消息名称的前公共部分(CAR_CTRL_CMD_)长度，如果该部分修改，则本长度要同步修改 */

enum CAR_CTRL_CMD_ENUM {
    CAR_CTRL_CMD_TELE_CTRL_CLEAN_CHANGE                 = 1,        /* 3号车，清扫/不清扫，按动后强制手动控制 */
    CAR_CTRL_CMD_TELE_CTRL_CLEAN_PAUSE_STOP             = 2,        /* 3号车，暂停/停止 */
    CAR_CTRL_CMD_TELE_CTRL_CLEAN_DEACT                  = 3,        /* 3号车，清除手动操作，恢复自动控制 */

    CAR_CTRL_CMD_COMB_KEY_BASE                          = 4,        /* 组合键 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_LOC_BAG_START          = 5,        /* 结合组合键 录制定位数据集 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_LOC_BAG_STOP           = 6,        /* 结合组合键 停止录制定位数据集 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_PCPT_BAG_START         = 7,        /* 结合组合键 录制感知数据集 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_PCPT_BAG_STOP          = 8,        /* 结合组合键 停止录制感知数据集 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_CLBT_BAG_START         = 9,        /* 结合组合键 录制雷达标定数据集 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_CLBT_BAG_STOP          = 10,       /* 结合组合键 停止录制雷达标定数据集 */

    CAR_CTRL_CMD_START_SAVE_ROUTE                       = 11,       /* 开始保存路径 */
    CAR_CTRL_CMD_STOP_SAVE_ROUTE                        = 12,       /* 停止保存路径 */

    CAR_CTRL_CMD_COMB_KEY_RECORD_NAV_BAG_START          = 13,       /* 结合组合键 开始录制导航数据集 */
    CAR_CTRL_CMD_COMB_KEY_RECORD_NAV_BAG_STOP           = 14,       /* 结合组合键 停止录制导航数据集 */
    CAR_CTRL_CMD_COMB_KEY_START_SAVE_GARBAGE_IMAGE      = 15,       /* 结合组合键 开始保存垃圾图片              */
    CAR_CTRL_CMD_COMB_KEY_STOP_SAVE_GARBAGE_IMAGE       = 16,       /* 结合组合键 停止保存垃圾图片              */

    CAR_CTRL_CMD_CHANGE_SAVE_LON_CTRL_INFO              = 15,       /* 保存纵向控制信息 */
    CAR_CTRL_CMD_CHANGE_TRC_REF_ROUTE                   = 16,       /* 跟踪原始参考路径 */

    CAR_CTRL_CMD_CLEAR_OBST                             = 17,       /* 清除障碍物一秒钟 */
    CAR_CTRL_CMD_CLEAR_LIMIT_LINE                       = 18,       /* 清除限制线一秒钟 */
    CAR_CTRL_CMD_CLEAR_OBST_AND_LIMIT_LINE              = 19,       /* 清除障碍物和限制线一秒钟 */

    CAR_CTRL_CMD_STOP_MOVE                              = 20,       /* 强制停止 */
    CAR_CTRL_CMD_CANCEL_STOP_MOVE                       = 21,       /* 取消强制停止 */

    CAR_CTRL_CMD_CHANGE_REMOTE_CTRL                     = 22,       /* 切换为远程遥控 */
    CAT_CTRL_CMD_CHANGE_AUTO_CTRL                       = 23,       /* 切换为自动控制 */

    CAR_CTRL_CMD_PLAY_AUDIO_1                           = 24,       /* 播放语音1 */
    CAR_CTRL_CMD_PLAY_AUDIO_2                           = 25,       /* 播放语音2 */

    CAR_CTRL_CMD_CHANGE_ALONG_CTRL_NEW                  = 26,       /* 固定路线任务避障切换新的跟踪控制 */

    CAR_CTRL_CMD_START_RECORD_TOPIC_BAG                 = 27,       /* 开始录制话题数据包 */
    CAR_CTRL_CMD_STOP_RECORD_TOPIC_BAG                  = 28,       /* 停止录制话题数据包 */

    /************************ 以上为手机工具显示的按钮 ************************/
    /************************    以上为内部控制命令    ************************/

    CAR_CTRL_CMD_CHANGE_DSP_STATIS_SHOW_SW              = 51,       /* 改变DSP统计功能开关 */
    CAR_CTRL_CMD_CHANGE_DSP_NRT_RPT_SHOW_SW             = 52,       /* 改变DSP非实时状态上报信息打印开关 */
    CAR_CTRL_CMD_CHANGE_SAVE_VEL_PLAN_INFO              = 54,       /* 改变打印上报人机消息消息 */
    CAR_CTRL_CMD_OUTPUT_VEL_PLAN_INFO_ONCE              = 56,       /* 输出一次速度规划的相关信息 */
    CAR_CTRL_CMD_OUTPUT_ST_PLAN_ROUTE_DBG_ONCE          = 57,       /* 多路径切换算法调试一次 */
    CAR_CTRL_CMD_CHANGE_DCU_OUT_TIME                    = 58,       /* 改变dcu通信超时状态 */
    CAR_CTRL_CMD_CHANGE_SET_VEL_INFO                    = 59,       /* 改变是否设置速度反馈状态 */
    CAR_CTRL_CMD_CHANGE_TRC_BY_P_P_SW                   = 60,       /* 改变pp跟踪算法开关 */
    CAR_CTRL_CMD_CHANGE_ACC_SMOOTH_SW                   = 61,       /* 改变速度规划加速度平滑开关 */
    CAR_CTRL_CMD_CHANGE_ALONG_TRC_INFO_SAVE_SW          = 62,       /* 改变沿线跟踪信息保存开关 */

    CAR_CTRL_CMD_DSP_ALL_DBG_SW_STATE                   = 100,      /* 打印所有调试开关状态 */

    CAR_CTRL_CMD_BUTT                                               /* 放到最后面 */
};

typedef enum {
    MSG_DOMAIN_DEV_MNG          = 1,   /* 设备管理 */
    MSG_DOMAIN_TASK_MNG         = 2,   /* 任务管理 */
    MSG_DOMAIN_ALARM_MNG        = 3,   /* 告警管理 */
    MSG_DOMAIN_SOFTWARE_MNG     = 4,   /* 软件管理 */
    MSG_DOMAIN_MAP_MNG          = 5,   /* 地图管理 */
    MSG_DOMAIN_COMM_FUNC_MNG    = 6,   /* 通用功能管理 */
} MSG_DOMAIN_ENUM;

typedef enum {
    MOTOR_CMD_MSG_CHARGE_MOTOR_OUT      = 1,   /* 充电电机伸出 */
    MOTOR_CMD_MSG_CHARGE_MOTOR_IN       = 2,   /* 充电电机缩回 */
    MOTOR_CMD_MSG_CHARGE_MOTOR_STOP     = 3,   /* 充电电机停止 */
    MOTOR_CMD_MSG_CHARGE_MOTOR_RESET    = 4,   /* 充电电机复位 */
    MOTOR_CMD_MSG_CHARGE_MOTOR_CLEAR    = 5,   /* 充电电机清除异常信息 */
    MOTOR_CMD_MSG_LIFT_MOTOR_OUT        = 6,   /* 升降电机伸出 */
    MOTOR_CMD_MSG_LIFT_MOTOR_IN         = 7,   /* 升降电机缩回 */
    MOTOR_CMD_MSG_LIFT_MOTOR_STOP       = 8,   /* 升降电机停止 */
    MOTOR_CMD_MSG_LIFT_MOTOR_RESET      = 9,   /* 升降电机复位 */
    MOTOR_CMD_MSG_LIFT_MOTOR_CLEAR      = 10,  /* 升降电机清除异常信息 */
    MOTOR_CMD_MSG_ROLL_MOTOR_OUT        = 11,  /* 翻转电机伸出 */
    MOTOR_CMD_MSG_ROLL_MOTOR_IN         = 12,  /* 翻转电机缩回 */
    MOTOR_CMD_MSG_ROLL_MOTOR_STOP       = 13,  /* 翻转电机停止 */
    MOTOR_CMD_MSG_ROLL_MOTOR_RESET      = 14,  /* 翻转电机复位 */
    MOTOR_CMD_MSG_ROLL_MOTOR_CLEAR      = 15,  /* 翻转电机清除异常信息 */

    MOTOR_CMD_MSG_TRASH_BIN_COVER_OPEN          = 16, /* 垃圾打包 *//* 垃圾斗后盖打开 */
    MOTOR_CMD_MSG_TRASH_BIN_COVER_CLOSE         = 17, /* 垃圾打包 *//* 垃圾斗后盖关闭 */
    MOTOR_CMD_MSG_TRASH_BIN_COVER_STOP          = 18, /* 垃圾打包 *//* 垃圾斗后盖停止不动 */
    MOTOR_CMD_MSG_TRASH_BAG_CLAMP_CLAMP         = 19, /* 垃圾打包 *//* 垃圾袋夹夹紧 */
    MOTOR_CMD_MSG_TRASH_BAG_CLAMP_RELEASE       = 20, /* 垃圾打包 *//* 垃圾袋夹松开 */
    MOTOR_CMD_MSG_TRASH_BAG_CLAMP_STOP          = 21, /* 垃圾打包 *//* 垃圾袋夹停止不动 */
    MOTOR_CMD_MSG_TRASH_BAG_SUBTERRENE_START    = 22, /* 垃圾打包 *//* 垃圾袋热熔开始 */
    MOTOR_CMD_MSG_TRASH_BAG_SUBTERRENE_STOP     = 23, /* 垃圾打包 *//* 垃圾袋热熔停止 */
    MOTOR_CMD_MSG_TRASH_BAG_DRAW_START          = 24, /* 垃圾打包 *//* 垃圾袋吸取开始 */
    MOTOR_CMD_MSG_TRASH_BAG_DRAW_STOP           = 25, /* 垃圾打包 *//* 垃圾袋吸取停止 */
    MOTOR_CMD_MSG_CHANGING_DOOR_OPEN            = 26, /* 充电舱盖板打开 */
    MOTOR_CMD_MSG_CHANGING_DOOR_CLOSE           = 27, /* 充电舱盖板关闭 */
    MOTOR_CMD_MSG_CHANGING_DOOR_STOP            = 28, /* 充电舱盖板停止 */
} MOTOR_CMD_MSG_ENUM;


/**************************************************************************************
功能描述: 和DSP接口消息类型
修改记录:
**************************************************************************************/
enum {
    CHARGE_CMD_MOTOR_OUT     = 0x10,     /* 伸出充电杆 */
    CHARGE_CMD_MOTOR_IN      = 0x11,     /* 缩回充电杆 */
    CHARGE_CMD_MOTOR_STOP    = 0x12,     /* 停止充电杆电机 */
    CHARGE_CMD_MOTOR_RESET   = 0x13,     /* 充电电机复位 */
    CHARGE_CMD_MOTOR_CLEAR   = 0x14,     /* 充电电机清除异常信息 */
};

/**************************************************************************************
功能描述: 和DSP接口消息类型
修改记录:
**************************************************************************************/
enum {
    TRASH_CMD_LIFT_MOTOR_OUT     = 0x01,     /* 升降电机伸出 */
    TRASH_CMD_LIFT_MOTOR_IN      = 0x02,     /* 升降电机缩回 */
    TRASH_CMD_LIFT_MOTOR_STOP    = 0x03,     /* 升降电机停止 */
    TRASH_CMD_ROLL_MOTOR_OUT     = 0x04,     /* 翻转电机伸出 */
    TRASH_CMD_ROLL_MOTOR_IN      = 0x05,     /* 翻转电机缩回 */
    TRASH_CMD_ROLL_MOTOR_STOP    = 0x06,     /* 翻转电机停止 */
    TRASH_CMD_LIFT_MOTOR_RESET   = 0x10,     /* 升降电机复位 */
    TRASH_CMD_ROLL_MOTOR_RESET   = 0x11,     /* 翻转电机复位 */
    TRASH_CMD_LIFT_MOTOR_CLEAR   = 0x12,     /* 升降电机清除异常信息 */
    TRASH_CMD_ROLL_MOTOR_CLEAR   = 0x13,     /* 翻转电机清除异常信息 */
};

/**************************************************************************************
功能描述: 地图节点状态
修改记录:
**************************************************************************************/
typedef enum {
    /* 异常定位状态值小于0 */
    MAP_NODE_STATE_ABNORMAL                = -2,       /* 异常 */
    MAP_NODE_STATE_IDLE                    = -1,       /* 空闲 */
    /* 正常定位状态值大于0 */
    MAP_NODE_STATE_NAVIGATE_NORMAL         = 0,        /* 正常导航状态 */
    MAP_NODE_STATE_NAVIGATE_CHANGING_MAP   = 1,        /* 正在切换导航地图 */
    MAP_NODE_STATE_GMAPPING_NORMAL         = 2,        /* 正常建图状态 */
    MAP_NODE_STATE_GMAPPING_CREATING_MAP   = 3,        /* 正在建立新地图 */
} MAP_NODE_STATE_ENUM;

/**
 * @brief 电机执行状态类型
 *
 */
typedef enum {
    MOTOR_STATE_NORMAL            = 0x00,   // 状态正常
    MOTOR_STATE_STICK_COMPLETE    = 0x01,   // 伸出完成
    MOTOR_STATE_WITHDRAW_COMPLETE = 0x02,   // 收回完成
    MOTOR_STATE_STICKING          = 0x03,   // 伸出中...
    MOTOR_STATE_WITHDRAWING       = 0x04,   // 收回中...
    MOTOR_STATE_NO_EXISIT         = 0xEE,   // 状态不存在
    MOTOR_STATE_ABNORMAL          = 0xFF,   // 状态异常
} MOTOR_STATE_ENUM;

/**************************************************************************************
功能描述: 垃圾打包结构中，垃圾斗盖状态
          注意：需要与DCU通信协议中定义完全一致。
修改记录:
**************************************************************************************/
enum DCU_TRASH_BIN_COVER_STATE_ENUM
{
    DCU_TRASH_BIN_COVER_STATE_STOPPING      = 0,    /* 停止 */
    DCU_TRASH_BIN_COVER_STATE_PUSH_COMPLETE = 1,    /* 推出到位 */
    DCU_TRASH_BIN_COVER_STATE_PULL_COMPLETE = 2,    /* 收回到位 */
    DCU_TRASH_BIN_COVER_STATE_PUSHING       = 3,    /* 推出中 */
    DCU_TRASH_BIN_COVER_STATE_PULLING       = 4,    /* 收回中 */
    DCU_TRASH_BIN_COVER_STATE_NO_EXIST      = 0xEE, /* 自定义，不存在 */
    DCU_TRASH_BIN_COVER_STATE_ABNORMAL      = 0xFF, /* 状态异常 */
};

/**************************************************************************************
功能描述: 垃圾打包结构中，垃圾袋夹状态
          注意：需要与DCU通信协议中定义完全一致。
修改记录:
**************************************************************************************/
enum DCU_TRASH_BAG_CLAMP_STATE_ENUM
{
    DCU_TRASH_BAG_CLAMP_STATE_STOPPING      = 0,    /* 停止 */
    DCU_TRASH_BAG_CLAMP_STATE_PUSH_COMPLETE = 1,    /* 推出到位 */
    DCU_TRASH_BAG_CLAMP_STATE_PULL_COMPLETE = 2,    /* 收回到位 */
    DCU_TRASH_BAG_CLAMP_STATE_PUSHING       = 3,    /* 推出中 */
    DCU_TRASH_BAG_CLAMP_STATE_PULLING       = 4,    /* 收回中 */
    DCU_TRASH_BAG_CLAMP_STATE_NO_EXIST      = 0xEE, /* 自定义，不存在 */
    DCU_TRASH_BAG_CLAMP_STATE_ABNORMAL      = 0xFF, /* 状态异常 */
};

/**************************************************************************************
功能描述: 垃圾打包结构中，垃圾袋热熔状态
          注意：需要与DCU通信协议中定义完全一致。
修改记录:
**************************************************************************************/
enum DCU_TRASH_BAG_SUBTERRENE_STATE_ENUM
{
    DCU_TRASH_BAG_SUBTERRENE_STATE_STOPPING = 0,    /* 停止 */
    DCU_TRASH_BAG_SUBTERRENE_STATE_COMPLETE = 1,    /* 热熔完成 */
    DCU_TRASH_BAG_SUBTERRENE_STATE_WORKING  = 3,    /* 热熔功能运行中 */
    DCU_TRASH_BAG_SUBTERRENE_STATE_NO_EXIST = 0xEE, /* 自定义，不存在 */
    DCU_TRASH_BAG_SUBTERRENE_STATE_ABNORMAL = 0xFF, /* 状态异常 */
};

/**************************************************************************************
功能描述: 垃圾打包结构中，垃圾袋吸取状态
          注意：需要与DCU通信协议中定义完全一致。
修改记录:
**************************************************************************************/
enum DCU_TRASH_BAG_DRAW_STATE_ENUM
{
    DCU_TRASH_BAG_DRAW_STATE_STOPPING       = 0,    /* 停止 */
    DCU_TRASH_BAG_DRAW_STATE_COMPLETE       = 1,    /* 吸取完成 */
    DCU_TRASH_BAG_DRAW_STATE_WORKING        = 3,    /* 吸取垃圾袋功能运行中 */
    DCU_TRASH_BAG_DRAW_STATE_NO_EXIST       = 0xEE, /* 自定义，不存在 */
    DCU_tRASH_BAG_DRAW_STATE_ABNORMAL       = 0XFF, /* 状态异常 */
};

/**************************************************************************************
功能描述: 充电舱们状态位
          注意：需要与DCU通信协议中定义完全一致。
修改记录:
**************************************************************************************/
enum DCU_CHARGE_DOOR_STATE_ENUM
{
    DCU_CHARGING_DOOR_STATE_UNKNOW            = 0,    /* 未知位置 */
    DCU_CHARGING_DOOR_STATE_PUSH_COMPLETE     = 1,    /* 推出到位 */
    DCU_CHARGING_DOOR_STATE_PULL_COMPLETE     = 2,    /* 收回到位 */
    DCU_CHARGING_DOOR_STATE_PUSHING           = 3,    /* 推出中 */
    DCU_CHARGING_DOOR_STATE_PULLING           = 4,    /* 收回中 */
    DCU_CHARGING_DOOR_STATE_NO_EXIST          = 0xEE, /* 自定义，不存在 */
    DCU_CHARGING_DOOR_STATE_ABNORMAL          = 0XFF, /* 状态异常 */
};



#endif

