
#ifndef __COMM_MAP_H__
#define __COMM_MAP_H__

#include "comm/comm_car.h"
#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 读取yaml文件
          yaml文件有专门的插件可以读入和写出，此处用得比较简单，按固定格式读取即可。
          限制:
            每一项必须单独为一行
            每一行最长1K
修改记录:
**************************************************************************************/
#define SIMPLE_YAML_FILE_MAX_ITEM_LEN   1024

constexpr int g_dirs[8][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}}; // 搜索方向

struct YAML_FILE_ITEM_STRU {
    const char *pStrTag;
    char strContent[SIMPLE_YAML_FILE_MAX_ITEM_LEN];     /* tag:之后的内容，不含空格 */
};

class SIMPLE_YAML_FILE_C
{
public:
    SIMPLE_YAML_FILE_C(const char *pcYamlFileName);

    int ReadItem(YAML_FILE_ITEM_STRU astItem[], int itemNum);
    int WriteItem(YAML_FILE_ITEM_STRU astItem[], int itemNum);
private:
    string strFileName_;

    void DelAllCharInStr(char *pcStr, char delChar);
    bool IsStrBeginWith(const char *pcStr, const char *pcSubStr);
    YAML_FILE_ITEM_STRU *MatchItem(YAML_FILE_ITEM_STRU astItem[], int itemNum, const char *pstrLine);
    int ReadItemFromFile(YAML_FILE_ITEM_STRU astItem[], int itemNum);
};

class MAP_YAML_FILE_C
{
public:
    int  Read(const string &strMapPath, string &strPgmFileName, float &resolution, COORDINATE_2D_STRU &stOriginPos) const;
    int  Write(const string &strMapPath, float resolution, const COORDINATE_2D_STRU &stOriginPos) const;

private:
    void GetYamlFilePath(const string &strMapPath, string &strYamlFilePath) const;
    void GetPgmFileName(const string &strMapPath, string &strPgmFileName) const;
};

class PGM_FILE_C
{
public:
    PGM_FILE_C(void);
    virtual ~PGM_FILE_C(void);

    int  ReadImageSize(const string &strMapPath, unsigned int &imageCols, unsigned int &imageRows);
    int  ReadImage(const string &strMapPath, unsigned char *pucImage, unsigned int imageMaxSize);

    void SaveFile(const string &strMapPath, unsigned char *pucImage, unsigned int imageCols, unsigned int imageRows);

private:

    void GetPgmFilePath(const string &strMapPath, string &strPgmFilePath);

