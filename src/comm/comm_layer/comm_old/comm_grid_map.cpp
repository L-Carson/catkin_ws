

#include "comm_grid_map.h"
#include "comm_dbg_log.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <queue>


/**************************************************************************************
功能描述: 创建图像
修改记录:
**************************************************************************************/
void GRAY_IMAGE_C::CreateImage(unsigned int sizeX, unsigned int sizeY)
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
void GRAY_IMAGE_C::CreateImage(unsigned int sizeX, unsigned int sizeY, unsigned char defaultVal)
{
    CreateImage(sizeX, sizeY);

    memset(pucMap_, defaultVal, mapSizeX_ * mapSizeY_);
}

/**************************************************************************************
功能描述: 从现有图像中拷贝一部分到本图像
修改记录:
**************************************************************************************/
int GRAY_IMAGE_C::CopyImage(const GRAY_IMAGE_C &objSrcImage, const IMAGE_COORDINATE_STRU &stCopyStart, const IMAGE_COORDINATE_STRU &stCopySize)
{
    R_ASSERT_4(objSrcImage.IsImageValid(),               -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);
    R_ASSERT_4(objSrcImage.IsCoordValid(stCopyStart),    -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);
    R_ASSERT_4((stCopySize.x > 0) && (stCopySize.y > 0), -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);

    IMAGE_COORDINATE_STRU stCopyMax;
    stCopyMax.x = stCopyStart.x + stCopySize.x - 1;
    stCopyMax.y = stCopyStart.y + stCopySize.y - 1;
    R_ASSERT_4(objSrcImage.IsCoordValid(stCopyMax),      -1, stCopyStart.x, stCopyStart.y, stCopySize.x, stCopySize.y);

    CreateImage((unsigned int)(stCopySize.x), (unsigned int)(stCopySize.y));

    for (int loop = 0; loop < stCopySize.y; loop++) {
        memcpy(GetCellPtrUnsafe(0, loop), objSrcImage.GetCellPtrUnsafe(stCopyStart.x, stCopyStart.y + loop), stCopySize.x);
    }

    mapSizeX_ = stCopySize.x;
    mapSizeY_ = stCopySize.y;
    return 0;
}

int GRAY_IMAGE_C::CopyImage(const GRAY_IMAGE_C &objSrcImage)
{
    R_ASSERT(objSrcImage.IsImageValid(), -1);

    CreateImage(objSrcImage.mapSizeX_, objSrcImage.mapSizeY_);

    memcpy(pucMap_, objSrcImage.pucMap_, mapSizeX_ * mapSizeY_);
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
    std::swap(this->mapSizeX_, objOtherImage.mapSizeX_);
    std::swap(this->mapSizeY_, objOtherImage.mapSizeY_);
    std::swap(this->pucMap_,   objOtherImage.pucMap_);
}

int GRAY_IMAGE_C::GetImageSize(unsigned int &sizeX, unsigned int &sizeY) const
{
    R_ASSERT(IsImageValid(), -1);

    sizeX = mapSizeX_;
    sizeY = mapSizeY_;
    return 0;
}

