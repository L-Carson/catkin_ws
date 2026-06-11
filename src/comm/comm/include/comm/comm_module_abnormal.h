
#ifndef __COMM_MODULE_ABNORMAL_H__
#define __COMM_MODULE_ABNORMAL_H__

#include "comm_msg/module_abnormal_info.h"
#include "comm/comm_label.h"
#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <ros/ros.h>

namespace COMM
{

/**************************************************************************************
功能描述: 异常模块定义
修改记录:
**************************************************************************************/
enum ABNORMAL_MODULE_ENUM
{
    ABNORMAL_MODULE_MAIN_LIDAR,
    ABNORMAL_MODULE_BLIND_LIDAR,
    ABNORMAL_MODULE_BLIND_LIDAR2,
    ABNORMAL_MODULE_BLIND_LIDAR3,
    ABNORMAL_MODULE_BLIND_LIDAR4,
    ABNORMAL_MODULE_IMU,
    ABNORMAL_MODULE_ULTRASONIC,
    ABNORMAL_MODULE_MNL,
    ABNORMAL_MODULE_PNP,
    ABNORMAL_MODULE_PNC,
    ABNORMAL_MODULE_BUTT            /* 新增模块在此之前依次定义 */
};

struct ABNORMAL_MODULE_INFO_STRU
{
    UINT32      module;
    std::string strDes;
};

const ABNORMAL_MODULE_INFO_STRU g_astAbnormalModuleInfo[] = {
    {ABNORMAL_MODULE_MAIN_LIDAR,    Lidar[LIDAR_ENUM::MAIN_LIDAR]},
    {ABNORMAL_MODULE_BLIND_LIDAR,   Lidar[LIDAR_ENUM::BLIND_LIDAR]},
    {ABNORMAL_MODULE_BLIND_LIDAR2,  Lidar[LIDAR_ENUM::BLIND_LIDAR2]},
    {ABNORMAL_MODULE_BLIND_LIDAR3,  Lidar[LIDAR_ENUM::BLIND_LIDAR3]},
    {ABNORMAL_MODULE_BLIND_LIDAR4,  Lidar[LIDAR_ENUM::BLIND_LIDAR4]},
    {ABNORMAL_MODULE_IMU,           "imu"},
    {ABNORMAL_MODULE_ULTRASONIC,    "ultrasonic"},
    {ABNORMAL_MODULE_MNL,           "mnl"},
    {ABNORMAL_MODULE_PNP,           "pnp"},
    {ABNORMAL_MODULE_PNC,           "pnc"}
};


/**************************************************************************************
功能描述: 模块异常发布
          说明:
            (1) 本机制用于关键模块在发生关键异常时,快速发布异常,进而触发紧急停车;
            (2) 本机制不能替代正常业务流程中异常保护处理,比如定位异常时发布结果为异常,感知异常时无感知数据发布,规划异常时发布停车指令等.
            (3) 本机制以最新收到的状态作为对应模块是否异常的结果.如果没有收到最新状态,则视为状态未发生变化,无超时机制.
            (4) 本机制中错误码为0表示无异常,非0表示异常.
修改记录:
**************************************************************************************/
class MODULE_ABNORMAL_PUBLISHER_C
{
public:
    MODULE_ABNORMAL_PUBLISHER_C(ros::NodeHandle &nh, ABNORMAL_MODULE_ENUM enModule) : enModule_(enModule)
    {
        objPublisher_ = nh.advertise<comm_msg::module_abnormal_info>(TOPIC_NAME_MODULE_ABNORMAL_INFO, 1, true);
    }

    void SendAbnormalInfo(INT32 errCode);

private:
    const ABNORMAL_MODULE_ENUM enModule_;

    ros::Publisher             objPublisher_;
    INT32                      oldErrCode_{0};

    void SendMsg(INT32 errCode);
};


/**************************************************************************************
功能描述: 模块异常订阅
修改记录:
**************************************************************************************/
class MODULE_ABNORMAL_SUBSCRIBER_C
{
public:
    MODULE_ABNORMAL_SUBSCRIBER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_MODULE_ABNORMAL_INFO, 100, &MODULE_ABNORMAL_SUBSCRIBER_C::ReceiveMsgCallBack, this);

        InitModuleAbnormalInfo();
    }

    BOOL IsAnyModulesAbnormal(void);

private:
    struct MODULE_ABNORMAL_INFO_STRU
    {
        UINT32      module;
        std::string strDes;
        INT32       errCode;
    };

    ros::Subscriber           objSubscriber_;
    MODULE_ABNORMAL_INFO_STRU astModuleAbnormalInfo_[ABNORMAL_MODULE_BUTT];
    BOOL                      isInitSucc_{false};
    UINT32                    abnormalModuleNum_{0};

    void InitModuleAbnormalInfo(void);
    void ReceiveMsgCallBack(const comm_msg::module_abnormal_info::ConstPtr &pstMsg);
};


}




#endif