    int  ReadNextLine(FILE *pfRead, char *pucLine, int maxLineLen);
    int  ReadFileHead(FILE *pfRead, unsigned int &imageCols, unsigned int &imageRows);
};

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

    void   CreateImage(UINT32 sizeX, UINT32 sizeY);
    void   CreateImage(UINT32 sizeX, UINT32 sizeY, UINT08 defaultVal);
    INT32  CreateImage(UINT32 sizeX, UINT32 sizeY, UINT08 *pucImage);

    INT32  CopyImage(const GRAY_IMAGE_C &objSrcImage, const IMAGE_COORDINATE_STRU &stCopyStart, const IMAGE_COORDINATE_STRU &stCopySize);
    INT32  CopyImage(const GRAY_IMAGE_C &objSrcImage);
    INT32  CopyImageAreaOfVal(const GRAY_IMAGE_C &objSrcImage, const IMAGE_COORDINATE_STRU &stStartOfSrc, const IMAGE_COORDINATE_STRU &stStartOfLoc, const IMAGE_COORDINATE_STRU &stCopySize, UINT08 copyVal);

    INT32  LoadImage(const string &strMapPath);
    INT32  SaveImage(const string &strMapPath) const;

    void   FreeImage(void);
    void   SwapImage(GRAY_IMAGE_C &objOtherImage);

    INT32  GetImageSize(UINT32 &sizeX, UINT32 &sizeY) const;
    INT32  GetImageSize(IMAGE_COORDINATE_STRU &stSize) const;
    UINT32 GetImageMemSize(void) const;

    void   GetCellsOfLineSeg(const IMAGE_COORDINATE_STRU &stBeginCell, const IMAGE_COORDINATE_STRU &stEndCell, BOOL isGetBegin, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const;

    void   SetCells(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, UINT08 setVal);
    void   SetCellsUnsafe(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, UINT08 setVal);
    INT32  SetCellsStrictly(const std::vector<IMAGE_COORDINATE_STRU> &vectCell, UINT08 setVal);

    INT32  ExpandCellsRight(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, UINT08 expandVal, UINT08 coverVal);
    INT32  ExpandCells(const std::vector<IMAGE_COORDINATE_STRU> &vectSrcCell, UINT08 expandVal, UINT32 expandDistt, UINT08 coverVal);

protected:
    inline BOOL    IsImageValid(void) const                                { return ((pucMap_ != NULL) && (mapSizeX_ != 0) && (mapSizeY_ != 0)); }
    inline BOOL    IsCoordValid(INT32 x, INT32 y) const                    { return ((pucMap_ != NULL) && IS_IN_C_O_RANGE(x, 0, INT32(mapSizeX_)) && IS_IN_C_O_RANGE(y, 0, INT32(mapSizeY_))); }
    inline BOOL    IsCoordValid(const IMAGE_COORDINATE_STRU &stCell) const { return IsCoordValid(stCell.x, stCell.y); }
    inline UINT32  GetImageSizeX(void) const                               { return mapSizeX_; }
    inline UINT32  GetImageSizeY(void) const                               { return mapSizeY_; }

    inline UINT08 *GetCellPtrUnsafe(INT32 x, INT32 y) const                           { return &pucMap_[y * mapSizeX_ + x]; };
    inline UINT08 *GetCellPtrUnsafe(const IMAGE_COORDINATE_STRU &stCell) const        { return GetCellPtrUnsafe(stCell.x, stCell.y); }
    inline UINT08 *GetCellPtr(INT32 x, INT32 y) const                                 { return IsCoordValid(x, y) ? GetCellPtrUnsafe(x, y) : nullptr; }
    inline UINT08 *GetCellPtr(const IMAGE_COORDINATE_STRU &stCell) const              { return GetCellPtr(stCell.x, stCell.y); }
    inline UINT08  GetCellValUnsafe(INT32 x, INT32 y) const                           { return *GetCellPtrUnsafe(x, y); }
    inline UINT08  GetCellValUnsafe(const IMAGE_COORDINATE_STRU &stCell) const        { return GetCellValUnsafe(stCell.x, stCell.y); }
    inline INT32   GetCellVal(INT32 x, INT32 y, UINT08 &val) const                    { if (!IsCoordValid(x, y)) { return -1; } val = GetCellValUnsafe(x, y); return 0; }
    inline INT32   GetCellVal(const IMAGE_COORDINATE_STRU &stCell, UINT08 &val) const { return GetCellVal(stCell.x, stCell.y, val); }

    inline void    SetCellUnsafe(INT32 x, INT32 y, UINT08 val)                    { *GetCellPtrUnsafe(x, y) = val; }
    inline void    SetCellUnsafe(const IMAGE_COORDINATE_STRU &stCell, UINT08 val) { SetCellUnsafe(stCell.x, stCell.y, val); }
    inline INT32   SetCell(INT32 x, INT32 y, UINT08 val)                          { if (!IsCoordValid(x, y)) { return -1; } SetCellUnsafe(x, y, val); return 0; }
    inline INT32   SetCell(const IMAGE_COORDINATE_STRU &stCell, UINT08 val)       { return SetCell(stCell.x, stCell.y, val); }

    INT32  GetRightTopCoord(IMAGE_COORDINATE_STRU &stRightTopCoord) const;
    INT32  TransSrcImageCoordToLoc(const IMAGE_COORDINATE_STRU &stSrcImageCoord, IMAGE_COORDINATE_STRU &stLocImageCoord) const;

    void   SetImage(unsigned char setVal);

    INT32  SetLeftAndRightPixelsOfLine(const std::vector<IMAGE_COORDINATE_STRU> &vectLine, UINT08 setLeftVal, UINT08 setRightVal, UINT08 splitVal, UINT32 maxStepLen);


    INT32   GetCellsInArea(UINT08 cellVal, const IMAGE_COORDINATE_STRU &stAreaLBCoord, const IMAGE_COORDINATE_STRU &stAreaSize, std::vector<IMAGE_COORDINATE_STRU> &vectCellList) const;

private:
    unsigned int   mapSizeX_;                           /* 图像X方向大小，即像素列数 */
    unsigned int   mapSizeY_;                           /* 图像Y方向大小，即像素行数 */
    unsigned char *pucMap_;                             /* 图像内存地址 */

    void TurnMapUpsideDown(unsigned char *pucSrcMap, unsigned char *pucTgtMap) const;

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
    GRAY_MAP_C(BOOL isOriginAlignedByResolution = false)
    {
        SetOriginAlignedSw(isOriginAlignedByResolution);
        SetOriginPos(COORDINATE_2D_STRU(0.0, 0.0));
        SetResolution(MAP_RESOLUTION);

        inscribe_num_ = (1.0f / GetResolution()) * (1.0f / GetResolution());
        weak_inscribe_num_ = (1.0f / GetResolution()) * (1.0f / GetResolution());
    }

    virtual ~GRAY_MAP_C(void)
    {
    }

    inline BOOL IsValid(void) const { return IsImageValid(); }

    INT32  GetMapInfo(const string &strMapPath, string &strMapPgmNameFrmYaml, IMAGE_COORDINATE_STRU &stMapSize, COORDINATE_2D_STRU &stOriginPos);
    INT32  GetMapInfo(COORDINATE_2D_STRU &stOriginPos, float &resolution, UINT32 &sizeX, UINT32 &sizeY, UINT08 **ppucMap) const;
    INT32  GetMapSize(COORDINATE_2D_STRU &stSize) const;
    INT32  GetMapArea(AREA_C &objArea) const;
    UINT32 GetDistByCell(float dist) const;

    INT32  TransPictureCoordToPos(const IMAGE_COORDINATE_STRU &stPictureCoord, COORDINATE_2D_STRU &stMapPos) const;
    void   ConvPosToMapCoord(const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const;
    void   ConvMapCoordToPos(const IMAGE_COORDINATE_STRU &stCoord, COORDINATE_2D_STRU &stPos) const;

    static inline UINT08 GetCostOfObst(void)       { return cellCostObst_;      }
    static inline UINT08 GetCostOfWeakObst(void)   { return cellCostWeakObst_;  }
    static inline UINT08 GetCostOfInscribe(void)   { return cellCostInscribe_;  }
    static inline UINT08 GetCostOfNull(void)       { return cellCostNull_;      }
    static inline UINT08 GetCostOfUnknown(void)    { return cellCostUnknown_;   }
    static inline UINT08 GetCostOfBorder(void)     { return cellCostBorder_;    }
    static inline UINT08 GetCostOfOver(void)       { return cellCostOver_;      }
    static inline UINT08 GetCostOfLeft(void)       { return cellCostLeftArea_;  }
    static inline UINT08 GetCostOfRight(void)      { return cellCostRightArea_; }
    static inline UINT08 GetCostOfInside(void)     { return cellCostInside_;    }
    static inline UINT08 GetCostOfOutside(void)    { return cellCostOutside_;   }

    INT32  LoadMap(const string &strMapPath, bool isExistYamlFile = true);
    INT32  SaveMap(const string &strMapPath) const;

    void  CreateMap(const COORDINATE_2D_STRU &stOriginPos, UINT32 mapSizeX, UINT32 mapSizeY, BOOL isDefaultNull = false, float resolution = MAP_RESOLUTION);
    INT32 CreateMap(const COORDINATE_2D_STRU &stOriginPos, UINT32 mapSizeX, UINT32 mapSizeY, UINT08 *pucImage, float resolution = MAP_RESOLUTION);
    INT32 CreateMap(const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint, BOOL isDefaultNull = false, float resolution = MAP_RESOLUTION);
    INT32 CreateMap(UINT32 mapSizeX, UINT32 mapSizeY, UINT08 *pucImage);
    void  CreateMap(UINT32 mapSizeX, UINT32 mapSizeY, UINT08 defaultVal, float resolution = MAP_RESOLUTION);

    INT32 CopyMap(const GRAY_MAP_C &objSrcMap, const COORDINATE_2D_STRU &stLeftButtomPoint, const COORDINATE_2D_STRU &stRightTopPoint);
    INT32 CopyMap(const GRAY_MAP_C &objSrcMap);
    INT32 CopyMapOverlapAreaOfVal(const GRAY_MAP_C &objSrcMap, UINT08 copyVal);

    void  DeleteMap(void);
    void  SwapMap(GRAY_MAP_C &objOtherMap);

    void  SetCellByPos(const COORDINATE_2D_STRU &stPos, unsigned char setVal);
    void  SetCellsByPos(const list<COORDINATE_2D_STRU> &posLst, unsigned char setVal);
    void  SetCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, UINT08 setVal);

    void  SetPassableCellsByPos(const std::vector<COORDINATE_2D_STRU> &vectPos, UINT08 setVal);

    void  SetCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, BOOL isSetEnd);
    void  SetCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint = NULL, UINT32 beginIdx = 0, UINT32 endIdx = UINT32_NULL);
    void  SetCellsOfArea(const std::vector<COORDINATE_2D_STRU> &areaVertexs, UINT08 setVal);

    void  SetPassableCellsOfLineSegment(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint, BOOL isSetEnd);
    void  SetPassableCellsOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, UINT08 setVal, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint = NULL, UINT32 beginIdx = 0, UINT32 endIdx = UINT32_NULL);

    INT32 SetLeftAndRightAreaOfLine(const std::vector<COORDINATE_2D_STRU> &vectLine, float areaDist, UINT32 splitLineSize);
    void  SetMapNull(void);

    BOOL  IsCellsOfLinSegAllNull(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const;
    BOOL  CheckCellsOfLinSegAllNull(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const;
    BOOL  IsCellsOfLinSegIncludeBorder(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const;
    BOOL  IsCellsOfLinSegIncludeObst(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2) const;

    inline float GetResolution(void) const { return imageResolution_; }
    UINT08 GetCellCost(int cellX, int cellY) const;
    UINT08 GetCellByPos(const COORDINATE_2D_STRU &stPos) const;
    INT32  GetOverlapArea(const AREA_C &objOtherArea, AREA_C &objOverlapArea) const;
    INT32  GetOverlapArea(const GRAY_MAP_C &objOtherMap, AREA_C &objOverlapArea) const;
    INT32  GetCellsPosListInArea(const AREA_C &objArea, UINT08 cellCost, std::vector<COORDINATE_2D_STRU> &vectCellsPosList) const;
    INT32  GetFirstObstPointOfLinSeg(const COORDINATE_2D_STRU &stPoint1, const COORDINATE_2D_STRU &stPoint2, COORDINATE_2D_STRU &stObstPoint) const;

    BOOL   IsConnectToMapBorderByNull(const COORDINATE_2D_STRU &stBeginPos, std::vector<IMAGE_COORDINATE_STRU> *pvectSetPoint = NULL) const;

    // 设置膨胀距离
    void SetInflationDis(float dis);

    // 栅格坐标->世界坐标
    void MapToWorld(int mx, int my, float &wx, float &wy) const;

    // 世界坐标->栅格坐标
    bool WorldToMap(float wx, float wy, int &mx, int &my) const;

    void GetXYBounds(float &x_min, float &y_min, float &x_max, float &y_max) const;

    // 填充膨胀
    void FillInflation(void);

protected:
    /* 基本类型 */
    static const UINT08 cellCostObst_            = 0;   /* 这三个数值写死的，可视化时对应了数值                   */
    static const UINT08 cellCostWeakObst_        = 2;
    static const UINT08 cellCostInscribe_        = 253;
    static const UINT08 cellCostNull_            = 254;
    static const UINT08 cellCostUnknown_         = 205;

    static const UINT08 cellCostBorder_          = 10;

    static const UINT08 cellCostOver_            = 1;

    /* 可视化 */
    static const UINT08 cellCostCarBodyTrc_      = 200;

    /* 区域划分 */
    static const UINT08 cellCostRightArea_       = 160; /* 这两个数值写死的，可视化时对应了数值                   */
    static const UINT08 cellCostLeftArea_        = 180;

    static const UINT08 cellCostSplitLine        = 170;
    static const UINT08 cellCostInside_          = 150;
    static const UINT08 cellCostOutside_         = 190;

    UINT08  GetCellCost(const IMAGE_COORDINATE_STRU &stCellCoord) const;
    INT32   GetRightTopPoint(COORDINATE_2D_STRU &stRightTopPoint) const;

    /* 获取指定线段上的栅格，相邻栅格不重复 */
    void    GetCellsOfLineSeg(const COORDINATE_2D_STRU &stBeginPoint, const COORDINATE_2D_STRU &stEndPoint, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const;
    void    GetCellsOfCurve(const std::vector<COORDINATE_2D_STRU> &vectCurvePoint, BOOL isNeedToClose, std::vector<IMAGE_COORDINATE_STRU> &vectCell) const;

private:
    BOOL               isOriginAligned_;        /* 原点是否按照分辨率对齐，也就是原点是分辨率的整倍数。 */
    COORDINATE_2D_STRU stOriginPos_;            /* 原点 */
    float              imageResolution_;        /* 分辨率 */
    float              reciprocalOfRes_;        /* 分辨率倒数 */

    inline void                      SetOriginAlignedSw(BOOL isOriginAligned)            { isOriginAligned_ = isOriginAligned; }
    inline void                      SetOriginPos(const COORDINATE_2D_STRU &stOriginPos) { stOriginPos_ = stOriginPos; }
    inline void                      SetResolution(float resolution)                     { imageResolution_ = resolution; reciprocalOfRes_ = 1.0f / resolution; }
    inline BOOL                      IsOriginAligned(void) const                         { return isOriginAligned_; }
    inline const COORDINATE_2D_STRU &GetOriginPos(void) const                            { return stOriginPos_; }
    inline float                     GetReciprocalOfRes(void) const                      { return reciprocalOfRes_; }

    COORDINATE_2D_STRU AlignOriginPos(const COORDINATE_2D_STRU &stOriginPos, float resolution) const;
    INT32              CalcMapInfo(const COORDINATE_2D_STRU &stLBPoint, const COORDINATE_2D_STRU &stRTPoint, float resolution, COORDINATE_2D_STRU &stOriginPos, IMAGE_COORDINATE_STRU &stMapSize);
    void               ConvPosToMapCoord(const COORDINATE_2D_STRU &stOriginPos, float resolution, const COORDINATE_2D_STRU &stPos, IMAGE_COORDINATE_STRU &stCoord) const;

private:
    // 单元格数据
    struct CellData
    {
        int index;
        int x;
        int y;
        int sx;
        int sy;
        int dis_square;
    };

    // 单元格数据比较器
    struct CellDataComparator
    {
        bool operator()(const CellData &a, const CellData &b)
        {
            return (a.dis_square < b.dis_square);
        }
    };

    int inscribe_num_;
    int weak_inscribe_num_;

    std::vector<bool> seen_;
    std::multiset<CellData, CellDataComparator> inflation_cells_;      // 膨胀单元格
    std::multiset<CellData, CellDataComparator> weak_inflation_cells_; // 虚膨胀单元格
};

/**************************************************************************************
功能描述: 车体轨迹地图
修改记录:
**************************************************************************************/
class TRC_MAP_C : public GRAY_MAP_C
{
public:
    BOOL Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront,
                             const COORDINATE_2D_STRU &stRightFront,
                             const COORDINATE_2D_STRU &stLeftBack,
                             const COORDINATE_2D_STRU &stRightBack) const;

    BOOL Is4SideAreaPassable(const std::vector<COORDINATE_2D_STRU> &vecMainBoby,
                             const std::vector<COORDINATE_2D_STRU> &vecWeakBoby,
                             bool isExpand) const;

    BOOL IsPointOutOfPassableArea(const COORDINATE_2D_STRU &stPoint) const;

    BOOL IsAllAreaPassable(const COORDINATE_2D_STRU &stLeftFront,
                           const COORDINATE_2D_STRU &stRightFront,
                           const COORDINATE_2D_STRU &stLeftBack,
                           const COORDINATE_2D_STRU &stRightBack) const;

    BOOL IsAllAreaPassable(const COORDINATE_2D_STRU &stLeftFront,
                           const COORDINATE_2D_STRU &stRightFront,
                           const COORDINATE_2D_STRU &stLeftBack,
                           const COORDINATE_2D_STRU &stRightBack,
                           bool is_break_collision,
                           std::vector<std::pair<int, int>> &collision_points) const;

    BOOL IsAllAreaPassable(const std::vector<COORDINATE_2D_STRU> &vecMainBoby,
                           const std::vector<COORDINATE_2D_STRU> &vecWeakBoby) const;

    // 检测是否碰撞
    BOOL DetectCollision(const DYNC_CAR_C &dync_car, float side_protect_len, float face_protect_len) const;

    // 检测是否碰撞
    BOOL DetectCollision(const DYNC_CAR_C &dync_car,
                         float side_protect_len,
                         float face_protect_len,
                         std::vector<std::pair<int, int>> &collision_points) const;

    // 检测是否碰撞
    BOOL DetectCollision(const DYNC_CAR_C &dync_car,
                         float side_protect_len,
                         float face_protect_len,
                         bool is_break_collision,
                         std::vector<std::pair<int, int>> &collision_points) const;

    void SetCarBodyArea(const COORDINATE_2D_STRU &stLeftFront,
                        const COORDINATE_2D_STRU &stRightFront,
                        const COORDINATE_2D_STRU &stLeftBack,
                        const COORDINATE_2D_STRU &stRightBack);

    BOOL IsCellObst(int cellX, int cellY) const;
};

/**************************************************************************************
功能描述: 区域地图
          根据给定的区域边界线，生成该区域的地图。
          起点和终点能够自动封闭。
修改记录:
**************************************************************************************/
class AREA_MAP_C : public GRAY_MAP_C
{
public:
    AREA_MAP_C(void) : GRAY_MAP_C(true)
    {
    }

    virtual ~AREA_MAP_C(void)
    {
    }

    INT32 CreateAreaMap(const std::vector<COORDINATE_2D_STRU> &vectAreaBorderPoint, float expandAreaDist = 0.0, float resolution = MAP_RESOLUTION);

    BOOL  IsInArea(const COORDINATE_2D_STRU &stPoint) const;

private:
    INT32 CreateMapByBorder(const std::vector<COORDINATE_2D_STRU> &vectBorderPoint, float expandAreaDist, float resolution);

    BOOL  IsCellNearClassifiedCells(const IMAGE_COORDINATE_STRU &stCell, BOOL &isCellNearInsideCells) const;
    BOOL  IsCellInAreaByBorder(const std::vector<IMAGE_COORDINATE_STRU> &vectBorderCell, const IMAGE_COORDINATE_STRU &stCell) const;
    BOOL  IsCellInArea(const std::vector<IMAGE_COORDINATE_STRU> &vectBorderCell, const IMAGE_COORDINATE_STRU &stCell) const;
    INT32 GetCellsNearBorder(const std::vector<IMAGE_COORDINATE_STRU> &vectBorderCell, std::vector<IMAGE_COORDINATE_STRU> &vectCellInAera, std::vector<IMAGE_COORDINATE_STRU> &vectCellOutAera);
    INT32 SetCellsInArea(const std::vector<IMAGE_COORDINATE_STRU> &vectCellNearBorderInAera);
    INT32 SetCellsExpand(const std::vector<IMAGE_COORDINATE_STRU> &vectCellNearBorderOutAera, float expandAreaDist);
    INT32 AddAreaWithBorder(const std::vector<COORDINATE_2D_STRU> &vectBorderPoint, float expandAreaDist);
};

/**************************************************************************************
功能描述: 规划覆盖地图
修改记录:
**************************************************************************************/
class COVER_MAP_C : public GRAY_MAP_C
{
public:
    INT32  MarkGridsExpand(const COORDINATE_2D_STRU &stCarPos, UINT08 value, std::vector<IMAGE_COORDINATE_STRU> *pVectMarginPoint = nullptr);
    INT32  MarkGridsInArea(const std::vector<COORDINATE_2D_STRU> &vertexList);
    INT32  MarkPassableArea(UINT08 setVal);
    INT32  MarkMargin();
    INT32  GetPoints(UINT08 value, std::vector<COORDINATE_2D_STRU> &vectPoint) const;

    INT32  CopyPassableArea(COVER_MAP_C &objMap) const;
    BOOL   Is4SideAreaPassable(const COORDINATE_2D_STRU &stLeftFront, const COORDINATE_2D_STRU &stRightFront, const COORDINATE_2D_STRU &stLeftBack, const COORDINATE_2D_STRU &stRightBack) const;
    BOOL   IsRouteInPassableArea(vector<COORDINATE_2D_STRU> &vecRoute, UINT32 &firstIdxInArea) const;

private:

    INT32  SearchSeedPoint(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, IMAGE_COORDINATE_STRU &seedPoint) const;
    BOOL   IsPointInPolygon(const std::vector<IMAGE_COORDINATE_STRU> &vertexCoordList, const IMAGE_COORDINATE_STRU &seedPoint) const;
    INT32  CreateSuccessor(const IMAGE_COORDINATE_STRU &stCoordCur, UINT32 i, IMAGE_COORDINATE_STRU &stCoordSucc) const;

    BOOL   IsMarginPoint(UINT32 sizeX, UINT32 sizeY, const IMAGE_COORDINATE_STRU &stPoint) const;

    // possible directions
    const UINT32 dirLimited_ = 4;
    const UINT32 dirAll_     = 8;

    // possible movements
    const INT32  dx_[8] = {-1, 0, 0, 1, 1, 1, -1, -1};
    const INT32  dy_[8] = {0, -1, 1, 0, 1, -1, 1, -1};
};

#endif

