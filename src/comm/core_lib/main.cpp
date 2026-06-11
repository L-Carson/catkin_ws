#include <iostream>
#include <ros/ros.h>

#include "log_demo.h"
//#include "json_handler.h"
#include "task_manager_demo.h"
#include "dev_manager_demo.h"

#include "alarm_manager_demo.h"
#include "basic_manager.h"
#include "wss_manager_demo.h"
#include "map_manager_demo.h"
#include "xml_demo.h"


int main1111(int argc, char** argv) {

    cout<<"程序启动222"<<endl;
    std::string log_file = "wss_pack_demo";
    std::string const_type="importMap";
    std::string log_path = PublisSystem::GetSysUserPath()+"/map_config/log/debug";
    LogOutInit(log_path,false);
    LOG_INFO(log_file,"程序开始");
    if(argc>3){
        std::string path= argv[1];
        const_type = argv[2];
        int pack_size = atoi(argv[3]);

        int length = PublisSystem::GetFileLength(path.c_str());
        LOG_INFO(log_file,"%s文件长度为：%d",path.c_str(),length);
        if(length>0){
            unsigned char *file_buf = (unsigned char *)malloc(length+1024);
            int read_len = length;
            if(0==PublisSystem::ReadFileBinary(path,file_buf+1024,read_len)){
                std::string fileData = PublisSystem::HexTostring(file_buf+1024,length);
                LOG_INFO(log_file,"文件内容：%s",fileData.c_str());

                unsigned char *one_buf = (unsigned char *)malloc(pack_size+1024);
                int sum=0;
                for(int i=0;i<length;i+=pack_size){
                    char buf[1024]={0};
                    std::string path1="";
                    std::string file_name_="";
                    PublisSystem::GetPathAndName(path,path1,file_name_);
                    int file_len=0;
                    if(i+pack_size<=length){
                        file_len = pack_size;
                    }else{
                        file_len = length-i;
                    }
                    snprintf(buf,sizeof(buf),"&id=1&type=%s&filename=%s&fileTotal=%d&filePos=%d&fileLen=%d&",const_type.c_str(),file_name_.c_str(),length,i,file_len);
                    std::string head= buf;
                    snprintf(buf,sizeof(buf),"length=%05d%s",head.length()+12,head.c_str());
                    head=buf;
                    memcpy(one_buf,head.c_str(),head.length());
                    memcpy(one_buf+head.length(),file_buf+1024+i,file_len);
                    sum++;
                    LOG_INFO(log_file,"第%d包头：%s",sum,head.c_str());
                    LOG_INFO(log_file,"第%d包内容：%s",sum,PublisSystem::HexTostring(file_buf+1024+i,file_len).c_str());
                    LOG_INFO(log_file,"整包内容：%s",PublisSystem::HexTostring(one_buf,head.length()+file_len).c_str());
                }

            }
            if(file_buf){
                free(file_buf);
            }
        }

    }

    LOG_INFO(log_file,"程序结束");
    return 0;
}


int main(int argc, char** argv) {
    ros::init(argc,argv,"saijie_basics_demo");

    SaijieBasicsLibInit(false);
    std::string node_name  = ros::this_node::getName();
    LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s节点程序启动，版本为：%s",node_name.c_str(),GetSaijieBasicsLibVer().c_str());

    //xml_test();
    //TaskManagerTest task_test;
    DevManagerTest dev_test;
    //dev_test.DevStart();
    //AlarmManagerTest alarm_test;
    //AlarmManagerTest1 alarm_test1;
    //WssClientDemo *wss_client_demo = NULL;
    //wss_client_demo = new WssClientDemo();
    //MapManagerTest map_test;


    Json::Value Status;
    Status["MapInfo"]["MapX"] = 9;
    //dev_test.SetStatusDev(Status);

    // sleep(3);
    // dev_test.SetStatusDev(Status);

    while(1)
    {
        //LOG_INFO(kS330AlgDemoLogSaveFileFlag,"开始上报状态信息");

        Json::Value Report;
        Json::Value Report_1;

        Report["PoseInfo"]["MapX"] = 9;
        Report["PoseInfo"]["MapY"] = 9;
        Report["PoseInfo"]["MapZ"] = 9;
        Report["MapInfo"]["SubMapId"] = 9;
        dev_test.Report(Report);
        sleep(0.5);
    }


    //WssClientDemo wss_client_demo;
    //LOG_INFO(kS330AlgDemoLogSaveFileFlag,"开始delete");
    //(5);

    ros::spin();
    ros::shutdown();

    LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s节点程序结束",node_name.c_str());

    return 0;
}