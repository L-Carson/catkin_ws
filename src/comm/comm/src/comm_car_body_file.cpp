#include "comm/comm_car_body_file.h"

std::atomic_bool CAR_BODY_FILE_C::isSavePcdFile_{false};

/**************************************************************************************
功能描述: 判断车身体素滤波文件是否存在
修改记录:
**************************************************************************************/
bool CAR_BODY_FILE_C::IsCarBodyFilterFileExist()
{
    string strFilterFilePath = GetFilePath();

    FILE_OPRT_C objFileOprt;
    return objFileOprt.IsExist(strFilterFilePath);
}

/**************************************************************************************
功能描述: 读取车身体素滤波文件
修改记录: 加载体素空间占用缓存
**************************************************************************************/
int CAR_BODY_FILE_C::ReadCarBodyFilterFile()
{
    string strFilterFilePath = objCfgFileMng_.GetCarBodyVoxelFilterFilePath();

    time_t modifyTime;
    if (GetCarBodyFilterFileModifyTime(strFilterFilePath, modifyTime)) return -1;
    modifyTime_ = modifyTime;

    FILE *pfRead = fopen(strFilterFilePath.c_str(), "rb");
    if (pfRead == NULL) {
        ST_LOG_ERR("打开车体过滤文件失败，请检测文件是否存在：%s", strFilterFilePath.data());
        return -1;
    }

    /* 读取体素滤波数据文件头 */
    int voxelSizeX, voxelSizeY, voxelSizeZ;
    int rslt = ReadFileHead(pfRead, voxelSizeX, voxelSizeY, voxelSizeZ);
    if (rslt != 0) {
        ST_LOG_ERR("读取车体过滤文件头失败：%s.", strFilterFilePath.data());
        fclose(pfRead);
        return -1;
    }

    /* 文件内数据与当前配置不匹配，则数据无效 */
    if (voxelSizeX != stCarVxl_.vxlSizeX ||
        voxelSizeY != stCarVxl_.vxlSizeY ||
        voxelSizeZ != stCarVxl_.vxlSizeZ) {
        ST_LOG_INFO("配置文件(%d %d %d) 过滤文件(%d %d %d)",
                    stCarVxl_.vxlSizeX, stCarVxl_.vxlSizeY, stCarVxl_.vxlSizeZ, voxelSizeX, voxelSizeY, voxelSizeZ);
        ST_LOG_ERR("车体过滤文件头不匹配：%s.", strFilterFilePath.data());
        fclose(pfRead);
        return -1;
    }

    /* 加载体素空间占用缓存，并统计占用体素数量 */
    uint32_t outCarBodyVxlNum = 0;
    uint32_t voxelCellNum = 0;
    char     newLine = '\n';
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZ>);

    for (INT32 vxlZ = 0; vxlZ < voxelSizeZ; ++vxlZ) {
        for (INT32 vxlY = 0; vxlY < voxelSizeY; ++vxlY) {
            if (voxelSizeX != int(fread(pVoxelCarBodyArr_[vxlZ][vxlY], 1, voxelSizeX, pfRead))) {
                ST_LOG_ERR("Read 3D Array Fail.");
                fclose(pfRead);
                return -1;
            }

            if (1 != fread(&newLine, 1, 1, pfRead)) {
                ST_LOG_ERR("Read Line Break Fail.");
                fclose(pfRead);
                return -1;
            }

            for(INT32 vxlX = 0; vxlX < voxelSizeX; ++vxlX) {
                if (!pVoxelCarBodyArr_[vxlZ][vxlY][vxlX]) continue;
                voxelCellNum++;

                /* 转为点云 */
                pcl::PointXYZ point;
                Vxl2Coor(stCarVxl_, vxlX, vxlY, vxlZ, point.x, point.y, point.z);
                pCloud->push_back(point);

                /* 判断是否在车体空间外 */
                if (!stCarBodyVxlVerify_.isIn(point.x, point.y, point.z)) outCarBodyVxlNum++;
            }
        }
    }

    ST_LOG_INFO("Read Success, Car Body Filter Voxel Cells Num:%u", voxelCellNum);
    fclose(pfRead);

    if (outCarBodyVxlNum <= stCarBodyVxlVerify_.outSpaceVxlNumThresh) {
        ST_LOG_INFO("车体空间外体素数量:%u(阈值:%u).", outCarBodyVxlNum, stCarBodyVxlVerify_.outSpaceVxlNumThresh);
    } else {
        ST_LOG_WARN("车体构造结果无效，请重新全量或增量构建。车体外体素数量:%u(阈值:%u).", outCarBodyVxlNum, stCarBodyVxlVerify_.outSpaceVxlNumThresh);
        //return -1;    //TODO: 当前测试阶段，暂不返回-1
    }

    /* 将体素过滤文件保存为pcd、img等文件 */
    if (!isSavePcdFile_) {
        isSavePcdFile_ = true;
        FILE_OPRT_C objFileOprt;

        string strPcdFilePath = objCfgFileMng_.GetCarBodyVoxelPcdFilePath();
        if (!objFileOprt.IsExist(strPcdFilePath)) {
            SaveCarBodyPcdFile(pCloud);
        }

        string strImgFilePath = objCfgFileMng_.GetCarBodyVoxelSideImgFilePath();
        if (!objFileOprt.IsExist(strImgFilePath)) {
            SaveCarBodyImgFile(pCloud);
        }
    }

    return 0;
}

