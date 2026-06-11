
#ifndef __COMM_GEOMETRY_CPP__
#define __COMM_GEOMETRY_CPP__

#include "comm/comm_geometry.h"
#include "comm/comm_base.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

VECTOR_C::VECTOR_C(void)
{
    isValid_ = false;
}

VECTOR_C::VECTOR_C(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint)
{
    stPoint_.x = stEndPoint.x - stStartPoint.x;
    stPoint_.y = stEndPoint.y - stStartPoint.y;

    isExistPoint_  = true;
    isExistDirLen_ = false;
    isValid_       = true;
}

VECTOR_C::VECTOR_C(float len, float dirAngle)
{
    len_      = len;
    dirAngle_ = dirAngle;

    isExistPoint_  = false;
    isExistDirLen_ = true;
    isValid_       = true;
}

/**************************************************************************************
功能描述: 判断方向是否一致
修改记录:
**************************************************************************************/
BOOL VECTOR_C::IsDirConsistent(VECTOR_C &objOther)
{
    R_ASSERT(isValid_, false);

    return (DotMult(objOther) > 0);
}

float VECTOR_C::GetDirAngle(void)
{
    R_ASSERT(isValid_, 0.0);

    if (!isExistDirLen_) {
        ToDirLen();
    }

    return dirAngle_;
}

float VECTOR_C::GetLenSqu(void) const
{
    R_ASSERT(isValid_, -1);

    if (isExistPoint_) {
        return (stPoint_.x * stPoint_.x + stPoint_.y * stPoint_.y);
    } else {
        return len_ * len_;
    }
}

float VECTOR_C::DotMult(VECTOR_C &objOther)
{
    R_ASSERT(isValid_ && objOther.isValid_, 0.0);

    /* 优先坐标计算，再次长度和角度计算，最后转成坐标进行计算 */
    if (isExistPoint_ && objOther.isExistPoint_) {
        return (stPoint_.x * objOther.stPoint_.x + stPoint_.y * objOther.stPoint_.y);

    } else if (isExistDirLen_ && isExistDirLen_) {
        return (len_ * objOther.len_ *cos(dirAngle_ - objOther.dirAngle_));

    } else {
        if (isExistPoint_) {
            objOther.ToPoint();
        } else {
            ToPoint();
        }

        return (stPoint_.x * objOther.stPoint_.x + stPoint_.y * objOther.stPoint_.y);
    }
}

DIRECTED_LINE_C::DIRECTED_LINE_C(void)
{
    Clear();
}

DIRECTED_LINE_C::DIRECTED_LINE_C(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint)
{
    Reset(stStartPoint, stEndPoint);
}

DIRECTED_LINE_C::DIRECTED_LINE_C(const COORDINATE_2D_STRU &stStartPoint, float dirAngle)
{
    Reset(stStartPoint, dirAngle);
}

DIRECTED_LINE_C::DIRECTED_LINE_C(const COORDINATE_2D_STRU &stStartPoint, float dirAngleSin, float dirAngleCos)
{
    Reset(stStartPoint, dirAngleSin, dirAngleCos);
}

DIRECTED_LINE_C::DIRECTED_LINE_C(POSE_2D_C &objPose)
{
    Reset(objPose.stPos, objPose.objDir.Sin(), objPose.objDir.Cos());
}

void DIRECTED_LINE_C::Reset(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint)
{
    stStartPoint_.Copy(stStartPoint);
    stVector_ = stEndPoint - stStartPoint_;
}

void DIRECTED_LINE_C::Reset(const COORDINATE_2D_STRU &stStartPoint, float dirAngle)
{
    stStartPoint_.Copy(stStartPoint);

    stVector_.x = cos(dirAngle);
    stVector_.y = sin(dirAngle);
}

void DIRECTED_LINE_C::Reset(const COORDINATE_2D_STRU &stStartPoint, float dirAngleSin, float dirAngleCos)
{
    stStartPoint_.Copy(stStartPoint);

    stVector_.x = dirAngleCos;
    stVector_.y = dirAngleSin;
}

void DIRECTED_LINE_C::Clear(void)
{
    stStartPoint_.SetZero();
    stVector_.SetZero();
}

BOOL DIRECTED_LINE_C::IsValid(void) const
{
    return (stVector_.GetVectorLenSqu() > std::numeric_limits<float>::epsilon());
}

/**************************************************************************************
功能描述: 获取给定点在直线的某一侧
          小于0表示右侧，大于0表示左侧，等于0表示在直线上
修改记录:
**************************************************************************************/
INT32 DIRECTED_LINE_C::GetPointSide(const COORDINATE_2D_STRU &stPoint) const
{
    /* 计算直线起点到指定点的向量 */
    COORDINATE_2D_STRU stPointVector = stPoint - stStartPoint_;

    /* 叉乘: V1 X V2 = |V1|*|V2|*sin */
    float vectorCrossMult  = stVector_.x * stPointVector.y - stVector_.y * stPointVector.x;
    float lenOfVecotr      = std::abs(stVector_.x) + std::abs(stVector_.y);            /* 向量长度，近似计算 */
    float lenOfPointVector = std::abs(stPointVector.x) + std::abs(stPointVector.y);    /* 向量长度，近似计算 */
    float lenMult          = lenOfVecotr * lenOfPointVector;
    if (lenMult <= std::numeric_limits<float>::epsilon()) {
        return 0;
    }

    /* 计算两个向量夹角，以及点到直线的距离。 */
    /* 夹角和距离任何一个小于阈值就表示点在线上。 */
    /* 该门限不能太小，容易误判 */
    float sinOf2Vector = vectorCrossMult / lenMult;
    float distOfToLine = std::abs(sinOf2Vector * lenOfPointVector);
    //ST_LOG_INFO("vectorCrossMult=%f, sinOf2Vector=%f, distOfToLine=%f.", vectorCrossMult, sinOf2Vector, distOfToLine);
    /* 这里的距离阈值与 IsIntersectedWithOtherLineSeg 保持一致             */
    if ((std::abs(sinOf2Vector) < 1.0e-4) || (distOfToLine < 1.0e-3)) {
        return 0;
    } else if (sinOf2Vector < 0.0) {
        return -1;
    } else {
        return 1;
    }
}

/**************************************************************************************
功能描述: 判断给定点是否在该有向直线的左侧
修改记录:
**************************************************************************************/
bool DIRECTED_LINE_C::IsPointOnLeft(const COORDINATE_2D_STRU &stPoint) const
{
    return (GetPointSide(stPoint) > 0);
}

/**************************************************************************************
功能描述: 判断给定点是否在该有向直线上
修改记录:
**************************************************************************************/
bool DIRECTED_LINE_C::IsPointOnLine(const COORDINATE_2D_STRU &stPoint) const
{
    return (GetPointSide(stPoint) == 0);
}

/**************************************************************************************
功能描述: 判断给定点是否在该有向直线的右侧
修改记录:
**************************************************************************************/
bool DIRECTED_LINE_C::IsPointOnRight(const COORDINATE_2D_STRU &stPoint) const
{
    return (GetPointSide(stPoint) < 0);
}

/**************************************************************************************
功能描述: 获取两条直线的交点
修改记录:
**************************************************************************************/
int DIRECTED_LINE_C::GetIntersectionWithAnotherLine(const DIRECTED_LINE_C &objLine2, COORDINATE_2D_STRU &stIntersection) const
{
    /* 判断是否平行，平行无交点 */
    float denominator = objLine2.stVector_.x * stVector_.y - objLine2.stVector_.y * stVector_.x;
    if (ABS(denominator) < 0.000001) {
        return -1;
    }

    float factor = ((stStartPoint_.x - objLine2.stStartPoint_.x) * stVector_.y - (stStartPoint_.y - objLine2.stStartPoint_.y) * stVector_.x) / denominator;
    stIntersection.x = objLine2.stStartPoint_.x + factor * objLine2.stVector_.x;
    stIntersection.y = objLine2.stStartPoint_.y + factor * objLine2.stVector_.y;
    return 0;
}

/**************************************************************************************
功能描述: 获取给定点到直线的垂直距离的平方
修改记录:
**************************************************************************************/
float DIRECTED_LINE_C::GetDistSquToPoint(const COORDINATE_2D_STRU &stPoint) const
{
    float lineVectorLenSqu = stVector_.GetVectorLenSqu();

    /* 如果表示直行反向的向量长度几乎为0，则无法确定直行方向，无法计算点到该直行的距离。 */
    if (IS_FLOAT_ALMOST_EQUAL_ZERO(lineVectorLenSqu)) {
        return -1;
    }

    /* 根据两个叉乘的结果数字上等于对应平行四边形面积，来计算点到直线的距离 */
    COORDINATE_2D_STRU stPointVector = stPoint - stStartPoint_;
    float crossMulti = stVector_.x * stPointVector.y - stPointVector.x * stVector_.y;
    return (crossMulti * crossMulti) / lineVectorLenSqu;
}

