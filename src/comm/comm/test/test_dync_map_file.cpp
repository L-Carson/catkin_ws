
#include "comm/comm_hd_map/comm_dync_map_file.h"
#include "comm/comm_base.h"

#include <ros/ros.h>


namespace TEST
{



}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_dync_map_file");
    ros::NodeHandle nh;
    INT32           runFrequency = 20;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    DYNC_MAP_FILE_C objDyncMapFile;

    DYNC_MAP_INFO_STRU stDyncMap;
    rslt = objDyncMapFile.LoadAndParse(stDyncMap);
    if (rslt != 0) {
        ST_LOG_ERR("objDyncMapFile.LoadAndParse() Fail.");
        return 1;
    }

    ST_LOG_INFO("Begin Print Dync Map Info. VerNo: %d.", stDyncMap.stRestrictedAreaInfo.verNo);
    for (const DYNC_MAP_RESTRICTED_AREA_STRU &stArea : stDyncMap.stRestrictedAreaInfo.vectArea) {
        ST_LOG_INFO("Id=%u, Time=%.2lf, type=%d.", stArea.id, stArea.expirationTime, stArea.enType);

        if (stArea.enType == DYNC_MAP_RESTRICTED_AREA_TYPE_CUTOFF) {
            ST_LOG_INFO("CutOff: StartPoint(%.2f, %.2f), EndPoint(%.2f, %.2f)", stArea.stCutOff.stStartPoint.x,
                                                                                stArea.stCutOff.stStartPoint.y,
                                                                                stArea.stCutOff.stEndPoint.x,
                                                                                stArea.stCutOff.stEndPoint.y);

        } else if (stArea.enType == DYNC_MAP_RESTRICTED_AREA_TYPE_DETOUR) {
            ST_LOG_INFO("Detour: Vertex Num: %u.", UINT32(stArea.stDetour.vectVertex.size()));
            for (const COORDINATE_2D_STRU &stVertex : stArea.stDetour.vectVertex) {
                ST_LOG_INFO("Vertex: (%.2f, %.2f)", stVertex.x, stVertex.y);
            }

        } else {
            ST_LOG_ERR("Unknown Type: %d.", stArea.enType);
            return 1;
        }
    }



    Json::Value objJsonDyncMap;
    rslt = objDyncMapFile.Load(objJsonDyncMap);
    if (rslt != 0) {
        ST_LOG_ERR("objDyncMapFile.Load() Fail.");
        return 1;
    }

    objJsonDyncMap["RestrictedAreaInfo"]["VerNo"] = objJsonDyncMap["RestrictedAreaInfo"]["VerNo"].asInt() + 10;
    rslt = objDyncMapFile.Save(objJsonDyncMap);
    if (rslt != 0) {
        ST_LOG_ERR("objDyncMapFile.Save() Fail.");
        return 1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}



