// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let nav_ctrl = require('./nav_ctrl.js');

//-----------------------------------------------------------

class nav_state {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.planMoveTrend = null;
      this.navCtrl = null;
    }
    else {
      if (initObj.hasOwnProperty('planMoveTrend')) {
        this.planMoveTrend = initObj.planMoveTrend
      }
      else {
        this.planMoveTrend = 0;
      }
      if (initObj.hasOwnProperty('navCtrl')) {
        this.navCtrl = initObj.navCtrl
      }
      else {
        this.navCtrl = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type nav_state
    // Serialize message field [planMoveTrend]
    bufferOffset = _serializer.uint8(obj.planMoveTrend, buffer, bufferOffset);
    // Serialize message field [navCtrl]
    // Serialize the length for message field [navCtrl]
    bufferOffset = _serializer.uint32(obj.navCtrl.length, buffer, bufferOffset);
    obj.navCtrl.forEach((val) => {
      bufferOffset = nav_ctrl.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type nav_state
    let len;
    let data = new nav_state(null);
    // Deserialize message field [planMoveTrend]
    data.planMoveTrend = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [navCtrl]
    // Deserialize array length for message field [navCtrl]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.navCtrl = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.navCtrl[i] = nav_ctrl.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.navCtrl.length;
    return length + 5;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/nav_state';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2b0151b05a659a933eedcf8f25c3b286';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8       planMoveTrend        # 0:停止前进 1:向前运动 2:向后运动 3:自由运动
    nav_ctrl[]  navCtrl
    ================================================================================
    MSG: comm_msg/nav_ctrl
    uint8 GARBAGE_DETECT = 1    # 垃圾寻扫
    uint8 CONTACT_EDGE   = 2    # 触边生成障碍物
    
    uint8 value
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new nav_state(null);
    if (msg.planMoveTrend !== undefined) {
      resolved.planMoveTrend = msg.planMoveTrend;
    }
    else {
      resolved.planMoveTrend = 0
    }

    if (msg.navCtrl !== undefined) {
      resolved.navCtrl = new Array(msg.navCtrl.length);
      for (let i = 0; i < resolved.navCtrl.length; ++i) {
        resolved.navCtrl[i] = nav_ctrl.Resolve(msg.navCtrl[i]);
      }
    }
    else {
      resolved.navCtrl = []
    }

    return resolved;
    }
};

module.exports = nav_state;
