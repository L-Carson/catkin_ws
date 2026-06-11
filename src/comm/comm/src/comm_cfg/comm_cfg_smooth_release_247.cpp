
#include "comm/comm_cfg/comm_cfg_amend.h"
#include "comm/comm_cfg/comm_cfg_smoother.h"
#include "comm/comm_car_body_file.h"


namespace COMM
{

namespace CFG
{

namespace V01_11_247_004_VS_V01_11_245_000 {

INT32 CopyCarBodyFilterFileToNew(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg)
{
    float vxlCellOld;
    CUBOID_STRU stCuboidOld;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "VoxelSize", vxlCellOld))       return -1;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinX",      stCuboidOld.minX)) return -1;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MaxX",      stCuboidOld.maxX)) return -1;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinY",      stCuboidOld.minY)) return -1;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MaxY",      stCuboidOld.maxY)) return -1;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinZ",      stCuboidOld.minZ)) return -1;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MaxZ",      stCuboidOld.maxZ)) return -1;

    string strCarType = "";
    if (objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", strCarType)) return -1;

    /**
     * stCuboidNew.minX = 车尾长 + 余量
     * stCuboidNew.maxX = 轴距 + 车头长 + 余量
     */
    const CAR_BODY_INFO_STRU *pstCarBodyInfo = CAR_BODY_C::GetCarBodyInfo(strCarType);
    if (pstCarBodyInfo == nullptr) {
        ST_LOG_ERR("CAR_BODY_C::GetCarBodyInfo(%s) Fail.", strCarType.c_str());
        return -1;
    }

    CUBOID_STRU stCuboidNew = stCuboidOld;
    switch (pstCarBodyInfo->enCarType) {
        case CAR_TYPE_S310: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.30; break; //无前扫刷
        case CAR_TYPE_S320: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.80; break;
        case CAR_TYPE_S323: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.80; break;
        case CAR_TYPE_S324: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.80; break;
        case CAR_TYPE_S325: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.30; break; //无前扫刷(侨银定制)
        case CAR_TYPE_S326: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.80; break;
        case CAR_TYPE_S330: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.30; break; //无前扫刷
        case CAR_TYPE_S332: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S333: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S334: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S335: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S336: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S337: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S350: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.30; break; //无前扫刷
        case CAR_TYPE_S520: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S530: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        default: ST_LOG_ERR("无效的车型参数: %s.", strCarType.c_str()); return -1;
    }
    stCuboidNew.minZ = 0.03;

    float vxlCellNew = 0.03;
    CAR_BODY_FILE_C objCarBodyFile(vxlCellOld, stCuboidOld, strCarType);
    if (objCarBodyFile.CopyCarBodyFilterdFileToNew(vxlCellNew, stCuboidNew)) {
        ST_LOG_ERR("将车体构造文件拷贝到新的参数体系下失败。");
        return -1;
    }

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "VoxelSize", vxlCellNew))       return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinX",      stCuboidNew.minX)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MaxX",      stCuboidNew.maxX)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinZ",      stCuboidNew.minZ)) return -1;

    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold", "0.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "UnitX",       "1.2")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "MaxAngleByX", "8.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "MaxAngleByY", "3.0")) return -1;

    if (CopyCarBodyFilterFileToNew(objCfgAmend, objCfg)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_247_016 {

INT32 UpgradeLidarCfg(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg, const char* lidarName)
{
    /* 是否有该雷达 */
    if (!objCfgAmend.IsElementExist(objCfg, {"LidarGroup", lidarName})) return 0;

    /* 获取雷达的最大探测距离 */
    float maxDist = 0;
    if (objCfgAmend.GetAttribute(objCfg, {"LidarGroup", lidarName, "DistParam"}, "MaxDist", maxDist)) return -1;

    /* 获取扇区切除的数量 并 添加扇区切除的距离参数 */
    int count = objCfgAmend.SiblingElementCount(objCfg, {"LidarGroup", lidarName, "SectionWipe"});
    for (int index = 0; index < count; ++index) {
        TiXmlElement* pElem = objCfgAmend.GetElement(objCfg, {"LidarGroup", lidarName, "SectionWipe"}, index);
        if (objCfgAmend.SetAttribute(*pElem, {}, "MinDist", 0))       return -1;
        if (objCfgAmend.SetAttribute(*pElem, {}, "MaxDist", maxDist)) return -1;
    }

    /* 根据雷达名称、车型、雷达类型设置扇区切除的距离参数，并开启对应雷达的感知功能 */
    if (string(lidarName) != Lidar[LIDAR_ENUM::MAIN_LIDAR]) return 0;
    string strLidarType = "";
    if (objCfgAmend.GetAttribute(objCfg, {"LidarGroup", lidarName, "Property"}, "LidarType", strLidarType)) return -1;
    if (strLidarType == "RSHELIOS_16") {
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "Property"}, "LidarType", LidarType[LIDAR_TYPE_ENUM::RSHELIOS_16P])) return -1;
        strLidarType = LidarType[LIDAR_TYPE_ENUM::RSHELIOS_16P];
    }
    if (strLidarType != LidarType[LIDAR_TYPE_ENUM::RSHELIOS_16P] &&
        strLidarType != LidarType[LIDAR_TYPE_ENUM::RS16]         &&
        strLidarType != LidarType[LIDAR_TYPE_ENUM::LS_C16]) return 0;

    string strCarType = "";
    if (objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", strCarType)) return -1;
    if (strCarType != "S330" && strCarType != "S332" && strCarType != "S333") return 0;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "Property"},    "IsSupportPnp", "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "SectionWipe"}, "StartAngle",   135))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "SectionWipe"}, "EndAngle",     225))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "SectionWipe"}, "MinDist",      0))      return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "SectionWipe"}, "MaxDist",      2))      return -1;

    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()))   return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()))  return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str())) return -1;


    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_247_024 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc"}, "enable", "true")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "minScore",    "0.6")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "srcCloudMinRange", "1.0"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "srcCloudMaxRange", "80.0"))   return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc"}, "type", "1"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "srcCloudMinRange", "1.0"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "srcCloudMaxRange", "80.0"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Ndt"}, "resolution", "1.0"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Ndt"}, "stepSize",   "0.1"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Ndt"}, "transEps",   "0.001")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Ndt"}, "maxIter",    "30"))    return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "srcCloudMinRange", "1.0"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "srcCloudMaxRange", "80.0"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Ndt"}, "resolution", "1.0"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Ndt"}, "stepSize",   "0.1"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Ndt"}, "transEps",   "0.001")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Ndt"}, "maxIter",    "30"))    return -1;

    return 0;
}

}

