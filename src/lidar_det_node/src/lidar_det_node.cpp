// Thin ROS adapter around a pluggable ILidarDetector backend.
//
//   sub: sensor_msgs/PointCloud2            (param ~input_topic)
//   pub: comm_msg/boxArray                  (param ~output_topic)
//        visualization_msgs/MarkerArray     (param ~marker_topic)   [RViz]
//
// The model itself lives in an external backend .so (e.g. libtransfusion_backend.so)
// behind lidar_detector_api.hpp, so this node has no TensorRT/CUDA dependency
// and is identical on x86 and Orin.

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

#include <comm_msg/box.h>
#include <comm_msg/boxArray.h>

#include "lidar_det_interface/lidar_detector_api.hpp"

namespace {

// Default nuScenes(0..9) -> comm_msg/label.msg value mapping. Overridable via
// the ~class_map param. Unmapped ids fall back to 0 (UNKNOWN).
std::unordered_map<int, int> defaultClassMap() {
    return {
        {0, 4},   // car             -> CAR
        {1, 5},   // truck           -> TRUCK
        {2, 5},   // constr. vehicle -> TRUCK
        {3, 8},   // bus             -> BUS
        {4, 5},   // trailer         -> TRUCK
        {5, 23},  // barrier         -> METAL_BARRIER
        {6, 2},   // motorcycle      -> CYCLIST
        {7, 3},   // bicycle         -> BICYCLE
        {8, 1},   // pedestrian      -> PEDESTRIAN
        {9, 22},  // traffic_cone    -> TRAFFIC_CONE
    };
}

}  // namespace

class LidarDetAdapter {
public:
    explicit LidarDetAdapter(ros::NodeHandle& nh, ros::NodeHandle& pnh) : nh_(nh) {
        // --- params -------------------------------------------------------
        pnh.param<std::string>("input_topic",  input_topic_,  "/MainLidar/car_filtered_points");
        pnh.param<std::string>("output_topic", output_topic_, "/pcpt_net/box");
        pnh.param<std::string>("marker_topic", marker_topic_, "/detect_box_markers");
        pnh.param<std::string>("frame_id",     frame_id_,     std::string());  // "" => use input header
        pnh.param("lidar_height",    lidar_height_,    1.68);
        pnh.param("score_threshold", score_threshold_, 0.1);

        lidar_det::DetectorConfig cfg;
        pnh.param<std::string>("backend",     cfg.backend,     "transfusion");
        pnh.param<std::string>("model_dir",   cfg.model_dir,   std::string());
        pnh.param<std::string>("scn_onnx",    cfg.scn_onnx,    std::string());
        pnh.param<std::string>("bev_weights", cfg.bev_weights, std::string());
        pnh.param<std::string>("bev_engine",  cfg.bev_engine,  std::string());
        pnh.param<std::string>("head_engine", cfg.head_engine, std::string());
        pnh.param("feature_num", cfg.feature_num, 5);
        cfg.score_threshold = static_cast<float>(score_threshold_);
        pnh.param("verbose", cfg.verbose, false);

        loadClassMap(pnh);

        // --- backend ------------------------------------------------------
        detector_ = lidar_det::CreateLidarDetector(cfg);
        if (!detector_) {
            ROS_ERROR("CreateLidarDetector returned null for backend '%s'", cfg.backend.c_str());
        } else if (detector_->prepare() != 0) {
            ROS_ERROR("Detector prepare() failed; node will spin but produce no detections.");
            detector_.reset();
        } else {
            ROS_INFO("Detector backend '%s' ready.", cfg.backend.c_str());
        }
        feature_num_ = cfg.feature_num;

        // --- pub/sub ------------------------------------------------------
        pub_box_    = nh_.advertise<comm_msg::boxArray>(output_topic_, 1, true);
        pub_marker_ = nh_.advertise<visualization_msgs::MarkerArray>(marker_topic_, 1, true);
        sub_cloud_  = nh_.subscribe(input_topic_, 1, &LidarDetAdapter::cloudCb, this);
        ROS_INFO("lidar_det_node: sub '%s' -> pub '%s'", input_topic_.c_str(), output_topic_.c_str());
    }

private:
    void loadClassMap(ros::NodeHandle& pnh) {
        class_map_ = defaultClassMap();
        std::map<std::string, int> override_map;
        if (pnh.getParam("class_map", override_map)) {
            for (const auto& kv : override_map) {
                try {
                    class_map_[std::stoi(kv.first)] = kv.second;
                } catch (const std::exception&) {
                    ROS_WARN("Ignoring non-integer class_map key '%s'", kv.first.c_str());
                }
            }
            ROS_INFO("class_map overridden with %zu entries.", override_map.size());
        }
    }

