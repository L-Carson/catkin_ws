
#include "comm/comm_cfg/comm_cfg_amend.h"
#include "comm/comm_cfg/comm_cfg_smoother.h"
#include "comm/comm_car_body_file.h"


namespace COMM
{

namespace CFG
{

namespace V01_11_203_013_VS_V01_11_198_000 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Localization", "Reloc"}, "FitnessScoreThreshold", 0.25)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_015 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "rangeDistGarbageBinPose", 0.2)) return -1;
    if (objCfgAmend.AddAttribute(objCfg, {"VisionFunction", "NetDet", "Pipeline"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "ReflectStripDetect"}, "ChargingPileHeight", 1.28)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"ChargingPileLoc", "ReflectStripDetect"}, "OffsetY", 0.0)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_020 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "trashBoxCtrlThresh", 2.0)) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_025 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "QrCode"},       "VaildDist",          1.5))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"},      "BinType",            1))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "IntensityThreshold", 200))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "ChargingPileHeight", 1.28)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "ChargingPile"}, "OffsetY",            0.0))  return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_026 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "QRCodeDet"}, "Camera", "rear_mid"))               return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet"},    "Camera", "front_left,front_right")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg"},    "Camera", "front_left,front_right")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_028 {

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

namespace V01_11_203_031 {

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

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_203_035 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc"}, "enable", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc"}, "enableRelocStation", "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc"}, "enableRecord",       "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc"}, "enableGnss",         "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "CoarseReloc", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/car_filtered_points")) return -1;

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
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio", "Lio", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/car_filtered_points"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocLio"}, "correctPeriod",  "0.25")) return -1;
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

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio", "LidarTopicList", "LidarTopic"}, "name", "/MainLidar/car_filtered_points"))  return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "CombLoc", "LidarLoc", "LidarLocWio"}, "correctPeriod",  "0.1")) return -1;
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

    if (objCfgAmend.DeleteElement(objCfg, "DustbinLoc"))      return -1;
    if (objCfgAmend.DeleteElement(objCfg, "ChargingPileLoc")) return -1;

    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_203_047 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;

    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "VoxelMatch"}, "minScore",          "0.35")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "VoxelMatch"}, "maxSearchPointNum", "5"))    return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VehicleLoc", "Reloc", "FineReloc", "Registerer"}, "srcCloudRes",       "1.0"))  return -1;

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

namespace V01_11_203_060 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.SetAttribute(objCfg, {"Plan", "GloalPlan"}, "minTurnAroundRoadWidth", "5.0")) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }
    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_203_065 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (0 != objCfgAmend.SetAttribute(objCfg, {"Plan", "TimeParam"}, "waitEscapeTime", "5.0")) {
        ST_LOG_ERR("objCfgAmend.SetAttribute() Fail.");
        return -1;
    }
    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_203_075 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold", "0.5")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetDet", "TrafficLight"},  "isSupport", "false")) return -1;

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

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "TrafficLight"}, "PixelDeviation", 200)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "TrafficLight"}, "MaxDiffRatio",   0.2)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "SolidCover"},  "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "HollowCover"}, "isSupport", "false")) return -1;


    ST_LOG_INFO("Succ.");

    return 0;
}
}

namespace V01_11_203_076 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "AccessibleArea"}, "isSupport", "true")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_082 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "AccessibleArea"}, "isSupport", "false")) return -1;

    ST_LOG_INFO("Succ.");
    return 0;
}
}

namespace V01_11_203_086 {

INT32 CopyCarBodyFilterFileToNew(CFG_AMEND_C &objCfgAmend, TiXmlElement &objCfg)
{
    float vxlCell;
    CUBOID_STRU stCuboidOld;
    if (objCfgAmend.GetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "VoxelSize", vxlCell))       return -1;
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
        case CAR_TYPE_S350: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.30; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.30; break; //无前扫刷
        case CAR_TYPE_S520: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        case CAR_TYPE_S530: stCuboidNew.minX = -pstCarBodyInfo->tailLen - 0.50; stCuboidNew.maxX = pstCarBodyInfo->wheelBaseLen + pstCarBodyInfo->stDynamicInfo.headLen + 0.50; break;
        default: ST_LOG_ERR("无效的车型参数: %s.", strCarType.c_str()); return -1;
    }
    stCuboidNew.minZ = 0.03;

    CAR_BODY_FILE_C objCarBodyFile(vxlCell, stCuboidOld, strCarType);
    if (objCarBodyFile.CopyCarBodyFilterdFileToNew(0.03, stCuboidNew)) {
        ST_LOG_ERR("将车体构造文件拷贝到新的参数体系下失败。");
        return -1;
    }

    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinX", stCuboidNew.minX)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MaxX", stCuboidNew.maxX)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "CarBodyVxlFilter", "FilterSize"}, "MinZ", stCuboidNew.minZ)) return -1;

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

