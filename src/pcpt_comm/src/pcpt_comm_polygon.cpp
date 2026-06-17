#include "pcpt_comm/pcpt_comm_polygon.h"

/**************************************************************************************
功能描述: 是否有效
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsValid(void) const
{
    return objAnyPolygon_.IsValid();
}

/**************************************************************************************
功能描述: 重置多边形
修改记录:
**************************************************************************************/
int PCPT_POLYGON_C::Reset(const std::vector<COORDINATE_2D_STRU> &vectVertice)
{
    if (vectVertice.size() < 3) return -1;
    return objAnyPolygon_.Reset(vectVertice);
}

/**************************************************************************************
功能描述: 修整多边形，删除相等的相邻点
修改记录:
**************************************************************************************/
void PCPT_POLYGON_C::PrunePolygon(vector<COORDINATE_2D_STRU> &polygon) const
{
    for (size_t i = 0; i < polygon.size();) {
        size_t j = i + 1;
        j %= polygon.size();

        const COORDINATE_2D_STRU& stCoor = polygon[i];
        if (stCoor.IsAlmostEuqal(polygon[j])) {
            auto iter = polygon.begin() + i;
            polygon.erase(iter);
        } else ++i;
    }
}

/**************************************************************************************
功能描述: 是否为有效的多边形
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsValidPolygon(const vector<COORDINATE_2D_STRU> &polygon) const
{
    /* 判断多边形首尾点是否相同，如果相同，删除末尾点 */
    vector<COORDINATE_2D_STRU> vstCoor = polygon;
    bool isEqual = false;
    do {
        const COORDINATE_2D_STRU& stCoorFront = vstCoor.front();
        const COORDINATE_2D_STRU& stCoorBack  = vstCoor.back();
        isEqual = stCoorFront.IsAlmostEuqal(stCoorBack);
        if (isEqual) vstCoor.pop_back();
    } while (isEqual);

    /* 根据点数判断是否为有效的多边形 */
    if (vstCoor.size() < 3) return false;
    if (vstCoor.size() == 3) {
        if (SIG(CrossProduct(polygon[0], polygon[1], polygon[2])) == 0) return false;
        return true;
    }

    /* 遍历所有边，判断是存在交叉（4边形及以上才可以进行交叉判断） */
    for (size_t i = 0; i < vstCoor.size() - 3; ++i) {
        const COORDINATE_2D_STRU& line1A = vstCoor[i];
        const COORDINATE_2D_STRU& line1B = vstCoor[i + 1];
        for (size_t j = i + 2; j < vstCoor.size() - 1; ++j) {
            const COORDINATE_2D_STRU& line2A = vstCoor[j];
            const COORDINATE_2D_STRU& line2B = vstCoor[j + 1];
            if (IsCross(line1A, line1B, line2A, line2B)) return false;
        }
    }

    return true;
}

/**************************************************************************************
功能描述: 点是否在多边形内部
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsPointInPolygon(const COORDINATE_2D_STRU &stPoint) const
{
    return objAnyPolygon_.IsPointIn(stPoint);
}

bool PCPT_POLYGON_C::IsPointInPolygon(const vector<COORDINATE_2D_STRU> &polygon, const COORDINATE_2D_STRU &stPoint) const
{
    ANY_POLYGON_C objAnyPolygon(polygon);
    if (!objAnyPolygon.IsValid()) return false;
    return objAnyPolygon.IsPointIn(stPoint);
}

/**************************************************************************************
功能描述: 计算任意平面多边形面积
修改记录:
**************************************************************************************/
double PCPT_POLYGON_C::PolygonArea(const COORDINATE_2D_STRU polygon[], int pointNum) const
{
    vector<COORDINATE_2D_STRU> vstPoint(polygon, polygon + pointNum);
    return PolygonArea(vstPoint);
}

double PCPT_POLYGON_C::PolygonArea(const vector<COORDINATE_2D_STRU> &polygon) const
{
    return objIoU_.PloygonArea(polygon);
}