namespace V01_11_247_031 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isDoorCloseRequire", "true")) return -1;

    return 0;
}
}

namespace V01_11_247_034 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "StepPadDet"}, "isSupport",  "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "StepPadDet"}, "ExtandDist", 0.3))    return -1;

    return 0;
}
}

namespace V01_11_247_036 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    float minWidth = 4.0;
    if (objCfgAmend.GetAttribute(objCfg, {"Plan", "GloalPlan"}, "minTurnAroundRoadWidth", minWidth)) {
        ST_LOG_ERR("读取参数minTurnAroundRoadWidth失败, 采用默认值4.0");
    }
    minWidth = LIMIT(minWidth, 4.0, 10.0);
    if (0 != objCfgAmend.SetAttribute(objCfg, {"Plan", "GloalPlan"}, "minTurnAroundRoadWidth", minWidth)) {
        ST_LOG_ERR("objCfgAmend.SetAttribute(minTurnAroundRoadWidth) Fail.");
        return -1;
    }

    float forwardDec = 1.6;
    if (objCfgAmend.GetAttribute(objCfg, {"VehicleChassis", "VelSmoother"}, "forwardDec", forwardDec)) {
        ST_LOG_ERR("读取参数forwardDec失败, 采用默认值1.6");
    }
    forwardDec = LIMIT(forwardDec, 1.6, 10.0);
    if (0 != objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "VelSmoother"}, "forwardDec", forwardDec)) {
        ST_LOG_ERR("objCfgAmend.SetAttribute(forwardDec) Fail.");
        return -1;
    }

    float backwardDec = 1.0;
    if (objCfgAmend.GetAttribute(objCfg, {"VehicleChassis", "VelSmoother"}, "backwardDec", backwardDec)) {
        ST_LOG_ERR("读取参数backwardDec失败, 采用默认值1.0");
    }
    backwardDec = LIMIT(backwardDec, 1.0, 10.0);
    if (0 != objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "VelSmoother"}, "backwardDec", backwardDec)) {
        ST_LOG_ERR("objCfgAmend.SetAttribute(backwardDec) Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");

    return 0;
}
}


namespace V01_11_247_041 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "isUseVelItem", "true")) return -1;

    return 0;
}
}


namespace V01_11_247_042 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.RenameElement(objCfg, {"Pnp", "StepPadDet"}, "SlopePadDet")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg"}, "SlopePad", "false")) return -1;

    return 0;
}
}


