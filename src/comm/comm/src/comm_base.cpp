#include "comm/comm_base.h"
#include "comm/comm_test_base.h"
#include "comm/comm_car.h"
#include "comm/comm_define.h"

#include "comm/comm_serial.h"
#include "comm/comm_cfg/comm_cfg_param_file.h"

#include <math.h>
#include <iostream>
#include <list>
#include <vector>
#include <stdio.h>

/**************************************************************************************
功能描述: 基础模块参数
          初始化不允许修改
修改记录:
**************************************************************************************/
struct COMM_BASE_RUN_PARA_STRU {
    BOOL isInitSucc;
    BOOL isSimulationMode;
};

static COMM_BASE_RUN_PARA_STRU g_stCommBaseRunPara = {false};  // 基础模块运行参数（用于确保基础模块启动）


static void comm_SetSimulationMode(void)
{
    ST_LOG_INFO("Set Simulation Mode.");

    g_stCommBaseRunPara.isSimulationMode = true;
}

BOOL Comm_IsSimulationMode(void)
{
    return g_stCommBaseRunPara.isSimulationMode;
};


/**************************************************************************************
功能描述: 打印码流
修改记录:
**************************************************************************************/
void Comm_PrintStream(const CHAR *pucDes, const UINT08 *pucStream, UINT32 streamLen)
{
    R_ASSERT(pucDes != NULL, NOTHING);
    R_ASSERT(pucStream != NULL, NOTHING);

    ST_LOG_INFO("\n[%s] Begin Stream(Len=%u) Print: ", pucDes, streamLen);

    for (UINT32 loop = 0; loop < streamLen; ++loop) {
        if ((loop % 16) == 0) {
            printf("\n%p: ", pucStream + loop);
        }

        printf(" %02x", pucStream[loop]);
    }

    printf("\n[%s] End\n\n", pucDes);
}

std::string comm_ConvertCvMatTypeToStr(INT32 type)
{
    switch (type) {
        case CV_8UC1:
            return "CV_8UC1";

        case CV_8UC3:
            return "CV_8UC3";

        case CV_32FC1:
            return "CV_32FC1";

        case CV_32FC3:
            return "CV_32FC3";

        case CV_64FC1:
            return "CV_64FC1";

        case CV_64FC3:
            return "CV_64FC3";

        default:
            return std::string("Unkown_") + ToString(UINT32(type));
    }
}

void Comm_PrintCvVec3dInfo(const std::string &strDes, const cv::Vec3d &objVec)
{
    ST_LOG_INFO("< %s > [%.5f, %.5f, %.5f]", strDes.c_str(), objVec[0], objVec[1], objVec[2]);
}

void Comm_PrintCvMatInfo(const std::string &strDes, const cv::Mat &objMat)
{
    INT32       dataType    = objMat.type();
    std::string strDataType = comm_ConvertCvMatTypeToStr(dataType);

    ST_LOG_INFO("< %s > < %u * %u %s >", strDes.c_str(), objMat.rows, objMat.cols, strDataType.c_str());
    if ((objMat.rows > 10) || (objMat.cols > 10) || ((dataType != CV_8UC1) && (dataType != CV_32FC1) && (dataType != CV_64FC1))) {
        return;
    }

    for (INT32 row = 0; row < objMat.rows; ++row) {
        std::string strLine;

        for (INT32 col = 0; col < objMat.cols; ++col) {
            if (col != 0) {
                strLine += " ";
            }

            if (dataType == CV_8UC1) {
                UINT32 pixelVal = objMat.at<UINT08>(row, col);
                strLine += ToString(pixelVal, 3, ' ');
            } else if (dataType == CV_32FC1) {
                double pixelVal = objMat.at<float>(row, col);
                strLine += DoubleToString(pixelVal, 5, 10, ' ');
            } else {
                double pixelVal = objMat.at<double>(row, col);
                strLine += DoubleToString(pixelVal, 5, 10, ' ');
            }
        }

        ST_LOG_INFO("%s", strLine.c_str());
    }
}

