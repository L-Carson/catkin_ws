#pragma once
#include "comm_msg/PoseInfo.h"
#include "comm_msg/detect_target.h"
#include "comm/comm_msg_move_info.h"
#include "comm/comm_msg_equipment_info.h"
#include "comm/comm_msg_equipment_pose.h"
#include "comm/comm_msg_dev_info.h"
#include "comm/comm_msg_dcu_info.h"
#include "comm/comm_msg_qr_code_pose.h"
#include "comm/comm_msg_nav_state.h"
#include "comm/comm_msg_traffic_light.h"
#include "comm/comm_topic_name.h"
#include "comm/comm_ros_base.h"
#include "comm_ros_extend/comm_ros_base_extend.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_tf_ros.h"
#include "pcpt_comm/pcpt_comm_singleton.h"

#include <sensor_msgs/Imu.h>

struct LOC_2D_STRU {
    LOC_2D_STRU(void) {}
    LOC_2D_STRU(const double &stamp, const COOR_2D_POSE_STRU& stPose) {
        this->stamp  = stamp;
        this->stPose = stPose;
    }

    double stamp{0};
    COOR_2D_POSE_STRU stPose;
};

struct LOC_3D_STRU {
    LOC_3D_STRU(void) {
        stamp = 0;
        stPose.SetZero();
    }

    LOC_3D_STRU(const double &stamp, const COOR_3D_POSE_STRU& stPose) {
        this->stamp  = stamp;
        this->stPose = stPose;
    }

    float x(void)     const {return stPose.stPos.x;}
    float y(void)     const {return stPose.stPos.y;}
    float z(void)     const {return stPose.stPos.z;}
    float roll(void)  const {return stPose.stAtt.roll;}
    float pitch(void) const {return stPose.stAtt.pitch;}
    float yaw(void)   const {return stPose.stAtt.yaw;}

    double stamp{0};
    COOR_3D_POSE_STRU stPose;
};

/**
 * @brief 3D定位信息接收器
 *
 */
class LOCATION_3D_INFO_RECEIVER_C
{
public:
    LOCATION_3D_INFO_RECEIVER_C(ros::NodeHandle &nh, const char *pcTopicName = TOPIC_NAME_3D_LOCATION_INFO_W2, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(pcTopicName, 1, &LOCATION_3D_INFO_RECEIVER_C::ReceiveMsgCallBack, this);
        enState_ = MAP_NODE_STATE_IDLE;
    };

    BOOL  IsNormal(void);
    BOOL  IsNormalNew(void);
    BOOL  IsChangingMap(void);
    BOOL  IsCreatingMap(void);

    // 3D定位信息接口
    void  Get(TIMESTAMP_POSE_STRU &stTsPose);
    INT32 GetNormal(TIMESTAMP_POSE_STRU &stTsPose);
    TIMESTAMP_POSE_STRU *GetNormal(void);

    // 2D定位信息接口
    void Get(COOR_2D_POSE_STRU &stPose);
    int  GetNormal(COOR_2D_POSE_STRU &stPose);

private:
    TIMER_C              objInvalidTimer_;
    ros::Subscriber      receiver_;
    MAP_NODE_STATE_ENUM  enState_;
    TIMESTAMP_POSE_STRU  stTsPose_;        // 带时间戳3D位姿缓存

private:
    void  ReceiveMsgCallBack(const comm_msg::PoseInfo::ConstPtr &pstMsg);
};

/**
 * @brief 2D定位信息接口（推荐使用3D定位信息接口）
 *
 */
class PCPT_LOC_2D_MSG_C {
public:
    PCPT_LOC_2D_MSG_C(ros::NodeHandle &nh, float cacheTime = 0.0, double overTime = 0.2)
    : objLocMsgReceiver_(nh, overTime)
    , objLocInfoReceiver_(nh, TOPIC_NAME_3D_LOCATION_INFO_W2, overTime)
    , cacheTime_(cacheTime)
    , isCache_(SIG(cacheTime))
    {}

    ~PCPT_LOC_2D_MSG_C(){}

