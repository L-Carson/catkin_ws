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

class body_pose {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.body_yaw = null;
      this.backWheel_x = null;
      this.backWheel_y = null;
      this.frontWheel_x = null;
      this.frontWheel_y = null;
      this.alongEdgeRefPoint_x = null;
      this.alongEdgeRefPoint_y = null;
    }
    else {
      if (initObj.hasOwnProperty('body_yaw')) {
        this.body_yaw = initObj.body_yaw
      }
      else {
        this.body_yaw = 0.0;
      }
      if (initObj.hasOwnProperty('backWheel_x')) {
        this.backWheel_x = initObj.backWheel_x
      }
      else {
        this.backWheel_x = 0.0;
      }
      if (initObj.hasOwnProperty('backWheel_y')) {
        this.backWheel_y = initObj.backWheel_y
      }
      else {
        this.backWheel_y = 0.0;
      }
      if (initObj.hasOwnProperty('frontWheel_x')) {
        this.frontWheel_x = initObj.frontWheel_x
      }
      else {
        this.frontWheel_x = 0.0;
      }
      if (initObj.hasOwnProperty('frontWheel_y')) {
        this.frontWheel_y = initObj.frontWheel_y
      }
      else {
        this.frontWheel_y = 0.0;
      }
      if (initObj.hasOwnProperty('alongEdgeRefPoint_x')) {
        this.alongEdgeRefPoint_x = initObj.alongEdgeRefPoint_x
      }
      else {
        this.alongEdgeRefPoint_x = 0.0;
      }
      if (initObj.hasOwnProperty('alongEdgeRefPoint_y')) {
        this.alongEdgeRefPoint_y = initObj.alongEdgeRefPoint_y
      }
      else {
        this.alongEdgeRefPoint_y = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type body_pose
    // Serialize message field [body_yaw]
    bufferOffset = _serializer.float32(obj.body_yaw, buffer, bufferOffset);
    // Serialize message field [backWheel_x]
    bufferOffset = _serializer.float32(obj.backWheel_x, buffer, bufferOffset);
    // Serialize message field [backWheel_y]
    bufferOffset = _serializer.float32(obj.backWheel_y, buffer, bufferOffset);
    // Serialize message field [frontWheel_x]
    bufferOffset = _serializer.float32(obj.frontWheel_x, buffer, bufferOffset);
    // Serialize message field [frontWheel_y]
    bufferOffset = _serializer.float32(obj.frontWheel_y, buffer, bufferOffset);
    // Serialize message field [alongEdgeRefPoint_x]
    bufferOffset = _serializer.float32(obj.alongEdgeRefPoint_x, buffer, bufferOffset);
    // Serialize message field [alongEdgeRefPoint_y]
    bufferOffset = _serializer.float32(obj.alongEdgeRefPoint_y, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type body_pose
    let len;
    let data = new body_pose(null);
    // Deserialize message field [body_yaw]
    data.body_yaw = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [backWheel_x]
    data.backWheel_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [backWheel_y]
    data.backWheel_y = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [frontWheel_x]
    data.frontWheel_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [frontWheel_y]
    data.frontWheel_y = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [alongEdgeRefPoint_x]
    data.alongEdgeRefPoint_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [alongEdgeRefPoint_y]
    data.alongEdgeRefPoint_y = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 28;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/body_pose';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f09536d45d4aed5f09a8a01687e32257';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 body_yaw
    float32 backWheel_x
    float32 backWheel_y
    float32 frontWheel_x
    float32 frontWheel_y
    float32 alongEdgeRefPoint_x
    float32 alongEdgeRefPoint_y
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new body_pose(null);
    if (msg.body_yaw !== undefined) {
      resolved.body_yaw = msg.body_yaw;
    }
    else {
      resolved.body_yaw = 0.0
    }

    if (msg.backWheel_x !== undefined) {
      resolved.backWheel_x = msg.backWheel_x;
    }
    else {
      resolved.backWheel_x = 0.0
    }

    if (msg.backWheel_y !== undefined) {
      resolved.backWheel_y = msg.backWheel_y;
    }
    else {
      resolved.backWheel_y = 0.0
    }

    if (msg.frontWheel_x !== undefined) {
      resolved.frontWheel_x = msg.frontWheel_x;
    }
    else {
      resolved.frontWheel_x = 0.0
    }

    if (msg.frontWheel_y !== undefined) {
      resolved.frontWheel_y = msg.frontWheel_y;
    }
    else {
      resolved.frontWheel_y = 0.0
    }

    if (msg.alongEdgeRefPoint_x !== undefined) {
      resolved.alongEdgeRefPoint_x = msg.alongEdgeRefPoint_x;
    }
    else {
      resolved.alongEdgeRefPoint_x = 0.0
    }

    if (msg.alongEdgeRefPoint_y !== undefined) {
      resolved.alongEdgeRefPoint_y = msg.alongEdgeRefPoint_y;
    }
    else {
      resolved.alongEdgeRefPoint_y = 0.0
    }

    return resolved;
    }
};

module.exports = body_pose;
