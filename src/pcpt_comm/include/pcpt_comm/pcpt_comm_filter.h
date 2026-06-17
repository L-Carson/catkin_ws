#ifndef __PCPT_COMM_FILTER_H__
#define __PCPT_COMM_FILTER_H__

#include "comm/comm_base.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_format.h"

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/crop_box.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/search/kdtree.h>
#include <pcl/segmentation/conditional_euclidean_clustering.h>
#include <pcl/segmentation/extract_clusters.h>

#ifdef PCL_NO_PRECOMPILE
#undef PCL_NO_PRECOMPILE
#endif

//屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <pcl/segmentation/sac_segmentation.h>
#pragma GCC diagnostic pop

using namespace std;

/**************************************************************************************
功能描述: 体素化滤波
修改记录:
**************************************************************************************/
template<class PointT>
class VOXELIZATION_FILTER_C
{
public:
    typedef typename pcl::PointCloud<PointT> PointCloud;
    typedef typename pcl::PointCloud<PointT>::Ptr PointCloudPtr;

public:
    VOXELIZATION_FILTER_C() {}

    ~VOXELIZATION_FILTER_C()
    {
        DelPtr(pobjPcptVoxel_);
        DelArrayPtr(pVoxelArray_);
    }

    /* 点云体素化 */
    int Voxelization(const CUBOID_STRU &stCuboid, const float voxelSize, PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut)
    {
        /* 构造体素空间映射器 */
        if (!pobjPcptVoxel_ || !(stCuboid_ == stCuboid)) {
            DelPtr(pobjPcptVoxel_);
            pobjPcptVoxel_ = new PCPT_VOXEL_C(stCuboid, voxelSize);
            if (!pobjPcptVoxel_ || !pobjPcptVoxel_->IsValid()) {
                ST_LOG_ERR("Create PCPT_VOXEL_C Object Fail.");
                return -1;
            }
            stCuboid_ = stCuboid;

            if (pVoxelArray_) delete[] pVoxelArray_;
            pVoxelArray_ = new uint16_t[pobjPcptVoxel_->GetVoxelNum()]();   //默认初始化为0
        } else {
            memset(pVoxelArray_, 0, pobjPcptVoxel_->GetVoxelNum() * sizeof(uint16_t));
        }

        /* 分离体素空间内外点云 */
        pCloudInSide_->clear();
        pCloudOutSide_->clear();
        pCloudInSide_->header = pCloudIn->header;
        pCloudOutSide_->header = pCloudIn->header;
        for (const auto &point : pCloudIn->points) {
            if (stCuboid.IsInCubiod(point.x, point.y, point.z))
                pCloudInSide_->push_back(point);
            else
                pCloudOutSide_->push_back(point);
        }

        /* 清除旧体素空间中的数据 */
        for (auto &index : vIndex_)
            vvPointIndex_[index].clear();
        if (int(vvPointIndex_.size()) != pobjPcptVoxel_->GetVoxelNum())
            vvPointIndex_.resize(pobjPcptVoxel_->GetVoxelNum());
        vIndex_.clear();

        /* 体素空间内点云体素化 */
        for (uint32_t i = 0; i < uint32_t(pCloudInSide_->size()); ++i) {
            const auto &point = pCloudInSide_->points[i];
            int index = pobjPcptVoxel_->GetVoxelIndex(point.x, point.y, point.z);
            if (index == -1) continue;
            if (!pVoxelArray_[index]) vIndex_.push_back(index);   //栅格索引不重复记录
            pVoxelArray_[index]++;
            vvPointIndex_[index].push_back(i);
        }

        /* 体素空间外的点云直接输出 */
        pCloudOut = pCloudOutSide_;
        return 0;
    }

    /* 半径滤波 */
    void RadiusOutlierFilter(const CUBOID_STRU &stCuboid, const float voxelSize,
                             const float searchRadius, const int minNeighbors,
                             PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut,
                             PointCloudPtr pCloudFilter = nullptr)
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (!pCloudIn->size()) return;

        PointCloudPtr pCloudOutTmp;
        if (Voxelization(stCuboid, voxelSize, pCloudIn, pCloudOutTmp)) return;
        int searchSteps = searchRadius / voxelSize + 0.5;

