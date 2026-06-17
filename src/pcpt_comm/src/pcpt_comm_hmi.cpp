#include "pcpt_comm/pcpt_comm_hmi.h"

std::recursive_mutex    PCPT_HMI_C::mutex_r_;
PCPT_HMI_C*             PCPT_HMI_C::pInstance_{nullptr};

int  PCPT_HMI_C::Init()
{
    pobjDev_ = BasicDevManager::Create();
    if (!pobjDev_) {
        ST_LOG_ERR("Failed to Create BasicDevManager Object.");
        return -1;
    }

    pobjSoftware_ = SoftwareManager::Create();
    if (!pobjSoftware_) {
        ST_LOG_ERR("Failed to Create SoftwareManager Object.");
        return -1;
    }

    pobjCommMng_ = CommonManager::Create();
    if (!pobjCommMng_) {
        ST_LOG_ERR("Failed to Create CommonManager Object.");
        return -1;
    }

    if (pobjDev_->StartStateReport()) {
        ST_LOG_ERR("Failed to Start the BasicDevManager State Reporting Function.");
        return -1;
    }

    int rslt = 0;
    rslt = pobjDev_->Reg(DEV_MNG_TYPE_OBST_DATA_REQ, &PCPT_HMI_C::ReqObstDataCallBack, this);
    if (rslt != 0) {
        ST_LOG_ERR("Failed to register <DEV_MNG_TYPE_OBST_DATA_REQ> msg callback function.");
        return -1;
    }
    
    rslt = pobjSoftware_->Reg(SOFTWARE_MNG_TYPE_HMI_UPLOAD_FILE_REQ, &PCPT_HMI_C::ReqObst3ViewDataCallBack, this);
    if (rslt != 0) {
        ST_LOG_ERR("Failed to register <SOFTWARE_MNG_TYPE_HMI_UPLOAD_FILE_REQ> msg callback function.");
        return -1;
    }

    rslt = pobjCommMng_->Reg(COMM_MNG_TYPE_HMI_UPLOAD_COOR_REQ,  &PCPT_HMI_C::ReqUploadCoorCallback, this);
    if (rslt) {
        ST_LOG_ERR("Failed to register <COMM_MNG_TYPE_HMI_UPLOAD_COORD_REQ> msg callback function.");
        return -1;
    }

    if (0 != InitObst3View()) {
        ST_LOG_ERR("InitObst3View() Fail.");
        return -1;
    }

    ros::NodeHandle nh;
    pobjLocInfoRec_ = new LOCATION_MSG_RECEIVER_C(nh);

    thread_ = std::thread(&PCPT_HMI_C::PeriodProcess, this);
    pthread_setname_np(thread_.native_handle(), "pcpt_hmi");

    ST_LOG_INFO("Init Success.");
    return 0;
}

void PCPT_HMI_C::PeriodProcess()
{
    int count = 0;
    int obst3dViewCnt = 0;
    int uploadDustbinStationCnt = 0;
    ros::Rate loop_rate(PCPT_HZ_10);

    while (g_isWork) {
        if (obstRspState_ == EN_PCPT_HMI_UNRSP) {
            PCPT_HMI_OBST_STRU stHmiObstMsg;
            GetHmiObstReqMsg(stHmiObstMsg);

            vstCoorRange_.clear();
            if (GetRangePoints(stHmiObstMsg.radius, vstCoorRange_)) {
                if (++count < 10) CONTINUE(50);
                ST_LOG_ERR("获取障碍点云异常，返回人机空消息！");//防止人机获取消息超时
            }

            count = 0;
            RspHmiObstData(stHmiObstMsg, vstCoorRange_);
            obstRspState_ = EN_PCPT_HMI_RSPED;
        }

        if (obst3dViewRspState_ == EN_PCPT_HMI_UNRSP) {
            PCPT_HMI_OBST_3VIEW_STRU stHmiObst3ViewMsg;
            GetHmiObst3ViewReqMsg(stHmiObst3ViewMsg);
            if (0 != RspHmiObst3ViewData(stHmiObst3ViewMsg)) {
                if (++obst3dViewCnt < 10) CONTINUE(50);
                ST_LOG_ERR("获取障碍点云3视图失败！");
            }
            obst3dViewCnt = 0;
            obst3dViewRspState_ = EN_PCPT_HMI_RSPED;
        }

        if (uploadDustbinStatioinCoorRspState_ == EN_PCPT_HMI_UNRSP) {
            PCPT_HMI_COOR_STRU stHmiUploadCoorMsg;
            GetHmiUploadCoorReqMsg(stHmiUploadCoorMsg);
            if (0 != RspHmiUploadDustbinStation(stHmiUploadCoorMsg)) {
                if (++uploadDustbinStationCnt < 10) CONTINUE(50);
                ST_LOG_ERR("获取垃圾桶坐标失败！");
            }
            uploadDustbinStationCnt = 0;
            uploadDustbinStatioinCoorRspState_ = EN_PCPT_HMI_RSPED;
        }
         
        loop_rate.sleep();
    }
}

