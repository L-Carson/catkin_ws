/**
 * @file     alarm_manager_demo.h
 * @brief
 * @details
 * @author   华仔
 * @date     10/20/21
 */

#ifndef S330_ALG_DEMO_ALARM_MANAGER_DEMO_H
#define S330_ALG_DEMO_ALARM_MANAGER_DEMO_H

#include "alarm_manager.h"
#include "my_std_thread.h"
#include "json_handler.h"

class AlarmManagerTest
{
public:
    /**
     * 构造函数
     */
    AlarmManagerTest(){
        alarm_= BasicAlarmManager::Create();
        //alarm_->Reg(1,&AlarmManagerTest::AlarmReportCallBack,this,&AlarmManagerTest::AlarmRecoverCallBack,this);
        alarm_->Reg(2,&AlarmManagerTest::AlarmReportCallBack,this,&AlarmManagerTest::AlarmRecoverCallBack,this);
        my_thread_.CreateThread(thread_id_, &AlarmManagerTest::DealThreadData, this, NULL);
    }
    /**
     * 析构函数
     */
    ~AlarmManagerTest(){

    }

private:
    /**
     * 告警发生回调
     * @param json
     */
    void AlarmReportCallBack(JsonValueConstPtr& json){
        Json::Value obj=*json;
        std::string str = JsonHandler::JsonToString(obj);
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s",str.c_str());
        int id = obj["AlarmId"].asInt();
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"发生%d告警",id);
    }
    /**
     * 告警取消回调
     * @param json
     */
    void AlarmRecoverCallBack(JsonValueConstPtr& json){
        Json::Value obj=*json;
        std::string str = JsonHandler::JsonToString(obj);
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s",str.c_str());
        int id = obj["AlarmId"].asInt();
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"取消%d告警",id);
    }

    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg){
        long long cur_time = PublisSystem::GetCurTimeMsLonglong();
        long long start_time = PublisSystem::GetCurTimeMsLonglong();
        bool alarm_reoprt_1 = false;
        bool alarm_reoprt_2 = false;
        bool alarm_recover_1 = false;
        bool alarm_recover_2 = false;
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"告警测试线程开启");
        while(ros::ok()){
            cur_time = PublisSystem::GetCurTimeMsLonglong();

            if(!alarm_reoprt_1 && cur_time-start_time>1000){
                Json::Value json;
                json["AlarmId"] = 1;
                alarm_->Report(json);
                alarm_reoprt_1 = true;
            }
            if(!alarm_recover_1 && cur_time-start_time>3000){
                Json::Value json;
                json["AlarmId"] = 1;
                alarm_->Recover(json);
                alarm_recover_1 = true;
            }
            usleep(1000);
        }

        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"告警测试线程关闭");
    }
private:
    BasicAlarmManager* alarm_;
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
};



class AlarmManagerTest1
{
public:
    /**
     * 构造函数
     */
    AlarmManagerTest1(){
        alarm_= BasicAlarmManager::Create();
        alarm_->Reg(1,&AlarmManagerTest1::AlarmReportCallBack,this,&AlarmManagerTest1::AlarmRecoverCallBack,this);
        alarm_->Reg(2,&AlarmManagerTest1::AlarmReportCallBack,this,&AlarmManagerTest1::AlarmRecoverCallBack,this);
        my_thread_.CreateThread(thread_id_, &AlarmManagerTest1::DealThreadData, this, NULL);
    }
    /**
     * 析构函数
     */
    ~AlarmManagerTest1(){

    }

private:
    /**
     * 告警发生回调
     * @param json
     */
    void AlarmReportCallBack(JsonValueConstPtr& json){
        Json::Value obj=*json;
        std::string str = JsonHandler::JsonToString(obj);
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s",str.c_str());
        int id = obj["AlarmId"].asInt();
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"发生%d告警",id);
    }
    /**
     * 告警取消回调
     * @param json
     */
    void AlarmRecoverCallBack(JsonValueConstPtr& json){
        Json::Value obj=*json;
        std::string str = JsonHandler::JsonToString(obj);
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s",str.c_str());
        int id = obj["AlarmId"].asInt();
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"取消%d告警",id);
    }
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg){
        long long cur_time = PublisSystem::GetCurTimeMsLonglong();
        long long start_time = PublisSystem::GetCurTimeMsLonglong();
        bool alarm_reoprt_1 = false;
        bool alarm_reoprt_2 = false;
        bool alarm_recover_1 = false;
        bool alarm_recover_2 = false;
        long long test_log = PublisSystem::GetCurTimeMsLonglong();
        int malloc_len = 20*1024;
        long long test_log_sum=0;
        char* test_log_data = (char*)malloc(malloc_len);
        memset(test_log_data,0x31,malloc_len-1);
        test_log_data[malloc_len-1]=0;
        test_log_data[malloc_len-2]=0x32;
        test_log_data[malloc_len-3]=0x32;
        //test_log_data[malloc_len]=0;
        std::string test_log_str = test_log_data;
        while(ros::ok()){
            cur_time = PublisSystem::GetCurTimeMsLonglong();

            if(!alarm_reoprt_1 && cur_time-start_time>1000){
                Json::Value json;
                json["AlarmId"] = 2;
                alarm_->Report(json);
                alarm_reoprt_1 = true;
            }
            if(!alarm_recover_1 && cur_time-start_time>90000){
                Json::Value json;
                json["AlarmId"] = 2;
                alarm_->Recover(json);
                alarm_recover_1 = true;
            }
            usleep(1000);
            if(cur_time-test_log>10){
                //LOG_INFO(kS330AlgDemoLogSaveFileFlag,"%s",test_log_str.c_str());
                test_log = PublisSystem::GetCurTimeMsLonglong();
                test_log_sum += test_log_str.length();

            }
        }
    }
private:
    BasicAlarmManager* alarm_;
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
};

#endif //S330_ALG_DEMO_ALARM_MANAGER_DEMO_H
