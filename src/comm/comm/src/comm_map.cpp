#include "comm/comm_map.h"
#include "comm/comm_base.h"
#include "comm/comm_file.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <queue>

using namespace std;

SIMPLE_YAML_FILE_C::SIMPLE_YAML_FILE_C(const char *pcYamlFileName)
{
    if (pcYamlFileName == NULL) {
        ST_LOG_ERR("Str Is Null.");
        return;
    }

    strFileName_ = pcYamlFileName;
}

int SIMPLE_YAML_FILE_C::ReadItem(YAML_FILE_ITEM_STRU astItem[], int itemNum)
{
    /* 检查并设置初始值 */
    for (int loop = 0; loop < itemNum; loop++) {
        if (astItem[loop].pStrTag == NULL) {
            ST_LOG_ERR("Item Tag Is Null.");
            return -1;
        }

        astItem[loop].strContent[0] = '\0';
    }

    /* 从文件中读取 */
    if (0 != ReadItemFromFile(astItem, itemNum)) {
        return -1;
    }

    /* 检查读取结果 */
    for (int loop = 0; loop < itemNum; loop++) {
        if (astItem[loop].strContent[0] == '\0') {
            ST_LOG_ERR("Item(%s) Read Fail.", astItem[loop].pStrTag);
            return -1;
        }
    }
    return 0;
}

int SIMPLE_YAML_FILE_C::WriteItem(YAML_FILE_ITEM_STRU astItem[], int itemNum)
{
    FILE *pfFile = fopen(strFileName_.c_str(), "w");
    if (pfFile == NULL) {
        ST_LOG_ERR("Open File Fail.");
        return -1;
    }

    bool isWriteFirstLine = true;

    for (int loop = 0; loop < itemNum; loop++) {
        if (!isWriteFirstLine) {
            fprintf(pfFile, "\n");
        }

        YAML_FILE_ITEM_STRU *pstItem = &astItem[loop];
        fprintf(pfFile, "%s: %s", pstItem->pStrTag, pstItem->strContent);

        isWriteFirstLine = false;
    }

    fclose(pfFile);
    return 0;
}

void SIMPLE_YAML_FILE_C::DelAllCharInStr(char *pcStr, char delChar)
{
    int srcStrLen = strlen(pcStr);
    int tgtStrLen = 0;
    for (int loop = 0; loop < srcStrLen; loop++) {
        if (pcStr[loop] == delChar) {
            continue;
        }

        /* 可能不需要赋值的也执行了一遍，无影响 */
        pcStr[tgtStrLen] = pcStr[loop];
        tgtStrLen++;
    }

    pcStr[tgtStrLen] = '\0';
}

bool SIMPLE_YAML_FILE_C::IsStrBeginWith(const char *pcStr, const char *pcSubStr)
{
    int strLen = strlen(pcStr);
    int subStrLen = strlen(pcSubStr);
    if (strLen < subStrLen) {
        return false;
    }

    for (int loop = 0; loop < subStrLen; loop++) {
        if (pcStr[loop] != pcSubStr[loop]) {
            return false;
        }
    }

    return true;
}

YAML_FILE_ITEM_STRU *SIMPLE_YAML_FILE_C::MatchItem(YAML_FILE_ITEM_STRU astItem[], int itemNum, const char *pstrLine)
{
    for (int loop = 0; loop < itemNum; loop++) {
        YAML_FILE_ITEM_STRU *pstItemTmp = &astItem[loop];
        if (IsStrBeginWith(pstrLine, pstItemTmp->pStrTag) && pstrLine[strlen(pstItemTmp->pStrTag) == ':']) {
            return pstItemTmp;
        }
    }

    return NULL;
}

int SIMPLE_YAML_FILE_C::ReadItemFromFile(YAML_FILE_ITEM_STRU astItem[], int itemNum)
{
    FILE *pfRead = fopen(strFileName_.c_str(), "rb");
    if (pfRead == NULL) {
        ST_LOG_ERR("Open File(%s) Fail.", strFileName_.c_str());
        return -1;
    }

    char acLine[SIMPLE_YAML_FILE_MAX_ITEM_LEN];
    while (fgets(acLine, sizeof(acLine), pfRead) != NULL) {
        /* 确保有结束符 */
        acLine[sizeof(acLine) - 1] = '\0';

        /* 如果最后一个字符是换行，则删除换行 */
        //int lineLen = strlen(acLine);
        //if ((lineLen > 0) && (acLine[lineLen - 1] == '\n')) {
        //    acLine[lineLen - 1] = '\0';
        //}

        DelAllCharInStr(acLine, '\r');
        DelAllCharInStr(acLine, '\n');

        /* 删除空格方便处理 */
        DelAllCharInStr(acLine, ' ');
        DelAllCharInStr(acLine, '\t');

        YAML_FILE_ITEM_STRU *pstItem = MatchItem(astItem, itemNum, acLine);
        if (pstItem == NULL) {
            continue;
        }

        /* 跳过:赋值剩下的内容 */
        strncpy(pstItem->strContent, &acLine[strlen(pstItem->pStrTag) + 1], sizeof(pstItem->strContent) - 1);
        pstItem->strContent[sizeof(pstItem->strContent) - 1] = '\0';
    }

    fclose(pfRead);
    return 0;
}

int MAP_YAML_FILE_C::Read(const string &strMapPath, string &strPgmFileName, float &resolution, COORDINATE_2D_STRU &stOriginPos) const
{
    YAML_FILE_ITEM_STRU *pstItem;
    YAML_FILE_ITEM_STRU astItem[] = {{"image",      {0}},
                                     {"resolution", {0}},
                                     {"origin",     {0}}};      /* 此处顺序和后续处理顺序要一致 */

    /* 从文件中读取各项 */
    string strYamlFilePath;
    GetYamlFilePath(strMapPath, strYamlFilePath);
    SIMPLE_YAML_FILE_C objYamlFile(strYamlFilePath.c_str());
    if (0 != objYamlFile.ReadItem(astItem, ARRAY_SIZE(astItem))) {
        ST_LOG_ERR("Read Content Fail.");
        return -1;
    }

    /* 处理 image  */
    pstItem = &astItem[0];
    strPgmFileName.assign(pstItem->strContent);
    ST_LOG_INFO("image Content(%s), pcPgmFileName=%s.", pstItem->strContent, strPgmFileName.c_str());

    /* 处理 resolution  */
    pstItem = &astItem[1];
    if (1 != sscanf(pstItem->strContent, "%f", &resolution)) {
        ST_LOG_ERR("resolution Content(%s) Trans Fail.", pstItem->strContent);
        return -1;
    }
    ST_LOG_INFO("resolution Content(%s), resolution=%f.", pstItem->strContent, resolution);

    /* 处理 origin  */
    pstItem = &astItem[2];
    float originPosZ;
    if (3 != sscanf(pstItem->strContent, "[%f,%f,%f]", &stOriginPos.x, &stOriginPos.y, &originPosZ)) {
        ST_LOG_ERR("origin Content(%s) Trans Fail.", pstItem->strContent);
        return -1;
    }
    ST_LOG_INFO("origin Content(%s), x=%f, y=%f.", pstItem->strContent, stOriginPos.x, stOriginPos.y);

    return 0;
}

int MAP_YAML_FILE_C::Write(const string &strMapPath, float resolution, const COORDINATE_2D_STRU &stOriginPos) const
{
    string strYamlFilePath;
    GetYamlFilePath(strMapPath, strYamlFilePath);

    string strPgmFileName;
    GetPgmFileName(strMapPath, strPgmFileName);

    SIMPLE_YAML_FILE_C objYamlFile(strYamlFilePath.c_str());
    YAML_FILE_ITEM_STRU astItem[6];
    astItem[0].pStrTag = "image";
    sprintf(astItem[0].strContent, "%s", strPgmFileName.c_str());
    astItem[1].pStrTag = "resolution";
    sprintf(astItem[1].strContent, "%.6f", resolution);
    astItem[2].pStrTag = "origin";
    sprintf(astItem[2].strContent, "[%.6f, %.6f, 0.000000]", stOriginPos.x, stOriginPos.y);
    astItem[3].pStrTag = "negate";
    sprintf(astItem[3].strContent, "0");
    astItem[4].pStrTag = "occupied_thresh";
    sprintf(astItem[4].strContent, "0.65");
    astItem[5].pStrTag = "free_thresh";
    sprintf(astItem[5].strContent, "0.196");
    if (0 != objYamlFile.WriteItem(astItem, ARRAY_SIZE(astItem))) {
        ST_LOG_ERR("Write to file fail.");
        return -1;
    }

    return 0;
}

void MAP_YAML_FILE_C::GetYamlFilePath(const string &strMapPath, string &strYamlFilePath) const
{
    strYamlFilePath = strMapPath + ".yaml";
}

void MAP_YAML_FILE_C::GetPgmFileName(const string &strMapPath, string &strPgmFileName) const
{
    string strMapName;

    FILE_OPRT_C objFileOpr;
    objFileOpr.GetFileNameFrmPath(strMapPath, strMapName);

    strPgmFileName = strMapName + ".pgm";
}

PGM_FILE_C::PGM_FILE_C(void)
{
}

PGM_FILE_C::~PGM_FILE_C(void)
{
}

int PGM_FILE_C::ReadImageSize(const string &strMapPath, unsigned int &imageCols, unsigned int &imageRows)
{
    string strPgmFilePath;
    GetPgmFilePath(strMapPath, strPgmFilePath);

    FILE *pfRead = fopen(strPgmFilePath.c_str(), "rb");
    if (pfRead == NULL) {
        return -1;
    }

    if (0 != ReadFileHead(pfRead, imageCols, imageRows)) {
        fclose(pfRead);
        return -1;
    }

    fclose(pfRead);
    return 0;
}

int PGM_FILE_C::ReadImage(const string &strMapPath, unsigned char *pucImage, unsigned int imageMaxSize)
{
    string strPgmFilePath;
    GetPgmFilePath(strMapPath, strPgmFilePath);

    FILE *pfRead = fopen(strPgmFilePath.c_str(), "rb");
    if (pfRead == NULL) {
        ST_LOG_ERR("Open %s Fail.", strMapPath.c_str());
        return -1;
    }

    unsigned int imageCols;
    unsigned int imageRows;
    if (   (0 != ReadFileHead(pfRead, imageCols, imageRows))
        || (imageMaxSize != (imageCols * imageRows))) {
        ST_LOG_ERR("Image Size(%u %u) Not Equ MaxSize(%u).", imageCols, imageRows, imageMaxSize);
        fclose(pfRead);
        return -1;
    }

    if (imageMaxSize != fread(pucImage, 1, imageMaxSize, pfRead)) {
        ST_LOG_ERR("Read Fail.");
        fclose(pfRead);
        return -1;
    }

    fclose(pfRead);
    return 0;
}

void PGM_FILE_C::SaveFile(const string &strMapPath, unsigned char *pucImage, unsigned int imageCols, unsigned int imageRows)
{
    string strPgmFilePath;
    GetPgmFilePath(strMapPath, strPgmFilePath);

    char acFileHead[100] = {0};
    sprintf(acFileHead, "P5\n%d %d\n255\n", imageCols, imageRows);

    FILE *pfSave = fopen(strPgmFilePath.c_str(), "wb");
    if (pfSave == NULL) {
        return;
    }

    (void)fwrite(acFileHead, 1, strlen(acFileHead), pfSave);
    (void)fwrite(pucImage, 1, imageCols * imageRows, pfSave);

    fclose(pfSave);
}

void PGM_FILE_C::GetPgmFilePath(const string &strMapPath, string &strPgmFilePath)
{
    strPgmFilePath = strMapPath + ".pgm";
}

int PGM_FILE_C::ReadNextLine(FILE *pfRead, char *pucLine, int maxLineLen)
{
    char *pcRslt;

    do {
        pcRslt = fgets(pucLine, maxLineLen, pfRead);
        /* # 之后的为解释文本，跳过 */
    } while ((pcRslt != NULL) && (*pcRslt == '#'));

    return (pcRslt == NULL) ? -1 : 0;
}

