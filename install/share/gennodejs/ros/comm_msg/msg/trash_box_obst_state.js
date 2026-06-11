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

class trash_box_obst_state {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.exisitObstUp = null;
      this.exisitObstDown = null;
    }
    else {
      if (initObj.hasOwnProperty('exisitObstUp')) {
        this.exisitObstUp = initObj.exisitObstUp
      }
      else {
        this.exisitObstUp = false;
      }
      if (initObj.hasOwnProperty('exisitObstDown')) {
        this.exisitObstDown = initObj.exisitObstDown
      }
      else {
        this.exisitObstDown = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type trash_box_obst_state
    // Serialize message field [exisitObstUp]
    bufferOffset = _serializer.bool(obj.exisitObstUp, buffer, bufferOffset);
    // Serialize message field [exisitObstDown]
    bufferOffset = _serializer.bool(obj.exisitObstDown, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type trash_box_obst_state
    let len;
    let data = new trash_box_obst_state(null);
    // Deserialize message field [exisitObstUp]
    data.exisitObstUp = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [exisitObstDown]
    data.exisitObstDown = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 2;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/trash_box_obst_state';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '051ec62ff947ba3d46f50bc3f41f117f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # trash box obst state
    bool  exisitObstUp        #  垃圾箱抬升方向是否有障碍
    bool  exisitObstDown      #  垃圾箱回收方向是否有障碍
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new trash_box_obst_state(null);
    if (msg.exisitObstUp !== undefined) {
      resolved.exisitObstUp = msg.exisitObstUp;
    }
    else {
      resolved.exisitObstUp = false
    }

    if (msg.exisitObstDown !== undefined) {
      resolved.exisitObstDown = msg.exisitObstDown;
    }
    else {
      resolved.exisitObstDown = false
    }

    return resolved;
    }
};

module.exports = trash_box_obst_state;
