
#include "comm_file_oprt.h"
#include "comm_dbg_log.h"
#include "comm_data_logic.h"

#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>





std::string FileOperate::ToPureDir(const std::string &strSrcDir)
{
    std::string strTgtDir = strSrcDir;

    while ((strTgtDir.length() > 0) && (strTgtDir[strTgtDir.length() - 1] == '/')) {
        strTgtDir.erase(strTgtDir.length() - 1, 1);
    }

    return strTgtDir;
}

std::string FileOperate::ToStyledDir(const std::string &strSrcDir)
{
    return ToPureDir(strSrcDir) + "/";
}

/**************************************************************************************
功能描述: 判断文件或者目录是否存在
修改记录:
**************************************************************************************/
bool FileOperate::IsExist(const std::string &strPath)
{
    return (0 == access(strPath.c_str(), 0));
}

/**************************************************************************************
功能描述: 修改文件名，修改成功返回0，失败返回-1
          注意，调用后目标老文件会被删除
修改记录:
**************************************************************************************/
int FileOperate::Rename(const std::string &strOldFilePath, const std::string &strNewFilePath, bool isCoverFile = false)
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
int FileOperate::DelFile(const std::string &strFilePath)
{
    int rslt = remove(strFilePath.c_str());
    if (rslt != 0) {
        ST_LOG_ERR("remove(%s) Fail.", strFilePath.c_str());
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 删除目录
修改记录:
**************************************************************************************/
int  FileOperate::DelDir(const std::string &strDirPath)
{
    int rslt = DelDirContent(strDirPath);
    if (rslt != 0) {
        ST_LOG_ERR("DelDirContent(%s) Fail.", strDirPath.c_str());
        return -1;
    }

    rmdir(strDirPath.c_str());
    return 0;
}

int FileOperate::DelDirContent(const std::string &strDirPath)
{
    std::string strPureDirPath = ToPureDir(strDirPath);
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

        std::string strSubObjPath = strPureDirPath + "/" + pstObjInfo->d_name;
        int rslt = DelDirOrFile(strSubObjPath);
        if (rslt != 0) {
            ST_LOG_ERR("DelDirOrFile(%s) Fail.", strSubObjPath.c_str());
            closedir(pstDir);
            return -1;
        }
    }

    closedir(pstDir);
    return 0;
}

int FileOperate::DelDirOrFile(const std::string &strPath)
{
    struct stat statbuf;
    int rslt = lstat(strPath.c_str(), &statbuf);
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

    return 0;
}

int  FileOperate::MakeDir(const std::string &strDirPath)
{
    unsigned int dirLen  = strDirPath.length();

     /* 递归创建 */
    unsigned int procDirLen = 0;
    while (procDirLen < dirLen) {
        unsigned int idx = strDirPath.find('/', procDirLen);

        procDirLen = std::min(dirLen, idx + 1);

        std::string strProcDir = strDirPath.substr(0, procDirLen);
        if (IsExist(strProcDir)) {
            continue;
        }

        int rslt = mkdir(strProcDir.c_str(), ACCESSPERMS);
        if (rslt != 0) {
            ST_LOG_ERR("Create Dir %s Of %s Fail.", strProcDir.c_str(), strDirPath.c_str());
            return -1;
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 复制文件
修改记录:
**************************************************************************************/
int FileOperate::Copy(const std::string &strSrcFilePath, const std::string &strTgtFilePath, bool isCoverFile = false)
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

    std::string strCmd = "cp -f " + strSrcFilePath + " " + strTgtFilePath;
    SYSTEM_EXEC(strCmd.c_str());

    return 0;
}

/**************************************************************************************
功能描述: 将文件缓存区内容输出到文件
修改记录:
**************************************************************************************/
void FileOperate::SyncBuffToFile(FILE *pf)
{
    if (pf != NULL) {
        fflush(pf);
    }
}

/**************************************************************************************
功能描述: 将内存区文件强制输出到硬盘
修改记录:
**************************************************************************************/
void FileOperate::SyncFileToDisk(FILE *pf)
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
void FileOperate::GetFileNameFrmPath(const std::string &strFilePath, std::string &strFileName)
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
void FileOperate::GetFileDirFrmPath(const std::string &strFilePath, std::string &strFileDir)
{
    int fileNamePos = strFilePath.find_last_of("/\\");   /* 包含linux和win的 */
    if (fileNamePos < 0) {
        fileNamePos = 0;
    }

    strFileDir = strFilePath.substr(0, fileNamePos);
}

int FileOperate::GetFilesOfDir(const std::string &strDirPath, std::vector<std::string> &vectFileName)
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

bool FileOperate::IsLinkFile(const std::string &strLinkFilePath)
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

int FileOperate::MakeLink(const std::string &strLinkFilePath, const std::string &strTgtFilePath)
{
    /* 先强制删除老的文件 */
    if (IsExist(strLinkFilePath)) {
        int rslt = DelFile(strLinkFilePath);
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

int FileOperate::GetLinkTgtFilePath(const std::string &strLinkFilePath, std::string &strTgtFilePath)
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

    R_ASSERT_2(lenOfTgtFilePathStr < (ssize_t)(ARRAY_SIZE(acTgtFilePath)), -1, lenOfTgtFilePathStr, ARRAY_SIZE(acTgtFilePath));
    acTgtFilePath[lenOfTgtFilePathStr] = '\0';

    strTgtFilePath = std::string(acTgtFilePath);
    return 0;
}