/**************************************************************************************
功能描述: 计算任意平面多边形的IoU(交并比)
修改记录:
**************************************************************************************/
double PCPT_POLYGON_C::IoUOfPolygon(const vector<COORDINATE_2D_STRU> &polygonA,
                                    const vector<COORDINATE_2D_STRU> &polygonB) const
{
    double iou = objIoU_.IoUOfPolygon(polygonA, polygonB);
    if (iou < 0) iou = 0;   //由于计算误差，会出现临界情况
    if (iou > 1) iou = 1;
    return iou;
}

/**************************************************************************************
功能描述: 任意多边形缩放，zoomDist > 0 时外扩，zoomDist < 0 时内缩
修改记录:
**************************************************************************************/
int PCPT_POLYGON_C::PolygonZoom(vector<COORDINATE_2D_STRU> &polygon, const float zoomDist) const
{
    if (polygon.size() < 3) {
        ST_LOG_ERR("The number of polygon points entered is %u, not a valid polygon.", uint32_t(polygon.size()));
        return -1;
    }

    /* 按照顺时针排序(有向面积<0)，才能保证zoomDist > 0 时外扩，zoomDist < 0 时内缩 */
    float zoomDistAmend = zoomDist * SIG(-1.0 * objIoU_.PloygonDirectArea(polygon));

    size_t count = polygon.size();
    vector<COORDINATE_2D_STRU> vstPointD;
    for (size_t i = 0; i < count; ++i) {
        int index = (i + 1) % count;
        COORDINATE_2D_STRU stPoint = polygon[index] - polygon[i];
        vstPointD.push_back(stPoint);
    }

    /* 初始化vstPointDN，单位化两顶点向量差 */
    vector<COORDINATE_2D_STRU> vstPointDN;
    for (size_t i = 0; i < count; ++i) {
        COORDINATE_2D_STRU stPoint = vstPointD[i] / sqrt(vstPointD[i] * vstPointD[i]);
        vstPointDN.push_back(stPoint);
    }

    VST_COOR vstPointTmp;
    vstPointTmp.reserve(polygon.size());
    for (size_t i = 0; i < count; ++i) {
        uint32_t startIndex = i==0 ? count-1 : i-1;
        uint32_t endIndex = i;
        double   dotProduct = DotProduct(vstPointDN[startIndex], vstPointDN[endIndex]);
        if (SIG(dotProduct) == 0) continue; //两向量共线
        double lenth = zoomDistAmend / dotProduct;
        COORDINATE_2D_STRU stCoor = polygon[i] + (vstPointDN[endIndex] - vstPointDN[startIndex]) * lenth;
        vstPointTmp.push_back(stCoor);
    }

    swap(polygon, vstPointTmp);

    return 0;
}

int PCPT_POLYGON_C::PolygonZoom(list<COORDINATE_2D_STRU> &polygon, const float zoomDist) const
{
    vector<COORDINATE_2D_STRU> vstPoint;
    vstPoint.insert(vstPoint.end(), polygon.begin(), polygon.end());
    if (PolygonZoom(vstPoint, zoomDist)) return -1;

    polygon.clear();
    polygon.insert(polygon.end(), vstPoint.begin(), vstPoint.end());
    return 0;
}

/**************************************************************************************
功能描述: 点到多边形的距离
修改记录:
**************************************************************************************/
double PCPT_POLYGON_C::PointToPolygonDistance(const vector<COORDINATE_2D_STRU> &polygon, const COORDINATE_2D_STRU& point) const
{
    if (IsPointInPolygon(polygon, point)) return 0;
    double minDist = DBL_MAX;
    int n = polygon.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        double dist = objCurve_.PointToLineSegmentDistance(point, polygon[i], polygon[j]);
        if (minDist > dist) minDist = dist;
    }
    return minDist;
}

