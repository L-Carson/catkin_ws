

#include "comm/comm_geometry.h"
#include "comm/comm_base.h"
#include "comm/comm_ros_base.h"


#include <ros/ros.h>

#include <vector>


namespace TEST
{

}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_geometry");
    ros::NodeHandle nh;
    int             runFrequency = 20;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    {
        COORDINATE_2D_STRU stLine1StartPoint(131.75, -162.85);
        COORDINATE_2D_STRU stLine1EndPoint(131.82, -162.92);
        COORDINATE_2D_STRU stLine2StartPoint(131.89, -162.99);
        COORDINATE_2D_STRU stLine2EndPoint(131.96, -163.06);

        DIRECTED_LINE_SEGMENT_C objLine1(stLine1StartPoint, stLine1EndPoint);
        DIRECTED_LINE_SEGMENT_C objLine2(stLine2StartPoint, stLine2EndPoint);

        INT32 line2StartPointSide = objLine1.GetPointSide(objLine2.GetStartPoint());
        INT32 line2EndPointSide   = objLine1.GetPointSide(objLine2.GetEndPoint());
        INT32 line1StartPointSide = objLine2.GetPointSide(objLine1.GetStartPoint());
        INT32 line1EndPointSide   = objLine2.GetPointSide(objLine1.GetEndPoint());

        ST_LOG_INFO("1-理想为全0: %d %d %d %d.", line2StartPointSide, line2EndPointSide, line1StartPointSide, line1EndPointSide);

        BOOL isIntersect1 = objLine1.IsIntersectedWithOtherLineSeg(objLine2);
        BOOL isIntersect2 = objLine2.IsIntersectedWithOtherLineSeg(objLine1);

        ST_LOG_INFO("1-应该为全0: %d %d.", isIntersect1, isIntersect2);
    }

    {
        COORDINATE_2D_STRU stLine1StartPoint(77.59, -114.19);
        COORDINATE_2D_STRU stLine1EndPoint(77.66, -114.27);
        COORDINATE_2D_STRU stLine2StartPoint(77.79, -114.42);
        COORDINATE_2D_STRU stLine2EndPoint(77.85, -114.49);

        DIRECTED_LINE_SEGMENT_C objLine1(stLine1StartPoint, stLine1EndPoint);
        DIRECTED_LINE_SEGMENT_C objLine2(stLine2StartPoint, stLine2EndPoint);

        INT32 line2StartPointSide = objLine1.GetPointSide(objLine2.GetStartPoint());
        INT32 line2EndPointSide   = objLine1.GetPointSide(objLine2.GetEndPoint());
        INT32 line1StartPointSide = objLine2.GetPointSide(objLine1.GetStartPoint());
        INT32 line1EndPointSide   = objLine2.GetPointSide(objLine1.GetEndPoint());

        ST_LOG_INFO("2-理想为全0: %d %d %d %d.", line2StartPointSide, line2EndPointSide, line1StartPointSide, line1EndPointSide);

        BOOL isIntersect1 = objLine1.IsIntersectedWithOtherLineSeg(objLine2);
        BOOL isIntersect2 = objLine2.IsIntersectedWithOtherLineSeg(objLine1);

        ST_LOG_INFO("2-应该为全0: %d %d.", isIntersect1, isIntersect2);
    }


    ros::Rate loop_rate(runFrequency);
    while(ros::ok())
    {
        Comm_UptByPeriod();
        ros::spinOnce();


        loop_rate.sleep();
    }

    return 0;
}



