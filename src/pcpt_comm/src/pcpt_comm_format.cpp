#include "pcpt_comm/pcpt_comm_format.h"

/**************************************************************************************
功能描述: 设置
修改记录:
**************************************************************************************/
int PCPT_GRID_C::Reset(const float minX, const float maxX, const float minY, const float maxY, const float gridSize)
{
    if (minX >= maxX || minY >= maxY || gridSize <= 0) {
        ST_LOG_ERR("Rectangle params error.");
        isValid_ = false;
    } else {
        minX_ = minX;
        maxX_ = maxX;
        minY_ = minY;
        maxY_ = maxY;

        gridSize_    = gridSize;
        gridSizeInv_ = 1 / gridSize;
        gridNumX_    = int((maxX - minX) / gridSize + 0.5);
        gridNumY_    = int((maxY - minY) / gridSize + 0.5);
        gridNumXY_   = gridNumX_* gridNumY_;

        isValid_ = true;
    }

    return isValid_ ? 0 : -1;
}

int PCPT_GRID_C::Reset(const RECTANGLE_STRU& stRectangle, const float gridSize)
{
    return Reset(stRectangle.minX, stRectangle.maxX, stRectangle.minY, stRectangle.maxY, gridSize);
}

/**************************************************************************************
功能描述: 计算栅格坐标，点在栅格范围内，返回栅格索引，否则返回-1
修改记录:
**************************************************************************************/
int PCPT_GRID_C::GetGridIndex(const float x, const float y) const
{
    int gridX, gridY;
    if (GetGridXYIndex(x, y, gridX, gridY)) return -1;
    else return gridY * gridNumX_ + gridX;
}

int PCPT_GRID_C::GetGridIndex(const int gridX, const int gridY) const
{
    if (!IsInRectangle(gridX, gridY)) return -1;
    return gridY * gridNumX_ + gridX;
}

int PCPT_GRID_C::GetGridIndex(const COORDINATE_2D_STRU& stCoor) const
{
    return GetGridIndex(stCoor.x, stCoor.y);
}

int PCPT_GRID_C::GetGridXYIndex(const float x, const float y, int &gridX, int &gridY) const
{
    if (!IsInRectangle(x, y)) return -1;
    gridX = int((x - minX_) * gridSizeInv_);
    gridY = int((y - minY_) * gridSizeInv_);
    if (!IsInRectangle(gridX, gridY)) return -1;
    return 0;
}

int PCPT_GRID_C::GetGridXYIndex(const int &index, int &gridX, int &gridY) const
{
    if (!IsInRectangle(index)) return -1;
    gridX = index % gridNumX_;
    gridY = index / gridNumX_;
    return 0;
}

/**************************************************************************************
功能描述: 计算原栅格索引在目标栅格下的索引值
修改记录:
**************************************************************************************/
int PCPT_GRID_C::GetIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int srcGridX, const int srcGridY) const
{
    float x, y;
    if (GetGridCoor(srcGridX, srcGridY, x, y)) return -1;
    int indexTar = objTarGrid.GetGridIndex(x, y);
    return indexTar;
}

int PCPT_GRID_C::GetIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int& srcIndex) const
{
    int srcGridX, srcGridY;
    if (GetGridXYIndex(srcIndex, srcGridX, srcGridY)) return -1;
    return GetIndexByTarGrid(objTarGrid, srcGridX, srcGridY);
}

int PCPT_GRID_C::GetXYIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int srcGridX, const int srcGridY, int &gridX, int &gridY) const
{
    int index = GetIndexByTarGrid(objTarGrid, srcGridX, srcGridY);
    return GetGridXYIndex(index, gridX, gridY);
}

int PCPT_GRID_C::GetXYIndexByTarGrid(PCPT_GRID_C& objTarGrid, const int& srcIndex, int &gridX, int &gridY) const
{
    int index = GetIndexByTarGrid(objTarGrid, srcIndex);
    return GetGridXYIndex(index, gridX, gridY);
}

/**************************************************************************************
功能描述: 计算栅格坐标
修改记录:
**************************************************************************************/
int PCPT_GRID_C::GetGridCoor(const int gridX, const int gridY, float &x, float &y) const
{
    if (!IsInRectangle(gridX, gridY)) return -1;
    x = (gridX + 0.5) * gridSize_ + minX_;
    y = (gridY + 0.5) * gridSize_ + minY_;
    return 0;
}

int PCPT_GRID_C::GetGridCoor(const int index, float &x, float &y) const
{
    int gridX, gridY;
    if (GetGridXYIndex(index, gridX, gridY)) return -1;
    return GetGridCoor(gridX, gridY, x, y);
}

