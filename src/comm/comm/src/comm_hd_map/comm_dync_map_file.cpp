
#include "comm/comm_hd_map/comm_dync_map_file.h"

#include "comm/comm_time.h"

#include <iostream>
#include <fstream>


INT32 DYNC_MAP_FILE_C::Load(Json::Value &objJsonDyncMap) const
{
    ST_LOG_INFO("Begin.");

    /* 当前清扫车上仅读取默认路径的地图即可 */
    /* 文件不存在为正常情况 */
    std::string strDyncMapFilePath = objSceneFileMng_.GetDyncMapFilePath();
    if (!objFileOprt_.IsExist(strDyncMapFilePath)) {
        ST_LOG_INFO("File(%s) Not Exist.", strDyncMapFilePath.c_str());
        return 0;
    }

    std::ifstream objIfs(strDyncMapFilePath);
    if (!objIfs.is_open()) {
        ST_LOG_ERR("Open File(%s) Fail.", strDyncMapFilePath.c_str());
        return -1;
    }

    Json::CharReaderBuilder objBuilder;
    JSONCPP_STRING          strErrs;
    BOOL isSucc = Json::parseFromStream(objBuilder, objIfs, &objJsonDyncMap, &strErrs);
    objIfs.close();
    if (!isSucc) {
        ST_LOG_ERR("Json::parseFromStream(%s) Fail. Err: %s.", strDyncMapFilePath.c_str(), strErrs.c_str());
        return -1;
    }

    ST_LOG_INFO("Load(%s) Succ.", strDyncMapFilePath.c_str());
    return 0;
}

