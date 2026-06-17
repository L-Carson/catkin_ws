#include "pcpt_comm/pcpt_comm_subgraph.h"

/**************************************************************************************
功能描述: 周期更新
修改记录:
**************************************************************************************/
void PCPT_SUBGRAPH_C::PeriodUpt()
{
    objLocMsg_.UptLocationMsg();
}

/**************************************************************************************
功能描述: 更新子图到map
修改记录:
**************************************************************************************/
bool PCPT_SUBGRAPH_C::UptSubgraphByMap(RECTANGLE_STRU& stRectByMap)
{
    return UptSubgraphByMap(stRectByCar_, stRectByMap);
}

bool PCPT_SUBGRAPH_C::UptSubgraphByMap(RECTANGLE_STRU& stRectByCar, RECTANGLE_STRU& stRectByMap)
{
    COOR_2D_POSE_STRU stCarPose = objCar_.GetCarPose();
    float difDis2 = stCarPose.stPos.CalcDistSqu(stCarPosPre_);

    if (difDis2 < pow(uptDist_, 2)) return false;
    else stCarPosPre_ = stCarPose.stPos;

    objRegion_.ExtendRectByCar2ByMap(stCarPose, stRectByCar, stRectByMap);
    stRectByMap.Round(gridSize_);
    return true;
}

/**************************************************************************************
功能描述: 获取与数据时间戳对应的tf对象指针
修改记录:
**************************************************************************************/
PCPT_3D_TF_C* PCPT_SUBGRAPH_C::GetTfByStamp()
{
    return pobjTfByStamp_;
}

/**************************************************************************************
功能描述: 根据时间戳更新tf
修改记录:
**************************************************************************************/
int PCPT_SUBGRAPH_C::UptTfByStamp(const double stamp, PCPT_3D_TF_C*& pobjTf)
{
    /* 根据数据的时间获取对应时刻的车体位姿信息 */
    COOR_3D_POSE_STRU stPos;
    INT32 rslt = objLocMsg_.GetLocationMsg(stamp, stPos);
    ST_LOG_ERR_TIMER_IF(rslt, objTimerLog1_, "Failed to obtain the matching pose.");
    if(rslt != 0) {
        return -1;
    }

    /* 判断是否实例化 */
    if (!pobjTf) {
        pobjTf = new PCPT_3D_TF_C();
        if (!pobjTf) {
            ST_LOG_ERR("New PCPT_3D_TF_C Object Fail.");
            return -1;
        }
    }

    /* 设置转换关系 */
    pobjTf->SetTfRelationship(stPos);
    return 0;
}

/**************************************************************************************
功能描述: 拷贝子图栅格
修改记录:
**************************************************************************************/
template<typename T>
int PCPT_SUBGRAPH_C::CopySubgraphGrid(PCPT_GRID_C* pobjGridPre,
                                      PCPT_GRID_C* pobjGridCur,
                                      RECTANGLE_STRU& stRectByMapPre,
                                      RECTANGLE_STRU& stRectByMapCur,
                                      T& stFiniteGridPre,
                                      T& stFiniteGridCur)
{
    if (pobjGridCur->Reset(stRectByMapCur, gridSize_)) return -1;
    stFiniteGridCur.vstGrid.clear();
    stFiniteGridCur.vstGrid.resize(pobjGridCur->GetGridNum());

    int difGridX, difGridY;
    pobjGridCur->GetDifGrid(stRectByMapPre.minX, stRectByMapPre.minY, difGridX, difGridY);

    for (auto iter = stFiniteGridPre.lstIndex.begin(); iter != stFiniteGridPre.lstIndex.end(); ++iter) {
        int gridX, gridY;
        if (pobjGridPre->GetGridXYIndex(*iter, gridX, gridY)) continue;
        gridX += difGridX;
        gridY += difGridY;
        int indexCur = pobjGridCur->GetGridIndex(gridX, gridY);
        if (indexCur == -1) continue;
        stFiniteGridCur.push_back(indexCur, stFiniteGridPre.GetGridValue(*iter));
    }

    pobjGridPre->Reset(stRectByMapCur, gridSize_);

    return 0;
}

