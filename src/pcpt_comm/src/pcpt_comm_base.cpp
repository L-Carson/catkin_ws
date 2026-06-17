#include "pcpt_comm/pcpt_comm_base.h"

std::atomic_int     g_move_trend{0};                                    //运动趋势
std::atomic_bool    g_detect_garbage{false};                            //是否开启垃圾检测
std::atomic_bool    g_contact_edge{false};                              //触边生成障碍

std::atomic_bool    g_butt_dustbin{false};                              //是否对接垃圾箱
std::atomic_bool    g_dustbin_state{false};                             //是否检测垃圾箱容量状态
std::atomic_bool    g_butt_charging_pile{false};                        //是否对接充电桩
std::atomic_bool    g_butt_fixed_site{false};                           //是否对接固定点（固定点倒垃圾）

std::atomic_int     g_blindLidarType{int(LIDAR_TYPE_ENUM::TYPE_NULL)};  //补盲雷达类型
std::atomic_ullong  g_lidar_stamp{0};                                   //雷达时间戳
std::atomic_bool    g_labelPile{false};                                 //是否标记充电桩
std::atomic_bool    g_isWork{false};                                    //是否工作
std::atomic_bool    g_isSim{false};                                     //是否仿真
std::atomic_bool    g_isRestartedPnp{false};                            //是否重启了感知（规控）

double              g_gravity{9.80};                                    //重力加速度
POINT_TPYE_ENUM     g_pointType = POINT_TPYE_ENUM::TYPE_NAN;            //点云数据类型

std::atomic_uchar   g_v_det_pipeline_state{uint8_t(LABEL_STATE_NO)};    //图像检测 - 水管管线存在状态
std::atomic_uchar   g_v_seg_pipeline_state{uint8_t(LABEL_STATE_NO)};    //图像分割 - 水管管线存在状态
std::atomic_bool    g_v_has_risk{false};                                //当前图像检测是否存在风险

double    LidarStamp() {return ZERO_6 * g_lidar_stamp;}

double    PclStamp(const pcl::PCLHeader &header) {return header.stamp * ZERO_6;}
uint64_t  PclStampInt(const pcl::PCLHeader &header) {return header.stamp;}

uint64_t  RosStampInt(const std_msgs::Header& header) {return header.stamp.toSec() * TEN_6;}
double    RosStamp(const ros::Time stamp) {return stamp.toSec();}
double    RosStamp(const std_msgs::Header& header) {return header.stamp.toSec();}
double    RosStampNow() {return ros::Time::now().toSec();}

ros::Time RosTime(const double stamp) {return ros::Time().fromSec(stamp);}
ros::Time RosTimeNow() {return ros::Time::now();}

std_msgs::Header RosHeader(const char* frame_id)
{
    std_msgs::Header header;
    header.stamp = ros::Time::now();
    header.frame_id = frame_id;
    return header;
}

void PrintCostTime(const std::vector<double> &vecTimes, const std::string &strMark, const float printThr)
{
    if (vecTimes.size() < 2) return;

    INT32 lastIdx = vecTimes.size() - 1;
    double allCost = vecTimes[lastIdx] - vecTimes[0];
    if (allCost > printThr){
        std::stringstream objSS;
        objSS << strMark <<" allcost:"<<allCost * 1000;

        for (UINT32 endIdx = 1; endIdx < vecTimes.size(); ++endIdx) {
            int beginIdx = endIdx - 1;
            double timeCost = (vecTimes[endIdx] - vecTimes[beginIdx]) * 1000;
            objSS << "step[" << beginIdx << "-" << endIdx << "][" << timeCost << "] ";
        }
        ST_LOG_INFO("%s", objSS.str().data());
    }

}

void PrintCostTime(const std::vector<std::chrono::time_point<std::chrono::steady_clock> > &vecTimes, 
                   const std::string &strMark, const float printThr)
{
    if (vecTimes.size() < 2) return;
    INT32 lastIdx = vecTimes.size() - 1;
    double allCost = std::chrono::duration_cast<std::chrono::microseconds>((vecTimes[lastIdx] - vecTimes[0])).count() / 1000.0;
    if (allCost > printThr){
        std::stringstream objSS;
        objSS << strMark << " allcost:"<<allCost;
        for (UINT32 endIdx = 1; endIdx < vecTimes.size(); ++endIdx) {
            int beginIdx = endIdx - 1;
            double timeCost = std::chrono::duration_cast<std::chrono::microseconds>((vecTimes[endIdx] - vecTimes[beginIdx])).count() / 1000.0;
            objSS << "step[" << beginIdx << "-" << endIdx << "][" << timeCost << "] ";
        }
        ST_LOG_INFO("%s", objSS.str().data());
    }
}
