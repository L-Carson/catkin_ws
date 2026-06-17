#include "pcpt_comm/pcpt_comm_fitting.h"

/**************************************************************************************
功能描述: 拟合box
修改记录:
**************************************************************************************/
INT32 L_SHAPE_FITTER_C::FitBox(const PointCloudXYZ::Ptr pCloud, ROTATED_RECT_STRU &stRotatedRect)
{
    if (pCloud->size() < 3) return -1;

    cv::Mat matrixPts = cv::Mat::zeros(pCloud->size(), 2, CV_64FC1);

    for (size_t i = 0; i < pCloud->size(); ++i) {
        matrixPts.at<double>(i, 0) = pCloud->points[i].x;
        matrixPts.at<double>(i, 1) = pCloud->points[i].y;
    }

    return FitBoxByCv(matrixPts, stRotatedRect);
}

INT32 L_SHAPE_FITTER_C::FitBox(const PointCloudXYZI::Ptr pCloud, ROTATED_RECT_STRU &stRotatedRect)
{
    if (pCloud->size() < 3) return -1;

    cv::Mat matrixPts = cv::Mat::zeros(pCloud->size(), 2, CV_64FC1);

    for (size_t i = 0; i < pCloud->size(); ++i) {
        matrixPts.at<double>(i, 0) = pCloud->points[i].x;
        matrixPts.at<double>(i, 1) = pCloud->points[i].y;
    }

    return FitBoxByCv(matrixPts, stRotatedRect);
}

/**************************************************************************************
功能描述: 矩形面积最小化
修改记录:
**************************************************************************************/
double L_SHAPE_FITTER_C::CalcAreaCriterion(const cv::Mat& c1, const cv::Mat& c2)
{
    std::vector<double> c1_deep;
    std::vector<double> c2_deep;

    for (int i = 0; i < c1.rows; i++) {
        for (int j = 0; j < c1.cols; j++) {
            c1_deep.push_back(c1.at<double>(i, j));
            c2_deep.push_back(c2.at<double>(i, j));
        }
    }

    // sort vector from min to max.
    sort(c1_deep.begin(), c1_deep.end());
    sort(c2_deep.begin(), c2_deep.end());

    int n_c1 = c1_deep.size();
    int n_c2 = c2_deep.size();

    double c1_min = c1_deep[0];
    double c2_min = c2_deep[0];

    double c1_max = c1_deep[n_c1 - 1];
    double c2_max = c2_deep[n_c2 - 1];

    double alpha = -(c1_max - c1_min) * (c2_max - c2_min);

    return alpha;
}

/**************************************************************************************
功能描述: 点到边缘贴近度最大化
修改记录:
**************************************************************************************/
double L_SHAPE_FITTER_C::CalcClosenessCriterion(const cv::Mat& c1, const cv::Mat& c2)
{
    std::vector<double> c1_deep;
    std::vector<double> c2_deep;

    for (int i = 0; i < c1.rows; i++) {
        for (int j = 0; j < c1.cols; j++) {
            c1_deep.push_back(c1.at<double>(i, j));
            c2_deep.push_back(c2.at<double>(i, j));
        }
    }

    // sort vector from min to max.
    sort(c1_deep.begin(), c1_deep.end());
    sort(c2_deep.begin(), c2_deep.end());

    int n_c1 = c1_deep.size();
    int n_c2 = c2_deep.size();

    double c1_min = c1_deep[0];
    double c2_min = c2_deep[0];

    double c1_max = c1_deep[n_c1 - 1];
    double c2_max = c2_deep[n_c2 - 1];

    std::vector<double> d1;
    std::vector<double> d2;

    for (int i = 0; i < n_c1; i++) {
        double temp = std::min(sqrt(pow((c1_max - c1_deep[i]), 2)), sqrt(pow((c1_deep[i] - c1_min), 2)));
        d1.push_back(temp);
    }

    for (int i = 0; i < n_c2; i++) {
        double temp = std::min(sqrt(pow((c2_max - c2_deep[i]), 2)), sqrt(pow((c2_deep[i] - c2_min), 2)));
        d2.push_back(temp);
    }

    double beta = 0;

    for (size_t i = 0; i < d1.size(); i++) {
        double d = std::max(std::min(d1[i], d2[i]), minDistOfNearestCrit_);
        beta += (1.0 / d);
    }

    return beta;
}

