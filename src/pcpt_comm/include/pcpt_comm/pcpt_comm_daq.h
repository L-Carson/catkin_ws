#pragma once

#include <ros/ros.h>
#include "comm_msg/daq_ctrl.h"
#include "comm/comm_dbg_log.h"
#include "comm/comm_topic_name.h"


/**************************************************************************************
功能描述: 数据采集指令发布器
修改记录:
**************************************************************************************/
class DAQ_CTRL_SENDER_C {
public:
    DAQ_CTRL_SENDER_C(ros::NodeHandle& nh)
    {
        publisher_ = nh.advertise<comm_msg::daq_ctrl>(TOPIC_NAME_DAQ_CTRL, 1);
    }

    ~DAQ_CTRL_SENDER_C(){}

    void Send(const bool daqFusion, const vector<uint8_t> daqCamera, const vector<uint8_t> daqLidar) {
        comm_msg::daq_ctrl msg;
        msg.daq_fusion = daqFusion;
        msg.daq_camera = daqCamera;
        msg.daq_lidar  = daqLidar;
        Send(msg);
    }

    void SendCameraDaqMsg(const bool daqFusion, vector<uint8_t> daqCamera) {Send(daqFusion, daqCamera, {});}
    void SendLidarDaqMsg( const bool daqFusion, vector<uint8_t> daqLidar)  {Send(daqFusion, {}, daqLidar);}
    void SendFusionDaqMsg(const bool daqFusion, vector<uint8_t> daqCamear, vector<uint8_t> daqLidar) {Send(daqFusion, daqCamear, daqLidar);}

private:
    ros::Publisher publisher_;

    void Send(comm_msg::daq_ctrl &msg) {
        if (publisher_.getNumSubscribers()) publisher_.publish(msg);
    }
};

/**************************************************************************************
功能描述: 数据采集指令接收器
修改记录:
**************************************************************************************/
class DAQ_CTRL_RECEIVER_C {
public:
    DAQ_CTRL_RECEIVER_C(ros::NodeHandle& nh)
    {
        receiver_ = nh.subscribe(TOPIC_NAME_DAQ_CTRL, 100, &DAQ_CTRL_RECEIVER_C::MsgReceiveCallback, this);
    }

    ~DAQ_CTRL_RECEIVER_C(){}

    void Get(comm_msg::daq_ctrl &msg) {
        msg = daqCtrlMsg_;
    }

private:
    ros::Subscriber     receiver_;
    comm_msg::daq_ctrl  daqCtrlMsg_;  // 数据采集控制指令缓存

    void MsgReceiveCallback(const comm_msg::daq_ctrl &msg) {
        daqCtrlMsg_ = msg;
    }
};

/**************************************************************************************
功能描述: 车辆急停状态消息接收器 (Emergency Stop)
修改记录:
**************************************************************************************/
class ES_MSG_RECEIVER_C {
public:
    ES_MSG_RECEIVER_C(ros::NodeHandle& nh) {
        //receiver_ = nh.subscribe(TOPIC_NAME_DAQ_CTRL, 100, &DAQ_CTRL_RECEIVER_C::MsgReceiveCallback, this);
    }

    ~ES_MSG_RECEIVER_C() {}

    bool IsES() {
        return isES_;
    }

private:
    void MsgReceiveCallback(/*const comm_msg::daq_ctrl &msg */) {
        //isES_ = msg;
    }

    ros::Subscriber receiver_;
    bool            isES_{false};
};