int CAR_BODY_FILE_C::ReadCarBodyFilterFile(const std::string &strFilterFilePath)
{
    time_t modifyTime;
    if (GetCarBodyFilterFileModifyTime(strFilterFilePath, modifyTime)) return -1;
    modifyTime_ = modifyTime;

    FILE *pfRead = fopen(strFilterFilePath.c_str(), "rb");
    if (pfRead == NULL) {
        ST_LOG_ERR("打开车体过滤文件失败，请检测文件是否存在：%s", strFilterFilePath.data());
        return -1;
    }

    /* 读取体素滤波数据文件头 */
    int voxelSizeX, voxelSizeY, voxelSizeZ;
    int rslt = ReadFileHead(pfRead, voxelSizeX, voxelSizeY, voxelSizeZ);
    if (rslt != 0) {
        ST_LOG_ERR("读取车体过滤文件头失败：%s.", strFilterFilePath.data());
        fclose(pfRead);
        return -1;
    }

    /* 文件内数据与当前配置不匹配，则数据无效 */
    if (voxelSizeX != stCarVxl_.vxlSizeX ||
        voxelSizeY != stCarVxl_.vxlSizeY ||
        voxelSizeZ != stCarVxl_.vxlSizeZ) {
        ST_LOG_INFO("配置文件(%d %d %d) 过滤文件(%d %d %d)",
                    stCarVxl_.vxlSizeX, stCarVxl_.vxlSizeY, stCarVxl_.vxlSizeZ, voxelSizeX, voxelSizeY, voxelSizeZ);
        ST_LOG_ERR("车体过滤文件头不匹配：%s.", strFilterFilePath.data());
        fclose(pfRead);
        return -1;
    }

    /* 加载体素空间占用缓存，并统计占用体素数量 */
    uint32_t outCarBodyVxlNum = 0;
    uint32_t voxelCellNum = 0;
    char     newLine = '\n';
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZ>);

    for (INT32 vxlZ = 0; vxlZ < voxelSizeZ; ++vxlZ) {
        for (INT32 vxlY = 0; vxlY < voxelSizeY; ++vxlY) {
            if (voxelSizeX != int(fread(pVoxelCarBodyArr_[vxlZ][vxlY], 1, voxelSizeX, pfRead))) {
                ST_LOG_ERR("Read 3D Array Fail.");
                fclose(pfRead);
                return -1;
            }

            if (1 != fread(&newLine, 1, 1, pfRead)) {
                ST_LOG_ERR("Read Line Break Fail.");
                fclose(pfRead);
                return -1;
            }

            for(INT32 vxlX = 0; vxlX < voxelSizeX; ++vxlX) {
                if (!pVoxelCarBodyArr_[vxlZ][vxlY][vxlX]) continue;
                voxelCellNum++;

                /* 转为点云 */
                pcl::PointXYZ point;
                Vxl2Coor(stCarVxl_, vxlX, vxlY, vxlZ, point.x, point.y, point.z);
                pCloud->push_back(point);

                /* 判断是否在车体空间外 */
                if (!stCarBodyVxlVerify_.isIn(point.x, point.y, point.z)) outCarBodyVxlNum++;
            }
        }
    }

    ST_LOG_INFO("Read Success, Car Body Filter Voxel Cells Num:%u", voxelCellNum);
    fclose(pfRead);

    if (outCarBodyVxlNum <= stCarBodyVxlVerify_.outSpaceVxlNumThresh) {
        ST_LOG_INFO("车体空间外体素数量:%u(阈值:%u).", outCarBodyVxlNum, stCarBodyVxlVerify_.outSpaceVxlNumThresh);
    } else {
        ST_LOG_WARN("车体构造结果无效，请重新全量或增量构建。车体外体素数量:%u(阈值:%u).", outCarBodyVxlNum, stCarBodyVxlVerify_.outSpaceVxlNumThresh);
        //return -1;    //TODO: 当前测试阶段，暂不返回-1
    }

    return 0;
}

/**************************************************************************************
功能描述: 将体素转为坐标点
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::Vxl2Coor(const CAR_VXL_STRU& stCarVxl, const int vxlX, const int vxlY, const int vxlZ, float &x, float &y, float &z)
{
    float halfCell = 0.5 * stCarVxl.vxlCell;
    x = (vxlX + stCarVxl.vxlMinX) * stCarVxl.vxlCell + halfCell;
    y = (vxlY + stCarVxl.vxlMinY) * stCarVxl.vxlCell + halfCell;
    z = (vxlZ + stCarVxl.vxlMinZ) * stCarVxl.vxlCell + halfCell;
}

/**************************************************************************************
功能描述: 将坐标点转为像素
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::Coor2ImgPixel(const float x, const float y, const float vxlCellInv, const RECTANGLE_STRU& stRect,
                                   const int row, const int col, int& imgRow, int& imgCol) const
{
    int gridX, gridY;
    gridX = int((x - stRect.minX) * vxlCellInv);
    gridY = int((y - stRect.minY) * vxlCellInv);

    imgRow = row - gridX - 1;
    imgCol = col - gridY - 1;

    if (imgRow < 0 || imgRow >= row ||
        imgCol < 0 || imgCol >= col) {
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 初始化图片
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::InitImgFile(const RECTANGLE_STRU& stRect, float vxlCellInv, cv::Mat& cvImg, int& row, int& col)
{
    row = ceil((stRect.maxX - stRect.minX) * vxlCellInv);
    col = ceil((stRect.maxY - stRect.minY) * vxlCellInv);
    cvImg = cv::Mat(row, col, CV_8UC1, cv::Scalar(255, 255, 255));

    /* 在画布上画上刻度尺，以车体坐标原点为参考，间隔1米 */
    float spacingDist = 1.0;
