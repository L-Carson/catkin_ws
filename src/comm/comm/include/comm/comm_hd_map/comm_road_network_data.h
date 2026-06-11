#ifndef __ROAD_NETWORK_DATA_H__
#define __ROAD_NETWORK_DATA_H__

#include "comm/comm_base.h"

#include <list>
#include <vector>
#include <string>

/**************************************************************************************
功能描述: 路网错误信息类型 0-99路线错误，100-199路沿错误，200-299站点错误， 300-399设备点错误
修改记录:
**************************************************************************************/
enum ROAD_ERROR_MSG_ENUM {
    ROUTE_PORT_ERROR       = 0,         // 路线端口错误
    LINE_PORT_ERROR        = 100,       // 路沿端口错误
    GOAL_NO_ROUTE_ERROR    = 200,       // 站点无路线相连
    POSE_NO_ROUTE_ERROR    = 300,       // 设备无路线相连
};

/**************************************************************************************
功能描述: 路网告警信息类型 0-99路线告警，100-199路沿告警，200-299站点告警， 300-399设备点告警
修改记录:
**************************************************************************************/
enum ROAD_WARNING_MSG_ENUM {
    ROUTE_CONNECT_WRONG_WARNING   = 10000,       // 路线可能连接错误
    ROUTE_SHORT_WARNING           = 10001,       // 路线太短
    ROUTE_PAIR_ROAD_WRONG         = 10002,       // 路线所属路段可能错误
    LINE_PAIR_ROAD_WRONG          = 10100,       // 虚拟墙所属路段可能错误
    LINE_PORT_WRONG               = 10101,       // 虚拟墙端口可能错误
    GOAL_NO_BACK_ROUTE_WARNING    = 10200,       // 站点没有倒退路线相连
    GOAL_PAIR_ROAD_WRONG          = 10201,       // 站点所属路段可能错误
    POSE_NO_BACK_ROUTE_WARNING    = 10300,       // 设备没有倒退路线相连
    POSE_PAIR_ROAD_WRONG          = 10301,       // 设备所属路段可能错误
};


/**************************************************************************************
功能描述: 道路类型
修改记录:
**************************************************************************************/
enum ROAD_TYPE_ENUM {
    ROAD_LANE       = 0,
    ROAD_NON_LANE   = 1,
    ROAD_SERVICE    = 2,
    ROAD_TYPE_BUTT
};

/**************************************************************************************
功能描述: 车辆类型 
修改记录:
**************************************************************************************/
enum ROAD_CARTYPE_ENUM {
    ROAD_CARTYPE_320       = 0,
    ROAD_CARTYPE_330       = 1,
    ROAD_CARTYPE_520       = 2,
    ROAD_CARTYPE_350       = 3,
    ROAD_CARTYPE_BUTT  
};

/**************************************************************************************
功能描述: 线路在道路中位置
修改记录:
**************************************************************************************/
enum ROUTE_ALONG_TYPE_ENUM {
    ROUTE_ALONG_LEFT        = 0,    /* 靠左 */
    ROUTE_ALONG_RIGHT       = 1,    /* 靠右 */
    ROUTE_ALONG_MIDDLE      = 2,    /* 居中 */
    ROUTE_ALONG_TYPE_BUTT
};

/**************************************************************************************
功能描述: 对准目标点类型
修改记录:
**************************************************************************************/
enum ROAD_POSE_TYPE_ENUM {
    ROAD_POSE_GARBAGE        = 0,    /* 对准任务的目标点，垃圾点 */
    ROAD_POSE_CHARGE         = 1,    /* 对准任务的目标点，充电点或加水点 */
    ROAD_POSE_GARBAGE_NO_BOX = 2,    /* 对准任务的目标点，垃圾点(无垃圾筒) */
    ROAD_POSE_THROW_RUBBISH  = 3,    /* 对准任务的目标点，扔垃圾点 */
    ROAD_POSE_TYPE_BUTT
};

/**************************************************************************************
功能描述: 设备点自动/手动
修改记录:
**************************************************************************************/
enum ROAD_ATTRIBUTE_TYPE_ENUM {
    ROAD_ATTRIBUTE_MANUAL = 0,    /* 手动 */
    ROAD_ATTRIBUTE_AUTO   = 1,    /* 自动 */
    ROAD_ATTRIBUTE_TYPE_BUTT
};

/**************************************************************************************
功能描述: 停车点类型
修改记录:
**************************************************************************************/
enum ROAD_GOAL_TYPE_ENUM {
    ROAD_GOAL_PARKING   = 0,    /* 库房或临时停靠点 */
    ROAD_GOAL_ALIGNING  = 1,    /* 对准任务的起始点 */
	ROAD_GOAL_MAP_CHANGE = 2,      /* 地图切换点 */
    ROAD_GOAL_START_NARROW    = 100, /* 窄道起始点,内部新增点非路网元素 */
    ROAD_GOAL_TYPE_BUTT
};

/**************************************************************************************
功能描述: 目标区域类型
修改记录:
**************************************************************************************/
enum ROAD_GOAL_AREA_TYPE_ENUM {
    ROAD_GOAL_AREA_RUBBISH   = 0,    /* 扔垃圾区域 */
    ROAD_GOAL_AREA_TYPE_BUTT
};

/**************************************************************************************
功能描述: 区域 盖子配置选项
修改记录:
**************************************************************************************/
enum ROAD_AREA_COVER_TYPE_ENUM {
    ROAD_AREA_COVER_TYPE_DITCH  = 0,  // 排水沟盖子
    ROAD_AREA_COVER_TYPE_WELL   = 1,  // 井盖
    ROAD_AREA_COVER_TYPE_BUTT,
};

/**************************************************************************************
功能描述: 道路元素类型
          与地图文件规范中内容保持一致
修改记录:
**************************************************************************************/
enum ROAD_ELEMEMT_TYPE_ENUM
{
    ROAD_ELEMEMT_TYPE_ROUTE     = 0, /* 路径 */
    ROAD_ELEMEMT_TYPE_PORT      = 1, /* 端口 */
    ROAD_ELEMEMT_TYPE_LINE      = 2, /* 限制线 */
    ROAD_ELEMEMT_TYPE_EQUIPMENT = 3, /* 设备 */
    ROAD_ELEMEMT_TYPE_GOAL      = 4, /* 停车位 */
    ROAD_ELEMENT_TYPE_GOAL_AREA = 5, /* 目标区域 */
};