float DIRECTED_LINE_C::GetDirAngle(void) const
{
    return atan2(stVector_.y, stVector_.x);
}

const COORDINATE_2D_STRU &RAY_C::GetStartPoint(void) const
{
    return stStartPoint_;
}

BOOL RAY_C::IsPointOnFront(const COORDINATE_2D_STRU &stPoint) const
{
    COORDINATE_2D_STRU stPointVectot = stPoint - stStartPoint_;

    /* 点乘大于0则在前方 */
    return ((stVector_.x * stPointVectot.x + stVector_.y * stPointVectot.y) >= 0.0);
}

BOOL RAY_C::IsPointOnRay(const COORDINATE_2D_STRU &stPoint) const
{
    if (!DIRECTED_LINE_C::IsPointOnLine(stPoint)) {
        return false;
    }

    /* 根据点乘符号判断是否在前方 */
    COORDINATE_2D_STRU stPointVector = stPoint - stStartPoint_;
    float dotMulti = stVector_.x * stPointVector.x + stVector_.y * stPointVector.y;
    return (dotMulti >= 0.0);
}

BOOL RAY_C::IsIntersectWithArea(const AREA_C &objRectArea) const
{
    if (!objRectArea.IsValid()) {
        return false;
    }

    COORDINATE_2D_STRU stRectAreaVector  = objRectArea.stRTPoint - objRectArea.stLBPoint;
    BOOL               isAreaVectorXZero = (std::abs(stRectAreaVector.x) <= std::numeric_limits<float>::epsilon());
    BOOL               isAreaVectorYZero = (std::abs(stRectAreaVector.y) <= std::numeric_limits<float>::epsilon());
    if (isAreaVectorXZero || isAreaVectorYZero) {
        /* 区域呈现为点或线段 */
        if (isAreaVectorXZero && isAreaVectorYZero) {
            return this->IsPointOnRay(objRectArea.stRTPoint);
        } else {
            DIRECTED_LINE_SEGMENT_C objLineSeg(objRectArea.stLBPoint, objRectArea.stRTPoint);
            return objLineSeg.IsIntersectedWithRay(*this);
        }
    } else {
        /* 正常矩形区域 */
        COORDINATE_2D_STRU stVertex1{objRectArea.stLBPoint.x, objRectArea.stLBPoint.y};
        COORDINATE_2D_STRU stVertex2{objRectArea.stLBPoint.x, objRectArea.stRTPoint.y};
        DIRECTED_LINE_SEGMENT_C objLineSeg1(stVertex1, stVertex2);
        if (objLineSeg1.IsIntersectedWithRay(*this)) {
            return true;
        }

        COORDINATE_2D_STRU stVertex3{objRectArea.stRTPoint.x, objRectArea.stRTPoint.y};
        DIRECTED_LINE_SEGMENT_C objLineSeg2(stVertex2, stVertex3);
        if (objLineSeg2.IsIntersectedWithRay(*this)) {
            return true;
        }

        COORDINATE_2D_STRU stVertex4{objRectArea.stRTPoint.x, objRectArea.stLBPoint.y};
        DIRECTED_LINE_SEGMENT_C objLineSeg3(stVertex3, stVertex4);
        if (objLineSeg3.IsIntersectedWithRay(*this)) {
            return true;
        }

        DIRECTED_LINE_SEGMENT_C objLineSeg4(stVertex4, stVertex1);
        if (objLineSeg4.IsIntersectedWithRay(*this)) {
            return true;
        }

        return false;
    }
}

/**************************************************************************************
功能描述: 判断该线段是否和给定射线相交
修改记录:
**************************************************************************************/
bool DIRECTED_LINE_SEGMENT_C::IsIntersectedWithRay(const RAY_C &objRay) const
{
    const COORDINATE_2D_STRU &stRayStartPoint = objRay.GetStartPoint();
    if        (this->IsPointOnLeft(stRayStartPoint)) {
        return (!objRay.IsPointOnLeft(this->GetStartPoint())  && !objRay.IsPointOnRight(this->stEndPoint_));
    } else if (this->IsPointOnRight(stRayStartPoint)) {
        return (!objRay.IsPointOnRight(this->GetStartPoint()) && !objRay.IsPointOnLeft(this->stEndPoint_));
    } else {
        return (   this->IsPointOnLineSeg(stRayStartPoint)
                || objRay.IsPointOnRay(stStartPoint_)
                || objRay.IsPointOnRay(stEndPoint_));
    }
}

BOOL DIRECTED_LINE_SEGMENT_C::IsIntersectedWithOtherLineSeg(const DIRECTED_LINE_SEGMENT_C &objOther) const
{
    /* 有重叠的点，就认为时相交 */
    /* 计算两个线段4个端点相对于另一条线段的位置 */
    INT32 sideOfOtherStart = this->GetPointSide(objOther.stStartPoint_);
    INT32 sideOfOtherEnd   = this->GetPointSide(objOther.stEndPoint_);
    INT32 sideOfThisStart  = objOther.GetPointSide(this->stStartPoint_);
    INT32 sideOfThisEnd    = objOther.GetPointSide(this->stEndPoint_);

    BOOL  isCalcCollineAbnormal = false;

    /* 如果某个端点在另一条线段的直线上，则该点为共线点。计算总的共线点的个数。 */
    INT32 numOfCollinePoint     = 4 - std::abs(sideOfOtherStart) - std::abs(sideOfOtherEnd) - std::abs(sideOfThisStart) - std::abs(sideOfThisEnd);
    switch (numOfCollinePoint) {
        case 0: {       /* 没有共线点，直接判是否交叉 */
            return (((sideOfOtherStart * sideOfOtherEnd) < 0) && ((sideOfThisStart * sideOfThisEnd) < 0));
        }

        case 1: {       /* 一个共线点，判断另一个线段两端点是否在该线段两侧。 */
            BOOL isCollinePointOnOther = (sideOfOtherStart == 0) || (sideOfOtherEnd == 0);
            if (isCollinePointOnOther) {
                return ((sideOfThisStart * sideOfThisEnd) < 0);
            } else {
                return ((sideOfOtherStart * sideOfOtherEnd) < 0);
            }
        }

        case 2: {       /* 两个共线点，分为两种情况，他们在同一个线段上和在不同线段上。 */
            BOOL isCollinePointsOnSame = (std::abs(sideOfOtherStart) == std::abs(sideOfOtherEnd));
            if (isCollinePointsOnSame) {
                /* 他们在同一个线段上 *//* 这种情况，理论上不应该出现，应该只出现在两线段共线时，计算误差导致另外两个端点共线。此处还是当共线处理 */
                isCalcCollineAbnormal = true;
                break;
            } else {
                /* 他们在不同线段上 *//* 又分为两种情况，两个共线点是重叠的，还有就是两个线段共线但计算误差导致 */
                const COORDINATE_2D_STRU &stCollinePointOfOther = (sideOfOtherStart == 0) ? objOther.stStartPoint_ : objOther.stEndPoint_;
                const COORDINATE_2D_STRU &stCollinePointOfThis  = (sideOfThisStart == 0)  ? this->stStartPoint_    : this->stEndPoint_;
                /* 这里的距离阈值与 GetPointSide 保持一致             */
                if (stCollinePointOfOther.CalcDistSqu(stCollinePointOfThis) < 1.0e-6) {
                    return true;
                } else {
                    isCalcCollineAbnormal = true;
                    break;
                }
            }
        }

        case 3: {       /* 三个共线点，理论上不应该出现，应该只出现在两线段共线时，计算误差导致另外两个端点共线。此处还是当共线处理 */
            isCalcCollineAbnormal = true;
            break;
        }

        default: {      /* 四个共线点，正常两线段共线 */
            break;
        }
    };

    /* 处理异常共线 */
    if (isCalcCollineAbnormal) {
        //ST_LOG_INFO("Judge Line Colline Abnormal. Sides(%d %d %d %d), Other(%.2f, %.2f)-(%.2f, %.2f), This(%.2f, %.2f)-(%.2f, %.2f).",
        //            sideOfOtherStart,         sideOfOtherEnd,           sideOfThisStart,        sideOfThisEnd,
        //            objOther.stStartPoint_.x, objOther.stStartPoint_.y, objOther.stEndPoint_.x, objOther.stEndPoint_.y,
        //            this->stStartPoint_.x,    this->stStartPoint_.y,    this->stEndPoint_.x,    this->stEndPoint_.y);
    }

    /* 共线时，一边两点比另一边两点都大或都小为不重叠交叉 */
    if (std::abs(this->stStartPoint_.x - this->stEndPoint_.x) > std::abs(this->stStartPoint_.y - this->stEndPoint_.y)) {
        return !(   (   (objOther.stStartPoint_.x > this->stStartPoint_.x)
                     && (objOther.stEndPoint_.x   > this->stStartPoint_.x)
                     && (objOther.stStartPoint_.x > this->stEndPoint_.x)
                     && (objOther.stEndPoint_.x   > this->stEndPoint_.x))
                 || (   (objOther.stStartPoint_.x < this->stStartPoint_.x)
                     && (objOther.stEndPoint_.x   < this->stStartPoint_.x)
                     && (objOther.stStartPoint_.x < this->stEndPoint_.x)
                     && (objOther.stEndPoint_.x   < this->stEndPoint_.x)));
    } else {
        return !(   (   (objOther.stStartPoint_.y > this->stStartPoint_.y)
                     && (objOther.stEndPoint_.y   > this->stStartPoint_.y)
                     && (objOther.stStartPoint_.y > this->stEndPoint_.y)
                     && (objOther.stEndPoint_.y   > this->stEndPoint_.y))
                 || (   (objOther.stStartPoint_.y < this->stStartPoint_.y)
                     && (objOther.stEndPoint_.y   < this->stStartPoint_.y)
                     && (objOther.stStartPoint_.y < this->stEndPoint_.y)
                     && (objOther.stEndPoint_.y   < this->stEndPoint_.y)));
    }
}