namespace V01_11_247_045 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"VisionFunction", "NetSeg"}, "SlopePad")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "SlopePad"}, "isSupport", "false")) return -1;

    return 0;
}
}


namespace V01_11_247_046 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "brushShortenLen", 0.0)) return -1;

    return 0;
}
}


namespace V01_11_247_074 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold", 0.3)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "RoadRegionFilter"}, "isSupport", "true")) return -1;

    return 0;
}
}


namespace V01_11_247_075 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, "VisionFunction", "isUseApi", "false")) return -1;

    return 0;
}
}


namespace V01_11_247_076 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "FullThresh", 0.2)) return -1;

    return 0;
}
}

namespace V01_11_247_081 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.IsAttributeExist(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "DistThresh")) {
        if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "FullThresh")) return -1;
    } else {
        if (objCfgAmend.RenameAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "FullThresh", "DistThresh")) return -1;
    }
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "NumThresh", 60)) return -1;

    int num = 0;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "IntensityThreshold", num)) return -1;
    if (num == 120 &&
        objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "IntensityThreshold", 180)) {
        return -1;
    }

    return 0;
}
}


namespace V01_11_247_087 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "Seg2dObst"}, "Height")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMinHeight", 0.5)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMaxHeight", 2.0)) return -1;

    return 0;
}

}

namespace V01_11_247_087_VS_V01_11_203_171
{

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    INT32 rslt = objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isDoorCloseRequire", "true");
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.SetAttribute(Plan.FunctionParam.isDoorCloseRequire=true) Fail.");
        return -1;
    }

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()})) {
        INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "Property"}, "LidarSerialNum", "sn");
        if (rslt != 0) {
            ST_LOG_ERR("Add LidarGroup.Mainlidar.Property.LidarSerialNum Fail.");
            return -1;
        }
    }

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()})) {
        INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "Property"}, "LidarSerialNum", "sn");
        if (rslt != 0) {
            ST_LOG_ERR("Add LidarGroup.Blindlidar.Property.LidarSerialNum Fail.");
            return -1;
        }
    }

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str()})) {
        INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "Property"}, "LidarSerialNum", "sn");
        if (rslt != 0) {
            ST_LOG_ERR("Add LidarGroup.Blindlidar2.Property.LidarSerialNum Fail.");
            return -1;
        }
    }

    rslt = objCfgAmend.AddAttribute(objCfg, {"Plan", "GloalPlan"}, "isSupportRoadPortTurnAround", true);
    if (rslt != 0) {
        ST_LOG_ERR("Add Plan.GloalPlan.isSupportRoadPortTurnAround Fail.");
        return -1;
    }

    rslt = objCfgAmend.AddAttribute(objCfg, {"Plan", "GloalPlan"}, "skipExecuteLen", 5.0);
    if (rslt != 0) {
        ST_LOG_ERR("Add Plan.GloalPlan.skipExecuteLen Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

}


namespace V01_11_247_111 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Bicycle"},        "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "TrafficCone"},    "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "MetalBarrier"},   "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "MedianBarrier"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "CeremonialPole"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Bollard"},        "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "SpeedBump"},      "isSupport", "false")) return -1;

    return 0;
}
}

namespace V01_11_247_118 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"},  "isDynamicObstStop", "false")) return -1;

    return 0;
}
}


namespace V01_11_247_127 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"},  "isSmartCtrlBrush", "true")) return -1;

    return 0;
}
}


namespace V01_11_247_137 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "FilterPointsRebuild"},  "isSupport", "false")) return -1;

    return 0;
}
}

namespace V01_11_247_138 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Ndt"}, "transEps",   "0.0001")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Ndt"}, "transEps",   "0.0001")) return -1;

    return 0;
}
}

namespace V01_11_247_139 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "matchThrK",   "0.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "matchThrB",   "0.315")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "matchThrK",   "0.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "matchThrB",   "0.315")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter"}, "type",   "0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeX", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeY", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeZ", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "voxelNum", "800")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "leafSize", "0.5")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "matchThrK",   "0.0"))   return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "matchThrB",   "0.315")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter"}, "type",   "0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeX", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeY", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeZ", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "voxelNum", "800")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "leafSize", "0.5")) return -1;

    return 0;
}
}


