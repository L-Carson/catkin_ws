

#ifndef __COMM_FILE_H__
#define __COMM_FILE_H__

#include "comm/comm_base.h"
#include "comm/comm_json_api.h"
#include <iostream>
#include <list>
#include <vector>
#include <boost/thread.hpp>

using namespace std;

#define WORK_SCENE_DIR_PARAM_PREFIX_STR     "-wsd="

/**************************************************************************************
功能描述: 项目文件管理
修改记录:
**************************************************************************************/
class FILE_MNG_C
{
public:
    FILE_MNG_C(void)
    {
        string strUsrName;
        if (0 != GetLoginUserName(strUsrName)) {
            ST_LOG_ERR("GetLoginUserName() Fail.");
            strUsrName = "ErrUsrName";
        }

        strUsrRootDir_       = "/home/" + strUsrName + "/";
        strMapFileDir_       = strUsrRootDir_ + "map_config/";
        strTraceFileDir_     = strUsrRootDir_ + "map_config/";
        strRouteFileDir_     = strUsrRootDir_ + "map_config/";
        strCfgFileDir_       = strUsrRootDir_ + "map_config/";
        strBoundaryFileDir_  = strUsrRootDir_ + "map_config/";
        strCameraConfigDir_  = strUsrRootDir_ + "map_config/camera_param/";
        strVxlFilterFileDir_ = strUsrRootDir_ + "map_config/";
        strLocationMapDir_   = strUsrRootDir_ + "map_config/work_scene/location_map/";
        strLogFileDir_       = strUsrRootDir_ + "map_config/log/processprint/";
        strMachineDir_       = strUsrRootDir_ + "robot_cfg/";
        strProductDir_       = strUsrRootDir_ + "navZero/cfg/products_param/";
        strModelFileDir_     = strUsrRootDir_ + "navZero/models/";
        strPnpLayerDir_      = strUsrRootDir_ + "map_config/work_scene/perception_map/";
    }

    std::string GetUsrRootDir(void) const;

    void GetMapDir(string &strMapDir);
    void GetTraceFileDir(string &strFileDir);
    void GetRouteFileDir(string &strFileDir);
    void GetCfgFileDir(string &strFileDir);
    void GetMachineDir(string &strFileDir);
    void GetProductDir(string &strFileDir);
    void GetBoundaryFileDir(string &strFileDir);
    void GetCameraConfigDir(string &strFileDir);
    void GetVxlFilterFileDir(string &strFileDir);
    void GetPnpLayerDir(string &strFileDir);

    string GetWorkSceneDir(int argc, char** argv) const;
    string GetWorkSceneDir(void) const;
    string GetModelFileDir(void) const;

    void GetLocationMapDir(string &strDir) const;

    void GetLogFileDir(string &strDir) const;

private:
    string strUsrRootDir_;          // 用户根目录
    string strMapFileDir_;
    string strTraceFileDir_;
    string strRouteFileDir_;
    string strCfgFileDir_;          // 配置参数文件目录
    string strMachineDir_;          // 机器目录（内容不随程序、场景而更新，存放机器固有信息：标定记录、操作记录等）
    string strProductDir_;          // 产品机型目录（内容跟随升级包更新）
    string strBoundaryFileDir_;
    string strCameraConfigDir_;     // 相机配置文件目录
    string strVxlFilterFileDir_;
    string strWorkSceneDir_;
    string strLocationMapDir_;
    string strLogFileDir_;
    string strModelFileDir_;
    string strPnpLayerDir_;         // 感知图层文件目录

    INT32 GetLoginUserName(string &strUserName);
};

/**
 * @brief 文件操作类
 *
 */
class FILE_OPRT_C
{
public:
    static string ToPureDir(const string &strSrcDir);
    static string ToStyledDir(const string &strSrcDir);

    static bool   IsExist(const string &strPath);
    static int    Rename(const string &strOldFilePath, const string &strNewFilePath, bool isCoverFile);
    static int    DelFile(const string &strFilePath);
    static int    DelDir(const string &strDirPath);
    static int    DelDirContent(const string &strDirPath);
    static int    DelDirOrFile(const string &strPath);
    static int    MakeDir(const string &strDirPath);
    static int    Copy(const string &strSrcFilePath, const string &strTgtFilePath, bool isCoverFile);
    static void   SyncBuffToFile(FILE *pf);
    static void   SyncFileToDisk(FILE *pf);

