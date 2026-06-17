#include "pcpt_comm/pcpt_comm_curve.h"

/**************************************************************************************
功能描述: 基于least squares(最小二乘法)拟合多阶曲线，order为要拟合函数的阶数
修改记录:
**************************************************************************************/
int PCPT_CURVE_C::FitMultiOrderCurve(const vector<COORDINATE_2D_STRU>& vstPos, const uint32_t order, vector<double>& vCoeff) const
{
    if (order <= 0 || order > vstPos.size() - 1) return -1;

    switch(order) {
        case 1:
            Fit1(vstPos, vCoeff);
            break;
        case 2:
            Fit2(vstPos, vCoeff);
            break;
        case 3:
            Fit3(vstPos, vCoeff);
            break;
        case 4:
            Fit4(vstPos, vCoeff);
            break;
        case 5:
            Fit5(vstPos, vCoeff);
            break;
        default:
            break;
    }

    return 0;
}

/**************************************************************************************
功能描述: 基于Eigen库拟合多阶曲线，order为要拟合函数的阶数
          原理：QR分解，所得结果与基于最小二乘法一致
修改记录:
**************************************************************************************/
int PCPT_CURVE_C::FitMultiOrderCurveE(const vector<COORDINATE_2D_STRU>& vstPos, const uint32_t order, vector<double>& vCoeff) const
{
    if (order <= 0 || order > vstPos.size() - 1) return -1;

    Eigen::MatrixXd matXY(vstPos.size(), order + 1);
    Eigen::VectorXd vecY(vstPos.size());

    for (uint32_t i = 0; i < vstPos.size(); ++i) {
        matXY(i, 0) = 1.0;
        vecY(i) = vstPos[i].y;
        for (uint32_t j = 0; j < order; ++j)
            matXY(i, j + 1) = matXY(i, j) * vstPos[i].x;
    }

    auto Q = matXY.householderQr(); //QR分解
    Eigen::VectorXd result = Q.solve(vecY);

    for (uint32_t i = 0; i <= order; ++i) vCoeff.push_back(result(i));

    return 0;
}

/**************************************************************************************
功能描述: 基于最小二乘法拟合圆的方程: (x-centerX)^2 + (y-centerY)^2 = radius^2
修改记录:
**************************************************************************************/
int PCPT_CURVE_C::FitCircle(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const
{
    if (vstPos.size() < 3) return -1;

    double centerX = 0.0f;
    double centerY = 0.0f;
    double radius  = 0.0f;

    double sum_x = 0.0f,  sum_y = 0.0f;
    double sum_x2 = 0.0f, sum_y2 = 0.0f;
    double sum_x3 = 0.0f, sum_y3 = 0.0f;
    double sum_xy = 0.0f, sum_x1y2 = 0.0f, sum_x2y1 = 0.0f;

    uint32_t num = vstPos.size();

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        double x = stPos.x;
        double y = stPos.y;
        double x2 = x * x;
        double y2 = y * y;
        sum_x += x;
        sum_y += y;
        sum_x2 += x2;
        sum_y2 += y2;
        sum_x3 += x2 * x;
        sum_y3 += y2 * y;
        sum_xy += x * y;
        sum_x1y2 += x * y2;
        sum_x2y1 += x2 * y;
    }

    double C, D, E, G, H;
    double a, b, c;

    C = num * sum_x2 - sum_x * sum_x;
    D = num * sum_xy - sum_x * sum_y;
    E = num * sum_x3 + num * sum_x1y2 - (sum_x2 + sum_y2) * sum_x;
    G = num * sum_y2 - sum_y * sum_y;
    H = num * sum_x2y1 + num * sum_y3 - (sum_x2 + sum_y2) * sum_y;
    a = (H * D - E * G) / (C * G - D * D);
    b = (H * C - E * D) / (D * D - G * C);
    c = -(a * sum_x + b * sum_y + sum_x2 + sum_y2) / num;

    centerX = a / (-2);
    centerY = b / (-2);
    radius = sqrt(a * a + b * b - 4 * c) / 2;

    vCoeff.resize(4);
    vCoeff[0] = centerX;
    vCoeff[1] = centerY;
    vCoeff[2] = radius;
    vCoeff[3] = 0;          //0表示拟合的是圆的参数

    return 0;
}