BOOL DIRECTED_LINE_SEGMENT_C::IsPointOnLineSeg(const COORDINATE_2D_STRU &stPoint) const
{
    if (!IsPointOnLine(stPoint)) {
        return false;
    }

    /* 共线时 */
    if (std::abs(this->stStartPoint_.x - this->stEndPoint_.x) > std::abs(this->stStartPoint_.y - this->stEndPoint_.y)) {
        return !(   ((stPoint.x > this->stStartPoint_.x) && (stPoint.x > this->stEndPoint_.x))
                 || ((stPoint.x < this->stStartPoint_.x) && (stPoint.x < this->stEndPoint_.x)));
    } else {
        return !(   ((stPoint.y > this->stStartPoint_.y) && (stPoint.y > this->stEndPoint_.y))
                 || ((stPoint.y < this->stStartPoint_.y) && (stPoint.y < this->stEndPoint_.y)));
    }
}

float DIRECTED_LINE_SEGMENT_C::GetDistanceToPoint(const COORDINATE_2D_STRU &stPoint) const {
    float len = std::sqrt(std::pow(stStartPoint_.x - stEndPoint_.x, 2) + std::pow(stStartPoint_.y - stEndPoint_.y, 2));
    if (len < 0.5) {
        return std::sqrt(std::pow(stPoint.x - stStartPoint_.x, 2) + std::pow(stPoint.y - stStartPoint_.y, 2));
    }
    float t = ((stPoint.x - stStartPoint_.x) * (stEndPoint_.x - stStartPoint_.x) +
               (stPoint.y - stStartPoint_.y) * (stEndPoint_.y - stStartPoint_.y)) / (len * len);

    if (t < 0.0) {
        return std::sqrt(std::pow(stPoint.x - stStartPoint_.x, 2) + std::pow(stPoint.y - stStartPoint_.y, 2));
    } else if (t > 1.0) {
        return std::sqrt(std::pow(stPoint.x - stEndPoint_.x, 2) + std::pow(stPoint.y - stEndPoint_.y, 2));
    }
    COORDINATE_2D_STRU closestPoint(stStartPoint_.x + t * (stEndPoint_.x - stStartPoint_.x),
                                    stStartPoint_.y + t * (stEndPoint_.y - stStartPoint_.y));
    return std::sqrt(std::pow(stPoint.x - closestPoint.x, 2) + std::pow(stPoint.y - closestPoint.y, 2));
}

const COORDINATE_2D_STRU &DIRECTED_LINE_SEGMENT_C::GetEndPoint(void) const
{
    return stEndPoint_;
}

void DIRECTED_LINE_SEGMENT_C::GetRectArea(AREA_C &objRectArea) const
{
    objRectArea.Reset();
    objRectArea.IncludePoint(stStartPoint_);
    objRectArea.IncludePoint(stEndPoint_);
}

int DIRECTED_LINE_SEGMENT_C::GetIntersectionWithRay(const RAY_C &objRay, COORDINATE_2D_STRU &stIntersection) const
{
    if (!IsIntersectedWithRay(objRay)) {
        return -1;
    }

    return GetIntersectionWithAnotherLine(objRay, stIntersection);
}

MULT_DIRECTED_LINE_C::MULT_DIRECTED_LINE_C(void)
{
    isFirstPointExist_ = false;
}

MULT_DIRECTED_LINE_C::~MULT_DIRECTED_LINE_C(void)
{
    Clear();
}

void MULT_DIRECTED_LINE_C::AddExtendPoint(const COORDINATE_2D_STRU &stNewPoint)
{
    if (isFirstPointExist_) {
        DIRECTED_LINE_C *pstNewLine = new DIRECTED_LINE_C(stLastPoint_, stNewPoint);
        if (pstNewLine == NULL) {
            ST_LOG_ERR("new fail, x=%f, y=%f.", stNewPoint.x, stNewPoint.y);
            return;
        }

        lstDirLine_.push_back(pstNewLine);
    }

    stLastPoint_.x = stNewPoint.x;
    stLastPoint_.y = stNewPoint.y;
    isFirstPointExist_ = true;
}

void MULT_DIRECTED_LINE_C::AddExtendPoint(float pointX, float pointY)
{
    COORDINATE_2D_STRU stNewPoint = {pointX, pointY};

    AddExtendPoint(stNewPoint);
}

void MULT_DIRECTED_LINE_C::Clear(void)
{
    for (list<DIRECTED_LINE_C *>::iterator iter = lstDirLine_.begin();
         iter != lstDirLine_.end();
         iter++) {
        delete *iter;
    }

    lstDirLine_.clear();
}

bool MULT_DIRECTED_LINE_C::IsPointOnLeft(const COORDINATE_2D_STRU &stPoint)
{
    if (lstDirLine_.size() == 0) {
        return false;
    }

    for (list<DIRECTED_LINE_C *>::iterator iter = lstDirLine_.begin();
         iter != lstDirLine_.end();
         iter++) {
        DIRECTED_LINE_C *pobjDirLine = *iter;
        if (!pobjDirLine->IsPointOnLeft(stPoint)) {
            return false;
        }
    }

    return true;
}

BOOL ANY_POLYGON_C::IsValid(void) const
{
    return (vectEdgeGroup_.size() > 0);
}

BOOL ANY_POLYGON_C::IsPointIn(const COORDINATE_2D_STRU &stPoint) const
{
    R_ASSERT(vectEdgeGroup_.size() > 0, false);

    if (!objRectArea_.IsIn(stPoint)) {
        return false;
    }

    /* 根据计算从指定点开始沿x周正方向的射线与多边形相交的边的个数来判断是否在内部，
       个数为奇数表示在内部，个数为偶数表示在外部。 */
    /* 如果射线穿过顶点，该方法失效，判断出该场景后，尝试更换射线，如果多次后还是穿过顶点，返回false */
    static COORDINATE_2D_STRU astDirVertor[] = {{1.1, 0.0}, {0.0, -1.1}, {0.55, 0.5}, {0.55, -0.5}, {-1.1, 0.0}, {0.0, 1.1}, {-0.55, 0.5}, {-0.55, -0.5}};
    for (UINT32 loop = 0; loop < ARRAY_SIZE(astDirVertor); ++loop) {
        RAY_C objRay(stPoint, stPoint + astDirVertor[loop]);
        BOOL  isCrossVertex = false;
        BOOL  isInPolygon   = false;

        for (const EDGE_GROUP_STRU &stGroup : vectEdgeGroup_) {
            if (!objRay.IsIntersectWithArea(stGroup.objArea)) {
                continue;
            }

            for (const DIRECTED_LINE_SEGMENT_C &objEdge : stGroup.vectEdge) {
                if (objRay.IsPointOnRay(objEdge.GetEndPoint())) {
                    isCrossVertex = true;
                    //ST_LOG_INFO("Ray<(%.2f, %.2f), (%.2f, %.2f)> Cross Vertex(%.2f, %.2f).", stPoint.x, stPoint.y, astDirVertor[loop].x, astDirVertor[loop].y, objEdge.GetEndPoint().x, objEdge.GetEndPoint().y);
                    break;
                }

                if (objEdge.IsIntersectedWithRay(objRay)) {
                    isInPolygon = !isInPolygon;
                }
            }

            if (isCrossVertex) {
                break;
            }
        }

        if (!isCrossVertex) {
            return isInPolygon;
        }
    }

    ST_LOG_FATAL("Try Fail To Change Dir To Test.");
    return false;
}