    static void   GetFileNameFrmPath(const string &strFilePath, string &strFileName);
    static void   GetFileDirFrmPath(const string &strFilePath, string &strFileDir);

    static INT32  GetFilesOfDir(const std::string &strDirPath, std::vector<std::string> &vectFileName);

    static BOOL   IsLinkFile(const std::string &strLinkFilePath);
    static INT32  MakeLink(const std::string &strLinkFilePath, const std::string &strTgtFilePath);
    static INT32  GetLinkTgtFilePath(const std::string &strLinkFilePath, std::string &strTgtFilePath);
private:
};

/**
 * @brief 机器人型号配置结构
 *
 */
struct RobotTypeCfgSt
{
    std::string robot_type;  // 机器人型号
    std::string parent_type; // 父型号
    std::string series_type; // 产品系列名
};

/**
 * @brief 车体型号解析器
 *
 */
class CAR_TYPE_PARSER_C
{
public:
    using Ptr = std::shared_ptr<CAR_TYPE_PARSER_C>;
    using ConstPtr = std::shared_ptr<const CAR_TYPE_PARSER_C>;

private:
    std::string parser_name_ = "车体型号解析器";
    std::string file_relative_name_ = "robot_cfg/robot_cfg.json"; // 机型文件相对名称
    RobotTypeCfgSt type_info_;                                    // 机器人型号信息

public:
    // 析构函数
    ~CAR_TYPE_PARSER_C(void);

    // 输出当前机型信息
    const RobotTypeCfgSt &GetRobotTypeInfo(void) const;

    // 输出当前机型名
    const std::string &GetRobotType(void) const;

    // 是否为室外清扫车
    bool IsOutdoorSweeper(void) const;

private:
    // 加载机器人配置文件
    bool LoadRobotCfgFile(const std::string &rel_file_name,
                          RobotTypeCfgSt &type_info);

    // 读取json文件
    int ReadJsonFile(const std::string &rel_file_name,
                     Json::Value &json_value,
                     std::string &abs_file_name);

    // 解析机器人信息配置
    int ParsingRobotInfoCfg(Json::Value &json_value,
                            RobotTypeCfgSt &type_info);

    // 解析型号信息
    bool ParsingTypeInfo(Json::Value &json_value,
                         RobotTypeCfgSt &type_info);

private:
    static std::recursive_mutex   instance_mutex_; // 递归锁
    static CAR_TYPE_PARSER_C::Ptr p_instance_;

public:
    // 返回对象指针
    static CAR_TYPE_PARSER_C::Ptr GetInstance(void)
    {
        if (nullptr == p_instance_)
        {
            std::unique_lock<std::recursive_mutex> lock(instance_mutex_);
            if (nullptr == p_instance_)
            {
                p_instance_.reset(new CAR_TYPE_PARSER_C());
            }
        }
        return p_instance_;
    }

private:
    CAR_TYPE_PARSER_C(void);                                          // 构造函数
    CAR_TYPE_PARSER_C(const CAR_TYPE_PARSER_C &other) = delete;       // 禁止使用拷贝构造函数
    CAR_TYPE_PARSER_C &operator=(const CAR_TYPE_PARSER_C &) = delete; // 禁止使用拷贝赋值运算符
};

/**************************************************************************************
功能描述: 地图文件管理
修改记录:
**************************************************************************************/
class MAP_FILE_MNG_C
{
public:
    MAP_FILE_MNG_C(void)
    {
        objFileMng_.GetMapDir(strMapDir_);
    }

    MAP_FILE_MNG_C(const string &strMapDir)
    {
        strMapDir_ = strMapDir;
    }

    void GetMapDir(string &strMapDir);
    void GetMapPath(const string &strMapName, string &strMapPath);
    void GetMapFilePath(const string &strMapFileName, string &strMapFilePath);

    void GetCombMapFileName(string &strCombMapFileName);
    void GetCombMapFilePath(string &strCombMapFilePath);
    void GetSplitCfgFileName(const string &strSrcMapName, string &strSplitCfgFileName);
    void GetSplitCfgFilePath(const string &strSrcMapName, string &strSplitCfgFilePath);

    void GetMapExYamlFilePath(string &strMapExYamlFilePath);