int PGM_FILE_C::ReadFileHead(FILE *pfRead, unsigned int &imageCols, unsigned int &imageRows)
{
    char acLine[1024];      /* 假设解释信息最长1024字节 */
    unsigned int imageMaxVal;

    if (   (0 != ReadNextLine(pfRead, acLine, sizeof(acLine)))
        || (acLine[0] != 'P') || (acLine[1] != '5')) {
        return -1;
    }

    if (   (0 != ReadNextLine(pfRead, acLine, sizeof(acLine)))
        || (2 != sscanf(acLine, "%u %u", &imageCols, &imageRows))) {
        return -1;
    }

    if (   (0 != ReadNextLine(pfRead, acLine, sizeof(acLine)))
        || (1 != sscanf(acLine, "%u", &imageMaxVal))
        || (imageMaxVal > 255)) {
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 创建图像
修改记录:
**************************************************************************************/
void GRAY_IMAGE_C::CreateImage(UINT32 sizeX, UINT32 sizeY)
{
    FreeImage();

    mapSizeX_ = sizeX;
    mapSizeY_ = sizeY;
    pucMap_   = new unsigned char[mapSizeX_ * mapSizeY_];
}

/**************************************************************************************
功能描述: 创建图像
修改记录:
**************************************************************************************/
void GRAY_IMAGE_C::CreateImage(UINT32 sizeX, UINT32 sizeY, UINT08 defaultVal)
{
    CreateImage(sizeX, sizeY);

    memset(pucMap_, defaultVal, mapSizeX_ * mapSizeY_);
}

/**************************************************************************************
功能描述: 创建图像
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::CreateImage(UINT32 sizeX, UINT32 sizeY, UINT08 *pucImage)
{
    R_ASSERT(pucImage != NULL, -1);

    CreateImage(sizeX, sizeY);

    /* 原始图像要上下颠倒 */
    TurnMapUpsideDown(pucImage, pucMap_);

    return 0;
}

/**************************************************************************************
功能描述: 从现有图像中拷贝一部分到本图像
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::CopyImage(const GRAY_IMAGE_C &objSrcImage, const IMAGE_COORDINATE_STRU &stCopyStart, const IMAGE_COORDINATE_STRU &stCopySize)
{
    R_ASSERT_4(objSrcImage.IsImageValid(),               -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);
    R_ASSERT_4(objSrcImage.IsCoordValid(stCopyStart),    -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);
    R_ASSERT_4((stCopySize.x > 0) && (stCopySize.y > 0), -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);

    IMAGE_COORDINATE_STRU stCopyMax;
    stCopyMax.x = stCopyStart.x + stCopySize.x - 1;
    stCopyMax.y = stCopyStart.y + stCopySize.y - 1;
    R_ASSERT_4(objSrcImage.IsCoordValid(stCopyMax),      -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);

    CreateImage(UINT32(stCopySize.x), UINT32(stCopySize.y));

    for (INT32 loop = 0; loop < stCopySize.y; loop++) {
        memcpy(GetCellPtrUnsafe(0, loop), objSrcImage.GetCellPtrUnsafe(stCopyStart.x, stCopyStart.y + loop), stCopySize.x);
    }

    mapSizeX_ = stCopySize.x;
    mapSizeY_ = stCopySize.y;
    return 0;
}

INT32 GRAY_IMAGE_C::CopyImage(const GRAY_IMAGE_C &objSrcImage)
{
    R_ASSERT(objSrcImage.IsImageValid(), -1);

    CreateImage(objSrcImage.mapSizeX_, objSrcImage.mapSizeY_);

    memcpy(pucMap_, objSrcImage.pucMap_, mapSizeX_ * mapSizeY_);
    return 0;
}

/**************************************************************************************
功能描述: 将另一张图片上指定范围内指定值的像素拷贝到本图片对应范围
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::CopyImageAreaOfVal(const GRAY_IMAGE_C &objSrcImage, const IMAGE_COORDINATE_STRU &stStartOfSrc, const IMAGE_COORDINATE_STRU &stStartOfLoc, const IMAGE_COORDINATE_STRU &stCopySize, UINT08 copyVal)
{
    R_ASSERT(this->IsImageValid(),       -1);
    R_ASSERT(objSrcImage.IsImageValid(), -1);

    IMAGE_COORDINATE_STRU stStopOfLoc{stStartOfLoc.x + stCopySize.x - 1, stStartOfLoc.y + stCopySize.y - 1};
    IMAGE_COORDINATE_STRU stStopOfSrc{stStartOfSrc.x + stCopySize.x - 1, stStartOfSrc.y + stCopySize.y - 1};
    R_ASSERT_4(this->IsCoordValid(stStartOfLoc),       -1, stStartOfLoc.x,  stStartOfLoc.y,  this->mapSizeX_,       this->mapSizeY_);
    R_ASSERT_4(this->IsCoordValid(stStopOfLoc),        -1, stStopOfLoc.x,   stStopOfLoc.y,   this->mapSizeX_,       this->mapSizeY_);
    R_ASSERT_4(objSrcImage.IsCoordValid(stStartOfSrc), -1, stStartOfSrc.x,  stStartOfSrc.y,  objSrcImage.mapSizeX_, objSrcImage.mapSizeY_);
    R_ASSERT_4(objSrcImage.IsCoordValid(stStopOfSrc),  -1, stStopOfSrc.x,   stStopOfSrc.y,   objSrcImage.mapSizeX_, objSrcImage.mapSizeY_);

    for (INT32 loopOfRow = 0; loopOfRow < stCopySize.y; ++loopOfRow) {
        UINT08 *pucStartOfRowOfSrc = objSrcImage.GetCellPtrUnsafe(stStartOfSrc.x, stStartOfSrc.y + loopOfRow);
        UINT08 *pucStartOfRowOfLoc = this->GetCellPtrUnsafe(stStartOfLoc.x, stStartOfLoc.y + loopOfRow);

        BOOL    isExistCopy = false;
        UINT08 *pucCopyStartOfSrc;
        UINT08 *pucCopyStartOfLoc;
        UINT32  copyNum;

        for (INT32 loopOfCol = 0; loopOfCol < stCopySize.x; ++loopOfCol) {
            UINT08 *pucCurCellOfSrc = pucStartOfRowOfSrc + loopOfCol;
            if (*pucCurCellOfSrc == copyVal) {
                if (!isExistCopy) {
                    isExistCopy         = true;
                    pucCopyStartOfSrc = pucCurCellOfSrc;
                    pucCopyStartOfLoc  = pucStartOfRowOfLoc + loopOfCol;
                    copyNum             = 1;
                } else {
                    ++copyNum;
                }
            } else if (isExistCopy) {
                (void)memcpy(pucCopyStartOfLoc, pucCopyStartOfSrc, copyNum);
                isExistCopy = false;
            } else {
                /* nothing to do */
            }
        }

        if (isExistCopy) {
            (void)memcpy(pucCopyStartOfLoc, pucCopyStartOfSrc, copyNum);
        }
    }

    return 0;
}

/**************************************************************************************
功能描述: 从pgm中加载地图
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::LoadImage(const string &strMapPath)
{
    FreeImage();

    PGM_FILE_C objPgmFile;
    if (0 != objPgmFile.ReadImageSize(strMapPath, mapSizeX_, mapSizeY_)) {
        ST_LOG_ERR("Read Pgm Size Fail.");
        return -1;
    }

    UINT32 mapSize =  mapSizeX_ * mapSizeY_;

    unsigned char *pucMapTmp = new unsigned char[mapSize];
    if (0 != objPgmFile.ReadImage(strMapPath, pucMapTmp, mapSize)) {
        delete[] pucMapTmp;
        ST_LOG_ERR("Read Pgm Image Fail.");
        return -1;
    }

    /* 由于图像坐标系和地图坐标系Y轴时反的，所以要将图片中行上下颠倒 */
    pucMap_ = new unsigned char[mapSize];
    TurnMapUpsideDown(pucMapTmp, pucMap_);

    delete[] pucMapTmp;
    ST_LOG_INFO("Load %s Succ.", strMapPath.c_str());
    return 0;
}

INT32 GRAY_IMAGE_C::SaveImage(const string &strMapPath) const
{
    R_ASSERT(pucMap_ != NULL, -1);

    /* 写PGM文件 */
    /* 地图读取的时候上下反过来了，还要反回去 */
    unsigned char *pucMapTmp = new unsigned char[mapSizeX_ * mapSizeY_];
    TurnMapUpsideDown(pucMap_, pucMapTmp);

    PGM_FILE_C objPgmFile;
    objPgmFile.SaveFile(strMapPath, pucMapTmp, mapSizeX_, mapSizeY_);

    delete[] pucMapTmp;
    return 0;
}

void GRAY_IMAGE_C::FreeImage(void)
{
    if (pucMap_ != NULL) {
        delete[] pucMap_;
        pucMap_ = NULL;
    }
}

void GRAY_IMAGE_C::SwapImage(GRAY_IMAGE_C &objOtherImage)
{
    SWAP_DATA(this->mapSizeX_, objOtherImage.mapSizeX_);
    SWAP_DATA(this->mapSizeY_, objOtherImage.mapSizeY_);
    SWAP_DATA(this->pucMap_,   objOtherImage.pucMap_);
}

INT32 GRAY_IMAGE_C::GetImageSize(UINT32 &sizeX, UINT32 &sizeY) const
{
    R_ASSERT(IsImageValid(), -1);

    sizeX = mapSizeX_;
    sizeY = mapSizeY_;
    return 0;
}

INT32 GRAY_IMAGE_C::GetImageSize(IMAGE_COORDINATE_STRU &stSize) const
{
    R_ASSERT(IsImageValid(), -1);

    stSize.x = mapSizeX_;
    stSize.y = mapSizeY_;
    return 0;
}

/**************************************************************************************
功能描述: 获取指定两点的线段对应的栅格列表，列表增量添加
          从起点到终点有序排列，含起点和终点
修改记录:
**************************************************************************************/
void GRAY_IMAGE_C::GetCellsOfLineSeg(const IMAGE_COORDINATE_STRU &stBeginCell, const IMAGE_COORDINATE_STRU &stEndCell, BOOL isGetBegin, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const
{
    INT32 deltaCoordX    = stEndCell.x - stBeginCell.x;
    INT32 deltaCoordY    = stEndCell.y - stBeginCell.y;
    INT32 deltaCoordXAbs = ABS(deltaCoordX);
    INT32 deltaCoordYAbs = ABS(deltaCoordY);

    INT32 totalCellsNum  = INT32(vectCell.size()) + deltaCoordXAbs + deltaCoordYAbs + 1;
    vectCell.reserve(totalCellsNum);

    if (isGetBegin) {
        vectCell.push_back(stBeginCell);
    }

    /* 起点和终点一样时不能往下处理。 */
    if ((deltaCoordXAbs == 0) && (deltaCoordYAbs == 0)) {
        return;
    }

    float incX;
    float incY;
    INT32 diffMax;
    if (deltaCoordXAbs >= deltaCoordYAbs) {
        incX    = 1.0;
        incY    = float(deltaCoordYAbs) / float(deltaCoordXAbs);
        diffMax = deltaCoordXAbs;
    } else {
        incX    = float(deltaCoordXAbs) / float(deltaCoordYAbs);
        incY    = 1.0;
        diffMax = deltaCoordYAbs;
    }

    INT32 signIncX   = SIGN(deltaCoordX);
    INT32 signIncY   = SIGN(deltaCoordY);
    float deltaX     = 0;
    float deltaY     = 0;
    INT32 imageDistX = 0;
    INT32 imageDistY = 0;

    for (INT32 loop = 0; loop < diffMax; ++loop) {
        deltaX += incX;
        if (deltaX >= 1.0) {
            ++imageDistX;
            deltaX -= 1.0;
        }

        deltaY += incY;
        if (deltaY >= 1.0) {
            ++imageDistY;
            deltaY -= 1.0;
        }

        IMAGE_COORDINATE_STRU stCell;
        stCell.x = stBeginCell.x + (imageDistX + ROUND_UP_DOWN(deltaX)) * signIncX;
        stCell.y = stBeginCell.y + (imageDistY + ROUND_UP_DOWN(deltaY)) * signIncY;
        vectCell.push_back(stCell);
    }
}

void GRAY_IMAGE_C::SetCells(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, UINT08 setVal)
{
    for (const IMAGE_COORDINATE_STRU &stCell : vectCell) {
        (void)SetCell(stCell, setVal);
    }
}

void GRAY_IMAGE_C::SetCellsUnsafe(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, UINT08 setVal)
{
    for (const IMAGE_COORDINATE_STRU &stCell : vectCell) {
        SetCellUnsafe(stCell, setVal);
    }
}

INT32 GRAY_IMAGE_C::SetCellsStrictly(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, UINT08 setVal)
{
    for (const IMAGE_COORDINATE_STRU &stCell : vectCell) {
        INT32 rsltOfSetCell = SetCell(stCell, setVal);
        R_ASSERT_4(rsltOfSetCell == 0, -1, stCell.x, stCell.y, mapSizeX_, mapSizeY_);
    }

    return 0;
}

/**************************************************************************************
功能描述: 向右扩散栅格
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::ExpandCellsRight(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, UINT08 expandVal, UINT08 coverVal)
{
    for (const IMAGE_COORDINATE_STRU &stSrcCell : vectSrcCell) {
        UINT08 *pucSrcCell = GetCellPtr(stSrcCell);
        R_ASSERT_4(pucSrcCell != nullptr, -1, stSrcCell.x, stSrcCell.y, GetImageSizeX(), GetImageSizeY());

        /* 原始栅格一定赋值。 */
        UINT32 expandNumMax = GetImageSizeX() - stSrcCell.x;
        UINT32 expandNum    = 1;
        while ((expandNum < expandNumMax) && (pucSrcCell[expandNum] == coverVal)) {
            ++expandNum;
        }

        memset(pucSrcCell, expandVal, expandNum);
    }

    return 0;
}

