#include "pcpt_det_dynamic/pcpt_det_dynamic.h"

/**************************************************************************************
功能描述: 发布msg
修改记录:
**************************************************************************************/
void FUSED_DYNAMIC_OBST_SENDER_C::SendMsg(const comm_msg::movTargetsArray &msgMovTargetsArray)
{
    if (pubMovTargets_.getNumSubscribers()) {
        pubMovTargets_.publish(msgMovTargetsArray);
    }
}

/**************************************************************************************
功能描述: 发布marker
修改记录:
**************************************************************************************/
void FUSED_DYNAMIC_OBST_SENDER_C::SendMarker(const std_msgs::Header& header, const vector<TRACK_MARKER_STRU>& vstTrackMarker)
{
    if (!pubMarker_.getNumSubscribers()) return;

    visualization_msgs::Marker linesT, linesP, points, arrow, text, box;
    visualization_msgs::MarkerArray markerArray;

    box.header  = header;
    box.ns      = "bbox";
    box.type    = visualization_msgs::Marker::CUBE;
    box.action  = visualization_msgs::Marker::ADD;
    box.color.r = 1.0f;
    box.color.g = 1.0f;
    box.color.b = 0.0f;
    box.color.a = 0.5;

    linesT.header             = header;
    linesT.ns                 = "tracking_line";
    linesT.action             = visualization_msgs::Marker::ADD;
    linesT.pose.orientation.w = 1.0;
    linesT.type               = visualization_msgs::Marker::LINE_LIST;
    linesT.scale.x            = 0.1;
    linesT.color.a            = 0.5;

    linesP.header             = header;
    linesP.ns                 = "pred_trajectory_line";
    linesP.action             = visualization_msgs::Marker::ADD;
    linesP.pose.orientation.w = 1.0;
    linesP.type               = visualization_msgs::Marker::LINE_LIST;
    linesP.scale.x            = 0.1;
    linesP.color.a            = 0.5;

    points.header             = header;
    points.ns                 = "pred_trajectory_point";
    points.action             = visualization_msgs::Marker::ADD;
    points.pose.orientation.w = 1.0;
    points.type               = visualization_msgs::Marker::POINTS;
    points.scale.x            = 0.2;
    points.scale.y            = 0.2;
    points.color.a            = 0.8;

    arrow.header              = header;
    arrow.ns                  = "pred_vel_arrow";
    arrow.action              = visualization_msgs::Marker::ADD;
    arrow.pose.orientation.w  = 1.0;
    arrow.type                = visualization_msgs::Marker::ARROW;
    arrow.scale.x             = 0.1;    //shaft diameter
    arrow.scale.y             = 0.3;    //head diameter
    arrow.scale.z             = 0.3;    //not zero, it specifies the head length
    arrow.color.a             = 1.0;

    text.header               = header;
    text.ns                   = "pred_vel_text";
    text.action               = visualization_msgs::Marker::ADD;
    text.pose.orientation.w   = 1.0;
    text.type                 = visualization_msgs::Marker::TEXT_VIEW_FACING;
    text.scale.z              = 0.8;
    text.color.b              = 1.0;
    text.color.a              = 1.0;

    list<uint32_t> lstId;
    lstId.clear();

    for (const TRACK_MARKER_STRU &stTrackMarker : vstTrackMarker) {
        POS_VEL_STRU stPosVel = stTrackMarker.stObjTrack.stPosVel;

        uint32_t id = stPosVel.objectID;
        lstId.push_back(id);

        float color   = id % 100 / 100.0;
        float color_r = 1.0;
        float color_g = color;
        float color_b = 1 - color;

        BOX_INFO_STRU stBoxInfo = stTrackMarker.stObjTrack.stBox;
        Eigen::Quaterniond quaternion;
        double yaw = stBoxInfo.stRotatedRect.angle;
        quaternion = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
                     Eigen::AngleAxisd(  0, Eigen::Vector3d::UnitY()) *
                     Eigen::AngleAxisd(  0, Eigen::Vector3d::UnitX());

        box.id                 = id + bboxInitId_;
        box.pose.position.x    = stBoxInfo.stRotatedRect.x;
        box.pose.position.y    = stBoxInfo.stRotatedRect.y;
        box.pose.position.z    = (stBoxInfo.minZ + stBoxInfo.maxZ) * 0.5;
        box.pose.orientation.x = quaternion.x();
        box.pose.orientation.y = quaternion.y();
        box.pose.orientation.z = quaternion.z();
        box.pose.orientation.w = quaternion.w();
        box.scale.x            = stBoxInfo.stRotatedRect.width;
        box.scale.y            = stBoxInfo.stRotatedRect.height;
        box.scale.z            = stBoxInfo.maxZ - stBoxInfo.minZ;

        linesT.id      = id + trackingLineInitId_;
        linesT.color.r = color_r;
        linesT.color.g = color_g;
        linesT.color.b = color_b;
        linesT.points.clear();

        linesP.id      = id + predTrajectoryLineInitId_;
        linesP.color.r = color_r;
        linesP.color.g = color_g;
        linesP.color.b = color_b;
        linesP.points.clear();

        points.id      = id + predTrajectoryPointInitId_;
        points.color.r = color_r;    //0.0 ~ 1.0
        points.color.g = color_g;
        points.color.b = color_b;
        points.points.clear();

        arrow.id      = id + predVelArrowInitId_;
        arrow.color.r = color_r;
        arrow.color.g = color_g;
        arrow.color.b = color_b;
        arrow.points.clear();

        text.id = id + predVelTextInitId_;
        text.pose.position.x = stPosVel.posX;
        text.pose.position.y = stPosVel.posY;

        geometry_msgs::Point point;

        //发布目标当前位置、速度
        point.x = stPosVel.posX;
        point.y = stPosVel.posY;
        points.points.push_back(point);
        arrow.points.push_back(point);

        point.x += stPosVel.velX;
        point.y += stPosVel.velY;
        arrow.points.push_back(point);

        ostringstream str;
        float vel = sqrt(pow(stPosVel.velX, 2) + pow(stPosVel.velY, 2));
        vel = ((float)((int)((vel + 0.005) * 100))) / 100;  //保留两位小数
        str << vel; str << "_"; str << stTrackMarker.stObjTrack.stBox.enClass;
        text.text = str.str();

        //发布历史轨迹
        if (stTrackMarker.stObjTrack.lstTrajectory.size() > 1) {
            list<TRAJECTORY_POINT_STRU>::const_reverse_iterator iter_r, iter_fr;   //使用快慢迭代器
            for (iter_r = stTrackMarker.stObjTrack.lstTrajectory.rbegin(); iter_r != stTrackMarker.stObjTrack.lstTrajectory.rend(); ++iter_r) {
                iter_fr = iter_r;
                if (++iter_fr != stTrackMarker.stObjTrack.lstTrajectory.rend()) {
                    point.x = iter_r->x;
                    point.y = iter_r->y;
                    linesT.points.push_back(point);

                    point.x = iter_fr->x;
                    point.y = iter_fr->y;
                    linesT.points.push_back(point);
                }
            }
        }

        //发布预测轨迹
        if (stTrackMarker.vstPredTrajectory.empty()) {
            linesP.action = visualization_msgs::Marker::DELETE;
        } else {
            linesP.action = visualization_msgs::Marker::ADD;
            vector<TRAJECTORY_POINT_STRU>::const_iterator iter, iterf;   //使用快慢迭代器
            for (iter = stTrackMarker.vstPredTrajectory.begin(); iter != stTrackMarker.vstPredTrajectory.end(); ++iter) {
                iterf = iter;
                if (iter == stTrackMarker.vstPredTrajectory.begin()) {
                    point.x = stPosVel.posX;
                    point.y = stPosVel.posY;
                    linesP.points.push_back(point);

                    point.x = iter->x;
                    point.y = iter->y;
                    linesP.points.push_back(point);
                }

                if (++iterf != stTrackMarker.vstPredTrajectory.end()) {
                    point.x = iter->x;
                    point.y = iter->y;
                    points.points.push_back(point);
                    linesP.points.push_back(point);

                    point.x = iterf->x;
                    point.y = iterf->y;
                    linesP.points.push_back(point);
                }

                if (iterf == stTrackMarker.vstPredTrajectory.end()) {
                    point.x = iter->x;
                    point.y = iter->y;
                    points.points.push_back(point);
                }
            }
        }
        markerArray.markers.push_back(box);
        markerArray.markers.push_back(linesT);
        markerArray.markers.push_back(linesP);
        markerArray.markers.push_back(points);
        markerArray.markers.push_back(arrow);
        markerArray.markers.push_back(text);
    }

    /* 清除无效的Marker在rviz上的显示 */
    for (auto &id : lstId) {
        list<uint32_t>::iterator iter = find(lstIdPre_.begin(), lstIdPre_.end(), id);
        if (iter != lstIdPre_.end())
            iter = lstIdPre_.erase(iter);
    }

    for (auto &id : lstIdPre_) {
        box.id = id + bboxInitId_;
        box.action = visualization_msgs::Marker::DELETE;
        markerArray.markers.push_back(box);

        linesT.id = id + trackingLineInitId_;
        linesT.action = visualization_msgs::Marker::DELETE;
        markerArray.markers.push_back(linesT);

        linesP.id = id + predTrajectoryLineInitId_;
        linesP.action = visualization_msgs::Marker::DELETE;
        markerArray.markers.push_back(linesP);

        points.id = id + predTrajectoryPointInitId_;
        points.action = visualization_msgs::Marker::DELETE;
        markerArray.markers.push_back(points);

        arrow.id = id + predVelArrowInitId_;
        arrow.action = visualization_msgs::Marker::DELETE;
        markerArray.markers.push_back(arrow);

        text.id = id + predVelTextInitId_;
        text.action = visualization_msgs::Marker::DELETE;
        markerArray.markers.push_back(text);
    }

    lstIdPre_ = lstId;

    pubMarker_.publish(markerArray);
}


