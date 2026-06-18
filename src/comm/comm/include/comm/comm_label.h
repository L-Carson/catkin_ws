#ifndef __COMM_LABEL_H__
#define __COMM_LABEL_H__

#include "comm_msg/label.h"
#include "comm_msg/label_state.h"
#include "comm_msg/coorLabel.h"
#include "comm_msg/nav_ctrl.h"
#include "comm_msg/visionFunc.h"
#include "comm/comm_dbg_log.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

/* 传感器水平朝向 */
enum class SENSOR_FACE_H_DIR_ENUM
{
    ANY,        /* 不定 */
    FRONT,      /* 前视 */
    BACK,       /* 后视 */
    LEFT,       /* 左视 */
    RIGHT       /* 右视 */
};

/* 传感器垂直朝向 */
enum class SENSOR_FACE_V_DIR_ENUM
{
    ANY,        /* 不定 */
    UP,         /* 斜向上 */
    FLAT,       /* 水平 */
    DOWN        /* 斜向下 */
};

/**
 * @brief radar id，须按照递增顺序定义
 *
 */
enum RADAR_ENUM {
    NO_RADAR = 0,   // 0 NULL
    LEFT_RADAR,     // 1 左向毫米波雷达
    FRONT_RADAR,    // 2 前向毫米波雷达
    RIGHT_RADAR,    // 3 右向毫米波雷达
    BACK_RADAR,     // 4 后向毫米波雷达
};

const std::string Radar[]   = {"", "LeftRadar", "FrontRadar", "RightRadar", "BackRadar"};  //须与RADAR_ENUM顺序保持一致
const std::string RadarCn[] = {"", "左向毫米波雷达", "前向毫米波雷达", "右向毫米波雷达", "后向毫米波雷达"};  //须与RADAR_ENUM顺序保持一致

/**
 * @brief rbg camera id，须按照递增顺序定义
 *
 */
enum RGB_CAMERA_ENUM {
    NO_CAMERA = 0,      //0 NULL
    FRONT_LEFT,         //1 前左相机-------- 视角: 斜向下
    FRONT_MID_LEFT,     //2 前中左相机------ 视角: 平视前方
    FRONT_MID,          //3 前中相机-------- 视角: 平视前方
    FRONT_MID_RIGHT,    //4 前中右相机------ 视角: 平视前方
    FRONT_RIGHT,        //5 前右相机-------- 视角: 斜向下
    FRONT_UP_LEFT,      //6 前上左相机------ 视角: 斜向上
    FRONT_UP_RIGHT,     //7 前上右相机------ 视角: 斜向上
    REAR_LEFT,          //8 后左相机-------- 视角: 平视后方
    REAR_MID,           //9 后中相机-------- 视角: 平视后方
    REAR_RIGHT,         //A 后右相机-------- 视角: 平视后方
    LEFT_MID,           //B 左中相机-------- 视角: 左侧
    RIGHT_MID,          //C 右中相机-------- 视角: 右侧
};

struct RGB_CAMERA_BASE_INFO_STRU
{
    RGB_CAMERA_ENUM         enNameId;
    std::string             strName;
    SENSOR_FACE_H_DIR_ENUM  enFaceDirH;
    SENSOR_FACE_V_DIR_ENUM  enFaceDirV;
};

// 须与RGB_CAMERA_ENUM顺序保持一致
const std::vector<std::string> RgbCamera      = {"", "front_left", "front_mid_left", "front_mid", "front_mid_right", "front_right", "front_up_left", "front_up_right", "rear_left", "rear_mid", "rear_right", "left_mid", "right_mid"};
const std::vector<std::string> RgbCameraFront = {"", "front_left", "front_mid_left", "front_mid", "front_mid_right", "front_right", "front_up_left", "front_up_right", "",          "",         "",           "",         ""};

