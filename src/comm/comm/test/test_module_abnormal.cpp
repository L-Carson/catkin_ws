
#include "comm/comm_module_abnormal.h"
#include "comm/comm_base.h"
#include "comm/comm_test_base.h"

#include <ros/ros.h>


namespace TEST
{


}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_module_abnormal");
    ros::NodeHandle nh;
    int             runFrequency = 1;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    COMM::MODULE_ABNORMAL_PUBLISHER_C objModuleAbnormalPublisher(nh, COMM::ABNORMAL_MODULE_MAIN_LIDAR);

    CHAR inputCtrl = 1;

    ros::Rate loop_rate(runFrequency);
    while(ros::ok())
    {
        Comm_UptByPeriod();
        ros::spinOnce();

        CHAR inputKey = GetInputKey();
        if (inputKey != 0) {
            inputCtrl = inputKey;
        }

        switch (inputCtrl) {
            case '1':
                objModuleAbnormalPublisher.SendAbnormalInfo(0);
                break;

            case '2':
                objModuleAbnormalPublisher.SendAbnormalInfo(1);
                break;

            default:
                break;
        }

        loop_rate.sleep();
    }

    return 0;
}



