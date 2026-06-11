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
#include "comm/comm_lidar_type_parser.h"

std::recursive_mutex   LidarTypeParser::instance_mutex_;
LidarTypeParser *LidarTypeParser::p_instance_ = nullptr;

// 构造函数
LidarTypeParser::LidarTypeParser(void)
{
    Init();
}

// 析构函数
LidarTypeParser::~LidarTypeParser(void)
{
}

// 初始化
int LidarTypeParser::Init(void)
{
    if (objCfgParam_.Init() != 0)
    {
        ST_LOG_ERR("配置参数服务器: 初始化失败.");
        return -1;
    }

    // 构造雷达型号参数信息
    if (!LoadLidarParamInfo(file_relative_name_, lidar_type_param_table_))
    {
        CreateDefaultTypeParamInfo(lidar_type_param_table_);
    }
    PrintLidarParamInfo(lidar_type_param_table_);

    // 构造雷达名称型号映射表
    const CFG_PARAM_LIDARS_STRU &lidars_cfg = objCfgParam_.GetLidarsCfgInfo();
    CreateLidarNameTypeTable(lidars_cfg, lidar_name_type_table_);
    return 0;
}

// 获取指定名称的lidar参数
const LidarParamSt *LidarTypeParser::GetGivenNameLidarParam(const std::string &lidar_name)
{
    LidarNameTypeTable::const_iterator cit_nt = lidar_name_type_table_.find(lidar_name);
    if (cit_nt == lidar_name_type_table_.end())
    {
        return nullptr;
    }

    return GetGivenTypeLidarParam(cit_nt->second);
}

// 是否存在指定系列的lidar
bool LidarTypeParser::IsExistGivenSeriesLidar(const LidarSeriesType &series_type)
{
    for (LidarNameTypeTable::const_iterator cit_nt = lidar_name_type_table_.begin();
         cit_nt != lidar_name_type_table_.end(); ++cit_nt)
    {
        const LidarParamSt *p_param = GetGivenTypeLidarParam(cit_nt->second);
        if (p_param != nullptr)
        {
            if (p_param->series_type == series_type)
            {
                return true;
            }
        }
    }
    return false;
}

// 获取指定型号的lidar参数
const LidarParamSt *LidarTypeParser::GetGivenTypeLidarParam(const std::string &lidar_type)
{
    LidarParamTable::const_iterator cit_tp = lidar_type_param_table_.find(lidar_type);
    if (cit_tp == lidar_type_param_table_.end())
    {
        return nullptr;
    }

    return &(cit_tp->second);
}

// 构造雷达名称型号映射表
void LidarTypeParser::CreateLidarNameTypeTable(const CFG_PARAM_LIDARS_STRU &lidars_cfg,
                                               LidarNameTypeTable &lidar_name_type_table)
{
    for (const CFG_PARAM_LIDAR_STRU& stLidarCfg : lidars_cfg.vstLidarsCfg)
        lidar_name_type_table[stLidarCfg.LidarName()] = stLidarCfg.LidarType();
}

// 构造默认雷达型号参数信息
void LidarTypeParser::CreateDefaultTypeParamInfo(LidarParamTable &lidar_type_param_table)
{
    std::string lidar_type;
    LidarParamSt lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::RSBP];
    lidar_param.series_type = LidarSeriesType::kRS;
    lidar_param.dev_type = LidarType[LIDAR_TYPE_ENUM::RSBP];
    lidar_param.v_angle_resolution = Deg2Rad(-1.0);
    lidar_param.h_angle_resolution = Deg2Rad(0.2);
    lidar_param.scan_points_num = 1800;
    lidar_param.rings_num = 32;
    lidar_type_param_table[lidar_type] = lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::RSAIRY];
    lidar_param.series_type = LidarSeriesType::kRS;
    lidar_param.dev_type = LidarType[LIDAR_TYPE_ENUM::RSAIRY];
    lidar_param.v_angle_resolution = Deg2Rad(0.947);
    lidar_param.h_angle_resolution = Deg2Rad(0.4);
    lidar_param.scan_points_num = 900;
    lidar_param.rings_num = 96;
    lidar_type_param_table[lidar_type] = lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::RS16];
    lidar_param.series_type = LidarSeriesType::kRS;
    lidar_param.dev_type = LidarType[LIDAR_TYPE_ENUM::RS16];
    lidar_param.v_angle_resolution = Deg2Rad(2.0);
    lidar_param.h_angle_resolution = Deg2Rad(0.18);
    lidar_param.scan_points_num = 2000;
    lidar_param.rings_num = 16;
    lidar_type_param_table[lidar_type] = lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::RSHELIOS_16P];
    lidar_param.series_type = LidarSeriesType::kRS;
    lidar_param.dev_type = LidarType[LIDAR_TYPE_ENUM::RSHELIOS_16P];
    lidar_param.v_angle_resolution = Deg2Rad(2.0);
    lidar_param.h_angle_resolution = Deg2Rad(0.18);
    lidar_param.scan_points_num = 2000;
    lidar_param.rings_num = 16;
    lidar_type_param_table[lidar_type] = lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::RSHELIOS];
    lidar_param.series_type = LidarSeriesType::kRS;
    lidar_param.dev_type = LidarType[LIDAR_TYPE_ENUM::RSHELIOS];
    lidar_param.v_angle_resolution = Deg2Rad(-1.0);
    lidar_param.h_angle_resolution = Deg2Rad(0.18);
    lidar_param.scan_points_num = 2000;
    lidar_param.rings_num = 32;
    lidar_type_param_table[lidar_type] = lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::LS_C16];
    lidar_param.series_type = LidarSeriesType::kLS_C;
    lidar_param.dev_type = "c16";
    lidar_param.v_angle_resolution = Deg2Rad(2.0);
    lidar_param.h_angle_resolution = Deg2Rad(0.18);
    lidar_param.scan_points_num = 2000;
    lidar_param.rings_num = 16;
    lidar_type_param_table[lidar_type] = lidar_param;

    lidar_type = LidarType[LIDAR_TYPE_ENUM::LIVOX_MID_360];
    lidar_param.series_type = LidarSeriesType::kLIVOX;
    lidar_param.dev_type = LidarType[LIDAR_TYPE_ENUM::LIVOX_MID_360];
    lidar_param.v_angle_resolution = Deg2Rad(2.0);
    lidar_param.h_angle_resolution = Deg2Rad(0.5);
    lidar_param.scan_points_num = 1800;
    lidar_param.rings_num = 32;
    lidar_type_param_table[lidar_type] = lidar_param;
    ST_LOG_INFO("%s: 构造默认雷达型号参数信息", parser_name_.data());
}