INT32 DET_DYNAMIC_C::Init(void)
{
    PCPT_CFG_C* pobjCfg = PCPT_CFG_C::GetInstance();
    const CFG_PARAM_C& objCfgParam = pobjCfg->GetCfgParam();
    if (objCfgParam.IsSupportPcdNet()) {
        pobjPcdNetRec_ = new PCD_NET_MSG_RECEIVER_C(nh_);
        if (!pobjPcdNetRec_) {
            ST_LOG_ERR("New PCD_NET_MSG_RECEIVER_C Object Fail.");
            return -1;
        }
        isSuptDetDynamic_ = true;
        ST_LOG_INFO("New PCD_NET_MSG_RECEIVER_C Object Succ.");
    }

    thread_ = std::thread(&DET_DYNAMIC_C::PeriodProcess, this);
    pthread_setname_np(thread_.native_handle(), "pcpt_dynamic");

    ST_LOG_INFO("Init Success.");

    return 0;
}

void  DET_DYNAMIC_C::PeriodUpt(void)
{
    /* 更新点云网络检测数据 */
    UptPcdNetMsg();

    /* 更新车辆位姿数据 */
    UptCarPose();
}

void  DET_DYNAMIC_C::PeriodProcess(void)
{
    ros::Rate objLoopRate(PCPT_HZ_DET_DYNAMIC);
    while (ros::ok() && (!isClose_)) {
        std::vector<OBJECT_TRACK_STRU> vstValidObjTrack;
        if (isSuptDetDynamic_) {
            /* 更新定位数据 */
            COOR_2D_POSE_STRU stCarPoseByMap;
            INT32 rslt = GetCarPose(stCarPoseByMap);
            objAb_.SetAbnormal(rslt != 0, "Det_Dynamic_Obst");
            if (rslt) CONTINUE(10);
            objCarToMapTf_.SetTransRelationship(stCarPoseByMap);

            /* 获取Lidar点云聚类结果（base_link） */
            LIDAR_CLUSTER_STRU stDectObj;
            if (0 == GetDectObj(stDectObj)){
                vstObjTrack_.clear();
                if (0 == GetObjTrajectoryTrack(stDectObj, vstObjTrack_)) {
                    /* 输出、可视化检测结果（可视化实现有轨迹预测）*/
                    PubDynamicObst(stDectObj.timeStamp, vstObjTrack_, vstValidObjTrack);
                }
                //UpLoadPerceptionObjInfo(stDectObj.timeStamp, vstValidObjTrack);
            }
        } else { /* 在不支持动态目标检测的情况下要发送空消息，规控要做时间保护 */
            PubDynamicObst(RosStampNow(), std::vector<OBJECT_TRACK_STRU>{}, vstValidObjTrack);
        }

        objLoopRate.sleep();
    }
}

