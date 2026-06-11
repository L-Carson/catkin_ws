#ifndef __COMM_ROAD_NETWORK_FILE_H__
#define __COMM_ROAD_NETWORK_FILE_H__

#include "comm_road_network_data.h"

#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/loc_xml/tinyxml.h"

#include <list>
#include <vector>
#include <string>

/**************************************************************************************
功能描述: 点线文件
修改记录:
**************************************************************************************/
class ROAD_LINE_FILE_C
{
public:
    ROAD_LINE_FILE_C(BOOL isPrintInfo = true) : isPrintInfo_(isPrintInfo)
    {
    }

    INT32 Read(const std::string &strFilePath, vector<COORDINATE_2D_STRU> &vectLine) const;
    INT32 Write(const std::string &strFilePath, const vector<COORDINATE_2D_STRU> &vectLine) const;

private:
    const BOOL isPrintInfo_{true};
};

/**************************************************************************************
功能描述: 导航地图文件
修改记录:
**************************************************************************************/
class ROAD_NETWORK_FILE_C
{
public:
    ROAD_NETWORK_FILE_C(BOOL isPrintInfo = true) : isPrintInfo_(isPrintInfo)
    {
    }

    INT32 Load(ROAD_NETWORK_INFO_STRU &stRoadNetwork);
    INT32 Load(const std::string &strSceneDir, ROAD_NETWORK_INFO_STRU &stRoadNetwork);
    INT32 Save(const std::string &strSceneDir, const ROAD_NETWORK_INFO_STRU &stRoadNetwork);

private:
    const BOOL            isPrintInfo_{true};

    FILE_OPRT_C           objFileOprt_;
    ROAD_LINE_FILE_C      objLineFile_;
    WORK_SCENE_FILE_MNG_C objSceneFileMng_{""};     /* 传入空路径是为了不让其使用默认路径进行构造，避免调用基础库接口。 */

    /**************************************************************************************
    功能描述: 从文件中加载路网信息
    修改记录:
    **************************************************************************************/
    INT32 LoadRoadList(const TiXmlElement *pobjRoads, vector<ROAD_INFO_STRU> &vectRoad) const;
    INT32 LoadRoad(const TiXmlElement *pobjRoad, ROAD_INFO_STRU &stRoadInfo) const;

