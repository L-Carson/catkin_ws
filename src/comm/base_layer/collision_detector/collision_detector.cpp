/*
Author      : gongzhihao
Update Date : 2025/07/23
*/
#include <set>

#include "base_layer/collision_detector/collision_detector.h"

// 构造函数
CollisionDetector::CollisionDetector(const Pose2D& ego_pose_,  const float& brush_arm_theta_)
    : ego_pose_(ego_pose_), brush_arm_theta_(brush_arm_theta_)
{
    collision_car_.SetEgoPose(ego_pose_);
    collision_car_.SetBrushTheta(brush_arm_theta_);
    local_map_ptr_ = LocalMap::GetInstancePointer();
}

// 更新机器人模型
void CollisionDetector::SetBrushArmTheta(const float &brush_arm_theta)
{
    brush_arm_theta_ = brush_arm_theta;
    collision_car_.SetBrushTheta(brush_arm_theta_);

}

void CollisionDetector::SetEgoPose(const Pose2D &ego_pose)
{
    ego_pose_ = ego_pose;
    collision_car_.SetEgoPose(ego_pose_);
}

bool CollisionDetector::IsCollisionEgoPose()
{
    Point2D left_head_brush, left_back_brush, right_back_brush, right_head_brush;
    collision_car_.Get4CornerBrushByMap(left_head_brush, left_back_brush, right_back_brush, right_head_brush);
    std::vector<Point2D> brush_ego_area{left_head_brush, left_back_brush, right_back_brush, right_head_brush};

    Point2D left_head_body, left_back_body, right_back_body, right_head_body;
    collision_car_.Get4CornerCarModelByMap(left_head_body, left_back_body, right_back_body, right_head_body);
    std::vector<Point2D> body_ego_area{left_head_body, left_back_body, right_back_body, right_head_body};
    return IsCollisionWithMap(brush_ego_area) || IsCollisionWithMap(body_ego_area);
}

