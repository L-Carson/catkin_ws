#ifndef __PCPT_COMM_FILE_H__
#define __PCPT_COMM_FILE_H__

#include "comm/comm_base.h"
#include "comm/comm_dbg_log.h"
#include "comm/comm_file.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

using namespace std;

/**
 * @brief pcpt文件管理器
 *
 */
class PCPT_FILE_MNG_C : public FILE_MNG_C, public FILE_OPRT_C
{
public:
    PCPT_FILE_MNG_C() : FILE_MNG_C(), FILE_OPRT_C() {}
    ~PCPT_FILE_MNG_C(){}

    bool IsFile(const char *filePath) const;
    bool IsFile(const string &filePath) const;

    bool IsDirectory(const char *dirPath) const;
    bool IsDirectory(const string &dirPath) const;

    int  DeleteFile(const char *filePath) const;
    int  DeleteFile(const string &filePath) const;

    bool IsDirExisted(string &dirPath) const;

    bool IsDirExist(const char *dirPath) const;
    bool IsDirExist(const string &dirPath) const;

    bool IsFileExist(const char *filePath) const;
    bool IsFileExist(const string &filePath) const;

    int  CreatDirectory(const char *dirPath) const;
    int  CreatDirectoryRecursive(string &dirPath) const;
    int  CheckDirPath(string &dirPath) const;
    void AddBackslashAtEnd(string &dirPath) const;

    char* GetCurrentPath() const;
    int GetDirSubFile(string &dirPath, list<string> &lstrFileName);

private:
};

#endif
