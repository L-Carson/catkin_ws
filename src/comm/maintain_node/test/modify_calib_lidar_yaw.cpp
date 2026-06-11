#include <ros/ros.h>
#include "comm/comm_base.h"
#include "comm/comm_cfg_param.h"
#include "comm/comm_cfg/comm_cfg_amend.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace {
int ReadYawFromFile(const std::string& yawFilePath, double& yaw)
{
    std::ifstream ifs(yawFilePath.c_str());
    if (!ifs.is_open()) {
        ST_LOG_ERR("打开yaw文件失败: %s.", yawFilePath.c_str());
        return -1;
    }

    if (!(ifs >> yaw)) {
        ST_LOG_ERR("读取yaw文件失败或格式非法: %s.", yawFilePath.c_str());
        return -1;
    }

    return 0;
}

bool ParseDoubleArg(const char* arg, double& value)
{
    if (arg == NULL) {
        return false;
    }

    errno = 0;
    char* endPtr = NULL;
    const double parsed = std::strtod(arg, &endPtr);
    if (endPtr == arg || *endPtr != '\0' || errno == ERANGE) {
        return false;
    }

    value = parsed;
    return true;
}

double NormalizeAngleDeg180(const double angleDeg)
{
    double normalized = std::fmod(angleDeg + 180.0, 360.0);
    if (normalized < 0.0) {
        normalized += 360.0;
    }
    return normalized - 180.0;
}

double RoundTo4Decimals(double value)
{
    static const double kScale = 10000.0;
    value = std::round(value * kScale) / kScale;
    if (std::fabs(value) < 0.00005) {
        value = 0.0;
    }
    return value;
}