BOOL ANY_POLYGON_C::IsIntersectWithLineSeg(const DIRECTED_LINE_SEGMENT_C &objLineSeg) const
{
    AREA_C objLineSegRectArea;
    objLineSeg.GetRectArea(objLineSegRectArea);

    for (const EDGE_GROUP_STRU &stEdgeGroup : vectEdgeGroup_) {
        if (!stEdgeGroup.objArea.IsExistOverlapArea(objLineSegRectArea)) {
            continue;
        }

        for (const DIRECTED_LINE_SEGMENT_C &objEdgeLineSeg : stEdgeGroup.vectEdge) {
            if (objEdgeLineSeg.IsIntersectedWithOtherLineSeg(objLineSeg)) {
                return true;
            }
        }
    }

    return false;
}

BOOL ANY_POLYGON_C::IsIntersectWithOtherPolygon(const ANY_POLYGON_C &objOtherPolygon) const
{
    R_ASSERT(vectEdgeGroup_.size() > 0, false);
    R_ASSERT(objOtherPolygon.vectEdgeGroup_.size() > 0, false);

    /* 先group与group比较，再lineseg与group比较，最后再用lineseg和lineseg精确比较 */
    for (const EDGE_GROUP_STRU &stOtherEdgeGroup : objOtherPolygon.vectEdgeGroup_) {
        if (!stOtherEdgeGroup.objArea.IsExistOverlapArea(objRectArea_)) {
            continue;
        }

        for (const EDGE_GROUP_STRU &stLocEdgeGroup : vectEdgeGroup_) {
            if (!stLocEdgeGroup.objArea.IsExistOverlapArea(stOtherEdgeGroup.objArea)) {
                continue;
            }

            for (const DIRECTED_LINE_SEGMENT_C &objOtherLineSeg : stOtherEdgeGroup.vectEdge) {
                AREA_C objOtherLineSegArea;
                objOtherLineSeg.GetRectArea(objOtherLineSegArea);
                if (!objOtherLineSegArea.IsExistOverlapArea(stLocEdgeGroup.objArea)) {
                    continue;
                }

                for (const DIRECTED_LINE_SEGMENT_C &objLocLineSeg : stLocEdgeGroup.vectEdge) {
                    if (!objLocLineSeg.IsIntersectedWithOtherLineSeg(objOtherLineSeg)) {
                        continue;
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

BOOL ANY_POLYGON_C::IsExistOverlapWithOtherPolygon(const ANY_POLYGON_C &objOtherPolygon) const
{
    R_ASSERT(vectEdgeGroup_.size() > 0, false);
    R_ASSERT(objOtherPolygon.vectEdgeGroup_.size() > 0, false);

    /* 判断两个多边形是否存在重叠 */
    /* 先判断两个多边形的边是否存在交叉，如果存在交叉则必然存在重叠 */
    if (IsIntersectWithOtherPolygon(objOtherPolygon)) {
        return true;
    }

    /* 再判断没有交叉时，还存在重叠，必然是一方被另一个全包围。所以任一顶点不在另一个多边形中则其不会被另一方包含。 */
    const EDGE_GROUP_STRU         &stLocFirstEdgeGroup    = this->vectEdgeGroup_.front();
    const DIRECTED_LINE_SEGMENT_C &objLocFirstEdge        = stLocFirstEdgeGroup.vectEdge.front();
    const COORDINATE_2D_STRU      &stLocFirstEdgeEndPoint = objLocFirstEdge.GetEndPoint();
    if (objOtherPolygon.IsPointIn(stLocFirstEdgeEndPoint)) {
        return true;
    }


    const EDGE_GROUP_STRU         &stOtherFirstEdgeGroup    = objOtherPolygon.vectEdgeGroup_.front();
    const DIRECTED_LINE_SEGMENT_C &objOtherFirstEdge        = stOtherFirstEdgeGroup.vectEdge.front();
    const COORDINATE_2D_STRU      &stOtherFirstEdgeEndPoint = objOtherFirstEdge.GetEndPoint();
    if (IsPointIn(stOtherFirstEdgeEndPoint)) {
        return true;
    }

    /* 即不交叉，一方又不被另一方包含，则不存在重叠 */
    return false;
}

BOOL ANY_POLYGON_C::IsCircleOut(const COORDINATE_2D_STRU &stCenter, float radius) const
{
    R_ASSERT(vectEdgeGroup_.size() > 0, false);

    if (IsPointIn(stCenter)) {
        return false;
    }
    CIRCULAR_C stCircle(stCenter, radius);
    for (const EDGE_GROUP_STRU &stGroup : vectEdgeGroup_) {
        for (const DIRECTED_LINE_SEGMENT_C &objEdge : stGroup.vectEdge) {
            const COORDINATE_2D_STRU &stLineStartPoint = objEdge.GetStartPoint();
            const COORDINATE_2D_STRU &stLineEndPoint   = objEdge.GetEndPoint();
            if (stCircle.IsIntersectedLineSegment(stLineStartPoint, stLineEndPoint)) {
                return false;
            }
        }
    }

    return true;
}

INT32 ANY_POLYGON_C::Reset(const std::vector<COORDINATE_2D_STRU> &vectVertice)
{
    Clear();

    INT32 rslt = MakePolygon(vectVertice);
    if (rslt != 0) {
        Clear();
        ST_LOG_ERR("MakePolygon() Fail.");
        return -1;
    }

    objRectArea_.Reset();
    for (const EDGE_GROUP_STRU &stEdgeGroup : vectEdgeGroup_) {
        objRectArea_.IncludeArea(stEdgeGroup.objArea);
    }

    return 0;
}

INT32 ANY_POLYGON_C::GetRectArea(AREA_C &objRectArea) const
{
    R_ASSERT(IsValid(), -1);

    objRectArea = objRectArea_;
    return 0;
}

void ANY_POLYGON_C::DspAllVertice(const std::vector<COORDINATE_2D_STRU> &vectVertice) const
{
    UINT32 verticeNum = UINT32(vectVertice.size());
    ST_LOG_INFO("Vertice Point Num = %u.", verticeNum);

    for (UINT32 loop = 0; loop < verticeNum; ++loop) {
        const COORDINATE_2D_STRU &stVertice = vectVertice[loop];
        ST_LOG_INFO("%3uth Verice x=%f, y=%f.", loop, stVertice.x, stVertice.y);
    }
}

void ANY_POLYGON_C::DivideEdgeGroup(const std::vector<COORDINATE_2D_STRU> &vectVertice, BOOL isClose, std::vector<EDGE_GROUP_STRU> &vectGroup) const
{
    /* 对顶点分组进行各边是否相交判断，从而提高效率，按照估算判断次数来计算最优分组数，0.2为估算的组间重叠率，5为区域比较和相交比较效率比 */
    /* 边数较多时才需要进行分组，如果分组，组数必须大于等于5，每组必须多于3个 */
    UINT32 edgeNum           = isClose ? UINT32(vectVertice.size() - 1) : UINT32(vectVertice.size());
    UINT32 edgeGroupNum      = (edgeNum < 15) ? 1 : UINT32(pow(double((0.5 + 0.2) * edgeNum * edgeNum * 5), 0.33333333));
           edgeGroupNum      = (edgeGroupNum < 5) ? 1 : edgeGroupNum;
    UINT32 maxEdgeNumInGroup = edgeNum / edgeGroupNum;
           maxEdgeNumInGroup = MAX(maxEdgeNumInGroup, 3);

    vectGroup.reserve(edgeNum / maxEdgeNumInGroup + 1);

    const COORDINATE_2D_STRU *pstLastVertice = &vectVertice.back();
    UINT32                    numInGroup     = 0;
    EDGE_GROUP_STRU           stGroup;
    stGroup.vectEdge.reserve(maxEdgeNumInGroup);
    stGroup.objArea.IncludePoint(*pstLastVertice);

    for (UINT32 loop = isClose ? 1 : 0; loop < vectVertice.size(); ++loop) {
        const COORDINATE_2D_STRU &stVertice = vectVertice[loop];
        stGroup.vectEdge.push_back(DIRECTED_LINE_SEGMENT_C(*pstLastVertice, stVertice));
        stGroup.objArea.IncludePoint(stVertice);

        ++numInGroup;
        if (numInGroup >= maxEdgeNumInGroup) {
            numInGroup = 0;

            vectGroup.push_back(stGroup);

            stGroup.vectEdge.clear();
            stGroup.objArea.Reset();
            stGroup.objArea.IncludePoint(stVertice);
        }

        pstLastVertice = &stVertice;
    }

    if (stGroup.vectEdge.size() > 0) {
        vectGroup.push_back(stGroup);
    }
}

BOOL ANY_POLYGON_C::IsExistEdgesIntersectInGroup(const EDGE_GROUP_STRU &stGroup, BOOL isEndClosed) const
{
    UINT32 edgeNum = UINT32(stGroup.vectEdge.size());
    R_ASSERT(edgeNum > 0, false);

    UINT32 endEdgeIdx = edgeNum - 1;
    for (UINT32 loop = 0; loop <= endEdgeIdx; ++loop) {
        const DIRECTED_LINE_SEGMENT_C &objEdge = stGroup.vectEdge[loop];

        /* 处理相邻的，非最后一个或者封闭时的最后一个都要做相邻边判断，封闭时最后一条和第一条是相邻边 */
        if ((loop < endEdgeIdx) || isEndClosed) {
            const DIRECTED_LINE_SEGMENT_C &objNextEdge = (loop < endEdgeIdx) ? stGroup.vectEdge[loop + 1] : stGroup.vectEdge[0];
            if (objEdge.IsPointOnLine(objNextEdge.GetEndPoint()) && (objEdge.IsPointOnLineSeg(objNextEdge.GetEndPoint()) || objNextEdge.IsPointOnLineSeg(objEdge.GetStartPoint()))) {
                ST_LOG_ERR("Neighbour Edge Overlap. Pre(%.2f, %.2f)-(%.2f, %.2f), Next(%.2f, %.2f)-(%.2f, %.2f).",
                           objEdge.GetStartPoint().x,     objEdge.GetStartPoint().y,
                           objEdge.GetEndPoint().x,       objEdge.GetEndPoint().y,
                           objNextEdge.GetStartPoint().x, objNextEdge.GetStartPoint().y,
                           objNextEdge.GetEndPoint().x,   objNextEdge.GetEndPoint().y);
                return true;
            }
        }

        /* 处理非相邻的，首尾封闭时，第一个和最后一个不是非相邻 */
        UINT32 endEdgeIdxOfNonAdjacent = (isEndClosed && (loop == 0)) ? (endEdgeIdx - 1) : endEdgeIdx;
        for (UINT32 loopOfNonAdjacent = loop + 2; loopOfNonAdjacent <= endEdgeIdxOfNonAdjacent; ++loopOfNonAdjacent) {
            const DIRECTED_LINE_SEGMENT_C &objNonAdjacentEdge = stGroup.vectEdge[loopOfNonAdjacent];
            if (objEdge.IsIntersectedWithOtherLineSeg(objNonAdjacentEdge)) {
                ST_LOG_ERR("NonAdjacent Edge Overlap. Pre(%.2f, %.2f)-(%.2f, %.2f), Next(%.2f, %.2f)-(%.2f, %.2f).",
                           objEdge.GetStartPoint().x,            objEdge.GetStartPoint().y,
                           objEdge.GetEndPoint().x,              objEdge.GetEndPoint().y,
                           objNonAdjacentEdge.GetStartPoint().x, objNonAdjacentEdge.GetStartPoint().y,
                           objNonAdjacentEdge.GetEndPoint().x,   objNonAdjacentEdge.GetEndPoint().y);
                return true;
            }
        }
    }

    return false;
}

BOOL ANY_POLYGON_C::IsExistEdgesIntersectBetweenSeparatedGroup(const EDGE_GROUP_STRU &stGroup1, const EDGE_GROUP_STRU &stGroup2) const
{
    if (!stGroup1.objArea.IsExistOverlapArea(stGroup2.objArea)) {
        return false;
    }

    for (const DIRECTED_LINE_SEGMENT_C &objEdge1 : stGroup1.vectEdge) {
        for (const DIRECTED_LINE_SEGMENT_C &objEdge2 : stGroup2.vectEdge) {
            if (objEdge1.IsIntersectedWithOtherLineSeg(objEdge2)) {
                ST_LOG_ERR("objEdge1(%.2f, %.2f)-(%.2f, %.2f), objEdge2(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
                           objEdge1.GetStartPoint().x, objEdge1.GetStartPoint().y,
                           objEdge1.GetEndPoint().x,   objEdge1.GetEndPoint().y,
                           objEdge2.GetStartPoint().x, objEdge2.GetStartPoint().y,
                           objEdge2.GetEndPoint().x,   objEdge2.GetEndPoint().y);
                return true;
            }
        }
    }

    return false;
}

BOOL ANY_POLYGON_C::IsExistEdgesIntersectBetweenConnectedGroup(const EDGE_GROUP_STRU &stFrontGroup, const EDGE_GROUP_STRU &stBackGroup, BOOL isEndClosed) const
{
    UINT32 frontGroupEdgeNum = UINT32(stFrontGroup.vectEdge.size());
    UINT32 backGroupEdgeNum  = UINT32(stBackGroup.vectEdge.size());
    R_ASSERT((frontGroupEdgeNum > 0) && (backGroupEdgeNum > 0), false);

    /* 前一个分组的最后一条和后一个分组的第一条为相邻边，需要判断是否共线重叠 */
    const DIRECTED_LINE_SEGMENT_C &objFrontGroupBackEdge = stFrontGroup.vectEdge.back();
    const DIRECTED_LINE_SEGMENT_C &objBackGroupFrontEdge = stBackGroup.vectEdge.front();
    if (   objFrontGroupBackEdge.IsPointOnLine(objBackGroupFrontEdge.GetEndPoint())
        && (   objFrontGroupBackEdge.IsPointOnLineSeg(objBackGroupFrontEdge.GetEndPoint())
            || objBackGroupFrontEdge.IsPointOnLineSeg(objFrontGroupBackEdge.GetStartPoint()))) {
            ST_LOG_ERR("objFrontGroupBackEdge(%.2f, %.2f)-(%.2f, %.2f), objBackGroupFrontEdge(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
                           objFrontGroupBackEdge.GetStartPoint().x, objFrontGroupBackEdge.GetStartPoint().y,
                           objFrontGroupBackEdge.GetEndPoint().x,   objFrontGroupBackEdge.GetEndPoint().y,
                           objBackGroupFrontEdge.GetStartPoint().x, objBackGroupFrontEdge.GetStartPoint().y,
                           objBackGroupFrontEdge.GetEndPoint().x,   objBackGroupFrontEdge.GetEndPoint().y);
        return true;
    }


    /* 前一个分组的非最后一条和后一个分组进行比较 */
    for (UINT32 loop = 0; loop < (frontGroupEdgeNum - 1); ++loop) {
        const DIRECTED_LINE_SEGMENT_C &objFrontGroupEdge = stFrontGroup.vectEdge[loop];

        for (const DIRECTED_LINE_SEGMENT_C &objBackGroupEdge : stBackGroup.vectEdge) {
            if (objFrontGroupEdge.IsIntersectedWithOtherLineSeg(objBackGroupEdge)) {
                ST_LOG_ERR("objFrontGroupEdge(%.2f, %.2f)-(%.2f, %.2f), objBackGroupEdge(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
                           objFrontGroupEdge.GetStartPoint().x, objFrontGroupEdge.GetStartPoint().y,
                           objFrontGroupEdge.GetEndPoint().x,   objFrontGroupEdge.GetEndPoint().y,
                           objBackGroupEdge.GetStartPoint().x,  objBackGroupEdge.GetStartPoint().y,
                           objBackGroupEdge.GetEndPoint().x,    objBackGroupEdge.GetEndPoint().y);
                return true;
            }
        }
    }

    /* 前一个分组的最后一条和后一个分组的非最前一条进行比价 */
    for (UINT32 loop = 1; loop < backGroupEdgeNum; ++loop) {
        const DIRECTED_LINE_SEGMENT_C &objBackGroupEdge = stBackGroup.vectEdge[loop];
        if (objFrontGroupBackEdge.IsIntersectedWithOtherLineSeg(objBackGroupEdge)) {
            ST_LOG_ERR("objFrontGroupBackEdge(%.2f, %.2f)-(%.2f, %.2f), stBackGroup(%.2f, %.2f)-(%.2f, %.2f) Intersected.",
                       objFrontGroupBackEdge.GetStartPoint().x, objFrontGroupBackEdge.GetStartPoint().y,
                       objFrontGroupBackEdge.GetEndPoint().x,   objFrontGroupBackEdge.GetEndPoint().y,
                       objBackGroupEdge.GetStartPoint().x,      objBackGroupEdge.GetStartPoint().y,
                       objBackGroupEdge.GetEndPoint().x,        objBackGroupEdge.GetEndPoint().y);
            return true;
        }
    }

    return false;
}

BOOL ANY_POLYGON_C::IsExistEdgesIntersected(const std::vector<EDGE_GROUP_STRU> &vectGroup) const
{
    UINT32 groupNum = UINT32(vectGroup.size());
    R_ASSERT(groupNum > 0, false);

    /* 只有一组时，组需要检测组内相交 */
    if (groupNum == 1) {
        return IsExistEdgesIntersectInGroup(vectGroup[0], true);
    }

    /* 多组时，先检测组内相交，再检查组间相交 */
    /* 先检查每个分组内部是否存在相交 */
    for (const EDGE_GROUP_STRU &stGroup : vectGroup) {
        if (IsExistEdgesIntersectInGroup(stGroup, false)) {
            ST_LOG_ERR("IsExistEdgesIntersectInGroup() True.");
            return true;
        }
    }

    /* 再检查分组间是否存在相交 */
    /* 对于只有两组，则首尾两两相连 */
    if (groupNum == 2) {
        return IsExistEdgesIntersectBetweenConnectedGroup(vectGroup[0], vectGroup[1], true);
    }

    /* 对于多组，要考虑第一组和最后一组的相交情况 */
    UINT32 endGroupIdx = groupNum - 1;
    for (UINT32 loop = 0; loop <= endGroupIdx; ++loop) {
        const EDGE_GROUP_STRU &stGroup = vectGroup[loop];

        /* 相邻组的 */
        const EDGE_GROUP_STRU &stNextGroup = (loop < endGroupIdx) ? vectGroup[loop + 1] : vectGroup[0];
        if (IsExistEdgesIntersectBetweenConnectedGroup(stGroup, stNextGroup, false)) {
            ST_LOG_ERR("IsExistEdgesIntersectBetweenConnectedGroup() True.");
            return true;
        }

        /* 非相邻的 */
        UINT32 endGroupIdxOfNonAdjacent = (loop == 0) ? (endGroupIdx - 1) : endGroupIdx;
        for (UINT32 loopOfCompare = loop + 2; loopOfCompare <= endGroupIdxOfNonAdjacent; ++loopOfCompare) {
            if (IsExistEdgesIntersectBetweenSeparatedGroup(stGroup, vectGroup[loopOfCompare])) {
                ST_LOG_ERR("IsExistEdgesIntersectBetweenSeparatedGroup() True.");
                return true;
            }
        }
    }

    return false;
}

INT32 ANY_POLYGON_C::MakePolygon(const std::vector<COORDINATE_2D_STRU> &vectVertice)
{
    const COORDINATE_2D_STRU &stFirstVertice = vectVertice.front();
    const COORDINATE_2D_STRU &stEndVertice   = vectVertice.back();
    BOOL   isClose = stFirstVertice.IsAlmostEuqal(stEndVertice);
    UINT32 edgeNum = isClose ? UINT32(vectVertice.size() - 1) : UINT32(vectVertice.size());
    if (edgeNum < 3) {
        ST_LOG_ERR("Edge Num(%d) Is Not Enough.", edgeNum);
        DspAllVertice(vectVertice);
        return -1;
    }

    /* 判断是否存在相交边时，为防止所有边都要进行两两比较，将边进行分组，并按照分组处理，减小比较次数，提高效率 */
    DivideEdgeGroup(vectVertice, isClose, vectEdgeGroup_);

    if (IsExistEdgesIntersected(vectEdgeGroup_)) {
        ST_LOG_ERR("Exist Intersected Edges.");
        //DspAllVertice(vectVertice);
        return -1;
    }

    return 0;
}

void ANY_POLYGON_C::Clear(void)
{
    vectEdgeGroup_.clear();
}

CONVEX_POLYGON_C::CONVEX_POLYGON_C(COORDINATE_2D_STRU astPoint[], int pointNum)
{
    isExistFirstVertice_ = false;
    stLastVertice_.SetZero();

    INT32 rslt = Make(astPoint, pointNum);
    if (rslt != 0) {
        ST_LOG_ERR("Make Fail.");
    }
}

CONVEX_POLYGON_C::CONVEX_POLYGON_C(void)
{
    isExistFirstVertice_ = false;
    stLastVertice_.SetZero();
}

CONVEX_POLYGON_C::~CONVEX_POLYGON_C(void)
{
    Clear();
}

/**************************************************************************************
功能描述: 重新设置多边形
修改记录:
**************************************************************************************/
int CONVEX_POLYGON_C::Reset(COORDINATE_2D_STRU astPoint[], int pointNum)
{
    isExistFirstVertice_ = false;
    stLastVertice_.SetZero();

    Clear();

    return Make(astPoint, pointNum);
}

/**************************************************************************************
功能描述: 判断指定点是否在多边形内部
修改记录:
**************************************************************************************/
bool CONVEX_POLYGON_C::IsPointIn(const COORDINATE_2D_STRU &stPoint) const
{
    if (lstDirLineSeg_.empty()) {
        ST_LOG_ERR("No Polygon.");
        return false;
    }

    for (list<DIRECTED_LINE_SEGMENT_C *>::const_iterator iter = lstDirLineSeg_.begin();
         iter != lstDirLineSeg_.end();
         iter++) {
        DIRECTED_LINE_SEGMENT_C *pobjLineSeg = *iter;
        if (!pobjLineSeg->IsPointOnLeft(stPoint)) {
            return false;
        }
    }

    return true;
}

/**************************************************************************************
功能描述: 获取多边形内点沿某个方向到多边形边的长度
          调用方保证给定点一定为内点
修改记录:
**************************************************************************************/
float CONVEX_POLYGON_C::GetDistSquOfInternalPointAlongDirToEdge(const COORDINATE_2D_STRU &stInternalPoint, float dirAngle)
{
    RAY_C objRay(stInternalPoint, dirAngle);

    DIRECTED_LINE_SEGMENT_C *pObjIntersectedLineSeg = GetLineSegIntersectedByInternalRay(objRay);
    if (pObjIntersectedLineSeg == NULL) {
        ST_LOG_ERR("Can Not Find Inersected Seg.");
        return -1.0;
    }

    COORDINATE_2D_STRU stIntersection;
    if (0 != pObjIntersectedLineSeg->GetIntersectionWithAnotherLine(objRay, stIntersection)) {
        ST_LOG_ERR("Get Intersection Fail.");
        return -1.0;
    }

    return stInternalPoint.CalcDistSqu(stIntersection);
}

/**************************************************************************************
功能描述: 获取指定内点到多边形最近的距离
          由调用方保证给定点为多边形内点
          返回值为负表示异常，计算失败
修改记录:
**************************************************************************************/
float CONVEX_POLYGON_C::GetMinDistSquOfInternalPointToEdge(const COORDINATE_2D_STRU &stInternalPoint)
{
    float minDistSqu = -1.0;

    for (list<DIRECTED_LINE_SEGMENT_C *>::iterator iter = lstDirLineSeg_.begin();
         iter != lstDirLineSeg_.end();
         iter++) {
        DIRECTED_LINE_SEGMENT_C *pobjLineSeg = *iter;
        float distSqu = pobjLineSeg->GetDistSquToPoint(stInternalPoint);
        if ((minDistSqu < 0.0) || (distSqu < minDistSqu)) {
            minDistSqu = distSqu;
        }
    }

    return minDistSqu;
}

/**************************************************************************************
功能描述: 给多边形逆时针依次增加顶点
修改记录:
**************************************************************************************/
int CONVEX_POLYGON_C::AddVertice(COORDINATE_2D_STRU &stPoint)
{
    if (!isExistFirstVertice_) {
        stLastVertice_.Copy(stPoint);
        isExistFirstVertice_ = true;
        return 0;
    }

    /* 如果已存在边了，则需要校验下一个顶点是否合法，即在前一个边的左边，防止出现凹的，或不是顶点 */
    if (!lstDirLineSeg_.empty()) {
        DIRECTED_LINE_SEGMENT_C *pObjLastLineSeg = lstDirLineSeg_.back();
        if (pObjLastLineSeg == NULL) {
            ST_LOG_ERR("Get Last Seg Fail. Seg Num = %u.", UINT32(lstDirLineSeg_.size()));
            return -1;
        }

        if (!pObjLastLineSeg->IsPointOnLeft(stPoint)) {
            ST_LOG_ERR("New Vertice Not No Left.");
            return -1;
        }
    }

    DIRECTED_LINE_SEGMENT_C *pObjNewLineSeg = new DIRECTED_LINE_SEGMENT_C(stLastVertice_, stPoint);
    if (pObjNewLineSeg == NULL) {
        ST_LOG_ERR("Alloc New Line Seg Fail.");
        return -1;
    }

    lstDirLineSeg_.push_back(pObjNewLineSeg);
    stLastVertice_.Copy(stPoint);
    return 0;
}

/**************************************************************************************
功能描述: 检查该多边形是否合法
修改记录:
**************************************************************************************/
bool CONVEX_POLYGON_C::Check(void)
{
    /* 至少是3条边 */
    if (lstDirLineSeg_.size() < 3) {
        ST_LOG_ERR("Seg Num(%u) Is Less.", UINT32(lstDirLineSeg_.size()));
        return false;
    }

    /* 检查是否是封闭的 */
    DIRECTED_LINE_SEGMENT_C *pobjFirstLineSeg = lstDirLineSeg_.front();
    DIRECTED_LINE_SEGMENT_C *pobjLastLineSeg  = lstDirLineSeg_.back();
    const COORDINATE_2D_STRU &stFirstVertice = pobjFirstLineSeg->GetStartPoint();
    const COORDINATE_2D_STRU &stLastVertice  = pobjLastLineSeg->GetEndPoint();
    if (!stFirstVertice.IsAlmostEuqal(stLastVertice)) {
        ST_LOG_ERR("Not Closed.");
        return false;
    }

    /* 检查是否是凸的，即判断所有点都不在所有边的右边 */
    for (list<DIRECTED_LINE_SEGMENT_C *>::iterator iter1 = lstDirLineSeg_.begin();
         iter1 != lstDirLineSeg_.end();
         iter1++) {
        DIRECTED_LINE_SEGMENT_C *pobjLineSeg1 = *iter1;
        for (list<DIRECTED_LINE_SEGMENT_C *>::iterator iter2 = lstDirLineSeg_.begin();
             iter2 != lstDirLineSeg_.end();
             iter2++) {
            DIRECTED_LINE_SEGMENT_C *pobjLineSeg2 = *iter2;
            const COORDINATE_2D_STRU &stLineSegStartPoint = pobjLineSeg2->GetStartPoint();
            if (pobjLineSeg1->IsPointOnRight(stLineSegStartPoint)) {
                ST_LOG_ERR("Not Convex.");
                return false;
            }
        }
    }

    return true;
}

void CONVEX_POLYGON_C::Clear(void)
{
    for (list<DIRECTED_LINE_SEGMENT_C *>::iterator iter = lstDirLineSeg_.begin();
         iter != lstDirLineSeg_.end();
         iter++) {
        delete *iter;
    }

    lstDirLineSeg_.clear();
}

/**************************************************************************************
功能描述: 构造凸多边形
          各个顶点逆时针顺序构成多边形
修改记录:
**************************************************************************************/
INT32 CONVEX_POLYGON_C::Make(COORDINATE_2D_STRU astPoint[], int pointNum)
{
    R_ASSERT_1(pointNum >= 3, -1, pointNum);

    for (int loop = 0; loop < pointNum; loop++) {
        if (0 != AddVertice(astPoint[loop])) {
            ST_LOG_ERR("Add Vertice(Idx=%d) Fail.", loop);
            Clear();
            return -1;
        }
    }

    /* 如果最后一个点和第一个点不相等，则再增加第一个点，使多边形闭合 */
    if (!astPoint[0].IsAlmostEuqal(astPoint[pointNum - 1])) {
        if (0 != AddVertice(astPoint[0])) {
            ST_LOG_ERR("Add First Vertice Fail.");
            Clear();
            return -1;
        }
    }

    if (!Check()) {
        ST_LOG_ERR("Is Invalid.");
        Clear();
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取多边形内点沿某个方向与多边形的边相交，获取该边的线段
          调用方保证给定点一定为内点
修改记录:
**************************************************************************************/
DIRECTED_LINE_SEGMENT_C *CONVEX_POLYGON_C::GetLineSegIntersectedByInternalRay(RAY_C &objInternalRay)
{
    for (list<DIRECTED_LINE_SEGMENT_C *>::iterator iter = lstDirLineSeg_.begin();
         iter != lstDirLineSeg_.end();
         iter++) {
        DIRECTED_LINE_SEGMENT_C *pobjLineSeg = *iter;
        if (pobjLineSeg->IsIntersectedWithRay(objInternalRay)) {
            return pobjLineSeg;
        }
    }

    return NULL;
}

/**************************************************************************************
功能描述: 判断是否为凸包
修改记录:
**************************************************************************************/
bool CONVEX_HULL_C::IsConvexHull(const vector<COORDINATE_2D_STRU>& points) const
{
    if (points.size() < 3) return false;    // 至少需要3个顶点才能构成多边形
    if (points.size() == 3) return true;    // 三角形一定为凸多边形

    int size = points.size();
    bool hasNegativeTurn(false), hasPositiveTurn(false);

    for (int i = 0; i < size; ++i) {
        int j = (i + 1) % size;
        int k = (i + 2) % size;

        /* 计算向量 points[i] -> points[j] 和 points[j] -> points[k] */
        double x1 = points[j].x - points[i].x;
        double y1 = points[j].y - points[i].y;
        double x2 = points[k].x - points[j].x;
        double y2 = points[k].y - points[j].y;

        /* 计算叉积 */
        double crossProduct = x1 * y2 - y1 * x2;

        if (crossProduct < 0) {
            if (hasPositiveTurn) return false;  // 如果已经检测到顺时针转向，则不是凸多边形
            hasNegativeTurn = true;
        } else if (crossProduct > 0) {
            if (hasNegativeTurn) return false;  // 如果已经检测到逆时针转向，则不是凸多边形
            hasPositiveTurn = true;
        }
        // crossProduct == 0 的情况表示共线，在凸多边形中允许这种情况
    }

    /**
     * 如果没有检测到顺时针和逆时针的转向，则认为是凸多边形
     * 或者所有转向都是同向的（共线情况），也认为是凸多边形
     */
    return true;
}

/**************************************************************************************
功能描述: 计算凸包
修改记录:
**************************************************************************************/
vector<COORDINATE_2D_STRU> CONVEX_HULL_C::ConvexHull(const vector<COORDINATE_2D_STRU>& points) const
{
    vector<POINT_COORD_2D_STRU> vstPoint;
    vstPoint.reserve(points.size());
    for (const COORDINATE_2D_STRU& stCoor : points) vstPoint.emplace_back(POINT_COORD_2D_STRU(stCoor.x, stCoor.y));

    vector<POINT_COORD_2D_STRU> vstPointRslt = ConvexHullTurn(vstPoint);
    vector<COORDINATE_2D_STRU> vstCoor;
    vstCoor.reserve(vstPointRslt.size());
    for (const POINT_COORD_2D_STRU& stPoint : vstPointRslt) vstCoor.emplace_back(COORDINATE_2D_STRU(stPoint.x, stPoint.y));
    return vstCoor;
}

/**************************************************************************************
功能描述: 比较函数，用于极角排序
修改记录:
**************************************************************************************/
bool CONVEX_HULL_C::Compare(const POINT_COORD_2D_STRU& a, const POINT_COORD_2D_STRU& b, const POINT_COORD_2D_STRU& origin) const
{
    double cross = (a.x - origin.x) * (b.y - origin.y) - (a.y - origin.y) * (b.x - origin.x);
    if (SIG(cross) == 0) return origin.CalcDistSqu(a) < origin.CalcDistSqu(b); //处理共线情况
    return cross > 0;
}

/**************************************************************************************
功能描述: 计算凸包（转成双精度进行计算）
修改记录:
**************************************************************************************/
vector<POINT_COORD_2D_STRU> CONVEX_HULL_C::ConvexHullTurn(vector<POINT_COORD_2D_STRU>& points) const
{
    size_t n = points.size();
    if (n <= 3) return points; // 凸包就是所有点

    /* 找到最左下方的点 */
    int minIndex = 0;
    for (size_t i = 1; i < n; ++i) {
        if (points[i].y < points[minIndex].y || (points[i].y == points[minIndex].y && points[i].x < points[minIndex].x)) {
            minIndex = i;
        }
    }
    swap(points[0], points[minIndex]); // 将最左下方的点放到第一个位置

    /* 极角排序 */
    sort(points.begin() + 1, points.end(), [&](const POINT_COORD_2D_STRU& a, const POINT_COORD_2D_STRU& b) {
        return Compare(a, b, points[0]);
    });

    /* 构建凸包 */
    vector<POINT_COORD_2D_STRU> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    for (size_t i = 2; i < n; ++i) {
        while (hull.size() >= 2 && !Compare(points[i], hull[hull.size()-2], hull[hull.size()-1])) hull.pop_back();
        if (hull.back() == points[i]) continue; //出现重合的点
        hull.push_back(points[i]);
    }

    /* 输出凸包 */
    return hull;
}

/**************************************************************************************
功能描述: 根据三角形的两条边和夹角求第三条边长的平方
修改记录:
**************************************************************************************/
float TRIANGLE_C::Get3thSideLenSqu(float sideLen1, float sideLen2, ANGLE_C &objAngleOf2Sides)
{
    return (sideLen1 * sideLen1 + sideLen2 * sideLen2 - 2.0 * sideLen1 * sideLen2 * objAngleOf2Sides.Cos());
}

CIRCULAR_C::CIRCULAR_C(const COORDINATE_2D_STRU &stCenter, float radius)
{
    R_ASSERT(radius > 0.0, NOTHING);

    stCenter_       = stCenter;
    radius_         = radius;
    radiusSqu_      = radius * radius;

    isExistRadius_  = true;
    isValid_        = true;
}

CIRCULAR_C::CIRCULAR_C(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, const COORDINATE_2D_STRU &stPoint3)
{
    /* 三点不能太近 */
    COORDINATE_2D_STRU stVector12(stPoint2.x - stPoint1.x, stPoint2.y - stPoint1.y);
    COORDINATE_2D_STRU stVector23(stPoint3.x - stPoint2.x, stPoint3.y - stPoint2.y);
    COORDINATE_2D_STRU stVector13(stPoint3.x - stPoint1.x, stPoint3.y - stPoint1.y);
    if (   (stVector12.GetVectorLenSqu() < 1.0e-10)
        || (stVector23.GetVectorLenSqu() < 1.0e-10)
        || (stVector13.GetVectorLenSqu() < 1.0e-10)) {
        return;
    }

    /* 三点不能共线 */
    float det = stVector12.x * stVector23.y - stVector12.y * stVector23.x;
    if (ABS(det) < 1.0e-10) {
        return;
    }

    /* 计算圆心 */
    float e    = ((stPoint2.x * stPoint2.x - stPoint1.x * stPoint1.x) + (stPoint2.y * stPoint2.y - stPoint1.y * stPoint1.y)) * 0.5;
    float f    = ((stPoint2.x * stPoint2.x - stPoint3.x * stPoint3.x) + (stPoint2.y * stPoint2.y - stPoint3.y * stPoint3.y)) * 0.5;
    float detR = 1.0 / det;
    stCenter_.x =  (stVector23.y * e + stVector12.y * f) * detR;
    stCenter_.y = -(stVector23.x * e + stVector12.x * f) * detR;

    /* 计算半径 */
    radiusSqu_ = stCenter_.CalcDistSqu(stPoint1);

    isExistRadius_ = false;
    isValid_       = true;
}

BOOL CIRCULAR_C::IsValid(void) const
{
    return isValid_;
}

INT32 CIRCULAR_C::GetCenter(COORDINATE_2D_STRU &stCenter) const
{
    R_ASSERT(isValid_, -1);

    stCenter = stCenter_;
    return 0;
}

float CIRCULAR_C::GetRadiusSqu(void) const
{
    R_ASSERT(isValid_, -1);
    return radiusSqu_;
}

float CIRCULAR_C::GetRadius(void)
{
    R_ASSERT(isValid_, -1);

    if (!isExistRadius_) {
        radius_        = sqrt(radiusSqu_);
        isExistRadius_ = true;
    }

    return radius_;
}

INT32 CIRCULAR_C::GetSamplePoints(const COORDINATE_2D_STRU &stStartPoint, const COORDINATE_2D_STRU &stEndPoint, float deltaRadAbs, vector<COORDINATE_2D_STRU> &vectSamplePoints)
{
    R_ASSERT(isValid_, -1);

    VECTOR_C objVectorS(stCenter_, stStartPoint);
    VECTOR_C objVectorE(stCenter_, stEndPoint);

    float startAngle = objVectorS.GetDirAngle();
    float endAngle   = objVectorE.GetDirAngle();
    float diffAngle  = AdjustAngle(endAngle - startAngle);
    float symbol     = (diffAngle >= 0) ? 1.0 : -1.0;
    INT32 sampleNum  = INT32(ABS(diffAngle) / deltaRadAbs);
    float sinStart   = sin(startAngle);
    float cosStart   = cos(startAngle);
    float sinDelta   = sin(deltaRadAbs);
    float cosDelta   = cos(deltaRadAbs);
    float radius     = GetRadius();

    vectSamplePoints.reserve(sampleNum + 1);

    float sinSample = sinStart;
    float cosSample = cosStart;
    for (INT32 loop = 1; loop <= sampleNum; ++loop) {
        float sinSamplePre = sinSample;
        float cosSamplePre = cosSample;
        sinSample = sinSamplePre * cosDelta + cosSamplePre * sinDelta * symbol;
        cosSample = cosSamplePre * cosDelta - sinSamplePre * sinDelta * symbol;

        COORDINATE_2D_STRU stSamplePoint;
        stSamplePoint.x = stCenter_.x + radius * cosSample;
        stSamplePoint.y = stCenter_.y + radius * sinSample;

        vectSamplePoints.push_back(stSamplePoint);
    }

    vectSamplePoints.push_back(stEndPoint);
    return 0;
}

BOOL CIRCULAR_C::IsPointIn(const COORDINATE_2D_STRU &point) const {
    if (isValid_) {
        if ((point.x - stCenter_.x) * (point.x - stCenter_.x) + (point.y - stCenter_.y) * (point.y - stCenter_.y)
            - radiusSqu_ <= 0.001) {
            return true;
        }
    }
    return false;
}

BOOL CIRCULAR_C::IsIntersectedLineSegment(const COORDINATE_2D_STRU &stStartPoint,
                                          const COORDINATE_2D_STRU &stEndPoint) const {
    if (!isValid_) {
        return false;
    }
    if (IsPointIn(stStartPoint) && IsPointIn(stEndPoint)) {
        return false;
    }
    if ((!IsPointIn(stStartPoint) && IsPointIn(stEndPoint)) ||
        (IsPointIn(stStartPoint) && !IsPointIn(stEndPoint))) {
        return true;
    }

    double a, b, c, dist1, dist2, angle1, angle2; // ax + by + c = 0;
    if (std::fabs(stStartPoint.x - stEndPoint.x) < 0.001)
        a = 1, b = 0, c = -stStartPoint.x;
    else if (std::fabs(stStartPoint.y - stEndPoint.y) < 0.001)
        a = 0, b = 1, c = -stStartPoint.y;
    else {
        a = stStartPoint.y - stEndPoint.y;
        b = stEndPoint.x - stStartPoint.x;
        c = stStartPoint.x * stEndPoint.y - stStartPoint.y * stEndPoint.x;
    }
    dist1 = a * stCenter_.x + b * stCenter_.y + c;
    dist1 *= dist1;
    dist2 = (a * a + b * b) * radiusSqu_;
    if (dist1 > dist2) {
        return false;
    }
    angle1 = (stCenter_.x - stStartPoint.x) * (stEndPoint.x - stStartPoint.x) +
             (stCenter_.y - stStartPoint.y) * (stEndPoint.y - stStartPoint.y);
    angle2 = (stCenter_.x - stEndPoint.x) * (stStartPoint.x - stEndPoint.x) +
             (stCenter_.y - stEndPoint.y) * (stStartPoint.y - stEndPoint.y);
    if (angle1 > 0 && angle2 > 0) {
        return true;//余弦都为正，则是锐角
    }
    return false;
}

BOOL SECTOR_C::IsPointIn(const COORDINATE_2D_STRU &stPoint)
{
    if ((radius_ > 0) && (stCenter_.CalcDistSqu(stPoint) > (radius_ * radius_))) {
        return false;
    }

    if (isAngleRangePositive_) {
        if (isAngleRangeOverPi_) {
            return objBeginAngleLine_.IsPointOnLeft(stPoint) || objEndAngleLine_.IsPointOnRight(stPoint);
        } else {
            return objBeginAngleLine_.IsPointOnLeft(stPoint) && objEndAngleLine_.IsPointOnRight(stPoint);
        }
    } else {
        if (isAngleRangeOverPi_) {
            return objBeginAngleLine_.IsPointOnRight(stPoint) || objEndAngleLine_.IsPointOnLeft(stPoint);
        } else {
            return objBeginAngleLine_.IsPointOnRight(stPoint) && objEndAngleLine_.IsPointOnLeft(stPoint);
        }
    }
}


#endif


