#include "base_layer/collision_detector/st_grash.h"


void STGrash::Init()
{
    CalaculateTrajSTPoints();
    CalaculateObstSTRect();
}


void STGrash::UpdateTrajPoints(const std::vector<TrajPoint> &traj_points)
{
    traj_points_ = traj_points;
    CalaculateTrajSTPoints();
}

void STGrash::UpdatePredictObsts(const std::vector<PredictObst> &predict_obsts)
{
    predict_obsts_ = predict_obsts;
    CalaculateObstSTRect();
}


void STGrash::GetObstVertex(cosnt unsigned int& obst_id, std::vector<STPoint>& obst_st_vertex)
{
    obst_st_vertex = obst_st_vertex_map_[obst_id];
}

void STGrash::GetTrajSTPoints(std::vector<STPoint>& traj_st_points)
{
    traj_st_points = traj_st_points_;
}

void STGrash::CalaculateTrajSTPoints()
{
    if (traj_points_.size() <= 1)
    {
        return;
    }

    for (size_t i = 0; i < traj_points_.size(); i++)
    {
        if (traj_points_[i].t > max_t_) 
        {
            break;
        }
        STPoint st_point;
        st_point.s = traj_points_[i].s;
        st_point.t = traj_points_[i].t;
        traj_st_points_.push_back(st_point);
    }

}


void STGrash::CalaculateObstSTRect()
{
    CarBody car_body;
    Cube body_cube = car_body.GetCarBodyCubeByCar();
    CarPartsCube = car_body.GetBrushFoldCubeByCar();
    float car_length = body_cude.length + CarPartsCube.cube.length;
    float car_width = std::max(body_cube.width, CarPartsCube.cube.width);
    float diff_center_to_rear_axle = car_length / 2.0f - car_body.GetTailToRearAxleLength();
    for (size_t i = 0; i < predict_obsts_.size(); i++)
    {
        float obst_s = 0.0f;
        bool is_collision = false;
        STPoint left_bottom, left_upper, right_bottom, right_upper;
        for (size_t j = 0; j < predict_obsts_[i].traj_poses.size(); j++)
        {
            Point2D obst_center(predict_obsts_[i].traj_poses[j].pose.x, predict_obsts_[i].traj_poses[j].pose.y);
            float obst_heading = predict_obsts_[i].traj_poses[j].pose.theta;
            if (j > 0)
            {
                obst_s += CalcDis(predict_obsts_[i].traj_poses[j].pose.x, predict_obsts_[i].traj_poses[j].pose.y,
                                predict_obsts_[i].traj_poses[j - 1].pose.x, predict_obsts_[i].traj_poses[j - 1].pose.y);
            }
            float obst_t = obst_s / predict_obsts_[i].vel;
            OrientedBox obst_box(obst_center, predict_obsts_[i].width, predict_obsts_[i].height, obst_heading);
            for (size_t k = 0; k < traj_points_.size(); k+=2)
            {
                Pose2D traj_pose = traj_points_[k].pose;
                Point2D traj_point_center(traj_pose.x + diff_center_to_rear_axle * std::cos(traj_pose.theta), 
                                          traj_pose.y + diff_center_to_rear_axle * std::sin(traj_pose.theta));
                OrientedBox traj_box(traj_point_center, car_length, car_width, traj_pose.theta);
                if (traj_box.IsIntersecting(obst_box))
                {
                    if (!is_collision)
                    {
                        float collosion_s;
                        for (size_t m = k; m < traj_points_.size(); m+=2)
                        {
                            Pose2D traj_pose_m = traj_points_[m].pose;
                            Point2D traj_point_center_m(traj_pose_m.x + diff_center_to_rear_axle * std::cos(traj_pose_m.theta), 
                                                    traj_pose_m.y + diff_center_to_rear_axle * std::sin(traj_pose_m.theta));
                            OrientedBox traj_box_m(traj_point_center_m, car_length, car_width, traj_pose_m.theta);
                            if (!traj_box_m.IsIntersecting(obst_box))
                            {
                                collosion_s = (traj_points_[m].s + traj_points_[k].s) / 2.0f;
                                break;
                            }
                        }
                        is_collision = true;
                        left_bottom = STPoint(collosion_s - predict_obsts_[i].width / 2.0f, obst_t);
                        left_upper = STPoint(collosion_s + predict_obsts_[i].width / 2.0f, obst_t);
                    }
                }
                else
                {
                    if (is_collision)
                    {
                        is_collision = false;
                        float collosion_s;
                        for (size_t m = k; m < traj_points_.size(); m+=2)
                        {
                            Pose2D traj_pose_m = traj_points_[m].pose;
                            Point2D traj_point_center_m(traj_pose_m.x + diff_center_to_rear_axle * std::cos(traj_pose_m.theta), 
                                                    traj_pose_m.y + diff_center_to_rear_axle * std::sin(traj_pose_m.theta));
                            OrientedBox traj_box_m(traj_point_center_m, car_length, car_width, traj_pose_m.theta);
                            if (!traj_box_m.IsIntersecting(obst_box))
                            {
                                collosion_s = (traj_points_[m].s + traj_points_[k].s) / 2.0f;
                                break;
                            }
                        }
                        right_bottom = STPoint(collosion_s - predict_obsts_[i].width / 2.0f, obst_t);
                        right_upper = STPoint(collosion_s + predict_obsts_[i].width / 2.0f, obst_t);
                        break;
                    }
                }
            }
        }

        std::vector<STPoint> obst_st_rect{left_bottom, left_upper, right_upper, right_bottom};
        obst_st_vertex_map_.emplace(predict_obsts_[i].id, obst_st_rect);
    }
}