// 须与RGB_CAMERA_ENUM顺序保持一致
const RGB_CAMERA_BASE_INFO_STRU g_astRgbCameraBaseInfo[] =
{
    {NO_CAMERA,         RgbCamera[NO_CAMERA],       SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::FLAT},
    {FRONT_LEFT,        RgbCamera[FRONT_LEFT],      SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::DOWN},
    {FRONT_MID_LEFT,    RgbCamera[FRONT_MID_LEFT],  SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::UP},
    {FRONT_MID,         RgbCamera[FRONT_MID],       SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::FLAT},
    {FRONT_MID_RIGHT,   RgbCamera[FRONT_MID_RIGHT], SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::UP},
    {FRONT_RIGHT,       RgbCamera[FRONT_RIGHT],     SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::DOWN},
    {FRONT_UP_LEFT,     RgbCamera[FRONT_UP_LEFT],   SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::UP},
    {FRONT_UP_RIGHT,    RgbCamera[FRONT_UP_RIGHT],  SENSOR_FACE_H_DIR_ENUM::FRONT, SENSOR_FACE_V_DIR_ENUM::UP},
    {REAR_LEFT,         RgbCamera[REAR_LEFT],       SENSOR_FACE_H_DIR_ENUM::BACK,  SENSOR_FACE_V_DIR_ENUM::FLAT},
    {REAR_MID,          RgbCamera[REAR_MID],        SENSOR_FACE_H_DIR_ENUM::BACK,  SENSOR_FACE_V_DIR_ENUM::FLAT},
    {REAR_RIGHT,        RgbCamera[REAR_RIGHT],      SENSOR_FACE_H_DIR_ENUM::BACK,  SENSOR_FACE_V_DIR_ENUM::FLAT},
    {LEFT_MID,          RgbCamera[LEFT_MID],        SENSOR_FACE_H_DIR_ENUM::LEFT,  SENSOR_FACE_V_DIR_ENUM::FLAT},
    {RIGHT_MID,         RgbCamera[RIGHT_MID],       SENSOR_FACE_H_DIR_ENUM::RIGHT, SENSOR_FACE_V_DIR_ENUM::FLAT}
};

inline const RGB_CAMERA_BASE_INFO_STRU *Comm_GetCameraBaseInfoByName(const std::string &strSearchName)
{
    const RGB_CAMERA_BASE_INFO_STRU *pstBegin = &g_astRgbCameraBaseInfo[0];
    const RGB_CAMERA_BASE_INFO_STRU *pstEnd   = pstBegin + ARRAY_SIZE(g_astRgbCameraBaseInfo);

    const RGB_CAMERA_BASE_INFO_STRU *pstSearch = std::find_if(pstBegin, pstEnd, [&strSearchName](const RGB_CAMERA_BASE_INFO_STRU &stBaseInfo){ return (stBaseInfo.strName == strSearchName); });
    return (pstSearch != pstEnd) ? pstSearch : nullptr;
}

inline const RGB_CAMERA_BASE_INFO_STRU *Comm_GetCameraBaseInfoByNameId(RGB_CAMERA_ENUM enNameId)
{
    R_ASSERT_2(enNameId < ARRAY_SIZE(g_astRgbCameraBaseInfo), nullptr, enNameId, ARRAY_SIZE(g_astRgbCameraBaseInfo));
    const RGB_CAMERA_BASE_INFO_STRU &stCameraBaseInfo = g_astRgbCameraBaseInfo[enNameId];
    R_ASSERT_2(stCameraBaseInfo.enNameId == enNameId, nullptr, stCameraBaseInfo.enNameId, enNameId);
    return &stCameraBaseInfo;
}

/**
 * @brief depth camera id，须按照递增顺序定义
 *
 */
enum D_CAMERA_ENUM {
    NO_D_CAMERA = 0,    //0 注意：为了兼容图像模块，顺序不可修改
    FRONT_D_CAMERA,     //1 前向深度相机
    BACK_D_CAMERA,      //2 后向深度相机
    LEFT_D_CAMERA,      //3 左向深度相机
    RIGHT_D_CAMERA,     //4 右向深度相机
};

