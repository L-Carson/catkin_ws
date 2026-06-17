
#include "pcpt_comm/pcpt_comm_data_collection.h"
#include "comm/comm_base.h"
#include "basic_manager.h"
#include <ros/ros.h>

namespace TEST
{
namespace DATA_COLLECTION
{




class COLLECT_TXT_C : public DATA_COLLECTION_BASE_C
{
public:
    COLLECT_TXT_C(void) : DATA_COLLECTION_BASE_C("test")
    {
    }

    void Write(void)
    {
        std::string strDataDir;
        INT32 rslt = GetDataDir(strDataDir);
        if (rslt != 0) {
            ST_LOG_ERR("GetDataDir() Fail.");
            return;
        }

        std::string strTimeStamp;
        rslt = GetTimeStr(ros::Time::now().toSec(), strTimeStamp);
        if (rslt != 0) {
            ST_LOG_ERR("GetTimeStr() Fail.");
            return;
        }

        ST_LOG_INFO("Data Dir: %s, Time Stamp: %s", strDataDir.c_str(), strTimeStamp.c_str());
    }

private:
};




}
}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_data_collection");
    ros::NodeHandle nh;
    INT32           runFrequency = 20;

    INT32 rslt = SaijieBasicsLibInit(false);
    if (rslt != 0) {
        ST_LOG_ERR("SaijieBasicsLibInit() Fail.");
        return -1;
    }

    rslt = Comm_Init(runFrequency, 0.3);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    TEST::DATA_COLLECTION::COLLECT_TXT_C objCollectTxt;
    TIMER_C                              objCollectTimer(1.0);

    ros::Rate loopRate(runFrequency);
    while (ros::ok())
    {
        ros::spinOnce();
        Comm_UptByPeriod();

        if (objCollectTimer.IsTimeOut()) {
            objCollectTimer.Reset();

            objCollectTxt.Write();
        }

        loopRate.sleep();
    }

    return 0;
}