#define CAR_BODY_TICK_MARK(scale, axis)                                                 \
    for (int i = 0; i <= int(fabsf(scale / spacingDist)); ++i) {                        \
        COORDINATE_2D_STRU stPoint1, stPoint2;                                          \
        if (axis == 'x') {                                                              \
            stPoint1.SetValue(spacingDist * i * SIGN(scale), stRect.minY + 0.0001);     \
            stPoint2.SetValue(spacingDist * i * SIGN(scale), stRect.maxY - 0.0001);     \
        } else if (axis == 'y') {                                                       \
            stPoint1.SetValue(stRect.minX + 0.0001, spacingDist * i * SIGN(scale));     \
            stPoint2.SetValue(stRect.maxX - 0.0001, spacingDist * i * SIGN(scale));     \
        }                                                                               \
        cv::Point point1, point2;                                                       \
        if (Coor2ImgPixel(stPoint1.x, stPoint1.y, vxlCellInv, stRect, row, col, point1.y, point1.x)) continue;    \
        if (Coor2ImgPixel(stPoint2.x, stPoint2.y, vxlCellInv, stRect, row, col, point2.y, point2.x)) continue;    \
        if (i == 0) {                                                                   \
            cv::line(cvImg, point1, point2, cv::Scalar(200, 200, 200), 1, 4);           \
        } else {                                                                        \
            cv::line(cvImg, point1, point2, cv::Scalar(245, 245, 245), 1, 4);           \
        }                                                                               \
    }                                                                                   \

    CAR_BODY_TICK_MARK(stRect.minX, 'x');
    CAR_BODY_TICK_MARK(stRect.maxX, 'x');
    CAR_BODY_TICK_MARK(stRect.minY, 'y');
    CAR_BODY_TICK_MARK(stRect.maxY, 'y');
}

/**************************************************************************************
功能描述: 保存车身体素点云文件
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::SaveCarBodyPcdFile(pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud, const bool isTemp)
{
    RECTANGLE_STRU stGridRect(-2, 3, -2, 2);    //参考栅格范围

    /* 添加X方向参考栅格 */
    for (float gridY = stGridRect.minY; gridY < stGridRect.maxY + 0.1; gridY += 1.0) {
        for (float gridX = stGridRect.minX; gridX <= stGridRect.maxX; gridX += 0.01) {
            pcl::PointXYZ point;
            point.x = gridX;
            point.y = gridY;
            point.z = 0;
            pCloud->push_back(point);
        }
    }

    /* 添加Y方向参考栅格 */
    for (float gridX = stGridRect.minX; gridX < stGridRect.maxX + 0.1; gridX += 1.0) {
        for (float gridY = stGridRect.minY; gridY <= stGridRect.maxY; gridY += 0.01) {
            pcl::PointXYZ point;
            point.x = gridX;
            point.y = gridY;
            point.z = 0;
            pCloud->push_back(point);
        }
    }

    /* 保存为pcd，方便使用pcl_viewer查看 */
    string strPcdFilePath = isTemp ? objCfgFileMng_.GetCarBodyVoxelPcdTempFilePath()
                                   : objCfgFileMng_.GetCarBodyVoxelPcdFilePath();
    pcl::io::savePCDFileBinary(strPcdFilePath, *pCloud);
    ST_LOG_INFO("Save Pcd File %s", strPcdFilePath.c_str());
}

/**************************************************************************************
功能描述: 保存车身体素图像文件
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::SaveCarBodyImgFile(pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud, const bool isTemp)
{
    /* 设置栅格范围 */
    CUBOID_STRU stBox(-2, 4, -2, 2, -1, 3);                                     //车体box范围
    RECTANGLE_STRU stRectSide(stBox.minZ, stBox.maxZ, stBox.minX, stBox.maxX);  //参考栅格范围 - 侧视图
    RECTANGLE_STRU stRectTop( stBox.minX, stBox.maxX, stBox.minY, stBox.maxY);  //参考栅格范围 - 俯视图
    RECTANGLE_STRU stRectRear(stBox.minZ, stBox.maxZ, stBox.minY, stBox.maxY);  //参考栅格范围 - 后视图

    /* 初始化底图 */
    float vxlCellInv = stCarVxl_.vxlCellInv;
    cv::Mat cvImgSide, cvImgTop, cvImgRear;
    int rowSide, colSide, rowTop, colTop, rowRear, colRear;
    InitImgFile(stRectSide, vxlCellInv, cvImgSide, rowSide, colSide);
    InitImgFile(stRectTop,  vxlCellInv, cvImgTop , rowTop,  colTop);
    InitImgFile(stRectRear, vxlCellInv, cvImgRear, rowRear, colRear);

    /* 车身占用体素描点 */
    for (const auto &point: pCloud->points) {
        int imgRow, imgCol;

        /* 侧视图(z & x) */
        if (0 == Coor2ImgPixel(point.z, point.x, vxlCellInv, stRectSide, rowSide, colSide, imgRow, imgCol)) {
            cvImgSide.at<uint8_t>(imgRow, imgCol) = 100;
        }

        /* 俯视图(x & y) */
        if (0 == Coor2ImgPixel(point.x, point.y, vxlCellInv, stRectTop,  rowTop,  colTop,  imgRow, imgCol)) {
            cvImgTop.at<uint8_t>(imgRow, imgCol) = 100;
        }

        /* 后视图(z & y) */
        if (0 == Coor2ImgPixel(point.z, point.y, vxlCellInv, stRectRear, rowRear, colRear, imgRow, imgCol)) {
            cvImgRear.at<uint8_t>(imgRow, imgCol) = 100;
        }
    }

    /* 对图片进行放大 */
    int times = 3;
    cv::Mat cvImgSideEx(times * rowSide, times * colSide, CV_8UC1);
    cv::Mat cvImgTopEx( times * rowTop,  times * colTop,  CV_8UC1);
    cv::Mat cvImgRearEx(times * rowRear, times * colRear, CV_8UC1);
    cv::resize(cvImgSide, cvImgSideEx, cvImgSideEx.size());
    cv::resize(cvImgTop,  cvImgTopEx,  cvImgTopEx.size());
    cv::resize(cvImgRear, cvImgRearEx, cvImgRearEx.size());

    /* 保存图片 */
    std::string strSideImgFilePath = isTemp ? objCfgFileMng_.GetCarBodyVoxelSideImgTempFilePath()
                                            : objCfgFileMng_.GetCarBodyVoxelSideImgFilePath();
    std::string strTopImgFilePath  = isTemp ? objCfgFileMng_.GetCarBodyVoxelTopImgTempFilePath()
                                            : objCfgFileMng_.GetCarBodyVoxelTopImgFilePath();
    std::string strRearImgFilePath = isTemp ? objCfgFileMng_.GetCarBodyVoxelRearImgTempFilePath()
                                            : objCfgFileMng_.GetCarBodyVoxelRearImgFilePath();
    cv::imwrite(strSideImgFilePath, cvImgSideEx);
    cv::imwrite(strTopImgFilePath,  cvImgTopEx);
    cv::imwrite(strRearImgFilePath, cvImgRearEx);
    ST_LOG_INFO("Save Side Image File %s", strSideImgFilePath.c_str());
    ST_LOG_INFO("Save Top  Image File %s", strTopImgFilePath.c_str());
    ST_LOG_INFO("Save Rear Image File %s", strRearImgFilePath.c_str());
}

