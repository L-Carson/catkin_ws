#include "pcpt_det_dynamic/pcpt_trajectory_predictor.h"

INT32 TRAJECTORY_PREDICTOR_C::GetPredictTraj(const OBJECT_TRACK_STRU& stObjTrack,
                                             std::vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory)
{
    /* 判断历史轨迹，是否足以用来预测未来轨迹*/
    if (IsOkTrackTraj(stObjTrack)) return -1;

    /* 拟合历史轨迹 */
    std::vector<double> vCoeff;
    if (0 != FittingTraj(stObjTrack.lstTrajectory, stObjTrack.stBox.enClass, vCoeff)) return -1;

    /* 预测轨迹，预测轨迹不包含当前位置 */
    PredictTrajectory(stObjTrack.stPosVel, stObjTrack.stBox.timeStamp, vCoeff, vstPredTrajectory);

    return 0;
}

BOOL  TRAJECTORY_PREDICTOR_C::IsOkTrackTraj(const OBJECT_TRACK_STRU& stObjTrack)
{
    /* 历史轨迹首尾距离是否足够长,  太短适合后续拟合 */
    if (stObjTrack.lstTrajectory.size() < 2) return false;
    const TRAJECTORY_POINT_STRU& stFrontPoint = stObjTrack.lstTrajectory.front();
    const TRAJECTORY_POINT_STRU& stBackPoint  = stObjTrack.lstTrajectory.back();
    if (stBackPoint.CalcDistSqu(stFrontPoint) < pow(1.5, 2)) return false;

    /* 历史轨迹点太少，则不进行拟合预测 */
    // TODO 需优化
    if (stObjTrack.lstTrajectory.size() < 2) return false;

    return true;
}

INT32 TRAJECTORY_PREDICTOR_C::FittingTraj(const std::list<TRAJECTORY_POINT_STRU>& lstTrajectory,
                                          const LABEL_ENUM enClass,
                                          std::vector<double>& vCoeff)
{
    const TRAJECTORY_POINT_STRU& stBackPoint  = lstTrajectory.back();

    /* 整理待拟合数据 */
    LST_COOR lstPos;
    for (auto iter = lstTrajectory.rbegin(); iter != lstTrajectory.rend(); ++iter) {
        /**
         * 在每个目标的跟踪器中缓存了3秒的轨迹数据，
         * 此处使用最近1秒内的数据进行拟合，
         * 可改为根据目标类型选择适当的时间长度
         */
        if (stBackPoint.DiffStamp(*iter) < 1.0 || lstPos.size() < 5)
            lstPos.push_front(COORDINATE_2D_STRU(iter->x, iter->y));
    }

    /* 平滑处理 */
    std::vector<COORDINATE_2D_STRU> vstPos, vstPosSmooth;
    vstPos.insert(vstPos.end(), lstPos.begin(), lstPos.end());
    if (0 == SmoothFilter(5, vstPos, vstPosSmooth))   vstPos = vstPosSmooth;
    if (0 == TrajOptimize(vstPos, 0.5, vstPosSmooth)) vstPos = vstPosSmooth;

    /* 不同类别使用不同阶数的拟合曲线 */
    switch(enClass) {
        case TAG_UNKNOWN:
        case TAG_PEDESTRIAN:
            //if (objFitCurve_.FitCircle(vstPos, vCoeff)) return -1;
            if (objFitCurve_.FitMultiOrderCurveE(vstPos, 1, vCoeff)) return -1;
            break;
        case TAG_CAR:
        case TAG_CYCLIST:
            //if (objFitCurve_.FitMultiOrderCurveE(vstPos, 3, vCoeff)) return -1;
            if (objFitCurve_.FitCircle(vstPos, vCoeff)) return -1;
            break;
        default:
            break;
    }

    return 0;
}

INT32 TRAJECTORY_PREDICTOR_C::SmoothFilter(UINT32 halfWinSize, std::vector<COORDINATE_2D_STRU>& vstPosIn,
                                           std::vector<COORDINATE_2D_STRU>& vstPosOut)
{
    uint32_t winSize = 2 * halfWinSize + 1;
    if (vstPosIn.size() < winSize) return -1;

    vstPosOut.resize(vstPosIn.size());

    bool hasAddFullWin = false;
    double addX = 0, addY = 0;

    for (uint32_t i = 0; i < vstPosIn.size(); ++i) {
        if (i < halfWinSize || i > vstPosIn.size() - 1 - halfWinSize) {
            vstPosOut[i] = vstPosIn[i];
            continue;
        }

        COORDINATE_2D_STRU stPos;

        uint32_t winStar = i - halfWinSize;
        uint32_t winEnd  = i + halfWinSize;

        if (!hasAddFullWin) {
            hasAddFullWin = true;

            for (uint32_t j = winStar; j <= winEnd; ++j) {
                addX += vstPosIn[j].x;
                addY += vstPosIn[j].y;
            }

            vstPosOut[i].x = addX / winSize;
            vstPosOut[i].y = addY / winSize;

            addX -= vstPosIn[winStar].x;
            addY -= vstPosIn[winStar].y;
        } else {
            addX += vstPosIn[winEnd].x;
            addY += vstPosIn[winEnd].y;

            vstPosOut[i].x = addX / winSize;
            vstPosOut[i].y = addY / winSize;

            addX -= vstPosIn[winStar].x;
            addY -= vstPosIn[winStar].y;
        }
    }
    return 0;
}

