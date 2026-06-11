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

class trash_box_area_obst_state {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.isExistHighObstInBoxArea = null;
      this.isExistHighObstOutBoxArea = null;
    }
    else {
      if (initObj.hasOwnProperty('isExistHighObstInBoxArea')) {
        this.isExistHighObstInBoxArea = initObj.isExistHighObstInBoxArea
      }
      else {
        this.isExistHighObstInBoxArea = false;
      }
      if (initObj.hasOwnProperty('isExistHighObstOutBoxArea')) {
        this.isExistHighObstOutBoxArea = initObj.isExistHighObstOutBoxArea
      }
      else {
        this.isExistHighObstOutBoxArea = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type trash_box_area_obst_state
    // Serialize message field [isExistHighObstInBoxArea]
    bufferOffset = _serializer.bool(obj.isExistHighObstInBoxArea, buffer, bufferOffset);
    // Serialize message field [isExistHighObstOutBoxArea]
    bufferOffset = _serializer.bool(obj.isExistHighObstOutBoxArea, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type trash_box_area_obst_state
    let len;
    let data = new trash_box_area_obst_state(null);
    // Deserialize message field [isExistHighObstInBoxArea]
    data.isExistHighObstInBoxArea = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [isExistHighObstOutBoxArea]
    data.isExistHighObstOutBoxArea = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 2;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/trash_box_area_obst_state';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4ae1ec08cec9667102da97364468a9db';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # trash box area obst state
    
    bool  isExistHighObstInBoxArea      #  车辆后方在垃圾箱翻起内的区域是否存在高于垃圾箱的障碍物
    bool  isExistHighObstOutBoxArea     #  车辆后方在垃圾箱翻起外的区域是否存在高于垃圾箱的障碍物
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new trash_box_area_obst_state(null);
    if (msg.isExistHighObstInBoxArea !== undefined) {
      resolved.isExistHighObstInBoxArea = msg.isExistHighObstInBoxArea;
    }
    else {
      resolved.isExistHighObstInBoxArea = false
    }

    if (msg.isExistHighObstOutBoxArea !== undefined) {
      resolved.isExistHighObstOutBoxArea = msg.isExistHighObstOutBoxArea;
    }
    else {
      resolved.isExistHighObstOutBoxArea = false
    }

    return resolved;
    }
};

module.exports = trash_box_area_obst_state;
