#include <ros/ros.h>
#include <std_msgs/String.h>
#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include "comm/comm_topic_name.h"
#include "comm/comm_cfg_param.h"
#include "comm/comm_file.h"
#include "comm/comm_cfg/comm_cfg_amend.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <memory>

#include "comm/comm_sys.h"

class StringMsgSub
{ 
public:
    typedef std::shared_ptr<StringMsgSub> Ptr;

public:
    StringMsgSub(ros::NodeHandle &nh, const std::string& strTopicName, const INT32 &maxSize)
    {
        sub_ = nh.subscribe<std_msgs::String>(strTopicName, maxSize, &StringMsgSub::MsgCallBack, this);
    }
   
    INT32 GetMsg(std::string &strMsg)
    {
        if (!isValid_) return -1;
        
        isValid_ = false;
        
        strMsg = strMsg_;

        return 0;
    }
      
private:
    void MsgCallBack(const std_msgs::StringConstPtr &pMsg)
    {
        std::string strMsg = pMsg->data;
        strMsg_  = strMsg; 
        isValid_ = true;
    }
    
private:
    ros::Subscriber sub_;
    std::string strMsg_;
    BOOL isValid_ = false;
};


int main(int argc, char** argv)
{
    ros::init(argc, argv, "modify_serial_num_node");
    ros::NodeHandle nh;
    INT32           runFrequency = 1;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    COMM::CFG::CFG_PARAM_FILE_MNG_C objCfgParamFileMng;
    std::string strChangeCfgParamFilePath = objCfgParamFileMng.GetChangeCfgParamFilePath();
    std::string strWorkCfgParamFilePath   = objCfgParamFileMng.GetWorkCfgParamFilePath();
    ST_LOG_INFO("修改Change配置文件路径为: %s.", strChangeCfgParamFilePath.c_str());
    ST_LOG_INFO("修改Work配置文件路径为: %s.",        strWorkCfgParamFilePath.c_str());

    TiXmlDocument objXmlCfgFile;
    if (0 != objCfgParamFileMng.ReadWorkCfgParamFile(strWorkCfgParamFilePath, objXmlCfgFile)) {
        ST_LOG_ERR("加载配置文件失败。");
        return -1;
    }

    TiXmlElement *pobjCfgParam = objXmlCfgFile.RootElement();
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("objXmlCfgFile.RootElement() Fail.");
        return -1;
    }

    COMM::CFG::CFG_AMEND_C objCfgAmend;
    std::vector<std::string> vstrLidarName{"MainLidar", "BlindLidar", "BlindLidar2", "BlindLidar3", "BlindLidar4"};
    std::vector<StringMsgSub::Ptr> vpobjSerialNumSub(5, nullptr);
    for (size_t i = 0; i < vstrLidarName.size(); i++) {
        if (objCfgAmend.IsElementExist(*pobjCfgParam, {"LidarGroup", vstrLidarName[i].c_str()})) {
            std::string strTopicName = std::string("/") + vstrLidarName[i] + "/serial_num";
            vpobjSerialNumSub[i] = std::make_shared<StringMsgSub>(nh, strTopicName, 10);
        }
    }
   
    ros::Rate loop_rate(runFrequency);
    while (ros::ok())
    {
        ros::spinOnce();
        Comm_UptByPeriod();

        INT32 cnt = 0;
        for (size_t i = 0; i < vpobjSerialNumSub.size(); i++) {
            if (vpobjSerialNumSub[i] == nullptr) continue;
            
            cnt++;
            std::string strSerialNum;
            if (0 != vpobjSerialNumSub[i]->GetMsg(strSerialNum)) {
                ST_LOG_INFO("Get %s Serial Num Fail.", vstrLidarName[i].c_str());
                continue;
            }
            
            if (0 == objCfgAmend.SetAttribute(*pobjCfgParam, {"LidarGroup", vstrLidarName[i].c_str(), "Property"}, "LidarSerialNum", strSerialNum)) {
                vpobjSerialNumSub[i].reset();
            }  
        }
        if (cnt == 0) break;
        
        loop_rate.sleep();
    }
    
    if (0 != objCfgParamFileMng.SaveCfgParamFile(strWorkCfgParamFilePath, objXmlCfgFile)) {
        ST_LOG_ERR("保存work配置文件失败。");
        return -1;
    }

    if (0 != objCfgParamFileMng.SaveCfgParamFile(strChangeCfgParamFilePath, objXmlCfgFile)) {
        ST_LOG_ERR("保存change配置文件失败。");
        return -1;
    }
    
    ST_LOG_ERR("修改配置文件成功。");
    
    return 0;
}



