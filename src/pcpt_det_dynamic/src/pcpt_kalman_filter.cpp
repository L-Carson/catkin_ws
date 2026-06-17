#include "pcpt_det_dynamic/pcpt_kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

KF_C::KF_C(void) : isInit_(false), uptTime_(0), dt_(0)
{
    /* 初始化激光雷达观测矩阵及观测噪声 */
    R_lidar_ = MatrixXd(2, 2);
    H_lidar_ = MatrixXd(2, 4);

    R_lidar_ << 0.0225,     0,
                0,     0.0225;

    H_lidar_ << 1, 0, 0, 0,
                0, 1, 0, 0;

    /* 初始化毫米波雷达观测矩阵及观测噪声 */
    R_radar_ = MatrixXd(4, 4);
    H_radar_ = MatrixXd(4, 4);

    R_radar_ <<   1.0,      0,      0,     0,
                    0,    1.0,      0,     0,
                    0,      0,   0.40,     0,
                    0,      0,      0,  0.40;

    H_radar_ <<     1,      0,      0,      0,
                    0,      1,      0,      0,
                    0,      0,      1,      0,
                    0,      0,      0,      1;

    /* 初始化毫米波雷达观测矩阵（极坐标形式）及观测噪声 */
    R_radar_E_ = MatrixXd(3, 3);
    H_radar_E_ = MatrixXd(3, 4);
    R_radar_E_<< 0.09,      0,      0,
                    0, 0.0009,      0,
                    0,      0,   0.09;

    /* 初始化状态转移矩阵及状态初始噪声 */
    P_ = MatrixXd(4, 4);
    P_ << 1,    0,      0,      0,
          0,    1,      0,      0,
          0,    0,   1000,      0,
          0,    0,      0,   1000;

    F_ = MatrixXd(4, 4);
    F_ << 1, 0, 1, 0,
          0, 1, 0, 1,
          0, 0, 1, 0,
          0, 0, 0, 1;
}

void  KF_C::Process(const MEASURE_PACKAGE_C &objMeasurePackage)
{
    double timeCurr = objMeasurePackage.timeStamp;
    if (!isInit_) {
        isInit_  = true;
        Init(objMeasurePackage);
    } else {
        if (0 != Predict(timeCurr)) {
            isInit_ = false;
            ST_LOG_ERR("Predict Fail. ");
            return;
        }
        Update(objMeasurePackage);
        ReformState(x_);
    }
    uptTime_ = timeCurr;
}

Eigen::VectorXd KF_C::GetState(void)
{
    return x_;
}

INT32  KF_C::Init(const MEASURE_PACKAGE_C &objMeasurePackage)
{
    /**
     * - Initialize the state x_ with the first measurement.
     * - Create the covariance matrix.
     * You'll need to convert radar from polar to cartesian coordinates.
     */

    /* first measurement */
    x_ = VectorXd(4);
    x_ << 1, 1, 1, 1;

    xPre_ = VectorXd(4);
    xPre_ << 1, 1, 1, 1;

    /**
     * We only know the position px and py of the initial state, but not the speed vx and vy.
     * In this case, we can set a larger initial value (1000) for vx variance and vy variance.
     */
    P_ = MatrixXd(4, 4);
    P_ << 1,    0,      0,      0,
          0,    1,      0,      0,
          0,    0,   1000,      0,
          0,    0,      0,   1000;

    if (objMeasurePackage.sensorType == MEASURE_PACKAGE_C::RADAR_E) {
        /* Convert radar from polar to cartesian coordinates and initialize state. */
        double rho       = objMeasurePackage.measureData[0];   // range
        double theta     = objMeasurePackage.measureData[1];   // bearing
        double rho_theta = objMeasurePackage.measureData[2];   // velocity of rho

        /* Convertion from polar to cartesian Coordinates */
        x_(0) = rho * cos(theta);
        x_(1) = rho * sin(theta);
        x_(2) = rho_theta * cos(theta);
        x_(3) = rho_theta * sin(theta);
    }
    else if (objMeasurePackage.sensorType == MEASURE_PACKAGE_C::RADAR) {
        /* Initialize state. */
        x_(0) = objMeasurePackage.measureData(0);
        x_(1) = objMeasurePackage.measureData(1);
        x_(2) = objMeasurePackage.measureData(2);
        x_(3) = objMeasurePackage.measureData(3);
    }
    else if (objMeasurePackage.sensorType == MEASURE_PACKAGE_C::LIDAR) {
        /* Initialize state. */
        x_(0) = objMeasurePackage.measureData(0);
        x_(1) = objMeasurePackage.measureData(1);
        x_(2) = 0;
        x_(3) = 0;
    }

    return 0;
}