/**************************************************************************************
功能描述: 基于最小二乘法拟合1阶多项式函数: y = a*x + b
           Fitting first-order polynomial by least square method
           a,b：返回拟合系数
修改记录:
**************************************************************************************/
void PCPT_CURVE_C::Fit1(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const
{
    uint32_t num = vstPos.size();

    double sum_x  = 0.0;
    double sum_y  = 0.0;
    double sum_x2 = 0.0;
    double sum_xy = 0.0;

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        sum_x  += stPos.x;
        sum_y  += stPos.y;
        sum_x2 += pow(stPos.x, 2);
        sum_xy += stPos.x * stPos.y;
    }

    sum_x  /= num;
    sum_y  /= num;
    sum_x2 /= num;
    sum_xy /= num;

    double a, b;
    a = (sum_xy - sum_x * sum_y) / (sum_x2 - sum_x * sum_x);
    b = (sum_x2 * sum_y - sum_x * sum_xy) / (sum_x2 - sum_x * sum_x);

    vCoeff.resize(2);
    vCoeff[0] = b;
    vCoeff[1] = a;
}

/**************************************************************************************
功能描述: 基于最小二乘法拟合2阶多项式函数: y = a*x^2 + b*x + c
           Fitting second-order polynomial by least square method
           a,b,c：返回拟合系数
修改记录:
**************************************************************************************/
void PCPT_CURVE_C::Fit2(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const
{
    uint32_t num = vstPos.size();

    double sum_x   = 0.0f;
    double sum_y   = 0.0f;
    double sum_x2  = 0.0f;
    double sum_x3  = 0.0f;
    double sum_x4  = 0.0f;
    double sum_xy  = 0.0f;
    double sum_x2y = 0.0f;

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        double x2 = pow(stPos.x, 2);
        double x3 = pow(stPos.x, 3);
        double x4 = pow(stPos.x, 4);

        sum_x   += stPos.x;
        sum_y   += stPos.y;
        sum_x2  += x2;
        sum_x3  += x3;
        sum_x4  += x4;
        sum_xy  += stPos.x * stPos.y;
        sum_x2y += x2 * stPos.y;
    }

    sum_x   /= num;
    sum_y   /= num;
    sum_x2  /= num;
    sum_x3  /= num;
    sum_x4  /= num;
    sum_xy  /= num;
    sum_x2y /= num;

    double a, b, c;
    b = ((sum_x * sum_y - sum_xy) / (sum_x3 - sum_x2 * sum_x) - (sum_x2 * sum_y - sum_x2y) / (sum_x4 - sum_x2 * sum_x2)) /
         ((sum_x3 - sum_x2 * sum_x) / (sum_x4 - sum_x2 * sum_x2) - (sum_x2 - sum_x * sum_x) / (sum_x3 - sum_x2 * sum_x));
    a = (sum_x2y - sum_x2 * sum_y - (sum_x3 - sum_x * sum_x2) * b) / (sum_x4 - sum_x2 * sum_x2);
    c = sum_y - sum_x2 * a - sum_x * b;

    vCoeff.resize(3);
    vCoeff[0] = c;
    vCoeff[1] = b;
    vCoeff[2] = a;
}