    void  GetSubMapName(UINT32 subMapNo, string &strSubMapName);
    INT32 GetSubMapNo(string &strSubMapName);
    void  GetSubMapPath(UINT32 subMapNo, string &strSubMapPath);

    void GetDefaultMapName(string &strDefaultMapName);
    void GetDefaultYamlFileName(string &strDefaultYamlFileName);
    void GetDefaultYamlFilePath(string &strDefaultYamlFilePath);
    void GetDefaultPgmFileName(string &strDefaultPgmFileName);
    void GetDefaultPgmFilePath(string &strDefaultPgmFilePath);

    void GetYamlFileName(const string &strMapName, string &strYamlFileName);
    void GetYamlFilePath(const string &strMapName, string &strYamlFilePath);
    void GetPgmFileName(const string &strMapName, string &strPgmFileName);
    void GetPgmFilePath(const string &strMapName, string &strPgmFilePath);

    void GetTmpMapName(const string &strMapName, string &strTmpMapName);
    void GetTmpYamlFileName(const string &strMapName, string &strTmpYamlFileName);
    void GetTmpYamlFilePath(const string &strMapName, string &strTmpYamlFilePath);
    void GetTmpPgmFileName(const string &strMapName, string &strTmpPgmFileName);
    void GetTmpPgmFilePath(const string &strMapName, string &strTmpPgmFilePath);

    void GetVirtualObstMapName(UINT32 subMapNo, string &strVirtualObstMapName);
    void GetVirtualObstMapPath(UINT32 subMapNo, string &strVirtualObstMapPath);

    void GetSimulaitonObstMapPath(string &strObstMapPath);

    bool isMapPathExist(const string &strMapPath);

    INT32 RenameMap(const string &strSrcMapName, const string &strTgtMapName);
    INT32 CopyMap(const string &strSrcMapName, const string &strTgtMapName);
    void  DelMap(const string &strMapName);

private:
    FILE_MNG_C  objFileMng_;
    FILE_OPRT_C objFileOprt_;

    string strMapDir_;

    const string strDefautMapName_               = "zhishanmap";
    const string strTmpFileExName_               = "_tmp";
    const string strVirtualObstMapExName_        = "_virtual_obst";
    const string strSimulationObstMapExName_     = "_simulation_obst";
    const string strMapYamlSuffix_               = ".yaml";
    const string strMapPgmSuffix_                = ".pgm";
    const string strCombMapFileName_             = "comb_map.xml";
    const string strSplitCfgFileExName_          = "_split_cfg.xml";
    const string strMapExYamlFileName_           = "zhishanmapEx.yaml";

    INT32 RenamePgmFile(const string &strSrcMapName, const string &strTgtMapName);
    INT32 RenameYamlFile(const string &strSrcMapName, const string &strTgtMapName);
    INT32 CopyPgmFile(const string &strSrcMapName, const string &strTgtMapName);
    INT32 CopyYamlFile(const string &strSrcMapName, const string &strTgtMapName);
    void  DelPgmFile(const string &strMapName);
    void  DelYamlFile(const string &strMapName);
};

/**
 * @brief 工作场景文件管理器
 *
 */
class WORK_SCENE_FILE_MNG_C
{
public:
    WORK_SCENE_FILE_MNG_C(void)
    {
        SetSceneDefaultDir();
    }

    WORK_SCENE_FILE_MNG_C(const std::string &strSceneDir)
    {
        SetSceneDir(strSceneDir);
    }

    static std::string GetDefaultSceneDir(void);
    static std::string ParseWorkSceneDir(int argc, char** argv);

    void SetSceneDir(const std::string &strDir);
    void SetSceneDefaultDir(void);