    int mapLabel(int native) const {
        auto it = class_map_.find(native);
        return it != class_map_.end() ? it->second : 0;  // 0 = UNKNOWN
    }

    void cloudCb(const sensor_msgs::PointCloud2ConstPtr& msg) {
        pcl::PointCloud<pcl::PointXYZI> cloud;
        pcl::fromROSMsg(*msg, cloud);
        const size_t n = cloud.size();

        const std::string out_frame = frame_id_.empty() ? msg->header.frame_id : frame_id_;

        comm_msg::boxArray arr;
        arr.header = msg->header;
        if (!frame_id_.empty()) arr.header.frame_id = frame_id_;

        if (detector_ && n > 0) {
            buf_.resize(n * feature_num_, 0.f);
            for (size_t i = 0; i < n; ++i) {
                float* p = buf_.data() + i * feature_num_;
                p[0] = cloud[i].x;
                p[1] = cloud[i].y;
                p[2] = cloud[i].z - static_cast<float>(lidar_height_);
                if (feature_num_ > 3) p[3] = cloud[i].intensity;
                if (feature_num_ > 4) p[4] = 0.f;  // time
            }
            const auto& dets =
                detector_->infer(buf_.data(), static_cast<int>(n), feature_num_);

            for (const auto& d : dets) {
                if (d.score < static_cast<float>(score_threshold_)) continue;
                comm_msg::box b;
                b.label.value = static_cast<uint8_t>(mapLabel(d.label));
                b.x = d.x;  b.y = d.y;  b.z = d.z;
                b.width = d.w;  b.length = d.l;  b.height = d.h;
                b.yaw = d.yaw;
                b.vel_x = d.vx;  b.vel_y = d.vy;  b.vel_z = 0.f;
                arr.boxes.push_back(b);
            }
        }

        pub_box_.publish(arr);
        publishMarkers(arr.boxes, out_frame, msg->header.stamp);
    }

    void publishMarkers(const std::vector<comm_msg::box>& boxes,
                        const std::string& frame, const ros::Time& stamp) {
        if (pub_marker_.getNumSubscribers() == 0) return;
        visualization_msgs::MarkerArray ma;

        visualization_msgs::Marker clear;
        clear.header.frame_id = frame;
        clear.header.stamp = stamp;
        clear.action = visualization_msgs::Marker::DELETEALL;
        ma.markers.push_back(clear);

        int id = 0;
        for (const auto& b : boxes) {
            visualization_msgs::Marker m;
            m.header.frame_id = frame;
            m.header.stamp = stamp;
            m.ns = "det";
            m.id = id++;
            m.type = visualization_msgs::Marker::CUBE;
            m.action = visualization_msgs::Marker::ADD;
            m.pose.position.x = b.x;
            m.pose.position.y = b.y;
            m.pose.position.z = b.z;
            const double half = 0.5 * b.yaw;
            m.pose.orientation.z = std::sin(half);
            m.pose.orientation.w = std::cos(half);
            m.scale.x = std::max(0.05f, b.width);
            m.scale.y = std::max(0.05f, b.length);
            m.scale.z = std::max(0.05f, b.height);
            m.color.r = 0.0;  m.color.g = 0.9;  m.color.b = 0.93;  m.color.a = 0.6;
            m.lifetime = ros::Duration(0.2);
            ma.markers.push_back(m);
        }
        pub_marker_.publish(ma);
    }

    ros::NodeHandle nh_;
    ros::Subscriber sub_cloud_;
    ros::Publisher  pub_box_;
    ros::Publisher  pub_marker_;

    std::unique_ptr<lidar_det::ILidarDetector> detector_;
    std::unordered_map<int, int>               class_map_;
    std::vector<float>                          buf_;

    std::string input_topic_, output_topic_, marker_topic_, frame_id_;
    double      lidar_height_   = 1.68;
    double      score_threshold_ = 0.1;
    int         feature_num_    = 5;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lidar_det_node");
    ros::NodeHandle nh;
    ros::NodeHandle pnh("~");
    LidarDetAdapter adapter(nh, pnh);
    ros::spin();
    return 0;
}