void Comm_PrintEgVec3dInfo(const std::string &strDes, const Eigen::Vector3d &objVec)
{
    ST_LOG_INFO("< %s > [%.5f, %.5f, %.5f]", strDes.c_str(), objVec[0], objVec[1], objVec[2]);
}

float CalcDistSqu(const COORDINATE_2D_STRU &stPos1, const COORDINATE_2D_STRU &stPos2)
{
    float diffX = stPos1.x - stPos2.x;
    float diffY = stPos1.y - stPos2.y;

    return (diffX * diffX + diffY * diffY);
}

float CalcAngleDiffAbs(float angle1, float angle2)
{
    float angleDiffAbsPre = ABS(angle1 - angle2);
    return (angleDiffAbsPre > PI) ? (2.0 * PI - angleDiffAbsPre) : angleDiffAbsPre;
}

/* 计算连点连线与X轴的夹角，从-PI到PI，连线方向为1->2 */
float Calc2PointLineAngle(const COORDINATE_2D_STRU *pstPos1, const COORDINATE_2D_STRU *pstPos2)
{
    return atan2(pstPos2->y - pstPos1->y, pstPos2->x - pstPos1->x);
}

int GetPosQuadrantWeight(float x, float y) {
    if (x >= 0) {
        if (y >= 0) {
            return 3;
        } else {
            return 2;
        }
    } else {
        if (y >= 0) {
            return 4;
        } else {
            return 1;
        }
    }
}

/* 比较两条射线方向角的大小 */
bool ComparePointToPointRelAngle(COORDINATE_2D_STRU *pstPosBase, COORDINATE_2D_STRU *pstPos1, COORDINATE_2D_STRU *pstPos2)
{
    float pos1diffX = pstPos1->x - pstPosBase->x;
    float pos1diffY = pstPos1->y - pstPosBase->y;
    float pos2diffX = pstPos2->x - pstPosBase->x;
    float pos2diffY = pstPos2->y - pstPosBase->y;

    int pos1W = GetPosQuadrantWeight(pos1diffX, pos1diffY);
    int pos2W = GetPosQuadrantWeight(pos2diffX, pos2diffY);
    if (pos1W == pos2W) {
        return (pos1diffY * pos2diffX > pos2diffY * pos1diffX);
    } else {
        return (pos1W > pos2W);
    }
}

/* 调整角度到-PI ~ PI */
float AdjustAngle(float srcAngle)
{
    if        (srcAngle > PI) {
        return (srcAngle - 2.0 * PI);
    } else if (srcAngle < (-PI)) {
        return (srcAngle + 2.0 * PI);
    } else {
        return srcAngle;
    }
}

/* 角度归一化-Pi - PI*/
float NormalizeAngle(float angle) {
  float a = std::fmod(angle + PI, 2.0 * PI);
  if (a < 0.0) {
    a += (2.0 * PI);
  }
  return a - PI;
}

/**************************************************************************************
功能描述: 拟合平面 ax+by=z
修改记录:
**************************************************************************************/
INT32 Comm_FitPlaneOfAxByEquZ(const std::vector<POINT_COORD_3D_STRU> &vectToFitPoint, POINT_COORD_2D_STRU &stParam)
{
    R_ASSERT(vectToFitPoint.size() >= 3, -1);
    double sumOfXX = 0;
    double sumOfYY = 0;
    double sumOfXY = 0;
    double sumOfXZ = 0;
    double sumOfYZ = 0;
    for (UINT32 loop = 0; loop < vectToFitPoint.size(); ++loop) {
        const POINT_COORD_3D_STRU &stItem = vectToFitPoint[loop];
        sumOfXX += stItem.x * stItem.x;
        sumOfYY += stItem.y * stItem.y;
        sumOfXY += stItem.x * stItem.y;
        sumOfXZ += stItem.x * stItem.z;
        sumOfYZ += stItem.y * stItem.z;
    }

    double denominator = sumOfXX * sumOfYY - sumOfXY * sumOfXY;
    R_ASSERT(ABS(denominator) > 1e-36, -1);
    double denominatorReciprocal = 1.0 / denominator;

    stParam.x = (sumOfXZ * sumOfYY - sumOfYZ * sumOfXY) * denominatorReciprocal;
    stParam.y = (sumOfYZ * sumOfXX - sumOfXZ * sumOfXY) * denominatorReciprocal;
    R_ASSERT((stParam.x >= 0) && (stParam.y >= 0), -1);

    return 0;
}

