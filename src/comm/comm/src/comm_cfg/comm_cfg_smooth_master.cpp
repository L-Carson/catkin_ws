
#include "comm/comm_define.h"
#include "comm/comm_cfg/comm_cfg_amend.h"
#include "comm/comm_cfg/comm_cfg_smoother.h"
#include "comm/comm_car_body_file.h"

namespace COMM
{

namespace CFG
{

namespace V01_11_001_174_096 {

INT32 UpgradeLidarCfg(TiXmlElement &xmlElem, const char* srcLidar, const char* tarLidar)
{
    CFG_AMEND_C objCfgAmend;
    int    i = 0;
    float  f = 0;
    string s = "";

    if (objCfgAmend.IsAttributeTrue(xmlElem, {"LidarGroup", srcLidar}, "isSupport")) {
        objCfgAmend.AddElement(xmlElem, {"LidarGroup", tarLidar});

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "DriverParam"}, "LidarType", s)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "Property"}, "LidarType", s)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "DriverParam", "DevParam"}, "Ip", s)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "NetCfg"}, "Ip", s)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "DriverParam", "DevParam"}, "MsopPort", i)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "NetCfg"}, "MsopPort", i)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "DriverParam", "DevParam"}, "DifopPort", i)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "NetCfg"}, "DifopPort", i)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "DriverParam", "DataParam"}, "MinDist", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "DistParam"}, "MinDist", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "DriverParam", "DataParam"}, "MaxDist", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "DistParam"}, "MaxDist", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "InstallParam"}, "Tx", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "InstallParam"}, "Tx", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "InstallParam"}, "Ty", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "InstallParam"}, "Ty", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "InstallParam"}, "Tz", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "InstallParam"}, "Tz", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "InstallParam"}, "Roll", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "InstallParam"}, "Roll", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "InstallParam"}, "Pitch", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "InstallParam"}, "Pitch", f)) return -1;

        if (objCfgAmend.GetAttribute(xmlElem, {"LidarGroup", srcLidar, "InstallParam"}, "Yaw", f)) return -1;
        if (objCfgAmend.AddAttribute(xmlElem, {"LidarGroup", tarLidar, "InstallParam"}, "Yaw", f)) return -1;

        if (objCfgAmend.IsAttributeTrue(xmlElem, {"LidarGroup", srcLidar, "LaserSectionWipe"}, "isSupport")) {
            int count = objCfgAmend.SiblingElementCount(xmlElem, {"LidarGroup", srcLidar, "LaserSectionWipe", "Section"});
            for (int index = 0; index < count; ++index) {
                TiXmlElement* pElemSrc = objCfgAmend.GetElement(xmlElem, {"LidarGroup", srcLidar, "LaserSectionWipe", "Section"}, index);
                TiXmlElement* pElemTar = objCfgAmend.AddElement(xmlElem, {"LidarGroup", tarLidar, "SectionWipe"}, true);

                if (objCfgAmend.GetAttribute(*pElemSrc, {}, "StartAngle", f)) return -1;
                if (objCfgAmend.AddAttribute(*pElemTar, {}, "StartAngle", f)) return -1;

                if (objCfgAmend.GetAttribute(*pElemSrc, {}, "EndAngle", f)) return -1;
                if (objCfgAmend.AddAttribute(*pElemTar, {}, "EndAngle", f)) return -1;
            }
        }
    }

    objCfgAmend.DeleteElement(xmlElem, {"LidarGroup", srcLidar});
    return 0;
}

INT32 UpgradePcptFuncDelete(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdFeatureExtr"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdFilter"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdPartition"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdSlopeProcess"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "FallArrest"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "ImpassableAreaDet"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdCluster", "DetectTruck"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdCluster", "DetectCar"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdCluster", "DetectMan"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"pcd", "PcdCluster", "SpeedMeasure"})) return -1;
    return 0;
}

INT32 UpgradePcptPlaneSegFunc(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    string s = "";
    if (objCfgAmend.GetAttribute(xmlElem, {"VehicleBody", "StructureInfo"}, "model", s)) return -1;

    if (s == "S310") {
        if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "Region"}, "MinX", 1.30)) return -1;
    } else if (s == "S320") {
        if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "Region"}, "MinX", 1.85)) return -1;
    } else if (s == "S323") {
        if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "Region"}, "MinX", 1.75)) return -1;
    } else if (s == "S330") {
        if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "Region"}, "MinX", 1.40)) return -1;
    } else if (s == "S332") {
        if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "Region"}, "MinX", 1.80)) return -1;
    } else if (s == "S520") {
        if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "Region"}, "MinX", 2.50)) return -1;
    } else {
        ST_LOG_ERR("The car body type <%s> is incorrect.", s.c_str());
        return -1;
    }

    if (objCfgAmend.SetAttribute(xmlElem, {"Pcd", "PlaneSegmentation", "HyperParam"}, "HeightThreshold", 0.07)) return -1;
    return 0;
}

INT32 UpgradeLidar(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (UpgradeLidarCfg(xmlElem, "HeadLeftRs16Lidar",      Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()))    return -1;
    if (UpgradeLidarCfg(xmlElem, "MiddleRs16Lidar",        Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()))    return -1;
    if (UpgradeLidarCfg(xmlElem, "HeadLeftRsBpearlLidar",  Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()))   return -1;
    if (UpgradeLidarCfg(xmlElem, "MiddleRsBpearlLidar",    Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()))   return -1;
    if (UpgradeLidarCfg(xmlElem, "HeadRightRsBpearlLidar", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str())) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"LidarGroup", "RsHeliosLidar"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"LidarGroup", "FRM_2D_LIDAR"}))  return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"LidarGroup", "BRD_2D_LIDAR"}))  return -1;
    return 0;
}

INT32 UpgradePcptFunc(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(xmlElem, {"Pcd", "CarBodyVxlFilter"}, "isSupport")) return -1;
    if (UpgradePcptFuncDelete(xmlElem)) return -1;
    if (UpgradePcptPlaneSegFunc(xmlElem)) return -1;
    if (objCfgAmend.RenameElement(xmlElem, "Pcd", "Pnp")) return -1;

    return 0;
}

INT32 UpgradeVisionFunc(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;

    if (objCfgAmend.DeleteElement(xmlElem, {"VisionFunction", "RoadObstDet"}))      return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"VisionFunction", "GarbageDet"}))       return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"VisionFunction", "CoverDet"}))         return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"VisionFunction", "CommObstDet"}))      return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"VisionFunction", "TrafficLightDet"}))  return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"VisionFunction", "RoadMarkDet"}))      return -1;

    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetSeg"}, "isSupport", "false")) return -1;

    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Unification"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Car"},         "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Bike"},        "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Pedestrian"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "SolidCover"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "HollowCover"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Bottle"},      "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Paper"},       "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Branch"},      "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetDet", "Leaves"},      "isSupport", "false")) return -1;

    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetSeg", "RoadEdge"},    "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetSeg", "Pipeline"},    "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetSeg", "Cordon"},      "isSupport", "false")) return -1;

    string s = "";
    if (objCfgAmend.GetAttribute(xmlElem, {"VehicleBody", "StructureInfo"}, "model", s)) return -1;
    if (s == "S330") {
        if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction"}, "isSupport", "true")) return -1;
        if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetSeg"}, "isSupport", "true")) return -1;
        if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction", "NetSeg", "Pipeline"}, "isSupport", "true")) return -1;
    } else {
        if (objCfgAmend.SetAttribute(xmlElem, {"VisionFunction"}, "isSupport", "false")) return -1;
    }

    return 0;
}

