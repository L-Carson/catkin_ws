#pragma once

#include <ros/ros.h>
#include "comm/comm_base.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_ros.h"
#include "pcpt_comm/pcpt_comm_format.h"
#include "pcpt_comm/pcpt_comm_region.h"

#include <list>
#include <vector>
#include <deque>

using namespace std;

struct GRID_PR_STRU {
    void Reset() {
        enState  = LABEL_STATE_NO;
        hitTimes = 0;
        accuSequ = 0;
        maxHitPr = 0;
        hitSequ.clear();
    }

    float HitPr() {
        if (hitSequ.empty()) return 0;
        return 1.0 * accuSequ / hitSequ.size();
    }

    uint16_t size() const {
        return hitSequ.size();
    }

    void push_back(uint8_t value) {
        hitSequ.push_back(value);
    }

    void pop_front() {
        hitSequ.pop_front();
    }

    uint8_t front() const {
        return hitSequ.front();
    }

    void SetState(const LABEL_STATE_ENUM _enState) {
        switch (_enState) {
            case LABEL_STATE_MAY: if (enState == LABEL_STATE_NO)  enState = LABEL_STATE_MAY; break;
            case LABEL_STATE_NO : if (enState == LABEL_STATE_MAY) enState = LABEL_STATE_NO;  break;
            case LABEL_STATE_YES: enState = LABEL_STATE_YES; break;
            default: break;
        }
    }

    LABEL_STATE_ENUM    enState{LABEL_STATE_NO};    //状态标记
    uint16_t            hitTimes{0};                //累计击中次数，与击中次序链表无关，因为链表长度有限
    uint16_t            accuSequ{0};                //击中次序链表中击中次数累加值
    float               maxHitPr{0};                //击中次序链表中击中的最大概率
    list<uint8_t>       hitSequ;                    //击中次序链表，保存击中次序
};

struct GRID_HOLD_STRU {
    GRID_HOLD_STRU () : isAnchor(false), hitTimes(0) {}

    bool     isAnchor;  //是否锚定，即认为是否永久有效
    uint16_t hitTimes;  //击中次数
};

template<typename T>
struct FINITE_GRID_STRU {
    /**
     * T: GRID_PR_STRU
     *    GRID_HOLD_STRU
     */
    void push_back(const int index, const T& stGrid) {
        lstIndex.push_back(index);
        vstGrid[index] = stGrid;
    }

    void Clear() {
        lstIndex.clear();
    }

    void Reset() {
        int len = vstGrid.size();
        vstGrid.clear();
        vstGrid.resize(len);
        lstIndex.clear();
    }

    uint64_t IndexSize() const {
        return lstIndex.size();
    }

    uint64_t GridSize() const {
        return vstGrid.size();
    }

    const T& GetGridValue(const int index) const {
        return vstGrid[index];
    }

    /**
     * lstIndex的长度和vstGrid的长度不一样
     * lstIndex存放的是被击中的栅格索引
     * vstGrid的长度是整个概率子图区域栅格的数量，并存储的每个栅格的属性T
     */
    list<int>  lstIndex;    //栅格索引index链表
    vector<T>  vstGrid;     //栅格索引index对应栅格的属性vstGrid<index>
};

struct GRID_PR_COORS_STRU {
    GRID_PR_COORS_STRU(const bool needHoldCoors = true, const bool needUnholdCoors = false) {
        SetNeedMark(needHoldCoors, needUnholdCoors);
    }

    void SetNeedMark(const bool needHoldCoors, const bool needUnholdCoors) {
        this->needHoldCoors   = needHoldCoors;
        this->needUnholdCoors = needUnholdCoors;
    }

    void clear() {
        lstHoldCoors.clear();
        lstUnholdCoors.clear();
    }

    bool needHoldCoors{true};
    LST_COOR lstHoldCoors;

    bool needUnholdCoors{false};
    LST_COOR lstUnholdCoors;
};

/**************************************************************************************
功能描述: 子图
修改记录:
**************************************************************************************/
class PCPT_SUBGRAPH_C
{
public:
    PCPT_SUBGRAPH_C(ros::NodeHandle &nh, const PCPT_CAR_C& objCar,
                    const float gridSize, const float uptDist, const RECTANGLE_STRU& stRectByCar,
                    const string strMark = "", const uint8_t exStep = 0)
    : objCar_(objCar)
    , gridSize_(gridSize)
    , stRectByCar_(stRectByCar)
    , objLocMsg_(nh, strMark.empty() ? "SubGraph" : strMark, 2.0)
    , uptDist_(uptDist)
    , exStep_(exStep)
    {}

    ~PCPT_SUBGRAPH_C(){}

    void PeriodUpt();
    bool UptSubgraphByMap(RECTANGLE_STRU& stRectByMap);

    template<typename T>
    int CopySubgraphGrid(PCPT_GRID_C* pobjGridPre,
                         PCPT_GRID_C* pobjGridCur,
                         RECTANGLE_STRU& stRectByMapPre,
                         RECTANGLE_STRU& stRectByMapCur,
                         T& stFiniteGridPre,
                         T& stFiniteGridCur);
    void GetHitMarkAndIndex(const list<COORDINATE_2D_STRU>& lstPosByMap,
                            PCPT_GRID_C* pobjGrid, bool* pHitMark, list<int>& lstHitIndex);