namespace V01_11_247_143 {

INT32 UpgradeLidarCfg(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg, const char* lidarName)
{
    /* 是否有该雷达 */
    if (!objCfgAmend.IsElementExist(objCfg, {"LidarGroup", lidarName})) return 0;

    /* 获取扇区切除的数量 并 添加扇区切除的高度参数 */
    int count = objCfgAmend.SiblingElementCount(objCfg, {"LidarGroup", lidarName, "SectionWipe"});
    for (int index = 0; index < count; ++index) {
        TiXmlElement* pElem = objCfgAmend.GetElement(objCfg, {"LidarGroup", lidarName, "SectionWipe"}, index);
        if (objCfgAmend.SetAttribute(*pElem, {}, "MinHeight", -5.0)) return -1;
        if (objCfgAmend.SetAttribute(*pElem, {}, "MaxHeight",  5.0)) return -1;
    }

    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()))   return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()))  return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str())) return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str())) return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str())) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_247_146 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Warehouse"}, "ReflectiveStripHeight", 0.3)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Warehouse"}, "IntensityThreshold",    180)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_247_153 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isAvoidObstRemotely", false)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_247_154 {

INT32 AddImuPort(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg, const char* lidarName, const int value)
{
    /* 是否有该雷达 */
    if (!objCfgAmend.IsElementExist(objCfg, {"LidarGroup", lidarName})) return 0;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "NetCfg"}, "ImuPort", value)) return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), 8901)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), 8902)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), 8903)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str(), 8904)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str(), 8905)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_247_156 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isAvoidObstRemotely", "false")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_247_157 {

INT32 AddLidarShadowFilterCfg(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg, const char* lidarName, const char* value)
{
    /* 是否有该雷达 */
    if (!objCfgAmend.IsElementExist(objCfg, {"LidarGroup", lidarName})) return 0;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "Property"}, "IsSupportShadowFilter", value)) return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str(), "true")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_247_189 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Bough"},        "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Cup"},          "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Can"},          "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Carton"},       "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Package"},      "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Brick"},        "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Cigaretteend"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "TaskMng"}, "trafficControlCheckDist", 10.0)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_247_193 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "DataProcess"}, "InputCloudMinRange",  "1.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "DataProcess"}, "InputCloudMaxRange", "80.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "DataProcess"}, "InputCloudJumpNum",   "4"))   return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "ActiveMap"}, "enable",      "false"))  return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "ActiveMap"}, "VoxelMapSize", "0.2"))   return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "ActiveMap"}, "MaxVoxelSize", "80000")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}
namespace V01_11_247_198 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Pericarp"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "CarBodyDenoising​​"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "CarBodyDenoising​​"}, "Resolution", "0.3"))  return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_247_209 {


INT32 AddLidarShadowFilterCfg(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg, const char* lidarName, const char* value)
{
    /* 是否有该雷达 */
    if (!objCfgAmend.IsElementExist(objCfg, {"LidarGroup", lidarName})) return 0;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "Property"}, "isSupportShadowLookupTableFilter", value)) return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "false")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "false")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "false")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str(), "false")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str(), "false")) return -1;
    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_247_212 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp"}, "isSupport", "true"))  return -1;
    ST_LOG_INFO("Succ.");
    return 0;
}

}