/**************************************************************************************
功能描述: 红绿灯类型
修改记录:
**************************************************************************************/
enum ROAD_TRAFFIC_LIGHT_TYPE_ENUM {
    ROAD_TRAFFIC_LIGHT_TYPE_NUMBER = 1,  // 读秒类型
    ROAD_TRAFFIC_LIGHT_TYPE_BLINK  = 2,  // 频闪类型
    ROAD_TRAFFIC_LIGHT_TYPE_FREQUENT_LIGHT_OFF  = 3,  // 高频亮又灭类型
    ROAD_TRAFFIC_LIGHT_TYPE_BUTT,
};

/**************************************************************************************
功能描述: 构造和解析pathid
        9  8  7 |    6     |    5  4     | 3  2  1
        RoadNo  | RoadType | ElementType | ElementId
修改记录:
**************************************************************************************/
#define MAKE_PATHID(road_, roadType_, eleType_, eleId_)     ((road_) * 1000000 + (roadType_) * 100000 + (eleType_) * 1000 + (eleId_))
#define MAKE_PATHID_OF_ROUTE(road_, roadType_, routeId_)    MAKE_PATHID(road_, roadType_, ROAD_ELEMEMT_TYPE_ROUTE,      routeId_)
#define MAKE_PATHID_OF_GOAL(road_, roadType_, goalId_)      MAKE_PATHID(road_, roadType_, ROAD_ELEMEMT_TYPE_GOAL,       goalId_)
#define MAKE_PATHID_OF_POSE(road_, roadType_, poseId_)      MAKE_PATHID(road_, roadType_, ROAD_ELEMEMT_TYPE_EQUIPMENT,  poseId_)
#define GET_ROAD_NO_OF_PATHID(pathId_)                      ((pathId_) / 1000000)
#define GET_ELE_TYPE_OF_PATHID(pathId_)                     ((pathId_) % 100000 / 1000)
#define GET_ELE_ID_OF_PATHID(pathId_)                       ((pathId_) % 1000)

/**************************************************************************************
功能描述: 检查告警信息
修改记录:
**************************************************************************************/
struct ROAD_WARNING_MSG_STRU {
    UINT32 id;                  /* 错误id */
    std::string reason;         /* 错误原因 */

    ROAD_WARNING_MSG_STRU(UINT32 id1, std::string reason1) : id(id1), reason(reason1){}
};

/**************************************************************************************
功能描述: 道路端口
修改记录:
**************************************************************************************/
struct ROAD_PORT_STRU {
    UINT32 id;                  /* 0~100外部端口 */

    static BOOL IsOuterPort(UINT32 portId)
    {
        return (portId <= 100);
    }

    static BOOL IsInnerPort(UINT32 portId)
    {
        return (portId > 100);
    }

    BOOL IsOuterPort(void) const
    {
        return (id <= 100);
    }

    BOOL IsInnerPort(void) const
    {
        return (id > 100);
    }
};

/**************************************************************************************
功能描述: 道路方向
修改记录:
**************************************************************************************/
struct ROAD_DIR_STRU {
    UINT32 startPortId;
    UINT32 stopPortId;

    BOOL IsDirSame(UINT32 startPortIdt, UINT32 stopPortIdt) const
    {
        return ((startPortId == startPortIdt) && (stopPortId == stopPortIdt));
    }

    BOOL IsDirSame(const ROAD_DIR_STRU &stDir) const
    {
        return ((startPortId == stDir.startPortId) && (stopPortId == stDir.stopPortId));
    }

    /**************************************************************************************
    功能描述: 检查方向是否互逆
    修改记录:
    **************************************************************************************/
    BOOL IsDirOpposite(UINT32 startPortIdt, UINT32 stopPortIdt) const
    {
        return (startPortId == stopPortIdt) && (stopPortId == startPortIdt) && (startPortId != stopPortId);
    }

    BOOL IsDirOpposite(const ROAD_DIR_STRU &stOtherDir) const
    {
        return (startPortId == stOtherDir.stopPortId) && (stopPortId == stOtherDir.startPortId) && (startPortId != stopPortId);
    }

    BOOL isDirSameOrOpposite(const ROAD_DIR_STRU &stDir) const
    {
        /* 方向严格相同或相反 */
        return (   ((startPortId == stDir.startPortId) && (stopPortId == stDir.stopPortId))
                || ((startPortId == stDir.stopPortId) && (stopPortId == stDir.startPortId) && (startPortId != stopPortId)));
    }

    void Reverse(void)
    {
        SWAP_DATA(startPortId, stopPortId);
    }
};

// 清洁模式
struct CleanModeSt
{
    bool is_blow;
    bool is_spray;
    bool is_stretch_brush;
};

/**************************************************************************************
功能描述: 道路路径
修改记录:
**************************************************************************************/
struct ROAD_ROUTE_STRU {
    UINT32                     id;
    vector<UINT32>             vecLimitLineId;
    vector<UINT32>             Types;	           // 路径类型  默认为0  0：普通路径， 1：补给路径
    ROAD_DIR_STRU              stDir;
    CleanModeSt                clean_mode;
    BOOL                       isGoForward;
    UINT32                     isNarrow;
    UINT32                     avoidType;
    BOOL                       isLockStart;
    BOOL                       isInCrossingArea;
    BOOL                       isReverseDriving;
    BOOL                       isIgnoreBodySideObst;
    ROUTE_ALONG_TYPE_ENUM      enRouteAlongType;
    vector<COORDINATE_2D_STRU> vectPoint;
    vector<COORDINATE_2D_STRU> vectAnchorPoint;
    vector<UINT32>             vectError;
    vector<UINT32>             vectWarning;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        for (size_t i = 0; i < vectPoint.size(); i++){
            vectPoint[i] = UptFunc(vectPoint[i]);
        }

        for (size_t i = 0; i < vectAnchorPoint.size(); i++){
            vectAnchorPoint[i] = UptFunc(vectAnchorPoint[i]);
        }
    }
};