const std::string DepthCamera[] = {"", "FrontDepthCamera",  "BackDepthCamera", "LeftDepthCamera", "RightDepthCamera"};  //须与D_CAMERA_ENUM顺序保持一致

/**
 * @brief lidar id，须按照递增顺序定义
 *
 */
enum LIDAR_ENUM {
    NO_LIDAR = 0,       //0 NULL
    MAIN_LIDAR,         //1 主雷达
    BLIND_LIDAR,        //2 补盲雷达（补盲雷达按照前后左右的顺序排列）
    BLIND_LIDAR2,       //3 补盲雷达2
    BLIND_LIDAR3,       //4 补盲雷达3
    BLIND_LIDAR4,       //5 补盲雷达4
};

const std::vector<std::string> Lidar = {     "", "MainLidar", "BlindLidar", "BlindLidar2", "BlindLidar3", "BlindLidar4"};   //须与LIDAR_ENUM顺序保持一致
const std::vector<std::string> BlindLidar = {"", ""           "BlindLidar", "BlindLidar2", "BlindLidar3", "BlindLidar4"};   //补盲雷达

/**
 * @brief lidar型号，须按照递增顺序定义
 *
 */
enum LIDAR_TYPE_ENUM {
    TYPE_NULL = 0,  //NULL
    RSBP,
    RS16,
    RSHELIOS_16P,   //i.e. RSHELIOS_16
    RSHELIOS,
    RSAIRY,
    LS_C16,
    LIVOX_MID_360,
};

const std::vector<std::string> LidarType = {     "",  "RSBP", "RS16", "RSHELIOS_16P", "RSHELIOS", "RSAIRY", "LS_C16", "LIVOX_MID_360"}; //须与LIDAR_TYPE_ENUM顺序保持一致
const std::vector<std::string> MainLidarType = { "",  "RSBP", "RS16", "RSHELIOS_16P", "RSHELIOS", "RSAIRY", "LS_C16", "LIVOX_MID_360"}; //主雷达类型
const std::vector<std::string> BlindLidarType = {"",  "RSBP", "",     "",             "",         "RSAIRY", "",       "LIVOX_MID_360"}; //补盲雷达类型

/**
 * @brief ultrasonic id，须按照递增顺序定义
 *
 */
enum US_ENUM {
    NO_US = 0,      //0 NULL
    LF_US,          //1 左前超声波
    LM_US,          //2 左中超声波
    LD_US,          //3 左对角超声波
    BL_US,          //4 后左超声波
    BR_US,          //5 后右超声波
    RD_US,          //6 右对角超声波
    RM_US,          //7 右中超声波
    RF_US,          //8 右前超声波
    FR_US,          //9 前右超声波
    FL_US,          //A 前左超声波
    FM_US,          //B 前中超声波
    BM_US,          //C 后中超声波
};

const std::vector<std::string> Ultrasonic = {"", "LF", "LM", "LD", "BL", "BR", "RD", "RM", "RF", "FR", "FL", "FM", "BM"};   //所有超声，须与US_ENUM顺序保持一致
const std::vector<std::string> FrontUs    = {"", "LF", "LM", "",   "",   "",   "",   "RM", "RF", "FR", "FL", "FM", ""};     //前超声，须与US_ENUM顺序保持一致
const std::vector<std::string> BackUs     = {"", "",   "",   "LD", "BL", "BR", "RD", "",   "",   "",   "",   "",   "BM"};   //后超声，须与US_ENUM顺序保持一致

/**
 * @brief tof id，须按照递增顺序定义
 *
 */
