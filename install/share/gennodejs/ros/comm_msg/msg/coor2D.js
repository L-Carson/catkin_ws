// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let coorLabel = require('./coorLabel.js');

//-----------------------------------------------------------

class coor2D {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.label = null;
      this.pos_x = null;
      this.pos_y = null;
    }
    else {
      if (initObj.hasOwnProperty('label')) {
        this.label = initObj.label
      }
      else {
        this.label = new coorLabel();
      }
      if (initObj.hasOwnProperty('pos_x')) {
        this.pos_x = initObj.pos_x
      }
      else {
        this.pos_x = 0.0;
      }
      if (initObj.hasOwnProperty('pos_y')) {
        this.pos_y = initObj.pos_y
      }
      else {
        this.pos_y = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type coor2D
    // Serialize message field [label]
    bufferOffset = coorLabel.serialize(obj.label, buffer, bufferOffset);
    // Serialize message field [pos_x]
    bufferOffset = _serializer.float32(obj.pos_x, buffer, bufferOffset);
    // Serialize message field [pos_y]
    bufferOffset = _serializer.float32(obj.pos_y, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type coor2D
    let len;
    let data = new coor2D(null);
    // Deserialize message field [label]
    data.label = coorLabel.deserialize(buffer, bufferOffset);
    // Deserialize message field [pos_x]
    data.pos_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pos_y]
    data.pos_y = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 9;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/coor2D';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'a605ddea652ab63420ad444791949b49';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    const resolved = new coor2D(null);
    if (msg.label !== undefined) {
      resolved.label = coorLabel.Resolve(msg.label)
    }
    else {
      resolved.label = new coorLabel()
    }

    if (msg.pos_x !== undefined) {
      resolved.pos_x = msg.pos_x;
    }
    else {
      resolved.pos_x = 0.0
    }

    if (msg.pos_y !== undefined) {
      resolved.pos_y = msg.pos_y;
    }
    else {
      resolved.pos_y = 0.0
    }

    return resolved;
    }
};

module.exports = coor2D;