/**************************************************************************************
功能描述: 道路限制线
修改记录:
**************************************************************************************/
struct ROAD_LINE_STRU {
    UINT32                     id;
    ROAD_DIR_STRU              stDir;
    vector<COORDINATE_2D_STRU> vectPoint;
    vector<COORDINATE_2D_STRU> vectAnchorPoint;
    vector<UINT32>             vectError;
    vector<UINT32>             vectWarning;

    void Reverse()
    {
        stDir.Reverse();
        reverse(vectPoint.begin(), vectPoint.end());
        reverse(vectAnchorPoint.begin(), vectAnchorPoint.end());
    }

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        for (size_t i = 0; i < vectPoint.size(); i++){
            vectPoint[i] = UptFunc(vectPoint[i]);
        }

        for (size_t i = 0; i < vectAnchorPoint.size(); i++){
            vectAnchorPoint[i] = UptFunc(vectAnchorPoint[i]);
        }
    }
};

/**************************************************************************************
功能描述: 自动站点辅助点距离配置选项
修改记录:
**************************************************************************************/
struct ROAD_EQUIPMENT_ITEM_STRU {
    float distance; // 最大行驶速度
};

struct ROAD_CHARGE_SITE_SN_STRU {
    std::string chargeSiteSN;
};

/**************************************************************************************
功能描述: 关联点
修改记录:
**************************************************************************************/
struct ROAD_POSE_STRU {
    UINT32                              id;
    ROAD_CARTYPE_ENUM                   carType;
    ROAD_POSE_TYPE_ENUM                 type;
    vector<UINT32>                      Types;
    ROAD_ATTRIBUTE_TYPE_ENUM            SiteAttribute;
    std::map<std::string, std::string>  SiteNames; // 新增的站点名称(多语言)
    ROAD_EQUIPMENT_ITEM_STRU            Distance;
    ROAD_CHARGE_SITE_SN_STRU            chargeSiteSNInfo;
    UINT32                              channelNumber;
    UINT32                              priority;
    UINT32                              PortId;
    COOR_2D_POSE_STRU                   pose;
    UINT32                              goalId;
    UINT32                              QRCodeId;
    double                              qRPosOffsetDis;        // 二维码位置基于中心左右偏移距离
    double                              liftBoxTravel;
    BOOL                                useReflectiveStrip;
    BOOL                                isRainProof;
    POINT_COORD_3D_STRU                 reflectiveStripPosLeft;
    POINT_COORD_3D_STRU                 reflectiveStripPosRight;
    vector<UINT32>                      vectError;
    vector<UINT32>                      vectWarning;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        COORDINATE_2D_STRU stPos;
        stPos.x = pose.stPos.x + 0.5 * cos(pose.dirAngle);
        stPos.y = pose.stPos.y + 0.5 * sin(pose.dirAngle);
        stPos      = UptFunc(stPos);
        pose.stPos = UptFunc(pose.stPos);
        pose.dirAngle = atan2(stPos.y - pose.stPos.y, stPos.x - pose.stPos.x);

        COORDINATE_2D_STRU stRefStripLeftPos = UptFunc(COORDINATE_2D_STRU(reflectiveStripPosLeft.x, reflectiveStripPosLeft.y));
        reflectiveStripPosLeft.x = stRefStripLeftPos.x;
        reflectiveStripPosLeft.y = stRefStripLeftPos.y;

        COORDINATE_2D_STRU stRefStripRightPos = UptFunc(COORDINATE_2D_STRU(reflectiveStripPosRight.x, reflectiveStripPosRight.y));
        reflectiveStripPosRight.x = stRefStripRightPos.x;
        reflectiveStripPosRight.y = stRefStripRightPos.y;
    }
};

struct ROAD_MAP_CHANGE_GOAL_STRU {
    float reachLimitRange;
    std::string changeMapName;
    std::string changeMapGoalPathId;
};

/**************************************************************************************
功能描述: 目标点
修改记录:
**************************************************************************************/
struct ROAD_GOAL_STRU {
    UINT32               id;
    ROAD_GOAL_TYPE_ENUM  type;
    vector<UINT32>       Types;
    std::map<std::string, std::string> SiteNames; // 新增的站点名称(多语言)
    COOR_2D_POSE_STRU    goal;
    UINT32               portId;
    UINT32               priority;
    ROAD_MAP_CHANGE_GOAL_STRU changeMapGoalInfo;
    vector<UINT32>       vectError;
    vector<UINT32>       vectWarning;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        COORDINATE_2D_STRU stPos;
        stPos.x = goal.stPos.x + 0.5 * cos(goal.dirAngle);
        stPos.y = goal.stPos.y + 0.5 * sin(goal.dirAngle);
        stPos      = UptFunc(stPos);
        goal.stPos = UptFunc(goal.stPos);
        goal.dirAngle = atan2(stPos.y - goal.stPos.y, stPos.x - goal.stPos.x);
    }
};

/**************************************************************************************
功能描述: 目标区域
修改记录:
**************************************************************************************/
struct ROAD_GOAL_AREA_STRU {
    UINT32               id;
    ROAD_GOAL_AREA_TYPE_ENUM  type;
    vector<COORDINATE_2D_STRU> vertexs;
    vector<UINT32>       portIds;
    vector<UINT32>       vectError;
    vector<UINT32>       vectWarning;
    std::map<std::string, std::string> SiteNames; // 新增的站点名称(多语言)

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        for(COORDINATE_2D_STRU &vertex : vertexs) {
            COORDINATE_2D_STRU stTransformPoint = UptFunc(COORDINATE_2D_STRU(vertex.x, vertex.y));
            vertex = stTransformPoint;
        }
    }
};

/**************************************************************************************
功能描述: 交叉路口
修改记录:
**************************************************************************************/
struct ROAD_CROSSING_STRU {
    unsigned int id; // 交叉口ID
    std::vector<COORDINATE_2D_STRU> points; // 交叉口的点列表
    std::vector<COORDINATE_2D_STRU> anchorPoints; // 交叉口的锚点列表

     // 更新方法
    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        // 更新点列表
        for (size_t i = 0; i < points.size(); i++) {
            points[i] = UptFunc(points[i]);
        }
        // 更新锚点列表
        for (size_t i = 0; i < anchorPoints.size(); i++) {
            anchorPoints[i] = UptFunc(anchorPoints[i]);
        }
    }   

};

