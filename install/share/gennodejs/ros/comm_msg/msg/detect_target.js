// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let coor2D = require('./coor2D.js');

//-----------------------------------------------------------

class detect_target {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.type = null;
      this.vertexes = null;
    }
    else {
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
      }
      if (initObj.hasOwnProperty('vertexes')) {
        this.vertexes = initObj.vertexes
      }
      else {
        this.vertexes = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type detect_target
    // Serialize message field [type]
    bufferOffset = _serializer.uint8(obj.type, buffer, bufferOffset);
    // Serialize message field [vertexes]
    // Serialize the length for message field [vertexes]
    bufferOffset = _serializer.uint32(obj.vertexes.length, buffer, bufferOffset);
    obj.vertexes.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type detect_target
    let len;
    let data = new detect_target(null);
    // Deserialize message field [type]
    data.type = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [vertexes]
    // Deserialize array length for message field [vertexes]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.vertexes = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.vertexes[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 9 * object.vertexes.length;
    return length + 5;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/detect_target';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4e9958080bbdd6468da1c554f1002943';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # 1开门 2关门
    uint8    type
    coor2D[] vertexes
    ================================================================================
    MSG: comm_msg/coor2D
    coorLabel   label       #坐标类别
    float32     pos_x
    float32     pos_y
    ================================================================================
    MSG: comm_msg/coorLabel
    uint8   UNKNOWN      = 0  #未知
    uint8   STATIC       = 1  #静态
    uint8   DYNAMIC      = 2  #动态
    uint8   HOLLOW_OBST  = 3  #空洞障碍
    uint8   SHORT_HOLD   = 4  #低矮维持
    uint8   COMMON_HOLD  = 5  #常规维持
    uint8   BLIND_SPOT   = 6  #盲区障碍
    uint8   SLOPE_PAD    = 7  #斜坡垫
    uint8   CONTACT_EDGE = 8  #触边障碍
    uint8   TREELAWN     = 9  #绿植障碍
    uint8   ULTRASONIC   = 10 #超声
    
    uint8 value
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new detect_target(null);
    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
    }

    if (msg.vertexes !== undefined) {
      resolved.vertexes = new Array(msg.vertexes.length);
      for (let i = 0; i < resolved.vertexes.length; ++i) {
        resolved.vertexes[i] = coor2D.Resolve(msg.vertexes[i]);
      }
    }
    else {
      resolved.vertexes = []
    }

    return resolved;
    }
};

module.exports = detect_target;
