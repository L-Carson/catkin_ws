#ifndef __PCPT_COMM_IMAGE_H__
#define __PCPT_COMM_IMAGE_H__

#include "pcpt_comm/pcpt_comm_base.h"

#include "comm/comm_base.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

enum CV_ALGO_TYPE_ENUM
{
    CV_ALGO_TYPE_DET = 1,
    CV_ALGO_TYPE_SEG = 2,
};

using IMAGE_SEG_CONTOUR_VECT = std::vector<std::vector<cv::Point>>;

struct IMAGE_CLASS_DES_STRU
{
    std::string strLabel;
    cv::Scalar  objColor;
};

struct IMAGE_DET_OBJECT_STRU
{
    cv::Rect_<float> rect;
    UINT32           label;
    float            prob;
};

struct IMAGE_OBJ_INFO_STRU
{
    UINT32                 label;
    std::vector<cv::Point> vectPoint;
};

struct IMAGE_COLLECT_INFO_STRU
{
    UINT32                                 algoType;            /* 1为检测，2为分割 */
    std::map<UINT32, IMAGE_CLASS_DES_STRU> mapClassDes;
    std::vector<UINT32>                    vectFuzzyClass;
    std::vector<IMAGE_OBJ_INFO_STRU>       vectObj;
};

/**************************************************************************************
功能描述: 图像信息文件
          文件内容格式：
          Is Exist Fuzzy Objects: 1
          Label=1, ObjPointNum=2:
          xx,yy
          xx,yy
          Label=1, ObjPointNum=2:
          xx,yy
          xx,yy
          ...
修改记录:
**************************************************************************************/
class IMAGE_INFO_FILE_C
{
public:
    INT32 Write(const std::string &strFilePath, const IMAGE_COLLECT_INFO_STRU &stInfo) const;
    INT32 Read(const std::string &strFilePath, IMAGE_COLLECT_INFO_STRU &stInfo) const;

private:
};

/**************************************************************************************
功能描述: 图像操作
修改记录:
**************************************************************************************/
class IMAGE_OPRT_C
{
public:
    void PrintTextOnImageTop(const std::string &strText, const cv::Scalar &objColor, double fontScale, INT32 thickness, cv::Mat &objTgtImage) const;
    void DrawDetRectangles(const std::vector<IMAGE_DET_OBJECT_STRU> &vectObject, const std::map<UINT32, IMAGE_CLASS_DES_STRU> &mapClassDes, cv::Mat &objTgtImage) const;
    void DrawSegContours(const std::map<UINT32, IMAGE_SEG_CONTOUR_VECT> &mapContours, const std::map<UINT32, IMAGE_CLASS_DES_STRU> &mapClassDes, cv::Mat &objTgtImage) const;

private:
    IMAGE_CLASS_DES_STRU stDefaultClassDes_ = {"Unknown", {0, 0, 255}};     // 默认红色
};

/**************************************************************************************
功能描述: 图像文件管理
修改记录:
**************************************************************************************/
class IMAGE_FILE_MNG_C
{
public:
    BOOL  IsImageFilePath(const std::string &strFilePath) const;
    BOOL  IsImageInfoFilePath(const std::string &strFilePath) const;
    void  GetCollectedImageAndInfoFileName(const std::string &strPrefix, const std::string &strTimeStamp, std::string &strImageFileName, std::string &strImageInfoFileName) const;
    INT32 GetCollectedImageInfoFilePath(const std::string &strImageFilePath, std::string &strImageInfoFilePath) const;
    INT32 GetCollectedImageFilePath(const std::string &strImageInfoFilePath, std::string &strImageFilePath) const;
    INT32 GetCollectedImageFilePathWithInfo(const std::string &strImageFilePath, std::string &strImageFilePathWithInfo) const;

public:
};



#endif
