
#include "comm/comm_cfg/comm_cfg_param_read_pnp.h"

namespace COMM
{

namespace CFG
{



/**************************************************************************************
功能描述: 读取车体过滤空间信息
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadCarBodyVxlFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CAR_BODY_VXL_FILTER_STRU &stCarBodyVxlFilter)
{
    TiXmlElement *pobjCarBodyVxlFilterCfg = GetFirstChildElement(pobjCfgParam, "CarBodyVxlFilter");
    if (pobjCarBodyVxlFilterCfg == NULL) return -1;

    TiXmlElement *pobjCarBodyFilterSizeCfg = GetFirstChildElement(pobjCarBodyVxlFilterCfg, "FilterSize");
    if (pobjCarBodyFilterSizeCfg == NULL) return -1;
    int rslt = 0;
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "VoxelSize", stCarBodyVxlFilter.voxelSize);
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "MinX",      stCarBodyVxlFilter.stCuboid.minX);
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "MaxX",      stCarBodyVxlFilter.stCuboid.maxX);
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "MinY",      stCarBodyVxlFilter.stCuboid.minY);
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "MaxY",      stCarBodyVxlFilter.stCuboid.maxY);
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "MinZ",      stCarBodyVxlFilter.stCuboid.minZ);
    rslt += GetAttribute(pobjCarBodyFilterSizeCfg, "MaxZ",      stCarBodyVxlFilter.stCuboid.maxZ);

    CUBOID_STRU& stBox = stCarBodyVxlFilter.stCuboid;
    ST_LOG_INFO("Params: Filter voxel size:%.3f", stCarBodyVxlFilter.voxelSize);
    ST_LOG_INFO("Params: Filter Box: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f minZ:%.3f maxZ:%.3f",
                stBox.minX, stBox.maxX, stBox.minY, stBox.maxY, stBox.minZ, stBox.maxZ);

    return rslt;
}

/**************************************************************************************
功能描述: 读取地面分割参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadGroundSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GROUND_SEG_STRU &stGroundSeg)
{
    TiXmlElement *pobjGroundSeg = GetFirstChildElement(pobjCfgParam, "PcdGroundSegmentation");
    if (pobjGroundSeg == NULL) return -1;

    INT32 rslt = 0;

    TiXmlElement *pobjHyperParam = GetFirstChildElement(pobjGroundSeg, "HyperParam");
    if (pobjHyperParam == NULL) return -1;
    rslt += GetAttribute(pobjHyperParam, "isUseRPY", stGroundSeg.isUseRPY);

    TiXmlElement *pobjSegParams = GetFirstChildElement(pobjGroundSeg, "SegParams");
    if (pobjSegParams == NULL) return -1;
    rslt += GetAttribute(pobjSegParams, "MaxLocalSlope",        stGroundSeg.stCfgSeg.maxLocalSlope);
    rslt += GetAttribute(pobjSegParams, "MaxGeneralSlope",      stGroundSeg.stCfgSeg.maxGeneralSlope);
    rslt += GetAttribute(pobjSegParams, "MinHeightThreshold",   stGroundSeg.stCfgSeg.minHeightThreshold);
    rslt += GetAttribute(pobjSegParams, "ReclassDistThreshold", stGroundSeg.stCfgSeg.reclassDistThreshold);
    rslt += GetAttribute(pobjSegParams, "RadiusThreshold",      stGroundSeg.stCfgSeg.radiusThreshold);

    TiXmlElement *pobjSegSecParams = GetFirstChildElement(pobjGroundSeg, "SegSecParams");
    if (pobjSegSecParams == NULL) return -1;
    rslt += GetAttribute(pobjSegSecParams, "SecNums",       stGroundSeg.stCfgSegSec.secNums);
    rslt += GetAttribute(pobjSegSecParams, "LoopNums",      stGroundSeg.stCfgSegSec.loopNums);
    rslt += GetAttribute(pobjSegSecParams, "MinDiffHeight", stGroundSeg.stCfgSegSec.minDiffHeight);
    rslt += GetAttribute(pobjSegSecParams, "Coefficient",   stGroundSeg.stCfgSegSec.coefficient);

    if(rslt != 0) return -1;
    ST_LOG_INFO("Cfg Param Read Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 读取感知区域参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadPnpRegionCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PCPT_REGION_STRU &stPcptRegion)
{
    TiXmlElement *pobjPcptRegionCfg = GetFirstChildElement(pobjCfgParam, "PcptRegion");
    if (pobjPcptRegionCfg == NULL) return -1;

    INT32 rslt = 0;
    //rslt += ReadCuboidCfg(pobjPcptRegionCfg, "CorseRegion", stPcptRegion.stCorseRegion);
    //rslt += ReadCuboidCfg(pobjPcptRegionCfg, "FineRegion",  stPcptRegion.stFineRegion);
    rslt += ReadCuboidCfg(pobjPcptRegionCfg, "ObstRegion",  stPcptRegion.stObstRegion);
    stPcptRegion.stObstRegion.minZ = MIN(stPcptRegion.stObstRegion.minZ, 0.05);

    //TiXmlElement *pobjCorseRegionCfg = GetFirstChildElement(pobjPcptRegionCfg, "CorseRegion");
    //if (pobjCorseRegionCfg == NULL) return -1;
    //rslt += GetAttribute(pobjCorseRegionCfg, "Cell", stPcptRegion.corseCell);

    //TiXmlElement *pobjFineRegionCfg = GetFirstChildElement(pobjPcptRegionCfg, "FineRegion");
    //if (pobjFineRegionCfg == NULL) return -1;
    //rslt += GetAttribute(pobjFineRegionCfg, "Cell", stPcptRegion.fineCell);

    if (!rslt) {
        ST_LOG_INFO("<%s-%s> Cfg Param: MinX:%.2f MaxX:%.2f MinY:%.2f MaxY:%.2f MinZ:%.2f MaxZ:%.2f Cell:%.2f.", "PcptRegion", "CorseRegion",
                    stPcptRegion.stCorseRegion.minX, stPcptRegion.stCorseRegion.maxX,
                    stPcptRegion.stCorseRegion.minY, stPcptRegion.stCorseRegion.maxY,
                    stPcptRegion.stCorseRegion.minZ, stPcptRegion.stCorseRegion.maxZ,
                    stPcptRegion.corseCell);
        ST_LOG_INFO("<%s-%s> Cfg Param: MinX:%.2f MaxX:%.2f MinY:%.2f MaxY:%.2f MinZ:%.2f MaxZ:%.2f Cell:%.2f.", "PcptRegion", "FineRegion",
                    stPcptRegion.stFineRegion.minX, stPcptRegion.stFineRegion.maxX,
                    stPcptRegion.stFineRegion.minY, stPcptRegion.stFineRegion.maxY,
                    stPcptRegion.stFineRegion.minZ, stPcptRegion.stFineRegion.maxZ,
                    stPcptRegion.fineCell);
        ST_LOG_INFO("<%s-%s> Cfg Param: MinX:%.2f MaxX:%.2f MinY:%.2f MaxY:%.2f MinZ:%.2f MaxZ:%.2f.", "PcptRegion", "ObstRegion",
                    stPcptRegion.stObstRegion.minX, stPcptRegion.stObstRegion.maxX,
                    stPcptRegion.stObstRegion.minY, stPcptRegion.stObstRegion.maxY,
                    stPcptRegion.stObstRegion.minZ, stPcptRegion.stObstRegion.maxZ);
    }

    return rslt;
}

/**************************************************************************************
功能描述: 读取感知点云维持参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadPnpCloudPersistenceCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PCPT_CLOUD_PERSISTENCE_STRU &stPcptCloudPersistence)
{
    TiXmlElement *pobjPcptCloudPersistenceCfg = GetFirstChildElement(pobjCfgParam, "PcptCloudPersistence");
    if (pobjPcptCloudPersistenceCfg == NULL) return -1;

    if (GetAttribute(pobjPcptCloudPersistenceCfg, "isSupport", stPcptCloudPersistence.isSupport))
        return -1;
    else
        ST_LOG_INFO("PcptCloudPersistence Cfg Param: <isSupport> = %d.", stPcptCloudPersistence.isSupport);

    if (!stPcptCloudPersistence.isSupport) return 0;

    INT32 rslt = 0;

    TiXmlElement *pobjGroundParams = GetFirstChildElement(pobjPcptCloudPersistenceCfg, "GroundParam");
    if (pobjGroundParams == NULL) return -1;
    rslt += GetAttribute(pobjGroundParams, "MaxVoxelNum", stPcptCloudPersistence.maxGroundVoxelNum);

    TiXmlElement *pobjNoGroundParams = GetFirstChildElement(pobjPcptCloudPersistenceCfg, "NoGroundParam");
    if (pobjNoGroundParams == NULL) return -1;
    rslt += GetAttribute(pobjNoGroundParams, "VoxelSize", stPcptCloudPersistence.voxelSize);
    rslt += GetAttribute(pobjNoGroundParams, "MinR", stPcptCloudPersistence.minR);
    rslt += GetAttribute(pobjNoGroundParams, "MaxR", stPcptCloudPersistence.maxR);
    rslt += GetAttribute(pobjNoGroundParams, "ThetaBins", stPcptCloudPersistence.thetaBins);
    rslt += GetAttribute(pobjNoGroundParams, "PhiBins", stPcptCloudPersistence.phiBins);
    rslt += GetAttribute(pobjNoGroundParams, "PhiMin", stPcptCloudPersistence.phiMin);
    rslt += GetAttribute(pobjNoGroundParams, "PhiMax", stPcptCloudPersistence.phiMax);

    TiXmlElement *pobjProbabilityParams = GetFirstChildElement(pobjPcptCloudPersistenceCfg, "Probability");
    if (pobjProbabilityParams == NULL) return -1;
    rslt += GetAttribute(pobjProbabilityParams, "LoOCc", stPcptCloudPersistence.loOcc);
    rslt += GetAttribute(pobjProbabilityParams, "LoFree", stPcptCloudPersistence.loFree);
    rslt += GetAttribute(pobjProbabilityParams, "OccThresh", stPcptCloudPersistence.occThresh);
    rslt += GetAttribute(pobjProbabilityParams, "FreeThresh", stPcptCloudPersistence.freeThresh);

    if(rslt != 0) return -1;
    ST_LOG_INFO("PcptCloudPersistence Param Read Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 读取2维障碍分割参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::Read2dObstSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LAYER_HEIGHT_STRU& stLayerHeight)
{
    TiXmlElement *pobjSeg2dObst = GetFirstChildElement(pobjCfgParam, "Seg2dObst");
    if (pobjSeg2dObst == NULL) return -1;
    int rslt = 0;
    rslt += GetAttribute(pobjSeg2dObst, "MidLayerMinHeight", stLayerHeight.midLayerMinHeight);
    rslt += GetAttribute(pobjSeg2dObst, "MidLayerMaxHeight", stLayerHeight.midLayerMaxHeight);
    if (rslt !=0 ) return -1;

    if (stLayerHeight.midLayerMinHeight > stLayerHeight.midLayerMaxHeight) {
        ST_LOG_ERR("参数错误：<MidLayerMinHeight>:%f <MidLayerMaxHeight>:%f.",
                    stLayerHeight.midLayerMinHeight, stLayerHeight.midLayerMaxHeight);
        return -1;
    }

    ST_LOG_INFO("Obstacle layering <MidLayerMinHeight>:%f <MidLayerMaxHeight>:%f.",
                stLayerHeight.midLayerMinHeight, stLayerHeight.midLayerMaxHeight);
    return 0;
}

INT32 CFG_PARAM_READ_PNP_C::ReadMultiframeCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_MUTLTIFRAME_STRU& stMultiframe)
{
    TiXmlElement *pobjMultiframe = GetFirstChildElement(pobjCfgParam, "Multiframe");
    if (!pobjMultiframe) return -1;

    INT32 rslt = GetAttribute(pobjMultiframe, "isSupport", stMultiframe.isSupport);
    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "Multiframe", stMultiframe.isSupport);

    if (!stMultiframe.isSupport) return 0;
    rslt = GetAttribute(pobjMultiframe, "FrameNum", stMultiframe.frameNum);

    if (rslt) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }
    ST_LOG_INFO("The frame number for multi frame processing of lidar is %d", stMultiframe.frameNum);
    return 0;
}

/**************************************************************************************
功能描述: 读取双激光雷达反光校验参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadDoubleLidarReflectVerifyCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_DOUBLE_LIDAR_VERIFY_STRU &stReflectVerify)
{
    TiXmlElement *pobjReflectVerify = GetFirstChildElement(pobjCfgParam, element);
    if (pobjReflectVerify == NULL) return -1;

    if (GetAttribute(pobjReflectVerify, "isSupport", stReflectVerify.isSupport))
        return -1;
    else
        ST_LOG_INFO("%s Cfg Param: <isSupport> = %d.", element, stReflectVerify.isSupport);

    if (!stReflectVerify.isSupport) return 0;

    if (ReadRectangleCfg(pobjReflectVerify, "Region", stReflectVerify.stVerifyReg)) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }

    RECTANGLE_STRU& stReg = stReflectVerify.stVerifyReg;
    ST_LOG_INFO("Verification Region: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f", stReg.minX, stReg.maxX, stReg.minY, stReg.maxY);

    return 0;
}

/**************************************************************************************
功能描述: 读取突现反光校验参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadEmergenceReflectVerifyCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_EMERGENCE_VERIFY_STRU &stReflectVerify)
{
    TiXmlElement *pobjReflectVerify = GetFirstChildElement(pobjCfgParam, element);
    if (pobjReflectVerify == NULL) return -1;
    if (GetAttribute(pobjReflectVerify, "isSupport", stReflectVerify.isSupport)) return -1;

    ST_LOG_INFO("%s Cfg Param: <isSupport> = %d.", element, stReflectVerify.isSupport);

    if (!stReflectVerify.isSupport) return 0;

    int rslt = ReadRectangleCfg(pobjReflectVerify, "Region", stReflectVerify.stVerifyReg);

    TiXmlElement *pobjVerifyParam = GetFirstChildElement(pobjReflectVerify, "VerifyParam");
    if (pobjVerifyParam == NULL) return -1;
    rslt += GetAttribute(pobjVerifyParam, "CompareRadius", stReflectVerify.CompareRadius);
    rslt += GetAttribute(pobjVerifyParam, "KeyFrameDist",  stReflectVerify.keyFrameDist);

    if (rslt) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }

    RECTANGLE_STRU& stReg = stReflectVerify.stVerifyReg;
    ST_LOG_INFO("Verification Region: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f", stReg.minX, stReg.maxX, stReg.minY, stReg.maxY);

    return 0;
}

/**************************************************************************************
功能描述: 读取路面标识校验参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadRoadmarkVerifyCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_ROADMARK_VERIFY_STRU &stReflectVerify)
{
    TiXmlElement *pobjReflectVerify = GetFirstChildElement(pobjCfgParam, element);
    if (pobjReflectVerify == NULL) return -1;
    if (GetAttribute(pobjReflectVerify, "isSupport", stReflectVerify.isSupport)) return -1;
    ST_LOG_INFO("%s Cfg Param: <isSupport> = %d.", element, stReflectVerify.isSupport);
    return 0;
}

/**************************************************************************************
功能描述: 读取雷达反光校验参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadReflectVerifyCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_REFLECT_VERIFY_STRU &stReflectVerify)
{
    TiXmlElement *pobjReflectVerify = GetFirstChildElement(pobjCfgParam, "ReflectVerify");
    if (pobjReflectVerify == NULL) return -1;

    INT32 rslt = 0;
    rslt += ReadDoubleLidarReflectVerifyCfg(pobjReflectVerify, "DoubleLidarVerify", stReflectVerify.stDoubleLidarVerify);
    rslt += ReadEmergenceReflectVerifyCfg(pobjReflectVerify, "EmergenceVerify",   stReflectVerify.stEmergenceVerify);
    rslt += ReadRoadmarkVerifyCfg(pobjReflectVerify, "RoadmarkVerify", stReflectVerify.stRoadmarkVerify);

    if (rslt) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取低矮障碍物维持配置参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadShortObstHoldCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SHORT_OBST_HOLD_STRU &stShortObstHold)
{
    TiXmlElement *pobjShortObstHold = GetFirstChildElement(pobjCfgParam, "ShortObstHold");
    if (pobjShortObstHold == NULL) return -1;

    INT32 rslt = GetAttribute(pobjShortObstHold, "isSupport", stShortObstHold.isSupport);
    if (rslt != 0)  return -1;
    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "ShortObstHold", stShortObstHold.isSupport);

    if (!stShortObstHold.isSupport) return 0;
    rslt += ReadRectangleCfg(pobjShortObstHold, "Region", stShortObstHold.stRegion);

    TiXmlElement *pobjHedgeRadius = GetFirstChildElement(pobjShortObstHold, "HyperParam");
    if (pobjHedgeRadius == NULL) return -1;
    rslt += GetAttribute(pobjHedgeRadius, "MaxHeight",   stShortObstHold.maxHeight);
    rslt += GetAttribute(pobjHedgeRadius, "HedgeRadius", stShortObstHold.hedgeRadius);

    if (rslt) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }

    RECTANGLE_STRU &stRect = stShortObstHold.stRegion;
    ST_LOG_INFO("HedgeRadius:%.3f", stShortObstHold.hedgeRadius);
    ST_LOG_INFO("Holding region: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f", stRect.minX, stRect.maxX, stRect.minY, stRect.maxY);

    return 0;
}

/**************************************************************************************
功能描述: 读取常规障碍物维持配置参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadCommonObstHoldCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_COMMON_OBST_HOLD_STRU &stCommonObstHold)
{
    TiXmlElement *pobjCommonObstHold = GetFirstChildElement(pobjCfgParam, "CommonObstHold");
    if (pobjCommonObstHold == NULL)      return -1;

    INT32 rslt = GetAttribute(pobjCommonObstHold, "isSupport", stCommonObstHold.isSupport);
    if (rslt != 0)  return -1;

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "CommonObstHold", stCommonObstHold.isSupport);

    if (!stCommonObstHold.isSupport) return 0;

    rslt += ReadRectangleCfg(pobjCommonObstHold, "MinRegion", stCommonObstHold.stMinRegion);
    rslt += ReadRectangleCfg(pobjCommonObstHold, "MaxRegion", stCommonObstHold.stMaxRegion);

    if (rslt) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }

    RECTANGLE_STRU &stRectMin = stCommonObstHold.stMinRegion;
    RECTANGLE_STRU &stRectMax = stCommonObstHold.stMaxRegion;
    ST_LOG_INFO("Min holding region: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f", stRectMin.minX, stRectMin.maxX, stRectMin.minY, stRectMin.maxY);
    ST_LOG_INFO("Max holding region: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f", stRectMax.minX, stRectMax.maxX, stRectMax.minY, stRectMax.maxY);

    return 0;
}

/**************************************************************************************
功能描述: 读取点云平面分割配置参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadPlaneSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PLANE_SEG_STRU &stPlaneSeg)
{
    TiXmlElement *pobjPlaneSeg = GetFirstChildElement(pobjCfgParam, "PlaneSegmentation");
    if (pobjPlaneSeg == NULL)      return -1;

    INT32 rslt = GetAttribute(pobjPlaneSeg, "isSupport", stPlaneSeg.isSupport);
    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "PlaneSegmentation", stPlaneSeg.isSupport);

    if (!stPlaneSeg.isSupport) return 0;

    TiXmlElement *pobjDrop = GetFirstChildElement(pobjPlaneSeg, "DropDet");
    if (pobjDrop == NULL) return -1;
    rslt += GetAttribute(pobjDrop, "isSupport",  stPlaneSeg.stDropDet.isSupport);
    rslt += GetAttribute(pobjDrop, "DropHeight", stPlaneSeg.stDropDet.height);
    if (stPlaneSeg.stDropDet.isSupport)
        ST_LOG_INFO("Supports drop detection, and the detection height threshold is :%f", stPlaneSeg.stDropDet.height);

    rslt += ReadRectangleCfg(pobjPlaneSeg, "Region", stPlaneSeg.stRegion);

    TiXmlElement *pobjHyper = GetFirstChildElement(pobjPlaneSeg, "HyperParam");
    if (pobjHyper == NULL) return -1;
    rslt += GetAttribute(pobjHyper, "IterationTimes",  stPlaneSeg.iterationTimes);
    rslt += GetAttribute(pobjHyper, "UnitX",           stPlaneSeg.unitX);
    rslt += GetAttribute(pobjHyper, "MaxAngleByX",     stPlaneSeg.maxAngleByX);
    rslt += GetAttribute(pobjHyper, "MaxAngleByY",     stPlaneSeg.maxAngleByY);
    rslt += GetAttribute(pobjHyper, "HeightThreshold", stPlaneSeg.heightThreshold);
    rslt += GetAttribute(pobjHyper, "isUseVelItem",    stPlaneSeg.isUseVelItem);

    if (IS_FLOAT_ALMOST_EQUAL_ZERO(stPlaneSeg.unitX)) {
        ST_LOG_ERR("Invalid Param: <%s-%s-UnitX> = %.1f", pobjPlaneSeg->Value(), pobjHyper->Value(), stPlaneSeg.unitX);
        return -1;
    }

    if (rslt) {
        ST_LOG_ERR("Failed to read configuration parameters");
        return -1;
    }

    RECTANGLE_STRU& stRect = stPlaneSeg.stRegion;
    ST_LOG_INFO("Plane Region: minX:%.3f maxX:%.3f minY:%.3f maxY:%.3f", stRect.minX, stRect.maxX, stRect.minY, stRect.maxY);
    ST_LOG_INFO("Plane HyperParam: IterationTimes:%d, UnitX:%.3f MaxAngleByX:%.3f MaxAngleByY:%.3f HeightThreshold:%.3f",
                stPlaneSeg.iterationTimes, stPlaneSeg.unitX, stPlaneSeg.maxAngleByX, stPlaneSeg.maxAngleByY, stPlaneSeg.heightThreshold);

    return 0;
}

/**************************************************************************************
功能描述: 读取点云聚类区域配置参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadClusterRegionCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_REGION_STRU &stRegion)
{
    INT32 rslt = 0;
    rslt += GetAttribute(pobjCfgParam, "OriginDistance",  stRegion.originDistance);
    rslt += GetAttribute(pobjCfgParam, "ClusterRadius",   stRegion.clusterRadius);
    rslt += GetAttribute(pobjCfgParam, "MinCluPointsNum", stRegion.minCluPointsNum);
    rslt += GetAttribute(pobjCfgParam, "MaxCluPointsNum", stRegion.maxCluPointsNum);
    if (stRegion.minCluPointsNum > stRegion.maxCluPointsNum) {
        ST_LOG_ERR("Points Num <%u~%u> Error. Please Set the Correct Points Num. Note: MinCluPointsNum <= MaxCluPointsNum", stRegion.minCluPointsNum, stRegion.maxCluPointsNum);
        return -1;
    }

    return rslt;
}

/**************************************************************************************
功能描述: 读取点云聚类参数配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadClusterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CLUSTER_STRU &stCluster)
{
    TiXmlElement *pobjClusterCfg = GetFirstChildElement(pobjCfgParam, "PcdCluster");
    if (pobjClusterCfg == NULL)      return -1;

    INT32 rslt = GetAttribute(pobjClusterCfg, "isSupport", stCluster.isSupport);
    if (rslt != 0)  return -1;

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "PcdCluster", stCluster.isSupport);

    if (!stCluster.isSupport) return 0;

    TiXmlElement *pobjVoxelGridCfg = GetFirstChildElement(pobjClusterCfg, "VoxelGrid");
    if (pobjVoxelGridCfg == NULL)       return -1;
    rslt += GetAttribute(pobjVoxelGridCfg, "Size", stCluster.voxelSize);

    TiXmlElement *pobjRegionPartitionCfg = GetFirstChildElement(pobjClusterCfg, "RegionPartition");
    if (pobjRegionPartitionCfg == NULL) return -1;
    CFG_PARAM_REGION_STRU stRegion;
    TiXmlElement *pobjRegionCfg = GetFirstChildElement(pobjRegionPartitionCfg, "RegCfg");
    if (pobjRegionCfg == NULL)          return -1;
    for(; pobjRegionCfg != NULL; pobjRegionCfg = pobjRegionCfg->NextSiblingElement("RegCfg")) {
        rslt += ReadClusterRegionCfg(pobjRegionCfg, stRegion);
        stCluster.vstRegion.push_back(stRegion);
    }

    ST_LOG_INFO("Clusting Params: VoxelGrid:%.3f", stCluster.voxelSize);
    for (uint32_t i = 0; i < stCluster.vstRegion.size(); ++i) {
        CFG_PARAM_REGION_STRU& stRegion = stCluster.vstRegion[i];
        ST_LOG_INFO("Clusting Region%u params: OriginDistance:%.3f ClusterRadius:%.3f, MinCluPointsNum:%u, MaxCluPointsNum:%u",
            i+1, stRegion.originDistance, stRegion.clusterRadius, stRegion.minCluPointsNum, stRegion.maxCluPointsNum);
    }

    return rslt;
}

/**************************************************************************************
功能描述: 读取点云数据网络检测相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadNetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_NET_STRU &stNet)
{
    TiXmlElement *pobjPcdNet = GetFirstChildElement(pobjCfgParam, "PcdNet");
    if (pobjPcdNet == NULL)      return -1;

    INT32 rslt = GetAttribute(pobjPcdNet, "isSupport", stNet.isSupport);
    if (rslt != 0)  return -1;

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", "PcdNet", stNet.isSupport);

    return 0;
}

/**************************************************************************************
功能描述: 读取检测桩（充电、加水、倒垃圾）相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadDetectPileCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_DETECT_PILE_STRU &stDetectPile)
{
    TiXmlElement *pobjPile = GetFirstChildElement(pobjCfgParam, "DetectPile");
    if (!pobjPile) return -1;

    TiXmlElement *pobjQRCode = GetFirstChildElement(pobjPile, "QrCode");
    if (!pobjQRCode) return -1;
    int rslt = GetAttribute(pobjQRCode, "VaildDist", stDetectPile.qrCodeValidDist);
    if (rslt) return -1;

    TiXmlElement *pobjDustbin = GetFirstChildElement(pobjPile, "Dustbin");
    if (!pobjDustbin) return -1;
    rslt = GetAttribute(pobjDustbin, "BinType",    stDetectPile.binType);
    rslt = GetAttribute(pobjDustbin, "DistThresh", stDetectPile.distThresh);
    rslt = GetAttribute(pobjDustbin, "NumThresh",  stDetectPile.numThresh);
    if (rslt) return -1;

    TiXmlElement *pobjCharging = GetFirstChildElement(pobjPile, "ChargingPile");
    if (!pobjCharging) return -1;
    rslt += GetAttribute(pobjCharging, "IntensityThreshold", stDetectPile.intensityThreshold);
    rslt += GetAttribute(pobjCharging, "ChargingPileHeight", stDetectPile.chargingPileHeight);
    rslt += GetAttribute(pobjCharging, "OffsetY",            stDetectPile.offsetY);
    if (rslt) return -1;

    ST_LOG_INFO("<%s-%s> Cfg Param: <%s> = %f.", "DetectPile", "QrCode",       "VaildDist",          stDetectPile.qrCodeValidDist);
    ST_LOG_INFO("<%s-%s> Cfg Param: <%s> = %d.", "DetectPile", "Dustbin",      "BinType",            stDetectPile.binType);
    ST_LOG_INFO("<%s-%s> Cfg Param: <%s> = %d.", "DetectPile", "ChargingPile", "IntensityThreshold", stDetectPile.intensityThreshold);
    ST_LOG_INFO("<%s-%s> Cfg Param: <%s> = %f.", "DetectPile", "ChargingPile", "ChargingPileHeight", stDetectPile.chargingPileHeight);
    ST_LOG_INFO("<%s-%s> Cfg Param: <%s> = %f.", "DetectPile", "ChargingPile", "OffsetY",            stDetectPile.offsetY);

    return 0;
}

/**************************************************************************************
功能描述: 读取动静态障碍检测相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadObstStateCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_OBST_STATE_STRU &stObstState)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "ObstState");
    if (!pobjCfg) return -1;
    if (GetAttribute(pobjCfg, "isSupport", stObstState.isSupport)) return -1;
    if (!stObstState.isSupport) {
        ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", pobjCfg->Value(), stObstState.isSupport);
    } else {
        if (GetAttribute(pobjCfg, "StaticThreshold", stObstState.staticThreshold)) return -1;
        ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d <StaticThreshold> = %f.", pobjCfg->Value(),
                                                        stObstState.isSupport, stObstState.staticThreshold);
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取交通灯检测相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadTrafficLightCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_TRAFFIC_LIGHT_STRU &stTrafficLight)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "TrafficLight");
    if (!pobjCfg) return -1;
    if (GetAttribute(pobjCfg, "PixelDeviation", stTrafficLight.pixelBias)) return -1;
    if (GetAttribute(pobjCfg, "MaxDiffRatio",   stTrafficLight.rectBias)) return -1;
    ST_LOG_INFO("<%s> Cfg Param: <%s> = %d.", pobjCfg->Value(), "PixelDeviation", stTrafficLight.pixelBias);
    ST_LOG_INFO("<%s> Cfg Param: <%s> = %f.", pobjCfg->Value(), "MaxDiffRatio",   stTrafficLight.rectBias);
    return 0;
}

/**************************************************************************************
功能描述: 读取可通行区域相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadAccessibleAreaCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ACCESSIBLE_AREA_STRU &stAccArea)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "AccessibleArea");
    if (!pobjCfg) return -1;

    INT32 rslt = GetAttribute(pobjCfg, "isSupport", stAccArea.isSupport);
    if (rslt != 0) return -1;

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", pobjCfg->Value(), stAccArea.isSupport);

    return 0;
}

/**************************************************************************************
功能描述: 读取斜坡垫检测相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadSlopePadDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SLOPE_PAD_DET_STRU &stSlopePadDet)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "SlopePadDet");
    if (!pobjCfg) return -1;

    INT32 rslt = GetAttribute(pobjCfg, "isSupport", stSlopePadDet.isSupport);
    if (rslt != 0) return -1;

    if (!stSlopePadDet.isSupport) {
        ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", pobjCfg->Value(), stSlopePadDet.isSupport);
    } else {
        rslt = GetAttribute(pobjCfg, "ExtandDist", stSlopePadDet.extandDist);
        ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d, <ExtandDist> = %.2f.", pobjCfg->Value(),
                                                                              stSlopePadDet.isSupport,
                                                                              stSlopePadDet.extandDist);
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取道路区域外点云过滤参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadRoadRegionFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ROAD_REGION_FILTER_STRU &stRoadRegionFilter)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "RoadRegionFilter");
    if (!pobjCfg) return -1;

    INT32 rslt = GetAttribute(pobjCfg, "isSupport", stRoadRegionFilter.isSupport);
    if (rslt != 0) return -1;

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", pobjCfg->Value(), stRoadRegionFilter.isSupport);
    return 0;
}

/**************************************************************************************
功能描述: 读取过滤点云重建参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadFilterPointsRebuildCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FILTER_POINTS_REBUILD_STRU &stFilterPointsRebuild)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "FilterPointsRebuild");
    if (!pobjCfg) return -1;

    INT32 rslt = GetAttribute(pobjCfg, "isSupport", stFilterPointsRebuild.isSupport);
    if (rslt != 0) return -1;

    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d.", pobjCfg->Value(), stFilterPointsRebuild.isSupport);
    return 0;
}

/**************************************************************************************
功能描述: 库房门相关配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadWarehouseCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_WAREHOUSE_STRU &stWarehouse)
{
    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "Warehouse");
    if (!pobjCfg) return -1;

    INT32 rslt = 0;
    rslt = GetAttribute(pobjCfg, "ReflectiveStripHeight", stWarehouse.reflectiveStripHeight);
    rslt = GetAttribute(pobjCfg, "IntensityThreshold",    stWarehouse.intensityThreshold);
    if (rslt != 0) return -1;

    ST_LOG_INFO("<%s> Cfg Param: <ReflectiveStripHeight> = %f, <IntensityThreshold> = %d.",
        pobjCfg->Value(), stWarehouse.reflectiveStripHeight, stWarehouse.intensityThreshold);
    return 0;
}

/**************************************************************************************
功能描述: 车身噪点去除配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadCarBodyDenoisingCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CAR_BODY_DENOSING_STRU &stCarBodyDenoising)
{

    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "CarBodyDenoising");
    if (!pobjCfg) {
        pobjCfg = GetFirstChildElement(pobjCfgParam, "CarBodyDenoising");
        if (!pobjCfg) return -1;
    }

    INT32 rslt = 0;
    rslt = GetAttribute(pobjCfg, "isSupport", stCarBodyDenoising.isSupport);
    rslt = GetAttribute(pobjCfg, "Resolution",stCarBodyDenoising.resolution);
    if (rslt != 0) return -1;
    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d, <Resolution> = %f.",
        pobjCfg->Value(), stCarBodyDenoising.isSupport, stCarBodyDenoising.resolution);
    return 0;
}

/**************************************************************************************
功能描述: 洁净度配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadCleanlinessCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CLEANLINESS_STRU &stCleanliness)
{

    TiXmlElement *pobjCfg = GetFirstChildElement(pobjCfgParam, "Cleanliness");
    if (!pobjCfg) {
        pobjCfg = GetFirstChildElement(pobjCfgParam, "Cleanliness");
        if (!pobjCfg) return -1;
    }

    INT32 rslt = 0;
    rslt = GetAttribute(pobjCfg, "isSupport", stCleanliness.isSupport);
    rslt = GetAttribute(pobjCfg, "length",stCleanliness.length);
    rslt = GetAttribute(pobjCfg, "width",stCleanliness.width);
    rslt = GetAttribute(pobjCfg, "coefficient",stCleanliness.coefficient);
    if (rslt != 0) return -1;
    ST_LOG_INFO("<%s> Cfg Param: <isSupport> = %d, <length> = %f, <width> = %f, <coefficient> = %f",
        pobjCfg->Value(), stCleanliness.isSupport, stCleanliness.length, stCleanliness.width, stCleanliness.coefficient);
    return 0;
}

/**************************************************************************************
功能描述: 读取点云数据处理相关参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadPnpCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PNP_STRU &stPnpParam)
{
    TiXmlElement *pobjPnpCfg = GetFirstChildElement(pobjCfgParam, "Pnp");
    if (!pobjPnpCfg){
        ST_LOG_ERR("Cfg Param Don't Has the Element <%s>.", "Pnp");
        return -1;
    }

    INT32 rslt = GetAttribute(pobjPnpCfg, "isSupport", stPnpParam.isSupport);
    if (rslt != 0)  return -1;
    if (!stPnpParam.isSupport) {
        ST_LOG_INFO("配置文件配置 不支持 感知.");
        return 0;
    }

    rslt = 0;
    rslt += ReadCarBodyVxlFilterCfg(   pobjPnpCfg, stPnpParam.stCarBodyVxlFilter);
    rslt += ReadGroundSegCfg(          pobjPnpCfg, stPnpParam.stGroundSeg);
    rslt += ReadPnpRegionCfg(          pobjPnpCfg, stPnpParam.stPcptRegion);
    rslt += ReadPnpCloudPersistenceCfg(pobjPnpCfg, stPnpParam.stPcptCloudPersistence);
    rslt += ReadReflectVerifyCfg(      pobjPnpCfg, stPnpParam.stReflectVerify);
    rslt += ReadShortObstHoldCfg(      pobjPnpCfg, stPnpParam.stShortObstHold);
    rslt += ReadCommonObstHoldCfg(     pobjPnpCfg, stPnpParam.stCommonObstHold);
    rslt += ReadPlaneSegCfg(           pobjPnpCfg, stPnpParam.stPlaneSeg);
    rslt += ReadClusterCfg(            pobjPnpCfg, stPnpParam.stCluster);
    rslt += ReadNetCfg(                pobjPnpCfg, stPnpParam.stNet);
    rslt += Read2dObstSegCfg(          pobjPnpCfg, stPnpParam.stLayerHeight);
    rslt += ReadMultiframeCfg(         pobjPnpCfg, stPnpParam.stMultiframe);
    rslt += ReadDetectPileCfg(         pobjPnpCfg, stPnpParam.stDetectPile);
    rslt += ReadObstStateCfg(          pobjPnpCfg, stPnpParam.stObstState);
    rslt += ReadTrafficLightCfg(       pobjPnpCfg, stPnpParam.stTrafficLight);
    rslt += ReadAccessibleAreaCfg(     pobjPnpCfg, stPnpParam.stAccessibleArea);
    rslt += ReadSlopePadDetCfg(        pobjPnpCfg, stPnpParam.stSlopePadDet);
    rslt += ReadRoadRegionFilterCfg(   pobjPnpCfg, stPnpParam.stRoadRegionFilter);
    rslt += ReadFilterPointsRebuildCfg(pobjPnpCfg, stPnpParam.stFilterPointsRebuild);
    rslt += ReadWarehouseCfg(          pobjPnpCfg, stPnpParam.stWarehouse);
    rslt += ReadCarBodyDenoisingCfg(   pobjPnpCfg, stPnpParam.stCarBodyDenoising);
    rslt += ReadCleanlinessCfg(        pobjPnpCfg, stPnpParam.stCleanliness);

    return rslt;
}

/**************************************************************************************
功能描述: 校验相机是否有效
修改记录:
**************************************************************************************/
BOOL CFG_PARAM_READ_PNP_C::IsCameraValid(string strCamera, vector<string>& vstrCamera) const
{
    if (strCamera.empty()) return false;

    size_t index = 0;
    while((index = strCamera.find(' ', index)) != string::npos) strCamera.erase(index, 1);    //删除空格

    istringstream iss(strCamera);
    string word;
    while(getline(iss, word, ',')) vstrCamera.push_back(word);

    if (vstrCamera.empty()) return false;

    for (const string& camera : vstrCamera) {
        if (!IsValidSensor(RgbCamera, camera)) {
            ST_LOG_ERR("This is not a valid camera <%s>.", camera.c_str());
            return false;
        }
    }

    return true;
}

/**************************************************************************************
功能描述: 相机Id转字符串
修改记录:
**************************************************************************************/
string CFG_PARAM_READ_PNP_C::CamerasId2Str(const vector<string>& vstrCamera) const
{
    if (vstrCamera.empty()) return "";

    string strCamera;
    for (auto iter = vstrCamera.begin(); iter != vstrCamera.end(); ++iter) {
        strCamera += *iter;
        if (iter != vstrCamera.end()-1)strCamera += ',';
    }

    return strCamera;
}

/**************************************************************************************
功能描述: 是否支持该视觉类别标签
修改记录:
**************************************************************************************/
BOOL CFG_PARAM_READ_PNP_C::IsSupportVisionLabel(const vector<LABEL_ENUM>& vValidLabel, const LABEL_ENUM label) const
{
    return std::find(vValidLabel.begin(), vValidLabel.end(), label) != vValidLabel.end();
}

/**************************************************************************************
功能描述: 删除视觉类别标签
修改记录:
**************************************************************************************/
void CFG_PARAM_READ_PNP_C::DeleteVisionLabel(vector<LABEL_ENUM>& vValidLabel, LABEL_ENUM label)
{
    auto iter = vValidLabel.begin();
    do {
        iter = std::find(vValidLabel.begin(), vValidLabel.end(), label);
        if (iter != vValidLabel.end()) iter = vValidLabel.erase(iter);
    } while (iter != vValidLabel.end());
}

/**************************************************************************************
功能描述: 读取该视觉功能基本参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionFuncBaseCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_VISION_BASE_STRU& stParam)
{
    TiXmlElement *pobjFuncCfg = GetFirstChildElement(pobjCfgParam, element);
    if (pobjFuncCfg == NULL) return -1;
    if (GetAttribute(pobjFuncCfg, "isSupport", stParam.isSupport)) return -1;
    printf("\n");
    ST_LOG_INFO("<VisionFunction-%s> Cfg Param: <isSupport> = %d.", element, stParam.isSupport);

    if (!stParam.isSupport) return 0;
    pobjFuncCfg->QueryBoolAttribute("IPU", &stParam.isSupportIpu);   //读取不到则认为不支持
    if (GetAttribute(pobjFuncCfg, "Hz", stParam.hz)) return -1;

    /* 读取支持该功能的相机配置 */
    string strCamera;
    if (GetAttribute(pobjFuncCfg, "Camera", strCamera)) return -1;
    if (!IsCameraValid(strCamera, stParam.vstrCamera)) {
        ST_LOG_ERR("The attr <%s> of element <%s> is invalid.", strCamera.c_str(), element);
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 读取视觉功能标签配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionLabelCfg(TiXmlElement *pobjCfgParam, const char *element, LABEL_ENUM enLabel, vector<LABEL_ENUM>& vValidLabel)
{
    TiXmlElement *pobjFuncCfg = GetFirstChildElement(pobjCfgParam, element);
    if (pobjFuncCfg == NULL) return -1;

    bool isSupport = false;
    if (GetAttribute(pobjFuncCfg, "isSupport", isSupport)) return -1;
    ST_LOG_INFO("<VisionFunction-%s> Cfg Param: <isSupport> = %d.", element, isSupport);
    if (!isSupport) return 0;

    vValidLabel.push_back(enLabel);
    return 0;
}

/**************************************************************************************
功能描述: 读取视觉功能标签（粗水管）配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionLabelThickPipeCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_LABEL_THICKPIPE_STRU& stParam)
{
    TiXmlElement *pobjFuncCfg = GetFirstChildElement(pobjCfgParam, element);
    if (pobjFuncCfg == NULL) return -1;
    if (GetAttribute(pobjFuncCfg, "isSupport", stParam.isSupport)) return -1;
    if (stParam.isSupport && GetAttribute(pobjFuncCfg, "width", stParam.width)) return -1;
    ST_LOG_INFO("<VisionFunction-%s> Cfg Param: <isSupport> = %d, <width> = %.2f.", element, stParam.isSupport, stParam.width);
    return 0;
}

/**************************************************************************************
功能描述: 读取该视觉二维码检测配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionQRCodeDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_BASE_STRU& stQRCode)
{
    if (ReadVisionFuncBaseCfg(pobjCfgParam, "QRCodeDet", stQRCode)) return -1;
    if (!stQRCode.isSupport) return 0;

    string strCamera = CamerasId2Str(stQRCode.vstrCamera);

    ST_LOG_INFO("************* QrCodeDet: IPU(%d) Hz(%d) Camera(%s) *************", stQRCode.isSupportIpu, stQRCode.hz, strCamera.c_str());
    return 0;
}

/**************************************************************************************
功能描述: 读取交通灯检测任务配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionTrafficLightDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_TRAFFIC_LIGHT_DET_STRU& stNetFunc)
{
    if (0 != ReadVisionFuncBaseCfg(pobjCfgParam, "TrafficLightDet", stNetFunc.stParam)) return -1;
    if (!stNetFunc.stParam.isSupport) return 0;

    string strCamera = CamerasId2Str(stNetFunc.stParam.vstrCamera);

    ST_LOG_INFO("************* TrafficLightDet: IPU(%d) Hz(%d) Camera(%s) *************",
        stNetFunc.stParam.isSupportIpu, stNetFunc.stParam.hz, strCamera.c_str());

    return 0;
}

/**************************************************************************************
功能描述: 读取垃圾箱检测任务配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionTrashBoxDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_BASE_STRU& stTrashBoxDet)
{
    if (0 != ReadVisionFuncBaseCfg(pobjCfgParam, "TrashBoxDet", stTrashBoxDet)) return -1;
    if (!stTrashBoxDet.isSupport) return 0;

    string strCamera = CamerasId2Str(stTrashBoxDet.vstrCamera);

    ST_LOG_INFO("************* TrashBoxDet: IPU(%d) Hz(%d) Camera(%s) *************",
        stTrashBoxDet.isSupportIpu, stTrashBoxDet.hz, strCamera.c_str());

    return 0;
}

/**************************************************************************************
功能描述: 读取洁净度检测任务配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionCleanClassDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_CLEAN_CLASS_DET_STRU& stCleanClassDet)
{
    if (ReadVisionFuncBaseCfg(pobjCfgParam, "CleanClassDet", stCleanClassDet.stParam)) return -1;
    if (!stCleanClassDet.stParam.isSupport) return 0;

    TiXmlElement *pobjHyperParam = GetFirstChildElement(pobjCfgParam, "HyperParam");
    if (!pobjHyperParam) return -1;

    INT32 rslt = 0;
    rslt += GetAttribute(pobjHyperParam, "length",stCleanClassDet.stDetArea.length);
    rslt += GetAttribute(pobjHyperParam, "width", stCleanClassDet.stDetArea.width);
    rslt += GetAttribute(pobjHyperParam, "blindSpotThreshold",stCleanClassDet.stDetArea.blindSpotThreshold);
    if (rslt != 0) return -1;
    ST_LOG_INFO("<CleanClass> Cfg Param: <isSupport> = %d, <length> = %f, <width> = %f, <blindSpotThreshold> = %f",
                stCleanClassDet.stParam.isSupport, stCleanClassDet.stDetArea.length, 
                stCleanClassDet.stDetArea.width, stCleanClassDet.stDetArea.blindSpotThreshold);
    return 0;
}

/**************************************************************************************
功能描述: 读取该视觉网络检测任务配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionNetDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_NET_DET_STRU& stNetFunc)
{
    if (ReadVisionFuncBaseCfg(pobjCfgParam, "NetDet", stNetFunc.stParam)) return -1;
    if (!stNetFunc.stParam.isSupport) return 0;

    TiXmlElement *pobjNetCfg = GetFirstChildElement(pobjCfgParam, "NetDet");
    if (!pobjNetCfg) return -1;

    string strCamera = CamerasId2Str(stNetFunc.stParam.vstrCamera);

    ST_LOG_INFO("************* NetDet: IPU(%d) Hz(%d) Camera(%s) *************", stNetFunc.stParam.isSupportIpu, stNetFunc.stParam.hz, strCamera.c_str());
    int rslt = 0;
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Bicycle",        TAG_BICYCLE,              stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "SolidCover",     TAG_COVER_SOLID,          stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "HollowCover",    TAG_COVER_HOLLOW,         stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Bottle",         TAG_GARBAGE_BOTTLE,       stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Paper",          TAG_GARBAGE_PAPER,        stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Branch",         TAG_GARBAGE_BRANCH,       stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Leaves",         TAG_GARBAGE_LEAVES,       stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Bough",          TAG_GARBAGE_BOUGH,        stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Cup",            TAG_GARBAGE_CUP,          stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Can",            TAG_GARBAGE_CAN,          stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Carton",         TAG_GARBAGE_CARTON,       stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Package",        TAG_GARBAGE_PACKAGE,      stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Brick",          TAG_GARBAGE_BRICK,        stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Cigaretteend",   TAG_GARBAGE_CIGARETTEEND, stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Pericarp",       TAG_GARBAGE_PERICARP,     stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Cord",           TAG_CORD,                 stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Pipeline",       TAG_PIPELINE,             stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "TrafficCone",    TAG_TRAFFIC_CONE,         stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "MetalBarrier",   TAG_METAL_BARRIER,        stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "MedianBarrier",  TAG_MEDIAN_BARRIER,       stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "CeremonialPole", TAG_CEREMONIAL_POLE,      stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Bollard",        TAG_BOLLARD,              stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "BlackVehicle",   TAG_BLACK_VEHICLE,        stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Car",            TAG_CAR          ,        stNetFunc.vValidLabel);

    /*
    if (0 == ReadVisionLabelCfg(pobjNetCfg, "TrafficLight", TAG_TRAFFIC_LIGHT, stNetFunc.vValidLabel)) {
        if (IsSupportVisionLabel(stNetFunc.vValidLabel, TAG_TRAFFIC_LIGHT)) {
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_OFF);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_RED);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_GREEN);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_YELLOW);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_GROUP);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_OFF_LITE);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_RED_LITE);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_GREEN_LITE);
            stNetFunc.vValidLabel.push_back(TAG_TRAFFIC_LIGHT_YELLOW_LITE);
        }
    } else return -1;
    */

    /* 对标签排序及去重 */
    std::sort(stNetFunc.vValidLabel.begin(), stNetFunc.vValidLabel.end());
    stNetFunc.vValidLabel.erase(std::unique(stNetFunc.vValidLabel.begin(), stNetFunc.vValidLabel.end()), stNetFunc.vValidLabel.end());

    return rslt;
}

/**************************************************************************************
功能描述: 读取该视觉网络分割任务配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionNetSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_NET_SEG_STRU& stNetFunc)
{
    if (ReadVisionFuncBaseCfg(pobjCfgParam, "NetSeg", stNetFunc.stParam)) return -1;
    if (!stNetFunc.stParam.isSupport) return 0;

    TiXmlElement *pobjNetCfg = GetFirstChildElement(pobjCfgParam, "NetSeg");
    if (!pobjNetCfg) return -1;

    string strCamera = CamerasId2Str(stNetFunc.stParam.vstrCamera);

    ST_LOG_INFO("************* NetSeg: IPU(%d) Hz(%d) Camera(%s) *************", stNetFunc.stParam.isSupportIpu, stNetFunc.stParam.hz, strCamera.c_str());
    int rslt = 0;
    rslt += ReadVisionLabelCfg(pobjNetCfg, "SolidCover",  TAG_COVER_SOLID,  stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "HollowCover", TAG_COVER_HOLLOW, stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "RoadEdge",    TAG_ROADEDGE,     stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Pipeline",    TAG_PIPELINE,     stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "ThickPipe",   TAG_THICKPIPE,    stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Cordon",      TAG_CORDON,       stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Treelawn",    TAG_TREELAWN,     stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "SlopePad",    TAG_SLOPE_PAD,    stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "SpeedBump",   TAG_SPEED_BUMP,   stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "FishingRod",  TAG_FISHING_ROD,  stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "FishingRodLlm",  TAG_FISHING_ROD_LLM,  stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "TreelawnLlm", TAG_TREELAWN_LLM ,stNetFunc.vValidLabel);
    rslt += ReadVisionLabelCfg(pobjNetCfg, "Cord", TAG_CORD ,stNetFunc.vValidLabel);

    rslt += ReadVisionLabelThickPipeCfg(pobjNetCfg, "ThickPipe", stNetFunc.stLabelThickPipe);

    /* 对标签排序及去重 */
    std::sort(stNetFunc.vValidLabel.begin(), stNetFunc.vValidLabel.end());
    stNetFunc.vValidLabel.erase(std::unique(stNetFunc.vValidLabel.begin(), stNetFunc.vValidLabel.end()), stNetFunc.vValidLabel.end());

