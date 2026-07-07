// Multi-LiDAR point cloud fusion for lidar_det_node.
//
// Trigger: each MainLidar frame merges the latest BlindLidar / BlindLidar2 clouds
// (if fresh enough). All topics are expected in the same frame (e.g. base_link).
//
// Passthrough mode (fusion_enabled=false) republishes main only — use for A/B or
// quick rollback without stopping the node.

#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/filters/voxel_grid.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

namespace {

struct RangeConfig {
    bool  enabled = true;
    float min_x = -30.f;
    float max_x =  30.f;
    float min_y = -30.f;
    float max_y =  30.f;
    float min_z =  -5.f;
    float max_z =   3.f;
};

struct FusionStats {
    uint64_t frame_count = 0;
    size_t   main_pts    = 0;
    size_t   blind1_pts  = 0;
    size_t   blind2_pts  = 0;
    size_t   fused_pts   = 0;
    bool     used_blind1 = false;
    bool     used_blind2 = false;
};

const sensor_msgs::PointField* findPointField(const sensor_msgs::PointCloud2& msg,
                                              const char* name) {
    for (const auto& field : msg.fields) {
        if (field.name == name) return &field;
    }
    return nullptr;
}

float readFloatField(const sensor_msgs::PointCloud2& msg,
                     const sensor_msgs::PointField& field,
                     size_t point_index) {
    if (field.count == 0 || field.offset + sizeof(float) > msg.point_step) return 0.f;

    const size_t byte_offset =
        static_cast<size_t>(point_index) * msg.point_step + field.offset;
    if (byte_offset + sizeof(float) > msg.data.size()) return 0.f;

    float value = 0.f;
    switch (field.datatype) {
        case sensor_msgs::PointField::FLOAT32:
            std::memcpy(&value, &msg.data[byte_offset], sizeof(float));
            return value;
        case sensor_msgs::PointField::UINT8:
            return static_cast<float>(msg.data[byte_offset]);
        case sensor_msgs::PointField::UINT16: {
            uint16_t v = 0;
            std::memcpy(&v, &msg.data[byte_offset], sizeof(uint16_t));
            return static_cast<float>(v);
        }
        case sensor_msgs::PointField::UINT32: {
            uint32_t v = 0;
            std::memcpy(&v, &msg.data[byte_offset], sizeof(uint32_t));
            return static_cast<float>(v);
        }
        default:
            return 0.f;
    }
}

bool toPointCloudXYZI(const sensor_msgs::PointCloud2& msg,
                      pcl::PointCloud<pcl::PointXYZI>& cloud) {
    pcl::PointCloud<pcl::PointXYZ> cloud_xyz;
    pcl::fromROSMsg(msg, cloud_xyz);

    const sensor_msgs::PointField* intensity_field =
        findPointField(msg, "intensity");
    const bool has_intensity = intensity_field != nullptr;

    cloud.resize(cloud_xyz.size());
    for (size_t i = 0; i < cloud_xyz.size(); ++i) {
        cloud[i].x = cloud_xyz[i].x;
        cloud[i].y = cloud_xyz[i].y;
        cloud[i].z = cloud_xyz[i].z;
        cloud[i].intensity = has_intensity
            ? readFloatField(msg, *intensity_field, i)
            : 0.f;
    }
    return true;
}

void filterByRange(pcl::PointCloud<pcl::PointXYZI>& cloud, const RangeConfig& range) {
    if (!range.enabled || cloud.empty()) return;

    pcl::PointCloud<pcl::PointXYZI> filtered;
    filtered.reserve(cloud.size());
    for (const auto& pt : cloud) {
        if (pt.x < range.min_x || pt.x > range.max_x) continue;
        if (pt.y < range.min_y || pt.y > range.max_y) continue;
        if (pt.z < range.min_z || pt.z > range.max_z) continue;
        filtered.push_back(pt);
    }
    cloud.swap(filtered);
}

void voxelDownsample(pcl::PointCloud<pcl::PointXYZI>& cloud, float leaf_size) {
    if (leaf_size <= 0.f || cloud.empty()) return;

    pcl::PointCloud<pcl::PointXYZI> filtered;
    pcl::VoxelGrid<pcl::PointXYZI> voxel;
    voxel.setLeafSize(leaf_size, leaf_size, leaf_size);
    voxel.setInputCloud(cloud.makeShared());
    voxel.filter(filtered);
    cloud.swap(filtered);
}

void capPointCount(pcl::PointCloud<pcl::PointXYZI>& cloud, size_t max_points) {
    if (max_points == 0 || cloud.size() <= max_points) return;

    pcl::PointCloud<pcl::PointXYZI> sampled;
    sampled.reserve(max_points);
    const double step = static_cast<double>(cloud.size()) / static_cast<double>(max_points);
    for (size_t i = 0; i < max_points; ++i) {
        const size_t idx = static_cast<size_t>(std::floor(i * step));
        sampled.push_back(cloud[idx]);
    }
    cloud.swap(sampled);
}

bool isBlindCloudUsable(const ros::Time& main_stamp,
                        const sensor_msgs::PointCloud2ConstPtr& blind,
                        double max_age_sec,
                        double max_lead_sec,
                        std::string* reject_reason) {
    if (!blind || blind->data.empty()) {
        if (reject_reason) *reject_reason = "empty";
        return false;
    }
    const double age_sec = (main_stamp - blind->header.stamp).toSec();
    if (age_sec > max_age_sec) {
        if (reject_reason) {
            *reject_reason = "too_old age=" + std::to_string(age_sec) + "s";
        }
        return false;
    }
    if (age_sec < -max_lead_sec) {
        if (reject_reason) {
            *reject_reason = "too_new lead=" + std::to_string(-age_sec) + "s";
        }
        return false;
    }
    return true;
}

}  // namespace

