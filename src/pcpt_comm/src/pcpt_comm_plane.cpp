#include "pcpt_comm/pcpt_comm_plane.h"

/**************************************************************************************
功能描述: 计算点到平面的距离
修改记录:
**************************************************************************************/
float PCPT_PLANE_C::GetDistByPlane(float x, float y, float z, const PLANE_PARAM_STRU &stPlane) const
{
    /* 平面法向量(stPlanePram.coeffX, stPlanePram.coeffY, stPlanePram.coeffZ) */
    return stPlane.coeffX * x + stPlane.coeffY * y + stPlane.coeffZ * z + stPlane.constH;
}

/**************************************************************************************
功能描述: 计算两个向量之间的夹角(弧度：0 ~ π)
修改记录: 向量A和向量B无先后顺序区分，所得结果都一样
**************************************************************************************/
float PCPT_PLANE_C::GetVectorIncludedAngle(const PCPT_VECTOR_STRU &A, const PCPT_VECTOR_STRU &B) const
{
    float l = A.x * B.x + A.y * B.y + A.z * B.z;
    float m = pow(A.x, 2) + pow(A.y, 2) + pow(A.z, 2);  // 平面参数归一化后m为1
    float n = pow(B.x, 2) + pow(B.y, 2) + pow(B.z, 2);  // 平面参数归一化后n为1
    float h = sqrt(m * n);                              // 平面参数归一化后h为1
    float theta = acos(l / h);                          // 弧度：0 ~ π
    return theta;
}

/**************************************************************************************
功能描述: 计算两平面的夹角，也即面的法向量的夹角(弧度：0 ~ 0.5π)
修改记录: PlaneA和PlaneB无先后顺序区分，所得结果都一样
**************************************************************************************/
float PCPT_PLANE_C::GetPlaneIncludedAngle(const PLANE_PARAM_STRU &stPlaneA, const PLANE_PARAM_STRU &stPlaneB) const
{
    PCPT_VECTOR_STRU stVecA(stPlaneA.coeffX, stPlaneA.coeffY, stPlaneA.coeffZ);  //平面法向量A
    PCPT_VECTOR_STRU stVecB(stPlaneB.coeffX, stPlaneB.coeffY, stPlaneB.coeffZ);  //平面法向量B
    float theta = GetVectorIncludedAngle(stVecA, stVecB);
    return theta < 0.5 * PI ? theta : PI - theta;
}

/**************************************************************************************
功能描述: 平面与X轴的夹角(弧度：0 ~ 0.5π)，标量
修改记录:
**************************************************************************************/
float PCPT_PLANE_C::GetPlaneIncludedAngleByAxisX(const PLANE_PARAM_STRU &stPlane) const
{
    PLANE_PARAM_STRU stPlaneRef(1, 0, 0, 0);                    // 以X轴为单位法向量
    float theta = GetPlaneIncludedAngle(stPlane, stPlaneRef);   // 计算两个面的夹角
    return 0.5 * PI - theta;                                    // 计算面与轴的夹角
}

/**************************************************************************************
功能描述: 平面与Y轴的夹角(弧度：0 ~ 0.5π)，标量
修改记录:
**************************************************************************************/
float PCPT_PLANE_C::GetPlaneIncludedAngleByAxisY(const PLANE_PARAM_STRU &stPlane) const
{
    PLANE_PARAM_STRU stPlaneRef(0, 1, 0, 0);                    // 以Y轴为单位法向量
    float theta = GetPlaneIncludedAngle(stPlane, stPlaneRef);   // 计算两个面的夹角
    return 0.5 * PI - theta;                                    // 计算面与轴的夹角
}

/**************************************************************************************
功能描述: 平面与Z轴的夹角(弧度：0 ~ 0.5π)，标量
修改记录:
**************************************************************************************/
float PCPT_PLANE_C::GetPlaneIncludedAngleByAxisZ(const PLANE_PARAM_STRU &stPlane) const
{
    PLANE_PARAM_STRU stPlaneRef(0, 0, 1, 0);                    // 以Z轴为单位法向量
    float theta = GetPlaneIncludedAngle(stPlane, stPlaneRef);   // 计算两个面的夹角
    return 0.5 * PI - theta;                                    // 计算面与轴的夹角
}

/**************************************************************************************
功能描述: 计算两个向量之间的旋转矩阵（基于罗德里格旋转公式）
修改记录:
**************************************************************************************/
void PCPT_PLANE_C::GetVectorRotMatrix(const PCPT_VECTOR_STRU &stBef, const PCPT_VECTOR_STRU &stAft, Eigen::Matrix3d& rotMatrix) const
{
    Eigen::Vector3d vectorBef(stBef.x, stBef.y, stBef.z);
    Eigen::Vector3d vectorAft(stAft.x, stAft.y, stAft.z);
    rotMatrix = Eigen::Quaterniond::FromTwoVectors(vectorBef, vectorAft).toRotationMatrix();
}

/**************************************************************************************
功能描述: 计算两个向量之间的欧拉角（基于罗德里格旋转公式）
修改记录:
**************************************************************************************/
void PCPT_PLANE_C::GetVectorEulerAngle(const PCPT_VECTOR_STRU &stBef, const PCPT_VECTOR_STRU &stAft, Eigen::Vector3d& eulerAngle) const
{
    Eigen::Vector3d vectorBef(stBef.x, stBef.y, stBef.z);
    Eigen::Vector3d vectorAft(stAft.x, stAft.y, stAft.z);
    eulerAngle = Eigen::Quaterniond::FromTwoVectors(vectorBef, vectorAft).matrix().eulerAngles(2,1,0);  //2:roll 1:pitch 0:yaw
}