INT32 UpgradePlan(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, {"Plan", "LocalPlan"}, "stopFaceSafeDist", 0.5)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"Plan", "LocalPlan"}, "parkFaceSafeDist", 0.3)) return -1;
    return 0;
}

INT32 UpgradeLoc(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, "Gps", "protocolType", 0)) return -1;
    if (objCfgAmend.RenameElement(xmlElem, "Gps", "Gnss")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, "Imu", "type", 0)) return -1;
    return 0;
}

INT32 UpgradeUltrasonic(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, "Ultrasonic", "isSupport", "false"))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, "Ultrasonic", "devName",   "/dev/ttyS0")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, "Ultrasonic", "baudRate",  115200))       return -1;
    return 0;
}

INT32 UpgradeTof(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, "Tof", "isSupport", "fasle")) return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteElement(objCfg, "DepthCameraGroup")) return -1;
    if (objCfgAmend.DeleteElement(objCfg, "MultiLidarFusion")) return -1;

    if (UpgradeLidar(objCfg))       return -1;
    if (UpgradePcptFunc(objCfg))    return -1;
    if (UpgradeVisionFunc(objCfg))  return -1;
    if (UpgradePlan(objCfg))        return -1;
    if (UpgradeLoc(objCfg))         return -1;
    if (UpgradeUltrasonic(objCfg))  return -1;
    if (UpgradeTof(objCfg))         return -1;

    ST_LOG_INFO("Doing");
    return 0;
}
}

namespace V01_11_001_174_098 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    ST_LOG_INFO("Doing");
    return 0;
}
}

namespace V01_11_001_174_101 {

INT32 UpgradePcptPlaneSegFunc(TiXmlElement &xmlElem)
{
    CFG_AMEND_C objCfgAmend;
    string s = "";
    if (objCfgAmend.GetAttribute(xmlElem, {"VehicleBody", "StructureInfo"}, "model", s)) return -1;

    float bodyWidth(0), carHeight(0);
    if      (s == "S310")                {bodyWidth = 1.26; carHeight = 1.45;}
    else if (s == "S320" || s == "S323") {bodyWidth = 1.68; carHeight = 1.80;}
    else if (s == "S330")                {bodyWidth = 1.10; carHeight = 1.65;}
    else if (s == "S332")                {bodyWidth = 1.60; carHeight = 1.65;}
    else if (s == "S520")                {bodyWidth = 1.56; carHeight = 2.10;}
    else {
        ST_LOG_ERR("The car body type <%s> is incorrect.", s.c_str());
        return -1;
    }

    float halfWidth = bodyWidth * 0.7;
    carHeight += 0.1;
    if (objCfgAmend.SetAttribute(xmlElem, {"Pnp", "PlaneSegmentation", "Region"}, "MinY", -halfWidth)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"Pnp", "PlaneSegmentation", "Region"}, "MaxY",  halfWidth)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"Pnp", "PcptRegion", "CorseRegion"}, "MaxZ", carHeight)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"Pnp", "PcptRegion", "FineRegion"},  "MaxZ", carHeight)) return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    UpgradePcptPlaneSegFunc(objCfg);
    if (objCfgAmend.SetAttribute(objCfg, "Tof", "isSupport", "false")) return -1;

    return 0;
}
}

namespace V01_11_001_174_102 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.SetAttribute(objCfg, {"Plan", "GloalPlan"}, "isSupportTurnAround", "true")) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_108 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    INT32 rslt = objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "DcuInfo"}, "SerialDevName", "/dev/ttyS3");
    if (0 != rslt) {
        ST_LOG_ERR("objCfgAmend.SetAttribute(SerialDevName) Fail.");
        return -1;
    }

    rslt = objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "ComputingPlatform"}, "NavIpcType", "default");
    if (0 != rslt) {
        ST_LOG_ERR("objCfgAmend.SetAttribute(ComputingPlatform) Fail.");
        return -1;
    }

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "DropDet"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "DropDet"}, "DropHeight", "0.07")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_111 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "CorseRegion"}, "MinZ", "-1.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "FineRegion"},  "MinZ", "-1.5")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "Region"},     "MinX",  "-4.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "Region"},     "MaxX",  "6.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "UnitX", "1.0")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Multiframe"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Multiframe"}, "FrameNum",  "3"))     return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "Property"}, "IsSupportLoc", "true")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "Property"}, "IsSupportLoc", "false")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "Property"}, "IsSupportLoc", "false")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_114 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"Plan", "LocalPlan"}, "maxSideSafeDistWhenAvoidAlongRoute", 0.75);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.AddAttribute(maxSideSafeDistWhenAvoidAlongRoute) Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_120 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"Localization", "LidarLoc"}, "VoxelGridSize", 2.0);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.AddAttribute(VoxelGridSize) Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_123 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"DustbinLoc"}, "QrCodeVaildDist", 1.5);
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.AddAttribute(QrCodeVaildDist) Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_124 {

INT32 UpgradeUsCfg(TiXmlElement &xmlElem, const char* us, const char* isSupport, const char* id, const char* type, float tx, float ty, float roll, float pitch, float yaw)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "isSupport", isSupport)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Id",        id))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Type",      type))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Tx",        tx))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Ty",        ty))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Roll",      roll))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Pitch",     pitch))     return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Yaw",       yaw))       return -1;
    return 0;
}

INT32 UpgradeTofCfg(TiXmlElement &xmlElem, const char* tof, const char* isSupport, const char* portName, uint32_t baudRate, float tx, float ty, float tz, float roll, float pitch, float yaw)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "isSupport", isSupport)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "PortName",  portName))  return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "BaudRate",  baudRate))  return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "Tx",        tx))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "Ty",        ty))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "Tz",        tz))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "Roll",      roll))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "Pitch",     pitch))     return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"TofGroup", tof}, "Yaw",       yaw))       return -1;
    return 0;
}