/**************************************************************************************
功能描述: 扩散栅格
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::ExpandCells(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, UINT08 expandVal, UINT32 expandDist, UINT08 coverVal)
{
    R_ASSERT(expandDist > 0, -1);

    auto pfExpandCellsOnce = [this](const std::vector<IMAGE_COORDINATE_STRU> &vectCellSrc, UINT08 expandVal, UINT08 coverVal, std::vector<IMAGE_COORDINATE_STRU> &vectCellTgt)
    {
        vectCellTgt.clear();
        vectCellTgt.reserve(std::min(UINT32(vectCellSrc.size()) * 4, this->GetImageMemSize()));

        for (const IMAGE_COORDINATE_STRU &stProcCell : vectCellSrc) {
            const IMAGE_COORDINATE_STRU stUpCell   {stProcCell.x,     stProcCell.y + 1};
            const IMAGE_COORDINATE_STRU stDownCell {stProcCell.x,     stProcCell.y - 1};
            const IMAGE_COORDINATE_STRU stLeftCell {stProcCell.x - 1, stProcCell.y};
            const IMAGE_COORDINATE_STRU stRightCell{stProcCell.x + 1, stProcCell.y};

            UINT08 cellVal;
            if ((0 == this->GetCellVal(stUpCell,    cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stUpCell,    expandVal); vectCellTgt.emplace_back(stUpCell); }
            if ((0 == this->GetCellVal(stDownCell,  cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stDownCell,  expandVal); vectCellTgt.emplace_back(stDownCell); }
            if ((0 == this->GetCellVal(stLeftCell,  cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stLeftCell,  expandVal); vectCellTgt.emplace_back(stLeftCell); }
            if ((0 == this->GetCellVal(stRightCell, cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stRightCell, expandVal); vectCellTgt.emplace_back(stRightCell); }
        }
    };

    INT32 rsltOfSetSrcCells = SetCellsStrictly(vectSrcCell, expandVal);
    R_ASSERT_1(rsltOfSetSrcCells == 0, -1, vectSrcCell.size());

    std::vector<IMAGE_COORDINATE_STRU> vectCellSrc;
    std::vector<IMAGE_COORDINATE_STRU> vectCellTgt;
    for (UINT32 loop = 0; loop < expandDist; ++loop) {
        if (loop == 0) {
            pfExpandCellsOnce(vectSrcCell, expandVal, coverVal, vectCellTgt);
        } else {
            pfExpandCellsOnce(vectCellSrc, expandVal, coverVal, vectCellTgt);
        }

        vectCellSrc.swap(vectCellTgt);
        if (vectCellSrc.empty()) {
            break;
        }
    }

    return 0;
}

UINT32 GRAY_IMAGE_C::GetImageMemSize(void) const
{
    return IsImageValid() ? (mapSizeX_ * mapSizeY_) : 0;
}

INT32 GRAY_IMAGE_C::GetRightTopCoord(IMAGE_COORDINATE_STRU &stRightTopCoord) const
{
    R_ASSERT(IsImageValid(), -1);

    stRightTopCoord.x = mapSizeX_ - 1;
    stRightTopCoord.y = mapSizeY_ - 1;

    return 0;
}

INT32 GRAY_IMAGE_C::TransSrcImageCoordToLoc(const IMAGE_COORDINATE_STRU &stSrcImageCoord, IMAGE_COORDINATE_STRU &stLocImageCoord) const
{
    if (!IsCoordValid(stSrcImageCoord)) {
        return -1;
    }

    stLocImageCoord.x = stSrcImageCoord.x;
    stLocImageCoord.y = ((int)mapSizeY_ - 1) - stSrcImageCoord.y;
    return 0;
}

void GRAY_IMAGE_C::SetImage(unsigned char setVal)
{
    if (pucMap_ == NULL) {
        return;
    }

    memset(pucMap_, setVal, mapSizeX_ * mapSizeY_);
}

/**************************************************************************************
功能描述: 设置指定曲线的左右两侧像素
修改记录:
**************************************************************************************/
INT32 GRAY_IMAGE_C::SetLeftAndRightPixelsOfLine(const std::vector<IMAGE_COORDINATE_STRU> &vectLine, UINT08 setLeftVal, UINT08 setRightVal, UINT08 splitVal, UINT32 maxStepLen)
{
    UINT32 lineSize = UINT32(vectLine.size());
    R_ASSERT_1(lineSize > 1, -1, lineSize);

    std::vector<IMAGE_COORDINATE_STRU> vectLeftPixel;
    std::vector<IMAGE_COORDINATE_STRU> vectRightPixel;

    vectLeftPixel.reserve(lineSize * 4);
    vectRightPixel.reserve(lineSize * 4);

    /* 第一步，找到指定曲线两侧紧挨着的种子像素 */
    for (UINT32 loop = 1; loop < lineSize; ++loop) {
        const IMAGE_COORDINATE_STRU &stCurPixel  = vectLine[loop - 1];
        const IMAGE_COORDINATE_STRU &stNextPixel = vectLine[loop];

        IMAGE_COORDINATE_STRU stNextPixelRelPos;
        stNextPixelRelPos.x = stNextPixel.x - stCurPixel.x;
        stNextPixelRelPos.y = stNextPixel.y - stCurPixel.y;

        #define PROC_AROUND_PIXEL_SIDE(xOfRelPos_, yOfRelPos_)                                                                      \
            do {                                                                                                                    \
                IMAGE_COORDINATE_STRU stAroundPixel;                                                                                \
                stAroundPixel.x = stCurPixel.x + (xOfRelPos_);                                                                      \
                stAroundPixel.y = stCurPixel.y + (yOfRelPos_);                                                                      \
                UINT08 *pucPixel = GetCellPtr(stAroundPixel.x, stAroundPixel.y);                                                    \
                if ((pucPixel != NULL) && (*pucPixel != setLeftVal) && (*pucPixel != setRightVal) && (*pucPixel != splitVal)) {     \
                    float crossMultiVal = stNextPixelRelPos.x * (yOfRelPos_) - stNextPixelRelPos.y * (xOfRelPos_);                  \
                    if (IS_FLOAT_ALMOST_EQUAL_ZERO(crossMultiVal)) {                                                                \
                        /* 线上的不要 */                                                                                            \
                    } else if (crossMultiVal > 0) {                                                                                 \
                        vectLeftPixel.push_back(stAroundPixel);                                                                     \
                        *pucPixel = setLeftVal;                                                                                     \
                    } else {                                                                                                        \
                        vectRightPixel.push_back(stAroundPixel);                                                                    \
                        *pucPixel = setRightVal;                                                                                    \
                    }                                                                                                               \
                }                                                                                                                   \
            } while (0)

        PROC_AROUND_PIXEL_SIDE( 0,  1);
        PROC_AROUND_PIXEL_SIDE( 0, -1);
        PROC_AROUND_PIXEL_SIDE(-1,  0);
        PROC_AROUND_PIXEL_SIDE( 1,  0);
    }

    /* 第二步，根据种子像素，向四周扩散指定步长 */
    for (UINT32 loop = 0; loop <= maxStepLen; ++loop) {
        std::vector<IMAGE_COORDINATE_STRU> vectNextStepRightPixel;
        std::vector<IMAGE_COORDINATE_STRU> vectNextStepLeftPixel;

        UINT32 rightPixelSize = vectRightPixel.size();
        UINT32 leftPixelSize  = vectLeftPixel.size();

        vectNextStepRightPixel.reserve(rightPixelSize * 4);
        vectNextStepLeftPixel.reserve(leftPixelSize * 4);

        for (UINT32 loopOfPixel = 0; loopOfPixel < rightPixelSize; ++loopOfPixel) {
            const IMAGE_COORDINATE_STRU &stPixel = vectRightPixel[loopOfPixel];

            #define PROC_RIGHT_PIXEL(xOfRelPos_, yOfRelPos_)                                                                        \
                do {                                                                                                                \
                    IMAGE_COORDINATE_STRU stAroundPixel;                                                                            \
                    stAroundPixel.x = stPixel.x + (xOfRelPos_);                                                                     \
                    stAroundPixel.y = stPixel.y + (yOfRelPos_);                                                                     \
                    UINT08 *pucPixel = GetCellPtr(stAroundPixel.x, stAroundPixel.y);                                                \
                    if ((pucPixel != NULL) && (*pucPixel != setLeftVal) && (*pucPixel != setRightVal) && (*pucPixel != splitVal)) { \
                        vectNextStepRightPixel.push_back(stAroundPixel);                                                            \
                        *pucPixel = setRightVal;                                                                                    \
                    }                                                                                                               \
                } while (0)

            PROC_RIGHT_PIXEL( 0,  1);
            PROC_RIGHT_PIXEL( 0, -1);
            PROC_RIGHT_PIXEL(-1,  0);
            PROC_RIGHT_PIXEL( 1,  0);
        }

        for (UINT32 loopOfPixel = 0; loopOfPixel < leftPixelSize; ++loopOfPixel) {
            const IMAGE_COORDINATE_STRU &stPixel = vectLeftPixel[loopOfPixel];

            #define PROC_LEFT_PIXEL(xOfRelPos_, yOfRelPos_)                                                                         \
                do {                                                                                                                \
                    IMAGE_COORDINATE_STRU stAroundPixel;                                                                            \
                    stAroundPixel.x = stPixel.x + (xOfRelPos_);                                                                     \
                    stAroundPixel.y = stPixel.y + (yOfRelPos_);                                                                     \
                    UINT08 *pucPixel = GetCellPtr(stAroundPixel.x, stAroundPixel.y);                                                \
                    if ((pucPixel != NULL) && (*pucPixel != setLeftVal) && (*pucPixel != setRightVal) && (*pucPixel != splitVal)) { \
                        vectNextStepLeftPixel.push_back(stAroundPixel);                                                             \
                        *pucPixel = setLeftVal;                                                                                     \
                    }                                                                                                               \
                } while (0)

            PROC_LEFT_PIXEL( 0,  1);
            PROC_LEFT_PIXEL( 0, -1);
            PROC_LEFT_PIXEL(-1,  0);
            PROC_LEFT_PIXEL( 1,  0);
        }

        vectNextStepRightPixel.swap(vectRightPixel);
        vectNextStepLeftPixel.swap(vectLeftPixel);
    }

    return 0;
}

INT32 GRAY_IMAGE_C::GetCellsInArea(UINT08 cellVal, const IMAGE_COORDINATE_STRU &stAreaLBCoord, const IMAGE_COORDINATE_STRU &stAreaSize, std::vector<IMAGE_COORDINATE_STRU> &vectCellList) const
{
    R_ASSERT(IsImageValid(), -1);

    IMAGE_COORDINATE_STRU stMaxCell{stAreaLBCoord.x + stAreaSize.x - 1, stAreaLBCoord.y + stAreaSize.y - 1};
    R_ASSERT_4(IsCoordValid(stMaxCell), -1, stMaxCell.x, stMaxCell.y, mapSizeX_, mapSizeY_);

    IMAGE_COORDINATE_STRU stCoord;
    for (INT32 loopX = 0; loopX < stAreaSize.x; ++loopX) {
        stCoord.x = stAreaLBCoord.x + loopX;

        for (INT32 loopY = 0; loopY < stAreaSize.y; ++loopY) {
            stCoord.y = stAreaLBCoord.y + loopY;

            if (cellVal == GetCellValUnsafe(stCoord)) {
                vectCellList.push_back(stCoord);
            }
        }
    }

    return 0;
}

void GRAY_IMAGE_C::TurnMapUpsideDown(unsigned char *pucSrcMap, unsigned char *pucTgtMap) const
{
    for (unsigned int loop = 0; loop < mapSizeY_; loop++) {
        memcpy(&pucTgtMap[(mapSizeY_ - loop - 1) * mapSizeX_],
               &pucSrcMap[loop * mapSizeX_],
               mapSizeX_);
    }
}

// 设置膨胀距离
void GRAY_MAP_C::SetInflationDis(float dis)
{
    inscribe_num_ = (dis / GetResolution()) * (dis / GetResolution());
    weak_inscribe_num_ = ((dis - 0.12f) / GetResolution()) * ((dis - 0.12f) / GetResolution());
}

// 填充膨胀
void GRAY_MAP_C::FillInflation(void)
{
    int size_x = static_cast<int>(GetImageSizeX());
    int size_y = static_cast<int>(GetImageSizeY());
    seen_.resize(size_x * size_y, false);
    std::fill(seen_.begin(), seen_.begin() + size_x * size_y, false);

    for (int i = 0; i < size_x; ++i)
    {
        for (int j = 0; j < size_y; ++j)
        {
            unsigned char curr_cost = GetCellCost(i, j);
            if (curr_cost == cellCostObst_ || curr_cost == cellCostBorder_)
            {
                int index = j * size_x + i;
                CellData cell = {index, i, j, i, j, 0};
                inflation_cells_.insert(cell);
            }
            if (curr_cost == cellCostWeakObst_)
            {
                int index = j * size_x + i;
                CellData cell = {index, i, j, i, j, 0};
                weak_inflation_cells_.insert(cell);
            }
        }
    }

    while (!weak_inflation_cells_.empty())
    {
        CellData cell = *(weak_inflation_cells_.begin());
        weak_inflation_cells_.erase(weak_inflation_cells_.begin());

        if (seen_[cell.index])
        {
            continue;
        }
        seen_[cell.index] = true;

        unsigned char cost = (cell.dis_square == 0 ? cellCostWeakObst_ : (cell.dis_square <= weak_inscribe_num_ ? cellCostInscribe_ : cellCostNull_));

        SetCell(cell.x, cell.y, cost);

        for (int i = 0; i < 4; ++i)
        {
            int nx = cell.x + g_dirs[i][0];
            int ny = cell.y + g_dirs[i][1];

            if (nx < 0 || nx >= size_x || ny < 0 || ny >= size_y)
            {
                continue;
            }

            int index = ny * size_x + nx;
            if (!seen_[index])
            {
                int dis_x = nx - cell.sx;
                int dis_y = ny - cell.sy;
                int dis_square = dis_x * dis_x + dis_y * dis_y;

                if (dis_square <= weak_inscribe_num_)
                {
                    CellData neighbor_cell = {index, nx, ny, cell.sx, cell.sy, dis_square};
                    weak_inflation_cells_.insert(neighbor_cell);
                }
            }
        }
    }

    std::fill(seen_.begin(), seen_.begin() + size_x * size_y, false);

    while (!inflation_cells_.empty())
    {
        CellData cell = *(inflation_cells_.begin());
        inflation_cells_.erase(inflation_cells_.begin());

        if (seen_[cell.index])
        {
            continue;
        }
        seen_[cell.index] = true;

        unsigned char cost = (cell.dis_square == 0 ? cellCostObst_ : (cell.dis_square <= inscribe_num_ ? cellCostInscribe_ : cellCostNull_));

        if (cost != cellCostInscribe_ || GetCellCost(cell.x, cell.y) != cellCostWeakObst_)
        {
            SetCell(cell.x, cell.y, cost);
        }

        for (int i = 0; i < 4; ++i)
        {
            int nx = cell.x + g_dirs[i][0];
            int ny = cell.y + g_dirs[i][1];

            if (nx < 0 || nx >= size_x || ny < 0 || ny >= size_y)
            {
                continue;
            }

            int index = ny * size_x + nx;
            if (!seen_[index])
            {
                int dis_x = nx - cell.sx;
                int dis_y = ny - cell.sy;
                int dis_square = dis_x * dis_x + dis_y * dis_y;

                if (dis_square <= inscribe_num_)
                {
                    CellData neighbor_cell = {index, nx, ny, cell.sx, cell.sy, dis_square};
                    inflation_cells_.insert(neighbor_cell);
                }
            }
        }
    }
}

/* 该函数为定制使用，其它地方不要使用 */
INT32 GRAY_MAP_C::GetMapInfo(const string &strMapPath, string &strPgmFileNameFrmYaml, IMAGE_COORDINATE_STRU &stMapSize, COORDINATE_2D_STRU &stOriginPos)
{
    /* 从YAML文件中读取原点坐标 */
    float  mapResolution;
    MAP_YAML_FILE_C objYamlFile;
    if (0 != objYamlFile.Read(strMapPath, strPgmFileNameFrmYaml, mapResolution, stOriginPos)) {
        ST_LOG_ERR("Read Yaml File Fail.");
        return -1;
    }

    /* 从PGM文件中读取地图大小 *//* 注意，pgm文件名不要从yaml中度，直接改变文件扩展名即可。 */
    UINT32   sizeX;
    UINT32   sizeY;
    PGM_FILE_C objPgmFile;
    if (0 != objPgmFile.ReadImageSize(strMapPath, sizeX, sizeY)) {
        ST_LOG_ERR("Read Pgm Size Fail.");
        return -1;
    }
    stMapSize.x = (int)sizeX;
    stMapSize.y = (int)sizeY;

    return 0;
}

INT32 GRAY_MAP_C::GetMapInfo(COORDINATE_2D_STRU &stOriginPos, float &resolution, UINT32 &sizeX, UINT32 &sizeY, UINT08 **ppucMap) const
{
    R_ASSERT(IsValid(), -1);

    stOriginPos = GetOriginPos();
    resolution  = GetResolution();
    sizeX       = GetImageSizeX();
    sizeY       = GetImageSizeY();
    *ppucMap    = GetCellPtrUnsafe(0, 0);

    return 0;
}

INT32 GRAY_MAP_C::GetMapSize(COORDINATE_2D_STRU &stSize) const
{
    IMAGE_COORDINATE_STRU stImageSize;
    INT32 rsltOfGetImageSize = GetImageSize(stImageSize);
    R_ASSERT(rsltOfGetImageSize == 0, -1);

    stSize.x = GetResolution() * float(stImageSize.x);
    stSize.y = GetResolution() * float(stImageSize.y);
    return 0;
}

INT32 GRAY_MAP_C::GetMapArea(AREA_C &objArea) const
{
    COORDINATE_2D_STRU stRightTopPoint;
    INT32 rsltOfGetRightTopPoint = GetRightTopPoint(stRightTopPoint);
    R_ASSERT(rsltOfGetRightTopPoint == 0, -1);

    objArea.IncludePoint(GetOriginPos());
    objArea.IncludePoint(stRightTopPoint);
    return 0;
}

UINT32 GRAY_MAP_C::GetDistByCell(float dist) const
{
    return UINT32(dist * GetReciprocalOfRes() + 0.5);
}

/**************************************************************************************
功能描述:  将图片位置转换为地图位置
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::TransPictureCoordToPos(const IMAGE_COORDINATE_STRU &stPictureCoord, COORDINATE_2D_STRU &stMapPos) const
{
    IMAGE_COORDINATE_STRU stMapCoord;

    if (0 != TransSrcImageCoordToLoc(stPictureCoord, stMapCoord)) {
        return -1;
    }

    ConvMapCoordToPos(stMapCoord, stMapPos);
    return 0;
}

/**************************************************************************************
功能描述:  加载地图
           不根据yaml文件中的pgm文件名加载
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::LoadMap(const string &strMapPath, bool isExistYamlFile)
{
    /* 存在yaml文件时，读取相关信息，不存在时采用默认参数 */
    if (isExistYamlFile) {
        std::string        strPgmFileName;      /* 暂时不用这个文件名来读PGM */
        float              resolution;
        COORDINATE_2D_STRU stOriginPos;

        MAP_YAML_FILE_C objYamlFile;
        if (0 != objYamlFile.Read(strMapPath, strPgmFileName, resolution, stOriginPos)) {
            ST_LOG_ERR("Read Yaml File Fail.");
            return -1;
        }

        SetOriginAlignedSw(false);      /* 已经生成地图了，无法进行调整。 */
        SetOriginPos(stOriginPos);
        SetResolution(resolution);

    } else {
        SetOriginAlignedSw(false);      /* 已经生成地图了，无法进行调整。 */
        SetOriginPos(COORDINATE_2D_STRU(0.0, 0.0));
        SetResolution(MAP_RESOLUTION);
    }

    if (0 != LoadImage(strMapPath)) {
        ST_LOG_ERR("Read Pgm File Fail.");
        return -1;
    }

    ST_LOG_INFO("Load Map(%s) Succ.", strMapPath.c_str());
    return 0;
}

