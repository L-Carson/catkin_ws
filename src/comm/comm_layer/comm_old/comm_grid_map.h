
#ifndef __COMM_GRID_MAP_H__
#define __COMM_GRID_MAP_H__

#include "comm_coordinate.h"
#include "comm_rect_area.h"


#include <list>
#include <vector>


/* 默认的地图分辨率 */
#define MAP_RESOLUTION                          0.05

/**************************************************************************************
功能描述: 灰度图像类
          该图像主要用于地图处理，所以图像行序是上下颠倒的
          本类作为栅格地图类的基类，不要用作它用
修改记录:
**************************************************************************************/
class GRAY_IMAGE_C
{
public:
    GRAY_IMAGE_C(void)
    {
        mapSizeX_ = 0;
        mapSizeY_ = 0;
        pucMap_   = NULL;
    }

    virtual ~GRAY_IMAGE_C(void)
    {
        FreeImage();
    }

    void   CreateImage(unsigned int sizeX, unsigned int sizeY);
    void   CreateImage(unsigned int sizeX, unsigned int sizeY, unsigned char defaultVal);

    int  CopyImage(const GRAY_IMAGE_C &objSrcImage, const IMAGE_COORDINATE_STRU &stCopyStart, const IMAGE_COORDINATE_STRU &stCopySize);
    int  CopyImage(const GRAY_IMAGE_C &objSrcImage);


    void   FreeImage(void);
    void   SwapImage(GRAY_IMAGE_C &objOtherImage);

    int  GetImageSize(unsigned int &sizeX, unsigned int &sizeY) const;
    int  GetImageSize(IMAGE_COORDINATE_STRU &stSize) const;
    unsigned int GetImageMemSize(void) const;