template int PCPT_SUBGRAPH_C::CopySubgraphGrid(PCPT_GRID_C*,
                                               PCPT_GRID_C*,
                                               RECTANGLE_STRU&,
                                               RECTANGLE_STRU&,
                                               FINITE_GRID_STRU<GRID_PR_STRU>&,
                                               FINITE_GRID_STRU<GRID_PR_STRU>&);

template int PCPT_SUBGRAPH_C::CopySubgraphGrid(PCPT_GRID_C*,
                                               PCPT_GRID_C*,
                                               RECTANGLE_STRU&,
                                               RECTANGLE_STRU&,
                                               FINITE_GRID_STRU<GRID_HOLD_STRU>&,
                                               FINITE_GRID_STRU<GRID_HOLD_STRU>&);

/**************************************************************************************
功能描述: 获取栅格击中标识以及击中栅格的索引
修改记录:
**************************************************************************************/
void PCPT_SUBGRAPH_C::GetHitMarkAndIndex(const list<COORDINATE_2D_STRU>& lstPosByMap,
                                         PCPT_GRID_C* pobjGrid, bool* pHitMark, list<int>& lstHitIndex)
{
    /* 计算当前帧数据所击中的栅格索引向量及击中标识 */
    for (const COORDINATE_2D_STRU& stPos : lstPosByMap) {
        int index = pobjGrid->GetGridIndex(stPos.x, stPos.y);
        if (index == -1) continue;

        /** 如果栅格已被标记为被当前帧击中，则不再对栅格数据进行更新，
         *  因为每帧数据可能会有多个点落在同一栅格内，确保每帧数据只更新一次栅格数据。
         */
        if (pHitMark[index]) continue;
        pHitMark[index] = 1;
        lstHitIndex.push_back(index);

        /* 扩展到临近栅格 */
        if (exStep_ != 0) {
            vector<int>* pNeighborIndex;
            if (pobjGrid->GetNeighborGrid(index, exStep_, pNeighborIndex)) return;
            if (pNeighborIndex == nullptr || pNeighborIndex->empty()) return;
            for (const size_t neighborIndex : *pNeighborIndex) {
                if (pHitMark[neighborIndex]) continue;
                pHitMark[neighborIndex] = 1;
                lstHitIndex.push_back(neighborIndex);
            }
        }
    }
}

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::Init()
{
    if (minHitPr_ < 0 || minHitPr_ > 1) {
        ST_LOG_ERR("The parameter minimum hit probability is set incorrectly, Please check again.");
        return -1;
    }

    pobjGridPre_ = new PCPT_GRID_C(stRectByCar_, gridSize_);
    if (!pobjGridPre_|| !pobjGridPre_->IsValid()) {
        ST_LOG_ERR("Creat PCPT_GRID_C Object Fail.");
        return -1;
    }

    pobjGridCur_ = new PCPT_GRID_C(stRectByCar_, gridSize_);
    if (!pobjGridCur_|| !pobjGridCur_->IsValid()) {
        ST_LOG_ERR("Creat PCPT_GRID_C Object Fail.");
        return -1;
    }

    stFiniteGrid_.vstGrid.resize(pobjGridCur_->GetGridNum());

    ST_LOG_INFO("Init [%s] Successful.", strMark_.c_str());
    return 0;
}