INT32 UpgradeCameraCfg(TiXmlElement &xmlElem, const char* cameraName, const char* isSupport, const char* portName)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName}, "isSupport", isSupport))                          return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "Property"},               "PortName",  portName)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "Property"},               "ImgHeight", 720))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "Property"},               "ImgWidth",  1280))     return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "Property"},               "FrameHz",   25))       return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "CameraMatrix"},           "Data",      "0,0,0,0,0,0,0,0,0")) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "DistortionCoefficients"}, "Data",      "0,0,0,0,0"))         return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "Rvecs"},                  "Data",      "0,0,0"))             return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", cameraName, "Tvecs"},                  "Data",      "0,0,0"))             return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcdGroundSegmentation", "SegSecParams"}, "SecNums", 180)) return -1;

    if (objCfgAmend.RenameElement(objCfg, "Ultrasonic", "UltrasonicGroup")) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LF_US].c_str(), "true",  "0xd0", "KS104", 0.8, 0.55, 0.0, 0.0, 90.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LM_US].c_str(), "true",  "0xd2", "KS104", 0.4, 0.55, 0.0, 0.0, 90.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LD_US].c_str(), "false", "0xd4", "KS104", -0.2, 0.55, 0.0, 0.0, 135.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::BL_US].c_str(), "true",  "0xd6", "KS104", -0.3, 0.4, 0.0, 0.0, 180.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::BR_US].c_str(), "true",  "0xd8", "KS104", -0.3, -0.4, 0.0, 0.0, -180.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RD_US].c_str(), "false", "0xda", "KS104", -0.2, -0.55, 0.0, 0.0, -135.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RM_US].c_str(), "true",  "0xdc", "KS104", 0.4, -0.55, 0.0, 0.0, -90.0)) return -1;
    if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RF_US].c_str(), "true",  "0xde", "KS104", 0.8, -0.55, 0.0, 0.0, -90.0)) return -1;

    if (objCfgAmend.RenameElement(objCfg, "Tof", "TofGroup")) return -1;
    if (UpgradeTofCfg(objCfg, Tof[TOF_ENUM::LS_TOF].c_str(), "true", "/dev/ttyACM0", 921600,  0.6,  0.5, 0.5, 0.0, 60.0,   90.0)) return -1;
    if (UpgradeTofCfg(objCfg, Tof[TOF_ENUM::LD_TOF].c_str(), "true", "/dev/ttyACM1", 921600, -0.2,  0.5, 0.5, 0.0, 60.0,  135.0)) return -1;
    if (UpgradeTofCfg(objCfg, Tof[TOF_ENUM::BL_TOF].c_str(), "true", "/dev/ttyACM2", 921600, -0.3,  0.4, 0.5, 0.0, 60.0,  180.0)) return -1;
    if (UpgradeTofCfg(objCfg, Tof[TOF_ENUM::BR_TOF].c_str(), "true", "/dev/ttyACM3", 921600, -0.3, -0.4, 0.5, 0.0, 60.0, -180.0)) return -1;
    if (UpgradeTofCfg(objCfg, Tof[TOF_ENUM::RD_TOF].c_str(), "true", "/dev/ttyACM4", 921600, -0.2, -0.5, 0.5, 0.0, 60.0, -135.0)) return -1;
    if (UpgradeTofCfg(objCfg, Tof[TOF_ENUM::RS_TOF].c_str(), "true", "/dev/ttyACM5", 921600,  0.6, -0.5, 0.5, 0.0, 60.0,  -90.0)) return -1;

    if (UpgradeCameraCfg(objCfg, RgbCamera[RGB_CAMERA_ENUM::FRONT_LEFT].c_str(),  "false", "/dev/MyVideo0")) return -1;
    if (UpgradeCameraCfg(objCfg, RgbCamera[RGB_CAMERA_ENUM::FRONT_MID].c_str(),   "false", "/dev/MyVideo1")) return -1;
    if (UpgradeCameraCfg(objCfg, RgbCamera[RGB_CAMERA_ENUM::FRONT_RIGHT].c_str(), "false", "/dev/MyVideo2")) return -1;
    if (UpgradeCameraCfg(objCfg, RgbCamera[RGB_CAMERA_ENUM::REAR_MID].c_str(),    "false", "/dev/MyVideo3")) return -1;

    if (objCfgAmend.DeleteElement(objCfg, {"VisionFunction", "LowObstDet"})) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "QRCodeDet"}, "Hz", 10)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet"},    "Hz", 2))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg"},    "Hz", 1))  return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MinX", -20.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MaxX",  20.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MinY", -20.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MaxY",  20.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MinZ",  0.05)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MaxZ",   2.0)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_125 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc"}, "QrCodeVaildDist", 1.5)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc"}, "Type",            1))   return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "QRCodeDet"}, "IPU", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet"},    "IPU", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg"},    "IPU", "false")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_126 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ShortObstHold"}, "isSupport", "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ShortObstHold", "Region"}, "MinX", -3.5)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ShortObstHold", "Region"}, "MaxX",  5.0)) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation"}, "isSupport", "true")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "Property"}, "IsSupportPnp", "true")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "Property"}, "IsSupportPnp", "true")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "Property"}, "IsSupportPnp", "true")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_129 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Localization", "LidarLoc"}, "FitnessScoreThreshold", 0.4)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Localization", "LidarLoc", "LocLidarList",  "Lidar"}, "Name", "MainLidar")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Localization", "Reloc", "RawReloc"}, "isSupportImuAxisZ", "true")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc",   "QrCodeDetect"}, "QrCodeVaildDist", "1.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc",   "PlaneDetect"},  "RoiRadius", "0.3")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc",   "PlaneDetect"},  "RoiPointThreshold", "200")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc",   "PlaneDetect"},  "SensLidar", "BlindLidar")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "QrCodeDetect"},  "QrCodeVaildDist", "1.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "ReflectStripDetect"},  "IntensityThreshold", "200")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "ReflectStripDetect"},  "SensLidar", "MainLidar")) return -1;

    if (objCfgAmend.DeleteAttribute(objCfg, {"DustbinLoc"}, "QrCodeVaildDist"))   return -1;
    if (objCfgAmend.DeleteAttribute(objCfg, {"DustbinLoc"}, "RoiRadius"))         return -1;
    if (objCfgAmend.DeleteAttribute(objCfg, {"DustbinLoc"}, "RoiPointThreshold")) return -1;

    if (objCfgAmend.DeleteAttribute(objCfg, {"ChargingPileLoc"}, "QrCodeVaildDist"))    return -1;
    if (objCfgAmend.DeleteAttribute(objCfg, {"ChargingPileLoc"}, "IntensityThreshold")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_001_174_131 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    /*增加扬尘滤波参数字段，默认关闭*/
    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "Property"}, "IsSupportDustFilter", "false")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "Property"}, "IsSupportDustFilter", "false")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str()}))
        if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "Property"}, "IsSupportDustFilter", "false")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"pnp", "Seg2dObst"}, "Height", 2.0)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_177_000 {

INT32 UpgradeUsCfg(TiXmlElement &xmlElem, const char* us, const char* isSupport, const char* id, const char* type, float tx, float ty, float roll, float pitch, float yaw)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "isSupport", isSupport)) return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Id",        id))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Type",      type))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Tx",        tx))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Ty",        ty))        return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Roll",      roll))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Pitch",     pitch))     return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"UltrasonicGroup", us}, "Yaw",       yaw))       return -1;
    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LinearVelPlan"},  "MaxAngularVel",    "0.2")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"},      "stopFaceSafeDist", "0.5")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "Treelawn"}, "isSupport", "false")) return -1;

    if (objCfgAmend.IsAttributeExist(objCfg, "DustbinLoc", "Type") &&
        objCfgAmend.IsAttributeExist(objCfg, "DustbinLoc", "BinType"))
        if (objCfgAmend.DeleteAttribute(objCfg, "DustbinLoc", "Type")) return -1;

    if (objCfgAmend.IsAttributeExist(objCfg, "DustbinLoc", "Type"))
        if (objCfgAmend.RenameAttribute(objCfg, "DustbinLoc", "Type", "BinType")) return -1;

    if (objCfgAmend.IsElementExist(objCfg,    {"UltrasonicGroup", "LB"}))
        if (objCfgAmend.RenameElement(objCfg, {"UltrasonicGroup", "LB"}, "BL")) return -1;

    if (objCfgAmend.IsElementExist(objCfg,    {"UltrasonicGroup", "RB"}))
        if (objCfgAmend.RenameElement(objCfg, {"UltrasonicGroup", "RB"}, "BR")) return -1;

    if (objCfgAmend.IsElementExist(objCfg,    {"TofGroup", "LB"}))
        if (objCfgAmend.RenameElement(objCfg, {"TofGroup", "LB"}, "BL")) return -1;

    if (objCfgAmend.IsElementExist(objCfg,    {"TofGroup", "RB"}))
        if (objCfgAmend.RenameElement(objCfg, {"TofGroup", "RB"}, "BR")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcdGroundSegmentation", "HyperParam"}, "isUseRPY", "true")) return -1;

    string s = "";
    if (objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", s)) return -1;

    if (s == "S330" || s == "S332")  {
        if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "Height", 0.5)) return -1;
    } else {
        if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "Height", 2.0)) return -1;
    }

    if (s == "S323") {
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LF_US].c_str(), "true",  "0xd0", "KS104", 0.8, 0.55, 0.0, 0.0, 90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LM_US].c_str(), "true",  "0xd2", "KS104", 0.4, 0.55, 0.0, 0.0, 90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LD_US].c_str(), "false", "0xd4", "KS104", -0.2, 0.55, 0.0, 0.0, 135.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::BL_US].c_str(), "true",  "0xd6", "KS104", -0.3, 0.4, 0.0, 0.0, 180.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::BR_US].c_str(), "true",  "0xd8", "KS104", -0.3, -0.4, 0.0, 0.0, -180.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RD_US].c_str(), "false", "0xda", "KS104", -0.2, -0.55, 0.0, 0.0, -135.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RM_US].c_str(), "true",  "0xdc", "KS104",  0.4, -0.55, 0.0, 0.0, -90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RF_US].c_str(), "true",  "0xde", "KS104",  0.8, -0.55, 0.0, 0.0, -90.0)) return -1;
    } else if (s == "S332") {
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LF_US].c_str(), "true",  "0xd0", "KS104", 0.88, 0.49, 0.0, 0.0, 90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LM_US].c_str(), "true",  "0xd2", "KS104", 0.38, 0.49, 0.0, 0.0, 90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::LD_US].c_str(), "false", "0xd4", "KS104", -0.42, 0.48, 0.0, 0.0, 135.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::BL_US].c_str(), "true",  "0xd6", "KS104", -0.5, 0.25, 0.0, 0.0, 180.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::BR_US].c_str(), "true",  "0xd8", "KS104", -0.5, -0.25, 0.0, 0.0, -180.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RD_US].c_str(), "false", "0xda", "KS104", -0.42, -0.48, 0.0, 0.0, -135.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RM_US].c_str(), "true",  "0xdc", "KS104", 0.38, -0.49, 0.0, 0.0, -90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::RF_US].c_str(), "true",  "0xde", "KS104", 0.88, -0.49, 0.0, 0.0, -90.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::FR_US].c_str(), "true",  "0xe0", "KS104", 1.25, -0.4, 0.0, 0.0, 0.0)) return -1;
        if (UpgradeUsCfg(objCfg, Ultrasonic[US_ENUM::FL_US].c_str(), "true",  "0xe2", "KS104", 1.25, 0.4, 0.0, 0.0, 0.0)) return -1;
    } else {
        if (objCfgAmend.SetAttribute(objCfg, "UltrasonicGroup", "isSupport", "false")) return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_184_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()}))
        if (objCfgAmend.DeleteAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "DistParam"}, "MinDist")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()}))
        if (objCfgAmend.DeleteAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "DistParam"}, "MinDist")) return -1;

    if (objCfgAmend.IsElementExist(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str()}))
        if (objCfgAmend.DeleteAttribute(objCfg, {"LidarGroup", Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "DistParam"}, "MinDist")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_185_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    ST_LOG_INFO("Succ.");
    return 0;
}
}


