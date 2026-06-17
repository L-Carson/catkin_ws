#ifndef __PCPT_COMM_IOU_H__
#define __PCPT_COMM_IOU_H__

#include "comm/comm_base.h"
#include "comm/comm_data_type.h"
#include "pcpt_comm/pcpt_comm_base.h"

using namespace std;

/**************************************************************************************
功能描述: IoU(交并比)
修改记录:
**************************************************************************************/
class PCPT_IOU_C
{
public:
    PCPT_IOU_C() {}
    ~PCPT_IOU_C() {}

    double PloygonDirectArea(const vector<POINT_COORD_2D_STRU> &vstPoint) const;
    double PloygonDirectArea(const vector<COORDINATE_2D_STRU> &vstPoint) const;
    double PloygonArea(const vector<COORDINATE_2D_STRU> &vstPoint) const;
    double IoUOfRectangle(RECTANGLE_STRU &stRect1, RECTANGLE_STRU &stRect2) const;
    double IoUOfPolygon(const vector<COORDINATE_2D_STRU>& vstCoor1, const vector<COORDINATE_2D_STRU>& vstCoor2) const;
    double CrossProduct(const POINT_COORD_2D_STRU& stPointO, const POINT_COORD_2D_STRU& stPointA, const POINT_COORD_2D_STRU& stPointB) const ;
    double CrossProduct(const COORDINATE_2D_STRU& stPointO,  const COORDINATE_2D_STRU& stPointA,  const COORDINATE_2D_STRU& stPointB) const ;

private:
    double DirectedArea(POINT_COORD_2D_STRU* pPolygon, int pointNum) const;
    double DirectedArea(const vector<POINT_COORD_2D_STRU>& polygon) const;
    int    LineCross(const double s1, const double s2, const POINT_COORD_2D_STRU& pointC, const POINT_COORD_2D_STRU& pointD, POINT_COORD_2D_STRU& pointT) const ;
    void   PolygonCut(vector<POINT_COORD_2D_STRU>& polygon, const POINT_COORD_2D_STRU& pointA, const POINT_COORD_2D_STRU& pointB) const;
    double IntersectArea(POINT_COORD_2D_STRU pointA, POINT_COORD_2D_STRU pointB, POINT_COORD_2D_STRU pointC, POINT_COORD_2D_STRU pointD) const;
    double IntersectArea(vector<POINT_COORD_2D_STRU>& polygon1, vector<POINT_COORD_2D_STRU>& polygon2) const;
    double IoUPolygon(vector<POINT_COORD_2D_STRU>& poly1, vector<POINT_COORD_2D_STRU>& poly2) const;
};


#endif

