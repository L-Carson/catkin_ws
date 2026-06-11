// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let simulation_circular_obst = require('./simulation_circular_obst.js');

//-----------------------------------------------------------

class simulation_obsts {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.circularObsts = null;
    }
    else {
      if (initObj.hasOwnProperty('circularObsts')) {
        this.circularObsts = initObj.circularObsts
      }
      else {
        this.circularObsts = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type simulation_obsts
    // Serialize message field [circularObsts]
    // Serialize the length for message field [circularObsts]
    bufferOffset = _serializer.uint32(obj.circularObsts.length, buffer, bufferOffset);
    obj.circularObsts.forEach((val) => {
      bufferOffset = simulation_circular_obst.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type simulation_obsts
    let len;
    let data = new simulation_obsts(null);
    // Deserialize message field [circularObsts]
    // Deserialize array length for message field [circularObsts]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.circularObsts = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.circularObsts[i] = simulation_circular_obst.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 12 * object.circularObsts.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/simulation_obsts';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'addd4aa7ed3126b9ca217aeb1d69aa70';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    simulation_circular_obst[] circularObsts
    ================================================================================
    MSG: comm_msg/simulation_circular_obst
    float32 x
    float32 y
    float32 radius
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new simulation_obsts(null);
    if (msg.circularObsts !== undefined) {
      resolved.circularObsts = new Array(msg.circularObsts.length);
      for (let i = 0; i < resolved.circularObsts.length; ++i) {
        resolved.circularObsts[i] = simulation_circular_obst.Resolve(msg.circularObsts[i]);
      }
    }
    else {
      resolved.circularObsts = []
    }

    return resolved;
    }
};

module.exports = simulation_obsts;