    void   GetCellsOfLineSeg(const IMAGE_COORDINATE_STRU &stBeginCell, const IMAGE_COORDINATE_STRU &stEndCell, bool isGetBegin, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const;

    void   SetCells(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, unsigned char setVal);
    void   SetCellsUnsafe(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, unsigned char setVal);
    int    SetCellsStrictly(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, unsigned char setVal);

    int    ExpandCellsRight(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, unsigned char expandVal, unsigned char coverVal);
    int    ExpandCells(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, unsigned char expandVal, unsigned int expandDistt, unsigned char coverVal);

protected:
    inline bool    IsImageValid(void) const                                { return ((pucMap_ != NULL) && (mapSizeX_ != 0) && (mapSizeY_ != 0)); }
    inline bool    IsCoordValid(int x, int y) const                    { return ((pucMap_ != NULL) && IS_IN_C_O_RANGE(x, 0, int(mapSizeX_)) && IS_IN_C_O_RANGE(y, 0, int(mapSizeY_))); }
    inline bool    IsCoordValid(const IMAGE_COORDINATE_STRU &stCell) const { return IsCoordValid(stCell.x, stCell.y); }
    inline unsigned int  GetImageSizeX(void) const                               { return mapSizeX_; }
    inline unsigned int  GetImageSizeY(void) const                               { return mapSizeY_; }

    inline unsigned char *GetCellPtrUnsafe(int x, int y) const                           { return &pucMap_[y * mapSizeX_ + x]; };
    inline unsigned char *GetCellPtrUnsafe(const IMAGE_COORDINATE_STRU &stCell) const        { return GetCellPtrUnsafe(stCell.x, stCell.y); }
    inline unsigned char *GetCellPtr(int x, int y) const                                 { return IsCoordValid(x, y) ? GetCellPtrUnsafe(x, y) : nullptr; }
    inline unsigned char *GetCellPtr(const IMAGE_COORDINATE_STRU &stCell) const              { return GetCellPtr(stCell.x, stCell.y); }
    inline unsigned char  GetCellValUnsafe(int x, int y) const                           { return *GetCellPtrUnsafe(x, y); }
    inline unsigned char  GetCellValUnsafe(const IMAGE_COORDINATE_STRU &stCell) const        { return GetCellValUnsafe(stCell.x, stCell.y); }
    inline int   GetCellVal(int x, int y, unsigned char &val) const                    { if (!IsCoordValid(x, y)) { return -1; } val = GetCellValUnsafe(x, y); return 0; }
    inline int   GetCellVal(const IMAGE_COORDINATE_STRU &stCell, unsigned char &val) const { return GetCellVal(stCell.x, stCell.y, val); }

    inline void    SetCellUnsafe(int x, int y, unsigned char val)                    { *GetCellPtrUnsafe(x, y) = val; }
    inline void    SetCellUnsafe(const IMAGE_COORDINATE_STRU &stCell, unsigned char val) { SetCellUnsafe(stCell.x, stCell.y, val); }
    inline int   SetCell(int x, int y, unsigned char val)                          { if (!IsCoordValid(x, y)) { return -1; } SetCellUnsafe(x, y, val); return 0; }
    inline int   SetCell(const IMAGE_COORDINATE_STRU &stCell, unsigned char val)       { return SetCell(stCell.x, stCell.y, val); }

    int  GetRightTopCoord(IMAGE_COORDINATE_STRU &stRightTopCoord) const;
    int  TransSrcImageCoordToLoc(const IMAGE_COORDINATE_STRU &stSrcImageCoord, IMAGE_COORDINATE_STRU &stLocImageCoord) const;

    void   SetImage(unsigned char setVal);

    int  SetLeftAndRightPixelsOfLine(const std::vector<IMAGE_COORDINATE_STRU> &vectLine, unsigned char setLeftVal, unsigned char setRightVal, unsigned char splitVal, unsigned int maxStepLen);

private:
    unsigned int   mapSizeX_;                           /* 图像X方向大小，即像素列数 */
    unsigned int   mapSizeY_;                           /* 图像Y方向大小，即像素行数 */
    unsigned char *pucMap_;                             /* 图像内存地址 */

    GRAY_IMAGE_C(const GRAY_IMAGE_C &) = delete;
    const GRAY_IMAGE_C &operator=(const GRAY_IMAGE_C &) = delete;
};

/**************************************************************************************
功能描述: 栅格地图
修改记录:
**************************************************************************************/
class GRAY_MAP_C : public GRAY_IMAGE_C
{
public:
    GRAY_MAP_C(bool isOriginAlignedByResolution = false)
    {
        SetOriginAlignedSw(isOriginAlignedByResolution);
        SetOriginPos(COORDINATE_2D_STRU(0.0, 0.0));
        SetResolution(MAP_RESOLUTION);
    }

    virtual ~GRAY_MAP_C(void)
    {
    }

    inline bool                      IsValid(void) const            { return IsImageValid(); }
    inline const COORDINATE_2D_STRU &GetOriginPos(void) const       { return stOriginPos_; }
    inline float                     GetResolution(void) const      { return imageResolution_; }
    inline float                     GetReciprocalOfRes(void) const { return reciprocalOfRes_; }

    int  GetMapInfo(COORDINATE_2D_STRU &stOriginPos, float &resolution, unsigned int &sizeX, unsigned int &sizeY, unsigned char **ppucMap) const;
    int  GetMapSize(COORDINATE_2D_STRU &stSize) const;
    int  GetMapArea(RECT_AREA_C &objArea) const;
    unsigned int GetDistByCell(float dist) const;

