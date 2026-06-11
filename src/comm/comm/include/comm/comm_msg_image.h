
#ifndef __COMM_MSG_IMAGE_H__
#define __COMM_MSG_IMAGE_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <functional>
#include <string>

/**************************************************************************************
功能描述: 图像触发源解析器
修改记录:
**************************************************************************************/
class IMAGE_FUNCID_PARSER_C
{
public:
    std::string Encode(const std::vector<UINT32> &vectFuncId) const
    {
        std::string strFuncIds;

        BOOL isFirst = true;
        for (const UINT32 &funcId : vectFuncId) {
            if (isFirst) {
                isFirst = false;
            } else {
                strFuncIds.append(1, splitChar_);
            }

            strFuncIds += ToString(funcId);
        }

        return strFuncIds;
    }

    INT32 Decode(const std::string &strFuncIds, std::vector<UINT32> &vectFuncId) const
    {
        vectFuncId.clear();

        size_t funcIdStartIdx = 0;
        while (funcIdStartIdx < strFuncIds.length()) {
            std::string strFuncId;

            size_t funcIdEndIdx = strFuncIds.find(splitChar_, funcIdStartIdx);
            if (funcIdEndIdx <= funcIdStartIdx) {
                ST_LOG_ERR("strFuncIds(%s) Is Err.", strFuncIds.c_str());
                return -1;

            } else if (funcIdEndIdx == std::string::npos) {
                strFuncId = strFuncIds.substr(funcIdStartIdx);
                funcIdStartIdx = funcIdEndIdx;

            } else {
                strFuncId = strFuncIds.substr(funcIdStartIdx, funcIdEndIdx - funcIdStartIdx);
                funcIdStartIdx = funcIdEndIdx + 1;
            }

            UINT32 funcId = 0;
            for (CHAR c : strFuncId) {
                if ((c < '0') || (c > '9')) {
                    ST_LOG_ERR("Exsit Non Digtal Char. %s.", strFuncIds.c_str());
                    return -1;
                }

                UINT32 digtal = c - '0';
                funcId = funcId * 10 + digtal;
            }

            vectFuncId.emplace_back(funcId);
        }

        return 0;
    }

private:
    const CHAR splitChar_   = ' ';
};

/**************************************************************************************
功能描述: 发布消息
修改记录:
**************************************************************************************/
class IMAGE_TRANSPORT_SENDER_C
{
public:
    IMAGE_TRANSPORT_SENDER_C(ros::NodeHandle &nh, const std::string &strTopicName) : objImageTransport_(nh)
    {
        objPublisher_ = objImageTransport_.advertise(strTopicName, 1);
    }

    BOOL IsExistSubscriber(void)
    {
        return (objPublisher_.getNumSubscribers() > 0);
    }

    /* 不同地方的描述信息的含义不一样，描述信息通过消息头中的frameID携带。 */
    void Send(const ros::Time &objImageTime, const cv::Mat &objImage, const std::string &strImageDescription = "")
    {
        if (!IsExistSubscriber()) {
            return;
        }

        std_msgs::Header objMsgHead;
        objMsgHead.stamp    = objImageTime;
        objMsgHead.frame_id = strImageDescription;

        sensor_msgs::ImagePtr ptrImageMsg;
        try {
            ptrImageMsg = cv_bridge::CvImage(objMsgHead, "bgr8", objImage).toImageMsg();
        } catch (cv_bridge::Exception &e) {
            ST_LOG_FATAL("cv_bridge::CvImage() Exception: %s.", e.what());
            return;
        }

        objPublisher_.publish(ptrImageMsg);
    }

private:
    image_transport::ImageTransport objImageTransport_;
    image_transport::Publisher      objPublisher_;
};



/**************************************************************************************
功能描述: 接收消息
修改记录:
**************************************************************************************/
class IMAGE_TRANSPORT_RECEIVER_C
{
using RECV_IMAGE_FUNC_1 = std::function<void(const ros::Time &, const cv::Mat &)>;
using RECV_IMAGE_FUNC_2 = std::function<void(const ros::Time &, const cv::Mat &, const std::string &)>;

public:
    IMAGE_TRANSPORT_RECEIVER_C(ros::NodeHandle &nh, const std::string &strTopicName, const RECV_IMAGE_FUNC_1 &objRecvFunc1, const RECV_IMAGE_FUNC_2 &objRecvFunc2)
                                                                : objRecvFunc1_(objRecvFunc1),
                                                                  objRecvFunc2_(objRecvFunc2),
                                                                  objImageTransport_(nh)
    {
        objSubscriber_ = objImageTransport_.subscribe(strTopicName, 1,  std::bind(&IMAGE_TRANSPORT_RECEIVER_C::ReceiveMsg, this, std::placeholders::_1));
    }


    IMAGE_TRANSPORT_RECEIVER_C(ros::NodeHandle &nh, const std::string &strTopicName)
                                                                : IMAGE_TRANSPORT_RECEIVER_C(nh, strTopicName, nullptr, nullptr)
    {
    }

    IMAGE_TRANSPORT_RECEIVER_C(ros::NodeHandle &nh, const std::string &strTopicName, const RECV_IMAGE_FUNC_1 &objRecvFunc)
                                                                : IMAGE_TRANSPORT_RECEIVER_C(nh, strTopicName, objRecvFunc, nullptr)
    {
    }

    IMAGE_TRANSPORT_RECEIVER_C(ros::NodeHandle &nh, const std::string &strTopicName, const RECV_IMAGE_FUNC_2 &objRecvFunc)
                                                                : IMAGE_TRANSPORT_RECEIVER_C(nh, strTopicName, nullptr, objRecvFunc)
    {
    }

    BOOL Get(ros::Time &objImageTime, cv::Mat &objImage, std::string &strDescription)
    {
        if (objImage_.empty()) {
            return false;
        }

        objImageTime   = objImageTime_;
        objImage       = objImage_;
        strDescription = strDescription_;
        objImage_.release();
        return true;
    }

private:
    const RECV_IMAGE_FUNC_1         objRecvFunc1_;
    const RECV_IMAGE_FUNC_2         objRecvFunc2_;
    image_transport::ImageTransport objImageTransport_;
    image_transport::Subscriber     objSubscriber_;

    ros::Time                       objImageTime_;
    cv::Mat                         objImage_;
    std::string                     strDescription_;

    void ReceiveMsg(const sensor_msgs::ImageConstPtr &ptrImageMsg)
    {
        R_ASSERT(ptrImageMsg != NULL, NOTHING);

        cv_bridge::CvImagePtr objBridgeImage;

        try {
            objBridgeImage = cv_bridge::toCvCopy(ptrImageMsg, "bgr8");
        } catch (cv_bridge::Exception &e) {
            ST_LOG_FATAL("cv_bridge::toCvCopy() Exception: %s.", e.what());
            return;
        }

        /* 只支持一种结果通知方式 */
        if (objRecvFunc1_) {
            objRecvFunc1_(ptrImageMsg->header.stamp, objBridgeImage->image);
            objImage_.release();
            return;
        }

        if (objRecvFunc2_) {
            objRecvFunc2_(ptrImageMsg->header.stamp, objBridgeImage->image, ptrImageMsg->header.frame_id);
            objImage_.release();
            return;
        }

        objImageTime_   = ptrImageMsg->header.stamp;
        objImage_       = objBridgeImage->image;
        strDescription_ = ptrImageMsg->header.frame_id;
        objBridgeImage->image.release();
    }
};


#endif