bool PCPT_HMI_C::IsUnRespondHmiReq()
{   
    bool isUnRespondHmiReq =   IsUnRespondHmiObstReq() 
                            || IsUnRespondHmiObst3ViewReq()
                            || IsUnRespondHmiUploadChargeStationReq()
                            || IsUnRespondHmiUploadDustbinStationReq();
    return isUnRespondHmiReq;
}

bool PCPT_HMI_C::IsUnRespondHmiObstReq()
{
    return obstRspState_ == EN_PCPT_HMI_UNRSP;
}

void PCPT_HMI_C::UpLoadTrafficLightInfo(const TRAFFIC_LIGHT_STATE_INFO_STRU &stTrafficLightStateInfo)
{
    static INT32 transId = 0;
    Json::Value objJsonValue;
    objJsonValue["MsgType"] = 118;
    objJsonValue["TransId"] = transId;
    objJsonValue["MsgContent"]["timestampGNSS"] = stTrafficLightStateInfo.timestampGNSS;
    objJsonValue["MsgContent"]["vehPhase"] = stTrafficLightStateInfo.vehPhase;
    objJsonValue["MsgContent"]["vehLight"] = stTrafficLightStateInfo.vehLight;
    if (0 != pobjDev_->Send(objJsonValue)) {
       ST_LOG_ERR("发送交通灯状态信息至人机失败.");
    }
    transId++;
}

void PCPT_HMI_C::UpLoadPerceptionObjInfo(const FRAME_OBJ_INFO_STRU &stFrameObjInfo)
{
    static INT32 transId = 0;
    Json::Value objJsonValue;
    objJsonValue["MsgType"] = 117;
    objJsonValue["TransId"] = transId;
    objJsonValue["MsgContent"]["timestampGNSS"] = stFrameObjInfo.timestampGNSS;
    objJsonValue["MsgContent"]["FrameID"] = stFrameObjInfo.id;

    Json::Value objJsonObjInfos;
    for (const OBJ_INFO_STRU &stObjInfo : stFrameObjInfo.vstObjInfo) {
        Json::Value objJsonObjInfo;
        objJsonObjInfo["ObjectID"]   = stObjInfo.id;
        objJsonObjInfo["typePept"]   = stObjInfo.type;
        objJsonObjInfo["Xdirection"] = stObjInfo.xDirection;
        objJsonObjInfo["position_transversal"]  = stObjInfo.transversalPos;
        objJsonObjInfo["position_longitudinal"] = stObjInfo.longitudinalPos;
        objJsonObjInfo["Xspeed"]     = stObjInfo.speed;
        objJsonObjInfo["lengthPept"] = stObjInfo.length;
        objJsonObjInfo["widthPept"]  = stObjInfo.width;
        objJsonObjInfo["heightPept"] = stObjInfo.height;
        objJsonObjInfos.append(objJsonObjInfo);
    }
    objJsonValue["MsgContent"]["Objects"] =   stFrameObjInfo.vstObjInfo.empty() 
                                            ? Json::Value(Json::arrayValue) : objJsonObjInfos;
    if (0 != pobjDev_->Send(objJsonValue)) {
       ST_LOG_ERR("发送感知目标信息至人机失败.");
    }
    transId++;
}

void PCPT_HMI_C::Get2dObstData(PCPT_OBST_STRU& st2dObst)
{
    PCPT_OBST_DATA_C* pobj2dObst = PCPT_OBST_DATA_C::GetInstance();
    if (pobj2dObst) pobj2dObst->Get2dObstData(st2dObst);
}

void PCPT_HMI_C::UptHmiObstReqMsg(const int msgDomain, const int transId, const float radius)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    UptHmiReqMsg(msgDomain, DEV_MNG_TYPE_OBST_DATA_RSP, transId, stHmiObstMsg_.stHmiMsg);
    stHmiObstMsg_.radius = radius;
}

void PCPT_HMI_C::GetHmiObstReqMsg(PCPT_HMI_OBST_STRU& stHmiObstMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiObstMsg = stHmiObstMsg_;
}