/**************************************************************************************
功能描述: 基于最小二乘法拟合3阶多项式函数: y = a*x^3 + b*x^2 + c*x + d
           Fitting third-order polynomial by least square method
           a,b,c,d：返回拟合系数
修改记录:
**************************************************************************************/
void PCPT_CURVE_C::Fit3(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const
{
    uint32_t num = vstPos.size();

    double sum_x   = 0;
    double sum_y   = 0;
    double sum_x2  = 0;
    double sum_x3  = 0;
    double sum_x4  = 0;
    double sum_x5  = 0;
    double sum_x6  = 0;
    double sum_xy  = 0;
    double sum_x2y = 0;
    double sum_x3y = 0;

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        double x2 = pow(stPos.x, 2);
        double x3 = pow(stPos.x, 3);
        double x4 = pow(stPos.x, 4);
        double x5 = pow(stPos.x, 5);
        double x6 = pow(stPos.x, 6);

        sum_x   += stPos.x;
        sum_y   += stPos.y;
        sum_x2  += x2;
        sum_x3  += x3;
        sum_x4  += x4;
        sum_x5  += x5;
        sum_x6  += x6;
        sum_xy  += stPos.x * stPos.y;
        sum_x2y += x2 * stPos.y;
        sum_x3y += x3 * stPos.y;
    }

    sum_x   /= num;
    sum_y   /= num;
    sum_x2  /= num;
    sum_x3  /= num;
    sum_x4  /= num;
    sum_x5  /= num;
    sum_x6  /= num;
    sum_xy  /= num;
    sum_x2y /= num;
    sum_x3y /= num;

    double m1 = (sum_x3 * sum_x6 - sum_x4 * sum_x5)   / (sum_x4 * sum_x6 - sum_x5 * sum_x5);
    double n1 = (sum_x2 * sum_x6 - sum_x3 * sum_x5)   / (sum_x4 * sum_x6 - sum_x5 * sum_x5);
    double k1 = (sum_x2y * sum_x6 - sum_x3y * sum_x5) / (sum_x4 * sum_x6 - sum_x5 * sum_x5);

    double m2 = (sum_x2 * sum_x6 - sum_x4 * sum_x4)   / (sum_x3 * sum_x6 - sum_x5 * sum_x4);
    double n2 = (sum_x * sum_x6 - sum_x3 * sum_x4)    / (sum_x3 * sum_x6 - sum_x5 * sum_x4);
    double k2 = (sum_xy * sum_x6 - sum_x3y * sum_x4)  / (sum_x3 * sum_x6 - sum_x5 * sum_x4);

    double m3 = (sum_x * sum_x6 - sum_x4 * sum_x3)    / (sum_x2 * sum_x6 - sum_x5 * sum_x3);
    double n3 = (sum_x6 - sum_x3 * sum_x3)            / (sum_x2 * sum_x6 - sum_x5 * sum_x3);
    double k3 = (sum_y * sum_x6 - sum_x3y * sum_x3)   / (sum_x2 * sum_x6 - sum_x5 * sum_x3);

    double a, b, c, d;
    d = ((k3 - k1) / (m3 - m1) - (k2 - k1) / (m2 - m1)) / ((n3 - n1) / (m3 - m1) - (n2 - n1) / (m2 - m1));
    c = (k2 - k1) / (m2 - m1) - (n2 - n1) / (m2 - m1) * d;
    b = k1 - m1 * c - n1 * d;
    a = sum_x3y / sum_x6 - sum_x5 / sum_x6 * b - sum_x4 / sum_x6 * c - sum_x3 / sum_x6 * d;

    vCoeff.resize(4);
    vCoeff[0] = d;
    vCoeff[1] = c;
    vCoeff[2] = b;
    vCoeff[3] = a;
}