COOR_TF_C::COOR_TF_C(void)
{
    COORDINATE_2D_STRU stOriginPosByFather = {0.0, 0.0};
    SetTransRelationship(stOriginPosByFather, 0.0);
}

COOR_TF_C::COOR_TF_C(const COORDINATE_2D_STRU &stSonOriginPosByFather, float sonDirAngleByFather)
{
    SetTransRelationship(stSonOriginPosByFather, sonDirAngleByFather);
}

COOR_TF_C::COOR_TF_C(const COOR_2D_POSE_STRU &stSonOriginPoseByFather)
{
    SetTransRelationship(stSonOriginPoseByFather.stPos, stSonOriginPoseByFather.dirAngle);
}

COOR_TF_C::COOR_TF_C(float sonDirAngleByFather)
{
    COORDINATE_2D_STRU stSonOrigin;
    stSonOrigin.SetZero();

    SetTransRelationship(stSonOrigin, sonDirAngleByFather);
}

void COOR_TF_C::SetTransRelationship(const COORDINATE_2D_STRU &stSonOriginPosByFather, float sonDirAngleByFather)
{
    stSonOriginPoseByFather_.stPos.Copy(stSonOriginPosByFather);
    stSonOriginPoseByFather_.dirAngle = sonDirAngleByFather;

    sonDirAngleByFatherSin_ = sin(sonDirAngleByFather);
    sonDirAngleByFatherCos_ = cos(sonDirAngleByFather);
}

void COOR_TF_C::SetTransRelationship(const COORDINATE_2D_STRU &stSonOriginPosByFather, float sonDirAngleByFather, float sonDirAngleByFatherSin, float sonDirAngleByFatherCos)
{
    stSonOriginPoseByFather_.stPos.Copy(stSonOriginPosByFather);
    stSonOriginPoseByFather_.dirAngle = sonDirAngleByFather;

    sonDirAngleByFatherSin_ = sonDirAngleByFatherSin;
    sonDirAngleByFatherCos_ = sonDirAngleByFatherCos;
}

void COOR_TF_C::SetTransRelationship(const COOR_2D_POSE_STRU &stSonOriginPoseByFather)
{
    SetTransRelationship(stSonOriginPoseByFather.stPos, stSonOriginPoseByFather.dirAngle);
}

/**************************************************************************************
功能描述: 设置父子坐标系转换关系
          通过设置同一个位姿在父子坐标系的表示来确认
修改记录:
**************************************************************************************/
void COOR_TF_C::SetTransRelationship(const COOR_2D_POSE_STRU &stPoseInFather, const COOR_2D_POSE_STRU &stPoseInSon)
{
    stSonOriginPoseByFather_.dirAngle = AdjustAngle(stPoseInFather.dirAngle - stPoseInSon.dirAngle);

    sonDirAngleByFatherSin_ = sin(stSonOriginPoseByFather_.dirAngle);
    sonDirAngleByFatherCos_ = cos(stSonOriginPoseByFather_.dirAngle);

    stSonOriginPoseByFather_.stPos.x = stPoseInFather.stPos.x - sonDirAngleByFatherCos_ * stPoseInSon.stPos.x + sonDirAngleByFatherSin_ * stPoseInSon.stPos.y;
    stSonOriginPoseByFather_.stPos.y = stPoseInFather.stPos.y - sonDirAngleByFatherSin_ * stPoseInSon.stPos.x - sonDirAngleByFatherCos_ * stPoseInSon.stPos.y;
}