int  PCPT_HMI_C::GetRangePoints(float radius, vector<COORDINATE_2D_STRU>& vstCoor)
{
    PCPT_OBST_STRU st2dObstByMap;
    Get2dObstData(st2dObstByMap);
    if (!SIG(st2dObstByMap.stamp - stamp_)) {
        //ST_LOG_WARN("The timestamps are equal and there is no new data. Timestamps is:%f.", stamp_);
        return -1;
    }

    stamp_ = st2dObstByMap.stamp;

    if (st2dObstByMap.empty()) {
        ST_LOG_WARN("There are no obstacles in the perception area.");
        return 0;
    }

    MULTILAYER_OBST_STRU stLayerObst;
    st2dObstByMap.MergeToLayerObst(stLayerObst);

    COORDINATE_3D_STRU& stPos = st2dObstByMap.stCarPose.stPos;
    RECTANGLE_STRU stRect(stPos.x - radius, stPos.x + radius, stPos.y - radius, stPos.y + radius);

    for (const COORDINATE_2D_STRU &stCoor : stLayerObst.lstObstDown)
        if (stRect.IsIn(stCoor.x, stCoor.y)) vstCoor.push_back(stCoor);

    for (const COORDINATE_2D_STRU &stCoor : stLayerObst.lstObstTop)
        if (stRect.IsIn(stCoor.x, stCoor.y)) vstCoor.push_back(stCoor);

    ST_LOG_INFO("------------------------------------------------------------------");
    ST_LOG_INFO("Origin points num: down_%u top_%u", (uint32_t)stLayerObst.lstObstDown.size(), (uint32_t)stLayerObst.lstObstTop.size());
    ST_LOG_INFO("Car pose: x:%.2f y:%.2f", stPos.x, stPos.y);
    ST_LOG_INFO("Range rectangle x:%.2f~%.2f, y:%.2f~%.2f, radius:%.2f", stRect.minX, stRect.maxX, stRect.minY, stRect.maxY, radius);
    ST_LOG_INFO("Range points num:%u", (uint32_t)vstCoor.size());
    ST_LOG_INFO("------------------------------------------------------------------");

    return 0;
}

void PCPT_HMI_C::ReqObstDataCallBack(JsonValueConstPtr& pobjJson)
{
    if (!pobjJson) {
        ST_LOG_ERR("Invalid callback parameter.");
        return;
    }

    Json::Value objJson;
    objJson = *pobjJson;
    R_ASSERT(objJson["MsgDomain"].isInt(),     NOTHING);
    R_ASSERT(objJson["MsgType"].isInt(),       NOTHING);
    R_ASSERT(objJson["TransId"].isInt(),       NOTHING);
    R_ASSERT(objJson["MsgContent"].isObject(), NOTHING);

    const Json::Value &objJsonContent = objJson["MsgContent"];
    R_ASSERT(objJsonContent["Radius"].isDouble(), NOTHING);

    int   msgDomain = objJson["MsgDomain"].asInt();
    int   msgType   = objJson["MsgType"].asInt();
    int   transId   = objJson["TransId"].asInt();
    float radius    = objJson["MsgContent"]["Radius"].asFloat();

    ST_LOG_INFO("Request: msgDomain:%d, msgType:%d, transId:%d, radius:%f.", msgDomain, msgType, transId, radius);
    ST_LOG_INFO("收到人机查询周边感知数据请求。");
    if (!SIG(radius)) {
        ST_LOG_INFO("查询半径为0，当前查询无效。");
        return;
    }

    if (msgDomain != 1) {
        ST_LOG_ERR("Msg domain error: %d.", msgDomain);
        return;
    }

    UptHmiObstReqMsg(msgDomain, transId, radius);
    obstRspState_ = EN_PCPT_HMI_UNRSP;
}

void PCPT_HMI_C::ReqObstDataCallBackSim()
{
    Json::Value objJson;
    objJson["MsgDomain"] = 1;
    objJson["MsgType"] = DEV_MNG_TYPE_OBST_DATA_REQ;
    objJson["TransId"] = 101;
    objJson["MsgContent"]["Radius"] = 3.0;

    Json::Value *pobjJsonT(new Json::Value);
    *pobjJsonT = objJson;

    JsonValueConstPtr pobjJson(pobjJsonT);
    ReqObstDataCallBack(pobjJson);
}

void PCPT_HMI_C::RspHmiObstData(PCPT_HMI_OBST_STRU &stHmiObstMsg, const vector<COORDINATE_2D_STRU>& vstCoor)
{
    Json::Value response;
    response["MsgDomain"] = stHmiObstMsg.stHmiMsg.msgDomain;
    response["MsgType"]   = stHmiObstMsg.stHmiMsg.msgType;
    response["TrandId"]   = stHmiObstMsg.stHmiMsg.transId;

    Json::Value points;
    for (const COORDINATE_2D_STRU &stCoor :vstCoor) {
        Json::Value point;
        point.append(stCoor.x);  // x
        point.append(stCoor.y);  // y
        points.append(point);
    }

    ST_LOG_INFO("Response: msgDomain:%d, msgType:%d, transId:%d, pointNum:%u.",
        stHmiObstMsg.stHmiMsg.msgDomain, stHmiObstMsg.stHmiMsg.msgType, stHmiObstMsg.stHmiMsg.transId, (uint32_t)vstCoor.size());

    response["MsgContent"]["Points"] = points;
    pobjDev_->Send(response);
}


void PCPT_HMI_C::UptHmiUploadCoorReqMsg(const PCPT_HMI_COOR_STRU &stHmiCoorMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiUploadCoorMsg_ = stHmiCoorMsg;
}

void PCPT_HMI_C::GetHmiUploadCoorReqMsg(PCPT_HMI_COOR_STRU &stHmiUploadCoorMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiUploadCoorMsg = stHmiUploadCoorMsg_;
}

