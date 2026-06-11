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

class move_close_loop_ctrl {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.plannedSteeringAngle = null;
      this.plannedMoveLinearVel = null;
      this.controlSteeringAngle = null;
      this.controlMoveLinearVel = null;
      this.measureSteeringAngle = null;
      this.measureMoveLinearVel = null;
    }
    else {
      if (initObj.hasOwnProperty('plannedSteeringAngle')) {
        this.plannedSteeringAngle = initObj.plannedSteeringAngle
      }
      else {
        this.plannedSteeringAngle = 0.0;
      }
      if (initObj.hasOwnProperty('plannedMoveLinearVel')) {
        this.plannedMoveLinearVel = initObj.plannedMoveLinearVel
      }
      else {
        this.plannedMoveLinearVel = 0.0;
      }
      if (initObj.hasOwnProperty('controlSteeringAngle')) {
        this.controlSteeringAngle = initObj.controlSteeringAngle
      }
      else {
        this.controlSteeringAngle = 0.0;
      }
      if (initObj.hasOwnProperty('controlMoveLinearVel')) {
        this.controlMoveLinearVel = initObj.controlMoveLinearVel
      }
      else {
        this.controlMoveLinearVel = 0.0;
      }
      if (initObj.hasOwnProperty('measureSteeringAngle')) {
        this.measureSteeringAngle = initObj.measureSteeringAngle
      }
      else {
        this.measureSteeringAngle = 0.0;
      }
      if (initObj.hasOwnProperty('measureMoveLinearVel')) {
        this.measureMoveLinearVel = initObj.measureMoveLinearVel
      }
      else {
        this.measureMoveLinearVel = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type move_close_loop_ctrl
    // Serialize message field [plannedSteeringAngle]
    bufferOffset = _serializer.float32(obj.plannedSteeringAngle, buffer, bufferOffset);
    // Serialize message field [plannedMoveLinearVel]
    bufferOffset = _serializer.float32(obj.plannedMoveLinearVel, buffer, bufferOffset);
    // Serialize message field [controlSteeringAngle]
    bufferOffset = _serializer.float32(obj.controlSteeringAngle, buffer, bufferOffset);
    // Serialize message field [controlMoveLinearVel]
    bufferOffset = _serializer.float32(obj.controlMoveLinearVel, buffer, bufferOffset);
    // Serialize message field [measureSteeringAngle]
    bufferOffset = _serializer.float32(obj.measureSteeringAngle, buffer, bufferOffset);
    // Serialize message field [measureMoveLinearVel]
    bufferOffset = _serializer.float32(obj.measureMoveLinearVel, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type move_close_loop_ctrl
    let len;
    let data = new move_close_loop_ctrl(null);
    // Deserialize message field [plannedSteeringAngle]
    data.plannedSteeringAngle = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [plannedMoveLinearVel]
    data.plannedMoveLinearVel = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [controlSteeringAngle]
    data.controlSteeringAngle = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [controlMoveLinearVel]
    data.controlMoveLinearVel = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [measureSteeringAngle]
    data.measureSteeringAngle = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [measureMoveLinearVel]
    data.measureMoveLinearVel = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 24;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/move_close_loop_ctrl';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e51a1651c7c8b7b7a901faef61d12ad6';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 plannedSteeringAngle
    float32 plannedMoveLinearVel
    float32 controlSteeringAngle
    float32 controlMoveLinearVel
    float32 measureSteeringAngle
    float32 measureMoveLinearVel
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new move_close_loop_ctrl(null);
    if (msg.plannedSteeringAngle !== undefined) {
      resolved.plannedSteeringAngle = msg.plannedSteeringAngle;
    }
    else {
      resolved.plannedSteeringAngle = 0.0
    }

    if (msg.plannedMoveLinearVel !== undefined) {
      resolved.plannedMoveLinearVel = msg.plannedMoveLinearVel;
    }
    else {
      resolved.plannedMoveLinearVel = 0.0
    }

    if (msg.controlSteeringAngle !== undefined) {
      resolved.controlSteeringAngle = msg.controlSteeringAngle;
    }
    else {
      resolved.controlSteeringAngle = 0.0
    }

    if (msg.controlMoveLinearVel !== undefined) {
      resolved.controlMoveLinearVel = msg.controlMoveLinearVel;
    }
    else {
      resolved.controlMoveLinearVel = 0.0
    }

    if (msg.measureSteeringAngle !== undefined) {
      resolved.measureSteeringAngle = msg.measureSteeringAngle;
    }
    else {
      resolved.measureSteeringAngle = 0.0
    }

    if (msg.measureMoveLinearVel !== undefined) {
      resolved.measureMoveLinearVel = msg.measureMoveLinearVel;
    }
    else {
      resolved.measureMoveLinearVel = 0.0
    }

    return resolved;
    }
};

module.exports = move_close_loop_ctrl;