    void UptLocationMsg();
    int  GetLocationMsg(COOR_2D_POSE_STRU& stPose);
    int  GetLocationMsg(LOC_2D_STRU& stLoc);
    int  GetLocationMsg(const double stamp, COOR_2D_POSE_STRU& stLoc);

private:
    TIMER_C                      objTimerLog_{1.0, false};
    LOCATION_MSG_RECEIVER_C      objLocMsgReceiver_;  // 2D定位信息接收器
    LOCATION_3D_INFO_RECEIVER_C  objLocInfoReceiver_; // 3D定位信息接收器
    float                        cacheTime_;          // 缓存定位数据时长
    bool                         isCache_;            // 是否缓存定位数据
    bool                         isNormal_{false};    // 定位是否正常
    double                       stamp_;              // 接口获取定位的时间戳
    COOR_2D_POSE_STRU            stCarPoseByMap_;     // 2D车体位姿（map坐标系）
    list<LOC_2D_STRU>            listLoc_;
    std::mutex                   mutex_;

    void CacheLocationMsg(COOR_2D_POSE_STRU& stPose, double& stamp);
};

/**
 * @brief 3D定位信息接口
 *
 */
class PCPT_LOC_3D_MSG_C {
public:
    PCPT_LOC_3D_MSG_C(ros::NodeHandle &nh, const string strMark = "", float cacheTime = 0.0, double overTime = 0.2)
    : objLocMsgReceiver_(nh, TOPIC_NAME_3D_LOCATION_INFO_SAIJIE, overTime)
    , objLocInfoReceiver_(nh, TOPIC_NAME_3D_LOCATION_INFO_W2, overTime)
    , cacheTime_(cacheTime)
    , isCache_(SIG(cacheTime))
    , strMark_(strMark.empty() ? "Loc" : strMark)
    {
        // 若为室内线机型，则通过tf获取定位
        pobjCarTypeParser_ = CAR_TYPE_PARSER_C::GetInstance();
        if (!pobjCarTypeParser_->IsOutdoorSweeper()) {
            pobjTfApi_.reset(new PCPT::TF_QUERY_ROS_API_C());
        }
    }

    ~PCPT_LOC_3D_MSG_C(){}

    void UptLocationMsg();

    // 3D定位信息接口
    int  GetLocationMsg(COOR_3D_POSE_STRU &stPose) const;
    int  GetLocationMsg(LOC_3D_STRU& stLoc) const;
    int  GetLocationMsg(const double stamp, COOR_3D_POSE_STRU& stPose);

    // 2D定位信息接口
    int  GetLocationMsg(COOR_2D_POSE_STRU& stPose) const;
    int  GetLocationMsg(LOC_2D_STRU& stLoc) const;
    int  GetLocationMsg(const double stamp, COOR_2D_POSE_STRU& stPose);

private:
    TIMER_C                        objTimerLog1_{1.0, false};
    TIMER_C                        objTimerLog2_{1.0, false};
    CAR_TYPE_PARSER_C::Ptr         pobjCarTypeParser_ = nullptr; // 车体型号解析器
    LOCATION_3D_MSG_RECEIVER_C     objLocMsgReceiver_;   // 3D定位信息接收器
    LOCATION_3D_INFO_RECEIVER_C    objLocInfoReceiver_;  // 3D定位信息接收器
    PCPT::TF_QUERY_ROS_API_C::Ptr  pobjTfApi_;           // TF查询ROS接口
    float                          cacheTime_;           // 缓存定位数据时长
    bool                           isCache_;             // 是否缓存定位数据
    bool                           isNormal_{false};     // 定位是否正常
    string                         strMark_;
    TIMESTAMP_POSE_STRU            stCarPoseByMap_;      // 带时间戳3D位姿缓存[base-->map]
    tf::Stamped<tf::Pose>          tfCar2MapStamped_;    // 带时间戳tf数据缓存[base-->map]
    list<LOC_3D_STRU>              listLoc_;             //
    mutable std::mutex             mutex_;
    void CacheLocationMsg(TIMESTAMP_POSE_STRU& stCarPoseByMap);
    bool UptLocationMsgByTf(TIMESTAMP_POSE_STRU &stCarPoseByMap);
};