void PCPT_HMI_C::ReqUploadCoorCallback(JsonValueConstPtr& pobjJson)
{
    ST_LOG_INFO("收到报文：%s", pobjJson->toStyledString().data());

    Json::Value objJson;
    objJson = *pobjJson;
    R_ASSERT(objJson["MsgDomain"].isInt(),     NOTHING);
    R_ASSERT(objJson["MsgType"].isInt(),       NOTHING);
    R_ASSERT(objJson["TransId"].isInt(),       NOTHING);
    R_ASSERT(objJson["MsgContent"].isObject(), NOTHING);
    const Json::Value &objJsonContent = objJson["MsgContent"];
    R_ASSERT(objJsonContent["CoordinateTypeList"].isArray(), NOTHING);

    PCPT_HMI_COOR_STRU stHmiUploadCoorMsg;
    stHmiUploadCoorMsg.stHmiMsg.msgDomain = objJson["MsgDomain"].asInt();
    stHmiUploadCoorMsg.stHmiMsg.transId   = objJson["TransId"].asInt();
    stHmiUploadCoorMsg.requestId = objJson["MsgContent"]["RequestId"].asString();
    int typeListNum = objJsonContent["CoordinateTypeList"].size();
    for (int i = 0; i < typeListNum; i++) {
        if (objJsonContent["CoordinateTypeList"][i] == 2) {        //垃圾箱坐标
            uploadDustbinStatioinCoorRspState_ = EN_PCPT_HMI_UNRSP;
        } else if (   objJsonContent["CoordinateTypeList"][i] == 4
                   || objJsonContent["CoordinateTypeList"][i] == 5) { //充电点坐标
            uploadChargeStatioinCoorRspState_ = EN_PCPT_HMI_UNRSP;
            break;
        } else {
           return;
        }
    }
    stHmiUploadCoorMsg.stHmiMsg.msgType = COMM_MNG_TYPE_NAV_UPLOAD_COOR;
    UptHmiUploadCoorReqMsg(stHmiUploadCoorMsg);
    
    Json::Value response;
    response["MsgDomain"] = stHmiUploadCoorMsg.stHmiMsg.msgDomain;
    response["MsgType"]   = COMM_MNG_TYPE_NAV_UPLOAD_COOR_RSP;
    response["TransId"]   = stHmiUploadCoorMsg.stHmiMsg.transId;
    response["MsgContent"]["ErrCode"] = 0;
    pobjCommMng_->Send(response);
    
    ST_LOG_INFO("发送报文：%s", response.toStyledString().data());
}

bool PCPT_HMI_C::IsUnRespondHmiUploadChargeStationReq(void)
{
    return uploadChargeStatioinCoorRspState_ == EN_PCPT_HMI_UNRSP;
}

bool PCPT_HMI_C::IsUnRespondHmiUploadDustbinStationReq(void)
{
    return uploadDustbinStatioinCoorRspState_ == EN_PCPT_HMI_UNRSP;
}

void PCPT_HMI_C::RspHmiUploadChargeStation(const bool isOk, 
                                           const COORDINATE_2D_STRU &stLeft, 
                                           const COORDINATE_2D_STRU &stRight)
{
    if (uploadChargeStatioinCoorRspState_ != EN_PCPT_HMI_UNRSP) return;
    uploadChargeStatioinCoorRspState_ = EN_PCPT_HMI_RSPED;
    
    PCPT_HMI_COOR_STRU stHmiUploadCoorMsg;
    GetHmiUploadCoorReqMsg(stHmiUploadCoorMsg);

    Json::Value response;
    response["MsgDomain"] = stHmiUploadCoorMsg.stHmiMsg.msgDomain;
    response["MsgType"]   = stHmiUploadCoorMsg.stHmiMsg.msgType;
    response["TransId"]   = stHmiUploadCoorMsg.stHmiMsg.transId;

    if (isOk) {
        Json::Value objJsonCoordinateList;
        Json::Value objLeftRefStripeInfo;
        objLeftRefStripeInfo["MapX"]   = stLeft.x;
        objLeftRefStripeInfo["MapY"]   = stLeft.y;
        objLeftRefStripeInfo["MapYaw"] = 0.0;
        objJsonCoordinateList.append(objLeftRefStripeInfo);

        Json::Value objRightRefStripeInfo;
        objRightRefStripeInfo["MapX"]   = stRight.x;
        objRightRefStripeInfo["MapY"]   = stRight.y;
        objRightRefStripeInfo["MapYaw"] = 0.0;
        objJsonCoordinateList.append(objRightRefStripeInfo);
        
        response["MsgContent"]["ErrCode"] = 0;
        response["MsgContent"]["RequestId"] = stHmiUploadCoorMsg.requestId;
        response["MsgContent"]["CoordinateList"] = objJsonCoordinateList;
    } else {
        response["MsgContent"]["RequestId"] = stHmiUploadCoorMsg.requestId;
        response["MsgContent"]["ErrCode"] = 0x00100012;
    }

    pobjCommMng_->Send(response);
    ST_LOG_INFO("发送报文：%s", response.toStyledString().data());
}

