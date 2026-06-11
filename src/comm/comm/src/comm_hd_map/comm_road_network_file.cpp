
#include "comm/comm_hd_map/comm_road_network_file.h"

/**************************************************************************************
功能描述: 获取指定属性
修改记录:
**************************************************************************************/
#define LoadMatch_GetAttr_(pf_, name_, val_)                        \
        if (TIXML_SUCCESS != pf_(name_, &val_)) {                   \
            ST_LOG_ERR("Get Attr <" name_ "> Fail.");               \
            return -1;                                              \
        }

/**************************************************************************************
功能描述: 读文件
修改记录:
**************************************************************************************/
INT32 ROAD_LINE_FILE_C::Read(const std::string &strFilePath, vector<COORDINATE_2D_STRU> &vectLine) const
{
    FILE *pfFile = fopen(strFilePath.c_str(), "r");
    if (pfFile == NULL) {
        ST_LOG_ERR("fopen(%s) fail.", strFilePath.c_str());
        ST_LOG_ERR("打开地图文件(%s) 失败.", strFilePath.c_str());
        return -1;
    }

    UINT32 lineNo = 0;
    char   acLine[256];
    while (NULL != fgets(acLine, sizeof(acLine), pfFile)) {
        ++lineNo;

        COORDINATE_2D_STRU stPoint;
        if (2 != sscanf(acLine, "%f %f", &stPoint.x, &stPoint.y)) {
            ST_LOG_ERR("File(%s) Line(%u) Str(%s) Is Err.", strFilePath.c_str(), (lineNo - 1), acLine);
            ST_LOG_ERR("文件(%s) 行数(%u) Str(%s) 格式错误.", strFilePath.c_str(), (lineNo - 1), acLine);
            continue;
        }

        vectLine.push_back(stPoint);
    }

    fclose(pfFile);
    pfFile = NULL;
    return 0;
}

/**************************************************************************************
功能描述: 写文件
修改记录:
**************************************************************************************/
INT32 ROAD_LINE_FILE_C::Write(const std::string &strFilePath, const vector<COORDINATE_2D_STRU> &vectLine) const
{
    FILE *pfFile = fopen(strFilePath.c_str(), "w");
    if (pfFile == NULL) {
        ST_LOG_ERR("Open File(%s) Fail.", strFilePath.c_str());
        return -1;
    }

    for (UINT32 loop  = 0; loop < vectLine.size(); ++loop) {
        fprintf(pfFile, "%8.4f %8.4f\n", vectLine[loop].x, vectLine[loop].y);
    }

    //FILE_OPRT_C objFileOprt;
    //objFileOprt.SyncBuffToFile(pfFile);
    //objFileOprt.SyncFileToDisk(pfFile);
    fclose(pfFile);
    pfFile = NULL;
    //ST_LOG_INFO("Succ. Line Num: %u", UINT32(vectLine.size()));
    return 0;
}