    PCPT_3D_TF_C* GetTfByStamp();

protected:
    TIMER_C                 objTimerLog1_{1.0};
    TIMER_C                 objTimerLog2_{1.0};
    const PCPT_CAR_C&       objCar_;
    PCPT_3D_TF_C*           pobjTfByStamp_{nullptr};    //指定时间戳下的tf对象
    PCPT_REGION_C           objRegion_;
    float                   gridSize_;
    RECTANGLE_STRU          stRectByCar_;

    int UptTfByStamp(const double stamp, PCPT_3D_TF_C*& pobjTf);

private:
    PCPT_LOC_3D_MSG_C       objLocMsg_;
    float                   uptDist_;
    uint8_t                 exStep_;
    COORDINATE_2D_STRU      stCarPosPre_{FLT_MAX, FLT_MAX};

    bool UptSubgraphByMap(RECTANGLE_STRU& stRectByCar, RECTANGLE_STRU& stRectByMap);
};

/**************************************************************************************
功能描述: 概率子图，关于统计障碍栅格的概率
修改记录:
**************************************************************************************/
class PROBABILITY_SUBGRAPH_C : public PCPT_SUBGRAPH_C
{
public:
    PROBABILITY_SUBGRAPH_C(ros::NodeHandle &nh, const PCPT_CAR_C& objCar, const float gridSize, const RECTANGLE_STRU& stRectByCar,
                           const uint32_t minHitTimes, const uint32_t hitSequSize, const float minHitPr,
                           const string strTopicName, const string strMark = "", const uint8_t exStep = 0)
    : PCPT_SUBGRAPH_C(nh, objCar, gridSize, 0.5, stRectByCar, strMark, exStep)
    , objPointsSenderByCar_(nh, strTopicName + "_by_car")
    , objPointsSenderByMap_(nh, strTopicName + "_by_map")
    , minHitTimes_(minHitTimes)
    , minPrHitTimes_(MAX(minHitTimes, round(0.5 * hitSequSize)))
    , hitSequSize_(hitSequSize)
    , minHitPr_(minHitPr)
    , strMark_(strMark)
    {
        publisher_ = nh.advertise<geometry_msgs::PolygonStamped>(strTopicName + "_by_map_polygon", 1);
    }

    ~PROBABILITY_SUBGRAPH_C()
    {
        DelPtr(pobjGridPre_);
        DelPtr(pobjGridCur_);
    }

    int  Init();

    int  GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp,                                              GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(                                const double stamp,                                              GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp, const list<COORDINATE_3D_STRU>& lstPosByCar, GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(                                const double stamp, const list<COORDINATE_3D_STRU>& lstPosByCar, GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp, const list<COORDINATE_2D_STRU>& lstPosByCar, GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(                                const double stamp, const list<COORDINATE_2D_STRU>& lstPosByCar, GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp, const PointCloudXYZ::Ptr  pCloudByCar,       GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(                                const double stamp, const PointCloudXYZ::Ptr  pCloudByCar,       GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp, const PointCloudXYZI::Ptr pCloudByCar,       GRID_PR_COORS_STRU& stPrCoorsByMap);
    int  GetPrCoors(                                const double stamp, const PointCloudXYZI::Ptr pCloudByCar,       GRID_PR_COORS_STRU& stPrCoorsByMap);

    void Reset();
    void ResetFiniteGrid(const list<COORDINATE_3D_STRU>& lstCoorByCar);
    void ResetFiniteGrid(const list<COORDINATE_2D_STRU>& lstCoorByCar);
    void ResetFiniteGrid(const PointCloudXYZI::Ptr pCloudByCar);

private:
    PCPT_GRID_C*             pobjGridPre_{nullptr}; //by map
    PCPT_GRID_C*             pobjGridCur_{nullptr}; //by map
    POINT_CLOUD_MSG_SENDER_C objPointsSenderByCar_;
    POINT_CLOUD_MSG_SENDER_C objPointsSenderByMap_;

    ros::Publisher           publisher_;

    uint32_t    minHitTimes_;           // 栅格视为障碍的最小击中次数
    uint32_t    minPrHitTimes_;         // 用于计算栅格击中概率的最小击中次数
    uint32_t    hitSequSize_;           // 栅格击中次序队列长度
    float       minHitPr_;              // 栅格视为障碍的最小击中概率
    string      strMark_;

    RECTANGLE_STRU                  stRectByMap_{0, 0, 0, 0};
    FINITE_GRID_STRU<GRID_PR_STRU>  stFiniteGrid_;  //by map

    void ResetFiniteGrid(const list<COORDINATE_3D_STRU>& lstCoorByCar, FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid);
    int  UptSubgraphGrid(const LABEL_STATE_ENUM enState, const list<COORDINATE_2D_STRU>& lstPosByMap, FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid);
    int  UptFiniteGrid(const LABEL_STATE_ENUM enState, const double stamp, const list<COORDINATE_3D_STRU>& lstPosByCar,
                       RECTANGLE_STRU& stRectByMap, FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid);
    void GetSubgraphGridCoors(const FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid, GRID_PR_COORS_STRU& stPrCoorsByMap) const;
    void PubValidSubgraphGrid(list<COORDINATE_2D_STRU>& lstPosByMap);
    void PubSubgraphPolygon(RECTANGLE_STRU& stRectByCar, RECTANGLE_STRU& stRectByMap);
};