int  PCPT_HMI_C::RspHmiUploadDustbinStation(PCPT_HMI_COOR_STRU &stHmiUploadCoorMsg)
{
    COOR_2D_POSE_STRU stCarPose;
    if (0 != pobjLocInfoRec_->GetNormal(stCarPose)) {
        ST_LOG_ERR("获取车辆位姿失败.");
        return -1;
    }

    PCPT_CAR_C objCar;
    float xByCar = - objCar.GetBackHalfBodyLen();
    //float yByCar = 0.0;
    float xByMap = stCarPose.stPos.x + xByCar * cos(stCarPose.dirAngle);
    float yByMap = stCarPose.stPos.y + xByCar * sin(stCarPose.dirAngle);
    
    Json::Value response;
    response["MsgDomain"] = stHmiUploadCoorMsg.stHmiMsg.msgDomain;
    response["MsgType"]   = stHmiUploadCoorMsg.stHmiMsg.msgType;
    response["TransId"]   = stHmiUploadCoorMsg.stHmiMsg.transId;

    Json::Value objJsonCoordinateList;
    Json::Value objDustbinCoor;
    objDustbinCoor["MapX"] = xByMap;
    objDustbinCoor["MapY"] = yByMap;
    objDustbinCoor["MapYaw"] = stCarPose.dirAngle;
    objJsonCoordinateList.append(objDustbinCoor);

    response["MsgContent"]["ErrCode"] = 0;
    response["MsgContent"]["RequestId"] = stHmiUploadCoorMsg.requestId;
    response["MsgContent"]["CoordinateList"] = objJsonCoordinateList;
    
    pobjCommMng_->Send(response);

    return 0;
}

int  PCPT_HMI_C::InitObst3View(void)
{
    PCPT_CAR_C objCar;
    float headHalfBody  = objCar.GetHeadHalfBodyLen();
    float backHalfBody  = objCar.GetBackHalfBodyLen();
    float halfBodyWidthNoExt = objCar.HalfBodyWidthNoExtend();
    objCar.SetExpandState(true);
    objCar.Update();
    float halfBodyWidthExt   = objCar.HalfBodyWidthWithExtend();
    float bodyHeight = objCar.GetCarHeight();

    RECTANGLE_STRU stViewReg1{vRegion_[0], vRegion_[1], vRegion_[2], vRegion_[3]}; 
    COORDINATE_2D_STRU stCarBodyLT1( headHalfBody,  halfBodyWidthNoExt);
    COORDINATE_2D_STRU stCarBodyRD1(-backHalfBody, -halfBodyWidthNoExt);
    COORDINATE_2D_STRU stCarBodyExtLT1( headHalfBody,  halfBodyWidthExt);
    COORDINATE_2D_STRU stCarBodyExtRD1(-backHalfBody, -halfBodyWidthExt);
    pobjObstTop2DownView_   = std::make_shared<PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C>(
        stCarBodyLT1, stCarBodyRD1, stCarBodyExtLT1, stCarBodyExtRD1,stViewReg1);

    RECTANGLE_STRU stViewReg2{vRegion_[4], vRegion_[5], vRegion_[2], vRegion_[3]};
    COORDINATE_2D_STRU stCarBodyLT2( bodyHeight,  halfBodyWidthNoExt);
    COORDINATE_2D_STRU stCarBodyRD2(-0.0,        -halfBodyWidthNoExt);
    COORDINATE_2D_STRU stCarBodyExtLT2( bodyHeight,  halfBodyWidthExt);
    COORDINATE_2D_STRU stCarBodyExtRD2(       -0.0, -halfBodyWidthExt);
    pobjObstBack2FrontView_ = std::make_shared<PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C>(
        stCarBodyLT2, stCarBodyRD2, stCarBodyExtLT2, stCarBodyExtRD2,stViewReg2);

    RECTANGLE_STRU stViewReg3{vRegion_[0], vRegion_[1], vRegion_[4], vRegion_[5]};
    COORDINATE_2D_STRU stCarBodyLT3( headHalfBody,  bodyHeight);
    COORDINATE_2D_STRU stCarBodyRD3(-backHalfBody,         0.0);
    pobjObstLeft2RightView_ = std::make_shared<PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C>(
        stCarBodyLT3, stCarBodyRD3, stViewReg3);

    return 0;
}

