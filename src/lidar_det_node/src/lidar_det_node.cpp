// Thin ROS adapter around a pluggable ILidarDetector backend.
//
//   sub: sensor_msgs/PointCloud2            (param ~input_topic)
//   pub: comm_msg/boxArray                  (param ~output_topic)
//        visualization_msgs/MarkerArray     (param ~marker_topic)   [RViz]
//        std_msgs/Float32MultiArray         (param ~health_topic)   [optional]
//
// The model itself lives in an external backend .so (e.g. libtransfusion_backend.so)
// behind lidar_detector_api.hpp, so this node has no TensorRT/CUDA dependency
// and is identical on x86 and Orin.

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <ros/package.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float32MultiArray.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

#include <comm_msg/box.h>
#include <comm_msg/boxArray.h>
#include <comm_msg/label.h>

#include "comm/comm_dbg_log.h"

#include "lidar_det_interface/lidar_detector_api.hpp"

namespace {

struct FrameMetrics {
    size_t pts_in              = 0;
    size_t raw_det             = 0;
    size_t kept                = 0;
    size_t skipped_unmapped    = 0;
    size_t skipped_non_output  = 0;
    size_t skipped_class_score = 0;
    float  best_score          = 0.f;
    int    best_native_lbl     = -1;
    float  top_kept_score      = -1.f;
    int    top_kept_native_lbl = -1;
    uint8_t top_kept_mapped_lbl = 0;
    float  top_x               = 0.f;
    float  top_y               = 0.f;
    double infer_ms            = 0.0;
    std::unordered_map<uint8_t, int> mapped_counts;
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
        case sensor_msgs::PointField::UINT8: {
            uint8_t v = msg.data[byte_offset];
            return static_cast<float>(v);
        }
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

// Always convert via PointXYZ to avoid PCL warnings when intensity is missing
// or present with a non-standard datatype/layout.
bool fromRosMsgToCloud(const sensor_msgs::PointCloud2& msg,
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
    return has_intensity;
}

// Model-native id -> comm_msg/label.msg (intermediate mapping).
std::unordered_map<int, int> classMapNuScenes10() {
    return {
        {0, comm_msg::label::CAR},            // car
        {1, comm_msg::label::TRUCK},          // truck
        {2, comm_msg::label::TRUCK},          // constr. vehicle
        {3, comm_msg::label::BUS},            // bus
        {4, comm_msg::label::TRUCK},          // trailer
        {5, comm_msg::label::METAL_BARRIER},  // barrier
        {6, comm_msg::label::CYCLIST},        // motorcycle
        {7, comm_msg::label::BICYCLE},        // bicycle
        {8, comm_msg::label::PEDESTRIAN},     // pedestrian
        {9, comm_msg::label::TRAFFIC_CONE},   // traffic_cone
        {10, comm_msg::label::TRICYCLE},      // tricycle (0825 nanning 11-cls)
    };
}

// Fine-tuned model: native 0=CAR, 1=CYCLIST, 2=PEDESTRIAN.
std::unordered_map<int, int> classMapCustom3() {
    return {
        {0, comm_msg::label::CAR},
        {1, comm_msg::label::CYCLIST},
        {2, comm_msg::label::PEDESTRIAN},
    };
}

std::unordered_map<int, int> classMapForProfile(const std::string& profile) {
    if (profile == "custom_3") return classMapCustom3();
    if (profile != "nuscenes_10") {
        ST_LOG_WARN("Unknown class_map_profile '%s', using nuscenes_10.",
                    profile.c_str());
    }
    return classMapNuScenes10();
}

// Collapse comm labels to the three categories published downstream.
// Returns false when the detection should be dropped (barrier, cone, UNKNOWN, ...).
bool collapseToOutputLabel(uint8_t comm, uint8_t& out) {
    switch (comm) {
        case comm_msg::label::PEDESTRIAN:
            out = comm_msg::label::PEDESTRIAN;
            return true;
        case comm_msg::label::CYCLIST:
        case comm_msg::label::BICYCLE:
        case comm_msg::label::TRICYCLE:
            out = comm_msg::label::CYCLIST;
            return true;
        case comm_msg::label::CAR:
        case comm_msg::label::TRUCK:
        case comm_msg::label::BUS:
        case comm_msg::label::TRAM:
            out = comm_msg::label::CAR;
            return true;
        default:
            return false;
    }
}

namespace fs = std::filesystem;

std::string absolutePath(const fs::path& p) {
    std::error_code ec;
    const fs::path canon = fs::weakly_canonical(p, ec);
    return ec ? fs::absolute(p).lexically_normal().string() : canon.string();
}

bool isExistingDir(const std::string& path) {
    std::error_code ec;
    return !path.empty() && fs::is_directory(fs::path(path), ec) && !ec;
}

// Default filenames match TransFusion::Config (transfusion.hpp).
std::string autoTransFusionModelDir() {
    const std::string pkg = ros::package::getPath("lidar_det_node");
    if (pkg.empty()) return "";

    const std::vector<std::string> rels = {
        "vendor/model",
        "model",
        "../../../3d_target_detection/deploy/Lidar_orin_Solution/TransFusion/model",
    };
    for (const auto& rel : rels) {
        const std::string cand = absolutePath(fs::path(pkg) / rel);
        if (isExistingDir(cand)) return cand;
    }
    return "";
}

std::string joinModelPath(const std::string& model_dir, const std::string& path) {
    if (path.empty()) return path;
    const fs::path p(path);
    if (p.is_absolute()) return absolutePath(p);
    if (model_dir.empty()) return path;
    return absolutePath(fs::path(model_dir) / p);
}

void resolveDetectorModelPaths(lidar_det::DetectorConfig& cfg) {
    if (cfg.model_dir.empty()) {
        cfg.model_dir = autoTransFusionModelDir();
    } else {
        const fs::path p(cfg.model_dir);
        if (!p.is_absolute()) {
            const std::string pkg = ros::package::getPath("lidar_det_node");
            if (!pkg.empty()) {
                cfg.model_dir = absolutePath(fs::path(pkg) / p);
            }
        } else {
            cfg.model_dir = absolutePath(p);
        }
    }

    if (cfg.scn_onnx.empty())    cfg.scn_onnx    = "transfusion_fast30.scn.onnx";
    if (cfg.bev_weights.empty()) cfg.bev_weights = "sparse_backbone_weights_fast30.bin";
    if (cfg.bev_engine.empty())  cfg.bev_engine  = "transfusion_bev_fast30_h100.engine";
    if (cfg.head_engine.empty()) cfg.head_engine = "transfusion_head_fast30_fp16.engine";

    cfg.scn_onnx    = joinModelPath(cfg.model_dir, cfg.scn_onnx);
    cfg.bev_weights = joinModelPath(cfg.model_dir, cfg.bev_weights);
    cfg.bev_engine  = joinModelPath(cfg.model_dir, cfg.bev_engine);
    cfg.head_engine = joinModelPath(cfg.model_dir, cfg.head_engine);

    ST_LOG_INFO("model_dir=%s", cfg.model_dir.c_str());
    ST_LOG_INFO("scn_onnx=%s", cfg.scn_onnx.c_str());
    ST_LOG_INFO("bev_weights=%s", cfg.bev_weights.c_str());
    ST_LOG_INFO("bev_engine=%s", cfg.bev_engine.c_str());
    ST_LOG_INFO("head_engine=%s", cfg.head_engine.c_str());
}

const char* nuScenesClassName(int native_id) {
    static const char* kNames[] = {
        "car", "truck", "constr_vehicle", "bus", "trailer",
        "barrier", "motorcycle", "bicycle", "pedestrian", "traffic_cone",
        "tricycle",
    };
    if (native_id >= 0 && native_id < static_cast<int>(sizeof(kNames) / sizeof(kNames[0]))) {
        return kNames[native_id];
    }
    return "invalid";
}

const char* custom3ClassName(int native_id) {
    static const char* kNames[] = {"CAR", "PEDESTRIAN","CYCLIST", };
    if (native_id >= 0 && native_id < static_cast<int>(sizeof(kNames) / sizeof(kNames[0]))) {
        return kNames[native_id];
    }
    return "invalid";
}

const char* nativeClassName(const std::string& profile, int native_id) {
    if (profile == "custom_3") return custom3ClassName(native_id);
    return nuScenesClassName(native_id);
}

// comm_msg/label.msg name for mapped output categories.
const char* commLabelName(uint8_t mapped) {
    switch (mapped) {
        case comm_msg::label::UNKNOWN:        return "UNKNOWN";
        case comm_msg::label::PEDESTRIAN:     return "PEDESTRIAN";
        case comm_msg::label::CYCLIST:        return "CYCLIST";
        case comm_msg::label::BICYCLE:        return "BICYCLE";
        case comm_msg::label::CAR:            return "CAR";
        case comm_msg::label::TRUCK:          return "TRUCK";
        case comm_msg::label::BUS:            return "BUS";
        case comm_msg::label::TRAM:           return "TRAM";
        case comm_msg::label::TRICYCLE:       return "TRICYCLE";
        case comm_msg::label::TRAFFIC_CONE:   return "TRAFFIC_CONE";
        case comm_msg::label::METAL_BARRIER:  return "METAL_BARRIER";
        default:                              return "OTHER";
    }
}

// Published label counts (CAR / CYCLIST / PEDESTRIAN only).
std::string formatMappedLabelCounts(const std::unordered_map<uint8_t, int>& counts) {
    static const uint8_t kOrder[] = {
        comm_msg::label::CAR,
        comm_msg::label::CYCLIST,
        comm_msg::label::PEDESTRIAN,
    };

    std::string out;
    for (uint8_t lbl : kOrder) {
        const auto it = counts.find(lbl);
        const int n = (it != counts.end()) ? it->second : 0;
        if (n == 0) continue;
        if (!out.empty()) out += ' ';
        out += commLabelName(lbl);
        out += '=';
        out += std::to_string(n);
    }

    for (const auto& kv : counts) {
        bool listed = false;
        for (uint8_t lbl : kOrder) {
            if (kv.first == lbl) { listed = true; break; }
        }
        if (listed || kv.second == 0) continue;
        if (!out.empty()) out += ' ';
        out += commLabelName(kv.first);
        out += '=';
        out += std::to_string(kv.second);
    }

    return out.empty() ? "(none)" : out;
}

void bumpMappedLabelCount(uint8_t mapped, FrameMetrics& m) {
    ++m.mapped_counts[mapped];
}

std::string expandUserPath(const std::string& path) {
    if (path.empty() || path[0] != '~') return path;
    const char* home = std::getenv("HOME");
    return (home != nullptr) ? (std::string(home) + path.substr(1)) : path;
}

struct StorageQuota {
    int64_t max_total_bytes = 0;
    size_t  max_files       = 0;
};

void pruneDirectoryFiles(const fs::path& dir,
                         const std::string& name_prefix,
                         const StorageQuota& quota) {
    if ((quota.max_total_bytes <= 0 && quota.max_files == 0) || name_prefix.empty()) {
        return;
    }

    std::error_code ec;
    if (!fs::exists(dir, ec)) return;

    struct FileEntry {
        fs::path path;
        fs::file_time_type mtime;
        int64_t size = 0;
    };
    std::vector<FileEntry> files;

    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (ec || !entry.is_regular_file(ec)) continue;
        const std::string name = entry.path().filename().string();
        if (name.compare(0, name_prefix.size(), name_prefix) != 0) continue;
        FileEntry fe;
        fe.path  = entry.path();
        fe.mtime = entry.last_write_time(ec);
        fe.size  = static_cast<int64_t>(fs::file_size(fe.path, ec));
        files.push_back(fe);
    }