/**************************************************************************************
功能描述: 保存车身体素滤波文件
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::SaveCarBodyFilterFile(const CAR_VXL_STRU& stCarVxl, char ***&array, bool isFill, bool isMerge, const bool isTemp)
{
    /* 是否使用填充 */
    if (isFill) {
        /* 内部填充 */
        int numVoxel = FillInternalVoxel(stCarVxl, stVxlOrigin_, array);
        ST_LOG_INFO("Fill Internal Voxel num %d", numVoxel);
    }

    string strFilterFilePath = isTemp ? objCfgFileMng_.GetCarBodyVoxelFilterTempFilePath()
                                      : objCfgFileMng_.GetCarBodyVoxelFilterFilePath();

    /* 是否合并 */
    if (isMerge) {
        /* 合并多雷达节点构建的车体过滤文件 */
        if (MergeCarBodyFilterFile(strFilterFilePath)) {
            ST_LOG_ERR("Failed to merge car body filter file.");
            return -1;
        }
    }

    /* 判断当前构造是否有效 */
    uint32_t outCarBodyVxlNum = 0;
    pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZ>);
    for (int vxlZ = 0; vxlZ < stCarVxl.vxlSizeZ; ++vxlZ) {
        for (int vxlY = 0; vxlY < stCarVxl.vxlSizeY; ++vxlY) {
            for (int vxlX = 0; vxlX < stCarVxl.vxlSizeX; ++vxlX) {
                if (!array[vxlZ][vxlY][vxlX]) continue;
                /* 转为点云 */
                pcl::PointXYZ point;
                Vxl2Coor(stCarVxl, vxlX, vxlY, vxlZ, point.x, point.y, point.z);
                pCloud->push_back(point);

                /* 判断是否在车体空间外 */
                if (!stCarBodyVxlVerify_.isIn(point.x, point.y, point.z)) outCarBodyVxlNum++;
            }
        }
    }

    if (outCarBodyVxlNum <= stCarBodyVxlVerify_.outSpaceVxlNumThresh) {
        ST_LOG_INFO("车体空间外体素数量:%u(阈值:%u).", outCarBodyVxlNum, stCarBodyVxlVerify_.outSpaceVxlNumThresh);
    } else {
        ST_LOG_ERR("车体构造结果无效，请重新全量或增量构建。车体外体素数量:%u(阈值:%u).", outCarBodyVxlNum, stCarBodyVxlVerify_.outSpaceVxlNumThresh);
        //return -1;    //TODO: 当前测试阶段，暂不返回-1
    }

    /* 保存构造结果 */
    FILE *pfSave = fopen(strFilterFilePath.c_str(), "wb");
    if (pfSave == NULL) {
        ST_LOG_ERR("Open %s Fail", strFilterFilePath.data());
        return -1;
    }

    char newLine = '\n';
    char acFileHead[100] = {0};
    sprintf(acFileHead, "Size:\n%d %d %d\n", stCarVxl.vxlSizeX, stCarVxl.vxlSizeY, stCarVxl.vxlSizeZ);

    (void)fwrite(acFileHead, 1, strlen(acFileHead), pfSave);

    for (int vxlZ = 0; vxlZ < stCarVxl.vxlSizeZ; ++vxlZ) {
        for (int vxlY = 0; vxlY < stCarVxl.vxlSizeY; ++vxlY) {
            (void)fwrite(array[vxlZ][vxlY], 1, stCarVxl.vxlSizeX, pfSave);
            (void)fwrite(&newLine, 1, 1, pfSave);
        }
    }

    /* 保存到体素构造文件 */
    fflush(pfSave);
    fsync(fileno(pfSave));
    fclose(pfSave);

    /* 保存到点云文件 */
    SaveCarBodyPcdFile(pCloud, isTemp);

    /* 保存到图像文件 */
    SaveCarBodyImgFile(pCloud, isTemp);

    ST_LOG_INFO("Success to save car body filter result to %s.", strFilterFilePath.c_str());
    return 0;
}

int CAR_BODY_FILE_C::SaveCarBodyFilterFile(const bool isTemp)
{ 
    return SaveCarBodyFilterFile(stCarVxl_, pVoxelCarBodyArr_, true, true, isTemp);
}