INT32 KF_C::Predict(const double &timeCurr)
{
    dt_ = timeCurr - uptTime_;
    if (dt_ < 0 || dt_ > 0.5) {
        ST_LOG_ERR("Time Abnormal Prev Time %f, Curr Time %f", uptTime_, timeCurr);
        return -1;
    }

    F_(0, 2) = dt_;
    F_(1, 3) = dt_;

    float noiseAx = 9.0;   //x方向加速度的方差（标准差为3），均值为0
    float noiseAy = 9.0;   //y方向加速度的方差（标准差为3），均值为0

    double dt2 = dt_ * dt_;
    double dt3 = dt2 * dt_;
    double dt4 = dt2 * dt2;

    Q_ = MatrixXd(4, 4);
    Q_ << dt4/4*noiseAx,              0, dt3/2*noiseAx,              0,
                       0, dt4/4*noiseAy,              0, dt3/2*noiseAy,
          dt3/2*noiseAx,              0,   dt2*noiseAx,              0,
                       0, dt3/2*noiseAy,              0,   dt2*noiseAy;

    x_ = F_ * x_;                   //状态预测，忽略控制量，由于采用恒速模型，忽略运动噪声
    MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;         //状态预测的不确定性

    return 0;
}

void  KF_C::Update(const MEASURE_PACKAGE_C &objMeasurePackage)
{
    /**
     * - Use the sensor type to perform the update step.
     * - Update the state and covariance matrices.
     */
    if (objMeasurePackage.sensorType == MEASURE_PACKAGE_C::RADAR_E) {
        /* Radar updates */
        H_radar_E_ = CalculateJacobian(x_);
        H_ = H_radar_E_;
        R_ = R_radar_E_;
        UpdateEkf(objMeasurePackage.measureData);
    } else if (objMeasurePackage.sensorType == MEASURE_PACKAGE_C::RADAR) {
        /* Radar updates */
        H_ = H_radar_;
        R_ = R_radar_;
        UpdateKf(objMeasurePackage.measureData);
    } else if (objMeasurePackage.sensorType == MEASURE_PACKAGE_C::LIDAR) {
        /* Lidar updates */
        H_ = H_lidar_;
        R_ = R_lidar_;
        UpdateKf(objMeasurePackage.measureData);
    }
}

void  KF_C::UpdateKf(const VectorXd &z)
{
    VectorXd z_pred = H_ * x_;              //测量矢量（由预测状态投影得到）
    VectorXd y      = z - z_pred;           //测量残差
    MatrixXd Ht     = H_.transpose();
    MatrixXd S      = H_ * P_ * Ht + R_;    //测量残差协方差
    MatrixXd Si     = S.inverse();
    MatrixXd K      = P_ * Ht * Si;         //最优卡尔曼增益

    x_ = x_ + K * y;
    int x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;
}

void  KF_C::UpdateEkf(const VectorXd &z)
{
    double px = x_(0);
    double py = x_(1);
    double vx = x_(2);
    double vy = x_(3);

    /* Coordinate conversion from Cartesian coordinates to Polar coordinates */
    double rho     = sqrt(px * px + py * py);
    double theta   = atan2(py, px);
    double rho_dot = (px * vx + py * vy) / rho;
    VectorXd h     = VectorXd(3);
    h << rho, theta, rho_dot;
    VectorXd y = z - h;

    /* Nominalization of angle */
    while (y(1) > PI)     y(1) -= 2 * PI;
    while (y(1) < -PI)    y(1) += 2 * PI;

    MatrixXd Ht = H_.transpose();
    MatrixXd S  = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd K  = P_ * Ht * Si;

    x_ = x_ + K * y;
    int x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;
}

VectorXd KF_C::CalculateRMSE(const vector<VectorXd> &estimations, const vector<VectorXd> &groundTruth)
{
    VectorXd rmse(4);
    rmse << 0, 0, 0, 0;

    if (estimations.size() != groundTruth.size() || estimations.size() == 0) {
        ST_LOG_ERR("Invalid estimation or ground_truth data");
        return rmse;
    }

    for (unsigned int i=0; i < estimations.size(); ++i) {

        VectorXd residual = estimations[i] - groundTruth[i];

        /* coefficient-wise multiplication */
        residual = residual.array() * residual.array();
        rmse += residual;
    }

    rmse = rmse / estimations.size();   // calculate the mean
    rmse = rmse.array().sqrt();         // calculate the squared root

    return rmse;
}

MatrixXd KF_C::CalculateJacobian(const VectorXd& stateX)
{
    MatrixXd Hj(3,4);

    /* state parameters */
    double px = stateX(0);
    double py = stateX(1);
    double vx = stateX(2);
    double vy = stateX(3);

    /* preparation of Jacobian terms */
    double c1 = px * px + py * py;
    double c2 = sqrt(c1);
    double c3 = (c1 * c2);

    if (fabs(c1) < 0.0001) {
        ST_LOG_ERR("ERROR - Division by Zero.");
        return Hj;
    }

    /* compute jacobian matrix */
    Hj <<  (px/c2), (py/c2), 0, 0,
          -(py/c1), (px/c1), 0, 0,
            py * (vx * py - vy * px) / c3, px * (px * vy - py * vx) / c3, px / c2, py / c2;

    return Hj;
}

void KF_C::ReformState(Eigen::VectorXd& x)
{
    float velDif = ABS(hypot(x(2), x(3)) - hypot(xPre_(2), xPre_(3)));  //速度差
    float angDif = ABS(atan2(x(3), x(2)) - atan2(xPre_(3), xPre_(2)));  //角度差

    xPre_ = x;

    if(velDif > 20 * dt_ || angDif > 3 * PI * dt_) {
        x(2) = 0;
        x(3) = 0;
    }
}