/**************************************************************************************
功能描述: 设置父子坐标系转换关系
          通过设置两个点的在父子坐标中的关系来确认
          由调用方保证坐标系间只旋转和平移，无伸缩
修改记录:
**************************************************************************************/
void COOR_TF_C::SetTransRelationship(const COORDINATE_2D_STRU &stPos1InFather, const COORDINATE_2D_STRU &stPos1InSon,
                                     const COORDINATE_2D_STRU &stPos2InFather, const COORDINATE_2D_STRU &stPos2InSon)
{
    float xDiffInSon    = stPos2InSon.x    - stPos1InSon.x;
    float yDiffInSon    = stPos2InSon.y    - stPos1InSon.y;
    float xDiffInFather = stPos2InFather.x - stPos1InFather.x;
    float yDiffInFather = stPos2InFather.y - stPos1InFather.y;

    float distSqu = xDiffInSon * xDiffInSon + yDiffInSon * yDiffInSon;
    distSqu = MAX(distSqu, 0.0000000001);
    sonDirAngleByFatherSin_ = (yDiffInFather * xDiffInSon - xDiffInFather * yDiffInSon) / distSqu;
    sonDirAngleByFatherCos_ = (xDiffInFather * xDiffInSon + yDiffInFather * yDiffInSon) / distSqu;

    stSonOriginPoseByFather_.stPos.x  = stPos1InFather.x - sonDirAngleByFatherCos_ * stPos1InSon.x + sonDirAngleByFatherSin_ * stPos1InSon.y;
    stSonOriginPoseByFather_.stPos.y  = stPos1InFather.y - sonDirAngleByFatherSin_ * stPos1InSon.x - sonDirAngleByFatherCos_ * stPos1InSon.y;
    stSonOriginPoseByFather_.dirAngle = atan2(sonDirAngleByFatherSin_, sonDirAngleByFatherCos_);
}

void COOR_TF_C::TransSonPosToFather(const COORDINATE_2D_STRU &stSonPos, COORDINATE_2D_STRU &stFatherPos) const
{
    stFatherPos.x = stSonOriginPoseByFather_.stPos.x + sonDirAngleByFatherCos_ * stSonPos.x - sonDirAngleByFatherSin_ * stSonPos.y;
    stFatherPos.y = stSonOriginPoseByFather_.stPos.y + sonDirAngleByFatherSin_ * stSonPos.x + sonDirAngleByFatherCos_ * stSonPos.y;
}

void COOR_TF_C::TransFatherPosToSon(const COORDINATE_2D_STRU &stFatherPos, COORDINATE_2D_STRU &stSonPos) const
{
    float diffX = stFatherPos.x - stSonOriginPoseByFather_.stPos.x;
    float diffY = stFatherPos.y - stSonOriginPoseByFather_.stPos.y;

    stSonPos.x =        sonDirAngleByFatherCos_ * diffX + sonDirAngleByFatherSin_ * diffY;
    stSonPos.y = -1.0 * sonDirAngleByFatherSin_ * diffX + sonDirAngleByFatherCos_ * diffY;
}

void COOR_TF_C::TransSonPoseToFather(const COOR_2D_POSE_STRU &stSonPose, COOR_2D_POSE_STRU &stFatherPose) const
{
    TransSonPosToFather(stSonPose.stPos, stFatherPose.stPos);

    stFatherPose.dirAngle = AdjustAngle(stSonOriginPoseByFather_.dirAngle + stSonPose.dirAngle);
}

void COOR_TF_C::TransFatherPoseToSon(const COOR_2D_POSE_STRU &stFatherPose, COOR_2D_POSE_STRU &stSonPose) const
{
    TransFatherPosToSon(stFatherPose.stPos, stSonPose.stPos);

    stSonPose.dirAngle = AdjustAngle(stFatherPose.dirAngle - stSonOriginPoseByFather_.dirAngle);
}