std::string ToFixed4String(const double value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4) << value;
    return oss.str();
}
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "modify_calib_lidar_yaw_node");
    INT32 runFrequency = 1;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    bool useCmdDeltaYaw = false;
    double cmdDeltaYaw = 0.0;
    const double kMaxCmdDeltaYawAbs = 1.5;
    std::string strYawFilePath;
    if (argc >= 2) {
        if (ParseDoubleArg(argv[1], cmdDeltaYaw)) {
            if (std::fabs(cmdDeltaYaw) > kMaxCmdDeltaYawAbs) {
                ST_LOG_ERR("终端输入的yaw增量非法: deltaYaw=%.10g (deg), 允许范围为[-%.10g, %.10g]。",
                           cmdDeltaYaw, kMaxCmdDeltaYawAbs, kMaxCmdDeltaYawAbs);
                return -1;
            }
            useCmdDeltaYaw = true;
            ST_LOG_INFO("使用命令行传入yaw增量模式: deltaYaw=%.10g (deg).", cmdDeltaYaw);
        } else {
            strYawFilePath = argv[1];
            ST_LOG_INFO("使用命令行输入的yaw文件路径: %s.", strYawFilePath.c_str());
        }
    } else {
        const char* pHomePath = std::getenv("HOME");
        if (NULL == pHomePath) {
            ST_LOG_ERR("获取HOME环境变量失败。");
            return -1;
        }

        strYawFilePath = std::string(pHomePath) +
                         "/tool_calibration/calibrate_yaw/calibr_results/results/baselidar/yaw.txt";
        ST_LOG_INFO("使用默认yaw文件路径: %s.", strYawFilePath.c_str());
    }

    COMM::CFG::CFG_PARAM_FILE_MNG_C objCfgParamFileMng;
    std::string strChangeCfgParamFilePath = objCfgParamFileMng.GetChangeCfgParamFilePath();
    std::string strWorkCfgParamFilePath   = objCfgParamFileMng.GetWorkCfgParamFilePath();
    ST_LOG_INFO("修改Change配置文件路径为: %s.", strChangeCfgParamFilePath.c_str());
    ST_LOG_INFO("修改Work配置文件路径为: %s.", strWorkCfgParamFilePath.c_str());

    TiXmlDocument objXmlCfgFile;
    if (0 != objCfgParamFileMng.ReadWorkCfgParamFile(strWorkCfgParamFilePath, objXmlCfgFile)) {
        ST_LOG_ERR("加载配置文件失败。");
        return -1;
    }

    TiXmlElement *pobjCfgParam = objXmlCfgFile.RootElement();
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("objXmlCfgFile.RootElement() Fail.");
        return -1;
    }

    COMM::CFG::CFG_AMEND_C objCfgAmend;
    if (!objCfgAmend.IsElementExist(*pobjCfgParam, {"LidarGroup", "Offset"})) {
        ST_LOG_ERR("配置文件中不存在<LidarGroup><Offset>节点。");
        return -1;
    }

    double offsetYaw = 0.0;
    if (useCmdDeltaYaw) {
        double currentOffsetYaw = 0.0;
        rslt = objCfgAmend.GetAttribute(*pobjCfgParam, {"LidarGroup", "Offset"}, "Yaw", currentOffsetYaw);
        if (0 != rslt) {
            ST_LOG_ERR("读取<LidarGroup><Offset Yaw>失败。");
            return -1;
        }

        offsetYaw = currentOffsetYaw + cmdDeltaYaw;
        ST_LOG_INFO("终端传参模式: currentOffsetYaw=%.10g, deltaYaw=%.10g, newOffsetYaw=%.10g.",
                    currentOffsetYaw, cmdDeltaYaw, offsetYaw);
    } else {
        if (!objCfgAmend.IsElementExist(*pobjCfgParam, {"LidarGroup", "MainLidar", "InstallParam"})) {
            ST_LOG_ERR("配置文件中不存在<LidarGroup><MainLidar><InstallParam>节点。");
            return -1;
        }

        double calibYaw = 0.0;
        if (0 != ReadYawFromFile(strYawFilePath, calibYaw)) {
            return -1;
        }
        ST_LOG_INFO("读取标定yaw成功: %.10g (deg), 文件: %s.", calibYaw, strYawFilePath.c_str());

        double mainLidarInstallYaw = 0.0;
        rslt = objCfgAmend.GetAttribute(*pobjCfgParam, {"LidarGroup", "MainLidar", "InstallParam"}, "Yaw", mainLidarInstallYaw);
        if (0 != rslt) {
            ST_LOG_ERR("读取<LidarGroup><MainLidar><InstallParam Yaw>失败。");
            return -1;
        }

        offsetYaw = calibYaw - mainLidarInstallYaw;
        ST_LOG_INFO("标定文件模式: calibYaw=%.10g, mainLidarInstallYaw=%.10g, newOffsetYaw=%.10g.",
                    calibYaw, mainLidarInstallYaw, offsetYaw);
    }

    const double rawOffsetYaw = offsetYaw;
    offsetYaw = NormalizeAngleDeg180(offsetYaw);
    offsetYaw = RoundTo4Decimals(offsetYaw);
    const std::string offsetYawStr = ToFixed4String(offsetYaw);
    ST_LOG_INFO("Offset Yaw归一化与保留4位小数: rawOffsetYaw=%.10g, normalizedOffsetYaw=%.10g, writeYaw=%s.",
                rawOffsetYaw, offsetYaw, offsetYawStr.c_str());

    rslt = objCfgAmend.SetAttribute(*pobjCfgParam, {"LidarGroup", "Offset"}, "Yaw", offsetYawStr);
    if (0 != rslt) {
        ST_LOG_ERR("修改<LidarGroup><Offset Yaw>失败。");
        return -1;
    }

    if (0 != objCfgParamFileMng.SaveCfgParamFile(strWorkCfgParamFilePath, objXmlCfgFile)) {
        ST_LOG_ERR("保存work配置文件失败。");
        return -1;
    }

    if (0 != objCfgParamFileMng.SaveCfgParamFile(strChangeCfgParamFilePath, objXmlCfgFile)) {
        ST_LOG_ERR("保存change配置文件失败。");
        return -1;
    }

    ST_LOG_INFO("修改配置文件成功, 最终offsetYaw=%.10g.", offsetYaw);
    return 0;
}
