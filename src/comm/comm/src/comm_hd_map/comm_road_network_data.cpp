
#include "comm/comm_hd_map/comm_road_network_data.h"

/**************************************************************************************
功能描述: 获取道路元素的位姿信息
修改记录:
**************************************************************************************/
INT32 ROAD_INFO_WRAPPER_C::GetElePoseInfo(UINT32 eleType, UINT32 eleId, ROAD_ELE_POSE_INFO_STRU &stElePoseInfo) const
{
    stElePoseInfo.pathId = MAKE_PATHID(stRoadInfo_.no, stRoadInfo_.roadType, eleType, eleId);
    stElePoseInfo.enType = (ROAD_ELEMEMT_TYPE_ENUM)eleType;

    switch (eleType) {
        case ROAD_ELEMEMT_TYPE_ROUTE: {
            const ROAD_ROUTE_STRU *pstRoadRoute = stRoadInfo_.SearchRoute(eleId);
            if (pstRoadRoute == nullptr) {
                ST_LOG_ERR("Find Road(%u) Route(%u) Fail.", stRoadInfo_.no, eleId);
                return -1;
            }

            stElePoseInfo.vectPoint = pstRoadRoute->vectPoint;
            break;
        }

        case ROAD_ELEMEMT_TYPE_GOAL: {
            const ROAD_GOAL_STRU *pstRoadGoal = stRoadInfo_.SearchGoal(eleId);
            if (pstRoadGoal == nullptr) {
                ST_LOG_ERR("Find Road(%u) Goal(%u) Fail.", stRoadInfo_.no, eleId);
                return -1;
            }

            stElePoseInfo.stPose = pstRoadGoal->goal;
            break;
        }

        case ROAD_ELEMEMT_TYPE_EQUIPMENT: {
            const ROAD_POSE_STRU *pstRoadPose = stRoadInfo_.SearchPose(eleId);
            if (pstRoadPose == nullptr) {
                ST_LOG_ERR("Find Road(%u) Pose(%u) Fail.", stRoadInfo_.no, eleId);
                return -1;
            }

            stElePoseInfo.stPose = pstRoadPose->pose;
            break;
        }

        default: {
            ST_LOG_ERR("Unkown EleType(%u).", eleType);
            return -1;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取道路所有端口处的限制线端点
修改记录:
**************************************************************************************/
INT32 ROAD_INFO_WRAPPER_C::GetPortsLinesPoint(std::map<UINT32, std::map<UINT32, COORDINATE_2D_STRU>> &mapPortLinePoints) const
{
    for (const ROAD_LINE_STRU &stLine : stRoadInfo_.vectLine) {
        R_ASSERT(!stLine.vectPoint.empty(), -1);
        R_ASSERT_2(ROAD_PORT_STRU::IsOuterPort(stLine.stDir.startPortId), -1, stRoadInfo_.no, stLine.stDir.startPortId);
        R_ASSERT_2(ROAD_PORT_STRU::IsOuterPort(stLine.stDir.stopPortId),  -1, stRoadInfo_.no, stLine.stDir.stopPortId);
        R_ASSERT_3(stLine.stDir.startPortId != stLine.stDir.stopPortId,   -1, stRoadInfo_.no, stLine.stDir.startPortId, stLine.stDir.stopPortId);

        auto iter = mapPortLinePoints.find(stLine.stDir.startPortId);
        if (iter != mapPortLinePoints.end()) {
            auto iterOfLineOfStartPort = iter->second.find(stLine.id);
            R_ASSERT_2(iterOfLineOfStartPort == iter->second.end(), -1, stLine.stDir.startPortId, stLine.id);
        }

        iter = mapPortLinePoints.find(stLine.stDir.stopPortId);
        if (iter != mapPortLinePoints.end()) {
            auto iterOfLineOfStopPort = iter->second.find(stLine.id);
            R_ASSERT_2(iterOfLineOfStopPort == iter->second.end(), -1, stLine.stDir.stopPortId, stLine.id);
        }

        mapPortLinePoints[stLine.stDir.startPortId][stLine.id] = stLine.vectPoint.front();
        mapPortLinePoints[stLine.stDir.stopPortId][stLine.id]  = stLine.vectPoint.back();
    }

    return 0;
}

/**************************************************************************************
功能描述: 过滤指定端口处的道路边界线
修改记录:
**************************************************************************************/
INT32 ROAD_INFO_WRAPPER_C::FilterPortBorderLines(std::map<UINT32, COORDINATE_2D_STRU> &mapLineId2Point) const
{
    R_ASSERT_2(mapLineId2Point.size() >= 2, -1, stRoadInfo_.no, mapLineId2Point.size());
    if (mapLineId2Point.size() == 2) {
        return 0;
    }

    UINT32 borderLineId1 = UINT32_NULL;
    UINT32 borderLineId2 = UINT32_NULL;
    float  portWidthSqu  = -1.0;
    for (auto iter1 = mapLineId2Point.begin(); iter1 != mapLineId2Point.end(); ++iter1) {
        COORDINATE_2D_STRU &stLinePoint1 = iter1->second;

        for (auto iter2 = std::next(iter1, 1); iter2 != mapLineId2Point.end(); ++iter2) {
            COORDINATE_2D_STRU &stLinePoint2 = iter2->second;

            float pointsDistSqu = stLinePoint2.CalcDistSqu(stLinePoint1);
            if (pointsDistSqu > portWidthSqu) {
                borderLineId1 = iter1->first;
                borderLineId2 = iter2->first;
                portWidthSqu  = pointsDistSqu;
            }

        }
    }

    R_ASSERT_2(portWidthSqu > 0.0, -1, stRoadInfo_.no, UINT32(portWidthSqu * 100));

    for (auto iter = mapLineId2Point.begin(); iter != mapLineId2Point.end(); ) {
        if ((iter->first != borderLineId1) && (iter->first != borderLineId2)) {
            iter = mapLineId2Point.erase(iter);
        } else {
            ++iter;
        }
    }

    R_ASSERT_2(mapLineId2Point.size() == 2, -1, stRoadInfo_.no, mapLineId2Point.size());

    return 0;
}

/**************************************************************************************
功能描述: 获取道路的所有端口处的边界线列表
修改记录:
**************************************************************************************/
INT32 ROAD_INFO_WRAPPER_C::GetBorderLines(std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> &mapPort2LineList) const
{
    mapPort2LineList.clear();

    /* 边界限制规则:
       限制线只能连接外部端口
       同一个端口连接多条限制时，端口处的点必须基本在一条直线上，距离最大的两个点所在的限制线即为道路边界线
    */
    /* 单限制线场景，他一定就是边界线 */
    if (stRoadInfo_.vectLine.size() == 1) {
        const ROAD_LINE_STRU &stLine = stRoadInfo_.vectLine.front();
        R_ASSERT_2(stLine.stDir.startPortId == stLine.stDir.stopPortId, -1, stLine.stDir.startPortId, stLine.stDir.stopPortId);
        mapPort2LineList[stLine.stDir.startPortId].emplace_back(&stRoadInfo_.vectLine.front());
        return 0;
    }

    /* 多限制线场景，找同一个端口点距离最远的两条限制线为边界限制线。 */
    std::map<UINT32, std::map<UINT32, COORDINATE_2D_STRU>> mapPort2LinePoints;
    INT32 rsltOfGetPortsLinesPoint = GetPortsLinesPoint(mapPort2LinePoints);
    R_ASSERT_1(rsltOfGetPortsLinesPoint == 0, -1, stRoadInfo_.no);

    for (auto &objPair1 : mapPort2LinePoints) {
        UINT32                                portId          = objPair1.first;
        std::map<UINT32, COORDINATE_2D_STRU> &mapLineId2Point = objPair1.second;

        INT32  rsltOfFilterPortBorderLines = FilterPortBorderLines(mapLineId2Point);
        R_ASSERT_2(rsltOfFilterPortBorderLines == 0, -1, stRoadInfo_.no, portId);

        for (auto &objPair2 : mapLineId2Point) {
            UINT32 lineId = objPair2.first;

            const ROAD_LINE_STRU *pstLine = stRoadInfo_.SearchLine(lineId);
            R_ASSERT_2(pstLine != nullptr, -1, stRoadInfo_.no, lineId);

            mapPort2LineList[portId].emplace_back(pstLine);
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 将边界线连接起来
修改记录:
**************************************************************************************/
INT32 ROAD_INFO_WRAPPER_C::JoinBorderLines(std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> &mapPort2LineList, std::vector<COORDINATE_2D_STRU> &vectBorder) const
{
    vectBorder.clear();

    /* 如果只有一个端口，肯定只有一条限制线，不用连接。 */
    if (mapPort2LineList.size() == 1) {
        auto                                 iter     = mapPort2LineList.begin();
        std::vector<const ROAD_LINE_STRU *> &vectLine = iter->second;
        R_ASSERT_2(vectLine.size() == 1, -1, stRoadInfo_.no, vectLine.size());
        const ROAD_LINE_STRU *pstLine = vectLine.front();
        R_ASSERT_1(pstLine != nullptr, -1, stRoadInfo_.no);
        R_ASSERT_2(pstLine->vectPoint.size() >= 3, -1, stRoadInfo_.no, pstLine->vectPoint.size());
        vectBorder = pstLine->vectPoint;
        return 0;
    }

    /* 如果有多个端口，则从某端口开始顺序限制线连接下去，直到回到开始的端口。 */
    UINT32 beginPortId = UINT32_NULL;
    UINT32 preLineId   = UINT32_NULL;
    UINT32 nextPortId  = UINT32_NULL;

    while (!mapPort2LineList.empty()) {
        const ROAD_LINE_STRU *pstExtendLine = nullptr;
        UINT32                curPortId     = UINT32_NULL;
        if (nextPortId == UINT32_NULL) {
            /* 第一个扩展端口，从其第一条路径开始扩展，并且保留该端口 */
            auto iterOfCurPort = mapPort2LineList.begin();
            curPortId   = iterOfCurPort->first;
            beginPortId = curPortId;
            auto &vectLineListOfCurPort = iterOfCurPort->second;
            R_ASSERT_3(vectLineListOfCurPort.size() == 2, -1, stRoadInfo_.no, vectLineListOfCurPort.size(), curPortId);
            pstExtendLine = vectLineListOfCurPort.front();
            vectLineListOfCurPort.erase(vectLineListOfCurPort.begin());

        } else {
            /* 不是第一个扩展的端口，处理后删除该端口 */
            /* 如果回到了第一个端口，则已经形成了闭环，不再继续处理 */
            curPortId = nextPortId;
            auto iterOfCurPort = mapPort2LineList.find(curPortId);
            R_ASSERT_3(iterOfCurPort != mapPort2LineList.end(), -1, stRoadInfo_.no, curPortId, preLineId);
            if (curPortId == beginPortId) {
                mapPort2LineList.erase(iterOfCurPort);
                break;
            }

            /* 对于中间扩展端口，需要删除来路，然后从另一条路径扩展 */
            auto &vectLineListOfCurPort = iterOfCurPort->second;
            R_ASSERT_4(vectLineListOfCurPort.size() == 2, -1, stRoadInfo_.no, vectLineListOfCurPort.size(), curPortId, preLineId);
            for (auto iterOfLine = vectLineListOfCurPort.begin(); iterOfLine != vectLineListOfCurPort.end(); ++iterOfLine) {
                if (((const ROAD_LINE_STRU *)*iterOfLine)->id == preLineId) {
                    vectLineListOfCurPort.erase(iterOfLine);
                    break;
                }
            }

            R_ASSERT_4(vectLineListOfCurPort.size() == 1, -1, stRoadInfo_.no, vectLineListOfCurPort.size(), curPortId, preLineId);
            pstExtendLine = vectLineListOfCurPort.front();
            mapPort2LineList.erase(iterOfCurPort);
        }

        /* 将扩展路径按照端口扩展的方向的加入边界线末端 */
        R_ASSERT_3(pstExtendLine->vectPoint.size() >= 3, -1, stRoadInfo_.no, pstExtendLine->id, curPortId);
        if (pstExtendLine->stDir.startPortId == curPortId) {
            vectBorder.insert(vectBorder.end(), pstExtendLine->vectPoint.begin(), pstExtendLine->vectPoint.end());
            preLineId  = pstExtendLine->id;
            nextPortId = pstExtendLine->stDir.stopPortId;

        } else {
            R_ASSERT_4(pstExtendLine->stDir.stopPortId == curPortId, -1, stRoadInfo_.no, pstExtendLine->id, curPortId, pstExtendLine->stDir.stopPortId);
            std::reverse_copy(pstExtendLine->vectPoint.begin(), pstExtendLine->vectPoint.end(), std::back_inserter(vectBorder));
            preLineId  = pstExtendLine->id;
            nextPortId = pstExtendLine->stDir.startPortId;
        }
    }

    R_ASSERT_3(mapPort2LineList.empty(),  -1, stRoadInfo_.no, beginPortId, nextPortId);
    R_ASSERT_3(beginPortId == nextPortId, -1, stRoadInfo_.no, beginPortId, nextPortId);
    return 0;
}

/**************************************************************************************
功能描述: 将道路边界线按照顺序连接起来构成封闭的多边形，即为该道路的边界
修改记录:
**************************************************************************************/
INT32 ROAD_INFO_WRAPPER_C::GetBorder(std::vector<COORDINATE_2D_STRU> &vectBorder) const
{
    std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> mapPort2LineList;
    INT32 rsltOfGetBorderLines = GetBorderLines(mapPort2LineList);
    R_ASSERT_1(rsltOfGetBorderLines == 0, -1, stRoadInfo_.no);
    R_ASSERT_1(mapPort2LineList.size() > 0, -1, stRoadInfo_.no);

    INT32 rsltOfJoinBorderLines = JoinBorderLines(mapPort2LineList, vectBorder);
    R_ASSERT_1(rsltOfJoinBorderLines == 0, -1, stRoadInfo_.no);

    return 0;
}

/**************************************************************************************
功能描述: 重置对象
修改记录:
**************************************************************************************/
void ROAD_NETWORK_INFO_WRAPPER_C::Reset(const ROAD_NETWORK_INFO_STRU &stNetWorkInfo, BOOL isResetAll)
{
    mapRoadExInfo_.clear();
    isRoadAreasInited_ = false;

    for (const ROAD_INFO_STRU &stRoadInfo : stNetWorkInfo.vectRoad) {
        ROAD_EX_INFO_STRU stRoadExInfo;
        stRoadExInfo.pstRoadInfo = &stRoadInfo;

        mapRoadExInfo_[stRoadInfo.no] = stRoadExInfo;
    }

    if (isResetAll) {
        InitRoadAreas();
    }
}

/**************************************************************************************
功能描述: 获取道路元素位姿信息
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_INFO_WRAPPER_C::GetElePoseInfo(UINT32 pathId, ROAD_ELE_POSE_INFO_STRU &stElePoseInfo) const
{
    UINT32 roadNo = GET_ROAD_NO_OF_PATHID(pathId);
    auto iter = mapRoadExInfo_.find(roadNo);
    if (iter == mapRoadExInfo_.end()) {
        ST_LOG_ERR("Find Road(%u) Info Fail. PathId=%u.", roadNo, pathId);
        return -1;
    }

    UINT32 eleType = GET_ELE_TYPE_OF_PATHID(pathId);
    UINT32 eleId   = GET_ELE_ID_OF_PATHID(pathId);

    const ROAD_EX_INFO_STRU &stRoadExInfo = iter->second;
    const ROAD_INFO_STRU    &stRoadInfo   = *(stRoadExInfo.pstRoadInfo);
    INT32 rslt = ROAD_INFO_WRAPPER_C(stRoadInfo).GetElePoseInfo(eleType, eleId, stElePoseInfo);
    if (rslt != 0) {
        ST_LOG_ERR("ROAD_INFO_WRAPPER_C(RoadNo=%u).GetElePoseInfo(eleType=%u, eleId=%u) Fail. PathId=%u.", roadNo, eleType, eleId, pathId);
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 根据给定位置选择路段
修改记录:
**************************************************************************************/
void ROAD_NETWORK_INFO_WRAPPER_C::GetRoadsSelectedByPosConst(const COORDINATE_2D_STRU &stSelectPos, std::vector<const ROAD_INFO_STRU *> &vectRoadPtr) const
{
    for (const auto &objPair : mapRoadExInfo_) {
        const ROAD_EX_INFO_STRU &stRoadExInfo = objPair.second;

        if (!stRoadExInfo.objArea.IsIn(stSelectPos)) {
            continue;
        }

        vectRoadPtr.push_back(stRoadExInfo.pstRoadInfo);
    }
}

/**************************************************************************************
功能描述: 根据给定位置选择路段
修改记录:
**************************************************************************************/
void  ROAD_NETWORK_INFO_WRAPPER_C::GetRoadsSelectedByPos(const COORDINATE_2D_STRU &stSelectPos, std::vector<const ROAD_INFO_STRU *> &vectRoadPtr)
{
    if (!isRoadAreasInited_) {
        InitRoadAreas();
    }

    GetRoadsSelectedByPosConst(stSelectPos, vectRoadPtr);
}

/**************************************************************************************
功能描述: 初始化各个路段的区域信息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_INFO_WRAPPER_C::InitRoadAreas(void)
{
    for (auto &objPair : mapRoadExInfo_) {
        ROAD_EX_INFO_STRU &stRoadExInfo = objPair.second;

        for (const ROAD_LINE_STRU &stRoadLine : stRoadExInfo.pstRoadInfo->vectLine) {
            stRoadExInfo.objArea.IncludePoints(stRoadLine.vectPoint);
        }
    }

    isRoadAreasInited_ = true;
}







