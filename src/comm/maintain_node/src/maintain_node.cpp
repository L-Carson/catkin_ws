
#include "maintain_param.h"
#include "comm/comm_sys.h"
#include "comm/comm_ros_trc.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "maintain_node");
    ros::NodeHandle nh;
    INT32           runFrequency = 10;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    SYS_INFO_RECORDER_C objSysInfoRecorder;
    MAINTAIN_C          objMaintain(nh);
    ROUTES_SAVER_C      objRouteSaver(nh);

    ros::Rate loop_rate(runFrequency);
    while (ros::ok())
    {
        ros::spinOnce();
        Comm_UptByPeriod();

        objSysInfoRecorder.PeriodUpt();
        objRouteSaver.PeriodUpt();

        loop_rate.sleep();
    }

    return 0 ;
}