INT32 DYNC_MAP_FILE_C::ParseDyncMapObj(const Json::Value &objJsonDyncMapObj, DYNC_MAP_OBJ_STRU &stDyncMapObj) const
{
    ST_LOG_INFO("Begin.");

    R_ASSERT(objJsonDyncMapObj["RestrictedArea"].isObject(), -1);

    INT32 rslt = ParseRestrictedArea(objJsonDyncMapObj["RestrictedArea"], stDyncMapObj.stRestrictedArea);
    if (rslt != 0) {
        ST_LOG_ERR("ParseRestrictedArea() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 DYNC_MAP_FILE_C::ParseDyncMap(const Json::Value &objJsonDyncMap, DYNC_MAP_INFO_STRU &stDyncMap) const
{
    ST_LOG_INFO("Begin.");

    if (objJsonDyncMap["RestrictedAreaInfo"].isObject()) {
        INT32 rslt = ParseRestrictedAreaInfo(objJsonDyncMap["RestrictedAreaInfo"], stDyncMap.stRestrictedAreaInfo);
        if (rslt != 0) {
            ST_LOG_ERR("ParseRestrictedAreaInfo() Fail.");
            return -1;
        }
    } else {
        ST_LOG_INFO("Not Exist Restricted Area Info.");
        stDyncMap.stRestrictedAreaInfo.verNo = -1;
    }


    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 DYNC_MAP_FILE_C::LoadAndParse(DYNC_MAP_INFO_STRU &stDyncMap) const
{
    ST_LOG_INFO("Begin.");

    Json::Value objJsonDyncMap;
    INT32 rslt = Load(objJsonDyncMap);
    if (rslt != 0) {
        ST_LOG_ERR("Load() Fail.");
        return -1;
    }

    rslt = ParseDyncMap(objJsonDyncMap, stDyncMap);
    if (rslt != 0) {
        ST_LOG_ERR("ParseDyncMap() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 DYNC_MAP_FILE_C::Save(const Json::Value &objJsonDyncMap) const
{
    ST_LOG_INFO("Begin.");

    /* 当前清扫车上仅读取默认路径的地图即可 */
    /* 先准备目录 */
    std::string strWorkSceneDir = objSceneFileMng_.GetWorkSceneDir();
    if (!objFileOprt_.IsExist(strWorkSceneDir)) {
        ST_LOG_ERR("Work Scene Dir(%s) Not Exist.", strWorkSceneDir.c_str());
        return -1;
    }

    std::string strDyncMapDir = objSceneFileMng_.GetDynamicMapDir();
    if (!objFileOprt_.IsExist(strDyncMapDir)) {
        INT32 rslt = objFileOprt_.MakeDir(strDyncMapDir);
        if (rslt != 0) {
            ST_LOG_ERR("objFileOprt_.MakeDir(%s) Fail.", strDyncMapDir.c_str());
            return -1;
        }
    }

    /* 在写入文件 */
    std::string              strDyncMapFilePath = objSceneFileMng_.GetDyncMapFilePath();
    std::ofstream            objOfs(strDyncMapFilePath, std::ios::out);
    Json::StyledStreamWriter objStreamWriter;
    objStreamWriter.write(objOfs, objJsonDyncMap);
    objOfs.close();

    ST_LOG_INFO("Save(%s) Succ.", strDyncMapFilePath.c_str());
    return 0;
}

INT32 DYNC_MAP_FILE_C::ParsePoint(const Json::Value &objJsonPoint, COORDINATE_2D_STRU &stPoint) const
{
    CHECK_FAIL_R(objJsonPoint["X"].isDouble(), -1);
    CHECK_FAIL_R(objJsonPoint["Y"].isDouble(), -1);

    stPoint.x = objJsonPoint["X"].asFloat();
    stPoint.y = objJsonPoint["Y"].asFloat();
    return 0;
}

INT32 DYNC_MAP_FILE_C::ParseRestrictedAreaCutOff(const Json::Value &objJsonCutOff, DYNC_MAP_RESTRICTED_AREA_CUTOFF_STRU &stCutOff) const
{
    CHECK_FAIL_R(objJsonCutOff["BeginPoint"].isObject(), -1);
    CHECK_FAIL_R(objJsonCutOff["EndPoint"].isObject(),   -1);

    INT32 rslt = ParsePoint(objJsonCutOff["BeginPoint"], stCutOff.stStartPoint);
    if (rslt != 0) {
        ST_LOG_ERR("ParsePoint(Start) Fail.");
        return -1;
    }

    rslt = ParsePoint(objJsonCutOff["EndPoint"], stCutOff.stEndPoint);
    if (rslt != 0) {
        ST_LOG_ERR("ParsePoint(End) Fail.");
        return -1;
    }

    return 0;
}

INT32 DYNC_MAP_FILE_C::ParseRestrictedAreaDetour(const Json::Value &objJsonDetour, DYNC_MAP_RESTRICTED_AREA_DETOUR_STRU &stDetour) const
{
    CHECK_FAIL_R(objJsonDetour["VertexList"].isArray(), -1);

    UINT32 vertexSize = objJsonDetour["VertexList"].size();
    stDetour.vectVertex.reserve(vertexSize);
    for (UINT32 loop = 0; loop < vertexSize; ++loop) {
        const Json::Value &objJsonVertex = objJsonDetour["VertexList"][loop];
        CHECK_FAIL_R(objJsonVertex.isObject(), -1);

        COORDINATE_2D_STRU stVertex;
        INT32 rslt = ParsePoint(objJsonVertex, stVertex);
        if (rslt != 0) {
            ST_LOG_ERR("ParsePoint() Fail.");
            return -1;
        }

        stDetour.vectVertex.push_back(stVertex);
    }

    return 0;
}

INT32 DYNC_MAP_FILE_C::ParseRestrictedArea(const Json::Value &objJsonRestrictedArea, DYNC_MAP_RESTRICTED_AREA_STRU &stRestrictedArea) const
{
    CHECK_FAIL_R(objJsonRestrictedArea["Id"].isInt(),                -1);
    CHECK_FAIL_R(objJsonRestrictedArea["ExpirationTime"].isString(), -1);
    CHECK_FAIL_R(objJsonRestrictedArea["Type"].isInt(),              -1);

    stRestrictedArea.id = objJsonRestrictedArea["Id"].asInt();

    std::string strExpirationTime = objJsonRestrictedArea["ExpirationTime"].asString();
    if (strExpirationTime.empty()) {
        stRestrictedArea.expirationTime = -1.0;
    } else {
        stRestrictedArea.expirationTime = Comm_ConvertTimeStr2ToTimeSec(strExpirationTime);
        if (stRestrictedArea.expirationTime <= 0.0) {
            ST_LOG_ERR("Comm_ConvertTimeStr2ToTimeSec(%s) Fail.", strExpirationTime.c_str());
            return -1;
        }
    }

    INT32 type = objJsonRestrictedArea["Type"].asInt();
    if (type == 1) {
        stRestrictedArea.enType = DYNC_MAP_RESTRICTED_AREA_TYPE_CUTOFF;
        CHECK_FAIL_R(objJsonRestrictedArea["CutOffLine"].isObject(), -1);

        INT32 rslt = ParseRestrictedAreaCutOff(objJsonRestrictedArea["CutOffLine"], stRestrictedArea.stCutOff);
        if (rslt != 0) {
            ST_LOG_ERR("ParseRestrictedAreaCutOff(Id=%d) Fail.", stRestrictedArea.id);
            return -1;
        }

    } else if (type == 2) {
        stRestrictedArea.enType = DYNC_MAP_RESTRICTED_AREA_TYPE_DETOUR;
        CHECK_FAIL_R(objJsonRestrictedArea["DetourArea"].isObject(), -1);

        INT32 rslt = ParseRestrictedAreaDetour(objJsonRestrictedArea["DetourArea"], stRestrictedArea.stDetour);
        if (rslt != 0) {
            ST_LOG_ERR("ParseRestrictedAreaDetour(Id=%d) Fail.", stRestrictedArea.id);
            return -1;
        }

    } else {
        ST_LOG_ERR("Unknow Type(%d).", type);
        return -1;
    }

    return 0;
}

INT32 DYNC_MAP_FILE_C::ParseRestrictedAreaInfo(const Json::Value &objJsonRestrictedAreaInfo, DYNC_MAP_RESTRICTED_AREA_INFO_STRU &stRestrictedAreaInfo) const
{
    CHECK_FAIL_R(objJsonRestrictedAreaInfo["VerNo"].isInt(),                -1);
    stRestrictedAreaInfo.verNo = objJsonRestrictedAreaInfo["VerNo"].asInt();
    if (stRestrictedAreaInfo.verNo < 0) {
        return 0;
    }

    CHECK_FAIL_R(objJsonRestrictedAreaInfo["RestrictedAreaList"].isArray(), -1);
    UINT32 restrictedAreaSize = objJsonRestrictedAreaInfo["RestrictedAreaList"].size();
    stRestrictedAreaInfo.vectArea.reserve(restrictedAreaSize);

    for (UINT32 loop = 0; loop < restrictedAreaSize; ++loop) {
        const Json::Value &objJsonRestrictedArea = objJsonRestrictedAreaInfo["RestrictedAreaList"][loop];
        CHECK_FAIL_R(objJsonRestrictedArea.isObject(), -1);

        DYNC_MAP_RESTRICTED_AREA_STRU stRestrictedArea;
        INT32 rslt = ParseRestrictedArea(objJsonRestrictedArea, stRestrictedArea);
        if (rslt != 0) {
            ST_LOG_ERR("ParseRestrictedArea(Idx=%u) Fail.", loop);
            return -1;
        }

        stRestrictedAreaInfo.vectArea.push_back(stRestrictedArea);
    }

    return 0;
}