/**************************************************************************************
功能描述: 基于最小二乘法拟合5阶多项式函数: y = a*x^4 + b*x^3 + c*x^2 + d*x + e
           Fitting fourth-order polynomial by least square method
           a,b,c,d,e：返回拟合系数
修改记录:
**************************************************************************************/
void PCPT_CURVE_C::Fit4(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const
{
    uint32_t num = vstPos.size();

    double sum_x   = 0;
    double sum_x2  = 0;
    double sum_x3  = 0;
    double sum_x4  = 0;
    double sum_x5  = 0;
    double sum_x6  = 0;
    double sum_x7  = 0;
    double sum_x8  = 0;
    double sum_y   = 0;
    double sum_xy  = 0;
    double sum_x2y = 0;
    double sum_x3y = 0;
    double sum_x4y = 0;

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        double x2 = pow(stPos.x, 2);
        double x3 = pow(stPos.x, 3);
        double x4 = pow(stPos.x, 4);
        double x5 = pow(stPos.x, 5);
        double x6 = pow(stPos.x, 6);
        double x7 = pow(stPos.x, 7);
        double x8 = pow(stPos.x, 8);

        sum_x   += stPos.x;
        sum_x2  += x2;
        sum_x3  += x3;
        sum_x4  += x4;
        sum_x5  += x5;
        sum_x6  += x6;
        sum_x7  += x7;
        sum_x8  += x8;
        sum_y   += stPos.y;
        sum_xy  += stPos.x * stPos.y;
        sum_x2y += x2 * stPos.y;
        sum_x3y += x3 * stPos.y;
        sum_x4y += x4 * stPos.y;
    }

    sum_x   /= num;
    sum_x2  /= num;
    sum_x3  /= num;
    sum_x4  /= num;
    sum_x5  /= num;
    sum_x6  /= num;
    sum_x7  /= num;
    sum_x8  /= num;
    sum_y   /= num;
    sum_xy  /= num;
    sum_x2y /= num;
    sum_x3y /= num;
    sum_x4y /= num;

    double m1 = (sum_x5 * sum_x8 - sum_x6 * sum_x7)   / (sum_x6 * sum_x8 - sum_x7 * sum_x7);
    double n1 = (sum_x4 * sum_x8 - sum_x5 * sum_x7)   / (sum_x6 * sum_x8 - sum_x7 * sum_x7);
    double j1 = (sum_x3 * sum_x8 - sum_x4 * sum_x7)   / (sum_x6 * sum_x8 - sum_x7 * sum_x7);
    double k1 = (sum_x3y * sum_x8 - sum_x4y * sum_x7) / (sum_x6 * sum_x8 - sum_x7 * sum_x7);

    double m2 = (sum_x4 * sum_x8 - sum_x6 * sum_x6)   / (sum_x5 * sum_x8 - sum_x7 * sum_x6);
    double n2 = (sum_x3 * sum_x8 - sum_x5 * sum_x6)   / (sum_x5 * sum_x8 - sum_x7 * sum_x6);
    double j2 = (sum_x2 * sum_x8 - sum_x4 * sum_x6)   / (sum_x5 * sum_x8 - sum_x7 * sum_x6);
    double k2 = (sum_x2y * sum_x8 - sum_x4y * sum_x6) / (sum_x5 * sum_x8 - sum_x7 * sum_x6);

    double m3 = (sum_x3 * sum_x8 - sum_x6 * sum_x5)   / (sum_x4 * sum_x8 - sum_x7 * sum_x5);
    double n3 = (sum_x2 * sum_x8 - sum_x5 * sum_x5)   / (sum_x4 * sum_x8 - sum_x7 * sum_x5);
    double j3 = (sum_x * sum_x8 - sum_x4 * sum_x5)    / (sum_x4 * sum_x8 - sum_x7 * sum_x5);
    double k3 = (sum_xy * sum_x8 - sum_x4y * sum_x5)  / (sum_x4 * sum_x8 - sum_x7 * sum_x5);

    double m4 = (sum_x2 * sum_x8 - sum_x6 * sum_x4)   / (sum_x3 * sum_x8 - sum_x7 * sum_x4);
    double n4 = (sum_x * sum_x8 - sum_x5 * sum_x4)    / (sum_x3 * sum_x8 - sum_x7 * sum_x4);
    double j4 = (sum_x8 - sum_x4 * sum_x4)            / (sum_x3 * sum_x8 - sum_x7 * sum_x4);
    double k4 = (sum_y * sum_x8 - sum_x4y * sum_x4)   / (sum_x3 * sum_x8 - sum_x7 * sum_x4);

    double o1 = (n2 - n1) / (m2 - m1);
    double p1 = (j2 - j1) / (m2 - m1);
    double q1 = (k2 - k1) / (m2 - m1);

    double o2 = (n3 - n1) / (m3 - m1);
    double p2 = (j3 - j1) / (m3 - m1);
    double q2 = (k3 - k1) / (m3 - m1);

    double o3 = (n4 - n1) / (m4 - m1);
    double p3 = (j4 - j1) / (m4 - m1);
    double q3 = (k4 - k1) / (m4 - m1);

    double r1 = (p2 - p1) / (o2 - o1);
    double s1 = (q2 - q1) / (o2 - o1);
    double r2 = (p3 - p1) / (o3 - o1);
    double s2 = (q3 - q1) / (o3 - o1);

    double a, b, c, d, e;
    e = (s2 - s1) / (r2 - r1);
    d = s1 - r1 * e;
    c = q1 - o1 * d - p1 * e;
    b = k1 - m1 * c - n1 * d - j1 * e;
    a = (sum_y - sum_x3 * b - sum_x2 * c - sum_x * d - e) / sum_x4;

    vCoeff.resize(5);
    vCoeff[0] = e;
    vCoeff[1] = d;
    vCoeff[2] = c;
    vCoeff[3] = b;
    vCoeff[4] = a;
}

