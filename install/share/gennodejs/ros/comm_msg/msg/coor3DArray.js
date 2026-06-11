// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let coor3D = require('./coor3D.js');

//-----------------------------------------------------------

class coor3DArray {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.coor3DArray = null;
    }
    else {
      if (initObj.hasOwnProperty('coor3DArray')) {
        this.coor3DArray = initObj.coor3DArray
      }
      else {
        this.coor3DArray = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type coor3DArray
    // Serialize message field [coor3DArray]
    // Serialize the length for message field [coor3DArray]
    bufferOffset = _serializer.uint32(obj.coor3DArray.length, buffer, bufferOffset);
    obj.coor3DArray.forEach((val) => {
      bufferOffset = coor3D.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type coor3DArray
    let len;
    let data = new coor3DArray(null);
    // Deserialize message field [coor3DArray]
    // Deserialize array length for message field [coor3DArray]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.coor3DArray = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.coor3DArray[i] = coor3D.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 12 * object.coor3DArray.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/coor3DArray';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f1d540e658b8fab024c8f2c1ea50f3fd';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    coor3D[]  coor3DArray
    ================================================================================
    MSG: comm_msg/coor3D
    float32 pos_x
    float32 pos_y
    float32 pos_z
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new coor3DArray(null);
    if (msg.coor3DArray !== undefined) {
      resolved.coor3DArray = new Array(msg.coor3DArray.length);
      for (let i = 0; i < resolved.coor3DArray.length; ++i) {
        resolved.coor3DArray[i] = coor3D.Resolve(msg.coor3DArray[i]);
      }
    }
    else {
      resolved.coor3DArray = []
    }

    return resolved;
    }
};

module.exports = coor3DArray;