    auto totalBytes = [&files]() {
        int64_t sum = 0;
        for (const auto& f : files) sum += f.size;
        return sum;
    };

    std::sort(files.begin(), files.end(),
              [](const FileEntry& a, const FileEntry& b) { return a.mtime < b.mtime; });

    while (!files.empty()) {
        const bool too_many =
            quota.max_files > 0 && files.size() > quota.max_files;
        const bool too_large =
            quota.max_total_bytes > 0 && totalBytes() > quota.max_total_bytes;
        if (!too_many && !too_large) break;

        std::error_code rm_ec;
        fs::remove(files.front().path, rm_ec);
        files.erase(files.begin());
    }
}

void rotateStLogFile(const std::string& log_path, int64_t max_single_bytes) {
    if (max_single_bytes <= 0 || pLogFileHandle == nullptr || log_path.empty()) return;

    g_objLogMutex.lock();
    fflush(pLogFileHandle);
    const long cur_size = ftell(pLogFileHandle);
    g_objLogMutex.unlock();
    if (cur_size < max_single_bytes) return;

    g_objLogMutex.lock();
    if (pLogFileHandle != nullptr) {
        fclose(pLogFileHandle);
        pLogFileHandle = nullptr;
    }
    g_objLogMutex.unlock();

    constexpr int kMaxRotated = 4;
    for (int i = kMaxRotated; i >= 1; --i) {
        const fs::path dst = log_path + "." + std::to_string(i);
        std::error_code ec;
        if (fs::exists(dst, ec)) fs::remove(dst, ec);
        if (i == 1) {
            if (fs::exists(log_path, ec)) fs::rename(log_path, dst, ec);
        } else {
            const fs::path src = log_path + "." + std::to_string(i - 1);
            if (fs::exists(src, ec)) fs::rename(src, dst, ec);
        }
    }

    g_objLogMutex.lock();
    pLogFileHandle = fopen(log_path.c_str(), "w");
    g_objLogMutex.unlock();

    if (pLogFileHandle != nullptr) {
        ST_LOG_INFO("log rotated: %s exceeded %lld bytes.",
                  log_path.c_str(), static_cast<long long>(max_single_bytes));
    } else {
        ST_LOG_ERR("log rotate failed: cannot reopen %s.", log_path.c_str());
    }
}

class AbnormalCloudSaver {
public:
    void configure(ros::NodeHandle& pnh) {
        pnh.param("save_abnormal_cloud", enabled_, false);
        pnh.param<std::string>("abnormal_cloud_dir", dir_,
                                 std::string("~/map_config/log/debug/lidar_det_abnormal"));
        int max_total_mb = 512;
        int max_files = 500;
        pnh.param("abnormal_cloud_max_total_mb", max_total_mb, 512);
        pnh.param("abnormal_cloud_max_files", max_files, 500);
        pnh.param("abnormal_cloud_min_interval_sec", min_interval_sec_, 2.0);

        dir_ = expandUserPath(dir_);
        quota_.max_total_bytes =
            max_total_mb > 0 ? static_cast<int64_t>(max_total_mb) * 1024 * 1024 : 0;
        quota_.max_files = max_files > 0 ? static_cast<size_t>(max_files) : 0;

        if (!enabled_) return;

        std::error_code ec;
        fs::create_directories(fs::path(dir_), ec);
        pruneDirectoryFiles(fs::path(dir_), "abnormal_", quota_);
        ST_LOG_INFO("abnormal cloud save enabled: dir=%s max_total_mb=%d max_files=%d "
                    "min_interval_sec=%.1f",
                    dir_.c_str(), max_total_mb, max_files, min_interval_sec_);
    }

