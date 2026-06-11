/**
 * @file     bag_handler.h
 * @brief    bag录制整理封装
 * @details  bag录制整理封装
 * @author   华仔
 * @date     11/9/21
 */

#ifndef NAV_BUSINESS_MODULE_BAGHANDLER_H
#define NAV_BUSINESS_MODULE_BAGHANDLER_H

#include <iostream>
using namespace std;
#include <ros/ros.h>
#include <mutex>
#include <vector>

///< rosbag record -j --split --size=1 -O /home/saite/ljh/bag/1231313.bag __name:=testtestnode /wheel_speed /scan /map /tf /odom

class BagHandler
{
public:
    /**
     * 构造函数
     * @param[in] node_name 节点名称，停止录制会用此名称关闭，如果要同时录制2个以上bag就需要不同的节点名，或则会中断前面录制
     * @param[in] save_path 录制后的bag保存路径
     * @param[in] file_name 录制后的bag保存名字
     * @param[in] max_file 录制bag单个文件最大大小，单位为M，当<0不做单个大小限制
     */
    BagHandler(string save_path,string file_name,string node_name="bag_handler_node",int max_file=100);
    /**
     * 析构函数
     */
    ~BagHandler();
    /**
     * 开始录制
     * @param[in] topics 录制话题,当为空时代表录制所有话题
     * @return 0：成功，其他失败
     */
    int Start(vector<std::string> topics);
    /**
     *开始录制-不压缩话题数据
     * 
     * @param topics  录制话题,当为空时代表录制所有话题
     * @return int 0：成功，其他失败
     */
    int StartUncompress(vector<std::string> topics);
    /**
     * 结束录制
     * @return 0：成功，其他失败
     */
    int End();
private:
    std::mutex data_mutex_;                                              ;
    string node_name_;///< 节点名称
    string save_path_;///< 文件保存路径
    string file_name_;///< 文件名
    int    max_file_;///< 文件最大大小
    bool   record_status_;//录制状态，true正在录制
};




#endif //NAV_BUSINESS_MODULE_BAGHANDLER_H
