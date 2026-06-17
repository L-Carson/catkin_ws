#include "pcpt_comm/pcpt_comm_iou.h"

/**************************************************************************************
功能描述: 计算任意平面多边形的有向面积
修改记录: 顺时针时面积小于0，逆时针时面积大于0
**************************************************************************************/
double PCPT_IOU_C::PloygonDirectArea(const vector<POINT_COORD_2D_STRU> &vstPoint) const
{
    return DirectedArea(vstPoint);
}

double PCPT_IOU_C::PloygonDirectArea(const vector<COORDINATE_2D_STRU> &vstPoint) const
{
    vector<POINT_COORD_2D_STRU> polygon;

    for (const COORDINATE_2D_STRU &coor : vstPoint) {
        POINT_COORD_2D_STRU point;
        point.x = coor.x;
        point.y = coor.y;
        polygon.push_back(point);
    }

    return DirectedArea(polygon);
}

/**************************************************************************************
功能描述: 计算任意平面多边形的面积
修改记录:
**************************************************************************************/
double PCPT_IOU_C::PloygonArea(const vector<COORDINATE_2D_STRU>& vstPoint) const
{
    double area = PloygonDirectArea(vstPoint);
    return (area < 0 ? -area : area);
}

/**************************************************************************************
功能描述: 计算矩形框的IOU
修改记录:
**************************************************************************************/
double PCPT_IOU_C::IoUOfRectangle(RECTANGLE_STRU &stRect1, RECTANGLE_STRU &stRect2) const
{
    double minX = MAX(stRect1.minX, stRect2.minX);
    double minY = MAX(stRect1.minY, stRect2.minY);
    double maxX = MIN(stRect1.maxX, stRect2.maxX);
    double maxY = MIN(stRect1.maxY, stRect2.maxY);

    double overlapArea = MAX(0, maxX - minX) * MAX(0, maxY - minY);

    double unionArea = stRect1.Area() + stRect2.Area() - overlapArea;

    return overlapArea / unionArea;
}

/**************************************************************************************
功能描述: 计算任意平面多边形的IOU
修改记录:
**************************************************************************************/
double PCPT_IOU_C::IoUOfPolygon(const vector<COORDINATE_2D_STRU>& vstCoor1, const vector<COORDINATE_2D_STRU>& vstCoor2) const
{
    vector<POINT_COORD_2D_STRU> polygon1;
    vector<POINT_COORD_2D_STRU> polygon2;

    for (const COORDINATE_2D_STRU &stCoor : vstCoor1) polygon1.push_back(POINT_COORD_2D_STRU(stCoor));
    for (const COORDINATE_2D_STRU &stCoor : vstCoor2) polygon2.push_back(POINT_COORD_2D_STRU(stCoor));

    return IoUPolygon(polygon1, polygon2);
}

/**************************************************************************************
功能描述: 计算任意平面多边形的有向面积，坐标点逆时针排序时面积为正
修改记录:
**************************************************************************************/
double PCPT_IOU_C::DirectedArea(POINT_COORD_2D_STRU* pPolygon, int pointNum) const
{
    vector<POINT_COORD_2D_STRU> polygon(pPolygon, pPolygon + pointNum);
    return DirectedArea(polygon);
}

double PCPT_IOU_C::DirectedArea(const vector<POINT_COORD_2D_STRU>& polygon) const
{
    double area = 0;
    for (size_t i = 0; i < polygon.size(); ++i) {
        size_t j = (i + 1) % polygon.size();
        area += polygon[i].x * polygon[j].y - polygon[i].y * polygon[j].x;
    }

    return area * 0.5;
}

/**************************************************************************************
功能描述: 计算叉积 OA x OB
修改记录:
**************************************************************************************/
double PCPT_IOU_C::CrossProduct(const POINT_COORD_2D_STRU& stPointO, const POINT_COORD_2D_STRU& stPointA, const POINT_COORD_2D_STRU& stPointB) const
{
    return (stPointA.x - stPointO.x) * (stPointB.y - stPointO.y) -
           (stPointB.x - stPointO.x) * (stPointA.y - stPointO.y);
}

double PCPT_IOU_C::CrossProduct(const COORDINATE_2D_STRU& stPointO, const COORDINATE_2D_STRU& stPointA, const COORDINATE_2D_STRU& stPointB) const
{
    return CrossProduct(POINT_COORD_2D_STRU(stPointO), POINT_COORD_2D_STRU(stPointA), POINT_COORD_2D_STRU(stPointB));
}

