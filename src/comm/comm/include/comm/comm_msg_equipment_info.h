#ifndef __COMM_MSG_EQUIPMENT_INFO_H__
#define __COMM_MSG_EQUIPMENT_INFO_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/equipment_info.h>
#include <ros/ros.h>
#include <iostream>
#include <list>
#include <vector>

typedef enum {
    EQUIPMENT_TYPE_DUSTBIN_REGION   = 0,  // 垃圾箱_区域
    EQUIPMENT_TYPE_DUSTBIN_CAPACITY = 1,  // 垃圾箱_容量状态
    EQUIPMENT_TYPE_CHARGING_PILE    = 2,  // 充电桩
    EQUIPMENT_TYPE_FIXED_SITE       = 3,  // 固定点
} EQUIPMENT_TYPE_ENUM;

struct EQUIPMENT_INFO_STRU {
    BOOL                             isAligning{false};
    EQUIPMENT_TYPE_ENUM              equipmentType{EQUIPMENT_TYPE_CHARGING_PILE};

    UINT32                           QRCodeId{0};
    COOR_3D_DIR_STRU                 QRCodePose;
    std::vector<POINT_COORD_3D_STRU> reflectiveStripPosLeft;
    std::vector<POINT_COORD_3D_STRU> reflectiveStripPosRight;
    POINT_COORD_3D_STRU              QRCodeOffset;// x，y，z


};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class EQUIPMENT_INFO_SENDER_C
{
public:
    EQUIPMENT_INFO_SENDER_C(ros::NodeHandle &nh)
    {
        publisher_ = nh.advertise<comm_msg::equipment_info>(TOPIC_NAME_EQUIPMENT_INFO, 1);

        InitMsg();
    }

    void Send(const EQUIPMENT_INFO_STRU &stEquipmentInfo)
    {
        stMsg_.status                   = stEquipmentInfo.isAligning;
        stMsg_.equipmentType            = (UINT08)stEquipmentInfo.equipmentType;
        stMsg_.QRCodeId                 = stEquipmentInfo.QRCodeId;
        stMsg_.QRCodePose.px            = stEquipmentInfo.QRCodePose.stPos.x;
        stMsg_.QRCodePose.py            = stEquipmentInfo.QRCodePose.stPos.y;
        stMsg_.QRCodePose.pz            = stEquipmentInfo.QRCodePose.stPos.z;
        stMsg_.QRCodePose.yaw           = stEquipmentInfo.QRCodePose.dirAngle;
        stMsg_.QRCodePose.roll          = 0.00;
        stMsg_.QRCodePose.pitch         = 0.00;

        stMsg_.reflectiveStripPosLeft.clear();
        for(const POINT_COORD_3D_STRU &point : stEquipmentInfo.reflectiveStripPosLeft) {
            geometry_msgs::Point32 pointG;
            pointG.x = point.x;
            pointG.y = point.y;
            pointG.z = point.z;
            stMsg_.reflectiveStripPosLeft.push_back(pointG);
        }

        stMsg_.reflectiveStripPosRight.clear();
        for(const POINT_COORD_3D_STRU &point : stEquipmentInfo.reflectiveStripPosRight) {
            geometry_msgs::Point32 pointG;
            pointG.x = point.x;
            pointG.y = point.y;
            pointG.z = point.z;
            stMsg_.reflectiveStripPosRight.push_back(pointG);
        }
        stMsg_.QRCodeOffset.pos_x = stEquipmentInfo.QRCodeOffset.x;
        stMsg_.QRCodeOffset.pos_y = stEquipmentInfo.QRCodeOffset.y;
        stMsg_.QRCodeOffset.pos_z = stEquipmentInfo.QRCodeOffset.z;




        publisher_.publish(stMsg_);
    }

private:
    ros::Publisher           publisher_;
    comm_msg::equipment_info stMsg_;

    void InitMsg(void)
    {
        stMsg_.status          = false;
        stMsg_.equipmentType   = (UINT08)EQUIPMENT_TYPE_CHARGING_PILE;
        stMsg_.QRCodeId        = UINT32_NULL;
        stMsg_.QRCodePose.px    = 0.00;
        stMsg_.QRCodePose.py    = 0.00;
        stMsg_.QRCodePose.pz    = 0.00;
        stMsg_.QRCodePose.yaw   = 0.00;
        stMsg_.QRCodePose.roll  = 0.00;
        stMsg_.QRCodePose.pitch = 0.00;
        stMsg_.QRCodeOffset.pos_x = 0.00;
        stMsg_.QRCodeOffset.pos_y = 0.00;
        stMsg_.QRCodeOffset.pos_z = 0.00;




        stMsg_.reflectiveStripPosLeft.clear();
        stMsg_.reflectiveStripPosRight.clear();
    }
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class EQUIPMENT_INFO_RECEIVER_C
{
using RECV_EQUIPMENT_INFO_FUNC = std::function<void(const EQUIPMENT_INFO_STRU &)>;

public:
    EQUIPMENT_INFO_RECEIVER_C(ros::NodeHandle &nh, double overTime = 0.5, const RECV_EQUIPMENT_INFO_FUNC &objRecvFunc = nullptr) : objRecvFunc_(objRecvFunc)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_EQUIPMENT_INFO, 1, &EQUIPMENT_INFO_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    const EQUIPMENT_INFO_STRU *Get(void) const
    {
        if (objInvalidTimer_.IsTimeOut()) {
            return nullptr;
        }

        return &stEquipmentInfo_;
    }

private:
    const RECV_EQUIPMENT_INFO_FUNC objRecvFunc_;
    ros::Subscriber                receiver_;

    TIMER_C                        objInvalidTimer_;       /* 数据失效定时器，超时后数据无效 */
    EQUIPMENT_INFO_STRU            stEquipmentInfo_;

    void ReceiveMsgCallBack(const comm_msg::equipment_info::ConstPtr &pstMsg)
    {
        objInvalidTimer_.Reset();

        stEquipmentInfo_.isAligning               = (BOOL)pstMsg->status;
        stEquipmentInfo_.equipmentType            = (EQUIPMENT_TYPE_ENUM)pstMsg->equipmentType;
        stEquipmentInfo_.QRCodeId                 = pstMsg->QRCodeId;
        stEquipmentInfo_.QRCodePose.stPos.x       = pstMsg->QRCodePose.px;
        stEquipmentInfo_.QRCodePose.stPos.y       = pstMsg->QRCodePose.py;
        stEquipmentInfo_.QRCodePose.stPos.z       = pstMsg->QRCodePose.pz;
        stEquipmentInfo_.QRCodePose.dirAngle      = pstMsg->QRCodePose.yaw;

        stEquipmentInfo_.reflectiveStripPosLeft.clear();
        for(const geometry_msgs::Point32 &point : pstMsg->reflectiveStripPosLeft) {
            POINT_COORD_3D_STRU stPoint;
            stPoint.x = point.x;
            stPoint.y = point.y;
            stPoint.z = point.z;
            stEquipmentInfo_.reflectiveStripPosLeft.push_back(stPoint);
        }

        stEquipmentInfo_.reflectiveStripPosRight.clear();
        for(const geometry_msgs::Point32 &point : pstMsg->reflectiveStripPosRight) {
            POINT_COORD_3D_STRU stPoint;
            stPoint.x = point.x;
            stPoint.y = point.y;
            stPoint.z = point.z;
            stEquipmentInfo_.reflectiveStripPosRight.push_back(stPoint);
        }
        stEquipmentInfo_.QRCodeOffset.x = pstMsg->QRCodeOffset.pos_x;
        stEquipmentInfo_.QRCodeOffset.y = pstMsg->QRCodeOffset.pos_y;
        stEquipmentInfo_.QRCodeOffset.z = pstMsg->QRCodeOffset.pos_z;


        if (objRecvFunc_) {
            objRecvFunc_(stEquipmentInfo_);
        }
    }
};

#endif

