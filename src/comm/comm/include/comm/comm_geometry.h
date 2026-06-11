

#ifndef __COMM_GEOMETRY_H__
#define __COMM_GEOMETRY_H__

#include "comm/comm_base.h"
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 向量
修改记录:
**************************************************************************************/
class VECTOR_C
{
public:
    VECTOR_C(void);
    VECTOR_C(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint);
    VECTOR_C(float len, float dirAngle);

    /* 判断方向是否一样 */
    BOOL  IsDirSame(const VECTOR_C &objOther) const;

    /* 判断方向是否一致，即夹角小于90° */
    BOOL  IsDirConsistent(VECTOR_C &objOther);

    /* 获取两向量夹角 */
    float GetIncludedAngle(const VECTOR_C &objOther) const;

    /* 获取向量长度 */
    float GetLen(void) const;

    float GetDirAngle(void);

    float GetLenSqu(void) const;

    /* 获取点乘 */
    float DotMult(VECTOR_C &objOther);

private:
    BOOL               isValid_;

    BOOL               isExistPoint_;
    COORDINATE_2D_STRU stPoint_;

    BOOL               isExistDirLen_;
    float              len_;
    float              dirAngle_;

    inline void ToPoint(void)
    {
        stPoint_.x = len_ * cos(dirAngle_);
        stPoint_.y = len_ * sin(dirAngle_);

        isExistPoint_ = true;
    }

    inline void ToDirLen(void)
    {
        len_      = sqrt(stPoint_.x * stPoint_.x + stPoint_.y * stPoint_.y);
        dirAngle_ = atan2(stPoint_.y, stPoint_.x);

        isExistDirLen_ = true;
    }
};

/**************************************************************************************
功能描述: 有向直线
          采用一个点和一个向量来表示
修改记录:
**************************************************************************************/
class DIRECTED_LINE_C
{
public:
    DIRECTED_LINE_C(void);
    DIRECTED_LINE_C(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint);
    DIRECTED_LINE_C(const COORDINATE_2D_STRU &stStartPoint, float dirAngle);
    DIRECTED_LINE_C(const COORDINATE_2D_STRU &stStartPoint, float dirAngleSin, float dirAngleCos);
    DIRECTED_LINE_C(POSE_2D_C &objPose);

    void  Reset(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint);
    void  Reset(const COORDINATE_2D_STRU &stStartPoint, float dirAngle);
    void  Reset(const COORDINATE_2D_STRU &stStartPoint, float dirAngleSin, float dirAngleCos);
    void  Clear(void);

    BOOL  IsValid(void) const;

    INT32 GetPointSide(const COORDINATE_2D_STRU &stPoint) const;
    bool  IsPointOnLeft(const COORDINATE_2D_STRU &stPoint) const;
    bool  IsPointOnLine(const COORDINATE_2D_STRU &stPoint) const;
    bool  IsPointOnRight(const COORDINATE_2D_STRU &stPoint) const;


    int   GetIntersectionWithAnotherLine(const DIRECTED_LINE_C &objLine2, COORDINATE_2D_STRU &stIntersection) const;
    float GetDistSquToPoint(const COORDINATE_2D_STRU &stPoint) const;
    float GetDirAngle(void) const;

protected:
    COORDINATE_2D_STRU stStartPoint_;
    COORDINATE_2D_STRU stVector_;

private:
};

/**************************************************************************************
功能描述: 射线
修改记录:
**************************************************************************************/
class RAY_C : public DIRECTED_LINE_C
{
public:
    RAY_C(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stForwardPoint) : DIRECTED_LINE_C(stStartPoint, stForwardPoint)
    {
    }

    RAY_C(const COORDINATE_2D_STRU &stPoint, float dirAngle)  : DIRECTED_LINE_C(stPoint, dirAngle)
    {
    }

    RAY_C(POSE_2D_C &objPose) : DIRECTED_LINE_C(objPose)
    {
    }

    const COORDINATE_2D_STRU &GetStartPoint(void) const;

    BOOL IsPointOnFront(const COORDINATE_2D_STRU &stPoint) const;
    BOOL IsPointOnRay(const COORDINATE_2D_STRU &stPoint) const;

    BOOL IsIntersectWithArea(const AREA_C &objRectArea) const;

protected:

private:
};

/**************************************************************************************
功能描述: 有向线段
修改记录:
**************************************************************************************/
class DIRECTED_LINE_SEGMENT_C : public RAY_C
{
public:
    DIRECTED_LINE_SEGMENT_C(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint)
                           : RAY_C(stStartPoint, stEndPoint)
    {
        stEndPoint_.Copy(stEndPoint);
    }

