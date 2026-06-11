// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let coor2D = require('./coor2D.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class coor2DArray {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.isContainForwardObst = null;
      this.isContainBackwardObst = null;
      this.coor2DArray = null;
      this.coor2DArrayTop = null;
      this.coor2DArrayMid = null;
      this.coor2DArrayDown = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('isContainForwardObst')) {
        this.isContainForwardObst = initObj.isContainForwardObst
      }
      else {
        this.isContainForwardObst = 0;
      }
      if (initObj.hasOwnProperty('isContainBackwardObst')) {
        this.isContainBackwardObst = initObj.isContainBackwardObst
      }
      else {
        this.isContainBackwardObst = 0;
      }
      if (initObj.hasOwnProperty('coor2DArray')) {
        this.coor2DArray = initObj.coor2DArray
      }
      else {
        this.coor2DArray = [];
      }
      if (initObj.hasOwnProperty('coor2DArrayTop')) {
        this.coor2DArrayTop = initObj.coor2DArrayTop
      }
      else {
        this.coor2DArrayTop = [];
      }
      if (initObj.hasOwnProperty('coor2DArrayMid')) {
        this.coor2DArrayMid = initObj.coor2DArrayMid
      }
      else {
        this.coor2DArrayMid = [];
      }
      if (initObj.hasOwnProperty('coor2DArrayDown')) {
        this.coor2DArrayDown = initObj.coor2DArrayDown
      }
      else {
        this.coor2DArrayDown = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type coor2DArray
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [isContainForwardObst]
    bufferOffset = _serializer.uint8(obj.isContainForwardObst, buffer, bufferOffset);
    // Serialize message field [isContainBackwardObst]
    bufferOffset = _serializer.uint8(obj.isContainBackwardObst, buffer, bufferOffset);
    // Serialize message field [coor2DArray]
    // Serialize the length for message field [coor2DArray]
    bufferOffset = _serializer.uint32(obj.coor2DArray.length, buffer, bufferOffset);
    obj.coor2DArray.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [coor2DArrayTop]
    // Serialize the length for message field [coor2DArrayTop]
    bufferOffset = _serializer.uint32(obj.coor2DArrayTop.length, buffer, bufferOffset);
    obj.coor2DArrayTop.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [coor2DArrayMid]
    // Serialize the length for message field [coor2DArrayMid]
    bufferOffset = _serializer.uint32(obj.coor2DArrayMid.length, buffer, bufferOffset);
    obj.coor2DArrayMid.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [coor2DArrayDown]
    // Serialize the length for message field [coor2DArrayDown]
    bufferOffset = _serializer.uint32(obj.coor2DArrayDown.length, buffer, bufferOffset);
    obj.coor2DArrayDown.forEach((val) => {
      bufferOffset = coor2D.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type coor2DArray
    let len;
    let data = new coor2DArray(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [isContainForwardObst]
    data.isContainForwardObst = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [isContainBackwardObst]
    data.isContainBackwardObst = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [coor2DArray]
    // Deserialize array length for message field [coor2DArray]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.coor2DArray = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.coor2DArray[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [coor2DArrayTop]
    // Deserialize array length for message field [coor2DArrayTop]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.coor2DArrayTop = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.coor2DArrayTop[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [coor2DArrayMid]
    // Deserialize array length for message field [coor2DArrayMid]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.coor2DArrayMid = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.coor2DArrayMid[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [coor2DArrayDown]
    // Deserialize array length for message field [coor2DArrayDown]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.coor2DArrayDown = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.coor2DArrayDown[i] = coor2D.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 9 * object.coor2DArray.length;
    length += 9 * object.coor2DArrayTop.length;
    length += 9 * object.coor2DArrayMid.length;
    length += 9 * object.coor2DArrayDown.length;
    return length + 18;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/coor2DArray';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'c15593ec81512cc55beecc6a469f2978';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header    header
    uint8     isContainForwardObst
    uint8     isContainBackwardObst
    coor2D[]  coor2DArray       #待删除
    coor2D[]  coor2DArrayTop
    coor2D[]  coor2DArrayMid
    coor2D[]  coor2DArrayDown
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
    MSG: comm_msg/coor2D
    coorLabel   label       #坐标类别
    float32     pos_x
    float32     pos_y
    ================================================================================
    MSG: comm_msg/coorLabel
    uint8   UNKNOWN      = 0  #未知
    uint8   STATIC       = 1  #静态
    uint8   DYNAMIC      = 2  #动态
    uint8   HOLLOW_OBST  = 3  #空洞障碍
    uint8   SHORT_HOLD   = 4  #低矮维持
    uint8   COMMON_HOLD  = 5  #常规维持
    uint8   BLIND_SPOT   = 6  #盲区障碍
    uint8   SLOPE_PAD    = 7  #斜坡垫
    uint8   CONTACT_EDGE = 8  #触边障碍
    uint8   TREELAWN     = 9  #绿植障碍
    uint8   ULTRASONIC   = 10 #超声
    
    uint8 value
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new coor2DArray(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.isContainForwardObst !== undefined) {
      resolved.isContainForwardObst = msg.isContainForwardObst;
    }
    else {
      resolved.isContainForwardObst = 0
    }

    if (msg.isContainBackwardObst !== undefined) {
      resolved.isContainBackwardObst = msg.isContainBackwardObst;
    }
    else {
      resolved.isContainBackwardObst = 0
    }

    if (msg.coor2DArray !== undefined) {
      resolved.coor2DArray = new Array(msg.coor2DArray.length);
      for (let i = 0; i < resolved.coor2DArray.length; ++i) {
        resolved.coor2DArray[i] = coor2D.Resolve(msg.coor2DArray[i]);
      }
    }
    else {
      resolved.coor2DArray = []
    }

    if (msg.coor2DArrayTop !== undefined) {
      resolved.coor2DArrayTop = new Array(msg.coor2DArrayTop.length);
      for (let i = 0; i < resolved.coor2DArrayTop.length; ++i) {
        resolved.coor2DArrayTop[i] = coor2D.Resolve(msg.coor2DArrayTop[i]);
      }
    }
    else {
      resolved.coor2DArrayTop = []
    }

    if (msg.coor2DArrayMid !== undefined) {
      resolved.coor2DArrayMid = new Array(msg.coor2DArrayMid.length);
      for (let i = 0; i < resolved.coor2DArrayMid.length; ++i) {
        resolved.coor2DArrayMid[i] = coor2D.Resolve(msg.coor2DArrayMid[i]);
      }
    }
    else {
      resolved.coor2DArrayMid = []
    }

    if (msg.coor2DArrayDown !== undefined) {
      resolved.coor2DArrayDown = new Array(msg.coor2DArrayDown.length);
      for (let i = 0; i < resolved.coor2DArrayDown.length; ++i) {
        resolved.coor2DArrayDown[i] = coor2D.Resolve(msg.coor2DArrayDown[i]);
      }
    }
    else {
      resolved.coor2DArrayDown = []
    }

    return resolved;
    }
};

module.exports = coor2DArray;