/**************************************************************************************
功能描述: 点到边缘平方误差最小化
修改记录:
**************************************************************************************/
double L_SHAPE_FITTER_C::CalcVariancesCriterion(const cv::Mat& c1, const cv::Mat& c2)
{
    std::vector<double> c1_deep;
    std::vector<double> c2_deep;

    for (int i = 0; i < c1.rows; i++) {
        for (int j = 0; j < c1.cols; j++) {
            c1_deep.push_back(c1.at<double>(i, j));
            c2_deep.push_back(c2.at<double>(i, j));
        }
    }

    // sort vector from min to max.
    sort(c1_deep.begin(), c1_deep.end());
    sort(c2_deep.begin(), c2_deep.end());

    int n_c1 = c1_deep.size();
    int n_c2 = c2_deep.size();

    double c1_min = c1_deep[0];
    double c2_min = c2_deep[0];

    double c1_max = c1_deep[n_c1 - 1];
    double c2_max = c2_deep[n_c2 - 1];

    std::vector<double> d1;
    std::vector<double> d2;

    // D1 = [ min( [np.linalg.norm(c1_max - ic1), np.linalg.norm(ic1 - c1_min)] ) for ic1 in c1 ]
    for (int i = 0; i < n_c1; i++) {
        double temp = std::min(sqrt(pow((c1_max - c1_deep[i]), 2)), sqrt(pow((c1_deep[i] - c1_min), 2)));
        d1.push_back(temp);
    }

    for (int i = 0; i < n_c2; i++) {
        double temp = std::min(sqrt(pow((c2_max - c2_deep[i]), 2)), sqrt(pow((c2_deep[i] - c2_min), 2)));
        d2.push_back(temp);
    }

    std::vector<double> e1;
    std::vector<double> e2;

    //assert(d1.size() == d2.size()); //表达式异常是退出程序，不推荐使用，频繁的调用会极大的影响程序的性能

    // d1.size() || d2.size() Is equals.
    for (size_t i = 0; i < d1.size(); i++) {
        if (d1[i] < d2[i]) e1.push_back(d1[i]);
        else               e2.push_back(d2[i]);
    }

    double v1 = 0.0;
    if (!e1.empty()) v1 = (-1.0) * CalcVar(e1);

    double v2 = 0.0;
    if (!e2.empty()) v2 = (-1.0) * CalcVar(e2);

    double gamma = v1 + v2;
    return gamma;
}

/**************************************************************************************
功能描述: 计算方差
修改记录:
**************************************************************************************/
double L_SHAPE_FITTER_C::CalcVar(const std::vector<double>& v)
{
    double sum  = std::accumulate(std::begin(v), std::end(v), 0.0);
    double mean = sum / v.size();

    double accVarNum = 0.0;

    std::for_each(std::begin(v), std::end(v), [&](const double d) { accVarNum += (d - mean) * (d - mean); });

    double var = sqrt(accVarNum / (v.size() - 1));

    return var;
}

void L_SHAPE_FITTER_C::CalcCrossPoint(const double a0, const double a1, const double b0, const double b1,
                                       const double c0, const double c1, double& x, double& y)
{
    x = (b0 * (-c1) - b1 * (-c0)) / (a0 * b1 - a1 * b0);
    y = (a1 * (-c0) - a0 * (-c1)) / (a0 * b1 - a1 * b0);
}

