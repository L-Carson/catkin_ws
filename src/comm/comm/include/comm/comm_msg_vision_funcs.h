
#ifndef __COMM_MSG_VISION_FUNCS_H__
#define __COMM_MSG_VISION_FUNCS_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/visionFuncs.h"
#include <ros/ros.h>
#include <functional>
#include <string>
#include <vector>


struct VISION_FUNC_CTRL_STRU
{
    BOOL                     isUseIpu;
    UINT32                   funcId;
    UINT32                   frameRate;
    std::vector<std::string> vectCameraName;
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class VISION_DISPATCH_MSG_SENDER_C
{
public:
    VISION_DISPATCH_MSG_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::visionFuncs>(TOPIC_NAME_VISION_FUNC, 1);
    }

    void Send(const std::vector<VISION_FUNC_CTRL_STRU> &vectFuncCtrl)
    {
        comm_msg::visionFuncs stMsg;

        for (const VISION_FUNC_CTRL_STRU &stFuncCtrl : vectFuncCtrl) {
            comm_msg::visionFunc stFuncCtrlMsg;
            stFuncCtrlMsg.funcId  = stFuncCtrl.funcId;
            stFuncCtrlMsg.hz      = stFuncCtrl.frameRate;
            stFuncCtrlMsg.cameras = stFuncCtrl.vectCameraName;

            stMsg.funcs.push_back(stFuncCtrlMsg);
        }

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};


/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/

using RECV_VISION_DISPATCH_FUNC = std::function<void(const std::vector<VISION_FUNC_CTRL_STRU> &)>;

class VISION_DISPATCH_MSG_RECEIVER_C
{
public:
    VISION_DISPATCH_MSG_RECEIVER_C(ros::NodeHandle &nh, const RECV_VISION_DISPATCH_FUNC &objRecvFunc) : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_VISION_FUNC, 1, &VISION_DISPATCH_MSG_RECEIVER_C::ReceiveMsg, this);
    }

private:
    const RECV_VISION_DISPATCH_FUNC objRecvFunc_;
    ros::Subscriber                 objSubscriber_;
    UINT32                          infoFeature_{0};

    UINT32 GetStringFeature(const std::string &strAny)
    {
        UINT32 feature = 0;
        UINT32 weight  = 1;
        for (char c : strAny) {
            feature += c * weight;
            weight  += 50;
        }

        return feature;
    }

    UINT32 GetMsgFeatrue(const comm_msg::visionFuncs::ConstPtr &pobjMsg)
    {
        UINT32 feature = 0;

        /* 这里写的数字是乱写的，尽量让算出来的数字在内容不重复时不要一样即可 */
        for (const comm_msg::visionFunc &objFuncCtrlMsg : pobjMsg->funcs) {
            feature += objFuncCtrlMsg.funcId;
            feature += objFuncCtrlMsg.hz * 123456;

            for (const std::string &strCameraName : objFuncCtrlMsg.cameras) {
                feature += GetStringFeature(strCameraName) * 321;
            }
        }

        return feature;
    }

    void ReceiveMsg(const comm_msg::visionFuncs::ConstPtr &pobjMsg)
    {
        R_ASSERT(pobjMsg != NULL, NOTHING);

        UINT32 newFeature = GetMsgFeatrue(pobjMsg);
        ST_LOG_INFO_SAMPLE_IF_TRUE(newFeature == infoFeature_, 20 * 60, "Ctrl Info Not Changed.");
        if (newFeature == infoFeature_) {
            return;
        }

        ST_LOG_INFO("Ctrl Info Is Changed, Old Feature Is %u, New Feature Is %u.", infoFeature_, newFeature);
        infoFeature_ = newFeature;

        std::vector<VISION_FUNC_CTRL_STRU> vectFuncCtrl;
        for (const comm_msg::visionFunc &objFuncCtrlMsg : pobjMsg->funcs) {
            VISION_FUNC_CTRL_STRU stFuncCtrl;

            stFuncCtrl.funcId         = objFuncCtrlMsg.funcId;
            stFuncCtrl.frameRate      = objFuncCtrlMsg.hz;
            stFuncCtrl.isUseIpu       = objFuncCtrlMsg.ipu;
            stFuncCtrl.vectCameraName = objFuncCtrlMsg.cameras;

            vectFuncCtrl.push_back(stFuncCtrl);
        }

        objRecvFunc_(vectFuncCtrl);
    }
};


#endif