/**************************************************************************************
功能描述: 获取概率栅格坐标（无输入数据）
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    list<COORDINATE_3D_STRU> lstPosByCar;
    return GetPrCoors(enState, stamp, lstPosByCar, stPrCoorsByMap);
}

int PROBABILITY_SUBGRAPH_C::GetPrCoors(const double stamp, GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    return GetPrCoors(LABEL_STATE_YES, stamp, stPrCoorsByMap);
}

/**************************************************************************************
功能描述: 获取概率栅格坐标（有输入数据: COORDINATE_3D_STRU）
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp,
                                       const list<COORDINATE_3D_STRU>& lstPosByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    if (0 != UptFiniteGrid(enState, stamp, lstPosByCar, stRectByMap_, stFiniteGrid_)) {
        ST_LOG_ERR("Failed to update the finite grid.");
        return -1;
    }

    list<COORDINATE_2D_STRU> lstUnholdPosByMap;
    GetSubgraphGridCoors(stFiniteGrid_, stPrCoorsByMap);
    PubValidSubgraphGrid(stPrCoorsByMap.lstHoldCoors);
    PubSubgraphPolygon(stRectByCar_, stRectByMap_);

    return 0;
}

int PROBABILITY_SUBGRAPH_C::GetPrCoors(const double stamp, const list<COORDINATE_3D_STRU>& lstPosByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    return GetPrCoors(LABEL_STATE_YES, stamp, lstPosByCar, stPrCoorsByMap);
}

/**************************************************************************************
功能描述: 获取概率栅格坐标（有输入数据: COORDINATE_2D_STRU
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp,
                                       const list<COORDINATE_2D_STRU>& lstPosByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    list<COORDINATE_3D_STRU> lst3dPosByCar;
    for (const COORDINATE_2D_STRU& stPos : lstPosByCar)
        lst3dPosByCar.push_back(COORDINATE_3D_STRU(stPos.x, stPos.y, 0));
    return GetPrCoors(enState, stamp, lst3dPosByCar, stPrCoorsByMap);
}

int PROBABILITY_SUBGRAPH_C::GetPrCoors(const double stamp, const list<COORDINATE_2D_STRU>& lstPosByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    return GetPrCoors(LABEL_STATE_YES, stamp, lstPosByCar, stPrCoorsByMap);
}

/**************************************************************************************
功能描述: 获取概率栅格坐标（有输入数据: PointCloudXYZ
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp,
                                       const PointCloudXYZ::Ptr pCloudByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    list<COORDINATE_3D_STRU> lst3dPosByCar;
    for (auto& point : pCloudByCar->points)
        lst3dPosByCar.push_back(COORDINATE_3D_STRU(point.x, point.y, point.z));
    return GetPrCoors(enState, stamp, lst3dPosByCar, stPrCoorsByMap);
}

int PROBABILITY_SUBGRAPH_C::GetPrCoors(const double stamp, const PointCloudXYZ::Ptr pCloudByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    return GetPrCoors(LABEL_STATE_YES, stamp, pCloudByCar, stPrCoorsByMap);
}

/**************************************************************************************
功能描述: 获取概率栅格坐标（有输入数据: PointCloudXYZI
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::GetPrCoors(const LABEL_STATE_ENUM enState, const double stamp,
                                       const PointCloudXYZI::Ptr pCloudByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    list<COORDINATE_3D_STRU> lst3dPosByCar;
    for (auto& point : pCloudByCar->points)
        lst3dPosByCar.push_back(COORDINATE_3D_STRU(point.x, point.y, point.z));
    return GetPrCoors(enState, stamp, lst3dPosByCar, stPrCoorsByMap);
}

int PROBABILITY_SUBGRAPH_C::GetPrCoors(const double stamp, const PointCloudXYZI::Ptr pCloudByCar,
                                       GRID_PR_COORS_STRU& stPrCoorsByMap)
{
    return GetPrCoors(LABEL_STATE_YES, stamp, pCloudByCar, stPrCoorsByMap);
}

/**************************************************************************************
功能描述: 复位
修改记录:
**************************************************************************************/
void PROBABILITY_SUBGRAPH_C::Reset()
{
    stFiniteGrid_.Reset();
}

/**************************************************************************************
功能描述: 重置有限栅格
修改记录:
**************************************************************************************/
void PROBABILITY_SUBGRAPH_C::ResetFiniteGrid(const list<COORDINATE_3D_STRU>& lstCoorByCar, FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid)
{
    if (!pobjTfByStamp_) return;

    /* 构建栅格占用数组 */
    size_t size = stFiniteGrid.GridSize();
    bool *pHitMarkOfCur = new bool[size](); //初始化为false
    if (!pHitMarkOfCur) return;
    for (const int &index : stFiniteGrid.lstIndex) pHitMarkOfCur[index] = true;

    /* 重置当前数据占用的栅格 */
    for (const COORDINATE_3D_STRU &stCoorByCar : lstCoorByCar) {
        COORDINATE_3D_STRU stCoorByMap;
        pobjTfByStamp_->Trans3dPosBySon2ByFather(stCoorByCar, stCoorByMap);
        int index = pobjGridCur_->GetGridIndex(stCoorByMap.x, stCoorByMap.y);
        if (index == -1)  continue;
        if (!pHitMarkOfCur[index]) continue;
        pHitMarkOfCur[index] = false;
        GRID_PR_STRU& stGridPr = stFiniteGrid.vstGrid[index];
        stGridPr.Reset();
    }

    /* 取回未被重置的栅格 */
    stFiniteGrid.lstIndex.clear();
    for (size_t index = 0; index < size; ++index)
        if (pHitMarkOfCur[index])
            stFiniteGrid.lstIndex.push_back(index);

    if (pHitMarkOfCur) delete[] pHitMarkOfCur;
}

