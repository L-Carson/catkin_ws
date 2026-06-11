/*****************************************************************************
*  @brief                                                                    *
*  @author   Zenghui Luo                                                     *
*  @version  3.0                                                             *
*  @date     2022/12/09                                                      *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/12/09 | 1.0       | Zenghui Luo    | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#ifndef __COMMON_LIDAR_TYPE_PARSER_H__
#define __COMMON_LIDAR_TYPE_PARSER_H__

#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include <sys/file.h>
#include <unistd.h>
#include <unordered_map>

#include "comm/comm_base.h"
#include "comm/comm_cfg_param.h"
#include "comm/comm_json_api.h"

// TODO:
// 机型、机器、设备
// 机型设备表： 记录该机型的全量设备（设备名、ip、frame等）
//              内部配置，跟随程序包
//              3D雷达通过xml实现该表
// 机器设备表： 记录该机器装载的设备（设备名、设备型号、序列号等）
//              生产/标定工具配置，跟随机器
//              3D雷达、深度相机均使用json实现
//              外参通过urdf实现
// 机器人设备管理器： 基于机型设备表、机器设备表、xml文件，维护当前机器配置的雷达名-型号映射表
// 雷达型号解析器： 维护雷达型号-参数映射表，从属于设备管理器
//

// TODO: 思路
// 0、机型设备表随程序包更新，机器设备表基于工具生产
// 1、加载机型设备表，获取全量设备
// 2、加载机器设备表，获取当前配置的设备，并保留合法的设备
// 3、机器设备表基于每个机型，设置缺省值，若配置则使用配置的信息
// 4、系统启动时，先启动机器人设备管理器
// 5、设备管理器：解析当前机器装载的设备型号，调用相应驱动
//              解析装载的设备参数、配置，变更pcpt相应参数（不同进程？如何同步？如何变更？）
//              需要加载json、xml文件，即需要调用清扫车comm包（问题在于清扫车日志系统，底层变更为log4）
//              进程间同步问题（S200，pcpt内有设备解析、也有设备管理器，如何兼容？）
// 7、清扫车：没有设备管理器，也没有机器配置表，只有xml（机型配置表）
//          提供一个解析器：挂载在comm内，可从属于速腾驱动、pcpt
//                       维持清扫车原有接口不变
//                       需要实现：通过配置解析雷达名称、开关、型号、ip、frame等
// 8、话题录制：不同机型的传感器名不一致，设备管理器广播当前机器的设备信息，录制者在公共话题之外，适配当前设备的数据
// 9、部署/运维：这两者监控的传感器数据是什么？scan？


// TODO:
// 1、comm内新增雷达型号解析器，定义雷达系列（在品牌之下，如RS、LS_C），解析器基于型号识别雷达系列
// 2、同时启动速腾、镭神驱动，雷达解析器加载参数，若相应系列没有雷达使能，则退出相应驱动
// 3、pcpt将雷达名称传入解析器，获取相应雷达对应的参数（水平角分辨率、scan点数、环数等）
// 4、pcpt、驱动将雷达名称传入雷达配置管理器，获取相应配置（frame、ip、tf等）

/**
 * @brief lidar系列型号
 *
 */
enum class LidarSeriesType
{
    kNull  = 0, // 无效
    kRS    = 1, // 速腾聚创全系
    kLS_C  = 2, // 镭神智能C系列
    kFR_C  = 3, // 富锐光学C系列
    kLIVOX = 4, // livox全系
};

inline LidarSeriesType GetSeriesType(const std::string &series_name)
{
    if (series_name == "kRS")
    {
        return std::move(LidarSeriesType::kRS);
    }
    else if (series_name == "kLS_C")
    {
        return std::move(LidarSeriesType::kLS_C);
    }
    else if (series_name == "kFR_C")
    {
        return std::move(LidarSeriesType::kFR_C);
    }
    else if (series_name == "kLIVOX")
    {
        return std::move(LidarSeriesType::kLIVOX);
    }
    return std::move(LidarSeriesType::kNull);
}

