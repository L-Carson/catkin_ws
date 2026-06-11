#include "comm/comm_time.h"
#include "comm/comm_map.h"
#include "comm/comm_base.h"
#include "comm/comm_ros_base.h"
#include "comm/comm_msg_init_pose.h"
#include "comm/comm_msg_goal_pose.h"
#include "comm/comm_msg_tool_point.h"

#include <ros/ros.h>

#define TOPIC_NAME_SHOW_POLYGON                     "/test/show/polygon"
#define TOPIC_NAME_SHOW_MAP                         "/test/show/map"


namespace TEST
{

void Test_GetTimeCost(ros::NodeHandle &nh, INT32 runFrequency)
{
    UINT32 testTimes = 100000;
    double beginTestTime = Comm_GetTimeSec();
    double tmp;
    for (UINT32 loop = 0; loop < testTimes; ++loop) {
        tmp = Comm_GetTimeSec();
    }

    double endTestTime = Comm_GetTimeSec();
    float  timeCostPer1000 = float((endTestTime - beginTestTime) / 100.0);
    ST_LOG_INFO("timeCostPer1000=%.6fs.", timeCostPer1000);     /* 0.168ms */
    ST_LOG_INFO("Not Use. %lf.", tmp);
}

CHAR *p;
CHAR setx = 0x00;


void Test_AllocBigMem(ros::NodeHandle &nh, INT32 runFrequency, INT32 inTestTimes)
{
    usleep(10 * 1000 * 1000);

    ST_LOG_INFO("begin big");

    /* 分别申请大内存和小内存，查看效率对比 */
    UINT32 testTimes     = UINT32(inTestTimes);
    UINT32 bigMemSize    = 10 * 1024 * 1024;

    double beginTestTime = Comm_GetTimeSec();

    for (UINT32 loop = 0; loop < testTimes; ++loop) {
        p = new CHAR[bigMemSize];
        for (UINT32 w = 0; w < bigMemSize; w += 1024) {
            *(p + w) = setx;
        }

        //memset(p, setx, bigMemSize);
        delete[] p;

        p = nullptr;

        ++setx;
    }

    double endTestTime = Comm_GetTimeSec();
    ST_LOG_INFO("%u Times Alloc Big Mem Spent Time: %.3fms.", testTimes, (endTestTime - beginTestTime) * 1000.0);

    usleep(100 * 1000 * 1000);
/*
    UINT32 smallMemSize = 64 * 1024;
    UINT32 mutiTimes    = bigMemSize / smallMemSize;

    beginTestTime = Comm_GetTimeSec();
    for (UINT32 loop = 0; loop < testTimes; ++loop) {
        for (UINT32 loop1 = 0; loop1 < mutiTimes; ++loop1) {
            CHAR *p = new CHAR[smallMemSize];
            for (UINT32 w = 0; w < smallMemSize; w += 1024) {
                *(p + w) = 0;
            }
            //memset(p, CHAR(0x00), smallMemSize);
            delete[] p;
        }
    }

    endTestTime = Comm_GetTimeSec();
    ST_LOG_INFO("%u Times Alloc Small Mem Spent Time: %.3fms.", testTimes, (endTestTime - beginTestTime) * 1000.0);
*/
}

}

int main(int argc, char** argv)
{
    if (argc < 2) {
        ST_LOG_ERR("Para Num(%d) Err.", argc);
        return 1;
    }

    INT32 ctrlCmdNo;
    if (1 != sscanf(argv[1], "%d", &ctrlCmdNo)) {
        ST_LOG_ERR("Cmd(%s) Is Not Digital.", argv[1]);
        return 1;
    }

    INT32 otherParam = 0;
    if (argc > 2) {
        if (1 != sscanf(argv[2], "%d", &otherParam)) {
            ST_LOG_ERR("Cmd(%s) Is Not Digital.", argv[2]);
            return 1;
        }
    }

    ros::init(argc, argv, "test_time");
    ros::NodeHandle nh;
    INT32           runFrequency = 20;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    switch (ctrlCmdNo) {
        case 0:
            TEST::Test_GetTimeCost(nh, runFrequency);
            break;

        case 1:
            TEST::Test_AllocBigMem(nh, runFrequency, otherParam);
            break;

        default:
            break;
    }



    return 0;
}