    BOOL IsIntersectedWithRay(const RAY_C &objRay) const;
    BOOL IsIntersectedWithOtherLineSeg(const DIRECTED_LINE_SEGMENT_C &objOther) const;
    BOOL IsPointOnLineSeg(const COORDINATE_2D_STRU &stPoint) const;

    const COORDINATE_2D_STRU &GetEndPoint(void) const;
    void GetRectArea(AREA_C &objRectArea) const;
    int  GetIntersectionWithRay(const RAY_C &objRay, COORDINATE_2D_STRU &stIntersection) const;
    float GetDistanceToPoint(const COORDINATE_2D_STRU &stPoint) const;

private:
    COORDINATE_2D_STRU stEndPoint_;
};

/**************************************************************************************
功能描述: 多段有向直线
          有序的多个点，两两依次组成多个有向直线
          处理多边形时，仅用于处理凸多边形
修改记录:
**************************************************************************************/
class MULT_DIRECTED_LINE_C
{
public:
    MULT_DIRECTED_LINE_C(void);
    ~MULT_DIRECTED_LINE_C(void);

    void AddExtendPoint(const COORDINATE_2D_STRU &stNewPoint);
    void AddExtendPoint(float pointX, float pointY);
    void Clear(void);
    bool IsPointOnLeft(const COORDINATE_2D_STRU &stPoint); /* 暂未使用，接口待优化 */

protected:

private:
    bool isFirstPointExist_;
    COORDINATE_2D_STRU stLastPoint_;
    list<DIRECTED_LINE_C *> lstDirLine_;
};

/**************************************************************************************
功能描述: 任意多边形
修改记录:
**************************************************************************************/
class ANY_POLYGON_C
{
public:
    ANY_POLYGON_C(const std::vector<COORDINATE_2D_STRU> &vectVertice)
    {
        INT32 rslt = Reset(vectVertice);
        if (rslt != 0) {
            ST_LOG_ERR("Reset() Fail.");
        }
    }

    ANY_POLYGON_C(void)
    {
    }

    ~ANY_POLYGON_C(void)
    {
    }

    BOOL  IsValid(void) const;
    BOOL  IsPointIn(const COORDINATE_2D_STRU &stPoint) const;
    BOOL  IsIntersectWithLineSeg(const DIRECTED_LINE_SEGMENT_C &objLineSeg) const;
    BOOL  IsIntersectWithOtherPolygon(const ANY_POLYGON_C &objOtherPolygon) const;
    BOOL  IsExistOverlapWithOtherPolygon(const ANY_POLYGON_C &objOtherPolygon) const;
    BOOL  IsCircleOut(const COORDINATE_2D_STRU &stCenter, float radius) const;
    INT32 Reset(const std::vector<COORDINATE_2D_STRU> &vectVertice);
    INT32 GetRectArea(AREA_C &objRectArea) const;

private:
    struct EDGE_GROUP_STRU {
        AREA_C                               objArea;
        std::vector<DIRECTED_LINE_SEGMENT_C> vectEdge;
    };

    AREA_C                       objRectArea_;
    std::vector<EDGE_GROUP_STRU> vectEdgeGroup_;

    void  DspAllVertice(const std::vector<COORDINATE_2D_STRU> &vectVertice) const;
    void  DivideEdgeGroup(const std::vector<COORDINATE_2D_STRU> &vectVertice, BOOL isClose, std::vector<EDGE_GROUP_STRU> &vectGroup) const;
    BOOL  IsExistEdgesIntersectInGroup(const EDGE_GROUP_STRU &stGroup, BOOL isEndClosed) const;
    BOOL  IsExistEdgesIntersectBetweenSeparatedGroup(const EDGE_GROUP_STRU &stGroup1, const EDGE_GROUP_STRU &stGroup2) const;
    BOOL  IsExistEdgesIntersectBetweenConnectedGroup(const EDGE_GROUP_STRU &stFrontGroup, const EDGE_GROUP_STRU &stBackGroup, BOOL isEndClosed) const;
    BOOL  IsExistEdgesIntersected(const std::vector<EDGE_GROUP_STRU> &vectGroup) const;
    INT32 MakePolygon(const std::vector<COORDINATE_2D_STRU> &vectVertice);

    void  Clear(void);
};

/**************************************************************************************
功能描述: 凸多边形
          给定的多边形定点合法性由调用方保证
修改记录:
**************************************************************************************/
class CONVEX_POLYGON_C
{
public:
    CONVEX_POLYGON_C(COORDINATE_2D_STRU astPoint[], int pointNum);
    CONVEX_POLYGON_C(void);
    ~CONVEX_POLYGON_C(void);