cv::RotatedRect L_SHAPE_FITTER_C::CalcRectContour()
{
    vertexPts_.clear();

    double topLeftX = 0.0, topLeftY = 0.0;
    CalcCrossPoint(a_[0], a_[1], b_[0], b_[1], c_[0], c_[1], topLeftX, topLeftY);
    vertexPts_.push_back(cv::Point2f(topLeftX, topLeftY));

    double topRightX = 0.0, topRightY = 0.0;
    CalcCrossPoint(a_[1], a_[2], b_[1], b_[2], c_[1], c_[2], topRightX, topRightY);
    vertexPts_.push_back(cv::Point2f(topRightX, topRightY));

    double bottomLeftX = 0.0, bottomLeftY = 0.0;
    CalcCrossPoint(a_[2], a_[3], b_[2], b_[3], c_[2], c_[3], bottomLeftX, bottomLeftY);
    vertexPts_.push_back(cv::Point2f(bottomLeftX, bottomLeftY));

    double bottomRightX = 0.0, bottomRightY = 0.0;
    CalcCrossPoint(a_[3], a_[0], b_[3], b_[0], c_[3], c_[0], bottomRightX, bottomRightY);
    vertexPts_.push_back(cv::Point2f(bottomRightX, bottomRightY));

    return cv::minAreaRect(vertexPts_);
}

INT32 L_SHAPE_FITTER_C::FitBoxByCv(const cv::Mat& matrixPts, ROTATED_RECT_STRU &stRotatedRect)
{
    double minimalCost = (-1.0) * std::numeric_limits<double>::max();
    double bestTheta   = std::numeric_limits<double>::max();

    // Search This Best Direction For ENUM.
    int loop_number = ceil((PI / 2.0 - stepAngleForSearch_) / stepAngleForSearch_);

    cv::Mat e1 = cv::Mat::zeros(1, 2, CV_64FC1);
    cv::Mat e2 = cv::Mat::zeros(1, 2, CV_64FC1);

    for (int k = 0; k < loop_number; ++k) {
        double theta = k * stepAngleForSearch_;
        double cost  = std::numeric_limits<double>::min();

        // Be Sure Yaw Is In Range.
        if (theta < (PI / 2.0 - stepAngleForSearch_)) {
            e1.at<double>(0, 0) = cos(theta);
            e1.at<double>(0, 1) = sin(theta);
            e2.at<double>(0, 0) = -sin(theta);
            e2.at<double>(0, 1) = cos(theta);

            cv::Mat c1 = matrixPts * e1.t();
            cv::Mat c2 = matrixPts * e2.t();

            switch(enCriterion_) {
                case CRITERION_ENUM::AREA:
                    cost = CalcAreaCriterion(c1, c2);
                    break;
                case CRITERION_ENUM::CLOSENESS:
                    cost = CalcClosenessCriterion(c1, c2);
                    break;
                case CRITERION_ENUM::VARIANCE:
                    cost = CalcVariancesCriterion(c1, c2);
                    break;
                default:
                    std::cout << "L-Shaped Algorithm Criterion Is Not Supported." << std::endl;
                    break;
            }

            if (minimalCost < cost) {
                minimalCost = cost;
                bestTheta   = theta;
            }
        } else {
            break;
        }
    }

    if (minimalCost > (-1.0) * std::numeric_limits<double>::max() && bestTheta < std::numeric_limits<double>::max()) {
        ;  // Do Nothing, Continue Run As Follows.
    } else {
        std::cout << "RotatedRect Fit Failed." << std::endl;
        return -1;
    }

    double sinS = sin(bestTheta);
    double cosS = cos(bestTheta);

    cv::Mat e1S          = cv::Mat::zeros(1, 2, CV_64FC1);
    e1S.at<double>(0, 0) = cosS;
    e1S.at<double>(0, 1) = sinS;

    cv::Mat e2S          = cv::Mat::zeros(1, 2, CV_64FC1);
    e2S.at<double>(0, 0) = -sinS;
    e2S.at<double>(0, 1) = cosS;

    cv::Mat c1S = matrixPts * e1S.t();  //返回e1S的转置，并没有对e1S转置
    cv::Mat c2S = matrixPts * e2S.t();

    double minC1S = std::numeric_limits<double>::max();
    double maxC1S = (-1.0) * std::numeric_limits<double>::max();
    double minC2S = std::numeric_limits<double>::max();
    double maxC2S = (-1.0) * std::numeric_limits<double>::max();

    cv::minMaxLoc(c1S, &minC1S, &maxC1S, NULL, NULL);
    cv::minMaxLoc(c2S, &minC2S, &maxC2S, NULL, NULL);

    a_.clear();
    b_.clear();
    c_.clear();

    if (minC1S < std::numeric_limits<double>::max()          &&
        maxC1S > (-1.0) * std::numeric_limits<double>::max() &&
        minC2S < std::numeric_limits<double>::max()          &&
        maxC2S > (-1.0) * std::numeric_limits<double>::max()) {
        a_.push_back(cosS);
        b_.push_back(sinS);
        c_.push_back(minC1S);

        a_.push_back(-sinS);
        b_.push_back(cosS);
        c_.push_back(minC2S);

        a_.push_back(cosS);
        b_.push_back(sinS);
        c_.push_back(maxC1S);

        a_.push_back(-sinS);
        b_.push_back(cosS);
        c_.push_back(maxC2S);

        cv::RotatedRect rRect = CalcRectContour();

        stRotatedRect.x      = rRect.center.x;
        stRotatedRect.y      = rRect.center.y;
        stRotatedRect.width  = rRect.size.width;
        stRotatedRect.height = rRect.size.height;
        stRotatedRect.angle  = ANGLE_TO_RADIAN(rRect.angle);

        return 0;
    } else {
        return -1;
    }
}