    void trySave(const std::string& reason,
                 const sensor_msgs::PointCloud2ConstPtr& msg,
                 const pcl::PointCloud<pcl::PointXYZI>& cloud,
                 uint64_t frame_count,
                 const FrameMetrics& metrics) {
        if (!enabled_ || reason.empty() || !msg) return;

        const ros::Time now = ros::Time::now();
        const auto it = last_save_.find(reason);
        if (it != last_save_.end() &&
            (now - it->second).toSec() < min_interval_sec_) {
            return;
        }

        pruneDirectoryFiles(fs::path(dir_), "abnormal_", quota_);

        std::ostringstream base;
        base << "abnormal_" << reason << "_f" << frame_count << "_"
             << std::fixed << std::setprecision(3) << msg->header.stamp.toSec();
        const std::string pcd_path = (fs::path(dir_) / (base.str() + ".pcd")).string();
        const std::string txt_path = (fs::path(dir_) / (base.str() + ".txt")).string();

        if (pcl::io::savePCDFileBinary(pcd_path, cloud) != 0) {
            ST_LOG_WARN("abnormal cloud save failed: %s", pcd_path.c_str());
            return;
        }

        std::ofstream meta(txt_path);
        if (meta) {
            meta << "reason=" << reason << "\n"
                 << "frame=" << frame_count << "\n"
                 << "stamp=" << msg->header.stamp.toSec() << "\n"
                 << "frame_id=" << msg->header.frame_id << "\n"
                 << "pts=" << metrics.pts_in << "\n"
                 << "raw_det=" << metrics.raw_det << "\n"
                 << "kept=" << metrics.kept << "\n"
                 << "best_score=" << metrics.best_score << "\n"
                 << "infer_ms=" << metrics.infer_ms << "\n";
        }

        last_save_[reason] = now;
        ST_LOG_WARN("abnormal cloud saved: reason=%s pts=%zu file=%s",
                    reason.c_str(), cloud.size(), pcd_path.c_str());
    }

private:
    bool         enabled_ = false;
    std::string  dir_;
    StorageQuota quota_;
    double       min_interval_sec_ = 2.0;
    std::map<std::string, ros::Time> last_save_;
};

struct DetDumpRecord {
    size_t det_idx = 0;
    lidar_det::DetectionBox det;
    const char* filter_reason = "published";
    bool published = false;
    uint8_t out_label = 0;
};

struct MarkerVisItem {
    comm_msg::box box;
    float         score = 0.f;
    int           native_id = -1;
};

void classMarkerColor(uint8_t out_label, float score,
                      float& r, float& g, float& b, float& a) {
    a = 0.55f;
    switch (out_label) {
        case comm_msg::label::PEDESTRIAN:
            r = 0.2f; g = 0.95f; b = 0.2f;
            break;
        case comm_msg::label::CAR:
            r = 0.2f; g = 0.45f; b = 1.0f;
            break;
        case comm_msg::label::CYCLIST:
            r = 1.0f; g = 0.85f; b = 0.1f;
            break;
        default:
            r = 0.0f; g = 0.9f; b = 0.93f;
            break;
    }
    // Lower score -> slightly more transparent (quick FP hint in RViz).
    if (score < 0.25f) {
        a = 0.35f;
    } else if (score < 0.35f) {
        a = 0.45f;
    }
}

std::string markerLabelText(uint8_t out_label, float score, int native_id,
                            float vel_x, float vel_y,
                            bool show_velocity,
                            float velocity_min_mps,
                            const std::string& class_map_profile) {
    const char* short_name = "?";
    switch (out_label) {
        case comm_msg::label::PEDESTRIAN: short_name = "PED"; break;
        case comm_msg::label::CAR:        short_name = "CAR"; break;
        case comm_msg::label::CYCLIST:    short_name = "CYC"; break;
        default:
            short_name = commLabelName(out_label);
            break;
    }
    std::ostringstream oss;
    oss << short_name << ' ' << std::fixed << std::setprecision(2) << score;
    if (show_velocity) {
        const float speed = std::hypot(vel_x, vel_y);
        oss << " v=" << std::setprecision(2) << speed << "m/s";
        if (speed >= velocity_min_mps) {
            oss << " (" << std::setprecision(1) << vel_x << ',' << vel_y << ')';
        }
    }
    if (native_id >= 0) {
        oss << " [" << nativeClassName(class_map_profile, native_id) << ']';
    }
    return oss.str();
}

// Optional per-frame TransFusion dump (TSV). Disabled by default; zero cost when off.
class DetDumpLogger {
public:
    void configure(ros::NodeHandle& pnh,
                   const std::string& backend,
                   const std::string& class_map_profile,
                   const std::string& input_topic,
                   double score_threshold,
                   double score_threshold_pedestrian,
                   double score_threshold_car,
                   double score_threshold_cyclist) {
        pnh.param("det_dump_enabled", enabled_, false);
        pnh.param<std::string>("det_dump_file", file_path_, std::string());
        pnh.param<std::string>("det_dump_dir",
                               dump_dir_,
                               std::string("~/map_config/log/debug/lidar_det_dump"));
        pnh.param("det_dump_flush_every", flush_every_, 30);
        int max_total_mb = 512;
        int max_files = 50;
        pnh.param("det_dump_max_total_mb", max_total_mb, 512);
        pnh.param("det_dump_max_files", max_files, 50);

        backend_            = backend;
        class_map_profile_  = class_map_profile;
        input_topic_        = input_topic;
        score_threshold_    = score_threshold;
        score_threshold_pedestrian_ = score_threshold_pedestrian;
        score_threshold_car_        = score_threshold_car;
        score_threshold_cyclist_    = score_threshold_cyclist;
        dump_dir_           = expandUserPath(dump_dir_);
        quota_.max_total_bytes =
            max_total_mb > 0 ? static_cast<int64_t>(max_total_mb) * 1024 * 1024 : 0;
        quota_.max_files = max_files > 0 ? static_cast<size_t>(max_files) : 0;

        if (!enabled_) return;

        if (file_path_.empty()) {
            const std::time_t now = std::time(nullptr);
            std::tm tm_local{};
            localtime_r(&now, &tm_local);
            std::ostringstream name;
            name << "lidar_det_dump_"
                 << std::put_time(&tm_local, "%Y%m%d_%H%M%S") << ".tsv";
            file_path_ = (fs::path(dump_dir_) / name.str()).string();
        } else {
            file_path_ = expandUserPath(file_path_);
        }

        if (!openFile()) {
            enabled_ = false;
            return;
        }

        pruneDirectoryFiles(fs::path(dump_dir_), "lidar_det_dump_", quota_);
        ST_LOG_INFO("det dump enabled: file=%s flush_every=%d",
                    file_path_.c_str(), flush_every_);
    }

