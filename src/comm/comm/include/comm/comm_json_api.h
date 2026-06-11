/*****************************************************************************
*  @brief                                                                    *
*  @author   Zenghui Luo                                                     *
*  @version  3.0                                                             *
*  @date     2022/11/15                                                      *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/01/29 | 1.0       | Zenghui Luo    | Create file                     *
*  2021/02/24 | 2.0       | Zenghui Luo    | Create file                     *
*  2022/11/15 | 3.0       | Zenghui Luo    | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#ifndef __COMMON_DEPEND__JSON_API_H__
#define __COMMON_DEPEND__JSON_API_H__

#include <iostream>
#include <list>
#include <vector>
#include <sys/file.h>
#include <unistd.h>

#include "loc_json/json.h"

namespace common_depend
{

// Json接口类
class JsonApiC
{
    public:
        // 构造函数
        JsonApiC(void);

        // 析构函数
        ~JsonApiC(void);

        /***********************************************************************************
         * 函数名：
         * 描述  ：得到根节点json对象
         * 输入  ：无
         * 输出  ：节点对象
         * 返回值 ：0:成功；其他：失败
         ************************************************************************************/
        int GetRootJsonObject(const std::string &file_name, Json::Value &root_value);

        /***********************************************************************************
         * 函数名：
         * 描述  ：读取文件
         * 输入  ：无
         * 输出  ：文件内容
         * 返回值 ：0:成功；1:配置不存在;
         ************************************************************************************/
        int ReadFile(const std::string &file_name, std::string &fileData);

        /***********************************************************************************
         * 函数名：
         * 描述  ：写入文件
         * 输入  ：文件内容
         * 输出  ：无
         * 返回值 ：0:成功；其他失败
         ************************************************************************************/
        int WriteFile(const std::string &file_name, std::string &fileData);
};

/**
 * @brief String转Json
 *
 * @param str
 * @param json
 * @return true
 * @return false
 */
inline bool StringToJson(const std::string &str, Json::Value &json)
{
    bool ret = false;
    Json::CharReaderBuilder rbuilder;
    // Json::CharReader *reader = rbuilder.newCharReader();
    std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
    rbuilder["collectcomments"] = true;
    rbuilder["emitUTF8"] = true;
    JSONCPP_STRING errs;
    // Json::Value root;

    do
    {
        json.clear();
        bool parse_ok = false;

        try
        {
            parse_ok = reader->parse((const char *)str.c_str(), (const char *)(str.c_str() + str.length()), &json, &errs);
        }
        catch (...)
        {
            // LOG_INFO_P(g_log_business_logic,"json格式解析抛异常");
        }

        if (parse_ok)
        {
        }
        else
        {
            // LOG_INFO_P(g_log_business_logic,"json格式解析失败：%s",errs.c_str());
            break;
        }

        if (!json.isObject())
        {
            break;
        }

        ret = true;
    } while (0);

    return ret;
}

/**
 * @brief Json转String
 *
 * @param json
 * @return std::string
 */
inline std::string JsonToString(Json::Value &json)
{
    Json::StreamWriterBuilder writerBuilder1;

    writerBuilder1.settings_["indentation"] = "";
    writerBuilder1.settings_["precision"] = 7;
    writerBuilder1.settings_["emitUTF8"] = true;

    std::ostringstream os;
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder1.newStreamWriter());
    jsonWriter->write(json, &os);
    return os.str();
}

} // namespace

#endif
