
#include "comm/comm_hd_map/comm_road_network_show.h"

/**************************************************************************************
功能描述: 周期更新
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::PeriodUpt(void)
{
    if (stRoadNetwork_.vectRoad.empty() && stDyncMap_.stRestrictedAreaInfo.vectArea.empty()) {
        return;
    }

    UINT32 preSubNumOfCurShow = preSubNumOfCurShow_;
    UINT32 curSubNumOfCurShow = objShow_.GetSubscriberNum();
    preSubNumOfCurShow_ = curSubNumOfCurShow;
    if (curSubNumOfCurShow <= preSubNumOfCurShow) {
        return;
    }

    ShowRoadNetwork(stRoadNetwork_);
    ShowDyncMapInfo(stDyncMap_);
    for (UINT32 i = 0; i < stChangedLines_.size(); i++) {
        objShow_.ShowLine(0, stChangedLines_[i], 0.05, SHOW_COLOR_PURPLE);
    }
}

/**************************************************************************************
功能描述: 发送路网可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::Show(const ROAD_NETWORK_INFO_STRU &stRoadNetwork)
{
    stRoadNetwork_      = stRoadNetwork;
    stChangedLines_.clear();
    preSubNumOfCurShow_ = 0;
    objShow_.Clear();
    PeriodUpt();
}

/**************************************************************************************
功能描述: 发送动态地图可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowDyncMap(const DYNC_MAP_INFO_STRU &stDyncMap)
{
    stDyncMap_ = stDyncMap;

    preSubNumOfCurShow_ = 0;
    objShow_.Clear();
    PeriodUpt();
}

/**************************************************************************************
功能描述: 发送贴边修正后的路线可视化信息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::UpdateChangedRouteByModifyMap(const std::vector<COORDINATE_2D_STRU> &stChangedLine)
{
    stChangedLines_.emplace_back(stChangedLine);
}

/**************************************************************************************
功能描述: 获取对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjId(UINT32 roadNo, ROAD_SHOW_OBJ_TYPE_ENUM enObjType, UINT32 objId, ROAD_SHOW_OBJ_INFO_TYPE_ENUM enObjInfoType) const
{
    /*       xx                     xx              xx                    xx */
    /*     道路号                    对象类型            对象标号             对象内容类型 */
    return roadNo * 1000000 + UINT32(enObjType) * 10000 + objId * 100 + UINT32(enObjInfoType);
}

/**************************************************************************************
功能描述: 获取非道路相关对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfNonRoad(NON_ROAD_SHOW_OBJ_TYPE_ENMU enObjType, UINT32 objId, ROAD_SHOW_OBJ_INFO_TYPE_ENUM enObjInfoType) const
{
    /*                           xx                     xx                     xx */
    /*                        对象类型                  对象标号             对象内容类型 */
    return 999000000 + UINT32(enObjType) * 10000 + objId * 10 + UINT32(enObjInfoType);
}

/**************************************************************************************
功能描述: 获取障碍线可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadObstLine(UINT32 roadNo, UINT32 obstLineId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_OBST_LINE, obstLineId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取停止线可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadStopLine(UINT32 roadNo, UINT32 stopLineId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_STOP_LINE, stopLineId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取区域可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadArea(UINT32 roadNo, UINT32 areaId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_AREA, areaId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取目标点可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfGoal(UINT32 roadNo, UINT32 goalId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_GOAL, goalId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取红绿灯点可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfTrafficLightSpot(UINT32 roadNo, UINT32 trafficSpotId) const {
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_TRAFFIC_SPOT, trafficSpotId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取目标点可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfPose(UINT32 roadNo, UINT32 poseId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_POSE, poseId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取目标区域可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfGoalArea(UINT32 roadNo, UINT32 goalAreaId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_GOAL_AREA, goalAreaId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取道路限制线可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadLine(UINT32 roadNo, UINT32 lineId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_LINE, lineId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取道路限制线标签可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadLineLabel(UINT32 roadNo, UINT32 lineId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_LINE, lineId, ROAD_SHOW_OBJ_INFO_TYPE_LABLEL);
}

/**************************************************************************************
功能描述: 获取道路限制线方向箭头可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadLineDir(UINT32 roadNo, UINT32 lineId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_LINE, lineId, ROAD_SHOW_OBJ_INFO_TYPE_DIR);
}

/**************************************************************************************
功能描述: 获取道路限制线端点可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadLineEndPoint(UINT32 roadNo, UINT32 lineId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_LINE, lineId, ROAD_SHOW_OBJ_INFO_TYPE_ENDPOINT);
}

/**************************************************************************************
功能描述: 获取道路路线可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadRoute(UINT32 roadNo, UINT32 routeId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_ROUTE, routeId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

/**************************************************************************************
功能描述: 获取道路路线标签可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadRouteLabel(UINT32 roadNo, UINT32 routeId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_ROUTE, routeId, ROAD_SHOW_OBJ_INFO_TYPE_LABLEL);
}

/**************************************************************************************
功能描述: 获取道路路线方向箭头可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadRouteDir(UINT32 roadNo, UINT32 routeId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_ROUTE, routeId, ROAD_SHOW_OBJ_INFO_TYPE_DIR);
}

/**************************************************************************************
功能描述: 获取道路路线端点可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadRouteEndPoint(UINT32 roadNo, UINT32 routeId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_ROUTE, routeId, ROAD_SHOW_OBJ_INFO_TYPE_ENDPOINT);
}

/**************************************************************************************
功能描述: 获取道路端口可视化对象ID
修改记录:
**************************************************************************************/
UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRoadPort(UINT32 roadNo, UINT32 portId) const
{
    return GetObjId(roadNo, ROAD_SHOW_OBJ_TYPE_PORT, portId, ROAD_SHOW_OBJ_INFO_TYPE_LABLEL);
}

UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRestrictedArea(UINT32 areaId) const
{
    return GetObjIdOfNonRoad(NON_ROAD_SHOW_OBJ_TYPE_RESTRICTED_AREA, areaId, ROAD_SHOW_OBJ_INFO_TYPE_SELF);
}

UINT32 ROAD_NETWORK_SHOW_C::GetObjIdOfRestrictedAreaLabel(UINT32 areaId) const
{
    return GetObjIdOfNonRoad(NON_ROAD_SHOW_OBJ_TYPE_RESTRICTED_AREA, areaId, ROAD_SHOW_OBJ_INFO_TYPE_LABLEL);
}

/**************************************************************************************
功能描述: 发送道路限制线可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadLine(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine)
{
    UINT32 objId = GetObjIdOfRoadLine(roadNo, stRoadLine.id);

    objShow_.ShowLine(objId, stRoadLine.vectPoint, 0.1, SHOW_COLOR_YELLOW);
}

/**************************************************************************************
功能描述: 发送道路限制线标签可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadLineLabel(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine)
{
    R_ASSERT_1(stRoadLine.vectPoint.size() > 0, NOTHING, stRoadLine.vectPoint.size());

    UINT32                    objId     = GetObjIdOfRoadLineLabel(roadNo, stRoadLine.id);
    const COORDINATE_2D_STRU &stShowPos = stRoadLine.vectPoint[stRoadLine.vectPoint.size() / 3];
    string                    strLabel  = "R" + ToString(roadNo)
                                        + "_L" + ToString(stRoadLine.id)
                                        + "_D" + ToString(stRoadLine.stDir.startPortId)
                                        + "->" + ToString(stRoadLine.stDir.stopPortId);

    objShow_.ShowLabel(objId, stShowPos, strLabel, 0.8, SHOW_COLOR_YELLOW);
}

/**************************************************************************************
功能描述: 发送道路限制线方向可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadLineDir(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine)
{
    UINT32 lineSize = UINT32(stRoadLine.vectPoint.size());
    R_ASSERT_1(lineSize > 0, NOTHING, lineSize);

    UINT32 objId                         = GetObjIdOfRoadLineDir(roadNo, stRoadLine.id);
    UINT32 startPosIdx                   = lineSize / 2;
    UINT32 stopPosIdx                    = MIN(lineSize - 1, startPosIdx + 10);
    const COORDINATE_2D_STRU &stStartPos = stRoadLine.vectPoint[startPosIdx];
    const COORDINATE_2D_STRU &stStopPos  = stRoadLine.vectPoint[stopPosIdx];

    objShow_.ShowArrow(objId, stStartPos, stStopPos, 0.2, SHOW_COLOR_RED);
}

/**************************************************************************************
功能描述: 发送道路限制线端点可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadLineEndPoint(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine)
{
    UINT32 lineSize = UINT32(stRoadLine.vectPoint.size());
    R_ASSERT_1(lineSize > 0, NOTHING, lineSize);

    UINT32 objId = GetObjIdOfRoadLineEndPoint(roadNo, stRoadLine.id);

    vector<COORDINATE_2D_STRU> vectPos;
    vectPos.push_back(stRoadLine.vectPoint.front());
    if (lineSize > 1) {
        vectPos.push_back(stRoadLine.vectPoint.back());
    }

    objShow_.ShowCubeList(objId, vectPos, 0.2, SHOW_COLOR_RED);
}

/**************************************************************************************
功能描述: 发送道路障碍线可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowObstLineList(UINT32 roadNo, const vector<ROAD_OBSTACLE_LINE_STRU> &vectLineList)
{
    for (UINT32 loop = 0; loop < vectLineList.size(); ++loop) {
        const ROAD_OBSTACLE_LINE_STRU &stLine = vectLineList[loop];
        ShowObstLine(roadNo, stLine);
        ST_LOG_INFO("ShowObstLine no %u, id %u, size %u.", roadNo, stLine.id, (UINT32)stLine.vectPoint.size());
    }
}

void ROAD_NETWORK_SHOW_C::ShowObstLine(UINT32 roadNo, const ROAD_OBSTACLE_LINE_STRU &stRoadObstLine)
{
    UINT32 objId = GetObjIdOfRoadObstLine(roadNo, stRoadObstLine.id);

    objShow_.ShowLine(objId, stRoadObstLine.vectPoint, 0.05, SHOW_COLOR_BLACK);
}

/**************************************************************************************
功能描述: 发送停止线可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowStopLineList(UINT32 roadNo, const vector<ROAD_STOP_LINE_STRU> &vectLineList)
{
    for (UINT32 loop = 0; loop < vectLineList.size(); ++loop) {
        const ROAD_STOP_LINE_STRU &stLine = vectLineList[loop];
        ShowStopLine(roadNo, stLine);
        ST_LOG_INFO("ShowStopLine no %u, id %u, size %u.", roadNo, stLine.id, (UINT32)stLine.vectPoint.size());
    }
}

void ROAD_NETWORK_SHOW_C::ShowStopLine(UINT32 roadNo, const ROAD_STOP_LINE_STRU &stRoadStopLine)
{
    UINT32 objId = GetObjIdOfRoadStopLine(roadNo, stRoadStopLine.id);

    objShow_.ShowArrow(objId, stRoadStopLine.vectPoint.front(), stRoadStopLine.vectPoint.back(), 0.1, SHOW_COLOR_RED);
}

void ROAD_NETWORK_SHOW_C::ShowRoadAreaList(UINT32 roadNo, const vector<ROAD_AREA_STRU> &vectArea)
{
    for (UINT32 loop = 0; loop < vectArea.size(); ++loop) {
        const ROAD_AREA_STRU &stArea = vectArea[loop];
        ShowRoadArea(roadNo, stArea);
        ST_LOG_INFO("ShowRoadArea no %u, id %u, size %u.", roadNo, stArea.id, (UINT32)stArea.vertexs.size());
    }
}

void ROAD_NETWORK_SHOW_C::ShowRoadArea(UINT32 roadNo, const ROAD_AREA_STRU &stRoadArea)
{
    R_ASSERT_3(stRoadArea.vertexs.size() > 3, NOTHING, roadNo, stRoadArea.id, UINT32(stRoadArea.vertexs.size()));

    UINT32 objId = GetObjIdOfRoadArea(roadNo, stRoadArea.id);

    vector<COORDINATE_2D_STRU> vectAreaVertex = stRoadArea.vertexs;
    vectAreaVertex.push_back(stRoadArea.vertexs[0]);

    objShow_.ShowLine(objId, vectAreaVertex, 0.05, SHOW_COLOR_BLUE_1);
}

/**************************************************************************************
功能描述: 发送道路限制线可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadLineList(UINT32 roadNo, const vector<ROAD_LINE_STRU> &vectLineList)
{
    for (UINT32 loop = 0; loop < vectLineList.size(); ++loop) {
        const ROAD_LINE_STRU &stLine = vectLineList[loop];
        ShowRoadLine(roadNo, stLine);
        ShowRoadLineLabel(roadNo, stLine);
        ShowRoadLineDir(roadNo, stLine);
        ShowRoadLineEndPoint(roadNo, stLine);
    }
}

/**************************************************************************************
功能描述: 发送道路路线可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadRoute(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute)
{
    UINT32 objId = GetObjIdOfRoadRoute(roadNo, stRoadRoute.id);

    if (stRoadRoute.isGoForward) {
        objShow_.ShowLine(objId, stRoadRoute.vectPoint, 0.05, SHOW_COLOR_GREEN);
    } else {
        objShow_.ShowLine(objId, stRoadRoute.vectPoint, 0.05, SHOW_COLOR_BLUE);
    }
}

/**************************************************************************************
功能描述: 发送道路路线标签可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadRouteLabel(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute)
{
    R_ASSERT_1(stRoadRoute.vectPoint.size() > 0, NOTHING, stRoadRoute.vectPoint.size());

    UINT32                    objId     = GetObjIdOfRoadRouteLabel(roadNo, stRoadRoute.id);
    const COORDINATE_2D_STRU &stShowPos = stRoadRoute.vectPoint[stRoadRoute.vectPoint.size() * 2 / 3];
    string                    strLabel  = "R" + ToString(roadNo)
                                        + "_R" + ToString(stRoadRoute.id)
                                        + "_D" + ToString(stRoadRoute.stDir.startPortId)
                                        + "->" + ToString(stRoadRoute.stDir.stopPortId);

    objShow_.ShowLabel(objId, stShowPos, strLabel, 0.5, SHOW_COLOR_GREEN);
}

/**************************************************************************************
功能描述: 发送道路路线方向可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadRouteDir(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute)
{
    UINT32 routeSize = UINT32(stRoadRoute.vectPoint.size());
    R_ASSERT_1(routeSize > 0, NOTHING, routeSize);

    UINT32 objId                         = GetObjIdOfRoadRouteDir(roadNo, stRoadRoute.id);
    UINT32 startPosIdx                   = routeSize / 2;
    UINT32 stopPosIdx                    = MIN(routeSize - 1, startPosIdx + 10);
    const COORDINATE_2D_STRU &stStartPos = stRoadRoute.vectPoint[startPosIdx];
    const COORDINATE_2D_STRU &stStopPos  = stRoadRoute.vectPoint[stopPosIdx];

    objShow_.ShowArrow(objId, stStartPos, stStopPos, 0.1, SHOW_COLOR_RED);
}

/**************************************************************************************
功能描述: 发送道路路线端点可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadRouteEndPoint(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute)
{
    UINT32 routeSize = UINT32(stRoadRoute.vectPoint.size());
    R_ASSERT_1(routeSize > 0, NOTHING, routeSize);

    UINT32 objId = GetObjIdOfRoadRouteEndPoint(roadNo, stRoadRoute.id);

    vector<COORDINATE_2D_STRU> vectPos;
    vectPos.push_back(stRoadRoute.vectPoint.front());
    if (routeSize > 1) {
        vectPos.push_back(stRoadRoute.vectPoint.back());
    }

    objShow_.ShowCubeList(objId, vectPos, 0.2, SHOW_COLOR_RED);
}

/**************************************************************************************
功能描述: 发送道路路线可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadRouteList(UINT32 roadNo, const vector<ROAD_ROUTE_STRU> &vectRouteList)
{
    for (UINT32 loop = 0; loop < vectRouteList.size(); ++loop) {
        const ROAD_ROUTE_STRU &stRoute = vectRouteList[loop];
        ShowRoadRoute(roadNo, stRoute);
        ShowRoadRouteLabel(roadNo, stRoute);
        ShowRoadRouteDir(roadNo, stRoute);
        ShowRoadRouteEndPoint(roadNo, stRoute);
    }
}

/**************************************************************************************
功能描述: 发送道路端口可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadPort(UINT32 portId, const ROAD_INFO_STRU &stRoadInfo)
{
    /* 将端口信息显示在其中一条限制线的始端 */
    for (UINT32 loop = 0; loop < stRoadInfo.vectLine.size(); ++loop) {
        const ROAD_LINE_STRU &stLine = stRoadInfo.vectLine[loop];
        if (stLine.stDir.startPortId == portId) {
            UINT32                    objId   = GetObjIdOfRoadPort(stRoadInfo.no, portId);
            const COORDINATE_2D_STRU &stPos   = stLine.vectPoint.front();
            string                    strPort = "R" + ToString(stRoadInfo.no) + "_P" + ToString(portId);

            objShow_.ShowLabel(objId, stPos, strPort, 0.4, SHOW_COLOR_RED);
        }
    }
}

