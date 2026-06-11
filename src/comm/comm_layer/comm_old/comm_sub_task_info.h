
#ifndef __COMM_SUB_TASK_INFO_H__
#define __COMM_SUB_TASK_INFO_H__

#include "comm_lane.h"
#include "road_network_data.h"


/**************************************************************************************
功能描述: 车道原始路径信息
修改记录:
**************************************************************************************/
struct LANE_ROUTE_SEG_INFO_STRU {
    unsigned int              pathId;
    unsigned int              pathIdFixed;
    unsigned int              roadNo;
    unsigned int              routeId;
    unsigned int              laneId;
    unsigned int              segId;
    unsigned int              routeStartIdx;
    unsigned int              routeSize;
    LANE_INFO_STRU      stLaneInfo;
    TASK_CTRL_INFO_STRU stTaskCtrlInfo;
};


/**************************************************************************************
功能描述: 车道
          指车辆运行的可行区域范围信息，不严格和实际道路车道信息吻合
修改记录:
**************************************************************************************/
struct ROAD_LANE_TASK_INFO_STRU {
    unsigned int                           isNarrow;
    bool                             isGoForward;
    unsigned int                           avoidType;
    bool                             isTurnAround;
    vector<LANE_ROUTE_SEG_INFO_STRU> vectRouteSegList;
    vector<COORDINATE_2D_STRU>       vectRoute;
};


/**************************************************************************************
功能描述: 覆盖任务信息
          指车辆运行的可行区域范围信息，不严格和实际道路车道信息吻合
修改记录:
**************************************************************************************/
struct AREA_COVERING_TASK_INFO_STRU {
    vector<ROAD_AREA_STRU>       vectArea;
    vector<COORDINATE_2D_STRU>   vertexList;
    vector<COORDINATE_2D_STRU>   vectRoute;
    vector<unsigned int>               vectCoverRoad;
};

/**************************************************************************************
功能描述: 自由空间
          指车辆运行的可行区域范围信息，不严格和实际道路车道信息吻合
修改记录:
**************************************************************************************/
struct OPEN_SPACE_TASK_INFO_STRU {
    OPEN_SPACE_TYPE_ENUM          type;
    COOR_2D_POSE_STRU             goal;
    COOR_2D_POSE_STRU             pose;
    std::vector<COORDINATE_2D_STRU>   goalArea;
    double                        liftBoxTravel;
    unsigned int                        QRCodeId;
    unsigned int                        portId;
    bool                          useReflectiveStrip;
    COORDINATE_3D_D_STRU           reflectiveStripPosLeft;
    COORDINATE_3D_D_STRU           reflectiveStripPosRight;
    LANE_INFO_STRU                stLaneInfo;
};


/**************************************************************************************
功能描述: 任务信息
修改记录:
**************************************************************************************/
struct TASK_INFO_STRU {
    unsigned int                       pathId;
    unsigned int                       lastLaneId;                    /* 多段时为最后一段的id */
    TASK_TYPE_ENUM               enTaskType;
    TASK_TYPE_ENUM               enHmiTaskType;                 /* 人机下发的任务类型 */
    LANE_TYPE_ENUM               enLaneType;
    TASK_TYPE_ENUM               enNextTaskType;                /* 下一个任务类型 */

    ROAD_LANE_TASK_INFO_STRU     stLaneTaskInfo;                /* 普通道路固定路线任务信息 */
    AREA_COVERING_TASK_INFO_STRU stAreaCoveringTaskInfo;        /* 覆盖区域固定路线任务信息 */
    OPEN_SPACE_TASK_INFO_STRU    stOpenSpaceTaskInfo;           /* 自由空间任务信息 */
};

/**************************************************************************************
功能描述: 任务列表
修改记录:
**************************************************************************************/
struct TASK_LIST_STRU {
    vector<TASK_INFO_STRU> vectTaskList;
};



#endif

