#ifndef __COMM_YAML_CPP__
#define __COMM_YAML_CPP__

#include "comm/comm_yaml.h"

bool YamlIsSubNodeExist(const YAML::Node& yaml, const std::string& strNode)
{
    YAML::Node ret = yaml[strNode.c_str()];
    if (!ret) {
        ST_LOG_INFO("Subnode <%s> is not exist.", strNode.c_str());
        return false;
    }
    return true;
}

YAML::Node YamlSubNodeAbort(const YAML::Node& yaml, const std::string& strNode)
{
    YAML::Node ret = yaml[strNode.c_str()];
    if (!ret) {
        ST_LOG_ERR("Cannot find subnode <%s>. Aborting!!!", strNode.c_str());
        exit(-1);
    }
    return ret;
}

YAML::Node YamlFileLoad(const string& strFilePath)
{
    YAML::Node config;
    try {
        config = YAML::LoadFile(strFilePath);
    } catch (...) {
        ST_LOG_ERR("Failed to load the file: %s.", strFilePath.c_str());
        exit(-1);
    }

    if (!config) {
        ST_LOG_ERR("Failed to parse the file: %s.", strFilePath.c_str());
        exit(-1);
    }

    return config;
}

#endif