#ifndef __PCPT_SHAPE_FITTING_H__
#define __PCPT_SHAPE_FITTING_H__

#include "pcpt_comm/pcpt_comm_base.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

/**
 * 由于在头文件"comm_extend/comm_base_extend.h"中"#define PCL_NO_PRECOMPILE",
 * 在使用<pcl/segmentation/sac_segmentation.h>等类似头文件时会产生boost库冲突，
 * 程序可成功编译，但是无法启动，直接段错误，
 * 因此在使用时，需要将包含有"#define PCL_NO_PRECOMPILE"的头文件放在<pcl/segmentation/sac_segmentation.h>后面，
 * 头文件"pcpt_comm/pcpt_comm_base.h"间接包含了"#define PCL_NO_PRECOMPILE"。
 * 或者使用"#undef PCL_NO_PRECOMPILE"。
 * ubuntu_14.04测试有该问题，ubuntu_18.04测试不存在该问题；
 */
#ifdef PCL_NO_PRECOMPILE
#undef PCL_NO_PRECOMPILE
#endif

// 屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <pcl/segmentation/sac_segmentation.h>
#pragma GCC diagnostic pop

#include <pcl/point_types.h>
#include <pcl/features/moment_of_inertia_estimation.h>
#include <pcl/ModelCoefficients.h>

#include <pcl/sample_consensus/sac_model_line.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/filters/project_inliers.h>

/* 该头文件中有编译告警，且该头文件应该不是必须的。 */
//#include <pcl/visualization/pcl_visualizer.h>

#include <vector>
#include <numeric>

using namespace std;

enum CRITERION_ENUM {
    AREA,       //矩形面积最小化
    CLOSENESS,  //点到边缘贴近度最大化
    VARIANCE    //点到边缘平方误差最小化
};

/**************************************************************************************
功能描述: 基于L-Shape的3DLidar障碍物位姿估计
修改记录: L-shape拟合器
**************************************************************************************/
class L_SHAPE_FITTER_C
{
public:
    L_SHAPE_FITTER_C(const double minDist   = 0.01,
                     const double stepAngle = 2.0,
                     const CRITERION_ENUM enCriterion = CRITERION_ENUM::VARIANCE)
    : minDistOfNearestCrit_(minDist)
    , stepAngleForSearch_(ANGLE_TO_RADIAN(stepAngle))
    , enCriterion_(enCriterion)
    {}

    ~L_SHAPE_FITTER_C(){}

    INT32 FitBox(const PointCloudXYZ::Ptr pCloud,  ROTATED_RECT_STRU &stRotatedRect);
    INT32 FitBox(const PointCloudXYZI::Ptr pCloud, ROTATED_RECT_STRU &stRotatedRect);

private:
    double              minDistOfNearestCrit_;
    double              stepAngleForSearch_;
    CRITERION_ENUM      enCriterion_;

    vector<double>      a_;
    vector<double>      b_;
    vector<double>      c_;

    vector<cv::Point2f> vertexPts_;  // 旋转矩形的顶点有序集
    cv::Point2f         hotPt_;

    double CalcAreaCriterion(const cv::Mat& c1, const cv::Mat& c2);
    double CalcClosenessCriterion(const cv::Mat& c1, const cv::Mat& c2);
    double CalcVariancesCriterion(const cv::Mat& c1, const cv::Mat& c2);
    double CalcVar(const std::vector<double>& v);
    void   CalcCrossPoint(const double a0, const double a1, const double b0, const double b1,
                          const double c0, const double c1, double& x, double& y);
    cv::RotatedRect CalcRectContour();
    INT32  FitBoxByCv(const cv::Mat& matrixPts, ROTATED_RECT_STRU &stRotatedRect);
};

/**************************************************************************************
功能描述: 随机采样一致性算法拟合平面
修改记录:
**************************************************************************************/
class PLANE_RANSAC_FITTER_C
{
public:
    void Ransac3DPlane(std::vector<COORDINATE_3D_STRU>& vstCoor, PLANE_PARAM_STRU &stPlane);

private:
    float DistanceToPlane(COORDINATE_3D_STRU& stCoor, PLANE_PARAM_STRU &stPlane);
    void  PlaneFromPoints(std::vector<COORDINATE_3D_STRU> &vstCoor, PLANE_PARAM_STRU &stPlane);
};