/**************************************************************************************
功能描述: 实时车体位姿
修改记录:
**************************************************************************************/
class PCPT_CAR_POSE_C
{
public:
    PCPT_CAR_POSE_C(){}
    PCPT_CAR_POSE_C(const PCPT_CAR_POSE_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_CAR_POSE_C &operator=(const PCPT_CAR_POSE_C &) = delete;   // 禁止使用拷贝赋值运算符

    static PCPT_CAR_POSE_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_CAR_POSE_C();
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

    void UptCarPose(const bool isValid, const LOC_3D_STRU& stCarPose = LOC_3D_STRU{});

    int  Get2dCarPose(LOC_2D_STRU& stCarPose);
    int  Get3dCarPose(LOC_3D_STRU& stCarPose);

    int  Get2dCarPose(COOR_2D_POSE_STRU& stCarPose);
    int  Get3dCarPose(COOR_3D_POSE_STRU& stCarPose);
    int  Get2dCarPos(COORDINATE_2D_STRU& stCarPos);
    int  Get3dCarPos(COORDINATE_3D_STRU& stCarPos);

    const LOC_2D_STRU        Get2dLocInfo();
    const LOC_3D_STRU        Get3dLocInfo();
    const COOR_2D_POSE_STRU  Get2dCarPose();
    const COOR_3D_POSE_STRU  Get3dCarPose();
    const COORDINATE_2D_STRU Get2dCarPos();
    const COORDINATE_3D_STRU Get3dCarPos();

private:
    bool                            isValid_{false};
    LOC_3D_STRU                     stCarPose_;

    std::recursive_mutex            mutex_;         // 递归锁
    static std::recursive_mutex     mutex_r_;
    static PCPT_CAR_POSE_C*         pInstance_;
};

/**
 * @brief 导航状态信息接口
 *
 */
class PCPT_NAV_STATE_MSG_C {
public:
    PCPT_NAV_STATE_MSG_C(ros::NodeHandle &nh) : objNavReceiver_(nh) {}
    ~PCPT_NAV_STATE_MSG_C(){}

    void UptNavStateMsg();
    void GetNavStateMsg(NAV_STATE_STRU &stNavState);

private:
    NAV_STATE_RECEIVER_C objNavReceiver_;  // 导航状态接收器
    NAV_STATE_STRU       stNavState_;      // 导航状态（规划运动趋势）
    std::mutex           mutex_;
};

/**
 * @brief imu信息综合接口
 *
 */
class PCPT_IMU_DATA_C {
public:
    PCPT_IMU_DATA_C(ros::NodeHandle &nh, const uint32_t buffSize = 1, double overTime = 0.1)
    : objImuMsgReceiver_(nh, "/yis_imu", buffSize)
    , objImuMsgReceiverW2_(nh, "/imu_mmd/data", buffSize)
    , objTimer_(overTime, false)
    , buffSize_(buffSize)
    {}

    ~PCPT_IMU_DATA_C(){}

    void UptImuData();
    int  GetImuData(deque<IMU_DATA_STRU>& deqImuData);
    int  GetImuData(IMU_DATA_STRU& stImuData);
    int  GetImuData(const double stamp, IMU_DATA_STRU& stImuData);

private:
    IMU_MSG_RECEIVER_C      objImuMsgReceiver_;
    IMU_MSG_RECEIVER_C      objImuMsgReceiverW2_;
    TIMER_C                 objTimer_;
    std::mutex              mutex_;                 // 缓存数据互斥锁
    uint32_t                buffSize_;              // 缓存队列尺寸
    deque<IMU_DATA_STRU>    deqImuData_;            // imu数据缓存队列
};

/**
 * @brief 运动信息接口
 *
 */
class PCPT_MOVE_INFO_C {
public:
    PCPT_MOVE_INFO_C(ros::NodeHandle &nh, const uint32_t buffSize = 1)
    : objMoveInfo_(nh)
    , buffSize_(buffSize)
    {}

    ~PCPT_MOVE_INFO_C(){}