    int  TransPictureCoordToPos(const IMAGE_COORDINATE_STRU &stPictureCoord, COORDINATE_2D_STRU &stMapPos) const;
    void   ConvPosToMapCoord(const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const;
    void   ConvMapCoordToPos(const IMAGE_COORDINATE_STRU &stCoord, COORDINATE_2D_STRU &stPos) const;

    static inline unsigned char GetCostOfObst(void)       { return cellCostObst_;      }
    static inline unsigned char GetCostOfNull(void)       { return cellCostNull_;      }
    static inline unsigned char GetCostOfUnknown(void)    { return cellCostUnknown_;   }
    static inline unsigned char GetCostOfBorder(void)     { return cellCostBorder_;    }
    static inline unsigned char GetCostOfOver(void)       { return cellCostOver_;      }
    static inline unsigned char GetCostOfLeft(void)       { return cellCostLeftArea_;  }
    static inline unsigned char GetCostOfRight(void)      { return cellCostRightArea_; }
    static inline unsigned char GetCostOfInside(void)     { return cellCostInside_;    }
    static inline unsigned char GetCostOfOutside(void)    { return cellCostOutside_;   }

    void  CreateMap(const COORDINATE_2D_STRU &stOriginPos, unsigned int mapSizeX, unsigned int mapSizeY, bool isDefaultNull = false, float resolution = MAP_RESOLUTION);
    int   CreateMap(const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint, bool isDefaultNull = false, float resolution = MAP_RESOLUTION);
    void  CreateMap(unsigned int mapSizeX, unsigned int mapSizeY, unsigned char defaultVal, float resolution = MAP_RESOLUTION);

    int   CopyMap(const GRAY_MAP_C &objSrcMap, const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint);
    int   CopyMap(const GRAY_MAP_C &objSrcMap);

    void  DeleteMap(void);
    void  SwapMap(GRAY_MAP_C &objOtherMap);

    void  SetCellByPos(const COORDINATE_2D_STRU &stPos, unsigned char setVal);
    void  SetCellsByPos(const std::list<COORDINATE_2D_STRU> &posLst, unsigned char setVal);
    void  SetCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, unsigned char setVal);

    void  SetPassableCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, unsigned char setVal);