/**************************************************************************************
功能描述: 基于PCL的平面拟合
修改记录: 由于"#define PCL_NO_PRECOMPILE"限制了环境，（预处理阶段）
          必须将成员函数的实现部分放在源文件中，因此无法使用模板（编译阶段）
**************************************************************************************/
class PCL_PLANE_FITTER_C
{
public:
    template<typename PointT>
    int FittingPlaneParam(const boost::shared_ptr<pcl::PointCloud<PointT>>& pCloud, const float disThresh, PLANE_PARAM_STRU &stPlane) const
    {
        if (!pCloud || pCloud->size() < 3) {
            ST_LOG_ERR("Too few points to fit the plane.");
            return -1;
        }

        pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
        pcl::PointIndices::Ptr inliers(new pcl::PointIndices);

        pcl::SACSegmentation<PointT> seg;
        seg.setOptimizeCoefficients(true);          // Optional
        seg.setModelType(pcl::SACMODEL_PLANE);      // Mandatory
        seg.setMethodType(pcl::SAC_RANSAC);
        seg.setDistanceThreshold(disThresh);        // 点到拟合平面的最大距离阈值 单位m
        seg.setInputCloud(pCloud);
        seg.segment(*inliers, *coefficients);

        if (inliers->indices.size() < 3) {
            ST_LOG_ERR("Could not estimate a planar model for the given dataset.");
            return -1;
        }

        INT32 sign = SIGN(coefficients->values[3]);
        coefficients->values[0] *= sign;
        coefficients->values[1] *= sign;
        coefficients->values[2] *= sign;
        coefficients->values[3] *= sign;

        stPlane.coeffX = coefficients->values[0];
        stPlane.coeffY = coefficients->values[1];
        stPlane.coeffZ = coefficients->values[2];
        stPlane.constH = coefficients->values[3];

        return 0;
    }
};

/**************************************************************************************
功能描述: 基于PCL的3d包围盒拟合
修改记录:
**************************************************************************************/
template<class PointT>
class PCL_BOX_FITTER_C
{
public:
    typedef typename pcl::PointCloud<PointT>      PointCloud;
    typedef typename pcl::PointCloud<PointT>::Ptr PointCloudPtr;

public:
    PCL_BOX_FITTER_C(){}
    ~PCL_BOX_FITTER_C(){}

    /**
     * AABB(Axis-Aligned Bounding Box) 轴对齐包围盒
     */
    int BoxAABB(PointCloudPtr pCloud, RECTANGLE_STRU& stRect)
    {
        /* 实例化一个Momentof */
        pcl::MomentOfInertiaEstimation <PointT> feature_extractor;
        feature_extractor.setInputCloud(pCloud);
        feature_extractor.compute();

        /* 声明一些必要的变量 */
        std::vector <float> moment_of_inertia;
        std::vector <float> eccentricity;
        PointT min_point_AABB;
        PointT max_point_AABB;

        /* 计算描述符和其他的特征 */
        feature_extractor.getMomentOfInertia(moment_of_inertia);
        feature_extractor.getEccentricity(eccentricity);
        feature_extractor.getAABB(min_point_AABB, max_point_AABB);

        stRect.SetValue(min_point_AABB.x, max_point_AABB.x, min_point_AABB.y, max_point_AABB.y);
        return 0;
    }

    /**
     * OBB(Oriented Bounding Bix) 定向包容盒
     */
    int BoxOBB(PointCloudPtr pCloud)
    {
        /* 创建惯性矩估算对象 */
        pcl::MomentOfInertiaEstimation<PointT> feature_extractor;
        feature_extractor.setInputCloud(pCloud);
        feature_extractor.compute();

        /* 矩阵特征值、特征向量 */
        std::vector<float> moment_of_inertia;   //惯性矩
        std::vector<float> eccentricity;        //主轴方向的特征向量
        PointT min_point_OBB;
        PointT max_point_OBB;
        PointT position_OBB;
        Eigen::Matrix3f rotational_matrix_OBB;  //矩阵
        float major_value, middle_value, minor_value;
        Eigen::Vector3f major_vector, middle_vector, minor_vector;  //主轴、中轴、辅助轴
        Eigen::Vector3f mass_center;                                //质心

        feature_extractor.getMomentOfInertia(moment_of_inertia);    //获取惯性矩
        feature_extractor.getEccentricity(eccentricity);            //获取离心率
        feature_extractor.getOBB(min_point_OBB, max_point_OBB, position_OBB, rotational_matrix_OBB);    //获取OBB盒子
        feature_extractor.getEigenValues(major_value, middle_value, minor_value);
        feature_extractor.getEigenVectors(major_vector, middle_vector, minor_vector);   //获取主轴major_vector，中轴middle_vector，辅助轴minor_vector
        feature_extractor.getMassCenter(mass_center);               //获取质心

        /* 最小外接立方体的长、宽、高；quat:旋转矩阵；position:中心位置 */
        cout << max_point_OBB.x << endl;
        cout << max_point_OBB.y << endl;
        cout << min_point_OBB.x << endl;
        cout << min_point_OBB.y << endl;
        cout << position_OBB.x << endl;
        cout << position_OBB.y << endl;
        cout << position_OBB.z << endl;
        cout << max_point_OBB.x - min_point_OBB.x << endl;
        cout << max_point_OBB.y - min_point_OBB.y << endl;
        cout << max_point_OBB.z - min_point_OBB.z << endl;
        cout << mass_center << std::endl;               //质心
        cout << rotational_matrix_OBB << endl;          //矩阵

        return 0;
    }
};

/**************************************************************************************
功能描述: 直线拟合
修改记录:
**************************************************************************************/
template<class PointT>
class PCL_LINE_FITTER_C
{
public:
    typedef typename pcl::PointCloud<PointT>                    PointCloud;
    typedef typename pcl::PointCloud<PointT>::Ptr               PointCloudPtr;
    typedef typename pcl::SampleConsensusModelLine<PointT>      SampleConsensusModelLine;
    typedef typename pcl::SampleConsensusModelLine<PointT>::Ptr SampleConsensusModelLinePtr;

public:
    PCL_LINE_FITTER_C() {}
    ~PCL_LINE_FITTER_C() {}

