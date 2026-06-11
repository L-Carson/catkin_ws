
#include "comm/comm_file.h"
#include "comm/comm_base.h"
#include "map_manager.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>  // 用于 _access
#else
    #include <unistd.h>
    #include <pwd.h>
#endif


using namespace std;

std::string FILE_MNG_C::GetUsrRootDir(void) const
{
    return strUsrRootDir_;
}

/**************************************************************************************
功能描述: 获取地图路径
修改记录:
**************************************************************************************/
void FILE_MNG_C::GetMapDir(string &strMapDir)
{
    strMapDir = strMapFileDir_;
}

/**************************************************************************************
功能描述: 获取保存路径的文件目录
修改记录:
**************************************************************************************/
void FILE_MNG_C::GetTraceFileDir(string &strFileDir)
{
    strFileDir = strTraceFileDir_;
}

void FILE_MNG_C::GetRouteFileDir(string &strFileDir)
{
    strFileDir = strRouteFileDir_;
}

void FILE_MNG_C::GetCfgFileDir(string &strFileDir)
{
    strFileDir = strCfgFileDir_;
}

void FILE_MNG_C::GetMachineDir(string &strFileDir)
{
    strFileDir = strMachineDir_;
}

void FILE_MNG_C::GetProductDir(string &strFileDir)
{
    strFileDir = strProductDir_;
}

void FILE_MNG_C::GetBoundaryFileDir(string &strFileDir)
{
    strFileDir = strBoundaryFileDir_;
}

void FILE_MNG_C::GetCameraConfigDir(string & strFileDir)
{
    strFileDir = strCameraConfigDir_;
}

void FILE_MNG_C::GetVxlFilterFileDir(string & strFileDir)
{
    strFileDir = strVxlFilterFileDir_;
}

void FILE_MNG_C::GetPnpLayerDir(string &strFileDir)
{
    strFileDir = strPnpLayerDir_;
}

string FILE_MNG_C::GetWorkSceneDir(int argc, char** argv) const
{
    string strParamBegin = WORK_SCENE_DIR_PARAM_PREFIX_STR;

    for (int loop = 0; loop < argc; ++loop) {
        R_ASSERT(argv[loop] != NULL, string(""));
        string strParam = argv[loop];

        if (0 == strParam.find_first_of(strParamBegin)) {
            return strParam.substr(strParamBegin.size());
        }
    }

    ST_LOG_INFO("No Input Work Scene Dir. Get Default Dir.");
    return strWorkSceneDir_;
}

string FILE_MNG_C::GetWorkSceneDir(void) const
{
    return strWorkSceneDir_;
}

string FILE_MNG_C::GetModelFileDir(void) const
{
    return strModelFileDir_;
}

void FILE_MNG_C::GetLocationMapDir(string &strDir) const
{
    strDir = strLocationMapDir_;
}

void FILE_MNG_C::GetLogFileDir(string &strDir) const
{
    strDir = strLogFileDir_;
}

INT32 FILE_MNG_C::GetLoginUserName(string &strUserName)
{
#ifdef _WIN32
    // Windows 实现
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    
    if (!GetUserNameA(username, &username_len)) {
        ST_LOG_ERR("GetUserName() Fail. Error code: %lu", GetLastError());
        return -1;
    }
    
    if (strlen(username) > 100) {
        ST_LOG_ERR("Username Len(%u) Err.", static_cast<UINT32>(strlen(username)));
        return -1;
    }
    
    strUserName = username;
    ST_LOG_INFO("UsrName: %s.", username);
    return 0;
#else
    // Linux/Unix 实现
    uid_t userid = getuid();
    struct passwd *pwd = getpwuid(userid);
    
    if ((pwd == NULL) || (pwd->pw_name == NULL)) {
        ST_LOG_ERR("getpwuid() Fail.");
        return -1;
    }

    if (strlen(pwd->pw_name) > 100) {
        ST_LOG_ERR("pwd->pw_name Len(%u) Err.", static_cast<UINT32>(strlen(pwd->pw_name)));
        return -1;
    }

    strUserName = pwd->pw_name;
    ST_LOG_INFO("UsrName: %s.", pwd->pw_name);
    return 0;
#endif
}

string FILE_OPRT_C::ToPureDir(const string &strSrcDir)
{
    string strTgtDir = strSrcDir;

    if (strTgtDir.empty()) {
        return strTgtDir;
    }

#ifdef _WIN32
    char separator = '\\';
#else
    char separator = '/';
#endif

    // 移除末尾的所有路径分隔符
    while ((strTgtDir.length() > 0) && 
           (strTgtDir[strTgtDir.length() - 1] == separator ||
            strTgtDir[strTgtDir.length() - 1] == '/' ||  // 处理混合路径
            strTgtDir[strTgtDir.length() - 1] == '\\')) {
        strTgtDir.erase(strTgtDir.length() - 1, 1);
    }

    return strTgtDir;
}

string FILE_OPRT_C::ToStyledDir(const string &strSrcDir)
{
#ifdef _WIN32
    char separator = '\\';
#else
    char separator = '/';
#endif
    
    return ToPureDir(strSrcDir) + separator;
}

/**************************************************************************************
功能描述: 判断文件或者目录是否存在
修改记录:
**************************************************************************************/
bool FILE_OPRT_C::IsExist(const string &strPath)
{
#ifdef _WIN32
    // Windows 实现
    DWORD fileAttr = GetFileAttributesA(strPath.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        DWORD error = GetLastError();
        return (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) ? false : true;
    }
    return true;
#else
    // Linux/Unix 实现
    return (access(strPath.c_str(), F_OK) == 0);
#endif
}