void PCPT_HMI_C::ReqObst3ViewDataCallBack(JsonValueConstPtr& pobjJson)
{
    if (!pobjJson) {
        ST_LOG_ERR("Invalid callback parameter.");
        return;
    }

    Json::Value objJson;
    objJson = *pobjJson;
    R_ASSERT(objJson["MsgDomain"].isInt(),     NOTHING);
    R_ASSERT(objJson["MsgType"].isInt(),       NOTHING);
    R_ASSERT(objJson["TransId"].isInt(),       NOTHING);
    R_ASSERT(objJson["MsgContent"].isObject(), NOTHING);

    const Json::Value &objJsonContent = objJson["MsgContent"];
    R_ASSERT(objJsonContent["FileInfoList"].isArray(), NOTHING);

    PCPT_HMI_OBST_3VIEW_STRU stObst3ViewMsg;
    stObst3ViewMsg.stHmiMsg.msgDomain = objJson["MsgDomain"].asInt();
    stObst3ViewMsg.stHmiMsg.msgType   = objJson["MsgType"].asInt();
    stObst3ViewMsg.stHmiMsg.transId   = objJson["TransId"].asInt();
    
    int fileNum = objJsonContent["FileInfoList"].size();
    for (int i = 0; i < fileNum; i++) {
        Json::Value jsonFileInfo = objJsonContent["FileInfoList"][i];
        std::string strFileId   = jsonFileInfo["FileId"].asString();
        std::string strFileType = jsonFileInfo["FileType"].asString();
        if (   strFileType == "points_top_image" 
            || strFileType == "points_rear_image"
            || strFileType == "points_side_image") {
            stObst3ViewMsg.vstrFileId.emplace_back(strFileId);
            stObst3ViewMsg.vstrFileType.emplace_back(strFileType);
        }
    }  

    if (stObst3ViewMsg.vstrFileType.size() != 3) {
        for (size_t i = 0; i < stObst3ViewMsg.vstrFileType.size(); i++) {
            ST_LOG_INFO("收到人机请求 %s .", stObst3ViewMsg.vstrFileType[i].c_str());
        } 
    }
    ST_LOG_INFO("Request: msgDomain:%d, msgType:%d, transId:%d.", 
        stObst3ViewMsg.stHmiMsg.msgDomain, stObst3ViewMsg.stHmiMsg.msgType, 
        stObst3ViewMsg.stHmiMsg.transId);
    ST_LOG_INFO("收到人机查询周边感知数据三视图请求。");

    Json::Value response;
    response["MsgDomain"] = stObst3ViewMsg.stHmiMsg.msgDomain;
    response["MsgType"]   = SOFTWARE_MNG_TYPE_NAV_UPLOAD_FILE_RSP;
    response["TransId"]   = stObst3ViewMsg.stHmiMsg.transId;

    ST_LOG_INFO("Response: msgDomain:%d, msgType:%d, transId:%d.",
                 stObst3ViewMsg.stHmiMsg.msgDomain, 
                 SOFTWARE_MNG_TYPE_NAV_UPLOAD_FILE_RSP, 
                 stObst3ViewMsg.stHmiMsg.transId);

    response["MsgContent"]["ErrCode"] = 0;
    pobjSoftware_->Send(response);

    stObst3ViewMsg.stHmiMsg.msgType = SOFTWARE_MNG_TYPE_NAV_UPLOAD_FILE;
    UptHmiObst3ViewReqMsg(stObst3ViewMsg);
    obst3dViewRspState_ = EN_PCPT_HMI_UNRSP;
}

void PCPT_HMI_C::UptHmiObst3ViewReqMsg(const PCPT_HMI_OBST_3VIEW_STRU& stHmiObst3ViewMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiObst3ViewMsg_ = stHmiObst3ViewMsg;  
}

void PCPT_HMI_C::GetHmiObst3ViewReqMsg(PCPT_HMI_OBST_3VIEW_STRU& stHmiObst3ViewMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiObst3ViewMsg = stHmiObst3ViewMsg_;
}

bool PCPT_HMI_C::IsUnRespondHmiObst3ViewReq(void)
{
    return obst3dViewRspState_ == EN_PCPT_HMI_UNRSP;
}