/**************************************************************************************
功能描述: 基于最小二乘法拟合5阶多项式函数: y = a*x^5 + b*x^4 + c*x^3 + d*x^2 + e*x + f
           Fitting fourth-order polynomial by least square method
           a,b,c,d,e,f：返回拟合系数
修改记录:
**************************************************************************************/
void PCPT_CURVE_C::Fit5(const vector<COORDINATE_2D_STRU>& vstPos, vector<double>& vCoeff) const
{
    uint32_t num = vstPos.size();

    double sum_x   = 0;
    double sum_x2  = 0;
    double sum_x3  = 0;
    double sum_x4  = 0;
    double sum_x5  = 0;
    double sum_x6  = 0;
    double sum_x7  = 0;
    double sum_x8  = 0;
    double sum_x9  = 0;
    double sum_x10 = 0;
    double sum_y   = 0;
    double sum_xy  = 0;
    double sum_x2y = 0;
    double sum_x3y = 0;
    double sum_x4y = 0;
    double sum_x5y = 0;

    for (const COORDINATE_2D_STRU &stPos : vstPos) {
        double x2  = pow(stPos.x, 2);
        double x3  = pow(stPos.x, 3);
        double x4  = pow(stPos.x, 4);
        double x5  = pow(stPos.x, 5);
        double x6  = pow(stPos.x, 6);
        double x7  = pow(stPos.x, 7);
        double x8  = pow(stPos.x, 8);
        double x9  = pow(stPos.x, 9);
        double x10 = pow(stPos.x, 10);

        sum_x   += stPos.x;
        sum_x2  += x2;
        sum_x3  += x3;
        sum_x4  += x4;
        sum_x5  += x5;
        sum_x6  += x6;
        sum_x7  += x7;
        sum_x8  += x8;
        sum_x9  += x9;
        sum_x10 += x10;
        sum_y   += stPos.y;
        sum_xy  += stPos.x * stPos.y;
        sum_x2y += x2 * stPos.y;
        sum_x3y += x3 * stPos.y;
        sum_x4y += x4 * stPos.y;
        sum_x5y += x5 * stPos.y;
    }

    sum_x   /= num;
    sum_x2  /= num;
    sum_x3  /= num;
    sum_x4  /= num;
    sum_x5  /= num;
    sum_x6  /= num;
    sum_x7  /= num;
    sum_x8  /= num;
    sum_x9  /= num;
    sum_x10 /= num;
    sum_y   /= num;
    sum_xy  /= num;
    sum_x2y /= num;
    sum_x3y /= num;
    sum_x4y /= num;
    sum_x5y /= num;

    double m01 = (sum_x8 * sum_x10 - sum_x9 * sum_x9)   / (sum_x9 * sum_x10);
    double n01 = (sum_x7 * sum_x10 - sum_x8 * sum_x9)   / (sum_x9 * sum_x10);
    double i01 = (sum_x6 * sum_x10 - sum_x7 * sum_x9)   / (sum_x9 * sum_x10);
    double j01 = (sum_x5 * sum_x10 - sum_x6 * sum_x9)   / (sum_x9 * sum_x10);
    double k01 = (sum_x4 * sum_x10 - sum_x5 * sum_x9)   / (sum_x9 * sum_x10);
    double l01 = (sum_x4y * sum_x10 - sum_x5y * sum_x9) / (sum_x9 * sum_x10);

    double m02 = (sum_x7 * sum_x10 - sum_x9 * sum_x8)   / (sum_x8 * sum_x10);
    double n02 = (sum_x6 * sum_x10 - sum_x8 * sum_x8)   / (sum_x8 * sum_x10);
    double i02 = (sum_x5 * sum_x10 - sum_x7 * sum_x8)   / (sum_x8 * sum_x10);
    double j02 = (sum_x4 * sum_x10 - sum_x6 * sum_x8)   / (sum_x8 * sum_x10);
    double k02 = (sum_x3 * sum_x10 - sum_x5 * sum_x8)   / (sum_x8 * sum_x10);
    double l02 = (sum_x3y * sum_x10 - sum_x5y * sum_x8) / (sum_x8 * sum_x10);

    double m03 = (sum_x6 * sum_x10 - sum_x9 * sum_x7)   / (sum_x7 * sum_x10);
    double n03 = (sum_x5 * sum_x10 - sum_x8 * sum_x7)   / (sum_x7 * sum_x10);
    double i03 = (sum_x4 * sum_x10 - sum_x7 * sum_x7)   / (sum_x7 * sum_x10);
    double j03 = (sum_x3 * sum_x10 - sum_x6 * sum_x7)   / (sum_x7 * sum_x10);
    double k03 = (sum_x2 * sum_x10 - sum_x5 * sum_x7)   / (sum_x7 * sum_x10);
    double l03 = (sum_x2y * sum_x10 - sum_x5y * sum_x7) / (sum_x7 * sum_x10);

    double m04 = (sum_x5 * sum_x10 - sum_x9 * sum_x6)   / (sum_x6 * sum_x10);
    double n04 = (sum_x4 * sum_x10 - sum_x8 * sum_x6)   / (sum_x6 * sum_x10);
    double i04 = (sum_x3 * sum_x10 - sum_x7 * sum_x6)   / (sum_x6 * sum_x10);
    double j04 = (sum_x2 * sum_x10 - sum_x6 * sum_x6)   / (sum_x6 * sum_x10);
    double k04 = (sum_x * sum_x10 - sum_x5 * sum_x6)    / (sum_x6 * sum_x10);
    double l04 = (sum_xy * sum_x10 - sum_x5y * sum_x6)  / (sum_x6 * sum_x10);

    double m05 = (sum_x4 * sum_x10 - sum_x9 * sum_x5)   / (sum_x5 * sum_x10);
    double n05 = (sum_x3 * sum_x10 - sum_x8 * sum_x5)   / (sum_x5 * sum_x10);
    double i05 = (sum_x2 * sum_x10 - sum_x7 * sum_x5)   / (sum_x5 * sum_x10);
    double j05 = (sum_x * sum_x10 - sum_x6 * sum_x5)    / (sum_x5 * sum_x10);
    double k05 = (sum_x10 - sum_x5 * sum_x5)            / (sum_x5 * sum_x10);
    double l05 = (sum_y * sum_x10 - sum_x5y * sum_x5)   / (sum_x5 * sum_x10);

    double n11 = (m01 * n02 - n01 * m02) / (m01 * m02);
    double i11 = (m01 * i02 - i01 * m02) / (m01 * m02);
    double j11 = (m01 * j02 - j01 * m02) / (m01 * m02);
    double k11 = (m01 * k02 - k01 * m02) / (m01 * m02);
    double l11 = (m01 * l02 - l01 * m02) / (m01 * m02);

    double n12 = (m01 * n03 - n01 * m03) / (m01 * m03);
    double i12 = (m01 * i03 - i01 * m03) / (m01 * m03);
    double j12 = (m01 * j03 - j01 * m03) / (m01 * m03);
    double k12 = (m01 * k03 - k01 * m03) / (m01 * m03);
    double l12 = (m01 * l03 - l01 * m03) / (m01 * m03);

    double n13 = (m01 * n04 - n01 * m04) / (m01 * m04);
    double i13 = (m01 * i04 - i01 * m04) / (m01 * m04);
    double j13 = (m01 * j04 - j01 * m04) / (m01 * m04);
    double k13 = (m01 * k04 - k01 * m04) / (m01 * m04);
    double l13 = (m01 * l04 - l01 * m04) / (m01 * m04);

    double n14 = (m01 * n05 - n01 * m05) / (m01 * m05);
    double i14 = (m01 * i05 - i01 * m05) / (m01 * m05);
    double j14 = (m01 * j05 - j01 * m05) / (m01 * m05);
    double k14 = (m01 * k05 - k01 * m05) / (m01 * m05);
    double l14 = (m01 * l05 - l01 * m05) / (m01 * m05);

    double i21 = (n11 * i12 - i11 * n12) / (n11 * n12);
    double j21 = (n11 * j12 - j11 * n12) / (n11 * n12);
    double k21 = (n11 * k12 - k11 * n12) / (n11 * n12);
    double l21 = (n11 * l12 - l11 * n12) / (n11 * n12);

    double i22 = (n11 * i13 - i11 * n13) / (n11 * n13);
    double j22 = (n11 * j13 - j11 * n13) / (n11 * n13);
    double k22 = (n11 * k13 - k11 * n13) / (n11 * n13);
    double l22 = (n11 * l13 - l11 * n13) / (n11 * n13);

    double i23 = (n11 * i14 - i11 * n14) / (n11 * n14);
    double j23 = (n11 * j14 - j11 * n14) / (n11 * n14);
    double k23 = (n11 * k14 - k11 * n14) / (n11 * n14);
    double l23 = (n11 * l14 - l11 * n14) / (n11 * n14);

    double j31 = (i21 * j22 - j21 * i22) / (i21 * i22);
    double k31 = (i21 * k22 - k21 * i22) / (i21 * i22);
    double l31 = (i21 * l22 - l21 * i22) / (i21 * i22);

    double j32 = (i21 * j23 - j21 * i23) / (i21 * i23);
    double k32 = (i21 * k23 - k21 * i23) / (i21 * i23);
    double l32 = (i21 * l23 - l21 * i23) / (i21 * i23);

    double k33 = (j31 * k32 - k31 * j32) / (j31 * j32);
    double l33 = (j31 * l32 - l31 * j32) / (j31 * j32);

    double a, b, c, d, e, f;
    f = l33 / k33;
    e = (l32 - k32 * f) / j32;
    d = (l23 - k23 * f - j23 * e) / i23;
    c = (l14 - k14 * f - j14 * e - i14 * d) / n14;
    b = (l05 - k05 * f - j05 * e - i05 * d - n05 * c) / m05;
    a = (sum_y - f - sum_x * e - sum_x2 * d - sum_x3 * c - sum_x4 * b) / sum_x5;

    vCoeff.resize(6);
    vCoeff[0] = f;
    vCoeff[1] = e;
    vCoeff[2] = d;
    vCoeff[3] = c;
    vCoeff[4] = b;
    vCoeff[5] = a;
}

