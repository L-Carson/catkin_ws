#pragma once

#include "comm/comm_base.h"
#include <Eigen/Dense>
#include "pcpt_comm/pcpt_comm_define.h"

/**
 * @brief 曲线拟合器
 *
 */
class PCPT_CURVE_C
{
public:
    PCPT_CURVE_C(){}
    ~PCPT_CURVE_C(){}

    int FitMultiOrderCurve( const vector<COORDINATE_2D_STRU>& vstPos, const uint32_t order, vector<double>& vCoeff) const;
    int FitMultiOrderCurveE(const vector<COORDINATE_2D_STRU>& vstPos, const uint32_t order, vector<double>& vCoeff) const;

    int FitCircle(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const;

    double GetMultiOrderCurveSlope(const double x, vector<double>& vCoeff) const;
    double GetMultiOrderCurveY(const double x, vector<double>& vCoeff) const;

    double GetCircleSlope(const double x, double y, vector<double>& vCoeff) const;

    void GetLineEndPointsByCoeff(const float coeffA, const float coeffB, COORDINATE_2D_STRU& lineStart, COORDINATE_2D_STRU& lineEnd) const;
    COORDINATE_2D_STRU PointToLineFootPoint(const COORDINATE_2D_STRU& point, const COORDINATE_2D_STRU& lineStart, const COORDINATE_2D_STRU& lineEnd) const;
    COORDINATE_2D_STRU PointToLineFootPoint(const COORDINATE_2D_STRU& point,  const float coeffA, const float coeffB) const;
    double PointToLineDistance(const COORDINATE_2D_STRU& point, const COORDINATE_2D_STRU& lineStart, const COORDINATE_2D_STRU& lineEnd) const;
    double PointToLineDistance(const COORDINATE_2D_STRU& point, const float coeffA, const float coeffB) const;
    double PointToLineSegmentDistance(const COORDINATE_2D_STRU& point, const COORDINATE_2D_STRU& LineStart, const COORDINATE_2D_STRU& LineEnd) const;

private:
    void Fit1(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const;
    void Fit2(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const;
    void Fit3(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const;
    void Fit4(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const;
    void Fit5(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const;
};