    void UptMoveInfo();
    int  GetMoveInfo(deque<DCU_MOVE_INFO_EX_STRU>& deqMoveInfo);
    int  GetMoveInfo(DCU_MOVE_INFO_EX_STRU& stMoveInfo);
    int  GetMoveInfo(const double stamp, DCU_MOVE_INFO_EX_STRU& stMoveInfo);

private:
    MOVE_INFO_RECEIVER_C            objMoveInfo_;
    std::mutex                      mutex_;                 // 缓存数据互斥锁
    bool                            isDataValid_{false};    // 缓存数据有效标识
    uint32_t                        buffSize_;              // 缓存队列尺寸
    deque<DCU_MOVE_INFO_EX_STRU>    deqMoveInfo_;           // 运动信息队列
};

/**
 * @brief 轮速信息接口
 *
 */
class PCPT_VELOCITY_C {
public:
    PCPT_VELOCITY_C(ros::NodeHandle &nh) : objVelMsgReceiver_(nh) {}
    ~PCPT_VELOCITY_C(){}

    void UptVelData();
    int  GetVelData(float& vel);

private:
    VEL_MSG_RECEIVER_C      objVelMsgReceiver_;
    std::mutex              mutex_;
    bool                    isVelDataValid_{false};
    float                   vel_{0};
};

class PCPT_VELOCITY_INST_C {
public:
    PCPT_VELOCITY_INST_C() {}
    PCPT_VELOCITY_INST_C(const PCPT_VELOCITY_INST_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_VELOCITY_INST_C &operator=(const PCPT_VELOCITY_INST_C &) = delete; // 禁止使用拷贝赋值运算符

    static PCPT_VELOCITY_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_VELOCITY_INST_C();
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

    void UptVelData(const bool isValid, const float& vel);
    int  GetVelData(float& vel);

private:
    bool                            isValid_{false};
    float                           vel_{0};
    DCU_MOVE_DEV_INFO_STRU          stDevInfo_;
    std::mutex                      mutex_;
    static std::recursive_mutex     mutex_r_;
    static PCPT_VELOCITY_INST_C*    pInstance_;
};

/**
 * @brief 设备信息
 *
 */
class PCPT_DEVICE_INFO_C {
public:
    PCPT_DEVICE_INFO_C(ros::NodeHandle &nh, double overTime = 0.3) : objDevInfoRecv_(nh, overTime) {}
    ~PCPT_DEVICE_INFO_C(){}

    void UptDevInfo();
    int  GetDevInfo(DCU_MOVE_DEV_INFO_STRU& stDevInfo);

private:
    DEV_INFO_RECEIVER_C     objDevInfoRecv_; // 设备信息接收器器;
    std::mutex              mutex_;
    bool                    isDevInfoValid_{false};
    DCU_MOVE_DEV_INFO_STRU  stDevInfo_;
};

class PCPT_DEVICE_INFO_INST_C {
public:
    PCPT_DEVICE_INFO_INST_C(){}
    PCPT_DEVICE_INFO_INST_C(const PCPT_DEVICE_INFO_INST_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_DEVICE_INFO_INST_C &operator=(const PCPT_DEVICE_INFO_INST_C &) = delete;   // 禁止使用拷贝赋值运算符

    static PCPT_DEVICE_INFO_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_DEVICE_INFO_INST_C();
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

    void UptDevInfo(const bool isValid, const DCU_MOVE_DEV_INFO_STRU& stDevInfo);
    int  GetDevInfo(DCU_MOVE_DEV_INFO_STRU& stDevInfo);

private:
    bool                            isValid_{false};
    DCU_MOVE_DEV_INFO_STRU          stDevInfo_;
    std::mutex                      mutex_;
    static std::recursive_mutex     mutex_r_;
    static PCPT_DEVICE_INFO_INST_C* pInstance_;
};

/**
 * @brief 装备信息
 *
 */
class PCPT_EQUIPMENT_INFO_C {
public:
    PCPT_EQUIPMENT_INFO_C(ros::NodeHandle &nh, double overTime = 0.3) : objEquInfoMsgReceiver_(nh, overTime) {}
    ~PCPT_EQUIPMENT_INFO_C(){}

