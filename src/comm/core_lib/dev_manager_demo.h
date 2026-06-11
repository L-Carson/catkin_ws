/** 
 * @file     dev_manager_demo.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     10/14/21 
 */

#ifndef S330_ALG_DEMO_DEV_MANAGER_DEMO_H
#define S330_ALG_DEMO_DEV_MANAGER_DEMO_H

#include "dev_manager.h"
#include "my_std_thread.h"
#include "public_system.h"

class DevManagerTest
{
public:
    DevManagerTest(){
        dev_ = BasicDevManager::Create();

        DevStart();//启动设备
        sleep(2);
        //注册上报状态默认信息
        SetStatusDefaults["PoseInfo"]["MapX"] = 0.0;
        SetStatusDefaults["PoseInfo"]["MapY"] = 0.0;
        SetStatusDefaults["PoseInfo"]["MapZ"] = 0.0;
        SetStatusDefaults["MapInfo"]["SubMapId"] = 0.0;

        dev_->SetStatusDefaults(SetStatusDefaults);///<设置测试状态上报的默认内容

        //消息注册回调
        dev_->Reg(3,&DevManagerTest::CallBack,this);
        dev_->Reg(6,&DevManagerTest::CallBack,this);
        dev_->Reg(7,&DevManagerTest::CallBack,this);
        dev_->Reg(9,&DevManagerTest::CallBack,this);
        dev_->Reg(11,&DevManagerTest::CallBack,this);
        dev_->Reg(12,&DevManagerTest::CallBack,this);
        dev_->Reg(15,&DevManagerTest::CallBack,this);
        dev_->Reg(16,&DevManagerTest::CallBack,this);
        //my_thread_.CreateThread(thread_id_, &DevManagerTest::DealThreadData, this, NULL);
    }
    /**
     * 设备启动
     */
    void DevStart(){
        dev_->StartStateReport();
    }

    void  SetStatusDev(Json::Value& state_info)
    {
        dev_->SetStatusDefaults(state_info);///<设置测试状态上报的默认内容
    }


    void  Report(Json::Value& state_info)
    {
        dev_->StateReport(state_info);
    }
private:
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg){
        long long cur_time;
        long long last_time = PublisSystem::GetCurTimeMsLonglong();
        while(ros::ok()){
            cur_time = PublisSystem::GetCurTimeMsLonglong();
            if(cur_time-last_time>10000){
                Json::Value json;
                Json::Value pose;
                Json::Value move;
                //{"PoseInfo":{"MapX":0.2,"MapY":0.2,"MapZ":0.2,"MapYaw":0.2,"GpsLon":0.2,"GpsLat":0.2,"GpsAlt":0.2,"GpsYaw":0.2},"MoveInfo":{"Vel":0.9,"SteeringAngle":0.3,"TurnDir":0}}
                pose["MapX"] = 0.2;
                pose["MapY"] = 0.2;
                pose["MapZ"] = 0.2;
                pose["MapYaw"] = 0.2;
                pose["GpsLon"] = 0.2;
                pose["GpsLat"] = 0.2;
                pose["GpsLat"] = 0.2;
                pose["GpsAlt"] = 0.2;
                json["PoseInfo"] = pose;
                move["Vel"] = 0.9;
                move["SteeringAngle"] = 0.3;
                move["TurnDir"] = 0;
                json["MoveInfo"] = move;
                dev_->StateReport(json);
                last_time = cur_time;
            }

            usleep(3000);
        }
    }
    void CallBack(JsonValueConstPtr& json){
        Json::Value obj_json;
        obj_json = *json;
        int type = 0;

        if(!obj_json["MsgType"].isInt()){
            return;
        }
        type = obj_json["MsgType"].asInt();
        switch(type){
            case 3://自检请求
            {
                ///< 收到自检通知

                ///< 自检响应
                Json::Value response;
                response["MsgType"] = 4;
                response["TransId"] = obj_json["TransId"];
                response["MsgContent"] = Json::objectValue;
                dev_->Send(response);
            }
                break;
            case 6:///< 状态响应
                break;
            default:
                break;
        }

    }


private:
    BasicDevManager* dev_;
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;

    Json::Value SetStatusDefaults;
};



#endif //S330_ALG_DEMO_DEV_MANAGER_DEMO_H
