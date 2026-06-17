#pragma once

#include "dev_manager.h"
#include "software_manager.h"
#include "common_manager.h"
#include "comm/comm_msg_nav_state.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_ros.h"
#include "pcpt_comm/pcpt_comm_car.h"
#include "pcpt_comm/pcpt_comm_points.h"
#include "pcpt_comm/pcpt_comm_car_obst_view.h"
#include "pcpt_comm/pcpt_comm_singleton.h"
#include <opencv2/opencv.hpp>

#define DEV_MNG_TYPE_OBST_DATA_REQ              66
#define DEV_MNG_TYPE_OBST_DATA_RSP              67

#define COMM_MNG_TYPE_HMI_UPLOAD_COOR_REQ        7
#define COMM_MNG_TYPE_NAV_UPLOAD_COOR_RSP        8
#define COMM_MNG_TYPE_NAV_UPLOAD_COOR            9
#define SOFTWARE_MNG_TYPE_HMI_UPLOAD_FILE_REQ   23
#define SOFTWARE_MNG_TYPE_NAV_UPLOAD_FILE_RSP   24
#define SOFTWARE_MNG_TYPE_NAV_UPLOAD_FILE       25

struct PCPT_HMI_STRU {
    int     msgDomain;
    int     msgType;
    int     transId;
    string     requestId;
};

struct PCPT_HMI_OBST_STRU {
    float         radius;
    PCPT_HMI_STRU stHmiMsg;
};

struct PCPT_HMI_COOR_STRU {
    PCPT_HMI_STRU stHmiMsg;
    std::string requestId;
};

struct PCPT_HMI_OBST_3VIEW_STRU {
    PCPT_HMI_STRU stHmiMsg;
    std::vector<std::string> vstrFileId;
    std::vector<std::string> vstrFileType;   
};

enum PCPT_HMI_STATE_ENUM {
    EN_PCPT_HMI_NULL,   // 未知
    EN_PCPT_HMI_RSPED,  // 已响应
    EN_PCPT_HMI_UNRSP,  // 未响应
};
    
class PCPT_HMI_C
{
public:
    PCPT_HMI_C() {}
    PCPT_HMI_C(const PCPT_HMI_C &other) = delete;         // 禁止使用拷贝构造函数
    PCPT_HMI_C &operator=(const PCPT_HMI_C &) = delete;   // 禁止使用拷贝赋值运算符
    bool isResponseCleanClassRequest_ = false;

    PCPT_HMI_STRU stHmiCleanClassMsg_;
    void UploadCleanClassParam(cv::Point ptLeftUp,cv::Point ptRightUp,cv::Point ptRightDown,cv::Point ptLeftDown);
    void UploadCleanClassImg(cv::Mat img);
    void Reset();

    ~PCPT_HMI_C(void)
    {
        /* 清理注册，防止析构后本对象注册的回调函数被调用。 */
        if (pobjDev_ != nullptr) {
            (void)pobjDev_->UnRegClear();
        }

        if (pobjSoftware_ != nullptr) {
            (void)pobjSoftware_->UnRegAll();
        }

        if (pobjCommMng_ != nullptr) {
            (void)pobjCommMng_->UnRegAll();
        }

        if (pobjLocInfoRec_ != nullptr) {
            delete pobjLocInfoRec_;
            pobjLocInfoRec_ = nullptr;
        }

        if (thread_.joinable()) {
            thread_.join();
        }
    }

    bool IsUnRespondHmiReq();
    bool IsUnRespondHmiObstReq();
    
    bool IsUnRespondHmiUploadChargeStationReq(void);
    void RspHmiUploadChargeStation(const bool isOk, 
                                   const COORDINATE_2D_STRU &stLeft, 
                                   const COORDINATE_2D_STRU &stRight);
    
    void UpLoadTrafficLightInfo(const TRAFFIC_LIGHT_STATE_INFO_STRU &stTrafficLightStateInfo);
    void UpLoadPerceptionObjInfo(const FRAME_OBJ_INFO_STRU &stFrameObjInfo);

    int RegisterCleanClassCallback();

    static PCPT_HMI_C *GetInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            return pInstance_;
        }

        pInstance_ = new PCPT_HMI_C();
        INT32 rslt = pInstance_->Init();
        if (rslt != 0) {
            ST_LOG_ERR("Create Object Fail.");
            DelPtr(pInstance_);
            return nullptr;
        }

        PCPT::SINGLETON_MNG_C::RegInstanceClrFunc(&ClrInstance);
        ST_LOG_INFO("Create Object Succ.");
        return pInstance_;
    }

    static void ClrInstance(void)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex_r_);

        if (pInstance_ != nullptr) {
            delete pInstance_;
            pInstance_ = nullptr;
            ST_LOG_INFO("Delete Object Succ.");
        }
    }