    void UptEquipmentInfo();
    int  GetEquipmentInfo(EQUIPMENT_INFO_STRU& stEquipmentInfo);

private:
    EQUIPMENT_INFO_RECEIVER_C   objEquInfoMsgReceiver_; // 装备信息接收器
    std::mutex                  mutex_;
    bool                        isDataValid_{false};
    EQUIPMENT_INFO_STRU         stEquipmentInfo_;
};

class PCPT_EQUIPMENT_INFO_INST_C {
public:
    PCPT_EQUIPMENT_INFO_INST_C(){}
    PCPT_EQUIPMENT_INFO_INST_C(const PCPT_EQUIPMENT_INFO_INST_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_EQUIPMENT_INFO_INST_C &operator=(const PCPT_EQUIPMENT_INFO_INST_C &) = delete; // 禁止使用拷贝赋值运算符

    void UptEquipmentInfo(const bool isValid, const EQUIPMENT_INFO_STRU& stEquipmentInfo);
    int  GetEquipmentInfo(EQUIPMENT_INFO_STRU& stEquipmentInfo);

    static PCPT_EQUIPMENT_INFO_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_EQUIPMENT_INFO_INST_C();
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
    bool                                isValid_{false};
    EQUIPMENT_INFO_STRU                 stEquipmentInfo_;
    std::mutex                          mutex_;
    static std::recursive_mutex         mutex_r_;
    static PCPT_EQUIPMENT_INFO_INST_C*  pInstance_;
};

/**
 * @brief 装备位姿
 *
 */
class PCPT_EQUIPMENT_POSE_C {
public:
    PCPT_EQUIPMENT_POSE_C(ros::NodeHandle &nh, double overTime = 0.2) : objRcv_(nh, overTime) {}
    ~PCPT_EQUIPMENT_POSE_C(){}

    void UptEquipmentPose();
    int  GetEquipmentPose(COOR_2D_POSE_STRU& stEquipmentPose);

private:
    EQUIPMENT_POSE_RECEIVER_C   objRcv_;    // 装备位姿接收器;
    std::mutex                  mutex_;
    bool                        isDataValid_{false};
    COOR_2D_POSE_STRU           stEquipmentPose_;
};

class PCPT_EQUIPMENT_POSE_INST_C {
public:
    PCPT_EQUIPMENT_POSE_INST_C() {}
    PCPT_EQUIPMENT_POSE_INST_C(const PCPT_EQUIPMENT_POSE_INST_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_EQUIPMENT_POSE_INST_C &operator=(const PCPT_EQUIPMENT_POSE_INST_C &) = delete; // 禁止使用拷贝赋值运算符

    void UptEquipmentPose(const bool isValid, const COOR_2D_POSE_STRU& stEquipmentPose);
    int  GetEquipmentPose(COOR_2D_POSE_STRU& stEquipmentPose);

    static PCPT_EQUIPMENT_POSE_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_EQUIPMENT_POSE_INST_C();
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
    bool                                isValid_{false};
    COOR_2D_POSE_STRU                   stEquipmentPose_;
    std::mutex                          mutex_;
    static std::recursive_mutex         mutex_r_;
    static PCPT_EQUIPMENT_POSE_INST_C*  pInstance_;
};

/**
 * @brief DCU信息
 *
 */
class PCPT_DCU_INFO_C {
public:
    PCPT_DCU_INFO_C(ros::NodeHandle &nh, double overTime = 2.5) : objRcv_(nh, overTime) {}
    ~PCPT_DCU_INFO_C(){}

    void UptDcuInfo();
    int  GetDcuInfo(DCU_INFO_STRU& stDcuInfo);

private:
    DCU_INFO_RECEIVER_C     objRcv_;
    DCU_INFO_STRU           stDcuInfo_;
    std::mutex              mutex_;
    bool                    isDataValid_{false};
};

class PCPT_DCU_INFO_INST_C {
public:
    PCPT_DCU_INFO_INST_C() {}
    PCPT_DCU_INFO_INST_C(const PCPT_DCU_INFO_INST_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_DCU_INFO_INST_C &operator=(const PCPT_DCU_INFO_INST_C &) = delete; // 禁止使用拷贝赋值运算符

    void UptDcuInfo(const bool isValid, const DCU_INFO_STRU& stDcuInfo);
    int  GetDcuInfo(DCU_INFO_STRU& stDcuInfo);

    static PCPT_DCU_INFO_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_DCU_INFO_INST_C();
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
    bool                            isValid_{false};
    DCU_INFO_STRU                   stDcuInfo_;
    std::mutex                      mutex_;
    static std::recursive_mutex     mutex_r_;
    static PCPT_DCU_INFO_INST_C*    pInstance_;
};

/**
 * @brief 二维码位姿
 *
 */
class PCPT_QR_CODE_POSE_C
{
public:
    PCPT_QR_CODE_POSE_C(ros::NodeHandle &nh) : objRcv_(nh, 0.2) {}
    ~PCPT_QR_CODE_POSE_C(){}