INT32 DET_DYNAMIC_C::GetDectObj(LIDAR_CLUSTER_STRU &stLidarCluster)
{
    if (0 != GetPcdNetMsg(stLidarCluster.vstBoxInfo)) return -1;

    FilterBoxByLabel(stLidarCluster.vstBoxInfo);

    if (stLidarCluster.vstBoxInfo.size() > 0) {
        static double time = 0;
        stLidarCluster.timeStamp = stLidarCluster.vstBoxInfo.front().timeStamp;
        if (stLidarCluster.timeStamp - time < 0.001) return -1;
        time = stLidarCluster.timeStamp;

    } else {
        stLidarCluster.timeStamp = RosStampNow();
    }

    objPubBoxMarker_.PubBoxMarker(stLidarCluster.vstBoxInfo, stLidarCluster.timeStamp);

    return 0;
}

INT32 DET_DYNAMIC_C::GetObjTrajectoryTrack(LIDAR_CLUSTER_STRU &stDectObj,
                                           std::vector<OBJECT_TRACK_STRU>& vstObjTrack)
{
    /* 变换Lidar数据到map坐标系 */
    TransLidarCluster2ByMap(stDectObj);

    /* 融合Lidar、Radar检测结果 */
    objTrajTracker_.GetObjTrackTraj(stDectObj, vstObjTrack);

    return 0;
}