/**************************************************************************************
功能描述: 修改文件名，修改成功返回0，失败返回-1
          注意，调用后目标老文件会被删除
修改记录:
**************************************************************************************/
int FILE_OPRT_C::Rename(const string &strOldFilePath, const string &strNewFilePath, bool isCoverFile = false)
{
    if (strOldFilePath == strNewFilePath) {
        ST_LOG_ERR("Old File Is Same With New File.");
        return 0;
    }

    if (!IsExist(strOldFilePath)) {
        ST_LOG_ERR("Old File(%s) Not Exist.", strOldFilePath.c_str());
        return -1;
    }

    if (isCoverFile && IsExist(strNewFilePath)) {
        ST_LOG_INFO("New File(%s) Exist And To Del.", strNewFilePath.c_str());
        DelFile(strNewFilePath);
    }

    return rename(strOldFilePath.c_str(), strNewFilePath.c_str());
}

/**************************************************************************************
功能描述: 删除文件
修改记录:
**************************************************************************************/
int FILE_OPRT_C::DelFile(const string &strFilePath)
{
#ifdef _WIN32
    // Windows 实现
    if (DeleteFileA(strFilePath.c_str())) {
        return 0;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) {
            return 0;  // 文件不存在，认为删除成功
        }
        ST_LOG_ERR("DeleteFile(%s) Fail. Error: %lu", strFilePath.c_str(), error);
        return -1;
    }
#else
    INT32 rslt = remove(strFilePath.c_str());
    if (rslt != 0) {
        ST_LOG_ERR("remove(%s) Fail.", strFilePath.c_str());
        return -1;
    }

    return 0;
#endif
}

/**************************************************************************************
功能描述: 删除目录
修改记录:
**************************************************************************************/
int  FILE_OPRT_C::DelDir(const string &strDirPath)
{
    INT32 rslt = DelDirContent(strDirPath);
    if (rslt != 0) {
        ST_LOG_ERR("DelDirContent(%s) Fail.", strDirPath.c_str());
        return -1;
    }

#ifdef _WIN32
    if (RemoveDirectoryA(strDirPath.c_str())) {
        return 0;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) {
            // 目录已不存在，认为删除成功
            return 0;
        }
        ST_LOG_ERR("RemoveDirectory(%s) Fail. Error: %lu", strDirPath.c_str(), error);
        return -1;
    }
#else
    rmdir(strDirPath.c_str());
    return 0;
#endif   
}

int FILE_OPRT_C::DelDirContent(const string &strDirPath)
{
    string strPureDirPath = ToPureDir(strDirPath);
    DIR* pstDir = opendir(strPureDirPath.c_str());
    if (pstDir == NULL) {
        ST_LOG_ERR("opendir(%s) Fail.", strPureDirPath.c_str());
        return -1;
    }

    struct dirent *pstObjInfo;
    while ((pstObjInfo = readdir(pstDir)) != NULL) {
        if ((strcmp(pstObjInfo->d_name, ".") == 0) || (strcmp(pstObjInfo->d_name, "..") == 0)) {
            continue;
        }

        string strSubObjPath = strPureDirPath + "/" + pstObjInfo->d_name;
        INT32 rslt = DelDirOrFile(strSubObjPath);
        if (rslt != 0) {
            ST_LOG_ERR("DelDirOrFile(%s) Fail.", strSubObjPath.c_str());
            closedir(pstDir);
            return -1;
        }
    }

    closedir(pstDir);
    return 0;
}

int FILE_OPRT_C::DelDirOrFile(const string &strPath)
{
#ifdef _WIN32
    // Windows 实现
    DWORD fileAttr = GetFileAttributesA(strPath.c_str());
    
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) {
            // 文件或目录不存在，直接返回成功
            return 0;
        }
        ST_LOG_ERR("GetFileAttributes(%s) Fail. Error: %lu", strPath.c_str(), error);
        return -1;
    }
    
    if (fileAttr & FILE_ATTRIBUTE_DIRECTORY) {
        // 是目录
        int rslt = DelDir(strPath);
        if (rslt != 0) {
            ST_LOG_ERR("DelDir(%s) Fail.", strPath.c_str());
            return -1;
        }
    } else {
        // 是文件
        int rslt = DelFile(strPath);
        if (rslt != 0) {
            ST_LOG_ERR("DelFile(%s) Fail.", strPath.c_str());
            return -1;
        }
    }
#else
    struct stat statbuf;
    INT32 rslt = lstat(strPath.c_str(), &statbuf);
    if (rslt != 0) {
        ST_LOG_ERR("lstat(%s) Fail.", strPath.c_str());
        return -1;
    }

    if (S_ISREG(statbuf.st_mode)) {
        rslt = DelFile(strPath);
        if (rslt != 0) {
            ST_LOG_ERR("DelFile(%s) Fail.", strPath.c_str());
            return -1;
        }
    } else if (S_ISDIR(statbuf.st_mode)) {
        rslt = DelDir(strPath);
        if (rslt != 0) {
            ST_LOG_ERR("DelDir(%s) Fail.", strPath.c_str());
            return -1;
        }
    } else {
        ST_LOG_ERR("Obj(%s) Attr Err.", strPath.c_str());
        return -1;
    }
#endif

    return 0;
}

