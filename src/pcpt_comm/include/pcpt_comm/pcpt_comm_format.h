#ifndef __PCPT_COMM_FORMAT_H__
#define __PCPT_COMM_FORMAT_H__

#include "comm/comm_base.h"
#include "comm_extend/comm_base_extend.h"
#include "pcpt_comm/pcpt_comm_define.h"

using namespace std;

/**
 * @brief 栅格索引结构
 *
 */
struct GRID_STRU {
    GRID_STRU(){}
    GRID_STRU(const int x, const int y) {
        this->x = x;
        this->y = y;
    }

    int x;
    int y;
};

/**
 * @brief 体素索引结构
 *
 */
struct VOXEL_STRU {
    VOXEL_STRU(){}
    VOXEL_STRU(const int x, const int y, const int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    int x;
    int y;
    int z;
};

/**
 * @brief 体柱索引结构
 *
 */
struct PILLAR_STRU {
    PILLAR_STRU(){}
    PILLAR_STRU(const int x, const int y) {
        this->x = x;
        this->y = y;
    }

    int x;
    int y;
};

/**************************************************************************************
功能描述: 栅格化
修改记录: 2D栅格离散器
**************************************************************************************/
class PCPT_GRID_C
{
public:
    PCPT_GRID_C(){}
    PCPT_GRID_C(const RECTANGLE_STRU &stRectangle, const float gridSize)
    {
        Reset(stRectangle, gridSize);
    }

    bool IsValid() {return isValid_;}

    int  Reset(const float minX, const float maxX, const float minY, const float maxY, const float gridSize);
    int  Reset(const RECTANGLE_STRU &stRectangle, const float gridSize);

    int  GetGridIndex(const float x, const float y) const;
    int  GetGridIndex(const int gridX, const int gridY) const;
    int  GetGridIndex(const COORDINATE_2D_STRU& stCoor) const;
    int  GetGridXYIndex(const float x, const float y, int &gridX, int &gridY) const;
    int  GetGridXYIndex(const int &index, int &gridX, int &gridY) const;

    int  GetIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int srcGridX, const int srcGridY) const;
    int  GetIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int& srcIndex) const;
    int  GetXYIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int srcGridX, const int srcGridY, int &gridX, int &gridY) const;
    int  GetXYIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int& srcIndex, int &gridX, int &gridY) const;

    int  GetGridCoor(const int gridX, const int gridY, float &x, float &y) const;
    int  GetGridCoor(const int index, float &x, float &y) const;

    bool IsInRectangle(const int gridX, const int gridY) const;
    bool IsInRectangle(const int index) const;
    bool IsInRectangle(const float x, const float y) const;
    bool IsInRectangle(const COORDINATE_2D_STRU& stCoor) const;

    int  GetGridNumX() const;
    int  GetGridNumY() const;
    int  GetGridNum() const;

    void GetNeighborGrid(const int gridX, const int gridY, const int steps, vector<int>*& pNeighborIndex) const;
    int  GetNeighborGrid(const int &index, const int steps, vector<int>*& pNeighborIndex) const;
    int  GetNeighborGrid(const int &index, const float radius, vector<int>*& pNeighborIndex) const;
    int  GetNeighborGrid(const float x, const float y, const int steps, vector<int>*& pNeighborIndex) const;
    int  GetNeighborGrid(const float x, const float y, const float radius, vector<int>*& pNeighborIndex) const;

    float GridSize() const {return gridSize_;}
    float LengthX() const {return maxX_ - minX_;}
    float LengthY() const {return maxY_ - minY_;}
    float MinX() const {return minX_;}
    float MaxX() const {return maxX_;}
    float MinY() const {return minY_;}
    float MaxY() const {return maxY_;}

    void GetDifGrid(const float x, const float y, int& difGridX, int& difGridY) const;
    void CopyToNewGrid(PCPT_GRID_C& objNewGrid, bool* pOldHitMark, bool* pNewHitMark);

