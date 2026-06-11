#ifndef __COMM_TOPIC_NAME_H__
#define __COMM_TOPIC_NAME_H__

#include <string>

#define TOPIC_NAME_DCU_SERIAL_PASS_DATA                         "/dsp_serial_pass_data"
#define TOPIC_NAME_HMI_SERIAL_PASS_DATA                         "/mmis_serial_pass_data"

#define TOPIC_NAME_MODULE_ABNORMAL_INFO                         "/module_abnormal_info"

#define TOPIC_NAME_WHEEL_SPEED                                  "/wheel_speed"
#define TOPIC_NAME_GNSS_INFO                                    "/gps_info"
#define TOPIC_NAME_IPU                                          "/ipu"
#define TOPIC_NAME_DAQ_CTRL                                     "/daq_ctrl"                                                             //数据采集控制

#define TOPIC_NAME_VISION_SELECT_DEPTH_CAMERA                   "/vision_select_depth_camera"                                           //选择深度相机
#define TOPIC_NAME_VISION_PCLOUD_SRC                            "/vision_point_cloud_src"                                               //深度相机原始点云数据
#define TOPIC_NAME_VISION_PCLOUD_TRANS                          "/vision_point_cloud_trans"                                             //深度相机tf后点云数据
#define TOPIC_NAME_VISION_PCLOUD_OBST                           "/vision_point_cloud_obst"                                              //深度相机障碍数据
#define TOPIC_NAME_DCAMERA_FUSED_POINTS                         "/dcamera/fused_depth_points"                                           //融合处理后的深度点云数据
#define TOPIC_NAME_VISION_FUNC                                  "/vision_function"                                                      //视觉功能调度
#define TOPIC_NAME_VISION_DET_INFO                              "/vision_detect"                                                        //视觉网络检测结果
#define TOPIC_NAME_VISION_DET_IMAGE(strCameraName_)             std::string("/camera/" + strCameraName_ + "/detect_image")              //视觉网络检测结果
#define TOPIC_NAME_VISION_SEG_INFO                              "/vision_segment"                                                       //视觉网络分割结果
#define TOPIC_NAME_VISION_SEG_IMAGE(strCameraName_)             std::string("/camera/" + strCameraName_ + "/seg_image")
#define TOPIC_NAME_VISION_SEG_MASK_IMAGE(strCameraName_)        std::string("/camera/" + strCameraName_ + "/seg_mask_image")
#define TOPIC_NAME_VISION_FILTER_SEG_MASK_IMAGE(strCameraName_) std::string("/camera/" + strCameraName_ + "/filter_seg_mask_image")
#define TOPIC_NAME_VISION_QR_CODE_DETECT_IMAGE(strCameraName_)  std::string("/camera/" + strCameraName_ + "/qr_code_detect_image")
#define TOPIC_NAME_VISION_CAMERA_POSE(strCameraName_)           std::string("/camera/" + strCameraName_ + "/camera_pose")
#define TOPIC_NAME_VISION_CAMERA_MARKER_POSE(strCameraName_)    std::string("/camera/" + strCameraName_ + "/detect_marker_pose")
#define TOPIC_NAME_VISION_CAMERA_TARGET_POSE(strCameraName_)    std::string("/camera/" + strCameraName_ + "/detect_target_pose")
#define TOPIC_NAME_VISION_GARBAGE_INFO                          "/garbage/garbage_segment"                                              //垃圾（长沙项目接口）

#define TOPIC_NAME_SENSOR_ULTRASONIC_POINTS                     "/sensor_us_points"                                                     // 超声波映射点云数据（用于rviz查看）
#define TOPIC_NAME_SENSOR_ULTRASONIC_DATA                       "/sensor_us_data"                                                       // 超声波映射数据（用于感知）
#define TOPIC_NAME_SENSOR_TOF_POINTS                            "/sensor_tof_points"                                                    // tof点云数据

#define TOPIC_NAME_DCU_FUSED_STATIC_OBST                        "/dcu/fused_static_obst"
#define TOPIC_NAME_DCU_FUSED_STATIC_2D_POINT_BY_CAR             "/dcu/fused_static_2d_point_by_car"

#define TOPIC_NAME_PCPT_REGION_ROAD_MAP                         "pcpt/region_road_map"
#define TOPIC_NAME_PCPT_REGION_FILTER_MAP                       "pcpt/region_filter_map"