    void UptQrCodePose();
    int  GetQrCodePose(POSE_ID_STRU& stQrCodePose);

private:
    QR_CODE_POSE_RECEIVER_C objRcv_;
    std::mutex              mutex_;
    bool                    isDataValid_{false};
    POSE_ID_STRU            stQrCodePose_;
};

class PCPT_QR_CODE_POSE_INST_C
{
public:
    PCPT_QR_CODE_POSE_INST_C(){}
    PCPT_QR_CODE_POSE_INST_C(const PCPT_QR_CODE_POSE_INST_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_QR_CODE_POSE_INST_C &operator=(const PCPT_QR_CODE_POSE_INST_C &) = delete; // 禁止使用拷贝赋值运算符

    void UptQrCodePose(const bool isValid, const POSE_ID_STRU& stQrCodePose);
    int  GetQrCodePose(POSE_ID_STRU& stQrCodePose);

    static PCPT_QR_CODE_POSE_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_QR_CODE_POSE_INST_C();
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
    bool                                isValid_{false};
    POSE_ID_STRU                        stQrCodePose_;
    std::mutex                          mutex_;
    static std::recursive_mutex         mutex_r_;
    static PCPT_QR_CODE_POSE_INST_C*    pInstance_;
};

/**************************************************************************************
功能描述: 获取里程计信息
修改记录:
**************************************************************************************/
class PCPT_ODOM_3D_C
{
public:
    PCPT_ODOM_3D_C(ros::NodeHandle &nh, const size_t buffSize = 1000) : buffSize_(buffSize)
    {
        receiver_ = nh.subscribe(TOPIC_ODOM_3D, 1000, &PCPT_ODOM_3D_C::ReceiveMsgCallBack, this, ros::TransportHints().tcpNoDelay());
    }

    void GetOdomData(deque<ODOM_DATA_STRU> &vOdomData);
    int  GetOdomData(ODOM_DATA_STRU  &stOdomData);
    size_t Buffsize() const {return buffSize_;}

private:
    ros::Subscriber         receiver_;
    std::mutex              mutex_;
    size_t                  buffSize_;
    deque<ODOM_DATA_STRU>   vOdomData_;

    void ReceiveMsgCallBack(const nav_msgs::Odometry::ConstPtr& pMsg);
};

class PCPT_ODOM_3D_INST_C
{
public:
    PCPT_ODOM_3D_INST_C() {}
    PCPT_ODOM_3D_INST_C(const PCPT_ODOM_3D_INST_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_ODOM_3D_INST_C &operator=(const PCPT_ODOM_3D_INST_C &) = delete;   // 禁止使用拷贝赋值运算符

    void UptOdomData(const size_t buffSize, const deque<ODOM_DATA_STRU> &vOdomData);
    int  GetOdomData(const double startStamp, deque<ODOM_DATA_STRU> &vOdomData);
    int  GetOdomData(ODOM_DATA_STRU  &stOdomData);

    static PCPT_ODOM_3D_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_ODOM_3D_INST_C();
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
    bool                            isValid_{false};
    deque<ODOM_DATA_STRU>           vOdomData_;
    std::mutex                      mutex_;
    static std::recursive_mutex     mutex_r_;
    static PCPT_ODOM_3D_INST_C*     pInstance_;
};

/**************************************************************************************
功能描述: 获取GNSS信息
修改记录:
**************************************************************************************/
class PCPT_GNSS_C
{
public:
    PCPT_GNSS_C(ros::NodeHandle &nh, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_GNSS_INFO, 1, &PCPT_GNSS_C::ReceiveMsgCallBack, this);
    }