float COOR_TF_C::TransSonAngleToFather(float sonAngle) const
{
    return AdjustAngle(stSonOriginPoseByFather_.dirAngle + sonAngle);
}

float COOR_TF_C::TransFatherAngleToSon(float fatherAngle) const
{
    return AdjustAngle(fatherAngle - stSonOriginPoseByFather_.dirAngle);
}

void COOR_TF_C::GetSonOriginPoseByFather(COOR_2D_POSE_STRU &stSonOriginPoseByFather) const
{
    stSonOriginPoseByFather.Copy(stSonOriginPoseByFather_);
}

#ifndef _______Main___________
#endif

CYCLE_LEN_STATIS_C *g_pobjCommMainCycleLenStatis{NULL};   // 主循环周期统计器

/**************************************************************************************
功能描述: 设置运行选项
修改记录:
**************************************************************************************/
static void comm_SetRuningOption(int argc, char** argv)
{
    if (IsSerialSimulationMode(argc, argv)) {
        SERIAL_C::SetSimulation();
        comm_SetSimulationMode();
    }

    if (IsIgnoreCfgParamFileModChecking(argc, argv)) {
        COMM::CFG::CFG_PARAM_FILE_MNG_C::SetIgnoreCfgParamChecking();
    }
}

/**************************************************************************************
功能描述: 模块初始化
修改记录:
**************************************************************************************/
int Comm_Init(int runFrequency, float abnCycleDiffRate)
{
    ST_LOG_INFO("***************** Env Config Para ********************");
    ST_LOG_INFO("               Software Version: %s", LOC_VERSION);
    ST_LOG_INFO("                 Config Version: %s", CFG_VERSION);
    ST_LOG_INFO("                   Complie Time: %s %s.", __DATE__, __TIME__);
    ST_LOG_INFO("******************************************************");

    // 初始化定时器参数缓存
    INT32 rslt = Comm_InitTimer(runFrequency);
    R_ASSERT_1(rslt == 0, -1, runFrequency);

    // 初始化系统异常信号处理相关
    Comm_InitSys();

    // 构造主循环周期统计器
    g_pobjCommMainCycleLenStatis = new CYCLE_LEN_STATIS_C("Main Cycle", 60.0, 1.0 / (double)runFrequency, abnCycleDiffRate);
    if (g_pobjCommMainCycleLenStatis == NULL) {
        ST_LOG_ERR("new CYCLE_LEN_STATIS_C() Fail.");
        return -1;
    }

    g_stCommBaseRunPara.isInitSucc   = true;

    ST_LOG_INFO("Succ.");
    return 0;
}

INT32 Comm_Init2(int argc, char** argv, int runFrequency, float abnCycleDiffRate)
{
    INT32 rsltOfCommInit = Comm_Init(runFrequency, abnCycleDiffRate);
    R_ASSERT_1(rsltOfCommInit == 0, -1, runFrequency);

    comm_SetRuningOption(argc, argv);

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 模块退出
修改记录:
**************************************************************************************/
void Comm_Uninit(void)
{
    g_stCommBaseRunPara.isInitSucc   = false;

    if (g_pobjCommMainCycleLenStatis != nullptr) {
        delete g_pobjCommMainCycleLenStatis;
        g_pobjCommMainCycleLenStatis = nullptr;
    }

    Comm_UnInitSys();

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 运行周期更新函数
修改记录:
**************************************************************************************/
void Comm_UptByPeriod(void)
{
    R_ASSERT(g_stCommBaseRunPara.isInitSucc, NOTHING);
    R_ASSERT(g_pobjCommMainCycleLenStatis != NULL, NOTHING);

    // 迭代所有定时器
    Comm_UptTimer();

    // 统计主线程运行时长
    g_pobjCommMainCycleLenStatis->Add();
    g_pobjCommMainCycleLenStatis->PeriodUpt();
}