inline std::string GetSeriesName(const LidarSeriesType &series_type)
{
    switch (series_type)
    {
    case LidarSeriesType::kRS:
        return std::move("kRS");
    case LidarSeriesType::kLS_C:
        return std::move("kLS_C");
    case LidarSeriesType::kFR_C:
        return std::move("kFR_C");
    case LidarSeriesType::kLIVOX:
        return std::move("kLIVOX");
    default:
        return std::move("kNull");
    }
}

/**
 * @brief lidar参数结构
 *
 */
struct LidarParamSt
{
    LidarSeriesType series_type = LidarSeriesType::kNull; // 雷达系列型号
    std::string dev_type;                                 // 设备型号/雷达型号（厂家定义）
    double v_angle_resolution = -1.0;                     // 垂直角分辨率[unit: rad]
    double h_angle_resolution = -1.0;                     // 水平角分辨率[unit: rad]
    int scan_points_num = -1;                             // 一环点数
    int rings_num = -1;                                   // 环数
};

// 说明: 两类表使用的雷达型号均为自定义，以方便识别，厂家定义的型号只用于驱动
typedef std::unordered_map<std::string, std::string> LidarNameTypeTable; // 雷达名称-型号映射表类型
typedef std::unordered_map<std::string, LidarParamSt> LidarParamTable;   // 雷达型号-参数映射表类型

/**
 * @brief 雷达型号解析器
 *
 */
class LidarTypeParser
{
    private:
        std::string parser_name_ = "雷达型号解析器";
        std::string file_relative_name_ = "navZero/cfg/normal_device_model.json"; // 机型文件相对名称
        LidarNameTypeTable lidar_name_type_table_;                                // 雷达名称-型号映射表
        LidarParamTable lidar_type_param_table_;                                  // 雷达型号-参数映射表

        CFG_PARAM_C objCfgParam_; // 配置参数服务器

    public:
        // 析构函数
        ~LidarTypeParser(void);

        // 获取指定名称的lidar参数
        const LidarParamSt *GetGivenNameLidarParam(const std::string &lidar_name);

        // 是否存在指定系列的lidar
        bool IsExistGivenSeriesLidar(const LidarSeriesType &series_type);

    private:
        // 初始化
        int Init(void);

        // 获取指定型号的lidar参数
        const LidarParamSt *GetGivenTypeLidarParam(const std::string &lidar_type);

        // 构造雷达名称型号映射表
        void CreateLidarNameTypeTable(const CFG_PARAM_LIDARS_STRU &lidars_cfg,
                                      LidarNameTypeTable &lidar_name_type_table);

        // 构造默认雷达型号参数信息
        void CreateDefaultTypeParamInfo(LidarParamTable &lidar_type_param_table);

        // 加载雷达型号参数信息
        bool LoadLidarParamInfo(const std::string &rel_file_name,
                                LidarParamTable &lidar_type_param_table);

        // 读取json文件
        int ReadJsonFile(const std::string &rel_file_name,
                         Json::Value &json_value,
                         std::string &abs_file_name);

        // 解析雷达型号参数信息
        int ParsingLidarParamInfo(Json::Value &json_value,
                                  LidarParamTable &lidar_type_param_table);

        // 解析一个雷达参数信息
        bool ParsingOneLidarInfo(Json::Value &json_value,
                                 std::string &lidar_type,
                                 LidarParamSt &lidar_param);

        // 打印雷达型号参数信息
        void PrintLidarParamInfo(const LidarParamTable &lidar_type_param_table);

    private:
        static std::recursive_mutex   instance_mutex_; // 递归锁
        static LidarTypeParser *p_instance_;

    public:
        // 返回对象指针
        static LidarTypeParser *GetInstancePoint(void)
        {
            if (nullptr == p_instance_)
            {
                // 区域递归锁
                std::unique_lock<std::recursive_mutex> lock(instance_mutex_);

                if (nullptr == p_instance_)
                {
                    p_instance_ = new LidarTypeParser();
                }
            }

            return p_instance_;
        }

    private:
        LidarTypeParser(void);                                        // 构造函数
        LidarTypeParser(const LidarTypeParser &other) = delete;       // 禁止使用拷贝构造函数
        LidarTypeParser &operator=(const LidarTypeParser &) = delete; // 禁止使用拷贝赋值运算符
};

#endif