/**************************************************************************************
功能描述: 将任意多边形拆分成多个凸多边形
修改记录: 输出结果为逆时针排序的凸多边形向量
**************************************************************************************/
int PCPT_POLYGON_C::PolygonToMultiConvexHull(const vector<COORDINATE_2D_STRU> &polygon, vector<vector<COORDINATE_2D_STRU>> &vConvexHull) const
{
    if (!IsValidPolygon(polygon)) return -1;

    /* 逆时针排列 */
    vector<COORDINATE_2D_STRU> vstCoor = polygon;
    if (objIoU_.PloygonDirectArea(vstCoor) < 0) reverse(vstCoor.begin(), vstCoor.end());
    for (const COORDINATE_2D_STRU& stCoor : vstCoor) ST_LOG_INFO("00 isConvex x:%f y:%f", stCoor.x, stCoor.y);

    /* 首先判断是否已经是凸包 */
    if (objConvex_.IsConvexHull(polygon)) {
        vConvexHull.push_back(polygon);
        return 0;
    }

    /* 存放到队列中，方便处理 */
    deque<COORDINATE_2D_STRU> deqCoor;
    deqCoor.assign(vstCoor.begin(), vstCoor.end());

    bool isBidirectionLook = true;  //是否双向查找
    /* 遍历查找凸包 */
    do {
        /* 首先判断是否已经是凸包 */
        vector<COORDINATE_2D_STRU> polygonTmp;
        polygonTmp.assign(deqCoor.begin(), deqCoor.end());
        if (objConvex_.IsConvexHull(polygonTmp)) {
            vConvexHull.push_back(polygonTmp);
            for (const COORDINATE_2D_STRU& stCoor : polygonTmp) ST_LOG_INFO("66 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
            return 0;
        }

        printf("*********************************************************************\n");
        /* 从第一个凸边开始查找 */
        bool isConvexSide = false;  // 是否为凸边
        do {
            if (SIG(CrossProduct(deqCoor[0], deqCoor[1], deqCoor[2])) < 0) {
                deqCoor.push_back(deqCoor.front());
                deqCoor.pop_front();
            } else {
                if (isBidirectionLook) {
                    bool isHead = false;
                    do {
                        if (SIG(CrossProduct(deqCoor.back(), deqCoor[0], deqCoor[1])) < 0) {
                            isHead = true;
                        } else {
                            deqCoor.push_front(deqCoor.back());
                            deqCoor.pop_back();
                        }
                    } while (!isHead);
                }
                isConvexSide = true;
            }
        } while (!isConvexSide);
        for (const COORDINATE_2D_STRU& stCoor : deqCoor) ST_LOG_INFO("isConvex x:%f y:%f", stCoor.x, stCoor.y);

        /* 计算凸多边形 */
        isBidirectionLook = false;
        vector<COORDINATE_2D_STRU> convex;
        convex.insert(convex.end(), deqCoor.begin(), deqCoor.begin() + 2);
        deqCoor.erase(deqCoor.begin(), deqCoor.begin() + 2);

        bool isOver = false;    // 查找是否结束
        do {
            size_t size = convex.size();
            /* 计算凸包 */
            int sign = SIG(CrossProduct(convex[size - 2], convex[size - 1], deqCoor.front()));

//            if (sign == 0) {
//                ST_LOG_INFO("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
//                ST_LOG_INFO("sign == 0  x:%f y:%f", deqCoor.front().x(), deqCoor.front().y());
//                convex.push_back(deqCoor.front());
//                deqCoor.pop_front();
//
//                if (deqCoor.empty()) {
//                    ST_LOG_INFO("00000000000000000000000000000000000000\n");
//                    deqCoor.push_back(convex.back());
//                    convex.pop_back();
//                    vConvexHull.push_back(convex);
//                    deqCoor.push_front(convex.back());
//                    deqCoor.push_back(convex.front());
//                    isOver = true;
//                    for (const COORDINATE_2D_STRU& stCoor : deqCoor) ST_LOG_INFO("77 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
//                    for (const COORDINATE_2D_STRU& stCoor : convex) ST_LOG_INFO("88 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
//                }
//            }

            if (sign >= 0) {
                convex.push_back(deqCoor.front());

                if (sign == 0) {
                    ST_LOG_INFO("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
                    ST_LOG_INFO("sign == 0 front x:%f y:%f", deqCoor.front().x, deqCoor.front().y);
                    //deqCoor.pop_front();
                    if (deqCoor.size() == 1) {
                        ST_LOG_INFO("00000000000000000000000000000000000000\n");
                        //deqCoor.push_back(convex.back());
                        convex.pop_back();
                        vConvexHull.push_back(convex);
                        deqCoor.push_front(convex.back());
                        deqCoor.push_back(convex.front());
                        isOver = true;
                        for (const COORDINATE_2D_STRU& stCoor : deqCoor) ST_LOG_INFO("77 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
                        for (const COORDINATE_2D_STRU& stCoor : convex) ST_LOG_INFO("88 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
                    }
                }

                if (!isOver) {
                    printf("----------------------- front 00 x:%f y:%f\n", deqCoor.front().x, deqCoor.front().y);
                    for (const COORDINATE_2D_STRU& stCoor : convex) ST_LOG_INFO("11 *** Convex x:%f y:%f", stCoor.x, stCoor.y);

                    bool isValid = objConvex_.IsConvexHull(convex); //是否为有效的凸包
                    ST_LOG_INFO("isValid  00: %d", isValid);

                    /* 凸包中是否包含其他点 */
                    if (isValid) {
                        for (size_t j = 1; j < deqCoor.size(); ++j) {
                            if (IsPointInPolygon(convex, deqCoor[j])) {
                                isValid = false;
                                isOver = true;
                                ST_LOG_INFO("in x:%f y:%f", deqCoor[j].x, deqCoor[j].y);
                                break;
                            }
                            ST_LOG_INFO("out x:%f y:%f", deqCoor[j].x, deqCoor[j].y);
                        }
                    }
                    ST_LOG_INFO("isValid  11: %d", isValid);

                    if (!isValid) {
                        convex.pop_back();
                        isOver = true;
                        deqCoor.push_front(convex.back());
                        deqCoor.push_front(convex.front());
                        if (convex.size() > 2) {
                            vConvexHull.push_back(convex);
                            for (const COORDINATE_2D_STRU& stCoor : convex) ST_LOG_INFO("22 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
                            isBidirectionLook = true;
                        } else {
                            deqCoor.push_back(deqCoor.front()); //调整到下一个元素，并指定单向查找
                            deqCoor.pop_front();
                            isBidirectionLook = false;
                        }
                    }
                }

                if (!isOver) {
                    printf("----------------------- pop front x:%f y:%f\n", deqCoor.front().x, deqCoor.front().y);
                    deqCoor.pop_front();
                }
                //SLEEP_S(1);
            }

            if (sign == -1) {
                ST_LOG_INFO("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
                while (!objConvex_.IsConvexHull(convex)) {
                    deqCoor.push_front(convex.back());
                    convex.pop_back();
                }
                for (const COORDINATE_2D_STRU& stCoor : deqCoor) ST_LOG_INFO("33 *** deqCoor x:%f y:%f", stCoor.x, stCoor.y);
                if (convex.size() > 2) {    //不存在<=2的情况
                    vConvexHull.push_back(convex);
                    for (const COORDINATE_2D_STRU& stCoor : convex) ST_LOG_INFO("44 *** Convex x:%f y:%f", stCoor.x, stCoor.y);
                    isBidirectionLook = true;
                    deqCoor.push_front(convex.back());
                    deqCoor.push_front(convex.front());
                    isOver = true;
                }
            }

            ST_LOG_INFO("deqCoor size:%lu", deqCoor.size());
        } while (!isOver);
    } while (deqCoor.size() > 2);

    return 0;
}

/**************************************************************************************
功能描述: 向量点乘
修改记录:
**************************************************************************************/
double PCPT_POLYGON_C::DotProduct(const COORDINATE_2D_STRU& a, const COORDINATE_2D_STRU& b) const
{
    return a.x * b.y - a.y * b.x;
}

/**************************************************************************************
功能描述: 计算叉积
修改记录:
**************************************************************************************/
double PCPT_POLYGON_C::CrossProduct(const POINT_COORD_2D_STRU& stPointO, const POINT_COORD_2D_STRU& stPointA, const POINT_COORD_2D_STRU& stPointB) const
{
    return objIoU_.CrossProduct(stPointO, stPointA, stPointB);
}

double PCPT_POLYGON_C::CrossProduct(const COORDINATE_2D_STRU& stPointO, const COORDINATE_2D_STRU& stPointA, const COORDINATE_2D_STRU& stPointB) const
{
    return CrossProduct(POINT_COORD_2D_STRU(stPointO.x, stPointO.y), POINT_COORD_2D_STRU(stPointA.x, stPointA.y), POINT_COORD_2D_STRU(stPointB.x, stPointB.y));
}

/**************************************************************************************
功能描述: 判断线段1和线段2是否交叉
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsCross(const COORDINATE_2D_STRU& line1A, const COORDINATE_2D_STRU& line1B, const COORDINATE_2D_STRU& line2A, const COORDINATE_2D_STRU& line2B) const
{
    /* 检查线段是否共线 */
    if (MAX(line1A.x, line1B.x) >= MIN(line2A.x, line2B.x) &&
        MIN(line1A.x, line1B.x) <= MAX(line2A.x, line2B.x) &&
        MAX(line1A.y, line1B.y) >= MIN(line2A.y, line2B.y) &&
        MIN(line1A.y, line1B.y) <= MAX(line2A.y, line2B.y)) {
        if (SIG(CrossProduct(line1A, line1B, line2A) * CrossProduct(line1A, line1B, line2B)) <= 0 &&
            SIG(CrossProduct(line2A, line2B, line1A) * CrossProduct(line2A, line2B, line1B)) <= 0) {
            return true;
        }
    }

    /* 检查端点是否相交 */
    if (line1A.IsAlmostEuqal(line2A) || line1A.IsAlmostEuqal(line2B) ||
        line1B.IsAlmostEuqal(line2A) || line1B.IsAlmostEuqal(line2B)) {
        return true;
    }

    return false;
}

/**************************************************************************************
功能描述: 判断点是否在线段上
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsPointOnLine(const double px0, const double py0,
                                   const double px1, const double py1,
                                   const double px2, const double py2) const
{
    bool flag = false;
    double d1 = (px1 - px0) * (py2 - py0) - (px2 - px0) * (py1 - py0);
    if (SIG(d1) == 0 && ((px0 - px1) * (px0 - px2) <= 0) && ((py0 - py1) * (py0 - py2) <= 0)) flag = true;
    return flag;
}

/**************************************************************************************
功能描述: 判断两线段相交
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsIntersect(const double px1, const double py1,
                                 const double px2, const double py2,
                                 const double px3, const double py3,
                                 const double px4, const double py4) const
{
    bool flag = false;
    double d = (px2 - px1) * (py4 - py3) - (py2 - py1) * (px4 - px3);
    if (SIG(d)) {
        double r = ((py1 - py3) * (px4 - px3) - (px1 - px3) * (py4 - py3)) / d;
        double s = ((py1 - py3) * (px2 - px1) - (px1 - px3) * (py2 - py1)) / d;
        if ((r >= 0) && (r <= 1) && (s >= 0) && (s <= 1)) flag = true;
    }
    return flag;
}

/**************************************************************************************
功能描述: 判断点在多边形内
修改记录:
**************************************************************************************/
bool PCPT_POLYGON_C::IsPointInPolygonTest(const vector<POINT_COORD_2D_STRU> &polygon, const POINT_COORD_2D_STRU& stPoint) const
{
    bool isInside = false;
    int count = 0;

    double minX = DBL_MAX;
    for (size_t i = 0; i < polygon.size(); i++) minX = MIN(minX, polygon[i].x);

    double px = stPoint.x;
    double py = stPoint.y;
    double linePoint1x = stPoint.x;
    double linePoint1y = stPoint.y;
    double linePoint2x = minX - 10;         //取最小的X值还小的值作为射线的终点
    double linePoint2y = stPoint.y;

    /* 遍历每一条边 */
    for (size_t i = 0; i < polygon.size() - 1; i++) {
        double cx1 = polygon[i].x;
        double cy1 = polygon[i].y;
        double cx2 = polygon[i + 1].x;
        double cy2 = polygon[i + 1].y;

        if (IsPointOnLine(px, py, cx1, cy1, cx2, cy2)) return true;
        if (SIG(cy2 - cy1) == 0) continue;  //平行则不相交

        if (IsPointOnLine(cx1, cy1, linePoint1x, linePoint1y, linePoint2x, linePoint2y)) {
            if (cy1 > cy2) count++;         //只保证上端点+1
        } else if (IsPointOnLine(cx2, cy2, linePoint1x, linePoint1y, linePoint2x, linePoint2y)) {
            if (cy2 > cy1) count++;         //只保证上端点+1
        } else if (IsIntersect(cx1, cy1, cx2, cy2, linePoint1x, linePoint1y, linePoint2x, linePoint2y)) count++;  //已经排除平行的情况
    }
    if (count % 2 == 1) isInside = true;
    return isInside;
}

