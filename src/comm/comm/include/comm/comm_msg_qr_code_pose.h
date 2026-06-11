#ifndef __COMM_MSG_QR_CODE_POSE_H__
#define __COMM_MSG_QR_CODE_POSE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/qrcode_pose.h"
#include <ros/ros.h>
#include <Eigen/Core>
#include <string>
#include <vector>

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class QR_CODE_POSE_SENDER_C
{
public:
    QR_CODE_POSE_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::qrcode_pose>(TOPIC_NAME_QRCODE_POSE, 1);
    }

    void Send(const ros::Time &objImageTime, UINT32 qrCodeId, const Eigen::Vector3d &objTranslVec, const Eigen::Quaterniond &objQuaterniond)
    {
        comm_msg::qrcode_pose stMsgQrCodePose;

        stMsgQrCodePose.header.stamp = objImageTime;

        stMsgQrCodePose.id = qrCodeId;

        stMsgQrCodePose.pose.position.x = objTranslVec(0);
        stMsgQrCodePose.pose.position.y = objTranslVec(1);
        stMsgQrCodePose.pose.position.z = objTranslVec(2);

        stMsgQrCodePose.pose.orientation.w = objQuaterniond.w();
        stMsgQrCodePose.pose.orientation.x = objQuaterniond.x();
        stMsgQrCodePose.pose.orientation.y = objQuaterniond.y();
        stMsgQrCodePose.pose.orientation.z = objQuaterniond.z();

        objPublisher_.publish(stMsgQrCodePose);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class QR_CODE_POSE_RECEIVER_C
{
public:
    QR_CODE_POSE_RECEIVER_C(ros::NodeHandle &nh, double overTime = 0.2)
    {
        objInvalidTimer_.SetOverTime(overTime);
        receiver_ = nh.subscribe(TOPIC_NAME_QRCODE_POSE, 1, &QR_CODE_POSE_RECEIVER_C::ReceiveMsgCallBack, this);
    };

    void ReceiveMsgCallBack(const comm_msg::qrcode_pose::ConstPtr& pstMsg)
    {
        objInvalidTimer_.Reset();
        stQrCodePose_.time = pstMsg->header.stamp.toSec();
        stQrCodePose_.id   = pstMsg->id;

        Quaternionf quat(pstMsg->pose.orientation.w, pstMsg->pose.orientation.x,
                         pstMsg->pose.orientation.y, pstMsg->pose.orientation.z);

        Vector3f pos = Vector3f(pstMsg->pose.position.x,
                                pstMsg->pose.position.y,
                                pstMsg->pose.position.z);
        Matrix4f mPose = Matrix4f::Identity();
        mPose.block<3,3>(0,0) = quat.toRotationMatrix();
        mPose.block<3,1>(0,3) = pos;

        stQrCodePose_.stPose.Copy(mPose);
    }

    int Get(POSE_ID_STRU &stQrCodePose)
    {
        if(objInvalidTimer_.IsTimeOut()) return -1;
        stQrCodePose = stQrCodePose_;
        return 0;
    }

private:
    TIMER_C            objInvalidTimer_;
    ros::Subscriber    receiver_;
    POSE_ID_STRU       stQrCodePose_;
};


#endif