void PROBABILITY_SUBGRAPH_C::ResetFiniteGrid(const list<COORDINATE_3D_STRU>& lstCoorByCar)
{
    ResetFiniteGrid(lstCoorByCar, stFiniteGrid_);
}

void PROBABILITY_SUBGRAPH_C::ResetFiniteGrid(const list<COORDINATE_2D_STRU>& lstCoorByCar)
{
    if (!pobjTfByStamp_) return;

    list<COORDINATE_3D_STRU> lst3DCoorByCar;
    for (const COORDINATE_2D_STRU& stCoor : lstCoorByCar) lst3DCoorByCar.push_back(COORDINATE_3D_STRU(stCoor.x, stCoor.y, 0));
    ResetFiniteGrid(lst3DCoorByCar, stFiniteGrid_);
}

void PROBABILITY_SUBGRAPH_C::ResetFiniteGrid(const PointCloudXYZI::Ptr pCloudByCar)
{
    if (!pobjTfByStamp_) return;

    list<COORDINATE_3D_STRU> lst3DCoorByCar;
    for (const auto& point : pCloudByCar->points) lst3DCoorByCar.push_back(COORDINATE_3D_STRU(point.x, point.y, point.z));
    ResetFiniteGrid(lst3DCoorByCar, stFiniteGrid_);
}

/**************************************************************************************
功能描述: 更新子图栅格
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::UptSubgraphGrid(const LABEL_STATE_ENUM enState,
                                            const list<COORDINATE_2D_STRU>& lstPosByMap,
                                            FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid)
{
    /* 当前帧中栅格被击中的标识数组，并初始化为0("()"表示默认初始化)，避免重复计数 */
    if (!stFiniteGrid.GridSize()) return -1;
    bool *pHitMarkOfCur = new bool[stFiniteGrid.GridSize()]();
    if (!pHitMarkOfCur) return -1;

    /* 计算当前帧数据所击中的栅格索引向量及击中标识 */
    list<int> lstHitIndexOfCur;     //当前帧击中栅格的索引链表
    GetHitMarkAndIndex(lstPosByMap, pobjGridCur_, pHitMarkOfCur, lstHitIndexOfCur);

    /* 更新已维持的栅格链表 */
    for (auto iter = stFiniteGrid.lstIndex.begin(); iter != stFiniteGrid.lstIndex.end(); ++iter) {
        GRID_PR_STRU& stGrid = stFiniteGrid.vstGrid[*iter];
        stGrid.SetState(enState);

        /**
         * 1. 如果栅格被当前帧击中：
         * 1.1 更新栅格数据;
         * 1.2 将该栅格索引对应的标识置零，标识已被用于更新，所有余下未被置零的标识即为新击中的栅格；
         * 2. 如果栅格没有被当前帧击中：栅格击中序列追加0；
         * 3. 确保击中序列的长度；
         * 4. 计算栅格连续n次内击中的最大概率；
         */

        /* 更新栅格元素值 */
        if (pHitMarkOfCur[*iter]) {
            pHitMarkOfCur[*iter] = false;
            stGrid.hitTimes++;
            stGrid.accuSequ++;
            stGrid.push_back(1);
        } else {
            stGrid.push_back(0);
        }

        while (stGrid.size() > hitSequSize_) {
            stGrid.accuSequ -= stGrid.front();
            stGrid.pop_front();
        }

        if (stGrid.size() >= minPrHitTimes_) {
            stGrid.maxHitPr = MAX(stGrid.maxHitPr, stGrid.HitPr());
        }
    }

    /* 将当前帧新击中的栅格添加到维持链表中，新增的栅格不计算概率 */
    for (auto &index : lstHitIndexOfCur) {
        if (!pHitMarkOfCur[index]) continue;
        stFiniteGrid.lstIndex.push_back(index);
        GRID_PR_STRU& stGrid = stFiniteGrid.vstGrid[index];
        stGrid.Reset();
        stGrid.SetState(enState);
        stGrid.hitTimes++;
        stGrid.accuSequ++;
        stGrid.push_back(1);
    }

    if (pHitMarkOfCur) delete[] pHitMarkOfCur;
    return 0;
}