enum TOF_ENUM {
    NO_TOF = 0,     //0 NULL
    LS_TOF,         //1 左侧tof
    LD_TOF,         //2 左对角tof
    BL_TOF,         //3 后左tof
    BR_TOF,         //4 后右tof
    RD_TOF,         //5 右对角tof
    RS_TOF,         //6 右侧tof
};

const std::vector<std::string> Tof = {"", "LS", "LD", "BL", "BR", "RD", "RS"};  //须与TOF_ENUM顺序保持一致

/**
 * @brief 障碍分层
 *
 */
enum OBST_LAYER_ENUM {
    OBST_LAYER_NULL = 0,
    OBST_LAYER_TOP,     //上层障碍
    OBST_LAYER_MID,     //中间层障碍
    OBST_LAYER_DOWN,    //底层障碍
};

const std::vector<std::string> ObstLayer = {"", "Top", "Mid", "Down"};  //须与OBST_LAYER_ENUM顺序保持一致

/**
 * @brief 障碍类别标签
 *
 */
enum OBST_LABEL_ENUM {
    OBST_LABEL_UNKNOWN      = comm_msg::coorLabel::UNKNOWN,       //未知类型
    OBST_LABEL_STATIC       = comm_msg::coorLabel::STATIC,        //静态障碍
    OBST_LABEL_DYNAMIC      = comm_msg::coorLabel::DYNAMIC,       //动态障碍
    OBST_LABEL_HOLLOW_OBST  = comm_msg::coorLabel::HOLLOW_OBST,   //空洞障碍
    OBST_LABEL_SHORT_HOLD   = comm_msg::coorLabel::SHORT_HOLD,    //低矮维持
    OBST_LABEL_COMMON_SHORT = comm_msg::coorLabel::COMMON_HOLD,   //常规维持
    OBST_LABEL_BLIND_SPOT   = comm_msg::coorLabel::BLIND_SPOT,    //盲区障碍
    OBST_LABEL_SLOPE_PAD    = comm_msg::coorLabel::SLOPE_PAD,     //斜坡垫
    OBST_LABEL_CONTACT_EDGE = comm_msg::coorLabel::CONTACT_EDGE,  //触边障碍
    OBST_LABEL_TREELAWN     = comm_msg::coorLabel::TREELAWN,      //低矮绿植障碍
    OBST_LABEL_ULTRASONIC   = comm_msg::coorLabel::ULTRASONIC,    //超声障碍
};

/**
 * @brief 视觉功能id
 *
 */
enum VISION_FUNC_ID_ENUM {
    VI_NULL_FUNC   = comm_msg::visionFunc::NULL_FUNC,       //null
    VI_QR_CODE_DET = comm_msg::visionFunc::QR_CODE_DET,     //二维码检测
    VI_NET_DET     = comm_msg::visionFunc::NET_DET,         //网络检测
    VI_NET_SEG     = comm_msg::visionFunc::NET_SEG,         //网络分割
    VI_SHORT_OBST_SEG   = 4,                                //低矮障碍物检测
    VI_TRAFFICLIGHT_DET = 5,                                //交通灯检测
    VI_TRASH_BOX_DET    = 6,                                //垃圾箱检测
    VI_CLEANCLASS_DET   = 7,                                //洁净度检测
};

const std::unordered_map<VISION_FUNC_ID_ENUM, const char*> VisionFuncId({
    {VISION_FUNC_ID_ENUM::VI_NULL_FUNC,   "无视觉任务"},
    {VISION_FUNC_ID_ENUM::VI_QR_CODE_DET, "二维码检测"},
    {VISION_FUNC_ID_ENUM::VI_NET_DET,     "网络检测"},
    {VISION_FUNC_ID_ENUM::VI_NET_SEG,     "网络分割"},
    {VISION_FUNC_ID_ENUM::VI_SHORT_OBST_SEG,     "低矮障碍分割"},
    {VISION_FUNC_ID_ENUM::VI_TRAFFICLIGHT_DET,     "交通灯检测"},
    {VISION_FUNC_ID_ENUM::VI_TRASH_BOX_DET,     "垃圾箱检测"},
    {VISION_FUNC_ID_ENUM::VI_CLEANCLASS_DET,    "洁净度检测"},
});