INT32 GRAY_MAP_C::SaveMap(const string &strMapPath) const
{
    if (0 != SaveImage(strMapPath)) {
        ST_LOG_ERR("Save Image File Fail.");
        return -1;
    }

    /* 写YAML文件 */
    MAP_YAML_FILE_C objYamlFile;
    if (0 != objYamlFile.Write(strMapPath, GetResolution(), GetOriginPos())) {
        ST_LOG_ERR("Save Yaml Fail.");
        return -1;
    }

    ST_LOG_INFO("Save %s Succ.", strMapPath.c_str());
    return 0;
}

/**************************************************************************************
功能描述:  创建地图
修改记录:
**************************************************************************************/
void GRAY_MAP_C::CreateMap(const COORDINATE_2D_STRU &stOriginPos, UINT32 mapSizeX, UINT32 mapSizeY, BOOL isDefaultNull, float resolution)
{
    SetOriginPos(AlignOriginPos(stOriginPos, resolution));
    SetResolution(resolution);

    UINT08 defaultVal = isDefaultNull ? cellCostNull_ : cellCostUnknown_;
    CreateImage(mapSizeX, mapSizeY, defaultVal);
}

/**************************************************************************************
功能描述:  创建地图
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::CreateMap(const COORDINATE_2D_STRU &stOriginPos, UINT32 mapSizeX, UINT32 mapSizeY, UINT08 *pucImage, float resolution)
{
    SetOriginPos(AlignOriginPos(stOriginPos, resolution));
    SetResolution(resolution);

    INT32 rsltOfCreateImage = CreateImage(mapSizeX, mapSizeY, pucImage);
    R_ASSERT_2(rsltOfCreateImage == 0, -1, mapSizeX, mapSizeX);

    return 0;
}

/**************************************************************************************
功能描述:  创建地图
           根据左下角和右上角坐标确定地图大小和原点坐标
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::CreateMap(const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint, BOOL isDefaultNull, float resolution)
{
    COORDINATE_2D_STRU    stOriginPos;
    IMAGE_COORDINATE_STRU stMapSize;
    INT32 rsltOfCalcMapInfo = CalcMapInfo(stLeftButtomPoint, stRightTopPoint, resolution, stOriginPos, stMapSize);
    R_ASSERT(rsltOfCalcMapInfo == 0, -1);

    SetOriginPos(stOriginPos);
    SetResolution(resolution);

    UINT08 defaultVal = isDefaultNull ? cellCostNull_ : cellCostUnknown_;
    CreateImage(UINT32(stMapSize.x), UINT32(stMapSize.y), defaultVal);

    return 0;
}

/**************************************************************************************
功能描述:  创建地图
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::CreateMap(UINT32 mapSizeX, UINT32 mapSizeY, UINT08 *pucImage)
{
    SetOriginPos(COORDINATE_2D_STRU(0.0, 0.0));
    SetResolution(MAP_RESOLUTION);

    INT32 rsltOfCreateImage = CreateImage(mapSizeX, mapSizeY, pucImage);
    R_ASSERT_2(rsltOfCreateImage == 0, -1, mapSizeX, mapSizeY);

    return 0;
}

/**************************************************************************************
功能描述:  创建地图
修改记录:
**************************************************************************************/
void GRAY_MAP_C::CreateMap(UINT32 mapSizeX, UINT32 mapSizeY, UINT08 defaultVal, float resolution)
{
    SetOriginPos(COORDINATE_2D_STRU(0.0, 0.0));
    SetResolution(resolution);

    CreateImage(mapSizeX, mapSizeY, defaultVal);
}

INT32 GRAY_MAP_C::CopyMap(const GRAY_MAP_C &objSrcMap, const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint)
{
    R_ASSERT(objSrcMap.IsValid(), -1);

    DeleteMap();

    IMAGE_COORDINATE_STRU stSrcLeftButtomPointCoord;
    IMAGE_COORDINATE_STRU stSrcRightTopPointCoord;
    objSrcMap.ConvPosToMapCoord(stLeftButtomPoint, stSrcLeftButtomPointCoord);
    objSrcMap.ConvPosToMapCoord(stRightTopPoint, stSrcRightTopPointCoord);
    IMAGE_COORDINATE_STRU stCopySize;
    stCopySize.x = stSrcRightTopPointCoord.x - stSrcLeftButtomPointCoord.x + 1;
    stCopySize.y = stSrcRightTopPointCoord.y - stSrcLeftButtomPointCoord.y + 1;

    INT32 rslt = CopyImage(objSrcMap, stSrcLeftButtomPointCoord, stCopySize);
    if (rslt != 0) {
        ST_LOG_ERR("CopyImage() Fail.");
        return -1;
    }

    COORDINATE_2D_STRU stOriginPos;
    objSrcMap.ConvMapCoordToPos(stSrcLeftButtomPointCoord, stOriginPos);

    SetOriginAlignedSw(objSrcMap.IsOriginAligned());
    SetOriginPos(stOriginPos);
    SetResolution(objSrcMap.GetResolution());
    return 0;
}

INT32 GRAY_MAP_C::CopyMap(const GRAY_MAP_C &objSrcMap)
{
    R_ASSERT(objSrcMap.IsValid(), -1);

    DeleteMap();

    INT32 rsltOfCopyImage = CopyImage(objSrcMap);
    R_ASSERT(rsltOfCopyImage == 0, -1);

    SetOriginAlignedSw(objSrcMap.IsOriginAligned());
    SetOriginPos(objSrcMap.GetOriginPos());
    SetResolution(objSrcMap.GetResolution());
    return 0;
}

/**************************************************************************************
功能描述: 将另一个地图中为指定值的栅格复制到本地图同坐标的栅格
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::CopyMapOverlapAreaOfVal(const GRAY_MAP_C &objSrcMap, UINT08 copyVal)
{
    /* 两张地图必须都有效，原点必须都调整对齐了，分辨率一致。 */
    R_ASSERT(IsValid(),                                                         -1);
    R_ASSERT(objSrcMap.IsValid(),                                               -1);
    R_ASSERT(IsOriginAligned(),                                                 -1);
    R_ASSERT(objSrcMap.IsOriginAligned(),                                       -1);
    R_ASSERT(IS_FLOAT_ALMOST_EQUAL(GetResolution(), objSrcMap.GetResolution()), -1);

    /* 计算重叠区域 */
    AREA_C objOverlapArea;
    INT32  rsltOfGetOverlapArea = GetOverlapArea(objSrcMap, objOverlapArea);
    R_ASSERT(rsltOfGetOverlapArea == 0, -1);
    R_ASSERT(objOverlapArea.IsValid(),  -1);

    /* 计算复制区域 */
    IMAGE_COORDINATE_STRU stCopyStartOfSrc;
    (void)objSrcMap.ConvPosToMapCoord(objOverlapArea.stLBPoint, stCopyStartOfSrc);

    IMAGE_COORDINATE_STRU stCopyStartOfLoc;
    (void)this->ConvPosToMapCoord(objOverlapArea.stLBPoint, stCopyStartOfLoc);

    COORDINATE_2D_STRU    stTmp;
    IMAGE_COORDINATE_STRU stCopySize;
    INT32 rsltOfCalcCopySize = CalcMapInfo(objOverlapArea.stLBPoint, objOverlapArea.stRTPoint, GetResolution(), stTmp, stCopySize);
    R_ASSERT(rsltOfCalcCopySize == 0, -1);

    /* 复制区域 */
    INT32 rsltOfCopyImageArea = CopyImageAreaOfVal(objSrcMap, stCopyStartOfSrc, stCopyStartOfLoc, stCopySize, copyVal);
    R_ASSERT(rsltOfCopyImageArea == 0, -1);

    return 0;
}

void GRAY_MAP_C::DeleteMap(void)
{
    FreeImage();
}

void GRAY_MAP_C::SwapMap(GRAY_MAP_C &objOtherMap)
{
    SWAP_DATA(this->isOriginAligned_, objOtherMap.isOriginAligned_);
    SWAP_DATA(this->stOriginPos_,     objOtherMap.stOriginPos_);
    SWAP_DATA(this->imageResolution_, objOtherMap.imageResolution_);
    SWAP_DATA(this->reciprocalOfRes_, objOtherMap.reciprocalOfRes_);

    SwapImage(objOtherMap);
}

void GRAY_MAP_C::SetCellByPos(const COORDINATE_2D_STRU &stPos, unsigned char setVal)
{
    IMAGE_COORDINATE_STRU stCoord;
    ConvPosToMapCoord(stPos, stCoord);

    (void)SetCell(stCoord, setVal);
}

void GRAY_MAP_C::SetCellsByPos(const list<COORDINATE_2D_STRU> &posLst, unsigned char setVal)
{
    for (list<COORDINATE_2D_STRU>::const_iterator iter = posLst.begin();
         iter != posLst.end();
         iter++) {
        SetCellByPos(*iter, setVal);
    }
}

void GRAY_MAP_C::SetCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, UINT08 setVal)
{
    for (vector<COORDINATE_2D_STRU>::const_iterator iter = vectPos.begin();
         iter != vectPos.end();
         iter++) {
        SetCellByPos(*iter, setVal);
    }
}

void GRAY_MAP_C::SetPassableCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, UINT08 setVal)
{
    UINT08 costPassable = GetCostOfNull();

    for (const COORDINATE_2D_STRU &stPos : vectPos) {
        IMAGE_COORDINATE_STRU stCoord;
        ConvPosToMapCoord(stPos, stCoord);

        UINT08 *pValue = GetCellPtr(stCoord);
        if ((pValue != nullptr) && (*pValue == costPassable)) {
            *pValue = setVal;
        }
    }
}

/**************************************************************************************
功能描述: 对线段路径设置对应值
修改记录:
**************************************************************************************/
void GRAY_MAP_C::SetCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, BOOL isSetEnd)
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    UINT32 cellsNum = vctCells.size();
    R_ASSERT(cellsNum > 0, NOTHING);
    UINT32 setCellsNum = isSetEnd ? cellsNum : (cellsNum - 1);

    for (UINT32 loop = 0; loop < setCellsNum; ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        UINT08 *pucCell = GetCellPtr(stCell);
        if (pucCell == nullptr) {
            continue;
        }

        if ((pvectSetPoint != NULL) && (*pucCell != setVal)) {
            pvectSetPoint->push_back(stCell);
        }

        *pucCell = setVal;
    }
}

/**************************************************************************************
功能描述: 将连续点表示的曲线所在的栅格设置为指定值
修改记录:
**************************************************************************************/
void GRAY_MAP_C::SetCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, UINT32 beginIdx, UINT32 endIdx)
{
    UINT32 LineSize = UINT32(vectLine.size());
    if (LineSize == 0) {
        return;
    }

    R_ASSERT_2(beginIdx < LineSize, NOTHING, beginIdx, LineSize);

    UINT32 endIdxEx = MIN(endIdx, LineSize - 1);
    for (UINT32 loop = beginIdx + 1; loop <= endIdxEx; ++loop) {
        SetCellsOfLineSegment(vectLine[loop - 1], vectLine[loop], setVal, pvectSetPoint, false);
    }

    IMAGE_COORDINATE_STRU stEndCell;
    ConvPosToMapCoord(vectLine[endIdxEx], stEndCell);

    UINT08 *pucCell = GetCellPtr(stEndCell);
    if (pucCell != nullptr) {
        if ((pvectSetPoint != NULL) && (*pucCell != setVal)) {
            pvectSetPoint->push_back(stEndCell);
        }

        *pucCell = setVal;
    }
}

/**************************************************************************************
功能描述: 对应的区域设置对应值,只有凸形区域才有效
修改记录:
**************************************************************************************/
void  GRAY_MAP_C::SetCellsOfArea(const std::vector<COORDINATE_2D_STRU> &areaVertexs, UINT08 setVal)
{
    if (areaVertexs.size() < 3)
    {
        return;
    }

    /* 得到多边形边的单元点 */
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(areaVertexs.back(),  areaVertexs.front(), vctCells);
    for (size_t i = 0; i < areaVertexs.size() - 1; i++)
    {
        GetCellsOfLineSeg(areaVertexs[i], areaVertexs[i + 1], vctCells);
    }

    INT32 minX = std::numeric_limits<INT32>::max(), minY = std::numeric_limits<INT32>::max(), maxX = 0, maxY = 0;
    for (const IMAGE_COORDINATE_STRU &point : vctCells) {
        if (point.x < minX) {
            minX = point.x;
        }

        if (point.y < minY) {
            minY = point.y;
        }

        if (point.x > maxX) {
            maxX = point.x;
        }

        if (point.y > maxY) {
            maxY = point.y;
        }
    }

    INT32 sizeX  = maxX - minX + 1;
    INT32 sizeY  = maxY - minY + 1;
    if (sizeX <= 0 || sizeY <= 0) {
        return;
    }

    UINT32 size   = MAX(sizeX, sizeY);
    std::vector<BOOL> space(size * size, false);
    for (const IMAGE_COORDINATE_STRU &point : vctCells) {
        UINT32 x = point.x - minX;
        UINT32 y = point.y - minY;
        space[y * size + x] = true;
    }

    // FILL THE SHAPE
    for (UINT32 i = 0; i < size; ++i) {
        // set inside to false
        BOOL inside = false;
        UINT32 hcross1 =0, hcross2 = 0;

        // determine x crossings
        for (UINT32 k = 0; k < size; ++k) {
            if (space[i * size + k] && !inside) {
                hcross1 = k;
                inside = true;
            }

            if (space[i * size + k] && inside) {
                hcross2 = k;
            }
        }

        for (UINT32 j = 0; j < size; ++j) {
            // if inside fill
            if (j > hcross1 && j < hcross2 && inside) {
                space[i * size + j] = true;
            }
        }
    }

    // FILL THE SHAPE
    for (UINT32 i = 0; i < size; ++i) {
        for (UINT32 j = 0; j < size; ++j) {
            if (space[i * size + j]) {
                IMAGE_COORDINATE_STRU stCell{INT32(j + minX), INT32(i + minY)};
                UINT08 *pucCell = GetCellPtr(stCell);
                if (pucCell != nullptr) {
                    *pucCell = setVal;
                }
            }
        }
    }

}

/**************************************************************************************
功能描述: 对线段路径设置对应值
修改记录:
**************************************************************************************/
void GRAY_MAP_C::SetPassableCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, BOOL isSetEnd)
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    UINT32 cellsNum = vctCells.size();
    R_ASSERT(cellsNum > 0, NOTHING);
    UINT32 setCellsNum = isSetEnd ? cellsNum : (cellsNum - 1);

    UINT08 costOfPassable = GetCostOfNull();
    for (UINT32 loop = 0; loop < setCellsNum; ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        UINT08 *pucCell = GetCellPtr(stCell);
        if (pucCell == nullptr) {
            continue;
        }

        if (*pucCell == costOfPassable) {
            if ((pvectSetPoint != NULL) && (*pucCell != setVal)) {
                pvectSetPoint->push_back(stCell);
            }

            *pucCell = setVal;
        }
    }
}