    bool enabled() const { return enabled_; }

    void logFrame(uint64_t frame_count,
                  const sensor_msgs::PointCloud2ConstPtr& msg,
                  const FrameMetrics& metrics,
                  const std::string& out_frame,
                  const std::vector<DetDumpRecord>& records) {
        if (!enabled_ || !out_ || !msg) return;

        const double stamp = msg->header.stamp.toSec();
        if (records.empty()) {
            writeFrameRow(frame_count, stamp, msg->header.frame_id, out_frame,
                          metrics, -1, nullptr, "no_detection", false, 0);
        } else {
            for (const auto& rec : records) {
                writeFrameRow(frame_count, stamp, msg->header.frame_id, out_frame,
                              metrics, static_cast<int>(rec.det_idx), &rec.det,
                              rec.filter_reason, rec.published, rec.out_label);
            }
        }

        if (flush_every_ > 0 &&
            frame_count % static_cast<uint64_t>(flush_every_) == 0) {
            out_->flush();
        }
    }

    ~DetDumpLogger() { close(); }

private:
    std::string timestampString() const {
        const std::time_t now = std::time(nullptr);
        std::tm tm_local{};
        localtime_r(&now, &tm_local);
        std::ostringstream oss;
        oss << std::put_time(&tm_local, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    bool openFile() {
        std::error_code ec;
        fs::create_directories(fs::path(file_path_).parent_path(), ec);
        out_.reset(new std::ofstream(file_path_, std::ios::out | std::ios::trunc));
        if (!out_ || !*out_) {
            ST_LOG_ERR("det dump: cannot open %s", file_path_.c_str());
            out_.reset();
            return false;
        }

        *out_ << "# lidar_det_node TransFusion detection dump\n"
              << "# created=" << timestampString() << "\n"
              << "# backend=" << backend_
              << " score_threshold=" << score_threshold_
              << " score_threshold_pedestrian=" << score_threshold_pedestrian_
              << " score_threshold_car=" << score_threshold_car_
              << " score_threshold_cyclist=" << score_threshold_cyclist_
              << " class_map_profile=" << class_map_profile_
              << " input_topic=" << input_topic_ << "\n"
              << "frame\tcloud_stamp_sec\tcloud_frame_id\tout_frame_id\t"
              << "pts_in\tinfer_ms\traw_det\tkept\tdet_idx\tnative_id\tnative_name\t"
              << "score\tx\ty\tz\tw\tl\th\tyaw\tvx\tvy\t"
              << "filter_reason\tpublished\tout_label\tout_label_name\n";
        return true;
    }

    void writeFrameRow(uint64_t frame_count,
                       double cloud_stamp_sec,
                       const std::string& cloud_frame_id,
                       const std::string& out_frame_id,
                       const FrameMetrics& metrics,
                       int det_idx,
                       const lidar_det::DetectionBox* det,
                       const char* filter_reason,
                       bool published,
                       uint8_t out_label) {
        if (!out_) return;

        *out_ << frame_count << '\t'
              << std::fixed << std::setprecision(6) << cloud_stamp_sec << '\t'
              << cloud_frame_id << '\t'
              << out_frame_id << '\t'
              << metrics.pts_in << '\t'
              << std::setprecision(3) << metrics.infer_ms << '\t'
              << metrics.raw_det << '\t'
              << metrics.kept << '\t'
              << det_idx << '\t';

        if (det == nullptr) {
            *out_ << -1 << "\t\t"
                  << std::setprecision(4) << 0.f << '\t'
                  << std::setprecision(3)
                  << 0.f << '\t' << 0.f << '\t' << 0.f << '\t'
                  << 0.f << '\t' << 0.f << '\t' << 0.f << '\t'
                  << 0.f << '\t' << 0.f << '\t' << 0.f << '\t'
                  << filter_reason << '\t'
                  << 0 << '\t' << 0 << '\t'
                  << '\n';
            return;
        }

        *out_ << det->label << '\t'
              << nativeClassName(class_map_profile_, det->label) << '\t'
              << std::setprecision(4) << det->score << '\t'
              << std::setprecision(3)
              << det->x << '\t' << det->y << '\t' << det->z << '\t'
              << det->w << '\t' << det->l << '\t' << det->h << '\t'
              << det->yaw << '\t'
              << det->vx << '\t' << det->vy << '\t'
              << filter_reason << '\t'
              << (published ? 1 : 0) << '\t'
              << static_cast<int>(out_label) << '\t'
              << commLabelName(out_label) << '\n';
    }

    void close() {
        if (out_) {
            out_->flush();
            out_.reset();
        }
    }

    bool         enabled_ = false;
    std::string  file_path_;
    std::string  dump_dir_;
    std::string  backend_;
    std::string  class_map_profile_;
    std::string  input_topic_;
    double       score_threshold_ = 0.1;
    double       score_threshold_pedestrian_ = 0.1;
    double       score_threshold_car_ = 0.1;
    double       score_threshold_cyclist_ = 0.1;
    int          flush_every_     = 30;
    StorageQuota quota_;
    std::unique_ptr<std::ofstream> out_;
};

}  // namespace

class LidarDetAdapter {
public:
    explicit LidarDetAdapter(ros::NodeHandle& nh, ros::NodeHandle& pnh,
                             const std::string& log_file_path)
        : nh_(nh), log_file_path_(log_file_path) {
        pnh.param<std::string>("input_topic",  input_topic_,  "/MainLidar/car_filtered_points");
        pnh.param<std::string>("output_topic", output_topic_, "/pcpt_net/box");
        pnh.param<std::string>("marker_topic", marker_topic_, "/detect_box_markers");
        pnh.param("marker_show_text", marker_show_text_, true);
        pnh.param("marker_show_velocity", marker_show_velocity_, true);
        pnh.param("marker_show_velocity_arrow", marker_show_velocity_arrow_, true);
        pnh.param("marker_velocity_min_mps", marker_velocity_min_mps_, 0.1);
        pnh.param("marker_velocity_arrow_scale", marker_velocity_arrow_scale_, 3.0);
        pnh.param("marker_velocity_arrow_min_length", marker_velocity_arrow_min_length_, 1.5);
        pnh.param("marker_velocity_arrow_shaft", marker_velocity_arrow_shaft_, 0.05);
        pnh.param("marker_velocity_arrow_head", marker_velocity_arrow_head_, 0.10);
        pnh.param("marker_color_by_class", marker_color_by_class_, true);
        pnh.param("marker_text_scale", marker_text_scale_, 0.45);
        pnh.param("marker_text_z_offset", marker_text_z_offset_, 0.6);
        pnh.param<std::string>("frame_id",     frame_id_,     std::string());
        pnh.param("lidar_height",    lidar_height_,    1.68);
        loadScoreThresholds(pnh);
        // true  = 把点云 intensity 写入特征 p[3]
        // false = 强制 p[3]=0（仿真/bag 或跨雷达时更稳）
        pnh.param("use_intensity",   use_intensity_,   false);

        pnh.param("debug_stats",       debug_stats_,       true);
        pnh.param("stats_interval",    stats_interval_,    30);
        pnh.param("summary_interval",  summary_interval_,  60);
        pnh.param("input_timeout_sec", input_timeout_sec_, 1.0);
        pnh.param<std::string>("health_topic", health_topic_, "/lidar_det/health");
        pnh.param("log_max_total_mb", log_max_total_mb_, 100);
        pnh.param("log_rotate_single_mb", log_rotate_single_mb_, 20);

        if (stats_interval_ < 1)   stats_interval_   = 1;
        if (summary_interval_ < 1) summary_interval_ = 1;

        abnormal_saver_.configure(pnh);

        lidar_det::DetectorConfig cfg;
        pnh.param<std::string>("backend",     cfg.backend,     "transfusion");
        pnh.param<std::string>("model_dir",   cfg.model_dir,   std::string());
        pnh.param<std::string>("scn_onnx",    cfg.scn_onnx,    std::string());
        pnh.param<std::string>("bev_weights", cfg.bev_weights, std::string());
        pnh.param<std::string>("bev_engine",  cfg.bev_engine,  std::string());
        pnh.param<std::string>("head_engine", cfg.head_engine, std::string());
        pnh.param("feature_num", cfg.feature_num, 5);
        cfg.score_threshold = static_cast<float>(score_threshold_);
        pnh.param("verbose", verbose_, false);
        cfg.verbose = verbose_;

        pnh.param<std::string>("class_map_profile", class_map_profile_, "nuscenes_10");
        pnh.param("filter_unmapped_labels", filter_unmapped_labels_, true);
        pnh.param("collapse_to_output_labels", collapse_to_output_labels_, true);
        loadClassMap(pnh);
        resolveDetectorModelPaths(cfg);
        det_dumper_.configure(pnh, cfg.backend, class_map_profile_,
                              input_topic_, score_threshold_,
                              score_threshold_pedestrian_, score_threshold_car_,
                              score_threshold_cyclist_);
        logStartupParams(cfg);

        detector_ = lidar_det::CreateLidarDetector(cfg);
        if (!detector_) {
            ST_LOG_ERR("CreateLidarDetector returned null for backend '%s'.", cfg.backend.c_str());
        } else if (detector_->prepare() != 0) {
            ST_LOG_ERR("Detector prepare() failed; node will spin but produce no detections.");
            detector_.reset();
        } else {
            ST_LOG_INFO("Detector backend '%s' ready.", cfg.backend.c_str());
        }
        feature_num_ = cfg.feature_num;

        pub_box_    = nh_.advertise<comm_msg::boxArray>(output_topic_, 1, true);
        pub_marker_ = nh_.advertise<visualization_msgs::MarkerArray>(marker_topic_, 1, true);
        if (!health_topic_.empty()) {
            pub_health_ = nh_.advertise<std_msgs::Float32MultiArray>(health_topic_, 1, true);
            ST_LOG_INFO("health topic enabled: %s", health_topic_.c_str());
        }
        sub_cloud_ = nh_.subscribe(input_topic_, 1, &LidarDetAdapter::cloudCb, this);

        if (debug_stats_ && input_timeout_sec_ > 0.0) {
            stall_timer_ = nh_.createTimer(
                ros::Duration(input_timeout_sec_),
                &LidarDetAdapter::stallCheckCb, this);
        }

        if (!log_file_path_.empty() &&
            (log_max_total_mb_ > 0 || log_rotate_single_mb_ > 0)) {
            log_maint_timer_ = nh_.createTimer(
                ros::Duration(30.0), &LidarDetAdapter::logMaintainCb, this);
        }

        ST_LOG_INFO("lidar_det_node: sub '%s' -> pub '%s' marker '%s'.",
                    input_topic_.c_str(), output_topic_.c_str(), marker_topic_.c_str());
        if (verbose_) {
            ST_LOG_INFO("verbose=true: backend stage timing goes to stdout/stderr "
                        "(navZero wrapper -> lidar_det_node.log), not ST log file.");
        }
    }

private:
    void logStartupParams(const lidar_det::DetectorConfig& cfg) const {
        ST_LOG_INFO("params: backend=%s score_threshold=%.3f "
                    "per_class(ped=%.3f car=%.3f cyc=%.3f) lidar_height=%.2f "
                    "feature_num=%d use_intensity=%d verbose=%d",
                    cfg.backend.c_str(), score_threshold_,
                    score_threshold_pedestrian_, score_threshold_car_,
                    score_threshold_cyclist_, lidar_height_,
                    cfg.feature_num, use_intensity_ ? 1 : 0, verbose_ ? 1 : 0);
        ST_LOG_INFO("params: debug_stats=%d stats_interval=%d summary_interval=%d "
                    "input_timeout_sec=%.2f",
                    debug_stats_ ? 1 : 0, stats_interval_, summary_interval_,
                    input_timeout_sec_);
        ST_LOG_INFO("topics: in=%s out=%s marker=%s frame_id=%s health=%s",
                    input_topic_.c_str(), output_topic_.c_str(), marker_topic_.c_str(),
                    frame_id_.empty() ? "<from cloud>" : frame_id_.c_str(),
                    health_topic_.empty() ? "<disabled>" : health_topic_.c_str());
        ST_LOG_INFO("marker: show_text=%d show_velocity=%d vel_arrow=%d "
                    "vel_min=%.2f arrow_scale=%.2f arrow_min_len=%.2f "
                    "arrow_shaft=%.3f arrow_head=%.3f color_by_class=%d "
                    "text_scale=%.2f z_offset=%.2f",
                    marker_show_text_ ? 1 : 0,
                    marker_show_velocity_ ? 1 : 0,
                    marker_show_velocity_arrow_ ? 1 : 0,
                    marker_velocity_min_mps_, marker_velocity_arrow_scale_,
                    marker_velocity_arrow_min_length_,
                    marker_velocity_arrow_shaft_, marker_velocity_arrow_head_,
                    marker_color_by_class_ ? 1 : 0,
                    marker_text_scale_, marker_text_z_offset_);
        ST_LOG_INFO("log quota: max_total_mb=%d rotate_single_mb=%d file=%s",
                    log_max_total_mb_, log_rotate_single_mb_,
                    log_file_path_.empty() ? "<stdout>" : log_file_path_.c_str());
        ST_LOG_INFO("det_dump: enabled=%d",
                    det_dumper_.enabled() ? 1 : 0);
    }

