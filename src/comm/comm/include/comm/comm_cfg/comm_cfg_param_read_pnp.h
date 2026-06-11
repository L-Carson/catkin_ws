#ifndef __COMM_CFG_PARAM_READ_PNP_H__
#define __COMM_CFG_PARAM_READ_PNP_H__

#include "comm_cfg_param_define.h"
#include "comm_cfg_param_read_base.h"

#include "comm/comm_base.h"
#include "comm/loc_xml/tinyxml.h"


namespace COMM
{

namespace CFG
{

/**************************************************************************************
功能描述: 感知配置参数读取
修改记录:
**************************************************************************************/
class CFG_PARAM_READ_PNP_C : public CFG_PARAM_READ_BASE_C
{
public:
    INT32 ReadPnpCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PNP_STRU &stPnpParam);
    INT32 ReadVisionFuncCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_FUNC_STRU &stVisionFuncParam);

private:
    /**************************************************************************************
    功能描述: 读取融合感知算法参数
    修改记录:
    **************************************************************************************/
    INT32 ReadCarBodyVxlFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CAR_BODY_VXL_FILTER_STRU &stCarBodyVxlFilter);
    INT32 ReadGroundSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GROUND_SEG_STRU &stGroundSeg);
    INT32 ReadPnpRegionCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PCPT_REGION_STRU &stPcptRegion);
    INT32 Read2dObstSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LAYER_HEIGHT_STRU& stLayerHeight);
    INT32 ReadMultiframeCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_MUTLTIFRAME_STRU& stMultiframe);
    INT32 ReadDoubleLidarReflectVerifyCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_DOUBLE_LIDAR_VERIFY_STRU &stReflectVerify);
    INT32 ReadEmergenceReflectVerifyCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_EMERGENCE_VERIFY_STRU &stReflectVerify);
    INT32 ReadRoadmarkVerifyCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_ROADMARK_VERIFY_STRU &stReflectVerify);
    INT32 ReadReflectVerifyCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_REFLECT_VERIFY_STRU &stReflectVerify);
    INT32 ReadShortObstHoldCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SHORT_OBST_HOLD_STRU &stShortObstHold);
    INT32 ReadCommonObstHoldCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_COMMON_OBST_HOLD_STRU &stCommonObstHold);
    INT32 ReadPlaneSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_PLANE_SEG_STRU &stPlaneSeg);
    INT32 ReadClusterRegionCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_REGION_STRU &stRegion);
    INT32 ReadClusterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CLUSTER_STRU &stCluster);
    INT32 ReadNetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_NET_STRU &stNet);
    INT32 ReadDetectPileCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_DETECT_PILE_STRU &stDetectPile);
    INT32 ReadObstStateCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_OBST_STATE_STRU &stObstState);
    INT32 ReadTrafficLightCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_TRAFFIC_LIGHT_STRU &stTrafficLight);
    INT32 ReadAccessibleAreaCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ACCESSIBLE_AREA_STRU &stAccArea);
    INT32 ReadSlopePadDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SLOPE_PAD_DET_STRU &stSlopePadDet);
    INT32 ReadRoadRegionFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ROAD_REGION_FILTER_STRU &stRoadRegionFilter);
    INT32 ReadFilterPointsRebuildCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FILTER_POINTS_REBUILD_STRU &stFilterPointsRebuild);
    INT32 ReadWarehouseCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_WAREHOUSE_STRU &stWarehouse);
    INT32 ReadCarBodyDenoisingCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CAR_BODY_DENOSING_STRU &stCarBodyDenoising);
    INT32 ReadCleanlinessCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_CLEANLINESS_STRU &stCleanliness);

    /**************************************************************************************
    功能描述: 读取视觉算法参数
    修改记录:
    **************************************************************************************/
    BOOL  IsCameraValid(string strCamera, vector<string>& vstrCamera) const;
    string CamerasId2Str(const vector<string>& vstrCamera) const;
    BOOL  IsSupportVisionLabel(const vector<LABEL_ENUM>& vValidLabel, const LABEL_ENUM label) const;
    void  DeleteVisionLabel(vector<LABEL_ENUM>& vValidLabel, LABEL_ENUM label);
    INT32 ReadVisionFuncBaseCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_VISION_BASE_STRU& stParam);
    INT32 ReadVisionLabelCfg(TiXmlElement *pobjCfgParam, const char *element, LABEL_ENUM enLabel, vector<LABEL_ENUM>& vValidLabel);
    INT32 ReadVisionLabelThickPipeCfg(TiXmlElement *pobjCfgParam, const char *element, CFG_PARAM_LABEL_THICKPIPE_STRU& stParam);
    INT32 ReadVisionQRCodeDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_BASE_STRU& stQRCode);
    INT32 ReadVisionNetDetCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_NET_DET_STRU& stNetFunc);
    INT32 ReadVisionNetSegCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VISION_NET_SEG_STRU& stNetFunc);
};


}

}

#endif