/**************************************************************************************
功能描述: 将连续点表示的曲线所在的栅格设置为指定值
修改记录:
**************************************************************************************/
void GRAY_MAP_C::SetPassableCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, UINT32 beginIdx, UINT32 endIdx)
{
    UINT32 LineSize = UINT32(vectLine.size());
    if (LineSize == 0) {
        return;
    }

    R_ASSERT_2(beginIdx < LineSize, NOTHING, beginIdx, LineSize);

    UINT32 endIdxEx = MIN(endIdx, LineSize - 1);
    for (UINT32 loop = beginIdx + 1; loop <= endIdxEx; ++loop) {
        SetPassableCellsOfLineSegment(vectLine[loop - 1], vectLine[loop], setVal, pvectSetPoint, false);
    }

    IMAGE_COORDINATE_STRU stEndCell;
    ConvPosToMapCoord(vectLine[endIdxEx], stEndCell);

    UINT08 *pucCell = GetCellPtr(stEndCell);
    if (pucCell != nullptr) {
        if (*pucCell == GetCostOfNull()) {
            if ((pvectSetPoint != NULL) && (*pucCell != setVal)) {
                pvectSetPoint->push_back(stEndCell);
            }

            *pucCell = setVal;
        }
    }
}

/**************************************************************************************
功能描述: 设置给定曲线左右两侧区域
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::SetLeftAndRightAreaOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, float areaDist, UINT32 splitLineSize)
{
    UINT32 lineSize = UINT32(vectLine.size());
    R_ASSERT_2(lineSize > (splitLineSize * 2), -1, lineSize, splitLineSize);

    SetCellsOfLine(vectLine, cellCostSplitLine);

    std::vector<IMAGE_COORDINATE_STRU> vectSetPoint;
    SetCellsOfLine(vectLine, cellCostRightArea_, &vectSetPoint, splitLineSize, lineSize - splitLineSize);

    INT32 rslt = SetLeftAndRightPixelsOfLine(vectSetPoint, cellCostLeftArea_, cellCostRightArea_, cellCostSplitLine, GET_CLOSEST_MULTIPLE(areaDist, GetResolution()));
    R_ASSERT(rslt == 0, -1);

    return 0;
}

void GRAY_MAP_C::SetMapNull(void)
{
    R_ASSERT(IsValid(), NOTHING);

    SetImage(cellCostNull_);
}

BOOL GRAY_MAP_C::IsCellsOfLinSegAllNull(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    for (UINT32 loop = 0; loop < vctCells.size(); ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        UINT32 cellCost = GetCellCost(stCell);

        if (cellCostNull_ != cellCost) {
            //ST_LOG_ERR("CellCost = %u. x-y(%u, %u).", cellCost, stCell.x, stCell.y);
            return false;
        }
    }

    return true;
}

BOOL GRAY_MAP_C::IsCellsOfLinSegIncludeBorder(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    for (UINT32 loop = 0; loop < vctCells.size(); ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        UINT32 cellCost = GetCellCost(stCell);

        if (cellCostBorder_ == cellCost) {
            return true;
        }
    }

    return false;
}

BOOL GRAY_MAP_C::CheckCellsOfLinSegAllNull(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const
{
    IMAGE_COORDINATE_STRU astDetourExtendCellRelPosOfDirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);
    for (UINT32 loop = 0; loop < vctCells.size(); ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        UINT32 cellCost = GetCellCost(stCell);
        if (cellCostNull_ != cellCost) {
            return false;
        }
        for (UINT32 loop = 0; loop < 4; ++loop) {
            const IMAGE_COORDINATE_STRU &stCurExtendCellRelPos = astDetourExtendCellRelPosOfDirs[loop];
            IMAGE_COORDINATE_STRU        stCurExtendCell       = {stCell.x + stCurExtendCellRelPos.x, stCell.y + stCurExtendCellRelPos.y};
            cellCost = GetCellCost(stCurExtendCell);
            if (cellCost != cellCostOver_ && cellCost != cellCostNull_) {
                return false;
            }
        }
    }

    return true;
}

BOOL GRAY_MAP_C::IsCellsOfLinSegIncludeObst(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    for (UINT32 loop = 0; loop < vctCells.size(); ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        UINT32 cellCost = GetCellCost(stCell);

        if (cellCost == cellCostObst_ || cellCost == cellCostWeakObst_) {
            return true;
        }
    }

    return false;
}

UINT08 GRAY_MAP_C::GetCellByPos(const COORDINATE_2D_STRU &stPos) const
{
    IMAGE_COORDINATE_STRU stCoord;
    ConvPosToMapCoord(stPos, stCoord);

    return GetCellCost(stCoord);
}

INT32 GRAY_MAP_C::GetOverlapArea(const AREA_C &objOtherArea, AREA_C &objOverlapArea) const
{
    AREA_C objMapArea;
    INT32 rsltOfGetMapArea = GetMapArea(objMapArea);
    R_ASSERT(rsltOfGetMapArea == 0, -1);

    objOtherArea.GetOverlapArea(objMapArea, objOverlapArea);
    return 0;
}

INT32 GRAY_MAP_C::GetOverlapArea(const GRAY_MAP_C &objOtherMap, AREA_C &objOverlapArea) const
{
    AREA_C objOtherMapArea;
    INT32 rsltOfGetOtherMapArea = objOtherMap.GetMapArea(objOtherMapArea);
    R_ASSERT(rsltOfGetOtherMapArea == 0, -1);

    INT32 rsltOfGetOverlap = GetOverlapArea(objOtherMapArea, objOverlapArea);
    R_ASSERT(rsltOfGetOverlap == 0, -1);

    return 0;
}

INT32 GRAY_MAP_C::GetCellsPosListInArea(const AREA_C &objArea, UINT08 cellCost, std::vector<COORDINATE_2D_STRU> &vectCellsPosList) const
{
    AREA_C objOverlapArea;
    INT32 rslt = GetOverlapArea(objArea, objOverlapArea);
    R_ASSERT(rslt == 0, -1);

    if (!objOverlapArea.IsValid()) {
        return 0;
    }

    IMAGE_COORDINATE_STRU stAreaLBCoord;
    ConvPosToMapCoord(objOverlapArea.stLBPoint, stAreaLBCoord);

    IMAGE_COORDINATE_STRU stAreaRTCoord;
    ConvPosToMapCoord(objOverlapArea.stRTPoint, stAreaRTCoord);

    IMAGE_COORDINATE_STRU stAreaSize;
    stAreaSize.x = stAreaRTCoord.x - stAreaLBCoord.x + 1;
    stAreaSize.y = stAreaRTCoord.y - stAreaLBCoord.y + 1;

    std::vector<IMAGE_COORDINATE_STRU> vectCellCoordList;
    rslt = GetCellsInArea(cellCost, stAreaLBCoord, stAreaSize, vectCellCoordList);
    R_ASSERT(rslt == 0, -1);

    for (UINT32 loop = 0; loop < UINT32(vectCellCoordList.size()); ++loop) {
        IMAGE_COORDINATE_STRU &stCellCoord = vectCellCoordList[loop];
        COORDINATE_2D_STRU     stCellPos;
        ConvMapCoordToPos(stCellCoord, stCellPos);

        vectCellsPosList.push_back(stCellPos);      /* 增量的，不能清除老的信息 */
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取线段上第一个障碍的点
修改记录:
**************************************************************************************/
INT32 GRAY_MAP_C::GetFirstObstPointOfLinSeg(const COORDINATE_2D_STRU &stPoint1,
                                            const COORDINATE_2D_STRU &stPoint2,
                                            COORDINATE_2D_STRU &stObstPoint) const
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);
    UINT32 cellsNum = vctCells.size();

    unsigned char obst_cost = GetCostOfObst();
    unsigned char weak_obst_cost = GetCostOfWeakObst();

    for (UINT32 loop = 0; loop < cellsNum; ++loop)
    {
        const IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        unsigned char cost = GetCellCost(stCell.x, stCell.y);

        if (cost == obst_cost || cost == weak_obst_cost)
        {
            ConvMapCoordToPos(stCell, stObstPoint);
            return 0;
        }
    }

    return -1;
}

/**************************************************************************************
功能描述: 判断指定位置是有能通过空白区域连通地图边界
          也就是判断是否存在非空障碍将指定位置包围在地图中
修改记录:
**************************************************************************************/
BOOL GRAY_MAP_C::IsConnectToMapBorderByNull(const COORDINATE_2D_STRU &stBeginPos, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint) const
{
    IMAGE_COORDINATE_STRU astDetourExtendCellRelPosOfDirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};         /* 不同延伸方向下延伸栅格相对位置 */

    IMAGE_COORDINATE_STRU stBeginCell;
    ConvPosToMapCoord(stBeginPos, stBeginCell);
    UINT32 beginCellCost = GetCellCost(stBeginCell.x, stBeginCell.y);
    if (beginCellCost == cellCostOver_) {
        return true;
    } else if (beginCellCost != cellCostNull_) {
        return false;
    } else {
    }

    /* 向右延伸，碰到非空非边界栅格，则从该点右侧贴障开始绕行，直到碰到地图边界，或者回到该点，或者绕行到障碍后的点继续向右延伸 */
    IMAGE_COORDINATE_STRU stExtendCell = stBeginCell;
    while (true) {
        /* 向右延伸，直到碰到非空栅格 */
        while (true) {
            UINT08 nextCellCost = GetCellCost(stExtendCell.x + 1, stExtendCell.y);
            if (nextCellCost == cellCostOver_) {
                return true;
            } else if (nextCellCost != cellCostNull_) {
                break;
            } else {
                ++stExtendCell.x;
                if (pvectSetPoint != NULL) {
                    pvectSetPoint->push_back(stExtendCell);
                }
            }
        }

        /* 向障碍物位于延伸方向右侧的的方向延伸绕行 */
        UINT08                firstExtendDirAtDetourBeginCell = UINT08_NULL;
        BOOL                  isPreDetourBeginCell            = false;
        IMAGE_COORDINATE_STRU stDetourBeginCell               = stExtendCell;
        UINT08                detourExtendDir                 = 2;
        while (true) {
            BOOL isExtendCellSucc = false;
            /* 延伸方向， 0右，1上，2左，3下，顺序和值不可更改 */
            for (UINT32 loop = 0; loop < 4; ++loop) {
                UINT08                       curExtendDir          = (detourExtendDir + loop + 3) & 0x03;
                const IMAGE_COORDINATE_STRU &stCurExtendCellRelPos = astDetourExtendCellRelPosOfDirs[curExtendDir];
                IMAGE_COORDINATE_STRU        stCurExtendCell       = {stExtendCell.x + stCurExtendCellRelPos.x, stExtendCell.y + stCurExtendCellRelPos.y};

                UINT08 curExtendCellCost = GetCellCost(stCurExtendCell.x, stCurExtendCell.y);
                if (curExtendCellCost == cellCostOver_) {
                    return true;

                }

                if (curExtendCellCost == cellCostNull_) {
                    stExtendCell     = stCurExtendCell;
                    detourExtendDir  = curExtendDir;
                    isExtendCellSucc = true;
                    if (pvectSetPoint != NULL) {
                        pvectSetPoint->push_back(stExtendCell);
                    }
                    break;
                }
            }

            if (!isExtendCellSucc) {
                return false;
            }

            /* 如果回到绕行起点且延伸方向和绕行初始方向一致，则表示被非空栅格包围了 */
            firstExtendDirAtDetourBeginCell = (firstExtendDirAtDetourBeginCell == UINT08_NULL) ? detourExtendDir : firstExtendDirAtDetourBeginCell;
            if (isPreDetourBeginCell &&(detourExtendDir == firstExtendDirAtDetourBeginCell)) {
                return false;
            }

            isPreDetourBeginCell = false;
            if (stExtendCell.y == stDetourBeginCell.y) {
                if (stExtendCell.x == stDetourBeginCell.x) {
                    isPreDetourBeginCell = true;
                } else if (stExtendCell.x > stDetourBeginCell.x) {
                    /* 如果延伸后绕行起点右侧，则表示绕行成功 */
                    break;
                }
            }
        }
    }
}

UINT08 GRAY_MAP_C::GetCellCost(int cellX, int cellY) const
{
    UINT08 cellCost;

    if (0 != GetCellVal(cellX, cellY, cellCost)) {
        return cellCostOver_;
    }

    return cellCost;
}

UINT08 GRAY_MAP_C::GetCellCost(const IMAGE_COORDINATE_STRU &stCellCoord) const
{
    return GetCellCost(stCellCoord.x, stCellCoord.y);
}

INT32 GRAY_MAP_C::GetRightTopPoint(COORDINATE_2D_STRU &stRightTopPoint) const
{
    IMAGE_COORDINATE_STRU stRightTopCoord;
    INT32 rsltOfGetRightTopCoord = GetRightTopCoord(stRightTopCoord);
    R_ASSERT(rsltOfGetRightTopCoord == 0, -1);

    ConvMapCoordToPos(stRightTopCoord, stRightTopPoint);
    return 0;
}

