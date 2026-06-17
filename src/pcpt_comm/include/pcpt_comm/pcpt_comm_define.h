#ifndef __PCPT_COMM_DEFINE_H__
#define __PCPT_COMM_DEFINE_H__

#include "comm_msg/coorLabel.h"
#include "comm/comm_base.h"
#include "comm_extend/comm_base_extend.h"
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include "Eigen/Dense"
#include <float.h>
#include <chrono>

#define PI2                 PI * 2
#define TEN_6               1000000
#define ZERO_6              0.000001
#define CONTINUE(t_)        {SLEEP_M(t_); continue;}
#define INIT_SUCC           ST_LOG_INFO("Init Success.");

#define PCPT_HZ_10                          10
#define PCPT_HZ_20                          20
#define PCPT_HZ_30                          30
#define PCPT_HZ_40                          40
#define PCPT_HZ_50                          50
#define PCPT_HZ_100                         100

#define PCPT_HZ_MAIN_FUN                    PCPT_HZ_40
#define PCPT_HZ_DET_DYNAMIC                 PCPT_HZ_30
#define PCPT_HZ_DET_STATIC                  PCPT_HZ_20
#define PCPT_HZ_OBST_STATE                  PCPT_HZ_50
#define PCPT_HZ_PERIPHERAL                  PCPT_HZ_20
#define PCPT_HZ_ACC                         PCPT_HZ_20

#define PIXEL_ROW_720                       720
#define PIXEL_COL_1280                      1280

#define PIXEL_ROW_1080                      1080
#define PIXEL_COL_1920                      1920

#define PCPT_FRAME_ID_MAP                       "map"
#define PCPT_FRAME_ID_ODOM                      "odom"
#define PCPT_FRAME_ID_CAR                       "base_link"
#define PCPT_FRAME_ID_QR_CODE                   "pcpt_qr_code"              // 二维码
#define PCPT_FRAME_ID_REFLECT_STRIP             "pcpt_reflect_strip"        // 反光条
#define PCPT_FRAME_ID_CHARGING_PILE             "pcpt_charging_pile"        // 充电桩
#define PCPT_FRAME_ID_CHARGING_PILE_REF         "pcpt_charging_pile_ref"    // 充电桩（地图上参考值）
#define PCPT_FRAME_ID_DUSTBIN                   "pcpt_dustbin"              // 垃圾箱
#define PCPT_FRAME_ID_DUZTBIN_REF               "pcpt_dustbin_ref"          // 垃圾箱站点（地图上参考值）
#define PCPT_FRAME_ID_BINSITE                   "pcpt_binsite"              // 垃圾箱站点
#define PCPT_FRAME_ID_EQUIPMENT                 "pcpt_equipment"            // 外部设备位姿（垃圾箱、充电桩）

#define LOC_MARK_INSTANCE                       "Instance"
#define LOC_MARK_MULTIFRAME                     "MultiFrame"
#define LOC_MARK_VISION_TF                      "VisionTf"

#define PCPT_MARK_COMMON_OBST_HOLD              "CommonObstHold"
#define PCPT_MARK_SHORT_OBST_HOLD               "ShortObstHold"
#define PCPT_MARK_BLIND_SPOT_OBST_HOLD          "BlindSpotObstHold"
#define PCPT_MARK_ACCESSIBLE_AREA_DET           "AccessibleAreaDet"
#define PCPT_MARK_ACCESSIBLE_AREA_HOLD          "AccessibleAreaHold"
#define PCPT_MARK_ROAD_EDGE_HOLD                "RoadEdgeHold"
#define PCPT_MARK_CONTACT_EDGE_HOLD             "ContactEdgeHold"

#define PCPT_MARK_ROAD_EDGE                     "RoadEdge"
#define PCPT_MARK_PIPELINE                      "Pipeline"
#define PCPT_MARK_THICK_PIPE                    "ThickPipe"
#define PCPT_MARK_SLOPE_PAD                     "SlopePad"
#define PCPT_MARK_TRAFFIC_CONE                  "TrafficCone"
#define PCPT_MARK_CORD                          "Cord"
#define PCPT_MARK_FISHING_ROD                   "FishingRod"
#define PCPT_MARK_BLACK_VEHICLE                 "BlackVehicle"
#define PCPT_MARK_CAR                           "Car"
#define PCPT_MARK_CORDON                        "Cordon"
#define PCPT_MARK_COVER_HOLLOW                  "CoverHollow"
#define PCPT_MARK_COVER_SOLID                   "CoverSolid"
#define PCPT_MARK_TRASH_BOX                     "TrashBox"

#define PCPT_MARK_LOW_OBST                      "LowObst"
#define PCPT_MARK_UNDERGROUND_OBST              "UndergroundObst"

#define PCPT_MARK_DUSTBIN_FULL                  "DustbinFull"               // 站点垃圾箱已满
#define PCPT_MARK_DUSTBIN_NULL                  "DustbinNull"               // 站点垃圾箱不存在
#define PCPT_MARK_TRASHBOX_LIFT_MOTOR_STICK     "TrashBoxLiftMotorStick"    // 车载垃圾箱抬升不到位
#define PCPT_MARK_TRASHBOX_ROLL_MOTOR_STICK     "TrashBoxRollMotorStick"    // 车载垃圾箱翻斗推出不到位
#define PCPT_MARK_TRASHBOX_ROLL_MOTOR_WITHDRAW  "TrashBoxRollMotorWithdraw" // 车载垃圾箱翻斗收回不到位

#define PCPT_ERROR   std::cout << "\033[1m\033[31m"  // bold red
#define PCPT_WARNING std::cout << "\033[1m\033[33m"  // bold yellow
#define PCPT_INFO    std::cout << "\033[1m\033[32m"  // bold green
#define PCPT_INFOL   std::cout << "\033[32m"         // green
#define PCPT_DEBUG   std::cout << "\033[1m\033[36m"  // bold cyan
#define PCPT_TITLE   std::cout << "\033[1m\033[35m"  // bold magenta
#define PCPT_MSG     std::cout << "\033[1m\033[37m"  // bold white
#define PCPT_REND    "\033[0m" << std::endl