    void  SetCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, bool isSetEnd);
    void  SetCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint = NULL, unsigned int beginIdx = 0, unsigned int endIdx = std::numeric_limits<unsigned int>::max());

    void  SetPassableCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, bool isSetEnd);
    void  SetPassableCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, unsigned char setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint = NULL, unsigned int beginIdx = 0, unsigned int endIdx = std::numeric_limits<unsigned int>::max());

    int SetLeftAndRightAreaOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, float areaDist, unsigned int splitLineSize);
    void  SetMapNull(void);

    bool  IsCellsOfLinSegAllNull(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const;
    bool  IsCellsOfLinSegIncludeBorder(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const;

    unsigned char GetCellByPos(const COORDINATE_2D_STRU &stPos) const;
    int  GetOverlapArea(const RECT_AREA_C &objOtherArea, RECT_AREA_C &objOverlapArea) const;
    int  GetOverlapArea(const GRAY_MAP_C &objOtherMap, RECT_AREA_C &objOverlapArea) const;

    bool   IsConnectToMapBorderByNull(const COORDINATE_2D_STRU &stBeginPos, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint = NULL) const;

protected:
    /* 基本类型 */
    static const unsigned char cellCostObst_            = 0;   /* 这三个数值写死的，可视化时对应了数值                   */
    static const unsigned char cellCostNull_            = 254;
    static const unsigned char cellCostUnknown_         = 205;

    static const unsigned char cellCostBorder_          = 10;

    static const unsigned char cellCostOver_            = 1;

    /* 可视化 */
    static const unsigned char cellCostCarBodyTrc_      = 200;

    /* 区域划分 */
    static const unsigned char cellCostRightArea_       = 160; /* 这两个数值写死的，可视化时对应了数值                   */
    static const unsigned char cellCostLeftArea_        = 180;

    static const unsigned char cellCostSplitLine        = 170;
    static const unsigned char cellCostInside_          = 150;
    static const unsigned char cellCostOutside_         = 190;

    unsigned char  GetCellCost(int cellX, int cellY) const;
    unsigned char  GetCellCost(const IMAGE_COORDINATE_STRU &stCellCoord) const;
    int   GetRightTopPoint(COORDINATE_2D_STRU &stRightTopPoint) const;

    /* 获取指定线段上的栅格，相邻栅格不重复 */
    void    GetCellsOfLineSeg(const COORDINATE_2D_STRU &stBeginPoint, const COORDINATE_2D_STRU &stEndPoint, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const;
    void    GetCellsOfCurve(const std::vector<COORDINATE_2D_STRU> &vectCurvePoint, bool isNeedToClose, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const;

private:
    bool               isOriginAligned_;        /* 原点是否按照分辨率对齐，也就是原点是分辨率的整倍数。 */
    COORDINATE_2D_STRU stOriginPos_;            /* 原点 */
    float              imageResolution_;        /* 分辨率 */
    float              reciprocalOfRes_;        /* 分辨率倒数 */

    inline void SetOriginAlignedSw(bool isOriginAligned)            { isOriginAligned_ = isOriginAligned; }
    inline void SetOriginPos(const COORDINATE_2D_STRU &stOriginPos) { stOriginPos_ = stOriginPos; }
    inline void SetResolution(float resolution)                     { imageResolution_ = resolution; reciprocalOfRes_ = 1.0f / resolution; }
    inline bool IsOriginAligned(void) const                         { return isOriginAligned_; }


    COORDINATE_2D_STRU AlignOriginPos(const COORDINATE_2D_STRU &stOriginPos, float resolution) const;
    int              CalcMapInfo(const COORDINATE_2D_STRU &stLBPoint, const COORDINATE_2D_STRU &stRTPoint, float resolution, COORDINATE_2D_STRU &stOriginPos, IMAGE_COORDINATE_STRU &stMapSize);
    void               ConvPosToMapCoord(const COORDINATE_2D_STRU &stOriginPos, float resolution, const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const;
};

/**************************************************************************************
功能描述: 车体轨迹地图
修改记录:
**************************************************************************************/
class TRC_MAP_C : public GRAY_MAP_C
{
public:
    bool  Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const;
    bool  IsAllAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const;
    bool  IsPointOutOfPassableArea(const COORDINATE_2D_STRU &stPoint) const;

    void  SetCarBodyArea(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack);

    bool  IsCellObst(int cellX, int cellY) const;

private:
};

/**************************************************************************************
功能描述: 规划覆盖地图
修改记录:
**************************************************************************************/
class COVER_MAP_C : public GRAY_MAP_C
{
public:
    int  MarkGridsExpand(const COORDINATE_2D_STRU &stCarPos, unsigned char value, std::vector<IMAGE_COORDINATE_STRU> *pVectMarginPoint = nullptr);
    int  MarkGridsInArea(const std::vector<COORDINATE_2D_STRU> &vertexList);
    int  MarkMargin();
    int  GetPoints(unsigned char value, std::vector<COORDINATE_2D_STRU> &vectPoint) const;

    int  CopyPassableArea(COVER_MAP_C &objMap) const;
    bool   Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const;
    bool   IsRouteInPassableArea(std::vector<COORDINATE_2D_STRU> &vecRoute, unsigned int &firstIdxInArea) const;

private:

    int  SearchSeedPoint(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, IMAGE_COORDINATE_STRU &seedPoint) const;
    bool   IsPointInPolygon(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, const IMAGE_COORDINATE_STRU &seedPoint) const;
    int  CreateSuccessor(const IMAGE_COORDINATE_STRU &stCoordCur, unsigned int i, IMAGE_COORDINATE_STRU &stCoordSucc) const;

    bool   IsMarginPoint(unsigned int sizeX, unsigned int sizeY, const IMAGE_COORDINATE_STRU &stPoint) const;

    // possible directions
    const unsigned int dirLimited_ = 4;
    const unsigned int dirAll_     = 8;

    // possible movements
    const int  dx_[8] = {-1, 0, 0, 1, 1, 1, -1, -1};
    const int  dy_[8] = {0, -1, 1, 0, 1, -1, 1, -1};
};

#endif