        /* 遍历有点的体素，过滤领域点数较少的体素内的点 */
        pCloudOut_->clear();
        pCloudFilter_->clear();
        for (auto &index : vIndex_) {
            vector<int>* pNeighborIndex;
            if (pobjPcptVoxel_->GetNeighborVoxel(index, searchSteps, pNeighborIndex)) continue;

            int neighborPointsNum = 0;
            for (auto &i : *pNeighborIndex) neighborPointsNum += pVoxelArray_[i];

            if (neighborPointsNum >= minNeighbors) {
                for (auto i : vvPointIndex_[index]) {
                    pCloudOut_->push_back(pCloudInSide_->points[i]);
                }
            } else {
                for (auto i : vvPointIndex_[index]) {
                    pCloudFilter_->push_back(pCloudInSide_->points[i]);
                }
            }
        }

        *pCloudOutTmp += *pCloudOut_;
        pCloudOut = pCloudOutTmp;
        if (pCloudFilter) *pCloudFilter = *pCloudFilter_;
    }

    /* 体素统计滤波，统计体素内点的数量，大于等于设定阈值时认为有效 */
    void VoxelStatisticalFilter(const CUBOID_STRU &stCuboid, const float voxelSize, const int numThresh,
                                PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut)
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (!pCloudIn->size()) return;

        PointCloudPtr pCloudOutTmp;
        if (Voxelization(stCuboid, voxelSize, pCloudIn, pCloudOutTmp)) return;

        pCloudOut_->clear();
        for (auto &index : vIndex_) {
            if (pVoxelArray_[index] < numThresh) continue;
            for (auto i : vvPointIndex_[index])
                pCloudOut_->push_back(pCloudInSide_->points[i]);
        }
        *pCloudOutTmp += *pCloudOut_;
        pCloudOut = pCloudOutTmp;
    }

private:
    PCPT_VOXEL_C*   pobjPcptVoxel_{nullptr};        // 体素空间映射器
    CUBOID_STRU     stCuboid_;                      // 体素空间立方体
    PointCloudPtr   pCloudInSide_{new PointCloud};  // 体素空间内的点云
    PointCloudPtr   pCloudOutSide_{new PointCloud}; // 体素空间外的点云
    PointCloudPtr   pCloudOut_{new PointCloud};     // 体素空间内待输出的点云
    PointCloudPtr   pCloudFilter_{new PointCloud};  // 体素空间内待输出的点云
    vector<int>     vIndex_;                        // 一维数组: 缓存有点的体素索引
    uint16_t*       pVoxelArray_{nullptr};          // 一维数组: 缓存每个体素内点数
    vector<vector<uint32_t>> vvPointIndex_;         // 一维数组: 缓存体素空间内的点在每个体素内的索引
};

/**************************************************************************************
功能描述: 点云滤波
修改记录:
**************************************************************************************/
template<class PointT>
class POINT_CLOUD_FILTER_C
{
public:
    POINT_CLOUD_FILTER_C(){}
    ~POINT_CLOUD_FILTER_C() {}
    typedef typename pcl::PointCloud<PointT>            PointCloud;
    typedef typename pcl::PointCloud<PointT>::Ptr       PointCloudPtr;
    typedef typename pcl::search::KdTree<PointT>        KdTree;
    typedef typename pcl::search::KdTree<PointT>::Ptr   KdTreePtr;

    /* 体素降采样 */
    void VoxelGridFilter(const float length, const float width, const float height,
                         PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut) const
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (pCloudIn->empty()) return;

        pcl::VoxelGrid<PointT> filter;
        filter.setInputCloud(pCloudIn);
        filter.setLeafSize(length, width, height);

