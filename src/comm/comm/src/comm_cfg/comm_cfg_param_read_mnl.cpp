
#include "comm/comm_cfg/comm_cfg_param_read_mnl.h"

namespace COMM
{

namespace CFG
{


/**************************************************************************************
功能描述: 读取定位参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadLocalizationCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LOC_STRU &stLocCfg)
{
    TiXmlElement *pobjLocCfg = pobjCfgParam->FirstChildElement("Localization");
    if (pobjLocCfg == NULL) {
        ST_LOG_ERR("Get <Localization> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjPoseEstimatorCfg = pobjLocCfg->FirstChildElement("PoseEstimator");
    if (ReadPoseEstimatorCfg(pobjPoseEstimatorCfg, stLocCfg.stPoseEstimator) != 0){
        ST_LOG_ERR("Pose Estimator Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjLidarLocCfg = pobjLocCfg->FirstChildElement("LidarLoc");
    if (ReadLidarLocCfg(pobjLidarLocCfg, stLocCfg.stLidarLoc) != 0){
        ST_LOG_ERR("Lidar Loc Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjReLocCfg = pobjLocCfg->FirstChildElement("Reloc");
    if (ReadReLocalizationCfg(pobjReLocCfg, stLocCfg.stReloc) != 0) {
        ST_LOG_ERR("Reloc Cfg Param Error!");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取位姿估计器参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadPoseEstimatorCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_POSE_ESTIMATOR_STRU &stPoseEstimator)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <PoseEstimator> Cfg Element Fail.");
        return -1;
    }

    INT32 fusionMethod;
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("FusionMethod", &fusionMethod)) {
        ST_LOG_ERR("Get <PoseEstimator> Cfg Attr <FusionMethod> Fail.");
        return -1;
    }
    stPoseEstimator.enFusionMethod = (FUSION_METHOD_ENUM)fusionMethod;
    INT32 motionModel;
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("MotionModel", &motionModel)) {
        ST_LOG_ERR("Get <PoseEstimator> Cfg Attr <MotionModel> Fail.");
        return -1;
    }
    stPoseEstimator.enMotionModel = (MOTION_MODEL_ENUM)motionModel;
    INT32 lidarObsModel;
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("LidarObsModel", &lidarObsModel)) {
        ST_LOG_ERR("Get <PoseEstimator> Cfg Attr <LidarObsModel> Fail.");
        return -1;
    }
    stPoseEstimator.enLidarObsModel = (OBS_MODEL_ENUM)lidarObsModel;
    INT32 gnssObsModel;
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("GnssObsModel", &gnssObsModel)) {
        ST_LOG_ERR("Get <PoseEstimator> Cfg Attr <GnssObsModel> Fail.");
        return -1;
    }
    stPoseEstimator.enGnssObsModel = (OBS_MODEL_ENUM)gnssObsModel;
    ST_LOG_INFO("Pose Estimator Fusion Method   %d.", (INT32)stPoseEstimator.enFusionMethod);
    ST_LOG_INFO("Pose Estimator Motion Model    %d.", (INT32)stPoseEstimator.enMotionModel);
    ST_LOG_INFO("Pose Estimator Lidar Obs Model %d.", (INT32)stPoseEstimator.enLidarObsModel);
    ST_LOG_INFO("Pose Estimator Gnss  Obs Model %d.", (INT32)stPoseEstimator.enGnssObsModel);

    return 0;
}

/**************************************************************************************
功能描述: 读取激光雷达定位参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadLidarLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_STRU &stLidarLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("VoxelGridSize", &stLidarLocCfg.voxelGridSize)) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Attr <VoxelGridSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("FitnessScoreThreshold", &stLidarLocCfg.fitnessScoreThreshold)) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Attr <FitnessScoreThreshold> Fail.");
        return -1;
    }

    INT32 matchMethod;
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("MatchType", &matchMethod)) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Attr <MatchType> Fail.");
        return -1;
    }
    stLidarLocCfg.enMatchMethod = (MATCH_METHOD_ENUM)matchMethod;

    TiXmlElement *pobjMatcherCfg = NULL;
    switch (stLidarLocCfg.enMatchMethod)
    {
        case NDT:
            pobjMatcherCfg = pobjCfgParam->FirstChildElement("NDT");
            if (ReadNdtMatcherCfg(pobjMatcherCfg, stLidarLocCfg.stNdtMatcher) != 0){
                ST_LOG_ERR("Ndt Matcher Cfg Param Error!");
                return -1;
            }
            break;
        default:
            ST_LOG_ERR("Unknow Match Method.");
            return -1;
    }

    TiXmlElement * pobjLocLidarListCfg = pobjCfgParam->FirstChildElement("LocLidarList");
    if (0 != ReadLocLidarListCfg(pobjLocLidarListCfg, stLidarLocCfg.vstrLidars)) {
        ST_LOG_ERR("Read Loc Lidar List Cfg Fail!");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取Ndt匹配算法参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadNdtMatcherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_NDT_MATCHER_STRU &stNdtMatcherCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <NDT> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("MaxIter", &stNdtMatcherCfg.maxIter)) {
        ST_LOG_ERR("Get <NdtMatcher> Cfg Attr <MaxIter> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("StepSize", &stNdtMatcherCfg.stepSize)) {
        ST_LOG_ERR("Get <NdtMatcher> Cfg Attr <StepSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("Resolution", &stNdtMatcherCfg.resolution)) {
        ST_LOG_ERR("Get <NdtMatcher> Cfg Attr <Resolution> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("TransEps", &stNdtMatcherCfg.transEps)) {
        ST_LOG_ERR("Get <NdtMatcher> Cfg Attr <TransEps> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLocLidarListCfg(TiXmlElement *pobjCfgParam, vector<string> &vstrLocLidars)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LocLidarList> Cfg Element Fail.");
        return -1;
    }

    vstrLocLidars.clear();
    TiXmlElement *pobjLidar = pobjCfgParam->FirstChildElement();
    for (; pobjLidar != nullptr; pobjLidar = pobjLidar->NextSiblingElement())
    {
        string strLidarName;
        if (TIXML_SUCCESS != pobjLidar->QueryStringAttribute("Name", &strLidarName)) {
            ST_LOG_ERR("Get <Lidar> Cfg Attr <Name> Fail.");
            return -1;
        }
        vstrLocLidars.push_back(strLidarName);
        ST_LOG_INFO("<%s> Is Loc Lidar.", strLidarName.c_str());
    }

    return 0;
}


/**************************************************************************************
功能描述: 读取重定位参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadReLocalizationCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RELOC_STRU &stReLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Relocalization> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("VoxelGridSize", &stReLocCfg.voxelGridSize)) {
        ST_LOG_ERR("Get <Relocalization> Cfg Attr <VoxelGridSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("FitnessScoreThreshold", &stReLocCfg.fitnessScoreThreshold)) {
        ST_LOG_ERR("Get <Relocalization> Cfg Attr <FitnessScoreThreshold> Fail.");
        return -1;
    }

    TiXmlElement *pobjRawRelocCfg = pobjCfgParam->FirstChildElement("RawReloc");
    if (ReadRawReLocalizationCfg(pobjRawRelocCfg, stReLocCfg.stRawReloc) != 0){
        ST_LOG_ERR("Raw Reloc Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjFineRelocCfg = pobjCfgParam->FirstChildElement("FineReloc");
    if (ReadFineReLocalizationCfg(pobjFineRelocCfg, stReLocCfg.stFineReloc) != 0){
        ST_LOG_ERR("Fine Reloc Cfg Param Error!");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取粗重定位参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadRawReLocalizationCfg(TiXmlElement *pobjCfgParam,  CFG_PARAM_RAW_RELOC_STRU  &stRawReLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <RawReloc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("isSupportStation", &stRawReLocCfg.isSupportStationReloc)) {
        ST_LOG_ERR("Get <RawReloc> Cfg Attr <isSupportStation> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("isSupportRecord", &stRawReLocCfg.isSupportRecordReloc)) {
        ST_LOG_ERR("Get <RawReloc> Cfg Attr <isSupportRecord> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("isSupportGnss", &stRawReLocCfg.isSupportGnssReloc)) {
        ST_LOG_ERR("Get <RawReloc> Cfg Attr <isSupportGnss> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("isSupportImuAxisZ", &stRawReLocCfg.isSupportImuAxisZReloc)) {
        ST_LOG_ERR("Get <RawReloc> Cfg Attr <isSupportImuAxisZ> Fail.");
        return -1;
    }


    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("isSupportScanContext", &stRawReLocCfg.isSupportScanContextReloc)) {
        ST_LOG_ERR("Get <RawReloc> Cfg Attr <isSupportScanContext> Fail.");
        return -1;
    }

    if (stRawReLocCfg.isSupportScanContextReloc) {
        TiXmlElement *pobjScanContextCfg = pobjCfgParam->FirstChildElement("ScanContext");
        if (ReadScanContextCfg(pobjScanContextCfg, stRawReLocCfg.stScanContext) != 0){
            ST_LOG_ERR("RawReloc Scan Context Cfg Param Error!");
            return -1;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取精重定位参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadFineReLocalizationCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FINE_RELOC_STRU &stFineReLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <FineReloc> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjNdtMatcherCfg = pobjCfgParam->FirstChildElement("NdtMatcher");
    if (ReadNdtMatcherCfg(pobjNdtMatcherCfg, stFineReLocCfg.stNdtMatcher) != 0){
        ST_LOG_ERR("Reloc Ndt Matcher Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjSearchRegionCfg = pobjCfgParam->FirstChildElement("SearchRegion");
    if (ReadSearchRegionCfg(pobjSearchRegionCfg, stFineReLocCfg.stSearchRegion) != 0) {
        ST_LOG_ERR("Search Region Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjSearchStepCfg = pobjCfgParam->FirstChildElement("SearchStep");
    if (ReadSearchStepCfg(pobjSearchStepCfg, stFineReLocCfg.stSearchStep) != 0) {
        ST_LOG_ERR("Search Step Cfg Param Error!");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取ScanContext参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadScanContextCfg(TiXmlElement *pobjCfgParam,  CFG_PARAM_SCAN_CONTEXT_STRU &stScanContextCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <ScanContext> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MaxRadius", &stScanContextCfg.maxRadius)) {
        ST_LOG_ERR("Get <ScanContext> Cfg Attr <MaxRadius> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("SearchRatio", &stScanContextCfg.searchRatio)) {
        ST_LOG_ERR("Get <ScanContext> Cfg Attr <SearchRatio> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("DistThreshold", &stScanContextCfg.scDistThreshold)) {
        ST_LOG_ERR("Get <ScanContext> Cfg Attr <DistThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("NumCandidate", &stScanContextCfg.numCandidate)) {
        ST_LOG_ERR("Get <ScanContext> Cfg Attr <NumCandidate> Fail.");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取重定位搜索区域参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadSearchRegionCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SEARCH_REG_STRU &stSearchRegCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MinX", &stSearchRegCfg.minX)) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Attr <MinX> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MaxX", &stSearchRegCfg.maxX)) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Attr <MaxX> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MinY", &stSearchRegCfg.minY)) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Attr <MinY> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MaxY", &stSearchRegCfg.maxY)) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Attr <MaxY> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MinYaw", &stSearchRegCfg.minYaw)) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Attr <MinYaw> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MaxYaw", &stSearchRegCfg.maxYaw)) {
        ST_LOG_ERR("Get <SearchRegion> Cfg Attr <MaxYaw> Fail.");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取重定位搜索步长参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_MNL_C::ReadSearchStepCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SEARCH_STEP_STRU &stSearchStepCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <SearchStep> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("StepX", &stSearchStepCfg.stepX)) {
        ST_LOG_ERR("Get <SearchStep> Cfg Attr <StepX> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("StepY", &stSearchStepCfg.stepY)) {
        ST_LOG_ERR("Get <SearchStep> Cfg Attr <StepY> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("StepYaw", &stSearchStepCfg.stepYaw)) {
        ST_LOG_ERR("Get <SearchStep> Cfg Attr <StepYaw> Fail.");
        return -1;
    }
    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadVehicleLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_LOC_STRU &stVehicleLocCfg)
{
    TiXmlElement *pobjVehicleLocCfg = pobjCfgParam->FirstChildElement("VehicleLoc");
    if (pobjVehicleLocCfg == NULL) {
        ST_LOG_ERR("Get <VehicleLoc> Cfg Element Fail.");
        return -1;
    }

    stVehicleLocCfg.enable = false;
    if (TIXML_SUCCESS != pobjVehicleLocCfg->QueryBoolAttribute("enable", &stVehicleLocCfg.enable)) {
        ST_LOG_ERR("Get <VehicleLoc> Cfg Attr <enable> Fail.");
        return -1;
    }

    if (!stVehicleLocCfg.enable) {
        ST_LOG_INFO("New Loc Sys Is Disable.");
        return 0;
    }
    ST_LOG_INFO("New Loc Sys Is Enable.");

    TiXmlElement *pobjRelocCfg = pobjVehicleLocCfg->FirstChildElement("Reloc");
    if (0 != ReadRelocCfg(pobjRelocCfg, stVehicleLocCfg.stReloc)){
        ST_LOG_ERR("Reloc Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjCombLocCfg = pobjVehicleLocCfg->FirstChildElement("CombLoc");
    if (0 != ReadCombLocCfg(pobjCombLocCfg, stVehicleLocCfg.stCombLoc)){
        ST_LOG_ERR("Comb Loc Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadRelocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RELOC_V3_STRU &stRelocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Reloc> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjCoarseRelocCfg = pobjCfgParam->FirstChildElement("CoarseReloc");
    if (0 != ReadCoarseRelocCfg(pobjCoarseRelocCfg, stRelocCfg.stCoarseReloc)){
        ST_LOG_ERR("Coarse Reloc Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjFineRelocCfg = pobjCfgParam->FirstChildElement("FineReloc");
    if (0 != ReadFineRelocCfg(pobjFineRelocCfg, stRelocCfg.stFineReloc)){
        ST_LOG_ERR("Fine Reloc Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadCoarseRelocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_COARSE_RELOC_STRU &stCoarseRelocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <CoarseReloc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("enableRelocStation", &stCoarseRelocCfg.enableRelocStation)) {
        ST_LOG_ERR("Get <CoarseReloc> Cfg Attr <enableRelocStation> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("enableRecord", &stCoarseRelocCfg.enableRecord)) {
        ST_LOG_ERR("Get <CoarseReloc> Cfg Attr <enableRecord> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("enableGnss", &stCoarseRelocCfg.enableGnss)) {
        ST_LOG_ERR("Get <CoarseReloc> Cfg Attr <enableGnss> Fail.");
        return -1;
    }

    TiXmlElement *pobjLidarTopicListCfg = pobjCfgParam->FirstChildElement("LidarTopicList");
    if (0 != ReadLidarTopicListCfg(pobjLidarTopicListCfg, stCoarseRelocCfg.vstrLidarTopicList)){
        ST_LOG_ERR("LidarTopicList Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadFineRelocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FINE_RELOC_V3_STRU &stFineRelocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <FineReloc> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjSearchStepCfg = pobjCfgParam->FirstChildElement("SearchStep");
    if (0 != ReadSearchStepCfg(pobjSearchStepCfg, stFineRelocCfg.stSearchStep)){
        ST_LOG_ERR("SearchStep Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjVoxelMatchCfg = pobjCfgParam->FirstChildElement("VoxelMatch");
    if (0 != ReadVoxelMatchCfg(pobjVoxelMatchCfg, stFineRelocCfg.stVoxelMatch)){
        ST_LOG_ERR("VoxelMatch Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjRegistererCfg = pobjCfgParam->FirstChildElement("Registerer");
    if (0 != ReadRegistererCfg(pobjRegistererCfg, stFineRelocCfg.stRegisterer)){
        ST_LOG_ERR("Registerer Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadVoxelMatchCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VOXEL_MATCH_STRU &stVoxelMatchCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <VoxelMatch> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudRes", &stVoxelMatchCfg.srcCloudRes)) {
        ST_LOG_ERR("Get <VoxelMatch> Cfg Attr <srcCloudRes> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("voxelMapRes", &stVoxelMatchCfg.voxelMapRes)) {
        ST_LOG_ERR("Get <VoxelMatch> Cfg Attr <voxelMapRes> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("minScore", &stVoxelMatchCfg.minScore)) {
        ST_LOG_ERR("Get <VoxelMatch> Cfg Attr <minScore> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("maxSearchPointNum", &stVoxelMatchCfg.maxSearchPointNum)) {
        ST_LOG_ERR("Get <VoxelMatch> Cfg Attr <maxSearchPointNum> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadRegistererCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_REGISTERER_STRU &stRegistererCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Registerer> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stRegistererCfg.type)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <type> Fail.");
        return -1;
    }

    
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudRes", &stRegistererCfg.srcCloudRes)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <srcCloudRes> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("minScore", &stRegistererCfg.minScore)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <minScore> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("matchThrK", &stRegistererCfg.matchThrK)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <matchThrK> Fail.");
        stRegistererCfg.matchThrK = 0.0;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("matchThrB", &stRegistererCfg.matchThrB)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <matchThrB> Fail.");
        stRegistererCfg.matchThrB = 0.315;
    }

    if (stRegistererCfg.type == 0) 
    {
        TiXmlElement *pobjOmpNdtCfg = pobjCfgParam->FirstChildElement("OmpNdt");
        if (0 != ReadOmpNdtCfg(pobjOmpNdtCfg, stRegistererCfg.stOmpNdt)){
            ST_LOG_ERR("OmpNdt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stRegistererCfg.type == 1) 
    {
        TiXmlElement *pobjNdtCfg = pobjCfgParam->FirstChildElement("Ndt");
        if (0 != ReadNdtCfg(pobjNdtCfg, stRegistererCfg.stNdt)){
            ST_LOG_ERR("Ndt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stRegistererCfg.type == 3) 
    {
        TiXmlElement *pobjMixNdtCfg = pobjCfgParam->FirstChildElement("MixNdt");
        if (0 != ReadMixNdtCfg(pobjMixNdtCfg, stRegistererCfg.stMixNdt)){
            ST_LOG_ERR("MixNdt Cfg Param Error!");
            return -1;
        }
    }
    else if (stRegistererCfg.type == 4) 
    {
        TiXmlElement *pobjSmallGicpCfg = pobjCfgParam->FirstChildElement("SmallGicp");
        if (0 != ReadSmallGicpCfg(pobjSmallGicpCfg, stRegistererCfg.stSmallGicp)){
            ST_LOG_ERR("SmallGicp Cfg Param Error!");
            return -1;
        }
    }  
    else 
    {
        ST_LOG_ERR("Registerer Type %d Is Unknow.", stRegistererCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadOmpNdtCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_OMPNDT_STRU &stOmpNdtCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <OmpNdt> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("maxIter", &stOmpNdtCfg.maxIter)) {
        ST_LOG_ERR("Get <OmpNdt> Cfg Attr <maxIter> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("stepSize", &stOmpNdtCfg.stepSize)) {
        ST_LOG_ERR("Get <OmpNdt> Cfg Attr <stepSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("transEps", &stOmpNdtCfg.transEps)) {
        ST_LOG_ERR("Get <OmpNdt> Cfg Attr <transEps> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("resolution", &stOmpNdtCfg.resolution)) {
        ST_LOG_ERR("Get <OmpNdt> Cfg Attr <resolution> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("threadNum", &stOmpNdtCfg.threadNum)) {
        ST_LOG_ERR("Get <OmpNdt> Cfg Attr <threadNum> Fail.");
        return -1;
    }
    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadSmallGicpCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SMALL_GICP_STRU &stSmallGicpCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("voxelRes", &stSmallGicpCfg.voxelRes)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <voxelRes> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("transEps", &stSmallGicpCfg.transEps)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <transEps> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("maxIter", &stSmallGicpCfg.maxIter)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <maxIter> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("threadNum", &stSmallGicpCfg.threadNum)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <threadNum> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("maxCorrespondDist", &stSmallGicpCfg.maxCorrespondDist)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <maxCorrespondDist> Fail.");
        stSmallGicpCfg.maxCorrespondDist = 1.0;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("correspondenceRand", &stSmallGicpCfg.correspondenceRand)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <correspondenceRand> Fail.");
        stSmallGicpCfg.correspondenceRand = 16;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("useVGICP", &stSmallGicpCfg.useVGICP)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <useVGICP> Fail.");
        stSmallGicpCfg.useVGICP = false;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("verbose", &stSmallGicpCfg.verbose)) {
        ST_LOG_ERR("Get <SmallGicp> Cfg Attr <verbose> Fail.");
        stSmallGicpCfg.verbose = false;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadNdtCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_NDT_STRU &stNdtCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Ndt> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("maxIter", &stNdtCfg.maxIter)) {
        ST_LOG_ERR("Get <Ndt> Cfg Attr <maxIter> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("stepSize", &stNdtCfg.stepSize)) {
        ST_LOG_ERR("Get <Ndt> Cfg Attr <stepSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("transEps", &stNdtCfg.transEps)) {
        ST_LOG_ERR("Get <Ndt> Cfg Attr <transEps> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("resolution", &stNdtCfg.resolution)) {
        ST_LOG_ERR("Get <Ndt> Cfg Attr <resolution> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadMixNdtCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_MIXNDT_STRU &stMixNdtCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <MixNdt> Cfg Element Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("maxIter", &stMixNdtCfg.maxIter)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <maxIter> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("stepSize", &stMixNdtCfg.stepSize)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <stepSize> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("transEps", &stMixNdtCfg.transEps)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <transEps> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("rawResolution", &stMixNdtCfg.rawResolution)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <rawResolution> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("fineResolution", &stMixNdtCfg.fineResolution)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <fineResolution> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("threadNum", &stMixNdtCfg.threadNum)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <threadNum> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("outlierThr", &stMixNdtCfg.outlierThr)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <outlierThr> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudDsRes", &stMixNdtCfg.srcCloudDsRes)) {
        ST_LOG_ERR("Get <MixNdt> Cfg Attr <srcCloudDsRes> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadCombLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_COMB_LOC_STRU &stCombLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <CombLoc> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjGnssLocCfg = pobjCfgParam->FirstChildElement("GnssLoc");
    if (0 != ReadGnssLocCfg(pobjGnssLocCfg, stCombLocCfg.stGnssLoc)){
        ST_LOG_ERR("Gnss Loc Cfg Param Error!");
        return -1;
    }

    #if 1
    TiXmlElement *pobjLidarLocCfg = pobjCfgParam->FirstChildElement("LidarLoc");
    if (0 != ReadLidarLocV4Cfg(pobjLidarLocCfg, stCombLocCfg.stLidarLoc)){
        ST_LOG_ERR("Lidar Loc Cfg Param Error!");
        return -1;
    }
    #else
    TiXmlElement *pobjLidarLocCfg = pobjCfgParam->FirstChildElement("LidarLoc");
    if (0 != ReadLidarLocV3Cfg(pobjLidarLocCfg, stCombLocCfg.stLidarLoc)){
        ST_LOG_ERR("Lidar Loc Cfg Param Error!");
        return -1;
    }
    #endif

    

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadGnssLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_LOC_STRU &stGnssLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <GnssLoc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stGnssLocCfg.type)) {
        ST_LOG_ERR("Get <GnssLoc> Cfg Attr <type> Fail.");
        return -1;
    }

    if (stGnssLocCfg.type == 0) {
        TiXmlElement *pobjGnssLocGeneralCfg = pobjCfgParam->FirstChildElement("General");
        if (0 != ReadGnssLocGeneralCfg(pobjGnssLocGeneralCfg, stGnssLocCfg.stGnssLocGeneral)){
            ST_LOG_ERR("Gnss Loc General Cfg Param Error!");
           return -1;
        }
    } else if (stGnssLocCfg.type == 1) {
        /*
        TiXmlElement *pobjGnssLocCustomizedCfg = pobjCfgParam->FirstChildElement("Customized");
        if (0 != ReadGnssLocCustomizedCfg(pobjGnssLocCustomizedCfg, stGnssLocCfg.stGnssLocCustomized)){
            ST_LOG_ERR("Gnss Loc Customized Cfg Param Error!");
           return -1;
        }
        */
    } else {
        ST_LOG_ERR("Gnss Loc Type %d Is Unknow.", stGnssLocCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadGnssLocGeneralCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_LOC_GENERAL_STRU &stGnssLocGeneralCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <General> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute("isDualAntenna", &stGnssLocGeneralCfg.isDualAntenna)) {
        ST_LOG_ERR("Get <General> Cfg Attr <isDualAntenna> Fail.");
        return -1;
    }

    TiXmlElement *pobjEskfCfg = pobjCfgParam->FirstChildElement("Eskf");
    if (0 != ReadEskfCfg(pobjEskfCfg, stGnssLocGeneralCfg.stEskf)){
        ST_LOG_ERR("Eskf Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadEskfCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ESKF_STRU &stEskfCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Eskf> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("ng", &stEskfCfg.ng)) {
        ST_LOG_ERR("Get <Eskf> Cfg Attr <ng> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("na", &stEskfCfg.na)) {
        ST_LOG_ERR("Get <Eskf> Cfg Attr <na> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("nbg", &stEskfCfg.nbg)) {
        ST_LOG_ERR("Get <Eskf> Cfg Attr <nbg> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("nba", &stEskfCfg.nba)) {
        ST_LOG_ERR("Get <Eskf> Cfg Attr <nba> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadGnssLocCustomizedCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_LOC_CUSTOMIZED_STRU &stGnssLocCustomizedCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Customized> Cfg Element Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLidarLocV3Cfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_V3_STRU &stLidarLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stLidarLocCfg.type)) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Attr <type> Fail.");
        return -1;
    }

    if (stLidarLocCfg.type == 0) {
        TiXmlElement *pobjLidarLocLioCfg = pobjCfgParam->FirstChildElement("LidarLocLio");
        if (0 != ReadLidarLocLioCfg(pobjLidarLocLioCfg, stLidarLocCfg.stLidarLocLio)){
            ST_LOG_ERR("Lidar Loc Lio Cfg Param Error!");
            return -1;
        }
    } else if (stLidarLocCfg.type == 1) {
        TiXmlElement *pobjLidarLocWioCfg = pobjCfgParam->FirstChildElement("LidarLocWio");
        if (0 != ReadLidarLocWioCfg(pobjLidarLocWioCfg, stLidarLocCfg.stLidarLocWio)){
            ST_LOG_ERR("Lidar Loc Wio Cfg Param Error!");
            return -1;
        }
    } else {
        ST_LOG_ERR("Lidar Loc Type %d Is Unknow.", stLidarLocCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLidarLocV4Cfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_V3_STRU &stLidarLocCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stLidarLocCfg.type)) {
        ST_LOG_ERR("Get <LidarLoc> Cfg Attr <type> Fail.");
        return -1;
    }

    if (stLidarLocCfg.type == 0) {
        TiXmlElement *pobjLidarLocLioCfg = pobjCfgParam->FirstChildElement("LidarLocLio");
        if (0 != ReadLidarLocLioCfg(pobjLidarLocLioCfg, stLidarLocCfg.stLidarLocLio)){
            ST_LOG_ERR("Lidar Loc Lio Cfg Param Error!");
            return -1;
        }
    } else if (stLidarLocCfg.type == 1) {
        TiXmlElement *pobjLidarLocWioCfg = pobjCfgParam->FirstChildElement("LidarLocWio");
        if (0 != ReadLidarLocWioCfgV1(pobjLidarLocWioCfg, stLidarLocCfg.stLidarLocWio)){
            ST_LOG_ERR("Lidar Loc Wio Cfg Param Error!");
            return -1;
        }
    } else {
        ST_LOG_ERR("Lidar Loc Type %d Is Unknow.", stLidarLocCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLidarLocLioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_LIO_STRU &stLidarLocLioCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarLocLio> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("correctPeriod", &stLidarLocLioCfg.correctPeriod)) {
        ST_LOG_ERR("Get <LidarLocLio> Cfg Attr <correctPeriod> Fail.");
        return -1;
    }

    TiXmlElement *pobjLioCfg = pobjCfgParam->FirstChildElement("Lio");
    if (0 != ReadLioCfg(pobjLioCfg, stLidarLocLioCfg.stLio)){
        ST_LOG_ERR("Lio Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjRegistererCfg = pobjCfgParam->FirstChildElement("Registerer");
    if (0 != ReadScanMatcherCfg(pobjRegistererCfg, stLidarLocLioCfg.stScanMatcher)){
        ST_LOG_ERR("Registerer Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjSmootherCfg = pobjCfgParam->FirstChildElement("Smoother");
    if (0 != ReadSmootherCfg(pobjSmootherCfg, stLidarLocLioCfg.stSmoother)){
        ST_LOG_ERR("Smoother Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIO_STRU &stLioCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Lio> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjLidarTopicListCfg = pobjCfgParam->FirstChildElement("LidarTopicList");
    if (0 != ReadLidarTopicListCfg(pobjLidarTopicListCfg, stLioCfg.vstrLidarTopicList)){
        ST_LOG_ERR("LidarTopicList Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLidarTopicListCfg(TiXmlElement *pobjCfgParam, std::vector<std::string> &vstrLidarTopicList)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarTopicList> Cfg Element Fail.");
        return -1;
    }

    vstrLidarTopicList.clear();
    TiXmlElement *pobjLidarTopic = pobjCfgParam->FirstChildElement();
    for (; pobjLidarTopic != nullptr; pobjLidarTopic = pobjLidarTopic->NextSiblingElement())
    {
        string strLidarTopicName;
        if (TIXML_SUCCESS != pobjLidarTopic->QueryStringAttribute("name", &strLidarTopicName)) {
            ST_LOG_ERR("Get <LidarTopic> Cfg Attr <name> Fail.");
            return -1;
        }
        vstrLidarTopicList.push_back(strLidarTopicName);
    }

    return 0;
}

//lio的
INT32 CFG_PARAM_READ_MNL_C::ReadSmootherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SMOOTHER_STRU &stSmootherCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Smoother> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("windowSize", &stSmootherCfg.windowSize)) {
        ST_LOG_ERR("Get <Smoother> Cfg Attr <windowSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("weight", &stSmootherCfg.weight)) {
        ST_LOG_ERR("Get <Smoother> Cfg Attr <weight> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("DiffDistance", &stSmootherCfg.diffDistance)) {
        ST_LOG_WARN("Get <Smoother> Cfg Attr <DiffDistance> Fail. Use default 0.65.");
        stSmootherCfg.diffDistance = 0.65;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLidarLocWioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_WIO_STRU &stLidarLocWioCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarLocWio> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("correctPeriod", &stLidarLocWioCfg.correctPeriod)) {
        ST_LOG_ERR("Get <LidarLocWio> Cfg Attr <correctPeriod> Fail.");
        return -1;
    }
    TiXmlElement *pobjWioCfg = pobjCfgParam->FirstChildElement("Wio");
    if (0 != ReadWioCfg(pobjWioCfg, stLidarLocWioCfg.stWio)){
        ST_LOG_ERR("Wio Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjRegistererCfg = pobjCfgParam->FirstChildElement("Registerer");
    if (0 != ReadScanMatcherCfg(pobjRegistererCfg, stLidarLocWioCfg.stScanMatcher)){
        ST_LOG_ERR("Registerer Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjSmootherCfg = pobjCfgParam->FirstChildElement("Smoother");
    if (0 != ReadSmootherCfg(pobjSmootherCfg, stLidarLocWioCfg.stSmoother)){
        ST_LOG_ERR("Smoother Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjLidarTopicListCfg = pobjCfgParam->FirstChildElement("LidarTopicList");
    if (0 != ReadLidarTopicListCfg(pobjLidarTopicListCfg, stLidarLocWioCfg.vstrLidarTopicList)){
        ST_LOG_ERR("LidarTopicList Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLidarLocWioCfgV1(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_WIO_STRU &stLidarLocWioCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LidarLocWio> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("correctPeriod", &stLidarLocWioCfg.correctPeriod)) {
        ST_LOG_ERR("Get <LidarLocWio> Cfg Attr <correctPeriod> Fail.");
        return -1;
    }

    TiXmlElement *pobjDataProcessCfg = pobjCfgParam->FirstChildElement("DataProcess");
    if (0 != ReadDataProcessCfg(pobjDataProcessCfg, stLidarLocWioCfg.stDataProcess)){
        ST_LOG_ERR("DataProcess Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjWioCfg = pobjCfgParam->FirstChildElement("Wio");
    if (0 != ReadWioCfg(pobjWioCfg, stLidarLocWioCfg.stWio)){
        ST_LOG_ERR("Wio Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjRegistererCfg = pobjCfgParam->FirstChildElement("Registerer");
    if (0 != ReadScanMatcherCfgV1(pobjRegistererCfg, stLidarLocWioCfg.stScanMatcher)){
        ST_LOG_ERR("Registerer Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjSmootherCfg = pobjCfgParam->FirstChildElement("Smoother");
    if (0 != ReadSmootherCfg(pobjSmootherCfg, stLidarLocWioCfg.stSmoother)){
        ST_LOG_ERR("Smoother Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjLidarTopicListCfg = pobjCfgParam->FirstChildElement("LidarTopicList");
    if (0 != ReadLidarTopicListCfg(pobjLidarTopicListCfg, stLidarLocWioCfg.vstrLidarTopicList)){
        ST_LOG_ERR("LidarTopicList Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadDataProcessCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_DATA_PROCESS_STRU &stDataProcCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <DataProcess> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("InputCloudMinRange", &stDataProcCfg.inputCloudMinRange)) {
        ST_LOG_ERR("Get <DataProcess> Cfg Attr <inputCloudMinRange> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("InputCloudMaxRange", &stDataProcCfg.inputCloudMaxRange)) {
        ST_LOG_ERR("Get <DataProcess> Cfg Attr <inputCloudMaxRange> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("InputCloudJumpNum", &stDataProcCfg.inputCloudJumpNum)) {
        ST_LOG_ERR("Get <DataProcess> Cfg Attr <inputCloudJumpNum> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadWioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_WIO_STRU &stWioCfg)
{

    return 0;
}



INT32 CFG_PARAM_READ_MNL_C::ReadScanMatcherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SCAN_MATCHER_STRU &stScanMatcherCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Registerer> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudMinRange", &stScanMatcherCfg.srcCloudMinRange)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <srcCloudMinRange> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudMaxRange", &stScanMatcherCfg.srcCloudMaxRange)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <srcCloudMaxRange> Fail.");
        return -1;
    }

    TiXmlElement *pobjActMapCfg = pobjCfgParam->FirstChildElement("ActiveMap");
    if (pobjActMapCfg == NULL) {
        ST_LOG_WARN("Get <ActiveMap> Cfg Element Fail. Use default disable.");
        stScanMatcherCfg.stActMap.enable = false;
    } else {
        if (TIXML_SUCCESS != pobjActMapCfg->QueryBoolAttribute("enable", &stScanMatcherCfg.stActMap.enable)) {
            ST_LOG_ERR("Get <ActiveMap> Cfg Attr <enable> Fail.");
            return -1;
        }

        if (stScanMatcherCfg.stActMap.enable) {
            ST_LOG_INFO("Active Map Is Enable.");
            if (0 != ReadActiveMapCfg(pobjActMapCfg, stScanMatcherCfg.stActMap)) {
                ST_LOG_ERR("ActiveMap Cfg Error.");
                return -1;
            }
        } else {
            ST_LOG_INFO("Active Map Disable.");
        }
    }

    TiXmlElement *pobjDsfCfg = pobjCfgParam->FirstChildElement("DownSampleFilter");
    if (0 != ReadDsFilterCfg(pobjDsfCfg, stScanMatcherCfg.stDsf)) {
        ST_LOG_ERR("DownSampleFilter Cfg Error.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stScanMatcherCfg.type)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <type> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("minScore", &stScanMatcherCfg.minScore)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <minScore> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("matchThrK", &stScanMatcherCfg.matchThrK)) {
        ST_LOG_WARN("Get <Registerer> Cfg Attr <matchThrK> Fail.");
        stScanMatcherCfg.matchThrK = 0.0;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("matchThrB", &stScanMatcherCfg.matchThrB)) {
        ST_LOG_WARN("Get <Registerer> Cfg Attr <matchThrB> Fail.");
        stScanMatcherCfg.matchThrB = 0.315;
    }

    TiXmlElement *pobjMapleafsizeCfg = pobjCfgParam->FirstChildElement("Mapleafsize");
    if (pobjMapleafsizeCfg != NULL) {
        if (TIXML_SUCCESS != pobjMapleafsizeCfg->QueryFloatAttribute("leafsize", &stScanMatcherCfg.mapLeafSize)) {
            ST_LOG_WARN("Get <Mapleafsize> Cfg Attr <leafsize> Fail. Use default 0.5.");
            stScanMatcherCfg.mapLeafSize = 0.5;
        }
    } else {
        ST_LOG_WARN("Get <Mapleafsize> Cfg Element Fail. Use default 0.5.");
        stScanMatcherCfg.mapLeafSize = 0.5;
    }

    TiXmlElement *pobjDiffDistanceCfg = pobjCfgParam->FirstChildElement("DiffDistance");
    if (pobjDiffDistanceCfg != NULL) {
        if (TIXML_SUCCESS != pobjDiffDistanceCfg->QueryFloatAttribute("distanceThr", &stScanMatcherCfg.diffDistanceThr)) {
            ST_LOG_WARN("Get <DiffDistance> Cfg Attr <distanceThr> Fail. Use default 0.65.");
            stScanMatcherCfg.diffDistanceThr = 0.65;
        }
    } else {
        ST_LOG_WARN("Get <DiffDistance> Cfg Element Fail. Use default 0.65.");
        stScanMatcherCfg.diffDistanceThr = 0.65;
    }

    if (stScanMatcherCfg.type == 0) 
    {
        TiXmlElement *pobjOmpNdtCfg = pobjCfgParam->FirstChildElement("OmpNdt");
        if (0 != ReadOmpNdtCfg(pobjOmpNdtCfg, stScanMatcherCfg.stOmpNdt)){
            ST_LOG_ERR("OmpNdt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stScanMatcherCfg.type == 1) 
    {
        TiXmlElement *pobjNdtCfg = pobjCfgParam->FirstChildElement("Ndt");
        if (0 != ReadNdtCfg(pobjNdtCfg, stScanMatcherCfg.stNdt)){
            ST_LOG_ERR("Ndt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stScanMatcherCfg.type == 3) 
    {
        TiXmlElement *pobjMixNdtCfg = pobjCfgParam->FirstChildElement("MixNdt");
        if (0 != ReadMixNdtCfg(pobjMixNdtCfg, stScanMatcherCfg.stMixNdt)){
            ST_LOG_ERR("MixNdt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stScanMatcherCfg.type == 4) 
    {
        TiXmlElement *pobjSmallGicpCfg = pobjCfgParam->FirstChildElement("SmallGicp");
        if (0 != ReadSmallGicpCfg(pobjSmallGicpCfg, stScanMatcherCfg.stSmallGicp)){
            ST_LOG_ERR("SmallGicp Cfg Param Error!");
            return -1;
        }
    } 
    else 
    {
        ST_LOG_ERR("Registerer Type %d Is Unknow.", stScanMatcherCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadScanMatcherCfgV1(TiXmlElement *pobjCfgParam, CFG_PARAM_SCAN_MATCHER_STRU &stScanMatcherCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Registerer> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudMinRange", &stScanMatcherCfg.srcCloudMinRange)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <srcCloudMinRange> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("srcCloudMaxRange", &stScanMatcherCfg.srcCloudMaxRange)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <srcCloudMaxRange> Fail.");
        return -1;
    }

    TiXmlElement *pobjActMapCfg = pobjCfgParam->FirstChildElement("ActiveMap");
    if (pobjActMapCfg == NULL) {
        ST_LOG_WARN("Get <ActiveMap> Cfg Element Fail. Use default disable.");
        stScanMatcherCfg.stActMap.enable = false;
    } else {
        if (TIXML_SUCCESS != pobjActMapCfg->QueryBoolAttribute("enable", &stScanMatcherCfg.stActMap.enable)) {
            ST_LOG_ERR("Get <ActiveMap> Cfg Attr <enable> Fail.");
            return -1;
        }
        if (!stScanMatcherCfg.stActMap.enable) {
            ST_LOG_INFO("Active Map Disable.");
        } else {
            ST_LOG_INFO("Active Map Is Enable.");
            if (0 != ReadActiveMapCfg(pobjActMapCfg, stScanMatcherCfg.stActMap)) {
                ST_LOG_ERR("ActiveMap Cfg Error.");
                return -1;
            }
        }
    }

    TiXmlElement *pobjDsfCfg = pobjCfgParam->FirstChildElement("DownSampleFilter");
    if (0 != ReadDsFilterCfg(pobjDsfCfg, stScanMatcherCfg.stDsf)) {
        ST_LOG_ERR("DownSampleFilter Cfg Error.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stScanMatcherCfg.type)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <type> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("minScore", &stScanMatcherCfg.minScore)) {
        ST_LOG_ERR("Get <Registerer> Cfg Attr <minScore> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("matchThrK", &stScanMatcherCfg.matchThrK)) {
        ST_LOG_WARN("Get <Registerer> Cfg Attr <matchThrK> Fail.");
        stScanMatcherCfg.matchThrK = 0.0;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("matchThrB", &stScanMatcherCfg.matchThrB)) {
        ST_LOG_WARN("Get <Registerer> Cfg Attr <matchThrB> Fail.");
        stScanMatcherCfg.matchThrB = 0.315;
    }

    TiXmlElement *pobjMapleafsizeCfg = pobjCfgParam->FirstChildElement("Mapleafsize");
    if (pobjMapleafsizeCfg != NULL) {
        if (TIXML_SUCCESS != pobjMapleafsizeCfg->QueryFloatAttribute("leafsize", &stScanMatcherCfg.mapLeafSize)) {
            ST_LOG_WARN("Get <Mapleafsize> Cfg Attr <leafsize> Fail. Use default 0.5.");
            stScanMatcherCfg.mapLeafSize = 0.5;
        }
    } else {
        ST_LOG_WARN("Get <Mapleafsize> Cfg Element Fail. Use default 0.5.");
        stScanMatcherCfg.mapLeafSize = 0.5;
    }

    TiXmlElement *pobjDiffDistanceCfg = pobjCfgParam->FirstChildElement("DiffDistance");
    if (pobjDiffDistanceCfg != NULL) {
        if (TIXML_SUCCESS != pobjDiffDistanceCfg->QueryFloatAttribute("distanceThr", &stScanMatcherCfg.diffDistanceThr)) {
            ST_LOG_WARN("Get <DiffDistance> Cfg Attr <distanceThr> Fail. Use default 0.65.");
            stScanMatcherCfg.diffDistanceThr = 0.65;
        }
    } else {
        ST_LOG_WARN("Get <DiffDistance> Cfg Element Fail. Use default 0.65.");
        stScanMatcherCfg.diffDistanceThr = 0.65;
    }



    if (stScanMatcherCfg.type == 0) 
    {
        TiXmlElement *pobjOmpNdtCfg = pobjCfgParam->FirstChildElement("OmpNdt");
        if (0 != ReadOmpNdtCfg(pobjOmpNdtCfg, stScanMatcherCfg.stOmpNdt)){
            ST_LOG_ERR("OmpNdt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stScanMatcherCfg.type == 1) 
    {
        TiXmlElement *pobjNdtCfg = pobjCfgParam->FirstChildElement("Ndt");
        if (0 != ReadNdtCfg(pobjNdtCfg, stScanMatcherCfg.stNdt)){
            ST_LOG_ERR("Ndt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stScanMatcherCfg.type == 3) 
    {
        TiXmlElement *pobjMixNdtCfg = pobjCfgParam->FirstChildElement("MixNdt");
        if (0 != ReadMixNdtCfg(pobjMixNdtCfg, stScanMatcherCfg.stMixNdt)){
            ST_LOG_ERR("MixNdt Cfg Param Error!");
            return -1;
        }
    } 
    else if (stScanMatcherCfg.type == 4) 
    {
        TiXmlElement *pobjSmallGicpCfg = pobjCfgParam->FirstChildElement("SmallGicp");
        if (0 != ReadSmallGicpCfg(pobjSmallGicpCfg, stScanMatcherCfg.stSmallGicp)){
            ST_LOG_ERR("SmallGicp Cfg Param Error!");
            return -1;
        }
    }
    else 
    {
        ST_LOG_ERR("Registerer Type %d Is Unknow.", stScanMatcherCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadActiveMapCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ACTIVE_MAP_STRU &stActMapCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <ActiveMap> Cfg Element Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("VoxelMapSize", &stActMapCfg.voxelMapSize)) {
        ST_LOG_ERR("Get <ActiveMap> Cfg Attr <voxelMapSize> Fail.");
        return -1;
    }
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("MaxVoxelSize", &stActMapCfg.maxVoxelSize)) {
        ST_LOG_ERR("Get <ActiveMap> Cfg Attr <MaxVoxelSize> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadDsFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_DOWN_SAMPLE_FILTER_STRU &stDsfCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_WARN("Get <DownSampleFilter> Cfg Element Fail.");
        return 0;
    }   

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("type", &stDsfCfg.type)) {
        ST_LOG_ERR("Get <DownSampleFilter> Cfg Attr <type> Fail.");
        return -1;
    }

    if (stDsfCfg.type == -1) {
        ST_LOG_WARN("No Down Sample."); 
    } else if (stDsfCfg.type == 0) {
        TiXmlElement *pobjVgfCfg = pobjCfgParam->FirstChildElement("VoxelGridFilter");
        if (0 != ReadVoxelGridFilterCfg(pobjVgfCfg, stDsfCfg.stVgf)){
            ST_LOG_ERR("VoxelGridFilter Cfg Param Error!");
            return -1;
        }
    } else if (stDsfCfg.type == 1) {
        TiXmlElement *pobjAvgfCfg = pobjCfgParam->FirstChildElement("AdaptiveVoxelGridFilter");
        if (0 != ReadAdaptiveVoxelGridFilterCfg(pobjAvgfCfg, stDsfCfg.stAvgf)){
            ST_LOG_ERR("AdaptiveVoxelGridFilter Cfg Param Error!");
            return -1;
        }
    } else {
        ST_LOG_ERR("DownSampleFilter Type %d Is Unknow.", stDsfCfg.type);
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadVoxelGridFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VOXEL_GRID_FILTER_STRU &stVgfCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <VoxelGridFilter> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("leafSizeX", &stVgfCfg.leafSizeX)) {
        ST_LOG_ERR("Get <VoxelGridFilter> Cfg Attr <leafSizeX> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("leafSizeY", &stVgfCfg.leafSizeY)) {
        ST_LOG_ERR("Get <VoxelGridFilter> Cfg Attr <leafSizeY> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("leafSizeZ", &stVgfCfg.leafSizeZ)) {
        ST_LOG_ERR("Get <VoxelGridFilter> Cfg Attr <leafSizeZ> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadAdaptiveVoxelGridFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ADAPTIVE_VOXEL_GRID_FILTER_STRU &stAvgfCfg)
{  
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <AdaptiveVoxelGridFilter> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("voxelNum", &stAvgfCfg.voxelNum)) {
        ST_LOG_ERR("Get <AdaptiveVoxelGridFilter> Cfg Attr <voxelNum> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("leafSize", &stAvgfCfg.leafSize)) {
        ST_LOG_ERR("Get <AdaptiveVoxelGridFilter> Cfg Attr <leafSize> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadMappingCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_MAPPING_STRU &stMappingCfg)
{
    TiXmlElement *pobjMappingCfg = pobjCfgParam->FirstChildElement("Mapping");
    if (pobjMappingCfg == NULL) {
        ST_LOG_ERR("Get <Mapping> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjDataProcCfg = pobjMappingCfg->FirstChildElement("DataProc");
    if (ReadDataProcCfg(pobjDataProcCfg, stMappingCfg.stDataProc) != 0){
        ST_LOG_ERR("Data Proc Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjFrontEndCfg = pobjMappingCfg->FirstChildElement("FrontEnd");
    if (ReadFrontEndCfg(pobjFrontEndCfg, stMappingCfg.stFrontEnd) != 0){
        ST_LOG_ERR("Front End Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjBackCfg = pobjMappingCfg->FirstChildElement("BackEnd");
    if (ReadBackEndCfg(pobjBackCfg, stMappingCfg.stBackEnd) != 0){
        ST_LOG_ERR("Back End Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjLoopCloseCfg = pobjMappingCfg->FirstChildElement("LoopClose");
    if (ReadLoopCloseCfg(pobjLoopCloseCfg, stMappingCfg.stLoopClose) != 0) {
        ST_LOG_ERR("Loop Close Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadDataProcCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_DATAPROC_STRU &stDataProcCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <DataProc> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("SatelliteNum",   &stDataProcCfg.satelliteNum)) {
        ST_LOG_ERR("Get <DataProc> Cfg Attr <SatelliteNum> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("ContinueTime", &stDataProcCfg.continueTime)) {
        ST_LOG_ERR("Get <DataProc> Cfg Attr <ContinueTime> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadFrontEndCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FRONTEND_STRU &stFrontEndCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <FrontEnd> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("MapSize", &stFrontEndCfg.mapSize)) {
        ST_LOG_ERR("Get <FrontEnd> Cfg Attr <MapSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MapIncDistThreshold", &stFrontEndCfg.mapIncDistThreshold)) {
        ST_LOG_ERR("Get <FrontEnd> Cfg Attr <MapIncDistThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MapIncAngleThreshold", &stFrontEndCfg.mapIncAngleThreshold)) {
        ST_LOG_ERR("Get <FrontEnd> Cfg Attr <MapIncAngleThreshold> Fail.");
        return -1;
    }

    TiXmlElement *pobjLoFrontEndCfg = pobjCfgParam->FirstChildElement("LOFrontEnd");
    if (ReadLOFrontEndCfg(pobjLoFrontEndCfg, stFrontEndCfg.stLOFrontEnd) != 0) {
        ST_LOG_ERR("LO Front End Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjLioFrontEndCfg = pobjCfgParam->FirstChildElement("LIOFrontEnd");
    if (ReadLIOFrontEndCfg(pobjLioFrontEndCfg, stFrontEndCfg.stLIOFrontEnd) != 0) {
        ST_LOG_ERR("LIO Front End Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadBackEndCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_BACKEND_STRU &stBackEndCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <BackEnd> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjLoFactorCfg = pobjCfgParam->FirstChildElement("LOFactor");
    if (ReadLOFactorCfg(pobjLoFactorCfg, stBackEndCfg.stLoFactor) != 0) {
        ST_LOG_ERR("LO Factor Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjLcFactorCfg = pobjCfgParam->FirstChildElement("LCFactor");
    if (ReadLCFactorCfg(pobjLcFactorCfg, stBackEndCfg.stLcFactor) != 0) {
        ST_LOG_ERR("Lc Factor Cfg Param Error!");
        return -1;
    }

    TiXmlElement *pobjGnssFactorCfg = pobjCfgParam->FirstChildElement("GnssFactor");
    if (ReadGnssFactorCfg(pobjGnssFactorCfg, stBackEndCfg.stGnssFactor) != 0) {
        ST_LOG_ERR("Gnss Factor Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLoopCloseCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LOOPCLOSE_STRU &stLoopCloseCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LoopClose> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("FrameDist", &stLoopCloseCfg.frameDist)) {
        ST_LOG_ERR("Get <LoopClose> Cfg Attr <FrameDist> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("GlobalMatchThreshold", &stLoopCloseCfg.globalMatchThreshold)) {
        ST_LOG_ERR("Get <LoopClose> Cfg Attr <GlobalMatchErr> Fail.");
        return -1;
    }

    TiXmlElement *pobjMatherCfg = pobjCfgParam->FirstChildElement("Mather");
    if (ReadMatcherCfg(pobjMatherCfg, stLoopCloseCfg.stMatcher) != 0) {
        ST_LOG_ERR("Matcher Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLOFrontEndCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LO_FRONTEND_STRU &stLoFrontEndCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LOFrontEnd> Cfg Element Fail.");
        return -1;
    }

    TiXmlElement *pobjMatherCfg = pobjCfgParam->FirstChildElement("Mather");
    if (ReadMatcherCfg(pobjMatherCfg, stLoFrontEndCfg.stMatcher) != 0) {
        ST_LOG_ERR("Matcher Cfg Param Error!");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLIOFrontEndCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIO_FRONTEND_STRU &stLioFrontEndCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LIOFrontEnd> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("LineFilterSize", &stLioFrontEndCfg.lineFilterSize)) {
        ST_LOG_ERR("Get <LIOFrontEnd> Cfg Attr <LineFilterSize> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("PlaneThreshold", &stLioFrontEndCfg.planeThreshold)) {
        ST_LOG_ERR("Get <LIOFrontEnd> Cfg Attr <PlaneThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("LidarNoise", &stLioFrontEndCfg.lidarNoise)) {
        ST_LOG_ERR("Get <LIOFrontEnd> Cfg Attr <LidarNoise> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadMatcherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_MATCH_STRU &stMatherCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <Matcher> Cfg Element Fail.");
        return -1;
    }

    INT32 matchMethod;
    if (TIXML_SUCCESS != pobjCfgParam->QueryIntAttribute("MatchType", &matchMethod)) {
        ST_LOG_ERR("Get <Matcher> Cfg Attr <MatchMethod> Fail.");
        return -1;
    }
    stMatherCfg.enMatchMethod = (MATCH_METHOD_ENUM)matchMethod;

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("MatchThreshold", &stMatherCfg.matchThreshold)) {
        ST_LOG_ERR("Get <Matcher> Cfg Attr <MatchThreshold> Fail.");
        return -1;
    }

    TiXmlElement *pobjMatcherCfg = NULL;
    switch (stMatherCfg.enMatchMethod)
    {
       case NDT:
           pobjMatcherCfg = pobjCfgParam->FirstChildElement("NDT");
           if (ReadNdtMatcherCfg(pobjMatcherCfg, stMatherCfg.stNdtMatcher) != 0){
               ST_LOG_ERR("Ndt Matcher Cfg Param Error!");
               return -1;
           }
           break;
       default:
           ST_LOG_ERR("Unknow Match Method.");
           return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLOFactorCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LO_FACTOR_STRU &stLoFactorCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LOFactor> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("AddDistThreshold", &stLoFactorCfg.addDistThreshold)) {
        ST_LOG_ERR("Get <LOFactor> Cfg Attr <AddDistThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("AddTimeThreshold", &stLoFactorCfg.addTimeThreshold)) {
        ST_LOG_ERR("Get <LOFactor> Cfg Attr <AddTimeThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("NoisePos", &stLoFactorCfg.noisePos)) {
        ST_LOG_ERR("Get <LOFactor> Cfg Attr <NoisePos> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("NoiseAtt", &stLoFactorCfg.noiseAtt)) {
        ST_LOG_ERR("Get <LOFactor> Cfg Attr <NoiseAtt> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadLCFactorCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LC_FACTOR_STRU &stLcFactorCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <LOFactor> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("NoisePos", &stLcFactorCfg.noisePos)) {
        ST_LOG_ERR("Get <LCFactor> Cfg Attr <NoisePos> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("NoiseAtt", &stLcFactorCfg.noiseAtt)) {
        ST_LOG_ERR("Get <LCFactor> Cfg Attr <NoiseAtt> Fail.");
        return -1;
    }

    return 0;
}

INT32 CFG_PARAM_READ_MNL_C::ReadGnssFactorCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_FACTOR_STRU &stGnssFactorCfg)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("Get <GnssFactor> Cfg Element Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("AddDistThreshold", &stGnssFactorCfg.addDistThreshold)) {
        ST_LOG_ERR("Get <GnssFactor> Cfg Attr <AddDistThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("AddTimeThreshold", &stGnssFactorCfg.addTimeThreshold)) {
        ST_LOG_ERR("Get <GnssFactor> Cfg Attr <AddTimeThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("TrajErrThreshold", &stGnssFactorCfg.trajErrThreshold)) {
        ST_LOG_ERR("Get <GnssFactor> Cfg Attr <TrajErrThreshold> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("NoiseXY", &stGnssFactorCfg.noiseXY)) {
        ST_LOG_ERR("Get <GnssFactor> Cfg Attr <NoiseXY> Fail.");
        return -1;
    }

    if (TIXML_SUCCESS != pobjCfgParam->QueryFloatAttribute("NoiseZ", &stGnssFactorCfg.noiseZ)) {
        ST_LOG_ERR("Get <GnssFactor> Cfg Attr <NoiseZ> Fail.");
        return -1;
    }

    return 0;
}


}

}
