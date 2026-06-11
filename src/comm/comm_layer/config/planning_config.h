/*
Author      : liuqunyang
Update Date : 2025/07/30
*/
#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>
#include <list>
#include <unordered_map>

enum class ScenarioTypeEnum
{
    LANE_FOLLOW = 0,
    LANE_CLEAN = 1,
    NARROW_ROAD_U_TURN = 2,
    NARROW_ROAD_PASS = 3,
    NARROW_ROAD_STATE_THROUGH = 4,
    OUT_DIFFICULT_ENVIRONMENT = 5,
    PATROL_CLEAN = 6,
    AVOID_BY_ROADSIDE = 7,
    PARKING = 8,
    CHARGING = 9,
    ADD_WATER = 10,
    EMPTY_TRASH = 11,
    THROW_RUBBISH = 12,
    MAX_SCENARIO_TYPE,
};

inline std::string GetScenarioName(const ScenarioTypeEnum &scenario_type)
{
    switch (scenario_type)
    {
    case ScenarioTypeEnum::LANE_FOLLOW:
        return "沿线巡逻场景";
    case ScenarioTypeEnum::LANE_CLEAN:
        return "沿线清扫场景";
    case ScenarioTypeEnum::NARROW_ROAD_U_TURN:
        return "窄道掉头场景";
    case ScenarioTypeEnum::NARROW_ROAD_PASS:
        return "窄道通行场景";
    case ScenarioTypeEnum::NARROW_ROAD_STATE_THROUGH:
        return "窄路桩通行场景";
    case ScenarioTypeEnum::OUT_DIFFICULT_ENVIRONMENT:
        return "脱困场景";
    case ScenarioTypeEnum::PATROL_CLEAN:
        return "巡扫场景";
    case ScenarioTypeEnum::AVOID_BY_ROADSIDE:
        return "靠边避让场景";
    case ScenarioTypeEnum::PARKING:
        return "泊车场景";
    case ScenarioTypeEnum::CHARGING:
        return "充电场景";
    case ScenarioTypeEnum::ADD_WATER:
        return "加水场景";
    case ScenarioTypeEnum::EMPTY_TRASH:
        return "倒垃圾场景";
    case ScenarioTypeEnum::THROW_RUBBISH:
        return "扔垃圾场景";
    }
}

enum class StageTypeEnum
{
    NO_STAGE = 0,
    LANE_FOLLOW_STAGE = 1,
    LANE_CLEAN_STAGE = 2,
    NARROW_ROAD_U_TURN_STAGE = 3,
    NARROW_ROAD_PASS_STAGE = 4,
    THROUGH_NARROW_ROAD_STAGE = 5,
    OUT_DIFFICULT_ENVIRONMENT_STAGE = 6,
    PATROL_CLEAN_STAGE = 7,
    CONTROL_DOOR_STAGE = 8,
    AVOID_BY_ROADSIDE_STAGE = 9,
    PARKING_STAGE = 10,
    ARRIVE_CHARGE_ASSIST_SPOT_STAGE = 11,
    ALIGNING_TO_CHARGING_STAGE = 12,
    ARRIVE_WATER_ASSIST_SPOT_STAGE = 13,
    ALIGNING_ADD_WATER_STAGE = 14,
    ARRIVE_TRASH_ASSIST_SPOT_STAGE = 15,
    ALIGNING_EMPTY_TRASH_STAGE = 16,
    THROW_RUBBISH_STAGE = 17,
};

inline std::string GetStageName(const StageTypeEnum &stage_type)
{
    switch (stage_type)
    {
    case StageTypeEnum::NO_STAGE:
        return "无阶段";
    case StageTypeEnum::LANE_FOLLOW_STAGE:
        return "沿线跟踪阶段";
    case StageTypeEnum::LANE_CLEAN_STAGE:
        return "沿线清扫阶段";
    case StageTypeEnum::NARROW_ROAD_U_TURN_STAGE:
        return "窄道掉头阶段";
    case StageTypeEnum::NARROW_ROAD_PASS_STAGE:
        return "窄道通行阶段";
    case StageTypeEnum::THROUGH_NARROW_ROAD_STAGE:
        return "窄路桩通行阶段";
    case StageTypeEnum::OUT_DIFFICULT_ENVIRONMENT_STAGE:
        return "脱困阶段";
    case StageTypeEnum::PATROL_CLEAN_STAGE:
        return "巡扫阶段";
    case StageTypeEnum::AVOID_BY_ROADSIDE_STAGE:
        return "靠边避让阶段";
    case StageTypeEnum::PARKING_STAGE:
        return "泊车阶段";
    case StageTypeEnum::ARRIVE_CHARGE_ASSIST_SPOT_STAGE:
        return "到达充电辅助点阶段";
    case StageTypeEnum::ALIGNING_TO_CHARGING_STAGE:
        return "充电对准阶段";
    case StageTypeEnum::ARRIVE_WATER_ASSIST_SPOT_STAGE:
        return "到达加水辅助点阶段";
    case StageTypeEnum::ALIGNING_ADD_WATER_STAGE:
        return "加水对准阶段";
    case StageTypeEnum::ARRIVE_TRASH_ASSIST_SPOT_STAGE:
        return "到达倒垃圾辅助点阶段";
    case StageTypeEnum::ALIGNING_EMPTY_TRASH_STAGE:
        return "倒垃圾对准阶段";
    case StageTypeEnum::THROW_RUBBISH_STAGE:
        return "扔垃圾阶段";
    }
}