namespace V01_11_197_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "ShortObstHold", "HyperParam"}, "MaxHeight", 0.3)) return -1;

    if (objCfgAmend.IsAttributeExist(objCfg, {"Pnp", "PcdGroundSegmentation", "HyperParam"}, "IsUseRPY")) {
        if (objCfgAmend.IsAttributeExist(objCfg, {"Pnp", "PcdGroundSegmentation", "HyperParam"}, "isUseRPY"))
            if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "PcdGroundSegmentation", "HyperParam"}, "isUseRPY")) return -1;
        if (objCfgAmend.RenameAttribute(objCfg, {"Pnp", "PcdGroundSegmentation", "HyperParam"}, "IsUseRPY", "isUseRPY")) return -1;
    }

    if (objCfgAmend.DeleteElement(objCfg, "pnp")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"DustbinLoc", "QrCodeDetect"}, "QrCodeVaildDist", 2.2)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_198_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteElement(objCfg, {"Pnp", "PcptRegion", "CorseRegion"})) return -1;
    if (objCfgAmend.DeleteElement(objCfg, {"Pnp", "PcptRegion", "FineRegion"}))  return -1;

    if (objCfgAmend.DeleteElement(objCfg, {"VisionFunction", "NetDet", "Unification"})) return -1;
    if (objCfgAmend.DeleteElement(objCfg, {"VisionFunction", "NetDet", "Car"}))         return -1;
    if (objCfgAmend.DeleteElement(objCfg, {"VisionFunction", "NetDet", "Bike"}))        return -1;
    if (objCfgAmend.DeleteElement(objCfg, {"VisionFunction", "NetDet", "Pedestrian"}))  return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Cord"},      "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "Pipeline"},  "isSupport", "false")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "ThickPipe"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "ThickPipe"}, "width",     0.08))    return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_204_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc"}, "enable", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc"}, "enableRelocStation", "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc"}, "enableRecord",       "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc"}, "enableGnss",         "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/trans_points")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "SearchStep"}, "StepX",   "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "SearchStep"}, "StepY",   "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "SearchStep"}, "StepZ",   "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "SearchStep"}, "StepYaw", "0.1")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "VoxelMatch"}, "srcCloudRes",       "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "VoxelMatch"}, "voxelMapRes",       "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "VoxelMatch"}, "minScore",          "100")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "VoxelMatch"}, "maxSearchPointNum", "3"))   return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "type",          "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "minScore",    "0.6")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "srcCloudRes", "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "OmpNdt"}, "resolution", "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "OmpNdt"}, "stepSize",   "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "OmpNdt"}, "transEps",   "0.01")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "OmpNdt"}, "maxIter",    "50"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "OmpNdt"}, "threadNum",  "1"))    return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "GnssLoc"}, "type",  "0"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "GnssLoc", "General"}, "isDualAntenna",  "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "GnssLoc", "General", "Eskf"}, "ng",    "0.001"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "GnssLoc", "General", "Eskf"}, "na",      "0.1"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "GnssLoc", "General", "Eskf"}, "nbg",  "0.0001"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "GnssLoc", "General", "Eskf"}, "nba",    "0.01"))  return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc"}, "type", "0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Lio", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/trans_points"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "type",          "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "minScore",    "0.7")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "srcCloudRes", "2.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "OmpNdt"}, "resolution", "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "OmpNdt"}, "stepSize",   "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "OmpNdt"}, "transEps",   "0.01")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "OmpNdt"}, "maxIter",    "50"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "OmpNdt"}, "threadNum",  "1"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Smoother"}, "windowSize",  "5"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Smoother"}, "weight",      "0.8"))  return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/trans_points"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "type",          "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "minScore",    "0.7")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "srcCloudRes", "2.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "OmpNdt"}, "resolution", "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "OmpNdt"}, "stepSize",   "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "OmpNdt"}, "transEps",   "0.01")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "OmpNdt"}, "maxIter",    "50"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "OmpNdt"}, "threadNum",  "1"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Smoother"}, "windowSize",  "5"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Smoother"}, "weight",      "0.8"))  return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "GloalPlan"}, "isSupportRoadPortTurnAround", "true")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_207_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Pipeline"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "ReflectStripDetect"}, "ChargingPileHeight", 1.28)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "ReflectStripDetect"}, "OffsetY", 0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Localization", "Reloc"}, "FitnessScoreThreshold", 0.25)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "rangeDistGarbageBinPose", 0.2)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "trashBoxCtrlThresh", 2.0)) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio"}, "correctPeriod", "0.25")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio"}, "correctPeriod",  "0.1")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/car_filtered_points")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Lio", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/car_filtered_points"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/car_filtered_points"))  return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "QrCode"},       "VaildDist",          1.5))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"},      "BinType",            1))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "IntensityThreshold", 200))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "ChargingPileHeight", 1.28)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "OffsetY",            0.0))  return -1;

    if (objCfgAmend.DeleteElement(objCfg, "DustbinLoc")) return -1;
    if (objCfgAmend.DeleteElement(objCfg, "ChargingPileLoc")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "QRCodeDet"}, "Camera", "rear_mid"))               return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet"},    "Camera", "front_left,front_right")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg"},    "Camera", "front_left,front_right")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_208_000 {

void CameraMatrixSTR2DBL(string& strData, vector<double>& vData)
{
    istringstream iss(strData);
    string strRslt;
    while(getline(iss, strRslt, ',')) vData.push_back(atof(strRslt.c_str()));
}

INT32 RGBCamearExternalParams(TiXmlElement &xmlElem, const char* camera)
{
    CFG_AMEND_C objCfgAmend;
    if (!objCfgAmend.IsElementExist(xmlElem, {"CameraGroup", camera})) return 0;

    string strTransData, strRoatateData;
    if (objCfgAmend.GetAttribute(xmlElem, {"CameraGroup", camera, "Tvecs"}, "Data", strTransData)) return -1;
    if (objCfgAmend.GetAttribute(xmlElem, {"CameraGroup", camera, "Rvecs"}, "Data", strRoatateData)) return -1;

    vector<double> vTransData;
    CameraMatrixSTR2DBL(strTransData, vTransData);

    vector<double> vRotateData;
    CameraMatrixSTR2DBL(strRoatateData, vRotateData);

    Eigen::Vector3d objRotateVec(vRotateData[0], vRotateData[1], vRotateData[2]);
    Eigen::Matrix3d objRotateMat = Eigen::AngleAxisd(objRotateVec.norm(), objRotateVec.normalized()).matrix();

    Eigen::Matrix4f objMatrixCamera2Car;
    objMatrixCamera2Car << objRotateMat(0, 0), objRotateMat(0, 1), objRotateMat(0, 2), vTransData[0],
                           objRotateMat(1, 0), objRotateMat(1, 1), objRotateMat(1, 2), vTransData[1],
                           objRotateMat(2, 0), objRotateMat(2, 1), objRotateMat(2, 2), vTransData[2],
                                          0.0,                0.0,                0.0,           1.0;
    COOR_3D_POSE_STRU stPose(objMatrixCamera2Car.inverse());

    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", camera, "InstallParam"}, "Tx",    stPose.x() / 1000)) return -1; //单位：毫米 -> 米
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", camera, "InstallParam"}, "Ty",    stPose.y() / 1000)) return -1; //单位：毫米 -> 米
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", camera, "InstallParam"}, "Tz",    stPose.z() / 1000)) return -1; //单位：毫米 -> 米
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", camera, "InstallParam"}, "Roll",  stPose.roll()))      return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", camera, "InstallParam"}, "Pitch", stPose.pitch()))    return -1;
    if (objCfgAmend.SetAttribute(xmlElem, {"CameraGroup", camera, "InstallParam"}, "Yaw",   stPose.yaw()))       return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"CameraGroup", camera, "Rvecs"})) return -1;
    if (objCfgAmend.DeleteElement(xmlElem, {"CameraGroup", camera, "Tvecs"})) return -1;

    return 0;
}

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (RGBCamearExternalParams(objCfg, RgbCamera[RGB_CAMERA_ENUM::FRONT_LEFT].c_str()))    return -1;
    if (RGBCamearExternalParams(objCfg, RgbCamera[RGB_CAMERA_ENUM::FRONT_MID].c_str()))     return -1;
    if (RGBCamearExternalParams(objCfg, RgbCamera[RGB_CAMERA_ENUM::FRONT_RIGHT].c_str()))   return -1;
    if (RGBCamearExternalParams(objCfg, RgbCamera[RGB_CAMERA_ENUM::REAR_RIGHT].c_str()))    return -1;
    if (RGBCamearExternalParams(objCfg, RgbCamera[RGB_CAMERA_ENUM::REAR_MID].c_str()))      return -1;
    if (RGBCamearExternalParams(objCfg, RgbCamera[RGB_CAMERA_ENUM::REAR_RIGHT].c_str()))    return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_216_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", "Offset"},  "Tx",    "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", "Offset"},  "Ty",    "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", "Offset"},  "Tz",    "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", "Offset"},  "Roll",  "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", "Offset"},  "Pitch", "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", "Offset"},  "Yaw",   "0")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Imu", "Offset"},  "Tx",    "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Imu", "Offset"},  "Ty",    "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Imu", "Offset"},  "Tz",    "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Imu", "Offset"},  "Roll",  "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Imu", "Offset"},  "Pitch", "0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Imu", "Offset"},  "Yaw",   "0")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "GloalPlan"}, "skipExecuteLen", 5.0)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_217_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold", "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "TrafficLight"},  "isSupport", "false")) return -1;
    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_221_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc",   "FineReloc", "VoxelMatch"}, "minScore",             "0.3"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc",   "FineReloc", "VoxelMatch"}, "maxSearchPointNum",    "5"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "rawResolution",  "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "fineResolution", "0.5"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "stepSize",       "0.1"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "transEps",       "0.01")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "outlierThr",     "20.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "maxIter",        "20"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "threadNum",      "1"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "MixNdt"}, "srcCloudDsRes",  "2.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "rawResolution",  "1.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "fineResolution", "0.5"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "stepSize",       "0.1"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "transEps",       "0.01")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "outlierThr",     "20.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "maxIter",        "20"))   return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "threadNum",      "1"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "MixNdt"}, "srcCloudDsRes",  "2.0"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio"}, "correctPeriod", "0.001"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio"}, "correctPeriod", "0.001"))    return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_237_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    const char* frontMidLeft = RgbCamera[RGB_CAMERA_ENUM::FRONT_MID_LEFT].c_str();
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft},             "isSupport", "false"))         return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "Property"}, "PortName",  "/dev/MyVideo2")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "Property"}, "ImgHeight", 720))             return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "Property"}, "ImgWidth",  1280))            return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "Property"}, "FrameHz",   25))              return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "CameraMatrix"},           "Data","0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "DistortionCoefficients"}, "Data","0.0, 0.0, 0.0, 0.0, 0.0"))                     return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "InstallParam"}, "Tx",    0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "InstallParam"}, "Ty",    0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "InstallParam"}, "Tz",    0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "InstallParam"}, "Roll",  0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "InstallParam"}, "Pitch", 0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidLeft, "InstallParam"}, "Yaw",   0.0)) return -1;

    const char* frontMidRight = RgbCamera[RGB_CAMERA_ENUM::FRONT_MID_RIGHT].c_str();
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight},             "isSupport", "false"))         return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "Property"}, "PortName",  "/dev/MyVideo3")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "Property"}, "ImgHeight", 720))             return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "Property"}, "ImgWidth",  1280))            return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "Property"}, "FrameHz",   25))              return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "CameraMatrix"},           "Data","0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "DistortionCoefficients"}, "Data","0.0, 0.0, 0.0, 0.0, 0.0"))                     return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "InstallParam"}, "Tx",    0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "InstallParam"}, "Ty",    0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "InstallParam"}, "Tz",    0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "InstallParam"}, "Roll",  0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "InstallParam"}, "Pitch", 0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"CameraGroup", frontMidRight, "InstallParam"}, "Yaw",   0.0)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_238_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "TrafficLight"}, "PixelDeviation", 200)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "TrafficLight"}, "MaxDiffRatio",   0.2)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "SolidCover"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "HollowCover"}, "isSupport", "false")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_243_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    /* 本配置版本主要是收编模板分支，相对于老版本没有配置变更。 */
    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_243_000_VS_V01_11_237_002 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    INT32 rslt = V01_11_238_000::Upgrade(objCfg);
    if (rslt != 0) {
        ST_LOG_ERR("V01_11_238_000::Upgrade() Fail.");
        return -1;
    }

    CFG_AMEND_C objCfgAmend;
    rslt = objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "ShortObstHold"}, "test1");
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.DeleteAttribute(test1) Fail.");
        return -1;
    }

    rslt = objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "ShortObstHold"}, "test2");
    if (rslt != 0) {
        ST_LOG_ERR("objCfgAmend.DeleteAttribute(test2) Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_244_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "AccessibleArea"}, "isSupport", "true")) return -1;
    if (0 != objCfgAmend.SetAttribute(objCfg, {"Plan", "TimeParam"}, "waitEscapeTime", "5.0")) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_245_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

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

    INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"Plan", "GloalPlan"}, "minTurnAroundRoadWidth", 5.0);
    if (rslt != 0) {
        ST_LOG_ERR("Add Plan.GloalPlan.minTurnAroundRoadWidth Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_245_000_VS_V01_11_203_082 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

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

    INT32 rslt = objCfgAmend.AddAttribute(objCfg, {"Plan", "GloalPlan"}, "isSupportRoadPortTurnAround", true);
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

namespace V01_11_250_000 {

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

namespace V01_11_251_000 {

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

namespace V01_11_264_000 {

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

namespace V01_11_265_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "StepPadDet"}, "isSupport",  "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "StepPadDet"}, "ExtandDist", 0.3))    return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "isUseVelItem", "true")) return -1;
    if (objCfgAmend.RenameElement(objCfg, {"Pnp", "StepPadDet"}, "SlopePadDet")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "SlopePad"}, "isSupport", "false")) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isDoorCloseRequire", "true")) return -1;

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

    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "brushShortenLen", 0.0)) return -1;

    return 0;
}
}