    void logMaintainCb(const ros::TimerEvent&) {
        if (log_file_path_.empty()) return;

        if (log_rotate_single_mb_ > 0) {
            rotateStLogFile(
                log_file_path_,
                static_cast<int64_t>(log_rotate_single_mb_) * 1024 * 1024);
        }
        if (log_max_total_mb_ > 0) {
            StorageQuota quota;
            quota.max_total_bytes =
                static_cast<int64_t>(log_max_total_mb_) * 1024 * 1024;
            pruneDirectoryFiles(fs::path(log_file_path_).parent_path(),
                                "lidar_det_node_st", quota);
        }
    }

    void saveAbnormalCloudIfNeeded(const sensor_msgs::PointCloud2ConstPtr& msg,
                                   const pcl::PointCloud<pcl::PointXYZI>& cloud,
                                   const FrameMetrics& m) {
        if (!detector_) {
            abnormal_saver_.trySave("detector_down", msg, cloud, frame_count_, m);
        }
        if (m.pts_in == 0) {
            abnormal_saver_.trySave("empty_cloud", msg, cloud, frame_count_, m);
            return;
        }
        if (!detector_) return;
        if (m.raw_det == 0) {
            abnormal_saver_.trySave("zero_raw_det", msg, cloud, frame_count_, m);
        }
        if (m.raw_det > 0 && m.kept == 0) {
            abnormal_saver_.trySave("all_filtered", msg, cloud, frame_count_, m);
        }
    }

