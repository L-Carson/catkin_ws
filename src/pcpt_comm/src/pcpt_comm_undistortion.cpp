#include "pcpt_comm/pcpt_comm_undistortion.h"

/**************************************************************************************
功能描述: imu数据转换
修改记录:
**************************************************************************************/
void PCPT_IMU_INTEGRATOR_C::ImuDataTrans(const IMU_DATA_STRU& stImuIn, IMU_DATA_STRU& stImuOut) const
{
    if (isPosDeskew_) stImuOut.acc  = imu2LidarRot_ * stImuIn.acc;
    stImuOut.gyro = imu2LidarRot_ * stImuIn.gyro;
    //stImuOut.orientation = stImuIn.orientation * Eigen::Quaternionf(imu2LidarRot_);
}

/**************************************************************************************
功能描述: 用于去畸变的相关信息或变量值计算
修改记录:
**************************************************************************************/
int PCPT_IMU_INTEGRATOR_C::SetImuDeskewInfo(deque<IMU_DATA_STRU>& deqImuData, double startTime, double endTime)
{
    while (deqImuData.size()) {
        if (deqImuData.front().time < startTime - 0.01)
            deqImuData.pop_front();
        else
            break;
    }

    if (deqImuData.empty()) return -1;

    vstImuTf_.clear();
    for (size_t i = 0; i < deqImuData.size(); ++i) {
        const IMU_DATA_STRU &stImu = deqImuData[i];
        double currentImuTime = stImu.time;
        if (currentImuTime > endTime + 0.01) break; //如果当前Imu时间比下一帧时间大于0.01，退出

        /* imu数据转换 */
        IMU_DATA_STRU stImuTrans;
        ImuDataTrans(stImu, stImuTrans);

        /* 第一次初始化时，以下值都是0 */
        if (i == 0) {
            vstImuTf_.push_back(PCPT_IMU_TF_STRU{0, 0, 0, 0, 0, 0, currentImuTime});
            continue;
        }

        /* 从imu信息中直接获得角速度 */
        PCPT_IMU_TF_STRU &stImuTfPre = vstImuTf_.back();
        double timeDiff = currentImuTime - stImuTfPre.stamp;
        if(timeDiff > 0.1 || timeDiff < -0.1)
            ST_LOG_INFO("TimeDiff:%f CurrentImuTime:%f PreImuTime:%f", timeDiff, currentImuTime, stImuTfPre.stamp);

        PCPT_IMU_TF_STRU stImuTfCur;
        if (isPosDeskew_) {
            stImuTfCur.stPose.stPos.x = stImuTfPre.x() + stImuTrans.acc[0] * timeDiff * timeDiff;
            stImuTfCur.stPose.stPos.y = stImuTfPre.y() + stImuTrans.acc[1] * timeDiff * timeDiff;
            stImuTfCur.stPose.stPos.z = stImuTfPre.z() + stImuTrans.acc[2] * timeDiff * timeDiff;
        }
        stImuTfCur.stPose.stAtt.roll  = stImuTfPre.roll()  + stImuTrans.gyro[0] * timeDiff;
        stImuTfCur.stPose.stAtt.pitch = stImuTfPre.pitch() + stImuTrans.gyro[1] * timeDiff;
        stImuTfCur.stPose.stAtt.yaw   = stImuTfPre.yaw()   + stImuTrans.gyro[2] * timeDiff;
        stImuTfCur.stamp = currentImuTime;
        vstImuTf_.push_back(stImuTfCur);
    }

    return 0;
}

int PCPT_IMU_INTEGRATOR_C::SetImuDeskewInfo(deque<IMU_DATA_STRU>& deqImuData)
{
    IMU_DATA_STRU &stFront = deqImuData.front();
    IMU_DATA_STRU &stBack  = deqImuData.back();
    return SetImuDeskewInfo(deqImuData, stFront.time, stBack.time);
}

/**************************************************************************************
功能描述: 由imu获取旋转参数
修改记录:
**************************************************************************************/
int PCPT_IMU_INTEGRATOR_C::GetTf(const double stamp, COOR_3D_POSE_STRU& stPose) const
{
    size_t imuCur = 0;
    for (const PCPT_IMU_TF_STRU &stImuTf : vstImuTf_) {
        if (stamp < stImuTf.stamp) break;
        ++imuCur;
    }

    if (imuCur >= vstImuTf_.size()) return -1;

    const PCPT_IMU_TF_STRU& stImuTfCur = vstImuTf_[imuCur];
    if (stamp > stImuTfCur.stamp || imuCur == 0) {
        stPose = stImuTfCur.stPose;
    } else {
        const PCPT_IMU_TF_STRU& stImuTfPre = vstImuTf_[imuCur-1];
        double ratioCur = (stamp - stImuTfPre.stamp) / (stImuTfCur.stamp - stImuTfPre.stamp);
        double ratioPre = (stImuTfCur.stamp - stamp) / (stImuTfCur.stamp - stImuTfPre.stamp);
        if (isPosDeskew_) {
            stPose.stPos.x = stImuTfCur.x() * ratioCur + stImuTfPre.x() * ratioPre;
            stPose.stPos.y = stImuTfCur.y() * ratioCur + stImuTfPre.y() * ratioPre;
            stPose.stPos.z = stImuTfCur.z() * ratioCur + stImuTfPre.z() * ratioPre;
        }
        stPose.stAtt.roll  = stImuTfCur.roll()  * ratioCur + stImuTfPre.roll()  * ratioPre;
        stPose.stAtt.pitch = stImuTfCur.pitch() * ratioCur + stImuTfPre.pitch() * ratioPre;
        stPose.stAtt.yaw   = stImuTfCur.yaw()   * ratioCur + stImuTfPre.yaw()   * ratioPre;
    }
    return 0;
}

int PCPT_IMU_INTEGRATOR_C::GetTf(PCPT_IMU_TF_STRU& stImuTf) const
{
    return GetTf(stImuTf.stamp, stImuTf.stPose);
}

/**************************************************************************************
功能描述: 参数初始化
修改记录:
**************************************************************************************/
void POINTS_UNDISTORER_C::ResetParameters()
{
    isFirstPoint_  = true;
    refPointStamp_ = 0.0;
}

/**************************************************************************************
功能描述: 获取旋转矩阵
修改记录:
**************************************************************************************/
Eigen::Affine3f POINTS_UNDISTORER_C::GetTrans(const PCPT_IMU_TF_STRU& stImuTf)
{
    /* 如果是第一次收到数据，旋转矩阵初始化一次，后续迭代使用 */
    if (isFirstPoint_) {
        transStartInverse_ = pcl::getTransformation(stImuTf.x(), stImuTf.y(), stImuTf.z(), stImuTf.roll(), stImuTf.pitch(), stImuTf.yaw()).inverse();
        refPointStamp_ = stImuTf.stamp;
        isFirstPoint_ = false;
    }

    /* 把点投影到每一帧扫描的起始时刻 */
    Eigen::Affine3f transFinal  = pcl::getTransformation(stImuTf.x(), stImuTf.y(), stImuTf.z(), stImuTf.roll(), stImuTf.pitch(), stImuTf.yaw());
    return transStartInverse_ * transFinal;
}