void  DET_DYNAMIC_C::PubDynamicObst(const double stamp, const vector<OBJECT_TRACK_STRU>& vstObjTrack, vector<OBJECT_TRACK_STRU>& vstObjTrackValid)
{
    std_msgs::Header header;
    header.stamp = RosTime(stamp);
    header.frame_id = PCPT_FRAME_ID_MAP;

    comm_msg::movTargetsArray msgMovTargetsArray;
    msgMovTargetsArray.header = header;
    msgMovTargetsArray.movTargetsArray.reserve(vstObjTrack.size());

    vector<TRACK_MARKER_STRU> vstTrackMarker;
    vstTrackMarker.reserve(vstObjTrack.size());

    for (const OBJECT_TRACK_STRU &stObjTrack : vstObjTrack)
    {
        comm_msg::box box;
        box.label.value = stObjTrack.stBox.enClass;
        box.x           = stObjTrack.stPosVel.posX;
        box.y           = stObjTrack.stPosVel.posY;
        box.z           = 0.5 * (stObjTrack.stBox.maxZ - stObjTrack.stBox.minZ);
        box.width       = stObjTrack.stBox.stRotatedRect.width;
        box.length      = stObjTrack.stBox.stRotatedRect.height;
        box.height      = stObjTrack.stBox.maxZ - stObjTrack.stBox.minZ;
        box.yaw         = stObjTrack.stBox.stRotatedRect.angle;
        box.vel_x       = stObjTrack.stPosVel.velX;
        box.vel_y       = stObjTrack.stPosVel.velY;
        box.vel_z       = 0;

        comm_msg::movPred movPred;
        movPred.timeStamp = stObjTrack.stBox.timeStamp;
        movPred.box       = box;

        comm_msg::movTarget msgMovTarget;
        msgMovTarget.id = stObjTrack.stPosVel.objectID;
        msgMovTarget.movPredArray.push_back(movPred);

        /* 用于 rviz 可视化*/
        TRACK_MARKER_STRU stTrackMarker;
        stTrackMarker.stObjTrack = std::move(stObjTrack);

        /* 速度太小不进行跟踪 */
        if (hypot(stObjTrack.stPosVel.velX, stObjTrack.stPosVel.velY) >= 0.8)
        {
            vstObjTrackValid.emplace_back(stObjTrack);
            vector<TRAJECTORY_POINT_STRU> vstPredTrajectory; //预测轨迹点，预测轨迹不包含当前位置
            bool isTracking = objPcptTrack_.Track(stObjTrack, vstPredTrajectory) == 0;  //是否跟踪上

            /* 如果跟踪上，则存在预测轨迹 */
            if (isTracking) {
                for (const TRAJECTORY_POINT_STRU &stPredTrajectory : vstPredTrajectory) {
                    movPred.timeStamp = stPredTrajectory.timeStamp;
                    movPred.box.x     = stPredTrajectory.x;
                    movPred.box.y     = stPredTrajectory.y;
                    movPred.box.yaw   = stPredTrajectory.angle;
                    msgMovTarget.movPredArray.push_back(movPred);
                }
                stTrackMarker.vstPredTrajectory = std::move(vstPredTrajectory);
            }
        }
        msgMovTargetsArray.movTargetsArray.push_back(msgMovTarget);
        vstTrackMarker.push_back(stTrackMarker);
    }

    objFusedDynaObstSender_.SendMsg(msgMovTargetsArray);
    objFusedDynaObstSender_.SendMarker(header, vstTrackMarker);
}

