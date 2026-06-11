/** 
 * @file     config_manager.cpp
 * @brief    配置文件管理
 * @details
 * @author   Yakumo
 * @date     2024-07-29
 */

#ifndef S330_BASICS_CONFIG_MANAGER_H
#define S330_BASICS_CONFIG_MANAGER_H

#include <json/json.h>
#include <openssl/md5.h>
#include <cstring>
#include <iomanip>
#include <sys/stat.h>

#include "tinyxml.h"


/** 导航工作区配置文件路径*/
constexpr char kNavAlgoWorkConfigPath[] = "/map_config/cfg/algo_cfg_work";
constexpr char kNavAlgoWorkConfigFilePath[] = "/map_config/cfg/algo_cfg_work/cfg_param.xml";
/** 导航修改区配置文件路径*/
constexpr char kNavAlgoChangeConfigPath[] = "/map_config/cfg/algo_cfg_change";
constexpr char kNavAlgoChangeConfigFilePath[] = "/map_config/cfg/algo_cfg_change/cfg_param.xml";

class ConfigManager
{
public:
    ConfigManager();
    
    ~ConfigManager();
    /**
     * 拷贝构造函数
     * @param[in] signal
     */
    ConfigManager(const ConfigManager& signal){}
    /**
     * 赋值函数
     * @param[in] signal
     * @return
     */
    const ConfigManager& operator=(const ConfigManager& signal){return signal;}

    //外部使用接口
public:
    /**
     * 获取工作区配置文件路径
     * @return 配置路径(绝对路径)
     */
    static std::string GetNavAlgoWorkConfigPath();

    /**
     * 获取修改区配置文件路径
     * @return 配置路径(绝对路径)
     */
    static std::string GetNavAlgoChangeConfigPath();

     /**
     * 从导航配置文件中获取XML对象
     * @param[in]  filePath XML文件路径
     * @param[in]  doc      XML文档对象
     * @param[in] isCheckMd5   是否校验文件的MD5
     * @return 执行结果
     */
    static int ReadTiXmlElementFromNavCfg(TiXmlDocument &doc, std::string filePath, bool isCheckMd5 = false);

    /**
     * 将XML对象保存成导航配置文件
     * @param[in] doc      XML文档对象
     * @param[in] filePath XML文件路径
     * @return 执行结果
     */
    static int SaveTiXmlDocumentToNavCfg(TiXmlDocument &doc,std::string filePath);


    /**
     * 备份工作区配置文件
     * @return 0:成功 其他:失败
     */
    static int BackCupWorkAlgoConfigFile();


    //内部使用接口
private:

    /**
     * 16位MD5值转换成字符串形式
     * @param[in] md5 16位MD5值数组指针
     * @return 字符串形式MD5值
     */
    static std::string MD5ToHex(unsigned char *md5);

    /**
     * 将XML对象转换成文本
     * @param[in] elem Xml对象指针
     * @return 文本
     */
    static std::string ElementToString(TiXmlElement* elem);

    /**
     * 获取文本的MD5值
     * @param[in] str string指针
     * @return MD5文本(16字节,文本长度32)
     */
    static std::string GetMD5FromString(std::string &str);

    /**
     * 从文件中获取XML对象
     * @param[in]  filePath XML文件路径
     * @param[in]  doc      XML文档对象
     * @return 执行结果
     */
    static int ReadTiXmlElementFromFile(std::string filePath, TiXmlDocument &doc);

    /**
     * 将XML对象保存成文件
     * @param[in] doc      XML文档对象
     * @param[in] filePath XML文件路径
     * @return 执行结果
     */
    static int SaveTiXmlDocumentToFile(TiXmlDocument &doc,std::string filePath);

    /**
     * 获取当前时间字符串
     * @return 当前时间字符串("%Y%m%d-%H:%M:%S")
     */
    static std::string GetCurrentTimeString();

    /**
     * 拷贝文件
     * @param[in] old_path 文件路径
     * @param[in] new_path 备份路径
     * @return 执行结果
     */
    static int CopyFile(std::string old_path, std::string new_path);

    /**
     * 创建文件夹
     * @param[in] path 文件夹路径
     * @return 0:成功 其他:失败
     */
    static int MakeDir(std::string path);
};
#endif //S330_BASICS_CONFIG_MANAGER_H
