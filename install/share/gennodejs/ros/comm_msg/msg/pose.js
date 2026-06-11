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

class pose {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.px = null;
      this.py = null;
      this.pz = null;
      this.roll = null;
      this.pitch = null;
      this.yaw = null;
    }
    else {
      if (initObj.hasOwnProperty('px')) {
        this.px = initObj.px
      }
      else {
        this.px = 0.0;
      }
      if (initObj.hasOwnProperty('py')) {
        this.py = initObj.py
      }
      else {
        this.py = 0.0;
      }
      if (initObj.hasOwnProperty('pz')) {
        this.pz = initObj.pz
      }
      else {
        this.pz = 0.0;
      }
      if (initObj.hasOwnProperty('roll')) {
        this.roll = initObj.roll
      }
      else {
        this.roll = 0.0;
      }
      if (initObj.hasOwnProperty('pitch')) {
        this.pitch = initObj.pitch
      }
      else {
        this.pitch = 0.0;
      }
      if (initObj.hasOwnProperty('yaw')) {
        this.yaw = initObj.yaw
      }
      else {
        this.yaw = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type pose
    // Serialize message field [px]
    bufferOffset = _serializer.float32(obj.px, buffer, bufferOffset);
    // Serialize message field [py]
    bufferOffset = _serializer.float32(obj.py, buffer, bufferOffset);
    // Serialize message field [pz]
    bufferOffset = _serializer.float32(obj.pz, buffer, bufferOffset);
    // Serialize message field [roll]
    bufferOffset = _serializer.float32(obj.roll, buffer, bufferOffset);
    // Serialize message field [pitch]
    bufferOffset = _serializer.float32(obj.pitch, buffer, bufferOffset);
    // Serialize message field [yaw]
    bufferOffset = _serializer.float32(obj.yaw, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type pose
    let len;
    let data = new pose(null);
    // Deserialize message field [px]
    data.px = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [py]
    data.py = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pz]
    data.pz = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [roll]
    data.roll = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pitch]
    data.pitch = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [yaw]
    data.yaw = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 24;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/pose';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4974c257f289cfad706f1b0679063a6b';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 px
    float32 py
    float32 pz
    float32 roll
    float32 pitch
    float32 yaw
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new pose(null);
    if (msg.px !== undefined) {
      resolved.px = msg.px;
    }
    else {
      resolved.px = 0.0
    }

    if (msg.py !== undefined) {
      resolved.py = msg.py;
    }
    else {
      resolved.py = 0.0
    }

    if (msg.pz !== undefined) {
      resolved.pz = msg.pz;
    }
    else {
      resolved.pz = 0.0
    }

    if (msg.roll !== undefined) {
      resolved.roll = msg.roll;
    }
    else {
      resolved.roll = 0.0
    }

    if (msg.pitch !== undefined) {
      resolved.pitch = msg.pitch;
    }
    else {
      resolved.pitch = 0.0
    }

    if (msg.yaw !== undefined) {
      resolved.yaw = msg.yaw;
    }
    else {
      resolved.yaw = 0.0
    }

    return resolved;
    }
};

module.exports = pose;