/**
 * @brief 目标类别标签
 *
 */
enum LABEL_ENUM {
    /* 常规类别 */
    TAG_UNKNOWN              = comm_msg::label::UNKNOWN,                        //未知（统一类别）
    TAG_PEDESTRIAN           = comm_msg::label::PEDESTRIAN,                     //行人
    TAG_CYCLIST              = comm_msg::label::CYCLIST,                        //骑自行车的人
    TAG_BICYCLE              = comm_msg::label::BICYCLE,                        //自行车（两轮车）
    TAG_CAR                  = comm_msg::label::CAR,                            //汽车
    TAG_TRUCK                = comm_msg::label::TRUCK,                          //卡车
    TAG_TRAM                 = comm_msg::label::TRAM,                           //有轨电车
    TAG_TRICYCLE             = comm_msg::label::TRICYCLE,                       //三轮车
    TAG_BUS                  = comm_msg::label::BUS,                            //公交车
    TAG_BLACK_VEHICLE        = 74,                                              //黑色车辆
    
    /* 像素点数据 */
    TAG_LOWOBST              = comm_msg::label::LOWOBST,                        //低矮障碍
    TAG_PIPELINE             = comm_msg::label::PIPELINE,                       //水管等管线
    TAG_ROADEDGE             = comm_msg::label::ROADEDGE,                       //道路边缘
    TAG_CORDON               = comm_msg::label::CORDON,                         //警戒线
    TAG_TREELAWN             = comm_msg::label::TREELAWN,                       //街道绿化带
    TAG_THICKPIPE            = comm_msg::label::THICKPIPE,                      //粗水管
    TAG_CORD                 = comm_msg::label::CORD,                           //电线、缆绳
    TAG_SLOPE_PAD            = comm_msg::label::SLOPE_PAD,                      //斜坡垫
    TAG_SPEED_BUMP           = comm_msg::label::SPEED_BUMP,                     //减速带
    TAG_FISHING_ROD          = 73,                                              //钓鱼竿
    TAG_FISHING_ROD_LLM      = 75,                                              //钓鱼竿大模型检测
    TAG_TREELAWN_LLM         = 76,                                              //AI绿植检测
    TAG_TRASH_BOX            = 77,                                              //垃圾箱
    

    /* 像素框数据 - 井盖类 */
    TAG_COVER_SOLID               = comm_msg::label::COVER_SOLID,               //实心井盖
    TAG_COVER_HOLLOW              = comm_msg::label::COVER_HOLLOW,              //空心井盖
    TAG_TRAFFIC_CONE              = comm_msg::label::TRAFFIC_CONE,              //雪糕筒（交通锥）
    TAG_METAL_BARRIER             = comm_msg::label::METAL_BARRIER,             //金属围栏（铁马）
    TAG_MEDIAN_BARRIER            = comm_msg::label::MEDIAN_BARRIER,            //栅栏（道路中央隔离带）
    TAG_CEREMONIAL_POLE           = comm_msg::label::CEREMONIAL_POLE,           //礼宾杆
    TAG_BOLLARD                   = comm_msg::label::BOLLARD,                   //路桩

