#ifndef __PCPT_DET_DYNAMIC_H__
#define __PCPT_DET_DYNAMIC_H__

#include <list>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include "comm/comm_msg.h"
#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/comm_car.h"
#include "comm/comm_ros_base.h"
#include "comm/comm_ros_trc.h"
#include "comm/comm_msg_nav_state.h"
#include "comm_msg/movTargetsArray.h"
#include "pcpt_comm/pcpt_comm_ros.h"
#include "pcpt_comm/pcpt_comm_abnormal.h"
#include "pcpt_comm/pcpt_comm_marker.h"
#include "pcpt_comm/pcpt_comm_hmi.h"
#include "pcpt_det_dynamic/pcpt_track.h"
#include "pcpt_det_dynamic/pcpt_trajectory_tracker.h"


using namespace std;

/**
 * @brief 跟踪目标marker结构
 *
 */
struct TRACK_MARKER_STRU {
    OBJECT_TRACK_STRU             stObjTrack;        //运动目标跟踪信息
    vector<TRAJECTORY_POINT_STRU> vstPredTrajectory; //预测轨迹点有序集
};

/**************************************************************************************
功能描述: 融合后的动态障碍发布器，发布移动目标位置、速度、历史轨迹、预测轨迹等信息
修改记录: 融合动态障碍发布器
**************************************************************************************/
class FUSED_DYNAMIC_OBST_SENDER_C
{
public:
    FUSED_DYNAMIC_OBST_SENDER_C(ros::NodeHandle &nh)
    {
        pubMovTargets_ = nh.advertise<comm_msg::movTargetsArray>(TOPIC_NAME_FUSED_MOV_TARGETS, 1);
        pubMarker_     = nh.advertise<visualization_msgs::MarkerArray>(TOPIC_NAME_MARKER_TARGETS_TRACK_PRED, 20);
    }

    void SendMsg(const comm_msg::movTargetsArray &msgMovTargetsArray);
    void SendMarker(const std_msgs::Header& header, const vector<TRACK_MARKER_STRU>& vstTrackMarker);

private:
    ros::Publisher  pubMovTargets_;
    ros::Publisher  pubMarker_;

    uint32_t        trackingLineInitId_{0};
    uint32_t        predTrajectoryLineInitId_{10000};
    uint32_t        predTrajectoryPointInitId_{20000};
    uint32_t        predVelArrowInitId_{30000};
    uint32_t        predVelTextInitId_{40000};
    uint32_t        bboxInitId_{40000};

    list<uint32_t> lstIdPre_;
};

/**
 * @brief 动态检测器
 *
 */
class DET_DYNAMIC_C
{
public:
    DET_DYNAMIC_C(ros::NodeHandle &nh)
    : nh_(nh)
    , objLocInfoRec_(nh, TOPIC_NAME_3D_LOCATION_INFO_SAIJIE, 0.2)
    , objFusedDynaObstSender_(nh)
    , objPubBoxMarker_(nh, TOPIC_NAME_MARKER_TARGETS_3D_BOX)
    , pobjPcptHmi_(PCPT_HMI_C::GetInstance())
    {}

    ~DET_DYNAMIC_C(void)
    {
        isClose_ = true;
        if (thread_.joinable()) {
            thread_.join();
            ST_LOG_INFO("线程退出.");
        }

        DelPtr(pobjPcdNetRec_);
    }

    INT32 Init(void);
    void  PeriodUpt(void);

private:
    void  PeriodProcess(void);
    INT32 GetDectObj(LIDAR_CLUSTER_STRU &stDectObj);
    INT32 GetObjTrajectoryTrack(LIDAR_CLUSTER_STRU &stDectObj,
                                std::vector<OBJECT_TRACK_STRU>& vstObjTrack);
    void  PubDynamicObst(const double stamp,
                         const std::vector<OBJECT_TRACK_STRU>& vstObjTrack,
                         std::vector<OBJECT_TRACK_STRU>& vstObjTrackValid);
    void  UpLoadPerceptionObjInfo(const double stamp,
                                  const std::vector<OBJECT_TRACK_STRU>& vstObjTrack);
    void  UptPcdNetMsg(void);
    INT32 GetPcdNetMsg(std::vector<BOX_INFO_STRU>& vstBoxesInfo);
    void  UptCarPose(void);
    INT32 GetCarPose(COOR_2D_POSE_STRU &stCarPose);
    void  TransLidarCluster2ByMap(LIDAR_CLUSTER_STRU &stLidarCluster);
    void  FilterBoxByLabel(std::vector<BOX_INFO_STRU>& vstBoxesInfo);

private:
    ros::NodeHandle&                nh_;
    LOCATION_3D_MSG_RECEIVER_C      objLocInfoRec_;
    PCD_NET_MSG_RECEIVER_C*         pobjPcdNetRec_{nullptr}; // 点云网络检测数据接收器
    FUSED_DYNAMIC_OBST_SENDER_C     objFusedDynaObstSender_; // 融合动态障碍发布器
    PUB_BOX_MARKER_C                objPubBoxMarker_;        // 体素Marker发布器
    PCPT_HMI_C*                     pobjPcptHmi_{nullptr};

    PCPT_TF_C                       objCarToMapTf_;          // TF变换器: car-->map
    TRAJECTORY_TRACKER_C            objTrajTracker_;
    PCPT_TRACK                      objPcptTrack_{3.0};             // 目标跟踪器
    PCPT_ABNORMAL_C                 objAb_;

    std::atomic_bool                isSuptDetDynamic_{false};       // 是否支持动态目标检测
    std::atomic_bool                isClose_{false};

    std::thread                     thread_;
    std::mutex                      objObjBoxInfoMutex_;
    std::mutex                      objLocMutex_;
    TIMESTAMP_POSE_STRU*            pstCarPose_{NULL};
    const vector<BOX_INFO_STRU>*    pvstBoxesInfo_{NULL};           // 点云网络检测数据
    std::vector<OBJECT_TRACK_STRU>  vstObjTrack_;                   // 目标跟踪
};


#endif