#define TOPIC_NAME_FUSED_GROUND_3D_POINTS                       "/pcpt/fused_ground_3d_points"
#define TOPIC_NAME_FUSED_NOGROUND_3D_POINTS                     "/pcpt/fused_noground_3d_points"
#define TOPIC_NAME_FUSED_UNDERGROUND_3D_POINTS                  "/pcpt/fused_underground_3d_points"
#define TOPIC_NAME_FUSED_UNDERGROUND_VALID_POINTS               "/pcpt/fused_underground_valid_points"
#define TOPIC_NAME_FUSED_UNDERGROUND_FILTERED_POINTS            "/pcpt/fused_underground_filtered_points"
#define TOPIC_NAME_FUSED_OVERGROUND_3D_POINTS                   "/pcpt/fused_overground_3d_points"
#define TOPIC_NAME_FUSED_GENERAL_3D_POINTS                      "/pcpt/fused_general_3d_points"
#define TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_MAP                 "/pcpt/fused_static_2d_point_by_map"
#define TOPIC_NAME_FUSED_STATIC_2D_POINT_BY_CAR                 "/pcpt/fused_static_2d_point_by_car"
#define TOPIC_NAME_FUSED_STATIC_OBST                            "/pcpt/fused_static_obst"
#define TOPIC_NAME_FUSED_MOV_TARGETS                            "/pcpt/fused_mov_target"
#define TOPIC_NAME_PCPT_ACCE_GROUND_POINTS                      "/pcpt/acce_ground_points"
#define TOPIC_NAME_PCPT_ACCE_HOLLOW_DET_POINTS                  "/pcpt/acce_hollow_det_points"
#define TOPIC_NAME_PCPT_ACCE_HOLLOW_HOLD_POINTS                 "/pcpt/acce_hollow_hold_points"
#define TOPIC_NAME_PCPT_ROAD_EDGE_POINTS                        "/pcpt/road_edge_points"
#define TOPIC_NAME_PCPT_STEP_PAD_POINTS                         "/pcpt/step_pad_points"
#define TOPIC_NAME_PCPT_OBST_WITH_LABEL                         "/pcpt/obst_with_label"

#define TOPIC_NAME_EMERGENCE_REFLECT_INVALID_POINTS             "/pcpt/lidar_emergence_reflect_invalid_points"
#define TOPIC_NAME_COMMON_OBST_HOLD_POINTS_BY_MAP               "/pcpt/common_obst_hold_points_by_map"
#define TOPIC_NAME_COMMON_OBST_HOLD_POINTS_BY_CAR               "/pcpt/common_obst_hold_points_by_car"

#define TOPIC_NAME_MARKER_RADAR_DETECTOR                        "/pcpt/marker_radar_detector"
#define TOPIC_NAME_MARKER_PCD_CLUSTER                           "/pcpt/marker_pcd_cluster"
#define TOPIC_NAME_MARKER_PCD_NET_BOX                           0 ? "/pcpt/marker_pcd_net_box" : "/box_msgs"
#define TOPIC_NAME_MARKER_TARGETS_3D_BOX                        "/pcpt/marker_targets_3d_box"
#define TOPIC_NAME_MARKER_TARGETS_TRACK_PRED                    "/pcpt/marker_targets_track_prediction"
#define TOPIC_NAME_MARKER_TRASH_BOX                             "/pcpt/marker_trash_box"
#define TOPIC_NAME_MARKER_DUSTBIN                               "/pcpt/marker_dustbin"
#define TOPIC_NAME_MARKER_DUSTBIN_CAPACITY                      "/pcpt/marker_dustbin_capacity"
#define TOPIC_NAME_MARKER_EQUIPMENT                             "/pcpt/marker_equipment"
#define TOPIC_NAME_MARKER_PLANE_SEG                             "/pcpt/marker_plane_seg"
#define TOPIC_NAME_MARKER_ROAD_EDGE_LINE                        "/pcpt/marker_road_edge_line"

#define TOPIC_NAME_LAYER_CMG                                    "/pcpt/layer_cmd"

#define TOPIC_NAME_SERVER_PCD_NET                               "/pcpt/pcd_net_server"
#define TOPIC_NAME_SERVER_RGB_CAMERA_PARAM                      "/pcpt/rgb_camera_param_server"
#define TOPIC_NAME_SERVER_VISION_FUNC                           "/pcpt/vision_function_server"
#define TOPIC_NAME_SERVER_VISION_FUNC_SIM                       "/pcpt/vision_function_sim_server"
#define TOPIC_NAME_SERVER_CLBT_RGB_CAMERA_PARAM                 "/clbt_rgb_camera_param_server"
#define TOPIC_NAME_SERVER_CLBT_LIDAR_PARAM                      "/clbt_lidar_param_server"
#define TOPIC_NAME_SERVER_CLBT_IMU_PARAM                        "/clbt_imu_param_server"
#define TOPIC_NAME_SERVER_CLBT_LIDAR_OFFSET_PARAM               "/clbt_lidar_offset_param_server"
#define TOPIC_NAME_SERVER_CLBT_IMU_OFFSET_PARAM                 "/clbt_imu_offset_param_server"