typedef list<COORDINATE_2D_STRU>        LST_COOR;
typedef vector<COORDINATE_2D_STRU>      VST_COOR;

/**
 * @brief 四元数
 *
 */
struct QUAT_STRU {
    float w, x, y, z;
};

/**
 * @brief 点云数据类型
 *
 */
enum POINT_TPYE_ENUM {
    TYPE_NAN = 0,
    TYPE_OLD = 1,   // PointXYZI     4元点结构（32Bytes）
    TYPE_NEW = 2,   // PointXYZIRCDT 8元点结构
};

/**
 * 
 */

struct HIT_MARK_IDX {
    HIT_MARK_IDX() = default;

    HIT_MARK_IDX(INT32 thetaIdx, INT32 phiIdx, INT32 rIdx)
    {
        this->thetaIdx = thetaIdx;
        this->phiIdx = phiIdx;
        this->rIdx = rIdx;
    }

    HIT_MARK_IDX(const HIT_MARK_IDX &other)
    {
        this->thetaIdx = other.thetaIdx;
        this->phiIdx = other.phiIdx;
        this->rIdx = other.rIdx;
    }

    INT32 thetaIdx = -1;
    INT32 phiIdx = -1;
    INT32 rIdx = -1;

    void SetNull() {
        thetaIdx = -1;
        phiIdx = -1;
        rIdx = -1;
    }

     bool IsNull() const {
        return thetaIdx == -1 || phiIdx == -1 || rIdx == -1;
    }

};

/**
 * @brief 4元点云分割结果结构（lidar数据）
 *
 */
struct SEGMENT_RESULT_STRU {
    SEGMENT_RESULT_STRU() {Reset();}

    void SetNull() {
        pGroundPoints   = nullptr;
        pNoGroundPoints = nullptr;
    }

    bool IsNull() const {
        return pGroundPoints == nullptr || pNoGroundPoints == nullptr;
    }

    bool IsEmpty() const {
        return pGroundPoints->empty() && pNoGroundPoints->empty();
    }

    void Clear() {
        if (pGroundPoints)   pGroundPoints->clear();
        if (pNoGroundPoints) pNoGroundPoints->clear();
    }

    bool IsClear() const {
        return IsEmpty();
    }

    void Reset() {
        pGroundPoints.reset(new PointCloudXYZI());
        pNoGroundPoints.reset(new PointCloudXYZI());
    }

    void Copy(const SEGMENT_RESULT_STRU& stSegData) {
        strLidarName     =  stSegData.strLidarName;
        strLidarType     =  stSegData.strLidarType;
        *pGroundPoints   = *stSegData.pGroundPoints;
        *pNoGroundPoints = *stSegData.pNoGroundPoints;
    }

    uint64_t Size() const {
        uint64_t size = 0;
        if (pGroundPoints)   size += pGroundPoints->size();
        if (pNoGroundPoints) size += pNoGroundPoints->size();
        return size;
    }

    int                 frameID = 0;
    double              time = 0.0;
    std::string         strLidarName;
    std::string         strLidarType;
    PointCloudXYZI::Ptr pGroundPoints;
    PointCloudXYZI::Ptr pNoGroundPoints;
};

/**
 * @brief 3元点云分割结果结构（深度相机数据）
 *
 */
struct V_SEGMENT_RESULT_STRU {
    V_SEGMENT_RESULT_STRU(){
        pGroundPoints.reset(new pcl::PointCloud<pcl::PointXYZ>);
        pNoGroundPoints.reset(new pcl::PointCloud<pcl::PointXYZ>);
    };
    pcl::PointCloud<pcl::PointXYZ>::Ptr pGroundPoints;
    pcl::PointCloud<pcl::PointXYZ>::Ptr pNoGroundPoints;
};

/**
 * @brief 图片像素坐标结构
 *
 */
struct IMG_COOR_STRU {
    IMG_COOR_STRU(){}
    IMG_COOR_STRU(const int x, const int y) {
        SetValue(x, y);
    }

    void SetValue(const int x, const int y) {
        this->x = x;
        this->y = y;
    }

    int x;
    int y;
};

/**
 * @brief 图片矩形框结构
 *
 */
struct IMG_RECT_STRU {
    IMG_RECT_STRU() {SetValue(0, 0, 0, 0);}
    IMG_RECT_STRU(const int minX, const int maxX, const int minY, const int maxY) {
        SetValue(minX, maxX, minY, maxY);
    }

    void SetValue(const int minX, const int maxX, const int minY, const int maxY) {
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
    }

    int Width()  const {return maxX - minX;}
    int Height() const {return maxY - minY;}
    int Area()   const {return Width() * Height();}
    int x()      const {return (minX + maxX) * 0.5;}
    int y()      const {return (minY + maxY) * 0.5;}
    COORDINATE_2D_STRU Center() const {return COORDINATE_2D_STRU(x(), y());}

    vector<IMG_COOR_STRU> GetImgCornerPoints() const {
        vector<IMG_COOR_STRU> vstCornerPoints;
        vstCornerPoints.push_back(IMG_COOR_STRU(minX, minY));
        vstCornerPoints.push_back(IMG_COOR_STRU(minX, maxY));
        vstCornerPoints.push_back(IMG_COOR_STRU(maxX, maxY));
        vstCornerPoints.push_back(IMG_COOR_STRU(maxX, minY));
        return vstCornerPoints;
    }

    vector<COORDINATE_2D_STRU> CornerPoints() const {
        vector<COORDINATE_2D_STRU> vstCornerPoints;
        vstCornerPoints.push_back(COORDINATE_2D_STRU(minX, minY));
        vstCornerPoints.push_back(COORDINATE_2D_STRU(minX, maxY));
        vstCornerPoints.push_back(COORDINATE_2D_STRU(maxX, maxY));
        vstCornerPoints.push_back(COORDINATE_2D_STRU(maxX, minY));
        return vstCornerPoints;
    }

