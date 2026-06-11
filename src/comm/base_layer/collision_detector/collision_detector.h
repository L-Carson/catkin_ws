/*
Author      : liuqunyang
Update Date : 2025/08/21
*/
#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <map>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_math/comm_math.h"
#include "base_layer/vehicle_model/collision_car.h"
#include "base_layer/log_manager/log_manager.h"
#include "collision_detector/st_grash.h"
#include "map_layer/local_map/local_map.h"


// 碰撞检测器
class CollisionDetector
{
public:
    CollisionDetector(const Pose2D& ego_pose_,  const float& brush_arm_theta_);

    // 设置更新机器人扫刷臂展开角度
    void SetBrushArmTheta(const float &brush_arm_theta);

    // 设置更新机器人位姿
    void SetEgoPose(const Pose2D &ego_pose);

    // 车体本身是否碰撞
    bool IsCollisionEgoPose();

    // 获取车路径碰撞的索引
    bool GetCollisionIndex(const std::vector<PathPose2D> ego_path,  
                           size_t &collision_index);

    // 获取车路径与预测障碍碰撞的索引
    bool GetPredictCollisionIndex(const std::vector<PathPose2D> ego_path,  
                                  const std::shared_ptr<std::vector<PredictObst>> &predict_obsts,
                                  size_t &collision_index);

    // 获取车与障碍物碰撞的时间ttc(time to collision)
    bool GetTTC(const std::vector<TrajPoint> &trajs,  
                const std::shared_ptr<std::vector<PredictObst>> &predict_obsts,
                float &ttc);

private:
    // 检测是否碰撞
    bool IsCollisionWithMap(const std::vector<Point2D> &area);
    bool IsCellObstacle(const CellProperty &cell_property);
    void GetSegStepSizeByKappa(const std::vector<PathPose2D> ego_path, std::map<int, int> &index_step_size);
    AnyPolygon GetObstPathPolygon(const PredictObst &predict_obst);
    bool IsCollisionWithPredictObst(const Pose2D &pose, 
                                    const std::shared_ptr<std::vector<PredictObst>> &predict_obsts);


private:
    Pose2D ego_pose_;
    float brush_arm_theta_{0.0f};
    CollisionCar collision_car_;

    LocalMap *local_map_ptr_{nullptr};
};

#endif