/**************************************************************************************
功能描述: 备份车身体素滤波文件
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::BackupCarBodyFilterFile()
{
    string strSrcFilePath = GetFilePath();

    size_t pos = strSrcFilePath.find(".dat");
    if (pos == string::npos) {
        ST_LOG_ERR("无效的车体构造文件: %s.", strSrcFilePath.c_str());
        return -1;
    }

    string strTgtFilePath = strSrcFilePath.substr(0, pos);
    strTgtFilePath += "_Backup_";
    strTgtFilePath += Comm_GetTimeStr2();
    strTgtFilePath += ".dat";

    do {
        pos = strTgtFilePath.find(':');
        if (pos != string::npos) strTgtFilePath[pos] = '_';
    } while (pos != string::npos);

    do {
        pos = strTgtFilePath.find(' ');
        if (pos != string::npos) strTgtFilePath[pos] = '_';
    } while (pos != string::npos);

    FILE_OPRT_C objFileOprt;
    if (objFileOprt.Copy(strSrcFilePath, strTgtFilePath, false)) {
        ST_LOG_ERR("备份车体构造文件失败: %s.", strSrcFilePath.c_str());
        return -1;
    }

    ST_LOG_INFO("完成车体构造文件备份: %s.", strTgtFilePath.c_str());

    return 0;
}

/**************************************************************************************
功能描述: 使用体素单元填充占用
修改记录: 更新体素空间占用缓存
**************************************************************************************/
int CAR_BODY_FILE_C::MarkVxlCoorWithStepExt(const float x, const float y, const float z, const UINT08 step)
{
    int indexX, indexY, indexZ;
    VxlIndex(x, y, z, stCarVxl_, indexX, indexY, indexZ);
    return MarkVxlCoorWithStepExt(stCarVxl_, indexX, indexY, indexZ, step);
}

int CAR_BODY_FILE_C::MarkVxlCoorWithStepExt(const CAR_VXL_STRU& stCarVxl, const int indexX, const int indexY, const int indexZ, const UINT08 step)
{
    int newVxl = 0;
    for(int z=0-step; z<=step; ++z) {
        if (indexZ + z < 0 || indexZ + z >= stCarVxl.vxlSizeZ) continue;

        for(int y=0-step; y<=step; ++y) {
            if (indexY + y < 0 || indexY + y >=stCarVxl.vxlSizeY) continue;

            for(int x=0-step; x<=step; ++x) {
                if (indexX + x < 0 || indexX + x >= stCarVxl.vxlSizeX) continue;

                if (pVoxelCarBodyArr_[indexZ+z][indexY+y][indexX+x] == 0) {
                    pVoxelCarBodyArr_[indexZ+z][indexY+y][indexX+x] = 1;
                    newVxl++;
                }
            }
        }
    }

    return newVxl;
}

/**************************************************************************************
功能描述: 体素值
修改记录:
**************************************************************************************/
char CAR_BODY_FILE_C::VxlValue(int indexX, int indexY, int indexZ)
{
    return pVoxelCarBodyArr_[indexZ][indexY][indexX];
}

/**************************************************************************************
功能描述: 获取车身体素参数
修改记录:
**************************************************************************************/
const CAR_VXL_STRU& CAR_BODY_FILE_C::GetCarBodyVxlParam() const
{
    return stCarVxl_;
}

/**************************************************************************************
功能描述: 计算体素单元格位置 （物理坐标-->体素坐标）
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::VxlIndex(const float x, const float y, const float z,
                               const CAR_VXL_STRU& stCarVxl, int &indexX, int &indexY, int &indexZ)
{
    indexX = int(floor(x * stCarVxl.vxlCellInv)) - stCarVxl.vxlMinX;
    indexY = int(floor(y * stCarVxl.vxlCellInv)) - stCarVxl.vxlMinY;
    indexZ = int(floor(z * stCarVxl.vxlCellInv)) - stCarVxl.vxlMinZ;
}

void CAR_BODY_FILE_C::VxlIndex(const float x, const float y, const float z, int &indexX, int &indexY, int &indexZ)
{
    VxlIndex(x, y, z, stCarVxl_, indexX, indexY, indexZ);
}

/**************************************************************************************
功能描述: 车体过滤文件拷贝至新的参数下
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::CopyCarBodyFilterdFileToNew(const float vxlCellNew, const CUBOID_STRU& stCuboidNew)
{
    if (!IsCarBodyFilterFileExist()) {
        ST_LOG_WARN("车体构造文件不存在，无需拷贝到新的参数体系下。");
        return 0;
    }

    if (ReadCarBodyFilterFile()) return -1;
    if (BackupCarBodyFilterFile()) return -1;

    CAR_VXL_STRU stCarVxlNew(vxlCellNew, stCuboidNew);
    char*** pVoxelCarBodyArrNew{nullptr};     // 新参数下的体素空间占用缓存
    Copy3DArray(stCarVxl_, pVoxelCarBodyArr_, stCarVxlNew, pVoxelCarBodyArrNew);

    SaveCarBodyFilterFile(stCarVxlNew, pVoxelCarBodyArrNew, false, false);
    Delete3dArray(stCarVxlNew, pVoxelCarBodyArrNew);
    //ST_LOG_INFO("Success to copy car body filter file to the new param system.");
    ST_LOG_INFO("成功将车体构造文件拷贝到新的参数体系下。");
    return 0;
}

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::Init()
{
    if (strModel_.empty()) {
        CFG_PARAM_C objCfgParam;
        INT32 rslt = objCfgParam.Init();
        if (rslt != 0) {
            ST_LOG_ERR("objCfgParam.Init() Fail.");
            return -1;
        }
        const CFG_PARAM_STRUCTURE_INFO_STRU &stStructureInfo = objCfgParam.GetStructureInfoCfg();
        strModel_ = stStructureInfo.strModel;
    }

    /* 初始化车体空间（外包多面体） */
    if (InitCarBodySpaces(strModel_)) return -1;

    ST_LOG_INFO("Car body voxel filter params: voxelSize:%f minX:%d maxX:%d minY:%d maxY:%d minZ:%d maxZ:%d.",
                stCarVxl_.vxlCell, stCarVxl_.vxlMinX, stCarVxl_.vxlMaxX,
                stCarVxl_.vxlMinY, stCarVxl_.vxlMaxY, stCarVxl_.vxlMinZ, stCarVxl_.vxlMaxZ);

    /* 计算物理坐标原点在体素空间中的坐标 */
    VxlIndex(0, 0, 0, stCarVxl_, stVxlOrigin_.vxlX, stVxlOrigin_.vxlY, stVxlOrigin_.vxlZ);

    Create3dArray(stCarVxl_, pVoxelCarBodyArr_);

    /* 初始化车体过滤文件时间属性，暂不关心返回值（文件可能不存在） */
    InitCarBodyFilterFileTime(modifyTime_);
    
    DeleteTempCarBodyFilterFile();
    
    return 0;
}