namespace V01_11_283_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold", 0.3)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "RoadRegionFilter"}, "isSupport", "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, "VisionFunction", "isUseApi", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "DistThresh", 0.2)) return -1;
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


namespace V01_11_285_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "Seg2dObst"}, "Height")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMinHeight", 0.5)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMaxHeight", 2.0)) return -1;

    return 0;
}
}


namespace V01_11_296_000 {

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

namespace V01_11_299_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"},  "isDynamicObstStop", "false")) return -1;

    return 0;
}
}


namespace V01_11_303_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"},  "isSmartCtrlBrush", "true")) return -1;

    return 0;
}
}

namespace V01_11_306_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "FilterPointsRebuild"},  "isSupport", "false")) return -1;

    return 0;
}
}

namespace V01_11_307_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "matchThrK",   "0.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "matchThrB",   "0.315")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "matchThrK",   "0.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer"}, "matchThrB",   "0.315")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Ndt"}, "transEps",   "0.0001")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter"}, "type",   "1")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeX", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeY", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeZ", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "voxelNum", "800")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "leafSize", "0.5")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "matchThrK",   "0.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer"}, "matchThrB",   "0.315")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Ndt"}, "transEps",   "0.0001")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter"}, "type",   "1")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeX", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeY", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "VoxelGridFilter"}, "leafSizeZ", "2.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "voxelNum", "800")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DownSampleFilter", "AdaptiveVoxelGridFilter"}, "leafSize", "0.5")) return -1;

    return 0;
}
}


