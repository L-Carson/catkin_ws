#ifndef __COMM_DYNC_MAP_FILE_H__
#define __COMM_DYNC_MAP_FILE_H__

#include "comm_dync_map_data.h"

#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/loc_json/json.h"

#include <vector>
#include <string>


/**************************************************************************************
功能描述: 动态地图文件
修改记录:
**************************************************************************************/
class DYNC_MAP_FILE_C
{
public:
    INT32 Load(Json::Value &objJsonDyncMap) const;
    INT32 ParseDyncMapObj(const Json::Value &objJsonDyncMapObj, DYNC_MAP_OBJ_STRU &stDyncMapObj) const;
    INT32 ParseDyncMap(const Json::Value &objJsonDyncMap, DYNC_MAP_INFO_STRU &stDyncMap) const;
    INT32 LoadAndParse(DYNC_MAP_INFO_STRU &stDyncMap) const;
    INT32 Save(const Json::Value &objJsonDyncMap) const;

private:
    FILE_OPRT_C           objFileOprt_;
    WORK_SCENE_FILE_MNG_C objSceneFileMng_;

    INT32 ParsePoint(const Json::Value &objJsonPoint, COORDINATE_2D_STRU &stPoint) const;
    INT32 ParseRestrictedAreaCutOff(const Json::Value &objJsonCutOff, DYNC_MAP_RESTRICTED_AREA_CUTOFF_STRU &stCutOff) const;
    INT32 ParseRestrictedAreaDetour(const Json::Value &objJsonDetour, DYNC_MAP_RESTRICTED_AREA_DETOUR_STRU &stDetour) const;
    INT32 ParseRestrictedArea(const Json::Value &objJsonRestrictedArea, DYNC_MAP_RESTRICTED_AREA_STRU &stRestrictedArea) const;
    INT32 ParseRestrictedAreaInfo(const Json::Value &objJsonRestrictedAreaInfo, DYNC_MAP_RESTRICTED_AREA_INFO_STRU &stRestrictedAreaInfo) const;
};

#endif