/**************************************************************************************
功能描述: 激光点是否在低矮障碍维护区域内
修改记录:
**************************************************************************************/
bool PCPT_GRID_C::IsInRectangle(const int gridX, const int gridY) const
{
    if (gridX < 0 || gridX >= gridNumX_ ||
        gridY < 0 || gridY >= gridNumY_)
        return false;
    return true;
}

bool PCPT_GRID_C::IsInRectangle(const int index) const
{
    return (index >= 0 && index < gridNumXY_);
}

bool PCPT_GRID_C::IsInRectangle(const float x, const float y) const
{
    if (x < minX_ || x >= maxX_ ||
        y < minY_ || y >= maxY_)
        return false;
    return true;
}

bool PCPT_GRID_C::IsInRectangle(const COORDINATE_2D_STRU& stCoor) const
{
    return IsInRectangle(stCoor.x, stCoor.y);
}

/**************************************************************************************
功能描述: 获取栅格数量
修改记录:
**************************************************************************************/
int PCPT_GRID_C::GetGridNumX() const
{
    return gridNumX_;
}

int PCPT_GRID_C::GetGridNumY() const
{
    return gridNumY_;
}

int PCPT_GRID_C::GetGridNum() const
{
    return gridNumXY_;
}

/**************************************************************************************
功能描述: 获取邻近栅格，不包含自身
修改记录:
**************************************************************************************/
void PCPT_GRID_C::GetNeighborGrid(const int gridX, const int gridY, const int steps, vector<int>*& pNeighborIndex) const
{
    vNeighborIndex_.clear();
     for(int y = 0 - steps; y <= steps; ++y) {
        int indexY = gridY + y;
        if (indexY < 0 || indexY >= gridNumY_) continue;
        for(int x = 0 - steps; x <= steps; ++x) {
            int indexX = gridX + x;
            if (indexX < 0 || indexX >= gridNumX_) continue;
            if (x == 0 && y == 0) continue; //Remove self
            //if ((x * x + y * y) > (steps + 1) * (steps + 1)) continue;
            int neighborIndex = GetGridIndex(indexX, indexY);
            if (neighborIndex != -1) vNeighborIndex_.push_back(neighborIndex);
        }
    }
    pNeighborIndex = &vNeighborIndex_;
}

int PCPT_GRID_C::GetNeighborGrid(const int &index, const int steps, vector<int>*& pNeighborIndex) const
{
    int gridX, gridY;
    if (GetGridXYIndex(index, gridX, gridY)) return -1;
    GetNeighborGrid(gridX, gridY, steps, pNeighborIndex);
    return 0;
}

int PCPT_GRID_C::GetNeighborGrid(const int &index, const float radius, vector<int>*& pNeighborIndex) const
{
    int steps = round(radius * gridSizeInv_);
    return GetNeighborGrid(index, steps, pNeighborIndex);
}

int PCPT_GRID_C::GetNeighborGrid(const float x, const float y, const int steps, vector<int>*& pNeighborIndex) const
{
    int gridX, gridY;
    if (GetGridXYIndex(x, y, gridX, gridY)) return -1;
    GetNeighborGrid(gridX, gridY, steps, pNeighborIndex);
    return 0;
}

int PCPT_GRID_C::GetNeighborGrid(const float x, const float y, const float radius, vector<int>*& pNeighborIndex) const
{
    int steps = round(radius * gridSizeInv_);
    return GetNeighborGrid(x, y, steps, pNeighborIndex);
}

/**************************************************************************************
功能描述: 获取栅格索引差
修改记录:
**************************************************************************************/
void PCPT_GRID_C::GetDifGrid(const float x, const float y, int& difGridX, int& difGridY) const
{
    difGridX = round((x - minX_) * gridSizeInv_);
    difGridY = round((y - minY_) * gridSizeInv_);
}

/**************************************************************************************
功能描述: 拷贝到新栅格对象
修改记录:
**************************************************************************************/
void PCPT_GRID_C::CopyToNewGrid(PCPT_GRID_C& objNewGrid, bool* pOldHitMark, bool* pNewHitMark)
{
    /* 如果新旧栅格对象的栅格分辨率一样，直接进行栅格间的比对，可以减少计算量 */
    if (0 == SIG(gridSize_ - objNewGrid.GridSize())) {
        int difGridX, difGridY;
        objNewGrid.GetDifGrid(minX_, minY_, difGridX, difGridY);

        for (int indexOld = 0; indexOld < GetGridNum(); ++indexOld) {
            int gridX, gridY;
            if (GetGridXYIndex(indexOld, gridX, gridY)) continue;
            gridX += difGridX;
            gridY += difGridY;
            int indexNew = objNewGrid.GetGridIndex(gridX, gridY);
            if (indexNew == -1) continue;
            pNewHitMark[indexNew] = pOldHitMark[indexOld];
        }
    } else {
        for (int indexNew = 0; indexNew < objNewGrid.GetGridNum(); ++indexNew) {
            float x, y;
            if (objNewGrid.GetGridCoor(indexNew, x, y)) continue;
            int indexOld = GetGridIndex(x, y);
            if (indexOld == -1) continue;
            pNewHitMark[indexNew] = pOldHitMark[indexOld];
        }
    }
}

