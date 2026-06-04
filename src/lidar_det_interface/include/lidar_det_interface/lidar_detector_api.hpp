// Backend-agnostic LiDAR 3D detector interface.
//
// This header is the ONLY contract shared between:
//   - the ROS adapter node (catkin, C++17), and
//   - the model backend shared library (e.g. libtransfusion_backend.so).
//
// It intentionally exposes NO ROS, TensorRT, CUDA or libspconv types, so the
// same header compiles unchanged on x86_64 (TRT10) and Jetson Orin (aarch64),
// and the backend .so can be swapped without recompiling the node.
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace lidar_det {

// One detection in the LiDAR frame. Layout is backend-neutral; a backend fills
// whatever it produces (velocity is 0 when the model has no velocity head).
struct DetectionBox {
    float x = 0.f, y = 0.f, z = 0.f;   // center (m)
    float w = 0.f, l = 0.f, h = 0.f;   // size (m)
    float yaw = 0.f;                   // heading (rad)
    float vx = 0.f, vy = 0.f;          // velocity (m/s)
    float score = 0.f;
    int   label = -1;                  // backend-native class id (e.g. nuScenes 0..9)
};

// Runtime configuration handed to a backend. Unknown keys for a given backend
// are simply ignored. Paths are absolute (resolved by the node from YAML).
struct DetectorConfig {
    std::string backend;        // "transfusion" / "pointpillars" / ...
    std::string model_dir;      // base dir for relative model assets (optional)

    // Generic model assets (a backend uses the subset it needs).
    std::string scn_onnx;       // TransFusion 3D sparse backbone ONNX
    std::string bev_weights;    // TransFusion BEV backbone weights .bin
    std::string bev_engine;     // TensorRT engine cache (per-platform!)
    std::string head_engine;    // TensorRT head engine (per-platform!)

    int   feature_num     = 5;       // input point stride (x,y,z,intensity,time)
    float score_threshold = 0.1f;    // backend may use this to pre-filter
    bool  verbose         = false;

    // Free-form extra params for backend-specific knobs (string-typed).
    std::unordered_map<std::string, std::string> extra;
};

// Pure-virtual detector. Lifecycle: construct -> prepare() once -> infer() N.
class ILidarDetector {
public:
    virtual ~ILidarDetector() = default;

    // Build/load engines. MUST run on the target platform (engines are not
    // portable across arch / TensorRT version). Returns 0 on success.
    virtual int prepare() = 0;

    // Run one frame. `points` is a host-side contiguous buffer of
    // num_points * feature_num floats. The returned vector reference is owned
    // by the detector and valid until the next infer() call.
    virtual const std::vector<DetectionBox>&
    infer(const float* points, int num_points, int feature_num) = 0;
};

}  // namespace lidar_det

// Factory exported by every backend .so. The node links exactly one backend
// at build time and calls this. Returns nullptr if cfg.backend is unsupported.
// (Plain C++ linkage on purpose: node and backend are built with the same
// toolchain per platform, so we can pass std::unique_ptr across the boundary.)
namespace lidar_det {
std::unique_ptr<ILidarDetector> CreateLidarDetector(const DetectorConfig& cfg);
}  // namespace lidar_det