/**************************************************************************************
功能描述: 升级注册表
修改记录:
**************************************************************************************/
const UPGRADE_REG_STRU g_astUpgradeTabOfRelease247[] =
{
    /* 【重要说明】
            配置参数变更时，必须增加平滑处理，在此表中注册平滑处理信息。
            此表中的注册的版本，指的是配置版本号，配置版本号与软件版本不一样，详见当前配置版本 CFG_VERSION 定义。
            此表中的注册的函数，表示从表中上一个配置版本升级到新版本时需要调用注册函数对配置文件进行适配修改。

            本分支升级函数，表示从本分支低版本或基础版本升级过来的处理函数。
            收编分支升级函数，表示从其它分支升级过来的处理函数。

            每个分支的升级表中，第一行表示本分支配置数据的原始来源。
                    仅Master分支升级表中第一行，本分支升级函数不为空，且收编分支升级函数为空，表示来源于基础版本升级；
                    非Master分支升级表中第一行，本分支升级函数为空，且收编分支升级函数不为空，表示来源于其它分支收编。
            每个分支的升级表中，后续行本分支升级函数和收编其它分支升级函数至少有一个不为空。
            本表中升级目标版本必须严格从小到大排列，收编分支版本严格小于升级目标版本。

            修改本注册表有三种场景：
                （1）新建分支场景，修改步骤详见 g_astBranchUpgradeRegTab 的 【分支注册表新建说明】。
                （2）本分支内升级场景，修改步骤详见后续【配置升级注册说明】。
                （3）收编分支场景，修改步骤详见 g_astBranchUpgradeRegTab 的 【分支注册表收编说明】。
    */

    /*   升级目标版本                    本分支升级函数                     收编分支版本               收编分支升级函数          */
    {"V01.11.247.004",        nullptr,                          "V01.11.245.000",   V01_11_247_004_VS_V01_11_245_000::Upgrade},
    {"V01.11.247.016",        V01_11_247_016::Upgrade,          "",                 nullptr},
    {"V01.11.247.024",        V01_11_247_024::Upgrade,          "",                 nullptr},
    {"V01.11.247.031",        V01_11_247_031::Upgrade,          "",                 nullptr},
    {"V01.11.247.034",        V01_11_247_034::Upgrade,          "",                 nullptr},
    {"V01.11.247.036",        V01_11_247_036::Upgrade,          "",                 nullptr},
    {"V01.11.247.041",        V01_11_247_041::Upgrade,          "",                 nullptr},
    {"V01.11.247.042",        V01_11_247_042::Upgrade,          "",                 nullptr},
    {"V01.11.247.045",        V01_11_247_045::Upgrade,          "",                 nullptr},
    {"V01.11.247.046",        V01_11_247_046::Upgrade,          "",                 nullptr},
    {"V01.11.247.074",        V01_11_247_074::Upgrade,          "",                 nullptr},
    {"V01.11.247.075",        V01_11_247_075::Upgrade,          "",                 nullptr},
    {"V01.11.247.076",        V01_11_247_076::Upgrade,          "",                 nullptr},
    {"V01.11.247.081",        V01_11_247_081::Upgrade,          "",                 nullptr},
    {"V01.11.247.087",        V01_11_247_087::Upgrade,          "V01.11.203.171",   V01_11_247_087_VS_V01_11_203_171::Upgrade},
    {"V01.11.247.111",        V01_11_247_111::Upgrade,          "",                 nullptr},
    {"V01.11.247.118",        V01_11_247_118::Upgrade,          "",                 nullptr},
    {"V01.11.247.127",        V01_11_247_127::Upgrade,          "",                 nullptr},
    {"V01.11.247.137",        V01_11_247_137::Upgrade,          "",                 nullptr},
    {"V01.11.247.138",        V01_11_247_138::Upgrade,          "",                 nullptr},
    {"V01.11.247.139",        V01_11_247_139::Upgrade,          "",                 nullptr},
    {"V01.11.247.143",        V01_11_247_143::Upgrade,          "",                 nullptr},
    {"V01.11.247.146",        V01_11_247_146::Upgrade,          "",                 nullptr},
    {"V01.11.247.153",        V01_11_247_153::Upgrade,          "",                 nullptr},
    {"V01.11.247.154",        V01_11_247_154::Upgrade,          "",                 nullptr},
    {"V01.11.247.156",        V01_11_247_156::Upgrade,          "",                 nullptr},
    {"V01.11.247.157",        V01_11_247_157::Upgrade,          "",                 nullptr},
    {"V01.11.247.189",        V01_11_247_189::Upgrade,          "",                 nullptr},
    {"V01.11.247.193",        V01_11_247_193::Upgrade,          "",                 nullptr},
    {"V01.11.247.198",        V01_11_247_198::Upgrade,          "",                 nullptr},
    {"V01.11.247.209",        V01_11_247_209::Upgrade,          "",                 nullptr},
    {"V01.11.247.212",        V01_11_247_212::Upgrade,          "",                 nullptr}

    /* 【配置升级注册说明】
            修改配置文件 checklist :
                (1) 将上一个配置注册版本的升级函数的域名                   NEW_CFG_VER 修改为   对应的版本号，比如 V01_01_001_000_001；
                (2) 将上一个配置注册版本的版本号 CFG_VERSION 修改为此时 CFG_VERSION 的具体的版本号字符串，比如 "V01.01.001.000.001"；
                (3) 新增加的新版本升级处理函数放到域名 NEW_CFG_VER 中；
                (4) 新增注册项的版本写为 CFG_VERSION ，即为自动识别最新配置版本；
                (5) 将 CFG_VERSION 的定义修改为 LOC_VERSION，即其定义变为
                    #define CFG_VERSION    LOC_VERSION
                (6) 将 \navZero\workspace\src\comm\comm\template\cfg_file\cfg_param.xml 修改为最新模板内容。

       注意：如果此时 CFG_VERSION 的定义本就是 LOC_VERSION，说明新版本已经有其他人增加注册了。
             此时不用新增注册项，直接在其注册函数中新增加变更内容即可。
             合并Merge时，应再次确认master最新代码中 CFG_VERSION 是否为 LOC_VERSION，如否则不上注册项。
    */
};

}

}