    int minX;
    int maxX;
    int minY;
    int maxY;
};

/**
 * @brief 视觉矩形框目标结构
 *
 */
struct V_RECT_STRU {
    V_RECT_STRU(){}
    V_RECT_STRU(const LABEL_ENUM lable) {this->label = lable;}

    int Width()  const {return stRect.Width();}
    int Height() const {return stRect.Height();}
    int Area()   const {return stRect.Area();}
    int x()      const {return stRect.x();}
    int y()      const {return stRect.y();}
    int minX()   const {return stRect.minX;}
    int maxX()   const {return stRect.maxX;}
    int minY()   const {return stRect.minY;}
    int maxY()   const {return stRect.maxY;}

    LABEL_ENUM          label{TAG_UNKNOWN};
    float               score{0.0};
    IMG_RECT_STRU       stRect;
    vector<float>       feature;
};

/**
 * @brief 视觉像素区域目标结构
 *
 */
struct V_PIXEL_STRU {
    int x() const {return stImg.x;}
    int y() const {return stImg.y;}

    LABEL_ENUM          label{TAG_UNKNOWN};
    float               score{0.0};
    IMG_COOR_STRU       stImg;
};

/**
 * @brief 视觉像素区域目标结构
 *
 */
struct V_AREA_STRU {
    void push_back(const V_PIXEL_STRU& stPixel) {lstPixel.push_back(stPixel);}
    size_t size() const {return lstPixel.size();}

    LABEL_ENUM          label{TAG_UNKNOWN};
    //float               score{0.0};
    list<V_PIXEL_STRU>  lstPixel;
};

/**
 * @brief 视觉3维空间平面目标组结构
 *
 */
struct V_COOR_STRU {
    float x() const {return stCoor.x;}
    float y() const {return stCoor.y;}

    LABEL_ENUM          label{TAG_UNKNOWN};
    float               score{0.0};
    COORDINATE_2D_STRU  stCoor;
};

/**
 * @brief 视觉3维空间平面目标组结构
 *
 */
struct V_COORS_STRU {
    void push_back(const COORDINATE_2D_STRU& stCoor) {lstCoor.push_back(stCoor);}
    void clear() {lstCoor.clear();}
    bool empty() const {return lstCoor.empty();}
    size_t size() const {return lstCoor.size();}

    LABEL_ENUM          label{TAG_UNKNOWN};
    float               score{0.0};
    LST_COOR            lstCoor;
};

/**
 * @brief 视觉目标结构
 *
 */
template<typename T>
struct VISION_OBJ_STRU {
    /**
     * T: V_RECT_STRU
     *    V_PIXEL_STRU
     *    V_AREA_STRU
     *    V_COOR_STRU
     *    V_COORS_STRU
     */

    void reset() {
        stamp = 0.0;
        data.clear();
    }

    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    void resize(uint32_t size) {
        data.resize(size);
    }

    void clear() {
        data.clear();
    }

    void push_back(const T& vst) {
        data.push_back(vst);
    }

    T& back() {
        return data.back();
    }

    void operator += (const VISION_OBJ_STRU& st) {
        stamp     = st.stamp;
        strCamera = st.strCamera;
        data.insert(data.end(), st.data.begin(), st.data.end());
    }

    double          stamp{0.0};
    string          strCamera;
    list<T>         data;
};

/**
 * @brief 视觉采用的目标结构
 *
 */
struct VISION_USAGE_STRU {
    void reset() {
        stamp = 0.0;
        data.clear();
    }

    void clear() {
        data.clear();
    }

    size_t size() const {
        return data.size();
    }

    void push_back(const COORDINATE_2D_STRU& stCoor) {
        data.push_back(stCoor);
    }

    void operator += (const VISION_USAGE_STRU& st) {
        data.insert(data.end(), st.data.begin(), st.data.end());
    }

    double      stamp{0.0};
    LABEL_ENUM  label{TAG_UNKNOWN};
    uint8_t     id{0};
    LST_COOR    data;
};

/**
 * @brief 旋转矩形结构
 *
 */
struct ROTATED_RECT_STRU {
    float x;        //center x
    float y;        //center y
    float width;
    float height;
    float angle;    //radian

    ROTATED_RECT_STRU() {
        SetZero();
    }

    ROTATED_RECT_STRU(const ROTATED_RECT_STRU& stRect) {
        SetValue(stRect.x, stRect.y, stRect.width, stRect.height, stRect.angle);
    }

    ROTATED_RECT_STRU(const float x, const float y, const float width, const float height, const float angle) {
        SetValue(x, y, width, height, angle);
    }

    void SetZero() {
        SetValue(0, 0, 0, 0, 0);
    }

    void SetValue(const float x, const float y, const float width, const float height, const float angle) {
        this->x      = x;
        this->y      = y;
        this->width  = width;
        this->height = height;
        this->angle  = angle;
    }

    bool IsRectangle(const vector<COORDINATE_2D_STRU>& vstCoors) const {
        if (vstCoors.size() != 4) {
            ST_LOG_ERR("Not a quadrangle.");    //不是四边形
            return -1;
        }

        /* 定义向量 */
        COORDINATE_2D_STRU vector0(vstCoors[0].x - vstCoors[1].x, vstCoors[0].y - vstCoors[1].y);
        COORDINATE_2D_STRU vector1(vstCoors[1].x - vstCoors[2].x, vstCoors[1].y - vstCoors[2].y);
        COORDINATE_2D_STRU vector2(vstCoors[2].x - vstCoors[3].x, vstCoors[2].y - vstCoors[3].y);
        COORDINATE_2D_STRU vector3(vstCoors[3].x - vstCoors[0].x, vstCoors[3].y - vstCoors[0].y);

        /* 矩形：对边平行、临边垂直 */
        if (SIG(vector0.x * vector2.y - vector0.y * vector2.x) ||   //对边平行
            SIG(vector1.x * vector3.y - vector1.y * vector3.x) ||   //对边平行
            SIG(vector0.x * vector0.y + vector1.x * vector1.y)) {   //邻边垂直
            ST_LOG_ERR("Not a rectangular.");   //不是矩形
            return false;
        }
         return true;
    }