/**************************************************************************************
功能描述: 根据x获取多阶曲线的斜率
修改记录:
**************************************************************************************/
double PCPT_CURVE_C::GetMultiOrderCurveSlope(const double x, vector<double>& vCoeff) const
{
    double slope = 0;

    for (uint32_t i = 1; i < vCoeff.size(); ++i)
        slope += i * vCoeff[i] * pow(x, i-1);

    return slope;
}

/**************************************************************************************
功能描述: 根据x获取多阶曲线的y值
修改记录:
**************************************************************************************/
double PCPT_CURVE_C::GetMultiOrderCurveY(const double x, vector<double>& vCoeff) const
{
    double y = 0;

    for (uint32_t i = 0; i < vCoeff.size(); ++i)
        y += vCoeff[i] * pow(x, i);

    return y;
}

/**************************************************************************************
功能描述: 获取圆上某点处的斜率
修改记录:
**************************************************************************************/
double PCPT_CURVE_C::GetCircleSlope(const double x, double y, vector<double>& vCoeff) const
{
    return (x - vCoeff[0]) / (vCoeff[1] - y);
}

/**************************************************************************************
功能描述: 根据直线方程系数，获取直线上的两点
修改记录:
**************************************************************************************/
void PCPT_CURVE_C::GetLineEndPointsByCoeff(const float coeffA, const float coeffB,
                                           COORDINATE_2D_STRU& lineStart,
                                           COORDINATE_2D_STRU& lineEnd) const
{

    /* 直线方程: y = coeffA * x + coeffB */
    if (SIG(coeffA) == 0) {
        lineStart.x = 0;
        lineStart.y = coeffB;
        lineEnd.x   = 1;
        lineEnd.y   = coeffB;
    } else {
        lineStart.x = 0;
        lineStart.y = coeffB;
        lineEnd.x   = -coeffB/coeffA;
        lineEnd.y   = 0;
    }
}