class LidarCloudFusionNode {
public:
    LidarCloudFusionNode(ros::NodeHandle& nh, ros::NodeHandle& pnh) {
        pnh.param<std::string>("main_topic", main_topic_, "/MainLidar/trans_points");
        pnh.param<std::string>("blind1_topic", blind1_topic_, "/BlindLidar/trans_points");
        pnh.param<std::string>("blind2_topic", blind2_topic_, "/BlindLidar2/trans_points");
        pnh.param<std::string>("output_topic", output_topic_, "/fused_lidar/trans_points");
        pnh.param<std::string>("output_frame_id", output_frame_id_, "base_link");

        pnh.param("fusion_enabled", fusion_enabled_, true);
        pnh.param("use_blind1", use_blind1_, true);
        pnh.param("use_blind2", use_blind2_, true);
        pnh.param("blind_max_age_sec", blind_max_age_sec_, 0.25);
        pnh.param("blind_max_lead_sec", blind_max_lead_sec_, 0.05);

        pnh.param("range_filter_enabled", range_.enabled, true);
        pnh.param("range_min_x", range_.min_x, -30.f);
        pnh.param("range_max_x", range_.max_x,  30.f);
        pnh.param("range_min_y", range_.min_y, -30.f);
        pnh.param("range_max_y", range_.max_y,  30.f);
        pnh.param("range_min_z", range_.min_z,  -5.f);
        pnh.param("range_max_z", range_.max_z,   3.f);

        pnh.param("voxel_leaf_size", voxel_leaf_size_, 0.f);
        int max_points = 300000;
        pnh.param("max_points", max_points, 300000);
        max_points_ = max_points > 0 ? static_cast<size_t>(max_points) : 0;

        pnh.param("stats_interval", stats_interval_, 30);
        if (stats_interval_ < 1) stats_interval_ = 1;

        pub_ = nh.advertise<sensor_msgs::PointCloud2>(output_topic_, 1);
        sub_main_ = nh.subscribe(main_topic_, 1, &LidarCloudFusionNode::mainCb, this);
        if (use_blind1_) {
            sub_blind1_ = nh.subscribe(blind1_topic_, 1,
                                       &LidarCloudFusionNode::blind1Cb, this);
        }
        if (use_blind2_) {
            sub_blind2_ = nh.subscribe(blind2_topic_, 1,
                                       &LidarCloudFusionNode::blind2Cb, this);
        }

        ROS_INFO("lidar_cloud_fusion: main=%s -> out=%s fusion_enabled=%d",
                 main_topic_.c_str(), output_topic_.c_str(), fusion_enabled_ ? 1 : 0);
        ROS_INFO("  blind1=%s use=%d blind2=%s use=%d max_age=%.3fs frame=%s",
                 blind1_topic_.c_str(), use_blind1_ ? 1 : 0,
                 blind2_topic_.c_str(), use_blind2_ ? 1 : 0,
                 blind_max_age_sec_, output_frame_id_.c_str());
        ROS_INFO("  range=[%.1f,%.1f]x[%.1f,%.1f]x[%.1f,%.1f] voxel=%.3f max_pts=%zu",
                 range_.min_x, range_.max_x, range_.min_y, range_.max_y,
                 range_.min_z, range_.max_z, voxel_leaf_size_, max_points_);
    }

private:
    void blind1Cb(const sensor_msgs::PointCloud2ConstPtr& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        latest_blind1_ = msg;
    }