enum class SubtaskTypeEnum
{
    NO_SUBTASK = 0,
    LANE_FOLLOW_PATH_GENERATOR = 1,
    LANE_FOLLOW_VEL_GENERATOR = 2,
    EDGE_CLEAN_DECIDER = 3,
    LANE_CLEAN_PATH_GENERATOR = 4,
    SPECIAL_OBJECTS_DECIDER = 5,
    GLOBAL_ROUTE_CHANGE_DECIDER = 6,
    LANE_CLEAN_VEL_GENERATOR = 7,
    NARROW_ROAD_U_TURN_PATH_GENERATOR = 8,
    NARROW_ROAD_U_TURN_VEL_GENERATOR = 9,
    THROUGH_NARROW_ROAD_PATH_GENERATOR = 10,
    THROUGH_NARROW_ROAD_VEL_GENERATOR = 11,
    OUT_DIFFICULT_ENVIRONMENT_TRAJ_GENERATOR = 12,
    LOOK_FOR_GARBAGE_DECIDER = 13,
    GARBAGE_CLUSTERING_DECIDER = 14,
    CLEAN_MISS_GARBAGE = 15,
    PATROL_CLEAN_PATH_GENERATOR = 16,
    PATROL_CLEAN_VEL_GENERATOR = 17,
    OPEN_SPACE_PATH_GENERATOR = 18,
    ALIGNING_CHANGE_DECIDER = 19,
    ALIGNING_EMPTY_TRASH_DECIDER = 20,
    ALIGNING_WATER_DECIDER = 21,
    THROW_RUBBISH_DECIDER = 22,
};

inline std::string GetSubtaskName(const SubtaskTypeEnum &subtask_type)
{
    switch (subtask_type)
    {
    case SubtaskTypeEnum::NO_SUBTASK:
        return "无子任务";
    case SubtaskTypeEnum::LANE_FOLLOW_PATH_GENERATOR:
        return "生成沿线路径子任务";
    case SubtaskTypeEnum::LANE_FOLLOW_VEL_GENERATOR:
        return "生成沿线速度子任务";
    case SubtaskTypeEnum::EDGE_CLEAN_DECIDER:
        return "贴边清扫决策子任务";
    case SubtaskTypeEnum::LANE_CLEAN_PATH_GENERATOR:
        return "生成清扫路径子任务";
    case SubtaskTypeEnum::SPECIAL_OBJECTS_DECIDER:
        return "特殊对象决策子任务";
    case SubtaskTypeEnum::GLOBAL_ROUTE_CHANGE_DECIDER:
        return "变更全局路由决策子任务";
    case SubtaskTypeEnum::LANE_CLEAN_VEL_GENERATOR:
        return "生成清扫速度子任务";
    case SubtaskTypeEnum::NARROW_ROAD_U_TURN_PATH_GENERATOR:
        return "生成窄道掉头路径子任务";
    case SubtaskTypeEnum::NARROW_ROAD_U_TURN_VEL_GENERATOR:
        return "生成窄道掉头速度子任务";
    case SubtaskTypeEnum::THROUGH_NARROW_ROAD_PATH_GENERATOR:
        return "生成窄路桩通行路径子任务";
    case SubtaskTypeEnum::THROUGH_NARROW_ROAD_VEL_GENERATOR:
        return "生成窄路桩通行速度子任务";
    case SubtaskTypeEnum::OUT_DIFFICULT_ENVIRONMENT_TRAJ_GENERATOR:
        return "生成脱困轨迹子任务";
    case SubtaskTypeEnum::LOOK_FOR_GARBAGE_DECIDER:
        return "寻找垃圾子任务";
    case SubtaskTypeEnum::GARBAGE_CLUSTERING_DECIDER:
        return "垃圾聚类子任务";
    case SubtaskTypeEnum::CLEAN_MISS_GARBAGE:
        return "清扫遗漏垃圾子任务";
    case SubtaskTypeEnum::PATROL_CLEAN_PATH_GENERATOR:
        return "生成巡扫路径子任务";
    case SubtaskTypeEnum::PATROL_CLEAN_VEL_GENERATOR:
        return "生成巡扫速度子任务";
    case SubtaskTypeEnum::OPEN_SPACE_PATH_GENERATOR:
        return "生成开放空间路径子任务";
    }
}

struct ScenarioConfig
{
    ScenarioTypeEnum scenario_type;
    std::vector<StageConfig> stage_configs;
};

struct StageConfig
{
    StageTypeEnum stage_type;
    std::vector<SubtaskConfig> subtask_configs;
    StageConfig(const StageTypeEnum &type, const std::vector<SubtaskConfig> &tasks) : stage_type(type), subtask_configs(tasks) {}
};

struct SubtaskConfig
{
    SubtaskTypeEnum subtask_type;
    float stop_distance;

    SubtaskConfig(const SubtaskTypeEnum &type) : subtask_type(type)
    {
        stop_distance = 0.0;
    }
};

class PlanningConfig final
{
public:
    PlanningConfig()
    {
        Init();
    }
    void Init();
    void InitSubtasks();
    void InitStages();
    std::list<ScenarioTypeEnum> GetScenarios() const;
    std::vector<StageConfig> GetStages(const ScenarioTypeEnum &scenario_type) const;
    std::vector<SubtaskConfig> GetSubtasks(const StageTypeEnum &stage_type) const;

private:
    std::list<ScenarioTypeEnum> scenarios_;
    std::unordered_map<ScenarioTypeEnum, std::vector<StageConfig>> stages_;
    std::unordered_map<StageTypeEnum, std::vector<SubtaskConfig>> subtasks_;
};

#endif
