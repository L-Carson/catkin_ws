
#include "comm/comm_base.h"
#include "comm/comm_topic_name.h"
#include "comm_srvs/lidarCalibration.h"

#include <ros/ros.h>


namespace TEST
{



}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_lidar_clbt");
    ros::NodeHandle nh;
    INT32           runFrequency = 20;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    ros::ServiceClient objSrvsClint = nh.serviceClient<comm_srvs::lidarCalibration>(TOPIC_NAME_SERVER_CLBT_LIDAR_PARAM);
    ST_LOG_INFO("Begin Waiting.");

    UINT32 waitTime           = 0;
    BOOL   isServiceAvailable = false;
    while (ros::ok() && !isServiceAvailable)
    {
        ++waitTime;

        isServiceAvailable = objSrvsClint.waitForExistence(ros::Duration(1.0));
        ST_LOG_ERR_SAMPLE_IF_TRUE(!isServiceAvailable && (waitTime >= 10), 10, "Waiting %us. Service Not Available Now.", waitTime);
    }

    if (!isServiceAvailable) {
        ST_LOG_ERR("Waiting End With Exception.");
        return -1;
    }

    comm_srvs::lidarCalibration::Request stReq;

    comm_srvs::LidarInstallParam stLidar1;
    stLidar1.name      = "MainLidar";    // MainLidar BlindLidar BlindLidar2
    stLidar1.x         = 100.1;
    stLidar1.y         = 100.2;
    stLidar1.z         = 100.3;
    stLidar1.roll      = 1.111;
    stLidar1.pitch     = 1.222;
    stLidar1.yaw       = 1.333;
    stReq.lidarsClbtParam.emplace_back(stLidar1);

    comm_srvs::LidarInstallParam stLidar2;
    stLidar2.name      = "BlindLidar";    // MainLidar BlindLidar BlindLidar2
    stLidar2.x         = 200.1;
    stLidar2.y         = 200.2;
    stLidar2.z         = 200.3;
    stLidar2.roll      = 2.111;
    stLidar2.pitch     = 2.222;
    stLidar2.yaw       = 2.333;
    stReq.lidarsClbtParam.emplace_back(stLidar2);

    comm_srvs::lidarCalibration::Response stRsp;
    BOOL isCallSucc = objSrvsClint.call(stReq, stRsp);     /* 阻塞式，无限时间等待响应 */
    if (!isCallSucc) {
        ST_LOG_ERR("Call Service Fail.");
        return -1;
    }

    ST_LOG_INFO("Call Service Succ.");

    if (!stRsp.success) {
        ST_LOG_ERR("Service Proc Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}