int  FILE_OPRT_C::MakeDir(const string &strDirPath)
{
    if (strDirPath.empty()) {
        ST_LOG_ERR("Directory path is empty");
        return -1;
    }

#ifdef _WIN32
    // Windows 实现
    // 统一路径分隔符为 Windows 格式
    string normalizedPath = strDirPath;
    std::replace(normalizedPath.begin(), normalizedPath.end(), '/', '\\');
    
    // 递归创建目录
    string currentPath;
    size_t pos = 0;
    
    // 处理盘符（如 C:）
    if (normalizedPath.length() >= 2 && normalizedPath[1] == ':') {
        currentPath = normalizedPath.substr(0, 2);
        pos = 2;
    }
    
    while (pos <= normalizedPath.length()) {
        // 找到下一个分隔符
        size_t nextPos = normalizedPath.find('\\', pos);
        if (nextPos == string::npos) {
            currentPath = normalizedPath;
            pos = normalizedPath.length() + 1;
        } else {
            currentPath = normalizedPath.substr(0, nextPos);
            pos = nextPos + 1;
        }
        
        if (currentPath.empty() || currentPath == ".") {
            continue;
        }
        
        // 检查目录是否已存在
        if (IsExist(currentPath)) {
            continue;
        }
        
        // 创建目录
        int rslt = _mkdir(currentPath.c_str());
        if (rslt != 0) {
            int err = errno;
            if (err != EEXIST) {  // 目录已存在不算错误
                ST_LOG_ERR("Create Dir %s Of %s Fail. Error: %d", 
                          currentPath.c_str(), strDirPath.c_str(), err);
                return -1;
            }
        }
    }
#else

    UINT32 dirLen  = strDirPath.length();

     /* 递归创建 */
    UINT32 procDirLen = 0;
    while (procDirLen < dirLen) {
        UINT32 idx = strDirPath.find('/', procDirLen);

        procDirLen = MIN(dirLen, idx + 1);

        string strProcDir = strDirPath.substr(0, procDirLen);
        if (IsExist(strProcDir)) {
            continue;
        }

        int rslt = mkdir(strProcDir.c_str(), ACCESSPERMS);
        if (rslt != 0) {
            ST_LOG_ERR("Create Dir %s Of %s Fail.", strProcDir.c_str(), strDirPath.c_str());
            return -1;
        }
    }
#endif

    return 0;
}

/**************************************************************************************
功能描述: 复制文件
修改记录:
**************************************************************************************/
int FILE_OPRT_C::Copy(const string &strSrcFilePath, const string &strTgtFilePath, bool isCoverFile = false)
{
    if (strSrcFilePath == strTgtFilePath) {
        ST_LOG_ERR("Src File Is Same With Tgt File.");
        return 0;
    }

    if (!IsExist(strSrcFilePath)) {
        ST_LOG_ERR("Src File(%s) Not Exist.", strSrcFilePath.c_str());
        return -1;
    }

    if (isCoverFile && IsExist(strTgtFilePath)) {
        ST_LOG_INFO("Tgt File(%s) Exist And To Del.", strTgtFilePath.c_str());
        DelFile(strTgtFilePath);
    }

    string strCmd = "cp -f " + strSrcFilePath + " " + strTgtFilePath;
    SYSTEM_EXEC(strCmd.c_str());

    return 0;
}

/**************************************************************************************
功能描述: 将文件缓存区内容输出到文件
修改记录:
**************************************************************************************/
void FILE_OPRT_C::SyncBuffToFile(FILE *pf)
{
    if (pf != NULL) {
        fflush(pf);
    }
}

/**************************************************************************************
功能描述: 将内存区文件强制输出到硬盘
修改记录:
**************************************************************************************/
void FILE_OPRT_C::SyncFileToDisk(FILE *pf)
{
#ifndef WIN32
    if (pf != NULL) {
        fsync(fileno(pf));
    }
#endif
}

/**************************************************************************************
功能描述: 从路径中获取文件名
修改记录:
**************************************************************************************/
void FILE_OPRT_C::GetFileNameFrmPath(const string &strFilePath, string &strFileName)
{
    int fileNamePos = strFilePath.find_last_of("/\\");   /* 包含linux和win的 */
    if (fileNamePos < 0) {
        fileNamePos = 0;        /* 没找到说明没有包含路径 */
    } else {
        ++fileNamePos;          /* 有路径时跳过分隔符 */
    }

    strFileName = strFilePath.substr(fileNamePos);
}

/**************************************************************************************
功能描述: 从路径中获取文件目录
修改记录:
**************************************************************************************/
void FILE_OPRT_C::GetFileDirFrmPath(const string &strFilePath, string &strFileDir)
{
    int fileNamePos = strFilePath.find_last_of("/\\");   /* 包含linux和win的 */
    if (fileNamePos < 0) {
        fileNamePos = 0;
    }

    strFileDir = strFilePath.substr(0, fileNamePos);
}

INT32 FILE_OPRT_C::GetFilesOfDir(const std::string &strDirPath, std::vector<std::string> &vectFileName)
{
    DIR *pstDir = opendir(strDirPath.c_str());
    if (pstDir == nullptr) {
        ST_LOG_ERR("opendir(%s) Fail.", strDirPath.c_str());
        return -1;
    }

    struct dirent *pstDirEntry;
    while ((pstDirEntry = readdir(pstDir)) != nullptr) {
        // 排除当前目录(".")和上级目录("..")
        if (strcmp(pstDirEntry->d_name, ".") == 0 || strcmp(pstDirEntry->d_name, "..") == 0) {
            continue;
        }

        vectFileName.emplace_back(pstDirEntry->d_name);
    }

    closedir(pstDir);
    return 0;
}