/**************************************************************************************
功能描述: 道路限制线
修改记录:
**************************************************************************************/
struct ROAD_OBSTACLE_LINE_STRU {
    UINT32                     id;
    vector<COORDINATE_2D_STRU> vectPoint;
    vector<COORDINATE_2D_STRU> vectAnchorPoint;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        for (size_t i = 0; i < vectPoint.size(); i++){
            vectPoint[i] = UptFunc(vectPoint[i]);
        }
        for (size_t i = 0; i < vectAnchorPoint.size(); i++){
            vectAnchorPoint[i] = UptFunc(vectAnchorPoint[i]);
        }
    }
};

/**************************************************************************************
功能描述: 停车线
修改记录:
**************************************************************************************/
struct ROAD_STOP_LINE_STRU {
    UINT32                     id;
    UINT32                     type; // 0: 绿灯通行; 1: 红灯通行
    vector<COORDINATE_2D_STRU> vectPoint;
    vector<COORDINATE_2D_STRU> vectAnchorPoint;
    UINT32                     areaId;
    std::vector<UINT32>        vectRouteId;
    UINT32                     trafficLightId;
    double                     greenLightSlowMaxTime;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        for (size_t i = 0; i < vectPoint.size(); i++){
            vectPoint[i] = UptFunc(vectPoint[i]);
        }
        for (size_t i = 0; i < vectAnchorPoint.size(); i++){
            vectAnchorPoint[i] = UptFunc(vectAnchorPoint[i]);
        }
    }
};

/**************************************************************************************
功能描述: 红绿灯点
修改记录:
**************************************************************************************/
struct ROAD_TRAFFIC_LIGHT_SPOT_STRU {
    INT32                         groupId;
    UINT32                        id;
    ROAD_TRAFFIC_LIGHT_TYPE_ENUM  type;
    COORDINATE_2D_STRU            point;
    double                        crossingTime;
    double                        height;
    double                        rowLength;
    double                        colLength;
    double                        pixelBias;
    double                        boxBias;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        point   = UptFunc(point);
    }
};

/**************************************************************************************
功能描述: 车道线
修改记录:
**************************************************************************************/
struct ROAD_LANE_LINE_STRU {
    UINT32                     id;
    UINT32                     type; 
    vector<COORDINATE_2D_STRU> vectPoint;
    vector<COORDINATE_2D_STRU> vectAnchorPoint;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        for (size_t i = 0; i < vectPoint.size(); i++){
            vectPoint[i] = UptFunc(vectPoint[i]);
        }
        for (size_t i = 0; i < vectAnchorPoint.size(); i++){
            vectAnchorPoint[i] = UptFunc(vectAnchorPoint[i]);
        }
    }
};

/**************************************************************************************
功能描述: 区域速度配置选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_VEL_ITEM_STRU {
    float slowDownVel; // 最大行驶速度
    BOOL  isOnlyEffectForClean; // 仅对清扫有效
};

/**************************************************************************************
功能描述: 区域刷子配置选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_BRUSH_ITEM_STRU {
    BOOL isEffectInOutOfArea = false;//是否在区域外生效
    BOOL isCloseMainBrush;  // 是否关闭主刷
    BOOL isCloseSideBrush;  // 是否关闭边刷
};

/**************************************************************************************
功能描述: 区域喷水配置选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_WATER_ITEM_STRU {
    BOOL isCloseSprayWater;  // 区域内是否喷水
};

/**************************************************************************************
功能描述: 区域斜坡配置选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_SLOPE_ITEM_STRU {
    BOOL isCloseSlope;
};

/**************************************************************************************
功能描述: 区域 盖子配置选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_COVER_ITEM_STRU {
    ROAD_AREA_COVER_TYPE_ENUM type;     // 0表示排水沟盖子，1表示井盖
};

/**************************************************************************************
功能描述: 区域 门的配置选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_DOOR_ITEM_STRU {
    int type;   // 门的类型0表示库房门，1表示道闸门
    float doorHeight;
    string id;     // 门的唯一编号
};

/**************************************************************************************
功能描述: 区域 感知限高区
修改记录:
**************************************************************************************/
struct ROAD_AREA_HEIGHT_ITEM_STRU {
    float minHeight;
    float maxHeight;
};

/**************************************************************************************
功能描述: 区域 感知忽略障碍区
修改记录:
**************************************************************************************/
struct ROAD_AREA_OBST_ITEM_STRU {
    BOOL isIgnoreAllObst;
};

/**************************************************************************************
功能描述: 区域 感知限制范围选项
修改记录:
**************************************************************************************/
struct ROAD_AREA_RANGE_ITEM_STRU {
    float range;
};

/**************************************************************************************
功能描述: 区域 低矮绿植区
修改记录:
**************************************************************************************/
struct ROAD_AREA_TREELAWN_ITEM_STRU {
    float maxHeight;
};

/**************************************************************************************
功能描述: 区域 管制区
修改记录:
**************************************************************************************/
struct ROAD_AREA_TRAFFIC_CONTROL_ITEM_STRU {
    int passageNum;
    int passageTime;
    std::map<std::string, std::string> names;
};

/**************************************************************************************
功能描述: 区域 增加胶皮挡板开闭管制区
修改记录:
**************************************************************************************/
struct ROAD_AREA_RUBBER_CONTROL_ITEM_STRU {
    int gridBaffleLiftingTime;
    int gridBaffleLoweringTime;
    int gridBaffleDescentTime;
};

/**************************************************************************************
功能描述: 区域 AI(云端大模型)识别区域类型
修改记录:
**************************************************************************************/
struct ROAD_AREA_RECOGNIZE_ITEM_STRU {
    std::vector<int> types;  // AI检测类型，1表示鱼竿
};

/**************************************************************************************
功能描述: 区域 增加区域是否禁止使能超声波
修改记录:
**************************************************************************************/
struct ROAD_AREA_UTRAL_ITEM_STRU {
    bool isEnableNoUtral;  // 是否禁止使能超声波
};

/**************************************************************************************
功能描述: 区域 增加区域是否禁止倒车
修改记录:
**************************************************************************************/
struct ROAD_AREA_NO_REVERSE_ITEM_STRU {
    bool isEnableNoReverse;  // 增加区域是否禁止倒车
};

