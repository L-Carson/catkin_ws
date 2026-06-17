#ifndef __PCPT_COMM_TF_H__
#define __PCPT_COMM_TF_H__

#include "pcpt_comm/pcpt_comm_base.h"
#include <pcl/common/transforms.h>
#include <pcl_ros/transforms.h>
#include <tf/transform_listener.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace std;

/**************************************************************************************
功能描述: 位姿变换
修改记录:
**************************************************************************************/
class PCPT_TF_C : public COOR_TF_C
{
public:
    PCPT_TF_C(void) {}
    PCPT_TF_C(const COOR_2D_POSE_STRU &stSonOriginPoseByFather) : COOR_TF_C(stSonOriginPoseByFather) {};
    PCPT_TF_C(const COOR_2D_POSE_STRU &stSonPoseByRef, const COOR_2D_POSE_STRU &stFatherPoseByRef) {
        SetTfRelationship(stSonPoseByRef, stFatherPoseByRef);
    }
    ~PCPT_TF_C() {}

    void SetTfRelationship(const COOR_2D_POSE_STRU &stSonPoseByRef, const COOR_2D_POSE_STRU &stFatherPoseByRef);

    template <typename T>
    void Trans2dPosBySon2ByFather(const T& posBySon, T& posByFather) const {
        posByFather.x = coeffAX_ * posBySon.x + coeffBX_ * posBySon.y + constCX_;
        posByFather.y = coeffAY_ * posBySon.x + coeffBY_ * posBySon.y + constCY_;
    }

private:
    float coeffAX_{0};
    float coeffBX_{0};
    float constCX_{0};
    float coeffAY_{0};
    float coeffBY_{0};
    float constCY_{0};
};

class PCPT_3D_TF_C
{
public:
    PCPT_3D_TF_C(void) {}
    PCPT_3D_TF_C(const COOR_3D_POSE_STRU &stSonPoseByFather) {SetTfRelationship(stSonPoseByFather);}
    PCPT_3D_TF_C(const COOR_3D_POSE_STRU &stSonPoseByRef, const COOR_3D_POSE_STRU &stFatherPoseByRef) {
        SetTfRelationship(stSonPoseByRef, stFatherPoseByRef);
    }
    PCPT_3D_TF_C(const Eigen::Quaternionf& quatSonPoseByFather) {SetTfRelationship(quatSonPoseByFather);}
    PCPT_3D_TF_C(const Eigen::Matrix4f& tfSonPoseByFather) {SetTfRelationship(tfSonPoseByFather);}
    PCPT_3D_TF_C(float roll, float pitch, float yaw) {SetTfRelationship(roll, pitch, yaw);}
    ~PCPT_3D_TF_C() {}

    void SetTfRelationship(const COOR_3D_POSE_STRU &stSonPoseByFather);
    void SetTfRelationship(const COOR_3D_POSE_STRU &stSonPoseByRef, const COOR_3D_POSE_STRU &stFatherPoseByRef);
    void SetTfRelationship(const Eigen::Quaternionf& quatSonPoseByFather);
    void SetTfRelationship(const Eigen::Matrix4f& tfSonPoseByFather);
    void SetTfRelationship(float roll, float pitch, float yaw);

    template <typename T>
    void Trans2dPosBySon2ByFather(const T& posBySon, T& posByFather) const {
        Eigen::Vector4f vecBySon(posBySon.x, posBySon.y, 0, 1);
        Eigen::Vector4f vecByFather = tfSonPoseByFather_ * vecBySon;
        posByFather.x = vecByFather[0];
        posByFather.y = vecByFather[1];
    }

    template <typename T>
    void Trans2dPosByFather2BySon(const T& posByFather, T& posBySon) const {
        Eigen::Vector4f vecByFather(posByFather.x, posByFather.y, 0, 1);
        Eigen::Vector4f vecBySon = tfFatherPoseBySon_ * vecByFather;
        posBySon.x = vecBySon[0];
        posBySon.y = vecBySon[1];
    }

    template <typename T>
    void Trans3dPosBySon2ByFather(const T& posBySon, T& posByFather) const {
        Eigen::Vector4f vecBySon(posBySon.x, posBySon.y, posBySon.z, 1);
        Eigen::Vector4f vecByFather = tfSonPoseByFather_ * vecBySon;
        posByFather.x = vecByFather[0];
        posByFather.y = vecByFather[1];
        posByFather.z = vecByFather[2];
    }

    template <typename T>
    void Trans3dPosByFather2BySon(const T& posByFather, T& posBySon) const {
        Eigen::Vector4f vecByFather(posByFather.x, posByFather.y, posByFather.z, 1);
        Eigen::Vector4f vecBySon = tfFatherPoseBySon_ * vecByFather;
        posBySon.x = vecBySon[0];
        posBySon.y = vecBySon[1];
        posBySon.z = vecBySon[2];
    }

    template<typename T>
    void Trans3dPointsBySon2ByFather(const T& pCloudBySon, T& pCloudByFather) const {
        pcl::transformPointCloud(*pCloudBySon, *pCloudByFather, tfSonPoseByFather_);
    }

    template<typename T>
    void Trans3dPointsByFather2BySon(const T& pCloudByFather, T& pCloudBySon) const {
        pcl::transformPointCloud(*pCloudByFather, *pCloudBySon, tfFatherPoseBySon_);
    }

    void TransSonPoseToFather(const COOR_3D_POSE_STRU &stSonPose, COOR_3D_POSE_STRU &stFatherPose);
    void TransFatherPoseToSon(const COOR_3D_POSE_STRU &stFatherPose, COOR_3D_POSE_STRU &stSonPose);

    int  GetLocationMsgFromTF(COOR_3D_POSE_STRU &stPose) const;
    int  GetTransMatrixFromTF(Eigen::Matrix4f &tfPoseByMap) const;
    void GetTransMatrix(Eigen::Matrix4f &tfSonPoseByFather) const;

private:
    Eigen::Matrix4f tfSonPoseByFather_{Eigen::Matrix4f::Identity()};
    Eigen::Matrix4f tfFatherPoseBySon_{Eigen::Matrix4f::Identity()};
};

#endif