/**************************************************************************************
功能描述: 获取指定两点的线段对应的像素列表，列表增量添加
          从起点到终点有序排列，含起点和终点
修改记录:
**************************************************************************************/
void GRAY_MAP_C::GetCellsOfLineSeg(const COORDINATE_2D_STRU &stBeginPoint, const COORDINATE_2D_STRU &stEndPoint, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const
{
    IMAGE_COORDINATE_STRU stBeginCell;
    ConvPosToMapCoord(stBeginPoint, stBeginCell);

    IMAGE_COORDINATE_STRU stEndCell;
    ConvPosToMapCoord(stEndPoint, stEndCell);

    GRAY_IMAGE_C::GetCellsOfLineSeg(stBeginCell, stEndCell, true, vectCell);
}

void GRAY_MAP_C::GetCellsOfCurve(const std::vector<COORDINATE_2D_STRU> &vectCurvePoint, BOOL isNeedToClose, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const
{
    UINT32 curvePointNum = UINT32(vectCurvePoint.size());
    if (curvePointNum == 0) {
        return;
    }

    vectCell.reserve(vectCell.size() + curvePointNum * 5);        /* 预估可能的栅格数量 */

    IMAGE_COORDINATE_STRU stBeginCell;
    ConvPosToMapCoord(vectCurvePoint.front(), stBeginCell);
    vectCell.push_back(stBeginCell);
    if (curvePointNum == 1) {
        return;
    }

    IMAGE_COORDINATE_STRU stSegBeginCell = stBeginCell;
    IMAGE_COORDINATE_STRU stSegEndCell;
    for (UINT32 loop = 1; loop < curvePointNum; ++loop) {
        ConvPosToMapCoord(vectCurvePoint[loop], stSegEndCell);

        GRAY_IMAGE_C::GetCellsOfLineSeg(stSegBeginCell, stSegEndCell, false, vectCell);
        stSegBeginCell = stSegEndCell;
    }

    if (isNeedToClose && !stSegEndCell.IsEqual(stBeginCell)) {
        GRAY_IMAGE_C::GetCellsOfLineSeg(stSegEndCell, stBeginCell, false, vectCell);
    }
}

void GRAY_MAP_C::ConvPosToMapCoord(const COORDINATE_2D_STRU &stOriginPos, float resolution, const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const
{
    float reciprocalOfRes = 1.0 / resolution;

    stCoord.x = (INT32)((stPos.x - stOriginPos.x) * reciprocalOfRes + 0.5);
    stCoord.y = (INT32)((stPos.y - stOriginPos.y) * reciprocalOfRes + 0.5);
}

void GRAY_MAP_C::ConvPosToMapCoord(const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const
{
    stCoord.x = (INT32)((stPos.x - GetOriginPos().x) * GetReciprocalOfRes() + 0.5);
    stCoord.y = (INT32)((stPos.y - GetOriginPos().y) * GetReciprocalOfRes() + 0.5);
}

void GRAY_MAP_C::ConvMapCoordToPos(const IMAGE_COORDINATE_STRU &stCoord, COORDINATE_2D_STRU &stPos) const
{
    stPos.x = stCoord.x * GetResolution() + GetOriginPos().x;
    stPos.y = stCoord.y * GetResolution() + GetOriginPos().y;
}

// 栅格坐标->世界坐标
void GRAY_MAP_C::MapToWorld(int mx, int my, float &wx, float &wy) const
{
    wx = GetOriginPos().x + mx * GetResolution();
    wy = GetOriginPos().y + my * GetResolution();
}

// 世界坐标->栅格坐标
bool GRAY_MAP_C::WorldToMap(float wx, float wy, int &mx, int &my) const
{
    mx = static_cast<int>((wx - GetOriginPos().x) / GetResolution() + 0.5f);
    my = static_cast<int>((wy - GetOriginPos().y) / GetResolution() + 0.5f);

    if (mx < 0 || static_cast<UINT32>(mx) >= GetImageSizeX() || my < 0 || static_cast<UINT32>(my) >= GetImageSizeY())
    {
        return false;
    }

    return true;
}

void GRAY_MAP_C::GetXYBounds(float &x_min, float &y_min, float &x_max, float &y_max) const
{
    x_min = GetOriginPos().x;
    y_min = GetOriginPos().y;
    x_max = GetOriginPos().x + GetImageSizeX() * GetResolution();
    y_max = GetOriginPos().y + GetImageSizeY() * GetResolution();
}

COORDINATE_2D_STRU GRAY_MAP_C::AlignOriginPos(const COORDINATE_2D_STRU &stOriginPos, float resolution) const
{
    if (IsOriginAligned()) {
        float reciprocalOfRes = 1.0 / resolution;
        return COORDINATE_2D_STRU(std::floor(stOriginPos.x * reciprocalOfRes + 0.5) * resolution,
                                  std::floor(stOriginPos.y * reciprocalOfRes + 0.5) * resolution);
    } else {
        return stOriginPos;
    }
}

INT32 GRAY_MAP_C::CalcMapInfo(const COORDINATE_2D_STRU &stLBPoint, const COORDINATE_2D_STRU &stRTPoint, float resolution, COORDINATE_2D_STRU &stOriginPos, IMAGE_COORDINATE_STRU &stMapSize)
{
    R_ASSERT((stRTPoint.x > stLBPoint.x) && (stRTPoint.y > stLBPoint.y), -1);

    stOriginPos = AlignOriginPos(stLBPoint, resolution);

    IMAGE_COORDINATE_STRU stRightTopCoord;
    ConvPosToMapCoord(stOriginPos, resolution, stRTPoint, stRightTopCoord);

    stMapSize.x = stRightTopCoord.x + 1;
    stMapSize.y = stRightTopCoord.y + 1;

    return 0;
}

BOOL TRC_MAP_C::IsCellObst(int cellX, int cellY) const
{
    unsigned char cost = GetCellCost(cellX, cellY);
    return (cost == cellCostObst_ || cost == cellCostWeakObst_);
}

BOOL TRC_MAP_C::Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const
{
    /* 判断车体四边 */
    return (   IsCellsOfLinSegAllNull(stLeftFront,  stRightFront)
            && IsCellsOfLinSegAllNull(stLeftBack,   stRightBack)
            && IsCellsOfLinSegAllNull(stLeftFront,  stLeftBack)
            && IsCellsOfLinSegAllNull(stRightFront, stRightBack));
}

BOOL TRC_MAP_C::Is4SideAreaPassable(const std::vector<COORDINATE_2D_STRU> &vecMainBoby,
                                    const std::vector<COORDINATE_2D_STRU> &vecWeakBoby,
                                    bool isExpand) const
{
    COORDINATE_2D_STRU stLeftFront = vecMainBoby[0];
    COORDINATE_2D_STRU stRightFront = vecMainBoby[1];
    COORDINATE_2D_STRU stLeftBack = vecMainBoby[2];
    COORDINATE_2D_STRU stRightBack = vecMainBoby[3];
    if (!isExpand && Is4SideAreaPassable(stLeftFront, stRightFront, stLeftBack, stRightBack))
    {
        return true;
    }

    /* 判断车体四边 */
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stLeftFront, stRightFront, vctCells);
    GetCellsOfLineSeg(stLeftBack, stRightBack, vctCells);
    GetCellsOfLineSeg(stLeftFront, stLeftBack, vctCells);
    GetCellsOfLineSeg(stRightFront, stRightBack, vctCells);
    std::vector<std::pair<int, int>> collision_points;
    bool is_weak_collision = true;
    bool is_passable = true;
    for (const IMAGE_COORDINATE_STRU &point : vctCells)
    {
        UINT32 cellCost = GetCellCost(point);

        if (cellCost == cellCostObst_ || cellCost == cellCostWeakObst_)
        {
            is_passable = false;
            if (cellCost == cellCostObst_)
            {                
                is_weak_collision = false;
                break;
            }
        }
    }
    if (isExpand && !is_passable)
    {
        return false;
    }
    if (isExpand || (!is_passable && is_weak_collision))
    {
        stLeftFront = vecWeakBoby[0];
        stRightFront = vecWeakBoby[1];
        stLeftBack = vecWeakBoby[2];
        stRightBack = vecWeakBoby[3];
        vctCells.clear();
        collision_points.clear();
        is_passable = true;
        GetCellsOfLineSeg(stLeftFront, stRightFront, vctCells);
        GetCellsOfLineSeg(stLeftBack, stRightBack, vctCells);
        GetCellsOfLineSeg(stLeftFront, stLeftBack, vctCells);
        GetCellsOfLineSeg(stRightFront, stRightBack, vctCells);
        for (const IMAGE_COORDINATE_STRU &point : vctCells)
        {
            UINT32 cellCost = GetCellCost(point);
            if (cellCost == cellCostObst_ || cellCost == cellCostWeakObst_)
            {
                is_passable = false;
                break;
            }
        }
    }

    return is_passable;
}

/**************************************************************************************
功能描述: 判断矩形区域的所有栅格是否可通行
修改记录:
**************************************************************************************/
BOOL TRC_MAP_C::IsAllAreaPassable(const COORDINATE_2D_STRU &stLeftFront,
                                  const COORDINATE_2D_STRU &stRightFront,
                                  const COORDINATE_2D_STRU &stLeftBack,
                                  const COORDINATE_2D_STRU &stRightBack) const
{
    std::vector<std::pair<int, int>> collision_points;

    return IsAllAreaPassable(stLeftFront, stRightFront, stLeftBack, stRightBack, true, collision_points);
}

BOOL TRC_MAP_C::IsAllAreaPassable(const std::vector<COORDINATE_2D_STRU> &vecMainBoby,
                                  const std::vector<COORDINATE_2D_STRU> &vecWeakBoby) const
{
    COORDINATE_2D_STRU stLeftFront = vecMainBoby[0];
    COORDINATE_2D_STRU stRightFront = vecMainBoby[1];
    COORDINATE_2D_STRU stLeftBack = vecMainBoby[2];
    COORDINATE_2D_STRU stRightBack = vecMainBoby[3];

    std::vector<std::pair<int, int>> collision_points;

    bool is_passable = IsAllAreaPassable(stLeftFront,
                                         stRightFront,
                                         stLeftBack,
                                         stRightBack,
                                         false,
                                         collision_points);

    if (!is_passable)
    {
        bool is_weak_collision = true;
        for (const auto &point : collision_points)
        {
            unsigned char cost = GetCellCost(point.first, point.second);
            if (cost == cellCostObst_)
            {
                is_weak_collision = false;
                break;
            }
        }

        if (is_weak_collision)
        {
            collision_points.clear();
            stLeftFront = vecWeakBoby[0];
            stRightFront = vecWeakBoby[1];
            stLeftBack = vecWeakBoby[2];
            stRightBack = vecWeakBoby[3];
            is_passable = IsAllAreaPassable(stLeftFront,
                                            stRightFront,
                                            stLeftBack,
                                            stRightBack,
                                            true,
                                            collision_points);
        }
    }

    return is_passable;
}

BOOL TRC_MAP_C::IsAllAreaPassable(const COORDINATE_2D_STRU &stLeftFront,
                                  const COORDINATE_2D_STRU &stRightFront,
                                  const COORDINATE_2D_STRU &stLeftBack,
                                  const COORDINATE_2D_STRU &stRightBack,
                                  bool is_break_collision,
                                  std::vector<std::pair<int, int>> &collision_points) const
{
    /* 判断车体四边 */
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stLeftFront, stRightFront, vctCells);
    GetCellsOfLineSeg(stLeftBack, stRightBack, vctCells);
    GetCellsOfLineSeg(stLeftFront, stLeftBack, vctCells);
    GetCellsOfLineSeg(stRightFront, stRightBack, vctCells);

    INT32 minX = std::numeric_limits<INT32>::max(), minY = std::numeric_limits<INT32>::max(), maxX = 0, maxY = 0;
    for (const IMAGE_COORDINATE_STRU &point : vctCells)
    {
        if (point.x < minX)
        {
            minX = point.x;
        }

        if (point.y < minY)
        {
            minY = point.y;
        }

        if (point.x > maxX)
        {
            maxX = point.x;
        }

        if (point.y > maxY)
        {
            maxY = point.y;
        }
    }

    INT32 sizeX = maxX - minX + 1;
    INT32 sizeY = maxY - minY + 1;
    R_ASSERT(sizeX > 0, false);
    R_ASSERT(sizeY > 0, false);

    UINT32 size = MAX(sizeX, sizeY);
    std::vector<BOOL> space(size * size, false);
    for (const IMAGE_COORDINATE_STRU &point : vctCells)
    {
        UINT32 x = point.x - minX;
        UINT32 y = point.y - minY;
        space[y * size + x] = true;
    }

    // FILL THE SHAPE
    for (UINT32 i = 0; i < size; ++i)
    {
        // set inside to false
        BOOL inside = false;
        UINT32 hcross1 = 0, hcross2 = 0;

        // determine x crossings
        for (UINT32 k = 0; k < size; ++k)
        {
            if (space[i * size + k] && !inside)
            {
                hcross1 = k;
                inside = true;
            }

            if (space[i * size + k] && inside)
            {
                hcross2 = k;
            }
        }

        for (UINT32 j = 0; j < size; ++j)
        {
            // if inside fill
            if (j > hcross1 && j < hcross2 && inside)
            {
                space[i * size + j] = true;
            }
        }
    }

    // FILL THE SHAPE
    for (UINT32 i = 0; i < size; ++i)
    {
        for (UINT32 j = 0; j < size; ++j)
        {
            if (space[i * size + j])
            {
                IMAGE_COORDINATE_STRU stCell{INT32(j + minX), INT32(i + minY)};
                UINT32 cellCost = GetCellCost(stCell);

                if (cellCost != cellCostNull_ && cellCost != cellCostInscribe_ && cellCost != cellCostInside_)
                {
                    collision_points.push_back(std::make_pair(stCell.x, stCell.y));

                    if (is_break_collision)
                    {
                        return false;
                    }
                }
            }
        }
    }

    if (!collision_points.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

// 检测是否碰撞
BOOL TRC_MAP_C::DetectCollision(const DYNC_CAR_C &dync_car, float side_protect_len, float face_protect_len) const
{
    std::vector<std::pair<int, int>> collision_points;

    return DetectCollision(dync_car, side_protect_len, face_protect_len, true, collision_points);
}

// 检测是否碰撞
BOOL TRC_MAP_C::DetectCollision(const DYNC_CAR_C &dync_car,
                                float side_protect_len,
                                float face_protect_len,
                                std::vector<std::pair<int, int>> &collision_points) const
{
    return DetectCollision(dync_car, side_protect_len, face_protect_len, false, collision_points);
}

// 检测是否碰撞
BOOL TRC_MAP_C::DetectCollision(const DYNC_CAR_C &dync_car,
                                float side_protect_len,
                                float face_protect_len,
                                bool is_break_collision,
                                std::vector<std::pair<int, int>> &collision_points) const
{
    COORDINATE_2D_STRU left_head, right_head, left_tail, right_tail;

    if (dync_car.IsExpand())
    {
        std::vector<std::pair<int, int>> brush_points;
        dync_car.Get4CornerBrushPos(left_head, right_head, left_tail, right_tail, side_protect_len, face_protect_len);
        bool is_brush_collision = !IsAllAreaPassable(left_head,
                                                     right_head,
                                                     left_tail,
                                                     right_tail,
                                                     is_break_collision,
                                                     brush_points);
        collision_points.insert(collision_points.end(),
                                std::make_move_iterator(brush_points.begin()),
                                std::make_move_iterator(brush_points.end()));

        if (is_brush_collision && is_break_collision)
        {
            return true;
        }

        std::vector<std::pair<int, int>> body_points;
        dync_car.Get4CornerBodyPos(left_head, right_head, left_tail, right_tail, side_protect_len, face_protect_len);
        bool is_body_collision = !IsAllAreaPassable(left_head,
                                                    right_head,
                                                    left_tail,
                                                    right_tail,
                                                    is_break_collision,
                                                    body_points);
        collision_points.insert(collision_points.end(),
                                std::make_move_iterator(body_points.begin()),
                                std::make_move_iterator(body_points.end()));

        return (is_brush_collision || is_body_collision);
    }
    else
    {
        dync_car.Get4CornerPosByMap(left_head, right_head, left_tail, right_tail, side_protect_len, face_protect_len);
        bool is_collision = !IsAllAreaPassable(left_head,
                                               right_head,
                                               left_tail,
                                               right_tail,
                                               is_break_collision,
                                               collision_points);

        if (!is_break_collision && is_collision)
        {
            bool is_weak_collision = true;

            for (const auto &point : collision_points)
            {
                unsigned char cost = GetCellCost(point.first, point.second);

                if (cost != cellCostWeakObst_)
                {
                    is_weak_collision = false;
                    break;
                }
            }

            if (is_weak_collision)
            {
                collision_points.clear();

                dync_car.Get4CornerPosByMap(left_head, right_head, left_tail, right_tail, side_protect_len - 0.12f, face_protect_len);
                is_collision = !IsAllAreaPassable(left_head,
                                                  right_head,
                                                  left_tail,
                                                  right_tail,
                                                  is_break_collision,
                                                  collision_points);
            }
        }

        return is_collision;
    }
}

BOOL TRC_MAP_C::IsPointOutOfPassableArea(const COORDINATE_2D_STRU &stPoint) const
{
    UINT08 cellCost = GetCellByPos(stPoint);
    return ((cellCost == GetCostOfUnknown()) || (cellCost == GetCostOfOver()));
}

void TRC_MAP_C::SetCarBodyArea(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack)
{
    /* 只需要将四边设置，车体移动时自然后将所有轨迹设置 */
    SetCellsOfLineSegment(stLeftFront,  stRightFront, cellCostCarBodyTrc_, nullptr, true);
    SetCellsOfLineSegment(stLeftBack,   stRightBack,  cellCostCarBodyTrc_, nullptr, true);
    SetCellsOfLineSegment(stLeftFront,  stLeftBack,   cellCostCarBodyTrc_, nullptr, true);
    SetCellsOfLineSegment(stRightFront, stRightBack,  cellCostCarBodyTrc_, nullptr, true);
}

/**************************************************************************************
功能描述: 创建区域地图
          调用方保证传入区域为合法多边形
修改记录:
**************************************************************************************/
INT32 AREA_MAP_C::CreateAreaMap(const std::vector<COORDINATE_2D_STRU> &vectAreaBorderPoint, float expandAreaDist, float resolution)
{
    INT32 rsltOfCreateMapByBorder = CreateMapByBorder(vectAreaBorderPoint, expandAreaDist, resolution);
    R_ASSERT_1(rsltOfCreateMapByBorder == 0, -1, vectAreaBorderPoint.size());

    INT32 rsltOfAddArea = AddAreaWithBorder(vectAreaBorderPoint, expandAreaDist);
    R_ASSERT_1(rsltOfAddArea == 0, -1, vectAreaBorderPoint.size());

    return 0;
}

/**************************************************************************************
功能描述: 是否在区域内
修改记录:
**************************************************************************************/
BOOL AREA_MAP_C::IsInArea(const COORDINATE_2D_STRU &stPoint) const
{
    IMAGE_COORDINATE_STRU stCell;
    ConvPosToMapCoord(stPoint, stCell);

    UINT08 valOfCell;
    return ((0 == GetCellVal(stCell, valOfCell)) && (valOfCell == GetCostOfInside()));
}

/**************************************************************************************
功能描述: 创建空白地图
修改记录:
**************************************************************************************/
INT32 AREA_MAP_C::CreateMapByBorder(const std::vector<COORDINATE_2D_STRU> &vectBorderPoint, float expandAreaDist, float resolution)
{
    /* 计算地图范围，地图范围包括扩展区域范围，还外留一个边 */
    AREA_C objArea;
    objArea.IncludePoints(vectBorderPoint);
    R_ASSERT_1(objArea.IsValid(), -1, vectBorderPoint.size());

    objArea.Expand(expandAreaDist + 5.0 * resolution);

    /* 创建地图，默认背景为空白 */
    INT32 rsltOfCreateMap = GRAY_MAP_C::CreateMap(objArea.stLBPoint, objArea.stRTPoint, true, resolution);
    R_ASSERT(rsltOfCreateMap == 0, -1);

    return 0;
}

BOOL AREA_MAP_C::IsCellNearClassifiedCells(const IMAGE_COORDINATE_STRU &stCell, BOOL &isCellNearInsideCells) const
{
    auto ProcNearCellFunc = [&](const IMAGE_COORDINATE_STRU &procCell, UINT08 &sideCell1Cost, const IMAGE_COORDINATE_STRU &sideCell1, UINT08 &sideCell2Cost, const IMAGE_COORDINATE_STRU &sideCell2)
    {
        UINT08 procCellCost = GetCellCost(procCell);
        if        (procCellCost == GetCostOfInside()) {
            isCellNearInsideCells = true;
            return true;
        } else if (procCellCost == GetCostOfOutside()) {
            isCellNearInsideCells = false;
            return true;
        } else if (procCellCost == GetCostOfNull()) {
            sideCell1Cost = (sideCell1Cost == UINT08_NULL) ? GetCellCost(sideCell1) : sideCell1Cost;
            sideCell2Cost = (sideCell2Cost == UINT08_NULL) ? GetCellCost(sideCell2) : sideCell2Cost;
            if        ((sideCell1Cost == GetCostOfInside()) || (sideCell2Cost == GetCostOfInside())) {
                isCellNearInsideCells = true;
                return true;
            } else if ((sideCell1Cost == GetCostOfOutside()) || (sideCell2Cost == GetCostOfOutside())) {
                isCellNearInsideCells = false;
                return true;
            } else {
            }
        }

        return false;
    };

    /* 提高效率硬编码 */
    UINT08 costOfUpLeftCell               = UINT08_NULL;
    UINT08 costOfUpRightCell              = UINT08_NULL;
    UINT08 costOfDownLeftCell             = UINT08_NULL;
    UINT08 costOfDownRightCell            = UINT08_NULL;

    IMAGE_COORDINATE_STRU stUpCell        = {stCell.x,     stCell.y + 1};
    IMAGE_COORDINATE_STRU stDownCell      = {stCell.x,     stCell.y - 1};
    IMAGE_COORDINATE_STRU stLeftCell      = {stCell.x - 1, stCell.y};
    IMAGE_COORDINATE_STRU stRightCell     = {stCell.x + 1, stCell.y};
    IMAGE_COORDINATE_STRU stUpLeftCell    = {stCell.x - 1, stCell.y + 1};
    IMAGE_COORDINATE_STRU stUpRightCell   = {stCell.x + 1, stCell.y + 1};
    IMAGE_COORDINATE_STRU stDownLeftCell  = {stCell.x - 1, stCell.y - 1};
    IMAGE_COORDINATE_STRU stDownRightCell = {stCell.x + 1, stCell.y - 1};

    if (ProcNearCellFunc(stUpCell,    costOfUpLeftCell,   stUpLeftCell,   costOfUpRightCell,   stUpRightCell))   { return true; }
    if (ProcNearCellFunc(stDownCell,  costOfDownLeftCell, stDownLeftCell, costOfDownRightCell, stDownRightCell)) { return true; }
    if (ProcNearCellFunc(stLeftCell,  costOfUpLeftCell,   stUpLeftCell,   costOfDownLeftCell,  stDownLeftCell))  { return true; }
    if (ProcNearCellFunc(stRightCell, costOfUpRightCell,  stUpRightCell,  costOfDownRightCell, stDownRightCell)) { return true; }

    return false;
}

/**************************************************************************************
功能描述: 判断指定栅格是否在区域内
修改记录:
**************************************************************************************/
BOOL AREA_MAP_C::IsCellInAreaByBorder(const std::vector<IMAGE_COORDINATE_STRU> &vectBorderCell, const IMAGE_COORDINATE_STRU &stCell) const
{
    INT32 passedQuadrant = 0;

    INT32 lastQuadrant   = 0;
    for (const IMAGE_COORDINATE_STRU &stBorderCell : vectBorderCell) {
        /* 计算当前边界点相对给定点的象限 */
        INT32 quadrant;
        if        ((stBorderCell.x >  stCell.x) && (stBorderCell.y >= stCell.y)) {
            quadrant = 1;
        } else if ((stBorderCell.x <= stCell.x) && (stBorderCell.y >  stCell.y)) {
            quadrant = 2;
        } else if ((stBorderCell.x <  stCell.x) && (stBorderCell.y <= stCell.y)) {
            quadrant = 3;
        } else {
            quadrant = 4;
        }

        /* 累计象限变化值 */
        if (lastQuadrant == 0) {
            lastQuadrant = quadrant;
            continue;
        }

        if (quadrant == lastQuadrant) {
            continue;
        }

        INT32 quadrantDiff = quadrant - lastQuadrant;
        if        ((quadrantDiff == 1) || (quadrantDiff == -3)) {
            ++passedQuadrant;
        } else if ((quadrantDiff == 3) || (quadrantDiff == -1)) {
            --passedQuadrant;
        } else {
            ST_LOG_FATAL("Quadrant Diff(%d) Err. BorderCell(x=%d, y=%d), stCell(x=%d, y=%d).", quadrantDiff, stBorderCell.x, stBorderCell.y, stCell.x, stCell.y);
            return false;
        }

        lastQuadrant = quadrant;
    }

    if (passedQuadrant == 0) {
        return false;
    } else if (std::abs(passedQuadrant) == 4) {     /* 如果单圈闭环只能为4，如果有交叉的多圈可能为它倍数，但这是非法多边形。 */
        return true;
    } else {
        ST_LOG_FATAL("Passed Quadrant Num(%d) Err.", passedQuadrant);
        return false;
    }
}

BOOL AREA_MAP_C::IsCellInArea(const std::vector<IMAGE_COORDINATE_STRU> &vectBorderCell, const IMAGE_COORDINATE_STRU &stCell) const
{
    /* 为提高效率，优先通过判断是否连通其它已经确定归属的栅格来进行判断 */
    BOOL isCellNearInsideCells;
    if (IsCellNearClassifiedCells(stCell, isCellNearInsideCells)) {
        return isCellNearInsideCells;
    }

    return IsCellInAreaByBorder(vectBorderCell, stCell);
}

INT32 AREA_MAP_C::GetCellsNearBorder(const std::vector<IMAGE_COORDINATE_STRU> &vectBorderCell, std::vector<IMAGE_COORDINATE_STRU> &vectCellInAera, std::vector<IMAGE_COORDINATE_STRU> &vectCellOutAera)
{
    UINT32 borderCellNum = UINT32(vectBorderCell.size());
    vectCellInAera.reserve(borderCellNum * 4);
    vectCellOutAera.reserve(borderCellNum * 4);

    const IMAGE_COORDINATE_STRU *pstLastBorderCell = nullptr;
    for (const IMAGE_COORDINATE_STRU &stBorderCell : vectBorderCell) {
        IMAGE_COORDINATE_STRU astNearCell[4];
        UINT32                nearCellNum;

        if (pstLastBorderCell != nullptr) {
            /* 必须相邻 */
            INT32 dist = std::abs(stBorderCell.x - pstLastBorderCell->x) + std::abs(stBorderCell.y - pstLastBorderCell->y);
            R_ASSERT_4(dist <= 2, -1, stBorderCell.x, stBorderCell.y, pstLastBorderCell->x, pstLastBorderCell->y);

            /* 构造邻接栅格列表 */
            /* 提高效率，此处硬编码 *//* 按照此顺序，减小调用根据所有边界才能来判断是否在封闭区内的概率                                          */
            if (stBorderCell.y > pstLastBorderCell->y) {
                if (stBorderCell.x > pstLastBorderCell->x) {
                    /* 向右上方向延伸时，按照下左右上顺序 */
                    astNearCell[0] = {stBorderCell.x,     stBorderCell.y - 1};
                    astNearCell[1] = {stBorderCell.x - 1, stBorderCell.y};
                    astNearCell[2] = {stBorderCell.x + 1, stBorderCell.y};
                    astNearCell[3] = {stBorderCell.x,     stBorderCell.y + 1};
                    nearCellNum = 4;
                } else if (stBorderCell.x == pstLastBorderCell->x) {
                    /* 向上延伸时，按照左右上顺序 */
                    astNearCell[0] = {stBorderCell.x - 1, stBorderCell.y};
                    astNearCell[1] = {stBorderCell.x + 1, stBorderCell.y};
                    astNearCell[2] = {stBorderCell.x,     stBorderCell.y + 1};
                    nearCellNum = 3;
                } else {
                    /* 向左上方向延伸时，按照下右左上顺序 */
                    astNearCell[0] = {stBorderCell.x,     stBorderCell.y - 1};
                    astNearCell[1] = {stBorderCell.x + 1, stBorderCell.y};
                    astNearCell[2] = {stBorderCell.x - 1, stBorderCell.y};
                    astNearCell[3] = {stBorderCell.x,     stBorderCell.y + 1};
                    nearCellNum = 4;
                }
            } else if (stBorderCell.y == pstLastBorderCell->y) {
                if (stBorderCell.x > pstLastBorderCell->x) {
                    /* 向右方向延伸时，按照下上右顺序 */
                    astNearCell[0] = {stBorderCell.x,     stBorderCell.y - 1};
                    astNearCell[1] = {stBorderCell.x,     stBorderCell.y + 1};
                    astNearCell[2] = {stBorderCell.x + 1, stBorderCell.y};
                    nearCellNum = 3;
                } else {
                    /* 向左方向延伸时，按照下上左顺序 */
                    astNearCell[0] = {stBorderCell.x,     stBorderCell.y - 1};
                    astNearCell[1] = {stBorderCell.x,     stBorderCell.y + 1};
                    astNearCell[2] = {stBorderCell.x - 1, stBorderCell.y};
                    nearCellNum = 3;
                }
            } else {
                if (stBorderCell.x > pstLastBorderCell->x) {
                    /* 向右下方向延伸时，按照上左右下顺序 */
                    astNearCell[0] = {stBorderCell.x,     stBorderCell.y + 1};
                    astNearCell[1] = {stBorderCell.x - 1, stBorderCell.y};
                    astNearCell[2] = {stBorderCell.x + 1, stBorderCell.y};
                    astNearCell[3] = {stBorderCell.x,     stBorderCell.y - 1};
                    nearCellNum = 4;
                } else if (stBorderCell.x == pstLastBorderCell->x) {
                    /* 向下延伸时，按照左右下顺序 */
                    astNearCell[0] = {stBorderCell.x - 1, stBorderCell.y};
                    astNearCell[1] = {stBorderCell.x + 1, stBorderCell.y};
                    astNearCell[2] = {stBorderCell.x,     stBorderCell.y - 1};
                    nearCellNum = 3;
                } else {
                    /* 向左下方向延伸时，按照上右左下顺序 */
                    astNearCell[0] = {stBorderCell.x,     stBorderCell.y + 1};
                    astNearCell[1] = {stBorderCell.x + 1, stBorderCell.y};
                    astNearCell[2] = {stBorderCell.x - 1, stBorderCell.y};
                    astNearCell[3] = {stBorderCell.x,     stBorderCell.y - 1};
                    nearCellNum = 4;
                }
            }
        }else {
            astNearCell[0] = {stBorderCell.x,     stBorderCell.y + 1};
            astNearCell[1] = {stBorderCell.x,     stBorderCell.y - 1};
            astNearCell[2] = {stBorderCell.x - 1, stBorderCell.y};
            astNearCell[3] = {stBorderCell.x + 1, stBorderCell.y};
            nearCellNum = 4;
        }

        pstLastBorderCell = &stBorderCell;

        /* 顺序判断各个邻接栅格是否为内部栅格或外部栅格 */
        for (UINT32 loop = 0; loop < nearCellNum; ++loop) {
            const IMAGE_COORDINATE_STRU &stNearCell = astNearCell[loop];
            UINT08 *pucNearCell = GetCellPtr(stNearCell);
            if (pucNearCell == nullptr) {
                continue;
            }

            UINT08 nearCellCost = *pucNearCell;
            if (nearCellCost == GetCostOfObst() ||
                nearCellCost == GetCostOfWeakObst() ||
                nearCellCost == GetCostOfInside() ||
                nearCellCost == GetCostOfOutside())
            {
                continue;
            }

            if (IsCellInArea(vectBorderCell, stNearCell)) {
                *pucNearCell = GetCostOfInside();
                vectCellInAera.push_back(stNearCell);
            } else {
                *pucNearCell = GetCostOfOutside();
                vectCellOutAera.push_back(stNearCell);
            }
        }
    }

    SetCellsUnsafe(vectCellInAera, GetCostOfNull());
    SetCellsUnsafe(vectCellOutAera, GetCostOfNull());
    return 0;
}

INT32 AREA_MAP_C::SetCellsInArea(const std::vector<IMAGE_COORDINATE_STRU> &vectCellNearBorderInAera)
{
    /* 要求边界栅格和邻接栅格可以无序，但是必须严格封闭 */
    /* 边界栅格必须已经设置 */
    /* 所有边界点向右填充，一定能填充满 */
    INT32 rsltOfExpand = ExpandCellsRight(vectCellNearBorderInAera, GetCostOfInside(), GetCostOfNull());
    R_ASSERT_1(rsltOfExpand == 0, -1, vectCellNearBorderInAera.size());

    return 0;
}

INT32 AREA_MAP_C::SetCellsExpand(const std::vector<IMAGE_COORDINATE_STRU> &vectCellNearBorderOutAera, float expandAreaDist)
{
    UINT32 expandCellDist = GetDistByCell(expandAreaDist);

    INT32 rsltOfExpand = ExpandCells(vectCellNearBorderOutAera, GetCostOfInside(), expandCellDist, GetCostOfNull());
    R_ASSERT(rsltOfExpand == 0, -1);

    return 0;
}

INT32 AREA_MAP_C::AddAreaWithBorder(const std::vector<COORDINATE_2D_STRU> &vectBorderPoint, float expandAreaDist)
{
    R_ASSERT(IsValid(), -1);

    /* 获取并设置边界栅格，可能重复，必须连续，且封闭。 */
    double time1 = Comm_GetTimeSec();

    std::vector<IMAGE_COORDINATE_STRU> vectBorderCell;
    GetCellsOfCurve(vectBorderPoint, true, vectBorderCell);
    R_ASSERT_2(!vectBorderCell.empty(), -1, vectBorderPoint.size(), vectBorderCell.size());
    const IMAGE_COORDINATE_STRU &stBeginCell = vectBorderCell.front();
    R_ASSERT_2(stBeginCell.IsEqual(vectBorderCell.back()), -1, vectBorderPoint.size(), vectBorderCell.size());

    INT32 rsltOfSetBorderCells = SetCellsStrictly(vectBorderCell, GetCostOfObst());
    R_ASSERT_1(rsltOfSetBorderCells == 0, -1, vectBorderCell.size());

    /* 获取边界附近的栅格 */
    double time2 = Comm_GetTimeSec();

    std::vector<IMAGE_COORDINATE_STRU> vectCellNearBorderInAera;
    std::vector<IMAGE_COORDINATE_STRU> vectCellNearBorderOutAera;
    INT32 rsltOfGetCellsNearBorder = GetCellsNearBorder(vectBorderCell, vectCellNearBorderInAera, vectCellNearBorderOutAera);
    R_ASSERT(rsltOfGetCellsNearBorder == 0, -1);

    /* 填充区域内的栅格 */
    double time3 = Comm_GetTimeSec();

    INT32 rsltOfSetCellsInArea = SetCellsInArea(vectCellNearBorderInAera);
    R_ASSERT(rsltOfSetCellsInArea == 0, -1);

    /* 填充区域扩展栅格和边界线 */
    double time4 = Comm_GetTimeSec();

    INT32 rsltOfSetExpand = SetCellsExpand(vectCellNearBorderOutAera, expandAreaDist);
    R_ASSERT(rsltOfSetExpand == 0, -1);

    SetCellsUnsafe(vectBorderCell, GetCostOfInside());

    double time5 = Comm_GetTimeSec();
    ST_LOG_INFO("Time Statis: Total(%.2fms), SetBorder(%.2fms), GetNearCell(%.2fms), SetInside(%.2fms), SetExpand(%.2fms).", (time5 - time1) * 1000.0, (time2 - time1) * 1000.0, (time3 - time2) * 1000, (time4 - time3) * 1000.0, (time5 - time4) * 1000.0);
    return 0;
}

BOOL COVER_MAP_C::IsMarginPoint(UINT32 sizeX, UINT32 sizeY, const IMAGE_COORDINATE_STRU &stPoint) const
{
    if (stPoint.x == 0) {
        return true;
    }

    if (stPoint.x + 1 == (INT32)sizeX) {
        return true;
    }

    if (stPoint.y == 0) {
        return true;
    }

    if (stPoint.y + 1 == (INT32)sizeY) {
        return true;
    }

    return false;
}

INT32 COVER_MAP_C::MarkGridsExpand(const COORDINATE_2D_STRU &stCarPos, UINT08 value, std::vector<IMAGE_COORDINATE_STRU> *pVectMarginPoint)
{
    UINT32 sizeX;
    UINT32 sizeY;
    INT32 rslt = GetImageSize(sizeX, sizeY);
    R_ASSERT(rslt == 0, -1);

    IMAGE_COORDINATE_STRU seedPoint;
    ConvPosToMapCoord(stCarPos, seedPoint);

    std::queue<IMAGE_COORDINATE_STRU> q;
    q.push(seedPoint);
    UINT08 costOfUnknown = GetCostOfUnknown();
    UINT08 costOfNull    = GetCostOfNull();
    UINT08 CostOfObst    = GetCostOfObst();
    while (!q.empty()) {
        IMAGE_COORDINATE_STRU curPoint = q.front();
        q.pop();
        BOOL obsAside = false;
        for (UINT32 i = 0; i < dirLimited_; i++) {
            IMAGE_COORDINATE_STRU pointSucc;
            INT32 rslt = CreateSuccessor(curPoint, i, pointSucc);
            if (rslt != 0) {
                continue;
            }
            UINT08 cellVal;
            rslt = GetCellVal(pointSucc.x, pointSucc.y, cellVal);
            R_ASSERT(rslt == 0, -1);
            if (cellVal == CostOfObst)
            {
                obsAside = true;
                break;
            }
        }
        if (obsAside) {
            continue;
        }
        for (UINT32 i = 0; i < dirLimited_; i++) {
            IMAGE_COORDINATE_STRU pointSucc;
            INT32 rslt = CreateSuccessor(curPoint, i, pointSucc);
            if (rslt != 0) {
                continue;
            }

            UINT08 cellVal;
            rslt = GetCellVal(pointSucc.x, pointSucc.y, cellVal);
            R_ASSERT(rslt == 0, -1);

            if (cellVal != costOfUnknown && cellVal != costOfNull) {
                continue;
            }

            if (cellVal == value) {
                continue;
            }

            if (pVectMarginPoint != nullptr && IsMarginPoint(sizeX, sizeY, pointSucc)) {
                pVectMarginPoint->push_back(pointSucc);
            }

            SetCell(pointSucc.x, pointSucc.y, value);
            q.push(pointSucc);
        }
    }

    return 0;
}

INT32 COVER_MAP_C::MarkGridsInArea(const std::vector<COORDINATE_2D_STRU> &vertexList)
{
    R_ASSERT(vertexList.size() >= 3, -1);
    for (UINT32 idx = 0; idx < vertexList.size(); idx++) {
        // 连接端点，保证地图完整性
        std::vector<COORDINATE_2D_STRU> vectEndLine;
        vectEndLine.push_back(vertexList[idx]);

        if (idx == (vertexList.size() - 1)) {
            vectEndLine.push_back(vertexList[0]);

        } else {
            vectEndLine.push_back(vertexList[idx + 1]);
        }
        SetCellsOfLine(vectEndLine, GetCostOfObst());
    }

    std::vector<IMAGE_COORDINATE_STRU> vertexCoordList;
    for (const COORDINATE_2D_STRU &vertex : vertexList) {
        IMAGE_COORDINATE_STRU stCoord;
        ConvPosToMapCoord(vertex, stCoord);
        vertexCoordList.push_back(stCoord);
    }

    IMAGE_COORDINATE_STRU seedPoint;
    INT32 rslt = SearchSeedPoint(vertexCoordList, seedPoint);
    if (rslt != 0) {
        ST_LOG_ERR("SearchSeedPoint Fail.");
        return -1;
    }

    COORDINATE_2D_STRU mapPos;
    ConvMapCoordToPos(seedPoint, mapPos);

    return MarkGridsExpand(mapPos, GetCostOfNull());
}

INT32 COVER_MAP_C::SearchSeedPoint(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, IMAGE_COORDINATE_STRU &seedPoint) const
{
    UINT32 sizeX;
    UINT32 sizeY;
    INT32 rslt = GetImageSize(sizeX, sizeY);
    R_ASSERT(rslt == 0, -1);

    for (UINT32 x = 0; x < sizeX; x++) {
        for (UINT32 y = 0; y < sizeY; y++) {
            /* 不取多边形上的点 */
            IMAGE_COORDINATE_STRU point{(INT32)x, (INT32)y};
            COORDINATE_2D_STRU stMapPos;
            rslt = TransPictureCoordToPos(point, stMapPos);
            R_ASSERT(rslt == 0, -1);

            unsigned char cost = GetCellByPos(stMapPos);
            if (cost == GetCostOfObst() || cost == GetCostOfWeakObst())
            {
                continue;
            }

            BOOL obsAside = false;
            for(UINT32 i = 0; i < dirAll_; i++) {
                IMAGE_COORDINATE_STRU pointSucc;
                rslt = CreateSuccessor(point, i, pointSucc);
                if (rslt != 0) {
                    continue;
                }

                UINT08 cellVal;
                rslt = GetCellVal(pointSucc.x, pointSucc.y, cellVal);
                R_ASSERT(rslt == 0, -1);
                if (cellVal == GetCostOfObst() || cellVal == GetCostOfWeakObst())
                {
                    obsAside = true;
                    break;
                }
            }

            if (obsAside) {
                continue;
            }

            if (IsPointInPolygon(vertexCoordList, point)) {
                ST_LOG_INFO("Seed Point (x %f, y %f).", stMapPos.x, stMapPos.y);
                seedPoint = point;
                return 0;
            }
        }
    }

    return -1;
}

BOOL COVER_MAP_C::IsPointInPolygon(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, const IMAGE_COORDINATE_STRU &seedPoint) const
{
    float sumAngle = 0.00;
    for (UINT32 idx = 0; idx < vertexCoordList.size(); idx++) {
        IMAGE_COORDINATE_STRU vertexCoord1 = vertexCoordList[idx];
        IMAGE_COORDINATE_STRU vertexCoord2;
        if (idx == vertexCoordList.size() - 1) {
            vertexCoord2 = vertexCoordList[0];

        } else {
            vertexCoord2 = vertexCoordList[idx + 1];
        }
        float angle1 = atan2(vertexCoord1.y - seedPoint.y, vertexCoord1.x - seedPoint.x);
        float angle2 = atan2(vertexCoord2.y - seedPoint.y, vertexCoord2.x - seedPoint.x);
        sumAngle += AdjustAngle(angle2 - angle1);
    }

    if (fabs(sumAngle) < 0.01) {
        return false;
    } else {
        ST_LOG_INFO("sumAngle %f.", sumAngle);
        return true;
    }
}

INT32 COVER_MAP_C::GetPoints(UINT08 value, std::vector<COORDINATE_2D_STRU> &vectPoint) const
{
    UINT32 curSizeX;
    UINT32 curSizeY;
    INT32 rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);

    for (UINT32 x = 0; x < curSizeX; x++) {
        for (UINT32 y = 0; y < curSizeY; y++) {
            UINT08 curValue;
            rslt = GetCellVal(x, y, curValue);
            R_ASSERT(rslt == 0, -1);

            if (curValue == value) {
                IMAGE_COORDINATE_STRU stCoord{(INT32)x, (INT32)y};
                COORDINATE_2D_STRU stMapPos;
                ConvMapCoordToPos(stCoord, stMapPos);

                vectPoint.push_back(stMapPos);
            }
        }
    }

    return 0;
}

//###################################################
//                                   CREATE SUCCESSOR
//###################################################
INT32 COVER_MAP_C::CreateSuccessor(const IMAGE_COORDINATE_STRU &stCoordCur, UINT32 i, IMAGE_COORDINATE_STRU &stCoordSucc) const
{
    R_ASSERT(i < dirAll_, -1);
    stCoordSucc.x = stCoordCur.x + dx_[i];
    stCoordSucc.y = stCoordCur.y + dy_[i];

    if (IsCoordValid(stCoordSucc)) {
        return 0;
    } else {
        return -1;
    }
}

INT32 COVER_MAP_C::CopyPassableArea(COVER_MAP_C &objMap) const
{
    UINT32 curSizeX;
    UINT32 curSizeY;
    INT32 rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);

    UINT32 targetSizeX;
    UINT32 targetSizeY;
    rslt = objMap.GetImageSize(targetSizeX, targetSizeY);
    R_ASSERT(rslt == 0, -1);

    R_ASSERT(curSizeX == targetSizeX, -1);
    R_ASSERT(curSizeY == targetSizeY, -1);

    for (UINT32 x = 0; x < curSizeX; x++) {
        for (UINT32 y = 0; y < curSizeY; y++) {
            UINT08 curValue;
            UINT08 targetValue;
            rslt = GetCellVal(x, y, curValue);
            R_ASSERT(rslt == 0, -1);

            rslt = objMap.GetCellVal(x, y, targetValue);
            R_ASSERT(rslt == 0, -1);

            if (curValue == GetCostOfNull() && targetValue != GetCostOfObst() && targetValue != GetCostOfWeakObst())
            {
                objMap.SetCell(x, y, curValue);
            }
        }
    }

    return 0;
}

INT32 COVER_MAP_C::MarkPassableArea(UINT08 setVal)
{
    UINT32 curSizeX;
    UINT32 curSizeY;
    INT32 rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);
    for (UINT32 x = 0; x < curSizeX; x++) {
        for (UINT32 y = 0; y < curSizeY; y++) {
            UINT08 curValue;
            rslt = GetCellVal(x, y, curValue);
            R_ASSERT(rslt == 0, -1);

            if (curValue == GetCostOfNull())
            {
                SetCell(x, y, setVal);
            }
        }
    }

    return 0;
}

INT32 COVER_MAP_C::MarkMargin()
{
    UINT32 curSizeX;
    UINT32 curSizeY;
    INT32 rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);

    for (UINT32 y = 0; y < curSizeY; y++) {
        SetCell(0, y, GetCostOfObst());
    }

    for (UINT32 y = 0; y < curSizeY; y++) {
        SetCell(curSizeX - 1, y, GetCostOfObst());
    }

    for (UINT32 x = 0; x < curSizeX; x++) {
        SetCell(x, 0, GetCostOfObst());
    }

    for (UINT32 x = 0; x < curSizeX; x++) {
        SetCell(x, curSizeY - 1, GetCostOfObst());
    }

    return 0;
}

BOOL COVER_MAP_C::IsRouteInPassableArea(vector<COORDINATE_2D_STRU> &vecRoute, UINT32 &firstIdxInArea) const
{
    for (UINT32 i = 0; i< vecRoute.size(); i++) {
        const COORDINATE_2D_STRU &stPoint = vecRoute[i];
        if (GetCellByPos(stPoint) == GetCostOfNull()) {
            firstIdxInArea = i;
            return true;
        }
    }

    return false;
}

BOOL COVER_MAP_C::Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const
{
    /* 判断车体四边 */
    return (   IsCellsOfLinSegAllNull(stLeftFront,  stRightFront)
            || IsCellsOfLinSegAllNull(stLeftBack,   stRightBack)
            || IsCellsOfLinSegAllNull(stLeftFront,  stLeftBack)
            || IsCellsOfLinSegAllNull(stRightFront, stRightBack));
}