    bool IsIn(float x, float y) const {
        double hw = 0.5 * this->width;
        double hh = 0.5 * this->height;
        double cosAngle = cos(this->angle);
        double sinAngle = sin(this->angle);
        double nTempX = this->x + (x - this->x) * cosAngle + (y - this->y) * sinAngle;
        double nTempY = this->y - (x - this->x) * sinAngle + (y - this->y) * cosAngle;

        if (nTempX > this->x - hw && nTempX < this->x + hw &&
            nTempY > this->y - hh && nTempY < this->y + hh)
            return true;
        return false;
    }

    template<typename T>
    bool IsIn(const T pos) const {
        return IsIn(pos.x, pos.y);
    }

    COORDINATE_2D_STRU CenterPoint() const {
        return COORDINATE_2D_STRU(x, y);
    }

    /* 角点-->旋转矩形 */
    int CornerPoints2RotatedRect(vector<COORDINATE_2D_STRU>& vstCoor) {
        if (!IsRectangle(vstCoor)) return -1;
        SetZero();
        for (const COORDINATE_2D_STRU& stCoor : vstCoor) {
            this->x += stCoor.x;
            this->y += stCoor.y;
        }
        this->x /= vstCoor.size();
        this->y /= vstCoor.size();
        this->width  = sqrt(vstCoor[0].CalcDistSqu(vstCoor[1]));
        this->height = sqrt(vstCoor[0].CalcDistSqu(vstCoor[3]));
        this->angle  = atan((vstCoor[1].y - vstCoor[0].y) / (vstCoor[1].x - vstCoor[0].x));
        return 0;
    }

    /* 旋转矩形-->角点 */
    vector<COORDINATE_2D_STRU> RotatedRect2CornerPoints() const {
        float hw = 0.5 * width;
        float hh = 0.5 * height;
        float ca = cos(angle);
        float sa = sin(angle);

        COORDINATE_2D_STRU stCoor2D;
        COORDINATE_2D_STRU stPoint;
        #define RATATED_RECT(stPoint)                           \
            stCoor2D = stPoint;                                 \
            stPoint.x = ca * stCoor2D.x - sa * stCoor2D.y + x;  \
            stPoint.y = sa * stCoor2D.x + ca * stCoor2D.y + y;  \

        vector<COORDINATE_2D_STRU> vstPoint;

        stPoint.x = -hw;
        stPoint.y = -hh;
        RATATED_RECT(stPoint);
        vstPoint.push_back(stPoint);

        stPoint.x = -hw;
        stPoint.y =  hh;
        RATATED_RECT(stPoint);
        vstPoint.push_back(stPoint);

        stPoint.x = hw;
        stPoint.y = hh;
        RATATED_RECT(stPoint);
        vstPoint.push_back(stPoint);

        stPoint.x =  hw;
        stPoint.y = -hh;
        RATATED_RECT(stPoint);
        vstPoint.push_back(stPoint);

        return vstPoint;
    }

    /* 外接矩形 */
    RECTANGLE_STRU BoundingRect() const {
        vector<COORDINATE_2D_STRU> vstCoor = RotatedRect2CornerPoints();
        RECTANGLE_STRU stRect(FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX);
        for (const COORDINATE_2D_STRU& stCoor : vstCoor) {
            stRect.minX = MIN(stRect.minX, stCoor.x);
            stRect.minY = MIN(stRect.minY, stCoor.y);
            stRect.maxX = MAX(stRect.maxX, stCoor.x);
            stRect.maxY = MAX(stRect.maxY, stCoor.y);
        }
        return stRect;
    }
};

/**
 * @brief Box信息结构
 *
 */
struct BOX_INFO_STRU {
    double              timeStamp{0.0};         // time stamp
    uint32_t            boxID{0};               // id
    LABEL_ENUM          enClass{TAG_UNKNOWN};   // enClass
    float               minZ{0.0};              // min z
    float               maxZ{0.0};              // max z
    ROTATED_RECT_STRU   stRotatedRect;          // rotated rectangle
    vector<float>       feature;                // box feature

    float x()     const {return stRotatedRect.x;}
    float y()     const {return stRotatedRect.y;}
    float angle() const {return stRotatedRect.angle;}
};

/**
 * @brief 定义平面参数
 *
 */
struct PLANE_PARAM_STRU {
    PLANE_PARAM_STRU() {SetZero();}
    PLANE_PARAM_STRU(const float coeffX, const float coeffY, const float coeffZ, const float constH) {
        SetVale(coeffX, coeffY, coeffZ, constH);
    }

    void SetVale(const float coeffX, const float coeffY, const float coeffZ, const float constH) {
        this->coeffX = coeffX;
        this->coeffY = coeffY;
        this->coeffZ = coeffZ;
        this->constH = constH;
    }

    /* 平面参数置为0 */
    void SetZero() {
        SetVale(0, 0, 0, 0);
    }

    /* 平面参数是否全为0 */
    bool IsZero() {
        return SIG(coeffX) || SIG(coeffY) || SIG(coeffZ) || SIG(constH);
    }

    /* 已知y和z, 计算平面点x */
    float PlaneX(const float y, const float z) const {
        if (!SIG(coeffX)) return 0;
        return (-constH - coeffY * y - coeffZ * z) / coeffX;
    }

    /* 已知x和z, 计算平面点y */
    float PlaneY(const float x, const float z) const {
        if (!SIG(coeffY)) return 0;
        return (-constH - coeffX * x - coeffZ * z) / coeffY;
    }

    /* 已知x和y, 计算平面点z */
    float PlaneZ(const float x, const float y) const {
        if (!SIG(coeffZ)) return 0;
        return (-constH - coeffX * x - coeffY * y) / coeffZ;
    }

    float coeffX;
    float coeffY;
    float coeffZ;
    float constH;
};