/**************************************************************************************
功能描述: 点到直线的垂足点
修改记录:
**************************************************************************************/
COORDINATE_2D_STRU PCPT_CURVE_C::PointToLineFootPoint(const COORDINATE_2D_STRU& point, const COORDINATE_2D_STRU& lineStart, const COORDINATE_2D_STRU& lineEnd) const
{
    double lengthSqu = lineEnd.CalcDistSqu(lineStart);
    if (SIG(lengthSqu) == 0) return COORDINATE_2D_STRU(lineStart.x, lineStart.y); //如果直线退化为一个点

    /* 计算直线法线的单位向量 */
    double u = ((lineEnd.x - lineStart.x) * (point.x - lineStart.x) + (lineEnd.y - lineStart.y) * (point.y - lineStart.y)) / lengthSqu;

    COORDINATE_2D_STRU stCoor;
    stCoor.x = lineStart.x + u * (lineEnd.x - lineStart.x);
    stCoor.y = lineStart.y + u * (lineEnd.y - lineStart.y);
    return stCoor;
}

COORDINATE_2D_STRU PCPT_CURVE_C::PointToLineFootPoint(const COORDINATE_2D_STRU& point, const float coeffA, const float coeffB) const
{
    /* 直线方程: y = coeffA * x + coeffB */
    COORDINATE_2D_STRU lineStart, lineEnd;
    GetLineEndPointsByCoeff(coeffA, coeffB, lineStart, lineEnd);
    return PointToLineFootPoint(point, lineStart, lineEnd);
}