int GRAY_IMAGE_C::GetImageSize(IMAGE_COORDINATE_STRU &stSize) const
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
void GRAY_IMAGE_C::GetCellsOfLineSeg(const IMAGE_COORDINATE_STRU &stBeginCell, const IMAGE_COORDINATE_STRU &stEndCell, bool isGetBegin, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const
{
    int deltaCoordX    = stEndCell.x - stBeginCell.x;
    int deltaCoordY    = stEndCell.y - stBeginCell.y;
    int deltaCoordXAbs = std::abs(deltaCoordX);
    int deltaCoordYAbs = std::abs(deltaCoordY);

    int totalCellsNum  = int(vectCell.size()) + deltaCoordXAbs + deltaCoordYAbs + 1;
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
    int diffMax;
    if (deltaCoordXAbs >= deltaCoordYAbs) {
        incX    = 1.0;
        incY    = float(deltaCoordYAbs) / float(deltaCoordXAbs);
        diffMax = deltaCoordXAbs;
    } else {
        incX    = float(deltaCoordXAbs) / float(deltaCoordYAbs);
        incY    = 1.0;
        diffMax = deltaCoordYAbs;
    }

    int signIncX   = SIGN(deltaCoordX);
    int signIncY   = SIGN(deltaCoordY);
    float deltaX     = 0;
    float deltaY     = 0;
    int imageDistX = 0;
    int imageDistY = 0;

    for (int loop = 0; loop < diffMax; ++loop) {
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
        stCell.x = stBeginCell.x + (imageDistX + ((deltaX >= 0.5) ? 1 : 0)) * signIncX;
        stCell.y = stBeginCell.y + (imageDistY + ((deltaY >= 0.5) ? 1 : 0)) * signIncY;
        vectCell.push_back(stCell);
    }
}

void GRAY_IMAGE_C::SetCells(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, unsigned char setVal)
{
    for (const IMAGE_COORDINATE_STRU &stCell : vectCell) {
        (void)SetCell(stCell, setVal);
    }
}

void GRAY_IMAGE_C::SetCellsUnsafe(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, unsigned char setVal)
{
    for (const IMAGE_COORDINATE_STRU &stCell : vectCell) {
        SetCellUnsafe(stCell, setVal);
    }
}

int GRAY_IMAGE_C::SetCellsStrictly(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, unsigned char setVal)
{
    for (const IMAGE_COORDINATE_STRU &stCell : vectCell) {
        int rsltOfSetCell = SetCell(stCell, setVal);
        R_ASSERT_4(rsltOfSetCell == 0, -1, stCell.x, stCell.y, mapSizeX_, mapSizeY_);
    }

    return 0;
}

/**************************************************************************************
功能描述: 向右扩散栅格
修改记录:
**************************************************************************************/
int GRAY_IMAGE_C::ExpandCellsRight(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, unsigned char expandVal, unsigned char coverVal)
{
    for (const IMAGE_COORDINATE_STRU &stSrcCell : vectSrcCell) {
        unsigned char *pucSrcCell = GetCellPtr(stSrcCell);
        R_ASSERT_4(pucSrcCell != nullptr, -1, stSrcCell.x, stSrcCell.y, GetImageSizeX(), GetImageSizeY());

        /* 原始栅格一定赋值。 */
        unsigned int expandNumMax = GetImageSizeX() - stSrcCell.x;
        unsigned int expandNum    = 1;
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
int GRAY_IMAGE_C::ExpandCells(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, unsigned char expandVal, unsigned int expandDist, unsigned char coverVal)
{
    R_ASSERT(expandDist > 0, -1);

    auto pfExpandCellsOnce = [this](const std::vector<IMAGE_COORDINATE_STRU> &vectCellSrc, unsigned char expandVal, unsigned char coverVal, std::vector<IMAGE_COORDINATE_STRU> &vectCellTgt)
    {
        vectCellTgt.clear();
        vectCellTgt.reserve(std::min((unsigned int)(vectCellSrc.size()) * 4, this->GetImageMemSize()));

        for (const IMAGE_COORDINATE_STRU &stProcCell : vectCellSrc) {
            const IMAGE_COORDINATE_STRU stUpCell   {stProcCell.x,     stProcCell.y + 1};
            const IMAGE_COORDINATE_STRU stDownCell {stProcCell.x,     stProcCell.y - 1};
            const IMAGE_COORDINATE_STRU stLeftCell {stProcCell.x - 1, stProcCell.y};
            const IMAGE_COORDINATE_STRU stRightCell{stProcCell.x + 1, stProcCell.y};

            unsigned char cellVal;
            if ((0 == this->GetCellVal(stUpCell,    cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stUpCell,    expandVal); vectCellTgt.emplace_back(stUpCell); }
            if ((0 == this->GetCellVal(stDownCell,  cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stDownCell,  expandVal); vectCellTgt.emplace_back(stDownCell); }
            if ((0 == this->GetCellVal(stLeftCell,  cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stLeftCell,  expandVal); vectCellTgt.emplace_back(stLeftCell); }
            if ((0 == this->GetCellVal(stRightCell, cellVal)) && (cellVal == coverVal)) { this->SetCellUnsafe(stRightCell, expandVal); vectCellTgt.emplace_back(stRightCell); }
        }
    };

    int rsltOfSetSrcCells = SetCellsStrictly(vectSrcCell, expandVal);
    R_ASSERT_1(rsltOfSetSrcCells == 0, -1, vectSrcCell.size());

    std::vector<IMAGE_COORDINATE_STRU> vectCellSrc;
    std::vector<IMAGE_COORDINATE_STRU> vectCellTgt;
    for (unsigned int loop = 0; loop < expandDist; ++loop) {
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

unsigned int GRAY_IMAGE_C::GetImageMemSize(void) const
{
    return IsImageValid() ? (mapSizeX_ * mapSizeY_) : 0;
}

int GRAY_IMAGE_C::GetRightTopCoord(IMAGE_COORDINATE_STRU &stRightTopCoord) const
{
    R_ASSERT(IsImageValid(), -1);

    stRightTopCoord.x = mapSizeX_ - 1;
    stRightTopCoord.y = mapSizeY_ - 1;

    return 0;
}

int GRAY_IMAGE_C::TransSrcImageCoordToLoc(const IMAGE_COORDINATE_STRU &stSrcImageCoord, IMAGE_COORDINATE_STRU &stLocImageCoord) const
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
int GRAY_IMAGE_C::SetLeftAndRightPixelsOfLine(const std::vector<IMAGE_COORDINATE_STRU> &vectLine, unsigned char setLeftVal, unsigned char setRightVal, unsigned char splitVal, unsigned int maxStepLen)
{
    unsigned int lineSize = (unsigned int)(vectLine.size());
    R_ASSERT_1(lineSize > 1, -1, lineSize);

    std::vector<IMAGE_COORDINATE_STRU> vectLeftPixel;
    std::vector<IMAGE_COORDINATE_STRU> vectRightPixel;

    vectLeftPixel.reserve(lineSize * 4);
    vectRightPixel.reserve(lineSize * 4);

    /* 第一步，找到指定曲线两侧紧挨着的种子像素 */
    for (unsigned int loop = 1; loop < lineSize; ++loop) {
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
                unsigned char *pucPixel = GetCellPtr(stAroundPixel.x, stAroundPixel.y);                                                    \
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
    for (unsigned int loop = 0; loop <= maxStepLen; ++loop) {
        std::vector<IMAGE_COORDINATE_STRU> vectNextStepRightPixel;
        std::vector<IMAGE_COORDINATE_STRU> vectNextStepLeftPixel;

        unsigned int rightPixelSize = vectRightPixel.size();
        unsigned int leftPixelSize  = vectLeftPixel.size();

        vectNextStepRightPixel.reserve(rightPixelSize * 4);
        vectNextStepLeftPixel.reserve(leftPixelSize * 4);

        for (unsigned int loopOfPixel = 0; loopOfPixel < rightPixelSize; ++loopOfPixel) {
            const IMAGE_COORDINATE_STRU &stPixel = vectRightPixel[loopOfPixel];

            #define PROC_RIGHT_PIXEL(xOfRelPos_, yOfRelPos_)                                                                        \
                do {                                                                                                                \
                    IMAGE_COORDINATE_STRU stAroundPixel;                                                                            \
                    stAroundPixel.x = stPixel.x + (xOfRelPos_);                                                                     \
                    stAroundPixel.y = stPixel.y + (yOfRelPos_);                                                                     \
                    unsigned char *pucPixel = GetCellPtr(stAroundPixel.x, stAroundPixel.y);                                                \
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

        for (unsigned int loopOfPixel = 0; loopOfPixel < leftPixelSize; ++loopOfPixel) {
            const IMAGE_COORDINATE_STRU &stPixel = vectLeftPixel[loopOfPixel];

            #define PROC_LEFT_PIXEL(xOfRelPos_, yOfRelPos_)                                                                         \
                do {                                                                                                                \
                    IMAGE_COORDINATE_STRU stAroundPixel;                                                                            \
                    stAroundPixel.x = stPixel.x + (xOfRelPos_);                                                                     \
                    stAroundPixel.y = stPixel.y + (yOfRelPos_);                                                                     \
                    unsigned char *pucPixel = GetCellPtr(stAroundPixel.x, stAroundPixel.y);                                                \
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

int GRAY_MAP_C::GetMapInfo(COORDINATE_2D_STRU &stOriginPos, float &resolution, unsigned int &sizeX, unsigned int &sizeY, unsigned char **ppucMap) const
{
    R_ASSERT(IsValid(), -1);

    stOriginPos = GetOriginPos();
    resolution  = GetResolution();
    sizeX       = GetImageSizeX();
    sizeY       = GetImageSizeY();
    *ppucMap    = GetCellPtrUnsafe(0, 0);

    return 0;
}

int GRAY_MAP_C::GetMapSize(COORDINATE_2D_STRU &stSize) const
{
    IMAGE_COORDINATE_STRU stImageSize;
    int rsltOfGetImageSize = GetImageSize(stImageSize);
    R_ASSERT(rsltOfGetImageSize == 0, -1);

    stSize.x = GetResolution() * float(stImageSize.x);
    stSize.y = GetResolution() * float(stImageSize.y);
    return 0;
}

int GRAY_MAP_C::GetMapArea(RECT_AREA_C &objArea) const
{
    COORDINATE_2D_STRU stRightTopPoint;
    int rsltOfGetRightTopPoint = GetRightTopPoint(stRightTopPoint);
    R_ASSERT(rsltOfGetRightTopPoint == 0, -1);

    objArea.IncludePoint(GetOriginPos());
    objArea.IncludePoint(stRightTopPoint);
    return 0;
}

unsigned int GRAY_MAP_C::GetDistByCell(float dist) const
{
    return (unsigned int)(dist * GetReciprocalOfRes() + 0.5);
}

/**************************************************************************************
功能描述:  将图片位置转换为地图位置
修改记录:
**************************************************************************************/
int GRAY_MAP_C::TransPictureCoordToPos(const IMAGE_COORDINATE_STRU &stPictureCoord, COORDINATE_2D_STRU &stMapPos) const
{
    IMAGE_COORDINATE_STRU stMapCoord;

    if (0 != TransSrcImageCoordToLoc(stPictureCoord, stMapCoord)) {
        return -1;
    }

    ConvMapCoordToPos(stMapCoord, stMapPos);
    return 0;
}

/**************************************************************************************
功能描述:  创建地图
修改记录:
**************************************************************************************/
void GRAY_MAP_C::CreateMap(const COORDINATE_2D_STRU &stOriginPos, unsigned int mapSizeX, unsigned int mapSizeY, bool isDefaultNull, float resolution)
{
    SetOriginPos(AlignOriginPos(stOriginPos, resolution));
    SetResolution(resolution);

    unsigned char defaultVal = isDefaultNull ? cellCostNull_ : cellCostUnknown_;
    CreateImage(mapSizeX, mapSizeY, defaultVal);
}

/**************************************************************************************
功能描述:  创建地图
           根据左下角和右上角坐标确定地图大小和原点坐标
修改记录:
**************************************************************************************/
int GRAY_MAP_C::CreateMap(const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint, bool isDefaultNull, float resolution)
{
    COORDINATE_2D_STRU    stOriginPos;
    IMAGE_COORDINATE_STRU stMapSize;
    int rsltOfCalcMapInfo = CalcMapInfo(stLeftButtomPoint, stRightTopPoint, resolution, stOriginPos, stMapSize);
    R_ASSERT(rsltOfCalcMapInfo == 0, -1);

    SetOriginPos(stOriginPos);
    SetResolution(resolution);

    unsigned char defaultVal = isDefaultNull ? cellCostNull_ : cellCostUnknown_;
    CreateImage((unsigned int)(stMapSize.x), (unsigned int)(stMapSize.y), defaultVal);

    return 0;
}

/**************************************************************************************
功能描述:  创建地图
修改记录:
**************************************************************************************/
void GRAY_MAP_C::CreateMap(unsigned int mapSizeX, unsigned int mapSizeY, unsigned char defaultVal, float resolution)
{
    SetOriginPos(COORDINATE_2D_STRU(0.0, 0.0));
    SetResolution(resolution);

    CreateImage(mapSizeX, mapSizeY, defaultVal);
}

int GRAY_MAP_C::CopyMap(const GRAY_MAP_C &objSrcMap, const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint)
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

    int rslt = CopyImage(objSrcMap, stSrcLeftButtomPointCoord, stCopySize);
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

int GRAY_MAP_C::CopyMap(const GRAY_MAP_C &objSrcMap)
{
    R_ASSERT(objSrcMap.IsValid(), -1);

    DeleteMap();

    int rsltOfCopyImage = CopyImage(objSrcMap);
    R_ASSERT(rsltOfCopyImage == 0, -1);

    SetOriginAlignedSw(objSrcMap.IsOriginAligned());
    SetOriginPos(objSrcMap.GetOriginPos());
    SetResolution(objSrcMap.GetResolution());
    return 0;
}

void GRAY_MAP_C::DeleteMap(void)
{
    FreeImage();
}

void GRAY_MAP_C::SwapMap(GRAY_MAP_C &objOtherMap)
{
    std::swap(this->isOriginAligned_, objOtherMap.isOriginAligned_);
    std::swap(this->stOriginPos_,     objOtherMap.stOriginPos_);
    std::swap(this->imageResolution_, objOtherMap.imageResolution_);
    std::swap(this->reciprocalOfRes_, objOtherMap.reciprocalOfRes_);

    SwapImage(objOtherMap);
}

void GRAY_MAP_C::SetCellByPos(const COORDINATE_2D_STRU &stPos, unsigned char setVal)
{
    IMAGE_COORDINATE_STRU stCoord;
    ConvPosToMapCoord(stPos, stCoord);

    (void)SetCell(stCoord, setVal);
}

void GRAY_MAP_C::SetCellsByPos(const std::list<COORDINATE_2D_STRU> &posLst, unsigned char setVal)
{
    for (std::list<COORDINATE_2D_STRU>::const_iterator iter = posLst.begin();
         iter != posLst.end();
         iter++) {
        SetCellByPos(*iter, setVal);
    }
}

void GRAY_MAP_C::SetCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, unsigned char setVal)
{
    for (std::vector<COORDINATE_2D_STRU>::const_iterator iter = vectPos.begin();
         iter != vectPos.end();
         iter++) {
        SetCellByPos(*iter, setVal);
    }
}

void GRAY_MAP_C::SetPassableCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, unsigned char setVal)
{
    unsigned char costPassable = GetCostOfNull();

    for (const COORDINATE_2D_STRU &stPos : vectPos) {
        IMAGE_COORDINATE_STRU stCoord;
        ConvPosToMapCoord(stPos, stCoord);

        unsigned char *pValue = GetCellPtr(stCoord);
        if ((pValue != nullptr) && (*pValue == costPassable)) {
            *pValue = setVal;
        }
    }
}

/**************************************************************************************
功能描述: 对线段路径设置对应值
修改记录:
**************************************************************************************/
void GRAY_MAP_C::SetCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, bool isSetEnd)
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    unsigned int cellsNum = vctCells.size();
    R_ASSERT(cellsNum > 0, NOTHING);
    unsigned int setCellsNum = isSetEnd ? cellsNum : (cellsNum - 1);

    for (unsigned int loop = 0; loop < setCellsNum; ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        unsigned char *pucCell = GetCellPtr(stCell);
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
void GRAY_MAP_C::SetCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, unsigned int beginIdx, unsigned int endIdx)
{
    unsigned int LineSize = (unsigned int)(vectLine.size());
    if (LineSize == 0) {
        return;
    }

    R_ASSERT_2(beginIdx < LineSize, NOTHING, beginIdx, LineSize);

    unsigned int endIdxEx = std::min(endIdx, LineSize - 1);
    for (unsigned int loop = beginIdx + 1; loop <= endIdxEx; ++loop) {
        SetCellsOfLineSegment(vectLine[loop - 1], vectLine[loop], setVal, pvectSetPoint, false);
    }

    IMAGE_COORDINATE_STRU stEndCell;
    ConvPosToMapCoord(vectLine[endIdxEx], stEndCell);

    unsigned char *pucCell = GetCellPtr(stEndCell);
    if (pucCell != nullptr) {
        if ((pvectSetPoint != NULL) && (*pucCell != setVal)) {
            pvectSetPoint->push_back(stEndCell);
        }

        *pucCell = setVal;
    }
}

/**************************************************************************************
功能描述: 对线段路径设置对应值
修改记录:
**************************************************************************************/
void GRAY_MAP_C::SetPassableCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, bool isSetEnd)
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    unsigned int cellsNum = vctCells.size();
    R_ASSERT(cellsNum > 0, NOTHING);
    unsigned int setCellsNum = isSetEnd ? cellsNum : (cellsNum - 1);

    unsigned char costOfPassable = GetCostOfNull();
    for (unsigned int loop = 0; loop < setCellsNum; ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        unsigned char *pucCell = GetCellPtr(stCell);
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
void GRAY_MAP_C::SetPassableCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, unsigned int beginIdx, unsigned int endIdx)
{
    unsigned int LineSize = (unsigned int)(vectLine.size());
    if (LineSize == 0) {
        return;
    }

    R_ASSERT_2(beginIdx < LineSize, NOTHING, beginIdx, LineSize);

    unsigned int endIdxEx = std::min(endIdx, LineSize - 1);
    for (unsigned int loop = beginIdx + 1; loop <= endIdxEx; ++loop) {
        SetPassableCellsOfLineSegment(vectLine[loop - 1], vectLine[loop], setVal, pvectSetPoint, false);
    }

    IMAGE_COORDINATE_STRU stEndCell;
    ConvPosToMapCoord(vectLine[endIdxEx], stEndCell);

    unsigned char *pucCell = GetCellPtr(stEndCell);
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
int GRAY_MAP_C::SetLeftAndRightAreaOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, float areaDist, unsigned int splitLineSize)
{
    unsigned int lineSize = (unsigned int)(vectLine.size());
    R_ASSERT_2(lineSize > (splitLineSize * 2), -1, lineSize, splitLineSize);

    SetCellsOfLine(vectLine, cellCostSplitLine);

    std::vector<IMAGE_COORDINATE_STRU> vectSetPoint;
    SetCellsOfLine(vectLine, cellCostRightArea_, &vectSetPoint, splitLineSize, lineSize - splitLineSize);

    int rslt = SetLeftAndRightPixelsOfLine(vectSetPoint, cellCostLeftArea_, cellCostRightArea_, cellCostSplitLine, GET_CLOSEST_MULTIPLE(areaDist, GetResolution()));
    R_ASSERT(rslt == 0, -1);

    return 0;
}

void GRAY_MAP_C::SetMapNull(void)
{
    R_ASSERT(IsValid(), NOTHING);

    SetImage(cellCostNull_);
}

bool GRAY_MAP_C::IsCellsOfLinSegAllNull(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    for (unsigned int loop = 0; loop < vctCells.size(); ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        unsigned int cellCost = GetCellCost(stCell);

        if (cellCostNull_ != cellCost) {
            //ST_LOG_ERR("CellCost = %u. x-y(%u, %u).", cellCost, stCell.x, stCell.y);
            return false;
        }
    }

    return true;
}

bool GRAY_MAP_C::IsCellsOfLinSegIncludeBorder(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const
{
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stPoint1, stPoint2, vctCells);

    for (unsigned int loop = 0; loop < vctCells.size(); ++loop) {
        IMAGE_COORDINATE_STRU &stCell = vctCells[loop];
        unsigned int cellCost = GetCellCost(stCell);

        if (cellCostBorder_ == cellCost) {
            return true;
        }
    }

    return false;
}

unsigned char GRAY_MAP_C::GetCellByPos(const COORDINATE_2D_STRU &stPos) const
{
    IMAGE_COORDINATE_STRU stCoord;
    ConvPosToMapCoord(stPos, stCoord);

    return GetCellCost(stCoord);
}

int GRAY_MAP_C::GetOverlapArea(const RECT_AREA_C &objOtherArea, RECT_AREA_C &objOverlapArea) const
{
    RECT_AREA_C objMapArea;
    int rsltOfGetMapArea = GetMapArea(objMapArea);
    R_ASSERT(rsltOfGetMapArea == 0, -1);

    objOtherArea.GetOverlapArea(objMapArea, objOverlapArea);
    return 0;
}

int GRAY_MAP_C::GetOverlapArea(const GRAY_MAP_C &objOtherMap, RECT_AREA_C &objOverlapArea) const
{
    RECT_AREA_C objOtherMapArea;
    int rsltOfGetOtherMapArea = objOtherMap.GetMapArea(objOtherMapArea);
    R_ASSERT(rsltOfGetOtherMapArea == 0, -1);

    int rsltOfGetOverlap = GetOverlapArea(objOtherMapArea, objOverlapArea);
    R_ASSERT(rsltOfGetOverlap == 0, -1);

    return 0;
}

/**************************************************************************************
功能描述: 判断指定位置是有能通过空白区域连通地图边界
          也就是判断是否存在非空障碍将指定位置包围在地图中
修改记录:
**************************************************************************************/
bool GRAY_MAP_C::IsConnectToMapBorderByNull(const COORDINATE_2D_STRU &stBeginPos, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint) const
{
    IMAGE_COORDINATE_STRU astDetourExtendCellRelPosOfDirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};         /* 不同延伸方向下延伸栅格相对位置 */

    IMAGE_COORDINATE_STRU stBeginCell;
    ConvPosToMapCoord(stBeginPos, stBeginCell);
    unsigned int beginCellCost = GetCellCost(stBeginCell.x, stBeginCell.y);
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
            unsigned char nextCellCost = GetCellCost(stExtendCell.x + 1, stExtendCell.y);
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
        unsigned char                firstExtendDirAtDetourBeginCell = std::numeric_limits<unsigned char>::max();
        bool                  isPreDetourBeginCell            = false;
        IMAGE_COORDINATE_STRU stDetourBeginCell               = stExtendCell;
        unsigned char                detourExtendDir                 = 2;
        while (true) {
            bool isExtendCellSucc = false;
            /* 延伸方向， 0右，1上，2左，3下，顺序和值不可更改 */
            for (unsigned int loop = 0; loop < 4; ++loop) {
                unsigned char                       curExtendDir          = (detourExtendDir + loop + 3) & 0x03;
                const IMAGE_COORDINATE_STRU &stCurExtendCellRelPos = astDetourExtendCellRelPosOfDirs[curExtendDir];
                IMAGE_COORDINATE_STRU        stCurExtendCell       = {stExtendCell.x + stCurExtendCellRelPos.x, stExtendCell.y + stCurExtendCellRelPos.y};

                unsigned char curExtendCellCost = GetCellCost(stCurExtendCell.x, stCurExtendCell.y);
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
            firstExtendDirAtDetourBeginCell = (firstExtendDirAtDetourBeginCell == std::numeric_limits<unsigned char>::max()) ? detourExtendDir : firstExtendDirAtDetourBeginCell;
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

unsigned char GRAY_MAP_C::GetCellCost(int cellX, int cellY) const
{
    unsigned char cellCost;

    if (0 != GetCellVal(cellX, cellY, cellCost)) {
        return cellCostOver_;
    }

    return cellCost;
}

unsigned char GRAY_MAP_C::GetCellCost(const IMAGE_COORDINATE_STRU &stCellCoord) const
{
    return GetCellCost(stCellCoord.x, stCellCoord.y);
}

int GRAY_MAP_C::GetRightTopPoint(COORDINATE_2D_STRU &stRightTopPoint) const
{
    IMAGE_COORDINATE_STRU stRightTopCoord;
    int rsltOfGetRightTopCoord = GetRightTopCoord(stRightTopCoord);
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

void GRAY_MAP_C::GetCellsOfCurve(const std::vector<COORDINATE_2D_STRU> &vectCurvePoint, bool isNeedToClose, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const
{
    unsigned int curvePointNum = (unsigned int)(vectCurvePoint.size());
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
    for (unsigned int loop = 1; loop < curvePointNum; ++loop) {
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

    stCoord.x = (int)((stPos.x - stOriginPos.x) * reciprocalOfRes + 0.5);
    stCoord.y = (int)((stPos.y - stOriginPos.y) * reciprocalOfRes + 0.5);
}

void GRAY_MAP_C::ConvPosToMapCoord(const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const
{
    stCoord.x = (int)((stPos.x - GetOriginPos().x) * GetReciprocalOfRes() + 0.5);
    stCoord.y = (int)((stPos.y - GetOriginPos().y) * GetReciprocalOfRes() + 0.5);
}

void GRAY_MAP_C::ConvMapCoordToPos(const IMAGE_COORDINATE_STRU &stCoord, COORDINATE_2D_STRU &stPos) const
{
    stPos.x = stCoord.x * GetResolution() + GetOriginPos().x;
    stPos.y = stCoord.y * GetResolution() + GetOriginPos().y;
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

int GRAY_MAP_C::CalcMapInfo(const COORDINATE_2D_STRU &stLBPoint, const COORDINATE_2D_STRU &stRTPoint, float resolution, COORDINATE_2D_STRU &stOriginPos, IMAGE_COORDINATE_STRU &stMapSize)
{
    R_ASSERT((stRTPoint.x > stLBPoint.x) && (stRTPoint.y > stLBPoint.y), -1);

    stOriginPos = AlignOriginPos(stLBPoint, resolution);

    IMAGE_COORDINATE_STRU stRightTopCoord;
    ConvPosToMapCoord(stOriginPos, resolution, stRTPoint, stRightTopCoord);

    stMapSize.x = stRightTopCoord.x + 1;
    stMapSize.y = stRightTopCoord.y + 1;

    return 0;
}

bool TRC_MAP_C::IsCellObst(int cellX, int cellY) const
{
    return (GetCellCost(cellX, cellY) == cellCostObst_);
}

bool TRC_MAP_C::Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const
{
    /* 判断车体四边 */
    return (   IsCellsOfLinSegAllNull(stLeftFront,  stRightFront)
            && IsCellsOfLinSegAllNull(stLeftBack,   stRightBack)
            && IsCellsOfLinSegAllNull(stLeftFront,  stLeftBack)
            && IsCellsOfLinSegAllNull(stRightFront, stRightBack));
}

/**************************************************************************************
功能描述: 判断车体区域的所有栅格是否可通行
修改记录:
**************************************************************************************/
bool TRC_MAP_C::IsAllAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const
{
    /* 判断车体四边 */
    std::vector<IMAGE_COORDINATE_STRU> vctCells;
    GetCellsOfLineSeg(stLeftFront,  stRightFront, vctCells);
    GetCellsOfLineSeg(stLeftBack,   stRightBack,  vctCells);
    GetCellsOfLineSeg(stLeftFront,  stLeftBack,   vctCells);
    GetCellsOfLineSeg(stRightFront, stRightBack,  vctCells);

    int minX = std::numeric_limits<int>::max(), minY = std::numeric_limits<int>::max(), maxX = 0, maxY = 0;
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

    int sizeX  = maxX - minX + 1;
    int sizeY  = maxY - minY + 1;
    R_ASSERT(sizeX > 0, false);
    R_ASSERT(sizeY > 0, false);

    unsigned int size   = std::max(sizeX, sizeY);
    std::vector<bool> space(size * size, false);
    for (const IMAGE_COORDINATE_STRU &point : vctCells) {
        unsigned int x = point.x - minX;
        unsigned int y = point.y - minY;
        space[y * size + x] = true;
    }

    // FILL THE SHAPE
    for (unsigned int i = 0; i < size; ++i) {
        // set inside to false
        bool inside = false;
        unsigned int hcross1 =0, hcross2 = 0;

        // determine x crossings
        for (unsigned int k = 0; k < size; ++k) {
            if (space[i * size + k] && !inside) {
                hcross1 = k;
                inside = true;
            }

            if (space[i * size + k] && inside) {
                hcross2 = k;
            }
        }

        for (unsigned int j = 0; j < size; ++j) {
            // if inside fill
            if (j > hcross1 && j < hcross2 && inside) {
                space[i * size + j] = true;
            }
        }
    }

    // FILL THE SHAPE
    for (unsigned int i = 0; i < size; ++i) {
        for (unsigned int j = 0; j < size; ++j) {
            if (space[i * size + j]) {
                IMAGE_COORDINATE_STRU stCell{int(j + minX), int(i + minY)};
                unsigned int cellCost = GetCellCost(stCell);

                if (cellCostNull_ != cellCost) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool TRC_MAP_C::IsPointOutOfPassableArea(const COORDINATE_2D_STRU &stPoint) const
{
    unsigned char cellCost = GetCellByPos(stPoint);
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

bool COVER_MAP_C::IsMarginPoint(unsigned int sizeX, unsigned int sizeY, const IMAGE_COORDINATE_STRU &stPoint) const
{
    if (stPoint.x == 0) {
        return true;
    }

    if (stPoint.x + 1 == (int)sizeX) {
        return true;
    }

    if (stPoint.y == 0) {
        return true;
    }

    if (stPoint.y + 1 == (int)sizeY) {
        return true;
    }

    return false;
}

int COVER_MAP_C::MarkGridsExpand(const COORDINATE_2D_STRU &stCarPos, unsigned char value, std::vector<IMAGE_COORDINATE_STRU> *pVectMarginPoint)
{
    unsigned int sizeX;
    unsigned int sizeY;
    int rslt = GetImageSize(sizeX, sizeY);
    R_ASSERT(rslt == 0, -1);

    IMAGE_COORDINATE_STRU seedPoint;
    ConvPosToMapCoord(stCarPos, seedPoint);

    std::queue<IMAGE_COORDINATE_STRU> q;
    q.push(seedPoint);
    unsigned char costOfUnknown = GetCostOfUnknown();
    unsigned char costOfNull    = GetCostOfNull();
    while (!q.empty()) {
        IMAGE_COORDINATE_STRU curPoint = q.front();
        q.pop();

        for (unsigned int i = 0; i < dirLimited_; i++) {
            IMAGE_COORDINATE_STRU pointSucc;
            int rslt = CreateSuccessor(curPoint, i, pointSucc);
            if (rslt != 0) {
                continue;
            }

            unsigned char cellVal;
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

int COVER_MAP_C::MarkGridsInArea(const std::vector<COORDINATE_2D_STRU> &vertexList)
{
    R_ASSERT(vertexList.size() >= 3, -1);
    for (unsigned int idx = 0; idx < vertexList.size(); idx++) {
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
    int rslt = SearchSeedPoint(vertexCoordList, seedPoint);
    if (rslt != 0) {
        ST_LOG_ERR("SearchSeedPoint Fail.");
        return -1;
    }

    COORDINATE_2D_STRU mapPos;
    ConvMapCoordToPos(seedPoint, mapPos);

    return MarkGridsExpand(mapPos, GetCostOfNull());
}

int COVER_MAP_C::SearchSeedPoint(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, IMAGE_COORDINATE_STRU &seedPoint) const
{
    unsigned int sizeX;
    unsigned int sizeY;
    int rslt = GetImageSize(sizeX, sizeY);
    R_ASSERT(rslt == 0, -1);

    for (unsigned int x = 0; x < sizeX; x++) {
        for (unsigned int y = 0; y < sizeY; y++) {
            /* 不取多边形上的点 */
            IMAGE_COORDINATE_STRU point{(int)x, (int)y};
            COORDINATE_2D_STRU stMapPos;
            rslt = TransPictureCoordToPos(point, stMapPos);
            R_ASSERT(rslt == 0, -1);

            if (GetCellByPos(stMapPos) == GetCostOfObst()) {
                continue;
            }

            bool obsAside = false;
            for(unsigned int i = 0; i < dirAll_; i++) {
                IMAGE_COORDINATE_STRU pointSucc;
                rslt = CreateSuccessor(point, i, pointSucc);
                if (rslt != 0) {
                    continue;
                }

                unsigned char cellVal;
                rslt = GetCellVal(pointSucc.x, pointSucc.y, cellVal);
                R_ASSERT(rslt == 0, -1);
                if (cellVal == GetCostOfObst()) {
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

bool COVER_MAP_C::IsPointInPolygon(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, const IMAGE_COORDINATE_STRU &seedPoint) const
{
    float sumAngle = 0.00;
    for (unsigned int idx = 0; idx < vertexCoordList.size(); idx++) {
        IMAGE_COORDINATE_STRU vertexCoord1 = vertexCoordList[idx];
        IMAGE_COORDINATE_STRU vertexCoord2;
        if (idx == vertexCoordList.size() - 1) {
            vertexCoord2 = vertexCoordList[0];

        } else {
            vertexCoord2 = vertexCoordList[idx + 1];
        }
        float angle1 = atan2(vertexCoord1.y - seedPoint.y, vertexCoord1.x - seedPoint.x);
        float angle2 = atan2(vertexCoord2.y - seedPoint.y, vertexCoord2.x - seedPoint.x);
        sumAngle += NormalizeAngle(angle2 - angle1);
    }

    if (fabs(sumAngle) < 0.01) {
        return false;
    } else {
        ST_LOG_INFO("sumAngle %f.", sumAngle);
        return true;
    }
}

int COVER_MAP_C::GetPoints(unsigned char value, std::vector<COORDINATE_2D_STRU> &vectPoint) const
{
    unsigned int curSizeX;
    unsigned int curSizeY;
    int rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);

    for (unsigned int x = 0; x < curSizeX; x++) {
        for (unsigned int y = 0; y < curSizeY; y++) {
            unsigned char curValue;
            rslt = GetCellVal(x, y, curValue);
            R_ASSERT(rslt == 0, -1);

            if (curValue == value) {
                IMAGE_COORDINATE_STRU stCoord{(int)x, (int)y};
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
int COVER_MAP_C::CreateSuccessor(const IMAGE_COORDINATE_STRU &stCoordCur, unsigned int i, IMAGE_COORDINATE_STRU &stCoordSucc) const
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

int COVER_MAP_C::CopyPassableArea(COVER_MAP_C &objMap) const
{
    unsigned int curSizeX;
    unsigned int curSizeY;
    int rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);

    unsigned int targetSizeX;
    unsigned int targetSizeY;
    rslt = objMap.GetImageSize(targetSizeX, targetSizeY);
    R_ASSERT(rslt == 0, -1);

    R_ASSERT(curSizeX == targetSizeX, -1);
    R_ASSERT(curSizeY == targetSizeY, -1);

    for (unsigned int x = 0; x < curSizeX; x++) {
        for (unsigned int y = 0; y < curSizeY; y++) {
            unsigned char curValue;
            unsigned char targetValue;
            rslt = GetCellVal(x, y, curValue);
            R_ASSERT(rslt == 0, -1);

            rslt = objMap.GetCellVal(x, y, targetValue);
            R_ASSERT(rslt == 0, -1);

            if (curValue == GetCostOfNull() && targetValue != GetCostOfObst()) {
                objMap.SetCell(x, y, curValue);
            }
        }
    }

    return 0;
}

int COVER_MAP_C::MarkMargin()
{
    unsigned int curSizeX;
    unsigned int curSizeY;
    int rslt = GetImageSize(curSizeX, curSizeY);
    R_ASSERT(rslt == 0, -1);

    for (unsigned int y = 0; y < curSizeY; y++) {
        SetCell(0, y, GetCostOfObst());
    }

    for (unsigned int y = 0; y < curSizeY; y++) {
        SetCell(curSizeX - 1, y, GetCostOfObst());
    }

    for (unsigned int x = 0; x < curSizeX; x++) {
        SetCell(x, 0, GetCostOfObst());
    }

    for (unsigned int x = 0; x < curSizeX; x++) {
        SetCell(x, curSizeY - 1, GetCostOfObst());
    }

    return 0;
}

bool COVER_MAP_C::IsRouteInPassableArea(std::vector<COORDINATE_2D_STRU> &vecRoute, unsigned int &firstIdxInArea) const
{
    for (unsigned int i = 0; i< vecRoute.size(); i++) {
        const COORDINATE_2D_STRU &stPoint = vecRoute[i];
        if (GetCellByPos(stPoint) == GetCostOfNull()) {
            firstIdxInArea = i;
            return true;
        }
    }

    return false;
}

bool COVER_MAP_C::Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const
{
    /* 判断车体四边 */
    return (   IsCellsOfLinSegAllNull(stLeftFront,  stRightFront)
            || IsCellsOfLinSegAllNull(stLeftBack,   stRightBack)
            || IsCellsOfLinSegAllNull(stLeftFront,  stLeftBack)
            || IsCellsOfLinSegAllNull(stRightFront, stRightBack));
}

