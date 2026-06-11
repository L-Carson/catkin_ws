// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let traffic_light_info = require('./traffic_light_info.js');

//-----------------------------------------------------------

class traffic_light_target {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.is_open = null;
      this.pixel_bias = null;
      this.rect_bias = null;
      this.target = null;
      this.nears = null;
    }
    else {
      if (initObj.hasOwnProperty('is_open')) {
        this.is_open = initObj.is_open
      }
      else {
        this.is_open = 0;
      }
      if (initObj.hasOwnProperty('pixel_bias')) {
        this.pixel_bias = initObj.pixel_bias
      }
      else {
        this.pixel_bias = 0.0;
      }
      if (initObj.hasOwnProperty('rect_bias')) {
        this.rect_bias = initObj.rect_bias
      }
      else {
        this.rect_bias = 0.0;
      }
      if (initObj.hasOwnProperty('target')) {
        this.target = initObj.target
      }
      else {
        this.target = new traffic_light_info();
      }
      if (initObj.hasOwnProperty('nears')) {
        this.nears = initObj.nears
      }
      else {
        this.nears = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type traffic_light_target
    // Serialize message field [is_open]
    bufferOffset = _serializer.uint8(obj.is_open, buffer, bufferOffset);
    // Serialize message field [pixel_bias]
    bufferOffset = _serializer.float32(obj.pixel_bias, buffer, bufferOffset);
    // Serialize message field [rect_bias]
    bufferOffset = _serializer.float32(obj.rect_bias, buffer, bufferOffset);
    // Serialize message field [target]
    bufferOffset = traffic_light_info.serialize(obj.target, buffer, bufferOffset);
    // Serialize message field [nears]
    // Serialize the length for message field [nears]
    bufferOffset = _serializer.uint32(obj.nears.length, buffer, bufferOffset);
    obj.nears.forEach((val) => {
      bufferOffset = traffic_light_info.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type traffic_light_target
    let len;
    let data = new traffic_light_target(null);
    // Deserialize message field [is_open]
    data.is_open = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [pixel_bias]
    data.pixel_bias = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [rect_bias]
    data.rect_bias = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [target]
    data.target = traffic_light_info.deserialize(buffer, bufferOffset);
    // Deserialize message field [nears]
    // Deserialize array length for message field [nears]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.nears = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.nears[i] = traffic_light_info.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 21 * object.nears.length;
    return length + 34;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/traffic_light_target';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '5c8d3210265e381480cecfd925687150';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8                   is_open     # 是否开启交通灯检测：0(关闭) 1(开启)
    float32                 pixel_bias  # 像素偏差
    float32                 rect_bias   # 检测框匹配率偏差
    traffic_light_info      target      # 目标交通灯信息
    traffic_light_info[]    nears       # 周边交通灯信息
    ================================================================================
    MSG: comm_msg/traffic_light_info
    uint8    type           # 交通灯显示类型：1(读秒类型) 2(频闪类型) 3(高频亮灭型)
    float32  pos_x          # 交通灯中在地图上的坐标x
    float32  pos_y          # 交通灯中在地图上的坐标y
    float32  pos_z          # 交通灯中在地图上的坐标z
    float32  row_length     # 交通灯横向长度
    float32  col_length     # 交通灯纵向长度
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new traffic_light_target(null);
    if (msg.is_open !== undefined) {
      resolved.is_open = msg.is_open;
    }
    else {
      resolved.is_open = 0
    }

    if (msg.pixel_bias !== undefined) {
      resolved.pixel_bias = msg.pixel_bias;
    }
    else {
      resolved.pixel_bias = 0.0
    }

    if (msg.rect_bias !== undefined) {
      resolved.rect_bias = msg.rect_bias;
    }
    else {
      resolved.rect_bias = 0.0
    }

    if (msg.target !== undefined) {
      resolved.target = traffic_light_info.Resolve(msg.target)
    }
    else {
      resolved.target = new traffic_light_info()
    }

    if (msg.nears !== undefined) {
      resolved.nears = new Array(msg.nears.length);
      for (let i = 0; i < resolved.nears.length; ++i) {
        resolved.nears[i] = traffic_light_info.Resolve(msg.nears[i]);
      }
    }
    else {
      resolved.nears = []
    }

    return resolved;
    }
};

module.exports = traffic_light_target;