    int GetGnssData(GPS_INFO_STRU &stGpsInfo);

private:
    ros::Subscriber         receiver_;
    TIMER_C                 objInvalidTimer_;
    GPS_INFO_STRU           stGpsInfo_;
    std::mutex              mutex_;

    void ReceiveMsgCallBack(const comm_msg::gpsInfo::ConstPtr &pstMsg);
};

class PCPT_GNSS_INST_C
{
public:
    PCPT_GNSS_INST_C() {}
    PCPT_GNSS_INST_C(const PCPT_GNSS_INST_C &other) = delete;       // 禁止使用拷贝构造函数
    PCPT_GNSS_INST_C &operator=(const PCPT_GNSS_INST_C &) = delete; // 禁止使用拷贝赋值运算符

    void UptGnssData(const bool isValid, const GPS_INFO_STRU &stGpsInfo);
    int  GetGnssData(GPS_INFO_STRU &stGpsInfo);

    static PCPT_GNSS_INST_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ == nullptr) {
            pInstance_ = new PCPT_GNSS_INST_C();
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
    bool                            isValid_{false};
    GPS_INFO_STRU                   stGpsInfo_;
    std::mutex                      mutex_;
    static std::recursive_mutex     mutex_r_;
    static PCPT_GNSS_INST_C*        pInstance_;
};

/**************************************************************************************
功能描述: 获取检测请求信息
修改记录:
**************************************************************************************/
class PCPT_DETECT_TARGET_MSG_C
{
public:
    PCPT_DETECT_TARGET_MSG_C(ros::NodeHandle &nh, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_DETECT_TARGET_DOOR, 1, &PCPT_DETECT_TARGET_MSG_C::ReceiveMsgCallBack, this);
    }

    int  GetDetectTargetInfo(uint8_t& type, VST_COOR& vstCoors);
    bool IsDetect();

private:
    ros::Subscriber         receiver_;
    TIMER_C                 objInvalidTimer_;
    uint8_t                 type_{0};   //1:开门 2:关门
    VST_COOR                vstCoors_;  //库房门位置
    std::recursive_mutex    mutex_;

    void ReceiveMsgCallBack(const comm_msg::detect_target::ConstPtr &pstMsg);
};

/**************************************************************************************
功能描述: 标记命令（充电桩反光条）
修改记录:
**************************************************************************************/
class PCPT_LABEL_CMD_C
{
public:
    PCPT_LABEL_CMD_C(ros::NodeHandle &nh)
    {
        subLabelCmd_ = nh.subscribe(TOPIC_NAME_LABEL_CMD, 1, &PCPT_LABEL_CMD_C::ReceiveMsgCallBack, this);
    }

    bool IsLabelChargingPile();

private:
    ros::Subscriber             subLabelCmd_;
    bool                        isLabel_{false};
    std::recursive_mutex        mutex_;
    TIMER_C                     objLabelTimer_{5.0};

    void ReceiveMsgCallBack(const std_msgs::Int8::ConstPtr pstCmdMsg);
};

/**************************************************************************************
功能描述: 获取导航红绿灯检测识别请求信息，subcribe订阅
修改记录:
**************************************************************************************/
class PCPT_TRAFFIC_LIGHT_TARGET_MSG_C
{
public :
    PCPT_TRAFFIC_LIGHT_TARGET_MSG_C(ros::NodeHandle & nh, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_TRAFFIC_LIGHT_TARGET, 1, &PCPT_TRAFFIC_LIGHT_TARGET_MSG_C::ReceiveMsgCallBack, this);
    }

    int GetTrafficLightTargetInfo(TRAFFIC_LIGHT_TARGET_STRU& stTrafficLight);

private:
    ros::Subscriber             receiver_;
    TIMER_C                     objInvalidTimer_;
    TRAFFIC_LIGHT_TARGET_STRU   stTrafficLight_;    // 交通灯信息
    std::recursive_mutex        mutex_;

    void ReceiveMsgCallBack(const comm_msg::traffic_light_target::ConstPtr &pstMsg);
};
