#ifndef __PCPT_COMM_POLYGON_H__
#define __PCPT_COMM_POLYGON_H__

#include "comm/comm_base.h"
#include "comm/comm_geometry.h"
#include "pcpt_comm/pcpt_comm_iou.h"
#include "pcpt_comm/pcpt_comm_define.h"
#include "pcpt_comm/pcpt_comm_curve.h"

#include <cmath>
#include <algorithm>
#include <stack>

using namespace std;

/**************************************************************************************
功能描述: 任意多边形
修改记录:
**************************************************************************************/
class PCPT_POLYGON_C
{
public:
    PCPT_POLYGON_C() {}
    PCPT_POLYGON_C(const std::vector<COORDINATE_2D_STRU> &polygon) : objAnyPolygon_(polygon) {}
    ~PCPT_POLYGON_C(){}

    bool IsValid(void) const;
    int  Reset(const std::vector<COORDINATE_2D_STRU> &vectVertice);

    void PrunePolygon(vector<COORDINATE_2D_STRU> &polygon) const;
    bool IsValidPolygon(const vector<COORDINATE_2D_STRU> &polygon) const;

    bool IsPointInPolygon(const COORDINATE_2D_STRU &stPoint) const;
    bool IsPointInPolygon(const vector<COORDINATE_2D_STRU> &polygon, const COORDINATE_2D_STRU &stPoint) const;

    double PolygonArea(const COORDINATE_2D_STRU polygon[], int pointNum) const;
    double PolygonArea(const vector<COORDINATE_2D_STRU> &polygon) const;

    double IoUOfPolygon(const vector<COORDINATE_2D_STRU> &polygonA, const vector<COORDINATE_2D_STRU> &polygonB) const;

    int PolygonZoom(vector<COORDINATE_2D_STRU> &polygon, const float zoomDist) const;
    int PolygonZoom(list<COORDINATE_2D_STRU> &lstPoint, const float zoomDist) const;

    double PointToPolygonDistance(const vector<COORDINATE_2D_STRU> &polygon, const COORDINATE_2D_STRU& point) const;

    int PolygonToMultiConvexHull(const vector<COORDINATE_2D_STRU>& polygon, vector<vector<COORDINATE_2D_STRU>>& vConvexHull) const;

private:
    ANY_POLYGON_C       objAnyPolygon_;
    PCPT_IOU_C          objIoU_;
    CONVEX_HULL_C       objConvex_;
    PCPT_CURVE_C        objCurve_;

    double DotProduct(const COORDINATE_2D_STRU& a, const COORDINATE_2D_STRU& b) const;
    double CrossProduct(const POINT_COORD_2D_STRU& stPointO, const POINT_COORD_2D_STRU& stPointA, const POINT_COORD_2D_STRU& stPointB) const;
    double CrossProduct(const COORDINATE_2D_STRU& stPointO, const COORDINATE_2D_STRU& stPointA, const COORDINATE_2D_STRU& stPointB) const;

    bool IsCross(const COORDINATE_2D_STRU& line1A, const COORDINATE_2D_STRU& line1B, const COORDINATE_2D_STRU& line2A, const COORDINATE_2D_STRU& line2B) const;

    bool IsPointOnLine(const double px0, const double py0, const double px1, const double py1, const double px2, const double py2) const;
    bool IsIntersect(const double px1, const double py1, const double px2, const double py2, const double px3, const double py3, const double px4, const double py4) const;
    bool IsPointInPolygonTest(const vector<POINT_COORD_2D_STRU> &vstPoint, const POINT_COORD_2D_STRU& stPoint) const;
};

#endif

