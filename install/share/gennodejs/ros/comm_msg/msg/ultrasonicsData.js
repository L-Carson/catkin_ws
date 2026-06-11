// Auto-generated. Do not edit!

// (in-package comm_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let ultrasonicData = require('./ultrasonicData.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class ultrasonicsData {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.usData = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('usData')) {
        this.usData = initObj.usData
      }
      else {
        this.usData = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type ultrasonicsData
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [usData]
    // Serialize the length for message field [usData]
    bufferOffset = _serializer.uint32(obj.usData.length, buffer, bufferOffset);
    obj.usData.forEach((val) => {
      bufferOffset = ultrasonicData.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type ultrasonicsData
    let len;
    let data = new ultrasonicsData(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [usData]
    // Deserialize array length for message field [usData]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.usData = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.usData[i] = ultrasonicData.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    object.usData.forEach((val) => {
      length += ultrasonicData.getMessageSize(val);
    });
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'comm_msg/ultrasonicsData';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '107cce378ac5b6b859f4402e5340a53f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header           header
    ultrasonicData[] usData
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
    MSG: comm_msg/ultrasonicData
    string   usName
    coor2D[] data
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
    const resolved = new ultrasonicsData(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.usData !== undefined) {
      resolved.usData = new Array(msg.usData.length);
      for (let i = 0; i < resolved.usData.length; ++i) {
        resolved.usData[i] = ultrasonicData.Resolve(msg.usData[i]);
      }
    }
    else {
      resolved.usData = []
    }

    return resolved;
    }
};

module.exports = ultrasonicsData;
