#include "comm/comm_time.h"
#include "comm/comm_map.h"
#include "comm/comm_base.h"
#include "comm/comm_ros_base.h"
#include "comm/comm_msg_init_pose.h"
#include "comm/comm_msg_goal_pose.h"
#include "comm/comm_msg_tool_point.h"

#include <ros/ros.h>

#define TOPIC_NAME_SHOW_POLYGON                     "/test/show/polygon"
#define TOPIC_NAME_SHOW_MAP                         "/test/show/map"


namespace TEST
{

void Test_CreateMapTime(ros::NodeHandle &nh, INT32 runFrequency)
{
    ros::Rate loop_rate(runFrequency);
    while(ros::ok())
    {
        Comm_UptByPeriod();
        ros::spinOnce();

        double time1 = Comm_GetTimeSec();

        GRAY_MAP_C objMap1;
        objMap1.CreateMap(UINT32(100.0 / MAP_RESOLUTION), UINT32(100.0 / MAP_RESOLUTION), 123, MAP_RESOLUTION);

        double time2 = Comm_GetTimeSec();

        GRAY_MAP_C objMap2;
        INT32 rsltOfCopyMap = objMap2.CopyMap(objMap1);
        R_ASSERT(rsltOfCopyMap == 0, NOTHING);

        double time3 = Comm_GetTimeSec();

        /* 3.814697M, CreateTime: 0.001325s, CopyTime: 0.001596s */
        ST_LOG_INFO("MapSize: %fM, CreateTime: %fs, CopyTime: %fs.",
                    float(objMap1.GetImageMemSize()) / (1024.0 * 1024.0),
                    time2 - time1,
                    time3 - time2);

        loop_rate.sleep();
    }
}

void Test_JudgeConnctToMapBorder(ros::NodeHandle &nh, INT32 runFrequency)
{
    INIT_POSE_RECEIVER_C  objChangeMakePolygonSwReceiver(nh);
    GOAL_POSE_RECEIVER_C  objClearMapSwReceiver(nh);
    TOOL_POINT_RECEIVER_C objPolygonPointReceiver(nh);
    OCC_MAP_MSG_SENDER_C  objMapShower(nh, TOPIC_NAME_SHOW_MAP);
    PATH_SENDER_C         objPolygonShower(nh, TOPIC_NAME_SHOW_POLYGON);

    float mapSizeX = 100;
    float mapSizeY = 100;

    GRAY_MAP_C objMap;
    objMap.CreateMap(UINT32(mapSizeX / MAP_RESOLUTION), UINT32(mapSizeY / MAP_RESOLUTION), objMap.GetCostOfNull(), MAP_RESOLUTION);

    BOOL                                         isMakingPolygon = false;
    std::vector<std::vector<COORDINATE_2D_STRU>> vectPolygon;

    objMapShower.SendMsg(objMap);
    objPolygonShower.Send(std::vector<COORDINATE_2D_STRU>());

    ros::Rate loop_rate(runFrequency);
    while(ros::ok())
    {
        Comm_UptByPeriod();
        ros::spinOnce();

        BOOL isNeedUptMap = false;

        COOR_2D_POSE_STRU stNothing;
        INT32 rslt = objChangeMakePolygonSwReceiver.Get(stNothing);
        if (rslt == 0) {
            if (isMakingPolygon) {
                isMakingPolygon = false;
                objPolygonShower.Send(std::vector<COORDINATE_2D_STRU>());
                ST_LOG_INFO("Stop Making Polygon.");

                isNeedUptMap = true;
            } else {
                isMakingPolygon = true;
                vectPolygon.push_back(std::vector<COORDINATE_2D_STRU>());
                ST_LOG_INFO("To Make New Polygon.");
            }
        }

        if (isMakingPolygon) {
            COORDINATE_2D_STRU stPoint;
            rslt = objPolygonPointReceiver.Get(stPoint);
            if (rslt == 0) {
                std::vector<COORDINATE_2D_STRU> &vectPolygonVertice = vectPolygon.back();
                vectPolygonVertice.push_back(stPoint);
                objPolygonShower.Send(vectPolygonVertice);
            }
        }

        rslt = objClearMapSwReceiver.Get(stNothing);
        if (rslt == 0) {
            objMap.SetMapNull();
            objMapShower.SendMsg(objMap);

            isMakingPolygon = false;

            vectPolygon.clear();
            objPolygonShower.Send(std::vector<COORDINATE_2D_STRU>());
        }

        if (isNeedUptMap) {
            objMap.SetMapNull();

            for (std::vector<COORDINATE_2D_STRU> &vectPolygonVertice : vectPolygon) {
                objMap.SetCellsOfLine(vectPolygonVertice, 100);
            }

            double maxSpentTime = 0;
            COORDINATE_2D_STRU stTestPoint;
            for (stTestPoint.x = 0.0; stTestPoint.x < mapSizeX; stTestPoint.x += 0.13) {
                for (stTestPoint.y = 0.0; stTestPoint.y < mapSizeY; stTestPoint.y += 0.17) {

                    double time1 = Comm_GetTimeSec();
                    BOOL isConnectBorder = objMap.IsConnectToMapBorderByNull(stTestPoint);
                    double time2 = Comm_GetTimeSec();
                    maxSpentTime = MAX(time2 - time1, maxSpentTime);

                    ST_LOG_INFO_SAMPLE(1000, "x=%f, y=%f.", stTestPoint.x, stTestPoint.y);

                    if (isConnectBorder) {
                        objMap.SetCellByPos(stTestPoint, objMap.GetCostOfLeft());
                    } else {
                        objMap.SetCellByPos(stTestPoint, objMap.GetCostOfObst());
                    }
                }
            }
            ST_LOG_INFO("Spent Time : %.3f ms", maxSpentTime * 1000.0);

            objMapShower.SendMsg(objMap);
        }

        if (!isMakingPolygon) {
            COORDINATE_2D_STRU stPoint;
            rslt = objPolygonPointReceiver.Get(stPoint);
            if (rslt == 0) {
                objMap.SetMapNull();

                for (std::vector<COORDINATE_2D_STRU> &vectPolygonVertice : vectPolygon) {
                    objMap.SetCellsOfLine(vectPolygonVertice, 100);
                }

                vector<IMAGE_COORDINATE_STRU> vectTrjCells;
                BOOL isConnectBorder = objMap.IsConnectToMapBorderByNull(stPoint, &vectTrjCells);
                ST_LOG_INFO("%f, %f isConnectBorder: %u.", stPoint.x, stPoint.y, isConnectBorder);
                objMap.SetCells(vectTrjCells, objMap.GetCostOfObst());

                objMapShower.SendMsg(objMap);
            }
        }

        loop_rate.sleep();
    }
}

void Test_AreaMap(ros::NodeHandle &nh, INT32 runFrequency)
{
    GOAL_POSE_RECEIVER_C  objGenAreaMapReceiver(nh);
    TOOL_POINT_RECEIVER_C objAreaPointReceiver(nh);
    OCC_MAP_MSG_SENDER_C  objAreaMapShower(nh, TOPIC_NAME_SHOW_MAP);
    PATH_SENDER_C         objAreaShower(nh, TOPIC_NAME_SHOW_POLYGON);

    std::vector<COORDINATE_2D_STRU> vectAreaVertice;

    ros::Rate loop_rate(runFrequency);
    while (ros::ok()) {
        Comm_UptByPeriod();
        ros::spinOnce();

        COORDINATE_2D_STRU stPoint;
        INT32 rslt = objAreaPointReceiver.Get(stPoint);
        if (rslt == 0) {
            vectAreaVertice.push_back(stPoint);

            if (vectAreaVertice.size() > 1) {
                objAreaShower.Send(vectAreaVertice);
            }
        }

        COOR_2D_POSE_STRU stNothing;
        rslt = objGenAreaMapReceiver.Get(stNothing);
        if ((rslt == 0) && !vectAreaVertice.empty()) {
            AREA_MAP_C objAreaMap;
            rslt = objAreaMap.CreateAreaMap(vectAreaVertice, 1.0, 0.1);
            if (rslt == 0) {
                objAreaMapShower.SendMsg(objAreaMap);
                vectAreaVertice.clear();

            } else {
                ST_LOG_ERR("objAreaMap.CreateAreaMap() Fail.");
            }
        }

        loop_rate.sleep();
    }
}


}


int main(int argc, char** argv)
{
    if (argc != 2) {
        ST_LOG_ERR("Para Num(%d) Err.", argc);
        return 1;
    }

    INT32 ctrlCmdNo;
    if (1 != sscanf(argv[1], "%d", &ctrlCmdNo)) {
        ST_LOG_ERR("Cmd(%s) Is Not Digital.", argv[1]);
        return 1;
    }

    ros::init(argc, argv, "test_map");
    ros::NodeHandle nh;
    INT32           runFrequency = 20;

    INT32 rslt = Comm_Init(runFrequency);
    if (0 != rslt) {
        ST_LOG_ERR("Comm_Init() Fail.");
        return 1;
    }

    switch (ctrlCmdNo) {
        case 0:
            TEST::Test_CreateMapTime(nh, runFrequency);
            break;

        case 1:
            TEST::Test_JudgeConnctToMapBorder(nh, runFrequency);
            break;

        case 2:
            TEST::Test_AreaMap(nh, runFrequency);
            break;

        default:
            break;
    }

    return 0;
}