namespace V01_11_309_000 {

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

INT32 AddImuPort(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg, const char* lidarName, const int value)
{
    /* 是否有该雷达 */
    if (!objCfgAmend.IsElementExist(objCfg, {"LidarGroup", lidarName})) return 0;
    if (objCfgAmend.SetAttribute(objCfg, {"LidarGroup", lidarName, "NetCfg"}, "ImuPort", value)) return -1;
    return 0;
}

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
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str()))  return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str()))  return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str())) return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str())) return -1;
    if (UpgradeLidarCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str())) return -1;

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Warehouse"}, "ReflectiveStripHeight", 0.3)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Warehouse"}, "IntensityThreshold",    180)) return -1;

    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), 8901)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), 8902)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), 8903)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str(), 8904)) return -1;
    if (AddImuPort(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str(), 8905)) return -1;

    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::MAIN_LIDAR].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR2].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR3].c_str(), "true")) return -1;
    if (AddLidarShadowFilterCfg(objCfgAmend, objCfg, Lidar[LIDAR_ENUM::BLIND_LIDAR4].c_str(), "true")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_314_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Bough"},        "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Cup"},          "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Can"},          "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Carton"},       "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Package"},      "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Brick"},        "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Cigaretteend"}, "isSupport", "false")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "DataProcess"}, "InputCloudMinRange",  "1.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "DataProcess"}, "InputCloudMaxRange", "80.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "DataProcess"}, "InputCloudJumpNum",   "4"))   return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "ActiveMap"}, "enable",      "false"))  return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "ActiveMap"}, "VoxelMapSize", "0.2"))   return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "ActiveMap"}, "MaxVoxelSize", "80000")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Pericarp"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "CarBodyDenoising​​"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "CarBodyDenoising​​"}, "Resolution", "0.3"))  return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp"}, "isSupport", "true"))                      return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "TaskMng"}, "trafficControlCheckDist", 10.0)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}


