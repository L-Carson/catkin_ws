
#ifndef __COMM_LANE_H__
#define __COMM_LANE_H__


#include "road_network_data.h"

/**************************************************************************************
功能描述: 车道类型
修改记录:
**************************************************************************************/
enum LANE_TYPE_ENUM {
    LANE_ALOGN_ROUTE = 0,
    LANE_POSE = 1,
    LANE_GOAL = 2,
    LANE_GOAL_AREA = 3,
};

/**************************************************************************************
功能描述: 车道类型
修改记录:
**************************************************************************************/
enum OPEN_SPACE_TYPE_ENUM {
    OPEN_SPACE_GARBAGE        = 0,    /* 对准任务的目标点，垃圾点 */
    OPEN_SPACE_CHARGE         = 1,    /* 对准任务的目标点，充电点或加水点 */
    OPEN_SPACE_PARKING        = 2,    /* 库房或临时停靠点 */
    OPEN_SPACE_ALIGNING       = 3,    /* 对准任务的起始点 */
    OPEN_SPACE_GARBAGE_NO_BOX = 4,    /* 对准任务的目标点，垃圾点(无垃圾筒) */
    OPEN_SPACE_THROW_RUBBISH  = 5,    /* 对准任务的目标点，扔垃圾点 */
    OPEN_SPACE_THROW_RUBBISH_NO_STATION = 6,       /*对准任务的目标点， 原地扔垃圾点*/
    OPEN_SPACE_START_NARROW   = 7,    /* 窄道起始点 */
    OPEN_SPACE_MAP_CHANGE     = 8,    /* 地图切换点 */
    OPEN_SPACE_EMPTY_RUBBISH_NO_STATION = 9,    /* 原地倒垃圾点 */
    OPEN_SPACE_TYPE_BUTT
};

/**************************************************************************************
功能描述: 道路车道信息
修改记录:
**************************************************************************************/
struct ROAD_LANE_INFO_STRU {
    unsigned int                     routeId = std::numeric_limits<unsigned int>::max();
    ROAD_DIR_STRU              stDir;
    bool                       isGoForward;
    unsigned int                     isNarrow;
    unsigned int                     avoidType;
    bool                       isLockStart;
    bool                       isMultiLane;
    bool                       isInCrossingArea;
    bool                       isReverseDriving;
    unsigned int                     beginIdx;
    unsigned int                     endIdx;
    ROUTE_ALONG_TYPE_ENUM      enRouteAlongType;
    CleanModeSt                clean_mode;
    std::vector<COORDINATE_2D_STRU> vectRouteLine;
};

/**************************************************************************************
功能描述: 自由空间信息
修改记录:
**************************************************************************************/
struct OPEN_SPACE_INFO_STRU {
    OPEN_SPACE_TYPE_ENUM          type;
    unsigned int                        goalId = std::numeric_limits<unsigned int>::max();
    COOR_2D_POSE_STRU             goal;
    unsigned int                        poseId = std::numeric_limits<unsigned int>::max();
    COOR_2D_POSE_STRU             pose;
    unsigned int                        goalAreaId = std::numeric_limits<unsigned int>::max();
    std::vector<COORDINATE_2D_STRU> goalArea;
    std::vector<unsigned int>           goalAreaPorts;
    unsigned int                        QRCodeId;
    unsigned int                        portId;
    bool                          useReflectiveStrip;
    COORDINATE_3D_D_STRU           reflectiveStripPosLeft;
    COORDINATE_3D_D_STRU           reflectiveStripPosRight;
    double                        liftBoxTravel;
    float                         goalReachLimitRange;
};

/**************************************************************************************
功能描述: 边信息，描述与其他lane的连接信息
修改记录:
**************************************************************************************/
struct EDGE_INFO_STRU {
    unsigned int connectPortId;       /* 本车道的连通端口 */
    unsigned int edgeLaneId;          /* 连通的车道 */
    unsigned int edgePortId;          /* 连通车道的连通端口 */
};

/**************************************************************************************
功能描述: 道路车道信息
修改记录:
**************************************************************************************/
struct LANE_INFO_STRU {
    unsigned int                                              laneId;             /* 内部索引 */
    unsigned int                                              pathId;             /* 有效值则表示该段为任务路径 */
    unsigned int                                              pathIdFixed;        /* pathId，不随这内部流程处理变化 */
    unsigned int                                              roadNo;
    ROAD_TYPE_ENUM                                      roadType;
    LANE_TYPE_ENUM                                      laneType;
    unsigned int                                              maxAccessCarNum;
    bool                                                isOnlyForTask;
    bool                                                isEasyTrafficJam;
    ROAD_SERVICE_STRU                                   serviceRoadStatus;

    std::vector<ROAD_AREA_STRU>                              vectArea;
    std::vector<ROAD_OBSTACLE_LINE_STRU>                     vectObstacleline;       /* 左右方位以车身朝向做为参考，及重叠的前进和后退路线，左右方位是一致的 */
    std::vector<ROAD_LINE_STRU>                              vectLimitLine;
    std::vector<ROAD_STOP_LINE_STRU>                         vectStopLine;           /* 停止线 */
    std::vector<ROAD_TRAFFIC_LIGHT_SPOT_STRU>                vectTrafficLightSpot;   /* 红绿灯点 */
    ROAD_LANE_INFO_STRU                                 roadLaneInfo;           /* 原始路径信息 */
    OPEN_SPACE_INFO_STRU                                openSpaceLaneInfo;
    RECT_AREA_C                                              objLaneRectArea;        /* 车道大概区域，用于快速筛选 */
    std::vector<EDGE_INFO_STRU>                         vecEdge;                /* 可以通向的LANE的索引 */
    std::vector<EDGE_INFO_STRU>                         vectFromEdge;           /* 上游LANE */

    bool                                                isBlockedByOnlyForTask; /* 当前车道是否因仅任务可用而被阻塞了 */

    bool                                                isConnectParking;       /* 是否连通了库房 */
    std::map<unsigned int, std::map<unsigned int, unsigned int>>          mapCutOffLineIdx;       /* 截断禁行区在限制线的截断点，同一个road的lane该信息有可能不一样。 */
    std::map<unsigned int, std::vector<COORDINATE_2D_STRU>>   mapDetourVertex;        /* 绕行禁行区，同一个road的lane该信息都是一样的。 */
    bool                                                isBlocked;              /* 当前车道是否被阻塞了，对于路径阻塞通行不阻塞连通其它车道，对于位姿阻塞与其它车道连通不阻塞自己。 */
    unsigned int                                              routeBlockIdx;          /* 路线阻塞点索引，laneType为route且isBlocked为true时有效               */

    inline bool IsOnlyForTask(void) const { return this->isOnlyForTask; }
    inline bool IsBlockedByOnlyForTask(void) const { return this->isBlockedByOnlyForTask; }
};


#endif

