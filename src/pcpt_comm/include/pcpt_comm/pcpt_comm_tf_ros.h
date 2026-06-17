#ifndef __PCPT_COMM_TF_ROS_H__
#define __PCPT_COMM_TF_ROS_H__

#include <iostream>
#include <string>
#include <vector>

#include "ros/ros.h"
#include "tf/tf.h"
#include "tf/transform_listener.h"
#include "geometry_msgs/Pose2D.h"

#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/comm_car.h"
#include "comm/comm_topic_name.h"
#include "comm_ros_extend/comm_ros_base_extend.h"

namespace PCPT
{

/**
 * @brief TF查询ROS接口类
 *
 */
class TF_QUERY_ROS_API_C
{
public:
    using Ptr = std::shared_ptr<TF_QUERY_ROS_API_C>;
    using ConstPtr = std::shared_ptr<const TF_QUERY_ROS_API_C>;

private:
    tf::TransformListener *p_tf_listener_ = nullptr;  // tf2监听者指针
    float max_cache_time_{0.0};

public:
    // 构造函数
    TF_QUERY_ROS_API_C(const double &max_cache_time = 10.0) : max_cache_time_(max_cache_time) {}

    // 析构函数
    ~TF_QUERY_ROS_API_C(void)
    {
        if (nullptr != p_tf_listener_)
        {
            delete p_tf_listener_;
        }
    }

    // 查询TF: src-->tar（src坐标系在tar坐标系中的位姿）
    bool UpdateTFImple(const std::string &tar_frame,
                       const std::string &src_frame,
                       geometry_msgs::Pose2D &tar_src_pose)
    {
        tf::Stamped<tf::Pose> src2tar_stamped;
        if (GetTFImple(tar_frame, src_frame, src2tar_stamped))
        {
            tar_src_pose.x = src2tar_stamped.getOrigin().x();
            tar_src_pose.y = src2tar_stamped.getOrigin().y();
            tar_src_pose.theta = tf::getYaw(src2tar_stamped.getRotation());
            return true;
        }

        return false;
    }

    // 查询TF: src-->tar（src坐标系在tar坐标系中的位姿）
    bool UpdateTFImple(const std::string &tar_frame,
                       const std::string &src_frame,
                       tf::Transform &src_to_tar)
    {
        tf::Stamped<tf::Pose> src2tar_stamped;
        if (GetTFImple(tar_frame, src_frame, src2tar_stamped))
        {
            src_to_tar = src2tar_stamped;
            return true;
        }

        return false;
    }

    // 查询TF: src-->tar（src坐标系在tar坐标系中的位姿）
    bool UpdateTFImple(const std::string &tar_frame,
                       const std::string &src_frame,
                       tf::Stamped<tf::Pose> &src2tar_stamped)
    {
        if (GetTFImple(tar_frame, src_frame, src2tar_stamped))
        {
            return true;
        }

        return false;
    }

private:
    // 查询TF: src-->tar（src坐标系在tar坐标系中的位姿）
    // src2tar_stamped[out]: [src-->tar](包含平移、旋转数据，及frame_id、stamp)
    bool GetTFImple(const std::string &tar_frame,
                    const std::string &src_frame,
                    tf::Stamped<tf::Pose> &src2tar_stamped,
                    const ros::Time &timestamp = ros::Time())
    {
        // src坐标系原点
        tf::Stamped<tf::Pose> src_stamped(tf::Transform(tf::createIdentityQuaternion(), tf::Vector3(0, 0, 0)),
                                          timestamp, src_frame);

        try
        {
            // 监听tf数据
            if (!p_tf_listener_) p_tf_listener_ = new tf::TransformListener(ros::Duration(max_cache_time_));
            if (p_tf_listener_) p_tf_listener_->transformPose(tar_frame, src_stamped, src2tar_stamped);
        }
        catch (tf::TransformException &e)
        {
            return false;
        }

        return true;
    }
};

/**
 * @brief 静态TF查询ROS接口类
 *
 */
class TF_STATIC_ROS_API_C : public TF_QUERY_ROS_API_C
{
private:
    tf::Transform tf_data_;
    bool tf_valid_ = false;

public:
    // 构造函数
    TF_STATIC_ROS_API_C(const double &max_cache_time = 10.0)
        : TF_QUERY_ROS_API_C(max_cache_time)
    {
    }

    // 查询TF: src-->tar（src坐标系在tar坐标系中的位姿）
    bool GetTfData(const std::string &tar_frame,
                   const std::string &src_frame,
                   tf::Transform &src_to_tar)
    {
        if (!tf_valid_)
        {
            tf_valid_ = UpdateTFImple(tar_frame, src_frame, tf_data_);
        }

        if (tf_valid_)
        {
            src_to_tar = tf_data_;
            return true;
        }

        return false;
    }
};


}

#endif