    void loadScoreThresholds(ros::NodeHandle& pnh) {
        pnh.param("score_threshold", score_threshold_, 0.1);
        score_threshold_pedestrian_ = score_threshold_;
        score_threshold_car_        = score_threshold_;
        score_threshold_cyclist_    = score_threshold_;
        pnh.param("score_threshold_pedestrian",
                  score_threshold_pedestrian_, score_threshold_pedestrian_);
        pnh.param("score_threshold_car",
                  score_threshold_car_, score_threshold_car_);
        pnh.param("score_threshold_cyclist",
                  score_threshold_cyclist_, score_threshold_cyclist_);
    }

    float outputScoreThreshold(uint8_t out_label) const {
        switch (out_label) {
            case comm_msg::label::PEDESTRIAN:
                return static_cast<float>(score_threshold_pedestrian_);
            case comm_msg::label::CAR:
                return static_cast<float>(score_threshold_car_);
            case comm_msg::label::CYCLIST:
                return static_cast<float>(score_threshold_cyclist_);
            default:
                return static_cast<float>(score_threshold_);
        }
    }

    void loadClassMap(ros::NodeHandle& pnh) {
        class_map_ = classMapForProfile(class_map_profile_);
        ST_LOG_INFO("class_map_profile=%s (%zu native ids)",
                    class_map_profile_.c_str(), class_map_.size());

        std::map<std::string, int> override_map;
        if (pnh.getParam("class_map", override_map)) {
            for (const auto& kv : override_map) {
                try {
                    class_map_[std::stoi(kv.first)] = kv.second;
                    ST_LOG_INFO("class_map override: native %d -> comm label %d",
                                std::stoi(kv.first), kv.second);
                } catch (const std::exception&) {
                    ST_LOG_WARN("Ignoring non-integer class_map key '%s'.", kv.first.c_str());
                }
            }
            ST_LOG_INFO("class_map overridden with %zu entries.", override_map.size());
        }

        ST_LOG_INFO("filter_unmapped_labels=%d collapse_to_output_labels=%d "
                    "(publish CAR/CYCLIST/PEDESTRIAN only)",
                    filter_unmapped_labels_ ? 1 : 0,
                    collapse_to_output_labels_ ? 1 : 0);
    }

    bool shouldPublishLabel(uint8_t comm_mapped, uint8_t& out_label) const {
        if (comm_mapped == comm_msg::label::UNKNOWN) return false;
        if (!collapse_to_output_labels_) {
            out_label = comm_mapped;
            return true;
        }
        return collapseToOutputLabel(comm_mapped, out_label);
    }

    int mapLabel(int native) const {
        auto it = class_map_.find(native);
        return it != class_map_.end() ? it->second : 0;
    }

    bool isLabelMapped(int native) const {
        return class_map_.find(native) != class_map_.end();
    }

    void stallCheckCb(const ros::TimerEvent&) {
        if (!first_cloud_received_) return;
        const double gap = (ros::Time::now() - last_cloud_recv_).toSec();
        if (gap > input_timeout_sec_) {
            ST_LOG_WARN("input stall: no cloud for %.2fs (topic=%s)",
                        gap, input_topic_.c_str());
        }
    }