/**************************************************************************************
功能描述: 基于随机采样拟合空间平面
修改记录:
**************************************************************************************/
void PLANE_RANSAC_FITTER_C::Ransac3DPlane(std::vector<COORDINATE_3D_STRU>& vstCoor, PLANE_PARAM_STRU &stPlane)
{
    float minSamplePercent = 0.4f;  // Select the initial sample percentage
    float maxSamplePercent = 0.9f;  // Maximum percentage control iteration stop
    uint32_t minSampleNum = static_cast<int>(float(vstCoor.size()) * minSamplePercent);
    uint32_t maxSampleNum = static_cast<int>(float(vstCoor.size()) * maxSamplePercent);

    float distThreshold = 0.2f;
    uint32_t maxIterations = 30;
    uint32_t iterations = 0;
    std::vector<COORDINATE_3D_STRU> maxPlanePoints; // Satisfy the maximum number of interior points of the plane model

    // 迭代拟合初步模型
    while (iterations < maxIterations || maxPlanePoints.size() < maxSampleNum) {
        iterations++;
        COORDINATE_3D_STRU ptTemp1, ptTemp2, ptTemp3;
        PLANE_PARAM_STRU stPlaneParamTemp;
        std::vector<COORDINATE_3D_STRU> points;
        std::vector<COORDINATE_3D_STRU> tempPlanePoints;
        std::random_shuffle(vstCoor.begin(), vstCoor.end());   // 随机排序输入点云

        // 初始采样部分点
        for (uint32_t i = 0; i < minSampleNum; ++i) {
            ptTemp1.x = vstCoor[i].x;
            ptTemp1.y = vstCoor[i].y;
            ptTemp1.z = vstCoor[i].z;

            points.push_back(ptTemp1);
        }

        // 初步拟合平面
        PlaneFromPoints(points, stPlaneParamTemp);
        points.clear();

        // 基于初步拟合参数，统计内点
        for (int i = 0; i < static_cast<int>(vstCoor.size()); ++i) {
            float distTemp;
            ptTemp2.x = vstCoor[i].x;
            ptTemp2.y = vstCoor[i].y;
            ptTemp2.z = vstCoor[i].z;
            distTemp = DistanceToPlane(ptTemp2, stPlaneParamTemp);
            if (distTemp < distThreshold)
                tempPlanePoints.push_back(ptTemp2);
            else
                continue;
        }

        // 缓存最优模型的内点
        if (tempPlanePoints.size() > maxPlanePoints.size()) {
            maxPlanePoints.clear();
            maxPlanePoints.reserve(tempPlanePoints.size());
            for (int i = 0; i < static_cast<int>(tempPlanePoints.size()); ++i) {
                ptTemp3.x = tempPlanePoints[i].x;
                ptTemp3.y = tempPlanePoints[i].y;
                ptTemp3.z = tempPlanePoints[i].z;
                maxPlanePoints.push_back(ptTemp3);
            }
        }
        tempPlanePoints.clear();
    }

    // 在初步选取的内点基础上，拟合最终的平面模型
    PlaneFromPoints(maxPlanePoints, stPlane);
}

