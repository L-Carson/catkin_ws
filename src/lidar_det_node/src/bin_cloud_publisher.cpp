// Replays raw .bin LiDAR frames from disk as sensor_msgs/PointCloud2, so the
// full chain (this publisher -> lidar_det_node -> comm_msg/boxArray + markers)
// can be exercised without a real LiDAR.
//
// .bin layout: N x feature_num float32, point i = [x, y, z, intensity, time, ...].
// Only x,y,z,intensity are put into the published PointXYZI cloud.
//
// Params (private ~):
//   bin_path     : file (*.bin) or directory of *.bin           [required]
//   topic        : output PointCloud2 topic   (default /MainLidar/car_filtered_points)
//   frame_id     : cloud header frame          (default base_link)
//   feature_num  : floats per point            (default 5)
//   rate_hz      : publish rate                 (default 10)
//   loop         : repeat directory forever     (default true)

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <dirent.h>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

namespace {

bool hasSuffix(const std::string& s, const std::string& suf) {
    return s.size() >= suf.size() &&
           s.compare(s.size() - suf.size(), suf.size(), suf) == 0;
}

std::vector<std::string> listBins(const std::string& path) {
    std::vector<std::string> out;
    if (hasSuffix(path, ".bin")) { out.push_back(path); return out; }
    DIR* dir = opendir(path.c_str());
    if (!dir) { ROS_ERROR("Cannot open bin_path: %s", path.c_str()); return out; }
    while (auto* ent = readdir(dir)) {
        std::string name = ent->d_name;
        if (hasSuffix(name, ".bin")) {
            out.push_back(path.back() == '/' ? path + name : path + "/" + name);
        }
    }
    closedir(dir);
    std::sort(out.begin(), out.end());
    return out;
}

bool loadBin(const std::string& file, std::vector<float>& data) {
    std::ifstream fin(file, std::ios::binary | std::ios::ate);
    if (!fin) { ROS_WARN("cannot open %s", file.c_str()); return false; }
    const std::streamsize bytes = fin.tellg();
    fin.seekg(0, std::ios::beg);
    data.resize(static_cast<size_t>(bytes) / sizeof(float));
    return static_cast<bool>(fin.read(reinterpret_cast<char*>(data.data()), bytes));
}

}  // namespace

int main(int argc, char** argv) {
    ros::init(argc, argv, "bin_cloud_publisher");
    ros::NodeHandle nh;
    ros::NodeHandle pnh("~");

    std::string bin_path, topic, frame_id;
    int feature_num = 5;
    double rate_hz = 10.0;
    bool loop = true;
    pnh.param<std::string>("bin_path", bin_path, std::string());
    pnh.param<std::string>("topic", topic, "/MainLidar/car_filtered_points");
    pnh.param<std::string>("frame_id", frame_id, "base_link");
    pnh.param("feature_num", feature_num, 5);
    pnh.param("rate_hz", rate_hz, 10.0);
    pnh.param("loop", loop, true);

    if (bin_path.empty()) {
        ROS_FATAL("Set _bin_path:=<file.bin|dir>");
        return 1;
    }
    const std::vector<std::string> files = listBins(bin_path);
    if (files.empty()) { ROS_FATAL("No .bin under %s", bin_path.c_str()); return 1; }
    ROS_INFO("bin_cloud_publisher: %zu frame(s) -> %s @ %.1f Hz (loop=%d)",
             files.size(), topic.c_str(), rate_hz, static_cast<int>(loop));

    ros::Publisher pub = nh.advertise<sensor_msgs::PointCloud2>(topic, 1, true);
    ros::Rate rate(rate_hz);
    std::vector<float> raw;

    do {
        for (const auto& f : files) {
            if (!ros::ok()) break;
            if (!loadBin(f, raw)) continue;
            const size_t n = raw.size() / static_cast<size_t>(feature_num);

            pcl::PointCloud<pcl::PointXYZI> cloud;
            cloud.reserve(n);
            for (size_t i = 0; i < n; ++i) {
                const float* p = raw.data() + i * feature_num;
                pcl::PointXYZI pt;
                pt.x = p[0]; pt.y = p[1]; pt.z = p[2];
                pt.intensity = feature_num > 3 ? p[3] : 0.f;
                cloud.push_back(pt);
            }

            sensor_msgs::PointCloud2 msg;
            pcl::toROSMsg(cloud, msg);
            msg.header.frame_id = frame_id;
            msg.header.stamp = ros::Time::now();
            pub.publish(msg);
            ROS_INFO_THROTTLE(2.0, "published %zu pts from %s", n, f.c_str());

            ros::spinOnce();
            rate.sleep();
        }
    } while (loop && ros::ok());

    return 0;
}