    /* 像素框数据 - 垃圾类 */
    TAG_GARBAGE                   = comm_msg::label::GARBAGE,                   //垃圾（统称）
    TAG_GARBAGE_BOTTLE            = comm_msg::label::GARBAGE_BOTTLE,            //瓶子
    TAG_GARBAGE_PAPER             = comm_msg::label::GARBAGE_PAPER,             //纸巾
    TAG_GARBAGE_BRANCH            = comm_msg::label::GARBAGE_BRANCH,            //树枝
    TAG_GARBAGE_LEAVES            = comm_msg::label::GARBAGE_LEAVES,            //树叶
    TAG_GARBAGE_BOUGH             = comm_msg::label::GARBAGE_BOUGH,             //粗树枝、长树枝
    TAG_GARBAGE_CUP               = comm_msg::label::GARBAGE_CUP,               //奶茶杯
    TAG_GARBAGE_CAN               = comm_msg::label::GARBAGE_CAN,               //易拉罐
    TAG_GARBAGE_CARTON            = comm_msg::label::GARBAGE_CARTON,            //纸盒
    TAG_GARBAGE_PACKAGE           = comm_msg::label::GARBAGE_PACKAGE,           //烟盒
    TAG_GARBAGE_BRICK             = comm_msg::label::GARBAGE_BRICK,             //砖块
    TAG_GARBAGE_CIGARETTEEND      = comm_msg::label::GARBAGE_CIGARETTEEND,      //烟头
    TAG_GARBAGE_PERICARP          = 72,                                         //果皮（类别增加会导致comm_msg/label消息变化，引发通信问题）

    /* 像素框数据 - 交通灯 */
    TAG_TRAFFIC_LIGHT             = comm_msg::label::TRAFFIC_LIGHT,             //交通灯（统称）
    TAG_TRAFFIC_LIGHT_OFF         = comm_msg::label::TRAFFIC_LIGHT_OFF,         //交通灯-关闭
    TAG_TRAFFIC_LIGHT_RED         = comm_msg::label::TRAFFIC_LIGHT_RED,         //交通灯-红灯
    TAG_TRAFFIC_LIGHT_GREEN       = comm_msg::label::TRAFFIC_LIGHT_GREEN,       //交通灯-绿灯
    TAG_TRAFFIC_LIGHT_YELLOW      = comm_msg::label::TRAFFIC_LIGHT_YELLOW,      //交通灯-黄灯

    TAG_TRAFFIC_LIGHT_GROUP       = comm_msg::label::TRAFFIC_LIGHT_GROUP,       //交通灯-灯组（小灯）
    TAG_TRAFFIC_LIGHT_OFF_LITE    = comm_msg::label::TRAFFIC_LIGHT_OFF_LITE,    //交通灯-关闭（小灯）
    TAG_TRAFFIC_LIGHT_RED_LITE    = comm_msg::label::TRAFFIC_LIGHT_RED_LITE,    //交通灯-红灯（小灯）
    TAG_TRAFFIC_LIGHT_GREEN_LITE  = comm_msg::label::TRAFFIC_LIGHT_GREEN_LITE,  //交通灯-绿灯（小灯）
    TAG_TRAFFIC_LIGHT_YELLOW_LITE = comm_msg::label::TRAFFIC_LIGHT_YELLOW_LITE, //交通灯-黄灯（小灯）

    /* 半自动充电枪检测 */
    TAG_CHARGING_GUN              = comm_msg::label::CHARGING_GUN,              //充电枪

    /* 占位符 */
    TAG_MAX                       = comm_msg::label::PLACEHOLDER,               //占位符
};