/**************************************************************************************
功能描述: 初始化车体过滤文件时间属性
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::InitCarBodyFilterFileTime(time_t& modifyTime)
{
    string strFilterFilePath = GetFilePath();
    if (GetCarBodyFilterFileModifyTime(strFilterFilePath, modifyTime)) return -1;
    return 0;
}

/**************************************************************************************
功能描述: 初始化车体体素过滤参数
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::InitCarBodySpaces(const string& strModel)
{
    const CAR_BODY_INFO_STRU *pstCarBodyInfo = CAR_BODY_C::GetCarBodyInfo(strModel);
    if (pstCarBodyInfo == nullptr) {
        ST_LOG_ERR("CAR_BODY_C::GetCarBodyInfo(%s) Fail.", strModel.c_str());
        return -1;
    }

    float tailLen       = pstCarBodyInfo->tailLen;                //车尾长
    float headLen       = pstCarBodyInfo->stDynamicInfo.headLen;  //车头长
    float wheelBaseLen  = pstCarBodyInfo->wheelBaseLen;           //前后轮轴距
    float halfBodyWidth = 0.5 * pstCarBodyInfo->bodyWidth;        //半车身宽度
    float carHeight     = pstCarBodyInfo->carHeight;              //车高

    float extend = 0.1;
    float minX = -1.0 * tailLen - extend;
    float maxX = wheelBaseLen + headLen + extend;
    float minY = -1.0 * halfBodyWidth - extend;
    float maxY = -1.0 * minY;
    float minZ = -1.0 * extend;
    float maxZ = carHeight + extend;

    switch (pstCarBodyInfo->enCarType) {
        case CAR_TYPE_S310:
        case CAR_TYPE_S325:
        case CAR_TYPE_S330: stCarBodyVxlVerify_.outSpaceVxlNumThresh = 300;
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX, maxX, minY, maxY, minZ, maxZ));
                            break;

        case CAR_TYPE_S320:
        case CAR_TYPE_S323:
        case CAR_TYPE_S324:
        case CAR_TYPE_S326: stCarBodyVxlVerify_.outSpaceVxlNumThresh = 400;
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX - 0.3, maxX + 0.5,         minY, maxY, minZ, 0.6));  //底层，含前扫刷
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX,       wheelBaseLen + 0.5, minY, maxY,  0.6, 1.5));  //中间层，不含前扫刷
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX,       0.5,                minY, maxY,  1.5, maxZ)); //上层
                            break;

        case CAR_TYPE_S332:
        case CAR_TYPE_S333:
        case CAR_TYPE_S334:
        case CAR_TYPE_S335:
        case CAR_TYPE_S336:
        case CAR_TYPE_S337: stCarBodyVxlVerify_.outSpaceVxlNumThresh = 400;
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX - 0.3, maxX + 0.5,         minY, maxY, minZ, 0.6));  //底层，含前扫刷
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX,       wheelBaseLen + 0.5, minY, maxY, 0.6,  maxZ)); //上层
                            break;

        case CAR_TYPE_S350: stCarBodyVxlVerify_.outSpaceVxlNumThresh = 400;
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX - 0.3, maxX + 0.5,         minY, maxY, minZ, 0.6));  //底层，含前扫刷
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX,       wheelBaseLen + 0.5, minY, maxY, 0.6,  maxZ)); //上层
                            break;

        case CAR_TYPE_S520: stCarBodyVxlVerify_.outSpaceVxlNumThresh = 400;
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX - 0.3, maxX + 0.5,         minY, maxY, minZ, 0.6));  //底层，含前扫刷
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX,       wheelBaseLen + 0.6, minY, maxY, 0.6,  maxZ)); //上层
                            break;

        case CAR_TYPE_S530: stCarBodyVxlVerify_.outSpaceVxlNumThresh = 400;
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX - 0.3, maxX + 0.6,         minY, maxY, minZ, 0.8));  //底层，含前扫刷
                            stCarBodyVxlVerify_.push_back(CUBOID_STRU(minX,       wheelBaseLen + 1.0, minY, maxY, 0.8,  maxZ)); //上层
                            break;

        default: ST_LOG_ERR("无效的车型参数: %s.", strModel.c_str()); return -1;
    }

    return 0;
}

void CAR_BODY_FILE_C::DeleteTempCarBodyFilterFile(void)
{
    std::string strFilePath = objCfgFileMng_.GetCarBodyVoxelFilterTempFilePath();
    FILE_OPRT_C objFileOprt;
    if (objFileOprt.IsExist(strFilePath)) {
        objFileOprt.DelFile(strFilePath);
        ST_LOG_INFO("Delete Temp Car Body Filter File %s.", strFilePath.c_str());
    }
}

/**************************************************************************************
功能描述: 获取体素滤波文件路径
修改记录:
**************************************************************************************/
string CAR_BODY_FILE_C::GetFilePath() const
{
    return objCfgFileMng_.GetCarBodyVoxelFilterFilePath();
}