/**************************************************************************************
功能描述: 设置
修改记录:
**************************************************************************************/
int PCPT_VOXEL_C::Reset(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ, const float voxelSize)
{
    if (minX >= maxX || minY >= maxY || minZ >= maxZ || voxelSize <= 0) {
        ST_LOG_ERR("Cuboid Params Error, Init Fail.");
        isValid_ = false;
    } else {
        minX_ = minX;
        maxX_ = maxX;
        minY_ = minY;
        maxY_ = maxY;
        minZ_ = minZ;
        maxZ_ = maxZ;

        voxelSize_    = voxelSize;
        voxelSizeInv_ = 1 / voxelSize;
        voxelNumX_    = round((maxX - minX) * voxelSizeInv_);
        voxelNumY_    = round((maxY - minY) * voxelSizeInv_);
        voxelNumZ_    = round((maxZ - minZ) * voxelSizeInv_);
        voxelNumXY_   = voxelNumX_ * voxelNumY_;
        voxelNumXYZ_  = voxelNumX_ * voxelNumY_ * voxelNumZ_;

        isValid_ = true;
    }

    return isValid_ ? 0 : -1;
}

int PCPT_VOXEL_C::Reset(const CUBOID_STRU &stCuboid, const float voxelSize)
{
    return Reset(stCuboid.minX, stCuboid.maxX, stCuboid.minY, stCuboid.maxY, stCuboid.minZ, stCuboid.maxZ, voxelSize);
}

/**************************************************************************************
功能描述: 是否在体素空间内
修改记录:
**************************************************************************************/
bool PCPT_VOXEL_C::IsInCuboid(const int voxelX, const int voxelY, const int voxelZ) const
{
    if (voxelX < 0 || voxelX >= voxelNumX_ ||
        voxelY < 0 || voxelY >= voxelNumY_ ||
        voxelZ < 0 || voxelZ >= voxelNumZ_ )
        return false;

    return true;
}

bool PCPT_VOXEL_C::IsInCuboid(const VOXEL_COORDINATE_STRU &stVoxelCoor) const
{
    return IsInCuboid(stVoxelCoor.x, stVoxelCoor.y, stVoxelCoor.z);
}

bool PCPT_VOXEL_C::IsInCuboid(const int index) const
{
    return (index >= 0 && index < voxelNumXYZ_);
}

bool PCPT_VOXEL_C::IsInCuboid(const float x, const float y, const float z) const
{
    if (x < minX_ || x >= maxX_ ||
        y < minY_ || y >= maxY_ ||
        z < minZ_ || z >= maxZ_ )
        return false;

    return true;
}

bool PCPT_VOXEL_C::IsInCuboid(const COORDINATE_3D_STRU& stCoor) const
{
    return IsInCuboid(stCoor.x, stCoor.y, stCoor.z);
}

/**************************************************************************************
功能描述: 获取体素索引
修改记录:
**************************************************************************************/
int PCPT_VOXEL_C::GetVoxelIndex(const int voxelX, const int voxelY, const int voxelZ) const
{
    if (!IsInCuboid(voxelX, voxelY, voxelZ)) return -1;
    return voxelZ * voxelNumXY_ + voxelY * voxelNumX_ + voxelX;
}

int PCPT_VOXEL_C::GetVoxelIndex(const float x, const float y, const float z) const
{
    int voxelX = int((x - minX_) * voxelSizeInv_);
    int voxelY = int((y - minY_) * voxelSizeInv_);
    int voxelZ = int((z - minZ_) * voxelSizeInv_);
    return GetVoxelIndex(voxelX, voxelY, voxelZ);
}

int PCPT_VOXEL_C::GetVoxelIndex(const VOXEL_COORDINATE_STRU &stVoxelCoor) const
{
    return GetVoxelIndex(stVoxelCoor.x, stVoxelCoor.y, stVoxelCoor.z);
}