/**************************************************************************************
功能描述: 更新有限栅格stFiniteGrid
修改记录:
**************************************************************************************/
int PROBABILITY_SUBGRAPH_C::UptFiniteGrid(const LABEL_STATE_ENUM enState, const double stamp,
                                          const list<COORDINATE_3D_STRU>& lstPosByCar,
                                          RECTANGLE_STRU& stRectByMap, FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid)
{
    RECTANGLE_STRU stRectByMapCur; //概率子图的区域尺寸
    if (UptSubgraphByMap(stRectByMapCur)) {
        FINITE_GRID_STRU<GRID_PR_STRU> stFiniteGridCur;
        if (CopySubgraphGrid(pobjGridPre_, pobjGridCur_, stRectByMap, stRectByMapCur, stFiniteGrid, stFiniteGridCur)) return -1;
        stRectByMap  = stRectByMapCur;
        stFiniteGrid = stFiniteGridCur;
    }

    if (SIG(stamp) != 0) {
        INT32 rslt = UptTfByStamp(stamp, pobjTfByStamp_);
        ST_LOG_ERR_TIMER_IF(rslt, objTimerLog2_, "Failed to update tf by stamp:%f.", stamp);
        if (rslt != 0) {
            DelPtr(pobjTfByStamp_);
            return 0;   //注意：此处返回0，获取定位tf失败时，不更新stFiniteGrid，但是还需要继续获取子图数据
        }
    } else {
        DelPtr(pobjTfByStamp_);
        return 0;
    }

    if (lstPosByCar.size()) {
        list<COORDINATE_2D_STRU> lstPosByMapCur;
        for (const COORDINATE_3D_STRU &stPosByCar : lstPosByCar) {
            COORDINATE_3D_STRU stPosByMap;
            pobjTfByStamp_->Trans3dPosBySon2ByFather(stPosByCar, stPosByMap);
            lstPosByMapCur.push_back(COORDINATE_2D_STRU(stPosByMap.x, stPosByMap.y));
        }
        if (UptSubgraphGrid(enState, lstPosByMapCur, stFiniteGrid)) return -1;
    }

    return 0;
}

/**************************************************************************************
功能描述: 获取子图栅格中维持的栅格坐标
修改记录:
**************************************************************************************/
void PROBABILITY_SUBGRAPH_C::GetSubgraphGridCoors(const FINITE_GRID_STRU<GRID_PR_STRU>& stFiniteGrid,
                                                  GRID_PR_COORS_STRU& stPrCoorsByMap) const
{
    /* 遍历有限栅格索引stFiniteGrid.lstIndex(被击中栅格的索引)，获取满足条件的栅格坐标 */
    if (stPrCoorsByMap.needHoldCoors) {
        for (const int &index : stFiniteGrid.lstIndex) {
            const GRID_PR_STRU& stGrid = stFiniteGrid.vstGrid[index];

            /* 栅格击中次数大于阈值，或着击中概率大于阈值，则认为是有效维持栅格 */
            if (stGrid.enState != LABEL_STATE_YES) continue;
            if (stGrid.hitTimes >= minHitTimes_ || stGrid.maxHitPr >= minHitPr_) {
                float x, y;
                if (pobjGridCur_->GetGridCoor(index, x, y)) continue;
                COORDINATE_2D_STRU stPosByMap(x, y);
                stPrCoorsByMap.lstHoldCoors.push_back(stPosByMap);
            }
        }
    }

    /* 遍历所有栅格stFiniteGrid.vstGrid，获取满足条件的栅格坐标 */
    if (stPrCoorsByMap.needUnholdCoors) {
        for (size_t index = 0; index < stFiniteGrid.GridSize(); ++index) {
            const GRID_PR_STRU& stGrid = stFiniteGrid.vstGrid[index];

            /* 栅格击中次数小于阈值，同时击中概率小于阈值，则认为是未维持栅格 */
            if (stGrid.enState != LABEL_STATE_NO) continue;
            if (stGrid.hitTimes < minHitTimes_ && stGrid.maxHitPr < minHitPr_) {
                float x, y;
                if (pobjGridCur_->GetGridCoor(index, x, y)) continue;
                COORDINATE_2D_STRU stPosByMap(x, y);
                stPrCoorsByMap.lstUnholdCoors.push_back(stPosByMap);
            }
        }
    }
}

