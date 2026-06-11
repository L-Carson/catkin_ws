// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class daq_ctrl {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.daq_fusion = null;
      this.daq_hz = null;
      this.daq_camera = null;
      this.daq_lidar = null;
    }
    else {
      if (initObj.hasOwnProperty('daq_fusion')) {
        this.daq_fusion = initObj.daq_fusion
      }
      else {
        this.daq_fusion = false;
      }
      if (initObj.hasOwnProperty('daq_hz')) {
        this.daq_hz = initObj.daq_hz
      }
      else {
        this.daq_hz = 0;
      }
      if (initObj.hasOwnProperty('daq_camera')) {
        this.daq_camera = initObj.daq_camera
      }
      else {
        this.daq_camera = [];
      }
      if (initObj.hasOwnProperty('daq_lidar')) {
        this.daq_lidar = initObj.daq_lidar
      }
      else {
        this.daq_lidar = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type daq_ctrl
    // Serialize message field [daq_fusion]
    bufferOffset = _serializer.bool(obj.daq_fusion, buffer, bufferOffset);
    // Serialize message field [daq_hz]
    bufferOffset = _serializer.uint8(obj.daq_hz, buffer, bufferOffset);
    // Serialize message field [daq_camera]
    bufferOffset = _arraySerializer.uint8(obj.daq_camera, buffer, bufferOffset, null);
    // Serialize message field [daq_lidar]
    bufferOffset = _arraySerializer.uint8(obj.daq_lidar, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type daq_ctrl
    let len;
    let data = new daq_ctrl(null);
    // Deserialize message field [daq_fusion]
    data.daq_fusion = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [daq_hz]
    data.daq_hz = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [daq_camera]
    data.daq_camera = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    // Deserialize message field [daq_lidar]
    data.daq_lidar = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.daq_camera.length;
    length += object.daq_lidar.length;
    return length + 10;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/daq_ctrl';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b65aad8c511a16f789728648f67ea4d6';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}"     # 相机+雷达：时间戳需对齐
    # e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[1,2,3,4,5]}"     # 相机+雷达：时间戳无需对齐
    
    # e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}"              # 相机：时间戳需对齐
    # e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[1,2,3,4,5,6,7,8,9,10], daq_lidar:[]}"              # 相机：时间戳无需对齐
    
    # e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}"                         # 雷达：时间戳需对齐
    # e.g. begin: rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[1,2,3,4,5]}"                         # 雷达：时间戳无需对齐
    
    # e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 1, daq_hz: 1, daq_camera:[], daq_lidar:[]}"                                  # 结束采集
    # e.g. end:   rostopic pub -r 1 /daq_ctrl comm_msg/daq_ctrl "{daq_fusion: 0, daq_hz: 1, daq_camera:[], daq_lidar:[]}"                                  # 结束采集（同上）
    
    bool    daq_fusion      # 采集融合数据（相机+雷达：时间戳对齐）
    uint8   daq_hz          # 采集频率
    uint8[] daq_camera      # 采集相机数据
    uint8[] daq_lidar       # 采集雷达数据
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new daq_ctrl(null);
    if (msg.daq_fusion !== undefined) {
      resolved.daq_fusion = msg.daq_fusion;
    }
    else {
      resolved.daq_fusion = false
    }

    if (msg.daq_hz !== undefined) {
      resolved.daq_hz = msg.daq_hz;
    }
    else {
      resolved.daq_hz = 0
    }

    if (msg.daq_camera !== undefined) {
      resolved.daq_camera = msg.daq_camera;
    }
    else {
      resolved.daq_camera = []
    }

    if (msg.daq_lidar !== undefined) {
      resolved.daq_lidar = msg.daq_lidar;
    }
    else {
      resolved.daq_lidar = []
    }

    return resolved;
    }
};

module.exports = daq_ctrl;
