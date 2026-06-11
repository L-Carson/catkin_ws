
"use strict";

let serialPassData = require('./serialPassData.js');
let SaveMap = require('./SaveMap.js');
let visionRect = require('./visionRect.js');
let dustbinState = require('./dustbinState.js');
let traffic_light_target = require('./traffic_light_target.js');
let locationInfo = require('./locationInfo.js');
let visionArea = require('./visionArea.js');
let coor3DArray = require('./coor3DArray.js');
let customRoute = require('./customRoute.js');
let qrcode_pose = require('./qrcode_pose.js');
let security_redundancy = require('./security_redundancy.js');
let visionPixel = require('./visionPixel.js');
let detect_target = require('./detect_target.js');
let pose = require('./pose.js');
let move_close_loop_ctrl = require('./move_close_loop_ctrl.js');
let hmiStatus = require('./hmiStatus.js');
let pose2D = require('./pose2D.js');
let body_pose = require('./body_pose.js');
let equipment_info = require('./equipment_info.js');
let ultrasonicData = require('./ultrasonicData.js');
let spoiler_detect = require('./spoiler_detect.js');
let detect_result = require('./detect_result.js');
let traffic_light_info = require('./traffic_light_info.js');
let roadmark = require('./roadmark.js');
let nav_algo_info = require('./nav_algo_info.js');
let ipu = require('./ipu.js');
let PoseInfo = require('./PoseInfo.js');
let roborExtriParam = require('./roborExtriParam.js');
let visionFuncs = require('./visionFuncs.js');
let visionAreas = require('./visionAreas.js');
let carBodyFilter = require('./carBodyFilter.js');
let coorLabel = require('./coorLabel.js');
let coor2DArray = require('./coor2DArray.js');
let nav_ctrl = require('./nav_ctrl.js');
let wheelSpeed = require('./wheelSpeed.js');
let visionRects = require('./visionRects.js');
let daq_ctrl = require('./daq_ctrl.js');
let boxArray = require('./boxArray.js');
let trash_box_area_obst_state = require('./trash_box_area_obst_state.js');
let sensorExtriParam = require('./sensorExtriParam.js');
let visionFunc = require('./visionFunc.js');
let radarTarget = require('./radarTarget.js');
let coor3D = require('./coor3D.js');
let box = require('./box.js');
let coor2DArrayLA = require('./coor2DArrayLA.js');
let pose3D = require('./pose3D.js');
let stop_ctrl = require('./stop_ctrl.js');
let visionDepthCamera = require('./visionDepthCamera.js');
let dev_info = require('./dev_info.js');
let trash_box_obst_state = require('./trash_box_obst_state.js');
let Mode = require('./Mode.js');
let radarTargetArray = require('./radarTargetArray.js');
let movTarget = require('./movTarget.js');
let label = require('./label.js');
let movPred = require('./movPred.js');
let coor2DArrayL = require('./coor2DArrayL.js');
let ultrasonicsData = require('./ultrasonicsData.js');
let move_ctrl = require('./move_ctrl.js');
let locationInfo3D = require('./locationInfo3D.js');
let simulation_obsts = require('./simulation_obsts.js');
let imudata = require('./imudata.js');
let dcu_info = require('./dcu_info.js');
let pose_score = require('./pose_score.js');
let coor2D = require('./coor2D.js');
let equipment_pose = require('./equipment_pose.js');
let label_state = require('./label_state.js');
let move_info = require('./move_info.js');
let gpsInfo = require('./gpsInfo.js');
let module_abnormal_info = require('./module_abnormal_info.js');
let mapping_ctrl = require('./mapping_ctrl.js');
let nav_state = require('./nav_state.js');
let movTargetsArray = require('./movTargetsArray.js');
let traffic_light_result = require('./traffic_light_result.js');
let zhishancmd = require('./zhishancmd.js');
let point_id = require('./point_id.js');
let simulation_circular_obst = require('./simulation_circular_obst.js');
let marker_visible = require('./marker_visible.js');

module.exports = {
  serialPassData: serialPassData,
  SaveMap: SaveMap,
  visionRect: visionRect,
  dustbinState: dustbinState,
  traffic_light_target: traffic_light_target,
  locationInfo: locationInfo,
  visionArea: visionArea,
  coor3DArray: coor3DArray,
  customRoute: customRoute,
  qrcode_pose: qrcode_pose,
  security_redundancy: security_redundancy,
  visionPixel: visionPixel,
  detect_target: detect_target,
  pose: pose,
  move_close_loop_ctrl: move_close_loop_ctrl,
  hmiStatus: hmiStatus,
  pose2D: pose2D,
  body_pose: body_pose,
  equipment_info: equipment_info,
  ultrasonicData: ultrasonicData,
  spoiler_detect: spoiler_detect,
  detect_result: detect_result,
  traffic_light_info: traffic_light_info,
  roadmark: roadmark,
  nav_algo_info: nav_algo_info,
  ipu: ipu,
  PoseInfo: PoseInfo,
  roborExtriParam: roborExtriParam,
  visionFuncs: visionFuncs,
  visionAreas: visionAreas,
  carBodyFilter: carBodyFilter,
  coorLabel: coorLabel,
  coor2DArray: coor2DArray,
  nav_ctrl: nav_ctrl,
  wheelSpeed: wheelSpeed,
  visionRects: visionRects,
  daq_ctrl: daq_ctrl,
  boxArray: boxArray,
  trash_box_area_obst_state: trash_box_area_obst_state,
  sensorExtriParam: sensorExtriParam,
  visionFunc: visionFunc,
  radarTarget: radarTarget,
  coor3D: coor3D,
  box: box,
  coor2DArrayLA: coor2DArrayLA,
  pose3D: pose3D,
  stop_ctrl: stop_ctrl,
  visionDepthCamera: visionDepthCamera,
  dev_info: dev_info,
  trash_box_obst_state: trash_box_obst_state,
  Mode: Mode,
  radarTargetArray: radarTargetArray,
  movTarget: movTarget,
  label: label,
  movPred: movPred,
  coor2DArrayL: coor2DArrayL,
  ultrasonicsData: ultrasonicsData,
  move_ctrl: move_ctrl,
  locationInfo3D: locationInfo3D,
  simulation_obsts: simulation_obsts,
  imudata: imudata,
  dcu_info: dcu_info,
  pose_score: pose_score,
  coor2D: coor2D,
  equipment_pose: equipment_pose,
  label_state: label_state,
  move_info: move_info,
  gpsInfo: gpsInfo,
  module_abnormal_info: module_abnormal_info,
  mapping_ctrl: mapping_ctrl,
  nav_state: nav_state,
  movTargetsArray: movTargetsArray,
  traffic_light_result: traffic_light_result,
  zhishancmd: zhishancmd,
  point_id: point_id,
  simulation_circular_obst: simulation_circular_obst,
  marker_visible: marker_visible,
};