#define TOPIC_NAME_PCPT_MAPPING_ROAD_EDGE_POINT                 "/pcpt_mapping/road_edge_point"

#define TOPIC_NAME_LOCATION_INFO                                "/location_info"
#define TOPIC_NAME_3D_LOCATION_INFO_SAIJIE                      "/location_info_3d"
#define TOPIC_NAME_3D_LOCATION_INFO_W2                          "/mnl/robot_pose"
#define TOPIC_NAME_QRCODE_POSE                                  "/qrcode_pose"

#define TOPIC_NAME_SHOW_PC_PATH                                 "/move_base/NavfnROS/plan"
#define TOPIC_NAME_SHOW_WORK_PATH                               "show/work_path"
#define TOPIC_NAME_SHOW_HISTORY_TRJ                             "/car_history_trj"

#define TOPIC_NAME_SHOW_PC_MAP                                  "map"

#define TOPIC_NAME_CHARGE_CMD                                   "/charge_cmd"
#define TOPIC_NAME_TRASH_BIN_DUMP_CMD                           "/trash_bin_dump_cmd"
#define TOPIC_NAME_TRASH_COVER_CTRL_CMD                         "/trash_cover_ctrl_cmd"
#define TOPIC_NAME_TRASH_BAG_PACK_CMD                           "/trash_bag_pack_cmd"
#define TOPIC_NAME_CHARGING_DOOR_CMD                              "/charge_door_cmd"
#define TOPIC_NAME_TRASH_BOX_OBST_STATE                         "/trash_box_obst_state"
#define TOPIC_NAME_TRASH_BOX_AREA_OBST_STATE                    "/trash_box_area_obst_state"
#define TOPIC_NAME_DUSTBIN_STATE                                "/dustbin_state"
#define TOPIC_NAME_TASK_CTRL                                    "/task_ctrl"
#define TOPIC_NAME_REMOTE_STOP_CTRL                             "/remote_stop_ctrl"
#define TOPIC_NAME_NAV_STATE                                    "/nav_state"
#define TOPIC_NAME_MOVE_CTRL                                    "/move_ctrl"
#define TOPIC_NAME_NAV_ALGO_INFO                                "/nav_algo_info"
#define TOPIC_NAME_MOVE_INFO                                    "/move_info"
#define TOPIC_NAME_DCU_MOVE_CTRL                                "/dcu_move_ctrl"
#define TOPIC_NAME_DCU_INFO                                     "/dcu_info"
#define TOPIC_NAME_DEV_INFO                                     "/dev_info"
#define TOPIC_NAME_EQUIPMENT_INFO                               "/equipment_info"
#define TOPIC_NAME_EQUIPMENT_POSE                               "/equipment_pose"
#define TOPIC_NAME_BODY_POSE                                    "/body_pose"
#define TOPIC_NAME_DETECT_TARGET_DOOR                           "/detect_target_door"
#define TOPIC_NAME_DETECT_RESULT_DOOR                           "/detect_result_door"
#define TOPIC_NAME_SECURITY_REDUNDANCY                          "/security_redundancy"
#define TOPIC_NAME_SPOILER_RANGE_DETECT                         "/spolier_range_detect"

#define TOPIC_NAME_TRAFFIC_LIGHT_RESULT                         "/traffic_light_result"
#define TOPIC_NAME_TRAFFIC_LIGHT_TARGET                         "/traffic_light_target"

#define TOPIC_NAME_INIT_POSE                                    "/initialpose"
#define TOPIC_NAME_GOAL_POSE                                    "/move_base_simple/goal"
#define TOPIC_NAME_SIMULATION_FULL_OBSTS                        "/simulation_obsts"
#define TOPIC_NAME_SIMULATION_INC_OBST                          "/clicked_point"
#define TOPIC_NAME_SIMULATION_INC_GARBAGE                       "/clicked_point_2"
#define TOPIC_NAME_SIMULATION_INC_PIPE                          "/clicked_point_4"
#define TOPIC_NAME_SIMULATION_INC_WIRE                          "/clicked_point_5"
#define TOPIC_NAME_SIMULATION_INC_RISK                          "/clicked_point_6"
#define TOPIC_NAME_TOOL_DYNC_MAP_VERTICE                        "/clicked_point_3"
#define TOPIC_NAME_TOOL_POINT                                   "/clicked_point"
#define TOPIC_NAME_SIMULATION_DYNAMIC_OBST                      "/move_base_simple/goal"
#define TOPIC_NAME_SIMULATION_DYNC_MAP_CENTER_TO_LOC            "/simulation/dync_map/center_to_loc"
#define TOPIC_NAME_SIMULATION_DYNC_MAP_LOC_TO_CENTER            "/simulation/dync_map/loc_to_center"

