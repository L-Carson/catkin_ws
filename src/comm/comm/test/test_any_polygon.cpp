#include "comm/comm_show.h"
#include "comm/comm_base.h"
#include "comm/comm_ros_base.h"

#include <visualization_msgs/Marker.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/io/pcd_io.h> //PCL的PCD格式文件的输入输出头文件
#include <pcl/point_types.h> //PCL对各种格式的点的支持头文件


#include <ros/ros.h>

#include <vector>

#define TOPIC_NAME_SHOW_POLYGON                     "/test/show/polygon"
#define TOPIC_NAME_SHOW_POLYGON_AREA_POINTS         "/test/show/polygon_area_points"

namespace TEST
{


typedef pcl::PointXYZ MyPoint;
typedef pcl::PointCloud<MyPoint> MyPointCloud;


class SHOW_POLYGON_C
{
public:
    SHOW_POLYGON_C(ros::NodeHandle &nh)
    {
        ros::Publisher objPub_ = nh.advertise<visualization_msgs::Marker>(TOPIC_NAME_SHOW_POLYGON, 10);
    }

    void Show(const std::vector<COORDINATE_2D_STRU> &vectVertice)
    {
        visualization_msgs::Marker stPolygonVerticeMsg;
        stPolygonVerticeMsg.header.frame_id = "/map";
        stPolygonVerticeMsg.header.stamp    = ros::Time::now();
        stPolygonVerticeMsg.ns     = "points_and_lines";
        stPolygonVerticeMsg.id     = 0;
        stPolygonVerticeMsg.type   = visualization_msgs::Marker::POINTS;
        stPolygonVerticeMsg.action = visualization_msgs::Marker::MODIFY;
        // POINTS markers use x and y scale for width/height respectively
        stPolygonVerticeMsg.scale.x = 0.2;
        stPolygonVerticeMsg.scale.y = 0.2;
        // Points are green
        stPolygonVerticeMsg.color.g = 1.0f;
        stPolygonVerticeMsg.color.a = 1.0;
        stPolygonVerticeMsg.pose.orientation.w = 1.0;

        visualization_msgs::Marker stPolygonSideMsg;
        stPolygonSideMsg.header.frame_id = "/map";
        stPolygonSideMsg.header.stamp    = ros::Time::now();
        stPolygonSideMsg.ns     = "points_and_lines";
        stPolygonSideMsg.id     = 1;
        stPolygonSideMsg.type   = visualization_msgs::Marker::LINE_STRIP;
        stPolygonSideMsg.action = visualization_msgs::Marker::MODIFY;
        // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
        stPolygonSideMsg.scale.x = 0.1;
        // Line strip is blue
        stPolygonSideMsg.color.b = GET_PRI_COLOR_B(SHOW_COLOR_BLUE);
        stPolygonSideMsg.color.g = GET_PRI_COLOR_G(SHOW_COLOR_BLUE);
        stPolygonSideMsg.color.r = GET_PRI_COLOR_R(SHOW_COLOR_BLUE);
        stPolygonSideMsg.color.a = 1.0;
        stPolygonSideMsg.pose.orientation.w = 1.0;

        for (const COORDINATE_2D_STRU stVertice : vectVertice) {
            geometry_msgs::Point objMsgPoint;
            objMsgPoint.x = stVertice.x;
            objMsgPoint.y = stVertice.y;
            objMsgPoint.z = 0.0;
            stPolygonVerticeMsg.points.push_back(objMsgPoint);
            stPolygonSideMsg.points.push_back(objMsgPoint);
        }

        const COORDINATE_2D_STRU &stFirstVertice = vectVertice.front();
        geometry_msgs::Point      objFirstMsgPoint;
        objFirstMsgPoint.x = stFirstVertice.x;
        objFirstMsgPoint.y = stFirstVertice.y;
        objFirstMsgPoint.z = 0.0;
        stPolygonSideMsg.points.push_back(objFirstMsgPoint);

        objPub_.publish(stPolygonVerticeMsg);
        objPub_.publish(stPolygonSideMsg);
    }

private:
    ros::Publisher objPub_;
};

class SHOW_POLYGON_AREA_POINTS_C
{
public:
    SHOW_POLYGON_AREA_POINTS_C(ros::NodeHandle &nh)
    {
        ros::Publisher objPub_ = nh.advertise<sensor_msgs::PointCloud2>(TOPIC_NAME_SHOW_POLYGON_AREA_POINTS, 1);
    }

    void Show(const std::vector<COORDINATE_2D_STRU> &vectVertice)
    {
        MyPointCloud  objPoints;
        GetAreaPoints(vectVertice, objPoints);

        Send(objPoints);
    }

private:
    ros::Publisher objPub_;

    void GetAreaPoints(const std::vector<COORDINATE_2D_STRU> &vectVertice, MyPointCloud &objPoints)
    {
        ANY_POLYGON_C objPolygon;
        INT32 rslt = objPolygon.Reset(vectVertice);
        if (rslt != 0) {
            return;
        }

        AREA_C objArea;
        objArea.IncludePoints(vectVertice);

        COORDINATE_2D_STRU stPoint;
        for (stPoint.x = objArea.stLBPoint.x; stPoint.x < objArea.stRTPoint.x; stPoint.x += 0.01) {
            for (stPoint.y = objArea.stLBPoint.y; stPoint.y < objArea.stRTPoint.y; stPoint.y += 0.01) {
                if (objPolygon.IsPointIn(stPoint)) {
                    MyPoint stOutPoint;
                    stOutPoint.x = stPoint.x;
                    stOutPoint.y = stPoint.y;
                    stOutPoint.z = 0.0;
                    objPoints.push_back(stOutPoint);
                }
            }
        }
    }

    void Send(MyPointCloud &objPoints)
    {
        sensor_msgs::PointCloud2 stMsgPoints;
        pcl::toROSMsg(objPoints, stMsgPoints);
        stMsgPoints.header.frame_id = "/map";
        stMsgPoints.header.stamp    = ros::Time::now();

        objPub_.publish(stMsgPoints);
    }
};

/* 随机生成点 */
void GenRandPoints(UINT32 verticeNum, std::vector<COORDINATE_2D_STRU> &vectVertice)
{
    srand(time(0));

    float areaSizeX = 10.0;
    float areaSizeY = 10.0;

    for (UINT32 loop = 0; loop < verticeNum; ++loop) {
        COORDINATE_2D_STRU stVertice;
        stVertice.x = float(rand() % UINT32(areaSizeX * 1000.0)) / 1000.0 - areaSizeX * 0.5;
        stVertice.y = float(rand() % UINT32(areaSizeY * 1000.0)) / 1000.0 - areaSizeY * 0.5;

        vectVertice.push_back(stVertice);
    }
}

}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_any_polygon");
    ros::NodeHandle nh;
    int             runFrequency = 1;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    TEST::SHOW_POLYGON_C              objShowPolygon(nh);
    TEST::SHOW_POLYGON_AREA_POINTS_C  objShowPolygonAreaPoints(nh);

    ros::Rate loop_rate(runFrequency);
    while(ros::ok())
    {
        Comm_UptByPeriod();
        ros::spinOnce();

        std::vector<COORDINATE_2D_STRU> vectVertice;
        TEST::GenRandPoints(4, vectVertice);

        objShowPolygon.Show(vectVertice);
        objShowPolygonAreaPoints.Show(vectVertice);

        loop_rate.sleep();
    }

    return 0;
}