// 加载雷达型号参数信息
bool LidarTypeParser::LoadLidarParamInfo(const std::string &rel_file_name,
                                         LidarParamTable &lidar_type_param_table)
{
    // 读取json文件
    Json::Value root_value;
    std::string abs_file_name;
    if (ReadJsonFile(rel_file_name, root_value, abs_file_name) < 0)
    {
        ST_LOG_WARN("%s: 文件读取失败[%s]",
                   parser_name_.data(), abs_file_name.data());
        return false;
    }

    int par_ret = ParsingLidarParamInfo(root_value, lidar_type_param_table);
    if (par_ret < 0)
    {
        ST_LOG_ERR("%s: 雷达型号参数信息解析失败! 异常类型[%d] 文件[%s]",
                   parser_name_.data(),
                   par_ret, abs_file_name.data());
        return false;
    }

    return true;
}

// 读取json文件
int LidarTypeParser::ReadJsonFile(const std::string &rel_file_name,
                                  Json::Value &json_value,
                                  std::string &abs_file_name)
{
    FILE_MNG_C objFileMng;
    abs_file_name = objFileMng.GetUsrRootDir() + rel_file_name;

    common_depend::JsonApiC json_api;
    int api_ret = json_api.GetRootJsonObject(abs_file_name, json_value);
    if (api_ret)
    {
        return -2;
    }
    return 0;
}

// 解析雷达型号参数信息
int LidarTypeParser::ParsingLidarParamInfo(Json::Value &json_value,
                                           LidarParamTable &lidar_type_param_table)
{
    if (!json_value.isObject())
    {
        return -1;
    }
    if (!json_value["lidars"].isArray())
    {
        return -2;
    }

    int lidars_num = json_value["lidars"].size();
    for (int idx = 0; idx < lidars_num; ++idx)
    {
        std::string lidar_type;
        LidarParamSt lidar_param;
        if (!ParsingOneLidarInfo(json_value["lidars"][idx],
                                 lidar_type, lidar_param))
        {
            return -3;
        }
        lidar_type_param_table[lidar_type] = std::move(lidar_param);
    }

    return 0;
}

// 解析一个雷达参数信息
bool LidarTypeParser::ParsingOneLidarInfo(Json::Value &json_value,
                                          std::string &lidar_type,
                                          LidarParamSt &lidar_param)
{
    if (!json_value["lidar_type"].isString() ||
        !json_value["series_type"].isString() ||
        !json_value["dev_type"].isString() ||
        !json_value["v_angle_resolution"].isDouble() ||
        !json_value["h_angle_resolution"].isDouble() ||
        !json_value["scan_points_num"].isInt() ||
        !json_value["rings_num"].isInt())
    {
        return false;
    }

    lidar_type = json_value["lidar_type"].asString();
    lidar_param.series_type = GetSeriesType(json_value["series_type"].asString());
    lidar_param.dev_type = json_value["dev_type"].asString();
    lidar_param.v_angle_resolution = Deg2Rad(json_value["v_angle_resolution"].asDouble());
    lidar_param.h_angle_resolution = Deg2Rad(json_value["h_angle_resolution"].asDouble());
    lidar_param.scan_points_num = json_value["scan_points_num"].asInt();
    lidar_param.rings_num = json_value["rings_num"].asInt();
    return true;
}

// 打印雷达型号参数信息
void LidarTypeParser::PrintLidarParamInfo(const LidarParamTable &lidar_type_param_table)
{
    std::stringstream ss_data;
    ss_data << endl;
    ss_data << "---------------------------------------------------";
    ss_data << "雷达型号参数信息: ";

    for (LidarParamTable::const_iterator cit_lidar = lidar_type_param_table.begin();
         cit_lidar != lidar_type_param_table.end(); ++cit_lidar)
    {
        const LidarParamSt &lidar_param = cit_lidar->second;
        ss_data << endl;
        ss_data << "雷达[" << cit_lidar->first << "] ";
        ss_data << "系列[" << GetSeriesName(lidar_param.series_type) << "] ";
        ss_data << "设备名[" << lidar_param.dev_type << "] ";
        ss_data << "环数[" << lidar_param.rings_num << "] ";
        ss_data << "scan点数[" << lidar_param.scan_points_num << "] ";
        ss_data << "角分辨率vh["
                << Rad2Deg(lidar_param.v_angle_resolution) << " "
                << Rad2Deg(lidar_param.h_angle_resolution) << "][deg] ";
    }
    ss_data << endl;
    ss_data << "---------------------------------------------------";
    ST_LOG_INFO("%s", ss_data.str().data());
}