private:
    bool  isValid_{false};
    float gridSize_;
    float gridSizeInv_;
    float minX_, minY_;
    float maxX_, maxY_;
    int   gridNumX_{0};
    int   gridNumY_{0};
    int   gridNumXY_{0};
    mutable vector<int> vNeighborIndex_;  // 临近点索引
};

/**************************************************************************************
功能描述: 体素化
修改记录:
**************************************************************************************/
class PCPT_VOXEL_C
{
public:
    PCPT_VOXEL_C(){}
    PCPT_VOXEL_C(const CUBOID_STRU &stCuboid, const float voxelSize)
    {
        Reset(stCuboid, voxelSize);
    }

    bool IsValid() {return isValid_;}

    int  Reset(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ, const float voxelSize);
    int  Reset(const CUBOID_STRU &stCuboid, const float voxelSize);

    bool IsInCuboid(const int voxelX, const int voxelY, const int voxelZ) const;
    bool IsInCuboid(const VOXEL_COORDINATE_STRU &stVoxelCoor) const;
    bool IsInCuboid(const int index) const;
    bool IsInCuboid(const float x, const float y, const float z) const;
    bool IsInCuboid(const COORDINATE_3D_STRU& stCoor) const;

    int  GetVoxelIndex(const int voxelX, const int voxelY, const int voxelZ) const;
    int  GetVoxelIndex(const float x, const float y, const float z) const;
    int  GetVoxelIndex(const VOXEL_COORDINATE_STRU &stVoxelCoor) const;
    int  GetVoxelIndex(const COORDINATE_3D_STRU &stCoor) const;

    int  GetVoxelXYZIndex(const int index, VOXEL_COORDINATE_STRU &stVoxelCoor) const;
    int  GetVoxelXYZIndex(const float x, const float y, const float z, VOXEL_COORDINATE_STRU &stVoxelCoor) const;

    int  GetVoxelNumX() const;
    int  GetVoxelNumY() const;
    int  GetVoxelNumZ() const;
    int  GetVoxelNumXY() const;
    int  GetVoxelNum() const;

    int  GetNeighborVoxel(const int voxelX, const int voxelY, const int voxelZ, const int steps, vector<int>*& pNeighborIndex) const;
    int  GetNeighborVoxel(const float x, const float y, const float z, const int steps, vector<int>*& pNeighborIndex) const;
    int  GetNeighborVoxel(const int vexelIndex, const int steps, vector<int>*& pNeighborIndex) const;
    int  GetNeighborVoxel(const VOXEL_COORDINATE_STRU &stVoxelCoor, const int steps, vector<int>*& pNeighborIndex) const;

    float VoxelSize() const {return voxelSize_;}
    float LengthX() const {return maxX_ - minX_;}
    float LengthY() const {return maxY_ - minY_;}
    float LengthZ() const {return maxZ_ - minZ_;}
    float MinX() const {return minX_;}
    float MaxX() const {return maxX_;}
    float MinY() const {return minY_;}
    float MaxY() const {return maxY_;}
    float MinZ() const {return minZ_;}
    float MaxZ() const {return maxZ_;}

private:
    bool  isValid_{false};
    float voxelSize_;
    float voxelSizeInv_;
    float minX_, minY_, minZ_;
    float maxX_, maxY_, maxZ_;
    int   voxelNumX_{0}, voxelNumY_{0}, voxelNumZ_{0};
    int   voxelNumXY_{0}, voxelNumXYZ_{0};
    mutable vector<int> vNeighborIndex_;  // 临近点索引
};

/**************************************************************************************
功能描述: 体柱化
修改记录:
**************************************************************************************/
class PCPT_PILLAR_C
{
public:
    PCPT_PILLAR_C(const CUBOID_STRU &stCuboid, const float pillarSize)
    : objGrid(RECTANGLE_STRU(stCuboid.minX, stCuboid.maxX, stCuboid.minY, stCuboid.maxY), pillarSize)
    , pillarSize_(pillarSize)
    {}

private:
    PCPT_GRID_C objGrid;
    float       pillarSize_;

};

#endif
