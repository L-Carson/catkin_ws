#include "comm_extend/comm_base_extend.h"

PointCloudXYZI::Ptr PointXyzirt2Xyzi(const PointCloudXYZIRT::Ptr pSrcPoints)
{
    PointCloudXYZI::Ptr pDstPoints(new PointCloudXYZI); 
    pDstPoints->points.resize(pSrcPoints->size());
    for (size_t i = 0; i < pSrcPoints->size(); i++) {
        pDstPoints->points[i].x = pSrcPoints->points[i].x;
        pDstPoints->points[i].y = pSrcPoints->points[i].y;
        pDstPoints->points[i].z = pSrcPoints->points[i].z;
        pDstPoints->points[i].intensity = pSrcPoints->points[i].intensity;
    }
    pDstPoints->height   = 1;
    pDstPoints->width    = pSrcPoints->size();
    pDstPoints->is_dense = true;

    return pDstPoints;
}

INT32 GetLoginUserName(string &strUserName)
{
#ifndef WIN32
#if 0

    // 手动执行时能正确获取，自动启动时获取失败
    char acUsrName[100];

    if (0 == getlogin_r(acUsrName, sizeof(acUsrName))) {
        strUserName = acUsrName;
        return 0;
    } else {
        return -1;
    }

#endif

    uid_t          userid = getuid();
    struct passwd *pwd    = getpwuid(userid);
    if ((pwd != NULL) && (pwd->pw_name != NULL)) {
        strUserName = pwd->pw_name;
        return 0;
    } else {
        return -1;
    }

#else
    strUserName = "";
    return 0;
#endif
}

INT32 RunSystem(const string &strCmd)
{
    INT32 status = system(strCmd.c_str());
    if (status == -1){
        ST_LOG_ERR("Run Cmd %s Fail.", strCmd.c_str());
    }
    else {
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0)
                return 0;
            else
                ST_LOG_ERR("Run Cmd %s Fail. Exit Code %d", strCmd.c_str(), WEXITSTATUS(status));
        }
        else
            ST_LOG_ERR("Run Cmd %s Fail. Exit Status %d", strCmd.c_str(), WEXITSTATUS(status));
    }
    return -1;
}

vector<string> GetFileNames(const string &strDir, const string &strSuffix)
{
    vector<string> vstrResult;
    DIR *pDir = opendir(strDir.c_str());
    if (pDir == NULL ) {
        ST_LOG_ERR("%s Is Not Dir or Is Not Exist.", strDir.c_str());
        return vstrResult;
    }

    struct dirent* pstDirent = NULL;
    while( (pstDirent = readdir(pDir)) != NULL )
    {
        if (pstDirent->d_type != DT_DIR)
        {
            string strFileName(pstDirent->d_name);
            if (strFileName.size() <= strSuffix.size()){
                continue;
            }

            if (strSuffix == strFileName.substr(strFileName.size() - strSuffix.size())) {
                vstrResult.push_back(strFileName);
            }
        }
    }
    closedir(pDir);

    if (vstrResult.size() > 1) {
        sort(vstrResult.begin(), vstrResult.end());
    }

    return vstrResult;
}

BOOL SaveKittiTraj(std::ostream &os, const vector<Matrix4f> &vmPoses)
{ 
    for (size_t i = 0; i < vmPoses.size(); i++) {
        for (INT32 rowInd = 0; rowInd < 3; ++rowInd) {
            for (INT32 colInd = 0; colInd < 4; ++colInd) {
                if (rowInd == 2 && colInd == 3)
                    os << vmPoses[i](rowInd, colInd) <<std::endl;
                else
                    os << vmPoses[i](rowInd, colInd) <<" ";
           }
        }
    } 
    return os.good();
}