        if (pCloudIn != pCloudOut) {
            filter.filter(*pCloudOut);
        } else {
            PointCloudPtr pCloudOutTmp(new PointCloud);
            filter.filter(*pCloudOutTmp);
            pCloudOut = pCloudOutTmp;
        }
    }

    void VoxelGridFilter(const float leafSize, PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut) const
    {
        VoxelGridFilter(leafSize, leafSize, leafSize, pCloudIn, pCloudOut);
    }

    /* 条件滤波 */
    void ConditionFilter(const float minX, const float minY, const float minZ,
                         const float maxX, const float maxY, const float maxZ,
                         PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut) const
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (!pCloudIn->size()) return;

        pcl::CropBox<PointT> clipper;
        clipper.setMin(Eigen::Vector4f(minX, minY, minZ, 1.0));
        clipper.setMax(Eigen::Vector4f(maxX, maxY, maxZ, 1.0));
        clipper.setInputCloud(pCloudIn);
        clipper.setNegative(false); //default value: false

        if (pCloudIn != pCloudOut) {
            clipper.filter(*pCloudOut);
        } else {
            PointCloudPtr pCloudOutTmp(new PointCloud);
            clipper.filter(*pCloudOutTmp);
            pCloudOut = pCloudOutTmp;
        }
    }

    void ConditionFilter(const CUBOID_STRU &stCuboid, PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut) const
    {
        ConditionFilter(stCuboid.minX, stCuboid.minY, stCuboid.minZ, stCuboid.maxX, stCuboid.maxY, stCuboid.maxZ, pCloudIn, pCloudOut);
    }

    /* 半径滤波：基于pcl kd-tree方法 */
    void RadiusOutlierFilter(const float searchRadius, const int minNeighbors, PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut) const
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (!pCloudIn->size()) return;

        pcl::RadiusOutlierRemoval<PointT> filter;
        filter.setInputCloud(pCloudIn);
        filter.setRadiusSearch(searchRadius);
        filter.setMinNeighborsInRadius(minNeighbors);
        filter.setNegative(false);

        if (pCloudIn != pCloudOut) {
            filter.filter(*pCloudOut);
        } else {
            PointCloudPtr pCloudOutTmp(new PointCloud);
            filter.filter(*pCloudOutTmp);
            pCloudOut = pCloudOutTmp;
        }
    }

    void RadiusOutlierFilter(const float searchRadius, const int minNeighbors, const CUBOID_STRU &stCuboid,
                             PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut) const
    {
        ConditionFilter(stCuboid, pCloudIn, pCloudIn);
        RadiusOutlierFilter(searchRadius, minNeighbors, pCloudIn, pCloudOut);
    }

    /* 半径滤波：基于自定义体素空间方法 */
    void RadiusOutlierFilter(const CUBOID_STRU &stCuboid, const float voxelSize,
                             const float searchRadius, const int minNeighbors,
                             PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut,
                             PointCloudPtr pCloudFilter = nullptr)
    {
        objRadiusOutlierFilter_.RadiusOutlierFilter(stCuboid, voxelSize, searchRadius, minNeighbors, pCloudIn, pCloudOut, pCloudFilter);
    }

    /* 体素统计滤波 */
    void VoxelStatisticalFilter(const CUBOID_STRU& stCuboid, const float voxelSize, const int numThresh,
                                PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut)
    {
        objVoxelStatisticalFilter_.VoxelStatisticalFilter(stCuboid, voxelSize, numThresh, pCloudIn, pCloudOut);
    }

    /* 统计滤波 */
    void StatisticalOutlierFilter(const int meanK, const float stddevMulThresh,
                                  PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut)
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (!pCloudIn->size()) return;

        pcl::StatisticalOutlierRemoval<PointT> Statistical;
        Statistical.setInputCloud(pCloudIn);
        Statistical.setMeanK(meanK);                        // K近邻搜索点个数
        Statistical.setStddevMulThresh(stddevMulThresh);    // 标准差倍数
        Statistical.setNegative(false);                     // 保留未滤波点（内点）

        if (pCloudIn != pCloudOut) {
            Statistical.filter(*pCloudOut);                 // 保存滤波结果到pCloudOut
        } else {
            PointCloudPtr pCloudOutTmp(new PointCloud);
            Statistical.filter(*pCloudOutTmp);              // 保存滤波结果到pCloudOut
            pCloudOut = pCloudOutTmp;
        }
    }

    /* 直通滤波 */
    void PassThroughFilter(const float min, const float max, const string axis,
                           PointCloudPtr pCloudIn, PointCloudPtr& pCloudOut)
    {
        if (pCloudIn != pCloudOut) pCloudOut->header = pCloudIn->header;
        if (!pCloudIn->size()) return;

        pcl::PassThrough<PointT> pass;          // 创建滤波器对象
        pass.setInputCloud(pCloudIn);           // 设置待滤波的点云
        pass.setFilterFieldName(axis);          // 设置在axis轴方向上进行滤波
        pass.setFilterLimits(min, max);         // 设置滤波范围
        pass.setFilterLimitsNegative(false);    // 保留

        if (pCloudIn != pCloudOut) {
            pass.filter(*pCloudOut);            // 滤波并存储
        } else {
            PointCloudPtr pCloudOutTmp(new PointCloud);
            pass.filter(*pCloudOutTmp);         // 滤波并存储
            pCloudOut = pCloudOutTmp;
        }
    }

    /* 圆柱直通滤波 */
    void CircleFilter(const float centerX, const float centerY, const float radius,
                      PointCloudPtr pCloudIn, PointCloudPtr pCloudOut) const
    {
        if (pCloudIn != pCloudOut) {
            pCloudOut->header = pCloudIn->header;
            pCloudOut->resize(pCloudIn->size());
        }

        float radius2 = radius * radius;
        size_t index = 0;

        for (size_t i = 0; i < pCloudIn->size(); ++i) {
            float dist = pow(pCloudIn->points[i].x - centerX, 2) + pow(pCloudIn->points[i].y - centerY, 2);
            if (dist > radius2) continue;
            pCloudOut->points[index++] = pCloudIn->points[i];
        }

        pCloudOut->resize(index);
        //pCloudOut->height   = 1;
        //pCloudOut->width    = static_cast<uint32_t>(index);
        //pCloudOut->is_dense = true;
    }

    /**
     * 地面滤波：
     * 基于随机采样一致性算法的地面滤波，
     * 近处效果较好，所耗资源不多，效果不稳定，有噪点
     */
    void FloorFilter(const float maxHeightDiff, const float maxFloorAngle, const int maxIterations,
                     const PointCloudPtr pCloudIn, PointCloudPtr pCloudOutNoFloor, PointCloudPtr pCloudOutFloor) const
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudOutNoFloorT(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudOutFloorT(new pcl::PointCloud<pcl::PointXYZ>);

        pcl::SACSegmentation<PointT> seg;
        pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
        pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
        seg.setOptimizeCoefficients(true);
        seg.setModelType(pcl::SACMODEL_PERPENDICULAR_PLANE);
        seg.setMethodType(pcl::SAC_RANSAC);
        seg.setMaxIterations(maxIterations);
        seg.setAxis(Eigen::Vector3f(0, 0, 1));
        seg.setEpsAngle(ANGLE_TO_RADIAN(maxFloorAngle));

        seg.setDistanceThreshold(maxHeightDiff);  // floor distance
        seg.setOptimizeCoefficients(true);
        seg.setInputCloud(pCloudIn);
        seg.segment(*inliers, *coefficients);

        if (inliers->indices.size() == 0)
            std::cout << "Could not estimate a planar model for the given dataset." << std::endl;

        // REMOVE THE FLOOR FROM THE CLOUD
        pcl::ExtractIndices<pcl::PointXYZ> extract;
        extract.setInputCloud(pCloudIn);
        extract.setIndices(inliers);
        extract.setNegative(true);  // true removes the indices, false leaves only the indices
        extract.filter(*pCloudOutNoFloorT);

        // EXTRACT THE FLOOR FROM THE CLOUD
        extract.setNegative(false);  // true removes the indices, false leaves only the indices
        extract.filter(*pCloudOutFloorT);

        pcl::copyPointCloud(*pCloudOutNoFloorT, *pCloudOutNoFloor);
        pcl::copyPointCloud(*pCloudOutFloorT, *pCloudOutFloor);
    }

    /* 点云欧式聚类 */
    void EuclideanCluster(const float radius, const int minNum, const int maxNum,
                          const PointCloudPtr pCloudIn, vector<pcl::PointIndices> &vIndices)
    {
        if (pCloudIn->empty()) return;
        KdTreePtr treePtr(new KdTree);
        treePtr->setInputCloud(pCloudIn);           // 创建点云索引向量，用于存储实际的点云信息

        pcl::EuclideanClusterExtraction<PointT> ec;
        ec.setClusterTolerance(radius);             // 设置近邻搜索的聚类半径
        ec.setMinClusterSize(minNum);               // 设置一个聚类需要的最少点数
        ec.setMaxClusterSize(maxNum);               // 设置一个聚类需要的最大点数
        ec.setSearchMethod(treePtr);                // 设置点云的搜索机制(KD tree)

        ec.setInputCloud(pCloudIn);
        ec.extract(vIndices);                       // 从点云中提取聚类，并将点云索引保存在local_indices中
    }

private:
    VOXELIZATION_FILTER_C<PointT>   objRadiusOutlierFilter_;
    VOXELIZATION_FILTER_C<PointT>   objVoxelStatisticalFilter_;
};

#endif