namespace V01_11_315_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isAvoidObstRemotely", "false")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_321_000 {

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

namespace V01_11_321_000_VS_V01_11_247_212
{

    INT32 Upgrade(TiXmlElement &objCfg)
    {
        ST_LOG_INFO("Succ.");
        return 0;
    }

} // namespace

namespace V01_11_339_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.IsElementExist(objCfg,    {"Pnp", "CarBodyDenoising​​"})) {
        if (objCfgAmend.RenameElement(objCfg, {"Pnp", "CarBodyDenoising​​"}, "CarBodyDenoising"))
            return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

}

namespace V01_11_343_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Car"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "BlackVehicle"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetSeg", "FishingRod"}, "isSupport", "false")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}

}

namespace V01_11_358_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "WheelSpeedSensor"}, "ScaleFactor", 1.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "SteeringAngleSensor"}, "ScaleFactor", 1.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "SteeringAngleSensor"}, "OffsetFactor", 0.0)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "stopFaceSafeDist", 0.1)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "parkFaceSafeDist", 0.1)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_366_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMinHeight", 0.5)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMaxHeight", 0.9)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_376_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMinHeight", 0.2)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMaxHeight", 0.3)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_377_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetSeg", "FishingRodLlm"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetSeg", "TreelawnLlm"},   "isSupport", "false")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_388_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    std::string strModel;

    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", strModel)) return -1;

    float pcptRegionMaxZ = 10;
    if (   strModel == "S310" || strModel == "S320" || strModel == "S323"
        || strModel == "S324" || strModel == "S325" || strModel == "S326"
        || strModel == "S330" || strModel == "S332" || strModel == "S333"
        || strModel == "S334" || strModel == "S335" || strModel == "S336"
        || strModel == "S337") {
        pcptRegionMaxZ = 1.8;
    } else if ( strModel == "S350") {
        pcptRegionMaxZ = 1.6;
    } else if (strModel == "S520" || strModel == "S530") {
        pcptRegionMaxZ = 3.0;
    } else {
        ST_LOG_ERR("Car Model<%s> Unknow.", strModel.c_str());
        return -1;
    }

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PcptRegion", "ObstRegion"}, "MaxZ", pcptRegionMaxZ)) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_396_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetSeg", "Cord"}, "isSupport", "false")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}

namespace V01_11_400_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "DcuInfo"}, "EnableCarPitch", "true")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace V01_11_408_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    std::string strModel;
    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", strModel)) return -1;

    if (strModel == "S310" || strModel == "S320" || strModel == "S323" ||
        strModel == "S324" || strModel == "S325" || strModel == "S326" ||
        strModel == "S330" || strModel == "S332" || strModel == "S333" ||
        strModel == "S334" || strModel == "S335" || strModel == "S336" ||
        strModel == "S337")
    {
        if (objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "DcuInfo"}, "EnableCarPitch", "false"))
        {
            return -1;
        }
    } 
    else 
    {
        if (objCfgAmend.SetAttribute(objCfg, {"VehicleChassis", "DcuInfo"}, "EnableCarPitch", "true"))
        {
            return -1;
        }
    }
    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace V01_11_411_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    std::string strModel;
    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", strModel)) return -1;

    if (strModel == "S320" || strModel == "S323" || strModel == "S324" || strModel == "S326")
    {
        if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "stopFaceSafeDist", 0.7)) return -1;
        if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "parkFaceSafeDist", 0.5)) return -1;
    } 
    else 
    {
        if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "stopFaceSafeDist", 0.1)) return -1;
        if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "parkFaceSafeDist", 0.1)) return -1;
    }
    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace V01_11_413_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "Cleanliness"}, "isSupport", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "Cleanliness"}, "length", "5")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "Cleanliness"}, "width", "2")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Pnp", "Cleanliness"}, "coefficient", "5")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace V01_11_419_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"Plan", "ChargeParam"}, "stopLenToEquipment", "0.38")) return -1;

    // Add location small_gicp params
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "voxelRes", "1.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "transEps", "0.01")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "maxIter",  "20")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "threadNum",  "2")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "maxCorrespondDist",  "0.5")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "correspondenceRand",  "16")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "useVGICP",  "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer", "SmallGicp"}, "verbose",  "false")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "voxelRes", "1.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "transEps", "0.01")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "maxIter",  "15")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "threadNum",  "2")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "maxCorrespondDist",  "0.5")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "correspondenceRand",  "16")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "useVGICP",  "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "SmallGicp"}, "verbose",  "false")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "voxelRes", "1.0")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "transEps", "0.01")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "maxIter",  "15")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "threadNum",  "2")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "maxCorrespondDist",  "0.5")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "correspondenceRand",  "16")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "useVGICP",  "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "SmallGicp"}, "verbose",  "false")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "DiffDistance"}, "distanceThr",  "0.65")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Registerer", "Mapleafsize"}, "leafsize",  "0.5")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Smoother"}, "DiffDistance",  "0.65")) return -1;

    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "DiffDistance"}, "distanceThr",  "0.65")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Registerer", "Mapleafsize"}, "leafsize",  "0.5")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "Smoother"}, "DiffDistance",  "0.65")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace V01_11_423_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"Plan", "FunctionParam"}, "isAligningUseLastPose", "false")) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"Plan", "FunctionParam"}, "isEscapeSideObst", "true")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace V01_11_425_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    std::string strModel;
    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.GetAttribute(objCfg, {"VehicleBody", "StructureInfo"}, "model", strModel)) return -1;

    if (strModel == "S330" || strModel == "S332" || strModel == "S333" || strModel == "S334"
        || strModel == "S335" || strModel == "S336" || strModel == "S337")
    {
        if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isEscapeSideObst", "false")) return -1;
    } 
    else 
    {
        if (objCfgAmend.SetAttribute(objCfg, {"Plan", "FunctionParam"}, "isEscapeSideObst", "true")) return -1;
    }
    ST_LOG_INFO("Succ.");

    return 0;
}

}