BOOL FILE_OPRT_C::IsLinkFile(const std::string &strLinkFilePath)
{
    if (!IsExist(strLinkFilePath)) {
        return false;
    }

    struct stat stFileInfo;
    if (lstat(strLinkFilePath.c_str(), &stFileInfo) == -1) {
        return false;
    }

    return S_ISLNK(stFileInfo.st_mode);
}

INT32 FILE_OPRT_C::MakeLink(const std::string &strLinkFilePath, const std::string &strTgtFilePath)
{
    /* 先强制删除老的文件 */
    if (IsExist(strLinkFilePath)) {
        INT32 rslt = DelFile(strLinkFilePath);
        if (rslt != 0) {
            ST_LOG_ERR("DelFile(%s) Fail.", strLinkFilePath.c_str());
            return -1;
        }
    }

    if (!IsExist(strTgtFilePath)) {
        ST_LOG_ERR("Tgt File %s Not Exist.", strTgtFilePath.c_str());
        return -1;
    }

    std::string strCmd = "ln -s " + strTgtFilePath + " " + strLinkFilePath;
    SYSTEM_EXEC(strCmd.c_str());

    if (!IsExist(strLinkFilePath)) {
        ST_LOG_ERR("Link %s To %s Fail.", strLinkFilePath.c_str(), strTgtFilePath.c_str());
        return -1;
    }

    return 0;
}

INT32 FILE_OPRT_C::GetLinkTgtFilePath(const std::string &strLinkFilePath, std::string &strTgtFilePath)
{
    if (!IsLinkFile(strLinkFilePath)) {
        ST_LOG_ERR("IsLinkFile(%s) False.", strLinkFilePath.c_str());
        return -1;
    }

    char acTgtFilePath[512];
    ssize_t lenOfTgtFilePathStr = readlink(strLinkFilePath.c_str(), acTgtFilePath, ARRAY_SIZE(acTgtFilePath) - 1);
    if (lenOfTgtFilePathStr <= 0) {
        ST_LOG_ERR("readlink(%s) Fail.", strLinkFilePath.c_str());
        return -1;
    }

    R_ASSERT_2(lenOfTgtFilePathStr < UINT32(ARRAY_SIZE(acTgtFilePath)), -1, lenOfTgtFilePathStr, ARRAY_SIZE(acTgtFilePath));
    acTgtFilePath[lenOfTgtFilePathStr] = '\0';

    strTgtFilePath = std::string(acTgtFilePath);
    return 0;
}

std::recursive_mutex   CAR_TYPE_PARSER_C::instance_mutex_;
CAR_TYPE_PARSER_C::Ptr CAR_TYPE_PARSER_C::p_instance_ = nullptr;

// 构造函数
CAR_TYPE_PARSER_C::CAR_TYPE_PARSER_C(void)
{
    if (LoadRobotCfgFile(file_relative_name_, type_info_))
    {
        ST_LOG_INFO("%s: 机型[%s]", parser_name_.data(), type_info_.robot_type.data());
    }
}

// 析构函数
CAR_TYPE_PARSER_C::~CAR_TYPE_PARSER_C(void)
{
}

// 加载机器人配置文件
bool CAR_TYPE_PARSER_C::LoadRobotCfgFile(const std::string &rel_file_name,
                                         RobotTypeCfgSt &type_info)
{
    // 读取json文件
    Json::Value root_value;
    std::string abs_file_name;
    if (ReadJsonFile(rel_file_name, root_value, abs_file_name) < 0)
    {
        ST_LOG_WARN("%s: [室内线机型配置信息]文件读取失败[%s]", parser_name_.data(), abs_file_name.c_str());
        ST_LOG_INFO("%s: 当前为室外线机型! ", parser_name_.data());
        return false;
    }

    int par_ret = ParsingRobotInfoCfg(root_value["robot_info"], type_info);
    if (par_ret < 0)
    {
        ST_LOG_ERR("%s: 机器人配置信息解析失败! 异常类型[%d] 文件[%s]",
                   parser_name_.data(),
                   par_ret, abs_file_name.data());
        return false;
    }

    return true;
}

// 读取json文件
int CAR_TYPE_PARSER_C::ReadJsonFile(const std::string &rel_file_name,
                                    Json::Value &json_value,
                                    std::string &abs_file_name)
{
    FILE_MNG_C objFileMng;
    abs_file_name = objFileMng.GetUsrRootDir() + rel_file_name;

    common_depend::JsonApiC json_api;
    int api_ret = json_api.GetRootJsonObject(abs_file_name, json_value);
    if (api_ret)
    {
        return -2;
    }
    return 0;
}

// 解析机器人信息配置
int CAR_TYPE_PARSER_C::ParsingRobotInfoCfg(Json::Value &json_value,
                                           RobotTypeCfgSt &type_info)
{
    if (!json_value.isObject())
    {
        return -1;
    }

    if (!ParsingTypeInfo(json_value, type_info))
    {
        return -2;
    }

    return 0;
}

// 解析型号信息
bool CAR_TYPE_PARSER_C::ParsingTypeInfo(Json::Value &json_value,
                                        RobotTypeCfgSt &type_info)
{
    // 机型
    if (json_value["robot_type"].isString())
    {
        type_info.robot_type = json_value["robot_type"].asString();
    }
    else
    {
        return false;
    }

    return true;
}