const std::unordered_map<LABEL_ENUM, const char*> Label({
    {LABEL_ENUM::TAG_UNKNOWN,                   "未知"},
    {LABEL_ENUM::TAG_PEDESTRIAN,                "行人"},
    {LABEL_ENUM::TAG_CYCLIST,                   "骑车的人"},
    {LABEL_ENUM::TAG_BICYCLE,                   "自行车（两轮车）"},
    {LABEL_ENUM::TAG_CAR,                       "汽车"},
    {LABEL_ENUM::TAG_TRUCK,                     "卡车"},
    {LABEL_ENUM::TAG_TRAM,                      "有轨电车"},
    {LABEL_ENUM::TAG_TRICYCLE,                  "三轮车"},
    {LABEL_ENUM::TAG_BUS,                       "公交车"},
    {LABEL_ENUM::TAG_LOWOBST,                   "低矮障碍"},
    {LABEL_ENUM::TAG_PIPELINE,                  "水管"},
    {LABEL_ENUM::TAG_ROADEDGE,                  "道路边缘"},
    {LABEL_ENUM::TAG_CORDON,                    "警戒线"},
    {LABEL_ENUM::TAG_TREELAWN,                  "绿化带"},
    {LABEL_ENUM::TAG_THICKPIPE,                 "粗水管"},
    {LABEL_ENUM::TAG_CORD,                      "线缆"},
    {LABEL_ENUM::TAG_SLOPE_PAD,                 "斜坡垫"},
    {LABEL_ENUM::TAG_SPEED_BUMP,                "减速带"},
    {LABEL_ENUM::TAG_COVER_SOLID,               "实心井盖"},
    {LABEL_ENUM::TAG_COVER_HOLLOW,              "空心井盖"},
    {LABEL_ENUM::TAG_TRAFFIC_CONE,              "雪糕筒（交通锥）"},
    {LABEL_ENUM::TAG_METAL_BARRIER,             "金属围栏（铁马）"},
    {LABEL_ENUM::TAG_MEDIAN_BARRIER,            "栅栏（道路中央隔离带）"},
    {LABEL_ENUM::TAG_CEREMONIAL_POLE,           "礼宾杆"},
    {LABEL_ENUM::TAG_BOLLARD,                   "路桩"},
    {LABEL_ENUM::TAG_GARBAGE,                   "垃圾"},
    {LABEL_ENUM::TAG_GARBAGE_BOTTLE,            "瓶子"},
    {LABEL_ENUM::TAG_GARBAGE_PAPER,             "纸巾"},
    {LABEL_ENUM::TAG_GARBAGE_BRANCH,            "树枝"},
    {LABEL_ENUM::TAG_GARBAGE_LEAVES,            "树叶"},
    {LABEL_ENUM::TAG_GARBAGE_BOUGH,             "粗树枝长树枝"},
    {LABEL_ENUM::TAG_GARBAGE_CUP,               "奶茶杯"},
    {LABEL_ENUM::TAG_GARBAGE_CAN,               "易拉罐"},
    {LABEL_ENUM::TAG_GARBAGE_CARTON,            "纸盒"},
    {LABEL_ENUM::TAG_GARBAGE_PACKAGE,           "烟盒"},
    {LABEL_ENUM::TAG_GARBAGE_BRICK,             "砖块"},
    {LABEL_ENUM::TAG_GARBAGE_CIGARETTEEND,      "烟头"},
    {LABEL_ENUM::TAG_GARBAGE_PERICARP,          "果皮"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT,             "交通灯"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_OFF,         "交通灯-关闭"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_RED,         "交通灯-红灯"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_GREEN,       "交通灯-绿灯"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_YELLOW,      "交通灯-黄灯"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_GROUP,       "交通灯-灯组(LITE)"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_OFF_LITE,    "交通灯-关闭(LITE)"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_RED_LITE,    "交通灯-红灯(LITE)"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_GREEN_LITE,  "交通灯-绿灯(LITE)"},
    {LABEL_ENUM::TAG_TRAFFIC_LIGHT_YELLOW_LITE, "交通灯-黄灯(LITE)"},
    {LABEL_ENUM::TAG_CHARGING_GUN,              "充电枪"},
    {LABEL_ENUM::TAG_FISHING_ROD,               "钓鱼竿"}, 
    {LABEL_ENUM::TAG_BLACK_VEHICLE,             "黑色车辆"}, 
    {LABEL_ENUM::TAG_FISHING_ROD_LLM,           "钓鱼竿大模型检测"},
    {LABEL_ENUM::TAG_TRASH_BOX,                 "垃圾箱检测"},
    {LABEL_ENUM::TAG_MAX,                       "占位符"}
});

