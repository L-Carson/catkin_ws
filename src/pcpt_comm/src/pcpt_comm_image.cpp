
#include "pcpt_comm/pcpt_comm_image.h"


#define FPRINTF_FAIL_RET(strFmt_, ...)                                                              \
do {                                                                                                \
    ++writeLinesNum;                                                                                \
    rslt = fprintf(pfFile, strFmt_, ##__VA_ARGS__);                                                 \
    if (rslt < 0) {                                                                                 \
        ST_LOG_ERR("fprintf() Fail. writeLinesNum=%u.", writeLinesNum);                             \
        fclose(pfFile);                                                                             \
        pfFile = NULL;                                                                              \
        return -1;                                                                                  \
    }                                                                                               \
} while(0)

#define FSCANF_FAIL_RET(paraNum_, strFmt_, ...)                                                     \
do {                                                                                                \
    ++readLinesNum;                                                                                 \
    rslt = fscanf(pfFile, strFmt_, ##__VA_ARGS__);                                                  \
    if (rslt != (paraNum_)) {                                                                       \
        ST_LOG_ERR("fscanf() Fail. readLinesNum=%u, rslt=%d.", readLinesNum, rslt);                 \
        fclose(pfFile);                                                                             \
        pfFile = NULL;                                                                              \
        return -1;                                                                                  \
    }                                                                                               \
} while (0)


INT32 IMAGE_INFO_FILE_C::Write(const std::string &strFilePath, const IMAGE_COLLECT_INFO_STRU &stInfo) const
{
    INT32 rslt;

    FILE *pfFile = fopen(strFilePath.c_str(), "w");
    if (pfFile == nullptr) {
        ST_LOG_ERR("fopen(%s) Fail.", strFilePath.c_str());
        return -1;
    }

    UINT32 writeLinesNum = 0;

    /* 写算法类型 */
    FPRINTF_FAIL_RET("Algo Type: %u\n", stInfo.algoType);

    /* 写分类描述信息 */
    FPRINTF_FAIL_RET("Label Des Num: %u\n", UINT32(stInfo.mapClassDes.size()));

    for (auto &objPair : stInfo.mapClassDes) {
        INT32                       label     = objPair.first;
        const IMAGE_CLASS_DES_STRU &stObjDes  = objPair.second;

        FPRINTF_FAIL_RET("Label=%d, Color=(%.2f, %.2f, %.2f), Str=%s\n", label, stObjDes.objColor[0], stObjDes.objColor[1], stObjDes.objColor[2], stObjDes.strLabel.c_str());
    }

    /* 写模糊类别 */
    FPRINTF_FAIL_RET("Fuzzy Class Num: %u\n", UINT32(stInfo.vectFuzzyClass.size()));

    for (UINT32 fuzzyClass : stInfo.vectFuzzyClass) {
        FPRINTF_FAIL_RET("%u\n", fuzzyClass);
    }

    /* 写识别对象点集 */
    for (const IMAGE_OBJ_INFO_STRU &stImageObj : stInfo.vectObj) {
        FPRINTF_FAIL_RET("Label=%u, ObjPointNum=%u\n", stImageObj.label, UINT32(stImageObj.vectPoint.size()));

        for (const cv::Point &stPoint : stImageObj.vectPoint) {
            FPRINTF_FAIL_RET("%u,%u\n", stPoint.x, stPoint.y);
        }
    }

    fclose(pfFile);
    pfFile = NULL;
    return 0;
}

INT32 IMAGE_INFO_FILE_C::Read(const std::string &strFilePath, IMAGE_COLLECT_INFO_STRU &stInfo) const
{
    INT32 rslt;

    FILE *pfFile = fopen(strFilePath.c_str(), "r");
    if (pfFile == nullptr) {
        ST_LOG_ERR("fopen(%s) Fail.", strFilePath.c_str());
        return -1;
    }

    UINT32 readLinesNum = 0;

    /* 读算法类型 */
    FSCANF_FAIL_RET(1, "Algo Type: %u\n", &stInfo.algoType);

    /* 读分类描述信息 */
    UINT32 objectDesNum;
    FSCANF_FAIL_RET(1, "Label Des Num: %u\n", &objectDesNum);

    for (UINT32 loop = 0; loop < objectDesNum; ++loop) {
        INT32  label;
        float  showColorB;
        float  showColorG;
        float  showColorR;
        CHAR   acDesStr[100] = {0};
        FSCANF_FAIL_RET(5, "Label=%d, Color=(%f, %f, %f), Str=%99s\n", &label, &showColorB, &showColorG, &showColorR, acDesStr);

        IMAGE_CLASS_DES_STRU stObjDes;
        stObjDes.strLabel = acDesStr;
        stObjDes.objColor = {showColorB, showColorG, showColorR};

        stInfo.mapClassDes[label] = stObjDes;
    }

    /* 读模糊类别 */
    UINT32 fuzzyClassNum;
    FSCANF_FAIL_RET(1, "Fuzzy Class Num: %u\n", &fuzzyClassNum);
    for (UINT32 loop = 0; loop < fuzzyClassNum; ++loop) {
        UINT32 fuzzyClass;
        FSCANF_FAIL_RET(1, "%u\n", &fuzzyClass);

        stInfo.vectFuzzyClass.push_back(fuzzyClass);
    }

    /* 写识别对象点集 */
    while (feof(pfFile) == 0) {
        INT32  label;
        UINT32 objPointNum;
        FSCANF_FAIL_RET(2, "Label=%d, ObjPointNum=%u\n", &label, &objPointNum);

        IMAGE_OBJ_INFO_STRU stImageObj;
        stImageObj.label = label;
        stImageObj.vectPoint.reserve(objPointNum);

        for (UINT32 loop = 0; loop < objPointNum; ++loop) {
            cv::Point stObjPoint;
            FSCANF_FAIL_RET(2, "%u,%u\n", &stObjPoint.x, &stObjPoint.y);

            stImageObj.vectPoint.push_back(stObjPoint);
        }

        stInfo.vectObj.push_back(stImageObj);
    }

    fclose(pfFile);
    pfFile = NULL;
    return 0;
}

void IMAGE_OPRT_C::PrintTextOnImageTop(const std::string &strText, const cv::Scalar &objColor, double fontScale, INT32 thickness, cv::Mat &objTgtImage) const
{
    INT32 fontFace  = cv::FONT_HERSHEY_SIMPLEX; // 字体
    // fontScale      字体大小
    // thickness      字体粗细
    // objColor       字体颜色
    // objOriginPoint 左下角坐标

    cv::Size objTextSize = cv::getTextSize(strText, fontFace, fontScale, thickness, nullptr);

    cv::Point objOriginPoint;
    objOriginPoint.x = (objTgtImage.cols - objTextSize.width) / 2;
    objOriginPoint.y = objTgtImage.rows / 100 + objTextSize.height;

    cv::putText(objTgtImage, strText, objOriginPoint, fontFace, fontScale, objColor, thickness);
}

void IMAGE_OPRT_C::DrawDetRectangles(const std::vector<IMAGE_DET_OBJECT_STRU> &vectObject, const std::map<UINT32, IMAGE_CLASS_DES_STRU> &mapClassDes, cv::Mat &objTgtImage) const
{
    for (UINT32 loop = 0; loop < vectObject.size(); ++loop) {
        const IMAGE_DET_OBJECT_STRU &stDetObj   = vectObject[loop];
        auto                         iter       = mapClassDes.find(stDetObj.label);
        const IMAGE_CLASS_DES_STRU  &stClassDes = (iter == mapClassDes.end()) ? stDefaultClassDes_ : iter->second;

        float      meanOfColor  = cv::mean(stClassDes.objColor)[0];
        cv::Scalar objTxtColor  = (meanOfColor > 144) ? cv::Scalar(0, 0, 0) : cv::Scalar(255, 255, 255);
        cv::rectangle(objTgtImage, stDetObj.rect, stClassDes.objColor, 2);

        char acDesText[256];
        if (stDetObj.prob > 0.0) {
            sprintf(acDesText, "%s_%.1f%%", stClassDes.strLabel.c_str(), stDetObj.prob * 100.0);
        } else {
            sprintf(acDesText, "%s", stClassDes.strLabel.c_str());
        }

        cv::Size stTextAreaSize = cv::getTextSize(acDesText, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, nullptr);

        cv::Point stTextOriginPos;
        stTextOriginPos.x = LIMIT(stDetObj.rect.x,                     0, objTgtImage.cols - stTextAreaSize.width);
        stTextOriginPos.y = LIMIT(stDetObj.rect.y, stTextAreaSize.height, objTgtImage.rows);

        cv::putText(objTgtImage, acDesText, stTextOriginPos, cv::FONT_HERSHEY_SIMPLEX, 0.4, objTxtColor, 0.5);
    }
}

void IMAGE_OPRT_C::DrawSegContours(const std::map<UINT32, IMAGE_SEG_CONTOUR_VECT> &mapContours, const std::map<UINT32, IMAGE_CLASS_DES_STRU> &mapClassDes, cv::Mat &objTgtImage) const
{
    for (auto &objPair : mapContours) {
        INT32                         segClass    = objPair.first;
        const IMAGE_SEG_CONTOUR_VECT &vectContour = objPair.second;

        auto                        iter = mapClassDes.find(segClass);
        const IMAGE_CLASS_DES_STRU &stClassDes = (iter == mapClassDes.end()) ? stDefaultClassDes_ : iter->second;

        cv::drawContours(objTgtImage, vectContour, -1, stClassDes.objColor, cv::FILLED);
    }
}

BOOL IMAGE_FILE_MNG_C::IsImageFilePath(const std::string &strFilePath) const
{
    UINT32 pathLen = UINT32(strFilePath.length());
    if (pathLen < 4) {
        return false;
    }

    if (strFilePath.substr(pathLen - 4) != ".jpg") {
        return false;
    }

    return true;
}

BOOL IMAGE_FILE_MNG_C::IsImageInfoFilePath(const std::string &strFilePath) const
{
    UINT32 pathLen = UINT32(strFilePath.length());
    if (pathLen < 5) {
        return false;
    }

    if (strFilePath.substr(pathLen - 5) != ".info") {
        return false;
    }

    return true;
}

void IMAGE_FILE_MNG_C::GetCollectedImageAndInfoFileName(const std::string &strPrefix, const std::string &strTimeStamp, std::string &strImageFileName, std::string &strImageInfoFileName) const
{
    strImageFileName     = strPrefix + "_" + strTimeStamp + ".jpg";
    strImageInfoFileName = strPrefix + "_" + strTimeStamp + ".info";
}

INT32 IMAGE_FILE_MNG_C::GetCollectedImageInfoFilePath(const std::string &strImageFilePath, std::string &strImageInfoFilePath) const
{
    if (!IsImageFilePath(strImageFilePath)) {
        ST_LOG_ERR("IsImageFilePath(%s) False.", strImageFilePath.c_str());
        return -1;
    }

    strImageInfoFilePath = strImageFilePath.substr(0, strImageFilePath.length() - std::string(".jpg").length()) + ".info";
    return 0;
}

INT32 IMAGE_FILE_MNG_C::GetCollectedImageFilePath(const std::string &strImageInfoFilePath, std::string &strImageFilePath) const
{
    if (!IsImageInfoFilePath(strImageInfoFilePath)) {
        ST_LOG_ERR("IsImageInfoFilePath(%s) False.", strImageInfoFilePath.c_str());
        return -1;
    }

    strImageFilePath = strImageInfoFilePath.substr(0, strImageInfoFilePath.length() - std::string(".info").length()) + ".jpg";
    return 0;
}

INT32 IMAGE_FILE_MNG_C::GetCollectedImageFilePathWithInfo(const std::string &strImageFilePath, std::string &strImageFilePathWithInfo) const
{
    if (!IsImageFilePath(strImageFilePath)) {
        ST_LOG_ERR("IsImageFilePath(%s) False.", strImageFilePath.c_str());
        return -1;
    }

    strImageFilePathWithInfo = strImageFilePath.substr(0, strImageFilePath.length() - std::string(".jpg").length()) + "_info.jpg";
    return 0;
}