/**************************************************************************************
功能描述: 点到平面的距离
修改记录:
**************************************************************************************/
float PLANE_RANSAC_FITTER_C::DistanceToPlane(COORDINATE_3D_STRU& stCoor, PLANE_PARAM_STRU &stPlane)
{
    return (stPlane.coeffX * stCoor.x + stPlane.coeffY * stCoor.y + stPlane.coeffZ * stCoor.z + stPlane.constH);
}

/**************************************************************************************
功能描述: 基于坐标点拟合平面
修改记录:
**************************************************************************************/
void PLANE_RANSAC_FITTER_C::PlaneFromPoints(std::vector<COORDINATE_3D_STRU> &vstCoor, PLANE_PARAM_STRU &stPlane)
{
    if (vstCoor.size() < 3)
        ST_LOG_ERR("Not enough points to calculate plane.");

    COORDINATE_3D_STRU sum = {0, 0, 0};

    for (int i = 0; i < static_cast<int>(vstCoor.size()); ++i) {
        sum.x += vstCoor[i].x;
        sum.y += vstCoor[i].y;
        sum.z += vstCoor[i].z;
    }

    COORDINATE_3D_STRU centroid = {0, 0, 0};    // 点簇形心点
    centroid.x = sum.x / float(vstCoor.size());
    centroid.y = sum.y / float(vstCoor.size());
    centroid.z = sum.z / float(vstCoor.size());

    float xx = 0, xy = 0, xz = 0, yy = 0, yz = 0, zz = 0;
    for (int i = 0; i < static_cast<int>(vstCoor.size()); ++i) {
        COORDINATE_3D_STRU temp;
        temp.x = vstCoor[i].x - centroid.x;
        temp.y = vstCoor[i].y - centroid.y;
        temp.z = vstCoor[i].z - centroid.z;

        xx += temp.x * temp.x;
        xy += temp.x * temp.y;
        xz += temp.x * temp.z;
        yy += temp.y * temp.y;
        yz += temp.y * temp.z;
        zz += temp.z * temp.z;
    }

    float detX = yy * zz - yz * yz;
    float detY = xx * zz - xz * xz;
    float detZ = xx * yy - xy * xy;

    float detMax = std::max(std::max(detX, detY), detZ);
    if (detMax <= 0)
        stPlane.SetVale(0, 0, 0, 0);

    // TODO: 假设平面与某一轴平行？
    COORDINATE_3D_STRU dir{};
    if (detMax == detX) {
        float a = static_cast<float>((xz * yz - xy * zz) / detX);
        float b = static_cast<float>((xy * yz - xz * yy) / detX);
        dir = {1.0, a, b};
    } else if (detMax == detY) {
        float a = static_cast<float>((yz * xz - xy * zz) / detY);
        float b = static_cast<float>((xy * xz - yz * xx) / detY);
        dir = {a, 1.0, b};
    } else {
        float a = static_cast<float>((yz * xy - xz * yy) / detZ);
        float b = static_cast<float>((xz * xy - yz * xx) / detZ);
        dir = {a, b, 1.0};
    }

    float dirTemp = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    dir.x /= dirTemp;
    dir.y /= dirTemp;
    dir.z /= dirTemp;

    stPlane.coeffX = dir.x;
    stPlane.coeffY = dir.y;
    stPlane.coeffZ = dir.z;
    stPlane.constH = -(stPlane.coeffX * centroid.x + stPlane.coeffY * centroid.y + stPlane.coeffZ * centroid.z);
}