#define TOPIC_NAME_CAMERA_IMAGE(strCameraName_)                 std::string("/camera/" + strCameraName_)

#define TOPIC_NAME_RADAR_LEFT_OBST                              "/radar/left_radar_obst"
#define TOPIC_NAME_RADAR_FRONT_OBST                             "/radar/front_radar_obst"
#define TOPIC_NAME_RADAR_RIGHT_OBST                             "/radar/right_radar_obst"
#define TOPIC_NAME_RADAR_BACK_OBST                              "/radar/back_radar_obst"

#define TOPIC_NAME_RADAR_LEFT_MARKER                            "/radar/left_radar_marker"
#define TOPIC_NAME_RADAR_FRONT_MARKER                           "/radar/front_radar_marker"
#define TOPIC_NAME_RADAR_RIGHT_MARKER                           "/radar/right_radar_marker"
#define TOPIC_NAME_RADAR_BACK_MARKER                            "/radar/back_radar_marker"

#define TOPIC_NAME_RADAR_LEFT_MARKER_SRC                        "/radar/left_radar_marker_src"
#define TOPIC_NAME_RADAR_FRONT_MARKER_SRC                       "/radar/front_radar_marker_src"
#define TOPIC_NAME_RADAR_RIGHT_MARKER_SRC                       "/radar/right_radar_marker_src"
#define TOPIC_NAME_RADAR_BACK_MARKER_SRC                        "/radar/back_radar_marker_src"

#define TOPIC_NAME_SHOW_SIMULATION_ENV_MAP                      "show/simulation/env_map"
#define TOPIC_NAME_SHOW_SIMULATION_TOOL_OBSTS                   "show/simulation/tool_obsts"
#define TOPIC_NAME_SHOW_SIMULATION_TOOL_GARBAGES                "show/simulation/tool_garbages"
#define TOPIC_NAME_SHOW_SIMULATION_TOOL_PIPES                   "show/simulation/tool_pipes"
#define TOPIC_NAME_SHOW_SIMULATION_TOOL_WIRES                   "show/simulation/tool_wires"
#define TOPIC_NAME_SHOW_SIMULATION_TOOL_RISK                    "show/simulation/tool_risk"
#define TOPIC_NAME_SHOW_SIMULATION_TASK_PATH                    "show/simulation/task_path"

#define TOPIC_NAME_SHOW_ROAD_NETWORK                            "show/global_plan/road_network"
#define TOPIC_NAME_SHOW_TASK_PATH                               "show/global_plan/task_path"
#define TOPIC_NAME_SHOW_TASK_LANE                               "show/global_plan/task_lane"
#define TOPIC_NAME_SHOW_TASK_LANE_TEST_PROGRESS                 "show/global_plan/task_lane_test_grogress"

#define TOPIC_NAME_SHOW_CAR_PROJECT_ROUTE                       "show/loc_plan/car_project_route"
#define TOPIC_NANE_SHOW_LEFT_ROUTE                              "show/loc_plan/left_route"
#define TOPIC_NAME_SHOW_LANE_ROUTE_LINE_MATCH                   "show/loc_plan/lane_route_line_match"
#define TOPIC_NAME_SHOW_REGION_MAP                              "show/loc_plan/region_map"
#define TOPIC_NAME_SHOW_STATIC_MAP                              "show/loc_plan/static_map"

#define TOPIC_NAME_SHOW_LOC_PLAN_CE_OBST_MAP                    "show/loc_plan/close_edge/obst_map"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_CAR_SHAPE                   "show/loc_plan/close_edge/car_shape"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_ORIGIN_ROUTE                "show/loc_plan/close_edge/origin_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_DEFAULT_ROUTE               "show/loc_plan/close_edge/default_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_PLAN_ROUTE                  "show/loc_plan/close_edge/plan_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_OUT_POINT                   "show/loc_plan/close_edge/out_point"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_READY_OUT_AREA              "show/loc_plan/close_edge/ready_out_area"
#define TOPIC_NAME_SHOW_LOC_PLAN_CE_SIDE_AREA                   "show/loc_plan/close_edge/side_area"

