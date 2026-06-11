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
#include "comm/comm_json_api.h"

namespace common_depend
{

/**
 * @brief ：构造函数
 * 
 */
JsonApiC::JsonApiC(void)
{
}

/**
 * @brief 析构函数
 *
 */
JsonApiC::~JsonApiC(void)
{
}

/***********************************************************************************
 * 函数名：
 * 描述  ：得到根节点json对象
 * 输入  ：无
 * 输出  ：节点对象
 * 返回值 ：0:成功；其他：失败
 ************************************************************************************/
int JsonApiC::GetRootJsonObject(const std::string &file_name, Json::Value &root_value)
{
    int ret = 0;
    std::string fileData = "";

    do
    {
        ret = ReadFile(file_name, fileData);
        if (ret)
        {
            break;
        }

        Json::CharReaderBuilder rbuilder;
        JSONCPP_STRING errs;
        std::istringstream istream_str(fileData);
        rbuilder["collectComments"] = true;
        bool parse_ok = Json::parseFromStream(rbuilder, istream_str, &root_value, &errs);
        if (!parse_ok)
        {
            ret = -10;
            break;
        }

        ret = 0;
    } while (0);

    return ret;
}

/***********************************************************************************
 * 函数名：
 * 描述  ：读取文件
 * 输入  ：无
 * 输出  ：文件内容
 * 返回值 ：0:成功；1:配置不存在;
 ************************************************************************************/
int JsonApiC::ReadFile(const std::string &file_name, std::string &fileData)
{
    int ret = 0;
    FILE *f = NULL;
    int fileLen = 0;
    int rFileLen = 0;
    char *fileD = NULL;
    bool isLock = false;
    int no = -1;

    do
    {
        f = fopen(file_name.c_str(), "rb");
        if (!f)
        {
            ret = -1;
            break;
        }

        no = fileno(f);
        //加锁
        if (flock(no, LOCK_EX))
        {
            ret = -2;
            break;
        }
        isLock = true;

        //得到文件大小
        if (0 != fseek(f, 0, SEEK_END))
        {
            ret = -3;
            break;
        }

        fileLen = ftell(f);
        fileD = (char *)calloc(fileLen + 1, 1);
        if (0 != fseek(f, 0, SEEK_SET))
        {
            ret = -4;
            break;
        }

        rFileLen = fread(fileD, 1, fileLen, f);
        if (rFileLen != fileLen)
        {
            ret = -5;
            break;
        }

        fileData = fileD;
        ret = 0;
    } while (0);

    if (f)
    {
        fclose(f);
    }
    if (isLock)
    {
        //解锁
        flock(no, LOCK_UN);
    }
    if (fileD)
    {
        free(fileD);
    }

    return ret;
}

/***********************************************************************************
 * 函数名：
 * 描述  ：写入文件
 * 输入  ：文件内容
 * 输出  ：无
 * 返回值 ：0:成功；其他失败
 ************************************************************************************/
int JsonApiC::WriteFile(const std::string &file_name, std::string &fileData)
{
    int ret = 0;
    FILE *f = NULL;
    int fileLen = 0;
    int wFileLen = 0;
    bool isLock = false;
    int fileNo = -1;

    do
    {
        f = fopen(file_name.c_str(), "rb+"); //不写wb的原因是会清空文件，而清空文件的时候没有加锁会被其他进程读取到
        if (!f)
        {
            ret = -1;
            break;
        }

        fileNo = fileno(f);
        //加锁
        if (flock(fileNo, LOCK_EX))
        {
            ret = -2;
            break;
        }

        if (ftruncate(fileNo, 0))
        {
            ret = -3;
            break;
        }

        if (fseek(f, 0, SEEK_SET))
        {
            ret = -4;
            break;
        }

        // sleep(10);
        isLock = true;
        fileLen = fileData.length();
        wFileLen = fwrite(fileData.c_str(), 1, fileLen, f);
        if (wFileLen != fileLen)
        {
            ret = -5;
            break;
        }

        ret = 0;
    } while (0);

    if (f)
    {
        fclose(f);
    }
    if (isLock)
    {
        //解锁
        flock(fileNo, LOCK_UN);
    }
    return ret;
}

} // namespace