    void cloudCb(const sensor_msgs::PointCloud2ConstPtr& msg) {
        ++frame_count_;
        last_cloud_recv_ = ros::Time::now();

        pcl::PointCloud<pcl::PointXYZI> cloud;
        const bool has_intensity = fromRosMsgToCloud(*msg, cloud);
        const size_t n = cloud.size();

        const std::string out_frame = frame_id_.empty() ? msg->header.frame_id : frame_id_;

        if (!first_cloud_received_) {
            first_cloud_received_ = true;
            ST_LOG_INFO("first cloud: frame=%s stamp=%.3f pts=%zu cloud_intensity=%s "
                        "use_intensity=%d (p[3]=%s)",
                        msg->header.frame_id.c_str(), msg->header.stamp.toSec(), n,
                        has_intensity ? "yes" : "no",
                        use_intensity_ ? 1 : 0,
                        use_intensity_ ? "cloud.intensity" : "0");
        }

        comm_msg::boxArray arr;
        arr.header = msg->header;
        if (!frame_id_.empty()) arr.header.frame_id = frame_id_;

        FrameMetrics m;
        m.pts_in = n;

        const bool dump_frame = det_dumper_.enabled();
        std::vector<DetDumpRecord> dump_records;
        std::vector<MarkerVisItem> marker_items;

        if (!detector_) {
            ST_LOG_ERR_SAMPLE_IF_TRUE(!detector_, 100,
                "cloudCb: detector unavailable, publishing empty boxArray");
        } else if (n == 0) {
            ST_LOG_SAMPLE(50, "WARN", "empty point cloud on %s", input_topic_.c_str());
        } else {
            buf_.resize(n * feature_num_, 0.f);
            for (size_t i = 0; i < n; ++i) {
                float* p = buf_.data() + i * feature_num_;
                p[0] = cloud[i].x;
                p[1] = cloud[i].y;
                p[2] = cloud[i].z - static_cast<float>(lidar_height_);
                if (feature_num_ > 3) {
                    p[3] = use_intensity_ ? cloud[i].intensity : 0.f;
                }
                if (feature_num_ > 4) p[4] = 0.f;
            }

            const auto t0 = std::chrono::steady_clock::now();
            const auto& dets =
                detector_->infer(buf_.data(), static_cast<int>(n), feature_num_);
            m.infer_ms = std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - t0).count();
            m.raw_det = dets.size();

            float best_kept_score = -1.f;
            if (dump_frame) dump_records.reserve(dets.size());

            for (size_t di = 0; di < dets.size(); ++di) {
                const auto& d = dets[di];
                if (d.score > m.best_score) {
                    m.best_score      = d.score;
                    m.best_native_lbl = d.label;
                }

                const char* filter_reason = "published";
                bool published = false;
                uint8_t out_label = 0;

                if (d.score < static_cast<float>(score_threshold_)) {
                    filter_reason = "low_score";
                    if (dump_frame) {
                        dump_records.push_back(
                            {di, d, filter_reason, false, 0});
                    }
                    continue;
                }

                if (filter_unmapped_labels_ &&
                    (d.label < 0 || !isLabelMapped(d.label))) {
                    ++m.skipped_unmapped;
                    ST_LOG_SAMPLE(50, "WARN",
                        "skip unmapped native label %d score=%.3f",
                        d.label, d.score);
                    filter_reason = "unmapped";
                    if (dump_frame) {
                        dump_records.push_back(
                            {di, d, filter_reason, false, 0});
                    }
                    continue;
                }

                const uint8_t comm_mapped =
                    static_cast<uint8_t>(mapLabel(d.label));
                out_label = comm_mapped;
                if (!shouldPublishLabel(comm_mapped, out_label)) {
                    ++m.skipped_non_output;
                    filter_reason = "filtered";
                    if (dump_frame) {
                        dump_records.push_back(
                            {di, d, filter_reason, false, comm_mapped});
                    }
                    continue;
                }

                const float class_thr = outputScoreThreshold(out_label);
                if (d.score < class_thr) {
                    ++m.skipped_class_score;
                    filter_reason = "low_score_class";
                    if (dump_frame) {
                        dump_records.push_back(
                            {di, d, filter_reason, false, out_label});
                    }
                    continue;
                }

                comm_msg::box b;
                b.label.value = out_label;
                b.x = d.x;  b.y = d.y;  b.z = d.z;
                b.width = d.w;  b.length = d.l;  b.height = d.h;
                b.yaw = d.yaw;
                b.vel_x = d.vx;  b.vel_y = d.vy;  b.vel_z = 0.f;
                //b.score = d.score;
                arr.boxes.push_back(b);
                marker_items.push_back({b, d.score, d.label});
                bumpMappedLabelCount(out_label, m);
                published = true;

                if (dump_frame) {
                    dump_records.push_back(
                        {di, d, filter_reason, published, out_label});
                }

                if (d.score > best_kept_score) {
                    best_kept_score       = d.score;
                    m.top_kept_score      = d.score;
                    m.top_kept_native_lbl = d.label;
                    m.top_kept_mapped_lbl = out_label;
                    m.top_x = d.x;
                    m.top_y = d.y;
                }
            }
            m.kept = arr.boxes.size();

            if (m.raw_det > 0 && m.kept == 0) {
                ST_LOG_SAMPLE(30, "WARN",
                    "all %zu raw detections filtered by score_threshold=%.3f "
                    "(best_score=%.3f)",
                    m.raw_det, score_threshold_, m.best_score);
            }
            if (n > 0 && m.raw_det == 0) {
                ST_LOG_SAMPLE(30, "WARN",
                    "infer returned 0 detections (pts_in=%zu, check range filter/backend)",
                    n);
            }
        }

        if (dump_frame) {
            det_dumper_.logFrame(frame_count_, msg, m, out_frame, dump_records);
        }

        logFrameMetrics(m, out_frame);
        saveAbnormalCloudIfNeeded(msg, cloud, m);

        pub_box_.publish(arr);
        publishMarkers(marker_items, out_frame, msg->header.stamp);
        publishHealth(m);
    }

    void logFrameMetrics(const FrameMetrics& m, const std::string& out_frame) {
        if (!debug_stats_) return;

        // P0 + P1: periodic frame / infer stats
        ST_LOG_INFO_SAMPLE(stats_interval_,
            "frame #%llu: pts=%zu raw=%zu kept=%zu skip_unmap=%zu skip_other=%zu "
            "skip_cls_score=%zu infer_ms=%.1f best_raw=%.3f native=%s(%d) frame=%s",
            static_cast<unsigned long long>(frame_count_),
            m.pts_in, m.raw_det, m.kept, m.skipped_unmapped, m.skipped_non_output,
            m.skipped_class_score,
            m.infer_ms, m.best_score,
            nativeClassName(class_map_profile_, m.best_native_lbl), m.best_native_lbl,
            out_frame.c_str());

        // P2: mapped comm_msg label counts (CAR/TRUCK/BUS/...)
        if (frame_count_ % static_cast<uint64_t>(summary_interval_) == 0) {
            const std::string counts = formatMappedLabelCounts(m.mapped_counts);
            if (m.top_kept_score >= 0.f) {
                ST_LOG_INFO(
                    "det summary #%llu: kept=%zu {%s} "
                    "top_kept=%s(native=%s/%d) score=%.3f pos=(%.1f,%.1f)",
                    static_cast<unsigned long long>(frame_count_),
                    m.kept, counts.c_str(),
                    commLabelName(m.top_kept_mapped_lbl),
                    nativeClassName(class_map_profile_, m.top_kept_native_lbl),
                    m.top_kept_native_lbl,
                    m.top_kept_score, m.top_x, m.top_y);
            } else {
                ST_LOG_INFO(
                    "det summary #%llu: kept=%zu {%s} top_kept=(none)",
                    static_cast<unsigned long long>(frame_count_),
                    m.kept, counts.c_str());
            }
        }
    }

    // P3: lightweight health topic for remote rostopic echo / monitoring.
    void publishHealth(const FrameMetrics& m) {
        if (!pub_health_) return;

        const double gap = first_cloud_received_
            ? (ros::Time::now() - last_cloud_recv_).toSec()
            : -1.0;

        const auto cnt = [&m](uint8_t lbl) -> float {
            const auto it = m.mapped_counts.find(lbl);
            return (it != m.mapped_counts.end())
                ? static_cast<float>(it->second) : 0.f;
        };

        std_msgs::Float32MultiArray ha;
        ha.data = {
            static_cast<float>(frame_count_),
            static_cast<float>(m.infer_ms),
            static_cast<float>(m.pts_in),
            static_cast<float>(m.raw_det),
            static_cast<float>(m.kept),
            detector_ ? 1.f : 0.f,
            static_cast<float>(gap),
            m.best_score,
            static_cast<float>(m.best_native_lbl),
            cnt(comm_msg::label::CAR),
            cnt(comm_msg::label::PEDESTRIAN),
            cnt(comm_msg::label::CYCLIST),
        };
        pub_health_.publish(ha);
    }