    INT32 LoadRoadPort(UINT32 roadNo, const TiXmlElement *pobjPort, ROAD_PORT_STRU &stPort) const;
    INT32 LoadRoadPortList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_PORT_STRU> &vectPort) const;
    void  GetValidPortList(const ROAD_INFO_STRU &stRoadInfo, const vector<ROAD_PORT_STRU> &vectOriginPort, vector<ROAD_PORT_STRU> &vectValidPort) const;

    INT32 LoadRoadRoute(UINT32 roadNo, const TiXmlElement *pobjRoute, ROAD_ROUTE_STRU &stRoute) const;
    INT32 LoadRoadRouteList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_ROUTE_STRU> &vectRoute) const;
    INT32 LoadRoadLine(UINT32 roadNo, const TiXmlElement *pobjLine, ROAD_LINE_STRU &stLine) const;
    INT32 LoadRoadLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_LINE_STRU> &vectLine) const;
    INT32 LoadObstacleLine(UINT32 roadNo, const TiXmlElement *pobjObstLine, ROAD_OBSTACLE_LINE_STRU &stObstLine) const;
    INT32 LoadObstacleLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_OBSTACLE_LINE_STRU> &vectObstLine) const;
    INT32 LoadRoadPose(UINT32 roadNo, const TiXmlElement *pobjPose, ROAD_POSE_STRU &stPose) const;
    INT32 LoadRoadPoses(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_POSE_STRU> &vectPose) const;
    INT32 LoadRoadGoal(UINT32 roadNo, const TiXmlElement *pobjGoal, ROAD_GOAL_STRU &stGoal) const;
    INT32 LoadRoadGoals(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_GOAL_STRU> &vectGoal) const;
    INT32 LoadRoadGoalArea(UINT32 roadNo, const TiXmlElement *pobjGoalArea, ROAD_GOAL_AREA_STRU &stGoalArea) const;
    INT32 LoadRoadGoalAreas(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_GOAL_AREA_STRU> &vectGoalArea) const;
    INT32 LoadRoadArea(UINT32 roadNo, const TiXmlElement *pobjArea, ROAD_AREA_STRU &stArea) const;
    INT32 LoadRoadAreas(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_AREA_STRU> &vectArea) const;
    INT32 LoadStopLine(UINT32 roadNo, const TiXmlElement *pobStopLine, ROAD_STOP_LINE_STRU &stStopLine) const;
    INT32 LoadStopLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_STOP_LINE_STRU> &vectStopLines) const;
    INT32 LoadLaneLine(UINT32 roadNo, const TiXmlElement *pobLaneLine, ROAD_LANE_LINE_STRU &stLaneLine) const;
    INT32 LoadLaneLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_LANE_LINE_STRU> &vectLaneLines) const;
    INT32 LoadTrafficLightSpot(UINT32 roadNo, const TiXmlElement *pobjTrafficLightSpot, ROAD_TRAFFIC_LIGHT_SPOT_STRU &stTrafficLightSpot) const;
    INT32 LoadTrafficLightSpots(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_TRAFFIC_LIGHT_SPOT_STRU> &vectTrafficLightSpot) const;
    INT32 LoadRoadType(const TiXmlElement *pobjMatch, ROAD_TYPE_ENUM &enRoadType) const;
    INT32 LoadCarType(const TiXmlElement *pobjMatch, ROAD_CARTYPE_ENUM &enCarType) const;
    INT32 LoadRoadAttribute(const TiXmlElement *pobjRoad, ROAD_INFO_STRU &stRoadInfo) const;
    INT32 LoadFromFile(ROAD_NETWORK_INFO_STRU &stRoadNetwork) const;
    INT32 LoadCrossing(const TiXmlElement *pobjCrossing, ROAD_CROSSING_STRU &stCrossing) const;
    INT32 LoadCrossingList(const UINT32 roadNo, const TiXmlElement *pobjCrossings, std::vector<ROAD_CROSSING_STRU> &vectCrossing) const; 
    INT32 LoadPoints(const TiXmlElement *pobjPoints, std::vector<COORDINATE_2D_STRU> &vectPoints) const;
    INT32 LoadAnchorPoints(const TiXmlElement *pobjAnchorPoints, std::vector<COORDINATE_2D_STRU> &vectAnchorPoints) const;

    /**************************************************************************************
    功能描述: 将路网信息写入文件
    修改记录:
    **************************************************************************************/
    void  SavePort(const ROAD_PORT_STRU &stPort, TiXmlElement *pobjPort) const;
    INT32 SavePortList(const vector<ROAD_PORT_STRU> &vectPort, TiXmlElement *pobjRoad) const;
    INT32 SaveLine(UINT32 roadNo, const ROAD_LINE_STRU &stLine, TiXmlElement *pobjLine) const;
    INT32 SaveLineList(UINT32 roadNo, const vector<ROAD_LINE_STRU> &vectLine, TiXmlElement *pobjRoad) const;
    void  SaveLimitLine(UINT32 limitLine, TiXmlElement *pobjLimitLine) const;
    INT32 SaveLimitLineList(const vector<UINT32> &vectLimitLine, TiXmlElement *pobjRoute) const;
    INT32 SaveRoute(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoute, TiXmlElement *pobjRoute) const;
    INT32 SaveRouteList(UINT32 roadNo, const vector<ROAD_ROUTE_STRU> &vectRoute, TiXmlElement *pobjRoad) const;
    INT32 SaveObstLine(UINT32 roadNo, const ROAD_OBSTACLE_LINE_STRU &stObstLine, TiXmlElement *pobjObstLine) const;
    INT32 SaveObstLineList(UINT32 roadNo, const vector<ROAD_OBSTACLE_LINE_STRU> &vectObstacleline, TiXmlElement *pobjRoad) const;
    void  SaveRouteId(UINT32 routeId, TiXmlElement *pobjRouteId) const;
    INT32 SaveRouteIdList(const vector<UINT32> &vectrouteId, TiXmlElement *pobjStopLine) const;
    INT32 SaveStopLine(UINT32 roadNo, const ROAD_STOP_LINE_STRU &stStopLine, TiXmlElement *pobjStopLine) const;
    INT32 SaveStopLineList(UINT32 roadNo, const vector<ROAD_STOP_LINE_STRU> &vectStopLine, TiXmlElement *pobjRoad) const;
    INT32 SaveLaneLine(UINT32 roadNo, const ROAD_LANE_LINE_STRU &stLaneLine, TiXmlElement *pobjLaneLine) const;
    INT32 SaveLaneLineList(UINT32 roadNo, const vector<ROAD_LANE_LINE_STRU> &vectLaneLine, TiXmlElement *pobjRoad) const;
    void  SaveGoal(const ROAD_GOAL_STRU &stGoal, TiXmlElement *pobjGoal) const;
    INT32 SaveGoalList(const vector<ROAD_GOAL_STRU> &vectGoal, TiXmlElement *pobjRoad) const;
    INT32 SaveGoalArea(const UINT32 &roadNo, const ROAD_GOAL_AREA_STRU &stGoalArea, TiXmlElement *pobjGoalArea) const;
    INT32 SaveGoalAreaList(const UINT32 &roadNo, const vector<ROAD_GOAL_AREA_STRU> &vectGoalArea, TiXmlElement *pobjRoad) const;
    void  SaveTrafficLightSpot(const ROAD_TRAFFIC_LIGHT_SPOT_STRU &stTrafficLightSpot, TiXmlElement *pobjTrafficLightSpot) const;
    INT32 SaveTrafficLightSpotList(const vector<ROAD_TRAFFIC_LIGHT_SPOT_STRU> &vectTrafficLightSpot, TiXmlElement *pobjRoad) const;
    INT32 SaveArea(UINT32 roadNo, const ROAD_AREA_STRU &stArea, TiXmlElement *pobjArea) const;
    INT32 SaveAreaList(UINT32 roadNo, const vector<ROAD_AREA_STRU> &vectArea, TiXmlElement *pobjRoad) const;
    INT32 SaveReflectiveStrip(const POINT_COORD_3D_STRU &reflectiveStripPosLeft, const POINT_COORD_3D_STRU &reflectiveStripPosRight,
            TiXmlElement *pobjEquipment) const;
    INT32 SaveEquipment(const ROAD_POSE_STRU &stEquipment, TiXmlElement *pobjEquipment) const;
    INT32 SaveEquipmentList(const vector<ROAD_POSE_STRU> &vectPose, TiXmlElement *pobjRoad) const;
    INT32 SaveRoad(const ROAD_INFO_STRU &stRoad, TiXmlElement *pobjRoad) const;
    INT32 SaveRoadList(const vector<ROAD_INFO_STRU> &vectRoad, TiXmlElement *pobjRoads) const;
    INT32 SaveToFile(const ROAD_NETWORK_INFO_STRU &stRoadNetwork) const;
    INT32 SaveCrossing(const ROAD_CROSSING_STRU &stCrossing, TiXmlElement *pobjCrossing) const;
    INT32 SaveCrossingList(UINT32 roadNo, const vector<ROAD_CROSSING_STRU> &vectCrossing, TiXmlElement *pobjRoad) const;
};

#endif

