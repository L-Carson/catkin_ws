#ifndef __ROAD_NETWORK_SHOW_H__
#define __ROAD_NETWORK_SHOW_H__

#include "comm/comm_hd_map/comm_road_network_data.h"
#include "comm/comm_hd_map/comm_dync_map_data.h"
#include "comm/comm_base.h"
#include "comm/comm_show.h"
#include "comm/comm_topic_name.h"

#include <list>
#include <vector>
#include <string>

#include <ros/ros.h>

enum ROAD_SHOW_OBJ_TYPE_ENUM
{
    ROAD_SHOW_OBJ_TYPE_LINE             = 0,              /* 限行线 */
    ROAD_SHOW_OBJ_TYPE_ROUTE            = 1,              /* 路线 */
    ROAD_SHOW_OBJ_TYPE_PORT             = 2,              /* 端口 */
    ROAD_SHOW_OBJ_TYPE_GOAL             = 3,              /* 目标位姿 */
    ROAD_SHOW_OBJ_TYPE_POSE             = 4,              /* 设备位姿 */
    ROAD_SHOW_OBJ_TYPE_OBST_LINE        = 5,              /* 障碍线 */
    ROAD_SHOW_OBJ_TYPE_AREA             = 6,              /* 区域 */
    ROAD_SHOW_OBJ_TYPE_STOP_LINE        = 7,              /* 停止线 */
    ROAD_SHOW_OBJ_TYPE_TRAFFIC_SPOT     = 8,              /* 红绿灯点*/
    ROAD_SHOW_OBJ_TYPE_GOAL_AREA        = 9,              /* 目标区域 */
};

enum NON_ROAD_SHOW_OBJ_TYPE_ENMU
{
    NON_ROAD_SHOW_OBJ_TYPE_RESTRICTED_AREA   = 0,       /* 禁行区 */
};

enum ROAD_SHOW_OBJ_INFO_TYPE_ENUM
{
    ROAD_SHOW_OBJ_INFO_TYPE_SELF        = 0,                /* 对象本身 */
    ROAD_SHOW_OBJ_INFO_TYPE_LABLEL      = 1,                /* 标签 */
    ROAD_SHOW_OBJ_INFO_TYPE_DIR         = 2,                /* 方向 */
    ROAD_SHOW_OBJ_INFO_TYPE_ENDPOINT    = 3,                /* 端点 */
};

/**************************************************************************************
功能描述: 路网信息显示
修改记录:
**************************************************************************************/
class ROAD_NETWORK_SHOW_C
{
public:
    ROAD_NETWORK_SHOW_C(ros::NodeHandle &nh) : objShow_(nh, TOPIC_NAME_SHOW_ROAD_NETWORK, 10000)
    {
        preSubNumOfCurShow_ = 0;
    }

    void PeriodUpt(void);
    void Show(const ROAD_NETWORK_INFO_STRU &stRoadNetwork);
    void ShowDyncMap(const DYNC_MAP_INFO_STRU &stDyncMap);

    void ShowRoadLine(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine);
    void UpdateChangedRouteByModifyMap(const std::vector<COORDINATE_2D_STRU> &stChangedLine);

private:
    ROAD_SHOW_C             objShow_;
    ROAD_NETWORK_INFO_STRU  stRoadNetwork_;
    DYNC_MAP_INFO_STRU      stDyncMap_;
    std::vector<std::vector<COORDINATE_2D_STRU>> stChangedLines_;
    UINT32                  preSubNumOfCurShow_;