INT32 TRAJECTORY_PREDICTOR_C::TrajOptimize(const std::vector<COORDINATE_2D_STRU>& vstPosIn, const double distInterval,
                                  std::vector<COORDINATE_2D_STRU>& vstPosOut)
{
    if (vstPosIn.size() < 2) return -1;

    std::vector<COORDINATE_2D_STRU> vstTempPoints;
    vstTempPoints.emplace_back(vstPosIn[0]);
    for (size_t i = 1; i < vstPosIn.size(); i++) {
        float dx = vstPosIn[i].x - vstTempPoints.back().x;
        float dy = vstPosIn[i].y - vstTempPoints.back().y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < 0.05) continue;
        if (dist < 0.1) {
            vstTempPoints.emplace_back(vstPosIn[i]);
            continue;
        }

        float dirAngle = atan2(dy, dx);
        Eigen::Vector2f direct = Eigen::Vector2f(cos(dirAngle), sin(dirAngle));
        Eigen::Vector2f step   = 0.1 * direct;

        Eigen::Vector2f newPos = Eigen::Vector2f(vstTempPoints.back().x, vstTempPoints.back().y) + step;
        BOOL   isCompareByX             = fabs(step(0)) > fabs(step(1));
        const float &addPointCompare    = isCompareByX ? newPos(0) : newPos(1);
        const float &lmtPointCompare    = isCompareByX ? vstPosIn[i].x : vstPosIn[i].y;
        BOOL   isCompareDeltaPositive   = isCompareByX ? (step(0) > 0) : (step(1) > 0);
        do {
            COORDINATE_2D_STRU stNewPos(newPos(0), newPos(1));
            vstTempPoints.emplace_back(stNewPos);
            newPos += step;
        } while (isCompareDeltaPositive == (addPointCompare < lmtPointCompare));
    }

    vstPosOut.clear();
    vstPosOut.emplace_back(vstTempPoints[0]);
    float accDist = 0.0;
    for (size_t i = 1; i < vstTempPoints.size(); i++) {
        accDist += sqrt(vstTempPoints[i].CalcDistSqu(vstTempPoints[i-1]));
        if (accDist <= distInterval) continue;
        vstPosOut.emplace_back(vstTempPoints[i]);
        accDist = 0.0;
    }

    return 0;
}

void  TRAJECTORY_PREDICTOR_C::PredictTrajectory(const POS_VEL_STRU& stPosVel, double timeStamp, vector<double>& vCoeff, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory)
{
    //if (vCoeff.size() == 4 && vCoeff.back() == 0)
    if (1)
        PredictTrajectoryByCircle(stPosVel, timeStamp, vCoeff, vstPredTrajectory);
    else
        PredictTrajectoryByMultiOrderCurve(stPosVel, timeStamp, vCoeff, vstPredTrajectory);
}

