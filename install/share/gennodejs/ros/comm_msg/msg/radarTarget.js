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

class radarTarget {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.targetDistx = null;
      this.targetDisty = null;
      this.targetVelx = null;
      this.targetVely = null;
    }
    else {
      if (initObj.hasOwnProperty('targetDistx')) {
        this.targetDistx = initObj.targetDistx
      }
      else {
        this.targetDistx = 0.0;
      }
      if (initObj.hasOwnProperty('targetDisty')) {
        this.targetDisty = initObj.targetDisty
      }
      else {
        this.targetDisty = 0.0;
      }
      if (initObj.hasOwnProperty('targetVelx')) {
        this.targetVelx = initObj.targetVelx
      }
      else {
        this.targetVelx = 0.0;
      }
      if (initObj.hasOwnProperty('targetVely')) {
        this.targetVely = initObj.targetVely
      }
      else {
        this.targetVely = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type radarTarget
    // Serialize message field [targetDistx]
    bufferOffset = _serializer.float32(obj.targetDistx, buffer, bufferOffset);
    // Serialize message field [targetDisty]
    bufferOffset = _serializer.float32(obj.targetDisty, buffer, bufferOffset);
    // Serialize message field [targetVelx]
    bufferOffset = _serializer.float32(obj.targetVelx, buffer, bufferOffset);
    // Serialize message field [targetVely]
    bufferOffset = _serializer.float32(obj.targetVely, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type radarTarget
    let len;
    let data = new radarTarget(null);
    // Deserialize message field [targetDistx]
    data.targetDistx = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [targetDisty]
    data.targetDisty = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [targetVelx]
    data.targetVelx = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [targetVely]
    data.targetVely = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 16;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/radarTarget';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2f0ce27dc024af9302b335afb3187795';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标
    float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标
    float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度
    float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new radarTarget(null);
    if (msg.targetDistx !== undefined) {
      resolved.targetDistx = msg.targetDistx;
    }
    else {
      resolved.targetDistx = 0.0
    }

    if (msg.targetDisty !== undefined) {
      resolved.targetDisty = msg.targetDisty;
    }
    else {
      resolved.targetDisty = 0.0
    }

    if (msg.targetVelx !== undefined) {
      resolved.targetVelx = msg.targetVelx;
    }
    else {
      resolved.targetVelx = 0.0
    }

    if (msg.targetVely !== undefined) {
      resolved.targetVely = msg.targetVely;
    }
    else {
      resolved.targetVely = 0.0
    }

    return resolved;
    }
};

module.exports = radarTarget;
