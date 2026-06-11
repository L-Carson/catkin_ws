#ifndef __COMM_MSG_MOVE_TARGET_LIST_H_
#define __COMM_MSG_MOVE_TARGET_LIST_H_

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include <comm_msg/movTargetsArray.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

enum MOVE_TARGET_TYPE {
    MOVE_TARGET_UNKNOWN    = 0,
    MOVE_TARGET_CAR        = 1,
    MOVE_TARGET_MOTOR      = 2,
    MOVE_TARGET_PEDESTRIAN = 3,
};

struct MOVE_TARGET_STRU {
    double                     timeStamp;
    float                      interval;
    COOR_2D_POSE_STRU          stCenterPose;
    vector<COORDINATE_2D_STRU> box;
};

struct MOVE_TARGET_LIST_STRU {
    UINT32                   id;
    MOVE_TARGET_TYPE         enType;
    float                    length;
    float                    width;
    vector<MOVE_TARGET_STRU> vectMoveTarget;
};

/**************************************************************************************
功能描述: 融合动态障碍物消息接收器
修改记录:
**************************************************************************************/
class MOVE_TARGET_LIST_RECEIVER_C
{
public:
    MOVE_TARGET_LIST_RECEIVER_C(ros::NodeHandle &nh)
    {
        subscriber_ = nh.subscribe(TOPIC_NAME_FUSED_MOV_TARGETS, 1, &MOVE_TARGET_LIST_RECEIVER_C::ReceiveMsgCallBack, this);

        objInvalidTimer_.SetOverTime(0.25);
    }

    ~MOVE_TARGET_LIST_RECEIVER_C()
    {
        subscriber_.shutdown();
    }

    INT32 Get(vector<MOVE_TARGET_LIST_STRU> &vectDyncObsts) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return -1;
        }

        vectDyncObsts = vectDyncObsts_;
        return 0;
    }

private:

    ros::Subscriber               subscriber_;
    TIMER_C                       objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    vector<MOVE_TARGET_LIST_STRU> vectDyncObsts_;
    ros::Time lastT_;

    void ReceiveMsgCallBack(const comm_msg::movTargetsArray::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != NULL, NOTHING);
        ros::Time nowT = ros::Time::now();

        ros::Duration d(nowT - lastT_);
        lastT_ = nowT;
        float intervalMs = d.toSec() * 1000;
        ST_LOG_INFO_IF(intervalMs > 150.0, "recv obs interval in ms: %f", intervalMs);

        objInvalidTimer_.Reset();

        vectDyncObsts_.clear();
        vectDyncObsts_.reserve(pstMsg->movTargetsArray.size());

        double startTime = pstMsg->header.stamp.toSec();
        for (UINT32 moveTargetIdx = 0; moveTargetIdx < pstMsg->movTargetsArray.size(); ++moveTargetIdx) {
            const comm_msg::movTarget &movTarget = pstMsg->movTargetsArray[moveTargetIdx];
            if (movTarget.movPredArray.empty()) {
                continue;
            }

            MOVE_TARGET_LIST_STRU stMoveTargetList;
            stMoveTargetList.id     = movTarget.id;
            stMoveTargetList.enType = (MOVE_TARGET_TYPE)movTarget.movPredArray[0].box.label.value;
            // 感知发出的障碍物，width是与障碍航向一致的边
            stMoveTargetList.length = movTarget.movPredArray[0].box.width;
            stMoveTargetList.width  = movTarget.movPredArray[0].box.length;

            for (UINT32 movePredictIdx = 0; movePredictIdx < movTarget.movPredArray.size(); ++movePredictIdx) {
                const comm_msg::movPred &movPred   = movTarget.movPredArray[movePredictIdx];
                MOVE_TARGET_STRU stMoveTarget;
                stMoveTarget.timeStamp             = movPred.timeStamp;
                stMoveTarget.interval              = movPred.timeStamp - startTime;
                stMoveTarget.stCenterPose.stPos.x  = movPred.box.x;
                stMoveTarget.stCenterPose.stPos.y  = movPred.box.y;
                stMoveTarget.stCenterPose.dirAngle = movPred.box.yaw;
                GetTransformedBox(stMoveTarget.stCenterPose, movPred.box.length, movPred.box.width, stMoveTarget.box);

                stMoveTargetList.vectMoveTarget.push_back(stMoveTarget);
            }

            vectDyncObsts_.push_back(stMoveTargetList);
        }
    }

    void GetTransformedBox(const COOR_2D_POSE_STRU &stPose, float length, float width, vector<COORDINATE_2D_STRU> &vectBoxPoint) const {
        vectBoxPoint.clear();
        // 感知发出的障碍物，width是与障碍航向一致的边
        COORDINATE_2D_STRU carLeftUpPoint   { width / 2,  length / 2};
        COORDINATE_2D_STRU carRightUpPoint  { width / 2, -length / 2};
        COORDINATE_2D_STRU carRightDownPoint{-width / 2, -length / 2};
        COORDINATE_2D_STRU carLeftDownPoint {-width / 2,  length / 2};

        COORDINATE_2D_STRU mapLeftUpPoint;
        mapLeftUpPoint.x = carLeftUpPoint.x * cos(stPose.dirAngle) - carLeftUpPoint.y * sin(stPose.dirAngle) + stPose.stPos.x;
        mapLeftUpPoint.y = carLeftUpPoint.x * sin(stPose.dirAngle) + carLeftUpPoint.y * cos(stPose.dirAngle) + stPose.stPos.y;
        vectBoxPoint.push_back(mapLeftUpPoint);

        COORDINATE_2D_STRU mapRightUpPoint;
        mapRightUpPoint.x = carRightUpPoint.x * cos(stPose.dirAngle) - carRightUpPoint.y * sin(stPose.dirAngle) + stPose.stPos.x;
        mapRightUpPoint.y = carRightUpPoint.x * sin(stPose.dirAngle) + carRightUpPoint.y * cos(stPose.dirAngle) + stPose.stPos.y;
        vectBoxPoint.push_back(mapRightUpPoint);

        COORDINATE_2D_STRU mapRightDownPoint;
        mapRightDownPoint.x = carRightDownPoint.x * cos(stPose.dirAngle) - carRightDownPoint.y * sin(stPose.dirAngle) + stPose.stPos.x;
        mapRightDownPoint.y = carRightDownPoint.x * sin(stPose.dirAngle) + carRightDownPoint.y * cos(stPose.dirAngle) + stPose.stPos.y;
        vectBoxPoint.push_back(mapRightDownPoint);

        COORDINATE_2D_STRU mapLeftDownPoint;
        mapLeftDownPoint.x = carLeftDownPoint.x * cos(stPose.dirAngle) - carLeftDownPoint.y * sin(stPose.dirAngle) + stPose.stPos.x;
        mapLeftDownPoint.y = carLeftDownPoint.x * sin(stPose.dirAngle) + carLeftDownPoint.y * cos(stPose.dirAngle) + stPose.stPos.y;
        vectBoxPoint.push_back(mapLeftDownPoint);
    }
};

#endif

