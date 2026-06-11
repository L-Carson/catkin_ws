#include "comm/comm_hd_map/comm_geo_coord_transformer.h"

INT32 GEO_COORD_TRANSFORMER_C::LoadOrigin(const string &strWorkSceneDir)
{
    string strGpsOriginFilePath = strWorkSceneDir + "/location_map/origin_gps_coord.txt";
    FILE *pfFile = fopen(strGpsOriginFilePath.c_str(), "r");
    if (pfFile == NULL) {
        ST_LOG_ERR("Open %s Fail.", strGpsOriginFilePath.c_str());
        return -1;
    }

    GPS_COORD_STRU stOrigin;
    INT32 num = fscanf(pfFile, "%lf %lf %lf", &stOrigin.lat, &stOrigin.lon, &stOrigin.alt);
    fclose(pfFile);

    if (num != 3 || abs(stOrigin.lat) + abs(stOrigin.lon) + abs(stOrigin.alt) < 0.000001) {
        ST_LOG_ERR("File Content Err.");
        return -1;
    }

    SetOrigin(stOrigin);

    return 0;
}

INT32 GEO_COORD_TRANSFORMER_C::SaveOrigin(const std::string &strWorkSceneDir)
{
    string strGpsOriginFilePath = strWorkSceneDir + "/location_map/origin_gps_coord.txt";
    FILE *pfFile = fopen(strGpsOriginFilePath.c_str(), "w");
    if (pfFile == NULL) {
        ST_LOG_ERR("Open File Fail.");
        return -1;
    }

    fprintf(pfFile, "%5.8f %5.8f %5.8f", stOrigin_.lat, stOrigin_.lon, stOrigin_.alt);

    fflush(pfFile);
    fsync(fileno(pfFile));
    fclose(pfFile);

    ST_LOG_INFO("Write Pose(%5.8f %5.8f %5.8f).", stOrigin_.lat, stOrigin_.lon, stOrigin_.alt);

    return 0;
}

void  GEO_COORD_TRANSFORMER_C::SetOrigin( const GPS_COORD_STRU &stOrigin)
{
    isSetOrigin_ = true;
    stOrigin_    = stOrigin;
    objConverter_.Reset(stOrigin_.lat, stOrigin_.lon, stOrigin_.alt);
}

INT32 GEO_COORD_TRANSFORMER_C::GetOrigin(GPS_COORD_STRU &stOrigin)
{
    if (!isSetOrigin_) {
        ST_LOG_ERR("Has Not Set Origin.");
        return -1;
    }

    stOrigin = stOrigin_;

    return -1;
}

INT32 GEO_COORD_TRANSFORMER_C::ToCartersian(const GPS_COORD_STRU &stGlobalPos, COORDINATE_3D_STRU &stLocalPos)
{
    if (!isSetOrigin_) {
        ST_LOG_ERR("Has Not Set Origin.");
        return -1;
    }
    double x, y , z;
    objConverter_.Forward(stGlobalPos.lat, stGlobalPos.lon, stGlobalPos.alt,
                                     x,           y ,            z);
    stLocalPos.SetValue(x, y, z);

    return 0;
}

INT32 GEO_COORD_TRANSFORMER_C::ToLonLatAlt(const COORDINATE_3D_STRU &stLocalPos, GPS_COORD_STRU &stGlobalPos)
{
    if (!isSetOrigin_) {
        ST_LOG_ERR("Has Not Set Origin.");
        return -1;
    }

    objConverter_.Reverse((double)stLocalPos.x, (double)stLocalPos.y, (double)stLocalPos.z,
                               stGlobalPos.lat,     stGlobalPos.lon,      stGlobalPos.alt);

    return 0;
}