int PCPT_VOXEL_C::GetVoxelIndex(const COORDINATE_3D_STRU &stCoor) const
{
    return GetVoxelIndex(stCoor.x, stCoor.y, stCoor.z);
}

/**************************************************************************************
功能描述: 计算体素坐标
修改记录:
**************************************************************************************/
int PCPT_VOXEL_C::GetVoxelXYZIndex(const int index, VOXEL_COORDINATE_STRU &stVoxelCoor) const
{
    if (!IsInCuboid(index)) return -1;

    stVoxelCoor.z = index / voxelNumXY_;
    stVoxelCoor.y = index % voxelNumXY_ / voxelNumX_;
    stVoxelCoor.x = index % voxelNumX_;

    return 0;
}

int PCPT_VOXEL_C::GetVoxelXYZIndex(const float x, const float y, const float z, VOXEL_COORDINATE_STRU &stVoxelCoor) const
{
    if (!IsInCuboid(x, y, z)) return -1;

    stVoxelCoor.x = int((x - minX_) * voxelSizeInv_);
    stVoxelCoor.y = int((y - minY_) * voxelSizeInv_);
    stVoxelCoor.z = int((z - minZ_) * voxelSizeInv_);

    if (!IsInCuboid(stVoxelCoor.x, stVoxelCoor.y, stVoxelCoor.z)) return -1;
    return 0;
}

/**************************************************************************************
功能描述: 获取体素数量
修改记录:
**************************************************************************************/
int PCPT_VOXEL_C::GetVoxelNumX() const
{
    return voxelNumX_;
}

int PCPT_VOXEL_C::GetVoxelNumY() const
{
    return voxelNumY_;
}

int PCPT_VOXEL_C::GetVoxelNumZ() const
{
    return voxelNumZ_;
}

int PCPT_VOXEL_C::GetVoxelNumXY() const
{
    return voxelNumXY_;
}

int PCPT_VOXEL_C::GetVoxelNum() const
{
    return voxelNumXYZ_;
}

/**************************************************************************************
功能描述: 获取所有邻近体素
修改记录:
**************************************************************************************/
int PCPT_VOXEL_C::GetNeighborVoxel(const int voxelX, const int voxelY, const int voxelZ, const int steps, vector<int>*& pNeighborIndex) const
{
    if (!IsInCuboid(voxelX, voxelY, voxelZ)) return -1;

    vNeighborIndex_.clear();
    for (int z = 0 - steps; z <= steps; ++z) {
        int indexZ = voxelZ + z;
        if (indexZ < 0 || indexZ >= voxelNumZ_)
            continue;

        for (int y = 0 - steps; y <= steps; ++y) {
            int indexY = voxelY + y;
            if (indexY < 0 || indexY >= voxelNumY_)
                continue;

            for (int x = 0 - steps; x <= steps; ++x) {
                int indexX = voxelX + x;
                if (indexX < 0 || indexX >= voxelNumX_)
                    continue;

                if (x == 0 && y == 0 && z == 0)     //Remove self
                    continue;

                //if ((x * x + y * y + z * z) > (steps + 1) * (steps + 1)) continue;

                int neighborIndex = GetVoxelIndex(indexX, indexY, indexZ);
                if (neighborIndex == -1) continue;
                vNeighborIndex_.push_back(neighborIndex);
            }
        }
    }

    pNeighborIndex = &vNeighborIndex_;
    return 0;
}

int PCPT_VOXEL_C::GetNeighborVoxel(const VOXEL_COORDINATE_STRU &stVoxelCoor, const int steps, vector<int>*& pNeighborIndex) const
{
    return GetNeighborVoxel(stVoxelCoor.x, stVoxelCoor.y, stVoxelCoor.z, steps, pNeighborIndex);
}

int PCPT_VOXEL_C::GetNeighborVoxel(const float x, const float y, const float z, const int steps, vector<int>*& pNeighborIndex) const
{
    VOXEL_COORDINATE_STRU stVoxelCoor;
    if (GetVoxelXYZIndex(x, y, z, stVoxelCoor)) return -1;
    return GetNeighborVoxel(stVoxelCoor.x, stVoxelCoor.y, stVoxelCoor.z, steps, pNeighborIndex);
}

int PCPT_VOXEL_C::GetNeighborVoxel(const int voxelIndex, const int steps, vector<int>*& pNeighborIndex) const
{
    VOXEL_COORDINATE_STRU stVoxelCoor;
    if (GetVoxelXYZIndex(voxelIndex, stVoxelCoor)) return -1;
    return GetNeighborVoxel(stVoxelCoor.x, stVoxelCoor.y, stVoxelCoor.z, steps, pNeighborIndex);
}

