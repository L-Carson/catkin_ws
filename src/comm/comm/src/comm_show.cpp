
#include "comm/comm_show.h"


BOOL SPHERR_SHOWER_C::IsExistSubscriber(void)
{
    return (objPublisher_.getNumSubscribers() > 0);
}

/**************************************************************************************
功能描述: 球体显示
修改记录:
**************************************************************************************/
void SPHERR_SHOWER_C::Show(const std::vector<COORDINATE_2D_STRU> &vectSphereCenter, float radius)
{
    if (!IsExistSubscriber()) {
        return;
    }

    radius_ = (radius > 0) ? radius : radius_;
    R_ASSERT_2(radius_ > 0, NOTHING, radius_, radius);

    SendMsg(vectSphereCenter);
}

void SPHERR_SHOWER_C::SendMsg(const std::vector<COORDINATE_2D_STRU> &vectorSphereCenter)
{
    visualization_msgs::Marker stSphereListMsg;

    stSphereListMsg.header.frame_id    = "map";
    stSphereListMsg.header.stamp       = ros::Time::now();
    stSphereListMsg.ns                 = "";

    stSphereListMsg.id                 = 0;
    stSphereListMsg.type               = visualization_msgs::Marker::SPHERE_LIST;
    stSphereListMsg.action             = visualization_msgs::Marker::ADD;

    for (const COORDINATE_2D_STRU &stSphereCenter : vectorSphereCenter) {
        geometry_msgs::Point stMsgPoint;
        stMsgPoint.x = stSphereCenter.x;
        stMsgPoint.y = stSphereCenter.y;
        stMsgPoint.z = 0;
        stSphereListMsg.points.push_back(stMsgPoint);
    }

    stSphereListMsg.pose.orientation.w = 1.0;
    stSphereListMsg.scale.x            = radius_ * 2.0;
    stSphereListMsg.scale.y            = radius_ * 2.0;
    stSphereListMsg.scale.z            = radius_ * 2.0;

    stSphereListMsg.color.b            = GET_PRI_COLOR_B(enColor_);
    stSphereListMsg.color.g            = GET_PRI_COLOR_G(enColor_);
    stSphereListMsg.color.r            = GET_PRI_COLOR_R(enColor_);
    stSphereListMsg.color.a            = 0.5;

    objPublisher_.publish(stSphereListMsg);
}

/**************************************************************************************
功能描述: 获取话题订阅数量
修改记录:
**************************************************************************************/
UINT32 ROAD_SHOW_C::GetSubscriberNum(void) const
{
    return UINT32(publisher_.getNumSubscribers());
}

/**************************************************************************************
功能描述: 显示道路线形信息
修改记录:
**************************************************************************************/
void ROAD_SHOW_C::ShowLine(UINT32 objId, const vector<COORDINATE_2D_STRU> &vectLine, float width, SHOW_COLOR_ENUM enColor)
{
    visualization_msgs::Marker stLineMsg;

    stLineMsg.header.frame_id    = "map";
    stLineMsg.header.stamp       = ros::Time::now();
    stLineMsg.ns                 = "road_show_line";

    stLineMsg.id                 = objId;
    stLineMsg.type               = visualization_msgs::Marker::LINE_STRIP;
    stLineMsg.action             = visualization_msgs::Marker::ADD;

    stLineMsg.pose.orientation.w = 1.0;
    stLineMsg.scale.x            = width;

    stLineMsg.color.b            = GET_PRI_COLOR_B(enColor);
    stLineMsg.color.g            = GET_PRI_COLOR_G(enColor);
    stLineMsg.color.r            = GET_PRI_COLOR_R(enColor);
    stLineMsg.color.a            = 1.0;

    UINT32 srcLineSize    = vectLine.size();
    UINT32 sampleInterval = srcLineSize / maxShowLineSize_ + 1;
    ST_LOG_INFO_IF(sampleInterval > 1, "Src Line Size : %u, sampleInterval : %u.", srcLineSize, sampleInterval);

    for (UINT32 loop = 0; loop < srcLineSize; loop += sampleInterval) {
        const COORDINATE_2D_STRU &stPoint = vectLine[loop];

        geometry_msgs::Point p;
        p.x = stPoint.x;
        p.y = stPoint.y;
        p.z = 0;
        stLineMsg.points.push_back(p);
    }

    publisher_.publish(stLineMsg);

    AddShowObj(stLineMsg.ns, stLineMsg.id);
}

/**************************************************************************************
功能描述: 显示道路文字标签
修改记录:
**************************************************************************************/
void ROAD_SHOW_C::ShowLabel(UINT32 objId, const COORDINATE_2D_STRU &stPos, const std::string &strLabel, float height, SHOW_COLOR_ENUM enColor)
{
    visualization_msgs::Marker stLabelMsg;

    stLabelMsg.header.frame_id    = "map";
    stLabelMsg.header.stamp       = ros::Time::now();
    stLabelMsg.ns                 = "road_show_label";

    stLabelMsg.id                 = objId;
    stLabelMsg.type               = visualization_msgs::Marker::TEXT_VIEW_FACING;
    stLabelMsg.action             = visualization_msgs::Marker::ADD;

    stLabelMsg.pose.position.x    = stPos.x;
    stLabelMsg.pose.position.y    = stPos.y;
    stLabelMsg.pose.position.z    = 0.0;
    stLabelMsg.pose.orientation.x = 0.0;
    stLabelMsg.pose.orientation.y = 0.0;
    stLabelMsg.pose.orientation.z = 0.0;
    stLabelMsg.pose.orientation.w = 1.0;

    stLabelMsg.text               = strLabel;

    stLabelMsg.scale.x            = 0.0;
    stLabelMsg.scale.y            = 0.0;
    stLabelMsg.scale.z            = height;         /* 字体高度 */

    stLabelMsg.color.b            = GET_PRI_COLOR_B(enColor);
    stLabelMsg.color.g            = GET_PRI_COLOR_G(enColor);
    stLabelMsg.color.r            = GET_PRI_COLOR_R(enColor);
    stLabelMsg.color.a            = 1.0;

    publisher_.publish(stLabelMsg);

    AddShowObj(stLabelMsg.ns, stLabelMsg.id);
}

