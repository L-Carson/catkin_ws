#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <ros/ros.h>

// 屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "pcpt_net_inference.h"
#pragma GCC diagnostic pop

#include <algorithm>
#include <yaml-cpp/yaml.h>

using namespace std;

int PCPT_INFERENCE_C::Init()
{
    subLidarData_   = nh_.subscribe(objModelConfig_.sensorNameRs16, 1 , &PCPT_INFERENCE_C::MsgReceiveCallBack,this);
    pubCommBoxMsgs_ = nh_.advertise<comm_msg::boxArray>(TOPIC_NAME_MARKER_PCD_NET_BOX, 1, true);

    objpcptRos_.Init();

    float* anchorsArray = nullptr;  // load anchors

    switch (objModelConfig_.modelType) {
        case EN_CENTER_POINT:
            scoreThreshold_ = 0.3;
            pobjNet_ = new CenterPoint(scoreThreshold_, nmsThreshold_, false, objModelConfig_.modelFile);
            break;
        case EN_POINT_PILLER_MAN_CAR:
            if (0 != LoadAnchor(anchorsArray, objModelConfig_.anchorFile)) return -1;
            scoreThreshold_ = 0.1;
            pobjNet_ = new PointPillars(scoreThreshold_, nmsThreshold_, false, objModelConfig_.modelFile, anchorsArray, 3);
            break;
        case EN_POINT_PILLER_LOW_OBST:
            if (0 != LoadAnchor(anchorsArray, objModelConfig_.anchorFile)) return -1;
            scoreThreshold_ = 0.0;
            pobjNet_ = new PointPillars(scoreThreshold_, nmsThreshold_, false, objModelConfig_.modelFile, anchorsArray, 1);
            break;
        default:
            break;
    }
    ROS_INFO("objModelConfig_.modelType New Succ.");
    if (anchorsArray != nullptr) delete[] anchorsArray;

    pointsVector_.clear();

    if (objModelConfig_.istest) {
        std::cout<<""<<std::endl;
        ROS_INFO("Model Test Enable.");
        mThread_ = thread(&PCPT_INFERENCE_C::TestThread, this);
    }
    return 0;
}

/**************************************************************************************
功能描述: 加载anchor文件
修改记录:
**************************************************************************************/
int PCPT_INFERENCE_C::LoadAnchor(float *&points_array, string file_name)
{
    if (-1 == npy2Arrary(points_array, objModelConfig_.anchorFile)) {
        ROS_INFO("Could Not Read Anchors File.");
        return -1;
    }
    return 0;
}

/**************************************************************************************
功能描述:不同类别设置不同置信度，过滤检测框
修改记录:
**************************************************************************************/
vector<lidarDataType> PCPT_INFERENCE_C::FilterByMulScore(vector<lidarDataType> &inputData)
{
    vector<lidarDataType> outData;
    for (size_t i = 0 ; i < inputData.size(); ++i) {
        switch (inputData[i].label) {
            case PCD_LABEL_ENUM::EN_CAR:        if (inputData[i].score > (scoreThreshold_ + 0.2)) outData.push_back(inputData[i]); break;
            case PCD_LABEL_ENUM::EN_BICYCLE:    if (inputData[i].score > (scoreThreshold_ + 0.1)) outData.push_back(inputData[i]); break;
            case PCD_LABEL_ENUM::EN_PEDESTRIAN: if (inputData[i].score > (scoreThreshold_ + 0.1)) outData.push_back(inputData[i]); break;
            default: break;
        }
    }
    return outData;
}

/**************************************************************************************
功能描述:通过设置检测框内最少点数，对检测框进行过滤
修改记录:
**************************************************************************************/
vector<lidarDataType> PCPT_INFERENCE_C::FilterByPoints(vector<lidarDataType> &inputData, pclCloud &inPoints, int minNum)
{
    vector<lidarDataType> outData;
    return outData;
}

/**************************************************************************************
功能描述:NetDoInference
修改记录:
**************************************************************************************/
int PCPT_INFERENCE_C::NetDoInference(float* pointsNetArray, int inNumPoints, vector<float> &outDetections,
                                     vector<int> &outLabels, vector<float> &outScores, ros::Time time)
{
    double startTime = ros::Time::now().toSec();
    pobjNet_->DoInference(pointsNetArray, inNumPoints, &outDetections, &outLabels , &outScores);
    std::cout<<"DoInference Time: " << ros::Time::now().toSec() - startTime <<std::endl;

    int BoxFeature = 7;
    int num_objects = outDetections.size() / BoxFeature;
    std::cout<<"num_objects:: "<< num_objects <<std::endl;

    vector<lidarDataType> outTemp;
    for (int i = 0; i < num_objects; ++i) {
        lidarDataType object;
        object.x = outDetections.at(i * BoxFeature + 0);
        object.y = outDetections.at(i * BoxFeature + 1);
        object.z = outDetections.at(i * BoxFeature + 2) ;
        object.width  = outDetections.at(i * BoxFeature + 3);
        object.length = outDetections.at(i * BoxFeature + 4);
        object.height = outDetections.at(i * BoxFeature + 5);
        float yaw = outDetections.at(i * BoxFeature + 6);
        object.angle = yaw;
        object.label = outLabels.at(i);
        object.score = outScores.at(i);
        outTemp.push_back(object);
    }

    vector<lidarDataType> outData = FilterByMulScore(outTemp);

    // send box marker
    objpcptRos_.ShowNetDetectMsgs(outData, time);

    // send box message
    comm_msg::boxArray sendBoxArray;
    for (size_t i = 0; i < outData.size(); ++i) {
        comm_msg::box tmpBox;
        switch(outData[i].label) {
            case PCD_LABEL_ENUM::EN_CAR:        tmpBox.label.value = LABEL_ENUM::TAG_CAR;        break;
            case PCD_LABEL_ENUM::EN_BICYCLE:    tmpBox.label.value = LABEL_ENUM::TAG_BICYCLE;    break;
            case PCD_LABEL_ENUM::EN_PEDESTRIAN: tmpBox.label.value = LABEL_ENUM::TAG_PEDESTRIAN; break;
            default: break;
        }
        tmpBox.x      = outData[i].x;
        tmpBox.y      = outData[i].y;
        tmpBox.z      = outData[i].z;
        tmpBox.width  = outData[i].width;
        tmpBox.length = outData[i].length;
        tmpBox.height = outData[i].height;
        tmpBox.yaw    = outData[i].angle;
        tmpBox.score  = outData[i].score;
        sendBoxArray.boxes.push_back(tmpBox);
    }

    sendBoxArray.header.frame_id = "base_link";
    sendBoxArray.header.stamp = time;
    pubCommBoxMsgs_.publish(sendBoxArray);

    return 0;
}