// 输出当前机型信息
const RobotTypeCfgSt &CAR_TYPE_PARSER_C::GetRobotTypeInfo(void) const
{
    return type_info_;
}

// 输出当前机型名
const std::string &CAR_TYPE_PARSER_C::GetRobotType(void) const
{
    return type_info_.robot_type;
}

// 是否为室外清扫车
bool CAR_TYPE_PARSER_C::IsOutdoorSweeper(void) const
{
    if (type_info_.robot_type.empty())
    {
        return true;
    }
    return false;
}

/**************************************************************************************
功能描述: 获取地图目录
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetMapDir(string &strMapDir)
{
    strMapDir = strMapDir_;
}

/**************************************************************************************
功能描述: 获取地图路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetMapPath(const string &strMapName, string &strMapPath)
{
    strMapPath = strMapDir_ + strMapName;
}

/**************************************************************************************
功能描述: 获取地图路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetMapFilePath(const string &strMapFileName, string &strMapFilePath)
{
    strMapFilePath = strMapDir_ + strMapFileName;
}

/**************************************************************************************
功能描述: 获取组合地图文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetCombMapFileName(string &strCombMapFileName)
{
    strCombMapFileName = strCombMapFileName_;
}

/**************************************************************************************
功能描述: 获取组合地图文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetCombMapFilePath(string &strCombMapFilePath)
{
    string strCombMapFileName;
    GetCombMapFileName(strCombMapFileName);

    GetMapFilePath(strCombMapFileName, strCombMapFilePath);
}

/**************************************************************************************
功能描述: 获取地图拆分配置文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetSplitCfgFileName(const string &strSrcMapName, string &strSplitCfgFileName)
{
    strSplitCfgFileName = strSrcMapName + strSplitCfgFileExName_;
}

/**************************************************************************************
功能描述: 获取地图拆分配置文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetSplitCfgFilePath(const string &strSrcMapName, string &strSplitCfgFilePath)
{
    string strSplitCfgFileName;
    GetSplitCfgFileName(strSrcMapName, strSplitCfgFileName);

    GetMapFilePath(strSplitCfgFileName, strSplitCfgFilePath);
}

void MAP_FILE_MNG_C::GetMapExYamlFilePath(string &strMapExYamlFilePath)
{
    GetMapFilePath(strMapExYamlFileName_, strMapExYamlFilePath);
}

/**************************************************************************************
功能描述: 根据子地图名称
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetSubMapName(UINT32 subMapNo, string &strSubMapName)
{
    char strSubMapNo[50];
    sprintf(strSubMapNo, "%u", subMapNo);

    strSubMapName = strDefautMapName_ + strSubMapNo;
}

/**************************************************************************************
功能描述: 获取子地图编号
修改记录:
**************************************************************************************/
INT32 MAP_FILE_MNG_C::GetSubMapNo(string &strSubMapName)
{
    R_ASSERT(strSubMapName.length() > strDefautMapName_.length(), -1);
    string strMapNo = strSubMapName.substr(strDefautMapName_.length());

    UINT32 subMapNo;
    if (1 != sscanf(strMapNo.c_str(), "%u", &subMapNo)) {
        return -1;
    }

    return (int)subMapNo;
}

