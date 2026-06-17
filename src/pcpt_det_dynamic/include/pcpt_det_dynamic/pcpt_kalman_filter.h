#ifndef __PCPT_KALMAN_FILTER_H__
#define __PCPT_KALMAN_FILTER_H__

#include <fstream>
#include <string>
#include <vector>
#include "Eigen/Dense"
#include "comm/comm_base.h"
#include "comm/comm_dbg_log.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_define.h"

/**
 * Please note that the Eigen library does not initialize
 * VectorXd or MatrixXd objects with zeros upon creation.
 */

/**************************************************************************************
功能描述: 传感器测量数据
修改记录:
**************************************************************************************/
class MEASURE_PACKAGE_C
{
public:
    enum SENSOR_TYPE_ENUM {
        LIDAR,      //激光雷达（笛卡尔坐标）
        RADAR,      //毫米波雷达（笛卡尔坐标）
        RADAR_E,    //毫米波雷达（极坐标）
    } sensorType;

    double timeStamp;

    Eigen::VectorXd measureData;
};

/**************************************************************************************
功能描述: 卡尔曼滤波 传感器融合
修改记录: 卡尔曼融合滤波器
**************************************************************************************/
class KF_C
{
public:
    KF_C(void);
    virtual ~KF_C(void) { };
    void  Process(const MEASURE_PACKAGE_C &objMeasurePackage);
    Eigen::VectorXd GetState(void);

private:
    INT32 Init(const MEASURE_PACKAGE_C &objMeasurePackage);
    INT32 Predict(const double &time);
    void  UpdateKf(const Eigen::VectorXd &z);
    void  UpdateEkf(const Eigen::VectorXd &z);
    void  Update(const MEASURE_PACKAGE_C &objMeasurePackage);

    Eigen::VectorXd CalculateRMSE(const std::vector<Eigen::VectorXd> &estimations,
                                  const std::vector<Eigen::VectorXd> &groundTruth);

    Eigen::MatrixXd CalculateJacobian(const Eigen::VectorXd& stateX);

    void ReformState(Eigen::VectorXd& x);

private:
    BOOL                isInit_;        //check whether the tracking toolbox was initialized or not (first measurement)
    double              uptTime_;       //测量更新时间
    double              dt_;            //时间差

    Eigen::VectorXd     x_;             //state vector
    Eigen::MatrixXd     P_;             //state covariance matrix
    Eigen::MatrixXd     F_;             //state transition matrix
    Eigen::MatrixXd     Q_;             //process covariance matrix
    Eigen::MatrixXd     H_;             //measurement change matrix (观测矩阵)
    Eigen::MatrixXd     R_;             //measurement covariance matrix

    Eigen::MatrixXd     H_lidar_;       //变换矩阵，将目标的理论估计状态矢量x'投射到传感器的测量空间
    Eigen::MatrixXd     R_lidar_;       //激光雷达测量噪声协方差矩阵

    Eigen::MatrixXd     H_radar_;       //变换矩阵，将目标的理论估计状态矢量x'投射到传感器的测量空间
    Eigen::MatrixXd     R_radar_;       //毫米波雷达测量噪声协方差矩阵

    Eigen::MatrixXd     H_radar_E_;     //雅可比矩阵
    Eigen::MatrixXd     R_radar_E_;     //毫米波雷达测量噪声协方差矩阵

    Eigen::VectorXd     xPre_;          //之前的状态向量，用于状态修正，与KF过程无关

};

#endif
