/*
Author      : liuqunyang
Update Date : 2025/07/30
*/
#include "comm_layer/config/planning_config.h"

void PlanningConfig::Init()
{
    InitSubtasks();
    InitStages();

    for (size_t i = 0; i < static_cast<size_t>(ScenarioTypeEnum::MAX_SCENARIO_TYPE); i++)
    {
        scenarios_.emplace_back(static_cast<ScenarioTypeEnum>(i));
    }
}

void PlanningConfig::InitStages()
{
    std::vector<StageConfig> lane_follow_stage_configs = {
        StageConfig(StageTypeEnum::LANE_FOLLOW_STAGE, GetSubtasks(StageTypeEnum::LANE_FOLLOW_STAGE))};
    stages_.emplace(ScenarioTypeEnum::LANE_FOLLOW, lane_follow_stage_configs);

    std::vector<StageConfig> lane_clean_stage_configs = {
        StageConfig(StageTypeEnum::LANE_CLEAN_STAGE, GetSubtasks(StageTypeEnum::LANE_CLEAN_STAGE))};
    stages_.emplace(ScenarioTypeEnum::LANE_CLEAN, lane_clean_stage_configs);

    std::vector<StageConfig> narrow_road_u_turn_configs = {
        StageConfig(StageTypeEnum::NARROW_ROAD_U_TURN_STAGE, GetSubtasks(StageTypeEnum::NARROW_ROAD_U_TURN_STAGE))};
    stages_.emplace(ScenarioTypeEnum::NARROW_ROAD_U_TURN, narrow_road_u_turn_configs);

    std::vector<StageConfig> narrow_road_pass_configs = {
        StageConfig(StageTypeEnum::NARROW_ROAD_PASS_STAGE, GetSubtasks(StageTypeEnum::NARROW_ROAD_PASS_STAGE))};
    stages_.emplace(ScenarioTypeEnum::NARROW_ROAD_PASS, narrow_road_pass_configs);

    std::vector<StageConfig> through_narrow_road_configs = {
        StageConfig(StageTypeEnum::THROUGH_NARROW_ROAD_STAGE, GetSubtasks(StageTypeEnum::THROUGH_NARROW_ROAD_STAGE))};
    stages_.emplace(ScenarioTypeEnum::NARROW_ROAD_STATE_THROUGH, through_narrow_road_configs);

    std::vector<StageConfig> out_difficult_configs = {
        StageConfig(StageTypeEnum::OUT_DIFFICULT_ENVIRONMENT_STAGE, GetSubtasks(StageTypeEnum::OUT_DIFFICULT_ENVIRONMENT_STAGE))};
    stages_.emplace(ScenarioTypeEnum::OUT_DIFFICULT_ENVIRONMENT, out_difficult_configs);

    std::vector<StageConfig> patrol_clean_configs = {
        StageConfig(StageTypeEnum::PATROL_CLEAN_STAGE, GetSubtasks(StageTypeEnum::PATROL_CLEAN_STAGE))};
    stages_.emplace(ScenarioTypeEnum::PATROL_CLEAN, patrol_clean_configs);

    std::vector<StageConfig> patrol_clean_configs = {
        StageConfig(StageTypeEnum::AVOID_BY_ROADSIDE_STAGE, GetSubtasks(StageTypeEnum::PATROL_CLEAN_STAGE))};
    stages_.emplace(ScenarioTypeEnum::AVOID_BY_ROADSIDE, patrol_clean_configs);

    std::vector<StageConfig> parking_configs = {
        StageConfig(StageTypeEnum::PARKING_STAGE, GetSubtasks(StageTypeEnum::PARKING_STAGE))};
    stages_.emplace(ScenarioTypeEnum::PARKING, parking_configs);

    std::vector<StageConfig> charging_configs = {
        StageConfig(StageTypeEnum::ARRIVE_CHARGE_ASSIST_SPOT_STAGE, GetSubtasks(StageTypeEnum::ARRIVE_CHARGE_ASSIST_SPOT_STAGE)),
        StageConfig(StageTypeEnum::ALIGNING_TO_CHARGING_STAGE, GetSubtasks(StageTypeEnum::ALIGNING_TO_CHARGING_STAGE))};
    stages_.emplace(ScenarioTypeEnum::CHARGING, charging_configs);

    std::vector<StageConfig> add_water_configs = {
        StageConfig(StageTypeEnum::ARRIVE_WATER_ASSIST_SPOT_STAGE, GetSubtasks(StageTypeEnum::ARRIVE_WATER_ASSIST_SPOT_STAGE)),
        StageConfig(StageTypeEnum::ALIGNING_ADD_WATER_STAGE, GetSubtasks(StageTypeEnum::ALIGNING_ADD_WATER_STAGE))};
    stages_.emplace(ScenarioTypeEnum::ADD_WATER, add_water_configs);

    std::vector<StageConfig> empty_trash_configs = {
        StageConfig(StageTypeEnum::ARRIVE_TRASH_ASSIST_SPOT_STAGE, GetSubtasks(StageTypeEnum::ARRIVE_TRASH_ASSIST_SPOT_STAGE)),
        StageConfig(StageTypeEnum::ALIGNING_EMPTY_TRASH_STAGE, GetSubtasks(StageTypeEnum::ALIGNING_EMPTY_TRASH_STAGE))};
    stages_.emplace(ScenarioTypeEnum::EMPTY_TRASH, empty_trash_configs);

    std::vector<StageConfig> throw_rubbish_configs = {
        StageConfig(StageTypeEnum::THROW_RUBBISH_STAGE, GetSubtasks(StageTypeEnum::THROW_RUBBISH_STAGE))};
    stages_.emplace(ScenarioTypeEnum::THROW_RUBBISH, throw_rubbish_configs);
}

