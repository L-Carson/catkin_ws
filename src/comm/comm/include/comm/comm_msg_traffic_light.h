#ifndef __COMM_MSG_TARGET_LIGHT_H__
#define __COMM_MSG_TARGET_LIGHT_H__

#include "comm/comm_topic_name.h"
#include "comm/comm_base.h"
#include <comm_msg/traffic_light_target.h>
#include <comm_msg/traffic_light_result.h>
#include <ros/ros.h>

#include <mutex>

/**
 * @brief 交通灯检测结构体
 *
 */
struct TRAFFIC_LIGHT_INFO_STRU
{
    TRAFFIC_LIGHT_INFO_STRU() {}
    TRAFFIC_LIGHT_INFO_STRU(const uint8_t type, const float px, const float py, const float pz,
                            const float rowLength, const float colLength) {
        this->type      = type;
        this->px        = px;
        this->py        = py;
        this->pz        = pz;
        this->rowLength = rowLength;
        this->colLength = colLength;
    }

    bool IsEqual(const TRAFFIC_LIGHT_INFO_STRU& stInfo) const {
        if (this->type != stInfo.type)      return false;
        if (0 != SIG(this->px - stInfo.px)) return false;
        if (0 != SIG(this->py - stInfo.py)) return false;
        if (0 != SIG(this->pz - stInfo.pz)) return false;
        if (0 != SIG(this->rowLength - stInfo.rowLength)) return false;
        if (0 != SIG(this->colLength - stInfo.colLength)) return false;
        return true;
    }

    bool operator == (const TRAFFIC_LIGHT_INFO_STRU& stTrafficLight) const {
        return IsEqual(stTrafficLight);
    }

    bool operator != (const TRAFFIC_LIGHT_INFO_STRU& stTrafficLight) const {
        return !IsEqual(stTrafficLight);
    }

    uint8_t     type{2};        // 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
    float       px;             // 交通灯中在地图上的坐标x
    float       py;             // 交通灯中在地图上的坐标y
    float       pz;             // 交通灯中在地图上的坐标z
    float       rowLength;      // 交通灯横向长度
    float       colLength;      // 交通灯纵向长度
};

struct TRAFFIC_LIGHT_TARGET_STRU
{
    bool IsListEqual(const list<TRAFFIC_LIGHT_INFO_STRU> &lstTrafficLight) const {
        if (this->lstNear.size() != lstTrafficLight.size()) return false;
        for (const TRAFFIC_LIGHT_INFO_STRU& stInfoA : this->lstNear) {
            bool isEqual = false;
            for (const TRAFFIC_LIGHT_INFO_STRU& stInfoB : lstTrafficLight) {
                if (stInfoA == stInfoB) {
                    isEqual = true;
                    break;
                }
            }
            if (isEqual == false) return false;
        }
        return true;
    }

    bool IsEqual(const TRAFFIC_LIGHT_TARGET_STRU& stTrafficLight) const {
        if (this->isOpen != stTrafficLight.isOpen) return false;
        if (0 != SIG(this->pixelBias - stTrafficLight.pixelBias)) return false;
        if (0 != SIG(this->rectBias  - stTrafficLight.rectBias)) return false;
        if (this->stTarget != stTrafficLight.stTarget) return false;
        if (!IsListEqual(stTrafficLight.lstNear)) return false;
        return true;
    }

    bool operator == (const TRAFFIC_LIGHT_TARGET_STRU& stTrafficLight) const {
        return IsEqual(stTrafficLight);
    }

    bool operator != (const TRAFFIC_LIGHT_TARGET_STRU& stTrafficLight) const {
        return !IsEqual(stTrafficLight);
    }

    bool NearsEmpty(void) const {return lstNear.empty();}
    void ClearNears() {lstNear.clear();}

    uint8_t type(void)      const {return stTarget.type;}
    float   px(void)        const {return stTarget.px;}
    float   py(void)        const {return stTarget.py;}
    float   pz(void)        const {return stTarget.pz;}
    float   rowLength(void) const {return stTarget.rowLength;}
    float   colLength(void) const {return stTarget.colLength;}

    bool                            isOpen{false};      // 是否开启交通灯检测
    float                           pixelBias;          // 像素偏差
    float                           rectBias;           // 检测框匹配率偏差
    TRAFFIC_LIGHT_INFO_STRU         stTarget;           // 目标交通灯信息
    list<TRAFFIC_LIGHT_INFO_STRU>   lstNear;            // 周边交通灯信息
};

