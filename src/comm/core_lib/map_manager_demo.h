/** 
 * @file     map_manager_demo.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     10/27/21 
 */

#ifndef S330_ALG_DEMO_MAP_MANAGER_DEMO_H
#define S330_ALG_DEMO_MAP_MANAGER_DEMO_H

#include "my_std_thread.h"
#include "map_manager.h"

class MapManagerTest
{

public:
    MapManagerTest(){
        map_ = BasicMapManager::Create();
        map_->RegMapChange(&MapManagerTest::MapChange,this);
        my_thread_.CreateThread(thread_id_, &MapManagerTest::DealThreadData, this, NULL);
    }
    ~MapManagerTest(){

    }
private:
    void MapChange(std::string map){
        LOG_INFO(kS330AlgDemoLogSaveFileFlag,"地图发生变更为：%s",map.c_str());
    }
    /**
     * 线程死循环
     * @param arg void*类型指针
     */
    void DealThreadData(void *arg){
        std::string last_map="";
        std::string cur_map="";
        while(ros::ok()){
            cur_map = map_->GetCurMap();
            if(cur_map!=last_map){
                LOG_INFO(kS330AlgDemoLogSaveFileFlag,"地图发生变更，之前为：%s，现在为：%s",last_map.c_str(),cur_map.c_str());
                last_map = cur_map;
            }

            usleep(3000);
        }
    }
private:
    BasicMapManager* map_;
    MyStdThread my_thread_;
    MyStdThread::Thread thread_id_=NULL;
};



#endif //S330_ALG_DEMO_MAP_MANAGER_DEMO_H