int  PCPT_HMI_C::RspHmiObst3ViewData(const PCPT_HMI_OBST_3VIEW_STRU& stHmiObst3ViewMsg)
{
    /* 获取点云 */
    PCPT_FUSED_DATA_C* pobjFusedData = PCPT_FUSED_DATA_C::GetInstance();
    PointCloudXYZ::Ptr pNoGroundPoints(new PointCloudXYZ);
    if (0 != pobjFusedData->GetPointCloud(pNoGroundPoints)) {
       ST_LOG_ERR("获取融合后的非地面点云失败.");
        return -1;
    }

    PointCloudXYZ::Ptr pRoiPoints(new PointCloudXYZ);
    for (size_t i = 0; i < pNoGroundPoints->size(); i++) {
        PointXYZ &p = pNoGroundPoints->points[i];
        if (    p.x > vRegion_[0] && p.x < vRegion_[1]
             && p.y > vRegion_[2] && p.y < vRegion_[3]
             && p.z > vRegion_[4] && p.z < vRegion_[5])
             pRoiPoints->push_back(p);
    }
    
    // static bool xxx = true;
    // if ((!xxx) || pRoiPoints->size() < 10) return -1;
    // xxx = false;
    
    /* 在视图中添加点云3视图 */
    std::vector<cv::Mat> v3dView;
    cv::Mat cvTop2DownImage;
    pobjObstTop2DownView_->ProjectionToImage(pRoiPoints,   0, cvTop2DownImage);
    v3dView.emplace_back(cvTop2DownImage);
        
    cv::Mat cvBack2FrontImage;
    pobjObstBack2FrontView_->ProjectionToImage(pRoiPoints, 0, cvBack2FrontImage);
    v3dView.emplace_back(cvBack2FrontImage);
    
    cv::Mat cvLeft2RightImage;
    pobjObstLeft2RightView_->ProjectionToImage(pRoiPoints, 0, cvLeft2RightImage);
    v3dView.emplace_back(cvLeft2RightImage);
        
    cv::imwrite("/home/saite/Desktop/top2down.jpg",   cvTop2DownImage);
    cv::imwrite("/home/saite/Desktop/back2front.jpg", cvBack2FrontImage);
    cv::imwrite("/home/saite/Desktop/left2right.jpg", cvLeft2RightImage);

    /* 转json发布 */
    Json::Value jsonUploadFileMsg;
    jsonUploadFileMsg["MsgDomain"] = stHmiObst3ViewMsg.stHmiMsg.msgDomain;
    jsonUploadFileMsg["MsgType"]   = stHmiObst3ViewMsg.stHmiMsg.msgType;
    jsonUploadFileMsg["TransId"]   = stHmiObst3ViewMsg.stHmiMsg.transId;
    
    Json::Value jsonFileInfoList;
    for (size_t i = 0; i < stHmiObst3ViewMsg.vstrFileId.size(); i++) {
        Json::Value jsonFileInfo;
        jsonFileInfo["FileId"]   = stHmiObst3ViewMsg.vstrFileId[i];
        jsonFileInfo["FileType"] = stHmiObst3ViewMsg.vstrFileType[i];
        jsonFileInfo["FileData"] = CvMatToBase64(v3dView[i]);
        jsonFileInfoList.append(jsonFileInfo);
    }
    jsonUploadFileMsg["MsgContent"]["FileInfoList"] = jsonFileInfoList;
    pobjSoftware_->Send(jsonUploadFileMsg);
    
    return 0;
}

void PCPT_HMI_C::UptHmiReqMsg(const int msgDomain, const int msgType, const int transId, PCPT_HMI_STRU& stHmiMsg)
{
    stHmiMsg.msgDomain = msgDomain;
    stHmiMsg.msgType   = msgType;
    stHmiMsg.transId   = transId;
}


/*
    洁净度业务:导航需要监听人机的：一个请求，两个响应回复（【图片】是否收到的回复，【检测区域值】是否收到的回复）。
*/
int PCPT_HMI_C::RegisterCleanClassCallback()
{
    // 该回调函数用于监听消息为“11”的话题（人机请求获取清洁质量检测结果）
    int rslt = pobjCommMng_->Reg(11, &PCPT_HMI_C::CleanClassRequestCallBack, this);
    if (rslt != 0) {
        ST_LOG_ERR("Failed to register <11> msg callback function.");
        return -1;
    }

    // 该回调函数用于监听消息为“20”的话题（人机收到图片文件数据的响应）
    rslt = pobjSoftware_->Reg(26, &PCPT_HMI_C::HmiImageResponseCallBack, this);
    if (rslt != 0) {
        ST_LOG_ERR("Failed to register <26> msg callback function.");
        return -1;
    }

    // 该回调函数用于监听消息为“14”的话题（人机收到清洁质量检测结果的响应）
    rslt = pobjCommMng_->Reg(14, &PCPT_HMI_C::CleanClassValueResponseCallBack, this);
    if (rslt != 0) {
        ST_LOG_ERR("Failed to register <14> msg callback function.");
        return -1;
    }
    return 0;
}

// 回调函数1：监听人机关于洁净度业务的请求
void PCPT_HMI_C::CleanClassRequestCallBack(JsonValueConstPtr& pobjJson)
{
    if (!pobjJson) {
        ST_LOG_ERR("Invalid callback parameter.");
        return;
    }

    ST_LOG_INFO("人机请求[获取清洁质量检测结果]，并开始对该请求进行回复\n");
    
    // 从人机获取到的消息
    Json::Value objJson;
    objJson = *pobjJson;

    // 回复人机请求的消息
    Json::Value response;
    response["MsgDomain"]  = 6;
    response["MsgType"]    = 12;
    response["TrandId"]    = objJson["TransId"].asInt();
    response["MsgContent"]["ErrCode"] = 0;
    
    
    // 保存请求信息,用于回复人机请求结果
    PCPT_HMI_STRU stHmiMsg;
    stHmiMsg.msgDomain = objJson["MsgDomain"].asInt();
    stHmiMsg.msgType   = objJson["MsgType"].asInt();
    stHmiMsg.transId   = objJson["TransId"].asInt();
    stHmiMsg.requestId = objJson["MsgContent"]["RequestId"].asString();
    UptHmiCleanClassReqMsg(stHmiMsg);

    if (0 != pobjCommMng_->Send(response)) {
        ST_LOG_ERR("发送[请求获取清洁质量检测结果响应]失败.");
    }
    else
    {
        ST_LOG_INFO("发送[请求获取清洁质量检测结果响应]成功");
        isResponseCleanClassRequest_ = true;
    }
}