/**************************************************************************************
功能描述: 发布红绿灯检测的消息
修改记录:
**************************************************************************************/
class TRAFFIC_LIGHT_TARGET_SENDER_C
{
public:
    TRAFFIC_LIGHT_TARGET_SENDER_C(ros::NodeHandle &nh)
    {
        objPublisher_ = nh.advertise<comm_msg::traffic_light_target>(TOPIC_NAME_TRAFFIC_LIGHT_TARGET, 1);
    }

    void DetectTargetTrafficLight(const UINT32 &isOpen, const float &pixelBias, const float &rectBias,
                                  const TRAFFIC_LIGHT_INFO_STRU& stTtargetInfo,
                                  const list<TRAFFIC_LIGHT_INFO_STRU>& lstNearInfo = {})
    {
        comm_msg::traffic_light_target stMsg;
        stMsg.is_open           = isOpen;                   // 是否开启交通灯检测：0(关闭) 1(开启)
        stMsg.pixel_bias        = pixelBias;
        stMsg.rect_bias         = rectBias;

        stMsg.target.type       = stTtargetInfo.type;       // 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
        stMsg.target.pos_x      = stTtargetInfo.px;
        stMsg.target.pos_y      = stTtargetInfo.py;
        stMsg.target.pos_z      = stTtargetInfo.pz;
        stMsg.target.row_length = stTtargetInfo.rowLength;
        stMsg.target.col_length = stTtargetInfo.colLength;

        for (const TRAFFIC_LIGHT_INFO_STRU& stInfo : lstNearInfo) {
            comm_msg::traffic_light_info stMsgInfo;
            stMsgInfo.type       = stInfo.type;
            stMsgInfo.pos_x      = stInfo.px;
            stMsgInfo.pos_y      = stInfo.py;
            stMsgInfo.pos_z      = stInfo.pz;
            stMsgInfo.row_length = stInfo.rowLength;
            stMsgInfo.col_length = stInfo.colLength;
            stMsg.nears.push_back(stMsgInfo);
        }

        objPublisher_.publish(stMsg);
    }

private:
    ros::Publisher objPublisher_;
};

/**************************************************************************************
功能描述: 接收红绿灯检测结果的消息
修改记录:
**************************************************************************************/
class TRAFFIC_LIGHT_RESULT_RECEIVER_C
{
public:
    TRAFFIC_LIGHT_RESULT_RECEIVER_C(ros::NodeHandle &nh)
    {
        objSubscriber_ = nh.subscribe(TOPIC_NAME_TRAFFIC_LIGHT_RESULT, 1, &TRAFFIC_LIGHT_RESULT_RECEIVER_C::ReceiveMsgCallBack, this);
    }

    INT32 GetResultTrafficLight(UINT32 &type, UINT32 &trafficLightResult, UINT32 &seconds, UINT32 &status)
    {
        BOOL isDataInvalid = objValidTimer_.IsTimeOut();
        ST_LOG_ERR_SAMPLE_IF_TRUE(isDataInvalid, 20 * 10, "Data Invalid.");
        if (isDataInvalid) {
            type = 0;
            trafficLightResult_ = comm_msg::label::TRAFFIC_LIGHT;
            seconds_ = 0;
            status_  = 0;
            return -1;
        }

        type = type_;
        trafficLightResult = trafficLightResult_;
        seconds        = seconds_;
        status         = status_;
        return 0;
    }

private:
    ros::Subscriber objSubscriber_;
    TIMER_C         objValidTimer_{0.25};
    UINT32          type_{0};
    UINT32          trafficLightResult_{comm_msg::label::TRAFFIC_LIGHT};
    UINT32          seconds_{0};
    UINT32          status_{0};

    void ReceiveMsgCallBack(const comm_msg::traffic_light_result::ConstPtr &pstMsg)
    {
        R_ASSERT(pstMsg != nullptr, NOTHING);

        objValidTimer_.Reset();

        if (pstMsg->label.value == comm_msg::label::TRAFFIC_LIGHT_GREEN ||
            pstMsg->label.value == comm_msg::label::TRAFFIC_LIGHT_OFF   ||
            pstMsg->label.value == comm_msg::label::TRAFFIC_LIGHT_RED   ||
            pstMsg->label.value == comm_msg::label::TRAFFIC_LIGHT_GREEN ||
            pstMsg->label.value == comm_msg::label::TRAFFIC_LIGHT_YELLOW) {
            trafficLightResult_ = pstMsg->label.value;
        }
        type_    = pstMsg->type;
        seconds_ = pstMsg->seconds;
        status_  = pstMsg->status;

    }
};

#endif

