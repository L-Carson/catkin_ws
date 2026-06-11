/** 
 * @file     task_manager_task.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     9/27/21 
 */

#ifndef S330_ALG_DEMO_TASK_MANAGER_TASK_H
#define S330_ALG_DEMO_TASK_MANAGER_TASK_H

#include "task_manager.h"

class TaskManagerTest
{
public:
    TaskManagerTest(){
        task_ = BasicTaskManager::Create();
        task_->Reg(&TaskManagerTest::CallBack,this);
    }
private:
    void CallBack(JsonValueConstPtr& json){
        Json::Value obj_json;
        obj_json = *json;
        int type = 0;

        do{
            if(!obj_json["MsgType"].isInt()){
                break;
            }
            type = obj_json["MsgType"].asInt();
            Json::Value response;
            response["MsgType"] = 100000+obj_json["MsgType"].asInt();
            response["TransId"] = obj_json["TransId"].asInt()+100000;
            response["MsgContent"]["TaskId"] = 1;
            response["MsgContent"]["ErrCode"] = 0;
            task_->Send(response);
        }while(0);
    }
private:
    BasicTaskManager* task_;
};



#endif //S330_ALG_DEMO_TASK_MANAGER_TASK_H