    INT32 Reset(COORDINATE_2D_STRU astPoint[], int pointNum);

    BOOL  IsPointIn(const COORDINATE_2D_STRU &stPoint) const;

    float GetDistSquOfInternalPointAlongDirToEdge(const COORDINATE_2D_STRU &stInternalPoint, float dirAngle);
    float GetMinDistSquOfInternalPointToEdge(const COORDINATE_2D_STRU &stInternalPoint);

private:
    BOOL                                 isExistFirstVertice_;            /* 临时变量，是否已存在第一个顶点 */
    COORDINATE_2D_STRU                   stLastVertice_;                  /* 临时变量，上次添加的顶点 */
    std::list<DIRECTED_LINE_SEGMENT_C *> lstDirLineSeg_;

    INT32 AddVertice(COORDINATE_2D_STRU &stPoint);
    BOOL  Check(void);
    void  Clear(void);
    INT32 Make(COORDINATE_2D_STRU astPoint[], int pointNum);

    DIRECTED_LINE_SEGMENT_C *GetLineSegIntersectedByInternalRay(RAY_C &objInternalRay);
};

/**************************************************************************************
功能描述: 计算凸包（基于Graham扫描法，极角排序）
修改记录:
**************************************************************************************/
class CONVEX_HULL_C
{
public:
    CONVEX_HULL_C(){}
    ~CONVEX_HULL_C(){}

    bool IsConvexHull(const vector<COORDINATE_2D_STRU>& points) const;
    vector<COORDINATE_2D_STRU> ConvexHull(const vector<COORDINATE_2D_STRU>& points) const;

private:
    bool Compare(const POINT_COORD_2D_STRU& a, const POINT_COORD_2D_STRU& b, const POINT_COORD_2D_STRU& origin) const;
    vector<POINT_COORD_2D_STRU> ConvexHullTurn(vector<POINT_COORD_2D_STRU>& points) const;
};

/**************************************************************************************
功能描述: 三角形
修改记录:
**************************************************************************************/
class TRIANGLE_C
{
public:
    float Get3thSideLenSqu(float sideLen1, float sideLen2, ANGLE_C &objAngleOf2Sides);
private:
};

/**************************************************************************************
功能描述: 圆形
修改记录:
**************************************************************************************/
class CIRCULAR_C
{
public:
    CIRCULAR_C(const COORDINATE_2D_STRU &stCenter, float radius);
    CIRCULAR_C(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, const COORDINATE_2D_STRU &stPoint3);

    BOOL  IsValid(void) const;
    INT32 GetCenter(COORDINATE_2D_STRU &stCenter) const;
    float GetRadiusSqu(void) const;
    float GetRadius(void);
    INT32 GetSamplePoints(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint, float deltaRadAbs, vector<COORDINATE_2D_STRU> &vectSamplePoints);
    BOOL  IsPointIn(const COORDINATE_2D_STRU &point) const;
    BOOL  IsIntersectedLineSegment(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint) const;

private:
    BOOL               isValid_         = false;

    COORDINATE_2D_STRU stCenter_;                   /* isValid_ 为 true 时有效 */
    float              radiusSqu_;                  /* isValid_ 为 true 时有效 */

    BOOL               isExistRadius_   = false;
    float              radius_;                     /* isExistRadius_ 为 true 时有效 */
};

/**************************************************************************************
功能描述: 扇形
修改记录:
**************************************************************************************/
class SECTOR_C
{
public:
    SECTOR_C(const COORDINATE_2D_STRU &stCenter, float radius, float beginAngle, float angleRange) : stCenter_(stCenter),
                                                                                                     radius_(radius),
                                                                                                     isAngleRangePositive_(angleRange >= 0.0),
                                                                                                     isAngleRangeOverPi_(ABS(angleRange) > PI),
                                                                                                     objBeginAngleLine_(stCenter, beginAngle),
                                                                                                     objEndAngleLine_(stCenter, beginAngle + angleRange)
    {
    }

    BOOL IsPointIn(const COORDINATE_2D_STRU &stPoint);

private:
    const COORDINATE_2D_STRU stCenter_;
    const float              radius_;
    const BOOL               isAngleRangePositive_;
    const BOOL               isAngleRangeOverPi_;
    const DIRECTED_LINE_C    objBeginAngleLine_;
    const DIRECTED_LINE_C    objEndAngleLine_;
};

#endif