/* TODO(hubery) : 路线为空时报错 */
/**************************************************************************************
功能描述: 加载路径信息
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::Load(ROAD_NETWORK_INFO_STRU &stRoadNetwork)
{
    ST_LOG_INFO("Begin.");

    objSceneFileMng_.SetSceneDefaultDir();

    INT32 rslt = LoadFromFile(stRoadNetwork);
    if (rslt != 0) {
        ST_LOG_ERR("LoadFromFile() Fail.");
        ST_LOG_ERR("加载导航地图失败.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 ROAD_NETWORK_FILE_C::Load(const std::string &strSceneDir, ROAD_NETWORK_INFO_STRU &stRoadNetwork)
{
    ST_LOG_INFO("Begin. Dir: %s.", strSceneDir.c_str());

    objSceneFileMng_.SetSceneDir(strSceneDir);

    INT32 rslt = LoadFromFile(stRoadNetwork);
    if (rslt != 0) {
        ST_LOG_ERR("LoadFromFile() Fail.");
        ST_LOG_ERR("加载导航地图失败，请检查地图格式是否正确.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 ROAD_NETWORK_FILE_C::Save(const std::string &strSceneDir, const ROAD_NETWORK_INFO_STRU &stRoadNetwork)
{
    ST_LOG_INFO("Begin. Dir: %s.", strSceneDir.c_str());

    objSceneFileMng_.SetSceneDir(strSceneDir);

    /* 创建地图目录 */
    std::string strNavMapDir = objSceneFileMng_.GetNavMapDir();
    if (objFileOprt_.IsExist(strNavMapDir)) {
        INT32 rslt = objFileOprt_.DelDirContent(strNavMapDir);
        if (rslt != 0) {
            ST_LOG_ERR("objFileOprt_.DelDirContent(%s) Fail.", strNavMapDir.c_str());
            return -1;
        }
    } else {
        INT32 rslt = objFileOprt_.MakeDir(strNavMapDir);
        if (rslt != 0) {
            ST_LOG_ERR("objFileOprt_.MakeDir(%s) Fail.", strNavMapDir.c_str());
            return -1;
        }

        ST_LOG_INFO("Create Scene Dir: %s.", strNavMapDir.c_str());
    }

    /* 写入文件 */
    INT32 rslt = SaveToFile(stRoadNetwork);
    if (rslt != 0) {
        ST_LOG_ERR("SaveToFile(%s) Fail.", strSceneDir.c_str());
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 加载道路端口
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadPort(UINT32 roadNo, const TiXmlElement *pobjPort, ROAD_PORT_STRU &stPort) const
{
    if (TIXML_SUCCESS != pobjPort->QueryUnsignedAttribute("id", &stPort.id)) {
        ST_LOG_ERR("Get Attr <id> Fail.");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载道路端口列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadPortList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_PORT_STRU> &vectPort) const
{
    for (const TiXmlElement *pobjPort = pobjRoad->FirstChildElement("port");
         pobjPort != NULL;
         pobjPort = pobjPort->NextSiblingElement("port")) {
        ROAD_PORT_STRU stPort;
        INT32 rslt = LoadRoadPort(roadNo, pobjPort, stPort);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadPort(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load Port Succ. RoadNo=%u, PortId=%u.", roadNo, stPort.id);
        vectPort.push_back(stPort);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, PortNum=%u.", roadNo, UINT32(vectPort.size()));

    return 0;
}

void ROAD_NETWORK_FILE_C::GetValidPortList(const ROAD_INFO_STRU &stRoadInfo, const vector<ROAD_PORT_STRU> &vectOriginPort, vector<ROAD_PORT_STRU> &vectValidPort) const
{
    for (const ROAD_PORT_STRU &stPort : vectOriginPort) {
        if (stPort.id > 100) {
            vectValidPort.push_back(stPort);
        }

        for (const ROAD_LINE_STRU &stLine : stRoadInfo.vectLine) {
            if (stLine.stDir.startPortId == stPort.id) {
                vectValidPort.push_back(stPort);
                break;
            }

            if (stLine.stDir.stopPortId == stPort.id) {
                vectValidPort.push_back(stPort);
                break;
            }
        }
    }

    for (const ROAD_PORT_STRU &stPort : vectValidPort) {
        ST_LOG_INFO("Road no %u, valid port id %u.", stRoadInfo.no, stPort.id);
    }
}

/**************************************************************************************
功能描述: 加载道路路径
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadRoute(UINT32 roadNo, const TiXmlElement *pobjRoute, ROAD_ROUTE_STRU &stRoute) const
{
    LoadMatch_GetAttr_(pobjRoute->QueryUnsignedAttribute, "id",                  stRoute.id);
    LoadMatch_GetAttr_(pobjRoute->QueryUnsignedAttribute, "startPort",           stRoute.stDir.startPortId);
    LoadMatch_GetAttr_(pobjRoute->QueryUnsignedAttribute, "stopPort",            stRoute.stDir.stopPortId);
    LoadMatch_GetAttr_(pobjRoute->QueryBoolAttribute,     "isGoForward",         stRoute.isGoForward);

    stRoute.isNarrow = 0;
    if (TIXML_SUCCESS != pobjRoute->QueryUnsignedAttribute("isNarrow", &stRoute.isNarrow)) {
        ST_LOG_ERR("Load Route isNarrow Fail, use default value %u.", stRoute.isNarrow);
    }

    stRoute.avoidType = 1;
    if (TIXML_SUCCESS != pobjRoute->QueryUnsignedAttribute("avoidType", &stRoute.avoidType)) {
        ST_LOG_ERR("Load Route avoidType Fail, use default value %u.", stRoute.avoidType);
    }

    for (const TiXmlElement *pobjLimitLine = pobjRoute->FirstChildElement("limitLine");
             pobjLimitLine != NULL; pobjLimitLine = pobjLimitLine->NextSiblingElement("limitLine")) {
        UINT32 limitLineId;
        if (TIXML_SUCCESS != pobjLimitLine->QueryUnsignedAttribute("id", &limitLineId)) {
            ST_LOG_ERR("Get Attr <id> Fail.");
            return -1;
        }

        stRoute.vecLimitLineId.push_back(limitLineId);
        ST_LOG_INFO("Load limitLine id Succ. RoadNo=%u, RouteId=%u, limitLineId=%u.", roadNo, stRoute.id, limitLineId);
    }
    
    const char* typesStr = pobjRoute->Attribute("Types");
    if (typesStr != nullptr) {
        std::istringstream ss(typesStr);
        std::string typeStr;
        
        while (std::getline(ss, typeStr, ',')) {
            try {
                UINT32 typeValue = static_cast<UINT32>(std::stoul(typeStr));
                stRoute.Types.push_back(typeValue);
                ST_LOG_INFO("Load Route Type succ. RoadNo=%u, RouteId=%u, Type=%u", roadNo, stRoute.id, typeValue);
            }
            catch (const std::exception& e) {
                ST_LOG_ERR("Parse Types value failed: %s, RoadNo=%u, RouteId=%u", e.what(), roadNo, stRoute.id);
                return -1;
            }
        }
    } else {
        stRoute.Types.clear();
        stRoute.Types.push_back(0);
        ST_LOG_INFO("No Types config, use default [0]. RoadNo=%u, RouteId=%u", roadNo, stRoute.id);
    }

    UINT32 type;
    if (TIXML_SUCCESS != pobjRoute->QueryUnsignedAttribute("type", &type)) {
        ST_LOG_ERR("加载道路路径失败: 缺失字段[type]");
        return -1;
    }
    if (type >= ROUTE_ALONG_TYPE_BUTT) {
        ST_LOG_ERR("加载道路路径失败: Route[%u] Type[%u]", stRoute.id, type);
        return -1;
    }
    stRoute.enRouteAlongType = (ROUTE_ALONG_TYPE_ENUM)type;

    std::string strRouteFilePath = objSceneFileMng_.GetRoadRouteFilePath(roadNo, stRoute.id);
    INT32 rslt = objLineFile_.Read(strRouteFilePath, stRoute.vectPoint);
    if (0 != rslt) {
        ST_LOG_ERR("加载道路路径失败: strRouteFilePath[%s]", strRouteFilePath.c_str());
        return -1;
    }

    /* 锚点文件可能不存在 */
    std::string strRouteAnchorPointFilePath = objSceneFileMng_.GetRoadRouteAnchorPointFilePath(roadNo, stRoute.id);
    if (objFileOprt_.IsExist(strRouteAnchorPointFilePath)) {
        rslt = objLineFile_.Read(strRouteAnchorPointFilePath, stRoute.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("加载道路路径失败: strRouteAnchorPointFilePath[%s]", strRouteAnchorPointFilePath.c_str());
            return -1;
        }
    }

    stRoute.isInCrossingArea = false;
    pobjRoute->QueryBoolAttribute("isInCrossingArea", &stRoute.isInCrossingArea);

    stRoute.isReverseDriving = false;
    pobjRoute->QueryBoolAttribute("isReverseDriving", &stRoute.isReverseDriving);

    stRoute.isIgnoreBodySideObst = false;
    pobjRoute->QueryBoolAttribute("isIgnoreBodySideObst", &stRoute.isIgnoreBodySideObst);

    stRoute.avoidType = 1;
    pobjRoute->QueryUnsignedAttribute("avoidType", &stRoute.avoidType);

    stRoute.isLockStart = false;
    pobjRoute->QueryBoolAttribute("isLockStart", &stRoute.isLockStart);

    stRoute.clean_mode.is_blow = false;
    pobjRoute->QueryBoolAttribute("isBlow", &stRoute.clean_mode.is_blow);

    stRoute.clean_mode.is_spray = false;
    pobjRoute->QueryBoolAttribute("isWatering", &stRoute.clean_mode.is_spray);

    stRoute.clean_mode.is_stretch_brush = true;
    pobjRoute->QueryBoolAttribute("isStretchBrush", &stRoute.clean_mode.is_stretch_brush);

    ST_LOG_INFO("isInCrossingArea[%u], isReverseDriving[%u], isIgnoreBodySideObst[%u], isLockStart[%u], isBlow[%u], isWatering[%u], isStretchBrush[%u]", 
                 stRoute.isInCrossingArea, stRoute.isReverseDriving, stRoute.isIgnoreBodySideObst, stRoute.isLockStart, 
                 stRoute.clean_mode.is_blow, stRoute.clean_mode.is_spray, stRoute.clean_mode.is_stretch_brush);

    return 0;
}

/**************************************************************************************
功能描述: 加载道路路径列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadRouteList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_ROUTE_STRU> &vectRoute) const
{
    for (const TiXmlElement *pobjRoute = pobjRoad->FirstChildElement("route");
         pobjRoute != NULL;
         pobjRoute = pobjRoute->NextSiblingElement("route")) {
        ROAD_ROUTE_STRU stRoute;
        INT32 rslt = LoadRoadRoute(roadNo, pobjRoute, stRoute);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadRoute(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load Route Succ. RoadNo=%u, RouteId=%u, StartPortId=%u, StopPortId=%u, IsGoForward=%u, RouteType=%u, RouteLen=%u, AnchorPointSize=%u.", roadNo, stRoute.id, stRoute.stDir.startPortId, stRoute.stDir.stopPortId, stRoute.isGoForward, stRoute.enRouteAlongType, UINT32(stRoute.vectPoint.size()), UINT32(stRoute.vectAnchorPoint.size()));
        vectRoute.push_back(stRoute);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, RouteNum=%u.", roadNo, UINT32(vectRoute.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载道路限制线
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadLine(UINT32 roadNo, const TiXmlElement *pobjLine, ROAD_LINE_STRU &stLine) const
{
    if (TIXML_SUCCESS != pobjLine->QueryUnsignedAttribute("id", &stLine.id)) {
        ST_LOG_ERR("Get Attr <id> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjLine->QueryUnsignedAttribute("startPort", &stLine.stDir.startPortId)) {
        ST_LOG_ERR("Get Attr <startPort> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjLine->QueryUnsignedAttribute("stopPort", &stLine.stDir.stopPortId)) {
        ST_LOG_ERR("Get Attr <stopPort> Fail.");
        return -1;
    }

    std::string strLineFilePath = objSceneFileMng_.GetRoadLineFilePath(roadNo, stLine.id);
    INT32 rslt = objLineFile_.Read(strLineFilePath, stLine.vectPoint);
    if (0 != rslt) {
        ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strLineFilePath.c_str());
        return -1;
    }

    /* 锚点文件可能不存在 */
    std::string strLineAnchorPointFilePath = objSceneFileMng_.GetRoadLineAnchorPointFilePath(roadNo, stLine.id);
    if (objFileOprt_.IsExist(strLineAnchorPointFilePath)) {
        rslt = objLineFile_.Read(strLineAnchorPointFilePath, stLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载道路限制线列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_LINE_STRU> &vectLine) const
{
    for (const TiXmlElement *pobjLine = pobjRoad->FirstChildElement("line");
         pobjLine != NULL;
         pobjLine = pobjLine->NextSiblingElement("line")) {
        ROAD_LINE_STRU stLine;
        INT32 rslt = LoadRoadLine(roadNo, pobjLine, stLine);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadLine(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load Line Succ. RoadNo=%u, LineId=%u, StartPortId=%u, StopPortId=%u, LineLen=%u, AnchorPointSize=%u.", roadNo, stLine.id, stLine.stDir.startPortId, stLine.stDir.stopPortId, UINT32(stLine.vectPoint.size()), UINT32(stLine.vectAnchorPoint.size()));
        vectLine.push_back(stLine);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, LineNum=%u.", roadNo, UINT32(vectLine.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载道路限制线
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadObstacleLine(UINT32 roadNo, const TiXmlElement *pobjObstLine, ROAD_OBSTACLE_LINE_STRU &stObstLine) const
{
    if (TIXML_SUCCESS != pobjObstLine->QueryUnsignedAttribute("id", &stObstLine.id)) {
        ST_LOG_ERR("Get Attr <id> Fail.");
        return -1;
    }

    std::string strObstLineFilePath = objSceneFileMng_.GetRoadObstLineFilePath(roadNo, stObstLine.id);
    INT32 rslt = objLineFile_.Read(strObstLineFilePath, stObstLine.vectPoint);
    if (0 != rslt) {
        ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strObstLineFilePath.c_str());
        return -1;
    }

    /* 锚点文件可能不存在 */
    std::string strObstLineAnchorPointFilePath = objSceneFileMng_.GetRoadObstLineAnchorPointFilePath(roadNo, stObstLine.id);
    if (objFileOprt_.IsExist(strObstLineAnchorPointFilePath)) {
        rslt = objLineFile_.Read(strObstLineAnchorPointFilePath, stObstLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strObstLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载道路限制线列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadObstacleLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_OBSTACLE_LINE_STRU> &vectObstLine) const
{
    for (const TiXmlElement *pobjLine = pobjRoad->FirstChildElement("obstacleLine");
         pobjLine != NULL;
         pobjLine = pobjLine->NextSiblingElement("obstacleLine")) {
        ROAD_OBSTACLE_LINE_STRU stLine;
        INT32 rslt = LoadObstacleLine(roadNo, pobjLine, stLine);
        if (rslt != 0) {
            ST_LOG_ERR("LoadObstacleLine(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load ObstLine Succ. RoadNo=%u, LineId=%u, LineLen=%u, AnchorPointSize=%u.", roadNo, stLine.id, UINT32(stLine.vectPoint.size()), UINT32(stLine.vectAnchorPoint.size()));
        vectObstLine.push_back(stLine);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, LineNum=%u.", roadNo, UINT32(vectObstLine.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载关联点
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadPose(UINT32 roadNo, const TiXmlElement *pobjPose, ROAD_POSE_STRU &stPose) const
{
    LoadMatch_GetAttr_(pobjPose->QueryUnsignedAttribute, "id",                 stPose.id);
    
    if (TIXML_SUCCESS != pobjPose->QueryUnsignedAttribute("priority", &stPose.priority))
    {
        stPose.priority = 1;
    }

    if (TIXML_SUCCESS != pobjPose->QueryBoolAttribute("isRainProof", &stPose.isRainProof))
    {
        stPose.isRainProof = true;
    }

    UINT32 type;
    LoadMatch_GetAttr_(pobjPose->QueryUnsignedAttribute, "type", type);

    INT32 rslt = LoadCarType(pobjPose, stPose.carType);
    if (rslt != 0)
    {
        // 默认设置为330机型
        stPose.carType = ROAD_CARTYPE_330;
    }

    // 解析 Types 属性
    const char* typesStr = pobjPose->Attribute("Types");
    if (typesStr != nullptr) {
        std::istringstream ss(typesStr);
        std::string type;
        while (std::getline(ss, type, ',')) {
            stPose.Types.push_back(std::stoul(type));
        }
    }

    UINT32 SiteAttribute;
    LoadMatch_GetAttr_(pobjPose->QueryUnsignedAttribute, "SiteAttribute", SiteAttribute);

    // 解析 SiteNames 子元素
    const TiXmlElement *pobjSiteNames = pobjPose->FirstChildElement("SiteNames");
    if (pobjSiteNames != nullptr) {
        const char* zhName = pobjSiteNames->Attribute("zh");
        const char* enName = pobjSiteNames->Attribute("en");
        if (zhName != nullptr) {
            stPose.SiteNames["zh"] = zhName;
        }
        if (enName != nullptr) {
            stPose.SiteNames["en"] = enName;
        }
    }

    switch (type)
    {
        case 0:
            stPose.type = ROAD_POSE_GARBAGE;
            break;
        case 1:
            stPose.type = ROAD_POSE_CHARGE;
            break;
        case 2:
            stPose.type = ROAD_POSE_GARBAGE_NO_BOX;
            break;
        case 3:
            stPose.type = ROAD_POSE_THROW_RUBBISH;
            break;
        default:
            ST_LOG_ERR("设备点类型不存在");
            return -1;
    }

    switch (SiteAttribute)
    {
        case 0:
            stPose.SiteAttribute = ROAD_ATTRIBUTE_MANUAL;
            break;
        case 1:
            stPose.SiteAttribute = ROAD_ATTRIBUTE_AUTO;
            // 仅当 SiteAttribute 为 1 时加载 Distance
            double distanceValue; // 临时变量用于存储读取的值
            LoadMatch_GetAttr_(pobjPose->QueryDoubleAttribute, "Distance", distanceValue);
            stPose.Distance.distance = distanceValue;
            break;
        default:
            return -1;
    }

    // LoadMatch_GetAttr_(pobjPose->QueryUnsignedAttribute, "PortId",     stPose.PortId);

    double x = 0.0, y = 0.0, yaw = 0.0;
    LoadMatch_GetAttr_(pobjPose->QueryDoubleAttribute, "x", x);
    LoadMatch_GetAttr_(pobjPose->QueryDoubleAttribute, "y", y);
    LoadMatch_GetAttr_(pobjPose->QueryDoubleAttribute, "yaw", yaw);
    stPose.pose.stPos.x     = x;
    stPose.pose.stPos.y     = y;
    stPose.pose.dirAngle    = yaw;

    LoadMatch_GetAttr_(pobjPose->QueryUnsignedAttribute, "goalId",   stPose.goalId);
    LoadMatch_GetAttr_(pobjPose->QueryUnsignedAttribute, "QRCodeId", stPose.QRCodeId);

    if (stPose.type == ROAD_POSE_CHARGE) {
        if (TIXML_SUCCESS != pobjPose->QueryUnsignedAttribute("channelNumber", &stPose.channelNumber))
        {
            stPose.channelNumber = 50;
        }
        ST_LOG_INFO("ReflectiveStrip.");
        const TiXmlElement *pobjReflectiveStrip = pobjPose->FirstChildElement("reflectiveStrip");
        if (pobjReflectiveStrip == nullptr) {
            ST_LOG_ERR("当前站点类型 %u（垃圾点 %u, 充电加水点 %u）, 需要配置反光条.", stPose.type, ROAD_POSE_GARBAGE, ROAD_POSE_CHARGE);
            return -1;
        }

        POINT_COORD_3D_STRU leftPoint;
        LoadMatch_GetAttr_(pobjReflectiveStrip->QueryDoubleAttribute, "leftX", leftPoint.x);
        LoadMatch_GetAttr_(pobjReflectiveStrip->QueryDoubleAttribute, "leftY", leftPoint.y);
        LoadMatch_GetAttr_(pobjReflectiveStrip->QueryDoubleAttribute, "leftZ", leftPoint.z);

        POINT_COORD_3D_STRU rightPoint;
        LoadMatch_GetAttr_(pobjReflectiveStrip->QueryDoubleAttribute, "rightX", rightPoint.x);
        LoadMatch_GetAttr_(pobjReflectiveStrip->QueryDoubleAttribute, "rightY", rightPoint.y);
        LoadMatch_GetAttr_(pobjReflectiveStrip->QueryDoubleAttribute, "rightZ", rightPoint.z);

        stPose.useReflectiveStrip = true;
        stPose.reflectiveStripPosLeft  = leftPoint;
        stPose.reflectiveStripPosRight = rightPoint;
        ST_LOG_INFO("leftX %f, leftY %f, leftZ %f, rightX %f, rightY %f, rightZ %f.", leftPoint.x, leftPoint.y, leftPoint.z, rightPoint.x, rightPoint.y, rightPoint.z);

        stPose.chargeSiteSNInfo.chargeSiteSN.clear();
        std::string chargeSiteSN{"chargeSiteSN"};
        const std::string *ptrName = pobjPose->Attribute(chargeSiteSN);
        if (ptrName != nullptr) {
            stPose.chargeSiteSNInfo.chargeSiteSN = *ptrName;
        }
    } else {
        ST_LOG_INFO("no ReflectiveStrip.");
        stPose.useReflectiveStrip = false;

        if (TIXML_SUCCESS != pobjPose->QueryDoubleAttribute("qRPosOffsetDis", &stPose.qRPosOffsetDis)) {
            stPose.qRPosOffsetDis = 0.0;
        }

        if (TIXML_SUCCESS != pobjPose->QueryDoubleAttribute("liftBoxTravel", &stPose.liftBoxTravel)) {
            stPose.liftBoxTravel = 3000.0; // 默认设置最大值
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载关联点列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadPoses(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_POSE_STRU> &vectPose) const
{
    for (const TiXmlElement *pobjPose = pobjRoad->FirstChildElement("pose");
        pobjPose != NULL;
        pobjPose = pobjPose->NextSiblingElement("pose")) {
        ROAD_POSE_STRU stPose;
        INT32 rslt = LoadRoadPose(roadNo, pobjPose, stPose);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadPose(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load Pose Succ. RoadNo=%u, PoseId=%u, SiteAttribute=%u, distance=%f, x=%f, y=%f, yaw=%f.", roadNo, stPose.id, stPose.SiteAttribute, 
            stPose.Distance.distance, stPose.pose.stPos.x, stPose.pose.stPos.y, stPose.pose.dirAngle);
        vectPose.push_back(stPose);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, PoseNum=%u.", roadNo, UINT32(vectPose.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载目标点
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadGoal(UINT32 roadNo, const TiXmlElement *pobjGoal, ROAD_GOAL_STRU &stGoal) const
{
    LoadMatch_GetAttr_(pobjGoal->QueryUnsignedAttribute, "id", stGoal.id);

    UINT32 type;
    LoadMatch_GetAttr_(pobjGoal->QueryUnsignedAttribute, "type", type);
    switch (type)
    {
        case 0:
            stGoal.type = ROAD_GOAL_PARKING;
            break;

        case 1:
            stGoal.type = ROAD_GOAL_ALIGNING;
            break;

        case 2:
            stGoal.type = ROAD_GOAL_MAP_CHANGE;
            break;

        default:
            return -1;
    }

    // 解析 Types 属性
    const char* typesStr = pobjGoal->Attribute("Types");
    if (typesStr != nullptr) {
        std::istringstream ss(typesStr);
        std::string type;
        while (std::getline(ss, type, ',')) {
            stGoal.Types.push_back(std::stoul(type));
        }
    }

    // 解析 SiteNames 子元素
    const TiXmlElement *pobjSiteNames = pobjGoal->FirstChildElement("SiteNames");
    if (pobjSiteNames != nullptr) {
        const char* zhName = pobjSiteNames->Attribute("zh");
        const char* enName = pobjSiteNames->Attribute("en");
        if (zhName != nullptr) {
            stGoal.SiteNames["zh"] = zhName;
        }
        if (enName != nullptr) {
            stGoal.SiteNames["en"] = enName;
        }
    }

    double x = 0.0, y = 0.0, yaw = 0.0;
    LoadMatch_GetAttr_(pobjGoal->QueryDoubleAttribute, "x", x);
    LoadMatch_GetAttr_(pobjGoal->QueryDoubleAttribute, "y", y);
    LoadMatch_GetAttr_(pobjGoal->QueryDoubleAttribute, "yaw", yaw);

    stGoal.goal.stPos.x     = x;
    stGoal.goal.stPos.y     = y;
    stGoal.goal.dirAngle    = yaw;

    LoadMatch_GetAttr_(pobjGoal->QueryUnsignedAttribute, "portId", stGoal.portId);

    if (stGoal.type == ROAD_GOAL_MAP_CHANGE) {
        double reachLimitRange = 1.0;
        LoadMatch_GetAttr_(pobjGoal->QueryDoubleAttribute, "reachLimitRange", reachLimitRange);
        stGoal.changeMapGoalInfo.reachLimitRange = reachLimitRange;
        stGoal.changeMapGoalInfo.changeMapName.clear();
        stGoal.changeMapGoalInfo.changeMapGoalPathId.clear();
        std::string changeMapName{"changeMapName"};
        const std::string *ptrName = pobjGoal->Attribute(changeMapName);
        if (ptrName != nullptr) {
            stGoal.changeMapGoalInfo.changeMapName = *ptrName;
        }
        std::string changeMapGoalPathId{"changeMapGoalPathId"};
        const std::string *ptrPathId = pobjGoal->Attribute(changeMapGoalPathId);
        if (ptrPathId != nullptr) {
            stGoal.changeMapGoalInfo.changeMapGoalPathId = *ptrPathId;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载目标点列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadGoals(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_GOAL_STRU> &vectGoal) const
{
    for (const TiXmlElement *pobjGoal = pobjRoad->FirstChildElement("goal");
        pobjGoal != NULL;
        pobjGoal = pobjGoal->NextSiblingElement("goal")) {
        ROAD_GOAL_STRU stGoal;
        INT32 rslt = LoadRoadGoal(roadNo, pobjGoal, stGoal);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadGoal(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load Goal Succ. RoadNo=%u, GoalId=%u, x=%f, y=%f, yaw=%f.", roadNo, stGoal.id, stGoal.goal.stPos.x,
            stGoal.goal.stPos.y, stGoal.goal.dirAngle);
        vectGoal.push_back(stGoal);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, GoalNum=%u.", roadNo, UINT32(vectGoal.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载目标点区域
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadGoalArea(UINT32 roadNo, const TiXmlElement *pobjGoalArea, ROAD_GOAL_AREA_STRU &stGoalArea) const
{
    LoadMatch_GetAttr_(pobjGoalArea->QueryUnsignedAttribute, "id", stGoalArea.id);

    UINT32 type;
    LoadMatch_GetAttr_(pobjGoalArea->QueryUnsignedAttribute, "type", type);
    switch (type)
    {
        case 0:
            stGoalArea.type = ROAD_GOAL_AREA_RUBBISH;
            break;

        default:
            return -1;
    }

    std::string strGoalAreaFilePath = objSceneFileMng_.GetRoadGoalAreaFilePath(roadNo, stGoalArea.id);
    INT32 rslt = objLineFile_.Read(strGoalAreaFilePath, stGoalArea.vertexs);
    if (0 != rslt) {
        ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strGoalAreaFilePath.c_str());
        return -1;
    }

    for (const TiXmlElement *pobjPort = pobjGoalArea->FirstChildElement("port");
        pobjPort != NULL;
        pobjPort = pobjPort->NextSiblingElement("port")) {
        UINT32 id;
        LoadMatch_GetAttr_(pobjPort->QueryUnsignedAttribute, "id", id);
        stGoalArea.portIds.emplace_back(id);
    }

    // 解析 SiteNames 子元素
    const TiXmlElement *pobjSiteNames = pobjGoalArea->FirstChildElement("SiteNames");
    if (pobjSiteNames != nullptr) {
        const char* zhName = pobjSiteNames->Attribute("zh");
        const char* enName = pobjSiteNames->Attribute("en");
        if (zhName != nullptr) {
            stGoalArea.SiteNames["zh"] = zhName;
        }
        if (enName != nullptr) {
            stGoalArea.SiteNames["en"] = enName;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载目标区域列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadGoalAreas(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_GOAL_AREA_STRU> &vectGoalArea) const
{
    for (const TiXmlElement *pobjGoalArea = pobjRoad->FirstChildElement("goalArea");
        pobjGoalArea != NULL;
        pobjGoalArea = pobjGoalArea->NextSiblingElement("goalArea")) {
        ROAD_GOAL_AREA_STRU stGoalArea;
        INT32 rslt = LoadRoadGoalArea(roadNo, pobjGoalArea, stGoalArea);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadGoalArea(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load GoalArea Succ. RoadNo=%u, GoalAreaId=%u.", roadNo, stGoalArea.id);
        vectGoalArea.push_back(stGoalArea);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, GoalAreaNum=%u.", roadNo, UINT32(vectGoalArea.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载红绿灯点
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadTrafficLightSpot(UINT32 roadNo, 
                                                const TiXmlElement *pobjTrafficLightSpot, 
                                                ROAD_TRAFFIC_LIGHT_SPOT_STRU &stTrafficLightSpot) const
{
    LoadMatch_GetAttr_(pobjTrafficLightSpot->QueryUnsignedAttribute, "id", stTrafficLightSpot.id);

    UINT32 type;
    LoadMatch_GetAttr_(pobjTrafficLightSpot->QueryUnsignedAttribute, "type", type);
    switch (type)
    {
        case 1:
            stTrafficLightSpot.type = ROAD_TRAFFIC_LIGHT_TYPE_NUMBER;
            break;

        case 2:
            stTrafficLightSpot.type = ROAD_TRAFFIC_LIGHT_TYPE_BLINK;
            break;
        
        case 3:
            stTrafficLightSpot.type = ROAD_TRAFFIC_LIGHT_TYPE_FREQUENT_LIGHT_OFF;
            break;

        default:
            return -1;
    }

    double x = 0.0, y = 0.0;
    LoadMatch_GetAttr_(pobjTrafficLightSpot->QueryDoubleAttribute, "x", x);
    LoadMatch_GetAttr_(pobjTrafficLightSpot->QueryDoubleAttribute, "y", y);
    stTrafficLightSpot.point.x     = x;
    stTrafficLightSpot.point.y     = y;
    LoadMatch_GetAttr_(pobjTrafficLightSpot->QueryDoubleAttribute, "crossingTime", stTrafficLightSpot.crossingTime);
    LoadMatch_GetAttr_(pobjTrafficLightSpot->QueryDoubleAttribute, "height", stTrafficLightSpot.height);
    if (TIXML_SUCCESS != pobjTrafficLightSpot->QueryDoubleAttribute("rowLength", &stTrafficLightSpot.rowLength))
    {
        stTrafficLightSpot.rowLength = 0.35;
    }
    if (TIXML_SUCCESS != pobjTrafficLightSpot->QueryDoubleAttribute("colLength", &stTrafficLightSpot.colLength))
    {
        stTrafficLightSpot.colLength = 0.70;
    }
    if (TIXML_SUCCESS != pobjTrafficLightSpot->QueryDoubleAttribute("pixelBias", &stTrafficLightSpot.pixelBias))
    {
        stTrafficLightSpot.pixelBias = 0.0;
    }
    if (TIXML_SUCCESS != pobjTrafficLightSpot->QueryDoubleAttribute("boxBias", &stTrafficLightSpot.boxBias))
    {
        stTrafficLightSpot.boxBias = 0.0;
    }
    if (TIXML_SUCCESS != pobjTrafficLightSpot->QueryIntAttribute("groupId", &stTrafficLightSpot.groupId))
    {
        stTrafficLightSpot.groupId = -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载红绿灯点列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadTrafficLightSpots(UINT32 roadNo, 
                                                 const TiXmlElement *pobjRoad, 
                                                 vector<ROAD_TRAFFIC_LIGHT_SPOT_STRU> &vectTrafficLightSpot) const
{
    for (const TiXmlElement *pobjTrafficLightSpot = pobjRoad->FirstChildElement("trafficLight");
        pobjTrafficLightSpot != NULL;
        pobjTrafficLightSpot = pobjTrafficLightSpot->NextSiblingElement("trafficLight")) {
        ROAD_TRAFFIC_LIGHT_SPOT_STRU stTrafficLightSpot;
        INT32 rslt = LoadTrafficLightSpot(roadNo, pobjTrafficLightSpot, stTrafficLightSpot);
        if (rslt != 0) {
            ST_LOG_ERR("LoadTrafficLightSpot(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load TrafficLightSpot Succ. RoadNo=%u, SpotId=%u, x=%f, y=%f.", roadNo, stTrafficLightSpot.id, 
        stTrafficLightSpot.point.x, stTrafficLightSpot.point.y);
        vectTrafficLightSpot.push_back(stTrafficLightSpot);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, TrafficLightSpotNum=%u.", roadNo, UINT32(vectTrafficLightSpot.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载特殊区域
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadArea(UINT32 roadNo, const TiXmlElement *pobjArea, ROAD_AREA_STRU &stArea) const
{
    stArea.roadNo = roadNo;

    LoadMatch_GetAttr_(pobjArea->QueryUnsignedAttribute, "id", stArea.id);
    if (TIXML_SUCCESS != pobjArea->QueryUnsignedAttribute("type", &stArea.type)) {
        stArea.type = 4;
        ST_LOG_ERR("Get Attr <id> Fail.");
    }

    std::string strAreaFilePath = objSceneFileMng_.GetRoadAreaFilePath(roadNo, stArea.id);
    INT32 rslt = objLineFile_.Read(strAreaFilePath, stArea.vertexs);
    if (0 != rslt) {
        ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strAreaFilePath.c_str());
        return -1;
    }
    R_ASSERT(stArea.vertexs.size() >= 3, -1);

    LoadMatch_GetAttr_(pobjArea->QueryBoolAttribute, "isEnableVelItem",   stArea.isEnableVelItem);
    LoadMatch_GetAttr_(pobjArea->QueryBoolAttribute, "isEnableBrushItem", stArea.isEnableBrushItem);
    LoadMatch_GetAttr_(pobjArea->QueryBoolAttribute, "isEnableWaterItem", stArea.isEnableWaterItem);
    LoadMatch_GetAttr_(pobjArea->QueryBoolAttribute, "isEnableCoverItem", stArea.isEnableCoverItem);
    stArea.isEnableDoorItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableDoorItem", &stArea.isEnableDoorItem)) {
    }
    ST_LOG_INFO("isEnableDoorItem[%u]", stArea.isEnableDoorItem);

    stArea.isEnableSlopeItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableSlopeItem", &stArea.isEnableSlopeItem)) {
    }
    ST_LOG_INFO("isEnableSlopeItem[%u]", stArea.isEnableSlopeItem);

    stArea.isEnableHeightItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableHeightItem", &stArea.isEnableHeightItem)) {
    }
    ST_LOG_INFO("isEnableHeightItem[%u]", stArea.isEnableHeightItem);

    stArea.isEnableCrossingItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableCrossingItem", &stArea.isEnableCrossingItem)) {
    }
    ST_LOG_INFO("isEnableCrossingItem[%u]", stArea.isEnableCrossingItem);

    stArea.isEnableObstItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableObstItem", &stArea.isEnableObstItem)) {
    }
    ST_LOG_INFO("isEnableObstItem[%u]", stArea.isEnableObstItem);

    stArea.isEnableRangeItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableRangeItem", &stArea.isEnableRangeItem)) {
    }
    ST_LOG_INFO("isEnableRangeItem[%u]", stArea.isEnableRangeItem);

    stArea.isEnableTreelawnItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableTreelawnItem", &stArea.isEnableTreelawnItem)) {
    }
    ST_LOG_INFO("isEnableTreelawnItem[%u]", stArea.isEnableTreelawnItem);

    stArea.isEnableStopExistObst = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableStopExistObst", &stArea.isEnableStopExistObst)) {
    }
    ST_LOG_INFO("isEnableStopExistObst[%u]", stArea.isEnableStopExistObst);

    stArea.isEnableTrafficControlItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableTrafficControlItem", &stArea.isEnableTrafficControlItem)) {
    }
    ST_LOG_INFO("isEnableTrafficControlItem[%u]", stArea.isEnableTrafficControlItem);

    stArea.isEnableRubberControlItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableRubberControlItem", &stArea.isEnableRubberControlItem)) {
    }
    ST_LOG_INFO("isEnableRubberControlItem[%u]", stArea.isEnableRubberControlItem);
    
    stArea.isEnableEntanglementItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableEntanglementItem", &stArea.isEnableEntanglementItem)) {
    }
    ST_LOG_INFO("isEnableEntanglementItem[%u]", stArea.isEnableEntanglementItem);

    stArea.isEnableRoadStakeItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableRoadStakeItem", &stArea.isEnableRoadStakeItem)) {
    }
    ST_LOG_INFO("isEnableRoadStakeItem[%u]", stArea.isEnableRoadStakeItem);

    stArea.isEnableRecognizeItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableRecognizeItem", &stArea.isEnableRecognizeItem)) {
    }
    ST_LOG_INFO("isEnableRecognizeItem[%u]", stArea.isEnableRecognizeItem);

    stArea.isEnableNoUtralItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableNoUtralItem", &stArea.isEnableNoUtralItem)) {
    }
    ST_LOG_INFO("isEnableNoUtralItem[%u]", stArea.isEnableNoUtralItem);

    stArea.isEnableNoReverseItem = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableNoReverseItem", &stArea.isEnableNoReverseItem)) {
    }
    ST_LOG_INFO("isEnableNoReverseItem[%u]", stArea.isEnableNoReverseItem);

    //add isEnableCloseObstacle
    stArea.isEnableCloseObstacle = false;
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableCloseObstacle", &stArea.isEnableCloseObstacle)) {
    }
    ST_LOG_INFO("isEnableCloseObstacle[%u]", stArea.isEnableCloseObstacle);

    stArea.isEnableNotAllowedAutoNavigation = false; 
    if (TIXML_SUCCESS != pobjArea->QueryBoolAttribute("isEnableNotAllowedAutoNavigation", &stArea.isEnableNotAllowedAutoNavigation)) {
    }
    ST_LOG_INFO("isEnableNotAllowedAutoNavigation[%u]", stArea.isEnableNotAllowedAutoNavigation); 

    if (stArea.isEnableVelItem) {
        const TiXmlElement *pobjVelItem = pobjArea->FirstChildElement("velItem");
        R_ASSERT(pobjVelItem != nullptr, -1);

        double slowDownVel;
        LoadMatch_GetAttr_(pobjVelItem->QueryDoubleAttribute, "slowDownVel", slowDownVel);
        stArea.velItem.slowDownVel = slowDownVel;

        stArea.velItem.isOnlyEffectForClean = false;
        if (TIXML_SUCCESS != pobjVelItem->QueryBoolAttribute("isOnlyEffectForClean", &stArea.velItem.isOnlyEffectForClean)) {
            ST_LOG_ERR("Load isOnlyEffectForClean Fail, use default value %u.", stArea.velItem.isOnlyEffectForClean);
        }
    }

    if (stArea.isEnableBrushItem) {
        const TiXmlElement *pobjBrushItem = pobjArea->FirstChildElement("brushItem");
        R_ASSERT(pobjBrushItem != nullptr, -1);
        //兼容旧版本
        if(TIXML_SUCCESS != pobjBrushItem->QueryBoolAttribute("isEffectInOutOfArea", &stArea.brushItem.isEffectInOutOfArea)){
            stArea.brushItem.isEffectInOutOfArea = false;
        }

        LoadMatch_GetAttr_(pobjBrushItem->QueryBoolAttribute,  "isCloseMainBrush", stArea.brushItem.isCloseMainBrush);
        LoadMatch_GetAttr_(pobjBrushItem->QueryBoolAttribute,  "isCloseSideBrush", stArea.brushItem.isCloseSideBrush);
    }

    if (stArea.isEnableWaterItem) {
        const TiXmlElement *pobjWaterItem = pobjArea->FirstChildElement("waterItem");
        R_ASSERT(pobjWaterItem != nullptr, -1);
        LoadMatch_GetAttr_(pobjWaterItem->QueryBoolAttribute,  "isCloseSprayWater", stArea.waterItem.isCloseSprayWater);
    }

    if (stArea.isEnableCoverItem) {
        const TiXmlElement *pobjCoverItem = pobjArea->FirstChildElement("coverItem");
        R_ASSERT(pobjCoverItem != nullptr, -1);

        UINT32 type;
        LoadMatch_GetAttr_(pobjCoverItem->QueryUnsignedAttribute, "type", type);
        switch (type)
        {
            case 0:
                stArea.coverItem.type = ROAD_AREA_COVER_TYPE_DITCH;
                break;
            case 1:
                stArea.coverItem.type = ROAD_AREA_COVER_TYPE_WELL;
                break;
            default:
                return -1;
        }
    }

    if (stArea.isEnableSlopeItem) {
        const TiXmlElement *pobjSlopeItem = pobjArea->FirstChildElement("slopeItem");
        R_ASSERT(pobjSlopeItem != nullptr, -1);
        LoadMatch_GetAttr_(pobjSlopeItem->QueryBoolAttribute, "isCloseSlope", stArea.slopeItem.isCloseSlope);
    }

    if (stArea.isEnableDoorItem) {
        const TiXmlElement *pobjDoorItem = pobjArea->FirstChildElement("doorItem");
        R_ASSERT(pobjDoorItem != nullptr, -1);
        if (TIXML_SUCCESS != pobjDoorItem->QueryIntAttribute("type", &stArea.doorItem.type)) {
            stArea.doorItem.type = 0; // 兼容之前只有联动库房门的部署，默认是联动库房门
        }
        double doorHeight;
        if(TIXML_SUCCESS != pobjDoorItem->QueryDoubleAttribute("height", &doorHeight)) {
            doorHeight = 0.0; // 如果为非库房门，则这个参数不使用，默认高度是0m
        }
        stArea.doorItem.doorHeight = doorHeight;
        if (TIXML_SUCCESS != pobjDoorItem->QueryStringAttribute("id", &stArea.doorItem.id)) {
            stArea.doorItem.id = "0"; // 默认id是0
        }
    }

    if (stArea.isEnableHeightItem) {
        const TiXmlElement *pobjHeightItem = pobjArea->FirstChildElement("heightItem");
        R_ASSERT(pobjHeightItem != nullptr, -1);

        double height;
        LoadMatch_GetAttr_(pobjHeightItem->QueryDoubleAttribute, "height", height);
        stArea.heightItem.maxHeight = height;

        stArea.heightItem.minHeight = -5.0f;
        if (TIXML_SUCCESS != pobjHeightItem->QueryFloatAttribute("low", &stArea.heightItem.minHeight)) {
            ST_LOG_ERR("Load minHeight Fail, use default value %f.", stArea.heightItem.minHeight);
        }

    }

    if (stArea.isEnableObstItem) {
        const TiXmlElement *pobjObstItem = pobjArea->FirstChildElement("obstItem");
        R_ASSERT(pobjObstItem != nullptr, -1);
        // 默认赋值为false
        stArea.obstItem.isIgnoreAllObst = false;
        if (TIXML_SUCCESS != pobjObstItem->QueryBoolAttribute("isIgnoreAllObst", &stArea.obstItem.isIgnoreAllObst)) {
            ST_LOG_ERR("Load isIgnoreAllObst Fail, use default value %u.", stArea.obstItem.isIgnoreAllObst);
        }
    }

    if (stArea.isEnableRangeItem) {
        const TiXmlElement *pobjRangeItem = pobjArea->FirstChildElement("rangeItem");
        R_ASSERT(pobjRangeItem != nullptr, -1);
        double range;
        LoadMatch_GetAttr_(pobjRangeItem->QueryDoubleAttribute, "range", range);
        stArea.rangeItem.range = range;
    }

    if (stArea.isEnableTreelawnItem) {
        const TiXmlElement *pobjTreelawnItem = pobjArea->FirstChildElement("treelawnItem");
        R_ASSERT(pobjTreelawnItem != nullptr, -1);
        double height;
        LoadMatch_GetAttr_(pobjTreelawnItem->QueryDoubleAttribute, "height", height);
        stArea.treelawnItem.maxHeight = height;
    }

    if (stArea.isEnableTrafficControlItem) {
        const TiXmlElement *pobjTrafficControlItem = pobjArea->FirstChildElement("trafficControlItem");
        R_ASSERT(pobjTrafficControlItem != nullptr, -1);
        if (TIXML_SUCCESS != pobjTrafficControlItem->QueryIntAttribute("passageNum", &stArea.trafficControlItem.passageNum))
        {
            stArea.trafficControlItem.passageNum = 1;
        }
        if (TIXML_SUCCESS != pobjTrafficControlItem->QueryIntAttribute("passageTime", &stArea.trafficControlItem.passageTime))
        {
            stArea.trafficControlItem.passageTime = 18000;
        }
        const TiXmlElement *pobjItemNames = pobjTrafficControlItem->FirstChildElement("name");
        while (pobjItemNames != nullptr)
        {
            const char* langId = pobjItemNames->Attribute("id");
            const char* nameText = pobjItemNames->Attribute("text");
            if (langId != nullptr && nameText != nullptr)
            {
                stArea.trafficControlItem.names[langId] = nameText;
            }
            pobjItemNames = pobjItemNames->NextSiblingElement("name");
        }
    }

    if (stArea.isEnableRubberControlItem) {
        const TiXmlElement *pobjRubberControlItem = pobjArea->FirstChildElement("rubberControlItem");
        R_ASSERT(pobjRubberControlItem != nullptr, -1);
        if (TIXML_SUCCESS != pobjRubberControlItem->QueryIntAttribute("gridBaffleLiftingTime", &stArea.rubberControlItem.gridBaffleLiftingTime)) {
            ST_LOG_ERR("Load rubberControlItem.gridBaffleLiftingTime Fail, use default value .");
            stArea.rubberControlItem.gridBaffleLiftingTime = 0;
        }
        ST_LOG_INFO("stArea.rubberControlItem.gridBaffleLiftingTime :%d ", stArea.rubberControlItem.gridBaffleLiftingTime);

        if (TIXML_SUCCESS != pobjRubberControlItem->QueryIntAttribute("gridBaffleLoweringTime", &stArea.rubberControlItem.gridBaffleLoweringTime)) {
            ST_LOG_ERR("Load rubberControlItem.gridBaffleLoweringTime Fail, use default value .");
            stArea.rubberControlItem.gridBaffleLoweringTime = 0;
        }
        ST_LOG_INFO("stArea.rubberControlItem.gridBaffleLoweringTime :%d ", stArea.rubberControlItem.gridBaffleLoweringTime);

        if (TIXML_SUCCESS != pobjRubberControlItem->QueryIntAttribute("gridBaffleDescentTime", &stArea.rubberControlItem.gridBaffleDescentTime)) {
            ST_LOG_ERR("Load rubberControlItem.gridBaffleDescentTime Fail, use default value .");
            stArea.rubberControlItem.gridBaffleDescentTime = 0;
        }
        ST_LOG_INFO("stArea.rubberControlItem.gridBaffleDescentTime :%d ", stArea.rubberControlItem.gridBaffleDescentTime);
    }

    if (stArea.isEnableRecognizeItem) {
        const TiXmlElement *pobjRecognizeItem = pobjArea->FirstChildElement("recognizeItem");
        const TiXmlElement *pobjTypes = pobjRecognizeItem->FirstChildElement("type");
        while (pobjTypes != nullptr)
        {
            int type_id;
            if (TIXML_SUCCESS == pobjTypes->QueryIntAttribute("id", &type_id)) {
                ST_LOG_INFO("stArea.recognizeItem.type :%d ", type_id);
                stArea.recognizeItem.types.emplace_back(type_id);
            }
            pobjTypes = pobjTypes->NextSiblingElement("type");
        }
    }

    if (stArea.isEnableNoUtralItem) {
        const TiXmlElement *pobNoUtralItem = pobjArea->FirstChildElement("utralItem");
        R_ASSERT(pobNoUtralItem != nullptr, -1);
        LoadMatch_GetAttr_(pobNoUtralItem->QueryBoolAttribute, "isEnableNoUtral", stArea.utralItem.isEnableNoUtral);
    }

    if (stArea.isEnableNoReverseItem) {
        const TiXmlElement *pobNoReverseItem = pobjArea->FirstChildElement("reverseItem");
        R_ASSERT(pobNoReverseItem != nullptr, -1);
        if (TIXML_SUCCESS != pobNoReverseItem->QueryBoolAttribute("isEnableNoReverse", &stArea.reverseItem.isEnableNoReverse)) {
            stArea.reverseItem.isEnableNoReverse = false;
        }
    }

    // 解析 JobRequirements 字段
    const TiXmlElement *pobjJobRequirements = pobjArea->FirstChildElement("JobRequirements");
    if (pobjJobRequirements != nullptr) {
        if (TIXML_SUCCESS != pobjJobRequirements->QueryUnsignedAttribute("edgeBrushOpen", &stArea.jobRequirements.edgeBrushOpen))
        {
            stArea.jobRequirements.edgeBrushOpen = 1;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryUnsignedAttribute("mainBrushOpen", &stArea.jobRequirements.mainBrushOpen))
        {
            stArea.jobRequirements.mainBrushOpen = 3;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryBoolAttribute("sprinklerOpening", &stArea.jobRequirements.sprinklerOpening))
        {
            stArea.jobRequirements.sprinklerOpening = true;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryUnsignedAttribute("leftWindOn", &stArea.jobRequirements.leftWindOn))
        {
            stArea.jobRequirements.leftWindOn = 0;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryUnsignedAttribute("rightWindOn", &stArea.jobRequirements.rightWindOn))
        {
            stArea.jobRequirements.rightWindOn = 0;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryUnsignedAttribute("linkageWarehouseDoor", &stArea.jobRequirements.linkageWarehouseDoor))
        {
            stArea.jobRequirements.linkageWarehouseDoor = 1;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryBoolAttribute("maskingPerception", &stArea.jobRequirements.maskingPerception))
        {
            stArea.jobRequirements.maskingPerception = false;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryDoubleAttribute("maximumSpeed", &stArea.jobRequirements.maximumSpeed))
        {
            stArea.jobRequirements.maximumSpeed = 0.5;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryDoubleAttribute("objectsObstructHeight", &stArea.jobRequirements.objectsObstructHeight))
        {
            stArea.jobRequirements.objectsObstructHeight = 0.5;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryBoolAttribute("shieldNegativeHeight", &stArea.jobRequirements.shieldNegativeHeight))
        {
            stArea.jobRequirements.shieldNegativeHeight = false;
        }
        if (TIXML_SUCCESS != pobjJobRequirements->QueryUnsignedAttribute("shieldingHighAltitudeObjects", &stArea.jobRequirements.shieldingHighAltitudeObjects))
        {
            stArea.jobRequirements.shieldingHighAltitudeObjects = 0;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载区域列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadAreas(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_AREA_STRU> &vectArea) const
{
    for (const TiXmlElement *pobjGoal = pobjRoad->FirstChildElement("area");
        pobjGoal != NULL;
        pobjGoal = pobjGoal->NextSiblingElement("area")) {
        ROAD_AREA_STRU stArea;
        INT32 rslt = LoadRoadArea(roadNo, pobjGoal, stArea);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoadArea(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load Area Succ. RoadNo=%u, AreaId=%u.", roadNo, stArea.id);
        vectArea.push_back(stArea);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, AreaNum=%u.", roadNo, UINT32(vectArea.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载路段路径点
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadPoints(const TiXmlElement *pobjPoints, std::vector<COORDINATE_2D_STRU> &vectPoints) const 
{
    const TiXmlElement* pobjPoint = pobjPoints->FirstChildElement("Point");
    while (pobjPoint != nullptr) {
        COORDINATE_2D_STRU point;
        double x, y;
        if (TIXML_SUCCESS != pobjPoint->QueryDoubleAttribute("X", &x) ||
            TIXML_SUCCESS != pobjPoint->QueryDoubleAttribute("Y", &y)) {
            ST_LOG_ERR("Failed to get point attributes.");
            return -1;
        }
        point.x = static_cast<float>(x);
        point.y = static_cast<float>(y);
        vectPoints.push_back(point); // 使用 vectPoints
        pobjPoint = pobjPoint->NextSiblingElement("Point");
    }
    return 0; // 返回成功
}

/**************************************************************************************
功能描述: 加载路段路径点
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadAnchorPoints(const TiXmlElement *pobjAnchorPoints, std::vector<COORDINATE_2D_STRU> &vectAnchorPoints) const 
{
    const TiXmlElement* pobjAnchorPoint = pobjAnchorPoints->FirstChildElement("AnchorPoint");
    while (pobjAnchorPoint != nullptr) {
        COORDINATE_2D_STRU anchorPoint;
        double x, y;
        if (TIXML_SUCCESS != pobjAnchorPoint->QueryDoubleAttribute("X", &x) ||
            TIXML_SUCCESS != pobjAnchorPoint->QueryDoubleAttribute("Y", &y)) {
            ST_LOG_ERR("Failed to get anchor point attributes.");
            return -1;
        }
        anchorPoint.x = static_cast<float>(x);
        anchorPoint.y = static_cast<float>(y);
        vectAnchorPoints.push_back(anchorPoint); // 使用 vectAnchorPoints
        pobjAnchorPoint = pobjAnchorPoint->NextSiblingElement("AnchorPoint");
    }
    return 0; // 返回成功
}

/**************************************************************************************
功能描述: 加载交叉路口
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadCrossing(const TiXmlElement *pobjCrossing, ROAD_CROSSING_STRU &stCrossing) const 
{
    // 解析交叉口 ID
    if (TIXML_SUCCESS != pobjCrossing->QueryUnsignedAttribute("id", &stCrossing.id)) {
        ST_LOG_ERR("Failed to get Crossing ID.");
        stCrossing.id = 0; // 设置默认值或根据需要处理
        // return -1; // 如果需要返回错误，可以取消注释这一行
    }

    // 处理 Points 字段
    const TiXmlElement* pobjPoints = pobjCrossing->FirstChildElement("Points");
    stCrossing.points.clear(); // 清空 points
    if (pobjPoints != nullptr) {
        const TiXmlElement* pobjPoint = pobjPoints->FirstChildElement("Point");
        while (pobjPoint != nullptr) {
            COORDINATE_2D_STRU point;
            double x, y;
            if (TIXML_SUCCESS != pobjPoint->QueryDoubleAttribute("X", &x) ||
                TIXML_SUCCESS != pobjPoint->QueryDoubleAttribute("Y", &y)) {
                ST_LOG_ERR("Failed to get point attributes.");
                return -1;
            }
            point.x = static_cast<float>(x);
            point.y = static_cast<float>(y);
            stCrossing.points.push_back(point); // 使用 points
            pobjPoint = pobjPoint->NextSiblingElement("Point");
        }
    }

    // 处理 AnchorPoints 字段
    const TiXmlElement* pobjAnchorPoints = pobjCrossing->FirstChildElement("AnchorPoints");
    stCrossing.anchorPoints.clear(); // 清空 anchorPoints
    if (pobjAnchorPoints != nullptr) {
        const TiXmlElement* pobjAnchorPoint = pobjAnchorPoints->FirstChildElement("AnchorPoint");
        while (pobjAnchorPoint != nullptr) {
            COORDINATE_2D_STRU anchorPoint;
            double x, y;
            if (TIXML_SUCCESS != pobjAnchorPoint->QueryDoubleAttribute("X", &x) ||
                TIXML_SUCCESS != pobjAnchorPoint->QueryDoubleAttribute("Y", &y)) {
                ST_LOG_ERR("Failed to get anchor point attributes.");
                return -1;
            }
            anchorPoint.x = static_cast<float>(x);
            anchorPoint.y = static_cast<float>(y);
            stCrossing.anchorPoints.push_back(anchorPoint); // 使用 anchorPoints
            pobjAnchorPoint = pobjAnchorPoint->NextSiblingElement("AnchorPoint");
        }
    }

    ST_LOG_INFO("LoadCrossing succ!");
    return 0; // 返回成功
}

/**************************************************************************************
功能描述: 加载交叉路口列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadCrossingList(const UINT32 roadNo, const TiXmlElement *pobjCrossings, std::vector<ROAD_CROSSING_STRU> &vectCrossing) const 
{
    vectCrossing.clear(); // 清空 vectCrossing

    const TiXmlElement* pobjCrossing = pobjCrossings->FirstChildElement("Crossings");
    while (pobjCrossing != nullptr) {
        ROAD_CROSSING_STRU stCrossing;
        if (LoadCrossing(pobjCrossing, stCrossing) != 0) {
            ST_LOG_ERR("Failed to load a Crossing.");
            return -1;
        }
        vectCrossing.push_back(stCrossing);
        pobjCrossing = pobjCrossing->NextSiblingElement("Crossings");
    }

    ST_LOG_INFO("LoadCrossingList succ!");
    return 0; // 返回成功
}

/**************************************************************************************
功能描述: 加载停车线
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadStopLine(UINT32 roadNo, const TiXmlElement *pobjStopLine, ROAD_STOP_LINE_STRU &stStopLine) const
{
    if (TIXML_SUCCESS != pobjStopLine->QueryUnsignedAttribute("id", &stStopLine.id)) {
        ST_LOG_ERR("Get Attr <id> Fail.");
        return -1;
    }

    std::string strStopLineFilePath = objSceneFileMng_.GetRoadStopLineFilePath(roadNo, stStopLine.id);
    INT32 rslt = objLineFile_.Read(strStopLineFilePath, stStopLine.vectPoint);
    if (0 != rslt) {
        ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strStopLineFilePath.c_str());
        return -1;
    }

    /* 锚点文件可能不存在 */
    std::string strStopLineAnchorPointFilePath = objSceneFileMng_.GetRoadStopLineAnchorPointFilePath(roadNo, stStopLine.id);
    if (objFileOprt_.IsExist(strStopLineAnchorPointFilePath)) {
        rslt = objLineFile_.Read(strStopLineAnchorPointFilePath, stStopLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strStopLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    if (TIXML_SUCCESS != pobjStopLine->QueryUnsignedAttribute("areaId", &stStopLine.areaId)) {
        ST_LOG_ERR("Get Attr <areaId> Fail.");
        return -1;
    }

    for (const TiXmlElement *pobjRouteId = pobjStopLine->FirstChildElement("routeId");
             pobjRouteId != NULL; pobjRouteId = pobjRouteId->NextSiblingElement("routeId")) {
        UINT32 RouteId;
        if (TIXML_SUCCESS != pobjRouteId->QueryUnsignedAttribute("id", &RouteId)) {
            ST_LOG_ERR("Get Attr <RouteId> Fail.");
            return -1;
        }

        stStopLine.vectRouteId.push_back(RouteId);
        ST_LOG_INFO("Load Routeid Succ. RouteId=%u.", RouteId);
    }

    /* 兼容旧版部署工具生成的xml */
    UINT32 routeOld;
    if (TIXML_SUCCESS == pobjStopLine->QueryUnsignedAttribute("routeId", &routeOld)) {
        if (stStopLine.vectRouteId.empty()) {
            stStopLine.vectRouteId.push_back(routeOld);
        }
    }

    if (TIXML_SUCCESS != pobjStopLine->QueryUnsignedAttribute("trafficLightId", &stStopLine.trafficLightId)) {
        ST_LOG_ERR("Get Attr <trafficLightId> Fail.");
        stStopLine.trafficLightId = 10000;
    }

    if (TIXML_SUCCESS != pobjStopLine->QueryUnsignedAttribute("type", &stStopLine.type)) {
        ST_LOG_ERR("Get Attr <type> Fail.");
        stStopLine.type = 0;
    }

    if (TIXML_SUCCESS != pobjStopLine->QueryDoubleAttribute("greenLightSlowMaxTime", &stStopLine.greenLightSlowMaxTime)) {
        ST_LOG_WARN("Get greenLightSlowMaxTime Failed.");
        stStopLine.greenLightSlowMaxTime = 15.0;
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载停车线列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadStopLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_STOP_LINE_STRU> &vectStopLine) const
{
    for (const TiXmlElement *pobjLine = pobjRoad->FirstChildElement("stopLine");
         pobjLine != NULL;
         pobjLine = pobjLine->NextSiblingElement("stopLine")) {
        ROAD_STOP_LINE_STRU stLine;
        INT32 rslt = LoadStopLine(roadNo, pobjLine, stLine);
        if (rslt != 0) {
            ST_LOG_ERR("LoadStopLine(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load StopLine Succ. RoadNo[%u], LineId[%u], LineType[%u], LineLen[%u], AnchorPointSize[%u], AreaId[%u], trafficLightId[%u]", 
                    roadNo, stLine.id, stLine.type, UINT32(stLine.vectPoint.size()), UINT32(stLine.vectAnchorPoint.size()), 
                    stLine.areaId, stLine.trafficLightId);
        vectStopLine.push_back(stLine);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, LineNum=%u.", roadNo, UINT32(vectStopLine.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载车道线
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadLaneLine(UINT32 roadNo, const TiXmlElement *pobjLaneLine, ROAD_LANE_LINE_STRU &stLaneLine) const
{
    if (TIXML_SUCCESS != pobjLaneLine->QueryUnsignedAttribute("id", &stLaneLine.id)) {
        ST_LOG_ERR("Get Attr <id> Fail.");
        return -1;
    }

    std::string strLaneLineFilePath = objSceneFileMng_.GetRoadLaneLineFilePath(roadNo, stLaneLine.id);
    INT32 rslt = objLineFile_.Read(strLaneLineFilePath, stLaneLine.vectPoint);
    if (0 != rslt) {
        ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strLaneLineFilePath.c_str());
        return -1;
    }

    /* 锚点文件可能不存在 */
    std::string strLaneLineAnchorPointFilePath = objSceneFileMng_.GetRoadLaneLineAnchorPointFilePath(roadNo, stLaneLine.id);
    if (objFileOprt_.IsExist(strLaneLineAnchorPointFilePath)) {
        rslt = objLineFile_.Read(strLaneLineAnchorPointFilePath, stLaneLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Read(%s) Fail.", strLaneLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    if (TIXML_SUCCESS != pobjLaneLine->QueryUnsignedAttribute("type", &stLaneLine.type)) {
        ST_LOG_ERR("Get Attr <type> Fail.");
        stLaneLine.type = 0;
    }

    return 0;
}

/**************************************************************************************
功能描述: 加载车道线列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadLaneLineList(UINT32 roadNo, const TiXmlElement *pobjRoad, vector<ROAD_LANE_LINE_STRU> &vectLaneLine) const
{
    for (const TiXmlElement *pobjLine = pobjRoad->FirstChildElement("laneLine");
         pobjLine != NULL;
         pobjLine = pobjLine->NextSiblingElement("laneLine")) {
        ROAD_LANE_LINE_STRU stLine;
        INT32 rslt = LoadLaneLine(roadNo, pobjLine, stLine);
        if (rslt != 0) {
            ST_LOG_ERR("LoadLaneLine(RoadNo=%u) Fail.", roadNo);
            return -1;
        }

        ST_LOG_INFO("Load LaneLine Succ. RoadNo[%u], LineId[%u], LineType[%u], LineLen[%u], AnchorPointSize[%u]", 
                    roadNo, stLine.id, stLine.type, UINT32(stLine.vectPoint.size()), 
                    UINT32(stLine.vectAnchorPoint.size()));
        vectLaneLine.push_back(stLine);
    }

    ST_LOG_INFO("Succ. RoadNo=%u, LineNum=%u.", roadNo, UINT32(vectLaneLine.size()));
    return 0;
}

/**************************************************************************************
功能描述: 加载道路信息
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoad(const TiXmlElement *pobjRoad, ROAD_INFO_STRU &stRoadInfo) const
{
    INT32 rslt = LoadRoadAttribute(pobjRoad, stRoadInfo);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadAttribute() Fail. RoadNo=%u.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadCarType(pobjRoad, stRoadInfo.carType);
    if (rslt != 0) {
        ST_LOG_ERR("CarType is err!");
        return -1;
    }

    // 处理 Points 字段
    const TiXmlElement* pobjPoints = pobjRoad->FirstChildElement("Points");
    if (pobjPoints != nullptr) {
        ST_LOG_INFO("find the points");
        INT32 rslt = LoadPoints(pobjPoints, stRoadInfo.vectPoints); // 直接加载 Points
        if (rslt != 0) {
            ST_LOG_ERR("LoadPoints() Fail. RoadNo=%u.", stRoadInfo.no);
            return -1;
        }
    }

    // 处理 AnchorPoints 字段
    const TiXmlElement* pobjAnchorPoints = pobjRoad->FirstChildElement("AnchorPoints");
    if (pobjAnchorPoints != nullptr) {
        ST_LOG_INFO("find the anchor points");
        INT32 rslt = LoadAnchorPoints(pobjAnchorPoints, stRoadInfo.vectAnchorPoints); // 直接加载 AnchorPoints
        if (rslt != 0) {
            ST_LOG_ERR("LoadAnchorPoints() Fail. RoadNo=%u.", stRoadInfo.no);
            return -1;
        }
    }

    rslt = LoadCrossingList(stRoadInfo.no, pobjRoad, stRoadInfo.vectCrossing);
    if (rslt != 0) {
        ST_LOG_ERR("LoadCrossingList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadLaneLineList(stRoadInfo.no, pobjRoad, stRoadInfo.vectLaneLine);
    if (rslt != 0) {
        ST_LOG_ERR("LoadLaneLineList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadRoadRouteList(stRoadInfo.no, pobjRoad, stRoadInfo.vectRoute);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadRouteList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadRoadPoses(stRoadInfo.no, pobjRoad, stRoadInfo.vectPose);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadPoses(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadRoadGoals(stRoadInfo.no, pobjRoad, stRoadInfo.vectGoal);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadGoals(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadRoadGoalAreas(stRoadInfo.no, pobjRoad, stRoadInfo.vectGoalArea);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadGoalAreas(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadRoadLineList(stRoadInfo.no, pobjRoad, stRoadInfo.vectLine);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadLineList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadObstacleLineList(stRoadInfo.no, pobjRoad, stRoadInfo.vectObstacleline);
    if (rslt != 0) {
        ST_LOG_ERR("LoadObstacleLineList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadRoadAreas(stRoadInfo.no, pobjRoad, stRoadInfo.vectArea);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadAreas(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadStopLineList(stRoadInfo.no, pobjRoad, stRoadInfo.vectStopLine);
    if (rslt != 0) {
        ST_LOG_ERR("LoadStopLineList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    vector<ROAD_PORT_STRU> vectPort;
    rslt = LoadRoadPortList(stRoadInfo.no, pobjRoad, vectPort);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadPortList(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    rslt = LoadTrafficLightSpots(stRoadInfo.no, pobjRoad, stRoadInfo.vectTrafficLightSpot);
    if (rslt != 0) {
        ST_LOG_ERR("LoadTrafficLightSpots(RoadNo=%u) Fail.", stRoadInfo.no);
        return -1;
    }

    /* 去除无用的外部端口 */
    GetValidPortList(stRoadInfo, vectPort, stRoadInfo.vectPort);

    UINT32 outerPortNum = 0;
    for (const ROAD_PORT_STRU &stPort : stRoadInfo.vectPort) {
        if (stPort.IsOuterPort()) {
            outerPortNum++;
        }
    }

    /* 判断是否多车道 */
    UINT32 limitLineNum = stRoadInfo.vectLine.size();
    stRoadInfo.isMultiLane = ((outerPortNum == 2) && (limitLineNum > 2));
    ST_LOG_INFO_IF(stRoadInfo.isMultiLane, "Road %u is multi lane, limeLineNum %u.", stRoadInfo.no, limitLineNum);
    /* 检查 */

    /* 斑马线等暂不处理 */
    ST_LOG_INFO("Succ. RoadNo=%u, IsOnlyForTask=%d, IsTrunAround=%d.", stRoadInfo.no, stRoadInfo.isOnlyForTask, stRoadInfo.isTurnAround);
    return 0;
}

/**************************************************************************************
功能描述: 加载道路列表
修改记录:
**************************************************************************************/
INT32 ROAD_NETWORK_FILE_C::LoadRoadList(const TiXmlElement *pobjRoads, vector<ROAD_INFO_STRU> &vectRoad) const
{
    for (const TiXmlElement *pobjRoad = pobjRoads->FirstChildElement("road");
         pobjRoad != NULL;
         pobjRoad = pobjRoad->NextSiblingElement("road")) {

        ROAD_INFO_STRU stRoadInfo;
        INT32 rslt = LoadRoad(pobjRoad, stRoadInfo);
        if (rslt != 0) {
            ST_LOG_ERR("LoadRoad() Fail.");
            return -1;
        }

        if (stRoadInfo.vectLine.empty()) {
            ST_LOG_ERR("Road %u has no limit line, road will be discarded.", stRoadInfo.no);
            ST_LOG_ERR("路段 %u 没有任何限制线, 路段将被丢弃.", stRoadInfo.no);
        } else {
            vectRoad.push_back(stRoadInfo);
        }
    }

    ST_LOG_INFO("Load %u roads Succ.", UINT32(vectRoad.size()));
    return 0;
}

INT32 ROAD_NETWORK_FILE_C::LoadRoadType(const TiXmlElement *pobjMatch, ROAD_TYPE_ENUM &enRoadType) const
{
    UINT32 roadType = 0;
    if (   (TIXML_SUCCESS != pobjMatch->QueryUnsignedAttribute("type",     &roadType))
        && (TIXML_SUCCESS != pobjMatch->QueryUnsignedAttribute("roadType", &roadType))) {
        ST_LOG_ERR("Load Road Type Fail, use default roadType %u.", roadType);
//        return -1;
    }

    switch (roadType) {
        case 0:
            enRoadType = ROAD_LANE;
            break;

        case 1:
            enRoadType = ROAD_NON_LANE;
            break;

        case 2:
            enRoadType = ROAD_SERVICE;
            break;

        default:
            ST_LOG_ERR("Unknown Road Type(%u).", roadType);
            return -1;
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::LoadCarType(const TiXmlElement *pobjMatch, ROAD_CARTYPE_ENUM &enCarType) const
{
    UINT32 carType = 0;
    if (TIXML_SUCCESS != pobjMatch->QueryUnsignedAttribute("carType",  &carType)) {
        ST_LOG_ERR("Load Car Type(%u) Fail.", carType);
//        return -1;
    }

    switch (carType) {
        case 0:
            enCarType = ROAD_CARTYPE_320;
            break;

        case 1:
            enCarType = ROAD_CARTYPE_330;
            break;
        case 2:
            enCarType = ROAD_CARTYPE_520;
            break;
        case 3:
            enCarType = ROAD_CARTYPE_350;
            break;

        default:
            ST_LOG_ERR("Unknown Car Type(%u).", carType);
            return -1;
    }

    return 0;
}

// INT32 ROAD_NETWORK_FILE_C::LoadRoadAttribute(const TiXmlElement *pobjMatch, UINT32 &maxAccessCarNum, UINT32 &isNarrow, BOOL &isEasyTrafficJam, BOOL &isControl, BOOL &isNormal) const
// {
//     if (TIXML_SUCCESS != pobjMatch->QueryUnsignedAttribute("maxAccessCarNum", &maxAccessCarNum)) {
//         maxAccessCarNum = 999;
//         ST_LOG_ERR("Load Road maxAccessCarNum Fail, use default maxAccessCarNum %u.", maxAccessCarNum);
//     }

//     if (maxAccessCarNum == 0) {
//         ST_LOG_ERR("Road maxAccessCarNum %u Err, must be bigger than 0.", maxAccessCarNum);
//         return -1;
//     }

//     if (TIXML_SUCCESS != pobjMatch->QueryUnsignedAttribute("isNarrow", &isNarrow)) {
//         isNarrow = 0;
//         ST_LOG_ERR("Load Road isNarrow Fail, use default value: 0.");
//     }

//     if (TIXML_SUCCESS != pobjMatch->QueryBoolAttribute("isEasyTrafficJam", &isEasyTrafficJam)) {
//         isEasyTrafficJam = false;
//         ST_LOG_ERR("Load Road isEasyTrafficJam Fail, use default value: false.");
//     }

//     if (TIXML_SUCCESS != pobjMatch->QueryBoolAttribute("isControl", &isControl)) {
//         isControl = false;
//         ST_LOG_ERR("Load Road isControl Fail, use default value: false.");
//     }

//     if (TIXML_SUCCESS != pobjMatch->QueryBoolAttribute("isNormal", &isNormal)) {
//         isNormal = false;
//         ST_LOG_ERR("Load Road isNormal Fail, use default value: false.");
//     }    

//     return 0;
// }

INT32 ROAD_NETWORK_FILE_C::LoadRoadAttribute(const TiXmlElement *pobjRoad, ROAD_INFO_STRU &stRoadInfo) const
{
    if (TIXML_SUCCESS != pobjRoad->QueryUnsignedAttribute("no", &stRoadInfo.no)) {
        ST_LOG_ERR("Get Attr <no> Fail.");
        return -1;
    }

    INT32 rslt = LoadRoadType(pobjRoad, stRoadInfo.roadType);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadType() Fail. RoadNo=%u.", stRoadInfo.no);
        return -1;
    }

    if (TIXML_SUCCESS != pobjRoad->QueryUnsignedAttribute("maxAccessCarNum", &stRoadInfo.maxAccessCarNum)) {
        stRoadInfo.maxAccessCarNum = 999;
        ST_LOG_ERR( "Load Road(%u) maxAccessCarNum Fail, use default maxAccessCarNum %u.", stRoadInfo.no, stRoadInfo.maxAccessCarNum);
    }

    if (stRoadInfo.maxAccessCarNum == 0) {
        ST_LOG_ERR("Road(%u) maxAccessCarNum %u Err, must be bigger than 0.", stRoadInfo.no, stRoadInfo.maxAccessCarNum);
        return -1;
    }

    if (TIXML_SUCCESS != pobjRoad->QueryUnsignedAttribute("isNarrow", &stRoadInfo.isNarrow)) {
        stRoadInfo.isNarrow = 0;
        ST_LOG_ERR("Load Road(%u) isNarrow Fail, use default value: 0.", stRoadInfo.no);
    }

    if (TIXML_SUCCESS != pobjRoad->QueryBoolAttribute("isEasyTrafficJam", &stRoadInfo.isEasyTrafficJam)) {
        stRoadInfo.isEasyTrafficJam = false;
        ST_LOG_ERR("Load Road(%u) isEasyTrafficJam Fail, use default value: false.", stRoadInfo.no);
    }

    if (TIXML_SUCCESS != pobjRoad->QueryBoolAttribute("isOnlyForTask", &stRoadInfo.isOnlyForTask)) {
        stRoadInfo.isOnlyForTask = false;
        ST_LOG_ERR("Load Road(%u) isOnlyForTask Fail, use default value: false.", stRoadInfo.no);
    }

    if (TIXML_SUCCESS != pobjRoad->QueryBoolAttribute("isTrunAround", &stRoadInfo.isTurnAround)) {
        stRoadInfo.isTurnAround = true;
        ST_LOG_ERR("Load Road(%u) isTurnAround Fail, use default value: true.", stRoadInfo.no);
    }

    if (stRoadInfo.roadType == ROAD_LANE) {
        if (TIXML_SUCCESS != pobjRoad->QueryBoolAttribute("isServiceRoadExist", &stRoadInfo.serviceRoadStatus.isServiceRoadExist)) {
            stRoadInfo.serviceRoadStatus.isServiceRoadExist = false;
            ST_LOG_ERR("Load Road(%u) isServiceRoadExist Fail, use default value: false.", stRoadInfo.serviceRoadStatus.isServiceRoadExist);
        }
        if (stRoadInfo.serviceRoadStatus.isServiceRoadExist) {
            if (TIXML_SUCCESS != pobjRoad->QueryUnsignedAttribute("serviceRoadNo", &stRoadInfo.serviceRoadStatus.serviceRoadNo)) {
                ST_LOG_ERR("Road(%u) serviceRoadNo %u Failed.", stRoadInfo.no, stRoadInfo.serviceRoadStatus.serviceRoadNo);
                return -1;
            }
        }
    }

    return 0;
}


INT32 ROAD_NETWORK_FILE_C::LoadFromFile(ROAD_NETWORK_INFO_STRU &stRoadNetwork) const
{
    std::string        strRoadsInfoFilePath = objSceneFileMng_.GetRoadsInfoFilePath();
    TiXmlDocument objXmlFile(strRoadsInfoFilePath.c_str());
    if (!objXmlFile.LoadFile()) {
        ST_LOG_ERR("Load File(%s) Fail.", strRoadsInfoFilePath.c_str());
        return -1;
    }

    TiXmlElement *pobjRoads = objXmlFile.RootElement();
    if (pobjRoads == NULL) {
        ST_LOG_ERR("Get Root Element Fail.");
        return -1;
    }

    INT32 rslt = LoadRoadList(pobjRoads, stRoadNetwork.vectRoad);
    if (rslt != 0) {
        ST_LOG_ERR("LoadRoadList() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

void ROAD_NETWORK_FILE_C::SavePort(const ROAD_PORT_STRU &stPort, TiXmlElement *pobjPort) const
{
    pobjPort->SetAttribute("id", stPort.id);
}

INT32 ROAD_NETWORK_FILE_C::SavePortList(const vector<ROAD_PORT_STRU> &vectPort, TiXmlElement *pobjRoad) const
{
    for (const ROAD_PORT_STRU &stPort :vectPort) {
        TiXmlElement *pobjPort = new TiXmlElement("port");
        if (pobjPort == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjPort);

        SavePort(stPort, pobjPort);
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveLine(UINT32 roadNo, const ROAD_LINE_STRU &stLine, TiXmlElement *pobjLine) const
{
    pobjLine->SetAttribute("id",        stLine.id);
    pobjLine->SetAttribute("startPort", stLine.stDir.startPortId);
    pobjLine->SetAttribute("stopPort",  stLine.stDir.stopPortId);

    std::string strLineFilePath = objSceneFileMng_.GetRoadLineFilePath(roadNo, stLine.id);
    INT32 rslt = objLineFile_.Write(strLineFilePath, stLine.vectPoint);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strLineFilePath.c_str());
        return -1;
    }

    /* 有锚点时才生成文件 */
    if (stLine.vectAnchorPoint.size() > 0) {
        std::string strLineAnchorPointFilePath = objSceneFileMng_.GetRoadLineAnchorPointFilePath(roadNo, stLine.id);
        rslt = objLineFile_.Write(strLineAnchorPointFilePath, stLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveLineList(UINT32 roadNo, const vector<ROAD_LINE_STRU> &vectLine, TiXmlElement *pobjRoad) const
{
    for (const ROAD_LINE_STRU &stLine :vectLine) {
        TiXmlElement *pobjLine = new TiXmlElement("line");
        if (pobjLine == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjLine);

        INT32 rslt = SaveLine(roadNo, stLine, pobjLine);
        if (rslt != 0) {
            ST_LOG_ERR("SaveLine(RoadNo=%u, LineId=%u) Fai.", roadNo, stLine.id);
            return -1;
        }
    }

    return 0;
}

void ROAD_NETWORK_FILE_C::SaveLimitLine(UINT32 limitLine, TiXmlElement *pobjLimitLine) const
{
    pobjLimitLine->SetAttribute("id", limitLine);
}

INT32 ROAD_NETWORK_FILE_C::SaveLimitLineList(const vector<UINT32> &vectLimitLine, TiXmlElement *pobjRoute) const
{
    for (const UINT32 &limitLine : vectLimitLine) {
        TiXmlElement *pobjLimitLine = new TiXmlElement("limitLine");
        if (pobjLimitLine == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoute->LinkEndChild(pobjLimitLine);

        SaveLimitLine(limitLine, pobjLimitLine);
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveRoute(UINT32 roadNo, const ROAD_ROUTE_STRU &stRoute, TiXmlElement *pobjRoute) const
{
    pobjRoute->SetAttribute("id",           stRoute.id);
    pobjRoute->SetAttribute("startPort",    stRoute.stDir.startPortId);
    pobjRoute->SetAttribute("stopPort",     stRoute.stDir.stopPortId);
    pobjRoute->SetAttribute("isGoForward",  stRoute.isGoForward);
    pobjRoute->SetAttribute("isNarrow",     stRoute.isNarrow);
    pobjRoute->SetAttribute("avoidType",    stRoute.avoidType);
    pobjRoute->SetAttribute("type",         stRoute.enRouteAlongType);

    INT32 rslt = SaveLimitLineList(stRoute.vecLimitLineId, pobjRoute);
    if (rslt != 0) {
        ST_LOG_ERR("SaveLimitLineList(RoadNo=%u, RouteId=%u) Fail.", roadNo, stRoute.id);
        return -1;
    }

    std::string typesStr;
    for (const UINT32 &type : stRoute.Types) {
        if (!typesStr.empty()) {
            typesStr += ",";
        }
        typesStr += std::to_string(type);
    }
    pobjRoute->SetAttribute("Types", typesStr.c_str());
    ST_LOG_INFO("Save Route Types succ. RoadNo=%u, RouteId=%u, Types=%s", 
                roadNo, stRoute.id, typesStr.c_str());

    std::string strRouteFilePath = objSceneFileMng_.GetRoadRouteFilePath(roadNo, stRoute.id);
    rslt = objLineFile_.Write(strRouteFilePath, stRoute.vectPoint);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRouteFilePath.c_str());
        return -1;
    }

    /* 有锚点时才生成文件 */
    if (stRoute.vectAnchorPoint.size() > 0) {
        std::string strRouteAnchorPointFilePath = objSceneFileMng_.GetRoadRouteAnchorPointFilePath(roadNo, stRoute.id);
        rslt = objLineFile_.Write(strRouteAnchorPointFilePath, stRoute.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRouteAnchorPointFilePath.c_str());
            return -1;
        }
    }

    pobjRoute->SetAttribute("isLockStart",              stRoute.isLockStart);
    pobjRoute->SetAttribute("isInCrossingArea",         stRoute.isInCrossingArea);
    pobjRoute->SetAttribute("isReverseDriving",         stRoute.isReverseDriving);
    pobjRoute->SetAttribute("isIgnoreBodySideObst",     stRoute.isIgnoreBodySideObst);
    pobjRoute->SetAttribute("isBlow",                   stRoute.clean_mode.is_blow);
    pobjRoute->SetAttribute("isWatering",               stRoute.clean_mode.is_spray);
    pobjRoute->SetAttribute("isStretchBrush",           stRoute.clean_mode.is_stretch_brush);

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveRouteList(UINT32 roadNo, const vector<ROAD_ROUTE_STRU> &vectRoute, TiXmlElement *pobjRoad) const
{
    for (const ROAD_ROUTE_STRU &stRoute : vectRoute) {
        TiXmlElement *pobjRoute = new TiXmlElement("route");
        if (pobjRoute == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjRoute);

        INT32 rslt = SaveRoute(roadNo, stRoute, pobjRoute);
        if (rslt != 0) {
            ST_LOG_ERR("SaveRoute(RoadNo=%u, RouteId=%u) Fail.", roadNo, stRoute.id);
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveObstLine(UINT32 roadNo, const ROAD_OBSTACLE_LINE_STRU &stObstLine, TiXmlElement *pobjObstLine) const
{
    pobjObstLine->SetAttribute("id", stObstLine.id);

    std::string strRoadObstLine = objSceneFileMng_.GetRoadObstLineFilePath(roadNo, stObstLine.id);
    INT32 rslt = objLineFile_.Write(strRoadObstLine, stObstLine.vectPoint);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRoadObstLine.c_str());
        return -1;
    }

    /* 有锚点时才生成文件 */
    if (stObstLine.vectAnchorPoint.size() > 0) {
        std::string strObstLineAnchorPointFilePath = objSceneFileMng_.GetRoadObstLineAnchorPointFilePath(roadNo, stObstLine.id);
        rslt = objLineFile_.Write(strObstLineAnchorPointFilePath, stObstLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strObstLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveObstLineList(UINT32 roadNo, const vector<ROAD_OBSTACLE_LINE_STRU> &vectObstacleline, TiXmlElement *pobjRoad) const
{
    for (const ROAD_OBSTACLE_LINE_STRU &stObstLine : vectObstacleline) {
        TiXmlElement *pobjObstLine = new TiXmlElement("obstacleLine");
        if (pobjObstLine == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjObstLine);

        INT32 rslt = SaveObstLine(roadNo, stObstLine, pobjObstLine);
        if (rslt != 0) {
            ST_LOG_ERR("SaveObstLine(RoadNo=%u, ObstLineId=%u) Fail.", roadNo, stObstLine.id);
            return -1;
        }
    }

    return 0;
}

void ROAD_NETWORK_FILE_C::SaveRouteId(UINT32 routeId, TiXmlElement *pobjRouteId) const
{
    pobjRouteId->SetAttribute("id", routeId);
}

INT32 ROAD_NETWORK_FILE_C::SaveRouteIdList(const vector<UINT32> &vectRouteId, TiXmlElement *pobjStopLine) const
{

    for (const UINT32 &RouteId : vectRouteId) {
        TiXmlElement *pobjRouteId = new TiXmlElement("routeId");
        if (pobjRouteId == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjStopLine->LinkEndChild(pobjRouteId);

        SaveRouteId(RouteId, pobjRouteId);
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveStopLine(UINT32 roadNo, const ROAD_STOP_LINE_STRU &stStopLine, TiXmlElement *pobjStopLine) const
{
    pobjStopLine->SetAttribute("id", stStopLine.id);
    pobjStopLine->SetAttribute("areaId", stStopLine.areaId);
    pobjStopLine->SetAttribute("type", stStopLine.type);
    pobjStopLine->SetAttribute("trafficLightId", stStopLine.trafficLightId);
    pobjStopLine->SetAttribute("greenLightSlowMaxTime", stStopLine.greenLightSlowMaxTime);

    INT32 rslt = SaveRouteIdList(stStopLine.vectRouteId, pobjStopLine);
    if (rslt != 0) {
        ST_LOG_ERR("SaveRouteIdList(RoadNo=%u, StopLineId=%u) Fail.", roadNo, stStopLine.id);
        return -1;
    }

    std::string strRoadStopLine = objSceneFileMng_.GetRoadStopLineFilePath(roadNo, stStopLine.id);
    rslt = objLineFile_.Write(strRoadStopLine, stStopLine.vectPoint);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRoadStopLine.c_str());
        return -1;
    }

    /* 有锚点时才生成文件 */
    if (stStopLine.vectAnchorPoint.size() > 0) {
        std::string strStopLineAnchorPointFilePath = objSceneFileMng_.GetRoadStopLineAnchorPointFilePath(roadNo, stStopLine.id);
        rslt = objLineFile_.Write(strStopLineAnchorPointFilePath, stStopLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strStopLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveStopLineList(UINT32 roadNo, const vector<ROAD_STOP_LINE_STRU> &vectStopLine, TiXmlElement *pobjRoad) const
{
    for (const ROAD_STOP_LINE_STRU &stStopLine : vectStopLine) {
        TiXmlElement *pobjStopLine = new TiXmlElement("stopLine");
        if (pobjStopLine == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjStopLine);

        INT32 rslt = SaveStopLine(roadNo, stStopLine, pobjStopLine);
        if (rslt != 0) {
            ST_LOG_ERR("SaveStopLine(RoadNo=%u, StopLineId=%u) Fail.", roadNo, stStopLine.id);
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveLaneLine(UINT32 roadNo, const ROAD_LANE_LINE_STRU &stLaneLine, TiXmlElement *pobjLaneLine) const
{
    pobjLaneLine->SetAttribute("id", stLaneLine.id);
    pobjLaneLine->SetAttribute("type", stLaneLine.type);

    std::string strRoadLaneLine = objSceneFileMng_.GetRoadLaneLineFilePath(roadNo, stLaneLine.id);
    INT32 rslt = objLineFile_.Write(strRoadLaneLine, stLaneLine.vectPoint);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRoadLaneLine.c_str());
        return -1;
    }

    /* 有锚点时才生成文件 */
    if (stLaneLine.vectAnchorPoint.size() > 0) {
        std::string strLaneLineAnchorPointFilePath = objSceneFileMng_.GetRoadLaneLineAnchorPointFilePath(roadNo, stLaneLine.id);
        rslt = objLineFile_.Write(strLaneLineAnchorPointFilePath, stLaneLine.vectAnchorPoint);
        if (rslt != 0) {
            ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strLaneLineAnchorPointFilePath.c_str());
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveLaneLineList(UINT32 roadNo, const vector<ROAD_LANE_LINE_STRU> &vectLaneLine, TiXmlElement *pobjRoad) const
{
    for (const ROAD_LANE_LINE_STRU &stLaneLine : vectLaneLine) {
        TiXmlElement *pobjLaneLine = new TiXmlElement("laneLine");
        if (pobjLaneLine == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjLaneLine);

        INT32 rslt = SaveLaneLine(roadNo, stLaneLine, pobjLaneLine);
        if (rslt != 0) {
            ST_LOG_ERR("SaveLaneLine(RoadNo=%u, LaneLineId=%u) Fail.", roadNo, stLaneLine.id);
            return -1;
        }
    }

    return 0;
}

void ROAD_NETWORK_FILE_C::SaveGoal(const ROAD_GOAL_STRU &stGoal, TiXmlElement *pobjGoal) const
{
    // 设置 goal 元素的属性
    pobjGoal->SetAttribute("id", stGoal.id);
    pobjGoal->SetAttribute("type", stGoal.type);
    pobjGoal->SetAttribute("portId", stGoal.portId);
    pobjGoal->SetAttribute("priority", stGoal.priority);
    pobjGoal->SetDoubleAttribute("x", stGoal.goal.stPos.x);
    pobjGoal->SetDoubleAttribute("y", stGoal.goal.stPos.y);
    pobjGoal->SetDoubleAttribute("yaw", stGoal.goal.dirAngle);

    if (stGoal.type == ROAD_GOAL_MAP_CHANGE) {
        pobjGoal->SetDoubleAttribute("reachLimitRange", stGoal.changeMapGoalInfo.reachLimitRange);
        pobjGoal->SetAttribute("changeMapName", stGoal.changeMapGoalInfo.changeMapName);
        pobjGoal->SetAttribute("changeMapGoalPathId", stGoal.changeMapGoalInfo.changeMapGoalPathId);
    }

    // 保存 Types 属性
    std::string typesStr;
    for (const UINT32 &type : stGoal.Types) {
        if (!typesStr.empty()) {
            typesStr += ",";
        }
        typesStr += std::to_string(type);
    }
    pobjGoal->SetAttribute("Types", typesStr.c_str());

    // 保存 SiteNames
    TiXmlElement *pobjSiteNames = new TiXmlElement("SiteNames");
    for (const auto& name : stGoal.SiteNames) {
        pobjSiteNames->SetAttribute(name.first.c_str(), name.second.c_str());
    }
    pobjGoal->LinkEndChild(pobjSiteNames);
}

INT32 ROAD_NETWORK_FILE_C::SaveGoalList(const vector<ROAD_GOAL_STRU> &vectGoal, TiXmlElement *pobjRoad) const
{
    for (const ROAD_GOAL_STRU &stGoal : vectGoal) {
        TiXmlElement *pobjGoal = new TiXmlElement("goal");
        if (pobjGoal == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjGoal);

        SaveGoal(stGoal, pobjGoal);
    }

    return 0;
}

void ROAD_NETWORK_FILE_C::SaveTrafficLightSpot(const ROAD_TRAFFIC_LIGHT_SPOT_STRU &stTrafficLightSpot, 
                                               TiXmlElement *pobjTrafficLightSpot) const
{
    pobjTrafficLightSpot->SetAttribute("groupId",     stTrafficLightSpot.groupId);
    pobjTrafficLightSpot->SetAttribute("id",     stTrafficLightSpot.id);
    pobjTrafficLightSpot->SetAttribute("type",   stTrafficLightSpot.type);
    pobjTrafficLightSpot->SetDoubleAttribute("x",      stTrafficLightSpot.point.x);
    pobjTrafficLightSpot->SetDoubleAttribute("y",      stTrafficLightSpot.point.y);
    pobjTrafficLightSpot->SetDoubleAttribute("crossingTime",      stTrafficLightSpot.crossingTime);
    pobjTrafficLightSpot->SetDoubleAttribute("height",      stTrafficLightSpot.height);
    pobjTrafficLightSpot->SetDoubleAttribute("rowLength", stTrafficLightSpot.rowLength);
    pobjTrafficLightSpot->SetDoubleAttribute("colLength", stTrafficLightSpot.colLength);
    pobjTrafficLightSpot->SetDoubleAttribute("pixelBias", stTrafficLightSpot.pixelBias);
    pobjTrafficLightSpot->SetDoubleAttribute("boxBias", stTrafficLightSpot.boxBias);
}

INT32 ROAD_NETWORK_FILE_C::SaveTrafficLightSpotList(const vector<ROAD_TRAFFIC_LIGHT_SPOT_STRU> &vectTrafficLightSpot, 
                                                    TiXmlElement *pobjRoad) const
{
    for (const ROAD_TRAFFIC_LIGHT_SPOT_STRU &stTrafficLightSpot : vectTrafficLightSpot) {
        TiXmlElement *pobjTrafficLightSpot = new TiXmlElement("trafficLight");
        if (pobjTrafficLightSpot == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjTrafficLightSpot);

        SaveTrafficLightSpot(stTrafficLightSpot, pobjTrafficLightSpot);
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveGoalArea(const UINT32 &roadNo, const ROAD_GOAL_AREA_STRU &stGoalArea, TiXmlElement *pobjGoalArea) const
{
    pobjGoalArea->SetAttribute("id",     stGoalArea.id);
    pobjGoalArea->SetAttribute("type",   stGoalArea.type);

    std::string strRoadAreaFilePath = objSceneFileMng_.GetRoadGoalAreaFilePath(roadNo, stGoalArea.id);
    INT32 rslt = objLineFile_.Write(strRoadAreaFilePath, stGoalArea.vertexs);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRoadAreaFilePath.c_str());
        return -1;
    }

    for (const UINT32 &portId : stGoalArea.portIds) {
        TiXmlElement *pobjPort = new TiXmlElement("port");
        if (pobjPort == NULL) {
           ST_LOG_ERR("New TiXmlElement() Fail.");
           return -1;
        }
        pobjGoalArea->LinkEndChild(pobjPort);
        pobjPort->SetAttribute("id", portId);
    }

    // 保存 SiteNames
    TiXmlElement *pobjSiteNames = new TiXmlElement("SiteNames");
    for (const auto& name : stGoalArea.SiteNames) {
        pobjSiteNames->SetAttribute(name.first.c_str(), name.second.c_str());
    }
    pobjGoalArea->LinkEndChild(pobjSiteNames);

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveGoalAreaList(const UINT32 &roadNo, const vector<ROAD_GOAL_AREA_STRU> &vectGoalArea, TiXmlElement *pobjRoad) const
{
    for (const ROAD_GOAL_AREA_STRU &stGoalArea : vectGoalArea) {
        TiXmlElement *pobjGoalArea = new TiXmlElement("goalArea");
        if (pobjGoalArea == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjGoalArea);

        INT32 rslt= SaveGoalArea(roadNo, stGoalArea, pobjGoalArea);
        R_ASSERT(rslt == 0, -1);
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveArea(UINT32 roadNo, const ROAD_AREA_STRU &stArea, TiXmlElement *pobjArea) const
{
    pobjArea->SetAttribute("id", stArea.id);
    pobjArea->SetAttribute("type", stArea.type);
    pobjArea->SetAttribute("isEnableVelItem", stArea.isEnableVelItem);
    pobjArea->SetAttribute("isEnableBrushItem", stArea.isEnableBrushItem);
    pobjArea->SetAttribute("isEnableWaterItem", stArea.isEnableWaterItem);
    pobjArea->SetAttribute("isEnableCoverItem", stArea.isEnableCoverItem);
    pobjArea->SetAttribute("isEnableDoorItem", stArea.isEnableDoorItem);
    pobjArea->SetAttribute("isEnableCrossingItem", stArea.isEnableCrossingItem);
    pobjArea->SetAttribute("isEnableSlopeItem", stArea.isEnableSlopeItem);
    pobjArea->SetAttribute("isEnableHeightItem", stArea.isEnableHeightItem);
    pobjArea->SetAttribute("isEnableObstItem", stArea.isEnableObstItem);
    pobjArea->SetAttribute("isEnableRangeItem", stArea.isEnableRangeItem);
    pobjArea->SetAttribute("isEnableTreelawnItem", stArea.isEnableTreelawnItem);
    pobjArea->SetAttribute("isEnableStopExistObst", stArea.isEnableStopExistObst);
    pobjArea->SetAttribute("isEnableTrafficControlItem", stArea.isEnableTrafficControlItem);
    pobjArea->SetAttribute("isEnableRubberControlItem", stArea.isEnableRubberControlItem);
    pobjArea->SetAttribute("isEnableEntanglementItem", stArea.isEnableEntanglementItem);
    pobjArea->SetAttribute("isEnableRoadStakeItem", stArea.isEnableRoadStakeItem);
    pobjArea->SetAttribute("isEnableRecognizeItem", stArea.isEnableRecognizeItem);
    pobjArea->SetAttribute("isEnableNoUtralItem", stArea.isEnableNoUtralItem);
    pobjArea->SetAttribute("isEnableNoReverseItem", stArea.isEnableNoReverseItem);
    pobjArea->SetAttribute("isEnableCloseObstacle", stArea.isEnableCloseObstacle);
    pobjArea->SetAttribute("isEnableNotAllowedAutoNavigation", stArea.isEnableNotAllowedAutoNavigation);

    if (stArea.isEnableVelItem) {
        TiXmlElement *pobjVelItem = new TiXmlElement("velItem");
        if (pobjVelItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjVelItem->SetDoubleAttribute("slowDownVel", stArea.velItem.slowDownVel);
        pobjVelItem->SetAttribute("isOnlyEffectForClean", stArea.velItem.isOnlyEffectForClean);
        pobjArea->LinkEndChild(pobjVelItem);
    }

    if (stArea.isEnableBrushItem) {
        TiXmlElement *pobjBrushItem = new TiXmlElement("brushItem");
        if (pobjBrushItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjBrushItem->SetAttribute("isEffectInOutOfArea", stArea.brushItem.isEffectInOutOfArea);
        pobjBrushItem->SetAttribute("isCloseMainBrush", stArea.brushItem.isCloseMainBrush);
        pobjBrushItem->SetAttribute("isCloseSideBrush", stArea.brushItem.isCloseSideBrush);
        pobjArea->LinkEndChild(pobjBrushItem);
    }

    if (stArea.isEnableWaterItem) {
        TiXmlElement *pobjWaterItem = new TiXmlElement("waterItem");
        if (pobjWaterItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjWaterItem->SetAttribute("isCloseSprayWater", stArea.waterItem.isCloseSprayWater);
        pobjArea->LinkEndChild(pobjWaterItem);
    }

    if (stArea.isEnableCoverItem) {
        TiXmlElement *pobjCoverItem = new TiXmlElement("coverItem");
        if (pobjCoverItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjCoverItem->SetAttribute("type", stArea.coverItem.type);
        pobjArea->LinkEndChild(pobjCoverItem);
    }

    if (stArea.isEnableDoorItem) {
        TiXmlElement *pobjDoorItem = new TiXmlElement("doorItem");
        if (pobjDoorItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjDoorItem->SetDoubleAttribute("height", stArea.doorItem.doorHeight);
        pobjDoorItem->SetAttribute("type", stArea.doorItem.type);
        pobjDoorItem->SetAttribute("id", stArea.doorItem.id);
        pobjArea->LinkEndChild(pobjDoorItem);
    }

    if (stArea.isEnableSlopeItem) {
        TiXmlElement *pobjSlopeItem = new TiXmlElement("slopeItem");
        if (pobjSlopeItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjSlopeItem->SetAttribute("isCloseSlope", stArea.slopeItem.isCloseSlope);
        pobjArea->LinkEndChild(pobjSlopeItem);
    }

    if (stArea.isEnableHeightItem) {
        TiXmlElement *pobjHeightItem = new TiXmlElement("heightItem");
        if (pobjHeightItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjHeightItem->SetDoubleAttribute("height", stArea.heightItem.maxHeight);
        pobjHeightItem->SetDoubleAttribute("low", stArea.heightItem.minHeight);
        pobjArea->LinkEndChild(pobjHeightItem);

    }

    if (stArea.isEnableObstItem) {
        TiXmlElement *pobjObstItem = new TiXmlElement("obstItem");
        if (pobjObstItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjObstItem->SetAttribute("isIgnoreAllObst", stArea.obstItem.isIgnoreAllObst);
        pobjArea->LinkEndChild(pobjObstItem);
    }

    if (stArea.isEnableRangeItem) {
        TiXmlElement *pobjRangeItem = new TiXmlElement("rangeItem");
        if (pobjRangeItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjRangeItem->SetDoubleAttribute("range", stArea.rangeItem.range);
        pobjArea->LinkEndChild(pobjRangeItem);
    }

    if (stArea.isEnableTreelawnItem) {
        TiXmlElement *pobjTreelawnItem = new TiXmlElement("treelawnItem");
        if (pobjTreelawnItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjTreelawnItem->SetDoubleAttribute("height", stArea.treelawnItem.maxHeight);
        pobjArea->LinkEndChild(pobjTreelawnItem);
    }

    if (stArea.isEnableTrafficControlItem) {
        TiXmlElement *pobjTrafficControlItem = new TiXmlElement("trafficControlItem");
        if (pobjTrafficControlItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }
        pobjTrafficControlItem->SetAttribute("passageNum", stArea.trafficControlItem.passageNum);
        pobjTrafficControlItem->SetAttribute("passageTime", stArea.trafficControlItem.passageTime);
        for (const auto& name : stArea.trafficControlItem.names) 
        {
            TiXmlElement *pobjItemNames = new TiXmlElement("name");
            pobjItemNames->SetAttribute("id", name.first.c_str());
            pobjItemNames->SetAttribute("text", name.second.c_str());
            pobjTrafficControlItem->LinkEndChild(pobjItemNames);
        }
        pobjArea->LinkEndChild(pobjTrafficControlItem);
    }
    
    if (stArea.isEnableRubberControlItem) {
        TiXmlElement *pobjRubberControlItem = new TiXmlElement("rubberControlItem");
        if (pobjRubberControlItem == NULL) {
            ST_LOG_ERR("New pobjRubberControlItem TiXmlElement() Fail.");
            return -1;
        }
        pobjRubberControlItem->SetAttribute("gridBaffleLiftingTime", stArea.rubberControlItem.gridBaffleLiftingTime);
        pobjRubberControlItem->SetAttribute("gridBaffleLoweringTime", stArea.rubberControlItem.gridBaffleLoweringTime);
        pobjRubberControlItem->SetAttribute("gridBaffleDescentTime", stArea.rubberControlItem.gridBaffleDescentTime);
        ST_LOG_INFO("stArea.rubberControlItem.gridBaffleLiftingTime %d", stArea.rubberControlItem.gridBaffleLiftingTime);
        ST_LOG_INFO("stArea.rubberControlItem.gridBaffleLoweringTime %d", stArea.rubberControlItem.gridBaffleLoweringTime);
        ST_LOG_INFO("stArea.rubberControlItem.gridBaffleDescentTime %d", stArea.rubberControlItem.gridBaffleDescentTime);
        pobjArea->LinkEndChild(pobjRubberControlItem);
    }

    if (stArea.isEnableRecognizeItem) {
        TiXmlElement *pobjRecognizeItem = new TiXmlElement("recognizeItem");
        if (pobjRecognizeItem == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        for (const auto &type_id : stArea.recognizeItem.types) 
        {
            TiXmlElement *pobjItemTypes = new TiXmlElement("type");
            pobjItemTypes->SetAttribute("id", type_id);
            pobjRecognizeItem->LinkEndChild(pobjItemTypes);
        }
        pobjArea->LinkEndChild(pobjRecognizeItem);
    }

    if (stArea.isEnableNoUtralItem) {
        TiXmlElement *pobjUtralItem = new TiXmlElement("utralItem");
        if (pobjUtralItem == NULL) {
            ST_LOG_ERR("New pobjUtralItem TiXmlElement() Fail.");
            return -1;
        }

        pobjUtralItem->SetAttribute("isEnableNoUtral", stArea.utralItem.isEnableNoUtral);
        pobjArea->LinkEndChild(pobjUtralItem);
    }

    if (stArea.isEnableNoReverseItem) {
        TiXmlElement *pobjReverseItem = new TiXmlElement("reverseItem");
        if (pobjReverseItem == NULL) {
            ST_LOG_ERR("New pobjReverseItem TiXmlElement() Fail.");
            return -1;
        }

        pobjReverseItem->SetAttribute("isEnableNoReverse", stArea.reverseItem.isEnableNoReverse);
        pobjArea->LinkEndChild(pobjReverseItem);
    }

    std::string strRouteFilePath = objSceneFileMng_.GetRoadAreaFilePath(roadNo, stArea.id);
    INT32 rslt = objLineFile_.Write(strRouteFilePath, stArea.vertexs);
    if (rslt != 0) {
        ST_LOG_ERR("objLineFile_.Write(%s) Fail.", strRouteFilePath.c_str());
        return -1;
    }

    // 保存 JobRequirements
    TiXmlElement *pobjJobRequirements = new TiXmlElement("JobRequirements");

    // 直接保存存在的字段
    pobjJobRequirements->SetAttribute("edgeBrushOpen", stArea.jobRequirements.edgeBrushOpen);
    pobjJobRequirements->SetAttribute("mainBrushOpen", stArea.jobRequirements.mainBrushOpen);
    pobjJobRequirements->SetAttribute("sprinklerOpening", stArea.jobRequirements.sprinklerOpening ? "true" : "false");
    pobjJobRequirements->SetAttribute("leftWindOn", stArea.jobRequirements.leftWindOn);
    pobjJobRequirements->SetAttribute("rightWindOn", stArea.jobRequirements.rightWindOn);
    pobjJobRequirements->SetAttribute("linkageWarehouseDoor", stArea.jobRequirements.linkageWarehouseDoor);
    pobjJobRequirements->SetAttribute("maskingPerception", stArea.jobRequirements.maskingPerception ? "true" : "false");
    pobjJobRequirements->SetDoubleAttribute("maximumSpeed", stArea.jobRequirements.maximumSpeed);
    pobjJobRequirements->SetDoubleAttribute("objectsObstructHeight", stArea.jobRequirements.objectsObstructHeight);
    pobjJobRequirements->SetAttribute("shieldNegativeHeight", stArea.jobRequirements.shieldNegativeHeight ? "true" : "false");
    pobjJobRequirements->SetAttribute("shieldingHighAltitudeObjects", stArea.jobRequirements.shieldingHighAltitudeObjects);

    // 只有在 jobReqJson 不为空时才添加到 pobjArea 中
    if (!pobjJobRequirements->NoChildren()) {
        pobjArea->LinkEndChild(pobjJobRequirements);
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveAreaList(UINT32 roadNo, const vector<ROAD_AREA_STRU> &vectArea, TiXmlElement *pobjRoad) const
{
    for (const ROAD_AREA_STRU &stArea : vectArea) {
        TiXmlElement *pobjArea = new TiXmlElement("area");
        if (pobjArea == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjArea);

        INT32 rslt = SaveArea(roadNo, stArea, pobjArea);
        if (rslt != 0) {
            ST_LOG_ERR("SaveArea(RoadNo=%u, AreaId=%u) Fail.", roadNo, stArea.id);
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveReflectiveStrip(const POINT_COORD_3D_STRU &reflectiveStripPosLeft,
        const POINT_COORD_3D_STRU &reflectiveStripPosRight, TiXmlElement *pobjEquipment) const
{
    TiXmlElement *pobjStrip = new TiXmlElement("reflectiveStrip");
    if (pobjStrip == NULL) {
        ST_LOG_ERR("New TiXmlElement() Fail.");
        return -1;
    }

    pobjStrip->SetDoubleAttribute("leftX", reflectiveStripPosLeft.x);
    pobjStrip->SetDoubleAttribute("leftY", reflectiveStripPosLeft.y);
    pobjStrip->SetDoubleAttribute("leftZ", reflectiveStripPosLeft.z);

    pobjStrip->SetDoubleAttribute("rightX", reflectiveStripPosRight.x);
    pobjStrip->SetDoubleAttribute("rightY", reflectiveStripPosRight.y);
    pobjStrip->SetDoubleAttribute("rightZ", reflectiveStripPosRight.z);

    pobjEquipment->LinkEndChild(pobjStrip);

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveEquipment(const ROAD_POSE_STRU &stEquipment, TiXmlElement *pobjEquipment) const
{
    // 创建 pose 元素并设置属性
    TiXmlElement *pobjPose = new TiXmlElement("pose");
    pobjPose->SetAttribute("id", stEquipment.id);
    pobjPose->SetAttribute("carType", stEquipment.carType);
    pobjPose->SetAttribute("type", stEquipment.type);
    pobjPose->SetAttribute("priority", stEquipment.priority);
    pobjPose->SetAttribute("isRainProof", stEquipment.isRainProof);

    // 仅仅在SiteAttribute为1时才保存这个字段
    if (stEquipment.SiteAttribute == 1) {
        pobjPose->SetDoubleAttribute("Distance", stEquipment.Distance.distance);
    }

    // pobjPose->SetAttribute("PortId", stEquipment.PortId);
    pobjPose->SetAttribute("goalId", stEquipment.goalId);
    pobjPose->SetAttribute("QRCodeId", stEquipment.QRCodeId);
    pobjPose->SetAttribute("SiteAttribute", stEquipment.SiteAttribute);
    pobjPose->SetDoubleAttribute("x", stEquipment.pose.stPos.x);
    pobjPose->SetDoubleAttribute("y", stEquipment.pose.stPos.y);
    pobjPose->SetDoubleAttribute("yaw", stEquipment.pose.dirAngle);
    pobjPose->SetDoubleAttribute("qRPosOffsetDis", stEquipment.qRPosOffsetDis);
    pobjPose->SetDoubleAttribute("liftBoxTravel", stEquipment.liftBoxTravel);

    // 保存 Types 属性
    std::string typesStr;
    for (const UINT32 &type : stEquipment.Types) {
        if (!typesStr.empty()) {
            typesStr += ",";
        }
        typesStr += std::to_string(type);
    }
    pobjPose->SetAttribute("Types", typesStr.c_str());

    // 保存 SiteNames
    TiXmlElement *pobjSiteNames = new TiXmlElement("SiteNames");
    for (const auto& name : stEquipment.SiteNames) {
        pobjSiteNames->SetAttribute(name.first.c_str(), name.second.c_str());
    }
    pobjPose->LinkEndChild(pobjSiteNames);

    // 保存 ReflectiveStrip
    if (stEquipment.useReflectiveStrip) {
        TiXmlElement *pobjReflectiveStrip = new TiXmlElement("reflectiveStrip");
        pobjReflectiveStrip->SetDoubleAttribute("leftX", stEquipment.reflectiveStripPosLeft.x);
        pobjReflectiveStrip->SetDoubleAttribute("leftY", stEquipment.reflectiveStripPosLeft.y);
        pobjReflectiveStrip->SetDoubleAttribute("leftZ", stEquipment.reflectiveStripPosLeft.z);
        pobjReflectiveStrip->SetDoubleAttribute("rightX", stEquipment.reflectiveStripPosRight.x);
        pobjReflectiveStrip->SetDoubleAttribute("rightY", stEquipment.reflectiveStripPosRight.y);
        pobjReflectiveStrip->SetDoubleAttribute("rightZ", stEquipment.reflectiveStripPosRight.z);
        pobjPose->LinkEndChild(pobjReflectiveStrip);
    }

    if (stEquipment.type == ROAD_POSE_CHARGE) {
        pobjPose->SetAttribute("chargeSiteSN", stEquipment.chargeSiteSNInfo.chargeSiteSN);
        pobjPose->SetAttribute("channelNumber", stEquipment.channelNumber);
    }

    pobjEquipment->LinkEndChild(pobjPose);

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveEquipmentList(const vector<ROAD_POSE_STRU> &vectPose, TiXmlElement *pobjRoad) const
{
    for (const ROAD_POSE_STRU &stEquipment : vectPose) {
        INT32 rslt = SaveEquipment(stEquipment, pobjRoad);
        if (rslt != 0) {
            ST_LOG_ERR("SaveEquipment(equipment id=%u) Fail.", stEquipment.id);
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveCrossing(const ROAD_CROSSING_STRU &stCrossing, TiXmlElement *pobjCrossing) const
{
    pobjCrossing->SetAttribute("id", stCrossing.id);

    // 保存 Points
    TiXmlElement *pobjPoints = new TiXmlElement("Points");
    for (const COORDINATE_2D_STRU &point : stCrossing.points) {
        TiXmlElement *pobjPoint = new TiXmlElement("Point");
        pobjPoint->SetDoubleAttribute("X", point.x);
        pobjPoint->SetDoubleAttribute("Y", point.y);
        pobjPoints->LinkEndChild(pobjPoint);
    }
    pobjCrossing->LinkEndChild(pobjPoints);

    // 保存 AnchorPoints
    TiXmlElement *pobjAnchorPoints = new TiXmlElement("AnchorPoints");
    for (const COORDINATE_2D_STRU &anchorPoint : stCrossing.anchorPoints) {
        TiXmlElement *pobjAnchorPoint = new TiXmlElement("AnchorPoint");
        pobjAnchorPoint->SetDoubleAttribute("X", anchorPoint.x);
        pobjAnchorPoint->SetDoubleAttribute("Y", anchorPoint.y);
        pobjAnchorPoints->LinkEndChild(pobjAnchorPoint);
    }
    pobjCrossing->LinkEndChild(pobjAnchorPoints);

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveCrossingList(UINT32 roadNo, const std::vector<ROAD_CROSSING_STRU> &vectCrossing, TiXmlElement *pobjRoad) const
{

    for (const ROAD_CROSSING_STRU &stCrossing : vectCrossing) {
        TiXmlElement *pobjCrossings = new TiXmlElement("Crossings");
        if (pobjCrossings == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoad->LinkEndChild(pobjCrossings);

        INT32 rslt = SaveCrossing(stCrossing, pobjCrossings);
        if (rslt != 0) {
            ST_LOG_ERR("SaveCrossing(CrossingId=%u) Fail.", stCrossing.id);
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveRoad(const ROAD_INFO_STRU &stRoad, TiXmlElement *pobjRoad) const
{
    pobjRoad->SetAttribute("no", stRoad.no);
    pobjRoad->SetAttribute("type", stRoad.roadType);
    pobjRoad->SetAttribute("maxAccessCarNum", stRoad.maxAccessCarNum);
    pobjRoad->SetAttribute("isNarrow", stRoad.isNarrow);
    pobjRoad->SetAttribute("isOnlyForTask", stRoad.isOnlyForTask);
    pobjRoad->SetAttribute("isTrunAround", stRoad.isTurnAround);
    pobjRoad->SetAttribute("isEasyTrafficJam", stRoad.isEasyTrafficJam);
    pobjRoad->SetAttribute("carType", stRoad.carType);
    pobjRoad->SetAttribute("isServiceRoadExist", stRoad.serviceRoadStatus.isServiceRoadExist);
    pobjRoad->SetAttribute("serviceRoadNo", stRoad.serviceRoadStatus.serviceRoadNo);

    // Save points
    TiXmlElement *pobjPoints = new TiXmlElement("Points");
    for (const COORDINATE_2D_STRU &point : stRoad.vectPoints) {
        TiXmlElement *pobjPoint = new TiXmlElement("Point");

        // 将 float 转换为字符串，保留原始精度
        pobjPoint->SetAttribute("X", std::to_string(point.x)); // 使用 std::to_string
        pobjPoint->SetAttribute("Y", std::to_string(point.y)); // 使用 std::to_string

        pobjPoints->LinkEndChild(pobjPoint);
    }
    pobjRoad->LinkEndChild(pobjPoints);

    // Save anchorPoints
    TiXmlElement *pobjAnchorPoints = new TiXmlElement("AnchorPoints");
    for (const COORDINATE_2D_STRU &anchorPoint : stRoad.vectAnchorPoints) {
        TiXmlElement *pobjAnchorPoint = new TiXmlElement("AnchorPoint");

        // 将 float 转换为字符串，保留原始精度
        pobjAnchorPoint->SetAttribute("X", std::to_string(anchorPoint.x)); // 使用 std::to_string
        pobjAnchorPoint->SetAttribute("Y", std::to_string(anchorPoint.y)); // 使用 std::to_string

        pobjAnchorPoints->LinkEndChild(pobjAnchorPoint);
    }
    pobjRoad->LinkEndChild(pobjAnchorPoints);

    INT32 rslt;
    // rslt = SaveCrossingList(stRoad.no, stRoad.vectCrossing, pobjRoad);
    // if (rslt != 0) {
    //     ST_LOG_ERR("SaveCrossingList(RoadNo=%u) Fail.", stRoad.no);
    //     return -1;
    // }

    // Save other components
    rslt = SavePortList(stRoad.vectPort, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SavePortList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveLineList(stRoad.no, stRoad.vectLine, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveLineList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveRouteList(stRoad.no, stRoad.vectRoute, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveRouteList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveObstLineList(stRoad.no, stRoad.vectObstacleline, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveObstLineList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveStopLineList(stRoad.no, stRoad.vectStopLine, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveStopLineList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveLaneLineList(stRoad.no, stRoad.vectLaneLine, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveLaneLineList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveGoalList(stRoad.vectGoal, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveGoalList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveGoalAreaList(stRoad.no, stRoad.vectGoalArea, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveGoalAreaList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveEquipmentList(stRoad.vectPose, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveEquipmentList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveAreaList(stRoad.no, stRoad.vectArea, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveAreaList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    }

    rslt = SaveTrafficLightSpotList(stRoad.vectTrafficLightSpot, pobjRoad);
    if (rslt != 0) {
        ST_LOG_ERR("SaveTrafficLightSpotList(RoadNo=%u) Fail.", stRoad.no);
        return -1;
    } 

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveRoadList(const vector<ROAD_INFO_STRU> &vectRoad, TiXmlElement *pobjRoads) const
{
    for (const ROAD_INFO_STRU &stRoad : vectRoad) {
        TiXmlElement *pobjRoad = new TiXmlElement("road");
        if (pobjRoad == NULL) {
            ST_LOG_ERR("New TiXmlElement() Fail.");
            return -1;
        }

        pobjRoads->LinkEndChild(pobjRoad);

        INT32 rslt = SaveRoad(stRoad, pobjRoad);
        if (rslt != 0) {
            ST_LOG_ERR("SaveRoad(RoadNo=%u) Fail.", stRoad.no);
            return -1;
        }
    }

    return 0;
}

INT32 ROAD_NETWORK_FILE_C::SaveToFile(const ROAD_NETWORK_INFO_STRU &stRoadNetwork) const
{
    TiXmlDocument objXmlFile;

    TiXmlDeclaration *pobjDecl = new TiXmlDeclaration("1.0", "UTF-8", "");
    if (pobjDecl == NULL) {
        ST_LOG_ERR("New TiXmlDeclaration() Fail.");
        return -1;
    }

    objXmlFile.LinkEndChild(pobjDecl);

    TiXmlElement *pobjRoads = new TiXmlElement("roads");
    if (pobjRoads == NULL) {
        ST_LOG_ERR("New TiXmlElement(roads) Fail.");
        return -1;
    }

    objXmlFile.LinkEndChild(pobjRoads);

    INT32 rslt = SaveRoadList(stRoadNetwork.vectRoad, pobjRoads);
    if (rslt != 0) {
        ST_LOG_ERR("SaveRoadList() Fail.");
        return -1;
    }

    std::string strRoadNetworkFilePath = objSceneFileMng_.GetRoadsInfoFilePath();
    if (!objXmlFile.SaveFile(strRoadNetworkFilePath)) {
        ST_LOG_INFO("objXmlFile.SaveFile(%s) Fail.", strRoadNetworkFilePath.c_str());
        return -1;
    }

    return 0;
}