/**************************************************************************************
功能描述: 核心算法 - 拟合
修改记录:
**************************************************************************************/
void DBSCAN_C::Fit(const vector<POINT_COORD_2D_STRU>& points)
{
    size_t n = points.size();
    visited_.resize(n, false);
    labels_.resize(n, noise_);
    int clusterId = 0;

    for (size_t i = 0; i < n; ++i) {
        if (visited_[i]) continue;  // 如果点已被访问，跳过
        visited_[i] = true;
        vector<int> neighbors = RegionQuery(i, points); // 获取点的ε邻域

        if (neighbors.size() + 1 < minPts_) {
            // 如果ε邻域内点少于minPts，标记为噪声点
            labels_[i] = noise_;
        } else {
            // 否则，开始一个新的簇
            ExpandCluster(i, neighbors, clusterId, points);
            ++clusterId;
        }
    }
}

/**************************************************************************************
功能描述: 获取每个点的簇标签（按照输入数据的下标索引进行排序）
修改记录:
**************************************************************************************/
const vector<int>& DBSCAN_C::GetLabelsByIndex() const
{
    return labels_;
}

/**************************************************************************************
功能描述: 获取每个点的簇标签
修改记录: unordered_map<标签, 索引>
**************************************************************************************/
std::unordered_map<int, vector<size_t>> DBSCAN_C::GetLabels() const
{
    std::unordered_map<int, vector<size_t>> umLabels;
    for (size_t i = 0; i < labels_.size(); ++i) {
        umLabels[labels_[i]].push_back(i);
    }
    return umLabels;
}

/**************************************************************************************
功能描述: 获取有效点的簇标签
修改记录: unordered_map<标签, 索引>
**************************************************************************************/
std::unordered_map<int, vector<size_t>> DBSCAN_C::GetValidLabels() const
{
    std::unordered_map<int, vector<size_t>> umLabels;
    for (size_t i = 0; i < labels_.size(); ++i) {
        if (labels_[i] == noise_) continue;
        umLabels[labels_[i]].push_back(i);
    }
    return umLabels;
}

/**************************************************************************************
功能描述: 获取点i的ε邻域
修改记录:
**************************************************************************************/
vector<int> DBSCAN_C::RegionQuery(size_t i, const vector<POINT_COORD_2D_STRU>& points) const
{
    vector<int> neighbors;
    for (size_t j = 0; j < points.size(); ++j) {
        if (i != j && Distance(points[i], points[j]) <= epsilon_) {
            neighbors.push_back(j);
        }
    }
    return neighbors;
}

/**************************************************************************************
功能描述: 计算两点之间的欧氏距离
修改记录:
**************************************************************************************/
double DBSCAN_C::Distance(const POINT_COORD_2D_STRU& stP1, const POINT_COORD_2D_STRU& stP2) const
{
    return sqrt(pow(stP1.x - stP2.x, 2) + pow(stP1.y - stP2.y, 2));
}

/**************************************************************************************
功能描述: 扩展簇
修改记录:
**************************************************************************************/
void DBSCAN_C::ExpandCluster(size_t i, const vector<int>& neighbors, int clusterId, const vector<POINT_COORD_2D_STRU>& points)
{
    labels_[i] = clusterId;  // 将点i标记为当前簇的成员
    queue<int> q;
    for (int neighbor : neighbors) q.push(neighbor);

    while (!q.empty()) {
        int pointIndex = q.front();
        q.pop();

        if (!visited_[pointIndex]) {
            visited_[pointIndex] = true;

            // 获取pointIndex的ε邻域
            vector<int> pointNeighbors = RegionQuery(pointIndex, points);
            if (pointNeighbors.size() >= minPts_) {
                // 如果邻域点数大于等于minPts，继续扩展
                for (int neighbor : pointNeighbors) q.push(neighbor);
            }
        }

        // 将pointIndex标记为当前簇的成员，如果是噪声点，改为当前簇
        if (labels_[pointIndex] == noise_) labels_[pointIndex] = clusterId;
    }
}