void PlanningConfig::InitSubtasks()
{
    std::vector<SubtaskConfig> lane_follow_configs = {
        SubtaskConfig(SubtaskTypeEnum::LANE_FOLLOW_PATH_GENERATOR),
        SubtaskConfig(SubtaskTypeEnum::LANE_FOLLOW_VEL_GENERATOR)};
    subtasks_.emplace(StageTypeEnum::LANE_FOLLOW_STAGE, lane_follow_configs);

    std::vector<SubtaskConfig> parking_configs = {
        SubtaskConfig(SubtaskTypeEnum::OPEN_SPACE_PATH_GENERATOR)};
    subtasks_.emplace(StageTypeEnum::PARKING_STAGE, parking_configs);

    std::vector<SubtaskConfig> arrive_charge_assist_spot_configs = {
        SubtaskConfig(SubtaskTypeEnum::OPEN_SPACE_PATH_GENERATOR)};
    subtasks_.emplace(StageTypeEnum::ARRIVE_CHARGE_ASSIST_SPOT_STAGE, arrive_charge_assist_spot_configs);

    std::vector<SubtaskConfig> aligning_to_charging_configs = {
        SubtaskConfig(SubtaskTypeEnum::ALIGNING_CHANGE_DECIDER)};
    subtasks_.emplace(StageTypeEnum::ALIGNING_TO_CHARGING_STAGE, aligning_to_charging_configs);

    std::vector<SubtaskConfig> arrive_water_assist_spot_configs = {
        SubtaskConfig(SubtaskTypeEnum::OPEN_SPACE_PATH_GENERATOR)};
    subtasks_.emplace(StageTypeEnum::ARRIVE_WATER_ASSIST_SPOT_STAGE, arrive_water_assist_spot_configs);

    std::vector<SubtaskConfig> aligning_water_configs = {
        SubtaskConfig(SubtaskTypeEnum::ALIGNING_WATER_DECIDER)};
    subtasks_.emplace(StageTypeEnum::ALIGNING_ADD_WATER_STAGE, aligning_water_configs);

    std::vector<SubtaskConfig> arrive_trash_assist_spot_configs = {
        SubtaskConfig(SubtaskTypeEnum::OPEN_SPACE_PATH_GENERATOR)};
    subtasks_.emplace(StageTypeEnum::ARRIVE_TRASH_ASSIST_SPOT_STAGE, arrive_trash_assist_spot_configs);

    std::vector<SubtaskConfig> aligning_empty_trash_configs = {
        SubtaskConfig(SubtaskTypeEnum::ALIGNING_EMPTY_TRASH_DECIDER)};
    subtasks_.emplace(StageTypeEnum::ALIGNING_EMPTY_TRASH_STAGE, aligning_empty_trash_configs);

    std::vector<SubtaskConfig> throw_rubbish_configs = {
        SubtaskConfig(SubtaskTypeEnum::THROW_RUBBISH_DECIDER)};
    subtasks_.emplace(StageTypeEnum::THROW_RUBBISH_STAGE, throw_rubbish_configs);
}

std::list<ScenarioTypeEnum> PlanningConfig::GetScenarios() const
{
    return scenarios_;
}

std::vector<StageConfig> PlanningConfig::GetStages(const ScenarioTypeEnum &scenario_type) const
{
    return stages_.at(scenario_type);
}

std::vector<SubtaskConfig> PlanningConfig::GetSubtasks(const StageTypeEnum &stage_type) const
{
    return subtasks_.at(stage_type);
}
