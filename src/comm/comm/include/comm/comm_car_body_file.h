#ifndef __COMM_CAR_BODY_FILE_H__
#define __COMM_CAR_BODY_FILE_H__

#include "comm/comm_base.h"
#include "comm/comm_car.h"
#include "comm/comm_file.h"

#include <pcl/io/pcd_io.h>
#include <opencv2/opencv.hpp>

/**
 * @brief 车身体素过滤器状态
 *
 */
enum CAR_BODY_VOXEL_FILTER_ENUM {
    CAR_BODY_VOXEL_FILTER_NEW       = 0,     //全量构建
    CAR_BODY_VOXEL_FILTER_ADD       = 1,     //增量构建
    CAR_BODY_VOXEL_FILTER_END_SAVE  = 2,     //保存并启用
    CAR_BODY_VOXEL_FILTER_CANCEL    = 3,     //取消构建
    CAR_BODY_VOXEL_FILTER_END       = 4,     //结束
    CAR_BODY_VOXEL_FILTER_SAVE      = 5,     //结束
    CAR_BODY_VOXEL_FILTER_NULL      = 100,   //为空
};

/**
 * @brief 物理坐标原点在体素空间中的坐标结构
 *
 */
struct VXL_ORIGIN_STRU {
    int vxlX;
    int vxlY;
    int vxlZ;
};

/**
 * @brief 车身体素过滤参数
 *
 */
struct CAR_BODY_VXL_STRU {
    CAR_TYPE_ENUM   enCarType;  //车辆类型
    const char*     pcCarType;
    float           voxel;
    float           minX;
    float           maxX;
    float           minY;
    float           maxY;
    float           minZ;
    float           maxZ;
};

struct CAR_VXL_STRU {
    CAR_VXL_STRU() {}
    CAR_VXL_STRU(const float vxlCell, const CUBOID_STRU& stCuboid) {Set(vxlCell, stCuboid);}

    /* 设置参数 */
    void Set(const float vxlCell, const CUBOID_STRU& stCuboid) {
        this->vxlCell = vxlCell;
        vxlCellInv = 1 / vxlCell;
        RoundParma(vxlCell, stCuboid.minX, vxlMinX);
        RoundParma(vxlCell, stCuboid.minY, vxlMinY);
        RoundParma(vxlCell, stCuboid.minZ, vxlMinZ);
        RoundParma(vxlCell, stCuboid.maxX, vxlMaxX);
        RoundParma(vxlCell, stCuboid.maxY, vxlMaxY);
        RoundParma(vxlCell, stCuboid.maxZ, vxlMaxZ);
        vxlSizeX = vxlMaxX - vxlMinX;
        vxlSizeY = vxlMaxY - vxlMinY;
        vxlSizeZ = vxlMaxZ - vxlMinZ;
    }

    /* 参数圆整 */
    void RoundParma(const float vxlCell, float length, int& voxel) {
#if 0   //正确操作
        if (length < 0) voxel = FLOOR(length / vxlCell, 1e-4);
        else            voxel =  CEIL(length / vxlCell, 1e-4);
#else   //错误操作，但将错就错，无影响
        if (length < 0) voxel = int(floor(length / vxlCell));
        else            voxel = int( ceil(length / vxlCell));
#endif
    }

    bool IsIn(const int indexX, const int indexY, const int indexZ) const {
        if (indexX < 0 || indexX >= vxlSizeX) return false;
        if (indexY < 0 || indexY >= vxlSizeY) return false;
        if (indexZ < 0 || indexZ >= vxlSizeZ) return false;
        return true;
    }

    float vxlCell{0};
    float vxlCellInv{0};

    int   vxlMinX{0};
    int   vxlMinY{0};
    int   vxlMinZ{0};
    int   vxlMaxX{0};
    int   vxlMaxY{0};
    int   vxlMaxZ{0};

    int   vxlSizeX{0};
    int   vxlSizeY{0};
    int   vxlSizeZ{0};
};

struct CAR_BODY_VXL_VERIFY_STRU {
    bool isIn(const float x, const float y, const float z) {
        for (const CUBOID_STRU& stCuboid : vCarBodySpace)
            if (stCuboid.IsInCubiod(x, y, z)) return true;
        return false;
    }

    void push_back(const CUBOID_STRU& stCuboid) {vCarBodySpace.push_back(stCuboid);}

    uint32_t            outSpaceVxlNumThresh;   //车体外体素数量阈值
    vector<CUBOID_STRU> vCarBodySpace;          //车体空间
};

