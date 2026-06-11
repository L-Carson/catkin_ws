

#ifndef FILE_SYSTEM
#define FILE_SYSTEM


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>


using namespace std;


/**
 * 文件信息,一般用于检索文件夹中的文件
 */
typedef struct FileInfoSt_
{
    std::string name;///< 文件名,绝对路径
    struct stat64 fileInfo;///< 文件信息，大小，创建时间，最后修改时间等
}FileInfoSt;

typedef struct FileInfoNewSt_
{
    char name[256];///< 文件名,绝对路径
    struct stat fileInfo;///< 文件信息，大小，创建时间，最后修改时间等
}FileInfoNewSt;

class FileManager
{

public:
    /**
    * 构造函数
    */
    FileManager() {}
    /**
     * 析构函数
     */
    ~FileManager() {}
 


    static long long GetFileLength(const char* fileName);
    static bool GetFolderFileInfo(std::string path,vector<FileInfoSt>& list,bool son=true);
    static bool GetFolderInfo(std::string path,vector<FileInfoSt>& list,bool son=true);
};



#endif //