bool CollisionDetector::GetCollisionIndex(const std::vector<PathPose2D> ego_path,
                                          size_t &collision_index)
{
    if (ego_path.empty())
    {
        LOG_INFO(gp_task_log, "车路径为空默认无碰撞");
        return false;
    }

    // 不同的曲率采用不同步长检测碰撞
    std::map<size_t, size_t> index_step_size;
    GetSegStepSizeByKappa(ego_path, index_step_size);
    size_t index[index_step_size.size()];
    size_t loop = 0;
    for (const auto &pair : index_step_size)
    {
        index[loop] = pair.first;
        loop++;
    }

    // 获取车身模型区域点
    collision_car_.SetEgoPose(ego_pose_);
    Point2D left_head_point, left_back_point, right_back_point, right_head_point;
    // 检测路径是否和障碍相撞，得到相撞最近的索引
    for (size_t i = 0; i < index_step_size.size(); i++)
    {
        size_t begin_index = (i == 0) ? 0 : index[i - 1];
        for (size_t j = begin_index; j <= index[i]; j += index_step_size[index[i]])
        {
            collision_car_.SetEgoPose(ego_path[j].pose);
            if (IsCollisionEgoPose())
            {
                if (j == begin_index)
                {
                    collision_index = j;
                    return true;
                }
                else
                {
                    for (size_t k = j - index_step_size[index[i]]; k <= j; k++)
                    {
                        collision_car_.SetEgoPose(ego_path[k].pose);
                        if (IsCollisionEgoPose())
                        {
                            collision_index = k;
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool CollisionDetector::GetPredictCollisionIndex(const std::vector<PathPose2D> ego_path,
                                                 const std::shared_ptr<std::vector<PredictObst>> &predict_obsts,
                                                 size_t &collision_index)
{
    if (ego_path.empty())
    {
        LOG_INFO(gp_task_log, "车路径为空默认无碰撞");
        return false;
    }

    if (predict_obsts->empty())
    {
        LOG_INFO(gp_task_log, "障碍物为空默认无碰撞");
        return false;
    }

    std::map<size_t, size_t> index_step_size;
    GetSegStepSizeByKappa(ego_path, index_step_size);
    size_t index[index_step_size.size()];
    size_t loop = 0;
    for (const auto &pair : index_step_size)
    {
        index[loop] = pair.first;
        loop++;
    }

    for (size_t i = 0; i < index_step_size.size(); i++)
    {
        size_t begin_index = (i == 0) ? 0 : index[i - 1];
        for (size_t j = begin_index; j <= index[i]; j += index_step_size[index[i]])
        {
            if (IsCollisionWithPredictObst(ego_path[j].pose, predict_obsts))
            {
                if (j == begin_index)
                {
                    collision_index = j;
                    return true;
                }
                else
                {
                    for (size_t k = j - index_step_size[index[i]]; k <= j; k++)
                    {
                        if (IsCollisionWithPredictObst(ego_path[k].pose, predict_obsts))
                        {
                            collision_index = k;
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}


// 获取车与障碍物碰撞的时间ttc(time to collision)
bool CollisionDetector::GetTTC(const std::vector<TrajPoint> &trajs,
                               const std::shared_ptr<std::vector<PredictObst>> &predict_obsts,
                               float &ttc)
{
    STGrash st_grash(trajs, *predict_obsts);
    std::vector<STPoint> traj_st_points;
    st_grash.GetTrajSTPoints(traj_st_points);
    if (traj_st_points.empty())
    {
        return false;
    }
    std::vector<AnyPolygon> obst_st_polygons;
    for (size_t i = 0; i < predict_obsts->size(); i++)
    {
        std::vector<STPoint> obst_st_vertex;
        st_grash.GetObstVertex(predict_obsts->at(i).id, obst_st_vertex);
        std::vector<Point2D> obst_vertex;
        for (size_t j = 0; j < obst_st_vertex.size(); j++)
        {
            obst_vertex.emplace_back(Point2D(obst_st_vertex[j].t, obst_st_vertex[j].s));
        }
        obst_st_polygons.emplace_back(obst_vertex);
    }
    if (obst_st_polygons.empty())
    {
        return false;
    }

    std::set<float> ts;
    Cube body_cube = collision_car_.GetCarBodyCubeByCar();
    CarPartsCube = collision_car_.GetBrushFoldCubeByCar();
    float car_length = body_cude.length + CarPartsCube.cube.length;
    float tail_to_rear_length = collision_car_.GetTailToRearAxleLength();
    for (size_t k = 0; k < traj_st_points.size(); k++)
    {
        DirectedLineSeg traj_line_seg(Point2D(traj_st_points[k].t, traj_st_points[k].s + car_length - tail_to_rear_length),
                                      Point2D(traj_st_points[k].t, traj_st_points[k].s - tail_to_rear_length));
        for (size_t m = 0; m < obst_st_polygons.size(); m++)
        {
            if (obst_st_polygons[m].IsIntersectWithLineSeg(traj_line_seg))
            {
                ts.insert(traj_st_points[k].t);
            }
        }
    }

    if (ts.empty())
    {
        return false;
    }

    ttc = *ts.begin();

    return true;
}


bool CollisionDetector::IsCollisionWithMap(const std::vector<Point2D> &area)
{
    if (local_map_ptr_ == nullptr)
    {
        return false;
    }
    std::vector<int> map_indices;
    if (local_map_ptr_->SearchPolygonIndices(area, map_indices))
    {
        return false;
    }
    for (size_t i = 0; i < map_indices.size(); i++)
    {
        CellProperty cell_property;
        if (dync_car.GetCellProperty(map_indices[i], cell_property))
        {
            if (IsCellObstacle(cell_property))
            {
                return true;
            }

        }
    }

    return false;
}


bool CollisionDetector::IsCellObstacle(const CellProperty& cell_property)
{
    return (cell_property.type == ObstPointTypeEnum::STATIC) ||
           (cell_property.type == ObstPointTypeEnum::HOLLOW) ||
           (cell_property.type == ObstPointTypeEnum::LOW) ||
           (cell_property.type == ObstPointTypeEnum::HOLD) ||
           (cell_property.type == ObstPointTypeEnum::BLIND_SPOT) ||
           (cell_property.type == ObstPointTypeEnum::SLOPE) ||
           (cell_property.type == ObstPointTypeEnum::BUMPER) ||
           (cell_property.type == ObstPointTypeEnum::TREELAWN) ||
           (cell_property.type == ObstPointTypeEnum::ULTRASONIC) ||
}

void CollisionDetector::GetSegStepSizeByKappa(const std::vector<PathPose2D> ego_path,
                                              std::map<size_t, size_t> &index_step_size)
{
    for (size_t i = 0; i < ego_path.size(); i++)
    {
        if (std::fabs(ego_path[i].kappa) >= 0.1)
        {
            index_step_size.emplace(i, 1);
        }
        else if(std::fabs(ego_path[i].kappa) >= 0.05)
        {
            index_step_size.emplace(i, 2);
        }
        else if (std::fabs(ego_path[i].kappa) >= 0.03)
        {
            index_step_size.emplace(i, 3);
        }
        else
        {
            index_step_size.emplace(i, 5);
        }
    }
}


AnyPolygon CollisionDetector::GetObstPathPolygon(const PredictObst &predict_obst)
{
    Pose2D first_obst = predict_obst.pose;
    float predict_time = 8.0;
    Pose2D last_obst(predict_obst.pose.x + predict_obst.vel * predict_time * std::cos(predict_obst.pose.theta),
                     predict_obst.pose.y + predict_obst.vel * predict_time * std::sin(predict_obst.pose.theta),
                     predict_obst.pose.theta);
    float obst_len = predict_obst.length;
    float obst_width = predict_obst.width;
    Pose2D left_head_local(last_obst.x + obst_len / 2.0f, last_obst.y + obst_width / 2.0f);
    Pose2D left_back_local(first_obst.x - obst_len / 2.0f, first_obst.y + obst_width / 2.0f);
    Pose2D right_back_local(first_obst.x - obst_len / 2.0f, first_obst.y - obst_width / 2.0f);
    Pose2D right_head_local(last_obst.x + obst_len / 2.0f, last_obst.y - obst_width / 2.0f);
    Pose2D left_head_global, left_back_global, right_back_global, right_head_global;
    LocalToGlobal2(last_obst, left_head_local, left_head_global);
    LocalToGlobal2(first_obst, left_back_local, left_back_global);
    LocalToGlobal2(first_obst, right_back_local, right_back_global);
    LocalToGlobal2(last_obst, right_head_local, right_head_global);
    std::vector<Point2D> obst_vertice{Point2D(left_head_global.x, left_head_global.y),
                                      Point2D(left_back_global.x, left_back_global.y),
                                      Point2D(right_back_global.x, right_back_global.y),
                                      Point2D(right_head_global.x, right_head_global.y)};
    AnyPolygon obst_polygon(obst_vertice);
    return obst_polygon;
}


bool CollisionDetector::IsCollisionWithPredictObst(const Pose2D &pose, 
                                                   const std::shared_ptr<std::vector<PredictObst>> &predict_obsts)
{
    collision_car_.SetEgoPose(pose);
    for (size_t i = 0; i < predict_obsts->size(); i++)
    {
        AnyPolygon obst_polygon = GetObstPathPolygon(predict_obsts->at(i));
        if (collision_car_.IsCollide(obst_polygon))
        {
            return true;
        }
    }

    return false;
}