/**************************************************************************************
功能描述: 车体过滤文件相关操作
修改记录:
**************************************************************************************/
class CAR_BODY_FILE_C
{
public:
    CAR_BODY_FILE_C(const float vxlCell, const CUBOID_STRU& stCarBodyCuboid, const string &strModle = "")
    : strModel_(strModle)
    {
        stCarVxl_.Set(vxlCell, stCarBodyCuboid);
        if (Init()) exit(EXIT_FAILURE);
    }

    ~CAR_BODY_FILE_C()
    {
        Delete3dArray(stCarVxl_, pVoxelCarBodyArr_);
    }

    bool IsCarBodyFilterFileExist();
    int  ReadCarBodyFilterFile();
    int  SaveCarBodyFilterFile(const bool isTemp = false);
    int  BackupCarBodyFilterFile();
    int  MarkVxlCoorWithStepExt(const float x, const float y, const float z, const UINT08 step);
    int  CopyCarBodyFilterdFileToNew(const float vxlCellNew, const CUBOID_STRU& stCuboidNew);
    void VxlIndex(const float x, const float y, const float z, int &indexX, int &indexY, int &indexZ);
    char VxlValue(int indexX, int indexY, int indexZ);
    const CAR_VXL_STRU& GetCarBodyVxlParam() const;

private:
    CFG_FILE_MNG_C                  objCfgFileMng_;             // 配置文件管理器

    string                          strModel_;
    char***                         pVoxelCarBodyArr_{nullptr}; // 体素空间占用缓存
    CAR_VXL_STRU                    stCarVxl_;                  // 车体体素过滤参数
    VXL_ORIGIN_STRU                 stVxlOrigin_;               // 物理坐标原点在体素空间中的坐标
    time_t                          modifyTime_;                // 车体过滤文件修改时间
    CAR_BODY_VXL_VERIFY_STRU        stCarBodyVxlVerify_;        // 车体体素校验

    static std::atomic_bool         isSavePcdFile_;

    int  Init();
    int  InitCarBodyFilterFileTime(time_t& modifyTime);
    int  InitCarBodySpaces(const string& strModel);
    void DeleteTempCarBodyFilterFile(void);
    string GetFilePath() const;
    string GetFileName() const;
    int  ReadNextLine(FILE *pfRead, char *pucLine, int maxLineLen);
    int  ReadFileHead(FILE *pfRead, int &voxelSizeX, int &voxelSizeY, int &voxelSizeZ);

    int  GetCarBodyFilterFileModifyTime(string strFile, time_t& modifyTime);
    bool IsCarBodyFilterFileModified(string strFile);

    void Create3dArray(const CAR_VXL_STRU& stCarVxl, char ***&array);
    void Delete3dArray(const CAR_VXL_STRU& stCarVxl, char ***&array);
    int  Clone3DArray(const CAR_VXL_STRU& stCarVxl, char ***&srcArray, char ***&dstArray);
    int  Add3DArray(const CAR_VXL_STRU& stCarVxl, char ***&srcArray, char ***&dstArray);
    void Copy3DArray(const CAR_VXL_STRU& stCarVxlOld, char ***&arrayOld, const CAR_VXL_STRU& stCarVxlNew, char ***&arrayNew);

    void Vxl2Coor(const CAR_VXL_STRU& stCarVxl, const int vxlX, const int vxlY, const int vxlZ, float &x, float &y, float &z);
    int  Coor2ImgPixel(const float x, const float y, const float vxlCellInv, const RECTANGLE_STRU& stRect,
                       const int row, const int col, int& imgRow, int& imgCol) const;
    void InitImgFile(const RECTANGLE_STRU& stRect, float vxlCellInv, cv::Mat& cvImg, int& row, int& col);
    void SaveCarBodyPcdFile(pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud, const bool isTemp = false);
    void SaveCarBodyImgFile(pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud, const bool isTemp = false);
    int  SaveCarBodyFilterFile(const CAR_VXL_STRU& stCarVxl, char ***&array, bool isFill = true, bool isMerge = true, const bool isTemp = false);
    int  ReadCarBodyFilterFile(const std::string &strFilePath);
    int  MarkVxlCoorWithStepExt(const CAR_VXL_STRU& stCarVxl, const int indexX, const int indexY, const int indexZ, const UINT08 step);
    void VxlIndex(const float x, const float y, const float z, const CAR_VXL_STRU& stCarVxl, int &indexX, int &indexY, int &indexZ);

    int  FillInternalVoxel(const CAR_VXL_STRU& stCarVxl, const VXL_ORIGIN_STRU& stVxlOrigin, char ***&array);
    int  MergeCarBodyFilterFile(string strFile);
};

#endif

