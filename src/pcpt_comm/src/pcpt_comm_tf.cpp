#include "pcpt_comm/pcpt_comm_tf.h"

/**************************************************************************************
功能描述: 设置转化关系
           fx = coeffAX * sx + coeffBX * sy + constCX
           fy = coeffAY * sx + coeffBY * sy + constCY
修改记录:
**************************************************************************************/
void PCPT_TF_C::SetTfRelationship(const COOR_2D_POSE_STRU &stSonPoseByRef, const COOR_2D_POSE_STRU &stFatherPoseByRef)
{
    float sinSonAng = sin(stSonPoseByRef.dirAngle);
    float cosSonAng = cos(stSonPoseByRef.dirAngle);

    float sinFatherAng = sin(stFatherPoseByRef.dirAngle);
    float cosFatherAng = cos(stFatherPoseByRef.dirAngle);

    float difX = stSonPoseByRef.stPos.x - stFatherPoseByRef.stPos.x;
    float difY = stSonPoseByRef.stPos.y - stFatherPoseByRef.stPos.y;

    coeffAX_ =  cosFatherAng * cosSonAng + sinFatherAng * sinSonAng;
    coeffBX_ = -cosFatherAng * sinSonAng + sinFatherAng * cosSonAng;
    constCX_ =  cosFatherAng * difX      + sinFatherAng * difY;

    coeffAY_ = -sinFatherAng * cosSonAng + cosFatherAng * sinSonAng;
    coeffBY_ =  sinFatherAng * sinSonAng + cosFatherAng * cosSonAng;
    constCY_ = -sinFatherAng * difX      + cosFatherAng * difY;
}

/**************************************************************************************
功能描述: 设置转换关系
修改记录:
**************************************************************************************/
void PCPT_3D_TF_C::SetTfRelationship(const COOR_3D_POSE_STRU &stSonPoseByFather)
{
    tfSonPoseByFather_ = stSonPoseByFather.GetTransform();
    tfFatherPoseBySon_ = tfSonPoseByFather_.inverse();
}

void PCPT_3D_TF_C::SetTfRelationship(const COOR_3D_POSE_STRU &stSonPoseByRef, const COOR_3D_POSE_STRU &stFatherPoseByRef)
{
    tfSonPoseByFather_ = stFatherPoseByRef.GetTransformInv() * stSonPoseByRef.GetTransform();
    tfFatherPoseBySon_ = tfSonPoseByFather_.inverse();
}

void PCPT_3D_TF_C::SetTfRelationship(const Eigen::Quaternionf& quatSonPoseByFather)
{
    Eigen::Matrix3f rotation;
    rotation = quatSonPoseByFather.matrix();

    Eigen::Vector3f translation(0, 0, 0);

    tfSonPoseByFather_.block(0, 0, 3, 3) = rotation;    //从(0, 0)开始，3*3的块
    tfSonPoseByFather_.block(0, 3, 3, 1) = translation; //从(0, 3)开始，3*1的块
    tfFatherPoseBySon_ = tfSonPoseByFather_.inverse();
}

void PCPT_3D_TF_C::SetTfRelationship(const Eigen::Matrix4f& tfSonPoseByFather)
{
    tfSonPoseByFather_ = tfSonPoseByFather;
    tfFatherPoseBySon_ = tfSonPoseByFather_.inverse();
}

void PCPT_3D_TF_C::SetTfRelationship(float roll, float pitch, float yaw)
{
    Eigen::AngleAxisf rotation_x(roll,  Eigen::Matrix<float, 3, 1>::UnitX());
    Eigen::AngleAxisf rotation_y(pitch, Eigen::Matrix<float, 3, 1>::UnitY());
    Eigen::AngleAxisf rotation_z(yaw,   Eigen::Matrix<float, 3, 1>::UnitZ());

    Eigen::Matrix3f rotation;
    rotation = rotation_z * rotation_y * rotation_x;

    Eigen::Vector3f translation(0, 0, 0);

    tfSonPoseByFather_.block(0, 0, 3, 3) = rotation;    //从(0, 0)开始，3*3的块
    tfSonPoseByFather_.block(0, 3, 3, 1) = translation; //从(0, 3)开始，3*1的块
    tfFatherPoseBySon_ = tfSonPoseByFather_.inverse();
}

void PCPT_3D_TF_C::TransSonPoseToFather(const COOR_3D_POSE_STRU &stSonPose, COOR_3D_POSE_STRU &stFatherPose)
{
    Eigen::Matrix4f mPose = tfSonPoseByFather_ * stSonPose.GetTransform();
    stFatherPose.SetValue(mPose);
}

void PCPT_3D_TF_C::TransFatherPoseToSon(const COOR_3D_POSE_STRU &stFatherPose, COOR_3D_POSE_STRU &stSonPose)
{
    Eigen::Matrix4f mPose = tfFatherPoseBySon_ * stFatherPose.GetTransform();
    stSonPose.SetValue(mPose);
}

/**************************************************************************************
功能描述: 由TF获取当前3维定位信息
修改记录:
**************************************************************************************/
int PCPT_3D_TF_C::GetLocationMsgFromTF(COOR_3D_POSE_STRU &stPose) const
{
    Eigen::Matrix4f tfPoseByMap;
    if (GetTransMatrixFromTF(tfPoseByMap) != 0) return -1;
    stPose.Copy(tfPoseByMap);
    return 0;
}

/**************************************************************************************
功能描述: 由TF获取当前位姿转换矩阵
修改记录:
**************************************************************************************/
int PCPT_3D_TF_C::GetTransMatrixFromTF(Eigen::Matrix4f &tfPoseByMap) const
{
    static tf::TransformListener   listenerTf_;
    static tf::StampedTransform    transform_;

    try {
        listenerTf_.lookupTransform(PCPT_FRAME_ID_MAP, PCPT_FRAME_ID_CAR, ros::Time(0), transform_);
    } catch (tf::TransformException &ex) {
        //ST_LOG_ERR("Transform error of sensor data: %s", ex.what());
        return -1;
    }

    pcl_ros::transformAsMatrix(transform_, tfPoseByMap);
    return 0;
}

/**************************************************************************************
功能描述: 由TF获取当前位姿转换矩阵
修改记录:
**************************************************************************************/
void PCPT_3D_TF_C::GetTransMatrix(Eigen::Matrix4f &tfSonPoseByFather) const
{
    tfSonPoseByFather = tfSonPoseByFather_;
}

