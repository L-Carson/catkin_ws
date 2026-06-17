#pragma once

#include "pcpt_comm/pcpt_comm_base.h"
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

struct PCPT_IMU_TF_STRU {
    PCPT_IMU_TF_STRU(){}
    PCPT_IMU_TF_STRU(const float x, const float y, const float z, const float roll, const float pitch, const float yaw, const double stamp)
    {
        this->stPose.SetValue(x, y, z, roll, pitch, yaw);
        this->stamp = stamp;
    }

    float x()     const {return stPose.stPos.x;}
    float y()     const {return stPose.stPos.y;}
    float z()     const {return stPose.stPos.z;}
    float roll()  const {return stPose.stAtt.roll;}
    float pitch() const {return stPose.stAtt.pitch;}
    float yaw()   const {return stPose.stAtt.yaw;}

    COOR_3D_POSE_STRU   stPose{0, 0, 0, 0, 0, 0};
    double              stamp{0.0};
};

/**************************************************************************************
功能描述: imu积分器
修改记录:
**************************************************************************************/
class PCPT_IMU_INTEGRATOR_C
{
public:
    PCPT_IMU_INTEGRATOR_C()
    {
        imu2LidarRot_ << -1, 0, 0, 0, 1, 0, 0, 0, 1;
    }

    ~PCPT_IMU_INTEGRATOR_C(){}

    int  SetImuDeskewInfo(deque<IMU_DATA_STRU>& deqImuData, double startTime, double endTime);
    int  SetImuDeskewInfo(deque<IMU_DATA_STRU>& deqImuData);
    int  GetTf(const double stamp, COOR_3D_POSE_STRU& stPose) const;
    int  GetTf(PCPT_IMU_TF_STRU& stImuTf) const;

private:
    vector<PCPT_IMU_TF_STRU>    vstImuTf_;
    bool                        isPosDeskew_{false};    //位置补偿，需要基于轮速里程计，低速车辆作用不大
    Eigen::Matrix3f             imu2LidarRot_;          //imu->lidar

    void ImuDataTrans(const IMU_DATA_STRU& stImuIn, IMU_DATA_STRU& stImuOut) const;
};

/**************************************************************************************
功能描述: 点云数据畸变矫正器
修改记录:
**************************************************************************************/
class POINTS_UNDISTORER_C
{
public:
    POINTS_UNDISTORER_C(){}
    ~POINTS_UNDISTORER_C(){}

    /* 获取去畸变参考点的时间 */
    int GetRefStamp(double& refStamp) {
        if (!SIG(refPointStamp_)) return -1;
        refStamp = refPointStamp_;
        return 0;
    }

    /* 点云去畸变 */
    template<typename PointT>
    int PointsUnDistortion(pcl::PointCloud<PointT> &PointCloud, deque<IMU_DATA_STRU>& deqImuData)
    {
        /* 重置参数 */
        ResetParameters();

        /* 获取点云数据起止时间 */
        double startTime =  DBL_MAX;
        double endTime   = -DBL_MAX;
        for (const auto &point : PointCloud.points) {
            startTime = startTime < point.timestamp ? startTime : point.timestamp;
            endTime   = endTime   > point.timestamp ? endTime   : point.timestamp;
        }

        /* 计算时间范围内的imu矫正数据 */
        if (objImuIntegrator_.SetImuDeskewInfo(deqImuData, startTime, endTime)) return -1;

        /* 点云畸变矫正 */
        for (auto &point : PointCloud.points) DeskewPoint(point);
        return 0;
    }

private:
    PCPT_IMU_INTEGRATOR_C       objImuIntegrator_;      //imu积分器
    bool                        isFirstPoint_{false};
    double                      refPointStamp_{0.0};
    Eigen::Affine3f             transStartInverse_{Eigen::Affine3f::Identity()};

    void ResetParameters();
    Eigen::Affine3f GetTrans(const PCPT_IMU_TF_STRU& stImuTf);

    /* 运动补偿 */
    template<typename PointT>
    void DeskewPoint(PointT& point)
    {
        /* 获取imu tf信息(旋转量+平移量) */
        PCPT_IMU_TF_STRU stImuTf(0, 0, 0, 0, 0, 0, point.timestamp);
        if (objImuIntegrator_.GetTf(stImuTf)) return;  //获取不到对应的imu数据就不做矫正，可能会存在问题

        /* 获取变换矩阵 */
        Eigen::Affine3f transBt = GetTrans(stImuTf);

        /* 得到去畸变后的点云 */
        PointT deskewPoint;
        deskewPoint.x = transBt(0,0) * point.x + transBt(0,1) * point.y + transBt(0,2) * point.z + transBt(0,3);
        deskewPoint.y = transBt(1,0) * point.x + transBt(1,1) * point.y + transBt(1,2) * point.z + transBt(1,3);
        deskewPoint.z = transBt(2,0) * point.x + transBt(2,1) * point.y + transBt(2,2) * point.z + transBt(2,3);

        point.x = deskewPoint.x;
        point.y = deskewPoint.y;
        point.z = deskewPoint.z;
    }
};