/**
 * @brief 定义矩形平面区域
 *
 */
struct PLANE_REGION_STRU {
    PLANE_REGION_STRU() {}
    PLANE_REGION_STRU(const RECTANGLE_STRU& stRect, const PLANE_PARAM_STRU& stPlane) {
        this->stRect  = stRect;
        this->stPlane = stPlane;
    }

    RECTANGLE_STRU      stRect;
    PLANE_PARAM_STRU    stPlane;
};

/**
 * @brief 2D运动信息结构
 *
 */
struct PVS_INFO_STRU {
    double                  timeStamp;
    vector<POS_VEL_STRU>    vstPosVel;
};

/**
 * @brief 点云聚类结果类型
 *
 */
struct LIDAR_CLUSTER_STRU {
    LIDAR_CLUSTER_STRU() : timeStamp(0.0) {}

    double                timeStamp;   // 数据时间戳
    vector<BOX_INFO_STRU> vstBoxInfo;  // 体素集合
};

/**
 * @brief Radar有效检测结果类型
 *
 */
struct RADAR_DETECT_STRU {
    RADAR_DETECT_STRU() : timeStamp(0.0) {}

    double             timeStamp;   // 数据时间戳
    list<POS_VEL_STRU> lstPosVel;   // 2D运动状态链表
};

/**
 * @brief 轨迹点
 *
 */
struct TRAJECTORY_POINT_STRU {
    float CalcDistSqu(const TRAJECTORY_POINT_STRU &stAnotherPoint) const {
        float diffX = x - stAnotherPoint.x;
        float diffY = y - stAnotherPoint.y;
        return (diffX * diffX + diffY * diffY);
    }

    double DiffStamp(const TRAJECTORY_POINT_STRU &stAnotherPoint) const {
        return fabs(timeStamp - stAnotherPoint.timeStamp);
    }

    double timeStamp;
    double x;
    double y;
    float  angle;
};

/**
 * @brief 目标跟踪
 *
 */
struct OBJECT_TRACK_STRU {
    uint32_t Id()    const {return stBox.boxID;}
    float    VelX()  const {return stPosVel.velX;}
    float    VelY()  const {return stPosVel.velY;}
    double   Stamp() const {return stBox.timeStamp;}
    bool     HasFeature() const {return !stBox.feature.empty();}

    void SetBoxId(const uint32_t id) {stBox.boxID = id;}
    void SetPosVelId(const uint32_t id) {stPosVel.objectID = id;}
    void SetId(const uint32_t id) {
        SetBoxId(id);
        SetPosVelId(id);
    }

    void MergeTrajectoryAtHead(const list<TRAJECTORY_POINT_STRU>& lstTrajectory) {
        this->lstTrajectory.insert(this->lstTrajectory.begin(), lstTrajectory.begin(), lstTrajectory.end());
    }

    void MergeTrajectoryAtTail(const list<TRAJECTORY_POINT_STRU>& lstTrajectory) {
        this->lstTrajectory.insert(this->lstTrajectory.end(), lstTrajectory.begin(), lstTrajectory.end());
    }

    BOX_INFO_STRU               stBox;
    POS_VEL_STRU                stPosVel;
    list<TRAJECTORY_POINT_STRU> lstTrajectory;
};

/**
 * @brief 锥体结构
 *
 */
enum CONE_TYPE_ENUM {
    CONE_RECT,      //矩形锥
    CONE_CIRCLE,    //圆形锥
};

/**
 * @brief 定义视锥
 *
 */
struct CONE_STRU {
    CONE_STRU() {
        enType     =  CONE_TYPE_ENUM::CONE_RECT;
        angleH_min =  FLT_MAX;
        angleH_max = -FLT_MAX;
        angleV_min =  FLT_MAX;
        angleV_max = -FLT_MAX;
        stOrigin.SetZero();
    }

    float GetViewAngleH() {
        return angleH_max - angleH_min;
    }

    float GetViewAngleV() {
        return angleV_max - angleV_min;
    }

    CONE_TYPE_ENUM enType;
    float angleH_min;
    float angleH_max;
    float angleV_min;
    float angleV_max;
    COORDINATE_3D_STRU stOrigin;
};

/**
 * @brief 多层障碍
 *
 */
struct MULTILAYER_OBST_STRU {
    LST_COOR            lstObstTop;
    LST_COOR            lstObstMid;
    LST_COOR            lstObstDown;

    MULTILAYER_OBST_STRU () {}
    MULTILAYER_OBST_STRU (const LST_COOR& lstObstTop, const LST_COOR& lstObstMid, const LST_COOR& lstObstDown) {
        this->lstObstTop  = lstObstTop;
        this->lstObstMid  = lstObstMid;
        this->lstObstDown = lstObstDown;
    }

    void clear() {
        lstObstTop.clear();
        lstObstMid.clear();
        lstObstDown.clear();
    }

    void clear(const OBST_LAYER_ENUM layer) {
        switch (layer) {
            case OBST_LAYER_TOP:  lstObstTop.clear(); break;
            case OBST_LAYER_MID:  lstObstMid.clear(); break;
            case OBST_LAYER_DOWN: lstObstDown.clear(); break;
            default: break;
        }
    }

    void reset() {clear();}
    void reset(const OBST_LAYER_ENUM layer) {clear(layer);}
    void reset(const MULTILAYER_OBST_STRU& stObst) {
        lstObstTop  = stObst.lstObstTop;
        lstObstMid  = stObst.lstObstMid;
        lstObstDown = stObst.lstObstDown;
    }

    void reset(const OBST_LAYER_ENUM layer, const LST_COOR& lstPos) {
        switch (layer) {
            case OBST_LAYER_TOP:  lstObstTop  = lstPos; break;
            case OBST_LAYER_MID:  lstObstMid  = lstPos; break;
            case OBST_LAYER_DOWN: lstObstDown = lstPos; break;
            default: break;
        }
    }