/**************************************************************************************
功能描述: 获取子地图路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetSubMapPath(UINT32 subMapNo, string &strSubMapPath)
{
    string strSubMapName;
    GetSubMapName(subMapNo, strSubMapName);

    GetMapPath(strSubMapName, strSubMapPath);
}

/**************************************************************************************
功能描述: 获取默认地图名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetDefaultMapName(string &strDefaultMapName)
{
    strDefaultMapName.assign(strDefautMapName_);
}

/**************************************************************************************
功能描述: 获取默认地图YAML文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetDefaultYamlFileName(string &strDefaultYamlFileName)
{
    strDefaultYamlFileName = strDefautMapName_ + strMapYamlSuffix_;
}

/**************************************************************************************
功能描述: 获取默认地图YAML文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetDefaultYamlFilePath(string &strDefaultYamlFilePath)
{
    string strDefaultYamlFileName;
    GetDefaultYamlFileName(strDefaultYamlFileName);

    GetMapFilePath(strDefaultYamlFileName, strDefaultYamlFilePath);
}

/**************************************************************************************
功能描述: 获取默认地图PGM文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetDefaultPgmFileName(string &strDefaultPgmFileName)
{
    strDefaultPgmFileName = strDefautMapName_ + strMapPgmSuffix_;
}

/**************************************************************************************
功能描述: 获取默认地图PGM文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetDefaultPgmFilePath(string &strDefaultPgmFilePath)
{
    string strDefaultPgmFileName;
    GetDefaultPgmFileName(strDefaultPgmFileName);

    GetMapFilePath(strDefaultPgmFileName, strDefaultPgmFilePath);
}

/**************************************************************************************
功能描述: 获取YAML文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetYamlFileName(const string &strMapName, string &strYamlFileName)
{
    strYamlFileName = strMapName + strMapYamlSuffix_;
}

/**************************************************************************************
功能描述: 获取YAML文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetYamlFilePath(const string &strMapName, string &strYamlFilePath)
{
    string strYamlFileName;
    GetYamlFileName(strMapName, strYamlFileName);

    GetMapFilePath(strYamlFileName, strYamlFilePath);
}

/**************************************************************************************
功能描述: 获取PGM文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetPgmFileName(const string &strMapName, string &strPgmFileName)
{
    strPgmFileName = strMapName + strMapPgmSuffix_;
}

/**************************************************************************************
功能描述: 获取PGM文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetPgmFilePath(const string &strMapName, string &strPgmFilePath)
{
    string strPgmFileName;
    GetPgmFileName(strMapName, strPgmFileName);

    GetMapFilePath(strPgmFileName, strPgmFilePath);
}

/**************************************************************************************
功能描述: 获取地图的临时名称
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetTmpMapName(const string &strMapName, string &strTmpMapName)
{
    strTmpMapName = strMapName + strTmpFileExName_;
}

/**************************************************************************************
功能描述: 获取yaml临时文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetTmpYamlFileName(const string &strMapName, string &strTmpYamlFileName)
{
    string strTmpMapName;
    GetTmpMapName(strMapName, strTmpMapName);

    strTmpYamlFileName = strTmpMapName + strMapYamlSuffix_;
}

/**************************************************************************************
功能描述: 获取yaml临时文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetTmpYamlFilePath(const string &strMapName, string &strTmpYamlFilePath)
{
    string strTmpYamlFileName;
    GetTmpYamlFileName(strMapName, strTmpYamlFileName);

    GetMapFilePath(strTmpYamlFileName, strTmpYamlFilePath);
}

/**************************************************************************************
功能描述: 获取pgm文件名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetTmpPgmFileName(const string &strMapName, string &strTmpPgmFileName)
{
    string strTmpMapName;
    GetTmpMapName(strMapName, strTmpMapName);

    strTmpPgmFileName = strTmpMapName + strMapPgmSuffix_;
}

/**************************************************************************************
功能描述: 获取pgm文件路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetTmpPgmFilePath(const string &strMapName, string &strTmpPgmFilePath)
{
    string strTmpPgmFileName;
    GetTmpPgmFileName(strMapName, strTmpPgmFileName);

    GetMapFilePath(strTmpPgmFileName, strTmpPgmFilePath);
}

/**************************************************************************************
功能描述: 获取虚拟障碍地图名
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetVirtualObstMapName(UINT32 subMapNo, string &strVirtualObstMapName)
{
    string strSubMapName;
    GetSubMapName(subMapNo, strSubMapName);

    strVirtualObstMapName = strSubMapName + strVirtualObstMapExName_;
}

/**************************************************************************************
功能描述: 获取虚拟障碍地图路径
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetVirtualObstMapPath(UINT32 subMapNo, string &strVirtualObstMapPath)
{
    string strVirtualObstMapName;
    GetVirtualObstMapName(subMapNo, strVirtualObstMapName);

    GetMapPath(strVirtualObstMapName, strVirtualObstMapPath);
}

/**************************************************************************************
功能描述: 获取仿真障碍地图
修改记录:
**************************************************************************************/
void MAP_FILE_MNG_C::GetSimulaitonObstMapPath(string &strObstMapPath)
{
    string strSimulationObstMapName;
    strSimulationObstMapName = strDefautMapName_ + strSimulationObstMapExName_;

    GetMapPath(strSimulationObstMapName, strObstMapPath);
}

/**************************************************************************************
功能描述: 判断地图是否存在
修改记录:
**************************************************************************************/
bool MAP_FILE_MNG_C::isMapPathExist(const string &strMapPath)
{
    string strYamlFilePath = strMapPath + strMapYamlSuffix_;
    string strPgmFilePath  = strMapPath + strMapPgmSuffix_;

    /* yaml和pgm同时存在才认为地图存在 */
    return (objFileOprt_.IsExist(strYamlFilePath) && objFileOprt_.IsExist(strPgmFilePath));
}

