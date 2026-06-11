// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let pose3D = require('./pose3D.js');

//-----------------------------------------------------------

class pose_score {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.score = null;
      this.pose = null;
    }
    else {
      if (initObj.hasOwnProperty('score')) {
        this.score = initObj.score
      }
      else {
        this.score = 0.0;
      }
      if (initObj.hasOwnProperty('pose')) {
        this.pose = initObj.pose
      }
      else {
        this.pose = new pose3D();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type pose_score
    // Serialize message field [score]
    bufferOffset = _serializer.float32(obj.score, buffer, bufferOffset);
    // Serialize message field [pose]
    bufferOffset = pose3D.serialize(obj.pose, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type pose_score
    let len;
    let data = new pose_score(null);
    // Deserialize message field [score]
    data.score = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [pose]
    data.pose = pose3D.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 28;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/pose_score';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '1ba278e321050d4e0b42ab7a7a69fe0c';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 score
    pose3D  pose
    ================================================================================
    MSG: comm_msg/pose3D
    float32 posX
    float32 posY
    float32 posZ
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
    const resolved = new pose_score(null);
    if (msg.score !== undefined) {
      resolved.score = msg.score;
    }
    else {
      resolved.score = 0.0
    }

    if (msg.pose !== undefined) {
      resolved.pose = pose3D.Resolve(msg.pose)
    }
    else {
      resolved.pose = new pose3D()
    }

    return resolved;
    }
};

module.exports = pose_score;