/**************************************************************************************
功能描述: 获取文件名
修改记录:
**************************************************************************************/
string CAR_BODY_FILE_C::GetFileName() const
{
    string strFilterFilePath = GetFilePath();

    size_t pos;
    do {
        pos = strFilterFilePath.find('/');
        if (pos != string::npos) strFilterFilePath = strFilterFilePath.substr(pos+1, strFilterFilePath.size()-1);
    } while (pos != string::npos);

    return strFilterFilePath;
}

/**************************************************************************************
功能描述: 读取下一行
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::ReadNextLine(FILE *pfRead, char *pucLine, int maxLineLen)
{
    char *pcRslt;

    do {
        pcRslt = fgets(pucLine, maxLineLen, pfRead);
        /* # 之后的为解释文本，跳过 */
    } while ((pcRslt != NULL) && (*pcRslt == '#'));

    return (pcRslt == NULL) ? -1 : 0;
}

/**************************************************************************************
功能描述: 读取体素滤波数据文件头
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::ReadFileHead(FILE *pfRead, int &voxelSizeX, int &voxelSizeY, int &voxelSizeZ)
{
    char acLine[1024];      /* 假设解释信息最长1024字节 */

    if ((0 != ReadNextLine(pfRead, acLine, sizeof(acLine))) ||
        (acLine[0] != 'S') || (acLine[1] != 'i')) {
        ST_LOG_ERR("Read Head <Size:> Fail");
        return -1;
    }

    if ((0 != ReadNextLine(pfRead, acLine, sizeof(acLine))) ||
        (3 != sscanf(acLine, "%d %d %d", &voxelSizeX, &voxelSizeY, &voxelSizeZ))) {
        ST_LOG_ERR("Read Size Data Fail");
        return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 车体过滤文件是否被修改
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::GetCarBodyFilterFileModifyTime(string strFile, time_t& modifyTime)
{
    struct stat fileStat;
    if (stat(strFile.c_str(), &fileStat) < 0) {
        ST_LOG_ERR("Unable to get file status for %s.", strFile.c_str());
        return 1;
    }

    modifyTime = fileStat.st_mtime;
    printf("\n");
    ST_LOG_INFO("车体过滤文件创建时间: %s", ctime(&fileStat.st_ctime));
    ST_LOG_INFO("车体过滤文件修改时间: %s", ctime(&fileStat.st_mtime));
    return 0;
}

/**************************************************************************************
功能描述: 车体过滤文件是否被修改
修改记录:
**************************************************************************************/
bool CAR_BODY_FILE_C::IsCarBodyFilterFileModified(string strFile)
{
    time_t modifyTime;
    if (GetCarBodyFilterFileModifyTime(strFile, modifyTime)) return true;
    return modifyTime_ != modifyTime;
}

/**************************************************************************************
功能描述: 创建3dArray 构造体素空间占用缓存
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::Create3dArray(const CAR_VXL_STRU& stCarVxl, char ***&array)
{
    array = (char***)calloc(stCarVxl.vxlSizeZ, sizeof(char**));
    for (int z = 0; z < stCarVxl.vxlSizeZ; ++z) {
        array[z] = (char**)calloc(stCarVxl.vxlSizeY, sizeof(char*));
        for (int y = 0; y < stCarVxl.vxlSizeY; ++y) {
            array[z][y] = (char*)calloc(stCarVxl.vxlSizeX, sizeof(char));
        }
    }

    ST_LOG_INFO("Create Success.");
}

/**************************************************************************************
功能描述: 删除3dArray 释放体素空间占用缓存
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::Delete3dArray(const CAR_VXL_STRU& stCarVxl, char ***&array)
{
    if (array) {
        for (int z = 0; z < stCarVxl.vxlSizeZ; ++z) {
            if (array[z] == nullptr) continue;
            for (int y = 0; y < stCarVxl.vxlSizeY; ++y) {
                free(array[z][y]);
                array[z][y] = nullptr;
            }
            free(array[z]);
            array[z] = nullptr;
        }
        free(array);
        array = nullptr;
    }

    ST_LOG_INFO("Delete Success.");
}

/**************************************************************************************
功能描述: 克隆3dArray
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::Clone3DArray(const CAR_VXL_STRU& stCarVxl, char ***&srcArray, char ***&dstArray)
{
    if (!srcArray || !dstArray) {
        ST_LOG_ERR("The 3D array is empty, please check.");
        return -1;
    }

    for (int z = 0; z < stCarVxl.vxlSizeZ; ++z) {
        if (!dstArray[z] || !srcArray[z]) {
            ST_LOG_ERR("The 2D array is empty, please check.");
            return -1;
        }

        for (int y = 0; y < stCarVxl.vxlSizeY; ++y) {
            if (!dstArray[z][y] || !srcArray[z][y]) {
                ST_LOG_ERR("The 1D array is empty, please check.");
                return -1;
            }

            for (int x = 0; x < stCarVxl.vxlSizeX; ++x) {
                dstArray[z][y][x] = srcArray[z][y][x];
            }
        }
    }

    ST_LOG_INFO("Clone Success.");
    return 0;
}

/**************************************************************************************
功能描述: 叠加3dArray
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::Add3DArray(const CAR_VXL_STRU& stCarVxl, char ***&srcArray, char ***&dstArray)
{
    if (!srcArray || !dstArray) {
        ST_LOG_ERR("The 3D array is empty, please check.");
        return -1;
    }

    for (int z = 0; z < stCarVxl.vxlSizeZ; ++z) {
        if (!dstArray[z] || !srcArray[z]) {
            ST_LOG_ERR("The 2D array is empty, please check.");
            return -1;
        }

        for (int y = 0; y < stCarVxl.vxlSizeY; ++y) {
            if (!dstArray[z][y] || !srcArray[z][y]) {
                ST_LOG_ERR("The 1D array is empty, please check.");
                return -1;
            }

            for (int x = 0; x < stCarVxl.vxlSizeX; ++x) {
                dstArray[z][y][x] |= srcArray[z][y][x];
            }
        }
    }

    ST_LOG_INFO("Add Success.");
    return 0;
}

/**************************************************************************************
功能描述: 拷贝3dArray
修改记录:
**************************************************************************************/
void CAR_BODY_FILE_C::Copy3DArray(const CAR_VXL_STRU& stCarVxlOld, char ***&arrayOld, const CAR_VXL_STRU& stCarVxlNew, char ***&arrayNew)
{
    if (!arrayNew) Create3dArray(stCarVxlNew, arrayNew);

    for (int vxlZ = 0; vxlZ < stCarVxlNew.vxlSizeZ; ++vxlZ) {
        float pz = (vxlZ + stCarVxlNew.vxlMinZ) * stCarVxlNew.vxlCell;
        for (int vxlY = 0; vxlY < stCarVxlNew.vxlSizeY; ++vxlY) {
            float py = (vxlY + stCarVxlNew.vxlMinY) * stCarVxlNew.vxlCell;
            for (int vxlX = 0; vxlX < stCarVxlNew.vxlSizeX; ++vxlX) {
                float px = (vxlX + stCarVxlNew.vxlMinX) * stCarVxlNew.vxlCell;
                int indexX, indexY, indexZ;
                VxlIndex(px, py, pz, stCarVxlOld, indexX, indexY, indexZ);
                if (!stCarVxlOld.IsIn(indexX, indexY, indexZ)) continue;
                if (arrayOld[indexZ][indexY][indexX]) arrayNew[vxlZ][vxlY][vxlX] = 1;
            }
        }
    }

    ST_LOG_INFO("Copy Success.");
}

/**************************************************************************************
功能描述: 填充内部的体素
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::FillInternalVoxel(const CAR_VXL_STRU& stCarVxl, const VXL_ORIGIN_STRU& stVxlOrigin, char ***&array)
{
    int numVxl = 0;

    for (int vxlZ = 0; vxlZ < stCarVxl.vxlSizeZ; ++vxlZ) {
        for (int vxlY = 0; vxlY < stCarVxl.vxlSizeY; ++vxlY) {
            for (int vxlX = 0; vxlX < stCarVxl.vxlSizeX; ++vxlX) {
                if (!array[vxlZ][vxlY][vxlX]) continue;
                for (int targetZ = 0; targetZ <= vxlZ; ++targetZ) {
                    int minY = MIN(vxlY, stVxlOrigin.vxlY);
                    int maxY = MAX(vxlY, stVxlOrigin.vxlY);
                    for (int targetY = minY; targetY <= maxY; ++targetY) {
                        int minX, maxX;
                        if (vxlY != stVxlOrigin.vxlY) {
                            float refX = 1.0 * (targetY- stVxlOrigin.vxlY) * (vxlX - stVxlOrigin.vxlX) /
                                               (vxlY - stVxlOrigin.vxlY) + stVxlOrigin.vxlX;
                            if (refX > vxlX) {
                                minX = vxlX;
                                maxX = CEIL(refX, 1e-4);
                            } else {
                                minX = FLOOR(refX, 1e-4);
                                maxX = vxlX;
                            }
                        } else {
                            minX = MIN(vxlX, stVxlOrigin.vxlX);
                            maxX = MAX(vxlX, stVxlOrigin.vxlX);
                        }

                        for (int targetX = minX; targetX < maxX; ++targetX) {
                            if (array[targetZ][targetY][targetX] == 0) {
                                array[targetZ][targetY][targetX] = 1;
                                numVxl++;
                            }
                        }
                    }
                }
            }
        }
    }

    return numVxl;
}

/**************************************************************************************
功能描述: 合并多雷达节点构建的车体过滤文件
修改记录:
**************************************************************************************/
int CAR_BODY_FILE_C::MergeCarBodyFilterFile(string strFile)
{
    /* 判断车体过滤文件有无被修改，如果有修改，则重新读取，并叠加过滤结果 */
    if (!IsCarBodyFilterFileModified(strFile)) return 0;

    ST_LOG_INFO("在保存车体过滤结果时检测到存在新的过滤文件，读取并累加新的结果。");
    char ***pVoxelCarBodyArrBk = nullptr;
    Create3dArray(stCarVxl_, pVoxelCarBodyArrBk);

    if (Clone3DArray(stCarVxl_, pVoxelCarBodyArr_, pVoxelCarBodyArrBk)) {
        ST_LOG_ERR("Failed to clone voxel car body array.");
        Delete3dArray(stCarVxl_, pVoxelCarBodyArrBk);
        return -1;
    }

    if (ReadCarBodyFilterFile(strFile)) {
        /* 在保存车体构建文件时，如果读取文件失败，不做任何处理，可能文件本就不存在 */
        ST_LOG_ERR("Failed to read car body filter file.");
        //Delete3dArray(pVoxelCarBodyArrBk);
        //return -1;
    }

    if (Add3DArray(stCarVxl_, pVoxelCarBodyArrBk, pVoxelCarBodyArr_)) {
        ST_LOG_ERR("Failed to add voxel car body array.");
        Delete3dArray(stCarVxl_, pVoxelCarBodyArrBk);
        return -1;
    }

    Delete3dArray(stCarVxl_, pVoxelCarBodyArrBk);

    return 0;
}