    void push_back(const OBST_LAYER_ENUM layer, const COORDINATE_2D_STRU& stPos) {
        switch(layer) {
            case OBST_LAYER_TOP : lstObstTop.push_back(stPos); break;
            case OBST_LAYER_MID : lstObstMid.push_back(stPos); break;
            case OBST_LAYER_DOWN: lstObstDown.push_back(stPos); break;
            default: break;
        }
    }

    void insert(const MULTILAYER_OBST_STRU& stObst) {
        lstObstTop.insert( lstObstTop.end(),  stObst.lstObstTop.begin(),  stObst.lstObstTop.end());
        lstObstMid.insert( lstObstMid.end(),  stObst.lstObstMid.begin(),  stObst.lstObstMid.end());
        lstObstDown.insert(lstObstDown.end(), stObst.lstObstDown.begin(), stObst.lstObstDown.end());
    }

    void insert(const OBST_LAYER_ENUM layer, const LST_COOR& lstPos) {
        switch(layer) {
            case OBST_LAYER_TOP:  lstObstTop.insert( lstObstTop.end(),  lstPos.begin(), lstPos.end()); break;
            case OBST_LAYER_MID:  lstObstMid.insert( lstObstMid.end(),  lstPos.begin(), lstPos.end()); break;
            case OBST_LAYER_DOWN: lstObstDown.insert(lstObstDown.end(), lstPos.begin(), lstPos.end()); break;
            default: break;
        }
    }

    void insert(const OBST_LAYER_ENUM layer, const VST_COOR& vstPos) {
        switch(layer) {
            case OBST_LAYER_TOP:  lstObstTop.insert( lstObstTop.end(),  vstPos.begin(), vstPos.end()); break;
            case OBST_LAYER_MID:  lstObstMid.insert( lstObstMid.end(),  vstPos.begin(), vstPos.end()); break;
            case OBST_LAYER_DOWN: lstObstDown.insert(lstObstDown.end(), vstPos.begin(), vstPos.end()); break;
            default: break;
        }
    }

    size_t size() const {return lstObstTop.size() + lstObstMid.size() + lstObstDown.size();}
    size_t size(const OBST_LAYER_ENUM layer) const {
        size_t len = 0;
        switch(layer) {
            case OBST_LAYER_TOP:  len = lstObstTop.size();  break;
            case OBST_LAYER_MID:  len = lstObstMid.size();  break;
            case OBST_LAYER_DOWN: len = lstObstDown.size(); break;
            default: break;
        }
        return len;
    }

    bool empty() const {return 0 == size();}

    LST_COOR* LayerObst(const OBST_LAYER_ENUM layer) {
        switch (layer) {
            case OBST_LAYER_TOP:  return &lstObstTop;
            case OBST_LAYER_MID:  return &lstObstMid;
            case OBST_LAYER_DOWN: return &lstObstDown;
            default: return nullptr;
        }
    }

    const LST_COOR* LayerObst(const OBST_LAYER_ENUM layer) const {
        switch (layer) {
            case OBST_LAYER_TOP:  return &lstObstTop;
            case OBST_LAYER_MID:  return &lstObstMid;
            case OBST_LAYER_DOWN: return &lstObstDown;
            default: return nullptr;
        }
    }

    LST_COOR LayerMergeToObst() const {
        LST_COOR stObst;
        stObst.insert(stObst.end(), lstObstTop.begin(),  lstObstTop.end());
        stObst.insert(stObst.end(), lstObstMid.begin(),  lstObstMid.end());
        stObst.insert(stObst.end(), lstObstDown.begin(), lstObstDown.end());
        return stObst;
    }

    void LayerMergeToObst(LST_COOR& stObst) const {
        stObst.insert(stObst.end(), lstObstTop.begin(),  lstObstTop.end());
        stObst.insert(stObst.end(), lstObstMid.begin(),  lstObstMid.end());
        stObst.insert(stObst.end(), lstObstDown.begin(), lstObstDown.end());
    }

    vector<OBST_LAYER_ENUM> Layers() const {
        vector<OBST_LAYER_ENUM> vLayer;
        for (const string& str : ObstLayer) {
            if (str.empty()) continue;
            int layer = ElementId(ObstLayer, str);
            if (layer == -1) continue;
            vLayer.push_back(OBST_LAYER_ENUM(layer));
        }
        if (vLayer.size() != ObstLayer.size() - 1) {
            ST_LOG_ERR("Failed to get the pnp obst layer number(%lu).", vLayer.size());
            return vector<OBST_LAYER_ENUM>{};
        }
        return vLayer;
    }
};

/**
 * @brief 感知障碍
 *
 */
struct PCPT_OBST_STRU {
    int frameID = 0;
    double stamp{0};                                                //统一时间
    COOR_3D_POSE_STRU stCarPose;                                    //统一位姿
    unordered_map<OBST_LABEL_ENUM, MULTILAYER_OBST_STRU> umObst;    //带有类别的分层障碍

    PCPT_OBST_STRU() {}
    PCPT_OBST_STRU(const double stamp) {
        this->stamp = stamp;
    }
    PCPT_OBST_STRU(const double stamp, const COOR_3D_POSE_STRU& stCarPose) {
        this->stamp     = stamp;
        this->stCarPose = stCarPose;
    }
    PCPT_OBST_STRU(const int frameID, const double stamp, const COOR_3D_POSE_STRU& stCarPose) {
        this->frameID   = frameID;
        this->stamp     = stamp;
        this->stCarPose = stCarPose;
    }

    float x()   const {return stCarPose.x();}
    float y()   const {return stCarPose.y();}
    float yaw() const {return stCarPose.yaw();}

    bool isExist(const OBST_LABEL_ENUM label) const {return umObst.find(label) != umObst.end();}    //是否存在

    void clear() {umObst.clear();}

    void clear(const OBST_LABEL_ENUM label) {
        if (!isExist(label)) return;
        umObst[label].clear();
    }

    void clear(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer) {
        if (!isExist(label)) return;
        umObst[label].clear(layer);
    }