#define TOPIC_NAME_SHOW_LOC_PLAN_ST_OBST_MAP                    "show/loc_plan/sw_trj/obst_map"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_CAR_SHAPE                   "show/loc_plan/sw_trj/car_shape"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_START_POSE                  "show/loc_plan/sw_trj/start_pose"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_PASS_POINTS                 "show/loc_plan/sw_trj/pass_points"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_ORIGIN_ROUTE                "show/loc_plan/sw_trj/origin_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_REF_ROUTE                   "show/loc_plan/sw_trj/ref_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_DEFAULT_ROUTE               "show/loc_plan/sw_trj/default_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_ORIGIN_PLAN_ROUTE           "show/loc_plan/sw_trj/origin_plan_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_PLAN_ROUTE                  "show/loc_plan/sw_trj/plan_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_PASS_POINT_TRANS_BEGIN_POSE "show/loc_plan/sw_trj/pass_point_trans_begin_pose"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_PASS_POINT_POSE             "show/loc_plan/sw_trj/pass_point_pose"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_PLANNING_ROUTE              "show/loc_plan/sw_trj/planning_route"
#define TOPIC_NAME_SHOW_LOC_PLAN_ST_PLANNING_ROUTE_PROCED_POINT "show/loc_plan/sw_trj/planning_route_proced_point"

#define TOPIC_NAME_SHOW_LOC_PLAN_AL_OBST_MAP                    "show/loc_plan/alignment/obst_map"
#define TOPIC_NAME_SHOW_LOC_PLAN_AL_CAR_SHAPE                   "show/loc_plan/alignment/car_shape"
#define TOPIC_NAME_SHOW_LOC_PLAN_AL_START_POSE                  "show/loc_plan/alignment/start_pose"
#define TOPIC_NAME_SHOW_LOC_PLAN_AL_ORIGIN_ROUTE                "show/loc_plan/alignment/origin_route"

#define TOPIC_NAME_SHOW_CTRL_PLAN_OBST_MAP                      "show/ctrl_plan/obst_map"
#define TOPIC_NAME_SHOW_CTRL_PLAN_REF_ROUTE                     "show/ctrl_plan/ref_route"
#define TOPIC_NAME_SHOW_CTRL_PLAN_CAR_SHAPE                     "show/ctrl_plan/car_shape"
#define TOPIC_NAME_SHOW_CTRL_PLAN_START_POSE                    "show/ctrl_plan/start_pose"
#define TOPIC_NAME_SHOW_CTRL_PLAN_CTRL_INFO                     "show/ctrl_plan/ctrl_info"
#define TOPIC_NAME_SHOW_CTRL_PLAN_CLOSE_LOOP_INFO               "show/ctrl_plan/close_loop_info"
#define TOPIC_NAME_SHOW_VEL_PLAN_MOVE_ROUTE                     "show/vel_plan/move_route"
#define TOPIC_NAME_SHOW_VEL_PLAN_OBST_DIST                      "show/vel_plan/obst_dist_list"
#define TOPIC_NAME_SHOW_VEL_PLAN_VEL_LIST                       "show/vel_plan/vel_list"
#define TOPIC_NAME_SHOW_VEL_PLAN_ROUTE_CAR_SHAPES               "show/vel_plan/route_car_shapes"
#define TOPIC_NAME_SHOW_VEL_PLAN_MOVING_OBJECTS_INFO            "show/vel_plan/moving_objects_info"

#define TOPIC_NAME_SHOW_DCU_CTRL_CLOSE_LOOP_INFO                "show/dcu_ctrl/close_loop_info"

#define TOPIC_NAME_CAR_BODY_FILTER                              "/car_body_filter"
#define TOPIC_NAME_LABEL_CMD                                    "/label_cmd"

#define TOPIC_NAME_MAP_POINTS                                   "/map_points"
#define TOPIC_NAME_GLOBAL_MAP                                   "/global_map"
#define TOPIC_NAME_LOCAL_MAP                                    "/local_map"
#define TOPIC_NAME_CURR_SCAN                                    "/curr_scan"

#define TOPIC_NAME_LOC_POINTS                                   "/loc_points"
#define TOPIC_NAME_ALIGNED_POINTS                               "/aligned_points"
#define TOPIC_ODOM_3D                                           "/odom_3d"

#define TOPIC_NAME_ST_GRAPH                                     "/st_graph"

#define SERVICE_NAME_INIT_POSE                                  "/initial_pose_srv"
#define SERVICE_NAME_VELOCITY_LEVEL                             "/velocity_level_srv"

#define TOPIC_NAME_EXTRI_PARAM                                  "/robor_extrinsic_parameters"

#endif

