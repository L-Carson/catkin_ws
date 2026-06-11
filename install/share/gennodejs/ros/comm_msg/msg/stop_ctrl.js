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

class stop_ctrl {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.isAct = null;
    }
    else {
      if (initObj.hasOwnProperty('isAct')) {
        this.isAct = initObj.isAct
      }
      else {
        this.isAct = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type stop_ctrl
    // Serialize message field [isAct]
    bufferOffset = _serializer.uint8(obj.isAct, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type stop_ctrl
    let len;
    let data = new stop_ctrl(null);
    // Deserialize message field [isAct]
    data.isAct = _deserializer.uint8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/stop_ctrl';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '6214bd4b533a199bfb2bc3ad84feb300';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8  isAct
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new stop_ctrl(null);
    if (msg.isAct !== undefined) {
      resolved.isAct = msg.isAct;
    }
    else {
      resolved.isAct = 0
    }

    return resolved;
    }
};

module.exports = stop_ctrl;
