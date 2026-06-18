#ifndef __COMM_MSG_VISION_FUNCS_H__
#define __COMM_MSG_VISION_FUNCS_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include "comm_msg/visionFuncs.h"
#include <ros/ros.h>
#include <functional>
#include <string>
#include <vector>

#include <std_msgs/String.h>
#include <json/json.h>

struct VISION_FUNC_CTRL_STRU
{
    BOOL                     isUseIpu;
    UINT32                   funcId;
    UINT32                   frameRate;
    std::vector<std::string> vectCameraName;
};

/**************************************************************************************
功能描述: 发布消息
**************************************************************************************/
class VISION_DISPATCH_MSG_SENDER_C
{
public:
    VISION_DISPATCH_MSG_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<std_msgs::String>(TOPIC_NAME_VISION_FUNC, 1);
    }

    void Send(const std::vector<VISION_FUNC_CTRL_STRU> &vectFuncCtrl)
    {
        Json::Value root;
        Json::Value funcs(Json::arrayValue);

        for (const auto& st : vectFuncCtrl) {
            Json::Value f;
            f["funcId"] = (Json::UInt)st.funcId;
            f["hz"]     = (Json::UInt)st.frameRate;
            f["isUseIpu"]    = st.isUseIpu;

            Json::Value cams(Json::arrayValue);
            for (const auto& cam : st.vectCameraName) {
                cams.append(cam);
            }
            f["cameras"] = cams;
            funcs.append(f);
        }

        root["visionFuncs"] = funcs;

        Json::StreamWriterBuilder wb;
        std::string json_str = Json::writeString(wb, root);

        std_msgs::String msg;
        msg.data = json_str;
        objPublisher_.publish(msg);
    }

private:
    ros::Publisher objPublisher_;
};


using RECV_VISION_DISPATCH_FUNC = std::function<void(const std::vector<VISION_FUNC_CTRL_STRU> &)>;

class VISION_DISPATCH_MSG_RECEIVER_C
{
public:
    VISION_DISPATCH_MSG_RECEIVER_C(ros::NodeHandle &nh, const RECV_VISION_DISPATCH_FUNC &objRecvFunc)
        : objRecvFunc_(objRecvFunc)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_VISION_FUNC, 1,
            &VISION_DISPATCH_MSG_RECEIVER_C::ReceiveMsg, this);
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

    UINT32 GetMsgFeature(const Json::Value &objJson)
    {
        UINT32 feature = 0;

        if (!objJson.isObject()) {
            return feature;
        }

        const Json::Value& visionFuncs = objJson.get("visionFuncs", Json::Value(Json::nullValue));
        if (!visionFuncs.isArray()) {
            return feature;
        }

        for (const auto &func : visionFuncs) {
            if (!func.isObject()) continue;

            UINT32 funcId = func.get("funcId", 0).asUInt();
            UINT32 hz     = func.get("hz", 0).asUInt();
            bool   ipu    = func.get("isUseIpu", false).asBool();

            feature += funcId;
            feature += hz * 123456;
            feature += (ipu ? 789 : 0);

            const Json::Value& cameras = func.get("cameras", Json::Value(Json::nullValue));
            if (cameras.isArray()) {
                for (const auto &cam : cameras) {
                    if (cam.isString()) {
                        feature += GetStringFeature(cam.asString()) * 321;
                    }
                }
            }
        }
        return feature;
    }

    int ParseJsonToStruct(const Json::Value &objJson, std::vector<VISION_FUNC_CTRL_STRU> &vectFuncCtrl)
    {
        vectFuncCtrl.clear();

        if (!objJson.isObject()) {
            ST_LOG_ERR("JSON is not object");
            return -1;
        }

        const Json::Value& visionFuncs = objJson.get("visionFuncs", Json::Value(Json::nullValue));
        if (!visionFuncs.isArray()) {
            ST_LOG_ERR("JSON no visionFuncs array");
            return -1;
        }

        for (const auto &func : visionFuncs) {
            if (!func.isObject()) continue;

            VISION_FUNC_CTRL_STRU stCtrl;
            stCtrl.funcId         = func.get("funcId", 0).asUInt();
            stCtrl.frameRate      = func.get("hz", 0).asUInt();
            stCtrl.isUseIpu       = func.get("isUseIpu", false).asBool();
            stCtrl.vectCameraName.clear();

            const Json::Value& cameras = func.get("cameras", Json::Value(Json::nullValue));
            if (cameras.isArray()) {
                for (const auto &cam : cameras) {
                    if (cam.isString()) {
                        stCtrl.vectCameraName.push_back(cam.asString());
                    }
                }
            }

            vectFuncCtrl.push_back(stCtrl);
        }
        return 0;
    }

    void ReceiveMsg(const std_msgs::String::ConstPtr &pMsg)
    {
        std::string strJson = pMsg->data;
        ST_LOG_INFO("%s", strJson.c_str());

        Json::Value             objJson;
        Json::String            strReaderErr;
        Json::CharReaderBuilder objReaderBuilder;
        Json::CharReader       *pReader = objReaderBuilder.newCharReader();

        bool isSucc = pReader->parse(
            strJson.c_str(),
            strJson.c_str() + strJson.size(),
            &objJson,
            &strReaderErr
        );
        delete pReader;

        if (!isSucc) {
            ST_LOG_ERR("JSON parse fail: %s", strReaderErr.c_str());
            return;
        }

        // ===================== 【安全】归一化 =====================
        Json::Value normalizedRoot;
        if (objJson.isObject()) {
            normalizedRoot = objJson;
        }
        else if (objJson.isArray()) {
            normalizedRoot = Json::objectValue;
            normalizedRoot["visionFuncs"] = objJson;
        }
        else {
            ST_LOG_WARN("JSON type not supported");
            return;
        }

        // ===================== 业务逻辑 =====================
        UINT32 newFeature = GetMsgFeature(normalizedRoot);
        if (newFeature == infoFeature_) {
            ST_LOG_INFO_SAMPLE(20*60, "Ctrl Info Not Changed.");
            return;
        }

        ST_LOG_INFO("Ctrl Info Changed, new=%u", newFeature);
        infoFeature_ = newFeature;

        std::vector<VISION_FUNC_CTRL_STRU> vectCtrl;
        if (ParseJsonToStruct(normalizedRoot, vectCtrl) == 0) {
            objRecvFunc_(vectCtrl);
        }
    }
};

#endif
