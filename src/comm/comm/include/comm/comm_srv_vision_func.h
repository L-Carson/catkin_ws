#ifndef __COMM_SRV_VISION_FUNC_H__
#define __COMM_SRV_VISION_FUNC_H__

#include "comm/comm_base.h"
#include "comm/loc_json/json.h"
#include "comm/comm_topic_name.h"
#include "comm_srvs/visionFunc.h"
#include <ros/ros.h>


struct VISION_FUNC_CFG_STRU
{
    VISION_FUNC_CFG_STRU() {}
    VISION_FUNC_CFG_STRU(UINT08 funcId, BOOL isUseIpu, UINT08 hz) {
        this->funcId   = funcId;
        this->isUseIpu = isUseIpu;
        this->hz       = hz;
    }

    UINT08  funcId;
    BOOL    isUseIpu;
    UINT08  hz;
};

struct VISION_LABEL_CFG_STRU
{
    std::vector<UINT32> vectDetLabel;
    std::vector<UINT32> vectSegLabel;

    BOOL                isSupportThickPipe;
    float               thickPipeWidth;
};

struct VISION_FUNCS_CFG_STRU
{
    std::vector<VISION_FUNC_CFG_STRU> vectFuncCfg;
    VISION_LABEL_CFG_STRU             stLabelCfg;
};

class VISION_FUNC_C
{
public:
    VISION_FUNC_C(ros::NodeHandle &nh)
    {
        objSrvsClint_ = nh.serviceClient<comm_srvs::visionFunc>(TOPIC_NAME_SERVER_VISION_FUNC);
    }

    /* 阻塞式等待 */
    INT32 WaitAndGet(VISION_FUNCS_CFG_STRU &stFuncsCfg);

private:
    /* 此处不能用 TIMER_C 类       */
    ros::ServiceClient objSrvsClint_;

    INT32 WaitServiceAvailable(void);
    INT32 GetParam(std::vector<VISION_FUNC_CFG_STRU> &vectFuncCfg, VISION_LABEL_CFG_STRU &stLabelCfg);
};



#endif