/**************************************************************************************
功能描述: 发送道路端口可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadPorts(const ROAD_INFO_STRU &stRoadInfo)
{
    for (UINT32 loop = 0; loop < stRoadInfo.vectPort.size(); ++loop) {
        const ROAD_PORT_STRU &stPort = stRoadInfo.vectPort[loop];

        ShowRoadPort(stPort.id, stRoadInfo);
    }
}

/**************************************************************************************
功能描述: 发送道路端口可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadPoses(const ROAD_INFO_STRU &stRoadInfo)
{
    for (const ROAD_POSE_STRU &stPose : stRoadInfo.vectPose) {
        UINT32 objId = GetObjIdOfPose(stRoadInfo.no, stPose.id);
        COORDINATE_2D_STRU start = stPose.pose.stPos;
        COORDINATE_2D_STRU end;
        double length = 0.5;
        end.x = start.x + cos(stPose.pose.dirAngle) * length;
        end.y = start.y + sin(stPose.pose.dirAngle) * length;

        objShow_.ShowArrow(objId, start, end,  0.1, SHOW_COLOR_BLUE);
    }
}

/**************************************************************************************
功能描述: 发送道路端口可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadGoals(const ROAD_INFO_STRU &stRoadInfo)
{
    for (const ROAD_GOAL_STRU &stGoal : stRoadInfo.vectGoal) {
        UINT32 objId = GetObjIdOfGoal(stRoadInfo.no, stGoal.id);
        COORDINATE_2D_STRU start = stGoal.goal.stPos;
        COORDINATE_2D_STRU end;
        double length = 0.5;
        end.x = start.x + cos(stGoal.goal.dirAngle) * length;
        end.y = start.y + sin(stGoal.goal.dirAngle) * length;

        objShow_.ShowArrow(objId, start, end,  0.1, SHOW_COLOR_YELLOW);
    }
}

/**************************************************************************************
功能描述: 发送道路目标区域可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadGoalAreas(const ROAD_INFO_STRU &stRoadInfo)
{
    for (const ROAD_GOAL_AREA_STRU &stGoalArea : stRoadInfo.vectGoalArea) {
        R_ASSERT_3(stGoalArea.vertexs.size() > 3, NOTHING, stRoadInfo.no, stGoalArea.id, UINT32(stGoalArea.vertexs.size()));
        UINT32 objId = GetObjIdOfGoalArea(stRoadInfo.no, stGoalArea.id);

        vector<COORDINATE_2D_STRU> vectAreaVertex = stGoalArea.vertexs;
        vectAreaVertex.push_back(stGoalArea.vertexs[0]);

        objShow_.ShowLine(objId, vectAreaVertex, 0.05, SHOW_COLOR_PURPLE);
    }
}

/**************************************************************************************
功能描述: 发送红绿灯点可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadTrafficLightSpots(const ROAD_INFO_STRU &stRoadInfo)
{
    std::vector<COORDINATE_2D_STRU> vectSpots;
    for (const ROAD_TRAFFIC_LIGHT_SPOT_STRU &stTrafficLightSpot : stRoadInfo.vectTrafficLightSpot) {
        vectSpots.emplace_back(stTrafficLightSpot.point);
        UINT32 objId = GetObjIdOfTrafficLightSpot(stRoadInfo.no, stTrafficLightSpot.id);
        std::string strSpot = "TrafficSpot" + std::to_string(stTrafficLightSpot.id);
        objShow_.ShowLabel(objId, stTrafficLightSpot.point, strSpot, 0.4, SHOW_COLOR_BLUE_1);
    }

    if (vectSpots.empty()) {
        return;
    }

    UINT32 objSpotId = GetObjIdOfTrafficLightSpot(stRoadInfo.no, 1);
    objShow_.ShowCubeList(objSpotId, vectSpots, 0.2, SHOW_COLOR_BLUE_1);
}

/**************************************************************************************
功能描述: 发送道路可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoad(const ROAD_INFO_STRU &stRoadInfo)
{
    ShowRoadLineList(stRoadInfo.no, stRoadInfo.vectLine);
    ShowRoadAreaList(stRoadInfo.no, stRoadInfo.vectArea);
    ShowObstLineList(stRoadInfo.no, stRoadInfo.vectObstacleline);
    ShowStopLineList(stRoadInfo.no, stRoadInfo.vectStopLine);
    ShowRoadRouteList(stRoadInfo.no, stRoadInfo.vectRoute);
    ShowRoadPorts(stRoadInfo);
    ShowRoadPoses(stRoadInfo);
    ShowRoadGoals(stRoadInfo);
    ShowRoadGoalAreas(stRoadInfo);
    ShowRoadTrafficLightSpots(stRoadInfo);
}

/**************************************************************************************
功能描述: 发送路网可视化消息
修改记录:
**************************************************************************************/
void ROAD_NETWORK_SHOW_C::ShowRoadNetwork(const ROAD_NETWORK_INFO_STRU &stRoadNetwork)
{
    for (UINT32 loop = 0; loop < stRoadNetwork.vectRoad.size(); ++loop) {
        ShowRoad(stRoadNetwork.vectRoad[loop]);
    }

    ST_LOG_INFO("Succ.");
}