/**************************************************************************************
功能描述: 重命名地图
          注意: 调用后如果失败，文件状态不确定
修改记录:
**************************************************************************************/
INT32 MAP_FILE_MNG_C::RenameMap(const string &strSrcMapName, const string &strTgtMapName)
{
    DelMap(strTgtMapName);

    if (0 != RenamePgmFile(strSrcMapName, strTgtMapName)) {
        ST_LOG_ERR("RenamePgmFile(%s, %s) Fail.", strSrcMapName.c_str(), strTgtMapName.c_str());
        return -1;
    }

    if (0 != RenameYamlFile(strSrcMapName, strTgtMapName)) {
        ST_LOG_ERR("RenameYamlFile(%s, %s) Fail.", strSrcMapName.c_str(), strTgtMapName.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 复制地图
修改记录:
**************************************************************************************/
INT32 MAP_FILE_MNG_C::CopyMap(const string &strSrcMapName, const string &strTgtMapName)
{
    DelMap(strTgtMapName);

    /* 复制pgm文件 */
    if (0 != CopyPgmFile(strSrcMapName, strTgtMapName)) {
        DelMap(strTgtMapName);
        ST_LOG_ERR("CopyPgmFile(%s, %s) Fail.", strSrcMapName.c_str(), strTgtMapName.c_str());
        return -1;
    }

    /* 复制yaml文件 */
    if (0 != CopyYamlFile(strSrcMapName, strTgtMapName)) {
        DelMap(strTgtMapName);
        ST_LOG_ERR("CopyYamlFile(%s, %s) Fail.", strSrcMapName.c_str(), strTgtMapName.c_str());
        return -1;
    }

    return 0;
}

void MAP_FILE_MNG_C::DelMap(const string &strMapName)
{
    DelPgmFile(strMapName);
    DelYamlFile(strMapName);
}

INT32 MAP_FILE_MNG_C::RenamePgmFile(const string &strSrcMapName, const string &strTgtMapName)
{
    /* pgm文件直接修改文件名即可 */
    string strSrcPgmFilePath;
    GetPgmFilePath(strSrcMapName, strSrcPgmFilePath);

    string strTgtPgmFilePath;
    GetPgmFilePath(strTgtMapName, strTgtPgmFilePath);

    if (0 != objFileOprt_.Rename(strSrcPgmFilePath, strTgtPgmFilePath, true)) {
        ST_LOG_ERR("objFileOprt_.Rename(%s, %s) Fail.", strSrcPgmFilePath.c_str(), strTgtPgmFilePath.c_str());
        return -1;
    }

    return 0;
}

INT32 MAP_FILE_MNG_C::RenameYamlFile(const string &strSrcMapName, const string &strTgtMapName)
{
    /* yaml文件需要修改文件类容，不能直接修改文件名 */
    if (0 != CopyYamlFile(strSrcMapName, strTgtMapName)) {
        ST_LOG_ERR("CopyYamlFile(%s, %s) Fail.", strSrcMapName.c_str(), strTgtMapName.c_str());
        return -1;
    }

    DelYamlFile(strSrcMapName);
    return 0;
}

INT32 MAP_FILE_MNG_C::CopyPgmFile(const string &strSrcMapName, const string &strTgtMapName)
{
    string strSrcPgmFilePath;
    GetPgmFilePath(strSrcMapName, strSrcPgmFilePath);

    string strTgtPgmFilePath;
    GetPgmFilePath(strTgtMapName, strTgtPgmFilePath);

    if (0 != objFileOprt_.Copy(strSrcPgmFilePath, strTgtPgmFilePath, true)) {
        ST_LOG_ERR("Copy Src(%s) Pgm To Tgt(%s) Fail.", strSrcPgmFilePath.c_str(), strTgtPgmFilePath.c_str());
        return -1;
    }

    return 0;
}

INT32 MAP_FILE_MNG_C::CopyYamlFile(const string &strSrcMapName, const string &strTgtMapName)
{
    string strSrcYamlFilePath;
    GetYamlFilePath(strSrcMapName, strSrcYamlFilePath);
    FILE *pfSrcYamlFile = fopen(strSrcYamlFilePath.c_str(), "r");
    if (pfSrcYamlFile == NULL) {
        ST_LOG_ERR("Open Src Yaml File(%s) Fail.", strSrcYamlFilePath.c_str());
        return -1;
    }

    string strTgtYamlFilePath;
    GetYamlFilePath(strTgtMapName, strTgtYamlFilePath);
    FILE *pfTgtYamlFile = fopen(strTgtYamlFilePath.c_str(), "w");
    if (pfTgtYamlFile == NULL) {
        fclose(pfSrcYamlFile);
        ST_LOG_ERR("Open Tgt File(%s) Fail.", strTgtYamlFilePath.c_str());
        return -1;
    }

    string strSrcPgmFileName;
    GetPgmFileName(strSrcMapName, strSrcPgmFileName);
    BOOL isExistSrcPgmFileName = false;
    char acLine[512];
    while (NULL != fgets(acLine, sizeof(acLine), pfSrcYamlFile)) {
        string strLine(acLine);
        if (strLine.length() >= (sizeof(acLine) - 1)) {
            fclose(pfSrcYamlFile);
            fclose(pfTgtYamlFile);
            ST_LOG_ERR("Line(%s) Is longer Than Buff.", acLine);
            return -1;
        }

        UINT32 idx = strLine.find(strSrcPgmFileName);
        if (string::npos != idx) {
            isExistSrcPgmFileName = true;

            string strTgtPgmFileName;
            GetPgmFileName(strTgtMapName, strTgtPgmFileName);
            strLine.replace(idx, strSrcPgmFileName.length(), strTgtPgmFileName, 0, strTgtPgmFileName.length());
        }

        fputs(strLine.c_str(), pfTgtYamlFile);
    }

    fclose(pfSrcYamlFile);
    fclose(pfTgtYamlFile);

    if (!isExistSrcPgmFileName) {
        ST_LOG_ERR("Src Yaml File(%s) Not Exit Pgm File Line.", strSrcYamlFilePath.c_str());
        return -1;
    }

    return 0;
}

void MAP_FILE_MNG_C::DelPgmFile(const string &strMapName)
{
    string strPgmFilePath;
    GetPgmFilePath(strMapName, strPgmFilePath);
    objFileOprt_.DelFile(strPgmFilePath);
}

void MAP_FILE_MNG_C::DelYamlFile(const string &strMapName)
{
    string strYamlFilePath;
    GetYamlFilePath(strMapName, strYamlFilePath);
    objFileOprt_.DelFile(strYamlFilePath);
}

std::string WORK_SCENE_FILE_MNG_C::GetDefaultSceneDir(void)
{
    /* 地图路径错误属于高风险，失败时直接程序退出。 */
    BasicMapManager *pobjMapManager = BasicMapManager::Create();
    if (pobjMapManager == nullptr) {
        ST_LOG_FATAL("BasicMapManager::Create() Fail.");
        ST_LOG_FATAL("获取地图路径失败！");
        exit(0);
    }

    return pobjMapManager->GetWorkMapPath();
}

std::string WORK_SCENE_FILE_MNG_C::ParseWorkSceneDir(int argc, char** argv)
{
    string strParamBegin = WORK_SCENE_DIR_PARAM_PREFIX_STR;

    for (int loop = 0; loop < argc; ++loop) {
        R_ASSERT(argv[loop] != NULL, string(""));
        string strParam = argv[loop];

        if (strParam.substr(0, strParamBegin.size()) == strParamBegin) {
            return strParam.substr(strParamBegin.size());
        }
    }

    ST_LOG_INFO("No Input Work Scene Dir. Get Default Dir.");
    return GetDefaultSceneDir();
}

void WORK_SCENE_FILE_MNG_C::SetSceneDir(const std::string &strDir)
{
    strWorkSceneDir_        = FILE_OPRT_C::ToStyledDir(strDir);

#ifdef _WIN32
    const char PATH_SEPARATOR = '\\';
#else
    const char PATH_SEPARATOR = '/';
#endif
strNavigationMapDir_    = strWorkSceneDir_ + "navigation_map" + PATH_SEPARATOR;
strLocationMapDir_      = strWorkSceneDir_ + "location_map" + PATH_SEPARATOR;
strSimulationMapDir_    = strWorkSceneDir_ + "simulation_map" + PATH_SEPARATOR;
strDynamicMapDir_       = strWorkSceneDir_ + "dynamic_map" + PATH_SEPARATOR;

    strRoadsInfoFileName_   = "map_roads.xml";
    strSimulationMapName_   = "simulation_obst";
}

void WORK_SCENE_FILE_MNG_C::SetSceneDefaultDir(void)
{
    SetSceneDir(GetDefaultSceneDir());
}

std::string WORK_SCENE_FILE_MNG_C::GetWorkSceneDir(void) const
{
    return strWorkSceneDir_;
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadsInfoFilePath(void) const
{
    return (strNavigationMapDir_ + strRoadsInfoFileName_);
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadRouteFilePath(UINT32 roadNo, UINT32 routeId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_route" + ToString(routeId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadRouteAnchorPointFilePath(UINT32 roadNo, UINT32 routeId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_route" + ToString(routeId) + "_anchor_point.txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadLineFilePath(UINT32 roadNo, UINT32 lineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_line" + ToString(lineId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadLineAnchorPointFilePath(UINT32 roadNo, UINT32 lineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_line" + ToString(lineId) + "_anchor_point.txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadObstLineFilePath(UINT32 roadNo, UINT32 obstLineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_obstline" + ToString(obstLineId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadObstLineAnchorPointFilePath(UINT32 roadNo, UINT32 obstLineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_obstline" + ToString(obstLineId) + "_anchor_point.txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadStopLineFilePath(UINT32 roadNo, UINT32 stopLineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_stopline" + ToString(stopLineId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadStopLineAnchorPointFilePath(UINT32 roadNo, UINT32 stopLineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_stopline" + ToString(stopLineId) + "_anchor_point.txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadLaneLineFilePath(UINT32 roadNo, UINT32 LaneLineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_laneline" + ToString(LaneLineId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadLaneLineAnchorPointFilePath(UINT32 roadNo, UINT32 LaneLineId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_laneline" + ToString(LaneLineId) + "_anchor_point.txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadAreaFilePath(UINT32 roadNo, UINT32 areaId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_area" + ToString(areaId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetRoadGoalAreaFilePath(UINT32 roadNo, UINT32 goalAreaId) const
{
    return (strNavigationMapDir_ + "road" + ToString(roadNo) + "_goalArea" + ToString(goalAreaId) + ".txt");
}

std::string WORK_SCENE_FILE_MNG_C::GetSimulationObstMapPath(void) const
{
    return (strSimulationMapDir_ + strSimulationMapName_);
}

std::string WORK_SCENE_FILE_MNG_C::GetDyncMapFilePath(void) const
{
    return (strDynamicMapDir_ + "dync_map.json");
}

const std::string &WORK_SCENE_FILE_MNG_C::GetLocMapDir(void) const
{
    return strLocationMapDir_;
}

const std::string &WORK_SCENE_FILE_MNG_C::GetNavMapDir(void) const
{
    return strNavigationMapDir_;
}

const std::string &WORK_SCENE_FILE_MNG_C::GetSimulationMapDir(void) const
{
    return strSimulationMapDir_;
}

const std::string &WORK_SCENE_FILE_MNG_C::GetDynamicMapDir(void) const
{
    return strDynamicMapDir_;
}

void CFG_FILE_MNG_C::GetCfgFileDir(string &strFileDir) const
{
    FILE_MNG_C objFileMng;
    objFileMng.GetCfgFileDir(strFileDir);
}

std::string CFG_FILE_MNG_C::GetCfgFileDir(void) const
{
    std::string strFileDir;
    FILE_MNG_C objFileMng;
    objFileMng.GetCfgFileDir(strFileDir);
    return strFileDir;
}

std::string CFG_FILE_MNG_C::GetTempDir(void) const
{
    FILE_MNG_C objFileMng;
    std::string strCfgDir;
    objFileMng.GetCfgFileDir(strCfgDir);
    std::string strTempDir = strCfgDir + "temp/"; 

    FILE_OPRT_C objFileOprt;
    if (!objFileOprt.IsExist(strTempDir)) {
        objFileOprt.MakeDir(strTempDir);
    }
    
    return strTempDir;
}