    return rslt;
}

/**************************************************************************************
功能描述: 读取视觉功能配置
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_PNP_C::ReadVisionFuncCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_FUNC_STRU &stVisionFuncParam)
{
    TiXmlElement *pobjVisionFuncCfg = GetFirstChildElement(pobjCfgParam, "VisionFunction");
    if (!pobjVisionFuncCfg) return -1;
    if (GetAttribute(pobjVisionFuncCfg, "isSupport", stVisionFuncParam.isSupport)) return -1;

    if (!stVisionFuncParam.isSupport) {
        ST_LOG_INFO("<VisionFunction> Cfg Param: <isSupport> = 0.");
        return 0;
    }

    int rslt = GetAttribute(pobjVisionFuncCfg, "isUseApi",  stVisionFuncParam.isUseApi);
    if (rslt == 0) {
        ST_LOG_INFO("<VisionFunction> Cfg Param: <isUseApi> = %d .", stVisionFuncParam.isUseApi);
    }

    rslt += ReadVisionQRCodeDetCfg(pobjVisionFuncCfg, stVisionFuncParam.stQRCode);
    rslt += ReadVisionTrafficLightDetCfg(pobjVisionFuncCfg, stVisionFuncParam.stTrafficLightDet);
    rslt += ReadVisionTrashBoxDetCfg(pobjVisionFuncCfg, stVisionFuncParam.stTrashBoxDet);
    rslt += ReadVisionCleanClassDetCfg(pobjVisionFuncCfg, stVisionFuncParam.stCleanClassDet);
    rslt += ReadVisionNetDetCfg(   pobjVisionFuncCfg, stVisionFuncParam.stNetDet);
    rslt += ReadVisionNetSegCfg(   pobjVisionFuncCfg, stVisionFuncParam.stNetSeg);
    printf("\n");

    /* 如果检测和分割同时支持“水管”类别，则关闭检测中的“水管”类别 */
    if (IsSupportVisionLabel(stVisionFuncParam.stNetDet.vValidLabel, TAG_PIPELINE) &&
        IsSupportVisionLabel(stVisionFuncParam.stNetSeg.vValidLabel, TAG_PIPELINE)) {
        DeleteVisionLabel(stVisionFuncParam.stNetDet.vValidLabel, TAG_PIPELINE);
    }

    if (rslt != 0) {
        ST_LOG_ERR("Read Vision Function Config Fail.");
        return -1;
    }

    return 0;
}

}

}