/**************************************************************************************
功能描述: 显示方向箭头
修改记录:
**************************************************************************************/
void ROAD_SHOW_C::ShowArrow(UINT32 objId, const COORDINATE_2D_STRU &stStartPos, const COORDINATE_2D_STRU &stStopPos, float width, SHOW_COLOR_ENUM enColor)
{
    visualization_msgs::Marker stArrowMsg;

    stArrowMsg.header.frame_id    = "map";
    stArrowMsg.header.stamp       = ros::Time::now();
    stArrowMsg.ns                 = "road_show_arrow";

    stArrowMsg.id                 = objId;
    stArrowMsg.type               = visualization_msgs::Marker::ARROW;
    stArrowMsg.action             = visualization_msgs::Marker::ADD;

    stArrowMsg.pose.orientation.w = 1.0;

    stArrowMsg.scale.x            = width;              /* 尾直径 */
    stArrowMsg.scale.y            = width * 2.0;        /* 头直径 */
    stArrowMsg.scale.z            = 0.0;                /* 头长度 */

    stArrowMsg.color.b            = GET_PRI_COLOR_B(enColor);
    stArrowMsg.color.g            = GET_PRI_COLOR_G(enColor);
    stArrowMsg.color.r            = GET_PRI_COLOR_R(enColor);
    stArrowMsg.color.a            = 1.0;

    geometry_msgs::Point stStartPoint;
    stStartPoint.x = stStartPos.x;
    stStartPoint.y = stStartPos.y;
    stStartPoint.z = 0;
    stArrowMsg.points.push_back(stStartPoint);

    geometry_msgs::Point stStopPoint;
    stStopPoint.x = stStopPos.x;
    stStopPoint.y = stStopPos.y;
    stStopPoint.z = 0;
    stArrowMsg.points.push_back(stStopPoint);

    publisher_.publish(stArrowMsg);

    AddShowObj(stArrowMsg.ns, stArrowMsg.id);
}

/**************************************************************************************
功能描述: 显示立方体列表
修改记录:
**************************************************************************************/
void ROAD_SHOW_C::ShowCubeList(UINT32 objId, const vector<COORDINATE_2D_STRU> &vectPos, float width, SHOW_COLOR_ENUM enColor)
{
    visualization_msgs::Marker stCubeListMsg;

    stCubeListMsg.header.frame_id    = "map";
    stCubeListMsg.header.stamp       = ros::Time::now();
    stCubeListMsg.ns                 = "road_show_cube";

    stCubeListMsg.id                 = objId;
    stCubeListMsg.type               = visualization_msgs::Marker::CUBE_LIST;
    stCubeListMsg.action             = visualization_msgs::Marker::ADD;

    stCubeListMsg.pose.position.x    = 0.0;
    stCubeListMsg.pose.position.y    = 0.0;
    stCubeListMsg.pose.position.z    = 0.0;
    stCubeListMsg.pose.orientation.x = 0.0;
    stCubeListMsg.pose.orientation.y = 0.0;
    stCubeListMsg.pose.orientation.z = 0.0;
    stCubeListMsg.pose.orientation.w = 1.0;

    stCubeListMsg.scale.x            = width;      /* 长宽高 */
    stCubeListMsg.scale.y            = width;
    stCubeListMsg.scale.z            = width;

    stCubeListMsg.color.b            = GET_PRI_COLOR_B(enColor);
    stCubeListMsg.color.g            = GET_PRI_COLOR_G(enColor);
    stCubeListMsg.color.r            = GET_PRI_COLOR_R(enColor);
    stCubeListMsg.color.a            = 1.0;

    for (UINT32 loop = 0; loop < vectPos.size(); ++loop) {
        const COORDINATE_2D_STRU &stPos = vectPos[loop];

        geometry_msgs::Point p;
        p.x = stPos.x;
        p.y = stPos.y;
        p.z = 0;
        stCubeListMsg.points.push_back(p);
    }

    publisher_.publish(stCubeListMsg);

    AddShowObj(stCubeListMsg.ns, stCubeListMsg.id);
}

/**************************************************************************************
功能描述: 清除当前所有显示
修改记录:
**************************************************************************************/
void ROAD_SHOW_C::Clear(void)
{
    for (const SHOW_OBJ_STRU &stShowObj : vectShowObj) {
        ClrObjShow(stShowObj);
    }

    vectShowObj.clear();
}

void ROAD_SHOW_C::AddShowObj(const std::string &strNS, UINT32 objId)
{
    SHOW_OBJ_STRU stShowObj;
    stShowObj.strNS = strNS;
    stShowObj.objId = objId;

    vectShowObj.push_back(std::move(stShowObj));
}

/**************************************************************************************
功能描述: 清除指定显示对象
修改记录:
**************************************************************************************/
void ROAD_SHOW_C::ClrObjShow(const SHOW_OBJ_STRU &stShowObj) const
{
    visualization_msgs::Marker stClearMsg;

    stClearMsg.header.frame_id    = "map";
    stClearMsg.header.stamp       = ros::Time::now();
    stClearMsg.ns                 = stShowObj.strNS;

    stClearMsg.id                 = stShowObj.objId;
    stClearMsg.action             = visualization_msgs::Marker::DELETE;

    publisher_.publish(stClearMsg);
}



