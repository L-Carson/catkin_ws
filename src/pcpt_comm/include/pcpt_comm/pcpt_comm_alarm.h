#ifndef __PCPT_ALARM_MANAGER_H__
#define __PCPT_ALARM_MANAGER_H__

#include "log_handler.h"
#include "alarm_manager.h"
#include "json_handler.h"
#include <unordered_map>

#include "comm/comm_base.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_singleton.h"


using namespace std;

namespace PCPT {

constexpr char pcptLogSaveFileFlag[] = "pcpt_node_log";

enum PCPT_ALARM_ENUM {
    EN_ALARM_NULL,
    EN_ALARM_LIDAR,     //激光雷达
    EN_ALARM_RADAR,     //毫米波雷达
    EN_ALARM_US,        //超声波雷达
    EN_ALARM_TOF,       //tof
    EN_ALARM_CAMERA,    //相机
};

struct PCPT_ALARM_STRU {
    int             regId;
    bool            isFault{false};
    PCPT_ALARM_ENUM enType{EN_ALARM_NULL};
};

class PCPT_ALARM_MANAGER_C
{
public:
    PCPT_ALARM_MANAGER_C(void)
    {
        if (Init()) exit(EXIT_FAILURE);
    }

    ~PCPT_ALARM_MANAGER_C(void)
    {
        /* 清理注册，防止析构后本对象注册的回调函数被调用。 */
        if (pAlarm_ != nullptr) {
            for (auto iter = umTypeId_.begin(); iter != umTypeId_.end(); ++iter) {
                const PCPT_ALARM_STRU &stAlarm = iter->second;
                (void)pAlarm_->UnReg(stAlarm.regId);
            }

            umTypeId_.clear();
        }
    }

    void RegAlarm(const string strType, PCPT_ALARM_ENUM enType = EN_ALARM_NULL, int regId = 0);
    void Alarm(const string strType, bool isFault, bool isReset = false);

    /* 返回对象指针 */
    static PCPT_ALARM_MANAGER_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_ALARM_MANAGER_C();
            PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
            ST_LOG_INFO("Create Object Succ.");
        }

        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

private:
    PCPT_ALARM_MANAGER_C(const PCPT_ALARM_MANAGER_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_ALARM_MANAGER_C &operator=(const PCPT_ALARM_MANAGER_C &) = delete; // 禁止使用拷贝赋值运算符

private:
    /**
     * 告警发生回调
     * @param json
     */
    void AlarmReportCallBack(JsonValueConstPtr& json) {
        Json::Value obj = *json;
        std::string str = JsonHandler::JsonToString(obj);
        LOG_INFO(pcptLogSaveFileFlag, "%s", str.c_str());
        int id = obj["AlarmId"].asInt();
        LOG_INFO(pcptLogSaveFileFlag, "发生告警：%d[0x%x]", id, id);
    }

    /**
     * 告警取消回调
     * @param json
     */
    void AlarmRecoverCallBack(JsonValueConstPtr& json) {
        Json::Value obj = *json;
        std::string str = JsonHandler::JsonToString(obj);
        LOG_INFO(pcptLogSaveFileFlag, "%s", str.c_str());
        int id = obj["AlarmId"].asInt();
        LOG_INFO(pcptLogSaveFileFlag, "取消告警：%d[0x%x]", id, id);
    }

private:
    BasicAlarmManager* pAlarm_;

    std::unordered_map<std::string, PCPT_ALARM_STRU> umTypeId_; // <type - id>

    static std::recursive_mutex     mutex_r_;       // 递归锁
    static PCPT_ALARM_MANAGER_C*    pInstance_;

    int  Init();
    void InitDefaultReg();
    PCPT_ALARM_STRU* GetAlarmInfo(const string strType);
    void Reg(int regId);
    void RegAlarmId(const string strType, PCPT_ALARM_ENUM enType, int regId);
    void AlarmId(int id, bool isFault);
};

}


#endif