    void publishMarkers(const std::vector<MarkerVisItem>& items,
                        const std::string& frame, const ros::Time& stamp) {
        if (pub_marker_.getNumSubscribers() == 0) return;
        visualization_msgs::MarkerArray ma;

        visualization_msgs::Marker clear;
        clear.header.frame_id = frame;
        clear.header.stamp = stamp;
        clear.action = visualization_msgs::Marker::DELETEALL;
        ma.markers.push_back(clear);

        int id = 0;
        for (const auto& item : items) {
            const auto& b = item.box;
            float cr = 0.f, cg = 0.9f, cb = 0.93f, ca = 0.6f;
            if (marker_color_by_class_) {
                classMarkerColor(b.label.value, item.score, cr, cg, cb, ca);
            }

            visualization_msgs::Marker cube;
            cube.header.frame_id = frame;
            cube.header.stamp = stamp;
            cube.ns = "det";
            cube.id = id++;
            cube.type = visualization_msgs::Marker::CUBE;
            cube.action = visualization_msgs::Marker::ADD;
            cube.pose.position.x = b.x;
            cube.pose.position.y = b.y;
            cube.pose.position.z = b.z;
            const double half = 0.5 * b.yaw;
            cube.pose.orientation.z = std::sin(half);
            cube.pose.orientation.w = std::cos(half);
            cube.scale.x = std::max(0.05f, b.width);
            cube.scale.y = std::max(0.05f, b.length);
            cube.scale.z = std::max(0.05f, b.height);
            cube.color.r = cr;
            cube.color.g = cg;
            cube.color.b = cb;
            cube.color.a = ca;
            cube.lifetime = ros::Duration(0.2);
            ma.markers.push_back(cube);

            if (marker_show_text_) {
                visualization_msgs::Marker text;
                text.header.frame_id = frame;
                text.header.stamp = stamp;
                text.ns = "det_label";
                text.id = id++;
                text.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
                text.action = visualization_msgs::Marker::ADD;
                text.pose.position.x = b.x;
                text.pose.position.y = b.y;
                text.pose.position.z =
                    b.z + 0.5f * std::max(0.05f, b.height) + marker_text_z_offset_;
                text.scale.z = marker_text_scale_;
                text.color.r = 1.f;
                text.color.g = 1.f;
                text.color.b = 1.f;
                text.color.a = 0.95f;
                text.text = markerLabelText(
                    b.label.value, item.score, item.native_id,
                    b.vel_x, b.vel_y,
                    marker_show_velocity_, marker_velocity_min_mps_,
                    class_map_profile_);
                text.lifetime = ros::Duration(0.2);
                ma.markers.push_back(text);
            }

            if (marker_show_velocity_arrow_) {
                const float speed = std::hypot(b.vel_x, b.vel_y);
                if (speed >= static_cast<float>(marker_velocity_min_mps_)) {
                    const double yaw_vel = std::atan2(b.vel_y, b.vel_x);
                    const float arrow_len = std::max(
                        static_cast<float>(marker_velocity_arrow_min_length_),
                        speed * static_cast<float>(marker_velocity_arrow_scale_));

                    visualization_msgs::Marker arrow;
                    arrow.header.frame_id = frame;
                    arrow.header.stamp = stamp;
                    arrow.ns = "det_vel";
                    arrow.id = id++;
                    arrow.type = visualization_msgs::Marker::ARROW;
                    arrow.action = visualization_msgs::Marker::ADD;
                    // Place arrow base near box center, tip extends along velocity.
                    arrow.pose.position.x =
                        b.x + 0.5 * arrow_len * std::cos(yaw_vel);
                    arrow.pose.position.y =
                        b.y + 0.5 * arrow_len * std::sin(yaw_vel);
                    arrow.pose.position.z = b.z;
                    const double half_yaw = 0.5 * yaw_vel;
                    arrow.pose.orientation.z = std::sin(half_yaw);
                    arrow.pose.orientation.w = std::cos(half_yaw);
                    arrow.scale.x = marker_velocity_arrow_shaft_;
                    arrow.scale.y = marker_velocity_arrow_head_;
                    arrow.scale.z = arrow_len;
                    arrow.color.r = cr;
                    arrow.color.g = cg;
                    arrow.color.b = cb;
                    arrow.color.a = 0.95f;
                    arrow.lifetime = ros::Duration(0.2);
                    ma.markers.push_back(arrow);
                }
            }
        }
        pub_marker_.publish(ma);
    }

    ros::NodeHandle nh_;
    ros::Subscriber sub_cloud_;
    ros::Publisher  pub_box_;
    ros::Publisher  pub_marker_;
    ros::Publisher  pub_health_;
    ros::Timer      stall_timer_;
    ros::Timer      log_maint_timer_;

    AbnormalCloudSaver abnormal_saver_;
    DetDumpLogger      det_dumper_;
    std::string log_file_path_;
    int         log_max_total_mb_    = 100;
    int         log_rotate_single_mb_ = 20;

    std::unique_ptr<lidar_det::ILidarDetector> detector_;
    std::unordered_map<int, int>               class_map_;
    std::string                                class_map_profile_ = "nuscenes_10";
    bool                                       filter_unmapped_labels_ = true;
    bool                                       collapse_to_output_labels_ = true;
    std::vector<float>                          buf_;

    std::string input_topic_, output_topic_, marker_topic_, frame_id_, health_topic_;
    double      lidar_height_        = 1.68;
    double      score_threshold_             = 0.1;
    double      score_threshold_pedestrian_  = 0.1;
    double      score_threshold_car_         = 0.1;
    double      score_threshold_cyclist_     = 0.1;
    double      input_timeout_sec_   = 1.0;
    int         feature_num_         = 5;
    int         stats_interval_      = 30;
    int         summary_interval_    = 60;
    bool        marker_show_text_            = true;
    bool        marker_show_velocity_        = true;
    bool        marker_show_velocity_arrow_  = true;
    bool        marker_color_by_class_       = true;
    double      marker_text_scale_           = 0.45;
    double      marker_text_z_offset_        = 0.6;
    double      marker_velocity_min_mps_       = 0.1;
    double      marker_velocity_arrow_scale_   = 3.0;
    double      marker_velocity_arrow_min_length_ = 1.5;
    double      marker_velocity_arrow_shaft_   = 0.05;
    double      marker_velocity_arrow_head_    = 0.10;
    bool        use_intensity_               = false;
    bool        verbose_             = false;
    bool        debug_stats_         = true;
    bool        first_cloud_received_ = false;
    uint64_t    frame_count_         = 0;
    ros::Time   last_cloud_recv_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lidar_det_node");
    ros::NodeHandle nh;
    ros::NodeHandle pnh("~");

    std::string log_file;
    pnh.param<std::string>("log_file", log_file, std::string());
    log_file = expandUserPath(log_file);

    int log_max_total_mb = 100;
    pnh.param("log_max_total_mb", log_max_total_mb, 100);
    if (!log_file.empty() && log_max_total_mb > 0) {
        StorageQuota quota;
        quota.max_total_bytes = static_cast<int64_t>(log_max_total_mb) * 1024 * 1024;
        pruneDirectoryFiles(fs::path(log_file).parent_path(),
                            "lidar_det_node_st", quota);
    }

    if (!log_file.empty()) {
        std::error_code ec;
        fs::create_directories(fs::path(log_file).parent_path(), ec);
        (void)InitXXXLog(log_file);
    }
    ST_LOG_INFO("lidar_det_node start. log_file=%s.",
                log_file.empty() ? "<stdout>" : log_file.c_str());

    LidarDetAdapter adapter(nh, pnh, log_file);
    ros::spin();
    return 0;
}