    void blind2Cb(const sensor_msgs::PointCloud2ConstPtr& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        latest_blind2_ = msg;
    }

    bool appendBlindCloud(const ros::Time& main_stamp,
                          const sensor_msgs::PointCloud2ConstPtr& blind,
                          const char* name,
                          pcl::PointCloud<pcl::PointXYZI>& fused,
                          size_t& blind_pts_out) {
        blind_pts_out = 0;
        if (!blind) return false;

        std::string reject_reason;
        if (!isBlindCloudUsable(main_stamp, blind, blind_max_age_sec_,
                                blind_max_lead_sec_, &reject_reason)) {
            ROS_WARN_THROTTLE(2.0, "skip %s: %s", name, reject_reason.c_str());
            return false;
        }
        if (!output_frame_id_.empty() &&
            blind->header.frame_id != output_frame_id_) {
            ROS_WARN_THROTTLE(2.0, "skip %s: frame_id %s != %s",
                              name, blind->header.frame_id.c_str(),
                              output_frame_id_.c_str());
            return false;
        }

        pcl::PointCloud<pcl::PointXYZI> blind_cloud;
        toPointCloudXYZI(*blind, blind_cloud);
        blind_pts_out = blind_cloud.size();
        fused += blind_cloud;
        return true;
    }

    void mainCb(const sensor_msgs::PointCloud2ConstPtr& msg) {
        FusionStats stats;
        stats.frame_count = ++frame_count_;

        sensor_msgs::PointCloud2ConstPtr blind1;
        sensor_msgs::PointCloud2ConstPtr blind2;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            blind1 = latest_blind1_;
            blind2 = latest_blind2_;
        }

        pcl::PointCloud<pcl::PointXYZI> fused;
        if (!fusion_enabled_) {
            toPointCloudXYZI(*msg, fused);
            stats.main_pts = fused.size();
        } else {
            toPointCloudXYZI(*msg, fused);
            stats.main_pts = fused.size();

            if (use_blind1_) {
                stats.used_blind1 = appendBlindCloud(
                    msg->header.stamp, blind1, "blind1", fused, stats.blind1_pts);
            }
            if (use_blind2_) {
                stats.used_blind2 = appendBlindCloud(
                    msg->header.stamp, blind2, "blind2", fused, stats.blind2_pts);
            }

            filterByRange(fused, range_);
            voxelDownsample(fused, voxel_leaf_size_);
            capPointCount(fused, max_points_);
        }

        stats.fused_pts = fused.size();

        sensor_msgs::PointCloud2 out;
        pcl::toROSMsg(fused, out);
        out.header.stamp = msg->header.stamp;
        out.header.frame_id = output_frame_id_.empty()
            ? msg->header.frame_id
            : output_frame_id_;
        pub_.publish(out);

        if (frame_count_ == 1 || frame_count_ % static_cast<uint64_t>(stats_interval_) == 0) {
            ROS_INFO("fusion #%llu: main=%zu blind1=%zu(%d) blind2=%zu(%d) fused=%zu "
                     "enabled=%d",
                     static_cast<unsigned long long>(stats.frame_count),
                     stats.main_pts, stats.blind1_pts, stats.used_blind1 ? 1 : 0,
                     stats.blind2_pts, stats.used_blind2 ? 1 : 0,
                     stats.fused_pts, fusion_enabled_ ? 1 : 0);
        }
    }

    ros::Publisher  pub_;
    ros::Subscriber sub_main_;
    ros::Subscriber sub_blind1_;
    ros::Subscriber sub_blind2_;

    std::mutex mutex_;
    sensor_msgs::PointCloud2ConstPtr latest_blind1_;
    sensor_msgs::PointCloud2ConstPtr latest_blind2_;

    std::string main_topic_;
    std::string blind1_topic_;
    std::string blind2_topic_;
    std::string output_topic_;
    std::string output_frame_id_;

    bool   fusion_enabled_ = true;
    bool   use_blind1_     = true;
    bool   use_blind2_     = true;
    double blind_max_age_sec_  = 0.25;
    double blind_max_lead_sec_ = 0.05;
    RangeConfig range_;
    float  voxel_leaf_size_ = 0.f;
    size_t max_points_      = 300000;
    int    stats_interval_  = 30;
    uint64_t frame_count_   = 0;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lidar_cloud_fusion");
    ros::NodeHandle nh;
    ros::NodeHandle pnh("~");
    LidarCloudFusionNode node(nh, pnh);
    ros::spin();
    return 0;
}