/**
 * @brief 标签类别存在状态
 *
 */
enum LABEL_STATE_ENUM {
    LABEL_STATE_NO  = comm_msg::label_state::NO,   //不存在
    LABEL_STATE_YES = comm_msg::label_state::YES,  //存在
    LABEL_STATE_MAY = comm_msg::label_state::MAY,  //可能存在
};

const std::unordered_map<LABEL_STATE_ENUM, const char*> LabelState({
    {LABEL_STATE_ENUM::LABEL_STATE_NO,  "不存在"},
    {LABEL_STATE_ENUM::LABEL_STATE_YES, "存在"},
    {LABEL_STATE_ENUM::LABEL_STATE_MAY, "可能存在"},
});

/**
 * @brief 导航控制标签
 *
 */
enum NAV_CTRL_ENUM {
    NAV_CTRL_GARBAGE_DETECT = comm_msg::nav_ctrl::GARBAGE_DETECT,   //垃圾寻扫
    NAV_CTRL_CONTACT_EDGE   = comm_msg::nav_ctrl::CONTACT_EDGE,     //触边生成障碍物
};

const std::unordered_map<NAV_CTRL_ENUM, const char*> NavCtrl({
    {NAV_CTRL_ENUM::NAV_CTRL_GARBAGE_DETECT, "垃圾寻扫"},
    {NAV_CTRL_ENUM::NAV_CTRL_CONTACT_EDGE,   "触边生成障碍物"},
});

/**
 * @brief 道路区域标识
 *
 */
enum ROAD_ITEM_ENUM {
    ROAD_ITEM_NULL = 0,
    ROAD_ITEM_COVER,        // 排水沟区
    ROAD_ITEM_SLOPE_PAD,    // 斜坡垫检测区
    ROAD_ITEM_VEL_LIMIT,    // 限速区
    ROAD_ITEM_HEIGHT_LIMIT, // 感知限高区
    ROAD_ITEM_OBST_IGNORE,  // 障碍忽略区
    ROAD_ITEM_RANGE_LIMIT,  // 感知范围区
    ROAD_ITEM_TREELAWN,     // 低矮绿植区
    ROAD_ITEM_DET,          // 水管检测使能区
    ROAD_ITEM_FISH_ROD_DET, // 钓鱼竿AI检测使能区
};

const std::unordered_map<ROAD_ITEM_ENUM, const char*> RoadItem({
    {ROAD_ITEM_ENUM::ROAD_ITEM_COVER,           "排水沟区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_SLOPE_PAD,       "斜坡垫区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_VEL_LIMIT,       "限速区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_HEIGHT_LIMIT,    "感知限高区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_OBST_IGNORE,     "障碍忽略区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_RANGE_LIMIT,     "感知范围区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_TREELAWN,        "低矮绿植区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_DET,             "水管检测区"},
    {ROAD_ITEM_ENUM::ROAD_ITEM_FISH_ROD_DET,    "钓鱼竿AI检测使能区"},
});

template<typename T>
const bool HasElement(const std::vector<T>& vele, const T ele) {
    std::stringstream ss;
    ss << ele;
    if (ss.str().empty()) {
        printf("[ERR]HasElement: The element is empty.\n");
        return false;
    }

    auto iter = std::find(vele.begin(), vele.end(), ele);
    return iter != vele.end();
}

template<typename T>
const int ElementId(const std::vector<T> vele, const T ele) {
    std::stringstream ss;
    ss << ele;
    if (ss.str().empty()) {
        printf("[ERR]ElementId: The element is empty.\n");
        return -1;
    }

    auto iter = std::find(vele.begin(), vele.end(), ele);
    if (iter == vele.end()) {
        printf("[ERR]ElementId: The element <%s> is not exist.\n", ss.str().c_str());
        return -1;
    }

    return int(iter-vele.begin());
}

#endif