    void reset(const OBST_LABEL_ENUM label) {clear(label);}
    void reset(const OBST_LABEL_ENUM label, const MULTILAYER_OBST_STRU& stObst) {umObst[label] = stObst;}
    void reset(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer, const LST_COOR& lstPos) {umObst[label].reset(layer, lstPos);}

    void push_back(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer, const COORDINATE_2D_STRU& stPos) {
        umObst[label].push_back(layer, stPos);
    }

    void emplace(const OBST_LABEL_ENUM label, const MULTILAYER_OBST_STRU& stLayerObst) {
        umObst.emplace(label, stLayerObst);
    }

    void insert(const OBST_LABEL_ENUM label, const MULTILAYER_OBST_STRU& stObst) {
        umObst[label].insert(stObst);
    }

    void insert(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer, const LST_COOR& lstPos) {
        umObst[label].insert(layer, lstPos);
    }

    void insert(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer, const VST_COOR& vstPos) {
        umObst[label].insert(layer, vstPos);
    }

    size_t size() const {return umObst.size();}

    size_t size(const OBST_LABEL_ENUM label) const {
        if (!isExist(label)) {
            printf("[ERR]PCPT_OBST_STRU size() : Invalid label:%d.\n", label);
            return 0;
        }
        return umObst.at(label).size();
    }

    size_t size(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer) const {
        if (!isExist(label)) {
            printf("[ERR]PCPT_OBST_STRU size() : Invalid label:%d.\n", label);
            return 0;
        }
        return umObst.at(label).size(layer);
    }

    size_t ObstSize() const {
        size_t size = 0;
        for (const auto& pair : umObst) size += pair.second.size();
        return size;
    }

    bool empty()                                                         const {return 0 == size();}
    bool empty(const OBST_LABEL_ENUM label)                              const {return 0 == size(label);}
    bool empty(const OBST_LABEL_ENUM label, const OBST_LAYER_ENUM layer) const {return 0 == size(label, layer);}
    bool ObstEmpty()                                                     const {return 0 == ObstSize();}

    MULTILAYER_OBST_STRU& operator [] (const OBST_LABEL_ENUM label) {
        return umObst[label];   //如果label不存在，会自动创建
    }

    MULTILAYER_OBST_STRU* LabelData(const OBST_LABEL_ENUM label) {
        if (!isExist(label)) return nullptr;
        return &umObst.at(label);
    }

    const MULTILAYER_OBST_STRU* LabelData(const OBST_LABEL_ENUM label) const {
        if (!isExist(label)) return nullptr;
        return &umObst.at(label);
    }

    vector<OBST_LAYER_ENUM> Layers() const {
        if (empty()) return vector<OBST_LAYER_ENUM>{};
        const MULTILAYER_OBST_STRU& stLayerObst = umObst.begin()->second;
        return stLayerObst.Layers();
    }

    /* 融合多类别障碍，融合后只分层，无类别 */
    void MergeToLayerObst(MULTILAYER_OBST_STRU& stLayerObst) const {
        for (const auto& pair : umObst) stLayerObst.insert(pair.second);
    }

    /* 融合多类别障碍，融合后无分层，无类别 */
    void MergeToObst(LST_COOR& stObst) const {
        for (const auto& pair : umObst) {
            const MULTILAYER_OBST_STRU& stLayerObst = pair.second;
            stLayerObst.LayerMergeToObst(stObst);
        }
    }
};

/**
 * @brief 障碍状态
 *
 */
struct OBST_STATE_STRU {
    OBST_LABEL_ENUM     enLabel{OBST_LABEL_UNKNOWN};    //障碍类别
    float               value{0};
    COORDINATE_2D_STRU  stPos;

    OBST_STATE_STRU(){}

    OBST_STATE_STRU(const COORDINATE_2D_STRU& stPos) {
        this->stPos   = stPos;
    }

    OBST_STATE_STRU(const OBST_LABEL_ENUM enLabel, const COORDINATE_2D_STRU& stPos) {
        this->enLabel = enLabel;
        this->stPos   = stPos;
    }

    OBST_STATE_STRU(const OBST_LABEL_ENUM enLabel, const float value, const COORDINATE_2D_STRU& stPos) {
        this->enLabel = enLabel;
        this->value   = value;
        this->stPos   = stPos;
    }

    float x() const {return stPos.x;}
    float y() const {return stPos.y;}
};

/**
 * @brief 定义三维空间向量
 *
 */
struct PCPT_VECTOR_STRU {
    PCPT_VECTOR_STRU() {}
    PCPT_VECTOR_STRU(const float x, const float y, const float z) {
        SetValue(x, y, z);
    }

    PCPT_VECTOR_STRU(const COORDINATE_3D_STRU& stOrigin, const COORDINATE_3D_STRU& stEnd) {
        SetValue(stOrigin, stEnd);
    }

    void SetValue(const float x, const float y, const float z) {
        this->x = x;
        this->y = y;
        this->z = z;

        this->stOrigin.SetValue(0, 0, 0);
        this->stEnd.SetValue(x, y, z);
    }

    void SetValue(const COORDINATE_3D_STRU& stOrigin, const COORDINATE_3D_STRU& stEnd) {
        this->x = stEnd.x - stOrigin.x;
        this->y = stEnd.y - stOrigin.y;
        this->z = stEnd.z - stOrigin.z;

        this->stOrigin = stOrigin;
        this->stEnd    = stEnd;
    }

    /* 向量归一化 */
    void Normalization() {
        double sqrtV = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        if (SIG(sqrtV)) {
            x /= sqrtV;
            y /= sqrtV;
            z /= sqrtV;
        }
    }

    float x{0};
    float y{0};
    float z{0};
    COORDINATE_3D_STRU stOrigin{0, 0, 0};   //向量原点
    COORDINATE_3D_STRU stEnd{0, 0, 0};      //向量端点
};

/**
 * @brief 定义二维空间向量
 *
 */
struct PCPT_2D_VECTOR_STRU {
    PCPT_2D_VECTOR_STRU() {}

