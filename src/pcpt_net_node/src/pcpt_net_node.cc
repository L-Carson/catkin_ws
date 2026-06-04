#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <pwd.h>

#include <yaml-cpp/yaml.h>
#include <ros/ros.h>
#include "pcpt_net_ros.h"
#include "pcpt_net_inference.h"
#include "pcpt_net_centerpoint.h"
#include "pcpt_net_pointpillars.h"
#include <pcpt_net_multi_frame_process.h>
#include <pcpt_net_multi_frame_undistor.h>

using namespace std;

int GetUserName(string &strUserName)
{
    uid_t          userid = getuid();
    struct passwd *pwd    = getpwuid(userid);
    if ((pwd == NULL) || (pwd->pw_name == NULL)) {
        printf("getpwuid() Fail.\n");
        return -1;
    }

    if (strlen(pwd->pw_name) > 100) {
        printf("pwd->pw_name Len(%u) Err.\n", uint32_t(strlen(pwd->pw_name)));
        return -1;
    }

    strUserName = pwd->pw_name;
    return 0;
}

int main(int argc, char** argv)
{
    string strUserName;
    if (GetUserName(strUserName)) exit(-1);
    string strConfigFile = "/home/" + strUserName + "/map_config/pcpt_net_config.yaml";

    YAML::Node config = YAML::LoadFile(strConfigFile);
    MODEL_CONFIG_STRU objModelConfig;
    objModelConfig.version            = config["version"].as<int>();
    objModelConfig.modelType          = config["modelType"].as<int>();
    objModelConfig.lidarHeight        = config["lidarHeight"].as<float>();
    objModelConfig.anchorFile         = config["anchorFile"].as<string>();
    objModelConfig.sensorNameRs16     = config["sensorNameRs16"].as<string>();
    objModelConfig.sensorNameRsBp     = config["sensorNameRsBp"].as<string>();
    objModelConfig.istest             = config["istest"].as<bool>();
    objModelConfig.testFileName       = config["testFileName"].as<string>();
    objModelConfig.onnxFile           = config["onnxFile"].as<string>();
    objModelConfig.generateEngineFile = config["generateEngineFile"].as<string>();
    objModelConfig.isOnnx2EngineFile  = config["isOnnx2EngineFile"].as<bool>();
    objModelConfig.modelFile          = config["modelFile"].as<string>();

    ROS_INFO("objModelConfig Version:     %d",   objModelConfig.version);
    ROS_INFO("objModelConfig ModelType:   %d",   objModelConfig.modelType);
    ROS_INFO("objModelConfig lidarHeight: %.2f", objModelConfig.lidarHeight);
    ROS_INFO("objModelConfig modelFile:   %s",   objModelConfig.modelFile.c_str());
    ROS_INFO("objModelConfig lidarData:   %s",   objModelConfig.sensorNameRs16.c_str());

    if (objModelConfig.isOnnx2EngineFile) {
        std::cout << "GenerateEngine FP16 :: " << objModelConfig.generateEngineFile << std::endl;
        Net_Base::GenerateEngine(objModelConfig.onnxFile, objModelConfig.generateEngineFile);
        return 0;
    } else {
        ros::init(argc, argv, "lidar_object_detect");
        ros::NodeHandle nh;
        ros::Rate       loopRate(10);

        /* 低矮障碍模型 */
        if (objModelConfig.modelType == EN_POINT_PILLER_LOW_OBST) {
            /*1 配置是否启动去畸变 */
            std::string middleRsBpLidar ;
            MULTI_FRAME_PARAM_STRU objMultiFrameParam;
            bool isUndistor = config["isUndistor"].as<bool>();
            if (isUndistor) {
                LIDAR_IMU_PARAM_STRU lidarImuParam;
                lidarImuParam.channel      = 32;
                lidarImuParam.pointsNum    = 1800;
                lidarImuParam.sensorNameRs = config["lidarTopic"].as<std::string>();
                lidarImuParam.imu2LidarRot = config["imu2LidarRot"].as<vector<double>>();
                lidarImuParam.imu2LidarRPY = config["imu2LidarRPY"].as<vector<double>>();
                //MULTI_FRAME_UNDISTORTION_C *motionUndistortion(new MULTI_FRAME_UNDISTORTION_C(nh,lidarImuParam));
                objMultiFrameParam.lidarName = "/MiddleRsBpLidar/car_filtered_points/undistor";
            } else {
                objMultiFrameParam.lidarName = "/MiddleRsBpLidar/car_filtered_points";
            }

            /*2 配置是否启动局部建图*/
            bool isLocalMap = config["isLocalMap"].as<bool>();
            if (isLocalMap) {
                objMultiFrameParam.pcptMap = "pcpt/map";
            } else {
                objMultiFrameParam.pcptMap = "map";
            }

            /*3 配置多帧 */
            objMultiFrameParam.saveBinDir      = config["saveBinDir"].as<std::string>();
            objMultiFrameParam.savePcdDir      = config["savePcdDir"].as<std::string>();
            objMultiFrameParam.isSaveMulti     = config["isSaveMulti"].as<bool>();
            objMultiFrameParam.startFrameId    = config["startFrameId"].as<int>();
            objMultiFrameParam.isLidarNewDrive = config["isLidarNewDrive"].as<bool>();
            ROS_INFO("saveBinDir/savePcdDir: %s %s startFrameId: %d isLidarNewDrive %d ",
                      objMultiFrameParam.saveBinDir.c_str(),
                      objMultiFrameParam.savePcdDir.c_str(),
                      objMultiFrameParam.startFrameId,
                      objMultiFrameParam.isLidarNewDrive);
            //MULTI_FRAME_PROCESS_C *pMultiFrameConversion(new MULTI_FRAME_PROCESS_C(nh,objMultiFrameParam));
        }

        /*4 初始化网络推理 */
        PCPT_INFERENCE_C pcptInference(nh, objModelConfig);
        if (0 != pcptInference.Init()) {
            std::cout << "net inference init failure" << std::endl;
            return -1;
        }

        while (ros::ok()) {
            loopRate.sleep();
            ros::spinOnce();
        }

        return  0;
    }
};
