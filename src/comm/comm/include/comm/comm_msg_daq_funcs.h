#ifndef __COMM_MSG_VISION_DAQ_FUNCS_H__
#define __COMM_MSG_VISION_DAQ_FUNCS_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/daq_ctrl.h"
#include <ros/ros.h>
#include <functional>
#include <string>
#include <vector>
#include "comm/comm_label.h"
#include <boost/optional.hpp>

constexpr uint32_t RATE_CAMERA_FUNC_ID = 999; // 使用常量替代宏定义

struct VISION_DAQ_FUNC_CTRL_STRU
{
    bool                   isUseIpu;            // 改用小写 bool 类型
    UINT32                 funcId;              // 功能ID
    UINT32                 frameRate;           // 帧率
    std::vector<std::string> vectCameraName;      // 相机名称列表
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
struct RECV_DAQ_STRU
{
    uint8_t hz;
    std::vector<uint8_t> cameraids;
};

class VISION_DAQ_DISPATCH_MSG_SENDER_C
{
public:
    VISION_DAQ_DISPATCH_MSG_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::daq_ctrl>(TOPIC_NAME_DAQ_CTRL, 1);
    }

    void Send(const RECV_DAQ_STRU &msg)
    {
        comm_msg::daq_ctrl stMsg;
        stMsg.daq_hz= msg.hz;
        stMsg.daq_camera = msg.cameraids;
 
        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};


using RECV_VISION_DAQ_DISPATCH_FUNC = std::function<void(const std::vector<VISION_DAQ_FUNC_CTRL_STRU> &)>;

class VISION_DAQ_DISPATCH_MSG_RECEIVER_C
{
public:
    VISION_DAQ_DISPATCH_MSG_RECEIVER_C(ros::NodeHandle &nh, const RECV_VISION_DAQ_DISPATCH_FUNC &objRecvFunc)
        : objRecvFunc_(objRecvFunc)
    {
        // 订阅 DAQ_RATE_CAMERA 话题
        rateCameraSubscriber_ = nh.subscribe(TOPIC_NAME_DAQ_CTRL, 1, &VISION_DAQ_DISPATCH_MSG_RECEIVER_C::ReceiveRateCameraMsg, this);
    }

private:
    const RECV_VISION_DAQ_DISPATCH_FUNC objRecvFunc_;
    ros::Subscriber rateCameraSubscriber_; // 订阅器
    boost::optional<uint8_t> rateCameraHz_;   // 使用 std::optional 存储频率值
    std::vector<uint8_t>     cameraIdRec_;

    // 处理 DAQ_RATE_CAMERA 消息
    void ReceiveRateCameraMsg(const comm_msg::daq_ctrl::ConstPtr &rateMsg)
    {
        if (!rateMsg) {
            ROS_WARN("Received null DAQ_RATE_CAMERA message.");
            return;
        }

        // 更新 DAQ_RATE_CAMERA 的频率
        rateCameraHz_ = rateMsg->daq_hz;
        cameraIdRec_ = rateMsg->daq_camera;
        if (!rateCameraHz_ || rateCameraHz_.get() < 1) {
            ROS_WARN("DAQ rate camera received invalid or zero frequency. : Hz = %u", rateCameraHz_.get());
        }

        // 构建功能控制向量
        std::vector<VISION_DAQ_FUNC_CTRL_STRU> vectDaqFuncCtrl;
        if (rateCameraHz_) {
            VISION_DAQ_FUNC_CTRL_STRU stFuncCtrl;
            stFuncCtrl.funcId = RATE_CAMERA_FUNC_ID;       // 使用常量定义的功能ID
            stFuncCtrl.frameRate = rateCameraHz_.get();
            stFuncCtrl.isUseIpu = false;

            for (size_t i = 0; i < cameraIdRec_.size(); ++i) {
                if (cameraIdRec_[i] < RgbCamera.size()) {
                    if (cameraIdRec_[i]==0){
                       ROS_WARN("Camera ID %u not exist!", cameraIdRec_[i]);
                    }
                    stFuncCtrl.vectCameraName.push_back(RgbCamera[cameraIdRec_[i]]);
                }
                else{
                    ROS_WARN("Camera ID %u out of range!", cameraIdRec_[i]);
                }
            }

            vectDaqFuncCtrl.push_back(stFuncCtrl);

            ROS_INFO("Added DAQ_RATE_CAMERA stFuncCtrl with funcId = %u, frameRate = %u",
                     stFuncCtrl.funcId, stFuncCtrl.frameRate);
        }

        // 回调函数
        objRecvFunc_(vectDaqFuncCtrl);
    }
};

#endif

