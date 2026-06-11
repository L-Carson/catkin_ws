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

class nav_algo_info {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.algoType = null;
    }
    else {
      if (initObj.hasOwnProperty('algoType')) {
        this.algoType = initObj.algoType
      }
      else {
        this.algoType = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type nav_algo_info
    // Serialize message field [algoType]
    bufferOffset = _serializer.uint8(obj.algoType, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type nav_algo_info
    let len;
    let data = new nav_algo_info(null);
    // Deserialize message field [algoType]
    data.algoType = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/nav_algo_info';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e5aec186900a8a01b0a68e06489a4999';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8 algoType
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new nav_algo_info(null);
    if (msg.algoType !== undefined) {
      resolved.algoType = msg.algoType;
    }
    else {
      resolved.algoType = 0
    }

    return resolved;
    }
};

module.exports = nav_algo_info;