// 回调函数2：监听人机是否收到图片
void PCPT_HMI_C::HmiImageResponseCallBack(JsonValueConstPtr& pobjJson)
{
    if (!pobjJson) {
        ST_LOG_ERR("Invalid callback parameter.");
        return;
    }
    ST_LOG_INFO("此响应表示人机接收到【图片】");
    // 从人机获取到的消息
    Json::Value objJson;
    objJson = *pobjJson;
}

// 回调函数3：监听人机是否收到相关数值
void PCPT_HMI_C::CleanClassValueResponseCallBack(JsonValueConstPtr& pobjJson)
{
    if (!pobjJson) {
        ST_LOG_ERR("Invalid callback parameter.");
        return;
    }

    ST_LOG_INFO("此响应表示人机接收到【检测区域值】");
    // 从人机获取到的消息
    Json::Value objJson;
    objJson = *pobjJson;
}

void PCPT_HMI_C::UptHmiCleanClassReqMsg(PCPT_HMI_STRU& stHmiMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiCleanClassMsg_ = stHmiMsg;
}

void PCPT_HMI_C::GetHmiCleanClassReqMsg(PCPT_HMI_STRU& stHmiMsg)
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    stHmiMsg = stHmiCleanClassMsg_;
}

void PCPT_HMI_C::Reset()
{
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    isResponseCleanClassRequest_ = false;
}

// 向人机发送图片
void PCPT_HMI_C::UploadCleanClassImg(cv::Mat img)
{
    ST_LOG_INFO("开始上报后置摄像头图像");
    /* 转json发布 */
    PCPT_HMI_STRU stHmiCleanClassMsg;
    GetHmiCleanClassReqMsg(stHmiCleanClassMsg);

    Json::Value jsonUploadFileMsg;
    jsonUploadFileMsg["MsgDomain"] = 4;
    jsonUploadFileMsg["MsgType"]   = 25;
    jsonUploadFileMsg["TransId"]   = stHmiCleanClassMsg.transId + 1;

    Json::Value jsonFileInfo;
    jsonFileInfo["FileId"]   = stHmiCleanClassMsg.requestId;
    jsonFileInfo["FileType"] = "cleaning_quality_inspection_diagram";
    jsonFileInfo["FileData"] = CvMatToBase64(img);

    Json::Value jsonFileInfoList;
    jsonFileInfoList.append(jsonFileInfo);
    jsonUploadFileMsg["MsgContent"]["FileInfoList"] = jsonFileInfoList;

    if (0 != pobjSoftware_->Send(jsonUploadFileMsg)) {
        ST_LOG_ERR("[后置摄像头图像]上报失败.");
    }
    else
    {
        ST_LOG_INFO("[后置摄像头图像]上报成功.");
    }
}

// 向人机发送检测区域像素
void PCPT_HMI_C::UploadCleanClassParam(cv::Point ptLeftUp,cv::Point ptRightUp,cv::Point ptRightDown,cv::Point ptLeftDown)
{
    ST_LOG_INFO("开始上报检测区域");
    PCPT_HMI_STRU stHmiCleanClassMsg;
    GetHmiCleanClassReqMsg(stHmiCleanClassMsg);

    Json::Value result;
    result["MsgDomain"]  = 6; // 消息域
    result["MsgType"]    = 13;// 消息类型
    result["TrandId"]    = stHmiCleanClassMsg.transId + 1;// 消息流水号
    result["MsgContent"]["RequestId"] = stHmiCleanClassMsg.requestId;// 请求ID
    
    Json::Value jsonROI;
    jsonROI["type"] = 1;
    Json::Value areaList;
        areaList[0][0] = ptLeftUp.x;
        areaList[0][1] = ptLeftUp.y;
        areaList[1][0] = ptRightUp.x;
        areaList[1][1] = ptRightUp.y;
        areaList[2][0] = ptRightDown.x;
        areaList[2][1] = ptRightDown.y;
        areaList[3][0] = ptLeftDown.x;
        areaList[3][1] = ptLeftDown.y;
    jsonROI["AreaList"] = areaList;

    Json::Value jsonROIList;
    jsonROIList.append(jsonROI);
    result["MsgContent"]["ImageInfo"]["ROIList"] = jsonROIList;
    
    if (0 != pobjCommMng_->Send(result)) {
        ST_LOG_ERR("[检测区域上报]失败.");
    }
    else
    {
        ST_LOG_INFO("[检测区域上报]成功.");
    }
}

