#include "pcpt_det_dynamic/pcpt_det_dynamic.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "det_dynamic_node");
    ros::NodeHandle nh;

    INT32 runFreq = 20;
    if (0 != Comm_Init(runFreq, 1.0)) {
        ST_LOG_ERR("Comm_Init函数执行失败.");
        return -1;
    } 

    DET_DYNAMIC_C objDetDynamic(nh);
    INT32 rslt = objDetDynamic.Init();
    if (rslt != 0) {
        ST_LOG_ERR("objDetDynamic.Init() Fail.");
        return -1;
    }
    ST_LOG_INFO("objDetDynamic.Init() Succ.");
    
    /* 开始工作 */
    ros::Rate objLoopRate(runFreq);
    while (ros::ok()) {
        Comm_UptByPeriod();
        objDetDynamic.PeriodUpt();
    
        ros::spinOnce();
        objLoopRate.sleep();
    }
    ST_LOG_INFO("End.");
    
    return EXIT_SUCCESS;
}