    std::string GetWorkSceneDir(void) const;
    std::string GetRoadsInfoFilePath(void) const;
    std::string GetRoadRouteFilePath(UINT32 roadNo, UINT32 routeId) const;
    std::string GetRoadRouteAnchorPointFilePath(UINT32 roadNo, UINT32 routeId) const;
    std::string GetRoadLineFilePath(UINT32 roadNo, UINT32 lineId) const;
    std::string GetRoadLineAnchorPointFilePath(UINT32 roadNo, UINT32 lineId) const;
    std::string GetRoadObstLineFilePath(UINT32 roadNo, UINT32 obstLineId) const;
    std::string GetRoadObstLineAnchorPointFilePath(UINT32 roadNo, UINT32 obstLineId) const;
    std::string GetRoadStopLineFilePath(UINT32 roadNo, UINT32 stopLineId) const;
    std::string GetRoadStopLineAnchorPointFilePath(UINT32 roadNo, UINT32 stopLineId) const;
    std::string GetRoadLaneLineFilePath(UINT32 roadNo, UINT32 laneLineId) const;
    std::string GetRoadLaneLineAnchorPointFilePath(UINT32 roadNo, UINT32 laneLineId) const;
    std::string GetRoadAreaFilePath(UINT32 roadNo, UINT32 areaId) const;
    std::string GetRoadGoalAreaFilePath(UINT32 roadNo, UINT32 goalAreaId) const;

    std::string GetSimulationObstMapPath(void) const;

    std::string GetDyncMapFilePath(void) const;

    const std::string &GetLocMapDir(void) const;
    const std::string &GetNavMapDir(void) const;
    const std::string &GetSimulationMapDir(void) const;
    const std::string &GetDynamicMapDir(void) const;

private:
    std::string strWorkSceneDir_;
    std::string strNavigationMapDir_;
    std::string strLocationMapDir_;
    std::string strSimulationMapDir_;
    std::string strDynamicMapDir_;

    std::string strRoadsInfoFileName_;
    std::string strSimulationMapName_;
};

/**************************************************************************************
功能描述: 配置文件管理
          老的cfg_param.xml文件也在该目录，新的cfg_param.xml目录由基础架构管理，在 config_manager.h 中获取。
修改记录:
**************************************************************************************/
class CFG_FILE_MNG_C
{
public:
    /**
     * @brief 输出绝对目录: 配置参数文件目录
     *
     * @param strFileDir
     */
    void GetCfgFileDir(string &strFileDir) const;
    
    std::string GetCfgFileDir(void) const;

    std::string GetTempDir(void) const;

    /**
     * @brief 输出文件绝对名称: 车身体素滤波数据文件
     *
     * @param strAbsFile
     */
    std::string GetCarBodyVoxelFilterFilePath(void)  const { return GetCfgFileDir() + strCarBodyVoxelFilterFileName_; };
    std::string GetCarBodyVoxelPcdFilePath(void)     const { return GetCfgFileDir() + strCarBodyVoxelPcdFileName_;    };
    std::string GetCarBodyVoxelSideImgFilePath(void) const { return GetCfgFileDir() + strCarBodyVoxelSideImgFileName_;};
    std::string GetCarBodyVoxelTopImgFilePath(void)  const { return GetCfgFileDir() + strCarBodyVoxelTopImgFileName_; };
    std::string GetCarBodyVoxelRearImgFilePath(void) const { return GetCfgFileDir() + strCarBodyVoxelRearImgFileName_;};

    std::string GetCarBodyVoxelFilterTempFilePath(void)  const { return GetTempDir() + strCarBodyVoxelFilterFileName_; };
    std::string GetCarBodyVoxelPcdTempFilePath(void) const { return GetTempDir() + strCarBodyVoxelPcdFileName_;    };
    std::string GetCarBodyVoxelSideImgTempFilePath(void) const { return GetTempDir() + strCarBodyVoxelSideImgFileName_;};
    std::string GetCarBodyVoxelTopImgTempFilePath(void)  const { return GetTempDir() + strCarBodyVoxelTopImgFileName_; };
    std::string GetCarBodyVoxelRearImgTempFilePath(void) const { return GetTempDir() + strCarBodyVoxelRearImgFileName_;};

private:
    std::string strCarBodyVoxelFilterFileName_{"car_body_voxel_filter.dat"};        // 车身体素滤波数据文件名
    std::string strCarBodyVoxelPcdFileName_{"car_body_voxel_filter.pcd"};           // 车身体素滤波点云文件名
    std::string strCarBodyVoxelSideImgFileName_{"car_body_voxel_filter_side.png"};  // 车身体素滤波图片文件名 - 侧视图
    std::string strCarBodyVoxelTopImgFileName_{"car_body_voxel_filter_top.png"};    // 车身体素滤波图片文件名 - 俯视图
    std::string strCarBodyVoxelRearImgFileName_{"car_body_voxel_filter_rear.png"};  // 车身体素滤波图片文件名 - 后视图
};

#endif