    PCPT_2D_VECTOR_STRU(const COORDINATE_2D_STRU& stOrigin, const float len, const float dirAngle) {
        SetValue(stOrigin, len, dirAngle);
    }

    PCPT_2D_VECTOR_STRU(const COORDINATE_2D_STRU& stOrigin, const COORDINATE_2D_STRU& stEnd) {
        SetValue(stOrigin, stEnd);
    }

    void SetValue(const COORDINATE_2D_STRU& stOrigin, const float len, const float dirAngle) {
        this->len      = len;
        this->dirAngle = dirAngle;
        this->stOrigin = stOrigin;
        this->stEndPos.SetValue(stOrigin.x + len * cos(dirAngle), stOrigin.y + len * sin(dirAngle));
    }

    void SetValue(const COORDINATE_2D_STRU& stOrigin, const COORDINATE_2D_STRU& stEnd) {
        this->len      = hypot(stEnd.y - stOrigin.y, stEnd.x - stOrigin.x);
        this->dirAngle = atan2(stEnd.y - stOrigin.y, stEnd.x - stOrigin.x);
        this->stOrigin = stOrigin;
        this->stEndPos = stEnd;
    }

    /* 判断点是否在向量的左侧 */
    template<typename T>
    bool IsOnLeft(const T& stPos) const {
        double cross = (stEndPos.x - stOrigin.x) * (stPos.y - stOrigin.y)
                     - (stEndPos.y - stOrigin.y) * (stPos.x - stOrigin.x);
        //if (SIG(cross) == 0) return 0; //处理共线情况
        return cross > 0;
    }

    float len{0};
    float dirAngle{0};
    COORDINATE_2D_STRU stOrigin{0, 0};  //向量原点
    COORDINATE_2D_STRU stEndPos{0, 0};  //向量端点
};

/**
 * @brief 圆形环带扇区
 *
 */
struct PCPT_LOOP_SECTION_STRU {
    PCPT_LOOP_SECTION_STRU(){}
    PCPT_LOOP_SECTION_STRU(const float minRadius, const float maxRadius, const float minAngle, const float maxAngle) {
        SetValue(minRadius, maxRadius, minAngle, maxAngle);
    }

    bool IsIn(const float x, const float y) {
        float angle = atan2(y, x);
        if (angle < minAngle) return false;
        if (angle > maxAngle) return false;

        float radius = pow(x, 2) + pow(y, 2);
        if (radius < pow(minRadius, 2)) return false;
        if (radius > pow(maxRadius, 2)) return false;

        return true;
    }

    bool IsValid() {
        if (minRadius < 0)          return false;
        if (minRadius >= maxRadius) return false;
        if (minAngle  >= maxAngle)  return false;
        return true;
    }

    void SetValue(const float minRadius, const float maxRadius, const float minAngle, const float maxAngle) {
        this->minRadius = minRadius;
        this->maxRadius = maxRadius;
        this->minAngle  = minAngle;
        this->maxAngle  = maxAngle;
    }

    float minRadius{0.0};
    float maxRadius{0.0};
    float minAngle{0.0};
    float maxAngle{0.0};
};

/**
 * @brief 垃圾箱类型
 *
 */
enum EQUIP_TYPE_ENUN {
    EN_BINSITE_TYPE_660L = 1,    // 大垃圾箱
    EN_BINSITE_TYPE_SML  = 2,    // 小垃圾箱
    EN_CHARGING_PILE     = 3,    // 充电桩
};

/**
 * @brief 外设尺寸（垃圾箱、充电桩）
 *
 */
struct EQUIP_SIZE_STRU {
    EQUIP_SIZE_STRU() {}
    EQUIP_SIZE_STRU(float length, float width, float height) {SetValue(length, width, height);}
    EQUIP_SIZE_STRU(EQUIP_TYPE_ENUN enType) {SetValue(enType);}

    void SetValue(EQUIP_TYPE_ENUN enType) {
        switch(enType) {
            case EN_BINSITE_TYPE_660L: SetValue(1.12, 0.64, 1.1); break;
            case EN_BINSITE_TYPE_SML:  SetValue( 0.8,  0.5, 1.1); break;
            case EN_CHARGING_PILE:     SetValue( 0.5,  0.4, 1.6); break;
            default:                   SetZero();                 break;
        }
    }

    void SetValue(float length, float width, float height) {
        this->length = length;
        this->width  = width;
        this->height = height;
    }

    void SetZero() {SetValue(0, 0, 0);}

    float length{0.0};  // 垃圾箱长（内径尺寸）
    float width{0.0};   // 垃圾箱宽（内径尺寸）
    float height{0.0};  // 垃圾箱高（外径尺寸）
};

/**
 * @brief 带有时间戳的2D位姿
 *
 */
struct COOR_2D_STAMP_POSE_STRU {
    COOR_2D_STAMP_POSE_STRU(){stPose.SetZero();}
    COOR_2D_STAMP_POSE_STRU(float x, float y, float angle) {
        stPose.SetValue(x, y, angle);
    }

    double              stamp{0};
    COOR_2D_POSE_STRU   stPose;
};

/**
 * @brief 带有时间戳的3D位姿
 *
 */
struct COOR_3D_STAMP_POSE_STRU {
    double              stamp{0};
    COOR_3D_POSE_STRU   stPose;
};

/**
 * @brief 道路边缘检测结构体
 *
 */
struct ROAD_EDGE_HEIGHT_STRU {
    float HeightDiff() const {return maxHeight - minHeight;}

    void Set(const float height) {
        minHeight = MIN(minHeight, height);
        maxHeight = MAX(maxHeight, height);
    }

    void Set(const float height, const float thresh) {
        if (isRoadEdge) return;
        minHeight = MIN(minHeight, height);
        maxHeight = MAX(maxHeight, height);
        if (thresh < HeightDiff()) isRoadEdge = true;
    }

    bool IsRoadEdge() const {return isRoadEdge;}

    bool  isRoadEdge{false};
    float minHeight{FLT_MAX};
    float maxHeight{-FLT_MAX};
};

#endif
