#ifndef __COMM_DYNC_MAP_DATA_H__
#define __COMM_DYNC_MAP_DATA_H__

#include "comm/comm_base.h"
#include "comm_road_network_data.h"

#include <list>
#include <vector>
#include <string>


/**************************************************************************************
功能描述: 动态地图处理错误码
          与导航人机接口文档定义一致
修改记录:
**************************************************************************************/
enum MAP_MNG_ERR_CODE_ENUM
{
    MAP_MNG_ERR_INTERSECT_WITH_MULT_ROADS          = 0x00100013,      /* 截断线与多条道路相交。 */
    MAP_MNG_ERR_INTERSECT_MULT_TIMES_WITH_ONE_LINE = 0x00100014,      /* 截断线与同一限制线相交多次。 */
    MAP_MNG_ERR_NOT_VALID_CUT_OFF_ROUTE            = 0x00100015,      /* 截断线未有效截断任何路径。 */
    MAP_MNG_ERR_NOT_VALID_POLYGON                  = 0x00100016,      /* 禁行区不是有效多边形。 */
    MAP_MNG_ERR_TOO_BIG_OR_TOO_SMALL               = 0x00100017,      /* 禁行区多边形区域过大或过小。 */
    MAP_MNG_ERR_NOT_IN_WORK_AREA                   = 0x00100018,      /* 禁行区不在工作区域。 */
    MAP_MNG_ERR_MAP_INFO_ERROR                     = 0x00100019,      /* 动态地图信息存在错误 */
    MAP_MNG_ERR_NOT_SUPPORT_WHEN_TASK              = 0x00100020,      /* 有任务执行时不支持修改禁行区 */
    MAP_MNG_ERR_CUT_OFF_NEAR_ROAD_PORT             = 0x00100021,      /* 截断禁行区不能靠近路口 */
};

/**************************************************************************************
功能描述: 地图管理消息类型
          与导航人机接口文档定义一致
修改记录:
**************************************************************************************/
enum MAP_MNG_MSG_TYPE_ENUM
{
    MAP_MNG_MSG_TYPE_DYNC_MAP_CHECK_REQ             = 7,
    MAP_MNG_MSG_TYPE_DYNC_MAP_CHECK_RSP             = 8,
    MAP_MNG_MSG_TYPE_DYNC_MAP_UPT_REQ               = 9,
    MAP_MNG_MSG_TYPE_DYNC_MAP_UPT_RSP               = 10,
    MAP_MNG_MSG_TYPE_DYNC_MAP_STATE_RPT_REQ         = 11,
    MAP_MNG_MSG_TYPE_DYNC_MAP_STATE_RPT_RSP         = 12,
};

/**************************************************************************************
功能描述: 动态地图对象类型
          与导航人机接口文档定义一致
修改记录:
**************************************************************************************/
enum DYNC_MAP_OBJECT_TYPE_ENUM
{
    DYNC_MAP_OBJECT_TYPE_RESTRICTED_AREA = 1,       /* 禁行区 */
};


/**************************************************************************************
功能描述: 临时禁行区类型
          与导航人机接口文档定义一致
修改记录:
**************************************************************************************/
enum DYNC_MAP_RESTRICTED_AREA_TYPE_ENUM
{
    DYNC_MAP_RESTRICTED_AREA_TYPE_CUTOFF = 1,   /* 截断禁行区 */
    DYNC_MAP_RESTRICTED_AREA_TYPE_DETOUR = 2,   /* 绕行禁行区 */
};

struct DYNC_MAP_RESTRICTED_AREA_CUTOFF_STRU
{
    COORDINATE_2D_STRU stStartPoint;
    COORDINATE_2D_STRU stEndPoint;
};

struct DYNC_MAP_RESTRICTED_AREA_DETOUR_STRU
{
    std::vector<COORDINATE_2D_STRU> vectVertex;
};

struct DYNC_MAP_RESTRICTED_AREA_STRU
{
    UINT32                               id;
    double                               expirationTime;
    DYNC_MAP_RESTRICTED_AREA_TYPE_ENUM   enType;
    DYNC_MAP_RESTRICTED_AREA_CUTOFF_STRU stCutOff;
    DYNC_MAP_RESTRICTED_AREA_DETOUR_STRU stDetour;
};

/**************************************************************************************
功能描述: 动态地图信息 - 临时禁行区
修改记录:
**************************************************************************************/
struct DYNC_MAP_RESTRICTED_AREA_INFO_STRU
{
    INT32                                      verNo;
    std::vector<DYNC_MAP_RESTRICTED_AREA_STRU> vectArea;    /* 版本号为-1时，该字段无效 */
};

/**************************************************************************************
功能描述: 动态地图对象
修改记录:
**************************************************************************************/
struct DYNC_MAP_OBJ_STRU
{
    DYNC_MAP_RESTRICTED_AREA_STRU stRestrictedArea;     /* 当前仅支持禁行区 */
};

/**************************************************************************************
功能描述: 动态地图信息
修改记录:
**************************************************************************************/
struct DYNC_MAP_INFO_STRU
{
    DYNC_MAP_RESTRICTED_AREA_INFO_STRU stRestrictedAreaInfo;
};

/**************************************************************************************
功能描述: 动态地图错误信息
修改记录:
**************************************************************************************/
struct DYNC_MAP_ERROR_INFO_STRU
{
    DYNC_MAP_OBJECT_TYPE_ENUM enObjType;
    UINT32                    objId;
    INT32                     errCode;
};

/**************************************************************************************
功能描述: 动态地图禁行区状态信息
修改记录:
**************************************************************************************/
struct DYNC_MAP_RESTRICTED_AREA_STATE_STRU
{
    BOOL                                  isAbnormal;
    std::vector<DYNC_MAP_ERROR_INFO_STRU> vectErrorInfo;
    std::vector<UINT32>                   vectEffectObj;
};

/**************************************************************************************
功能描述: 动态地图状态信息
修改记录:
**************************************************************************************/
struct DYNC_MAP_STATE_STRU
{
    DYNC_MAP_RESTRICTED_AREA_STATE_STRU stRestritedArea;
};


#endif

