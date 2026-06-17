#ifndef __PCPT_COMM_SCAN_H__
#define __PCPT_COMM_SCAN_H__

#include <iostream>
#include <string>
#include <vector>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/comm_car.h"
#include "comm/comm_topic_name.h"
#include "comm_ros_extend/comm_ros_base_extend.h"
#include "pcpt_comm/pcpt_comm_ros.h"

namespace PCPT
{

/**
 * @brief scan方向数据结构
 *
 */
struct ScanBearingDataSt
{
    std::vector<unsigned int> indices;
    std::vector<double> bearings;
    std::vector<double> coss;
    std::vector<double> sins;
};

/**
 * @brief scan报文信息结构
 *
 */
struct ScanMsgInfoSt
{
    unsigned int seq;
    ros::Time stamp;
    std::string frame_id;

    float angle_min;
    float angle_max;
    float angle_increment;
    float time_increment;
    float scan_time;
    float range_min;
    float range_max;
};

/**
 * @brief scan至点云转换器
 *
 */
class SCAN_2_CLOUD_CONVERTER_C
{
    private:
        unsigned int points_num_ = 0;    // 波束端点数量
        ScanBearingDataSt bearing_data_; // scan方向数据

    public:
        // 构造函数
        SCAN_2_CLOUD_CONVERTER_C(void){}

        // 变换scan至点云结构
        void Scan2Cloud(const sensor_msgs::LaserScan::ConstPtr &p_scan,
                        PointCloudXYZIRCDT::Ptr &ptrCloud)
        {
            // 缓存scan的方向数据
            CacheBearingData(p_scan);

            // 重新分配点云空间
            ptrCloud->clear();
            pcl_conversions::toPCL(p_scan->header, ptrCloud->header);
            ptrCloud->resize(points_num_);

            // 校验scan的强度值是否有效
            bool inte_valid = false;
            if (p_scan->intensities.size() == points_num_)
            {
                inte_valid = true;
            }

            PointXYZIRCDT pointT;
            pointT.z = 0;
            pointT.ring = uint16_t(0);
            pointT.timestamp = 0;

            // 依次变换所有波束端点
            unsigned int valid_beams = 0;
            for (unsigned int idx = 0; idx < points_num_; ++idx)
            {
                const float &beam_range = p_scan->ranges[idx];
                const float &range_min = p_scan->range_min;
                const float &range_max = p_scan->range_max;

                // 忽略无效端点
                if (!std::isfinite(beam_range))
                {
                    continue;
                }
                if ((beam_range < range_min) || (beam_range > range_max))
                {
                    continue;
                }

                pointT.x = bearing_data_.coss[idx] * beam_range;
                pointT.y = bearing_data_.sins[idx] * beam_range;
                pointT.col = idx;
                pointT.dis = beam_range;
                if (inte_valid)
                {
                    pointT.intensity = uint8_t(p_scan->intensities[idx]);
                }
                ptrCloud->at(valid_beams) = pointT;
                ++valid_beams;
            }
            ptrCloud->resize(valid_beams);
        }

    private:
        // 缓存scan的方向数据
        void CacheBearingData(const sensor_msgs::LaserScan::ConstPtr &p_scan)
        {
            // 监控激光波束数量
            bool has_change = (points_num_ > 0) ? false : true;
            has_change |= MonitorBeamsNum(p_scan, points_num_);
            if (has_change == false)
            {
                return;
            }

            // 分配缓存空间
            bearing_data_.indices.resize(points_num_);
            bearing_data_.bearings.resize(points_num_);
            bearing_data_.coss.resize(points_num_);
            bearing_data_.sins.resize(points_num_);

            // 更新方向缓存信息
            double beam_angle;
            for (unsigned int idx = 0; idx < points_num_; ++idx)
            {
                beam_angle = p_scan->angle_min + idx * p_scan->angle_increment;

                bearing_data_.indices[idx] = idx;
                bearing_data_.bearings[idx] = beam_angle;
                bearing_data_.coss[idx] = cos(beam_angle);
                bearing_data_.sins[idx] = sin(beam_angle);
            }
            ST_LOG_INFO("雷达frame<%s>: 方向信息缓存ok",
                        p_scan->header.frame_id.data());
        }

        // 监控激光波束数量
        bool MonitorBeamsNum(const sensor_msgs::LaserScan::ConstPtr &p_scan,
                             unsigned int &origin_points_num)
        {
            unsigned int ranges_num = p_scan->ranges.size();
            if (ranges_num != origin_points_num)
            {
                ST_LOG_INFO("雷达frame<%s>: 波束数量有变更:[%d]->[%d]",
                            p_scan->header.frame_id.data(), origin_points_num, ranges_num);
                origin_points_num = ranges_num;
                return true;
            }
            return false;
        }
};

}

#endif