/**************************************************************************************
功能描述: 特殊区域
修改记录:
**************************************************************************************/
struct ROAD_AREA_STRU {
    UINT32                          roadNo;
    UINT32                          id;
    UINT32                          type;
    vector<COORDINATE_2D_STRU>      vertexs;

    BOOL                            isEnableVelItem{false};
    BOOL                            isEnableBrushItem{false};
    BOOL                            isEnableWaterItem{false};
    BOOL                            isEnableCoverItem{false};
    BOOL                            isEnableDoorItem{false};
    BOOL                            isEnableCrossingItem{false};
    BOOL                            isEnableSlopeItem{false};
    BOOL                            isEnableHeightItem{false};
    BOOL                            isEnableObstItem{false};
    BOOL                            isEnableRangeItem{false};
    BOOL                            isEnableTreelawnItem{false};
    BOOL                            isEnableStopExistObst{false};
    BOOL                            isEnableRubberControlItem{false};
    BOOL                            isEnableTrafficControlItem{false};
    BOOL                            isEnableEntanglementItem{false};
    BOOL                            isEnableRoadStakeItem{false};
    BOOL                            isEnableRecognizeItem{false};
    BOOL                            isEnableNoUtralItem{false};
    BOOL                            isEnableNoReverseItem{false};
    BOOL                            isEnableCloseObstacle{true};
    BOOL                            isEnableNotAllowedAutoNavigation{false};
    ROAD_AREA_VEL_ITEM_STRU         velItem;
    ROAD_AREA_BRUSH_ITEM_STRU       brushItem;
    ROAD_AREA_WATER_ITEM_STRU       waterItem;
    ROAD_AREA_SLOPE_ITEM_STRU       slopeItem;
    ROAD_AREA_COVER_ITEM_STRU       coverItem;
    ROAD_AREA_DOOR_ITEM_STRU        doorItem;
    ROAD_AREA_HEIGHT_ITEM_STRU      heightItem;
    ROAD_AREA_OBST_ITEM_STRU        obstItem;
    ROAD_AREA_RANGE_ITEM_STRU       rangeItem;
    ROAD_AREA_TREELAWN_ITEM_STRU    treelawnItem;
    ROAD_AREA_TRAFFIC_CONTROL_ITEM_STRU trafficControlItem;
    ROAD_AREA_RUBBER_CONTROL_ITEM_STRU  rubberControlItem;
    ROAD_AREA_RECOGNIZE_ITEM_STRU  recognizeItem;
    ROAD_AREA_UTRAL_ITEM_STRU   utralItem;
    ROAD_AREA_NO_REVERSE_ITEM_STRU reverseItem;

    // 新增的作业要求字段
    struct JobRequirements {
        UINT32 edgeBrushOpen;                 // 边刷开启 1展开 2收缩 3下放 4抬起
        UINT32 mainBrushOpen;                 // 主刷开启 3下放 4抬起
        BOOL   sprinklerOpening;              // 洒水开启
        UINT32 leftWindOn;                    // 左吹风开启 1，2，3，4级
        UINT32 rightWindOn;                   // 右吹风开启 1，2，3，4级
        UINT32 linkageWarehouseDoor;          // 联动库房门
        BOOL   maskingPerception;             // 屏蔽感知
        double maximumSpeed;                  // 最大速度
        double objectsObstructHeight;         // 物体蔽障高度
        BOOL   shieldNegativeHeight;          // 屏蔽负高（针对镂空排水渠盖）
        UINT32 shieldingHighAltitudeObjects;  // 屏蔽高空物
    } jobRequirements;                        // 实例化 JobRequirements 结构体   

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        for(COORDINATE_2D_STRU &vertex : vertexs) {
            COORDINATE_2D_STRU stTransformPoint = UptFunc(COORDINATE_2D_STRU(vertex.x, vertex.y));
            vertex = stTransformPoint;
        }
    }
};

/**************************************************************************************
功能描述: 道路分段线
          一般垂直于道路
修改记录:
**************************************************************************************/
struct ROAD_SEG_LINE_STRU {
    COORDINATE_2D_STRU stLeftPoint;
    COORDINATE_2D_STRU stRightPoint;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        stLeftPoint  = UptFunc(stLeftPoint);
        stRightPoint = UptFunc(stRightPoint);
    }
};

/**************************************************************************************
功能描述: 减速带
修改记录:
**************************************************************************************/
struct ROAD_SPEED_BUMPS_STRU {
    ROAD_SEG_LINE_STRU stStartLine;
    ROAD_SEG_LINE_STRU stEndLine;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        stStartLine.Upt(UptFunc);
        stEndLine.Upt(UptFunc);
    }
};

/**************************************************************************************
功能描述: 人行横道
修改记录:
**************************************************************************************/
struct ROAD_ZEBRA_CROSS_STRU {
    ROAD_SEG_LINE_STRU stStartLine;
    ROAD_SEG_LINE_STRU stEndLine;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        stStartLine.Upt(UptFunc);
        stEndLine.Upt(UptFunc);
    }
};

/**************************************************************************************
功能描述: 交通灯
修改记录:
**************************************************************************************/
struct ROAD_TRAFFIC_LIGHT_STRU {
    ROAD_SEG_LINE_STRU stStartLine;
    ROAD_SEG_LINE_STRU stEndLine;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        stStartLine.Upt(UptFunc);
        stEndLine.Upt(UptFunc);
    }
};

/**************************************************************************************
功能描述: 绑定辅助路段
修改记录:
**************************************************************************************/
struct ROAD_SERVICE_STRU {
    BOOL isServiceRoadExist;
    UINT32 serviceRoadNo;
};

