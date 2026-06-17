#include "pcpt_comm/pcpt_comm_file.h"

/**************************************************************************************
功能描述: 是否为文件
修改记录:
**************************************************************************************/
bool PCPT_FILE_MNG_C::IsFile(const char *filePath) const
{
     struct stat buffer;
     return (stat(filePath, &buffer) == 0 && S_ISREG(buffer.st_mode));
}

bool PCPT_FILE_MNG_C::IsFile(const string &filePath) const
{
    return IsFile(filePath.c_str());
}

/**************************************************************************************
功能描述: 是否为文件夹
修改记录:
**************************************************************************************/
bool PCPT_FILE_MNG_C::IsDirectory(const char *dirPath) const
{
    struct stat buffer;
    return (stat(dirPath, &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool PCPT_FILE_MNG_C::IsDirectory(const string &dirPath) const
{
    return IsDirectory(dirPath.c_str());
}

/**************************************************************************************
功能描述: 删除文件或空目录，成功返回0，失败返回-1
修改记录:
**************************************************************************************/
int PCPT_FILE_MNG_C::DeleteFile(const char *filePath) const
{
    return remove(filePath);
}

int PCPT_FILE_MNG_C::DeleteFile(const string &filePath) const
{
    return DeleteFile(filePath.c_str());
}

/**************************************************************************************
功能描述: 检查目录是否存在，如果不存在，则创建
修改记录:
**************************************************************************************/
bool PCPT_FILE_MNG_C::IsDirExisted(string &dirPath) const
{
    if (IsDirExist(dirPath.c_str())) {
        return true;
    } else if (0 == CreatDirectoryRecursive(dirPath)){
        return true;
    }

    return false;
}

/**************************************************************************************
功能描述: 检查目录是否存在
修改记录:
**************************************************************************************/
bool PCPT_FILE_MNG_C::IsDirExist(const char *dirPath) const
{
    DIR *dp;
    if ((dp = opendir(dirPath)) == NULL) {
        return false;
    }

    closedir(dp);
    return true;
}

bool PCPT_FILE_MNG_C::IsDirExist(const string &dirPath) const
{
    return IsDirExist(dirPath.c_str());
}

/**************************************************************************************
功能描述: 检查文件/文件夹(所有类型)是否存在
修改记录:
**************************************************************************************/
bool PCPT_FILE_MNG_C::IsFileExist(const char *filePath) const
{
    if (0 == access(filePath, F_OK)) {
        return true;
    } else {
        return false;
    }
}

bool PCPT_FILE_MNG_C::IsFileExist(const string& filePath) const
{
    return IsFileExist(filePath.c_str());
}

/**************************************************************************************
功能描述: 创建目录，0:成功 -1:失败
修改记录:
**************************************************************************************/
int PCPT_FILE_MNG_C::CreatDirectory(const char *dirPath) const
{
    int isCreate = mkdir(dirPath, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO); //成功返回0

    if(!isCreate) {
        ST_LOG_INFO("create path:%s", dirPath);
        usleep(1000);
        return 0;
    } else {
        ST_LOG_ERR("create path failed! error code: %s", dirPath);
        return -1;
    }
}

/**************************************************************************************
功能描述: 递归创建目录，0:成功 -1:失败
修改记录:
**************************************************************************************/
int PCPT_FILE_MNG_C::CreatDirectoryRecursive(string &dirPath) const
{
    if (0 != CheckDirPath(dirPath)) {
        return -1;
    }

    std::string::size_type len = dirPath.size();

    for (size_t i = 0; i < len; ++i) {
        std::string temp = dirPath.substr(0, i+1);

        if (('/' == dirPath[i] || i == len-1) && access(temp.c_str(), F_OK) != 0) {
            //S_IRWXU|S_IRWXG|S_IRWXO目录访问权限
            if (mkdir(temp.c_str(), S_IRWXU|S_IRWXG|S_IRWXO)) {
                ST_LOG_ERR("Failed to create the path:%s! Error code: %s", dirPath.c_str(), temp.c_str());
                return -1;
            }
        }
    }

    ST_LOG_INFO("Secceed to create the path:%s", dirPath.c_str());
    SLEEP_U(1000);
    return 0;
}

/**************************************************************************************
功能描述: 检查目录是否合法
修改记录:
**************************************************************************************/
int PCPT_FILE_MNG_C::CheckDirPath(string &dirPath) const
{
    if (dirPath.empty()) {
        ST_LOG_ERR("Path Empty, Error");
        return -1;
    }

    for (size_t i = 0; i < dirPath.size() - 2; ++i) {
        if (dirPath[i] == '/' && dirPath[i+1] == '/') {
            ST_LOG_ERR("%s Path Error.", dirPath.c_str());
            return -1;
        }
    }

    AddBackslashAtEnd(dirPath);

    return 0;
}

/**************************************************************************************
功能描述: 在路径末尾添加反斜线"/"
修改记录:
**************************************************************************************/
void PCPT_FILE_MNG_C::AddBackslashAtEnd(string &dirPath) const
{
    if ('/' != dirPath.back()) {
        dirPath += "/";
    }
}

/**************************************************************************************
功能描述: 获取当前路径
修改记录:
**************************************************************************************/
char* PCPT_FILE_MNG_C::GetCurrentPath() const
{
    return getcwd(nullptr, 0);
}

/**************************************************************************************
功能描述: 获取指定路径下的所有文件及目录（无递归）
修改记录:
**************************************************************************************/
int PCPT_FILE_MNG_C::GetDirSubFile(string &dirPath, list<string> &lstrFileName)
{
    AddBackslashAtEnd(dirPath);

    DIR *dir = opendir(dirPath.data());
    if (!dir) {
        ST_LOG_ERR("Filed to open dir: %s", dirPath.data());
        return -1;
    }

    struct dirent *dirent;
    while ((dirent = readdir(dir))) {
        string dirName = dirent->d_name;
        if(dirName  == "." || dirName  == "..") continue; //ignore . or .. file
        lstrFileName.push_back(dirName);
    }

    closedir(dir);
    return 0;
}