namespace NEW_CFG_VER {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.AddAttribute(objCfg, {"Plan", "ChargeParam"}, "addWaterAdjustLen", "0.0")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}

}



/**************************************************************************************
功能描述: 升级注册表
修改记录:
**************************************************************************************/
const UPGRADE_REG_STRU g_astUpgradeTabOfMaster[] =
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
    {"V01.11.001.174.096",    V01_11_001_174_096::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.098",    V01_11_001_174_098::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.101",    V01_11_001_174_101::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.102",    V01_11_001_174_102::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.108",    V01_11_001_174_108::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.111",    V01_11_001_174_111::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.114",    V01_11_001_174_114::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.120",    V01_11_001_174_120::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.123",    V01_11_001_174_123::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.124",    V01_11_001_174_124::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.125",    V01_11_001_174_125::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.126",    V01_11_001_174_126::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.129",    V01_11_001_174_129::Upgrade,      "",                 nullptr},
    {"V01.11.001.174.131",    V01_11_001_174_131::Upgrade,      "",                 nullptr},
    {"V01.11.177.000",        V01_11_177_000::Upgrade,          "",                 nullptr},
    {"V01.11.184.000",        V01_11_184_000::Upgrade,          "",                 nullptr},
    {"V01.11.185.000",        V01_11_185_000::Upgrade,          "",                 nullptr},
    {"V01.11.197.000",        V01_11_197_000::Upgrade,          "",                 nullptr},
    {"V01.11.198.000",        V01_11_198_000::Upgrade,          "",                 nullptr},
    {"V01.11.204.000",        V01_11_204_000::Upgrade,          "",                 nullptr},
    {"V01.11.207.000",        V01_11_207_000::Upgrade,          "",                 nullptr},
    {"V01.11.208.000",        V01_11_208_000::Upgrade,          "",                 nullptr},
    {"V01.11.216.000",        V01_11_216_000::Upgrade,          "",                 nullptr},
    {"V01.11.217.000",        V01_11_217_000::Upgrade,          "",                 nullptr},
    {"V01.11.221.000",        V01_11_221_000::Upgrade,          "",                 nullptr},
    {"V01.11.237.000",        V01_11_237_000::Upgrade,          "",                 nullptr},
    {"V01.11.238.000",        V01_11_238_000::Upgrade,          "",                 nullptr},
    {"V01.11.243.000",        V01_11_243_000::Upgrade,          "V01.11.237.002",   V01_11_243_000_VS_V01_11_237_002::Upgrade},
    {"V01.11.244.000",        V01_11_244_000::Upgrade,          "",                 nullptr},
    {"V01.11.245.000",        V01_11_245_000::Upgrade,          "V01.11.203.082",   V01_11_245_000_VS_V01_11_203_082::Upgrade},
    {"V01.11.250.000",        V01_11_250_000::Upgrade,          "",                 nullptr},
    {"V01.11.251.000",        V01_11_251_000::Upgrade,          "",                 nullptr},
    {"V01.11.264.000",        V01_11_264_000::Upgrade,          "",                 nullptr},
    {"V01.11.265.000",        V01_11_265_000::Upgrade,          "",                 nullptr},
    {"V01.11.283.000",        V01_11_283_000::Upgrade,          "",                 nullptr},
    {"V01.11.285.000",        V01_11_285_000::Upgrade,          "",                 nullptr},
    {"V01.11.296.000",        V01_11_296_000::Upgrade,          "",                 nullptr},
    {"V01.11.299.000",        V01_11_299_000::Upgrade,          "",                 nullptr},
    {"V01.11.303.000",        V01_11_303_000::Upgrade,          "",                 nullptr},
    {"V01.11.306.000",        V01_11_306_000::Upgrade,          "",                 nullptr},
    {"V01.11.307.000",        V01_11_307_000::Upgrade,          "",                 nullptr},
    {"V01.11.309.000",        V01_11_309_000::Upgrade,          "",                 nullptr},
    {"V01.11.314.000",        V01_11_314_000::Upgrade,          "",                 nullptr},
    {"V01.11.315.000",        V01_11_315_000::Upgrade,          "",                 nullptr},
    {"V01.11.321.000",        V01_11_321_000::Upgrade,          "V01.11.247.212",   V01_11_321_000_VS_V01_11_247_212::Upgrade},
    {"V01.11.339.000",        V01_11_339_000::Upgrade,          "",                 nullptr},
    {"V01.11.343.000",        V01_11_343_000::Upgrade,          "",                 nullptr},
    {"V01.11.358.000",        V01_11_358_000::Upgrade,          "",                 nullptr},
    {"V01.11.366.000",        V01_11_366_000::Upgrade,          "",                 nullptr},
    {"V01.11.376.000",        V01_11_376_000::Upgrade,          "",                 nullptr},
    {"V01.11.377.000",        V01_11_377_000::Upgrade,          "",                 nullptr},
    {"V01.11.388.000",        V01_11_388_000::Upgrade,          "",                 nullptr},
    {"V01.11.396.000",        V01_11_396_000::Upgrade,          "",                 nullptr},
    {"V01.11.400.000",        V01_11_400_000::Upgrade,          "",                 nullptr},
    {"V01.11.408.000",        V01_11_408_000::Upgrade,          "",                 nullptr},
    {"V01.11.411.000",        V01_11_411_000::Upgrade,          "",                 nullptr},
    {"V01.11.413.000",        V01_11_413_000::Upgrade,          "",                 nullptr},
    {"V01.11.419.000",        V01_11_419_000::Upgrade,          "",                 nullptr},
    {"V01.11.423.000",        V01_11_423_000::Upgrade,          "",                 nullptr},
    {"V01.11.425.000",        V01_11_425_000::Upgrade,          "",                 nullptr},
    {CFG_VERSION,             NEW_CFG_VER::Upgrade,             "",                 nullptr}
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