void  TRAJECTORY_PREDICTOR_C::PredictTrajectoryByMultiOrderCurve(const POS_VEL_STRU& stPosVel, double timeStamp, vector<double>& vCoeff, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory)
{
    float vel = sqrt(pow(stPosVel.velX, 2) + pow(stPosVel.velY, 2));    //当前速度，也可以表示以当前速度为参考，1s移动的距离
    float predPointsInterval = PredDurationTime_ / predPointsNum_ * vel; //相邻预测点的间距

    vstPredTrajectory.reserve(predPointsNum_);

    vector<double> vecPre(2);   //方向向量
    vector<double> vecCur(2);   //方向向量
    vecPre[0] = stPosVel.velX;  //用速度初始化向量
    vecPre[1] = stPosVel.velY;

    double px = stPosVel.posX;
    double py = stPosVel.posY;

    bool isAlign = false;
    int  count = 0;

    while (count < predPointsNum_) {
        float totalStride = 0;
        double slope = 0;
        while (totalStride < predPointsInterval) {
            slope = objFitCurve_.GetMultiOrderCurveSlope(px, vCoeff);    //斜率
            vecCur[0] = 1;      //x
            vecCur[1] = slope;  //y  slope=y/x;

            //每次步进迭代，两相邻向量的夹角确保是锐角
            if (vecCur[0] * vecPre[0] + vecCur[1] * vecPre[1] < 0) {
                vecCur[0] *= -1;
                vecCur[1] *= -1;
            }
            vecPre = vecCur;

            double pxs = px;
            double pys = objFitCurve_.GetMultiOrderCurveY(pxs, vCoeff);

            if (!isAlign) {
                float diffY = pys - py;
                vCoeff[0] -= diffY; //沿Y轴平移，使曲线经过当前所在的位置
                isAlign = true;
            }

            pxs = 0.02 / fabs(slope) * vecCur[0] + px;  //沿直线(斜率slope)的y轴方向，每次步进0.02m
            pys = objFitCurve_.GetMultiOrderCurveY(pxs, vCoeff);

            totalStride += sqrt(pow(pxs - px, 2) + pow(pys - py, 2));

            px = pxs;
            py = pys;
        }

        timeStamp += totalStride / vel;
        count++;

        TRAJECTORY_POINT_STRU stTrajectory;
        stTrajectory.timeStamp = timeStamp;
        stTrajectory.x = px;
        stTrajectory.y = py;
        stTrajectory.angle = atan2(slope * SIGN(vecCur[1]), SIGN(vecCur[0]));
        vstPredTrajectory.push_back(stTrajectory);
    }
}

void  TRAJECTORY_PREDICTOR_C::PredictTrajectoryByCircle(const POS_VEL_STRU& stPosVel, double timeStamp, vector<double>& vCoeff, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory)
{
    double centerX = vCoeff[0];
    double centerY = vCoeff[1];
    double radius  = vCoeff[2];

    /* 按直线预测 */
    if (radius < 100000) {
        PredictTrajectoryByVel(stPosVel, timeStamp, vstPredTrajectory);
        return;
    }

    float length = sqrt(pow(centerX - stPosVel.posX, 2) + pow(centerY - stPosVel.posY, 2));
    float ratio = (length - radius) / length;

    centerX = centerX + (stPosVel.posX - centerX) * ratio;  //平移圆心，使圆经过当前位置
    centerY = centerY + (stPosVel.posY - centerY) * ratio;

    double dt = PredDurationTime_ / predPointsNum_;
    float  dTheta = sqrt(pow(stPosVel.velX, 2) + pow(stPosVel.velY, 2)) * dt / radius;
    float  curTheta = atan2(stPosVel.posY - centerY, stPosVel.posX - centerX);
    float  crossProductZ = stPosVel.velX * (centerY - stPosVel.posY) - stPosVel.velY * (centerX - stPosVel.posX);

    vstPredTrajectory.reserve(predPointsNum_);
    for (int i = 0; i < predPointsNum_; ++i) {
        curTheta += SIGN(crossProductZ) * dTheta;

        timeStamp += dt;

        TRAJECTORY_POINT_STRU stPredTrajectory;
        stPredTrajectory.timeStamp = timeStamp;
        stPredTrajectory.x = centerX + radius * cos(curTheta);
        stPredTrajectory.y = centerY + radius * sin(curTheta);
        double slope = -1 / tan(curTheta);     //圆上一点斜率
        stPredTrajectory.angle = atan2(slope * SIGN(crossProductZ), SIGN(crossProductZ));
        vstPredTrajectory.push_back(stPredTrajectory);
    }
}

void  TRAJECTORY_PREDICTOR_C::PredictTrajectoryByVel(const POS_VEL_STRU& stPosVel, double timeStamp, vector<TRAJECTORY_POINT_STRU>& vstPredTrajectory)
{
    double dt = PredDurationTime_ / predPointsNum_;
    float  dx = stPosVel.velX * dt;
    float  dy = stPosVel.velY * dt;

    TRAJECTORY_POINT_STRU stTrajectory;
    stTrajectory.timeStamp = timeStamp;
    stTrajectory.x = stPosVel.posX;
    stTrajectory.y = stPosVel.posY;
    stTrajectory.angle = atan2(stPosVel.velY, stPosVel.velX);

    vstPredTrajectory.reserve(predPointsNum_);
    for (int i = 0; i < predPointsNum_; ++i) {
        stTrajectory.timeStamp += dt;
        stTrajectory.x += dx;
        stTrajectory.y += dy;
        vstPredTrajectory.push_back(stTrajectory);
    }
}