/**************************************************************************************
功能描述: 通过bin 文件进行算法测试
修改记录:
**************************************************************************************/
void PCPT_INFERENCE_C::Test()
{
    float* pointsArray;
    int inNumPoints;
    inNumPoints = bin2Arrary(pointsArray, objModelConfig_.testFileName);
    pclCloud pclCloud;
    pcl::PointXYZI pointTemp;
    shufferArrary(pointsArray, inNumPoints);

    for (int i = 0; i < inNumPoints; ++i) {
        pointTemp.x         = pointsArray[i * 5];
        pointTemp.y         = pointsArray[i * 5 + 1];
        pointTemp.z         = pointsArray[i * 5 + 2];
        pointTemp.intensity = pointsArray[i * 5 + 3];
        pclCloud.points.push_back(pointTemp);
    }
    sensor_msgs::PointCloud2::Ptr pointCloudMessage(new sensor_msgs::PointCloud2());
    pcl::toROSMsg(pclCloud, *pointCloudMessage);
    objpcptRos_.PublishPoints(pointCloudMessage, ros::Time::now());

    vector<float> outDetections;
    vector<int> outLabels;
    vector<float> outScores;
    NetDoInference(pointsArray, inNumPoints, outDetections, outLabels, outScores, ros::Time::now());
    delete[] pointsArray;
}

/**************************************************************************************
功能描述:testThread
修改记录:
**************************************************************************************/
void  PCPT_INFERENCE_C::TestThread(void){
    while(true) {
        Test();
        sleep(1);
    }
}

/**************************************************************************************
功能描述:16线雷达接受数据和模型推理
修改记录:
**************************************************************************************/
void PCPT_INFERENCE_C::MsgReceiveCallBack(const sensor_msgs::PointCloud2ConstPtr &pstMsg)
{
    frameCount_++;
    printf("\n");
    std::cout<< "frameCount_:: " << frameCount_ <<std::endl;
    float*  pointsNetArray = NULL;
    int     inNumPoints = 0;
    uint8_t byteNum = 5;

    if (objModelConfig_.version == 2) {
        pclCloud ptrCloud;
        if (pstMsg->data.size()) pcl::fromROSMsg(*pstMsg, ptrCloud);
        pointsNetArray = new float[ptrCloud.points.size() * byteNum];
        for (size_t i = 0; i < ptrCloud.points.size(); i++) {
            const pcl::PointXYZI& pt = ptrCloud.points[i];
            pointsNetArray[i * byteNum ]    =  pt.x;
            pointsNetArray[i * byteNum + 1] =  pt.y;
            pointsNetArray[i * byteNum + 2] =  pt.z ;
            //pointsNetArray[i * byteNum + 3] =  intensity;
            pointsNetArray[i * byteNum + 3] =  0;
            pointsNetArray[i * byteNum + 4] =  0;
        }
        inNumPoints = ptrCloud.points.size();
    } 
    else if (objModelConfig_.version == 3) {
        CloudXYZIRT ptrCloud;
        if (pstMsg->data.size()) pcl::fromROSMsg(*pstMsg, ptrCloud);

        pointsNetArray = new float[ptrCloud.points.size() * byteNum];
        for (size_t i = 0; i < ptrCloud.size(); ++i) {
            pcl::PointXYZI pt;
            pt.x = ptrCloud.points[i].x;
            pt.y = ptrCloud.points[i].y;
            pt.z = ptrCloud.points[i].z - lidarHeight_;
            pt.intensity = ptrCloud.points[i].intensity;
            pointsNetArray[i * byteNum]     = pt.x;
            pointsNetArray[i * byteNum + 1] = pt.y;
            pointsNetArray[i * byteNum + 2] = pt.z ;
            pointsNetArray[i * byteNum + 3] = pt.intensity;
            pointsNetArray[i * byteNum + 4] = 0;
        }
        inNumPoints = ptrCloud.points.size();
    }

    shufferArrary(pointsNetArray, inNumPoints);
    vector<float> outDetections;
    vector<int>   outLabels;
    vector<float> outScores;
    NetDoInference(pointsNetArray, inNumPoints, outDetections, outLabels, outScores, pstMsg->header.stamp);
    if (pointsNetArray != NULL) delete[] pointsNetArray;
}