private:
    TIMER_C                     objTimer_{1.0};
    BasicDevManager*            pobjDev_{nullptr};
    SoftwareManager*            pobjSoftware_{nullptr};
    CommonManager*              pobjCommMng_{nullptr};

    std::atomic_int             obstRspState_{EN_PCPT_HMI_NULL};
    std::thread                 thread_;
    std::recursive_mutex        mutex_;
    static std::recursive_mutex mutex_r_;
    static PCPT_HMI_C*          pInstance_;

    double                      stamp_{0.0};
    PCPT_HMI_OBST_STRU          stHmiObstMsg_;
    
    vector<COORDINATE_2D_STRU>  vstCoorRange_;

    LOCATION_MSG_RECEIVER_C* pobjLocInfoRec_;
    PCPT_HMI_COOR_STRU stHmiUploadCoorMsg_;
    std::atomic_int uploadChargeStatioinCoorRspState_{EN_PCPT_HMI_NULL};
    std::atomic_int uploadDustbinStatioinCoorRspState_{EN_PCPT_HMI_NULL};
    
    PCPT_HMI_OBST_3VIEW_STRU stHmiObst3ViewMsg_;
    std::atomic_int obst3dViewRspState_{EN_PCPT_HMI_NULL};
    std::vector<float> vRegion_{-4.0, 6.0, -4.0, 4.0, -1.0, 4.0};
    PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C::Ptr    pobjObstTop2DownView_{nullptr};
    PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C::Ptr  pobjObstBack2FrontView_{nullptr};
    PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C::Ptr  pobjObstLeft2RightView_{nullptr};
    
    int  Init();
    void PeriodProcess();
    void UptHmiReqMsg(const int msgDomain, const int msgType, const int transId, PCPT_HMI_STRU& stHmiMsg);

    void Get2dObstData(PCPT_OBST_STRU& st2dObst);
    void UptHmiObstReqMsg(const int msgDomain, const int transId, const float radius);
    void GetHmiObstReqMsg(PCPT_HMI_OBST_STRU& stHmiObstMsg);
    int  GetRangePoints(float radius, vector<COORDINATE_2D_STRU>& vstCoor);
    void ReqObstDataCallBack(JsonValueConstPtr& pobjJson);
    void ReqObstDataCallBackSim();
    void RspHmiObstData(PCPT_HMI_OBST_STRU &stHmiObstMsg, const vector<COORDINATE_2D_STRU>& vstCoor);

    void ReqUploadCoorCallback(JsonValueConstPtr& pobjJson);
    void UptHmiUploadCoorReqMsg(const PCPT_HMI_COOR_STRU &stHmiCoorMsg);
    void GetHmiUploadCoorReqMsg(PCPT_HMI_COOR_STRU &stHmiUploadCoorMsg);
    int  RspHmiUploadDustbinStation(PCPT_HMI_COOR_STRU &stHmiUploadCoorMsg);
    bool IsUnRespondHmiUploadDustbinStationReq(void);
    
    int  InitObst3View(void);
    void ReqObst3ViewDataCallBack(JsonValueConstPtr& pobjJson);
    void UptHmiObst3ViewReqMsg(const PCPT_HMI_OBST_3VIEW_STRU& stHmiObst3ViewMsg);
    void GetHmiObst3ViewReqMsg(PCPT_HMI_OBST_3VIEW_STRU& stHmiObst3ViewMsg);
    bool IsUnRespondHmiObst3ViewReq(void);
    int  RspHmiObst3ViewData(const PCPT_HMI_OBST_3VIEW_STRU& stHmiObst3ViewMsg);

    // 接收“人机关于检测清洁度的请求”
    void CleanClassRequestCallBack(JsonValueConstPtr& pobjJson);
    void HmiImageResponseCallBack(JsonValueConstPtr& pobjJson);
    void CleanClassValueResponseCallBack(JsonValueConstPtr& pobjJson);

    void UptHmiCleanClassReqMsg(PCPT_HMI_STRU& stHmiMsg);
    void GetHmiCleanClassReqMsg(PCPT_HMI_STRU& stHmiMsg);
};