    /**
     * 直线拟合: y = ax + b
     */
    int PclSACModelLine(const PointCloudPtr pCloud, double& coeffA, double& coeffB, PointCloudPtr pCloudInlier = nullptr,
                        const float distThres = 0.01, const int maxIterTimes = 500) const
    {
        if (pCloud->size() < 2) return -1;

        Eigen::VectorXf coeff;
        std::vector<int> inliers;                                   // 获取属于拟合出的内点

        /* RANSAC直线拟合 */
        SampleConsensusModelLinePtr modelLine(new SampleConsensusModelLine(pCloud));    // 创建直线模型
        pcl::RandomSampleConsensus<PointT> ransac(modelLine);       // 创建RANSAC算法对象
        ransac.setDistanceThreshold(distThres);                     // 设置内点距离阈值
        ransac.setMaxIterations(maxIterTimes);                      // 设置最大迭代次数
        ransac.computeModel();                                      // 执行RANSAC拟合
        ransac.getInliers(inliers);                                 // 获取内点索引
        ransac.getModelCoefficients(coeff);                         // 获取直线模型参数

        if (pCloudInlier) {
            pcl::copyPointCloud(*pCloud, inliers, *pCloudInlier);  // 拟合出的内点
        }

#if 0
        /* 投影到拟合直线 */
        PointCloudPtr lineProCloud(new PointCloud);
        pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
        coefficients->values.resize(6);
        for (int i = 0; i < 6; ++i) coefficients->values[i] = coeff[i];     // 获取直线的6个参数
        pcl::ProjectInliers<PointT> proj;
        proj.setModelType(pcl::SACMODEL_LINE);
        proj.setInputCloud(pCloud);
        proj.setModelCoefficients(coefficients);
        proj.filter(*lineProCloud);     // 投影点云到直线上

        /* 可视化原始点云和投影点云 */
        int vp1, vp2;
        pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Visualization"));

        viewer->createViewPort(0.0, 0.0, 0.5, 1.0, vp1);
        viewer->setBackgroundColor(1.0, 1.0, 1.0, vp1);  // 白色背景
        viewer->addText("Original Point Cloud", 10, 10, "vp1_text", vp1);
        pcl::visualization::PointCloudColorHandlerCustom<PointT> cloudColor(pCloud, 255, 0, 0);           // 红色
        viewer->addPointCloud(pCloud, cloudColor, "original_cloud", vp1);

        viewer->createViewPort(0.5, 0.0, 1.0, 1.0, vp2);
        viewer->setBackgroundColor(0.98, 0.98, 0.98, vp2);  // 浅灰色背景
        viewer->addText("Projected Point Cloud", 10, 10, "vp2_text", vp2);
        pcl::visualization::PointCloudColorHandlerCustom<PointT> projectedColor(lineProCloud, 0, 255, 0); // 绿色
        viewer->addPointCloud(lineProCloud, projectedColor, "projected_cloud", vp2);

#endif

        if (0) {
            //coeff[0], coeff[1], coeff[2] 为直线上的一点(x, y, z)
            //coeff[3], coeff[4], coeff[5] 为直线的方向向量
            std::cout << "直线上一点的x坐标为：" << coeff[0]
                      << "\n直线上一点的y坐标为：" << coeff[1]
                      << "\n直线上一点的z坐标为：" << coeff[2]
                      << "\n直线方向向量的x为：" << coeff[3]
                      << "\n直线方向向量的y为：" << coeff[4]
                      << "\n直线方向向量的z为：" << coeff[5]
                      << std::endl;
        }

        if (SIG(coeff[3]) == 0){
            coeffA = TEN_6;
        } else {
            coeffA = coeff[4] / coeff[3];
        }
        coeffB = coeff[1] - coeffA * coeff[0];

        return 0;
    }

};

class DBSCAN_C
{
public:
    DBSCAN_C(double epsilon, size_t minPts) : epsilon_(epsilon), minPts_(minPts) {}

    void Fit(const vector<POINT_COORD_2D_STRU>& points);
    const vector<int>& GetLabelsByIndex() const;
    std::unordered_map<int, vector<size_t>> GetLabels() const;
    std::unordered_map<int, vector<size_t>> GetValidLabels() const;

private:
    int             noise_{-1};     // 噪点标签
    double          epsilon_;
    size_t          minPts_;
    vector<bool>    visited_;       // 用于标记点是否已访问
    vector<int>     labels_;        // 每个点的簇标签

    vector<int> RegionQuery(size_t i, const vector<POINT_COORD_2D_STRU>& points) const;
    double Distance(const POINT_COORD_2D_STRU& stP1, const POINT_COORD_2D_STRU& stP2) const;
    void   ExpandCluster(size_t i, const vector<int>& neighbors, int clusterId, const vector<POINT_COORD_2D_STRU>& points);
};

#endif