/**************************************************************************************
功能描述: 交叉线
修改记录:
**************************************************************************************/
int PCPT_IOU_C::LineCross(const double s1, const double s2, const POINT_COORD_2D_STRU& pointC, const POINT_COORD_2D_STRU& pointD, POINT_COORD_2D_STRU& pointT) const
{
    //if (SIG(s1) == 0 && SIG(s2) == 0) return -1;
    if (SIG(s2 - s1) == 0) return -1;

    pointT.x = (pointC.x * s2 - pointD.x * s1) / (s2 - s1);
    pointT.y = (pointC.y * s2 - pointD.y * s1) / (s2 - s1);

    return 0;
}

/**************************************************************************************
功能描述: 多边形切割
           用直线AB切割多边形polygon，切割后的在向量AB的左侧，并原地保存切割结果。
           如果退化为一个点，也会返回去，此时向量polygon长度为1。
修改记录:
**************************************************************************************/
void PCPT_IOU_C::PolygonCut(vector<POINT_COORD_2D_STRU>& polygon, const POINT_COORD_2D_STRU& pointA, const POINT_COORD_2D_STRU& pointB) const
{
    polygon.push_back(polygon.front());

    vector<POINT_COORD_2D_STRU> vstPointBuffer;

    for (size_t i = 0; i < polygon.size()-1; i++) {
        double crossProductCur  = CrossProduct(pointA, pointB, polygon[i]);
        double crossProductNext = CrossProduct(pointA, pointB, polygon[i+1]);

        if (SIG(crossProductCur) > 0) vstPointBuffer.push_back(polygon[i]);

        if (SIG(crossProductCur) != SIG(crossProductNext)) {
            POINT_COORD_2D_STRU pointT;
            if (!LineCross(crossProductCur, crossProductNext, polygon[i], polygon[i+1], pointT))
                vstPointBuffer.push_back(pointT);
        }
    }

    polygon.clear();
    for (size_t i = 0; i < vstPointBuffer.size(); i++) {
        if (!i || !(vstPointBuffer[i] == vstPointBuffer[i-1]))
            polygon.push_back(vstPointBuffer[i]);
    }

    while (polygon.size() > 1 && polygon.back() == polygon.front()) {
        polygon.erase(polygon.end() - 1);
    }
}

/**************************************************************************************
功能描述: 返回三角形OAB和三角形OCD的有向交面积, O是原点
修改记录:
**************************************************************************************/
double PCPT_IOU_C::IntersectArea(POINT_COORD_2D_STRU pointA, POINT_COORD_2D_STRU pointB, POINT_COORD_2D_STRU pointC, POINT_COORD_2D_STRU pointD) const
{
    POINT_COORD_2D_STRU pointO(0, 0);
    int s1 = SIG(CrossProduct(pointO, pointA, pointB));
    int s2 = SIG(CrossProduct(pointO, pointC, pointD));

    if (s1 == 0 || s2 == 0) return 0.0;     //退化，面积为0
    if (s1 == -1) swap(pointA, pointB);
    if (s2 == -1) swap(pointC, pointD);

    vector<POINT_COORD_2D_STRU> vstPoint{pointO, pointA, pointB};

    PolygonCut(vstPoint, pointO, pointC);
    PolygonCut(vstPoint, pointC, pointD);
    PolygonCut(vstPoint, pointD, pointO);

    double res = fabs(DirectedArea(vstPoint));
    if (s1 * s2 == -1) res = -res;

    return res;
}

/**************************************************************************************
功能描述: 求两多边形的交面积
修改记录:
**************************************************************************************/
double PCPT_IOU_C::IntersectArea(vector<POINT_COORD_2D_STRU>& polygon1, vector<POINT_COORD_2D_STRU>& polygon2) const
{
    if (DirectedArea(polygon1) < 0) reverse(polygon1.begin(), polygon1.end());
    if (DirectedArea(polygon2) < 0) reverse(polygon2.begin(), polygon2.end());

    double res = 0;

    for (size_t i = 0; i < polygon1.size(); ++i) {
        size_t j = (i + 1) % polygon1.size();
        for (size_t m = 0; m < polygon2.size(); ++m) {
            size_t n = (m + 1) % polygon2.size();
            res += IntersectArea(polygon1[i], polygon1[j], polygon2[m], polygon2[n]);
        }
    }

    return res;     //assume res is positive
}

/**************************************************************************************
功能描述: 计算多边形IoU
修改记录:
**************************************************************************************/
double PCPT_IOU_C::IoUPolygon(vector<POINT_COORD_2D_STRU>& polygon1, vector<POINT_COORD_2D_STRU>& polygon2) const
{
    double interArea = IntersectArea(polygon1, polygon2);
    double unionArea = fabs(DirectedArea(polygon1)) + fabs(DirectedArea(polygon2)) - interArea;
    double iou = interArea / unionArea;
    return iou;
}
