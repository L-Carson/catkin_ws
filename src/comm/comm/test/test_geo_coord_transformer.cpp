#include "comm/comm_file.h"
#include "comm/comm_base.h"
#include "comm/comm_hd_map/comm_geo_coord_transformer.h"
#include <ros/ros.h>

using namespace std;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "comm_geo_coord_transformer");
    GEO_COORD_TRANSFORMER_C objGeoCoordTransformer;
    if ( 0 != objGeoCoordTransformer.LoadOrigin(WORK_SCENE_FILE_MNG_C().GetWorkSceneDir())) {
        ST_LOG_ERR("Load Origin Fail.");
        return -1;
    }

    COOR_2D_POSE_STRU stLoaclPose(1.0, 0, 0);
    GPS_POSE_STRU     stGlobalPose;

    COORDINATE_3D_STRU stPos(stLoaclPose.stPos.x, stLoaclPose.stPos.y, 0);
    objGeoCoordTransformer.ToLonLatAlt(stPos, stGlobalPose.stCoord);

    stGlobalPose.yawAngle = AdjustAngle(0.5 * PI - stLoaclPose.dirAngle);

    cout<<setprecision(11)
        <<stGlobalPose.stCoord.lat<<" "
        <<stGlobalPose.stCoord.lon<<" "
        <<stGlobalPose.stCoord.alt<<endl;

    return 0;
}