/**************************************************************************************
功能描述: 道路信息
修改记录:
**************************************************************************************/
struct ROAD_INFO_STRU {
    UINT32                          no;
    ROAD_TYPE_ENUM                  roadType;
    UINT32                          maxAccessCarNum;
    UINT32                          isNarrow;
    UINT32                          avoidType;
    BOOL                            isMultiLane;
    BOOL                            isEasyTrafficJam;
    BOOL                            isOnlyForTask;
    BOOL                            isTurnAround;
    ROAD_SERVICE_STRU               serviceRoadStatus;
    vector<COORDINATE_2D_STRU>      vectPoints;
    vector<COORDINATE_2D_STRU>      vectAnchorPoints;
    ROAD_CARTYPE_ENUM               carType;
    vector<ROAD_PORT_STRU>          vectPort;
    vector<ROAD_ROUTE_STRU>         vectRoute;
    vector<ROAD_LINE_STRU>          vectLine;
    vector<ROAD_POSE_STRU>          vectPose;
    vector<ROAD_GOAL_STRU>          vectGoal;
    vector<ROAD_GOAL_AREA_STRU>     vectGoalArea;
    vector<ROAD_OBSTACLE_LINE_STRU> vectObstacleline;
    vector<ROAD_AREA_STRU>          vectArea;
    vector<ROAD_STOP_LINE_STRU>     vectStopLine;
    vector<ROAD_SPEED_BUMPS_STRU>   vectSpeedBumps;
    vector<ROAD_TRAFFIC_LIGHT_SPOT_STRU> vectTrafficLightSpot;
    unordered_map<UINT32, UINT32>   mapReverseDrivingRoute;
    vector<ROAD_CROSSING_STRU>      vectCrossing;
    vector<ROAD_LANE_LINE_STRU>     vectLaneLine;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc) {
        for (size_t i = 0; i < vectRoute.size(); i++) {
            vectRoute[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectLine.size(); i++) {
            vectLine[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectPose.size(); i++) {
            vectPose[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectGoal.size(); i++) {
            vectGoal[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectGoalArea.size(); i++) {
            vectGoalArea[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectObstacleline.size(); i++) {
            vectObstacleline[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectArea.size(); i++) {
            vectArea[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectStopLine.size(); i++) {
            vectStopLine[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectLaneLine.size(); i++) {
            vectLaneLine[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectSpeedBumps.size(); i++) {
            vectSpeedBumps[i].Upt(UptFunc);
        }

        for (size_t i = 0; i < vectTrafficLightSpot.size(); i++) {
            vectTrafficLightSpot[i].Upt(UptFunc);
        }

        // 对交叉口信息进行更新
        for (size_t i = 0; i < vectCrossing.size(); i++) {
            // 假设 ROAD_CROSSING_STRU 也有 Upt 方法
            vectCrossing[i].Upt(UptFunc);
        }
    }

    BOOL IfRoadInArea(const AREA_C &objArea) const
    {
        R_ASSERT(objArea.IsValid(), false);

        for (const ROAD_LINE_STRU &stLine : vectLine) {
            for (const COORDINATE_2D_STRU &stPoint : stLine.vectPoint) {
                if (objArea.IsIn(stPoint)) {
                    return true;
                }
            }
        }

        for (const ROAD_ROUTE_STRU &stLine : vectRoute) {
            for (const COORDINATE_2D_STRU &stPoint : stLine.vectPoint) {
                if (objArea.IsIn(stPoint)) {
                    return true;
                }
            }
        }

        for (const ROAD_OBSTACLE_LINE_STRU &stLine : vectObstacleline) {
            for (const COORDINATE_2D_STRU &stPoint : stLine.vectPoint) {
                if (objArea.IsIn(stPoint)) {
                    return true;
                }
            }
        }

        return false;
    }

    /* 获取道路所有端口处的限制线端点 */
    INT32 GetPortsLinesPoint(std::map<UINT32, std::map<UINT32, COORDINATE_2D_STRU>> &mapPortLinePoints) const
    {
        for (const ROAD_LINE_STRU &stLine : vectLine) {
            R_ASSERT(!stLine.vectPoint.empty(), -1);
            R_ASSERT_2(ROAD_PORT_STRU::IsOuterPort(stLine.stDir.startPortId), -1, this->no, stLine.stDir.startPortId);
            R_ASSERT_2(ROAD_PORT_STRU::IsOuterPort(stLine.stDir.stopPortId),  -1, this->no, stLine.stDir.stopPortId);
            R_ASSERT_3(stLine.stDir.startPortId != stLine.stDir.stopPortId,   -1, this->no, stLine.stDir.startPortId, stLine.stDir.stopPortId);

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

    /* 过滤指定端口处的道路边界线 */
    INT32 FilterPortBorderLines(std::map<UINT32, COORDINATE_2D_STRU> &mapLineId2Point) const
    {
        R_ASSERT_2(mapLineId2Point.size() >= 2, -1, this->no, mapLineId2Point.size());
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

        R_ASSERT_2(portWidthSqu > 0.0, -1, this->no, UINT32(portWidthSqu * 100));

        for (auto iter = mapLineId2Point.begin(); iter != mapLineId2Point.end(); ) {
            if ((iter->first != borderLineId1) && (iter->first != borderLineId2)) {
                iter = mapLineId2Point.erase(iter);
            } else {
                ++iter;
            }
        }

        R_ASSERT_2(mapLineId2Point.size() == 2, -1, this->no, mapLineId2Point.size());

        return 0;
    }

    /* 获取道路的所有端口处的边界线列表   */
    INT32 GetBorderLines(std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> &mapPort2LineList) const
    {
        mapPort2LineList.clear();

        /* 边界限制规则:
           限制线只能连接外部端口
           同一个端口连接多条限制时，端口处的点必须基本在一条直线上，距离最大的两个点所在的限制线即为道路边界线
        */
        /* 单限制线场景，他一定就是边界线 */
        if (this->vectLine.size() == 1) {
            const ROAD_LINE_STRU &stLine = this->vectLine.front();
            R_ASSERT_2(stLine.stDir.startPortId == stLine.stDir.stopPortId, -1, stLine.stDir.startPortId, stLine.stDir.stopPortId);
            mapPort2LineList[stLine.stDir.startPortId].emplace_back(&this->vectLine.front());
            return 0;
        }

        /* 多限制线场景，找同一个端口点距离最远的两条限制线为边界限制线。 */
        std::map<UINT32, std::map<UINT32, COORDINATE_2D_STRU>> mapPort2LinePoints;
        INT32 rsltOfGetPortsLinesPoint = GetPortsLinesPoint(mapPort2LinePoints);
        R_ASSERT_1(rsltOfGetPortsLinesPoint == 0, -1, this->no);

        for (auto &objPair1 : mapPort2LinePoints) {
            UINT32                                portId          = objPair1.first;
            std::map<UINT32, COORDINATE_2D_STRU> &mapLineId2Point = objPair1.second;

            INT32  rsltOfFilterPortBorderLines = FilterPortBorderLines(mapLineId2Point);
            R_ASSERT_2(rsltOfFilterPortBorderLines == 0, -1, this->no, portId);

            for (auto &objPair2 : mapLineId2Point) {
                UINT32 lineId = objPair2.first;

                const ROAD_LINE_STRU *pstLine = SearchLine(lineId);
                R_ASSERT_2(pstLine != nullptr, -1, this->no, lineId);

                mapPort2LineList[portId].emplace_back(pstLine);
            }
        }

        return 0;
    }

    INT32 JoinBorderLines(std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> &mapPort2LineList, std::vector<COORDINATE_2D_STRU> &vectBorder) const
    {
        vectBorder.clear();

        /* 如果只有一个端口，肯定只有一条限制线，不用连接。 */
        if (mapPort2LineList.size() == 1) {
            auto                                 iter     = mapPort2LineList.begin();
            std::vector<const ROAD_LINE_STRU *> &vectLine = iter->second;
            R_ASSERT_2(vectLine.size() == 1, -1, this->no, vectLine.size());
            const ROAD_LINE_STRU *pstLine = vectLine.front();
            R_ASSERT_1(pstLine != nullptr, -1, this->no);
            R_ASSERT_2(pstLine->vectPoint.size() >= 3, -1, this->no, pstLine->vectPoint.size());
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
                R_ASSERT_3(vectLineListOfCurPort.size() == 2, -1, this->no, vectLineListOfCurPort.size(), curPortId);
                pstExtendLine = vectLineListOfCurPort.front();
                vectLineListOfCurPort.erase(vectLineListOfCurPort.begin());

            } else {
                /* 不是第一个扩展的端口，处理后删除该端口 */
                /* 如果回到了第一个端口，则已经形成了闭环，不再继续处理 */
                curPortId = nextPortId;
                auto iterOfCurPort = mapPort2LineList.find(curPortId);
                R_ASSERT_3(iterOfCurPort != mapPort2LineList.end(), -1, this->no, curPortId, preLineId);
                if (curPortId == beginPortId) {
                    mapPort2LineList.erase(iterOfCurPort);
                    break;
                }

                /* 对于中间扩展端口，需要删除来路，然后从另一条路径扩展 */
                auto &vectLineListOfCurPort = iterOfCurPort->second;
                R_ASSERT_4(vectLineListOfCurPort.size() == 2, -1, this->no, vectLineListOfCurPort.size(), curPortId, preLineId);
                for (auto iterOfLine = vectLineListOfCurPort.begin(); iterOfLine != vectLineListOfCurPort.end(); ++iterOfLine) {
                    if (((const ROAD_LINE_STRU *)*iterOfLine)->id == preLineId) {
                        vectLineListOfCurPort.erase(iterOfLine);
                        break;
                    }
                }

                R_ASSERT_4(vectLineListOfCurPort.size() == 1, -1, this->no, vectLineListOfCurPort.size(), curPortId, preLineId);
                pstExtendLine = vectLineListOfCurPort.front();
                mapPort2LineList.erase(iterOfCurPort);
            }

            /* 将扩展路径按照端口扩展的方向的加入边界线末端 */
            R_ASSERT_3(pstExtendLine->vectPoint.size() >= 3, -1, this->no, pstExtendLine->id, curPortId);
            if (pstExtendLine->stDir.startPortId == curPortId) {
                vectBorder.insert(vectBorder.end(), pstExtendLine->vectPoint.begin(), pstExtendLine->vectPoint.end());
                preLineId  = pstExtendLine->id;
                nextPortId = pstExtendLine->stDir.stopPortId;

            } else {
                R_ASSERT_4(pstExtendLine->stDir.stopPortId == curPortId, -1, this->no, pstExtendLine->id, curPortId, pstExtendLine->stDir.stopPortId);
                std::reverse_copy(pstExtendLine->vectPoint.begin(), pstExtendLine->vectPoint.end(), std::back_inserter(vectBorder));
                preLineId  = pstExtendLine->id;
                nextPortId = pstExtendLine->stDir.startPortId;
            }
        }

        R_ASSERT_3(mapPort2LineList.empty(),  -1, this->no, beginPortId, nextPortId);
        R_ASSERT_3(beginPortId == nextPortId, -1, this->no, beginPortId, nextPortId);
        return 0;
    }

    /* 将道路边界线按照顺序连接起来构成封闭的多边形，即为该道路的边界 */
    INT32 GetBorder(std::vector<COORDINATE_2D_STRU> &vectBorder) const
    {
        std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> mapPort2LineList;
        INT32 rsltOfGetBorderLines = GetBorderLines(mapPort2LineList);
        R_ASSERT_1(rsltOfGetBorderLines == 0, -1, this->no);
        // R_ASSERT_1(mapPort2LineList.size() > 0, -1, this->no);

        INT32 rsltOfJoinBorderLines = JoinBorderLines(mapPort2LineList, vectBorder);
        R_ASSERT_1(rsltOfJoinBorderLines == 0, -1, this->no);

        return 0;
    }

    UINT32 GetLineNumConnectedWithPort(UINT32 portId) const
    {
        UINT32 num = 0;

        for (const ROAD_LINE_STRU &stLine : vectLine) {
            if (   (stLine.stDir.startPortId == portId)
                || (stLine.stDir.stopPortId  == portId)) {
                ++num;
            }
        }

        return num;
    }

    const ROAD_LINE_STRU *GetAnotherLineConnectedWithPort(UINT32 portId, UINT32 exceptedLineId) const
    {
        for (const ROAD_LINE_STRU &stLine : vectLine) {
            if (   (stLine.id != exceptedLineId)
                && ((stLine.stDir.startPortId == portId) || (stLine.stDir.stopPortId  == portId))) {
                return &stLine;
            }
        }

        return NULL;
    }

    void GetOuterPortIds(std::vector<UINT32> &vectPortId) const
    {
        vectPortId.reserve(vectPort.size());

        for (const ROAD_PORT_STRU &stPort : vectPort) {
            if (stPort.IsOuterPort()) {
                vectPortId.push_back(stPort.id);
            }
        }
    }

    const ROAD_ROUTE_STRU *SearchRoute(UINT32 routeId) const
    {
        for (UINT32 loop = 0; loop < vectRoute.size(); ++loop) {
            const ROAD_ROUTE_STRU &stRoute = vectRoute[loop];
            if (stRoute.id == routeId) {
                return &stRoute;
            }
        }

        return nullptr;
    }

    /**************************************************************************************
    功能描述: 搜索限制线
    修改记录:
    **************************************************************************************/
    const ROAD_LINE_STRU *SearchLine(UINT32 lineId) const
    {
        for (UINT32 loop = 0; loop < vectLine.size(); ++loop) {
            const ROAD_LINE_STRU &stLine = vectLine[loop];
            if (stLine.id == lineId) {
                return &stLine;
            }
        }

        return nullptr;
    }

    /**************************************************************************************
    功能描述: 搜索道路端口
    修改记录:
    **************************************************************************************/
    const ROAD_PORT_STRU *SearchPort(UINT32 portId) const
    {
        for (UINT32 loop = 0; loop < vectPort.size(); ++loop) {
            const ROAD_PORT_STRU &stPort = vectPort[loop];
            if (stPort.id == portId) {
                return &stPort;
            }
        }

        return nullptr;
    }

    /**************************************************************************************
    功能描述: 搜索路径
    修改记录:
    **************************************************************************************/
    const ROAD_GOAL_STRU *SearchGoal(UINT32 goalId) const
    {
        for (UINT32 loop = 0; loop < vectGoal.size(); ++loop) {
            const ROAD_GOAL_STRU &stGoal = vectGoal[loop];
            if (stGoal.id == goalId) {
                return &stGoal;
            }
        }

        return nullptr;
    }

    /**************************************************************************************
    功能描述: 搜索路径
    修改记录:
    **************************************************************************************/
    const ROAD_POSE_STRU *SearchPose(UINT32 poseId) const
    {
        for (UINT32 loop = 0; loop < vectPose.size(); ++loop) {
            const ROAD_POSE_STRU &stPose = vectPose[loop];
            if (stPose.id == poseId) {
                return &stPose;
            }
        }

        return nullptr;
    }
};

/**************************************************************************************
功能描述: 路网信息
修改记录:
**************************************************************************************/
struct ROAD_NETWORK_INFO_STRU {
    vector<ROAD_INFO_STRU>  vectRoad;

    void Upt(const std::function<COORDINATE_2D_STRU(const COORDINATE_2D_STRU&)> UptFunc)
    {
        for (size_t i = 0; i < vectRoad.size(); i++) {
            vectRoad[i].Upt(UptFunc);
        }
    }

    /**************************************************************************************
    功能描述: 搜索道路
    修改记录:
    **************************************************************************************/
    const ROAD_INFO_STRU *SearchRoad(UINT32 roadNo) const
    {
        for (const ROAD_INFO_STRU &stRoad : vectRoad) {
            if (stRoad.no == roadNo) {
                return &stRoad;
            }
        }

        return nullptr;
    }
};

struct ROAD_ELE_POSE_INFO_STRU
{
    UINT32                          pathId;
    ROAD_ELEMEMT_TYPE_ENUM          enType;
    COOR_2D_POSE_STRU               stPose;
    std::vector<COORDINATE_2D_STRU> vectPoint;
};

/**************************************************************************************
功能描述: 道路信息
          注意：传入的信息不能在本对象之前释放
修改记录:
**************************************************************************************/
class ROAD_INFO_WRAPPER_C
{
public:
    ROAD_INFO_WRAPPER_C(const ROAD_INFO_STRU &stRoadInfo) : stRoadInfo_(stRoadInfo)
    {
    }

    INT32 GetElePoseInfo(UINT32 eleType, UINT32 eleId, ROAD_ELE_POSE_INFO_STRU &stElePoseInfo) const;

    INT32 GetBorder(std::vector<COORDINATE_2D_STRU> &vectBorder) const;

private:
    const ROAD_INFO_STRU &stRoadInfo_;

    /**************************************************************************************
    功能描述: 提取边界线
    修改记录:
    **************************************************************************************/
    INT32 GetPortsLinesPoint(std::map<UINT32, std::map<UINT32, COORDINATE_2D_STRU>> &mapPortLinePoints) const;
    INT32 FilterPortBorderLines(std::map<UINT32, COORDINATE_2D_STRU> &mapLineId2Point) const;
    INT32 GetBorderLines(std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> &mapPort2LineList) const;
    INT32 JoinBorderLines(std::map<UINT32, std::vector<const ROAD_LINE_STRU *>> &mapPort2LineList, std::vector<COORDINATE_2D_STRU> &vectBorder) const;
};

/**************************************************************************************
功能描述: 路网信息
          注意：传入的信息不能在本对象之前释放
修改记录:
**************************************************************************************/
class ROAD_NETWORK_INFO_WRAPPER_C
{
public:
    ROAD_NETWORK_INFO_WRAPPER_C(void)
    {
    }

    ROAD_NETWORK_INFO_WRAPPER_C(const ROAD_NETWORK_INFO_STRU &stNetWorkInfo, BOOL isResetAll = false)
    {
        Reset(stNetWorkInfo, isResetAll);
    }

    void  Reset(const ROAD_NETWORK_INFO_STRU &stNetWorkInfo, BOOL isResetAll = false);
    INT32 GetElePoseInfo(UINT32 pathId, ROAD_ELE_POSE_INFO_STRU &stElePoseInfo) const;
    void  GetRoadsSelectedByPosConst(const COORDINATE_2D_STRU &stSelectPos, std::vector<const ROAD_INFO_STRU *> &vectRoadPtr) const;
    void  GetRoadsSelectedByPos(const COORDINATE_2D_STRU &stSelectPos, std::vector<const ROAD_INFO_STRU *> &vectRoadPtr);

private:
    struct ROAD_EX_INFO_STRU
    {
        AREA_C                objArea;
        const ROAD_INFO_STRU *pstRoadInfo;
    };

    std::map<UINT32, ROAD_EX_INFO_STRU> mapRoadExInfo_;
    BOOL                                isRoadAreasInited_{false};

    void InitRoadAreas(void);
};


#endif

