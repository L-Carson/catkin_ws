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

class dustbinState {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.isFull = null;
    }
    else {
      if (initObj.hasOwnProperty('isFull')) {
        this.isFull = initObj.isFull
      }
      else {
        this.isFull = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type dustbinState
    // Serialize message field [isFull]
    bufferOffset = _serializer.bool(obj.isFull, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type dustbinState
    let len;
    let data = new dustbinState(null);
    // Deserialize message field [isFull]
    data.isFull = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/dustbinState';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '3815cb2adedcd1d91ff6caca01a84001';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool  isFull    #站点垃圾箱是否已满
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new dustbinState(null);
    if (msg.isFull !== undefined) {
      resolved.isFull = msg.isFull;
    }
    else {
      resolved.isFull = false
    }

    return resolved;
    }
};

module.exports = dustbinState;
