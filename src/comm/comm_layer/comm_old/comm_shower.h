#ifndef __COMM_SHOWER_H__
#define __COMM_SHOWER_H__

#include "comm_coordinate.h"
#include "comm_data_type.h"

#include "comm/comm_car.h"

#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PolygonStamped.h>
#include <nav_msgs/Path.h>
#include <nav_msgs/OccupancyGrid.h>
#include "sensor_msgs/PointCloud2.h"
#include <ros/ros.h>

#include <vector>
#include <string>
#include <list>

/**************************************************************************************
功能描述: 工具显示的颜色
          左1字节为红色原色
          左2字节为绿色原色
          左3字节为蓝色原色
修改记录:
**************************************************************************************/
typedef enum
{
    SHOW_COLOR_RED = 0xFF000000,    /* 红色 */
    SHOW_COLOR_GREEN = 0x00FF0000,  /* 绿色 */
    SHOW_COLOR_BLUE = 0x0000FF00,   /* 蓝色 */
    SHOW_COLOR_BLUE_1 = 0x9B9BFF00, /* 浅蓝色 */
    SHOW_COLOR_WHITE = 0xFFFFFF00,  /* 白色 */
    SHOW_COLOR_BLACK = 0X00000000,  /* 黑色 */
    SHOW_COLOR_YELLOW = 0xFFFF0000, /* 黄色 */
    SHOW_COLOR_PURPLE = 0xA020F000, /* 紫色 */
} SHOW_COLOR_ENUM;

/* 获取指定颜色的原色 */
#define GET_PRI_COLOR_B(enColor_) (float(GET_BYTE(enColor_, 1)) / 255.0)
#define GET_PRI_COLOR_G(enColor_) (float(GET_BYTE(enColor_, 2)) / 255.0)
#define GET_PRI_COLOR_R(enColor_) (float(GET_BYTE(enColor_, 3)) / 255.0)

// 道路元素显示
class ROAD_ELE_SHOWER_C
{
public:
    ROAD_SHOW_C(ros::NodeHandle &nh, const std::string &strTopicName, unsigned int queueSize = 10000, bool isKeep = false)
    {
        publisher_ = nh.advertise<visualization_msgs::Marker>(strTopicName.c_str(), queueSize, false);
    }

    unsigned int GetSubscriberNum(void) const;

    void ShowLine(unsigned int objId, const std::vector<COORDINATE_2D_STRU> &vectLine, float width, SHOW_COLOR_ENUM enColor);
    void ShowLabel(unsigned int objId, const COORDINATE_2D_STRU &stPos, const std::string &strLabel, float height, SHOW_COLOR_ENUM enColor);
    void ShowArrow(unsigned int objId, const COORDINATE_2D_STRU &stStartPos, const COORDINATE_2D_STRU &stStopPos, float width, SHOW_COLOR_ENUM enColor);
    void ShowCubeList(unsigned int objId, const std::vector<COORDINATE_2D_STRU> &vectPos, float width, SHOW_COLOR_ENUM enColor);

    void Clear(void);

private:
    struct SHOW_OBJ_STRU
    {
        std::string strNS; /* 不同的命名空间是不一样的对象 */
        unsigned int objId;
    };

    const unsigned int maxShowLineSize_ = 10000; /* 数量不能太大否则容易导致RVIZ崩溃 */

    ros::Publisher publisher_;
    std::vector<SHOW_OBJ_STRU> vectShowObj;

    void AddShowObj(const std::string &strNS, unsigned int objId);
    void ClrObjShow(const SHOW_OBJ_STRU &stShowObj) const;
};

#endif
