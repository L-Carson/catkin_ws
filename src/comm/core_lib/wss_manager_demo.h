/** 
 * @file     wss_manager_demo.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     10/22/21 
 */

#ifndef S330_ALG_DEMO_WSS_MANAGER_DEMO_H
#define S330_ALG_DEMO_WSS_MANAGER_DEMO_H

#include "wss_manager.h"
class WssClientDemo
{
public:
    WssClientDemo(){
        client_lib_ = new WssClientLib();
        client_lib_->Start(&WssClientDemo::RecvServer,this);
        my_thread_.CreateThread(thread_id_, &WssClientDemo::DealThreadData, this, NULL);
    }
    ~WssClientDemo(){
        if(client_lib_){
            delete client_lib_;
            client_lib_=NULL;
        }
    }
private:
    void RecvServer(std::string& str){
        std::string recv_str = str;
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"收到导航报文：%s",recv_str.c_str());
        //cout<<"收到导航报文："<<recv_str<<endl;
    }
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg){
        long long start_time = PublisSystem::GetCurTimeMsLonglong();
        long long cur_time = 0;
        while(ros::ok()){
            cur_time = PublisSystem::GetCurTimeMsLonglong();

            if(cur_time-start_time>10000){
//                Json::Value json;
//                json["MsgDomain"] = 2;
//                json["MsgType"] = 1;
//                json["TransId"] = 1;
//                json["MsgContent"] = Json::objectValue;
//                client_lib_->Send(json);
//                start_time = PublisSystem::GetCurTimeMsLonglong();
//                LOG_INFO(kS330AlgDemoLogSaveFileFlag,"发送数据到导航报文：%s",JsonHandler::JsonToString(json).c_str());
//                if(client_lib_){
//                    delete client_lib_;
//                    usleep(10000);
//                    client_lib_=new WssClientLib();
//                    client_lib_->Start(&WssClientDemo::RecvServer,this);
//                }
                start_time = PublisSystem::GetCurTimeMsLonglong();
            }

            usleep(1000);
        }
    }
private:
    WssClientLib* client_lib_;
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
};



#endif //S330_ALG_DEMO_WSS_MANAGER_DEMO_H
