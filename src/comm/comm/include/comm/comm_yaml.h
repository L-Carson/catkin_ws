#ifndef __COMM_YAML_H__
#define __COMM_YAML_H__

#include "comm/comm_dbg_log.h"
#include <yaml-cpp/yaml.h>

template <typename T>
void YamlReadAbort(const YAML::Node& yaml, const std::string& strKey, T& outVal)
{
    if (!yaml[strKey] || yaml[strKey].Type() == YAML::NodeType::Null) {
        ST_LOG_ERR("Not set <%s> value, Aborting!!!", strKey.c_str());
        exit(-1);
    } else {
        outVal = yaml[strKey].as<T>();
    }
}

template <typename T>
int YamlRead(const YAML::Node& yaml, const std::string& strKey, T& outVal, const T& defaultVal)
{
    if (!yaml[strKey] || yaml[strKey].Type() == YAML::NodeType::Null) {
        outVal = defaultVal;
        ST_LOG_ERR("Read <%s> fail.", strKey.c_str());
        return -1;
    } else {
        outVal = yaml[strKey].as<T>();
        return 0;
    }
}

bool YamlIsSubNodeExist(const YAML::Node& yaml, const std::string& strNode);
YAML::Node YamlSubNodeAbort(const YAML::Node& yaml, const std::string& strNode);
YAML::Node YamlFileLoad(const string& strFilePath);

#endif