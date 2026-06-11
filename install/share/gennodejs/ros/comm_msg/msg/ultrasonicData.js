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

class ultrasonicData {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.usName = null;
      this.data = null;
    }
    else {
      if (initObj.hasOwnProperty('usName')) {
        this.usName = initObj.usName
      }
      else {
        this.usName = '';
      }
      if (initObj.hasOwnProperty('data')) {
        this.data = initObj.data
      }
      else {
        this.data = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type ultrasonicData
    // Serialize message field [usName]
    bufferOffset = _serializer.string(obj.usName, buffer, bufferOffset);
    // Serialize message field [data]
    // Serialize the length for message field [data]
    bufferOffset = _serializer.uint32(obj.data.length, buffer, bufferOffset);
    obj.data.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type ultrasonicData
    let len;
    let data = new ultrasonicData(null);
    // Deserialize message field [usName]
    data.usName = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [data]
    // Deserialize array length for message field [data]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.data = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.data[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.usName);
    length += 9 * object.data.length;
    return length + 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/ultrasonicData';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '16d3d45049c1ab8e0a2113ea61e36760';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string   usName
    coor2D[] data
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
    const resolved = new ultrasonicData(null);
    if (msg.usName !== undefined) {
      resolved.usName = msg.usName;
    }
    else {
      resolved.usName = ''
    }

    if (msg.data !== undefined) {
      resolved.data = new Array(msg.data.length);
      for (let i = 0; i < resolved.data.length; ++i) {
        resolved.data[i] = coor2D.Resolve(msg.data[i]);
      }
    }
    else {
      resolved.data = []
    }

    return resolved;
    }
};

module.exports = ultrasonicData;