/**************************************************************************************
功能描述: 获取车体坐标系下平面的roll pitch yaw
修改记录:
**************************************************************************************/
void PCPT_PLANE_C::GetPlaneRPY(const PLANE_PARAM_STRU &stPlane, ATTITUDE_STRU& stAttitude) const
{
    PCPT_VECTOR_STRU stBef(0, 0, 1);
    PCPT_VECTOR_STRU stAft(stPlane.coeffX, stPlane.coeffY, stPlane.coeffZ);
    Eigen::Vector3d eulerAngle;
    GetVectorEulerAngle(stBef, stAft, eulerAngle);
    stAttitude.roll  = eulerAngle[2];
    stAttitude.pitch = eulerAngle[1];
    stAttitude.yaw   = eulerAngle[0];
}

/**************************************************************************************
功能描述: 平面参数归一化
修改记录:
**************************************************************************************/
void PCPT_PLANE_C::PlaneParamNormalization(PLANE_PARAM_STRU& stPlane) const
{
    double sqrtV = sqrt(pow(stPlane.coeffX, 2) + pow(stPlane.coeffY, 2) + pow(stPlane.coeffZ, 2));
    if (sqrtV - 1 > 1e-8) {
        PLANE_PARAM_STRU stPlaneNor;
        stPlaneNor.coeffX = stPlane.coeffX / sqrtV;
        stPlaneNor.coeffY = stPlane.coeffY / sqrtV;
        stPlaneNor.coeffZ = stPlane.coeffZ / sqrtV;
        stPlane = stPlaneNor;
    }
}

/**************************************************************************************
功能描述: 设置平面拟合区域
修改记录:
**************************************************************************************/
template<class T>
int PCPT_MULTI_PLANE_C<T>::SetRectangle(RECTANGLE_STRU& stRectangle, float unitX, float unitY)
{
    int sizeX = (stRectangle.maxX - stRectangle.minX) / unitX + 0.5;
    if (sizeX <= 0) {
        ST_LOG_INFO("Rectangle parameter X setting Error.");
        return -1;
    }

    int sizeY = (stRectangle.maxY - stRectangle.minY) / unitY + 0.5;
    if (sizeY <= 0) {
        ST_LOG_INFO("Rectangle parameter Y setting Error.");
        return -1;
    }

    /* 根据unitX和unitY适当调试矩形框stRectangle的尺寸 */
    float midX = (stRectangle.maxX + stRectangle.minX) * 0.5;
    stRectangle.minX = midX - sizeX * unitX * 0.5;
    stRectangle.maxX = midX + sizeX * unitX * 0.5;

    float midY = (stRectangle.maxY + stRectangle.minY) * 0.5;
    stRectangle.minY = midY - sizeY * unitY * 0.5;
    stRectangle.maxY = midY + sizeY * unitY * 0.5;

    stRectangle_ = stRectangle;

    /* 根据unitX和unitY划分矩形框stRectangle的为多个子框 */
    for (int indexY = 0; indexY < sizeY; ++indexY) {
        for (int indexX = 0; indexX < sizeX; ++indexX) {
            RECTANGLE_STRU stSubRect;
            stSubRect.minX = indexX * unitX;
            stSubRect.maxX = stSubRect.minX + unitX;
            stSubRect.minY = indexY * unitY;
            stSubRect.maxY = stSubRect.minY + unitY;
            vstSubRect_.push_back(stSubRect);
        }
    }

    /* 判断是否需要调整子框的数量 */
    if (vstSubRect_.size() != subRectSize_) {
        subRectSize_ = vstSubRect_.size();
        vpCloud_.resize(subRectSize_);
    }

    return 0;
}

/**************************************************************************************
功能描述: 拟合平面参数
修改记录:
**************************************************************************************/
template<class T>
int PCPT_MULTI_PLANE_C<T>::FittingPlaneParam(T& pCloud, int recursionTimes, vector<PLANE_PARAM_STRU>& vstPlane)
{
    /* 将vpPoints_作为成员变量，可降低cpu资源占用 */
    for (auto &pCloudT : vpCloud_) pCloudT->clear();

    for (auto &point : pCloud->points) {
        if(!stRectangle_.IsIn(point.x, point.y)) continue;
        for (uint32_t index = 0; index < vstSubRect_.size(); ++ index) {
            RECTANGLE_STRU& stSubRect = vstSubRect_[index];
            if (!stSubRect.IsIn(point.x, point.y)) continue;
            vpCloud_[index]->push_back(point);
            break;
        }
    }

    for (auto &pCloudT : vpCloud_) {
        PLANE_PARAM_STRU stPlane;
        /* 地面点比较少的时候不进行平面拟合 or 地面拟合失败时 */
        if (pCloudT->size() < 20 || FittingPlaneParam(pCloudT, 0.2, stPlane)) {
            stPlane.SetZero();
            vstPlane.push_back(stPlane);
            continue;
        }
        vstPlane.push_back(stPlane);
    }

    return 0;
}
