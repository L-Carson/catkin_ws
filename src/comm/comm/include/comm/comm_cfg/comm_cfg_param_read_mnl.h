#ifndef __COMM_CFG_PARAM_READ_MNL_H__
#define __COMM_CFG_PARAM_READ_MNL_H__

#include "comm_cfg_param_define.h"
#include "comm_cfg_param_read_base.h"
#include "comm/comm_base.h"
#include "comm/loc_xml/tinyxml.h"


namespace COMM
{

namespace CFG
{

/**************************************************************************************
功能描述: 定位配置参数读取
修改记录:
**************************************************************************************/
class CFG_PARAM_READ_MNL_C : public CFG_PARAM_READ_BASE_C
{
public:
    INT32 ReadLocalizationCfg(TiXmlElement  *pobjCfgParam, CFG_PARAM_LOC_STRU &stLocCfg);
    INT32 ReadVehicleLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VEHICLE_LOC_STRU &stVehicleLocCfg);
    INT32 ReadMappingCfg(TiXmlElement     *pobjCfgParam, CFG_PARAM_MAPPING_STRU      &stMappingCfg);

private:
    INT32 ReadPoseEstimatorCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_POSE_ESTIMATOR_STRU &stPoseEstimator);
    INT32 ReadLidarLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_STRU &stLidarLocCfg);
    INT32 ReadNdtMatcherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_NDT_MATCHER_STRU &stNdtMatcherCfg);
    INT32 ReadLocLidarListCfg(TiXmlElement *pobjCfgParam, std::vector<std::string> &vstrLocLidars);
    INT32 ReadReLocalizationCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RELOC_STRU &stReLocCfg);
    INT32 ReadRawReLocalizationCfg(TiXmlElement *pobjCfgParam,  CFG_PARAM_RAW_RELOC_STRU  &stRawReLocCfg);
    INT32 ReadFineReLocalizationCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FINE_RELOC_STRU &stFineReLocCfg);
    INT32 ReadScanContextCfg(TiXmlElement *pobjCfgParam,  CFG_PARAM_SCAN_CONTEXT_STRU &stScanContextCfg);
    INT32 ReadSearchRegionCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SEARCH_REG_STRU   &stSearchRegCfg);
    INT32 ReadSearchStepCfg(TiXmlElement *pobjCfgParam,   CFG_PARAM_SEARCH_STEP_STRU  &stSearchStepCfg);

    INT32 ReadRelocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_RELOC_V3_STRU &stReLocCfg);
    INT32 ReadCoarseRelocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_COARSE_RELOC_STRU &stCoarseReLocCfg);
    INT32 ReadFineRelocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_FINE_RELOC_V3_STRU &stFineReLocCfg);
    INT32 ReadVoxelMatchCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VOXEL_MATCH_STRU &stVoxelMatchCfg);
    INT32 ReadRegistererCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_REGISTERER_STRU &stRegistererCfg);
   
    INT32 ReadNdtCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_NDT_STRU &stNdtCfg);
    INT32 ReadMixNdtCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_MIXNDT_STRU &stMixNdtCfg);
    INT32 ReadCombLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_COMB_LOC_STRU &stCombLocCfg);
    INT32 ReadGnssLocCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_LOC_STRU &stGnssLocCfg);
    INT32 ReadGnssLocGeneralCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_LOC_GENERAL_STRU &stGnssLocGeneralCfg);
    INT32 ReadEskfCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ESKF_STRU &stEskfCfg);
    INT32 ReadGnssLocCustomizedCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_GNSS_LOC_CUSTOMIZED_STRU &stGnssLocCustomizedCfg);
    INT32 ReadLidarLocV3Cfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_V3_STRU &stLidarLocCfg);
    INT32 ReadLidarLocV4Cfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_V3_STRU &stLidarLocCfg);
    INT32 ReadLidarLocLioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_LIO_STRU &stLidarLocLioCfg);
    INT32 ReadLioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIO_STRU &stLioCfg);
    INT32 ReadLidarTopicListCfg(TiXmlElement *pobjCfgParam, std::vector<std::string> &vstrLidarTopicList);
    INT32 ReadSmootherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SMOOTHER_STRU &stSmootherCfg);
    INT32 ReadLidarLocWioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_WIO_STRU &stLidarLocWioCfg);
    INT32 ReadLidarLocWioCfgV1(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_WIO_STRU &stLidarLocWioCfg);
    INT32 ReadDataProcessCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIDAR_LOC_DATA_PROCESS_STRU &stDataProcCfg);
    INT32 ReadWioCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_WIO_STRU &stWioCfg);
    INT32 ReadScanMatcherCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SCAN_MATCHER_STRU &stScanMatcherCfg);
    INT32 ReadScanMatcherCfgV1(TiXmlElement *pobjCfgParam, CFG_PARAM_SCAN_MATCHER_STRU &stScanMatcherCfg);
    INT32 ReadActiveMapCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ACTIVE_MAP_STRU &stActMapCfg);
    INT32 ReadDsFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_DOWN_SAMPLE_FILTER_STRU &stDsfCfg);
    INT32 ReadVoxelGridFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_VOXEL_GRID_FILTER_STRU &stVgfCfg);
    INT32 ReadAdaptiveVoxelGridFilterCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_ADAPTIVE_VOXEL_GRID_FILTER_STRU &stAvgfCfg);

    INT32 ReadOmpNdtCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_OMPNDT_STRU &stOmpNdtCfg);
    INT32 ReadSmallGicpCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_SMALL_GICP_STRU &stSmallGicpCfg);
    INT32 ReadDataProcCfg(TiXmlElement    *pobjCfgParam, CFG_PARAM_DATAPROC_STRU     &stDataProcCfg);
    INT32 ReadFrontEndCfg(TiXmlElement    *pobjCfgParam, CFG_PARAM_FRONTEND_STRU     &stFrontEndCfg);
    INT32 ReadBackEndCfg(TiXmlElement     *pobjCfgParam, CFG_PARAM_BACKEND_STRU      &stBackEndCfg);
    INT32 ReadLoopCloseCfg(TiXmlElement   *pobjCfgParam, CFG_PARAM_LOOPCLOSE_STRU    &stLoopCloseCfg);
    INT32 ReadLOFrontEndCfg(TiXmlElement  *pobjCfgParam, CFG_PARAM_LO_FRONTEND_STRU  &stLoFrontEndCfg);
    INT32 ReadLIOFrontEndCfg(TiXmlElement *pobjCfgParam, CFG_PARAM_LIO_FRONTEND_STRU &stLioFrontEndCfg);
    INT32 ReadMatcherCfg(TiXmlElement     *pobjCfgParam, CFG_PARAM_MATCH_STRU        &stMatherCfg);
    INT32 ReadLOFactorCfg(TiXmlElement    *pobjCfgParam, CFG_PARAM_LO_FACTOR_STRU    &stLoFactorCfg);
    INT32 ReadLCFactorCfg(TiXmlElement    *pobjCfgParam, CFG_PARAM_LC_FACTOR_STRU    &stLcFactorCfg);
    INT32 ReadGnssFactorCfg(TiXmlElement  *pobjCfgParam, CFG_PARAM_GNSS_FACTOR_STRU  &stGnssFactorCfg);
};

}

}

#endif

