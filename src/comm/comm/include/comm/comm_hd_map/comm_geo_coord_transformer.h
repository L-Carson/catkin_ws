#ifndef __COMM_GEO_COORD_TRANSFORMER_H__
#define __COMM_GEO_COORD_TRANSFORMER_H__

#include "comm/comm_base.h"
#include "comm/loc_geocentric/LocalCartesian.hpp"

class GEO_COORD_TRANSFORMER_C
{
public:
    /**
     * @brief 构造函数
     */
    GEO_COORD_TRANSFORMER_C(void) : isSetOrigin_(false)
    {
    
    };

    /**
     * @brief 从场景地图加载地图原点经纬高坐标
     * @param[in] strWorkSceneDir 场景地图目录
     * @return 0：成功， -1：失败
     */
    INT32 LoadOrigin(const std::string &strWorkSceneDir);

    /**
     * @brief 保存地图原点经纬高坐标到场景地图
     * @param[in] strWorkSceneDir 场景地图目录
     * @return 0：成功， -1：失败
     */
    INT32 SaveOrigin(const std::string &strWorkSceneDir);

    /**
     * @brief 设置地图原点经纬高
     * @param[in] stOrigin 地图原点经纬高
     * @return 0：成功， -1：失败
     */
    void  SetOrigin(const GPS_COORD_STRU &stOrigin);

    /**
     * @brief 获取地图原点经纬高
     * @param[in] stOrigin 地图原点经纬高
     * @return 0：成功， -1：失败
     */
    INT32 GetOrigin(GPS_COORD_STRU &stOrigin); 

    /**
     * @brief 判断是否已设置地图原点经纬高
     * @return true：已设置， false: 未设置
     */
    BOOL  IsSetOrigin(void) { return isSetOrigin_;};

    /**
     * @brief 经纬高到局部坐标的转换
     * @param[in]  stGlobalPos 经纬高坐标
     * @param[out] stLocalPos  笛卡尔坐标
     * @return 0：成功， -1：失败
     */
    INT32 ToCartersian(const GPS_COORD_STRU &stGlobalPos,   COORDINATE_3D_STRU &stLocalPos);

    /**
     * @brief 局部坐标到经纬高的转换
     * @param[in]  stLocalPos  笛卡尔坐标
     * @param[out] stGlobalPos 经纬高坐标
     * @return 0：成功， -1：失败
     */
    INT32 ToLonLatAlt(const COORDINATE_3D_STRU &stLocalPos, GPS_COORD_STRU &stGlobalPos);
    
private:
    BOOL            isSetOrigin_;     
    GPS_COORD_STRU  stOrigin_;
    GeographicLib::LocalCartesian objConverter_;
};

#endif