void ROAD_NETWORK_SHOW_C::ShowDyncMapCutOff(UINT32 id, const DYNC_MAP_RESTRICTED_AREA_CUTOFF_STRU &stCutOff)
{
    UINT32 objIdOfArea = GetObjIdOfRestrictedArea(id);
    vector<COORDINATE_2D_STRU> vectCutOff;
    vectCutOff.push_back(stCutOff.stStartPoint);
    vectCutOff.push_back(stCutOff.stEndPoint);
    objShow_.ShowLine(objIdOfArea, vectCutOff, 0.3, SHOW_COLOR_RED);

    UINT32                    objIdOfAreaLabel = GetObjIdOfRestrictedAreaLabel(id);
    const COORDINATE_2D_STRU &stShowLabelPos   = stCutOff.stStartPoint;
    string                    strAreaLabel     = "Restricted_Area_" + ToString(id);
    objShow_.ShowLabel(objIdOfAreaLabel, stShowLabelPos, strAreaLabel, 0.8, SHOW_COLOR_YELLOW);
}

void ROAD_NETWORK_SHOW_C::ShowDyncMapDetour(UINT32 id, const DYNC_MAP_RESTRICTED_AREA_DETOUR_STRU &stDetour)
{
    R_ASSERT_2(!stDetour.vectVertex.empty(), NOTHING, id, stDetour.vectVertex.size());

    UINT32 objIdOfArea = GetObjIdOfRestrictedArea(id);
    vector<COORDINATE_2D_STRU> vectDetour;
    for (const COORDINATE_2D_STRU &stVertex : stDetour.vectVertex) {
        vectDetour.push_back(stVertex);
    }
    vectDetour.push_back(stDetour.vectVertex.front());
    objShow_.ShowLine(objIdOfArea, vectDetour, 0.3, SHOW_COLOR_RED);

    UINT32                    objIdOfAreaLabel = GetObjIdOfRestrictedAreaLabel(id);
    const COORDINATE_2D_STRU &stShowLabelPos   = stDetour.vectVertex.front();
    string                    strAreaLabel     = "Restricted_Area_" + ToString(id);
    objShow_.ShowLabel(objIdOfAreaLabel, stShowLabelPos, strAreaLabel, 0.8, SHOW_COLOR_YELLOW);
}

void ROAD_NETWORK_SHOW_C::ShowDyncMapRestrictedArea(const DYNC_MAP_RESTRICTED_AREA_STRU &stRestrictedArea)
{
    switch (stRestrictedArea.enType) {
        case DYNC_MAP_RESTRICTED_AREA_TYPE_CUTOFF:
            ShowDyncMapCutOff(stRestrictedArea.id, stRestrictedArea.stCutOff);
            break;

        case DYNC_MAP_RESTRICTED_AREA_TYPE_DETOUR:
            ShowDyncMapDetour(stRestrictedArea.id, stRestrictedArea.stDetour);
            break;

        default:
            ST_LOG_ERR("Unkown Type(%u).", stRestrictedArea.enType);
            return;
    }
}

void ROAD_NETWORK_SHOW_C::ShowDyncMapInfo(const DYNC_MAP_INFO_STRU &stDyncMapInfo)
{
    for (const DYNC_MAP_RESTRICTED_AREA_STRU &stArea : stDyncMapInfo.stRestrictedAreaInfo.vectArea) {
        ShowDyncMapRestrictedArea(stArea);
    }

    ST_LOG_INFO("Succ.");
}





