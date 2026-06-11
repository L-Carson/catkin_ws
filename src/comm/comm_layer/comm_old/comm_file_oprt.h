

#ifndef __COMM_FILE_OPRT_H__
#define __COMM_FILE_OPRT_H__


#include <cstdio>
#include <string>
#include <vector>

/**
 * @brief 文件操作类
 *
 */
class FileOperate
{
public:
    static std::string ToPureDir(const std::string &strSrcDir);
    static std::string ToStyledDir(const std::string &strSrcDir);

    static bool   IsExist(const std::string &strPath);
    static int    Rename(const std::string &strOldFilePath, const std::string &strNewFilePath, bool isCoverFile);
    static int    DelFile(const std::string &strFilePath);
    static int    DelDir(const std::string &strDirPath);
    static int    DelDirContent(const std::string &strDirPath);
    static int    DelDirOrFile(const std::string &strPath);
    static int    MakeDir(const std::string &strDirPath);
    static int    Copy(const std::string &strSrcFilePath, const std::string &strTgtFilePath, bool isCoverFile);
    static void   SyncBuffToFile(FILE *pf);
    static void   SyncFileToDisk(FILE *pf);

    static void   GetFileNameFrmPath(const std::string &strFilePath, std::string &strFileName);
    static void   GetFileDirFrmPath(const std::string &strFilePath, std::string &strFileDir);

    static int    GetFilesOfDir(const std::string &strDirPath, std::vector<std::string> &vectFileName);

    static bool   IsLinkFile(const std::string &strLinkFilePath);
    static int    MakeLink(const std::string &strLinkFilePath, const std::string &strTgtFilePath);
    static int    GetLinkTgtFilePath(const std::string &strLinkFilePath, std::string &strTgtFilePath);

private:

};

#endif

