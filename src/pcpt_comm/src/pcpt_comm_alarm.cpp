#include "pcpt_comm/pcpt_comm_alarm.h"

namespace PCPT {

std::recursive_mutex    PCPT_ALARM_MANAGER_C::mutex_r_;
PCPT_ALARM_MANAGER_C*   PCPT_ALARM_MANAGER_C::pInstance_{nullptr};

/**************************************************************************************
功能描述: 注册告警，当形参regId为默认值0时，regId顺延
修改记录:
**************************************************************************************/
void PCPT_ALARM_MANAGER_C::RegAlarm(const string strType, PCPT_ALARM_ENUM enType, int regId)
{
    PCPT_ALARM_STRU* pstAlarm = GetAlarmInfo(strType);
    if (pstAlarm) {
        ST_LOG_WARN("The alarm <%s> has been registered, and the registration ID is:0X%x.", strType.c_str(), pstAlarm->regId);
        return;
    }

    if (regId) {
        RegAlarmId(strType, enType, regId);
    } else {
        int maxId = 0;
        for (auto iter = umTypeId_.begin(); iter != umTypeId_.end(); ++iter) {
            PCPT_ALARM_STRU& stAlarm = iter->second;
            if (stAlarm.enType == enType) maxId = MAX(maxId, stAlarm.regId);
        }
        RegAlarmId(strType, enType, ++maxId);
    }
    ST_LOG_INFO("Now register the alarm <%s>, and the registration ID is:0X%x.", strType.c_str(), regId);

    Alarm(strType, false, true);  //启动程序时默认复位所有故障
}

/**************************************************************************************
功能描述: 获取告警信息
修改记录:
**************************************************************************************/
PCPT_ALARM_STRU* PCPT_ALARM_MANAGER_C::GetAlarmInfo(const string strType)
{
    auto iter = umTypeId_.find(strType);
    if (iter == umTypeId_.end()) {
        ST_LOG_WARN("No alarm type <%s> exists.", strType.c_str());
        return nullptr;
    }

    return &(iter->second);
}

/**************************************************************************************
功能描述: 告警管理
修改记录: isFault: 是否故障
          isReset: 是否复位，将当前故障状态设置isFault
**************************************************************************************/
void PCPT_ALARM_MANAGER_C::Alarm(const string strType, bool isFault, bool isReset)
{
    PCPT_ALARM_STRU* pstAlarm = GetAlarmInfo(strType);
    if (!pstAlarm) return;

    if (isReset) AlarmId(pstAlarm->regId, isFault); //复位故障状态为isFault

    if (pstAlarm->isFault == isFault) return;       //避免重复上报
    if (pstAlarm->enType == EN_ALARM_LIDAR) Alarm("Lidar", isFault);
    if (pstAlarm->enType == EN_ALARM_RADAR) Alarm("Radar", isFault);

    pstAlarm->isFault = isFault;
    AlarmId(pstAlarm->regId, isFault);
}

/**************************************************************************************
功能描述: 初始化默认告警注册信息
修改记录:
**************************************************************************************/
void PCPT_ALARM_MANAGER_C::InitDefaultReg()
{
//    RegAlarm("Lidar",                              EN_ALARM_NULL,  ALARM_ID_LIDAR);                        //激光雷达故障告警
//    RegAlarm(Lidar[LIDAR_ENUM::MAIN_LIDAR],        EN_ALARM_LIDAR, ALARM_ID_LIDAR + 1);                    //主雷达故障（定位雷达）告警
//    RegAlarm(Lidar[LIDAR_ENUM::BLIND_LIDAR],       EN_ALARM_LIDAR, ALARM_ID_LIDAR + 2);                    //左/中补盲雷达故障告警
//    RegAlarm(Lidar[LIDAR_ENUM::BLIND_LIDAR2],      EN_ALARM_LIDAR, ALARM_ID_LIDAR + 3);                    //右补盲雷达故障告警

//    RegAlarm("Radar",                              EN_ALARM_NULL,  ALARM_ID_RADAR);                        //毫米波雷达故障告警
//    RegAlarm(Radar[RADAR_ENUM::LEFT_RADAR],        EN_ALARM_RADAR, ALARM_ID_RADAR + 1);                    //左毫米波雷达故障告警
//    RegAlarm(Radar[RADAR_ENUM::FRONT_RADAR],       EN_ALARM_RADAR, ALARM_ID_RADAR + 2);                    //前毫米波雷达故障告警
//    RegAlarm(Radar[RADAR_ENUM::RIGHT_RADAR],       EN_ALARM_RADAR, ALARM_ID_RADAR + 3);                    //右毫米波雷达故障告警
//    RegAlarm(Radar[RADAR_ENUM::BACK_RADAR],        EN_ALARM_RADAR, ALARM_ID_RADAR + 4);                    //后毫米波雷达故障告警

//    RegAlarm("Ultrasonic",                         EN_ALARM_NULL,  ALARM_ID_ULTRASONIC);                   //超声波传感器故障告警
//    RegAlarm("DepthCamera",                        EN_ALARM_NULL,  ALARM_ID_ULTRASONIC);                   //深度相机故障告警
//    RegAlarm("Tof",                                EN_ALARM_NULL,  ALARM_ID_ULTRASONIC);                   //tof传感器故障告警

    RegAlarm("IPU",                                  EN_ALARM_NULL,  ALARM_ID_IPU);                          //图像处理单元故障告警(image processing unit)
    RegAlarm(PCPT_MARK_PIPELINE,                     EN_ALARM_NULL,  ALARM_ID_VISION_PIPELINE);              //视觉检测到水管告警
    RegAlarm(PCPT_MARK_THICK_PIPE,                   EN_ALARM_NULL,  ALARM_ID_VISION_THICKPIPE);             //视觉检测到粗水管告警
    RegAlarm(PCPT_MARK_CORD,                         EN_ALARM_NULL,  ALARM_ID_VISINO_CORD);                  //视觉检测到电线、缆绳
    RegAlarm(PCPT_MARK_SLOPE_PAD,                    EN_ALARM_NULL,  ALARM_ID_VISION_SLOPE_PAD);             //视觉检测到斜坡垫告警

    RegAlarm(PCPT_MARK_LOW_OBST,                     EN_ALARM_NULL,  0X50201);                               //检测到低矮障碍
    RegAlarm(PCPT_MARK_UNDERGROUND_OBST,             EN_ALARM_NULL,  0x50202);                               //检测到负高障碍

    RegAlarm(PCPT_MARK_DUSTBIN_FULL,                 EN_ALARM_NULL,  ALARM_ID_DUSTBIN_FULL);                 //站点垃圾箱已满
    RegAlarm(PCPT_MARK_DUSTBIN_NULL,                 EN_ALARM_NULL,  ALARM_ID_DUSTBIN_NULL);                 //站点垃圾箱不存在
    RegAlarm(PCPT_MARK_TRASHBOX_LIFT_MOTOR_STICK,    EN_ALARM_NULL,  ALARM_ID_TRASHBOX_LIFT_MOTOR_STICK);    //车载垃圾箱抬升不到位
    RegAlarm(PCPT_MARK_TRASHBOX_ROLL_MOTOR_STICK,    EN_ALARM_NULL,  ALARM_ID_TRASHBOX_ROLL_MOTOR_STICK);    //车载垃圾箱翻斗收回不到位
    RegAlarm(PCPT_MARK_TRASHBOX_ROLL_MOTOR_WITHDRAW, EN_ALARM_NULL,  ALARM_ID_TRASHBOX_ROLL_MOTOR_WITHDRAW); //车载垃圾箱翻斗收回不到位
}

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int PCPT_ALARM_MANAGER_C::Init()
{
    pAlarm_= BasicAlarmManager::Create();
    InitDefaultReg();

    ST_LOG_INFO("Init Success.");
    return 0;
}

/**************************************************************************************
功能描述: 注册
修改记录:
**************************************************************************************/
void PCPT_ALARM_MANAGER_C::Reg(int regId)
{
    pAlarm_->Reg(regId, &PCPT_ALARM_MANAGER_C::AlarmReportCallBack, this, &PCPT_ALARM_MANAGER_C::AlarmRecoverCallBack, this);
}

/**************************************************************************************
功能描述: 注册告警ID
修改记录:
**************************************************************************************/
void PCPT_ALARM_MANAGER_C::RegAlarmId(const string strType, PCPT_ALARM_ENUM enType, int regId)
{
    PCPT_ALARM_STRU stAlarm;
    stAlarm.regId  = regId;
    stAlarm.enType = enType;
    umTypeId_.emplace(strType, stAlarm);
    Reg(regId);
}

/**************************************************************************************
功能描述: 告警
修改记录:
**************************************************************************************/
void PCPT_ALARM_MANAGER_C::AlarmId(int id, bool isFault)
{
    Json::Value json;
    json["AlarmId"] = id;
    if (isFault) {
        pAlarm_->Report(json);
        ST_LOG_ERR("发生告警：0x%x.", id);
    } else {
        pAlarm_->Recover(json);
        ST_LOG_WARN("取消告警：0x%x.", id);
    }
}


}