void  DET_DYNAMIC_C::UpLoadPerceptionObjInfo(const double timeStamp, const vector<OBJECT_TRACK_STRU>& vstObjTrack)
{
    static int frameId = 0;
    FRAME_OBJ_INFO_STRU stFrameObjInfo;
    stFrameObjInfo.timestampGNSS = timeStamp * 1000;
    stFrameObjInfo.id = frameId++;
    for (const OBJECT_TRACK_STRU &stObjTrack : vstObjTrack) {
        OBJ_INFO_STRU stObjInfo;
        stObjInfo.id   = stObjTrack.Id();
        if (stObjTrack.stBox.enClass == TAG_CYCLIST) {
            stObjInfo.type = 2;
        } else if (stObjTrack.stBox.enClass == TAG_CAR) {
            stObjInfo.type = 6;
        } else {
           continue;
        }
        stObjInfo.length = stObjTrack.stBox.stRotatedRect.width;
        stObjInfo.width  = stObjTrack.stBox.stRotatedRect.height;
        stObjInfo.height = stObjTrack.stBox.maxZ - stObjTrack.stBox.minZ;

        COORDINATE_2D_STRU stPosByCar;
        objCarToMapTf_.TransFatherPosToSon(stObjTrack.stBox.stRotatedRect.CenterPoint(), stPosByCar);

        stObjInfo.longitudinalPos = stPosByCar.x;
        stObjInfo.transversalPos  = stPosByCar.y;
        stObjInfo.speed = sqrt(stObjTrack.VelX() * stObjTrack.VelX() + stObjTrack.VelY() * stObjTrack.VelY());
        stObjInfo.xDirection = atan2(stObjTrack.VelY(), stObjTrack.VelX());

        stFrameObjInfo.vstObjInfo.emplace_back(stObjInfo);
    }

    pobjPcptHmi_->UpLoadPerceptionObjInfo(stFrameObjInfo);
}

void  DET_DYNAMIC_C::UptCarPose(void)
{
    std::unique_lock<std::mutex> lock(objLocMutex_);
    pstCarPose_ = objLocInfoRec_.GetNormal();
}

INT32 DET_DYNAMIC_C::GetCarPose(COOR_2D_POSE_STRU &stCarPose)
{
    std::unique_lock<std::mutex> lock(objLocMutex_);
    if (!pstCarPose_) return -1;

    stCarPose.stPos.x = pstCarPose_->stPose.x();
    stCarPose.stPos.y = pstCarPose_->stPose.y();
    stCarPose.dirAngle = pstCarPose_->stPose.yaw();

    return 0;
}

void  DET_DYNAMIC_C::UptPcdNetMsg(void)
{
    if (!pobjPcdNetRec_) return;

    std::lock_guard<std::mutex> lock(objObjBoxInfoMutex_);
    pvstBoxesInfo_ = pobjPcdNetRec_->Get();
}

INT32 DET_DYNAMIC_C::GetPcdNetMsg(std::vector<BOX_INFO_STRU>& vstBoxesInfo)
{
    std::lock_guard<std::mutex> lock(objObjBoxInfoMutex_);
    if (!pvstBoxesInfo_) return -1;

    vstBoxesInfo = *pvstBoxesInfo_;
    pvstBoxesInfo_ = nullptr;

    return 0;
}

void  DET_DYNAMIC_C::TransLidarCluster2ByMap(LIDAR_CLUSTER_STRU &stLidarCluster)
{
    for (BOX_INFO_STRU &stBoxInfo : stLidarCluster.vstBoxInfo) {
        /* trans rotated rectangle by car to by map */
        COOR_2D_POSE_STRU stPoseByCar(stBoxInfo.x(), stBoxInfo.y(), stBoxInfo.angle()), stPoseByMap;
        objCarToMapTf_.TransSonPoseToFather(stPoseByCar, stPoseByMap);
        stBoxInfo.stRotatedRect.x     = stPoseByMap.x();
        stBoxInfo.stRotatedRect.y     = stPoseByMap.y();
        stBoxInfo.stRotatedRect.angle = stPoseByMap.dirAngle;
    }
}

void  DET_DYNAMIC_C::FilterBoxByLabel(std::vector<BOX_INFO_STRU>& vstBoxesInfo)
{
    for (auto iter = vstBoxesInfo.begin(); iter != vstBoxesInfo.end();) {
        const BOX_INFO_STRU& stBox = *iter;
        if (stBox.enClass == TAG_PEDESTRIAN) {
            iter = vstBoxesInfo.erase(iter);
        } else {
            ++iter;
        }
    }

    for (size_t i = 0; i < vstBoxesInfo.size(); i++) {
        if (vstBoxesInfo[i].enClass == TAG_BICYCLE)
            vstBoxesInfo[i].enClass = TAG_CYCLIST;
    }
}