/**************************************************************************************
功能描述: 以点云形式发布有效子图栅格
修改记录:
**************************************************************************************/
void PROBABILITY_SUBGRAPH_C::PubValidSubgraphGrid(list<COORDINATE_2D_STRU>& lstPosByMap)
{
    PointCloudXYZI::Ptr pCloud(new PointCloudXYZI);
    pCloud->reserve(lstPosByMap.size());

    for (COORDINATE_2D_STRU& stPosByMap : lstPosByMap) {
        PointXYZI point;
        point.x = stPosByMap.x;
        point.y = stPosByMap.y;
        point.z = 0;
        pCloud->push_back(point);
    }

    objPointsSenderByMap_.Send(pCloud, PCPT_FRAME_ID_MAP);

    if (objPointsSenderByCar_.GetNumSubscribers()) {
        pCloud->clear();
        for (COORDINATE_2D_STRU& stPosByMap : lstPosByMap) {
            COORDINATE_2D_STRU stPosByCar;
            objCar_.TransformPosByMapToByCar(stPosByMap, stPosByCar);
            PointXYZI point;
            point.x = stPosByCar.x;
            point.y = stPosByCar.y;
            point.z = 0;
            pCloud->push_back(point);
        }
        objPointsSenderByCar_.Send(pCloud, PCPT_FRAME_ID_CAR);
    }
}

/**************************************************************************************
功能描述: 发布子图区域多边形
修改记录:
**************************************************************************************/
void PROBABILITY_SUBGRAPH_C::PubSubgraphPolygon(RECTANGLE_STRU& stRectByCar, RECTANGLE_STRU& stRectByMap)
{
    if (!publisher_.getNumSubscribers()) return;

    COORDINATE_2D_STRU stPosByCar, stPosByMap;
#define RECT_BY_CAR_2_POS(xx, yy, point)                        \
    stPosByCar.SetValue(xx, yy);                                \
    objCar_.TransformPosByCarToByMap(stPosByCar, stPosByMap);   \
    point.x = stPosByMap.x;                                     \
    point.y = stPosByMap.y;                                     \
    point.z = 0;                                                \

    vector<geometry_msgs::Point32> vPointInside(4);
    RECT_BY_CAR_2_POS(stRectByCar.minX, stRectByCar.minY, vPointInside[0]);
    RECT_BY_CAR_2_POS(stRectByCar.minX, stRectByCar.maxY, vPointInside[1]);
    RECT_BY_CAR_2_POS(stRectByCar.maxX, stRectByCar.maxY, vPointInside[2]);
    RECT_BY_CAR_2_POS(stRectByCar.maxX, stRectByCar.minY, vPointInside[3]);

    geometry_msgs::PolygonStamped polygon;
    polygon.header.frame_id = PCPT_FRAME_ID_MAP;
    polygon.header.stamp = RosTimeNow();

    for (auto &point : vPointInside)
        polygon.polygon.points.push_back(point);

    polygon.polygon.points.push_back(vPointInside.front());

    // car与map坐标系的轮廓不一定完全匹配，map下轮廓基于栅格分辨率外扩了一些
#define RECT_BY_MAP_2_POS(_x, _y, point)    \
    point.x = _x;                           \
    point.y = _y;                           \
    point.z = 0;                            \

    vector<geometry_msgs::Point32> vPointOutSide(4);
    RECT_BY_MAP_2_POS(stRectByMap.minX, stRectByMap.minY, vPointOutSide[0]);
    RECT_BY_MAP_2_POS(stRectByMap.minX, stRectByMap.maxY, vPointOutSide[1]);
    RECT_BY_MAP_2_POS(stRectByMap.maxX, stRectByMap.maxY, vPointOutSide[2]);
    RECT_BY_MAP_2_POS(stRectByMap.maxX, stRectByMap.minY, vPointOutSide[3]);

    for (auto &point : vPointOutSide) {
        polygon.polygon.points.push_back(point);
    }
    polygon.polygon.points.push_back(vPointOutSide.front());

    publisher_.publish(polygon);
}
