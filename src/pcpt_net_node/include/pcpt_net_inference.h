#ifndef __PCPT_INFERENCE_H__
#define __PCPT_INFERENCE_H__
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <ros/ros.h>
#include "pcpt_net_centerpoint.h"
#include "pcpt_net_pointpillars.h"
#include "pcpt_net_files.h"
#include "pcpt_net_ros.h"
#include "comm_msg/boxArray.h"
#include "comm/comm_label.h"
#include "comm/comm_topic_name.h"

using namespace std;

enum MODEL_TYPE_ENUM {
    EN_CENTER_POINT          = 0,   //CenterPoint
    EN_POINT_PILLER_MAN_CAR  = 1,   //PointPiller（人车）
    EN_POINT_PILLER_LOW_OBST = 2,   //PointPiller（低矮障碍）
};

typedef struct MODEL_CONFIG_STRU {
    int    version;
    int    modelType;
    float  lidarHeight;
    string anchorFile;
    string sensorNameRs16;
    string sensorNameRsBp;
    bool   istest;
    string testFileName;
    string onnxFile;
    string generateEngineFile;
    bool   isOnnx2EngineFile;
    string modelFile;
} MODEL_CONFIG_STRU;

/**************************************************************************************
功能描述: PCPT_INFERENCE_C
修改记录:
**************************************************************************************/
class PCPT_INFERENCE_C
{
public:
    PCPT_INFERENCE_C(ros::NodeHandle &nh, MODEL_CONFIG_STRU &modelConfig)
    : nh_(nh)
    , objpcptRos_(nh, modelConfig.lidarHeight)
    , objModelConfig_(modelConfig)
    , lidarHeight_(modelConfig.lidarHeight)
    {}

    ~PCPT_INFERENCE_C() {
        if (pobjNet_ != NULL) delete pobjNet_;
    }

    int Init();

private:
    ros::NodeHandle         nh_;
    ros::Subscriber         subLidarData_;
    ros::Publisher          pubCommBoxMsgs_;

    Net_Base*               pobjNet_{nullptr};
    PCPT_ROS_C              objpcptRos_;
    MODEL_CONFIG_STRU       objModelConfig_;

    float                   lidarHeight_;
    float                   scoreThreshold_{0.0};
    float                   nmsThreshold_{0.2};
    int                     frameCount_{0};
    std::thread             mThread_;
    boost::mutex            mutex_;
    vector<pclCloud>        pointsVector_;

    int LoadAnchor(float *&points_array, string file_name);
    vector<lidarDataType> FilterByMulScore(vector<lidarDataType> &inputData);
    vector<lidarDataType> FilterByPoints(vector<lidarDataType> &inputData, pclCloud &inPoints, int minNum);
    int  NetDoInference(float* pointsNetArray, int inNumPoints, vector<float> &outDetections,
                        vector<int> &outLabels, vector<float> &outScores, ros::Time time);
    void MsgReceiveCallBack(const sensor_msgs::PointCloud2ConstPtr &pstMsg);
    void Test();
    void TestThread(void);
};

#endif