namespace V01_11_203_093 {

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

namespace V01_11_203_098 {

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

namespace V01_11_203_113 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "StepPadDet"}, "isSupport",  "true")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "StepPadDet"}, "ExtandDist", 0.3))    return -1;

    return 0;
}

}

namespace V01_11_203_117 {

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


namespace V01_11_203_130 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "PlaneSegmentation", "HyperParam"}, "isUseVelItem", "true")) return -1;

    return 0;
}
}


namespace V01_11_203_132 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.RenameElement(objCfg, {"Pnp", "StepPadDet"}, "SlopePadDet")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg"}, "SlopePad", "false")) return -1;

    return 0;
}
}

namespace V01_11_203_135 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"VisionFunction", "NetSeg"}, "SlopePad")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"VisionFunction", "NetSeg", "SlopePad"}, "isSupport", "false")) return -1;

    return 0;
}
}

namespace V01_11_203_137 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.SetAttribute(objCfg, {"Plan", "LocalPlan"}, "brushShortenLen", 0.0)) return -1;

    return 0;
}
}

namespace V01_11_203_164 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "ObstState"}, "StaticThreshold", 0.3)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "RoadRegionFilter"}, "isSupport", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, "VisionFunction", "isUseApi", "false")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "DetectPile", "Dustbin"}, "FullThresh", 0.2)) return -1;

    return 0;
}
}


namespace V01_11_203_168 {

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


namespace V01_11_203_171 {

INT32 Upgrade(TiXmlElement &objCfg)
{
    CFG_AMEND_C objCfgAmend;
    if (objCfgAmend.DeleteAttribute(objCfg, {"Pnp", "Seg2dObst"}, "Height")) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMinHeight", 0.5)) return -1;
    if (objCfgAmend.SetAttribute(objCfg, {"Pnp", "Seg2dObst"}, "MidLayerMaxHeight", 2.0)) return -1;

    return 0;
}
}

/**************************************************************************************
功能描述: 升级注册表
修改记录:
**************************************************************************************/
const UPGRADE_REG_STRU g_astUpgradeTabOfRelease203[] =
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
    {"V01.11.203.013",        nullptr,                          "V01.11.198.000",   V01_11_203_013_VS_V01_11_198_000::Upgrade},
    {"V01.11.203.015",        V01_11_203_015::Upgrade,          "",                 nullptr},
    {"V01.11.203.020",        V01_11_203_020::Upgrade,          "",                 nullptr},
    {"V01.11.203.025",        V01_11_203_025::Upgrade,          "",                 nullptr},
    {"V01.11.203.026",        V01_11_203_026::Upgrade,          "",                 nullptr},
    {"V01.11.203.028",        V01_11_203_028::Upgrade,          "",                 nullptr},
    {"V01.11.203.031",        V01_11_203_031::Upgrade,          "",                 nullptr},
    {"V01.11.203.035",        V01_11_203_035::Upgrade,          "",                 nullptr},
    {"V01.11.203.047",        V01_11_203_047::Upgrade,          "",                 nullptr},
    {"V01.11.203.060",        V01_11_203_060::Upgrade,          "",                 nullptr},
    {"V01.11.203.065",        V01_11_203_065::Upgrade,          "",                 nullptr},
    {"V01.11.203.075",        V01_11_203_075::Upgrade,          "",                 nullptr},
    {"V01.11.203.076",        V01_11_203_076::Upgrade,          "",                 nullptr},
    {"V01.11.203.082",        V01_11_203_082::Upgrade,          "",                 nullptr},
    {"V01.11.203.086",        V01_11_203_086::Upgrade,          "",                 nullptr},
    {"V01.11.203.093",        V01_11_203_093::Upgrade,          "",                 nullptr},
    {"V01.11.203.098",        V01_11_203_098::Upgrade,          "",                 nullptr},
    {"V01.11.203.113",        V01_11_203_113::Upgrade,          "",                 nullptr},
    {"V01.11.203.117",        V01_11_203_117::Upgrade,          "",                 nullptr},
    {"V01.11.203.130",        V01_11_203_130::Upgrade,          "",                 nullptr},
    {"V01.11.203.132",        V01_11_203_132::Upgrade,          "",                 nullptr},
    {"V01.11.203.135",        V01_11_203_135::Upgrade,          "",                 nullptr},
    {"V01.11.203.137",        V01_11_203_137::Upgrade,          "",                 nullptr},
    {"V01.11.203.164",        V01_11_203_164::Upgrade,          "",                 nullptr},
    {"V01.11.203.168",        V01_11_203_168::Upgrade,          "",                 nullptr},
    {"V01.11.203.171",        V01_11_203_171::Upgrade,          "",                 nullptr}
    /* 【配置升级注册说明】
            修改配置文件 checklist :
                (1) 将上一个配置注册版本的升级函数的域名 NEW_CFG_VER 修改为此时 CFG_VERSION 对应的版本号，比如 V01_01_001_000_001；
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


