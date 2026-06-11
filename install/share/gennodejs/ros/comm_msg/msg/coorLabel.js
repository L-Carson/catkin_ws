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

class coorLabel {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.value = null;
    }
    else {
      if (initObj.hasOwnProperty('value')) {
        this.value = initObj.value
      }
      else {
        this.value = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type coorLabel
    // Serialize message field [value]
    bufferOffset = _serializer.uint8(obj.value, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type coorLabel
    let len;
    let data = new coorLabel(null);
    // Deserialize message field [value]
    data.value = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/coorLabel';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'c0283db75cf753d6e1cb791ae85ce57c';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    const resolved = new coorLabel(null);
    if (msg.value !== undefined) {
      resolved.value = msg.value;
    }
    else {
      resolved.value = 0
    }

    return resolved;
    }
};

// Constants for message
coorLabel.Constants = {
  UNKNOWN: 0,
  STATIC: 1,
  DYNAMIC: 2,
  HOLLOW_OBST: 3,
  SHORT_HOLD: 4,
  COMMON_HOLD: 5,
  BLIND_SPOT: 6,
  SLOPE_PAD: 7,
  CONTACT_EDGE: 8,
  TREELAWN: 9,
  ULTRASONIC: 10,
}

module.exports = coorLabel;