    UINT32 GetObjId(UINT32 roadNo, ROAD_SHOW_OBJ_TYPE_ENUM enObjType, UINT32 objId, ROAD_SHOW_OBJ_INFO_TYPE_ENUM enObjInfoType) const;
    UINT32 GetObjIdOfNonRoad(NON_ROAD_SHOW_OBJ_TYPE_ENMU enObjType, UINT32 objId, ROAD_SHOW_OBJ_INFO_TYPE_ENUM enObjInfoType) const;
    UINT32 GetObjIdOfRoadLine(UINT32 roadNo, UINT32 lineId) const;
    UINT32 GetObjIdOfRoadLineLabel(UINT32 roadNo, UINT32 lineId) const;
    UINT32 GetObjIdOfRoadLineDir(UINT32 roadNo, UINT32 lineId) const;
    UINT32 GetObjIdOfRoadLineEndPoint(UINT32 roadNo, UINT32 lineId) const;
    UINT32 GetObjIdOfRoadObstLine(UINT32 roadNo, UINT32 obstLineId) const;
    UINT32 GetObjIdOfRoadStopLine(UINT32 roadNo, UINT32 stopLineId) const;
    UINT32 GetObjIdOfRoadArea(UINT32 roadNo, UINT32 areaId) const;
    UINT32 GetObjIdOfRoadRoute(UINT32 roadNo, UINT32 routeId) const;
    UINT32 GetObjIdOfRoadRouteLabel(UINT32 roadNo, UINT32 routeId) const;
    UINT32 GetObjIdOfRoadRouteDir(UINT32 roadNo, UINT32 routeId) const;
    UINT32 GetObjIdOfRoadRouteEndPoint(UINT32 roadNo, UINT32 routeId) const;
    UINT32 GetObjIdOfRoadPort(UINT32 roadNo, UINT32 portId) const;
    UINT32 GetObjIdOfPose(UINT32 roadNo, UINT32 poseId) const;
    UINT32 GetObjIdOfGoal(UINT32 roadNo, UINT32 goalId) const;
    UINT32 GetObjIdOfGoalArea(UINT32 roadNo, UINT32 goalAreaId) const;
    UINT32 GetObjIdOfTrafficLightSpot(UINT32 roadNo, UINT32 trafficSpotId) const;
    UINT32 GetObjIdOfRestrictedArea(UINT32 areaId) const;
    UINT32 GetObjIdOfRestrictedAreaLabel(UINT32 areaId) const;

    void   ShowRoadLineLabel(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine);
    void   ShowRoadLineDir(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine);
    void   ShowRoadLineEndPoint(UINT32 roadNo, const ROAD_LINE_STRU &stRoadLine);
    void   ShowRoadLineList(UINT32 roadNo, const vector<ROAD_LINE_STRU> &vectLineList);

    void   ShowRoadAreaList(UINT32 roadNo, const vector<ROAD_AREA_STRU> &vectArea);
    void   ShowRoadArea(UINT32 roadNo, const ROAD_AREA_STRU &stRoadArea);

    void   ShowObstLineList(UINT32 roadNo, const vector<ROAD_OBSTACLE_LINE_STRU> &vectLineList);
    void   ShowObstLine(UINT32 roadNo, const ROAD_OBSTACLE_LINE_STRU &stRoadObstLine);

    void   ShowStopLineList(UINT32 roadNo, const vector<ROAD_STOP_LINE_STRU> &vectLineList);
    void   ShowStopLine(UINT32 roadNo, const ROAD_STOP_LINE_STRU &stRoadStopLine);

    void   ShowRoadRoute(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute);
    void   ShowRoadRouteLabel(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute);
    void   ShowRoadRouteDir(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute);
    void   ShowRoadRouteEndPoint(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoadRoute);
    void   ShowRoadRouteList(UINT32 roadNo, const vector<ROAD_ROUTE_STRU> &vectRouteList);

    void   ShowRoadPort(UINT32 portId, const ROAD_INFO_STRU &stRoadInfo);
    void   ShowRoadPorts(const ROAD_INFO_STRU &stRoadInfo);

    void   ShowRoadPoses(const ROAD_INFO_STRU &stRoadInfo);
    void   ShowRoadGoals(const ROAD_INFO_STRU &stRoadInfo);
    void   ShowRoadGoalAreas(const ROAD_INFO_STRU &stRoadInfo);
    void   ShowRoadTrafficLightSpots(const ROAD_INFO_STRU &stRoadInfo);


    void   ShowRoad(const ROAD_INFO_STRU &stRoadInfo);
    void   ShowRoadNetwork(const ROAD_NETWORK_INFO_STRU &stRoadNetwork);

    void   ShowDyncMapCutOff(UINT32 id, const DYNC_MAP_RESTRICTED_AREA_CUTOFF_STRU &stCutOff);
    void   ShowDyncMapDetour(UINT32 id, const DYNC_MAP_RESTRICTED_AREA_DETOUR_STRU &stDetour);
    void   ShowDyncMapRestrictedArea(const DYNC_MAP_RESTRICTED_AREA_STRU &stRestrictedArea);
    void   ShowDyncMapInfo(const DYNC_MAP_INFO_STRU &stDyncMapInfo);
};


#endif

