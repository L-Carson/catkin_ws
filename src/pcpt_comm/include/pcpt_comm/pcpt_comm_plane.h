#ifndef __PCPT_COMM_PLANE_H__
#define __PCPT_COMM_PLANE_H__

#include "comm/comm_base.h"
#include "comm/comm_dbg_log.h"
#include "pcpt_comm/pcpt_comm_fitting.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <Eigen/Dense>

using namespace std;

/**************************************************************************************
功能描述: 平面拟合
修改记录:
**************************************************************************************/
class PCPT_PLANE_C
{
public:
    PCPT_PLANE_C(){}
    ~PCPT_PLANE_C(){}

    template<typename T>
    int FittingPlaneParam(const T &pCloud, const float disThresh, PLANE_PARAM_STRU &stPlane) const {
        return objPlanePcl_.FittingPlaneParam(pCloud, disThresh, stPlane);
    }

    template<typename T>
    float GetDistByPlane(const T &point, const PLANE_PARAM_STRU &stPlane) const {
        return GetDistByPlane(point.x, point.y, point.z, stPlane);
    }

    template<typename T>
    float GetDirDistByPlaneX(const T &point, const PLANE_PARAM_STRU &stPlane) const {
        float dist = GetDistByPlane(point, stPlane);
        return 1.0 * SIGN(stPlane.coeffX) * dist;   //设定平面前为正，平面后为负
    }

    template<typename T>
    float GetDirDistByPlaneY(const T &point, const PLANE_PARAM_STRU &stPlane) const {
        float dist = GetDistByPlane(point, stPlane);
        return 1.0 * SIGN(stPlane.coeffY) * dist;   //设定平面左为正，平面右为负
    }

    template<typename T>
    float GetDirDistByPlaneZ(const T &point, const PLANE_PARAM_STRU &stPlane) const {
        float dist = GetDistByPlane(point, stPlane);
        return 1.0 * SIGN(stPlane.coeffZ) * dist;   //设定平面上为正，平面下为负
    }

    float GetDistByPlane(float x, float y, float z, const PLANE_PARAM_STRU &stPlane) const;
    float GetVectorIncludedAngle(const PCPT_VECTOR_STRU &A, const PCPT_VECTOR_STRU &B) const;
    float GetPlaneIncludedAngle(const PLANE_PARAM_STRU &stPlaneA, const PLANE_PARAM_STRU &stPlaneB) const;
    float GetPlaneIncludedAngleByAxisX(const PLANE_PARAM_STRU &stPlane) const;
    float GetPlaneIncludedAngleByAxisY(const PLANE_PARAM_STRU &stPlane) const;
    float GetPlaneIncludedAngleByAxisZ(const PLANE_PARAM_STRU &stPlane) const;

    void  GetVectorRotMatrix(const PCPT_VECTOR_STRU &stBef, const PCPT_VECTOR_STRU &stAft, Eigen::Matrix3d& rotMatrix) const;
    void  GetVectorEulerAngle(const PCPT_VECTOR_STRU &stBef, const PCPT_VECTOR_STRU &stAft, Eigen::Vector3d& eulerAngle) const;
    void  GetPlaneRPY(const PLANE_PARAM_STRU &stPlane, ATTITUDE_STRU& stAttitude) const;
    void  PlaneParamNormalization(PLANE_PARAM_STRU &stPlane) const;

private:
    PCL_PLANE_FITTER_C objPlanePcl_;    // 基于PCL的平面拟合器
};

/**************************************************************************************
功能描述: 地面拟合，只支持PCL点云格式
修改记录: 根据区间设置自动拟合多个子平面
**************************************************************************************/
template<class T>
class PCPT_MULTI_PLANE_C : public PCPT_PLANE_C
{
public:
    PCPT_MULTI_PLANE_C() : PCPT_PLANE_C(), subRectSize_(0) {}
    ~PCPT_MULTI_PLANE_C() {}

    int SetRectangle(RECTANGLE_STRU& stRectangle, float unitX, float unitY);
    int FittingPlaneParam(T& pCloud, int recursionTimes, vector<PLANE_PARAM_STRU>& vstPlane);

private:
    T                       vpCloud_;
    uint32_t                subRectSize_;
    RECTANGLE_STRU          stRectangle_;
    vector<RECTANGLE_STRU>  vstSubRect_;
};

#endif

