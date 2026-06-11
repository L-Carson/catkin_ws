/*
Author      : liuqunyang
Update Date : 2025/07/30
*/
#ifndef LOCAL_VIEW_H
#define LOCAL_VIEW_H

#include <memory>
#include "comm_layer/comm_base/comm_type.h"

struct PredictionObstacle
{
};

struct StaticObstacle
{
};

struct Localization
{
};

struct TrafficLightDetection
{
};

struct SpecialObjectivesDetection
{
};

struct Chassis
{
};

struct MovePartsMsg
{
};

struct TaskStageProgress
{
    int task_stage;
    DevCtrlMotionStateEnum exec_state;
};

struct HmiMsg
{
    bool is_cancle_task;
    TaskStageProgress stage_progress;
};

struct LocalView
{
    std::shared_ptr<PredictionObstacle> prediction_obstacles;
    std::shared_ptr<StaticObstacle> static_obstacles;
    std::shared_ptr<Localization> localization;
    std::shared_ptr<TrafficLightDetection> traffic_light;
    std::shared_ptr<SpecialObjectivesDetection> special_objectives;
    std::shared_ptr<Chassis> chassis;
    std::shared_ptr<MovePartsMsg> move_parts_msg;
    std::shared_ptr<HmiMsg> hmi_msg;
};

#endif