/**************************************************************************************
功能描述: 点到直线的距离
修改记录:
**************************************************************************************/
double PCPT_CURVE_C::PointToLineDistance(const COORDINATE_2D_STRU& point, const COORDINATE_2D_STRU& lineStart, const COORDINATE_2D_STRU& lineEnd) const
{
    COORDINATE_2D_STRU stCoor = PointToLineFootPoint(point, lineStart, lineEnd);    //垂足
    return sqrt(stCoor.CalcDistSqu(point)); //返回点到直线的最短距离
}

double PCPT_CURVE_C::PointToLineDistance(const COORDINATE_2D_STRU& point, const float coeffA, const float coeffB) const
{
    COORDINATE_2D_STRU lineStart, lineEnd;
    GetLineEndPointsByCoeff(coeffA, coeffB, lineStart, lineEnd);
    return PointToLineDistance(point, lineStart, lineEnd);
}

/**************************************************************************************
功能描述: 点到线段的最小距离
修改记录:
**************************************************************************************/
double PCPT_CURVE_C::PointToLineSegmentDistance(const COORDINATE_2D_STRU& point, const COORDINATE_2D_STRU& lineStart, const COORDINATE_2D_STRU& lineEnd) const
{
    COORDINATE_2D_STRU stCoor = PointToLineFootPoint(point, lineStart, lineEnd);    //垂足

    /* 垂足在线段断点之间，返回点到直线的最短距离 */
    double lengthSqu = lineEnd.CalcDistSqu(lineStart);
    if (stCoor.CalcDistSqu(lineStart) < lengthSqu && stCoor.CalcDistSqu(lineEnd) < lengthSqu) return sqrt(stCoor.CalcDistSqu(point));

    /* 垂足不在线段断点之间，返回点到断点的最小距离 */
    return MIN(sqrt(lineStart.CalcDistSqu(point)), sqrt(lineEnd.CalcDistSqu(point)));
}

