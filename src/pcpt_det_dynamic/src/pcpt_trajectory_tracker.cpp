#include "pcpt_det_dynamic/pcpt_trajectory_tracker.h"

INT32 TRAJECTORY_TRACKER_C::GetObjTrackTraj(const LIDAR_CLUSTER_STRU&       stDectObj,
                                            std::vector<OBJECT_TRACK_STRU>& vstObjTrack)
{
    /* 当前检测到的目标与已检测到的目标进行匹配关联 */
    LIDAR_CLUSTER_STRU stDectObjCurr = stDectObj;
    DectObjAssociation(stDectObjCurr);

    /* 更新每个目标运动状态估计器 */
    UptDectObjMotionEstimator(stDectObjCurr);

    if (0 != GetObjTrackTraj(stDectObjCurr.timeStamp, vstObjTrack)) {
        ST_LOG_ERR("Get Obj Track Traj Fail." );
        return -1;
    }

    return 0;
}

void  TRAJECTORY_TRACKER_C::DectObjAssociation(LIDAR_CLUSTER_STRU& stDectObjCurr)
{
    if (umObjMotionEstimator_.empty() || stDectObjCurr.vstBoxInfo.empty()) return;
    /* 当前帧检测到的目标信息 */
    double timeCurr = stDectObjCurr.timeStamp;
    std::vector<BOX_INFO_STRU>& vstBoxInfoCurr = stDectObjCurr.vstBoxInfo;

    /* 历史帧检测到的目标信息在当前时刻的预测 */
    std::vector<BOX_INFO_STRU> vstBoxInfoPrev;
    for (auto& iter : umObjMotionEstimator_) {
        auto pobjEstimator = iter.second;
        OBJECT_STATE_STRU stObjState;
        if (0 != pobjEstimator->GetObjState(timeCurr, stObjState)){
            continue;
        }
        vstBoxInfoPrev.emplace_back(stObjState.stBoxInfo);
    }

    /* 计算IOU代价矩阵 */
    std::vector<std::vector<double> > vvDistMatrix;
    CalcIouDistMatrix(vstBoxInfoCurr, vstBoxInfoPrev, vvDistMatrix);

    /* 基于匈牙利算法进行匹配 */
    std::vector<INT32> vAssignment;
    objHungarian_.Munkres(vvDistMatrix, vAssignment);

    /* row为当前检测的到的目标，col为上一帧雷达数据检测到的目标 */
    for (INT32 row = 0; row < (INT32)vvDistMatrix.size(); ++row) {
        INT32 col = vAssignment[row];
        if (col == -1) continue;

        /* 匈牙利算法是最大匹配，但不是最优匹配，当代价比较大时，认为没有匹配 */
        if (fabs(vvDistMatrix[row][col] - 1) <= 1e-5) continue;

        /* 如果匹配，则ID相同（基于此进行ID关联）*/
        vstBoxInfoCurr[row].boxID = vstBoxInfoPrev[col].boxID;
    }
}

void  TRAJECTORY_TRACKER_C::UptDectObjMotionEstimator(LIDAR_CLUSTER_STRU& stDectObjCurr)
{
    for (BOX_INFO_STRU &stCurr : stDectObjCurr.vstBoxInfo) {
        auto iter = umObjMotionEstimator_.find(stCurr.boxID);
        if (iter != umObjMotionEstimator_.end()) {
            MOTION_ESTIMATOR_C*& pobjEstimator = iter->second;
            pobjEstimator->Process(stCurr);
        } else {
            MOTION_ESTIMATOR_C* pobjEstimator = new MOTION_ESTIMATOR_C(stCurr);
            umObjMotionEstimator_.emplace(stCurr.boxID, pobjEstimator);
        }
    }

    /* 在旧运动状态估计器map中删除长时间未更新的估计器 */
    double timeCurr = stDectObjCurr.timeStamp;
    for (auto iter = umObjMotionEstimator_.begin(); iter != umObjMotionEstimator_.end();) {
        if (iter->second->IsLost(timeCurr)) {
            DelPtr(iter->second);
            iter = umObjMotionEstimator_.erase(iter);
        } else {
            ++iter;
        }
    }
}

INT32 TRAJECTORY_TRACKER_C::GetObjTrackTraj(const double time,
                                            std::vector<OBJECT_TRACK_STRU>& vstObjTrack)
{
    for (auto& pair : umObjMotionEstimator_) {
        if (!pair.second->IsLost(time)) {
            OBJECT_TRACK_STRU stObjTrack;
            if (0 != pair.second->GetObjState(time, stObjTrack)){
                continue;
            }
            vstObjTrack.emplace_back(stObjTrack);
        }
    }

    return 0;
}

void  TRAJECTORY_TRACKER_C::CalcIouDistMatrix(std::vector<BOX_INFO_STRU>&      vstBoxInfoCurr,
                                              std::vector<BOX_INFO_STRU>&      vstBoxInfoPrev,
                                              std::vector<std::vector<double>>& vvCostMatrix)
{
    //TODO 类别未考虑
    vvCostMatrix.resize(vstBoxInfoCurr.size());
    for (auto &vCostMatrix : vvCostMatrix)
         vCostMatrix.resize(vstBoxInfoPrev.size());

    for (UINT32 row = 0; row < vstBoxInfoCurr.size(); ++row) {
         for (UINT32 col = 0; col < vstBoxInfoPrev.size(); ++col) {
             BOX_INFO_STRU& stRowBox = vstBoxInfoCurr[row];
             BOX_INFO_STRU& stColBox = vstBoxInfoPrev[col];
             std::vector<COORDINATE_2D_STRU> polygonRow = stRowBox.stRotatedRect.RotatedRect2CornerPoints();
             std::vector<COORDINATE_2D_STRU> polygonCol = stColBox.stRotatedRect.RotatedRect2CornerPoints();
             double iou = objAnyPolygon_.IoUOfPolygon(polygonRow, polygonCol);
             double cost = 1 - iou;          //cost = 1 - iou
             vvCostMatrix[row][col] = cost;  //cost matrix
        }
   }
}

