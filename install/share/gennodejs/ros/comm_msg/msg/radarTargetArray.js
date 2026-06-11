// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let radarTarget = require('./radarTarget.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class radarTargetArray {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.radarID = null;
      this.allTargets = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('radarID')) {
        this.radarID = initObj.radarID
      }
      else {
        this.radarID = 0;
      }
      if (initObj.hasOwnProperty('allTargets')) {
        this.allTargets = initObj.allTargets
      }
      else {
        this.allTargets = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type radarTargetArray
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [radarID]
    bufferOffset = _serializer.int16(obj.radarID, buffer, bufferOffset);
    // Serialize message field [allTargets]
    // Serialize the length for message field [allTargets]
    bufferOffset = _serializer.uint32(obj.allTargets.length, buffer, bufferOffset);
    obj.allTargets.forEach((val) => {
      bufferOffset = radarTarget.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type radarTargetArray
    let len;
    let data = new radarTargetArray(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [radarID]
    data.radarID = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [allTargets]
    // Deserialize array length for message field [allTargets]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.allTargets = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.allTargets[i] = radarTarget.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 16 * object.allTargets.length;
    return length + 6;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/radarTargetArray';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '6dbbce6501fbb2b1edc228d20004f209';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header          header
    int16           radarID
    radarTarget[]   allTargets
    
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    ================================================================================
    MSG: comm_msg/radarTarget
    float32   targetDistx         # 目标相对于清扫车参考坐标系X轴坐标
    float32   targetDisty         # 目标相对于清扫车参考坐标系Y轴坐标
    float32   targetVelx          # 目标相对于清扫车参考坐标系X轴速度
    float32   targetVely          # 目标相对于清扫车参考坐标系Y轴速度
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new radarTargetArray(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.radarID !== undefined) {
      resolved.radarID = msg.radarID;
    }
    else {
      resolved.radarID = 0
    }

    if (msg.allTargets !== undefined) {
      resolved.allTargets = new Array(msg.allTargets.length);
      for (let i = 0; i < resolved.allTargets.length; ++i) {
        resolved.allTargets[i] = radarTarget.Resolve(msg.allTargets[i]);
      }
    }
    else {
      resolved.allTargets = []
    }

    return resolved;
    }
};

module.exports = radarTargetArray;
